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
  explicit smart_ptr(T* ptr = nullptr) : ptr_(ptr) {
    if (ptr) {
      shared_count_ = new shared_count();
    }
  }

  smart_ptr(const smart_ptr& other) {
    ptr_ = other.ptr_;
    if (ptr_) {
      other.shared_count_->add_count();
      shared_count_ = other.shared_count_;
    }
  }

  template <typename U>
  smart_ptr(const smart_ptr<U>& other) noexcept {
    ptr_ = other.ptr_;
    if (ptr_) {
      other.shared_count_->add_count();
      shared_count_ = other.shared_count_;
    }
  }

  /// 这里引入模板U, 是为了自动类型推导来支持子类指针向基类指针的转换
  template <typename U>
  smart_ptr(const smart_ptr<U>&& other) noexcept {
    ptr_ = other.ptr_;
    if (ptr_) {
      shared_count_ = other.shared_count_;
      other.ptr = nullptr;
    }
  }

  ~smart_ptr() {
    if (nullptr != ptr_ && !shared_count_()->reduce_count()) {
      delete ptr_;
      delete shared_count_;
    }
    delete ptr_;
  }

  T* get() const noexcept { return ptr_; }
  T& operator*() const noexcept { return *ptr_; }
  T* operator->() const noexcept { return ptr_; }
  operator bool() const noexcept { return ptr_; }

  smart_ptr& operator=(smart_ptr rhs) noexcept {
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
    swap(shared_count_, rhs.shared_count_);
  }

  template <typename U>
  smart_ptr(const smart_ptr<U>& other, T* ptr) {
    ptr_ = ptr;
    if (ptr_) {
      other.shared_count_->add_count();
      shared_count_ = other.shared_count_;
    }
  }

  template <typename U>
  smart_ptr<T> dynamic_pointer_cast(const smart_ptr<U>& other) {
    T* ptr = dynamic_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
  }

 private:
  class shared_count {
   public:
    shared_count() : count_(-1) {}

    void add_count() noexcept { ++count_; }
    long reduce_count() noexcept { return --count_; }
    long get_count() const noexcept { return count_; }
    long use_count() const noexcept {
      if (ptr_) {
        return shared_count_->get_count_();
      } else {
        return 0;
      }
    }

   private:
    long count_;
  };

  T* ptr_;
  shared_count* shared_count_;
};

}  // namespace smart_ptr
}  // namespace data_struct
}  // namespace Lee
#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_SMART_PTR_H_