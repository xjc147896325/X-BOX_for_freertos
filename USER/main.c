/**
  ******************************************************************************
  * @file    main.c
  * @author  jOKERII
  * @version v1.0
  * @date    21-06-19
  * @brief   
  ******************************************************************************
  */
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
/* 硬件bsp头文件 */
#include "bsp_led.h"
//#include "bsp_sdio.h"
#include "sdio_test.h"
#include "bsp_debug_usart.h"
#include "bsp_zhengdian_lcd.h"



/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle;	
/* SD任务句柄 */
static TaskHandle_t SD_Task_Handle;		

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */


/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */


 
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */

static void LED_Task(void* pvParameters);/* LED_Task任务实现 */

static void SD_Task(void* pvParameters);/* SD_Task任务实现 */

static void BSP_Init(void);/* 用于初始化板载相关资源 */

///**
//	* 使用了静态分配内存，以下这两个函数是由用户实现，函数在task.c文件中有引用
//	*	当且仅当 configSUPPORT_STATIC_ALLOCATION 这个宏定义为 1 的时候才有效
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
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void)
{	
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

	/* 开发板硬件初始化 */
	BSP_Init();

	printf("start!\r\n");
	/* 创建 AppTaskCreate 任务 */
	xReturn = xTaskCreate((TaskFunction_t	)AppTaskCreate,		//任务函数
							(const char* 	)"AppTaskCreate",		//任务名称
							(uint32_t 		)512,	//任务堆栈大小
							(void* 		  	)NULL,				//传递给任务函数的参数
							(UBaseType_t 	)1, 	//任务优先级
							//(StackType_t*   )AppTaskCreate_Stack,	//任务堆栈
							(TaskHandle_t*  )&AppTaskCreate_Handle);	//任务控制块   
															
	if(pdPASS == xReturn)/* 创建成功 */
	vTaskStartScheduler();   /* 启动任务，开启调度 */

	while(1);   /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

	taskENTER_CRITICAL();           //进入临界区

	/* 创建LED_Task任务 */
	xReturn = xTaskCreate((TaskFunction_t	)LED_Task,		//任务函数
							(const char* 	)"LED_Task",		//任务名称
							(uint32_t 		)128,					//任务堆栈大小
							(void* 		  	)NULL,				//传递给任务函数的参数
							(UBaseType_t 	)4, 				//任务优先级
							//(StackType_t*   )LED_Task_Stack,	//任务堆栈
							(TaskHandle_t*  )&LED_Task_Handle);	//任务控制块   
							
	if(pdPASS == xReturn)/* 创建成功 */
		printf("LED_Task任务创建成功!\r\n");
	else
		printf("LED_Task任务创建失败!\r\n");
	
	/* 创建SD_Task任务 */
	xReturn = xTaskCreate((TaskFunction_t	)SD_Task,		//任务函数
							(const char* 	)"SD_Task",		//任务名称
							(uint32_t 		)128,					//任务堆栈大小
							(void* 		  	)NULL,				//传递给任务函数的参数
							(UBaseType_t 	)5, 				//任务优先级
							(TaskHandle_t*  )&SD_Task_Handle);	//任务控制块   
	if(pdPASS == xReturn)/* 创建成功 */
		printf("SD_Task任务创建成功!\r\n");
	else
		printf("SD_Task任务创建失败!\r\n");
   

	vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务

	taskEXIT_CRITICAL();            //退出临界区
}



/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void LED_Task(void* parameter)
{	
	/* 文件系统初始化 */
	FileSystem_Init();
  /* 文件系统读写测试 */
	FileSystem_Test();
    while (1)
    {
        LED1_ON;
        vTaskDelay(500);   /* 延时500个tick */
        //printf("LED_Task Running,LED1_ON\r\n");
        
        LED1_OFF;     
        vTaskDelay(500);   /* 延时500个tick */		 		
        //printf("LED_Task Running,LED1_OFF\r\n");
		//LCD_ShowString(30, 50, 200, 24, 24, (uint8_t *)"test!");
		//while(1);

    }
}

/**********************************************************************
  * @ 函数名  ： SD_Task
  * @ 功能说明： SD_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void SD_Task(void* parameter)
{	
    while (1)
    {
        //printf("\r\n开始进行SD卡读写实验\r\n");	
		//SD_Test();
			 		
        vTaskDelay(20000);   /* 延时500个tick */	

    }
}

/***********************************************************************
  * @ 函数名  ： BSP_Init
  * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_Init(void)
{
	uint16_t lcd_num;
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED 初始化 */
	LED_GPIO_Config();

	/* 串口初始化	*/
	Debug_USART_Config();
	
	lcd_num = LCD_Init();
	
	printf("LCD ID = %x\r\n", lcd_num);
	POINT_COLOR = RED;//设置字体为红色 
	LCD_ShowString(30, 50, 200, 24, 24, (uint8_t *)"test!");

	printf("Init finish\r\n");
}
 
