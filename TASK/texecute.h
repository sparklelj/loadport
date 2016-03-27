#ifndef __TEXECUTE_H
#define __TEXECUTE_H

#include "sys.h"

#define SCAN_UPP 0
#define SCAN_LOW 1

extern u8 scan_mode;
bool Analyze_Scan(u8* result);

#endif
