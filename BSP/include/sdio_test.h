#ifndef __FATFS_TEST_H__
#define __FATFS_TEST_H__

/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/


/* STM32 �̼���ͷ�ļ� */
#include "stm32f4xx.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"

/* ������Ӳ��bspͷ�ļ� */
#include "bsp_debug_usart.h"
#include "bsp_led.h"
//#include "bsp_key.h"
#include "ff.h"
//#include "FatFs_test.h"


/*
*************************************************************************
*                             ��������
*************************************************************************
*/


/*
*************************************************************************
*                               ��������
*************************************************************************
*/

void FileSystem_Init(void);

void FileSystem_Test(void);




#endif /* __FATFS_TEST_H__ */




