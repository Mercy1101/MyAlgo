///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   random.h
/// @brief  这个文件中主要放置一些生成随机数的函数
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-11 09:38:52
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_RANDOM_HPP_
#define INCLUDE_DETAIL_RANDOM_HPP_

#include <exception>
#include <random>


namespace lee {
inline namespace random {
/// @name     get_range_random_number
/// @brief    等概率获取一个区间中的数值
///
/// @param    range_bound1 [in]
/// @param    range_bound2 [in]
///
/// @return   [rang_bound1,rang_bound2]区间中的数值
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-07 12:07:27
/// @warning  线程安全
/// template <typename T>
/// inline T get_range_random_number(T range_bound1, T range_bound2) {
///   if constexpr (std::is_integral<std::remove_const<std::remove_reference<T>::value>::value>::value ||
///                 std::is_floating_point<std::remove_const<std::remove_reference<T>::value>::value>::value) {
///     if (range_bound1 > range_bound2) std::swap(range_bound1, range_bound2);
///     static std::random_device r;
///     static std::default_random_engine e(r());
///     std::uniform_real_distribution<T> u(range_bound1, range_bound2);
///     return u(e);
///   } else {
///     static_assert(false, "unsupport type in get_range_random_number()");
///     throw std::logic_error("unsupport type in get_range_random_number()");
///   }
/// }

}  // namespace random
}  // namespace lee
#endif  ///< INCLUDE_DETAIL_RANDOM_HPP_