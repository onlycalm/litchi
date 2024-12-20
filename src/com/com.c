/**
 * @file com.c
 * @brief 通用文件。
 * @details None
 * @author Calm
 * @date 2021-10-14
 * @version v1.0.0
 * @copyright Calm
 */

#include <stdio.h>
#include "com.h"
#include "typ.h"

#ifdef COM_H

/*****************************************************************************
 *函数定义                                                                   *
 *****************************************************************************/
//=============================================================================
//全局函数
//-----------------------------------------------------------------------------
//普通函数
/**
 * @fn BOOL CmpBy(const byte* const cpbyDat1, const byte* const cpbyDat2, const word cwAmt)
 * @brief 比较byte。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] cpbyDat1 数组1。
 * @param[in] cpbyDat2 数组2。
 * @param[in] cwAmt 要比较的byte数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
BOOL CmpBy(const byte* const cpbyDat1, const byte* const cpbyDat2, const word cwAmt)
{
    BOOL bEq = FALSE;
    word wi = 0u;

    for(wi = 0u; wi < cwAmt; wi++)
    {
        if(cpbyDat1[wi] != cpbyDat2[wi])
        {
            break;
        }
    }

    if(wi == cwAmt)
    {
        bEq = TRUE;
    }

    return bEq;
}

/**
 * @fn BOOL CmpWd(const word* const cpwDat1, const word* const cpwDat2, const word cwAmt)
 * @brief 比较word。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] cpwDat1 数组1。
 * @param[in] cpwDat2 数组2。
 * @param[in] cwAmt 要比较的word数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
BOOL CmpWd(const word* const cpwDat1, const word* const cpwDat2, const word cwAmt)
{
    BOOL bEq = FALSE;
    word wi = 0u;

    for(wi = 0u; wi < cwAmt; wi++)
    {
        if(cpwDat1[wi] != cpwDat2[wi])
        {
            break;
        }
    }

    if(wi == cwAmt)
    {
        bEq = TRUE;
    }

    return bEq;
}

/**
 * @fn BOOL CmpDwd(const dword* const cpdwDat1, const dword* const cpdwDat2, const word cwAmt)
 * @brief 比较dword。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] cpdwDat1 数组1。
 * @param[in] cpdwDat2 数组2。
 * @param[in] cwAmt 要比较的dword数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
BOOL CmpDwd(const dword* const cpdwDat1, const dword* const cpdwDat2, const word cwAmt)
{
    BOOL bEq = FALSE;
    word wi = 0u;

    for(wi = 0u; wi < cwAmt; wi++)
    {
        if(cpdwDat1[wi] != cpdwDat2[wi])
        {
            break;
        }
    }

    if(wi == cwAmt)
    {
        bEq = TRUE;
    }

    return bEq;
}

u16 u16CvtEndn(const u16 ku16Dat)
{
    return ((ku16Dat >> 8U) & 0x00FFU) | ((ku16Dat << 8U) & 0xFF00U);
}

u32 u32CvtEndn(const u32 ku32Dat)
{
    return ((ku32Dat >> 24U) & 0x000000FFU) |
           ((ku32Dat >> 8U) & 0x0000FF00U) |
           ((ku32Dat << 8U) & 0x00FF0000U) |
           ((ku32Dat << 24U) & 0xFF000000U);
}

#endif //COM_H
