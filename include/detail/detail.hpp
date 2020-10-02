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
/// @date   2020-10-02 09:37:40
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_DETAIL_HPP_
#define INCLUDE_DETAIL_DETAIL_HPP_

#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <functional>
#include <limits>
#include <mutex>
#include <sstream>
#include <string>
#include <type_traits>

namespace lee {
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
///           条款一：理解模板型别推导
///
/// @date     2019-12-01 18:48:08
/// @warning  线程安全
/// @note     用法如ARRAY_SIZE(a)一样
///           只有原生数组可以使用该函数，std::array、std::string、
///           std::vector等容器不可以调用该函数
template <class T, std::size_t N>
constexpr inline std::size_t ArraySize(T (&)[N]) noexcept {
  return N;
}

namespace lee_array_size {
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
}  // namespace lee_array_size

#define arraysize(array) (sizeof(::lee::lee_array_size::ArraySizeHelper(array)))

/// @name     ignore_unused
/// @brief    使用该空函数可以屏蔽编译器对未使用过的变量的警告
///
/// @param    模板变参列表，这意味着括号里可以填写任意类型，任意数量的变量。
///
/// @return   NONE
/// @author   Lijiancong, pipinstall@163.com
///           (Taken from boost boost/core/ignore_unused.hpp)
/// @date     2019-12-01 18:50:59
/// @warning  线程安全
/// @note     当编译器警告有未使用变量时，首选应该删除未使用变量，
///           只有当必须留存该变量时使用该函数
///
/// @example  如想要屏蔽编译器的这句警告：
///           2>F:\1WorkStation\test\src\main.cpp(53,9): warning C4101:
///             “i”: 未引用的局部变量
///           在定义变量i的下面写上这句话：
///           lee::ignore_unused(i);
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
    lee::checked_delete(x);
  }
};

/// @name     checked_array_deleter
/// @brief    函数对象类型的checked_array_delete，
///           为了适配STL函数而设计，功能等价于checked_array_delete
///
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
    lee::checked_array_delete(x);
  }
};

/// @name     up_cast
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
///           (Taken from boost boost/up_cast.hpp)
/// @date     2019-10-31 09:01:33
/// @warning  线程不安全
/// @note     1. 用法可见:doc/TechnicalSummary/Technical Summary.pdf中小节:
///              基类与派生类之间的转换规则
///           2. static_cast与up_cast的区别:
///              You can down-cast with static_cast. Not so with up_cast.
///              static_cast basically allows you to do any implicit conversion,
///              and in addition the reverse of any implicit conversion
///              (up to some limits. you can't downcast if there is
///              a virtual base-class involved).
///              But up_cast will only accept implicit conversions.
///              no down-cast, no void*->T*, no U->T if T has only explicit
///              constructors for U.
///              引用自(https://stackoverflow.com/questions/868306/what-is-the-difference-between-static-cast-and-implicit-cast)
///           3. 关于更多信息可以参考下面这篇文章:
///              《c++小技巧(三)更好的类型转换implicit_cast和down_cast》
///              https://blog.csdn.net/xiaoc_fantasy/article/details/79570788
namespace lee_up_cast {
template <class T>
struct icast_identity {
  typedef T type;
};  ///< 用于implicit_cast函数参数的类型推导
}  // namespace lee_up_cast

template <typename T>
inline T up_cast(typename ::lee::lee_up_cast::icast_identity<T>::type x) {
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
namespace lee_down_cast {
template <typename To, typename From>
inline To check_cast(From const &f) {
  return f;
}
}  // namespace lee_down_cast
template <typename To, typename From>
inline To down_cast(From *f)  // 只接受指针
{
  if (false) ::lee::lee_down_cast::check_cast<From *, To>(0); /** 编译期检查 */
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

/// @name     to_hex
/// @brief    创建16进制字符串
///
/// @param    dec [in]
///
/// @return   形如"0x123"的16进制字符串
///
/// @author   lijiancong, pipinstall@163.com
/// @date     2020-07-20 10:11:58
/// @warning  线程不安全
inline std::string to_hex(const size_t dec) {
  std::ostringstream h;
  h << std::hex << (dec);
  std::string result("0x");
  result += h.str();
  return result;
}

/// @name     pointer_to_hex
/// @brief    指针转换为16进制字符串
///
/// @param    pointer [in]  必须为指针类型
///
/// @return   形如"0x123"的16进制字符串
///
/// @author   lijiancong, pipinstall@163.com
/// @date     2020-07-20 10:22:00
/// @warning  线程不安全
template <typename T>
inline std::string pointer_to_hex(const T pointer) {
  static_assert(std::is_pointer<T>::value,
                " pointer_to_hex param is not a pointer!");
  return to_hex(reinterpret_cast<size_t>(pointer));
}

/// @name     remove_repeat_element
/// @brief    去除二维数组中重复的元素
///
/// @param    res [in/out]
///
/// @return   res
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-09-11 20:41:39
inline std::vector<std::vector<int>> &remove_repeat_element(
    std::vector<std::vector<int>> &res) {
  std::sort(res.begin(), res.end(),
            [](const std::vector<int> &l, const std::vector<int> &r) {
              if (l.size() == r.size()) {
                for (size_t i = 0; i < l.size(); ++i) {
                  if (l.at(i) == r.at(i)) {
                    continue;
                  } else {
                    return l.at(i) < r.at(i);
                  }
                }
                return false;
              } else {
                return l.size() < r.size();
              }
            });
  auto it = std::unique(res.begin(), res.end(),
                        [](const std::vector<int> &l,
                           const std::vector<int> &r) { return l == r; });
  res.erase(it, res.end());
  return res;
}


template <typename T>
class optional {
 public:
  optional() : val_(), has_val_(false) {}
  optional(T val) : val_(val), has_val_(true) {}
  ~optional() = default;
  optional(const optional &other) {
    this->val_ = other.val_;
    this->has_val_ = other.has_val_;
  }
  optional(optional &&other) {
    this->val_ = other.val_;
    this->has_val_ = other.has_val_;
  }
  optional &operator=(const optional &other) {
    this->val_ = other.val_;
    this->has_val_ = other.has_val_;
    return *this;
  }
  optional &operator=(optional &&other) {
    this->val_ = other.val_;
    this->has_val_ = other.has_val_;
    return *this;
  }

  bool has_value() { return has_val_; }
  T value() {
    if (!has_val_) {
      throw std::runtime_error("has no value in this");
    }
    return val_;
  }
  T value_or(const T &val) { return has_val_ ? val_ : val; }

 private:
  T val_{};
  bool has_val_ = false;
};

/// @brief  对调用函数进行了封装, 使用了完美转发
/// @author lijiancong, pipinstall@163.com
///         Taken from <C++ Templates> 2e
///         Chapter 11 (11.1.3 Wrapping Function Calls)
/// @date   2020-02-10 13:17:58
template <typename Callable, typename... Args>
decltype(auto) call(Callable &&op, Args &&... args) {
  if constexpr (std::is_same_v<std::invoke_result_t<Callable, Args...>, void>) {
    // return type is void:
    std::invoke(std::forward<Callable>(op), std::forward<Args>(args)...);
    //...
    return;
  } else {
    // return type is not void:
    decltype(auto) ret{
        std::invoke(std::forward<Callable>(op), std::forward<Args>(args)...)};
    //...
    return ret;
  }
}

/// @brief  一个对象实现次数统计的模板
/// @author lijiancong, pipinstall@163.com
///         Taken from <C++ Templates> 2e
///         Chapter 21 (21.2 The Curiously Recurring Template Pattern (CRTP))
/// @date   2020-02-10 12:41:47
template <typename CountedType>
class ObjectCounter {
 private:
  inline static std::atomic<int> count = 0;  // number of existing objects

 protected:
  // default constructor
  ObjectCounter() { ++count; }

  // copy constructor
  ObjectCounter(ObjectCounter<CountedType> const &) { ++count; }

  // move constructor
  ObjectCounter(ObjectCounter<CountedType> &&) { ++count; }

  // destructor
  ~ObjectCounter() { --count; }

 public:
  // return number of existing objects:
  static std::size_t live() { return count; }
};

}  // namespace detail
}  // namespace lee

#endif  // end of INCLUDE_DETAIL_DETAIL_HPP_