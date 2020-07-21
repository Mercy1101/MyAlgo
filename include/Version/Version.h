/** 
 * Copyright (c) 2019 Lijiancong. All rights reserved.
 * 
 * Use of this source code is governed by a MIT license
 * that can be found in the License file.
 */

///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 
/**
* @file                 Version.h
* @brief                ����ļ���Ҫд�˹��������汾�Ĵ�ӡ����

* @details              �汾�ŵĺ궨����.cpp�ļ��У���main�����еĵ�һ�е���PrintVersion()������
                        �����������д�ӡ�汾��Ϣ�ͱ���ʱ�䡣

* @author               Lijiancong, pipinstall@163.com
* @date                 2019-11-14 16:49:19

* @note
*/
///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 

#ifndef _PUBLIC_VERSION_H__
#define _PUBLIC_VERSION_H__

#include "VersionConfig.h"
#include "utility/utility.h"    // for lee::GetTimeString(lee::GetCompileTimeStamp())
#include <log/log.h>
#include <string>

/** ��ǰ�����汾 */
const std::string VERSION("MyAlgo");
/** ��ǰ���������ǩ�� */
const std::string LABEL = std::to_string(CURRENT_TAGS_NUMBER);

/**
* @name                PrintVersion
* @brief               �����д�ӡ�汾�źͱ���ʱ��
* @param               NONE
* @return              NONE
* @author              Lijiancong, pipinstall@163.com
* @date                2019-08-05 09:59:22

* @note
*/
inline void PrintVersion()
{
    std::string strLogText;
    strLogText = strLogText + 
                 "\n*************************************************************\n" +
                 "Version:      " + std::string(VERSION) + "\n" +
                 "Label:        " + std::string(LABEL) + "\n" +
                 "Compile Time: " + lee::GetTimeString(lee::GetCompileTimeStamp()) + "\n" +
                 "*************************************************************\n";
    LOG_INFO(strLogText);
}

#endif  // end of _PUBLIC_VERSION_H__
