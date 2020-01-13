///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019 Lijiancong. All rights reserved.
///
/// @file   thread_guard_utility.h
/// @brief  定义一个析构就把自己的线程调用join()函数的类
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-01-13 09:15:54
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_THREAD_GUARD_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_THREAD_GUARD_UTILITY_H_

#include <thread>

#include "utility/detail/marco_utility.h"

namespace Lee {
inline namespace utility {
inline namespace thread_guard_ {

/// 用来传入一个线程，并在析构时自动调用join(), 来令线程join(),
/// 防止语句发生异常时线程没有调用join函数
class thread_guard {
 public:
  explicit thread_guard(std::thread& t) : t_(t) {}
  ~thread_guard() {
    if (t_.joinable()) {
      t_.join();
    } else {
      /// TODO（李建聪）: 如果不能join，怎么处理？
    }
  }

 private:
  MYALGO_DISALLOW_COPY_AND_ASSIGN_(thread_guard);
  std::thread& t_;  ///< 用来保留一个线程的引用

};  /// end of class thread_guard
}  // namespace thread_guard_
}  // namespace utility
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_THREAD_GUARD_UTILITY_H_