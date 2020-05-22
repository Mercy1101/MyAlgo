///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019, 2020 Lijiancong. All rights reserved.
///
/// @file   stack_practice.h
/// @brief  使用栈来实现四则运算
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-05-14 10:52:48
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_STACK_PRACTICE_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_STACK_PRACTICE_H_

#include <exception>
#include <iostream>
#include <stack>
#include <string>

namespace Lee {
inline namespace data_struct {
inline namespace stack_practice {

/// @name     simple_calculate
/// @brief    用于计算一个输入的字符串的对象
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-05-22 11:50:25
/// @warning  线程不安全
class simple_calculate {
 public:
  simple_calculate() {}
  ~simple_calculate() {}

  /// @name     calculate
  /// @brief    用于解析并计算出一个字符串中的算数符号
  ///
  /// @param    expression [in] 输入的表达式
  ///
  /// @return   计算的结果
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-05-22 11:52:54
  /// @warning  线程不安全
  [[nodiscard]] int calculate(const std::string& expression) {
    if (confirm_expression_valid(expression)) {
      throw;
    }
    std::stack<int> number_stack;
    std::stack<int> operator_stack;
  }

 private:
  /// @name     confirm_expression_valid
  /// @brief    确认输入的字符串是否合法
  ///
  /// @param    expression  [in]
  ///
  /// @return
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-05-22 12:45:49
  /// @warning  线程不安全
  bool confirm_expression_valid(const std::string& expression) {
    /// 表达式不能为空
    if (expression.empty()) {
      return false;
    }
    /// 表达式不能包含以下特殊字符
    if (expression.find_first_of("~!@#$%^&*_{}|:\"<>?`=\\[];',./") !=
        expression.end()) {
      return false;
    }
  }

  /// @name     calculate_exception
  /// @brief    用于抛出计算错误的结果
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-05-22 12:44:49
  /// @warning  线程不安全
  class calculate_exception : public std::logic_error {
    explicit calculate_exception(const std::string& error_log)
        : std::logic_error(error_log) {}
  };
}

}  // namespace stack_practice
}  // namespace data_struct
}  // namespace Lee

#endif