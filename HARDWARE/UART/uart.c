#include "uart.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

#define UART_PR 0x03

#define ONLINE_BUFF_SIZE 256
#define CMD_BUFF_SZIE 256

 

__IO u8 uOnlineTxHead = 0x00;
__IO u8 uOnlineTxTail = 0x00;
__IO u8 uOnlineRxHead = 0x00;
__IO u8 uOnlineRxTail = 0x00;
__IO u8 uOnlineTxEnd = 1;
u8 OnlineTxBuffer[ONLINE_BUFF_SIZE];
u8 OnlineRxBuffer[ONLINE_BUFF_SIZE];

__IO u8 uCmdTxHead = 0x00;
__IO u8 uCmdTxTail = 0x00;
__IO u8 uCmdRxHead = 0x00;
__IO u8 uCmdRxTail = 0x00;
__IO u8 uCmdTxEnd = 1;
u8 CmdTxBuffer[CMD_BUFF_SZIE];
u8 CmdRxBuffer[CMD_BUFF_SZIE];

void UART_init(void)
{
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* Enable USART clock */
  RCC_AHB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  /* Connect USART pins to AF7 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USART2, ENABLE);  

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART_PR;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable USART */
  USART_Cmd(USART2, ENABLE);
	
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* Enable USART clock */
  RCC_AHB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  /* Connect USART pins to AF7 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART2);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USART3, ENABLE);  

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);
  
  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART_PR;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable USART */
  USART_Cmd(USART3, ENABLE);
	
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

__inline u8 ONLINE_RxLen(void)
{
	return uOnlineRxTail - uOnlineRxHead ;
}

__inline u8 ONLINE_TxLen(void)
{
	return uOnlineTxTail - uOnlineTxHead ;
}

u8 ONLINE_Peer(u8 *buff, u8 len)
{
	u8 tmp = 0;
	u8 tmpcnt = uOnlineRxHead;
	while(uCmdRxTail != tmpcnt)
	{
		if(tmp >= len)
		{
			break;
		}
		*(buff + (tmp++)) = CmdRxBuffer[tmpcnt++];
	}
	return tmp;
	
}

u8 ONLINE_Read(u8 *buff, u8 len)
{
	u8 tmp = 0;
	while(uOnlineRxTail != uOnlineRxHead)
	{
		if(tmp >= len)
		{
			break;
		}
		*(buff + (tmp++)) = OnlineRxBuffer[uOnlineRxHead++];
	}
	return tmp;
}

u8 ONLINE_Write(u8 *buff, u8 len)
{
	u8 tmp = 0;
	while((uOnlineTxTail + 1) != uOnlineTxHead)
	{
		if(tmp >= len)
		{
			break;
		}
		OnlineTxBuffer[uOnlineTxTail++] = *(buff + (tmp++));	
	}
	if(uOnlineTxEnd == 1)
	{	
		USART_SendData(USART2, OnlineTxBuffer[uOnlineTxHead++]);
		uOnlineTxEnd = 0;
	}
	return tmp;
}

__inline u8 CMD_RxLen(void)
{
	return uCmdRxTail - uCmdRxHead ;
}

__inline u8 CMD_TxLen(void)
{
	return uCmdTxTail - uCmdTxHead ;
}

u8 CMD_Peer(u8 *buff, u8 len)
{
	u8 tmp = 0;
	u8 tmpcnt = uCmdRxHead;
	while(uCmdRxTail != tmpcnt)
	{
		if(tmp >= len)
		{
			break;
		}
		*(buff + (tmp++)) = CmdRxBuffer[tmpcnt++];
	}
	return tmp;
	
}

u8 CMD_Read(u8 *buff, u8 len)
{
	u8 tmp = 0;
	while(uCmdRxTail != uCmdRxHead)
	{
		if(tmp >= len)
		{
			break;
		}
		*(buff + (tmp++)) = CmdRxBuffer[uCmdRxHead++];
	}
	return tmp;
}

u8 CMD_Write(u8 *buff, u8 len)
{
	u8 tmp = 0;
	while((uCmdTxTail + 1) != uCmdTxHead)
	{
		if(tmp >= len)
		{
			break;
		}
		OnlineTxBuffer[uCmdTxTail++] = *(buff + (tmp++));	
	}
	if(uCmdTxEnd == 1)
	{	
		USART_SendData(USART2, OnlineTxBuffer[uCmdTxHead++]);
		uCmdTxEnd = 0;
	}
	return tmp;
}

void USART2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
	OSIntEnter();    
#endif	
  /* USART in Receiver mode */
  if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {
      /* Receive Transaction data */
      OnlineRxBuffer[uOnlineRxTail++] = USART_ReceiveData(USART2);
  }
  /* USART in Tramitter mode */
  if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
  {
    if (uOnlineTxHead != uOnlineTxTail)
    {
      /* Send Transaction data */
      USART_SendData(USART2, OnlineTxBuffer[uOnlineTxHead++]);
    }
		else
		{
			uOnlineTxEnd = 1;
		}
  }
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//退出中断
#endif
}

void USART3_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
	OSIntEnter();    
#endif	
  /* USART in Receiver mode */
  if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
  {
      /* Receive Transaction data */
      CmdRxBuffer[uCmdRxTail++] = USART_ReceiveData(USART3);
  }
  /* USART in Tramitter mode */
  if (USART_GetITStatus(USART3, USART_IT_TXE) == SET)
  {
    if (uCmdTxHead != uCmdTxTail)
    {
      /* Send Transaction data */
      USART_SendData(USART3, CmdTxBuffer[uCmdTxHead++]);
    }
		else
		{
			uCmdTxEnd = 1;
		}
  }
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//退出中断
#endif
}
