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
* @function           : ��������޷�������ת��ΪC�ַ������,������32λϵͳ�޷�����������ֵ
* @class              : 
* @classparam         : NONE
* @database           : NONE
* @return             : ����szDigit�е����ֵĳ��ȣ��确1000��Ϊ4����10��Ϊ2
* @author             : Leijie
* @createtime         : 2019-05-10 10:25:20
* @note               : szDigit:��Ϊ�ַ����飬�򳤶�����Ϊ11λ�������п������(win32)��
*/
int gos_itoa(unsigned ulDigit, char *szDigit);

/*
* @name               : gos_itoa_imporve
* @function           : ����ת��ΪC�ַ�����������Χ32λ��int������ת��(-2137483648 .. 2137483648)
* @globalparam        : NONE
* @classparam         : NONE
* @database           : NONE
* @return             : ����ת������ַ���
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
* @brief                ��ȡ�����������
* @param                NONE
* @return               ��ȡ�������
* @author               δ֪
* @date                 2019-06-19 14:50:29

* @warning
Lijaincong: �ú������ܲ������̰߳�ȫ�ԡ�
��Ϊ�ú������˼·Ϊ�ڳ�����ʱ�Ե�ǰUNIXʱ�������������ӡ�
���Ժ�ÿһ�ε��øú�������ʹ�ø���������������������
������ո�����ʱ����������߳�ͬʱ��һ�ε��øú������ܻ���������һ�����������

* @note
���飺���ڳ�����̸߳�����ʱ���øú�����
*/
unsigned gos_get_rand();

/**
* @name                 gos_get_range_rand
* @brief                ��ȡһ���ض���Χ�����������ΧΪ[uMin, uMax]
* @param                ulMin   [in]
* @param                ulMax   [in]
* @return               �����������
* @author               δ֪
* @date                 2019-06-19 15:06:22

* @warning
���ڸú�����Ҳʹ����srand����ʼ����������Ӻ�����,
���Ըú�����gos_get_rand���������������Ϊһ���ģ����ǳ�������ʱ����Ϊ���ӣ���
����һ�������������Ҳ��һ����������һ�º������ɵ��������һ�������̲߳���Ҳ��һ����
����Ȼ�Ƽ���ʹ��gos_get_rand�����������Ȼ��ȡ��Χ��

* @note
Lijiancong: modifytime: 2019-06-19 16:04:51
�Ķ�����Ϊ��ʹ��rand()�����������Ϊʹ��gos_get_rand�����������
*/
unsigned gos_get_range_rand(unsigned ulMin, unsigned ulMax);

/**
* @name                gos_get_gmtime
* @brief               ��ȡ��ǰUTCʱ�䣨�ȱ���ʱ�����Сʱ���� 1970 �� 1 �� 1 �� 00:00 �����ڵ�����
* @param               NONE
* @return              ��ǰUTCʱ���UNIXʱ���
* @author              Lijiancong, 316, lijiancong@fritt.com.cn
* @date                2019-06-20 10:28:35

* @warning
1) �����е��õ�gmtime������C++11�б��Ƽ�ʹ��gmtime_s
2) ��Ϊʹ���˸ú���ʹ���� gmtime���ú������ܲ����̰߳�ȫ�ġ�(��׼��д��)

* @note
����֪���������д�����ֱ��ʹ��gos_current_time()�����ˡ�
*/
unsigned gos_get_gmtime();

/**
* @name                 gos_get_current_time
* @brief                ��ȡ��ǰʱ�����1970/01/01 08:00:00(����ʱ��)������
* @class                NONE
* @param                NONE
* @return               ���ػ�ȡ��������
* @author               δ֪
* @date                 2019-06-25 14:24:54
*/
unsigned gos_get_current_time();

/**
* @name                gos_set_gmtime
* @brief               �ı䱾�ص��Ե�ʱ��
* @param               ulSecond [in] ��Ҫ������ʱ��
* @return              NONE
* @author              δ֪
* @date                2019-06-20 11:03:44

* @note
SetLocalTime����������ͷ�ļ�<sysinfoapi.h>

* @warning
1. Lijiancong: �Լ�������һ�£���֪��Ϊʲô����û�ܸı����ʱ��
*/
void gos_set_gmtime(unsigned ulSecond);

/**
* @name                Binary_Search_Array
* @brief               ���ֲ���auSearchArray�����б�ulBeginValue���ڵ��ڵĵ�һ����ֵ

* @param               ulBeginValue     [in] ��ѯ�Ļ�׼��ֵ��������ֵҪ���ڵ������ֵ
* @param               auSearchArray    [in] Ҫ��ѯ������
* @param               ulArraySize      [in] ���������Ĵ�С

* @return              ���ش��ڻ����ulBeginValue����ֵ
*   @retval            UINT_MAX     �쳣����ֵ��ulBeginValue���ڵ�һ���������ֵ��104729����unsigned�����ֵ
*   @retval            ����         ��������ֵ

* @author              Lijiancong, 316, lijiancong@fritt.com.cn
* @date                2019-06-21 09:11:20

* @note
*/
const unsigned Binary_Search_Array(const unsigned ulBeginValue, const unsigned auSearchArray[], const unsigned ulArraySize);

/**
* @name                gos_get_next_prime
* @brief               ��ȡ���ڵ���ulBeginNumber��ֵ�ĵ�һ������
* @param               ulBeginNumber    ��ȡ�����Ļ�׼ֵ

* @return              ���ڵ���ulBeginNumber�ĵ�һ������
*   @retval            UINT_MAX     �쳣����ֵ��ulBeginValue���ڵ�һ���������ֵ��104729����unsigned�����ֵ
*   @retval            ����         ��������ֵ

* @author              Lijiancong, 316, lijiancong@fritt.com.cn
* @date                2019-06-21 09:21:39

* @note
*/
const unsigned gos_get_next_prime(const unsigned ulBeginNumber);


unsigned gos_get_prime_old(unsigned ulStart);

#endif