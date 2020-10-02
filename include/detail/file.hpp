///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   file.h
/// @brief  关于文件的函数
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-02 14:26:04
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_FILE_HPP_
#define INCLUDE_DETAIL_FILE_HPP_

#include <filesystem>

namespace lee {
inline namespace file {

/// @name     get_root_path_s
/// @brief    获取根路径
///
/// @param    NONE
///
/// @return   当前路径
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-24 21:08:01
/// @warning  线程不安全
inline std::string get_root_path_s() {
  return std::filesystem::current_path().generic_string();
}

/// @name     create_folder_s
/// @brief    创建一个文件夹
///
/// @param    floder_path
///
/// @return   创建成功与否
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-24 21:08:01
/// @warning  线程不安全
inline bool create_folder_s(const std::string& floder_path) {
  return std::filesystem::create_directory(floder_path);
}

/// @name     is_file_exist_s
/// @brief    判断文件是否存在
///
/// @param    file_path_name  [in]
///
/// @return   存在或不存在
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-25 10:47:06
/// @warning  线程不安全
inline bool is_file_exist_s(const std::string& file_path_name) {
  return std::filesystem::exists(file_path_name);
}

/// @name     remove_file_or_folder
/// @brief    删除一个文件或者递归删除一个目录
///
/// @param    file_or_folder_name  [in] 文件名或文件目录名称
///
/// @return   删除成功与否
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-25 10:57:20
/// @warning  线程不安全
inline bool remove_file_or_folder_s(const std::string& file_or_folder_name) {
  return 0 != std::filesystem::remove_all(file_or_folder_name);
}
}  // namespace file
}  // namespace lee
#endif