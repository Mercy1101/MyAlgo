#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include "gos_func.h"

/*
* @name               : gos_itoa
* @function           : 将输入的无符号整型转化为C字符串输出,工作在32位系统无符号整形所有值
* @class              : 
* @classparam         : NONE
* @database           : NONE
* @return             : 返回szDigit中的数字的长度，如‘1000’为4，‘10’为2
* @author             : Leijie
* @createtime         : 2019-05-10 10:25:20
* @note               : szDigit:如为字符数组，则长度最少为11位，否则有可能溢出(win32)。
*/
INT32 gos_itoa(const  UINT32 ulDigit,   // [in]
               CHAR *        szDigit    // [out]
               )
{
    static BOOL         bInit = FALSE;
    static UINT32       aulLen[100000] = {0};
    static UINT64       au64Value[100000] = {0};
    static UINT64       au64Value2[100000] = {0};
    UINT32              ulDigitLow = 0;
    UINT32              ulDigitHigh = 0;

    if (!bInit)
    {
        bInit = TRUE;

        for (UINT32 i = 0; i < ARRAY_SIZE(au64Value); i++)
        {
            aulLen[i] = sprintf((CHAR*)&au64Value[i], "%u", i);
            sprintf((CHAR*)&au64Value2[i], "%05u", i);
        }
    }

    ulDigitLow = MOD(ulDigit, 100000);
    ulDigitHigh = ulDigit / 100000;
    if (ulDigitHigh > 0)
    {
        SET_INT64(szDigit, au64Value[ulDigitHigh]);
        szDigit += aulLen[ulDigitHigh];
        memcpy(szDigit, &au64Value2[ulDigitLow], 6);
        return aulLen[ulDigitHigh] + 5;
    }
    else
    {
        memcpy(szDigit, &au64Value[ulDigitLow], aulLen[ulDigitLow] + 1);
        return aulLen[ulDigitLow];
    }
}

template<class BidirIt>
void reverse(BidirIt first, BidirIt last)
{
    while ((first != last) && (first != --last)) 
    {
        std::iter_swap(first++, last);
    }
}

/*
* @name               : gos_itoa_imporve
* @function           : 数字转换为C字符串，工作范围32位的int都可以转换(-2137483648 .. 2137483648)
* @globalparam        : NONE
* @classparam         : NONE
* @database           : NONE
* @return             : 返回转换后的字符串
* @author             : Lijiancong, 316, lijiancong@fritt.com.cn
* @createtime         : 2019-05-23 14:24:24
* @modifytime         : 2019-05-23 14:24:24
* @note               : 
*/
const char* gos_itoa_imporve(const int value,     // [in]
                             char      buf[]      // [out]
                             )
{
    static char digits[] = {'9','8','7','6','5','4','3','2','1','0','1','2','3','4','5','6','7','8','9'};
    static const char* zero = digits + 9;   // zero points to '0'

    // works for -2137483648 .. 2137483648
    int   i = value;
    char* p = buf;

    do
    {
        // lsd - least significant digits int 
        int lsd = i % 10;   // lsd 可能小于0
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0)
    {
        *p++ = '-';
    }

    *p = '\0';
    reverse(buf, p);
    /* p在这个时刻指向buf后面的空字符 */
    return buf; // p - buf 为整数长度
}