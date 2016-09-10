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

//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С
#define START_STK_SIZE 		128
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define TASK1_TASK_PRIO		5
//�����ջ��С
#define TASK1_STK_SIZE 		128
//������ƿ�
OS_TCB Task1_TaskTCB;
//�����ջ
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

//�������ȼ�
#define TASK2_TASK_PRIO		5
//�����ջ��С
#define TASK2_STK_SIZE 		128
//������ƿ�
OS_TCB Task2_TaskTCB;
//�����ջ
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
//������
void task2_task(void *p_arg);



//�������ȼ�
#define CMD_TASK_PRIO		6
//�����ջ��С
#define CMD_STK_SIZE 		1024
//������ƿ�
OS_TCB CMD_TaskTCB;
//�����ջ
CPU_STK CMD_TASK_STK[CMD_STK_SIZE];

//�������ȼ�
#define LED_TASK_PRIO		7
//�����ջ��С
#define LED_STK_SIZE 		1024
//������ƿ�
OS_TCB LED_TaskTCB;
//�����ջ
CPU_STK LED_TASK_STK[LED_STK_SIZE];

//�������ȼ�
#define EXE_TASK_PRIO		4
//�����ջ��С
#define EXE_STK_SIZE 		2048
//������ƿ�
OS_TCB EXE_TaskTCB;
//�����ջ
CPU_STK EXE_TASK_STK[EXE_STK_SIZE];

//�������ȼ�
#define INPUT_TASK_PRIO		8
//�����ջ��С
#define INPUT_STK_SIZE 		256
//������ƿ�
OS_TCB INPUT_TaskTCB;
//�����ջ
CPU_STK INPUT_TASK_STK[INPUT_STK_SIZE];

//�������ȼ�
#define MOTOR_TASK_PRIO		3
//�����ջ��С
#define MOTOR_STK_SIZE 		2048
//������ƿ�
OS_TCB MOTOR_TaskTCB;
//�����ջ
CPU_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];

//�������ȼ�
#define MINIT_TASK_PRIO		6
//�����ջ��С
#define MINIT_STK_SIZE 		512
//������ƿ�
OS_TCB MINIT_TaskTCB;
//�����ջ
CPU_STK MINIT_TASK_STK[MINIT_STK_SIZE];

void init_all(void)
{
	  u8 time = 10;
	
	//��ʼ������
    UART_init();
	
  //��ʼ������
    INPUT_Init();
	
	//��ʼ�����
    OUTPUT_Init();
	
	//��ʼ�����
    MOTOR_Init();
	
	// �ȴ����ֹͣ
	/*
	  while((time--) && (is_m_err()))
    {
			delay_ms(500);
		}
	*/
		if(time == 0)
		{
			enable_m(DIS_M); //�涨ʱ����δֹͣ ����
			while(true)
			{
			}
		}
		else
		{
			enable_m(ENA_M);  //ʹ�ܵ������ʹ�ܣ�
		}		
}

//������
int main(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();

    delay_init(168);  //ʱ�ӳ�ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
    uart_init(115200);   //���ڳ�ʼ��
    LED_Init();         //LED��ʼ��

    init_all();

		gMotor_state = MS_UNINIT; //MS_SCANNING; //״̬Ϊδ��ʼ��

    OSInit(&err);		    //��ʼ��UCOSIII
    OS_CRITICAL_ENTER();	//�����ٽ���
    //������ʼ����
    OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
                 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
    OS_CRITICAL_EXIT();	//�˳��ٽ���
    OSStart(&err);      //����UCOSIII
}


//��ʼ����������
void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);  	//ͳ������
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
    //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
    OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

    OS_CRITICAL_ENTER();	//�����ٽ���
    //����TASK1����
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

    //����TASK2����
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

    OS_CRITICAL_EXIT();	//�˳��ٽ���
//		OSTaskResume(&MINIT_TaskTCB,&err);
    OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}

void test_init(void);
//task1������
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
        task1_num++;	//����ִ1�д�����1 ע��task1_num1�ӵ�255��ʱ������㣡��
 //       LED0= ~LED0;
 //       printf("����1�Ѿ�ִ�У�%d��\r\n",task1_num);
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
  //          OSTaskDel((OS_TCB*)&Task2_TaskTCB,&err);	//����1ִ��5�˺�ɾ��������2
  //          printf("����1ɾ��������2!\r\n");
	//				task1_num = 0;
/*				
				OUTPUT_SetOne(CS_O_0, SOL07B_0);
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
				OUTPUT_SetOne(CS_O_0, SOL07A_0);		 
       	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
				OUTPUT_ResetOne(CS_O_0, SOL07B_0);
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
				OUTPUT_SetOne(CS_O_0, SOL07A_0);		 
       	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
*/				
    }
}



//task2������
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
        task2_num++;	//����2ִ�д�����1 ע��task1_num2�ӵ�255��ʱ������㣡��
//			printf("m:%d v:%d d:%d p:%d\r\n", gMotion_num,gCur_vel,gDir_vel,gPulse_num);
//        LED1=~LED1;
//			OUTPUT_TogOne(CS_O_0, SOL07A_0);
//        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
//				OUTPUT_TogOne(CS_O_0, SOL07B_0);
//			OUTPUT_TogOne(CS_O_0, SOL07A_0);
				OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);//��ʱ1s
//			OUTPUT_TogOne(CS_O_0, SOL07B_0);
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);//��ʱ1s
//			if(task2_num == 1)
			{
//				format_mapdt(param);
			}
//			OUTPUT_ResetOne(CS_O_0, SOL08A_0);
//			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);//��ʱ1s
//			OUTPUT_SetOne(CS_O_0, SOL08A_0);
//			OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
//			while(1)
//			{
//			}
   //     OUTPUT_ResetOne(CS_O_0, SOL07A_0);
    //    OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
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

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOD,GPIOEʱ��
 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //��Ƭ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ������	
	
	GPIO_SetBits(GPIOD,GPIO_InitStructure.GPIO_Pin);//����Ϊ�ߣ�����ѡ
}


