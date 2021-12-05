#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#define LOG_ENABLE

#define BLACK         "\033[30m"    /* 黑色 */
#define RED           "\033[31m"    /* 红色 */
#define GREEN         "\033[32m"    /* 绿色 */
#define YELLOW        "\033[33m"    /* 黄色 */
#define BLUE          "\033[34m"    /* 蓝色 */
#define MAGENTA       "\033[35m"    /* 紫色 */
#define CYAN          "\033[36m"    /* 青色 */
#define WHITE         "\033[37m"    /* 白色 */
#define DEFAULT       "\033[39m"    /* 默认 */
#define RESET         "\033[0m"     /* 重置 */

/* Define debug level */
#define TRACE    0
#define DEBUG    1
#define FATAL    2

/* Log macro define */
#ifdef LOG_ENABLE
#define Log(debug_level, format, ...)                                                                    \
            do                                                                                               \
            {                                                                                                \
                switch(debug_level)                                                                          \
                {                                                                                            \
                case TRACE:                                                                                  \
                    printf(format RESET, ##__VA_ARGS__);                                                           \
                    break;                                                                                   \
                case DEBUG:                                                                                  \
                    printf("[%s] "format"\r\n"RESET, __FUNCTION__, ##__VA_ARGS__);                                \
                    break;                                                                                   \
                case FATAL:                                                                                  \
                    printf("[%s, %s, %d] "format"\r\n"RESET, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);    \
                    break;                                                                                   \
                default:                                                                                     \
                    printf("[%s, %s, %d] "format"\r\n"RESET, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);    \
                    break;                                                                                   \
                }                                                                                            \
            }while(0)
#else
#define Log(debug_level, format, ...)
#endif

#endif    /* End define macro __COMMON_H__ */
