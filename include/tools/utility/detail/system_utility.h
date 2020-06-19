///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019, Lijiancong Inc. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   system_utility.h
/// @brief  出于可移植性考虑,把与系统相关的工具集放到该文件下。
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-11-06 15:21:21
/// @note   该文件不可以使用log模块（会出编译错误），使用std::cout打印错误输出
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_SYSTEM_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_SYSTEM_UTILITY_H_

#  pragma warning(disable : 26451)  // unreachable code

#include <Windows.h>
#include <crtdbg.h>   // for _CrtDumpMemoryLeaks()
#include <direct.h>   // for _getcwd()
#include <fileapi.h>  // for CreateDirectory()
#include <io.h>       /// for _access()
#include <stdio.h>
#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>

namespace lee {
inline namespace utility {
inline namespace system {
/// @name     is_file_exist
/// @brief    判断一个文件是否存在
///
/// @param    strFilePathAndName  [in]    文件完整路径和文件名
///
/// @return   存在与否
///  @retval  true    存在
///  @retval  false   不存在
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-05 13:54:29
/// @warning  线程安全
inline bool is_file_exist(const std::string& strFilePathAndName) {
  if (strFilePathAndName.empty()) return false;
  return (_access(strFilePathAndName.c_str(), 0) == 0) ? true : false;
}

/// @name     get_root_path
/// @brief    获取当前程序所在的根目录.(windows系统限定)
///
/// @param    NONE
///
/// @return   当前根目录,获取失败则为空
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-06 14:33:49
/// @warning  线程安全
inline std::string get_root_path() {
  static std::once_flag InstanceFlag;
  static std::string strPathName;
  std::call_once(InstanceFlag, [&]() {
    char szPath[MAX_PATH] = {0};
    if (!GetModuleFileName(NULL, szPath, MAX_PATH)) {
      std::cout << "Cannot GetModuleFileName!Error Code: " << GetLastError()
                << std::endl;
      assert(false && "Cannot GetModuleFileName!");
      /// assert(false && "Cannot GetModuleFileName!");
      std::cout << "Cannot GetModuleFileName!" << std::endl;
      strPathName = "";
    }
    std::string Path(szPath);
    auto pos = Path.find_last_of("\\/");
    strPathName = Path.substr(0, pos);
  });
  return strPathName;
}
/// @name     create_file_folder
/// @brief    创建一个windows系统的文件夹
///
/// @param    strFolderPath   [in]    要创建目录的绝对路径
///
/// @return   创建成功与否
///  @retval  true    创建成功
///  @retval  false   创建失败
///           (入参为空或目录已经存在或该目录的上一级目录不存在)
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-06 15:13:34
/// @warning  线程安全
/// @note     注意:该函数只能创建一级目录,
///           如果上一级目录也不存在则需要先创建上一级目录,创建二级目录.
inline bool create_file_folder(const std::string& strFolderPath) {
  if (strFolderPath.empty()) return false;
  return 0 == CreateDirectory(strFolderPath.c_str(), NULL) ? false : true;
}

}  // namespace system
}  // namespace utility
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_SYSTEM_UTILITY_H_
