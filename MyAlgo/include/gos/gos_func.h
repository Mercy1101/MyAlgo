#ifndef  INCLUDE_GOS_FUNC_H
#define  INCLUDE_GOS_FUNC_H

#include "gos_typedef.h"

#if 0
typedef struct _SYSTEMTIME
{
    UINT16 wYear;
    UINT16 wMonth;
    UINT16 wDayOfWeek;
    UINT16 wDay;
    UINT16 wHour;
    UINT16 wMinute;
    UINT16 wSecond;
    UINT16 wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
#endif

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
int gos_itoa(unsigned ulDigit, char *szDigit);

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
                             );

/**
* @name                 gos_get_rand
* @brief                获取并返回随机数
* @param                NONE
* @return               获取的随机数
* @author               未知
* @date                 2019-06-19 14:50:29

* @warning
Lijaincong: 该函数可能不具有线程安全性。
因为该函数设计思路为在程序开启时以当前UNIX时间戳，随机数种子。
在以后每一次调用该函数都会使用该随机数种子生成随机数。
当程序刚刚启动时如果有两个线程同时第一次调用该函数可能会生成两个一样的随机数。

* @note
建议：不在程序或线程刚启动时调用该函数。
*/
unsigned gos_get_rand();

/**
* @name                 gos_get_range_rand
* @brief                获取一个特定范围的随机数，范围为[uMin, uMax]
* @param                ulMin   [in]
* @param                ulMax   [in]
* @return               产生的随机数
* @author               未知
* @date                 2019-06-19 15:06:22

* @warning
由于该函数内也使用了srand（初始化随机数种子函数）,
所以该函数和gos_get_rand函数的随机数种子为一样的（都是程序启动时间作为种子），
种子一样产生的随机数也是一样，但测试一下好像生成的随机数不一样，多线程测试也不一样。
但仍然推荐先使用gos_get_rand生成随机数，然后取范围。

* @note
Lijiancong: modifytime: 2019-06-19 16:04:51
改动内容为从使用rand()生成随机数改为使用gos_get_rand产生随机数。
*/
unsigned gos_get_range_rand(unsigned ulMin, unsigned ulMax);

/**
* @name                gos_get_gmtime
* @brief               获取当前UTC时间（比北京时间晚八小时）即 1970 年 1 月 1 日 00:00 到现在的秒数
* @param               NONE
* @return              当前UTC时间的UNIX时间戳
* @author              Lijiancong, 316, lijiancong@fritt.com.cn
* @date                2019-06-20 10:28:35

* @warning
1) 函数中调用的gmtime函数在C++11中被推荐使用gmtime_s
2) 因为使用了该函数使用了 gmtime，该函数可能不是线程安全的。(标准中写的)

* @note
并不知道这个函数写来干嘛，直接使用gos_current_time()就行了。
*/
unsigned gos_get_gmtime();

/**
* @name                 gos_get_current_time
* @brief                获取当前时间距离1970/01/01 08:00:00(东八时区)的秒数
* @class                NONE
* @param                NONE
* @return               返回获取的秒数。
* @author               未知
* @date                 2019-06-25 14:24:54
*/
unsigned gos_get_current_time();

/**
* @name                gos_set_gmtime
* @brief               改变本地电脑的时间
* @param               ulSecond [in] 想要调整的时间
* @return              NONE
* @author              未知
* @date                2019-06-20 11:03:44

* @note
SetLocalTime函数定义在头文件<sysinfoapi.h>

* @warning
1. Lijiancong: 自己测试了一下，不知道为什么好像没能改变电脑时间
*/
void gos_set_gmtime(unsigned ulSecond);

/**
* @name                Binary_Search_Array
* @brief               二分查找auSearchArray数组中比ulBeginValue大于等于的第一个数值

* @param               ulBeginValue     [in] 查询的基准数值，返回数值要大于等于这个值
* @param               auSearchArray    [in] 要查询的数组
* @param               ulArraySize      [in] 传入该数组的大小

* @return              返回大于或等于ulBeginValue的数值
*   @retval            UINT_MAX     异常返回值。ulBeginValue大于第一万个质数的值即104729返回unsigned的最大值
*   @retval            其他         正常返回值

* @author              Lijiancong, 316, lijiancong@fritt.com.cn
* @date                2019-06-21 09:11:20

* @note
*/
const unsigned Binary_Search_Array(const unsigned ulBeginValue, const unsigned auSearchArray[], const unsigned ulArraySize);

/**
* @name                gos_get_next_prime
* @brief               获取大于等于ulBeginNumber数值的第一个质数
* @param               ulBeginNumber    获取质数的基准值

* @return              大于等于ulBeginNumber的第一个质数
*   @retval            UINT_MAX     异常返回值。ulBeginValue大于第一万个质数的值即104729返回unsigned的最大值
*   @retval            其他         正常返回值

* @author              Lijiancong, 316, lijiancong@fritt.com.cn
* @date                2019-06-21 09:21:39

* @note
*/
const unsigned gos_get_next_prime(const unsigned ulBeginNumber);


unsigned gos_get_prime_old(unsigned ulStart);

#endif