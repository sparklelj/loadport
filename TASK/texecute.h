#ifndef __TEXECUTE_H
#define __TEXECUTE_H

#include "sys.h"

#define SCAN_UPP 0
#define SCAN_LOW 1


#define MAP_UNMAP  0x00
#define MAP_MAPING 0x01
#define MAP_ERR    0x02
#define MAP_ABORT  0x03
#define MAP_STOP   0x04
#define MAP_END    0x05

extern u8 scan_mode;
extern u8 gCur_action;
extern bool gIs_init;
extern bool is_origin;
extern bool is_aborg;
extern u8 gCur_pause;
extern u8 gCur_stop;
extern u8 gCur_abort;
extern u8 gCur_retry;
extern bool gIsAborg;
extern u8 gErr_no;
extern u8 gMap_status;
extern bool gissysinit;
extern bool gIsError;

extern u8 gMapState;

u8 run_dradsr(bool pause);
u8 get_init(void);
u8 get_oper(void);
u8 get_mapsts(void);
bool Analyze_Scan(u8* result);
void tExe_Action(void *p_arg);
#endif
