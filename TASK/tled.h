#ifndef __TLED_H
#define __TLED_H

#include "includes.h"
#include "stdio.h"
#include "stdbool.h"

#define MAX_LED 7

#define LED_LOAD      0x01
#define LED_UNLOAD    0x02
#define LED_OPACCE    0x03
#define LED_PRESENT   0x04
#define LED_PLACEMENT 0x05
#define LED_ALARM     0x06
#define LED_MANUAL    0x07

#define LED_ON      0x04
#define LED_FLASH   0x08
#define LED_OFF     0x0C
#define LED_ON_M    0x11
#define LED_FLASH_M 0x12
#define LED_OFF_M   0x13

void set_led(u8 led, u8 status);
void tLED_Control(void *p_arg);
#endif
