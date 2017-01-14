#ifndef __TSTATUS_H
#define __TSTATUS_H

#include "stdbool.h"

void tStatus_Check(void *p_arg);
void motor_error(void);

bool is_onload(void);
bool is_unload(void);
bool is_normal(void);
bool is_nothing(void);
#endif
