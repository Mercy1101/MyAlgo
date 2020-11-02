///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   quick_sort.h
/// @brief  快速排序的实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-11-02 13:30:39
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_QUICK_SORT_HPP_
#define INCLUDE_DETAIL_QUICK_SORT_HPP_

#include <algorithm>
#include <execution>

namespace lee {
inline namespace sort {
  template<typename ForwardIt>
  inline void quick_sort(ForwardIt first, ForwardIt last) {
    if(first == last) return;
    auto temp = rand() % std::distance(first, last);
    auto pivot = *std::next(first, temp);

    auto middle1 = std::partition(std::excution::par, first, last, [pivot](const auto& em){
      return em < pivot;
    });

    auto middle2 = std::partition(std::excution::par, middle1, last, [pivot](const auto& em){
      return !(pivot < em);
    });

    quick_sort(first, middle1);
    quick_sort(middle2, last);
  }
}}

#endif  ///< end of INCLUDE_DETAIL_EXAMPLE_HPP_