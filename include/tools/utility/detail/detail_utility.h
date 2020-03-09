///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   detail_utility.h
/// @brief  比较杂的工具集
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-11 19:26:47
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_DETAIL_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_DETAIL_UTILITY_H_

#include <stdio.h>
#include <stdlib.h>  // for RAND_MAX
#include <stdlib.h>  // for exit()
#include <string.h>  // for strnlen()

#include <cassert>
#include <chrono>
#include <ctime>
#include <functional>
#include <limits>
#include <mutex>
#include <optional>
#include <random>
#include <string>
#include <thread>
#include <type_traits>

#include "log/log.h"
#include "utility/detail/random_utility.h"

namespace Lee {
inline namespace utility {
inline namespace detail {
/// @name     ArraySize
/// @brief    得出入参数组的长度(如int a0[5]; 5 == ArraySize(a0);)
///
/// @details  之所以要再实现一个ARRAYSIZE(),是因为ARRAYSIZE作为宏定义
///           无法在编译期间识别数组和指针的区别。尤其在数组传入函数时，
///           数组参数降级为指针，使用宏定义实现的ARRAYSIZE将会固定返回1，
///           而不是正确的数组长度。当使用该函数时，在数组传入函数时，
///           编译期间会报出一个模板无法编译的错误，
///           从而避免了数组参数降级为指针所引发的错误
///
/// @param    匿名参数(但类型要求为任意类型数组)
///
/// @return   数组的长度
///
/// @author   Lijiancong, pipinstall@163.com
///           Token From Scott Meyers <Effective Modern C++>
///           条款一：理解模板型别推到
///
/// @date     2019-12-01 18:48:08
/// @warning  线程不安全
/// @note     用法如ARRAY_SIZE(a)一样
///           只有原生数组可以使用该函数，std::array、std::string、
///           std::vector等容器不可以调用该函数
template <class T, std::size_t N>
constexpr inline std::size_t ArraySize(T (&)[N]) noexcept {
  return N;
}

/// 我也见过 github.com/google/benchmark/src/arraysize.h 中的另一种写法
// The arraysize(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.  If you use arraysize on
// a pointer by mistake, you will get a compile-time error.
//
// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

// That gcc wants both of these prototypes seems mysterious. VC, for
// its part, can't decide which to use (another mystery). Matching of
// template overloads: the final frontier.
#ifndef COMPILER_MSVC
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(::Lee::ArraySizeHelper(array)))

/// @name     ignore_unused
/// @brief    使用该空函数可以屏蔽编译器对未使用过的变量的警告
///
/// @param    模板变参列表，这意味着括号里可以填写任意类型，任意数量的变量。
///
/// @return   NONE
/// @author   Lijiancong, pipinstall@163.com
///           (Taken from boost boost/core/ignore_unused.hpp)
/// @date     2019-12-01 18:50:59
/// @warning  线程不安全
/// @note     当编译器警告有未使用变量时，首选应该删除未使用变量，
///           只有当必须留存该变量时使用该函数
///
/// @example  如想要屏蔽编译器的这句警告：
///           2>F:\1WorkStation\test\src\main.cpp(53,9): warning C4101:
///             “i”: 未引用的局部变量
///           在定义变量i的下面写上这句话：
///           Lee::ignore_unused(i);
template <typename... Ts>
inline constexpr void ignore_unused(Ts const &...) {}
template <typename... Ts>
inline constexpr void ignore_unused() {}

/// @name     checked_delete
/// @brief    完全可以替代关键字delete,比delete多了编译期检查不完整类型的检查。
///
/// @details  sizeof关键字在对不完整类型运算时会返回0，
///           所以在type_must_be_complete会被定义为索引为-1的数组。
///           编译时不会允许有数组的索引为-1，所以会产生一个编译期错误，
///           避免对不完整类型进行delete。
///
/// @param    x    [in]    要删除的指针指向的对象
/// @return
/// @author   Lijiancong, pipinstall@163.com
///           (Taken from boost boost/core/checked_delete.hpp)
/// @date     2019-10-17 16:58:43
/// @warning  线程安全
/// @note     这个函数不能用于指向数组指针的delete，
///           数组指针应使用函数checked_array_delete
template <class T>
inline void checked_delete(T *x) {
  // intentionally complex - simplification causes regressions
  typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
  (void)sizeof(type_must_be_complete);
  delete x;
}

/// @name     checked_array_delete
/// @brief    完全可以替代关键字delete[],
///           比delete[]多了编译期检查不完整类型的检查。
///
/// @param    x    [in]    要删除的指针指向的对象
///
/// @return   NONE
/// @author   Lijiancong, pipinstall@163.com
///           (Taken from boost boost/core/checked_delete.hpp)
/// @date     2019-12-01 19:25:24
/// @warning  线程安全
/// @note     这个函数不能用于删除指向非数组指针的对象,
///           非数组指针的对象的删除用函数checked_delete
template <class T>
inline void checked_array_delete(T *x) {
  typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
  (void)sizeof(type_must_be_complete);
  delete[] x;
}

/// @name     checked_deleter
/// @brief    函数对象类型的checked_delete，
///           为了适配STL函数而设计，功能等价于checked_delete
///
/// @param    x    [in]    要删除的指针指向的对象
///
/// @return   NONE
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-01 19:26:43
/// @warning  线程安全
/// @note     注意：不可以对数组指针使用
struct checked_deleter {
  typedef void result_type;

  template <class T>
  void operator()(T *x) const {
    Lee::checked_delete(x);
  }
};

/// @name     checked_array_deleter
/// @brief    函数对象类型的checked_array_delete，
///           为了适配STL函数而设计，功能等价于checked_array_delete

/// @param    x    [in]    要删除的指针指向的对象
/// @return
/// @author   Lijiancong, pipinstall@163.com
///           (Taken from boost boost/core/checked_delete.hpp)
/// @date     2019-10-18 13:49:46
/// @warning  线程不安全
/// @note     注意：不可以对非数组指针使用
struct checked_array_deleter {
  typedef void result_type;

  template <class T>
  void operator()(T *x) const {
    Lee::checked_array_delete(x);
  }
};

/// @name     implicit_cast
/// @brief    用来对派生类对基类的转换(up-cast),
///           反过来基类到派生类的转换(down-cast)会编译错误
///
/// @details  作用与static_cast几乎相同但多了对隐式转换的判断.
///           对于不可以隐式转换的类型(down-cast)会编译期报错,
///           对于可以隐式转换的类型(up-cast)作用与static_cast相同
///
/// @param    x   [in/out]
///
/// @return   转换后的类型
///
/// @author   Lijiancong, pipinstall@163.com
///           (Taken from boost boost/implicit_cast.hpp)
/// @date     2019-10-31 09:01:33
/// @warning  线程不安全
/// @note     1. 用法可见:doc/TechnicalSummary/Technical Summary.pdf中小节:
///              基类与派生类之间的转换规则
///           2. static_cast与implicit_cast的区别:
///              You can down-cast with static_cast. Not so with implicit_cast.
///              static_cast basically allows you to do any implicit conversion,
///              and in addition the reverse of any implicit conversion
///              (up to some limits. you can't downcast if there is
///              a virtual base-class involved).
///              But implicit_cast will only accept implicit conversions.
///              no down-cast, no void*->T*, no U->T if T has only explicit
///              constructors for U.
///              引用自(https://stackoverflow.com/questions/868306/what-is-the-difference-between-static-cast-and-implicit-cast)
///           3. 关于更多信息可以参考下面这篇文章:
///              《c++小技巧(三)更好的类型转换implicit_cast和down_cast》
///              https://blog.csdn.net/xiaoc_fantasy/article/details/79570788
template <class T>
struct icast_identity {
  typedef T type;
};  ///< 用于implicit_cast函数参数的类型推导

template <typename T>
inline T implicit_cast(typename icast_identity<T>::type x) {
  return x;
}

/// @name     down_cast
/// @brief    用于替代dynamic_cast<>()关键字转换基类指针到派生类指针的情况
///
/// @details  相较于dynamic_cast,
///           优点:编译期抛出错误或assert抛出错误,调用方便(不用手动写判断空指针)
///           缺点: 只能对指针进行检查，无法对引用进行检查。
///
/// @param    f   [in]    只接受指针类型
///
/// @return   转换后的指针
/// @author   Lijiancong, pipinstall@163.com
///           (Taken from google-protobuf/stubs/common.h)
/// @date     2019-10-31 14:30:04
/// @warning  线程安全
template <typename To, typename From>
inline To CheckCast(From const &f) {
  return f;
}
template <typename To, typename From>
inline To down_cast(From *f)  // 只接受指针
{
  if (false) CheckCast<From *, To>(0);              /** 编译期检查 */
  assert(f == NULL || dynamic_cast<To>(f) != NULL); /** 运行期检查 */
  return static_cast<To>(f);
}

/// @brief: 不可被拷贝的类,如果想要定义一个不可被拷贝的类,继承这个类就行了
class noncopyable {
 protected:
  constexpr noncopyable() = default;
  ~noncopyable() = default;

  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;
};

/// @brief: 不可被拷贝移动的类,如果想要定义一个不可被拷贝的类,继承这个类就行了
struct non_transferable {
  /// 默认构造函数
  non_transferable() = default;
  /// 复制构造函数不定义
  non_transferable(non_transferable const &) = delete;
  /// 移动构造函数不定义
  non_transferable(non_transferable &&) = delete;
  /// 复制赋值符不定义
  non_transferable &operator=(non_transferable const &) = delete;
  /// 移动赋值符不定义
  non_transferable &operator=(non_transferable &&) = delete;
};

/// @name     IsMultiOverFlow
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
inline bool IsMultiOverFlow(T1 x, T2 y) noexcept {
  static_assert(std::is_same<T1, T2>::value, "IsMultiOverFlow need same type!");
  static_assert(std::is_integral<T1>::value,
                "IsMultiOverFlow need integral type!");

  if constexpr (std::is_unsigned<T1>::value) {
    unsigned long long overflow_number = static_cast<unsigned long long>(x) * y;
    return overflow_number != static_cast<T1>(overflow_number);
  } else if constexpr (std::is_signed<T1>::value) {
    long long overflow_number = static_cast<long long>(x) * y;
    return overflow_number != static_cast<T1>(overflow_number);
  } else {
    return false;
  }
}
/// IsMultiOverFlow函数的模板特例，用来单独对long long类型进行判断
template <>
inline bool IsMultiOverFlow(long long x, long long y) noexcept {
  if (x < y) std::swap(x, y);
  if (0 == y || 0 == x) return false;
  if (LLONG_MIN == y && -1 == x) return true;
  if ((y > 0) && (x > (LLONG_MAX / y))) return true;
  if ((x < 0) && (y < (LLONG_MAX / x))) return true;
  if ((x > 0 && y < 0) && (x > (LLONG_MIN / y))) return true;
  return false;
}
/// IsMultiOverFlow函数的模板特例，用来单独对unsigned long long类型进行判断
template <>
inline bool IsMultiOverFlow(unsigned long long x,
                            unsigned long long y) noexcept {
  if (x < y) std::swap(x, y);
  if (0 == y || 0 == x) return false;
  if (x > (ULLONG_MAX / y)) return true;
  return false;
}

/// @name     Multiplies_s
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
/// @warning  线程不安全
template <typename T1, typename T2>
std::optional<T1> Multiplies_s(const T1 x, const T2 y) noexcept {
  static_assert(std::is_same<T1, T2>::value, "Multiplies_s need same type!");
  static_assert(std::is_integral<T1>::value,
                "Multiplies_s need integral type!");
  if (IsMultiOverFlow(x, y)) return {};
  return x * y;
}
/// @name     IsLittleEndian
/// @brief    判断电脑大小端
///
/// @param    NONE
///
/// @return   是否是小端
///   @retval true     小端
///   @retval false    大端
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-01 18:45:21
/// @warning  线程不安全
inline bool IsLittleEndian() noexcept {
  union ss {
    int i;
    char a[sizeof(int)];
  };
  ss Endian;
  Endian.i = 0x0102;
  return Endian.a[0] == 0x02;
}

/// @name     SleepForRandomMilliSecond
/// @brief    生成一个随机数，让调用本函数的线程"睡眠随机时间"
///
/// @param    range_start [in]
/// @param    range_end   [in]
///
/// @return   NONE
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-05 13:31:45
/// @warning  线程不安全
inline void SleepForRandomMilliSecond(Lee::MilliSecond range_start,
                                      Lee::MilliSecond range_end) {
  /// 设置最大时间限制
  constexpr Lee::MilliSecond SLEEP_FOR_RANGE_MAX_MILLISECOND = 30 * 1000;
  if (range_start > range_start) std::swap(range_start, range_end);
  if (range_start < 0) range_start = 0;
  if (range_end < 0) range_end = 0;
  if (range_end > SLEEP_FOR_RANGE_MAX_MILLISECOND) {
    /// assert("range_end is too large!" && false);
    std::cout << "range_end is too large!" << std::endl;
    range_end = SLEEP_FOR_RANGE_MAX_MILLISECOND;
  }
  Lee::MilliSecond sleep_time = Lee::GetRangeRandom(
      static_cast<int>(range_start), static_cast<int>(range_end));
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
}
inline void SleepForRandomMilliSecond(Lee::MilliSecond range_end) {
  SleepForRandomMilliSecond(0, range_end);
}

/// @name     quick_exit
/// @brief    先打印日志，引发正常的程序终止并清理
///
/// @param    code      [in]  退出码
/// @param    exit_info [in]  退出的信息
///
/// @return   NONE
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-01-07 16:35:46
/// @warning  线程不安全
inline void quick_exit(const int &code, const std::string &exit_info) {
  LOG(LOG_ERROR, exit_info.c_str());
  LOG(LOG_ERROR, "Exist the program!");
  exit(code);
}

}  // namespace detail
}  // namespace utility
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_DETAIL_UTILITY_H_