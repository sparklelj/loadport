#ifndef __OUTPUT_H
#define __OUTPUT_H

#include "sys.h"

#define SOL07A_0 0   //PF
#define SOL07B_0 1
#define SOL08A_0 2
#define SOL08B_0 3
#define IF11_0   4
#define IF12_0   5
#define IF13_0   6
#define IF14_0   7

#define SOL01A_1 0
#define SOL01B_1 1
#define SOL02A_1 2
#define SOL02B_1 3
#define SOL03A_1 4
#define SOL03B_1 5
#define SOL04A_1 6
#define SOL04B_1 7

#define SOL05A_2      0
#define SOL05B_2      1
#define SOL06A_2      2
#define SOL06B_2      3
#define SOL09B_2      4
#define SOL09N_2      5
#define SW01OUT_2     6
#define SWLED01OUT_2  7

#define LED01OUT_3 0
#define LED02OUT_3 1
#define LED03OUT_3 2
#define LED04OUT_3 3
#define LED05OUT_3 4
#define LED06OUT_3 5
#define LED07OUT_3 6
#define LED08OUT_3 7

#define CS_O_0 0  //PE
#define CS_O_1 1
#define CS_O_2 2
#define CS_O_3 3


/*
#define SOL07A_0 PFout(0)
#define SOL07B_0 PFout(1)
#define SOL08A_0 PFout(2)
#define SOL08B_0 PFout(3)
#define IF11_0   PFout(4)
#define IF12_0   PFout(5)
#define IF13_0   PFout(6)
#define IF14_0   PFout(7)

#define SOL01A_1 PFout(0)
#define SOL01B_1 PFout(1)
#define SOL02A_1 PFout(2)
#define SOL02B_1 PFout(3)
#define SOL03A_1 PFout(4)
#define SOL03B_1 PFout(5)
#define SOL04A_1 PFout(6)
#define SOL04B_1 PFout(7)

#define SOL05A_2      PFout(0)
#define SOL05B_2      PFout(1)
#define SOL06A_2      PFout(2)
#define SOL06B_2      PFout(3)
#define SOL09B_2      PFout(4)
#define SOL09N_2      PFout(5)
#define SW01OUT_2     PFout(6)
#define SWLED01OUT_2  PFout(7)

#define LED01OUT_3 PFout(0)
#define LED02OUT_3 PFout(1)
#define LED03OUT_3 PFout(2)
#define LED04OUT_3 PFout(3)
#define LED05OUT_3 PFout(4)
#define LED06OUT_3 PFout(5)
#define LED07OUT_3 PFout(6)
#define LED08OUT_3 PFout(7)

#define CS_O_0 PEout(0)
#define CS_O_1 PEout(1)
#define CS_O_2 PEout(2)
#define CS_O_3 PEout(3)
*/

void OUTPUT_Init(void);
void OUTPUT_Write(u8 cs_num, u8 val);
void OUTPUT_SetOne(u8 cs_num, u8 pin);
void OUTPUT_ResetOne(u8 cs_num, u8 pin);
#endif
