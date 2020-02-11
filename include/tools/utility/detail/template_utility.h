/// Copyright (c) 2019 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.

///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
///
/// @file   template_utility.h
/// @brief  从<C++ Templates>这本书里摘抄的东西
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-02-10 12:41:47
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_TEMPLATE_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_TEMPLATE_UTILITY_H_

#include <cstddef>

namespace Lee {
inline namespace utility {
inline namespace templates {

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
  inline static std::size_t count = 0;  // number of existing objects

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

// primary template: yield the second argument by default and rely on
//      a partial specialization to yield the third argument
//      if COND is false
template <bool COND, typename TrueType, typename FalseType>
struct IfThenElseT {
  using Type = TrueType;
};

// partial specialization: false yields third argument
template <typename TrueType, typename FalseType>
struct IfThenElseT<false, TrueType, FalseType> {
  using Type = FalseType;
};

template <bool COND, typename TrueType, typename FalseType>
using IfThenElse = typename IfThenElseT<COND, TrueType, FalseType>::Type;

/// 是否是左值引用
template <typename T>
struct IsLValueReferenceT : std::false_type {  // by default no lvalue reference
};
template <typename T>
struct IsLValueReferenceT<T &>
    : std::true_type {  // unless T is lvalue references
  using BaseT = T;      // type referring to
};

/// 是否是右值引用
template <typename T>
struct IsRValueReferenceT : std::false_type {  // by default no rvalue reference
};
template <typename T>
struct IsRValueReferenceT<T &&>
    : std::true_type {  // unless T is rvalue reference
  using BaseT = T;      // type referring to
};

/// 是否是引用
template <typename T>
class IsReferenceT
    : public IfThenElseT<IsLValueReferenceT<T>::value, IsLValueReferenceT<T>,
                         IsRValueReferenceT<T>>::Type {};

// primary template: in general T is not a fundamental type
template <typename T>
struct IsFundaT : std::false_type {};

// macro to specialize for fundamental types
#define MK_FUNDA_TYPE(T) \
  template <>            \
  struct IsFundaT<T> : std::true_type {};

MK_FUNDA_TYPE(void)

MK_FUNDA_TYPE(bool)
MK_FUNDA_TYPE(char)
MK_FUNDA_TYPE(signed char)
MK_FUNDA_TYPE(unsigned char)
MK_FUNDA_TYPE(wchar_t)
MK_FUNDA_TYPE(char16_t)
MK_FUNDA_TYPE(char32_t)

MK_FUNDA_TYPE(signed short)
MK_FUNDA_TYPE(unsigned short)
MK_FUNDA_TYPE(signed int)
MK_FUNDA_TYPE(unsigned int)
MK_FUNDA_TYPE(signed long)
MK_FUNDA_TYPE(unsigned long)
MK_FUNDA_TYPE(signed long long)
MK_FUNDA_TYPE(unsigned long long)

MK_FUNDA_TYPE(float)
MK_FUNDA_TYPE(double)
MK_FUNDA_TYPE(long double)

MK_FUNDA_TYPE(std::nullptr_t)

/// 是否是指针
template <typename T>
struct IsPointerT
    : std::false_type {  // primary template: by default not a pointer
};
template <typename T>
struct IsPointerT<T *>
    : std::true_type {  // partial specialization for pointers
  using BaseT = T;      // type pointing to
};

/// 是否是数组
template <typename T>
struct IsArrayT : std::false_type {  // primary template: not an array
};

template <typename T, std::size_t N>
struct IsArrayT<T[N]> : std::true_type {  // partial specialization for arrays
  using BaseT = T;
  static constexpr std::size_t size = N;
};

template <typename T>
struct IsArrayT<T[]>
    : std::true_type {  // partial specialization for unbound arrays
  using BaseT = T;
  static constexpr std::size_t size = 0;
};

/// 是否是成员指针
template <typename T>
struct IsPointerToMemberT
    : std::false_type {  // by default no pointer-to-member
};

template <typename T, typename C>
struct IsPointerToMemberT<T C::*> : std::true_type {  // partial specialization
  using MemberT = T;
  using ClassT = C;
};

template <typename T, typename = std::void_t<>>
struct IsClassT : std::false_type {  // primary template: by default no class
};

template <typename T>
struct IsClassT<T, std::void_t<int T::*>>  // classes can have pointer-to-member
    : std::true_type {};

/// 是否是函数
template <typename... Elements>
class Typelist {};
template <typename T>
struct IsFunctionT : std::false_type {  // primary template: no function
};

template <typename R, typename... Params>
struct IsFunctionT<R(Params...)> : std::true_type {  // functions
  using Type = R;
  using ParamsT = Typelist<Params...>;
  static constexpr bool variadic = false;
};

template <typename R, typename... Params>
struct IsFunctionT<R(Params..., ...)> : std::true_type {  // variadic functions
  using Type = R;
  using ParamsT = Typelist<Params...>;
  static constexpr bool variadic = true;
};

/// 是否是枚举
template <typename T>
struct IsEnumT {
  static constexpr bool value =
      !IsFundaT<T>::value && !IsPointerT<T>::value && !IsReferenceT<T>::value &&
      !IsArrayT<T>::value && !IsPointerToMemberT<T>::value &&
      !IsFunctionT<T>::value && !IsClassT<T>::value;
};

}  // namespace templates
}  // namespace utility
}  // namespace Lee
#endif  /// end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_TEMPLATE_UTILITY_H_