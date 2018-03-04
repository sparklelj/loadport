#include "tstatus.h"
#include "tmotion.h"
#include "tinput.h"
#include "motor.h"
#include "input.h"
#include "includes.h"
#include "tled.h"
#include "tcmd.h"
#include "texecute.h"

bool foupStateCur = false;
bool foupStatePre = false;
bool isKeyOn = false;
bool isKeyOf = false;
void motor_error(void)
{
    enable_m(DIS_M);
    gTarPos = gCurPos;
    gTarVel = VEL_MIN;
}

bool is_onload(void)
{
    if(INPUT_ReadOne(CS_I_15, LOAD_15) == 1)
    {
        return true;
    }
    return false;
}

bool is_unload(void)
{
    if(INPUT_ReadOne(CS_I_15, ULAD_15) == 1)
    {
        return true;
    }
    return false;
}

bool is_normal(void)
{
    return (is_foup_place() && is_foup_presence() && (!is_obstacle()));
}

bool is_nothing(void)
{
    return (is_no_foup() && (!is_obstacle()));
}

void tStatus_Check(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    while(1)
    {
        OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
        if(is_m_err())
        {
//				motor_error(); //µç»ú´íÎó
        }
        if(is_foup_presence() == true)
        {
            set_led(LED_PRESENT,LED_ON);
        }
        else
        {
            set_led(LED_PRESENT,LED_OFF);
        }
        if(is_foup_place() == true)
        {
            set_led(LED_PLACEMENT,LED_ON);
        }
        else
        {
            set_led(LED_PLACEMENT,LED_OFF);
        }

        if(is_normal())
        {
            foupStateCur = true;
            if(foupStateCur != foupStatePre)
            {
							gMap_status = 0x00;
                send_foupon();
            }
        }
        if(is_nothing())
        {
            foupStateCur = false;
            if(foupStateCur != foupStatePre)
            {
							gMap_status = 0x00;
                send_foupof();
            }
        }

        foupStatePre = foupStateCur;

        if(is_onload())
        {
            if(isKeyOn == false)
            {
							gMap_status = 0x00;
                send_fouponb();
                isKeyOn = true;
            }
        }
        else
        {
            isKeyOn = false;
        }

        if(is_unload())
        {
            if(isKeyOf == false)
            {
							gMap_status = 0x00;
                send_foupofb();
                isKeyOf = true;
            }
        }
        else
        {
            isKeyOf = false;
        }

        if(is_mstop())
        {
            switch (gMotor_state)
            {
            case MS_GOMAPSTR:
                if(is_mapstart())
                {
                    gMotor_state = MS_MAPSTR;
                }
                break;
            case MS_SCANNING:
                if(is_mapstart())
                {
                    gMotor_state = MS_SCANED;
                }
                break;
            case MS_GOEND:
                if(is_mapend())
                {
                    gMotor_state = MS_END;
                }
                break;
            case MS_BAKINIT:
                if(is_drdwlmt())
                {
                    gMotor_state = MS_INITED;
                }
                break;
            case MS_GOINIT:
                if(is_druplmt())
                {
                    gMotor_state = MS_INITED;
                }
                break;
            }
        }
        OS_CRITICAL_ENTER();
        OS_CRITICAL_EXIT();
    }
}
