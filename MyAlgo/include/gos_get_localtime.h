#ifndef MYALGO_INCLUDE_GOS_GET_LOCALTIME_H
#define MYALGO_INCLUDE_GOS_GET_LOCALTIME_H

#include "gos_typedef.h"

struct GOS_DATETIME_T
{
    UINT16              usYear;     /* 四位数 */
    UINT8               ucMonth;    /* 1-12 */
    UINT8               ucDay;      /* 1-31 */

    UINT8               ucHour;     /* 0-23 */
    UINT8               ucMinute;   /* 0-59 */
    UINT8               ucSecond;   /* 0-59 */
    UINT8               ucSn;
};

struct GOS_DATETIME_IMPORVE_T
{
    UINT32               ulYear;     /* 四位数 */
    UINT32               ulMonth;    /* 1-12 */
    UINT32               ulDay;      /* 1-31 */

    UINT32               ulHour;     /* 0-23 */
    UINT32               ulMinute;   /* 0-59 */
    UINT32               ulSecond;   /* 0-59 */
    UINT32               ulSn;
};

BOOL gos_get_localtime(UINT32 *pulCurrTime, GOS_DATETIME_T *pstTime);

BOOL gos_get_localtime_imporve(const time_t   * const    pulCurrTime,   // [in]
                               GOS_DATETIME_IMPORVE_T   *pstTime        // [out]
                               );

#endif