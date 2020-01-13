﻿///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
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

#pragma warning(disable : 4127)

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

namespace Lee {
inline namespace utility {
inline namespace system {
/// @name     IsFileExist
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
inline bool IsFileExist(const std::string& strFilePathAndName) {
  if (strFilePathAndName.empty()) return false;
  return (_access(strFilePathAndName.c_str(), 0) == 0) ? true : false;
}

/// @name     StringToLPCWSTR
/// @brief    std::string 向LPCWSTR的转换
///
/// @param    strText [in]
///
/// @return   转换后的LPCWSTR
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-22 09:52:06
/// @warning  线程安全
/// @note     注意：不可以使用空字符串传入，会抛出异常
/// inline LPCWSTR StringToLPCWSTR(const std::string& strText) {
///   // assert(!strText.empty());
///   size_t origsize = strText.length() + 1;
///   const size_t newsize = 100;
///   size_t convertedChars = 0;
///   wchar_t* wcstring =
///       (wchar_t*)malloc(sizeof(wchar_t) * (strText.length() - 1));
///   mbstowcs_s(&convertedChars, wcstring, origsize, strText.c_str(),
///   _TRUNCATE);
///
///   return wcstring;
/// }

/// @name     StringToWSTR
/// @brief    std::string 向std::wstring的转换
///
/// @param    strText [in]
///
/// @return   转换后的std::wstring
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-22 09:52:06
/// @warning  线程不安全
/// inline std::wstring StringToWSTR(const std::string& s) {
///   int length = static_cast<int>(s.length()) + 1;
///   int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), length, 0, 0);
///   wchar_t* buf = new wchar_t[len];
///   MultiByteToWideChar(CP_ACP, 0, s.c_str(), length, buf, len);
///   std::wstring r(buf);
///   delete[] buf;
///   return r;
/// }

/// @name     Wchar_tToString
/// @brief    wchar_t to string
///
/// @param    szDst    [in]
/// @param    wchar    [out]
///
/// @return   NONE
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-22 10:09:35
/// @warning  线程不安全
/// inline void Wchar_tToString(std::string& szDst, wchar_t* wchar) {
///   wchar_t* wText = wchar;
///   DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0,
///   NULL,
///                                     FALSE);  // WideCharToMultiByte的运用
///   char* psText;  // psText为char*的临时数组，作为赋值给std::string的中间变量
///   psText = new char[dwNum];
///   WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL,
///                       FALSE);  // WideCharToMultiByte的再次运用
///   szDst = psText;              // std::string赋值
///   delete[] psText;             // psText的清除
/// }
/// inline void Wchar_tToString(std::string& szDst, wchar_t* wchar) {
///   std::wstring ws(wchar);
///   std::string temp(ws.begin(), ws.end());
///   szDst = temp;
/// }

/// @name     GetRootPath
/// @brief    获取当前程序所在的根目录.(windows系统限定)
///
/// @param    NONE
///
/// @return   当前根目录,获取失败则为空
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-06 14:33:49
/// @warning  线程安全
inline std::string GetRootPath() {
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
/// @name     CreateFileFolder
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
inline bool CreateFileFolder(const std::string& strFolderPath) {
  if (strFolderPath.empty()) return false;
  return 0 == CreateDirectory(strFolderPath.c_str(), NULL) ? false : true;
}
/// @name     StringToWstring
/// @brief    string to wstring
///
/// @param    szDst    [out]
/// @param    str      [in]
///
/// @return   NONE
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-22 10:16:52
/// @warning  线程不安全
inline void StringToWstring(std::wstring& szDst, const std::string& str) {
  // std::string temp = str;
  int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, NULL, 0);
  wchar_t* wszUtf8 = new wchar_t[len + 1];
  memset(wszUtf8, 0, len * 2 + 2);
  MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, (LPWSTR)wszUtf8, len);
  szDst = wszUtf8;
  // std::wstring r = wszUtf8;
  delete[] wszUtf8;
}

}  // namespace system
}  // namespace utility
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_SYSTEM_UTILITY_H_
