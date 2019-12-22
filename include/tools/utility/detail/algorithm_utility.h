/// Copyright (c) 2019 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.

///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
///
/// @file   algorithm_utility.h
/// @brief  放置一些算法的简单实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-11-07 09:40:44
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_ALGORITHM_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_ALGORITHM_UTILITY_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <mutex>
#include <string>
#include <vector>


namespace Lee {
inline namespace utility {
inline namespace algorithm {

/**
@brief 一个用来查找字符串首个匹配子串的位置的静态类，只有一个接口Search()
*/
class KMP {
 public:
  /// @name     Search
  /// @brief    查找字符串首个匹配子串的位置
  ///
  /// @param    strText      [in]    要查询的内容
  /// @param    strSubText   [in]    要匹配的子串
  /// @param    pos          [out]   返回子串的首次出现的位置
  ///
  /// @return               true or false
  ///   @retval             true     找到子串位置
  ///   @retval             false    没找到子串位置或入参不正
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-11-07 09:22:52
  /// @warning  线程安全
  static bool Search(const std::string& strText, const std::string& strSubText,
                     size_t& pos) {
    if (strText.empty() || strSubText.empty() ||
        strText.size() < strSubText.size())
      return false;
    const auto&& next = kmp_next(strSubText);
    size_t tp = 0;
    size_t mp = 0;
    for (tp = 0; tp < strText.size(); tp++) {
      while (strText.at(tp) != strSubText.at(mp) && mp) mp = next[mp - 1];
      if (strText.at(tp) == strSubText.at(mp)) mp++;

      if (mp == strSubText.size()) {
        pos = tp - mp + 1;
        return true;
      }
    }
    return (tp != strText.size());
  }

 private:
  KMP() = delete; /** 不可被实例化 */

  static const std::vector<size_t>&& kmp_next(const std::string& strSubText) {
    static std::mutex Mutex;  ///< 保证静态变量next的线程安全
    std::lock_guard<std::mutex> Lock(Mutex);

    static std::vector<size_t> next(strSubText.size());
    next.at(0) = 0;
    size_t temp = 0;
    for (size_t i = 1; i < next.size(); i++) {
      temp = next.at(i - 1);
      while (strSubText.at(i) != strSubText.at(temp) && temp > 0)
        temp = next.at(temp - 1);
      if (strSubText.at(i) == strSubText.at(temp))
        next.at(i) = temp + 1;
      else
        next.at(i) = 0;
    }
    return std::move(next);
  }
};

/// @name     power
/// @brief    求某个整型数值的乘幂(x^n),编译期计算不影响程序速度
///
/// @param    x   [in]    乘幂的底
/// @param    n   [in]    乘幂的指数(必须大于等于0)
///
/// @return   计算的结果
///
/// @author   Lijiancong, pipinstall@163.com
///           (Taken from Knuth, The Art of Computer Programming, Volume2:
//             Seminumerical Algorithms, Section 4.6.3)
///
/// @date     2019-11-04 08:23:09
/// @warning  线程安全
///
/// @note     TODO: 注意这个函数没有考虑溢出的问题，要是类型T无法容纳结果，
///                 那么将会产生一个溢出后的值，将会不正确
///           TODO: 注意这个函数不支持负数次幂，以后改正
template <typename T, typename Integer>
constexpr typename std::enable_if<std::is_integral<Integer>::value, T>::type
power(T x, Integer n) {
  assert(!(x == 0 && n == 0) && "0 can't power by 0");
  assert((n >= 0) && "power can't support negative number power");
  T y = 1;  // Should be "T y{1};"
  if (n == 0) return y;
  while (true) {
    if (n % 2 == 1) {
      y = x * y;
      if (n == 1) return y;
    }
    n = n / 2;
    x = x * x;
  }
  return y;
}

/// @name     is_palindrome
/// @brief    判断一个区间是不是回文
///
/// @param    begin   [in]    区间的开始
/// @param    end     [in]    区间的结束
/// @param    p       [in]    谓词（做相等判断）
///
/// @return   是或不是回文
///
/// @author   Lijiancong, pipinstall@163.com
///           (taken from boost/algorithm/is_palindrome.hpp)
/// @date     2019-11-29 11:40:27
/// @warning  线程不安全
template <typename BidirectionalIterator, typename Predicate>
bool is_palindrome(BidirectionalIterator begin, BidirectionalIterator end,
                   Predicate p) {
  if (begin == end) {
    return true;
  }

  --end;
  while (begin != end) {
    if (!p(*begin, *end)) {
      return false;
    }
    ++begin;
    if (begin == end) {
      break;
    }
    --end;
  }
  return true;
}
template <typename BidirectionalIterator>
bool is_palindrome(BidirectionalIterator begin, BidirectionalIterator end) {
  return is_palindrome(
      begin, end,
      std::equal_to<
          typename std::iterator_traits<BidirectionalIterator>::value_type>());
}

/**
 * the quick-sort partition routine
 */
template <typename T>
static int partition_(T list[], int begin, int end) {
  int pivot_idx = RANDOM(begin, end);
  T pivot = list[pivot_idx];
  swap(list[begin], list[pivot_idx]);

  int i = begin + 1;
  int j = end;

  while (i <= j) {
    while ((i <= end) && (list[i] <= pivot)) i++;
    while ((j >= begin) && (list[j] > pivot)) j--;
    if (i < j) std::swap(list[i], list[j]);
  }

  std::swap(list[begin], list[j]);
  return j;  // final pivot position
}

template <typename T>
static void quicksort(T list[], int begin, int end) {
  if (begin < end) {
    int pivot_idx = partition_<T>(list, begin, end);
    quicksort(list, begin, pivot_idx - 1);
    quicksort(list, pivot_idx + 1, end);
  }
}

}  // namespace algorithm
}  // namespace utility
}  // end of namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_ALGORITHM_UTILITY_H_
