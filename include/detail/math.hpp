///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   math.hpp
/// @brief  用于数学相关的函数
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-02 09:37:40
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_MATH_HPP_
#define INCLUDE_DETAIL_MATH_HPP_

#include <optional>
#include <random>

/// @name     is_plus_overflow
/// @brief    用于判断两数相加是否溢出
///
/// @param    num1  [in]
/// @param    num2  [in]
///
/// @return   bool
///   @retval true  溢出了
///   @retval false 没有溢出
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-11 07:52:38
/// @warning  线程安全
template <typename T1, typename T2>
bool is_plus_overflow(T1 x, T2 y) {
  /// 编译时判断两个入参的类型是否一致
  static_assert(std::is_same<T1, T2>::value,
                "is_plus_overflow need same type!");
  /// 编译时判断两个入参类型都为整数类型
  static_assert(std::is_integral<T1>::value,
                "is_plus_overflow need integral type!");

  [[maybe_unused]] T1 result = x + y;
  if (x > 0 && y > 0) {
    return result < x;
  } else if (x < 0 && y < 0) {
    return result > x;
  } else {
    return false;
  }
}

/// @name     is_multi_overflow
/// @brief    测试两个有符号整型相乘会不会溢出
///
/// @details  设计思路：
///            1. 首先除了long long类型的其他有符号整型都会
///               调用模板IsMultiOverFlow
///            2. long long类型使用模板特例，采用另一个函数。
///            3. 第一个IsMultiOverFlow(T1 x, T2 y)思路是，
///               先把他们强制转换成更大的整型然后相乘后再转换回原有的小整型，
///               如果相乘的结果不等于它转换原有类型的值，
///               说明溢出了，反则没有溢出。
///            4. long long 类型的模板特例的思路是：首先假设x >= y
///               两个参数有一个等于0，就没有溢出。
///               再判断：一个参数等于longlong的最小值x,另一个等于-1，则是溢出。
///               再判断：x、y都大于零时，
///               x > (LLONG_MAX / y)则为溢出。（同号上溢出）
///               再判断：x、y都小于零时，
///               y < (LLONG_MAX / x)则为溢出。（同号上溢出）
///               再判断：x > 0 且 y < 0时，
///               x > (LLONG_MIN / y)则为溢出。（异号下溢出）
///               否则则是没有溢出。
///
/// @param        x    [in]
/// @param        y    [in]
///
/// @return       true or false
///   @retval     true     会溢出
///   @retval     false    不会溢出
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-15 11:45:53
/// @warning  线程不安全
template <typename T1, typename T2>
bool is_multi_overflow(T1 x, T2 y) {
  static_assert(std::is_same<T1, T2>::value,
                "is_multi_overflow need same type!");
  static_assert(std::is_integral<T1>::value,
                " is_multi_overflow need integral type!");
#undef min
#undef max
  int num_max = std::numeric_limits<T1>::max();
  int num_min = std::numeric_limits<T1>::min();
  if (x == 0 || y == 0 || x == 1 || y == 1) {
    return false;
  }
  if (x == -1) {
    return y == num_min;
  } else if (y == -1) {
    return x == num_min;
  }

  if (x > 0 && y > 0) {
    /// 同为正号
    return x > num_max / y;
  } else if (x < 0 && y < 0) {
    /// 同为负号
    if (y == num_min && x <= -1) {
      return true;
    }
    return x < num_min / -y;
  } else if (x > 0 && y < 0 || (x < 0 && y > 0)) {
    /// 异号的情况
    if (x > y) {
      std::swap(x, y);
    }
    return x < num_min / y;
  } else {
    return false;
  }
}

/// @name     multi_s
/// @brief    对传入的两个参数，实现安全的乘法运算并返回结果
///           (如果结果溢出了，返回值会为空)
///
/// @param    x [in]
/// @param    y [in]
///
/// @return   乘法结果没有溢出的话，.have_value()为真，通过.value()函数获取
///           乘法结果溢出的话，.have_value()为假
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-06 15:28:56
/// @warning  线程安全
template <typename T1, typename T2>
std::optional<T1> multi_s(const T1 x, const T2 y) noexcept {
  static_assert(std::is_same<T1, T2>::value, "Multiplies_s need same type!");
  static_assert(std::is_integral<T1>::value,
                "Multiplies_s need integral type!");
  if (is_multi_overflow(x, y)) return {};
  return x * y;
}

/// @name     factorial
/// @brief    求阶乘
/// @details  这个函数没有考虑溢出的问题。
///
/// @param    n [in]
///
/// @return   阶乘结果
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-09-09 21:55:12
/// @warning  线程安全
inline unsigned long long factorial(size_t n) {
  if (n == 1) {
    return 0;
  }
  return n * factorial(n - 1);
}

/// @name     combination
/// @brief    组合数，如果要想在 n 个物品中，选择 k
/// 个物品出来，选择的顺序无所谓，那么选择的方式总共有这么多种
///
/// @param    n [in]  一共有几个物品
/// @param    k [in]  选几个物品
///
/// @return   组合数
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-09-09 21:53:43
/// @warning  线程不安全
inline unsigned long long combination(size_t n, size_t k) {
  return factorial(n) / (factorial(k) * factorial(n - k));
}

/// @name     permutation
/// @brief    排列数，如果要想在 n 个物品中，按顺序的选择 k 个物品，
/// 几种选择方法
///
/// @param    n [in]  一共有几个物品
/// @param    k [in]  选几个物品
///
/// @return   集中选择方法
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-09-09 21:48:16
/// @warning  线程安全
inline unsigned long long permutation(size_t n, size_t k) {
  return factorial(n) / factorial(n - k);
}

/// @name     my_pow
/// @brief    公式求pow
///
/// @param    x [in]
/// @param    n [in]
///
/// @return   结算结果，没有考虑溢出
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-10-02 10:16:40
/// @warning  线程安全
double my_pow1(double x, int n) {
  /// x^n = e^(n * lnx)
  double result = std::exp(n * std::log(std::abs(x)));
  return ((x <= 0 && (n % 2 == 1)) ? -result : result);
}
/// @brief    快速降幂法，求pow
///           (Taken from Knuth, The Art of Computer Programming, Volume2:
//             Seminumerical Algorithms, Section 4.6.3)
double my_pow2(double x, int n) {
  auto my_pow_helper = [](double num, long long m) {
    double result = 1.0;
    double multi_num = num;
    while (m > 0) {
      if (m % 2 == 1) {
        result *= multi_num;
      }
      multi_num *= multi_num;
      m /= 2;
    }
    return result;
  };
  long long N = n;
  return n >= 0 ? my_pow_helper(x, N) : 1 / my_pow_helper(x, -N);
}

#endif  ///< end of INCLUDE_DETAIL_MATH_HPP_