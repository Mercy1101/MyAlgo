///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   optional.h
/// @brief  optional的简易实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-19 18:53:53
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_EXAMPLE_HPP_
#define INCLUDE_DETAIL_EXAMPLE_HPP_

#include <exception>

namespace lee {
inline namespace optional_impl {
template <typename T>
class optional {
 public:
  optional() = default;
  optional(T value) : has_value_(true), value_(value) {}
  optional(const optional& other)
      : has_value_(other.has_value_), value_(other.value_) {}

  bool has_value() { return has_value_; }

  T value() {
    if (!has_value_) {
      throw std::exception("Has no value!");
    } else {
      return value_;
    }
  }

  T value_or(T default_value) {
    if (has_value_) {
      return value_;
    } else {
      return default_value;
    }
  }

 private:
  bool has_value_ = false;
  T value_{};
};
}  // namespace optional_impl
}  // namespace lee

#endif  ///< end of INCLUDE_DETAIL_EXAMPLE_HPP_