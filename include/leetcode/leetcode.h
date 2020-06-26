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
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace lee {
namespace leetcode {

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

  std::unordered_map<char, int>
      min_substring_map;  ///< 用于保存输入字符串的字母出现次数
  std::unordered_map<char, int> need_map;  ///< 用于保存子字符串的字母出现次数
  for (const auto &it : t) {
    ++need_map[it];
    min_substring_map[it] = 0;
  }

  int match_count = 0;             ///< 用于保存匹配次数
  auto it_right = s.begin();       ///< 滑动窗口右窗口
  auto it_left = s.begin();        ///< 滑动窗口左窗口
  auto it_start = s.begin();       ///< 用于保存最小子串的起始位置
  ptrdiff_t min_length = INT_MAX;  ///< 用于保存最小长度

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
  return min_length == INT_MAX
             ? ""
             : s.substr(std::distance(s.begin(), it_start), min_length);
}

/// 792. 匹配子序列的单词数
/// 给定字符串 S 和单词字典 words, 求 words[i] 中是 S 的子序列的单词个数。
///
/// 示例:
/// 输入:
/// S = "abcde"
/// words = ["a", "bb", "acd", "ace"]
/// 输出: 3
/// 解释: 有三个是 S 的子序列的单词: "a", "acd", "ace"。
/// 注意:
///
/// 所有在words和 S 里的单词都只由小写字母组成。
/// S 的长度在 [1, 50000]。
/// words 的长度在 [1, 5000]。
/// words[i]的长度在[1, 50]。
inline int num_matching_subseq(std::string S, std::vector<std::string> &words) {
  int count = 0;
  for (const auto &it : words) {
    size_t position = std::string::npos;
    auto c = it.begin();
    for (; c != it.end(); ++c) {
      position = S.find_first_of(*c, position + 1);
      if (position == std::string::npos) {
        break;
      }
    }
    if (c == it.end()) {
      count++;
    }
  }
  return count;
}

/// 70. 爬楼梯
/// 假设你正在爬楼梯。需要 n 阶你才能到达楼顶。
///
/// 每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？
///
/// 注意：给定 n 是一个正整数。
///
/// 示例 1：
///
/// 输入： 2
/// 输出： 2
/// 解释： 有两种方法可以爬到楼顶。
/// 1.  1 阶 + 1 阶
/// 2.  2 阶
/// 示例 2：
///
/// 输入： 3
/// 输出： 3
/// 解释： 有三种方法可以爬到楼顶。
/// 1.  1 阶 + 1 阶 + 1 阶
/// 2.  1 阶 + 2 阶
/// 3.  2 阶 + 1 阶
inline int climb_statirs_helper(std::vector<int> &vec, int n) {
  if (n == 1) {
    return 1;
  } else if (n == 2) {
    return 2;
  } else if (vec.at(n) == 0) {
    vec.at(n) =
        climb_statirs_helper(vec, n - 1) + climb_statirs_helper(vec, n - 2);
  }
  return vec.at(n);
}

inline int climb_stairs_1(int n) {
  if (n < 1) {
    return 0;
  }
  std::vector<int> vec;
  vec.resize(n + 1, 0);
  return climb_statirs_helper(vec, n);
}

inline int climb_stairs(int n) {
  std::vector<int> vec;
  vec.resize(n + 2, 0);
  vec.at(0) = 0;
  vec.at(1) = 1;
  vec.at(2) = 2;
  for (int i = 3; i < n + 1; ++i) {
    vec.at(i) = vec.at(i - 2) + vec.at(i - 1);
  }
  return vec.at(n);
}

/// 50. Pow(x, n)
/// 实现 pow(x, n) ，即计算 x 的 n 次幂函数。
///
/// 示例 1:
///
/// 输入: 2.00000, 10
/// 输出: 1024.00000
/// 示例 2:
///
/// 输入: 2.10000, 3
/// 输出: 9.26100
/// 示例 3:
///
/// 输入: 2.00000, -2
/// 输出: 0.25000
/// 解释: 2-2 = 1/22 = 1/4 = 0.25
/// 说明:
///
/// -100.0 < x < 100.0
/// n 是 32 位有符号整数，其数值范围是 [−231, 231 − 1] 。
inline double my_pow(double x, int n) {
  long long N = n;
  if (n < 0) {
    N = -static_cast<long long>(n);
  }
  double result = 1.0;
  double multi_num = x;
  while (N > 0) {
    if (N % 2 == 1) {
      result *= multi_num;
    }
    multi_num *= multi_num;
    N /= 2;
  }

  return n >= 0 ? result : 1 / result;
}

inline double my_pow1(double x, int n) {
  /// x^n = e^(n * lnx)
  double result = std::exp(n * std::log(std::abs(x)));
  return (x <= 0 && (n % 2 == 1)) ? result : -result;
}

/// 372. 超级次方
/// 你的任务是计算 ab 对 1337 取模，a 是一个正整数，b
/// 是一个非常大的正整数且会以数组形式给出。
///
/// 示例 1:
///
/// 输入: a = 2, b = [3]
/// 输出: 8
/// 示例 2:
///
/// 输入: a = 2, b = [1,0]
/// 输出: 1024
inline int super_pow_helper(int num, int pow, int mod_num) {
  int res = 1;
  while (pow != 0) {
    if (pow & 1) {
      res = res * num % mod_num;
    }
    num = num * num % mod_num;
    pow >>= 1;
  }
  return res;
}

inline int superPow(int a, std::vector<int> &b) {
  /// (ab)%c = ((a%c)(b%c)%c
  constexpr int mod_num = 1337;
  int size = static_cast<int>(b.size()) - 1;
  int result = 1;
  int helper = a % mod_num;
  for (; size >= 0; --size) {
    result = (result * super_pow_helper(helper, b.at(size), mod_num)) % mod_num;
    helper = super_pow_helper(helper, 10, mod_num) % mod_num;
  }
  return result;
}

inline int superPow_2(int a, std::vector<int> &b) {
  constexpr int mod_num = 1337;
  int res = 1;
  for (int x : b)
    res = super_pow_helper(res, 10, mod_num) * super_pow_helper(a, x, mod_num) %
          mod_num;
  return res;
}

/// 1300. 转变数组后最接近目标值的数组和
/// 给你一个整数数组 arr 和一个目标值 target
/// ，请你返回一个整数 value ，使得将数组中所有大于 value 的值变成 value
/// 后，数组的和最接近  target （最接近表示两者之差的绝对值最小）。
///
/// 如果有多种使得和最接近 target 的方案，请你返回这些整数中的最小值。
///
/// 请注意，答案不一定是 arr 中的数字。
/// 示例 1：
///
/// 输入：arr = [4,9,3], target = 10
/// 输出：3
/// 解释：当选择 value 为 3 时，数组会变成 [3, 3, 3]，和为 9 ，这是最接近 target
/// 的方案。 示例 2：
///
/// 输入：arr = [2,3,5], target = 10
/// 输出：5
/// 示例 3：
///
/// 输入：arr = [60864,25176,27249,21296,20204], target = 56803
/// 输出：11361
/// inline int find_best_value(std::vector<int> &arr, int target) {
///   std::sort(arr.begin(), arr.end());
///   auto begin = arr.begin();
///   auto end = arr.end();
///   auto mid = arr.begin();
///
///   std::set<int> offset = INX_MAX;  /// 用于保存距离目标值的差值
///   /// std::find(arr.begin(), arr.end());
///   while (mid != begin) {
///     std::advance(mid, std::distance(begin, end));
///     int result = 0;
///     std::for_each(arr.begin(), arr.end(),
///                   [&result, mid](int i) { result += std::min(*mid, i); });
///     int temp = result - target;
///     if (std::abs(temp) > std::abs(*offset.begin())) {
///       end = mid;
///       offset.insert(temp);
///     } else {
///       begin = mid;
///     }
///   }
/// }

/// 1.排序
/// 2.前缀和：eg:[3,4,9] ，其前缀和为[0,3,7,16]
/// 3.二分法：(1)定义起始区间：[1, arr_max](2)分区间：[l, mid] [mid + 1,
/// r](3)判断：若大于target则向左取，若小于等于target向右取（此处若等于target，就可之前return
/// mid) 4.getsum()表示mid的前缀和+value和，此处value = mid
/// 5.最后一步判断是为了判断l是不是最小值，eg:[3,4,9] ，其当l = 4时，有[3,4,4] =
/// 11，但当l = 3时，有[3,3,3] = 9 所以判断abs(11 - 10) < abs(9 - 11) ? 4 : 3;
/// 6.lower_bound()返回第一个大于等于value的指针

/// inline int getsum(std::vector<int> &arr, std::vector<int> &presum, int
/// value) {
///   auto iter = std::lower_bound(arr.begin(), arr.end(), value);  /// 迭代器
///   int sun = presum.at(iter - arr.begin()) +
///             (arr.end() - iter) * value;  /// 前缀和 + value和
///   return sun;
/// }
/// inline int find_best_value(std::vector<int> &arr, int target) {
///   std::sort(arr.begin(), arr.end());
///   std::vector<int> presum(arr.size() + 1, 0);
///   for (int i = 1; i <= arr.size(); i++) {
///     presum.at(i) = presum.at(i - 1) + arr.at(i - 1);  ///< 前缀和
///   }
///   int l = 1, r = arr[arr.size() - 1];  ///(1, max)
///   while (l < r) {
///     int mid = 1 + r >> 1;  // 分区间
///     getsum(arr, presum, mid) < target ? l = mid + 1 : r = mid;
///   }
///   return std::abs(getsum(arr, presum, l)-target < std::abs(getsum(arr,
///   presum, l - 1) - target)) ? l:l-1;
/// }

inline std::vector<int> twoSum(std::vector<int> &nums, int target) {
  std::vector<decltype(nums.begin())> itor;
  for (auto it = nums.begin(); it != nums.end(); ++it) {
    itor.push_back(it);
  }
  std::sort(itor.begin(), itor.end(),
            [](decltype(nums.begin()) x, decltype(nums.begin()) y) {
              return *x < *y;
            });
  auto it_left = itor.begin();
  auto it_right = std::prev(itor.end());
  while (it_left <= it_right) {
    auto result = **it_left + **it_right;
    if (result < target) {
      ++it_left;
    } else if (result > target) {
      --it_right;
    } else {
      std::vector<int> temp{
          static_cast<int>(std::distance(nums.begin(), *it_left)),
          static_cast<int>(std::distance(nums.begin(), *it_right))};
      return temp;
    }
  }
  std::vector<int> temp{-1, -1};
  return temp;
}

/// 编写一个函数来查找字符串数组中的最长公共前缀。
///
/// 如果不存在公共前缀，返回空字符串 ""。
///
/// 示例 1:
///
/// 输入: ["flower","flow","flight"]
/// 输出: "fl"
/// 示例 2:
///
/// 输入: ["dog","racecar","car"]
/// 输出: ""
/// 解释: 输入不存在公共前缀。
/// 说明:
///
/// 所有输入只包含小写字母 a-z 。
inline std::string longest_common_prefix(std::vector<std::string> &strs) {
#undef min
  if (strs.empty()) {
    return "";
  }
  std::string prefix = *strs.begin();

  for (auto it = std::next(strs.begin()); it != strs.end(); ++it) {
    int index = 0;
    int length = static_cast<int>(std::min(prefix.size(), it->size()));
    while (index < length && prefix.at(index) == it->at(index)) {
      index++;
    }
    prefix = strs.at(0).substr(0, index);
    if (prefix.empty()) {
      return "";
    }
  }
  return prefix;
}

/// 121. 买卖股票的最佳时机
/// 给定一个数组，它的第 i 个元素是一支给定股票第 i 天的价格。
///
/// 如果你最多只允许完成一笔交易（即买入和卖出一支股票一次），设计一个算法来计算你所能获取的最大利润。
///
/// 注意：你不能在买入股票前卖出股票。
///
///  
///
/// 示例 1:
///
/// 输入: [7,1,5,3,6,4]
/// 输出: 5
/// 解释: 在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 =
/// 6）的时候卖出，最大利润 = 6-1 = 5 。
///      注意利润不能是 7-1 = 6,
///      因为卖出价格需要大于买入价格；同时，你不能在买入前卖出股票。
/// 示例 2:
///
/// 输入: [7,6,4,3,1]
/// 输出: 0
/// 解释: 在这种情况下, 没有交易完成, 所以最大利润为 0。
inline int maxProfit(std::vector<int> &prices) {
  if (prices.empty()) {
    return 0;
  }
  int min = prices.front();
  int result = 0;
  for (auto it : prices) {
    result = std::max(result, it - min);
    min = std::min(it, min);
  }
  return result;
}

/// 单调栈
inline int maxProfit1(std::vector<int> &prices) {
  prices.push_back(-1);
  int result = 0;
  std::vector<int> increase;
  for (auto it : prices) {
    if (!increase.empty() && increase.back() > it) {
      result = std::max(result, increase.back() - increase.front());
      increase.pop_back();
    }
    increase.push_back(it);
  }
}

/// dp 区间和等于求差问题
/// 区间和可以转换成求差的问题，求差问题，也可以转换成区间和的问题。
/// dp[i]=max(0,dp[i−1])
inline int maxProfit_dp1(std::vector<int> &prices) {
  int last = 0;
  int profit = 0;

  for (int i = 0; i < prices.size() - 1; ++i) {
    last = std::max(0, last + prices[i + 1] - prices[i]);
    profit = std::max(profit, last);
  }
  return profit;
}
inline int maxProfit_dp2(std::vector<int> &prices) {
  auto n = prices.size();
  if (n == 0) return 0;  // 边界条件
  int minprice = prices[0];
  std::vector<int> dp(n, 0);

  for (int i = 1; i < n; i++) {
    minprice = std::min(minprice, prices[i]);
    dp[i] = std::max(dp[i - 1], prices[i] - minprice);
  }
  return dp[n - 1];
}

/// 给定一个数组，它的第 i 个元素是一支给定股票第 i 天的价格。
///
/// 设计一个算法来计算你所能获取的最大利润。你可以尽可能地完成更多的交易（多次买卖一支股票）。
///
/// 注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。
///
///  
///
/// 示例 1:
///
/// 输入: [7,1,5,3,6,4]
/// 输出: 7
/// 解释: 在第 2 天（股票价格 = 1）的时候买入，在第 3 天（股票价格 =
/// 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。      随后，在第 4
/// 天（股票价格 = 3）的时候买入，在第 5 天（股票价格 = 6）的时候卖出,
/// 这笔交易所能获得利润 = 6-3 = 3 。 示例 2:
///
/// 输入: [1,2,3,4,5]
/// 输出: 4
/// 解释: 在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 =
/// 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。      注意你不能在第 1
/// 天和第 2 天接连购买股票，之后再将它们卖出。  
/// 因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。 示例 3:
///
/// 输入: [7,6,4,3,1]
/// 输出: 0
/// 解释: 在这种情况下, 没有交易完成, 所以最大利润为 0。
///  
///
/// 提示：
///
/// 1 <= prices.length <= 3 * 10 ^ 4
/// 0 <= prices[i] <= 10 ^ 4
inline int max_profit(std::vector<int> &prices) {
  if (prices.size() < 2) {
    return 0;
  }

  int result = 0;
  for (int j = 1; j < prices.size(); ++j) {
    result += std::max(prices.at(j) - prices.at(j - 1), 0);
  }
  return result;
}

/// 给定正整数数组 A，A[i] 表示第 i 个观光景点的评分，并且两个景点 i
/// 和 j 之间的距离为 j - i。
///
/// 一对景点（i < j）组成的观光组合的得分为（A[i] + A[j] + i -
/// j）：景点的评分之和减去它们两者之间的距离。
///
/// 返回一对观光景点能取得的最高分。
///
/// 示例：
///
/// 输入：[8,1,5,2,6]
/// 输出：11
/// 解释：i = 0, j = 2, A[i] + A[j] + i - j = 8 + 5 + 0 - 2 = 11
///
/// 提示：
///
/// 2 <= A.length <= 50000
/// 1 <= A[i] <= 1000
inline int maxScoreSightseeingPair(std::vector<int> &A) {
  int result = 0;
  int mx = A.front();
  for (int j = 1; j < A.size(); ++j) {
    result = std::max(result, mx + A.at(j) - j);
    mx = std::max(mx, A.at(j) + j);
  }
  return result;
}

/// 2. 两数相加
/// 给出两个 非空
/// 的链表用来表示两个非负的整数。其中，它们各自的位数是按照 逆序 的方式存储的，并且它们的每个节点只能存储 一位 数字。
///
/// 如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和。
///
/// 您可以假设除了数字 0 之外，这两个数都不会以 0 开头。
///
/// 示例：
///
/// 输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)
/// 输出：7 -> 0 -> 8
/// 原因：342 + 465 = 807
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(nullptr) {}
};
inline ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
  std::string num1;
  std::string num2;
  while (l1 != nullptr) {
    num1 += std::to_string(l1->val);
    l1 = l1->next;
  }

  while (l2 != nullptr) {
    num2 += std::to_string(l2->val);
    l2 = l2->next;
  }
  std::reverse(num1.begin(), num1.end());
  std::reverse(num2.begin(), num2.end());
  auto result = std::to_string(std::stoll(num1) + std::stoll(num2));
  ListNode *ans = new ListNode(0);
  auto temp = ans;
  auto it = result.rbegin();
  while (true) {
    std::string x;
    x += *it;
    temp->val = std::stoi(x);
    if (++it != result.rend()) {
      temp->next = new ListNode(0);
      temp = temp->next;
    } else {
      break;
    }
  }
  return ans;
}

inline void rotate(std::vector<int> &nums, int k) {
  auto size = nums.size();
  auto it_end = nums.end();
  auto it = nums.end();
  it -= k % size;
  nums.insert(nums.begin(), it, it_end);
  auto temp = nums.end();
  auto it_temp = nums.end();
  it_temp -= k % size;
  nums.erase(it_temp, temp);
}

inline void rotate_1(std::vector<int> &nums, int k) {
  std::rotate(nums.begin(), nums.end() - k % nums.size(), nums.end());
}

inline bool containsDuplicate(std::vector<int> &nums) {
  std::map<int, int> map;
  for (const auto &it : nums) {
    if (++map[it] > 1) {
      return true;
    }
  }
  return false;
}

inline std::vector<int> intersect(std::vector<int> &nums1,
                                  std::vector<int> &nums2) {
  std::vector<int> result;
  std::sort(nums1.begin(), nums1.end());
  std::sort(nums2.begin(), nums2.end());
  std::set_intersection(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(),
                        std::back_inserter(result));
  return result;
}

inline std::vector<int> plusOne(std::vector<int> &digits) {
  if (digits.empty()) {
    return digits;
  }
  auto i = std::prev(digits.end());
  while (++(*i) / 10 != 0) {
    *i = (*i) % 10;
    if (i == digits.begin()) {
      digits.insert(digits.begin(), 1);
      return digits;
    } else {
      --i;
    }
  }
  return digits;
}

inline void moveZeroes(std::vector<int> &nums) {
  auto it_temp = nums.begin();
  for (auto it = nums.begin(); it != nums.end(); ++it) {
    if (*it != 0) {
      std::swap(*it, *it_temp);
      ++it_temp;
    }
  }
}

inline bool isPalindrome(std::string s) {
  for (auto it = s.begin(); it != s.end();) {
    if (std::isupper(*it)) {
      *it = static_cast<char>(std::tolower(*it));
      ++it;
      continue;
    }
    if (std::isalpha(*it) || std::isdigit(*it)) {
      ++it;
    } else {
      it = s.erase(it);
    }
  }
  if (s.empty()) {
    return true;
  }
  std::string temp(s);
  std::reverse(temp.begin(), temp.end());
  return temp == s;
}

inline int myAtoi(std::string str) {
  str.erase(0, str.find_first_not_of(' '));
  if (str.front() == '+') {
    if (std::isdigit(*std::next(str.begin()))) {
      str.erase(str.begin());
    } else {
      return 0;
    }
  }
  int flag = 1;
  if (str.front() == '-') {
    if (!std::isdigit(*std::next(str.begin()))) {
      return 0;
    } else {
      str.erase(str.begin());
      flag = -1;
    }
  }
  auto it_end = str.end();
  for (auto it = str.begin(); it != str.end(); ++it) {
    if (!std::isdigit(*it)) {
      it_end = it;
      break;
    }
  }

  std::string num(str.begin(), it_end);
  if (num.empty()) {
    return 0;
  }
  long long result = 0;
  for (int i = 0; i < num.size(); ++i) {
    result += num.at(i) - '0';

    if (result > INT_MAX && (flag == 1)) {
      return INT_MAX;
    }
    if (result * flag < INT_MIN && (flag == -1)) {
      return INT_MIN;
    }
    if (i == num.size() - 1) {
      break;
    }
    result *= 10;
  }
  return static_cast<int>(result * flag);
}

inline int myAtoi1(std::string str) {
  size_t len = str.length();
  if (len == 0) return 0;
  int index = 0;
  int num = 0;
  int flag = 1;
  bool flag_set = false;
  bool num_set = false;
  while (index < len) {
    if (str[index] == ' ' && !num_set && !flag_set) {
      index++;
      continue;
    } else if (str[index] == '+' && !num_set && !flag_set) {
      flag = 1;
      flag_set = true;
      index++;
      continue;
    } else if (str[index] == '-' && !num_set && !flag_set) {
      flag_set = true;
      flag = -1;
      index++;
      continue;
    }

    if (str[index] >= '0' && str[index] <= '9') {
      int temp = str[index] - '0';

      if (flag == 1 &&
          (num > INT_MAX / 10 || num == INT_MAX / 10 && temp > 7)) {
        return INT_MAX;
      }
      if (flag == -1 &&
          (num < INT_MIN / 10 || num == INT_MIN / 10 && temp > 8)) {
        return INT_MIN;
      }
      num = num * 10 + flag * temp;
      if (!num_set) {
        num_set = true;
      }

      index++;
    } else {
      break;
    }
  }

  return num;
}

class automaton {
 public:
  int sign_ = 1;
  long long ans_ = 0;

  void get(const char &c) {
    state_ = table[state_][get_col(c)];
    if (state_ == "in_number") {
      ans_ = ans_ * 10 + c - '0';
      ans_ = sign_ == 1 ? std::min(ans_, static_cast<long long>(INT_MAX))
                        : std::min(ans_, -(long long)INT_MIN);
    } else if (state_ == "singed") {
      sign_ = c == '+' ? 1 : -1;
    }
  }

 private:
  std::string state_ = "state";
  std::unordered_map<std::string, std::vector<std::string>> table = {
      {"start", {"start", "signed", "in_number", "end"}},
      {"signed", {"end", "end", "in_number", "end"}},
      {"in_number", {"end", "end", "in_number", "end"}},
      {"end", {"end", "end", "end", "end"}},
  };

  int get_col(const char &c) {
    if (std::isspace(c)) {
      return 0;
    }
    if (c == '+' || c == '-') {
      return 1;
    }
    if (std::isdigit(c)) {
      return 2;
    }
    return 3;
  }
};
inline int myAtoi2(std::string str) {
  automaton automaton;
  for (char c : str) {
    automaton.get(c);
  }
  return static_cast<int>(automaton.sign_ * automaton.ans_);
}

inline int strStr(std::string haystack, std::string needle) {
  if (needle.empty()) {
    return 0;
  }
  if (haystack.empty()) {
    return -1;
  }

  auto result = haystack.find(needle);
  if (result == std::string::npos) {
    return -1;
  } else {
    return static_cast<int>(result);
  }
}

inline std::vector<int> runningSum(std::vector<int> &nums) {
  if (nums.empty() || nums.size() == 1) {
    return nums;
  }
  for (auto i = std::next(nums.begin()); i != nums.end(); ++i) {
    *i = *i + *std::prev(i);
  }
  return nums;
}

inline std::string longestCommonPrefix(std::vector<std::string> &strs) {
  if (strs.empty()) {
    return "";
  }
  auto prefix = strs.front();
  for (auto it = std::next(strs.begin()); it != strs.end(); ++it) {
    size_t i = 0;
    size_t length = std::min(prefix.size(), it->size());
    while (i < length) {
      if (prefix.at(i) != it->at(i)) {
        break;
      }
      i++;
    }
    prefix = it->substr(0, i);
    if (prefix.empty()) {
      return prefix;
    }
  }
  return prefix;
}
inline ListNode *removeNthFromEnd(ListNode *head, int n) {
  if (head == nullptr || n == 0) {
    return head;
  }
  if (head->next == nullptr && n == 1) {
    return nullptr;
  }
  auto fast_ptr = head;
  auto slow_ptr = head;
  while (fast_ptr != nullptr) {
    if (n + 1 > 0) {
      --n;
      fast_ptr = fast_ptr->next;
      continue;
    } else {
      fast_ptr = fast_ptr->next;
      slow_ptr = slow_ptr->next;
      continue;
    }
  }
  if (slow_ptr->next == nullptr) {
    slow_ptr = nullptr;
  } else {
    slow_ptr->val = slow_ptr->next->val;
    slow_ptr->next = slow_ptr->next->next;
  }
  return head;
}

}  // namespace leetcode
}  // namespace lee

#endif  // end of LEET_CODE_H__
