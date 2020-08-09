///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019, 2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   calculate.h
/// @brief  四则运算
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-05-14 10:52:48
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_CALCULATE_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_CALCULATE_H_

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <string>

namespace lee {
inline namespace data_struct {
inline namespace calculate {

/// @name     simple_calculate
/// @brief    用于计算一个输入的字符串的对象
/// @details  注意：
///           1. 现在不支持数字中间有空格的情况。2020-05-30 18:20:37
///           2. 目前不支持余数、浮点运算和溢出检测。2020-05-30 18:26:29
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-05-22 11:50:25
/// @warning  线程不安全
/// @example  用法如下
/// @code
///     #inlcude <iostream>
///     #include <cassert>
///     #include "caculate.h"
///
///     int main() {
///       asserst((41 == lee::calculate::cal("5+8*4+8/2")) && "计算错误！");
///       asserst((41 == lee::calculate::cal("  5+  8*4+8   /2 ")) &&
///       "计算错误！"));
///     }
/// @endcode
class simple_calculate {
 public:
  [[nodiscard]] static std::optional<int> cal(const std::string& expression) {
    static std::once_flag instance_flag;
    std::call_once(instance_flag, []() {
      if (nullptr == instance) {
        instance = new simple_calculate();
      }
    });
    return instance->calculate(expression);
  }

  ~simple_calculate() {}

 private:
  simple_calculate() {}
  inline static simple_calculate* instance = nullptr;

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
  std::optional<int> calculate(const std::string& expression) {
    try {
      if (!confirm_expression_valid(expression)) {
         ("Expression is invalid!");
      }

      std::string parsed_string = parse_expression(expression);
      if (!isalnum(parsed_string.at(0))) {
         ("Expression is invalid!");
      }

      std::vector<int> number_list;
      std::vector<char> operator_list;
      parse_number(parsed_string, number_list, operator_list);

      return calculate_result(number_list, operator_list);
    } catch (std::exception e) {
      (void)e;
      /// std::cout << e.what() << std::endl;
      return {};
    }
  }

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
  int calculate_result(std::vector<int>& number_list,
                       std::vector<char>& operator_list) {
    /// 优先运算乘法和除法
    priority_calculate(number_list, operator_list);

    /// 开始加法和减法的计算结果
    int result = number_list.front();
    number_list.erase(number_list.begin());
    while (!operator_list.empty()) {
      auto it = operator_list.front();
      operator_list.erase(operator_list.begin());
      if (number_list.empty()) {
         ("number_stack is invalid!");
      }
      auto temp_result = number_list.front();
      number_list.erase(number_list.begin());
      result = calculate_number(result, temp_result, it);
    }
    return result;
  }

  /// @name     priority_calculate
  /// @brief    优先计算一下乘除法，并把计算完的乘法存储在number_list中
  ///
  /// @param    number_list     [in/out]
  /// @param    operator_list   [in/out]
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-05-30 15:10:36
  /// @warning  线程不安全
  void priority_calculate(std::vector<int>& number_list,
                          std::vector<char>& operator_list) {
    for (auto priority_operator = std::find_if(
             operator_list.begin(), operator_list.end(),
             [](const char& temp) { return temp == '*' || temp == '/'; });
         priority_operator != operator_list.end();
         priority_operator = std::find_if(
             operator_list.begin(), operator_list.end(),
             [](const char& temp) { return temp == '*' || temp == '/'; })) {
      /// 获取该运算符
      auto op = *priority_operator;
      /// 找寻这个迭代器在容器的为第几个元素，用于找寻对应的运算数字
      int position = 0;
      for (auto it = operator_list.begin(); it != priority_operator; ++it) {
        ++position;
      }

      /// 擦除该运算符
      operator_list.erase(priority_operator);
      /// 计算结果
      number_list.at(position + 1) = calculate_number(
          number_list.at(position), number_list.at(position + 1), op);
      /// 找寻并擦除第二操作数
      auto it = number_list.begin();
      for (int i = 0; i < position; ++i) {
        ++it;
      }
      number_list.erase(it);
    }
  }

  /// @name      parse_number
  /// @brief     解析出来数字和符号
  ///
  /// @param     parsed_string   [in]
  /// @param     number_list     [out]
  /// @param     operator_list   [out]
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-05-27 12:11:06
  /// @warning  线程不安全
  void parse_number(const std::string& parsed_string,
                    std::vector<int>& number_list,
                    std::vector<char>& operator_list) {
    /// 提取数字
    size_t start_index = 0;
    for (size_t current_index =
             parsed_string.find_first_of("+-*/", start_index);
         current_index != std::string::npos;
         current_index = parsed_string.find_first_of("+-*/", start_index)) {
      auto num_string =
          parsed_string.substr(start_index, current_index - start_index);
      number_list.push_back(std::stoi(num_string));
      operator_list.push_back(parsed_string.at(current_index));
      start_index = current_index + 1;
    }
    /// 提取最后一个数字
    number_list.push_back(
        std::stoi(parsed_string.substr(start_index, parsed_string.size())));
    if (number_list.size() != operator_list.size() + 1) {
       ("invalid number!");
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
         ("div zero error!");
      }
      return first_number / second_number;
    } else {
       ("unknow operator!");
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
    for (size_t index = parsed_string.find(' '); index != std::string::npos;
         index = parsed_string.find(' ')) {
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
    if (expression.find_first_of("~!@#$%^&_{}|:\"<>?`=\\[];',.") !=
        std::string::npos) {
      return false;
    }
    return true;
  }
};

}  // namespace calculate
}  // namespace data_struct
}  // namespace lee

#endif
