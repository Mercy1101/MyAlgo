#include <time.h>
#include <string.h>
#include "gos_get_localtime.h"

INT32 gos_get_timezone(VOID)
{
    time_t          ulTime = 24 * 3600;
    struct tm       stTime;

#ifdef WIN32
    memcpy(&stTime, localtime(&ulTime), sizeof(stTime));
#else
    localtime_r(&ulTime, &stTime);
#endif

    return (INT32)((stTime.tm_mday - 1) * 24 * 3600 + stTime.tm_hour * 3600 - ulTime);
}

/*
* @name               : gos_get_localtime
* @function           : 在pulCurrTime不为NULL时，pstTime传出相应的时间结构体。
                        在pulCurrTime为NULL时，pstTime传出当前时间的时间结构体。
* @globalparam        : NONE
* @classparam         : NONE
* @database           : NONE
* @return             :
* @author             : Lijiancong, 316, lijiancong@fritt.com.cn
* @createtime         : 2019-05-21 15:14:00
* @note               :
*/
BOOL gos_get_localtime(UINT32           *pulCurrTime,   // [in]
    GOS_DATETIME_T   *pstTime        // [out]
)
{
    static UINT32           bInit = FALSE;
    static UINT32           ulSecPerDay = 24 * 3600;
    static GOS_DATETIME_T   astDayInfo[(2038 - 1970) * 365];
    static GOS_DATETIME_T   astSecInfo[24 * 3600];
    UINT32                  ulDay;
    UINT32                  ulSec;
    UINT32                  ulCurrTime;
    static INT32            iTimeZone = 0x7fffffff;
    UINT32                  i;

    if (!bInit)
    {
        time_t      ulStartTime = 0;
        struct tm   stTime;
        time_t      ulTime;

        bInit = TRUE;

        iTimeZone = gos_get_timezone();

        memset(astDayInfo, 0, sizeof(astDayInfo));
        for (i = 0; i < ARRAY_SIZE(astDayInfo); i++)
        {
#ifdef WIN32
            memcpy(&stTime, localtime(&ulStartTime), sizeof(stTime));
#else
            localtime_r(&ulStartTime, &stTime);
#endif
            astDayInfo[i].usYear = stTime.tm_year + 1900;
            astDayInfo[i].ucMonth = stTime.tm_mon + 1;
            astDayInfo[i].ucDay = stTime.tm_mday;
            ulStartTime += ulSecPerDay;
        }

        memset(astSecInfo, 0, sizeof(astSecInfo));
        for (i = 0; i < ARRAY_SIZE(astSecInfo); i++)
        {
            ulTime = 24 * 3600 + i;
            memcpy(&stTime, gmtime((time_t*)&ulTime), sizeof(stTime));
            astSecInfo[i].ucHour = stTime.tm_hour;
            astSecInfo[i].ucMinute = stTime.tm_min;
            astSecInfo[i].ucSecond = stTime.tm_sec;
        }
    }

    if (NULL == pulCurrTime)
    {
        ulCurrTime = (UINT32)time(NULL) + iTimeZone;
    }
    else
    {
        ulCurrTime = pulCurrTime[0] + iTimeZone;
    }

    ulDay = ulCurrTime / ulSecPerDay;
    ulSec = ulCurrTime % ulSecPerDay;

    if (ulDay >= ARRAY_SIZE(astSecInfo))
    {
        return FALSE;
    }

    SET_INT(pstTime, GET_INT(&astDayInfo[ulDay]));
    SET_INT(&pstTime->ucHour, GET_INT(&astSecInfo[ulSec].ucHour));

    return TRUE;
}

BOOL gos_get_localtime_imporve(const time_t   * const    pulCurrTime,   // [in]
    GOS_DATETIME_IMPORVE_T   *pstTime        // [out]
)
{
    tm      *pstTimeTemp = nullptr;

    if (nullptr == pulCurrTime)
    {
        time_t  now = time(0);
        pstTimeTemp = localtime(&now);      // Current Time (UINTX time stamp)
    }
    else
    {
        pstTimeTemp = localtime(pulCurrTime);
    }

    pstTime->ulYear = pstTimeTemp->tm_year + 1900;
    pstTime->ulMonth = pstTimeTemp->tm_mon + 1;
    pstTime->ulDay = pstTimeTemp->tm_mday;
    pstTime->ulHour = pstTimeTemp->tm_hour;
    pstTime->ulMinute = pstTimeTemp->tm_min;
    pstTime->ulSecond = pstTimeTemp->tm_sec;

    return TRUE;
}