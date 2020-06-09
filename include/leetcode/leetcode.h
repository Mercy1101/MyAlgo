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
#include <numeric>
#include <string>
#include <unordered_map>
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
bool IsValidParentheses(std::string const &s);

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
bool ParseChanNumber(const std::string &str, std::vector<int> *chan_number_vec);

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
inline std::vector<int> top_k_frequent(std::vector<int> &nums, int k) {
  std::map<int, int> nums_map;
  for (const auto &it : nums) {
    ++nums_map[it];
  }

  std::multimap<int, int, std::greater<int>> temp_map;
  for (const auto &it : nums_map) {
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

#if 0
inline std::string sort_string_sample(std::string s) {
  int cnt[26] = {0};
  for (auto ch : s) ++cnt[ch - 'a'];

  std::string ans;
  int n = 0;
    auto N = s.size();
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
#endif

/// @name     generate_pascal_trangel
/// @brief
///
/// @param    numRows [in]
///
/// @return
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-04 12:02:31
/// @warning  线程不安全
/// @note
/// 118. Pascal's Triangle
///   Given a non-negative integer numRows, generate the first numRows of
/// Pascal's triangle.
///   In Pascal's trangle, each number is the sum of the two number directly
/// above it.
inline std::vector<std::vector<int>> generate_pascal_trangel(int numRows) {
  std::vector<std::vector<int>> result;
  result.reserve(numRows);
  for (size_t i = 0; i < numRows; ++i) {
    std::vector<int> temp(i + 1, 1);
    for (size_t x = 0; x < temp.size(); ++x) {
      if (x != 0 && x != temp.size() - 1) {
        temp.at(x) = result.back().at(x) + result.back().at(x - 1);
      }
    }
    result.emplace_back(temp);
  }
  return result;
}

/// @name     decompress_rle_list
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-06 19:09:42
/// @warning  线程不安全
/// 1313. Decompress Run-Length Encoded List
/// We are given a list nums of integers representing a list compressed with
/// run-length encoding.
/// Consider each adjacent pair of elements
/// [freq, val] = [nums[2*i], nums[2*i+1]](with i >= 0). For each such pair,
/// there are freq elements with value val concatenated in a sublist.
/// Comcatenate all the sublists from left to right to generate the
/// decompressed list.
///
/// Return the decompressed list.
/// Example 1:
/// Input: nums = [1,2,3,4]
/// Output: [2,4,4,4]
/// Explanation: The first pair [1,2] means we have freq = 1 and val = 2 so
/// we generate the array[2].
/// The second pair [3,4] means we have freq = 1 and val = 2 so we generate
/// the array [2].
/// At the end the concatenation [2] + [4,4,4] is [2,4,4,4]
///
/// Example 2:
/// Input: nums = [1,1,2,3]
/// Output: [1,3,3]
///
/// Constraints:
/// * 2 <= nums.length <= 100
/// * nums.length % 2 == 0
/// * 1 <= nums[i] <= 100
inline std::vector<int> decompress_rle_list(std::vector<int> &nums) {
  std::vector<int> result;
  result.reserve(nums.size() * 2);
  for (auto it = nums.begin(); it != nums.end(); ++it) {
    auto temp = it;
    std::vector<int> temp_vec(*temp, *(++it));
    result.insert(result.end(), temp_vec.begin(), temp_vec.end());
  }
  return result;
}

/// @name     equations_possible
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-09 08:34:46
/// @warning  线程不安全
/// 990. Satisfiability of Equations
/// Given an array equations of strings that represent relationships between
/// variables, each string equations[i] has length 4 and takes one of two
/// different forms: "a==b" or "a!=b, Here, a and b are lowcase letters
/// (not necessarily different) that represent one-letter variable names.
/// Return true if and only if it is possible to assgin integers to varable
/// names so as to satisfy all the given equations.
/// Example 1:
///
/// Input: ["a==b","b!=a"]
/// Output: false
/// Explanation: If we assign say, a = 1 and b = 1, then the first equation is
/// satisfied, but not the second.  There is no way to assign the variables to
/// satisfy both equations. Example 2:
///
/// Input: ["b==a","a==b"]
/// Output: true
/// Explanation: We could assign a = 1 and b = 1 to satisfy both equations.
/// Example 3:
///
/// Input: ["a==b","b==c","a==c"]
/// Output: true
/// Example 4:
///
/// Input: ["a==b","b!=c","c==a"]
/// Output: false
/// Example 5:
///
/// Input: ["c==c","b==d","x!=z"]
/// Output: true
class union_find_char {
 public:
  union_find_char() {
    parent_.resize(26);
    std::iota(parent_.begin(), parent_.end(), 0);
  }

  void union_(int index1, int index2) { parent_[find(index1)] = find(index2); }

  int find(int index) {
    if (parent_[index] == index) return index;
    parent_[index] = find(parent_[index]);
    return parent_[index];
  }

 private:
  std::vector<int> parent_;
};
inline bool equations_possible(std::vector<std::string> &equations) {
  union_find_char temp;
  for (auto it = equations.begin(); it != equations.end();) {
    if ((*it)[1] == '=') {
      int index1 = (*it)[0] - 'a';
      int index2 = (*it)[3] - 'a';
      temp.union_(index1, index2);
      it = equations.erase(it);
      continue;
    }
    ++it;
  }

  for (auto it : equations) {
    if (temp.find(it[0] - 'a') == temp.find((it[3] - 'a'))) {
      return false;
    }
  }
  return true;
}

/// @name     min_window
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-09 18:52:15
/// @warning  线程不安全
/// 76. Minimum Window Substring
/// Given a string S and a string T, find the minimum window in S which
/// will contain all the characters in T in complexity O(n)
///
/// Example:
/// Input: S = "ADOBECODEBANC", T = "ABC"
/// Output: "BANC"
///
/// Note:
/// * If there is no such window in S that covers all characters in T, return
///   the empty string "".
/// * If there is such window, you are guaranteed that there will always be
///   only unique minimum window in S.
inline std::string min_window(std::string s, std::string t) {
  /// 如果子串长于输入字符串则返回空字符串
  if (s.size() < t.size()) {
    return "";
  }

  std::unordered_map<char, int> min_substring_map;  ///< 用于保存输入字符串的字母出现次数
  std::unordered_map<char, int> need_map; ///< 用于保存子字符串的字母出现次数
  for (const auto &it : t) {
    ++need_map[it];
    min_substring_map[it] = 0;
  }

  int match_count = 0;        ///< 用于保存匹配次数
  auto it_right = s.begin();  ///< 滑动窗口右窗口
  auto it_left = s.begin();   ///< 滑动窗口左窗口
  auto it_start = s.begin();  ///< 用于保存最小子串的起始位置
  ptrdiff_t min_length = INT_MAX; ///< 用于保存最小长度

  /// 滑动窗口, 右迭代器自增遍历string，获得对应字串字母的计数
  for (; it_right != s.end();) {
    if (min_substring_map.end() != min_substring_map.find(*it_right)) {
      if (need_map[*it_right] == ++min_substring_map[*it_right]) {
        ++match_count;
      }
    }

    ++it_right;
    while (match_count == need_map.size()) {
      auto current_length = std::distance(it_left, it_right);
      if (current_length < min_length) {
        min_length = current_length;
        it_start = it_left;
      }
      if (min_substring_map.end() != min_substring_map.find(*it_left)) {
        if (--min_substring_map[*it_left] < need_map[*it_left]) {
          --match_count;
        }
      }
      ++it_left;
    }
  }
  return min_length == INT_MAX ? "" : s.substr(std::distance(s.begin(), it_start), min_length);
}

}  // end of namespace Leetcode
}  // namespace Lee

#endif  // end of LEET_CODE_H__
