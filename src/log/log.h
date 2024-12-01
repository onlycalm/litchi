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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "com.h"

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//配置宏
#ifndef LOG
#define LOG ENABLE //!< Log模块开关。
#endif // LOG

#ifndef LOG_LV
#define LOG_LV LOG_TRACE //!<默认Log等级。
#endif // LOG_LV

#ifndef LOG_FILE
#define LOG_FILE ENABLE
#endif // LOG_FILE

#ifndef LOG_TM_STMP
#define LOG_TM_STMP ENABLE //!< Log timestamp switch.
#endif // LOG_TM_STMP

#ifndef LOG_STY
#define LOG_STY ENABLE
#endif // LOG_STY

#ifndef LOG_FNT
#define LOG_FNT ENABLE
#endif // LOG_FNT

#ifndef LOG_FNT_CLR
#define LOG_FNT_CLR ENABLE
#endif // LOG_FNT_CLR

#ifndef LOG_BG_CLR
#define LOG_BG_CLR ENABLE
#endif // LOG_BG_CLR

//=============================================================================
//Log等级
#define LOG_CRITICAL             50u                 //!<致命。
#define LOG_ERROR                40u                 //!<错误。
#define LOG_WARNING              30u                 //!<警告。
#define LOG_SUCCESS              25u                 //!<成功。
#define LOG_INFO                 20u                 //!<信息。
#define LOG_DEBUG                10u                 //!<调试。
#define LOG_TRACE                5u                  //!<跟踪。

//=============================================================================
#define LOG_STY_RESET       "\033[0m"

// Font.
#define LOG_FNT_DFLT        0
#define LOG_FNT_BOLD        1
#define LOG_FNT_UNDERLINE   4
#define LOG_FNT_BLINK       5
#define LOG_FNT_REVERSE     7
#define LOG_FNT_HIDDEN      8

// Front Color.
#define LOG_FNT_CLR_DFLT    39
#define LOG_FNT_CLR_BLACK   30
#define LOG_FNT_CLR_GREY    90
#define LOG_FNT_CLR_RED     91
#define LOG_FNT_CLR_GREEN   92
#define LOG_FNT_CLR_YELLOW  93
#define LOG_FNT_CLR_BLUE    94
#define LOG_FNT_CLR_MAGENTA 95
#define LOG_FNT_CLR_CYAN    96
#define LOG_FNT_CLR_WHITE   37

// Background color.
#define LOG_BG_CLR_DFLT    49
#define LOG_BG_CLR_BLACK   40
#define LOG_BG_CLR_GREY    100
#define LOG_BG_CLR_RED     101
#define LOG_BG_CLR_GREEN   102
#define LOG_BG_CLR_YELLOW  103
#define LOG_BG_CLR_BLUE    104
#define LOG_BG_CLR_MAGENTA 105
#define LOG_BG_CLR_CYAN    106
#define LOG_BG_CLR_WHITE   47

#if LOG_FNT == ENABLE
#define LOG_FNT_CRITICAL LOG_FNT_DFLT
#define LOG_FNT_ERROR    LOG_FNT_DFLT
#define LOG_FNT_WARNING  LOG_FNT_DFLT
#define LOG_FNT_SUCCESS  LOG_FNT_DFLT
#define LOG_FNT_INFO     LOG_FNT_DFLT
#define LOG_FNT_DEBUG    LOG_FNT_DFLT
#define LOG_FNT_TRACE    LOG_FNT_DFLT
#else
#define LOG_FNT_CRITICAL LOG_FNT_DFLT
#define LOG_FNT_ERROR    LOG_FNT_DFLT
#define LOG_FNT_WARNING  LOG_FNT_DFLT
#define LOG_FNT_SUCCESS  LOG_FNT_DFLT
#define LOG_FNT_INFO     LOG_FNT_DFLT
#define LOG_FNT_DEBUG    LOG_FNT_DFLT
#define LOG_FNT_TRACE    LOG_FNT_DFLT
#endif // LOG_FNT == ENABLE

#if LOG_FNT_CLR == ENABLE
#define LOG_FNT_CLR_CRITICAL LOG_FNT_CLR_MAGENTA
#define LOG_FNT_CLR_ERROR    LOG_FNT_CLR_RED
#define LOG_FNT_CLR_WARNING  LOG_FNT_CLR_YELLOW
#define LOG_FNT_CLR_SUCCESS  LOG_FNT_CLR_GREEN
#define LOG_FNT_CLR_INFO     LOG_FNT_CLR_WHITE
#define LOG_FNT_CLR_DEBUG    LOG_FNT_CLR_BLUE
#define LOG_FNT_CLR_TRACE    LOG_FNT_CLR_GREY
#else
#define LOG_FNT_CLR_CRITICAL LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_ERROR    LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_WARNING  LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_SUCCESS  LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_INFO     LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_DEBUG    LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_TRACE    LOG_FNT_CLR_DFLT
#endif // LOG_FNT_CLR == ENABLE

#if LOG_BG_CLR == ENABLE
#define LOG_BG_CLR_CRITICAL LOG_BG_CLR_DFLT
#define LOG_BG_CLR_ERROR    LOG_BG_CLR_DFLT
#define LOG_BG_CLR_WARNING  LOG_BG_CLR_DFLT
#define LOG_BG_CLR_SUCCESS  LOG_BG_CLR_DFLT
#define LOG_BG_CLR_INFO     LOG_BG_CLR_DFLT
#define LOG_BG_CLR_DEBUG    LOG_BG_CLR_DFLT
#define LOG_BG_CLR_TRACE    LOG_BG_CLR_DFLT
#else
#define LOG_BG_CLR_CRITICAL LOG_BG_CLR_DFLT
#define LOG_BG_CLR_ERROR    LOG_BG_CLR_DFLT
#define LOG_BG_CLR_WARNING  LOG_BG_CLR_DFLT
#define LOG_BG_CLR_SUCCESS  LOG_BG_CLR_DFLT
#define LOG_BG_CLR_INFO     LOG_BG_CLR_DFLT
#define LOG_BG_CLR_DEBUG    LOG_BG_CLR_DFLT
#define LOG_BG_CLR_TRACE    LOG_BG_CLR_DFLT
#endif // LOG_BG_CLR == ENABLE

#if LOG_STY == ENABLE
#define LOG_STY_CRITICAL "\033[" pcToStr(LOG_FNT_CRITICAL) ";"\
                                 pcToStr(LOG_FNT_CLR_CRITICAL)  ";"\
                                 pcToStr(LOG_BG_CLR_CRITICAL) "m"
#define LOG_STY_ERROR "\033[" pcToStr(LOG_FNT_ERROR) ";"\
                              pcToStr(LOG_FNT_CLR_ERROR)  ";"\
                              pcToStr(LOG_BG_CLR_ERROR) "m"
#define LOG_STY_WARNING "\033[" pcToStr(LOG_FNT_WARNING) ";"\
                                pcToStr(LOG_FNT_CLR_WARNING)  ";"\
                                pcToStr(LOG_BG_CLR_WARNING) "m"
#define LOG_STY_SUCCESS "\033[" pcToStr(LOG_FNT_SUCCESS) ";"\
                                pcToStr(LOG_FNT_CLR_SUCCESS)  ";"\
                                pcToStr(LOG_BG_CLR_SUCCESS) "m"
#define LOG_STY_INFO "\033[" pcToStr(LOG_FNT_INFO) ";"\
                             pcToStr(LOG_FNT_CLR_INFO)  ";"\
                             pcToStr(LOG_BG_CLR_INFO) "m"
#define LOG_STY_DEBUG "\033[" pcToStr(LOG_FNT_DEBUG) ";"\
                              pcToStr(LOG_FNT_CLR_DEBUG)  ";"\
                              pcToStr(LOG_BG_CLR_DEBUG) "m"
#define LOG_STY_TRACE "\033[" pcToStr(LOG_FNT_TRACE) ";"\
                              pcToStr(LOG_FNT_CLR_TRACE)  ";"\
                              pcToStr(LOG_BG_CLR_TRACE) "m"
#else
#define LOG_STY_CRITICAL
#define LOG_STY_ERROR
#define LOG_STY_WARNING
#define LOG_STY_SUCCESS
#define LOG_STY_INFO
#define LOG_STY_DEBUG
#define LOG_STY_TRACE
#endif // LOG_STY == ENABLE

//-----------------------------------------------------------------------------
//宏函数
#if LOG == ENABLE
#if LOG_TM_STMP == ENABLE
#define FmtLog(Lv, Str) "%s" "[" Lv "] %s:%s:%d - " Str LOG_STY_RESET "\n",\
                        pcGetTmStmp(), pcGetFileNm(__FILE__), __FUNCTION__, __LINE__
#else
#define FmtLog(Lv, Str) "[" Lv "] %s:%s:%d - " Str LOG_STY_RESET "\n",\
                        pcGetFileNm(__FILE__), __FUNCTION__, __LINE__
#endif // ENABLE

#if LOG_FILE == ENABLE
#define vidLog(Str, ...) \
    do { \
        printf(Str, ##__VA_ARGS__); \
        vidLogPrintf(Str, ##__VA_ARGS__); \
    } while (0u)
#else
#define vidLog(Str, ...) \
    do { \
        printf(Str, ##__VA_ARGS__); \
    } while (0u)
#endif // LOG_FILE

#if LOG_LV <= LOG_CRITICAL
#define LogCrt(Str, ...) vidLog(LOG_STY_CRITICAL FmtLog("CRITICAL", Str), ##__VA_ARGS__)
#else
#define LogCrt(Str, ...) ((void)0u)
#endif // LOG_LV

#if LOG_LV <= LOG_ERROR
#define LogErr(Str, ...) vidLog(LOG_STY_ERROR FmtLog("ERROR", Str) , ##__VA_ARGS__)
#else
#define LogErr(Str, ...) ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_WARNING
#define LogWrn(Str, ...) vidLog(LOG_STY_WARNING FmtLog("WARNING", Str), ##__VA_ARGS__)
#else
#define LogWrn(Str, ...) ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_SUCCESS
#define LogScs(Str, ...) vidLog(LOG_STY_SUCCESS FmtLog("SUCCESS", Str), ##__VA_ARGS__)
#else
#define LogScs(Str, ...) ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_INFO
#define LogInf(Str, ...) vidLog(LOG_STY_INFO FmtLog("INFO", Str), ##__VA_ARGS__)
#else
#define LogInf(Str, ...) ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_DEBUG
#define LogDbg(Str, ...) vidLog(LOG_STY_DEBUG FmtLog("DEBUG", Str), ##__VA_ARGS__)
#else
#define LogDbg(Str, ...) ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_TRACE
#define LogTr(Str, ...) vidLog(LOG_STY_TRACE FmtLog("TRACE", Str), ##__VA_ARGS__)
#else
#define LogTr(Str, ...) ((void)0u)
#endif //LOG_LV
#else
#define FmtLog(Lv, Str)  ((void)0u)
#define LogCrt(Str, ...) ((void)0u)
#define LogErr(Str, ...) ((void)0u)
#define LogWrn(Str, ...) ((void)0u)
#define LogScs(Str, ...) ((void)0u)
#define LogInf(Str, ...) ((void)0u)
#define LogDbg(Str, ...) ((void)0u)
#define LogTr(Str, ...)  ((void)0u)
#endif //LOG

extern const char* pcGetTmStmp(void);
extern char* pcGetFileNm(const char* const cpPath);
extern err erInitLog(const char* pcFilePth);
extern void vidClsLog(void);
extern void vidLogPrintf(const char* pcFmt, ...);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //LOG_H
