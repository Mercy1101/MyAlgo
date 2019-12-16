// Copyright (c) 2019 Lijiancong. All rights reserved.
//
// Use of this source code is governed by a MIT license
// that can be found in the License file.

///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
//
// @file                 leetcode.h
// @brief                这个文件用来写一些leetcode的题
//
// @author               lijiancong
// @date                 2019-11-28 19:38:49
//
// @note
//
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
#ifndef LEET_CODE_H__
#define LEET_CODE_H__

#include <ctype.h>  // for isalnum
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace Lee {
namespace Leetcode {

/**
 * @name                isPalindrome
 * @brief               检查某字符串是否为回文字符串
 *
 * @param               s   [in]    需要检查的字符串
 *
 * @return              是回文字符串或不是
 *
 * @author              Lijiancong, pipinstall@163.com
 * @date                2019-11-28 19:41:20
 * @warning             线程不安全

* @note
* 题目：Valid Palindrome
* Given a string, determine if it is a palindrome, considering only alphanumeric
* characters and ignoring cases. For example, "A man, a plan, a canal: Panama"
is
* a palindrome. "race a car" is not a palindrome. Note: Have you consider that
the
* string might be empty? This is a good question to ask during an interview. For
* the purpose of this problem, we define empty string as valid palindrome.
*/
bool isPalindrome(std::string s);

/**
 * @name                IsValidParentheses
 * @brief               检查括号是否配对
 *
 * @param               s   [in]    要检查的字符串
 *
 * @return              配对或不配对
 *
 * @author              Lijiancong, pipinstall@163.com
 * @date                2019-11-29 11:57:42
 * @warning             线程不安全

 * @note
 * 题目：Valid Parentheses
 * Given a string containing just the characters '(', ')', '{', '}', '[' and
 * ']', determine if the
 * input string is valid. The brackets must close in the correct order, "()" and
 "()[]" are
 * all valid but "(]" and "([)]" are not.
 */
bool IsValidParentheses(std::string const& s);

/// @name     ParseChanNumber
/// @brief    解析ChanNumber的函数,解释失败会返回false
///
/// @details  chan_number_vec在解析之前不会被清空,
///           当解析成功时容器中通道号会被排序,
///           如果容器中有重复的通道号会返回false
///
/// @param    str [in]  要解析的string
/// @param    chan_number_vec [out] 解析好的通道号的容器
///
/// @return   解析成功与否
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-12 19:49:13
/// @warning  线程不安全
bool ParseChanNumber(const std::string& str, std::vector<int>* chan_number_vec);

}  // end of namespace Leetcode
}  // end of namespace Lee

#endif  // end of LEET_CODE_H__
