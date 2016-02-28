#include "output.h"


#define IS_OUTPUT_CS(CS) (((CS) == 0) || \
													((CS) == 1) || \
													((CS) == 2) || \
													((CS) == 3))
#define IS_OUTPUT_PIN(PIN) (((PIN) == 0) || \
														((PIN) == 1) || \
														((PIN) == 2) || \
														((PIN) == 3) || \
														((PIN) == 4) || \
														((PIN) == 5) || \
														((PIN) == 6) || \
														((PIN) == 7) ||)
														

void OUTPUT_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOE,GPIOF时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
																GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; //输出引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化输入
	
	GPIO_SetBits(GPIOF,GPIO_InitStructure.GPIO_Pin);//设置为高
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; //输出片选
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化输入	
	
	GPIO_SetBits(GPIOE,GPIO_InitStructure.GPIO_Pin);//设置为高，都不选
}

void OUTPUT_Write(u8 cs_num, u8 val)
{
	u16 tmp = 0;
	assert_param(IS_OUTPUT_CS(cs_num));
	tmp = GPIO_ReadInputData(GPIOF);
	tmp = (tmp & 0xFF00) + val;
	PDout(cs_num) = 0; //相应片选，低有效
	GPIO_Write(GPIOF, tmp);
}

void OUTPUT_SetOne(u8 cs_num, u8 pin)
{
	assert_param(IS_OUTPUT_CS(cs_num));
	assert_param(IS_OUTPUT_PIN(pin));
	PDout(cs_num) = 0; //相应片选，低有效
	PFout(pin) = 1;
}

void OUTPUT_ResetOne(u8 cs_num, u8 pin)
{
	assert_param(IS_OUTPUT_CS(cs_num));
	assert_param(IS_OUTPUT_PIN(pin));
	PDout(cs_num) = 0; //相应片选，低有效
	PFout(pin) = 0;
}
