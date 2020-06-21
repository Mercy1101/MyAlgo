///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   allocator_utility.h
/// @brief  分配器的实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-06-21 14:55:24
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_ALLOCATOR_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_ALLOCATOR_UTILITY_H_

namespace Lee {
inline namespace utility {
inline namespace allocator {
template <typename T>
struct my_allocator {
  using value_type = T;
  my_allocator() = default;
  template <class U>
  my_allocator(const my_allocator<U>&) {}

  T* allocate(std::size_t n, void const* = 0) {
    return reinterpret_cast<T*>(::operator new(n * sizeof(T)));
  }

  void deallocate(T* ptr, size_t) { ::operator delete(ptr); }
};
template <typename T, typename U>
inline bool operator==(const my_allocator<T>&, const my_allocator<U>&) {
  return true;
}
template <typename T, typename U>
inline bool operator!=(const my_allocator<T>& a, const my_allocator<U>& b) {
  return !(a == b);
}

}  // namespace allocator
}  // namespace utility
}  // namespace Lee