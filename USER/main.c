#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "tmotion.h"
#include "motor.h"
#include "texecute.h"
#include "tinput.h"
#include "input.h"
#include "uart.h"
#include "output.h"
#include "tcmd.h"
#include "tled.h"
#include "tinput.h"

//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小
#define START_STK_SIZE 		128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//任务优先级
#define TASK1_TASK_PRIO		5
//任务堆栈大小
#define TASK1_STK_SIZE 		128
//任务控制块
OS_TCB Task1_TaskTCB;
//任务堆栈
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

//任务优先级
#define TASK2_TASK_PRIO		5
//任务堆栈大小
#define TASK2_STK_SIZE 		128
//任务控制块
OS_TCB Task2_TaskTCB;
//任务堆栈
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
//任务函数
void task2_task(void *p_arg);



//任务优先级
#define CMD_TASK_PRIO		6
//任务堆栈大小
#define CMD_STK_SIZE 		1024
//任务控制块
OS_TCB CMD_TaskTCB;
//任务堆栈
CPU_STK CMD_TASK_STK[CMD_STK_SIZE];

//任务优先级
#define LED_TASK_PRIO		7
//任务堆栈大小
#define LED_STK_SIZE 		1024
//任务控制块
OS_TCB LED_TaskTCB;
//任务堆栈
CPU_STK LED_TASK_STK[LED_STK_SIZE];

//任务优先级
#define EXE_TASK_PRIO		4
//任务堆栈大小
#define EXE_STK_SIZE 		2048
//任务控制块
OS_TCB EXE_TaskTCB;
//任务堆栈
CPU_STK EXE_TASK_STK[EXE_STK_SIZE];

//任务优先级
#define INPUT_TASK_PRIO		8
//任务堆栈大小
#define INPUT_STK_SIZE 		256
//任务控制块
OS_TCB INPUT_TaskTCB;
//任务堆栈
CPU_STK INPUT_TASK_STK[INPUT_STK_SIZE];

//任务优先级
#define MOTOR_TASK_PRIO		3
//任务堆栈大小
#define MOTOR_STK_SIZE 		2048
//任务控制块
OS_TCB MOTOR_TaskTCB;
//任务堆栈
CPU_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];

//任务优先级
#define MINIT_TASK_PRIO		6
//任务堆栈大小
#define MINIT_STK_SIZE 		512
//任务控制块
OS_TCB MINIT_TaskTCB;
//任务堆栈
CPU_STK MINIT_TASK_STK[MINIT_STK_SIZE];

void init_all(void)
{
	  u8 time = 10;
	
	//初始化串口
    UART_init();
	
  //初始化输入
    INPUT_Init();
	
	//初始化输出
    OUTPUT_Init();
	
	//初始化电机
    MOTOR_Init();
	
	// 等待电机停止
	/*
	  while((time--) && (is_m_err()))
    {
			delay_ms(500);
		}
	*/
		if(time == 0)
		{
			enable_m(DIS_M); //规定时间内未停止 错误
			while(true)
			{
			}
		}
		else
		{
			enable_m(ENA_M);  //使能电机（已使能）
		}		
}

//主函数
int main(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();

    delay_init(168);  //时钟初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
    uart_init(115200);   //串口初始化
    LED_Init();         //LED初始化

    init_all();

		gMotor_state = MS_UNINIT; //MS_SCANNING; //状态为未初始化

    OSInit(&err);		    //初始化UCOSIII
    OS_CRITICAL_ENTER();	//进入临界区
    //创建开始任务
    OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
                 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
    OS_CRITICAL_EXIT();	//退出临界区
    OSStart(&err);      //开启UCOSIII
}


//开始任务任务函数
void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
    //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
    OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

    OS_CRITICAL_ENTER();	//进入临界区
    //创建TASK1任务
    OSTaskCreate((OS_TCB 	* )&Task1_TaskTCB,
                 (CPU_CHAR	* )"Task1 task",
                 (OS_TASK_PTR )task1_task,
                 (void		* )0,
                 (OS_PRIO	  )TASK1_TASK_PRIO,
                 (CPU_STK   * )&TASK1_TASK_STK[0],
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,
                 (CPU_STK_SIZE)TASK1_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

    //创建TASK2任务
    OSTaskCreate((OS_TCB 	* )&Task2_TaskTCB,
                 (CPU_CHAR	* )"task2 task",
                 (OS_TASK_PTR )task2_task,
                 (void		* )0,
                 (OS_PRIO	  )TASK2_TASK_PRIO,
                 (CPU_STK   * )&TASK2_TASK_STK[0],
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,
                 (CPU_STK_SIZE)TASK2_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

    OSTaskCreate((OS_TCB 	* )&CMD_TaskTCB,
                 (CPU_CHAR	* )"cmd task",
                 (OS_TASK_PTR )tCMD_Proc,
                 (void		* )0,
                 (OS_PRIO	  )CMD_TASK_PRIO,
                 (CPU_STK   * )&CMD_TASK_STK[0],
                 (CPU_STK_SIZE)CMD_STK_SIZE/10,
                 (CPU_STK_SIZE)CMD_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

    OSTaskCreate((OS_TCB 	* )&EXE_TaskTCB,
                 (CPU_CHAR	* )"exe task",
                 (OS_TASK_PTR )tExe_Action,
                 (void		* )0,
                 (OS_PRIO	  )EXE_TASK_PRIO,
                 (CPU_STK   * )&EXE_TASK_STK[0],
                 (CPU_STK_SIZE)EXE_STK_SIZE/10,
                 (CPU_STK_SIZE)EXE_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

    OSTaskCreate((OS_TCB 	* )&LED_TaskTCB,
                 (CPU_CHAR	* )"led task",
                 (OS_TASK_PTR )tLED_Control,
                 (void		* )0,
                 (OS_PRIO	  )LED_TASK_PRIO,
                 (CPU_STK   * )&LED_TASK_STK[0],
                 (CPU_STK_SIZE)LED_STK_SIZE/10,
                 (CPU_STK_SIZE)LED_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

    OSTaskCreate((OS_TCB 	* )&INPUT_TaskTCB,
                 (CPU_CHAR	* )"input task",
                 (OS_TASK_PTR )tInput_Scan,
                 (void		* )0,
                 (OS_PRIO	  )INPUT_TASK_PRIO,
                 (CPU_STK   * )&INPUT_TASK_STK[0],
                 (CPU_STK_SIZE)INPUT_STK_SIZE/10,
                 (CPU_STK_SIZE)INPUT_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

		OSTaskCreate((OS_TCB 	* )&MINIT_TaskTCB,
                 (CPU_CHAR	* )"minittor task",
                 (OS_TASK_PTR )tMotor_Init,
                 (void		* )0,
                 (OS_PRIO	  )MINIT_TASK_PRIO,
                 (CPU_STK   * )&MINIT_TASK_STK[0],
                 (CPU_STK_SIZE)MINIT_STK_SIZE/10,
                 (CPU_STK_SIZE)MINIT_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
								 
    OSTaskCreate((OS_TCB 	* )&MOTOR_TaskTCB,
                 (CPU_CHAR	* )"motor task",
                 (OS_TASK_PTR )tMotor_Motion,
                 (void		* )0,
                 (OS_PRIO	  )MOTOR_TASK_PRIO,
                 (CPU_STK   * )&MOTOR_TASK_STK[0],
                 (CPU_STK_SIZE)MOTOR_STK_SIZE/10,
                 (CPU_STK_SIZE)MOTOR_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);

    OS_CRITICAL_EXIT();	//退出临界区
//		OSTaskResume(&MINIT_TaskTCB,&err);
    OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}

void test_init(void);
//task1任务函数
void task1_task(void *p_arg)
{
    u8 task1_num=0;
		s32 tt1;
		u8 poa, pob = 0;
	
		int j = 0;
    OS_ERR err;
//	CPU_SR_ALLOC();
    p_arg = p_arg;
		gtestcnt = 0;
		gtestpul = 0;
		test_init();
    while(1)
    {
			poa = GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_0);
			pob = GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_1);
			if(gtestcnt > gtestpul)
			{
				if(poa == pob)
				{
					PDout(1) = ~PDout(1);
				}
				else
				{
					PDout(0) = PDout(1);
				}
				gtestcnt--;
			}
			if(gtestcnt < gtestpul)
			{
				if(poa == pob)
				{
					PDout(0) = ~PDout(0);
				}
				else
				{
					PDout(1) = PDout(0);
				}
				gtestcnt++;
			}
        task1_num++;	//任务执1行次数加1 注意task1_num1加到255的时候会清零！！
 //       LED0= ~LED0;
 //       printf("任务1已经执行：%d次\r\n",task1_num);
        if((task1_num % 300) == 1)
        {
					LED0= ~LED0;
					OUTPUT_SetOne(CS_O_0, SOL07A_0);
					OUTPUT_ResetOne(CS_O_0, SOL07B_0);
					
					OUTPUT_SetOne(CS_O_1, SOL01B_1);
					OUTPUT_ResetOne(CS_O_1, SOL01A_1);
//					PEout(7) = 0;
				}
					OSTimeDlyHMSM(0,0,0,2,OS_OPT_TIME_HMSM_STRICT,&err);
//					tt1 = gPos_num;
	//				printf("m:%d v:%d d:%d p:%d cm:%d ps:%d\r\n", gMotion_num,gCur_vel,gDir_vel,gPulse_num ,gMotion_cmd,gPos_num);
  //          OSTaskDel((OS_TCB*)&Task2_TaskTCB,&err);	//任务1执行5此后删除掉任务2
  //          printf("任务1删除了任务2!\r\n");
	//				task1_num = 0;
/*				
				OUTPUT_SetOne(CS_O_0, SOL07B_0);
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
				OUTPUT_SetOne(CS_O_0, SOL07A_0);		 
       	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
				OUTPUT_ResetOne(CS_O_0, SOL07B_0);
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
				OUTPUT_SetOne(CS_O_0, SOL07A_0);		 
       	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
*/				
    }
}



//task2任务函数
void task2_task(void *p_arg)
{
    u8 task2_num=0;
	u8 param[30];
	u8 i,j = 0;
    OS_ERR err;
//	CPU_SR_ALLOC();
    p_arg = p_arg;

//OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
//	OUTPUT_SetOne(CS_O_0, SOL08A_0);
//START_Motion(100, 0xEFF0);
OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err);
OSTaskResume(&MINIT_TaskTCB,&err);	
    while(1)
    {
			for(i=0;i<8;i++)
			{
				printf("input state num:%d val:",i);
				for(j=0;j<8;j++)
				{
					printf("%d", (gStatus_scan[i] >> j) & 0x01);
				}
				printf("\r\n");
			}
			printf("count:%d \r\n",COUNT_Get());
			printf("gPulse_num:%d  gMotion_cmd:%d  gMotion_num:%d  gtestcnt:%d  gtestpul:%d \r\n",gPulse_num,gMotion_cmd,gMotion_num,gtestcnt,gtestpul);
			printf("gScan_num:%d  gPos_num:%d  gvel:%d\r\n",gScan_num, gPos_num,gCur_vel);
			printf("M_POS_P:%d \r\n",PFin(M_POS_P));
			printf("M_SCAN:%d \r\n",PFin(M_SCAN));
			printf("M_ERR:%d \r\n",PFin(M_ERR));
			
			printf("scan result:");
			for(j=0;j<gScan_num;j++){
				printf("%d  ",gScan_pos[j]);
			}
			printf("\r\n");
			printf("\r\n");
        task2_num++;	//任务2执行次数加1 注意task1_num2加到255的时候会清零！！
//			printf("m:%d v:%d d:%d p:%d\r\n", gMotion_num,gCur_vel,gDir_vel,gPulse_num);
//        LED1=~LED1;
//			OUTPUT_TogOne(CS_O_0, SOL07A_0);
//        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
//				OUTPUT_TogOne(CS_O_0, SOL07B_0);
//			OUTPUT_TogOne(CS_O_0, SOL07A_0);
				OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);//延时1s
//			OUTPUT_TogOne(CS_O_0, SOL07B_0);
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);//延时1s
//			if(task2_num == 1)
			{
//				format_mapdt(param);
			}
//			OUTPUT_ResetOne(CS_O_0, SOL08A_0);
//			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);//延时1s
//			OUTPUT_SetOne(CS_O_0, SOL08A_0);
//			OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
//			while(1)
//			{
//			}
   //     OUTPUT_ResetOne(CS_O_0, SOL07A_0);
    //    OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
			/*
			while(!(is_mstop() && (gMotion_num == 10)))
			{}
				OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
				START_Motion(-50, 0xEFF0);
			while(gMotion_num > -10)
			{}
				printf("-300");
				START_Motion(-100, 0xEFF0);
			while(gMotion_num > -40)
			{}
				printf("100");
				START_Motion(100, 0xDFF0);	
				*/
    }
}


void test_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOD,GPIOE时钟
 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //输片引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化输入	
	
	GPIO_SetBits(GPIOD,GPIO_InitStructure.GPIO_Pin);//设置为高，都不选
}


