#ifndef __TEXECUTE_H
#define __TEXECUTE_H

#include "sys.h"

#define SCAN_UPP 0
#define SCAN_LOW 1

extern u8 scan_mode;
extern u8 gCur_action;
extern bool gIs_init;
extern u8 gCur_pause;
extern u8 gCur_stop;
extern u8 gCur_abort;
extern u8 gCur_retry;

bool Analyze_Scan(u8* result);

#endif
