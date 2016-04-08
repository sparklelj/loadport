#ifndef __TCMD_H
#define __TCMD_H

#include "includes.h"
#include "stdio.h"
#include "stdbool.h"

#define G_CUR_STA_UNI 0
#define G_CUR_STA_INI 1
#define G_CUR_STA_RUN 2
#define G_CUR_STA_PAU 3
#define G_CUR_STA_RSM 4
#define G_CUR_STA_ABO 5
#define G_CUR_STA_STP 6
#define G_CUR_STA_ERR 7
#define G_CUR_STA_INT 8
#define G_CUR_STA_NAC 9
#define G_CUR_STA_END 10

#define G_CUR_CMD_NO 0

#define CMD_ACTION_NOACT 0x00
#define CMD_ACTION_ORGSH 0x01
#define CMD_ACTION_ABORG 0x02
#define CMD_ACTION_CLOAD 0x03
#define CMD_ACTION_CLDDK 0x04
#define CMD_ACTION_CLDYD 0x05
#define CMD_ACTION_CLDOP 0x06
#define CMD_ACTION_CLDMP 0x07
#define CMD_ACTION_CLMPO 0x08
#define CMD_ACTION_CULOD 0x09
#define CMD_ACTION_CULDK 0x0A
#define CMD_ACTION_CUDCL 0x0B
#define CMD_ACTION_CUDNC 0x0C
#define CMD_ACTION_CULYD 0x0D
#define CMD_ACTION_CULFC 0x0E
#define CMD_ACTION_CUDMP 0x0F
#define CMD_ACTION_CUMDK 0x10
#define CMD_ACTION_CUMFC 0x11
#define CMD_ACTION_MAPDO 0x12
#define CMD_ACTION_REMAP 0x13
#define CMD_ACTION_PODOP 0x14
#define CMD_ACTION_PODCL 0x15
#define CMD_ACTION_VACON 0x16
#define CMD_ACTION_VACOF 0x17
#define CMD_ACTION_DOROP 0x18
#define CMD_ACTION_DORCL 0x19
#define CMD_ACTION_MAPOP 0x1A
#define CMD_ACTION_MAPCL 0x1B
#define CMD_ACTION_ZDRUP 0x1C
#define CMD_ACTION_ZDRDW 0x1D
#define CMD_ACTION_ZDRMP 0x1E
#define CMD_ACTION_ZMPST 0x1F
#define CMD_ACTION_ZMPED 0x20
#define CMD_ACTION_MSTON 0x21
#define CMD_ACTION_MSTOF 0x22
#define CMD_ACTION_YWAIT 0x23
#define CMD_ACTION_YDOOR 0x24
#define CMD_ACTION_DORBK 0x25
#define CMD_ACTION_DORFW 0x26
#define CMD_ACTION_RETRY 0x27
#define CMD_ACTION_STOP_ 0x28
#define CMD_ACTION_PAUSE 0x29
#define CMD_ACTION_ABORT 0x2A
#define CMD_ACTION_RESUM 0x2B

#define BCAK_ACK 0x00
#define BCAK_NAK 0x01
#define BCAK_FIN 0x02
#define BCAK_ABS 0x03
#define BCAK_RIF 0x04
#define BCAK_RAS 0x05

extern u8 gAddr[2];
extern u8 gLED_status[8];
extern u8 gCmd_action;
extern u8 gCom_mod;
extern u8 gCUr_status;
extern u8 gEnd_act;

bool send_msg(u8 type, char* cmd_n, u8* param, u8 pLen);
void tCMD_Proc(void *p_arg);
#endif
