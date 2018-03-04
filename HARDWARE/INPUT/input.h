#ifndef __INPUT_H
#define __INPUT_H

#include "sys.h"

#define CLS01A_9  8   //PE
#define CLS01B_9  9
#define CLS01C_9  10
#define CLS02A_9  11
#define CLS02B_9  12
#define CLS03A_9  13
#define CLS03B_9  14
#define FAN01_9   15

#define CLS04A_10 8
#define CLS04B_10 9
#define CLS05A_10 10
#define CLS05B_10 11
#define CLS08A_10 12
#define CLS08B_10 13
#define PH14_10   14
#define PH15_10   15

#define CLS07A_11 8
#define CLS07B_11 9
#define CLS06A_11 10
#define CLS06B_11 11
#define FS02_11   12
#define PH06_11   13
#define PH07_11   14
#define PH08_11   15

#define ES04_12   8
#define ES05_12   9
#define ES06_12   10
#define ES01_12   11
#define FS01_12   12
#define AS01_12   13
#define VS01_12   14
#define F24LO_12  15

#define IOADR01_13   8
#define IOADR02_13   9
#define IOADR03_13   10
#define IOADR04_13   11
#define IOSW01_13    12
#define IOSW02_13    13
#define IF01_13      14
#define IF02_13      15

#define LED01IN_14   8
#define LED02IN_14   9
#define LED03IN_14   10
#define LED04IN_14   11
#define LED05IN_14   12
#define LED06IN_14   13
#define IL02_14      14
#define IL01_14      15

#define SW01IN_15  8
#define PH01_15    9
#define PH02_15    10
#define PH03_15    11
#define PH09_15    12
#define PH10_15    13
#define LOAD_15    12
#define ULAD_15    14
#define PH13_15    14
#define FZ01_15    15

#define CS_I_9    9    //PD
#define CS_I_10   10
#define CS_I_11   11
#define CS_I_12   12
#define CS_I_13   13
#define CS_I_14   14
#define CS_I_15   15


#define IS_INPUT_CS(CS) (((CS) == 9))  || \
												 ((CS) == 10)) || \
												 ((CS) == 11)) || \
												 ((CS) == 12)) || \
												 ((CS) == 13)) || \
												 ((CS) == 14)) || \
												 ((CS) == 15)))
#define IS_INPUT_PIN(PIN) (((CS) == 8))  || \
													 ((CS) == 9))  || \
												 	 ((CS) == 10)) || \
													 ((CS) == 11)) || \
													 ((CS) == 12)) || \
													 ((CS) == 13)) || \
													 ((CS) == 14)) || \
													 ((CS) == 15)))

void INPUT_Init(void);
u8 INPUT_Read(u8 cs_num);
u8 INPUT_ReadOne(u8 cs_num, u8 pin);

#endif
