#include "tmotion.h"
#include "motor.h"
#include "includes.h"


#define DIR_POS 1
#define DIE_NEG 0

#define IS_STOP ((gMotion_cmd == gMotion_num) && (gCur_vel == 0))

#define BACK_POS -800000
#define BACK_VEL 0xF000

#define FORWD_POS 10000
#define FORWD_VEL 0xF000

s32 gMotion_cmd = 0;
u16 gVel_cmd = 0;



void tMotor_Motion(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	while(1)
	{
		OS_CRITICAL_ENTER();
		if(gCur_vel !=0)
		{
			if(gMotion_cmd >= ((gCur_vel - VEL_MIN) >> VEL_ACCB) * gDir_vel + 1 + gMotion_num)
			{
				if(gDir_vel == DIR_POS)
				{
					gPulse_num = gMotion_cmd - gMotion_num;
				}
				else
				{
					gPulse_num = ((gCur_vel - VEL_MIN) >> VEL_ACCB) + 1;
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
					gPulse_num = ((gCur_vel - VEL_MIN) >> VEL_ACCB) + 1;
				}
			}
		}
		else
		{
			if(gMotion_cmd - gMotion_num >= 0)
			{
				gPulse_num = gMotion_cmd - gMotion_num;
				if(gDir_vel == DIE_NEG)
				{					
					OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);
					PEout(M_DIR) = 1;
					gDir_vel = DIR_POS;
					OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);
				}
			}
			else
			{
				gPulse_num = gMotion_num - gMotion_cmd;
				if(gDir_vel == DIR_POS)
				{				
					OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);
					PEout(M_DIR) = 0;
					gDir_vel = DIE_NEG;
					OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);
				}
			}
		}
		OS_CRITICAL_EXIT();
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void STOP_Motion(void)
{
	if((gMotor_state < MS_INITED) && (gMotor_state > MS_UNINIT))
	{
		gMotor_state = MS_UNINIT;
		
	}
	if((gCur_vel != 0) || (gCur_vel < VEL_MIN))
	{
		gMotion_cmd = (gMotion_num + (((gCur_vel - VEL_MIN) >> VEL_ACCB) + 2) * gDir_vel) & 0xFFFFFFFE ;
	}
}

void START_Motion(s32 target_pos, u16 target_vel)
{
	gMotion_cmd = target_pos << 1;
	gVel_cmd = target_vel;
}



void tMotor_Init(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	BEGIN:
	OSTaskSuspend(NULL, &err);
	gMotor_state = MS_STRTINIT;
	if(!IS_STOP)
	{
		STOP_Motion();
	}
	
	while(!IS_STOP)
	{
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		if(gMotor_state == MS_UNINIT)
		{
			goto BEGIN;
		}
	}
	
	if(PEin(M_POS_P) == 0)
	{
		START_Motion(FORWD_POS, FORWD_VEL);
	}

	while(!IS_STOP)
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
	while(!IS_STOP)
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
	gMotion_num = gPos_num << 1;
	gMotion_cmd = gMotion_num;
	OS_CRITICAL_EXIT();
	
	START_Motion(0, FORWD_VEL);
	
	while(!IS_STOP)
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
