///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   visit.hpp
/// @brief  std::visit的实际使用的封装示例
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-11-03 14:26:51
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_VISIT_HPP_
#define INCLUDE_DETAIL_VISIT_HPP_

#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace lee {
inline namespace visit {
template<class... Fs> struct overload :Fs... {using Fs::operator()...; };
template<class... Fs> overload(Fs...)->overload<Fs...>;

template<class... Ts>
struct matcher {
  std::tuple<Ts...> vs;
  template<class... Vs> constexpr matcher(Vs&&... vs) : vs(std::forward<Vs>(vs)...) {}
  template<class Fs> constexpr auto operator->*(Fs&& f) const {
    auto curry = [&](auto&&... vs) {return std::visit(std::forward<Fs>(f), vs...); };
    return std::apply(curry, std::move(vs));
  }
};

template<class... Ts> matcher(Ts&&...)->matcher<Ts&&...>;
#define Match(...) matcher{__VA_ARGS__}->* overload

int example() {
  std::vector<std::variant<int, double, std::string>> vec{1, 1.0, "ljslkfjskd"};
  for (auto& it : vec) {
    Match(it) {
      [](auto&& x) {std::cout << "unknow type!" << std::endl; },
      [](int x) {std::cout << "int: " << x << std::endl; },
      [](double x) {std::cout << "double: " << x << std::endl; },
    };
  }
}

}}

#endif  ///< end of  INCLUDE_DETAIL_VISIT_HPP_
