#include "tinput.h"
#include "input.h"
#include "includes.h"



u8 gStatus_scan[INPUT_NUM];

u8 status_ready[STATUS_NUM][INPUT_NUM<<1] = {{},\
{},\
{}};

void tInput_Scan(void *p_arg)
{
	u8 cnt = 0;
	OS_ERR err;
	while(cnt < INPUT_NUM)
	{
		gStatus_scan[cnt] = INPUT_Read(CS_I_9 + cnt);
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

u8 Get_Status(void)
{
	
}	
