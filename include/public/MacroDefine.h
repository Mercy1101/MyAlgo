/** 
 * Copyright (c) 2019 Lijiancong. All rights reserved.
 * 
 * Use of this source code is governed by a MIT license
 * that can be found in the License file.
 */

///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 
/**
* @file                 log.h
* @brief                这个文件主要定义了重写的新log模块的接口，会使用C++11以后的关键字，c++Builder不能使用

* @author               Lijiancong, pipinstall@163.com
* @date                 2019-09-17 10:37:35

* @note
*/
///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 
#pragma once
#ifndef PUBLIC_MACRO_DEFINE_H
#define PUBLIC_MACRO_DEFINE_H

#include <string>

/** 默认的配置文件放置的路径（根目录下的文件夹名） */
const std::string DEFAULT_CONFIG_FOLDER_NAME = "config";

/** 默认的配置文件名 */
const std::string DEFAULT_CONFIG_FILE_NAME = "conf.ini";

/** 默认日志根文件夹 */
const std::string DEFAULT_LOG_FILE_FOLDER = "log";

#endif  // end of PUBLIC_MACRO_DEFINE_H