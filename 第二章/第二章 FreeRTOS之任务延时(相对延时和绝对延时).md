# 第二章 FreeRTOS之任务延时（相对延时和绝对延时）

[TOC]

### （1）认识延时函数

FreeRTOS操作系统提供了两种延时函数，它们分别是相对延时和绝对延时功能。

a）、相对延时的函数原型如下：

```c
void vTaskDelay( TickType_t xTicksToDelay );
```

参数xTicksToDelay表示的是系统时钟节拍，它是从调用vTaskDelay开始记时钟节拍，一直到设定的参数值为止，期间被调用的任务处于阻塞状态。比如xTicksToDelay的值为1000，那么从调用vTaskDelay开始，任务处于阻塞状态，一直到系统时钟节拍为1000为止；需要注意的是调用vTaskDelay期间，任务可被打断。

b）、绝对延时函数的原型如下：

```c
void vTaskDelayUntil( TickType_t *pxPreviousWakeTime, TickType_t xTimeIncrement );
```

参数pxPreviousWakeTime是一个唤醒时间的参考点，它记录的是调用vTaskDelayUntil函数时的任务的阻塞时间，它会自动更新。

参数xTimeIncrement表示的是任务下一次唤醒时的时间。比如设定100ms后唤醒任务，那么任务便会在100ms后被唤醒。

c）、时钟节拍转换毫秒宏定义pdMS_TO_TICKS

vTaskDelay和vTaskDelayUntil函数中的时间参数单位是时钟节拍，指的是系统时钟计数1的时间。在实际使用中，往往需要的是秒数，所以就需要j将使用时钟节拍转换毫秒的宏pdMS_TO_TICKS，比如使用vTaskDelay延时1秒，那么写成vTaskDelay(pdMS_TO_TICKS(1000))即可。

### （2）相对延时和绝对延时使用区别

由上面的介绍可以看出：

​	a）、相对延时函数vTaskDelay依赖的是系统时钟节拍计数，当在计数的过程中，如果被其它中断打断，那么实际需要耗费的时间是比设定的时间多的，也就是说相对延时不能保证精确的延时。它常用于任务对延时不要求非常准确的场景中。

​	b）、绝对延时函数vTaskDelayUntil由于第一个参数记录的是上一次任务阻塞的时间，第二个参数时下一次任务被唤醒的时间，中间是不管是否被打算的，到达设定的时间任务就会被唤醒。因此可以认为vTaskDelayUntil定时是准确的。它常用于周期性执行的任务中。

### （3）使用示例

```c
/* 包含FreeRTOS的头文件 */
#include "FreeRTOS.h"
#include "task.h"

/* 包含需要支持的头文件 */
#include "supporting_functions.h"
#include "common.h"
#include "windows.h"

/* 获取系统时间 */
static char* strDateTimeGet(void)
{
    static char strDateTime[64] = {0};
    SYSTEMTIME st;

    /* 获取系统当前时间 */
    GetLocalTime(&st);

    sprintf(strDateTime, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    return strDateTime;
}

/* 相对延时任务1 */
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

        /* 相对延时1秒 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* 绝对延时任务2 */
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
        
        /* 绝对延时1秒 */
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }
}

/*-----------------------------------------------------------*/

int main(void)
{
    /* 创建任务1 */
    BaseType_t task_status = xTaskCreate(vTask1,      /* 任务函数指针 */
                                         "vTask1",    /* 任务函数名称 */
                                         1000,        /* 给任务函数分配的堆栈空间大小 */
                                         NULL,        /* 给任务函数传递的参数 */
                                         1,           /* 给任务函数分配的优先级 */
                                         NULL);       /* 任务函数的句柄，调用时可用 */
    if (task_status != pdPASS)
    {
        Log(FATAL, RED"Create vTask1 failed.");
    }

    /* 创建任务2 */
    task_status = xTaskCreate(vTask2, "vTask2", 1000, NULL, 1, NULL);
    if (task_status != pdPASS)
    {
        Log(FATAL, RED"Create vTask2 failed.");
    }

    /* 启动调度器以启动所创建的任务. */
    vTaskStartScheduler();

    return 0;
}
/*-----------------------------------------------------------*/
```

程序运行演示：

![1](D:\FreeRTOS-Tutorial\第二章\1.png)