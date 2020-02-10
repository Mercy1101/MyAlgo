/// Copyright (c) 2019 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.

///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
///
/// @file   object_counter_utility.h
/// @brief  一个对象实现次数统计的模板
///
/// @author lijiancong, pipinstall@163.com
///         Taken from < C++ Templates > 2e
///         Chapter 21 (21.2 The Curiously Recurring Template Pattern (CRTP))
/// @date   2020-02-10 12:41:47
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_OBJECT_COUNTER_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_OBJECT_COUNTER_H_

#include <cstddef>

template <typename CountedType>
class ObjectCounter {
 private:
  inline static std::size_t count = 0;  // number of existing objects

 protected:
  // default constructor
  ObjectCounter() { ++count; }

  // copy constructor
  ObjectCounter(ObjectCounter<CountedType> const &) { ++count; }

  // move constructor
  ObjectCounter(ObjectCounter<CountedType> &&) { ++count; }

  // destructor
  ~ObjectCounter() { --count; }

 public:
  // return number of existing objects:
  static std::size_t live() { return count; }
};

#endif  /// end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_OBJECT_COUNTER_H_