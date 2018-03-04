#ifndef __TMOTION_H
#define __TMOTION_H

#include "sys.h"
#include "includes.h"

//1000 pules per circle 
//2000 counts per circle

#define M_UPLMT -80        //初始位置
#define M_STRMP 6000  //开始扫盘位置
#define M_STPMP 31700  //扫盘结束
#define M_DNLMT 38950  //下限位置

#define MW_INIT 1
#define MW_UPL  2
#define MW_UPM  3
#define MW_DNM  4
#define MW_DNL  5

#define M_VEL VEL_MAX

#define DIS_M 0
#define ENA_M 1

u8 Get_MStatus(void);
extern u8 gMotion_status;
extern s32 gMotion_cmd;
extern bool gParkErr;
extern bool gismotinit;
extern bool gstopmotin;
extern bool gismoting;

extern OS_TCB MINIT_TaskTCB;

void STOP_Minit(void);
void PAUSE_Motion(s32 target);
void START_Motion(s32 target_pos, u16 target_vel);
void MOTOR_GO(u8 workpos, u16 tarvel);
void STOP_Motion(void);
void tMotor_Motion(void *p_arg);
void tMotor_Init(void *p_arg);
bool is_mstop(void);
bool is_m_err(void);
bool enable_m(bool ctrl);

#endif
