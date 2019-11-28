/** 
 * Copyright (c) 2019 Lijiancong. All rights reserved.
 * 
 * Use of this source code is governed by a MIT license
 * that can be found in the License file.
 */

///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 
/**
* @file                 init.h
* @brief                放一些main函数所用到的函数

* @author               lijiancong
* @date                 2019-11-27 11:18:34

* @note
*/
///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 
#pragma once
#ifndef INIT_INTI_H
#define INIT_INTI_H

/**
 * @name                CheckConfig
 * @brief               检查配置文件夹是否存在，如果不存在就创建该文件夹，创建失败就直接退出程序
 * @param               NONE
 * @return              NONE
 * @author              Lijiancong, pipinstall@163.com
 * @date                2019-11-12 11:47:06
 * @warning             线程安全

 * @note
 */
void CheckConfigFolder();

#endif  // end of INIT_INTI_H