///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019, 2020 Lijiancong. All rights reserved.
///
/// @file   smart_ptr.h
/// @brief  自己尝试实现一个智能指针
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-05-14 10:52:48
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_SMART_PTR_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_SMART_PTR_H_

namespace Lee {
inline namespace data_struct {
inline namespace smart_ptr {

/// @name     smart_ptr
/// @brief    用于实现一个简单的智能指针
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-05-14 10:59:29
/// @warning  线程不安全
template <typename T>
class smart_ptr {
 public:
  explicit smart_ptr(T* ptr = nullptr) : ptr_(ptr) {}
  ~smart_ptr() { delete ptr_; }

  T* get() const { return ptr_; }
  T& operator*() const { return *ptr_; }
  T* operator->() const { return ptr_; }
  operator bool() const { return ptr_; }

  /// 子类指针向基类指针的转换
  template <typename U>
  smart_ptr(smart_ptr<U>&& other) {
    ptr = other.release();
  }

  smart_ptr& operator=(smart_ptr rhs) {
    smart_ptr(rhs).swap(*this);
    return *this;
  }

  T* release() {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  void swap(smart_ptr& rhs) {
    using std::swap;
    swap(ptr_, rhs.ptr_);
  }

 private:
  T* ptr_;
};

}  // namespace smart_ptr
}  // namespace data_struct
}  // namespace Lee
#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_SMART_PTR_H_