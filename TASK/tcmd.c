#include "tcmd.h"
#include "includes.h"
#include "uart.h"
#include "stdio.h"

u8 gAddr[2] = {'0','0'};

bool check_sum(u8* msg)
{
	u8 i;
	u16 len;
	u8 sum = 0;
	u8 strsum[5];
	len = *(msg + 1);
	len = (len << 8) + *(msg + 2);
	for(i=1;i<=len;i++)
	{
		sum += *(msg + i);
	}
	sprintf((char*)strsum, "%X", sum);
	if(strsum[0] == *(msg+len+1) && strsum[0] == *(msg+len+2))
	{
		return true;
	}
	return false;
}

bool proc_cmd(u8* msg)
{
	u8 addr[2];
	u8 cmd_t[3];
	u8 cmd_n[5];
	memcpy(addr, msg+3, 2);
	if(memcmp(addr, gAddr, 2) != 0)
	{
		return false;
	}
	memcpy(cmd_t, msg+5, 3);
	memcpy(cmd_n, msg+9, 5);
	
	if(memcmp(cmd_t, "MOV", 3) == 0)
	{
		
	}
	
}

void tCMD_Proc(void *p_arg)
{
	u8 msg[105];
	u8 len;
	u16 lencmd;
	OS_ERR err;
	CPU_SR_ALLOC();
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,900,OS_OPT_TIME_HMSM_STRICT,&err);
		len = ONLINE_Peer(msg, 1);
		if(len == 0)
		{
			continue;
		}
		if(msg[0] != 0x01)
		{
			ONLINE_Read(msg, 1);
			continue;
		}
		len = ONLINE_RxLen();
		if(len < 8)
		{
			continue;
		}
		ONLINE_Peer(msg, 3);
		lencmd = (msg[1] << 8) + msg[2];
		if(lencmd < 4)
		{
			ONLINE_Read(msg, 1);
			continue;
		}
		lencmd += 4;
		len = ONLINE_RxLen();
		if(lencmd > len)
		{
			continue;
		}
		len = ONLINE_Peer(msg, lencmd);
		if(len < lencmd)
		{
			continue;
		}
		if(check_sum(msg))
		{
			len = ONLINE_Read(msg, lencmd);
			proc_cmd(msg);
		}
		else
		{
			ONLINE_Read(msg, 1);
			continue;
		}
	}
}
