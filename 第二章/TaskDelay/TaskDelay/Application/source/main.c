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
#include "windows.h"

/* ��ȡϵͳʱ�� */
static char* strDateTimeGet(void)
{
    static char strDateTime[64] = {0};
    SYSTEMTIME st;

    /* ��ȡϵͳ��ǰʱ�� */
    GetLocalTime(&st);

    sprintf(strDateTime, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    return strDateTime;
}

/* �����ʱ����1 */
void vTask1(void* pvParameters)
{
    uint16_t uiTimeCount = 0;
    char* strDateTime = NULL;

    while (1)
    {
        strDateTime = strDateTimeGet();
        Log(DEBUG, BLUE"[%s] uiTimeCount: %d", strDateTime, uiTimeCount);

        if (uiTimeCount < 1024)
        {
            uiTimeCount++;
        }
        else
        {
            uiTimeCount = 0;
        }

        /* �����ʱ1�� */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* ������ʱ����2 */
static void vTask2(void* pvParameters)
{
    char ch = 'A';
    char* strDateTime = NULL;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        strDateTime = strDateTimeGet();
        if (ch < 'Z')
        {
            Log(DEBUG, MAGENTA"[%s] character: %c", strDateTime, ch);

            ch++;
        }
        else
        {
            Log(DEBUG, MAGENTA"[%s] character: %c", strDateTime, ch);

            ch = 'A';
        }
        
        /* ������ʱ1�� */
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }
}

/*-----------------------------------------------------------*/

int main(void)
{
    /* ��������1 */
    BaseType_t task_status = xTaskCreate(vTask1,      /* ������ָ�� */
                                         "vTask1",    /* ���������� */
                                         1000,        /* ������������Ķ�ջ�ռ��С */
                                         NULL,        /* �����������ݵĲ��� */
                                         1,           /* ����������������ȼ� */
                                         NULL);       /* �������ľ��������ʱ���� */
    if (task_status != pdPASS)
    {
        Log(FATAL, RED"Create vTask1 failed.");
    }

    /* ��������2 */
    task_status = xTaskCreate(vTask2, "vTask2", 1000, NULL, 1, NULL);
    if (task_status != pdPASS)
    {
        Log(FATAL, RED"Create vTask2 failed.");
    }

    /* ����������������������������. */
    vTaskStartScheduler();

    return 0;
}
/*-----------------------------------------------------------*/

