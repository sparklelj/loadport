#ifndef __TMOTION_H
#define __TMOTION_H

#include "sys.h"

#define M_UPLMT 1000000
#define M_STRMP 900000
#define M_STPMP 600000
#define M_DNLMT 200000

#define M_VEL VEL_MAX

void STOP_Minit(void);
void START_Motion(s32 target_pos, u16 target_vel);
void STOP_Motion(void);
u8 Get_MStatus(void);
extern u8 gMotion_status;
#endif
