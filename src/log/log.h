/**
 * @file log.h
 * @brief log模块。
 * @details 无
 * @author Calm
 * @date 2021-08-11
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "com.h"

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//配置宏
#ifndef LOG
#define LOG                      ENABLE              //!<Log模块开关。
#endif //LOG

#ifndef LOG_LV
#define LOG_LV                   TRACE               //!<默认Log等级。
#endif //LOG_LV

//=============================================================================
//Log等级
#define LOG_CRITICAL             50u                 //!<致命。
#define LOG_ERROR                40u                 //!<错误。
#define LOG_WARNING              30u                 //!<警告。
#define LOG_SUCCESS              25u                 //!<成功。
#define LOG_INFO                 20u                 //!<信息。
#define LOG_DEBUG                10u                 //!<调试。
#define LOG_TRACE                5u                  //!<跟踪。
                                                     //
//=============================================================================
// Font.
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"
#define REVERSE     "\033[7m"
#define HIDDEN      "\033[8m"

// Front Color.
#define LOG_FNT_BLACK   "\033[30m"
#define LOG_FNT_GREY    "\033[90m"
#define LOG_FNT_RED     "\033[91m"
#define LOG_FNT_GREEN   "\033[92m"
#define LOG_FNT_YELLOW  "\033[93m"
#define LOG_FNT_BLUE    "\033[94m"
#define LOG_FNT_MAGENTA "\033[95m"
#define LOG_FNT_CYAN    "\033[96m"
#define LOG_FNT_WHITE   "\033[37m"

// Background color.
#define LOG_BG_BLACK   "\033[40m"
#define LOG_BG_GREY    "\033[100m"
#define LOG_BG_RED     "\033[101m"
#define LOG_BG_GREEN   "\033[102m"
#define LOG_BG_YELLOW  "\033[103m"
#define LOG_BG_BLUE    "\033[104m"
#define LOG_BG_MAGENTA "\033[105m"
#define LOG_BG_CYAN    "\033[106m"
#define LOG_BG_WHITE   "\033[47m"

//-----------------------------------------------------------------------------
//宏函数
#if LOG == ENABLE
#define FmtLog(Lv, Str)          "[" Lv "] %s:%s:%d - " Str RESET "\n", GetFileNm(__FILE__), __FUNCTION__, __LINE__

#if LOG_LV <= LOG_CRITICAL
#define LogCrt(Str, ...)         printf(LOG_FNT_MAGENTA FmtLog("CRITICAL", Str), ##__VA_ARGS__)
#else
#define LogCrt()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_ERROR
#define LogErr(Str, ...)         printf(LOG_FNT_RED FmtLog("ERROR", Str) , ##__VA_ARGS__)
#else
#define LogErr()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_WARNING
#define LogWrn(Str, ...)         printf(LOG_FNT_YELLOW FmtLog("WARNING", Str), ##__VA_ARGS__)
#else
#define LogWrn()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_SUCCESS
#define LogScs(Str, ...)         printf(LOG_FNT_GREEN FmtLog("SUCCESS", Str), ##__VA_ARGS__)
#else
#define LogScs()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_INFO
#define LogInf(Str, ...)         printf(LOG_FNT_WHITE FmtLog("INFO", Str), ##__VA_ARGS__)
#else
#define LogInf ()                ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_DEBUG
#define LogDbg(Str, ...)         printf(LOG_FNT_BLUE FmtLog("DEBUG", Str), ##__VA_ARGS__)
#else
#define LogDbg()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_TRACE
#define LogTr(Str, ...)          printf(LOG_FNT_GREY FmtLog("TRACE", Str), ##__VA_ARGS__)
#else
#define LogTr()                  ((void)0u)
#endif //LOG_LV

#else
#define FmtLog()                 ((void)0u)
#define LogCrt()                 ((void)0u)
#define LogErr()                 ((void)0u)
#define LogWrn()                 ((void)0u)
#define LogScs()                 ((void)0u)
#define LogInf()                 ((void)0u)
#define LogDbg()                 ((void)0u)
#define LogTr()                  ((void)0u)
#endif //LOG

#endif //LOG_H
