#include "tmotion.h"
#include "motor.h"
#include "texecute.h"
#include "includes.h"

#define WAFER_THICK 100
#define WTHICK_MARG 10

#define WPOS_MARGIN   10
#define WPOS_START    20000
#define WPOS_INTERVAL 10000

#define WAFER_NUM (25<<1)

#define INSLOT_HH  4
#define INSLOT_HI  3
#define INSLOT_LI  2
#define INSLOT_LL  1
#define INSLOT_ERR 0
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

void Analyze_Scan(s32* result)
{
	s32 wafer_pos[WAFER_NUM];
	u8 cnt = 0;
	u8 slot = 0;
	s32 pos1, pos2 = 0;
	u8 i;
	for(i = 0; i < WAFER_NUM; )
	{
		wafer_pos[i] = WPOS_START + ((WAFER_NUM - i - 2) >> 1) * WPOS_INTERVAL + WAFER_THICK;
		wafer_pos[i] = WPOS_START + ((WAFER_NUM - i - 2) >> 1) * WPOS_INTERVAL;
		i += 2;
	}
	while(cnt <= gScan_num)
	{
		pos1 = (gScan_pos[cnt++] - WPOS_START) % WPOS_INTERVAL;
		pos2 = (gScan_pos[cnt] - WPOS_START) % WPOS_INTERVAL;
		slot = (gScan_pos[cnt++] - WPOS_START) / WPOS_INTERVAL;
	}
}

