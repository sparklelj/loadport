#include "tcmd.h"
#include "includes.h"
#include "uart.h"

void t(void *p_arg)
{
	u8 head[105];
	u8 len;
	u16 lencmd;
	OS_ERR err;
	CPU_SR_ALLOC();
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,900,OS_OPT_TIME_HMSM_STRICT,&err);
		len = ONLINE_Peer(head, 1);
		if(len == 0)
		{
			continue;
		}
		if(head[0] != 0x01)
		{
			ONLINE_Read(head, 1);
			continue;
		}
		len = ONLINE_RxLen();
		if(len < 8)
		{
			continue;
		}
		ONLINE_Peer(head, 3);
		lencmd = (head[1] << 8) + head[2];
		if(lencmd < 4)
		{
			ONLINE_Read(head, 1);
			continue;
		}
		lencmd += 4;
		len = ONLINE_RxLen();
		if(lencmd > len)
		{
			continue;
		}
		ONLINE_Peer(head, lencmd);
	}
}
