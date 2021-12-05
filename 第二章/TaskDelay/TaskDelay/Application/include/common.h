#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#define LOG_ENABLE

#define BLACK         "\033[30m"    /* ��ɫ */
#define RED           "\033[31m"    /* ��ɫ */
#define GREEN         "\033[32m"    /* ��ɫ */
#define YELLOW        "\033[33m"    /* ��ɫ */
#define BLUE          "\033[34m"    /* ��ɫ */
#define MAGENTA       "\033[35m"    /* ��ɫ */
#define CYAN          "\033[36m"    /* ��ɫ */
#define WHITE         "\033[37m"    /* ��ɫ */
#define DEFAULT       "\033[39m"    /* Ĭ�� */
#define RESET         "\033[0m"     /* ���� */

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
