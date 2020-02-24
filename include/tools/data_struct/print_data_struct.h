///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   print_data_struct.h
/// @brief  这个文件主要写如何打印数据结构的函数
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-10 09:02:14
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_PRINT_DATA_STRUCT_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_PRINT_DATA_STRUCT_H_

namespace Lee {
inline namespace data_struct {
inline namespace print_data_struct {
/// @name     PrintCompleteBinaryTree
/// @brief    打印一个完整的树，树的深度不超过5
///
/// @param    depth [in]  树的深度
///
/// @return   NONE
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-10 13:53:10
/// @warning  线程不安全
void PrintCompleteBinaryTree(const int &depth);

}  // namespace print_data_struct
}  // namespace data_struct
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_PRINT_DATA_STRUCT_H_
