#include "tmotion.h"
#include "motor.h"
#include "includes.h"


#define DIR_POS 1
#define DIE_NEG 0

#define IS_STOP ((gMotion_cmd == gMotion_num) && (gCur_vel == 0))

#define BACK_POS -800000
#define BACK_VEL 0xF000

#define FORWD_POS 100000
#define FORWD_VEL 0xF000

s32 gMotion_cmd = 0;
u16 gVel_cmd = 0;

u8 gMotion_status;



bool is_mstop(void)
{
    if((gMotion_cmd == gMotion_num) && (is_stop == true))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_m_err(void)
{
	if(PFin(M_ERR) == 0x01)
	{
		return true;
	}
	return false;
}

bool enable_m(bool ctrl)
{
	PEout(M_ENABLE) = ctrl;
	return true;
}


void tMotor_Motion(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    while(1)
    {
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        OS_CRITICAL_ENTER();
        if(gMotion_cmd == gMotion_num)
        {
            continue;
        }
        if(!is_stop)
        {
            if(gMotion_cmd >= ((VEL_MIN - gCur_vel) >> VEL_ACCB) * gDir_vel + gMotion_num)
            {
                if(gDir_vel == DIR_POS)
                {
                    gPulse_num = gMotion_cmd - gMotion_num;
                }
                else
                {
                    gPulse_num = (VEL_MIN - gCur_vel) >> VEL_ACCB;
                }
            }
            else
            {
                if(gDir_vel == DIE_NEG)
                {
                    gPulse_num = gMotion_num - gMotion_cmd ;
                }
                else
                {
                    gPulse_num = (VEL_MIN - gCur_vel) >> VEL_ACCB;
                }
            }
        }
        else
        {
            if(gMotion_cmd - gMotion_num >= 0)
            {               
                if(gDir_vel == DIE_NEG)
                {
                    OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
                    PEout(M_DIR) = 1;
                    gDir_vel = DIR_POS;
                    OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
                }
								gPulse_num = gMotion_cmd - gMotion_num;
            }
            else
            {                
                if(gDir_vel == DIR_POS)
                {
                    OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
                    PEout(M_DIR) = 0;
                    gDir_vel = DIE_NEG;
                    OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
                }
								gPulse_num = gMotion_num - gMotion_cmd;
            }
        }
        
        if(is_mstop())
        {
            switch (gMotor_state)
            {
            case MS_GOMAPSTR:
                gMotor_state = MS_MAPSTR;
                break;
            case MS_SCANNING:
                gMotor_state = MS_SCANED;
                break;
            case MS_GOEND:
                gMotor_state = MS_END;
                break;
            case MS_BAKINIT:
                gMotor_state = MS_INITED;
                break;
            case MS_GOINIT:
                gMotor_state = MS_INITED;
                break;
            }
        }
				OS_CRITICAL_EXIT();
//       OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

void STOP_Minit(void)
{
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
    gMotor_state = MS_UNINIT;
    STOP_Motion();
	OS_CRITICAL_EXIT();
}

void STOP_Motion(void)
{
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
//	if((gMotor_state < MS_INITED) && (gMotor_state > MS_UNINIT))
//	{
//		gMotor_state = MS_UNINIT;
//	}
//    if((gCur_vel != 0) || (gCur_vel < VEL_MIN))
 //   {
        gMotion_cmd = gMotion_num + ((VEL_MIN - gCur_vel) >> VEL_ACCB) * gDir_vel;
//    }
	OS_CRITICAL_EXIT();
}

void PAUSE_Motion(s32 target)
{
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	switch(target)
	{
		case M_STRMP:
			if(gMotion_num + ((VEL_MIN - gCur_vel) >> VEL_ACCB) * gDir_vel < M_STRMP)
			{
				gMotion_cmd = M_STRMP;
			}
			else
			{
				STOP_Motion();
			}
			break;
		case M_STPMP:
			if(gMotion_num + ((VEL_MIN - gCur_vel) >> VEL_ACCB) * gDir_vel < M_STPMP)
			{
				gMotion_cmd = M_STPMP;
			}
			else
			{
				STOP_Motion();
			}
			break;
		case M_DNLMT:
			if(gMotion_num + ((VEL_MIN - gCur_vel) >> VEL_ACCB) * gDir_vel < M_DNLMT)
			{
				gMotion_cmd = M_DNLMT;
			}
			else
			{
				STOP_Motion();
			}
			break;
		case M_UPLMT:
			if(gMotion_num + ((VEL_MIN - gCur_vel) >> VEL_ACCB) * gDir_vel > M_UPLMT)
			{
				gMotion_cmd = M_UPLMT;
			}
			else
			{
				STOP_Motion();
			}
			break;
	}
	OS_CRITICAL_EXIT();
}
void START_Motion(s32 target_pos, u16 target_vel)
{
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
    gMotion_cmd = target_pos;
    gVel_cmd = target_vel;
	OS_CRITICAL_EXIT();
}

void MOTOR_GO(s32 target_pos, u16 target_vel)
{
    START_Motion(target_pos, target_vel);
    switch (target_pos)
    {
    case M_UPLMT:
        gMotor_state = MS_BAKINIT;
        break;
    case M_STRMP:
        gMotor_state = MS_GOMAPSTR;
        break;
    case M_STPMP:
        gMotor_state = MS_SCANNING;
        break;
    case M_DNLMT:
        gMotor_state = MS_GOEND;
        break;
    }
}

void tMotor_Init(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
BEGIN:
    OSTaskSuspend(NULL, &err);
    gMotor_state = MS_INITING;
    if(!is_mstop())
    {
        STOP_Motion();
    }

    while(!is_mstop())
    {
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        if(gMotor_state == MS_UNINIT)
        {
            goto BEGIN;
        }
    }

    if(PFin(M_POS_P) == 0)
    {
        START_Motion(FORWD_POS, FORWD_VEL);
    }
		
		while(PFin(M_POS_P) == 0)
    {
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        if(gMotor_state == MS_UNINIT)
        {
            goto BEGIN;
        }
    }
		STOP_Motion();
    while(!is_mstop())
    {
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        if(gMotor_state == MS_UNINIT)
        {
            goto BEGIN;
        }
    }

    while(gMotor_state != MS_PRKEND)
    {
        START_Motion(BACK_POS, BACK_VEL);
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        if(gMotor_state == MS_UNINIT)
        {
            goto BEGIN;
        }
    }

    STOP_Motion();
    while(!is_mstop())
    {
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        if(gMotor_state == MS_UNINIT)
        {
            goto BEGIN;
        }
    }

    OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
    if(gMotor_state == MS_UNINIT)
    {
        goto BEGIN;
    }

    OS_CRITICAL_ENTER();
//	gPos_num = gPos_num - gPark_num;
    POS_SET(gPark_num);
    gMotion_num = gPos_num ;
    gMotion_cmd = gMotion_num;
    OS_CRITICAL_EXIT();

    START_Motion(MS_GOINIT, FORWD_VEL);

    while(!is_mstop())
    {
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        if(gMotor_state == MS_UNINIT)
        {
            goto BEGIN;
        }
    }

    gMotor_state = MS_INITED;
    OSTaskDel(NULL,&err);
    goto BEGIN;
}

u8 Get_MStatus(void)
{
    gMotion_status = 0x00;
    if(gMotor_state == MS_BAKINIT)
    {
        if(is_mstop())
        {
            gMotion_status = 0x01;
        }
        else
        {
            gMotion_status = 0x08;
        }
    }
    if(gMotor_state == MS_GOMAPSTR)
    {
        if(is_mstop())
        {
            gMotion_status = 0x03;
        }
        else
        {
            gMotion_status = 0x02;
        }
    }
    if(gMotor_state == MS_SCANNING)
    {
        if(is_mstop())
        {
            gMotion_status = 0x05;
        }
        else
        {
            gMotion_status = 0x04;
        }
    }
    if(gMotor_state == MS_GOEND)
    {
        if(is_mstop())
        {
            gMotion_status = 0x07;
        }
        else
        {
            gMotion_status = 0x06;
        }
    }
    return gMotion_status;
}
