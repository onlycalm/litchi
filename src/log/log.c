/**
 * @file log.c
 * @brief log模块。
 * @details 无
 * @author Calm
 * @date 2021-08-11
 * @version v1.0.0
 * @copyright Calm
 */

#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>  // for mkdir on Unix-like systems
#include "com.h"
#include "log.h"
#include "typ.h"
#include "err.h"

#ifdef LOG_H

#define TIME_BUFFER_SIZE 31 // "YYYY-MM-DD HH:MM:SS.MMMUUU \0"
#define PTH_MAX_SIZE     256u

FILE* pfLogFile = NULL;

const char* pcGetTmStmp(void)
{
    static char acTmStr[TIME_BUFFER_SIZE]; // 使用静态数组
    struct timespec tTmSpec = {0};
    struct tm* ptTmInfo = {0};
    char* pcTmStr = NULL;

    if(clock_gettime(CLOCK_REALTIME, &tTmSpec) != -1)
    {
        ptTmInfo = localtime(&tTmSpec.tv_sec);
        strftime(acTmStr, sizeof(acTmStr), "%Y-%m-%d %H:%M:%S", ptTmInfo);
        snprintf(acTmStr + strlen(acTmStr),
                 sizeof(acTmStr) - strlen(acTmStr),
                 ".%06ld ", (long)(tTmSpec.tv_nsec / 1000));
        pcTmStr = acTmStr;
    }
    else
    {
        pcTmStr = NULL;
    }

    return pcTmStr;
}

err erIsVldPth(const char* pcPth)
{
    const char* invalid_chars = "<>\"|?*";
    const char* pcChr = NULL;
    BOOL bInvChr = FALSE;
    err erRtn = EC_NOK;

    if((pcPth != NULL) && (strlen(pcPth) > 0))
    {
        for(pcChr = pcPth; (bInvChr == FALSE) && (*pcChr != '\0'); pcChr++)
        {
            if(strchr(invalid_chars, *pcChr))
            {
                bInvChr = TRUE;
            }
        }

        if(bInvChr == FALSE)
        {
            LogTr("Valid path.");

            erRtn = EC_OK;
        }
        else
        {
            LogErr("Invalid path.");

            erRtn = EC_NOK;
        }
    }
    else
    {
        LogErr("Invalid function parameter.");

        erRtn = EC_NOK;
    }

    return erRtn;
}

/**
 * @fn char* pcGetFileNm(const char* const cpPath)
 * @brief 获取全路径的文件名。
 * @details 通过查找最后一个匹配的\符号来截取文件名。
 * @param[in] cpPath 文件全路径指针。
 * @return 文件名字符串指针。
 * @attention 该函数只适用于Linux平台，Windows平台需要将 '/' 改为 '\\' 。
 */
char* pcGetFileNm(const char* const cpPath)
{
    char* pcFileNm = NULL;

    pcFileNm = strrchr(cpPath, '/');

    if(pcFileNm)
    {
        pcFileNm = pcFileNm + 1u;
    }
    else
    {
        pcFileNm = (char*)cpPath;
    }

    return pcFileNm;
}

err erCrtDir(const char* pcPth)
{
    char acPth[PTH_MAX_SIZE] = {0};
    const char* pcChr = NULL;
    u16 u16PthSz = 0u;
    struct stat st = {0};
    err erRtn = EC_NOK;

    if((pcPth != NULL) && (strlen(pcPth) > 0) && (erIsVldPth(pcPth) == EC_OK))
    {
        LogInf("Directory path is %s", pcPth);

        if(stat(pcPth, &st) == 0)
        {
            if(S_ISDIR(st.st_mode))
            {
                LogTr("The path already exists.");

                erRtn = EC_OK;
            }
            else
            {
                LogErr("The path already exists but is not a directory.");

                erRtn = EC_NOK;
            }
        }
        else
        {
            if(errno == ENOENT)
            {
                LogTr("The path does not exist, create a new path.");

                u16PthSz = strlen(pcPth);

                for(pcChr = pcPth + 1; (pcChr - pcPth) <= u16PthSz; pcChr++)
                {
                    // if(*pcChr == '/')
                    if((*pcChr == '/') || (*pcChr == '\0'))
                    {
                        // 构建当前目录路径
                        strncpy(acPth, pcPth, pcChr - pcPth);
                        acPth[pcChr - pcPth] = '\0';
                        LogInf("Create directory %s", acPth);

                        if(stat(acPth, &st) == 0)
                        {
                            LogTr("The directory already exists.");
                        }
                        else
                        {
                            if(errno == ENOENT)
                            {
                                LogTr("create a new directory");

                                if(mkdir(acPth, 0755) == 0)
                                {
                                    LogScs("Directory [%s] created successfully", acPth);
                                }
                                else
                                {
                                    erRtn = EC_NOK;
                                }
                            }
                            else
                            {
                                LogErr("Directory access failed.");

                                erRtn = EC_NOK;
                            }
                        }
                    }
                }

                erRtn = EC_OK;
            }
            else
            {
                LogErr("Path access failed.");

                erRtn = EC_NOK;
            }
        }
    }
    else
    {
        LogErr("Invalid function parameter.");

        erRtn = EC_NOK;
    }

    LogTr("Exit erCrtDir function.");

    return erRtn;
}

err erInitLog(const char* pcFilePth)
{
    LogTr("Enter erInitLog function.");

    char acPth[PTH_MAX_SIZE] = {0};
    char* pcLstSlash = NULL;
    u16 u16FilePthSz = 0u;
    err erRtn = EC_NOK;

    if(pcFilePth != NULL)
    {
        LogInf("Log file path is %s", pcFilePth);

        u16FilePthSz = strlen(pcFilePth);

        if((u16FilePthSz > 0) &&
           (u16FilePthSz <= PTH_MAX_SIZE) &&
           (erIsVldPth(pcFilePth) == EC_OK))
        {
            snprintf(acPth, PTH_MAX_SIZE, "%s", pcFilePth);
            LogInf("Log directory path is %s", acPth);
            pcLstSlash = strrchr(acPth, '/');

            if(pcLstSlash != NULL)
            {
                *pcLstSlash = '\0';
            }
            else
            {
                strcpy(acPth, ".");
            }

            if(erCrtDir(acPth) == EC_OK)
            {
                pfLogFile = fopen(pcFilePth, "a");

                if(pfLogFile != NULL)
                {
                    LogScs("The log file has been successfully opened.");

                    erRtn = EC_OK;
                }
                else
                {
                    LogErr("Failed to open log file.");

                    erRtn = EC_NOK;
                }
            }
            else
            {
                LogErr("Log directory verification failed.");

                erRtn = EC_NOK;
            }
        }
        else
        {
            LogErr("Log path check error.");

            erRtn = EC_NOK;
        }
    }
    else
    {
        LogErr("Invalid function parameter.");

        erRtn = EC_NOK;
    }

    LogTr("Exit erInitLog function.");

    return erRtn;
}

void vidClsLog(void)
{
    if(pfLogFile != NULL)
    {
        fclose(pfLogFile);
    }
}

// 自定义的日志函数
void vidLogPrintf(const char* pcFmt, ...)
{
    va_list args;
    va_start(args, pcFmt);

    // vprintf(pcFmt, args);

    if(pfLogFile != NULL)
    {
        vfprintf(pfLogFile, pcFmt, args);
        fflush(pfLogFile);
    }

    va_end(args);
}

#endif //LOG_H
