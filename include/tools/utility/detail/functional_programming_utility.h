
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   functional_programming_utility.h
/// @brief  << C++ 函数式编程>> 读书笔记
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-06-23 12:03:14
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_FP_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_FP_UTILITY_H_

#include <iostream>
#include <memory>
#include <mutex>

namespace Lee {
inline namespace utility {
inline namespace fp {

/// @name     lazy_val
/// @brief    惰性求值的对象的创建
///
/// @author   Lijiancong, pipinstall@163.com
///           摘录自第六章惰性求值
/// @date     2020-06-23 12:07:31
/// @warning  线程不安全
template <typename F>
class lazy_val {
 private:
  F computation_;
  mutable decltype(computation_()) cache_;
  mutable std::once_flag value_flag_;

 public:
  const decltype(computation()) &operator() const {
    std::call_once(value_flag_, [this] { cache_ = computation_(); });
    return cache_;
  }
};

/// 保存任一数目字符串的结构
template <typename... str>
class lazy_string_concat_helper;

template <typename last_str, typename... str>
class lazy_string_concat_helper<last_str, str...> {
 private:
  lazy_string_concat_helper(last_str data,
                            lazy_string_concat_helper<str...> tail)
      : data_(data), tail_(tail) {}

  int size() const { return data.size() + tail.size(); }

  template <typename It>
  void save(It end) const {
    const auto begin = end - data.size();
    std::copy(data.cbegin(), data.cend(), begin);
    tail_.save(begin);
  }

  operator std::string() const {
    std::string result(size(), '\0');
    save(result.end());
    return result;
  }

  lazy_string_concat_helper<std::string, last_str, str...> operator+(
      const std::string &other) const {
    return lazy_string_concat_helper<std::string, last_str, str...>(other,
                                                                    *this);
  }
};

/// 特化一个中止条件对象
template <>
class lazy_string_concat_helper<> {
 public:
  lazy_string_concat_helper() {}
  int size() const { return 0; }
  template <typename It>
  void save(It) const {}

  lazy_string_concat_helper<std::string> operator+(
      const std::string &other) const {
    return lazy_string_concat_helper<std::string>(other, *this);
  }
};

template <typename T>
class lazy_ptr {
 public:
  lazy_ptr() : child(0) {}
  ~lazy_ptr() { delete child; }
  T &operator*() {
    if (!child) child = new T;
    return *child;
  }
  // might dereference NULL pointer if unset...
  // but if this is const, what else can be done?
  const T &operator*() const { return *child; }
  T *operator->() { return &**this; }
  const T *operator->() const { return &**this; }

 private:
  T *child;
};

/// 函数式数据结构
template <typename T>
class list {
 public:
  /// 递归析构扁平化
  ~node() {
    auto next_node = std::move(tail);
    while (next_node) {
      if (!next_node.unique()) break;
      std::shared_ptr<node> tail;
      std::swap(tail, next_node->tail);
      next_node.reset();
      noext_node = std::move(tail);
    }
  }

 private:
  struct node {
    T value;
    std::shared_ptr<node> tail;
  };
  std::shared_ptr<node> m_head;
};

/// 代数数据类型及模式匹配
void point_for(player which_player){
  std::visit(
    overloaded{
      [&](const normal_scoring &state){
        /// 增加得分，或者切换状态
      },
      [&](const forty_scoring &state){
        /// 玩家胜，或切换到平局状态
      },
      [&](const deduce &state){
        /// 切换到占先状态
      },
      [&](const advantage &state){
        /// 玩家胜， 或者回到平局状态
      },

    }
  );

}

}  // namespace fp
}  // namespace utility
}  // namespace Lee