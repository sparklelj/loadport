#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define M_ENABLE 4 //PE
#define M_DIR    5
#define M_PULSE  6

#define M_POS_A  12 //PF
#define M_POS_B  13
#define M_POS_Z  14
#define M_POS_P  15

#define MS_UNINIT    0x00
#define MS_STRTINIT  0x01
#define MS_INITING   0x02
#define MS_PRKEND    0x03
#define MS_INITED    0x04

#define VEL_MAX 	 0xFF
#define VEL_MIN 	 0xFFF0
#define VEL_ACCB   3
#define VEL_ACC 	 0x08

extern int32_t gpos_num;
extern s32 gMotion_num;
extern u32 gPulse_num;
extern u16 gCur_vel;
extern u16 gMax_vel;
extern s8  gDir_vel;

extern s32 gPos_num;
extern s32 gPark_num;
extern u8 gMotor_state;

void POS_SET(s32 target);
#endif
