#include "input.h"

void INPUT_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOD,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
																GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //��������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ������
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | \
																GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //��Ƭ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ������	
	
	GPIO_SetBits(GPIOD,GPIO_InitStructure.GPIO_Pin);//����Ϊ�ߣ�����ѡ
}

u8 INPUT_Read(u8 cs_num)
{
	u16 tmp = 0;
	u8 input = 0xFF;
	assert_param(IS_INPUT_CS(cs_num));
	PDout(cs_num) = 0; //��ӦƬѡ������Ч
	tmp = GPIO_ReadInputData(GPIOE);
	input = tmp >> 8;
	return input;
}

u8 INPUT_ReadOne(u8 cs_num, u8 pin)
{
	assert_param(IS_INPUT_CS(cs_num));
	assert_param(IS_INPUT_PIN(pin));
	PDout(cs_num) = 0; //��ӦƬѡ������Ч
	return PEin(pin);
}
