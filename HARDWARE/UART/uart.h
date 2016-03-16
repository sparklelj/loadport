#ifndef __UART_H
#define __UART_H

#include "sys.h"

void UART_init(void);
__inline u8 ONLINE_RxLen(void);
__inline u8 ONLINE_TxLen(void);
u8 ONLINE_Peer(u8 *buff, u8 len);
u8 ONLINE_Read(u8 *buff, u8 len);
u8 ONLINE_Write(u8 *buff, u8 len);
__inline u8 CMD_RxLen(void);
__inline u8 CMD_TxLen(void);
u8 CMD_Peer(u8 *buff, u8 len);
u8 CMD_Read(u8 *buff, u8 len);
u8 CMD_Write(u8 *buff, u8 len);

#endif
