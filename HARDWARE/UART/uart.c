#include "uart.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
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
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2
	
	//USART2�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3

   //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);//��������ж�

	//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=UART_PR;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���



	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	
	//USART3�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB10��PB11

   //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3
	
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//��������ж�

	//Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=UART_PR;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

}

u8 ONLINE_RxLen(void)
{
	return uOnlineRxTail - uOnlineRxHead ;
}

u8 ONLINE_TxLen(void)
{
	return uOnlineTxTail - uOnlineTxHead ;
}

u8 ONLINE_Peer(u8 *buff, u8 len)
{
	u8 tmp = 0;
	u8 tmpcnt = uOnlineRxHead;
	while(uOnlineRxTail != tmpcnt)
	{
		if(tmp >= len)
		{
			break;
		}
		*(buff + (tmp++)) = OnlineRxBuffer[tmpcnt++];
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
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	return tmp;
}

u8 CMD_RxLen(void)
{
	return uCmdRxTail - uCmdRxHead ;
}

u8 CMD_TxLen(void)
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
		CmdTxBuffer[uCmdTxTail++] = *(buff + (tmp++));	
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	return tmp;
}

void USART2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
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
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			uOnlineTxEnd = 1;
	
		}
  }
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//�˳��ж�
#endif
}

void USART3_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
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
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
			uCmdTxEnd = 1;
		}
  }
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//�˳��ж�
#endif
}
