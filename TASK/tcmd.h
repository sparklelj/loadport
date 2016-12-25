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
#define CMD_ACTION_SYSIN 0x2C

#define BCAK_ACK 0x00
#define BCAK_NAK 0x01
#define BCAK_INF 0x02
#define BCAK_ABS 0x03
#define BCAK_RIF 0x04
#define BCAK_RAS 0x05

#define B_ERROR 0x30
#define B_CBUSY 0x31
#define B_FPILG 0x32
#define B_DPOSI 0x33
#define B_MPARM 0x34
#define B_MPSTP 0x35
#define B_ZPOSI 0x37
#define B_DVACM 0x38
#define B_LTCHU 0x39
#define B_ORGYT 0x80
#define B_CLDDK 0x81
#define B_YPOSI 0x82
#define B_LATCH 0x83
#define B_CULDK 0x84
#define B_CLOAD 0x85
#define B_RMPOS 0x86
#define B_SYSIN 0xBA

#define R_SAFTY  0xFF
#define R_AIRSN  0x27
#define R_FNAST  0xFC
#define R_ZLMITD 0x02
#define R_ZLMITU 0x42
#define R_YLMITD 0x04
#define R_YLMITU 0x44
#define R_PROTS  0x07
#define R_DLMITO 0x08
#define R_DLMITC 0x48
#define R_MPBARO 0x09
#define R_MPBARC 0x49
#define R_MPZLMD 0x10
#define R_MPZLMU 0x50
#define R_MPSTPN 0x11
#define R_MPSTPF 0x51
#define R_MPEDL  0x12
#define R_CLOPS  0x21
#define R_CLCLS  0x22
#define R_CLOPSU 0x61
#define R_CLOPSD 0x62
#define R_CLOPSM 0x63
#define R_DROPS  0x23
#define R_DRCLS  0x24
#define R_VACCS  0x25
#define R_VACOS  0x26
#define R_INTOP  0xA1
#define R_INTCL  0xA2
#define R_SYSIN  0xB1
#define R_RESVE  0xA3
#define R_PARER  0xA4
#define R_INLKE  0xFD
#define R_COMER  0xFE

#define W_NO    0x00
#define W_ONE   0x01
#define W_OVER  0x02
#define W_CROSS 0x03
#define W_OTHER 0x10

extern u8 gAddr[2];
extern u8 gLED_status[9];
extern u8 gCmd_action;
extern u8 gCom_mod;
extern u8 gCur_status;
extern u8 gPre_status;
extern u8 gEnd_act;

bool send_msg(u8 type, char* cmd_n, u8* param, u8 pLen);
void tCMD_Proc(void *p_arg);
bool format_mapdt(u8* param);
#endif
