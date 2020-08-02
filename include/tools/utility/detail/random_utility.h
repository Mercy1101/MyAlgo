///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   random_utility.h
/// @brief  这个文件中主要放置一些生成随机数的函数
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-11 09:38:52
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_RANDOM_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_RANDOM_UTILITY_H_

#include <limits>
#include <mutex>
#include <random>
#include <string>
#include <type_traits>

#include "utility/detail/time_utility.h"  // for GetCurrentMilliSecondStamp()

namespace lee {
inline namespace utility {
inline namespace random {
/// @name     GetRandom
/// @brief    获取[0, RAND_MAX]中一个随机数。
///
/// @param    NONE
///
/// @return   [0, RAND_MAX]中一个随机数
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-01 17:12:37
/// @warning  线程不安全
inline int GetRandom() noexcept {
  static std::once_flag InitFlag;
  std::call_once(InitFlag, []() {
    srand(static_cast<unsigned>(lee::GetCurrentMilliSecondStamp()));
  });
  return rand();
}

/// @name     GetRangeRandom
/// @brief    生成[x, y]或[y,x]区间中的一个随机数。
///
/// @details  生成的最大区间为[INT_MIN, INT_MAX]
///
/// @param    x    [in]    不能输入比INT_MAX大或比INT_MIN小的数字
/// @param    y    [in]    不能输入比INT_MAX大或比INT_MIN小的数字
///
/// @return   [x, y]或[y, x]区间中的一个随机数
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-01 17:10:53
/// @warning  线程不安全
inline int GetRangeRandom(int x, int y) noexcept {
  if (x > y) std::swap(x, y);
  return (lee::GetRandom() % (y - x + 1)) + x;
}
inline int GetRangeRandom(int x) noexcept { return GetRangeRandom(0, x); }

/// @name     GetRangeRandomNumberFloat
/// @brief    等概率获取一个区间中的实数值
///
/// @param    range_bound1 [in]
/// @param    range_bound2 [in]
///
/// @return   [rang_bound1,rang_bound2]区间中的浮点数
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-07 12:07:27
/// @warning  线程不安全
template <typename T = double>
inline T GetRangeRandomNumberFloat(T range_bound1, T range_bound2) {
  static_assert(std::is_floating_point<T>() && !std::is_const<T>(),
                "GetRangeRandomNumberFloat need floating_point and "
                "non-const type Param");
  if (range_bound1 > range_bound2) std::swap(range_bound1, range_bound2);
  static std::random_device r;
  static std::default_random_engine e(r());
  std::uniform_real_distribution<T> u(range_bound1, range_bound2);
  return u(e);
}
inline double GetRangeRandomNumberFloat(double range_end) {
  return lee::GetRangeRandomNumberFloat(0.0, range_end);
}

/// @name     GetRandomRangeNumberInt
/// @brief    等概率获取一个区间中的整数值
///
/// @param    range_bound1 [in]
/// @param    range_bound2 [in]
///
/// @return   [range_bound1,range_bound2]区间中的实数
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-12 16:13:37
/// @warning  线程不安全
template <typename T = int>
inline T GetRangeRandomNumberInt(T range_bound1, T range_bound2) {
  static_assert(
      std::is_integral<T>() && !std::is_const<T>(),
      "GetRangeRandomNumberInt need integral and non-const type Param!");
  if (range_bound1 > range_bound2) std::swap(range_bound1, range_bound2);
  static std::random_device r;
  static std::default_random_engine e(r());
  std::uniform_int_distribution<T> u(range_bound1, range_bound2);
  return u(e);
}

inline int GetRangeRandomNumberInt(int range_end) {
  return lee::GetRangeRandomNumberInt(0, range_end);
}

/// @name     GetApproximationNumber
/// @brief    获取一个值的近似值，
///           这个近似值的误差区间为入参的百分比乘以基数来算出来的
///
/// @param    base_number       [in]  数值的基数
/// @param    deviation_percent [in]  浮动的百分比
///
/// @return   返回一个近似值(区间：[base_number-base_number*deviation_percent,
///                          base_number+base_number*deviation_percent])
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-06 16:47:28
/// @warning  线程安全
template <typename T>
inline T GetApproximationNumber(const T &base_number,
                                const double &deviation_percent) {
  static_assert(std::is_integral(T) || std::is_floating_point(T),
                "GetApproximationNumber need a POD type number!");
  double range_bound = base_number * deviation_percent;
  auto random_number = lee::GetRandomRangeNumberDouble(
      base_number + range_bound, base_number - range_bound);
  return static_cast<T>(random_number);
}
constexpr double DEFAULT_DEVIATION_PERCENT = 0.1;
template <typename T>
inline T GetApproximationNumber(const T &base_number) {
  lee::GetApproximationNumber(base_number, DEFAULT_DEVIATION_PERCENT);
}

/// @name     GetRandomRangeNumber_WithoutRepeat
/// @brief    生成不重复随机数并放入入参的容器，但是不保证能够生成够数目
///
/// @param    bound1  [in]  生成随机数的区间
/// @param    bound2  [in]  生成随机数的区间
/// @param    limit_number  [in]  限定最多生成几个数字
/// @param    random_number_vector  [out]  传出的随机数的容器
///
/// @return
///   @retval true  生成等于limit_number数量的不重复随机数
///   @retval false 生成了少于limit_number数量的不重复随机数
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-10 17:38:28
/// @warning
template <typename T>
bool GetRandomRangeNumber_WithoutRepeat(const T &bound1, const T &bound2,
                                        const int &limit_number,
                                        std::vector<T> *random_number_vector) {
  static_assert(std::is_integral(T),
                "GetRandomRangeNumber_WithoutRepeat need integral param!");
  random_number_vector->reserve(limit_number);
  for (int i = 0; i < limit_number; ++i) {
    random_number_vector->emplace_back(lee::GetRandomRange(bound1, bound2));
  }
  std::sort(random_number_vector->begin(), random_number_vector->end());
  auto last =
      std::unique(random_number_vector->begin(), random_number_vector->end());
  random_number_vector->erase(last, random_number_vector->end());
  /// 打乱容器
  std::random_shuffle(random_number_vector);
  /// 如果生成的数量小于限定的数值，则返回false
  return random_number_vector->size() == limit_number;
}

/// @name     RandomSequenceOfUnique
/// @brief    生成不重复随机数
///
/// @author   Lijiancong, pipinstall@163.com
///           Taken From (https://github.com/preshing/RandomSequence)
///
/// @date     2019-12-11 09:48:36
/// @warning  线程不安全
/// @note
/// This class implements a random number generator which outputs
/// a unique 32-bit integer each time it's called, and verifies that
/// the first 2^32 integers returns really are unique.
/// The algorithm is described in the blog post
/// (http://preshing.com/20121224/how-to-generate-a-sequence-of-unique-random-integers).
/// @example
/// int dump()
/// {
///     unsigned int seed = (unsigned int) time(NULL);
///     RandomSequenceOfUnique rsu(seed, seed + 1);
///     unsigned int i = 0;
///     do
///     {
///       printf("0x%08x\n", rsu.next());
///     }
///     while (++i != 0);
///   return 0;
/// }
class RandomSequenceOfUnique {
 private:
  unsigned int m_index;
  unsigned int m_intermediateOffset;

  static unsigned int permuteQPR(unsigned int x) {
    static const unsigned int prime = 4294967291u;
    if (x >= prime)
      return x;  // The 5 integers out of range are mapped to themselves.
    unsigned int residue = ((unsigned long long)x * x) % prime;
    return (x <= prime / 2) ? residue : prime - residue;
  }

 public:
  RandomSequenceOfUnique(unsigned int seedBase, unsigned int seedOffset) {
    m_index = permuteQPR(permuteQPR(seedBase) + 0x682f0161);
    m_intermediateOffset = permuteQPR(permuteQPR(seedOffset) + 0x46790905);
  }

  unsigned int next() {
    return permuteQPR((permuteQPR(m_index++) + m_intermediateOffset) ^
                      0x5bf03635);
  }
};

/// @name     GetRandomPassWord
/// @brief    获取一个确定长度的随机密码,包含数字和大小写字母
///
/// @param    length  [in]  想要获取的密码长度
///
/// @return   生成好的密码
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-16 08:54:04
/// @warning  线程不安全
inline std::string GetRandomPassWord(const int &length) {
  static const char letter[62] = {
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c',
      'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
      'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C',
      'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
      'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
  std::string password;
  for (int i = 0; i < length; ++i) {
    auto index = lee::GetRangeRandomNumberInt(62 - 1);
    password += letter[index];
  }
  return password;
}
inline std::string GetRandomPassWord() { return GetRandomPassWord(6); }

/// @name     kunth_shuffle
/// @brief    洗牌算法
///
/// @param    vec [in/out]
///
/// @return   NONE
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-08-02 15:22:28
/// @warning  线程不安全
template <typename T>
void kunth_shuffle(std::vector<T> &vec) {
  static std::random_device r;
  static std::default_random_engine e(r());
  for (size_t i = 0; i < vec.size(); ++i) {
    std::uniform_int_distribution<T> u(i, vec.size() - 1);
    std::swap(vec.at(u(e)), vec.at(i));
  }
}

/// @name     shuffle
/// @brief    std::shuffle 的实现
///
/// @param    first [in]
/// @param    end   [in]
/// @param    g     [in]  随机数引擎
///
/// @return   NONE
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-08-02 15:23:39
/// @warning  线程不安全
/// @note     URBG: UniformRandomBitGenerator
template <class RandomIt, class URBG>
void shuffle(RandomIt first, RandomIt last, URBG &&g) {
  typedef typename std::iterator_traits<RandomIt>::difference_type diff_t;
  typedef std::uniform_int_distribution<diff_t> distr_t;
  typedef typename distr_t::param_type param_t;

  distr_t D;
  diff_t n = last - first;
  for (diff_t i = n - 1; i > 0; --i) {
    using std::swap;
    swap(first[i], first[D(g, param_t(0, i))]);
  }
}

}  // namespace random
}  // namespace utility
}  // namespace lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_RANDOM_UTILITY_H_
