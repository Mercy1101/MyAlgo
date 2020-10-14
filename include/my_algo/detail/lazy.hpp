///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   lazy.h
/// @brief  惰性求值的相关函数
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-02 09:37:40
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_LAZY_HPP_
#define INCLUDE_DETAIL_LAZY_HPP_

#include <functional>
#include <mutex>

namespace lee {
inline namespace lazy {

/// @name     lazy_val
/// @brief    惰性求值的对象的创建
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-23 12:07:31
/// @warning  线程不安全
template <typename F>
class lazy_val {
 private:
  F computation_;
  mutable decltype(computation_()) cache_;
  std::once_flag value_flag_;

 public:
  const decltype(computation_()) &operator()() {
    std::call_once(value_flag_, [&] { cache_ = computation_(); });
    return cache_;
  }
};

template <typename T>
class lazy_ptr {
 public:
  lazy_ptr() : child(0) {}
  ~lazy_ptr() { delete child; }
  T &operator*() {
    std::call_once(flag_, [&]() { child = new T; });
    return *child;
  }
  // might dereference NULL pointer if unset...
  // but if this is const, what else can be done?
  const T &operator*() const { return *child; }
  T *operator->() { return &**this; }
  const T *operator->() const { return &**this; }

 private:
  T *child;
  mutable std::once_flag flag_;
};
}  // namespace lazy
}  // namespace lee
#endif  ///< end of INCLUDE_DETAIL_LAZY_HPP_