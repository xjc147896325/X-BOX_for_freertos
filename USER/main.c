/**
  ******************************************************************************
  * @file    main.c
  * @author  jOKERII
  * @version v1.0
  * @date    21-06-19
  * @brief   
  ******************************************************************************
  */
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
/* Ӳ��bspͷ�ļ� */
#include "bsp_led.h"
//#include "bsp_sdio.h"
#include "sdio_test.h"
#include "bsp_debug_usart.h"
#include "bsp_zhengdian_lcd.h"



/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle;
/* LED������ */
static TaskHandle_t LED_Task_Handle;	
/* SD������ */
static TaskHandle_t SD_Task_Handle;		

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */


/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */


 
/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void LED_Task(void* pvParameters);/* LED_Task����ʵ�� */

static void SD_Task(void* pvParameters);/* SD_Task����ʵ�� */

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

///**
//	* ʹ���˾�̬�����ڴ棬�������������������û�ʵ�֣�������task.c�ļ���������
//	*	���ҽ��� configSUPPORT_STATIC_ALLOCATION ����궨��Ϊ 1 ��ʱ�����Ч
//	*/
//void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
//																		StackType_t **ppxTimerTaskStackBuffer, 
//																		uint32_t *pulTimerTaskStackSize);

//void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
//																	 StackType_t **ppxIdleTaskStackBuffer, 
//																	 uint32_t *pulIdleTaskStackSize);

//void _sys_exit(int x) 
//{ 
//    x = x; 
//} 

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

	/* ������Ӳ����ʼ�� */
	BSP_Init();

	printf("start!\r\n");
	/* ���� AppTaskCreate ���� */
	xReturn = xTaskCreate((TaskFunction_t	)AppTaskCreate,		//������
							(const char* 	)"AppTaskCreate",		//��������
							(uint32_t 		)512,	//�����ջ��С
							(void* 		  	)NULL,				//���ݸ��������Ĳ���
							(UBaseType_t 	)1, 	//�������ȼ�
							//(StackType_t*   )AppTaskCreate_Stack,	//�����ջ
							(TaskHandle_t*  )&AppTaskCreate_Handle);	//������ƿ�   
															
	if(pdPASS == xReturn)/* �����ɹ� */
	vTaskStartScheduler();   /* �������񣬿������� */

	while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

	taskENTER_CRITICAL();           //�����ٽ���

	/* ����LED_Task���� */
	xReturn = xTaskCreate((TaskFunction_t	)LED_Task,		//������
							(const char* 	)"LED_Task",		//��������
							(uint32_t 		)128,					//�����ջ��С
							(void* 		  	)NULL,				//���ݸ��������Ĳ���
							(UBaseType_t 	)4, 				//�������ȼ�
							//(StackType_t*   )LED_Task_Stack,	//�����ջ
							(TaskHandle_t*  )&LED_Task_Handle);	//������ƿ�   
							
	if(pdPASS == xReturn)/* �����ɹ� */
		printf("LED_Task���񴴽��ɹ�!\r\n");
	else
		printf("LED_Task���񴴽�ʧ��!\r\n");
	
	/* ����SD_Task���� */
	xReturn = xTaskCreate((TaskFunction_t	)SD_Task,		//������
							(const char* 	)"SD_Task",		//��������
							(uint32_t 		)128,					//�����ջ��С
							(void* 		  	)NULL,				//���ݸ��������Ĳ���
							(UBaseType_t 	)5, 				//�������ȼ�
							(TaskHandle_t*  )&SD_Task_Handle);	//������ƿ�   
	if(pdPASS == xReturn)/* �����ɹ� */
		printf("SD_Task���񴴽��ɹ�!\r\n");
	else
		printf("SD_Task���񴴽�ʧ��!\r\n");
   

	vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����

	taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LED_Task(void* parameter)
{	
	/* �ļ�ϵͳ��ʼ�� */
	FileSystem_Init();
  /* �ļ�ϵͳ��д���� */
	FileSystem_Test();
    while (1)
    {
        LED1_ON;
        vTaskDelay(500);   /* ��ʱ500��tick */
        //printf("LED_Task Running,LED1_ON\r\n");
        
        LED1_OFF;     
        vTaskDelay(500);   /* ��ʱ500��tick */		 		
        //printf("LED_Task Running,LED1_OFF\r\n");
		//LCD_ShowString(30, 50, 200, 24, 24, (uint8_t *)"test!");
		//while(1);

    }
}

/**********************************************************************
  * @ ������  �� SD_Task
  * @ ����˵���� SD_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void SD_Task(void* parameter)
{	
    while (1)
    {
        //printf("\r\n��ʼ����SD����дʵ��\r\n");	
		//SD_Test();
			 		
        vTaskDelay(20000);   /* ��ʱ500��tick */	

    }
}

/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	uint16_t lcd_num;
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED ��ʼ�� */
	LED_GPIO_Config();

	/* ���ڳ�ʼ��	*/
	Debug_USART_Config();
	
	lcd_num = LCD_Init();
	
	printf("LCD ID = %x\r\n", lcd_num);
	POINT_COLOR = RED;//��������Ϊ��ɫ 
	LCD_ShowString(30, 50, 200, 24, 24, (uint8_t *)"test!");

	printf("Init finish\r\n");
}
 
