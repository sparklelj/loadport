#ifndef __TEXECUTE_H
#define __TEXECUTE_H

#include "sys.h"

#define SCAN_UPP 0
#define SCAN_LOW 1

extern u8 scan_mode;
extern u8 gCur_action;
extern bool gIs_init;
extern bool is_origin;
extern bool is_aborg;
extern u8 gCur_pause;
extern u8 gCur_stop;
extern u8 gCur_abort;
extern u8 gCur_retry;
extern u8 gErr_no;
extern u8 gMap_status;

u8 get_init(void);
u8 get_oper(void);
u8 get_mapsts(void);
bool Analyze_Scan(u8* result);
void tExe_Action(void *p_arg);
#endif
