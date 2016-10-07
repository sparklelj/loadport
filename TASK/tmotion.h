#ifndef __TMOTION_H
#define __TMOTION_H

#include "sys.h"

#define M_UPLMT 900000        //��ʼλ��
#define M_STRMP 700000  //��ʼɨ��λ��
#define M_STPMP 300000  //ɨ�̽���
#define M_DNLMT 200000  //����λ��

#define M_VEL VEL_MAX

#define DIS_M 0
#define ENA_M 1

u8 Get_MStatus(void);
extern u8 gMotion_status;
extern s32 gMotion_cmd;

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
