#include "tmotion.h"
#include "motor.h"
#include "texecute.h"
#include "includes.h"

#define WAFER_THICK 100
#define WTHICK_MARG 10

#define WPOS_MARGIN   10
#define WPOS_START    20000
#define WPOS_INTERVAL 10000

#define WAFER_NUM 25

#define INSLOT_HH  4
#define INSLOT_HI  3
#define INSLOT_LI  2
#define INSLOT_LL  1
#define INSLOT_ERR 99

u8 scan_mode = SCAN_UPP;



u8 is_inslot(s32 cpos, u8 slot)
{
	if(cpos > (WPOS_START + WPOS_INTERVAL * (slot - 1) + WAFER_THICK + WTHICK_MARG))
	{
		return INSLOT_HH;
	}
	
	if((cpos > (WPOS_START + WPOS_INTERVAL * (slot - 1) + WAFER_THICK - WTHICK_MARG)) && (cpos < (WPOS_START + WPOS_INTERVAL * (slot - 1) + WAFER_THICK + WTHICK_MARG)))
	{
		return INSLOT_HI;
	}
	if((cpos > (WPOS_START + WPOS_INTERVAL * (slot - 1) - WPOS_MARGIN)) && (cpos < (WPOS_START + WPOS_INTERVAL * (slot - 1) + WPOS_MARGIN)))
	{
		return INSLOT_LI;
	}
	if(cpos < (WPOS_START + WPOS_INTERVAL * (slot - 1) - WPOS_MARGIN))
	{
		return INSLOT_LL;
	}
	return INSLOT_ERR;
}

u8 is_inthick(s32 height)
{
	if(height > WAFER_THICK + WTHICK_MARG)
	{
		return INSLOT_HH;
	}
	if(height < WAFER_THICK - WTHICK_MARG)
	{
		return INSLOT_LL;
	}
	return INSLOT_HI;
}
void Analyze_Scan(u8* result)
{
	u8 cntp = 0;
	u8 cnts = 0;
//	u8 p1,p2,p3,p4,p5;
	u8 p1,p2,p4,p5;
	memset(result, 0, WAFER_NUM);
	while(cntp <= gScan_num)
	{
		p1 = is_inslot(gScan_pos[cntp], WAFER_NUM - cnts);
		p2 = is_inslot(gScan_pos[cntp+1], WAFER_NUM - cnts);
		p5 = is_inthick(gScan_pos[cntp] - gScan_pos[cntp+1]);
		if(cnts < WAFER_NUM)
		{
//			p3 = is_inslot(gScan_pos[cntp+2], WAFER_NUM - cnts - 1);
			p4 = is_inslot(gScan_pos[cntp+3], WAFER_NUM - cnts - 1);
		}
		if((p1 >= INSLOT_HI) && (p2 == INSLOT_LI))
		{
			if(p5 == INSLOT_HI)
			{
				if(*(result+cnts) == 0)
				{
					*(result+cnts) = 1;
				}
				else
				{
					*(result+cnts) = 5;
				}
				cntp += 2;
				cnts += 1;
				continue;
			}
			if(p5 == INSLOT_HH)
			{
				if(*(result+cnts) == 0)
				{
					*(result+cnts) = 2;
				}
				else
				{
					*(result+cnts) = 5;
				}
				cntp += 2;
				cnts += 1;
			}
			if(p5 == INSLOT_LL)
			{
				*(result+cnts) = 5;
				cntp += 2;
				cnts += 1;
			}
		}
		if((p1 == INSLOT_LL) && (p2 == INSLOT_LL))
		{
			if(cnts < WAFER_NUM - 1)
			{
//				p3 = is_inslot(gScan_pos[cntp+2], WAFER_NUM - cnts - 1);
				p4 = is_inslot(gScan_pos[cntp+3], WAFER_NUM - cnts - 1);
				if(p4 <= INSLOT_LI)
				{
					*(result+cnts) = 0;
					cntp += 2;
					cnts += 1;
				}
				else
				{
					*(result+cnts) = 3;
					*(result+cnts+1) = 3;
					cntp += 2;
					cnts += 1;
				}
			}
			else
			{
				*(result+cnts) = 3;
				cntp += 2;
				cnts += 1;
			}
		}

	}
	if(cnts < WAFER_NUM - 1)
	{
		memset(result+cnts, 0, WAFER_NUM - cnts - 1);
	}
}

