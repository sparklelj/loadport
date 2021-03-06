#include "motor.h"
#include "texecute.h"
#include "tmotion.h"


#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif


#define P_POS_A 6 //PB TIM4
#define P_POS_B 7

#define SCAN_PR 0x03

#define PARK_PR 0x03
#define ENCODER_PR 0x03

#define COUNT_PR 0x03
#define COUNT_ARR 999 //1ms (COUNT_ARR+1)*(COUNT_PSC+1)/84M
#define COUNT_PSC 839

#define MOTION_PR 0x03

#define CNT_VAL 0x8000

#define MOTION_DIV 83

bool bpulse = true;
//bool is_stop = true;
s32 gPos_num = 0;
s32 gPark_num = 0;
u8  gMotor_state = MS_UNINIT;

s32 gMotion_num = 0;
u32 gPulse_num = 0;
u16 gCur_vel = 0;
u16 gMax_vel = VEL_MAX;
s8  gDir_vel = 1;


s32 gtestcnt = 0;
s32 gtestpul = 0;

s32 gScan_pos[SCAN_NUM_MAX] = {26100,26000,25200,25000,24100,24000,22800,22600,21100,21000,20200,20000,19200,19000,17800,17600,16200,16000,15200,15000,14100,14000,12800,12600,11100,11000,8800,8600,6800,6600,3100,3000};
u8  gScan_num = 0;

u32 gDePos = 0;
u16 gCurVel = VEL_MIN;
s32 gTarPos = 0;
s32 gCurPos = 0;
s32 gStopPos = 0;
s32 gParkPos = 0;
s32 gCurDis = 0;
u16 gTarVel = VEL_MIN;
s8 gCurDir = 1;

bool gisMotorPark = false;

void CTRL_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOE

    GPIO_InitStructure.GPIO_Pin = CONN(GPIO_Pin_, M_ENABLE); //输出引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化输入
    GPIO_ResetBits(GPIOE,GPIO_InitStructure.GPIO_Pin);//设置为高

    GPIO_InitStructure.GPIO_Pin = CONN(GPIO_Pin_, M_DIR) | CONN(GPIO_Pin_, M_PULSE); //输出引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化输入
    GPIO_SetBits(GPIOE,GPIO_InitStructure.GPIO_Pin);//设置为高
}

void ENC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = CONN(GPIO_Pin_, M_ERR) | CONN(GPIO_Pin_, M_POS_P);
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource12);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource13);
//  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, CONN(EXTI_PinSource, M_POS_Z));
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, CONN(EXTI_PinSource, M_POS_P));

    EXTI_InitStructure.EXTI_Line = CONN(EXTI_Line, M_POS_P);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PARK_PR;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);



    /* TIM4 clock source enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* Enable GPIOB, clock */
    RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB,CONN(GPIO_PinSource, P_POS_A),GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB,CONN(GPIO_PinSource, P_POS_B),GPIO_AF_TIM4);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = CONN(GPIO_Pin_, P_POS_A) | CONN(GPIO_Pin_, P_POS_B);
    GPIO_Init(GPIOB, &GPIO_InitStructure);



    /* Enable the TIM4 Update Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ENCODER_PR;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Timer configuration in Encoder mode */
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = 0;  // No prescaling
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
//		TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
//    TIM_ICInit(TIM4, &TIM_ICInitStructure);
//    TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
//    TIM_ICInit(TIM4, &TIM_ICInitStructure);

// Clear all pending interrupts
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    TIM_SetCounter(TIM4, CNT_VAL);

    TIM_Cmd(TIM4, ENABLE);
}

void COUNT_Init(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
    TIM_TimeBaseStructure.TIM_Period = COUNT_ARR; 	//自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = COUNT_PSC;  //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);//初始化TIM12

    TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE); //允许定时器12更新中断
    TIM_Cmd(TIM12,ENABLE); //使能定时器12

    NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn; //定时器12中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = COUNT_PR; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void MOTION_Scan(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = CONN(GPIO_Pin_, M_SCAN);
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, CONN(EXTI_PinSource, M_SCAN));

    EXTI_InitStructure.EXTI_Line = CONN(EXTI_Line, M_SCAN);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    /*
    // test
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_Pin = CONN(GPIO_Pin_, M_POS_A);
        GPIO_Init(GPIOF, &GPIO_InitStructure);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, CONN(EXTI_PinSource, M_POS_A));

        EXTI_InitStructure.EXTI_Line = CONN(EXTI_Line, M_POS_A);
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);
    // test
    */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SCAN_PR;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void MOTION_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟

    TIM_TimeBaseStructure.TIM_Period = 0; 	//自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = MOTION_DIV;  //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化TIM3

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
//	TIM_SetAutoreload(TIM3, 0);
    TIM_Cmd(TIM3,ENABLE); //使能定时器3

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //定时器3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MOTION_PR; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void MOTOR_Init(void)
{
    CTRL_Init(); //初始化控制
    ENC_Init();  //初始化编码器反馈
    COUNT_Init(); //初始化计数器
    MOTION_Init(); //初始化多圈计数时钟
    MOTION_Scan(); //初始化扫盘
}

s32 COUNT_Get(void)
{
    s32 tmp;
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    tmp = TIM_GetCounter(TIM4);
    TIM_SetCounter(TIM4, CNT_VAL);
    tmp = tmp - CNT_VAL;
    gPos_num += tmp;
    CPU_CRITICAL_EXIT();
    return gPos_num;
}

void POS_SET(s32 target, s32 pos)
{
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
//	gPos_num += COUNT_Get();
    gPos_num = COUNT_Get();
    gPos_num = gPos_num - target;
//		gCurPos -= pos;
//  	gTarPos -= pos;
    gCurPos = gPos_num >> 1;
    gTarPos = gPos_num >> 1;
    CPU_CRITICAL_EXIT();
}

void TIM4_IRQHandler(void)
{
    /* Clear the interrupt pending flag */
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
    CPU_SR_ALLOC();
    if(TIM_GetITStatus(TIM12,TIM_IT_Update)==SET) //溢出中断
    {
        CPU_CRITICAL_ENTER();
        COUNT_Get();
//        gPos_num = COUNT_Get();
//       TIM_SetCounter(TIM4, CNT_VAL);
        CPU_CRITICAL_EXIT();
    }
    TIM_ClearITPendingBit(TIM12,TIM_IT_Update);  //清除中断标志位
}

void EXTI15_10_IRQHandler(void)
{
    CPU_SR_ALLOC();
    if(EXTI_GetITStatus(CONN(EXTI_Line, M_POS_P)) == SET)
    {
//        if(gMotor_state == MS_INITING)
        if(gismoting == true)
        {
            CPU_CRITICAL_ENTER();
            gPark_num = COUNT_Get();
            gParkPos = gCurPos;
            gisMotorPark = true;
            gMotor_state = MS_PRKEND;
            CPU_CRITICAL_EXIT();
        }

        EXTI_ClearITPendingBit(CONN(EXTI_Line, M_POS_P));
    }

    if(EXTI_GetITStatus(CONN(EXTI_Line, M_SCAN)) == SET)
    {
//        if(gMotor_state == MS_SCANNING)
        if(gMapState == MAP_MAPING)
        {
            CPU_CRITICAL_ENTER();
            gScan_pos[gScan_num] = COUNT_Get();
            gScan_num++;
            CPU_CRITICAL_EXIT();
        }
        EXTI_ClearITPendingBit(CONN(EXTI_Line, M_SCAN));
    }

    if(EXTI_GetITStatus(CONN(EXTI_Line, M_POS_A)) == SET)
    {
        if(PFin(M_POS_B) == 1)
        {
            gtestpul++;
        }
        else
        {
            gtestpul--;
        }
        EXTI_ClearITPendingBit(CONN(EXTI_Line, M_POS_A));
    }

}

__inline bool _is_stop()
{
    if((gTarPos == gCurPos) && (gDePos <= STOP_DEPOS))
    {
        return true;
    }
    return false;
}

bool is_stop()
{
    return   _is_stop();
}
__inline bool is_sameDir()
{
    s32 tmp = gCurDir;
    if((gCurDis & 0x80000000) == (tmp & 0x80000000))
    {
        return true;
    }
    return false;
}

__inline void vel_de()
{
    if(gCurVel == VEL_MIN)
    {
        gCurDir = -gCurDir;
    }
    if(gCurVel < (VEL_MIN - VEL_ACC))
    {
        gCurVel += VEL_ACC;
    }
    else
    {
        gCurVel = VEL_MIN;
    }
}

__inline void vel_acc()
{
    if(gCurVel > (gTarVel + VEL_ACC))
    {
        if(gCurDis >= 2 || gCurDis <= -2)
        {
            gCurVel -= VEL_ACC;
        }
    }
    else
    {
        gCurVel = gTarVel;
    }
}

__inline void set_pulse()
{
    if(gCurDir > 0)
    {
        if(PEout(M_PULA) == PEout(M_PULB))
        {
            PEout(M_PULA) = ~PEout(M_PULA);
        }
        else
        {
            PEout(M_PULB) = ~PEout(M_PULB);
        }
    }
    else
    {
        if(PEout(M_PULA) == PEout(M_PULB))
        {
            PEout(M_PULB) = ~PEout(M_PULB);
        }
        else
        {
            PEout(M_PULA) = ~PEout(M_PULA);
        }
    }
    gCurPos +=	gCurDir;
}

void TIM3_IRQHandler(void)
{
    CPU_SR_ALLOC();
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
    {
        CPU_CRITICAL_ENTER();
        gDePos = (VEL_MIN - gCurVel) >> VEL_ACCB;
        gStopPos = gCurPos + gDePos * gCurDir;
        gCurDis = gTarPos - gStopPos;
        if( !is_stop())
        {

            if(is_sameDir())
            {
                vel_acc();
            }
            else
            {
                vel_de();
            }
            set_pulse();
        }
        else
        {
            gCurVel = VEL_MIN;
            gStopPos = gTarPos;
        }
        TIM_SetAutoreload(TIM3, gCurVel);

        CPU_CRITICAL_EXIT();
    }
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

