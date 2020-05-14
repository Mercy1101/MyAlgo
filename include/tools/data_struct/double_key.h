
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   double_key.h
/// @brief  双键的实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-05-13 13:13:24
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_DOUBLE_KEY_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_DOUBLE_KEY_H_

#include <map>

namespace Lee {
inline namespace data_struct {
inline namespace double_key {
/// @name     double_key
/// @brief    键值对，两个都可以作为键来查找元素
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-05-13 13:12:20
/// @warning  线程不安全
template <typename T1, typename T2>
class double_key {
 public:
  double_key() {
    static_assert(!std::is_same<T1, T2>::value, "T1 T2 can not be same");
  }

  void add(T1 value1, T2 value2) {
    if (map_1.find(value1) != map_1.end()) {
      return;
    }

    if (map_2.find(value2) != map_2.end()) {
      return;
    }

    /// C++ 17 使用try_emplace代替
    map_1[value1] = value2;
    map_2[value2] = value1;
  }

  void add(T2 value2, T1 value1) {
    add(std::forward<T1>(value1), std::forward<T2>(value2));
  }

  void erase(T1 value1) {
    auto it = map_1.find(value1);
    if (it == map_1.end()) {
      return;
    }
    map_2.erase(map_2.find(it->second));
    map_1.erase(it);
  }

  void erase(T2 value2) {
    auto it = map_2.find(value2);
    if (it == map_2.end()) {
      return;
    }
    map_1.erase(map_1.find(it->second));
    map_2.erase(it);
  }

  T2 at(T1 value1) { return map_1.at(value1); }

  T1 at(T2 value2) { return map_2.at(value2); }

 private:
  std::map<T1, T2> map_1;
  std::map<T2, T1> map_2;
};

}  // namespace double_key
}  // namespace data_struct
}  // namespace Lee
#endif