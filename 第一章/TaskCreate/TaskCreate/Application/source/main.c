/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* ����FreeRTOS��ͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"

/* ������Ҫ֧�ֵ�ͷ�ļ� */
#include "supporting_functions.h"
#include "common.h"

const char* pcTextForTask1 = "Task 1 is running\r\n";    /* ����1���� */
const char* pcTextForTask2 = "Task 2 is running\r\n";    /* ����1���� */

typedef struct
{
    const char* name;
    uint8_t age;
} person_t;

/* ����1������2��ͬ��ʵ�� */
void vTask12(void* pvParameters)
{
    /* ��ȡ���񴴽�ʱ���ݵĲ��� */
    const char* pcTaskName = (char*)pvParameters;

    /* ��������ѭ���еĴ����һֱִ�У�ֱ���������� */
    while (1)
    {
        /* ��ӡ����ǰ��������� */
        Log(DEBUG, WHITE"%s", pcTaskName);

        /* ��ʱ1�� */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vTask3(void* pvParameters)
{
    uint32_t value = (uint32_t)pvParameters;

    while (1)
    {
        Log(DEBUG, BLUE"value = %d\n", value);

        /* ��ʱ1�� */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vTask4(void* pvParameters)
{
    person_t* p = (person_t*)pvParameters;

    while (1)
    {
        Log(DEBUG, MAGENTA"name: %s, age: %hhu\n", p->name, p->age);

        /* ��ʱ1�� */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vTask5(void* pvParameters)
{
    char* buffer = (char*)pvPortMalloc(4096);

    while (1)
    {
        Log(DEBUG, CYAN"test_task_3");

        /* ��ʱ1�� */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*-----------------------------------------------------------*/

int main(void)
{
    /* ��������1 */
    xTaskCreate(vTask12,                /* ������ָ�� */
                "vTask1",               /* ���������� */
                1000,                   /* ������������Ķ�ջ�ռ��С */
                (void*)pcTextForTask1,  /* �����������ݵĲ��� */
                1,                      /* ����������������ȼ� */
                NULL);                  /* �������ľ��������ʱ���� */

    /* ��������2��ʵ�ֺ�����1��ͬ */
    xTaskCreate(vTask12, "vTask2", 1000, (void*)pcTextForTask2, 1, NULL);

    /* �����޷����������� */
    uint32_t value = 1024;
    BaseType_t create_success = xTaskCreate(vTask3, "vTask3", 1000, (void*)value, 1, NULL);
    if (create_success != pdPASS)
    {
        Log(FATAL, RED"Create vTask3 failed.");
    }

    /* ���ݽṹ������ */
    const person_t person =
    {
        .name = "Candy",
        .age = 18,
    };
    create_success = xTaskCreate(vTask4, "vTask4", 1000, (void*)&person, 1, NULL);
    if (create_success != pdPASS)
    {
        Log(FATAL, RED"Create vTask3 failed.");
    }

#if 0
    /* ���Դ�������ʧ����� */
    create_success = xTaskCreate(vTask5, "vTask5", 128, NULL, 1, NULL);
    if (create_success != pdPASS)
    {
        Log(FATAL, RED"Create vTask5 failed.");
    }
#endif

    /* ����������������������������. */
    vTaskStartScheduler();

    return 0;
}
/*-----------------------------------------------------------*/

