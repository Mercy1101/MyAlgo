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
#include <map>
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

/// @name     top_k_frequent
/// @brief    找寻一个数组中出现频次前k个的元素
///
/// @param    nums  [in]  需要寻找的数组
/// @param    k     [in]  出现频次前k个
///
/// @return   出现频次前k个元素组成的容器
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-01 12:02:05
/// @warning  线程不安全
/// @note
/// Title: Top K Frequent Elements.
/// Given a non-empty array of integers, return the k most frequent elements.
/// 1. You may assume k is always vaild, 1 ≤ k ≤ number of unique elements.
/// 2. You algorithm's time coplexity must be better than O(nlogn), where n
///    is the array's size.
/// 3. It's guaranteed that the answer is unique, in other words the set of the
///    top k frequent elements is unique.
/// 4. You can return the answer in any order.
///
/// usage1:
/// Input: nums = [1,1,1,2,2,3], k=2
/// Output: [1,2]
///
/// usage2:
/// Input: nums = [1], k = 1
/// Output: [1]
inline std::vector<int> top_k_frequent(std::vector<int>& nums, int k) {
  std::map<int, int> nums_map;
  for (const auto& it : nums) {
    ++nums_map[it];
  }

  std::multimap<int, int, std::greater<int>> temp_map;
  for (const auto& it : nums_map) {
    temp_map.insert({it.second, it.first});
  }

  std::vector<int> result_temp;
  result_temp.reserve(k);
  auto it = temp_map.begin();
  while (k-- > 0) {
    result_temp.emplace_back((it++)->second);
  }

  return result_temp;
}

/// @name     sort_string
/// @brief    对一个字符串排序详细见下方
/// @details  LeetCode: 1370. Increasing Decreasing String
///
/// @param    s [in]
///
/// @return   排序后的图片
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-02 12:08:43
/// @warning  线程不安全
/// Given a string s. You should re-order the string using the following
/// algorithm:
/// 1. Pick the smallest character from s and append it to the result.
/// 2. Pick the smallest character from s which is greater than the last
///    appended character to the result and append it.
/// 3. Repeat step 2 until you cannot pick more characters.
/// 4. Pick the largest character from s and append it to the result.
/// 5. Pick the largest character from s which is smaller than the last appended
///    character to the result and append it.
/// 6. Repeat step 5 until you cannot pick more characters.
/// 7. Repeat the step from 1 to 6 until you pick all characters from s.
///
/// In each step, if the smallest or the largest character appears more than
/// once you can choose any occurrence and append it to the result.
/// Return the result string after sorting s with algorithm.
///
/// Example 1:
/// Input: s = "aaaabbbbcccc"
/// Output: "abccbaabccba"
///
/// Example 2:
/// Input: s = "rat"
/// Output: "art"
///
/// Example 3:
/// Input: s = "leetcode"
/// Output: "cdelotee"
///
/// Example 4:
/// Input: s = "ggggggg"
/// Output "ggggggg"
///
/// Example 5:
/// Input: s = "spo"
/// Output: "ops"
///
/// Constraints:
/// * 1 <= s.length <= 500
/// * s contains only lower-case English letters.
inline std::string sort_string(std::string s) {
  std::sort(s.begin(), s.end(), std::greater());
  std::string result;
  while (!s.empty()) {
    std::reverse(s.begin(), s.end());
    result += s.at(0);
    s.erase(s.begin());
    for (auto it = s.begin(); it != s.end();) {
      if (*it != *(--result.end())) {
        result += *it;
        it = s.erase(it);
      } else {
        ++it;
      }
    }
  }
  return result;
}

inline std::string sort_string_sample(std::string s) {
  int cnt[26] = {0};
  for (auto ch : s) ++cnt[ch - 'a'];

  std::string ans;
  int n = 0, N = s.size();
  auto appendChar = [&](int i) {
    if (cnt[i]) {
      ans.push_back(i + 'a');
      --cnt[i];
      ++n;
    }
  };

  while (n < N) {
    for (int i = 0; i < 26; ++i) appendChar(i);
    for (int i = 25; i >= 0; --i) appendChar(i);
  }

  return ans;
}

}  // end of namespace Leetcode
}  // end of namespace Lee

#endif  // end of LEET_CODE_H__
