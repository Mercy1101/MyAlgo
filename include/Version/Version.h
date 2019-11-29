/** 
 * Copyright (c) 2019 Lijiancong. All rights reserved.
 * 
 * Use of this source code is governed by a MIT license
 * that can be found in the License file.
 */

///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 
/**
* @file                 Version.h
* @brief                这个文件主要写了关于软件版本的打印函数

* @details              版本号的宏定义在.cpp文件中，在main函数中的第一行调用PrintVersion()函数，
                        会在命令行中打印版本信息和编译时间。

* @author               Lijiancong, pipinstall@163.com
* @date                 2019-11-14 16:49:19

* @note
*/
///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 

#ifndef _PUBLIC_VERSION_H__
#define _PUBLIC_VERSION_H__

#include <string>
#include <log/log.h>
#include "utility/utility.h"    // for Lee::GetTimeString(Lee::GetCompileTimeStamp())
#include "VersionConfig.h"

/** 当前软件版本 */
const std::string VERSION("MyAlgo");
/** 当前软件具体标签号 */
const std::string LABEL = std::to_string(CURRENT_TAGS_NUMBER);

/**
* @name                PrintVersion
* @brief               命令行打印版本号和编译时间
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
                 "Compile Time: " + Lee::GetTimeString(Lee::GetCompileTimeStamp()) + "\n" +
                 "*************************************************************\n";
    LOG_NO_FUNCTIONNAME(LOG_INFO, strLogText);
}

#endif  // end of _PUBLIC_VERSION_H__