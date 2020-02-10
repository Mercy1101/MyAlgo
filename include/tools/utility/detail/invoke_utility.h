/// Copyright (c) 2019 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.

///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// @file   invoke_utility.h
/// @brief  对调用函数进行了封装, 使用了完美转发
///
/// @author lijiancong, pipinstall@163.com
///         Taken from <C++ Templates> 2e
///         Chapter 11 (11.1.3 Wrapping Function Calls)
/// @date   2020-02-10 13:17:58
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_INVOKE_UTILITY_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_INVOKE_UTILITY_UTILITY_H_

#include <functional>   // for std::forward()
#include <type_traits>  // for std::is_same<> and invoke_result<>
#include <utility>      // for std::invoke()

namespace Lee {
inline namespace utility {
inline namespace invoke {

template <typename Callable, typename... Args>
decltype(auto) call(Callable&& op, Args&&... args) {
  if constexpr (std::is_same_v<std::invoke_result_t<Callable, Args...>, void>) {
    // return type is void:
    std::invoke(std::forward<Callable>(op), std::forward<Args>(args)...);
    //...
    return;
  } else {
    // return type is not void:
    decltype(auto) ret{
        std::invoke(std::forward<Callable>(op), std::forward<Args>(args)...)};
    //...
    return ret;
  }
}
}  // namespace invoke
}  // namespace utility
}  // namespace Lee

#endif  /// end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_INVOKE_UTILITY_UTILITY_H_