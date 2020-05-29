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
#include <functional>
#include <iostream>
#include <optional>
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
  [[nodiscard]] std::optional<int> calculate(const std::string& expression) {
    try {
      if (!confirm_expression_valid(expression)) {
        throw std::exception("Expression is invalid!");
      }

      std::string parsed_string = parse_expression(expression);
      if (!isalnum(parsed_string.at(0))) {
        throw std::exception("Expression is invalid!");
      }

      std::stack<int> number_stack;
      std::stack<char> operator_stack;
      get_number(parsed_string, number_stack, operator_stack);

      return calculate_result(number_stack, operator_stack);
    } catch (std::exception e) {
      std::cout << e.what() << std::endl;
      return {};
    }
  }

 private:
  /// @name     calculate_result
  /// @brief    计算出结果
  ///
  /// @param    number_stack    [in]
  /// @param    operator_stack  [in]
  ///
  /// @return   计算出来的结果
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date
  /// @warning  线程不安全
  int calculate_result(std::stack<int>& number_stack,
                       std::stack<char>& operator_stack) {
    /// 开始计算结果
    int result = number_stack.top();
    number_stack.pop();
    while (!operator_stack.empty()) {
      auto it = operator_stack.top();
      operator_stack.pop();
      if (number_stack.empty()) {
        throw std::exception("number_stack is invalid!");
      }
      auto temp_result = number_stack.top();
      number_stack.pop();
      result = calculate_number(temp_result, result, it);
    }
    return result;
  }

  /// @name      get_number
  /// @brief     解析出来数字和符号
  ///
  /// @param     parsed_string   [in]
  /// @param     number_stack    [out]
  /// @param     operator_stack  [out]
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-05-27 12:11:06
  /// @warning  线程不安全
  void get_number(const std::string& parsed_string,
                  std::stack<int>& number_stack,
                  std::stack<char>& operator_stack) {
    /// 提取数字
    size_t start_index = 0;
    for (size_t current_index =
             parsed_string.find_first_of("+-*/", start_index);
         current_index != std::string::npos;
         current_index = parsed_string.find_first_of("+-*/", start_index)) {
      auto num_string = parsed_string.substr(start_index, current_index-start_index);
      number_stack.push(std::stoi(num_string));
      operator_stack.push(parsed_string.at(current_index));
      start_index = current_index + 1;
    }
    /// 提取最后一个数字
    number_stack.push(
        std::stoi(parsed_string.substr(start_index, parsed_string.size())));
    if (number_stack.size() != operator_stack.size() + 1) {
      throw std::exception("invalid number!");
    }
  }
  /// @name     calculate_number
  /// @brief    计算结果并返回，遇到错误抛出异常
  ///
  /// @param    first_number    [in]  第一个运算数字
  /// @param    seconde_number  [in]  第二个运算数字
  /// @param    op              [in]  运算符
  ///
  /// @return   运算的结果
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-05-25 12:54:38
  /// @warning  线程不安全
  int calculate_number(const int first_number, const int second_number,
                       const char op) {
    if ('+' == op) {
      return first_number + second_number;
    } else if ('-' == op) {
      return first_number - second_number;
    } else if ('*' == op) {
      return first_number * second_number;
    } else if ('/' == op) {
      if (second_number == 0) {
        throw std::exception("div zero error!");
      }
      return first_number / second_number;
    } else {
      throw std::exception("unkonw operator!");
    }
  }
  /// @name     parse_expression
  /// @brief    解析输入字符串，去除了空格
  ///
  /// @param    expression [in]
  ///
  /// @return   解析后的字符串
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-05-23 12:49:01
  /// @warning  线程不安全
  std::string parse_expression(const std::string& expression) {
    std::string parsed_string = expression;

    while (int index = parsed_string.find(' ') != std::string::npos) {
      parsed_string.erase(index, 1);
    }
    return parsed_string;
  }

  /// @name     confirm_expression_valid
  /// @brief    确认输入的字符串是否合法
  ///
  /// @param    expression  [in]
  ///
  /// @return   true or false
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
        std::string::npos) {
      return false;
    }
    return true;
  }
};

}  // namespace stack_practice
}  // namespace data_struct
}  // namespace Lee

#endif