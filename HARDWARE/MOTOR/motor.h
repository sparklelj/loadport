#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define CONN(x,y) conn(x,y)
#define conn(x,y) (x##y)

#define M_ENABLE 4 //PE
#define M_DIR    5
#define M_PULSE  6
#define M_PULA   5
#define M_PULB   6

#define M_SCAN   11 //PF
#define M_POS_A  12 //PF
#define M_POS_B  13
#define M_ERR    14
#define M_POS_P  15

#define MS_UNINIT    0x00
#define MS_STRTINIT  0x01
#define MS_INITING   0x02
#define MS_PRKEND    0x03
#define MS_INITED    0x04
#define MS_GOINIT    0x05
#define MS_GOMAPSTR  0x06
#define MS_MAPSTR    0x07
#define MS_SCANNING  0x08
#define MS_SCANED    0x09
#define MS_GOEND     0x0A
#define MS_END       0x0B
#define MS_BAKINIT   0x0C

//spark debug
#define VEL_MAX 	 0x80
#define VEL_MIN 	 0x4080
#define VEL_ACCB   0x08
//debug

//#define VEL_MAX 	 0x80
//#define VEL_MIN 	 0x4080
//#define VEL_ACCB   0x08

//#define VEL_ACC 	 0x0400
#define VEL_ACC 	 (0x0001 << VEL_ACCB) 
#define STOP_DEPOS  3

extern int32_t gpos_num;
extern s32 gMotion_num;
extern u32 gPulse_num;
extern u16 gCur_vel;
extern u16 gMax_vel;
extern s8  gDir_vel;

extern s32 gPos_num;
extern s32 gPark_num;
extern u8 gMotor_state;

extern u32 gPulse_num;
extern s32 gTarPos;
extern u16 gTarVel;
extern s32 gStopPos;
extern u16 gCurVel;
extern s32 gCurPos;
extern s8 gCurDir;
extern s32 gCurDis;
extern s32 gParkPos;
extern bool gisMotorPark;

extern s32 gtestcnt;
extern s32 gtestpul;



#define SCAN_NUM_MAX 100
extern s32 gScan_pos[SCAN_NUM_MAX];
extern u8 gScan_num;

s32 COUNT_Get(void);
bool is_stop(void);
void POS_SET(s32 target, s32 pos);
void MOTOR_Init(void);

#endif
