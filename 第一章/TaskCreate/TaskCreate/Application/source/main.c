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

/* 包含FreeRTOS的头文件 */
#include "FreeRTOS.h"
#include "task.h"

/* 包含需要支持的头文件 */
#include "supporting_functions.h"
#include "common.h"

const char* pcTextForTask1 = "Task 1 is running\r\n";    /* 任务1参数 */
const char* pcTextForTask2 = "Task 2 is running\r\n";    /* 任务1参数 */

typedef struct
{
    const char* name;
    uint8_t age;
} person_t;

/* 任务1和任务2共同的实现 */
void vTask12(void* pvParameters)
{
    /* 获取任务创建时传递的参数 */
    const char* pcTaskName = (char*)pvParameters;

    /* 以下无线循环中的代码会一直执行，直到任务被销毁 */
    while (1)
    {
        /* 打印出当前任务的名称 */
        Log(DEBUG, WHITE"%s", pcTaskName);

        /* 延时1秒 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vTask3(void* pvParameters)
{
    uint32_t value = (uint32_t)pvParameters;

    while (1)
    {
        Log(DEBUG, BLUE"value = %d\n", value);

        /* 延时1秒 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vTask4(void* pvParameters)
{
    person_t* p = (person_t*)pvParameters;

    while (1)
    {
        Log(DEBUG, MAGENTA"name: %s, age: %hhu\n", p->name, p->age);

        /* 延时1秒 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vTask5(void* pvParameters)
{
    char* buffer = (char*)pvPortMalloc(4096);

    while (1)
    {
        Log(DEBUG, CYAN"test_task_3");

        /* 延时1秒 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*-----------------------------------------------------------*/

int main(void)
{
    /* 创建任务1 */
    xTaskCreate(vTask12,                /* 任务函数指针 */
                "vTask1",               /* 任务函数名称 */
                1000,                   /* 给任务函数分配的堆栈空间大小 */
                (void*)pcTextForTask1,  /* 给任务函数传递的参数 */
                1,                      /* 给任务函数分配的优先级 */
                NULL);                  /* 任务函数的句柄，调用时可用 */

    /* 创建任务2，实现和任务1相同 */
    xTaskCreate(vTask12, "vTask2", 1000, (void*)pcTextForTask2, 1, NULL);

    /* 传递无符号整数任务 */
    uint32_t value = 1024;
    BaseType_t create_success = xTaskCreate(vTask3, "vTask3", 1000, (void*)value, 1, NULL);
    if (create_success != pdPASS)
    {
        Log(FATAL, RED"Create vTask3 failed.");
    }

    /* 传递结构体任务 */
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
    /* 测试创建任务失败情况 */
    create_success = xTaskCreate(vTask5, "vTask5", 128, NULL, 1, NULL);
    if (create_success != pdPASS)
    {
        Log(FATAL, RED"Create vTask5 failed.");
    }
#endif

    /* 启动调度器以启动所创建的任务. */
    vTaskStartScheduler();

    return 0;
}
/*-----------------------------------------------------------*/

