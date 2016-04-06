#include "includes.h"
#include "output.h"
#include "tcmd.h"


void tLED_Control(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		if((gLED_status[0] & 0xF0) == 0x01)
		{
			if((gLED_status[0] & 0x03) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED01OUT_3);
			}
			if((gLED_status[0] & 0x03) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED01OUT_3);
			}
			if((gLED_status[0] & 0x03) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED01OUT_3);
			}
		}
		else
		{
			if((gLED_status[0] & 0x0C) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED01OUT_3);
			}
			if((gLED_status[0] & 0x0C) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED01OUT_3);
			}
			if((gLED_status[0] & 0x0C) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED01OUT_3);
			}
		}
		if((gLED_status[1] & 0xF0) == 0x01)
		{
			if((gLED_status[1] & 0x03) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED02OUT_3);
			}
			if((gLED_status[1] & 0x03) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED02OUT_3);
			}
			if((gLED_status[1] & 0x03) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED02OUT_3);
			}
		}
		else
		{
			if((gLED_status[1] & 0x0C) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED02OUT_3);
			}
			if((gLED_status[1] & 0x0C) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED02OUT_3);
			}
			if((gLED_status[1] & 0x0C) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED02OUT_3);
			}
		}
		if((gLED_status[2] & 0xF0) == 0x01)
		{
			if((gLED_status[2] & 0x03) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED03OUT_3);
			}
			if((gLED_status[2] & 0x03) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED03OUT_3);
			}
			if((gLED_status[2] & 0x03) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED03OUT_3);
			}
		}
		else
		{
			if((gLED_status[2] & 0x0C) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED03OUT_3);
			}
			if((gLED_status[2] & 0x0C) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED03OUT_3);
			}
			if((gLED_status[2] & 0x0C) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED03OUT_3);
			}
		}
		if((gLED_status[3] & 0xF0) == 0x01)
		{
			if((gLED_status[3] & 0x03) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED04OUT_3);
			}
			if((gLED_status[3] & 0x03) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED04OUT_3);
			}
			if((gLED_status[3] & 0x03) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED04OUT_3);
			}
		}
		else
		{
			if((gLED_status[3] & 0x0C) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED04OUT_3);
			}
			if((gLED_status[3] & 0x0C) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED04OUT_3);
			}
			if((gLED_status[3] & 0x0C) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED04OUT_3);
			}
		}
		if((gLED_status[4] & 0xF0) == 0x01)
		{
			if((gLED_status[4] & 0x03) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED05OUT_3);
			}
			if((gLED_status[4] & 0x03) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED05OUT_3);
			}
			if((gLED_status[4] & 0x03) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED05OUT_3);
			}
		}
		else
		{
			if((gLED_status[4] & 0x0C) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED05OUT_3);
			}
			if((gLED_status[4] & 0x0C) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED05OUT_3);
			}
			if((gLED_status[4] & 0x0C) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED05OUT_3);
			}
		}
		if((gLED_status[5] & 0xF0) == 0x01)
		{
			if((gLED_status[5] & 0x03) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED06OUT_3);
			}
			if((gLED_status[5] & 0x03) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED06OUT_3);
			}
			if((gLED_status[5] & 0x03) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED06OUT_3);
			}
		}
		else
		{
			if((gLED_status[5] & 0x0C) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED06OUT_3);
			}
			if((gLED_status[5] & 0x0C) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED06OUT_3);
			}
			if((gLED_status[5] & 0x0C) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED06OUT_3);
			}
		}
		if((gLED_status[6] & 0xF0) == 0x01)
		{
			if((gLED_status[6] & 0x03) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED07OUT_3);
			}
			if((gLED_status[6] & 0x03) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED07OUT_3);
			}
			if((gLED_status[6] & 0x03) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED07OUT_3);
			}
		}
		else
		{
			if((gLED_status[6] & 0x0C) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED07OUT_3);
			}
			if((gLED_status[6] & 0x0C) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED07OUT_3);
			}
			if((gLED_status[6] & 0x0C) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED07OUT_3);
			}
		}
		if((gLED_status[7] & 0xF0) == 0x01)
		{
			if((gLED_status[7] & 0x03) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED08OUT_3);
			}
			if((gLED_status[7] & 0x03) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED08OUT_3);
			}
			if((gLED_status[7] & 0x03) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED08OUT_3);
			}
		}
		else
		{
			if((gLED_status[7] & 0x0C) == 0x01)
			{
				OUTPUT_SetOne(CS_O_3, LED08OUT_3);
			}
			if((gLED_status[7] & 0x0C) == 0x02)
			{
				OUTPUT_TogOne(CS_O_3, LED08OUT_3);
			}
			if((gLED_status[7] & 0x0C) == 0x03)
			{
				OUTPUT_ResetOne(CS_O_3, LED08OUT_3);
			}
		}
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
