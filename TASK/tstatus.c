#include "tstatus.h"
#include "tmotion.h"
#include "tinput.h"
#include "motor.h"
#include "includes.h"

void motor_error()
{
	enable_m(DIS_M);
	gTarPos = gCurPos;
	gTarVel = VEL_MIN;
}
void tStatus_Check(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    while(1)
    {
			OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
			if(is_m_err())
			{
//				motor_error(); //µç»ú´íÎó
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