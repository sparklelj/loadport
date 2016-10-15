#ifndef __TMOTION_H
#define __TMOTION_H

#include "sys.h"

#define M_UPLMT 10000        //初始位置
#define M_STRMP 70000  //开始扫盘位置
#define M_STPMP 30000  //扫盘结束
#define M_DNLMT 20000  //下限位置

#define M_VEL VEL_MAX

#define DIS_M 0
#define ENA_M 1

u8 Get_MStatus(void);
extern u8 gMotion_status;
extern s32 gMotion_cmd;
extern bool gParkErr;

void STOP_Minit(void);
void PAUSE_Motion(s32 target);
void START_Motion(s32 target_pos, u16 target_vel);
void STOP_Motion(void);
void tMotor_Motion(void *p_arg);
void tMotor_Init(void *p_arg);
bool is_mstop(void);
bool is_m_err(void);
bool enable_m(bool ctrl);

#endif
