///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019 Lijiancong. All rights reserved.
///
/// @file   thread_utility.h
/// @brief  这个文件中放置与线程相关的小工具
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-01-13 09:15:54
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_THREAD_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_THREAD_UTILITY_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "utility/detail/marco_utility.h"

namespace Lee {
inline namespace utility {
inline namespace thread_guard_ {

/// @name     thread_guard
/// @brief    用来传入一个线程，并在析构时自动调用join(), 来令线程join(),
///           防止语句发生异常时线程没有调用join函数
///
/// @author   Lijiancong, pipinstall@163.com
///           Taken from <C++ Concurrency In Action> 第二章：管理线程
///           清单2.3 使用RAII等待线程完成
///
/// @date     2020-01-13 10:04:34
/// @warning  线程安全
class thread_guard {
 public:
  explicit thread_guard(std::thread& t) : t_(t) {}
  ~thread_guard() {
    if (t_.joinable()) {
      t_.join();
    } else {
      /// TODO（李建聪）: 如果不能join，怎么处理？
    }
  }

 private:
  MYALGO_DISALLOW_COPY_AND_ASSIGN_(thread_guard);
  std::thread& t_;  ///< 用来保留一个线程的引用

};  /// end of class thread_guard

/// @name     scoped_thread
/// @brief    用来移动一个线程进入本class，然后在析构的时候调用join。
///           防止线程因为异常而提前退出没有调用join函数
///
/// @author   Lijiancong, pipinstall@163.com
///           Taken from <C++ Concurrency In Action> 第二章：管理线程
///           清单2.6 scoped_thread和示例用法
/// @date     2020-01-13 10:07:11
/// @warning  线程安全
class scoped_thread {
 public:
  explicit scoped_thread(std::thread t) : t_(std::move(t)) {
    if (!t_.joinable()) throw std::logic_error("No thread");
  }
  ~scoped_thread() { t_.join(); }

 private:
  MYALGO_DISALLOW_COPY_AND_ASSIGN_(scoped_thread);
  std::thread t_;
};  /// end of class scoped_thread

/// @name     hierarchical_mutex
/// @brief    为了实现在单个线程中保证加锁顺序的锁，保证加锁顺序否则就抛出异常
///           可以配合std::lock_guard<>()来实现保证加锁解锁顺序，防止死锁出现
///
/// @author   Lijiancong, pipinstall@163.com
///           Taken from <C++ Concurrency In Action> 第三章：管理线程
///           清单3.8 简单的分层次互斥元
/// @date     2020-01-13 12:31:46
/// @warning  线程不安全
class hierarchical_mutex {
 public:
  explicit hierarchical_mutex(const unsigned long value)
      : hierarchy_value_(value), previous_hierarchy_value_(0) {}

  void lock() {
    check_for_hierarchy_violation();
    internal_mutex_.lock();
    update_hierarchy_value();
  }

  void unlock() {
    this_thread_hierarchy_value_ = previous_hierarchy_value_;
    internal_mutex_.unlock();
  }

  bool try_lock() {
    check_for_hierarchy_violation();
    if (!internal_mutex_.try_lock()) return false;
    update_hierarchy_value();
    return true;
  }

 private:
  std::mutex internal_mutex_;

  unsigned long const hierarchy_value_;
  unsigned long previous_hierarchy_value_;
  static thread_local unsigned long this_thread_hierarchy_value_;

  void check_for_hierarchy_violation() {
    if (this_thread_hierarchy_value_ <= hierarchy_value_) {
      throw std::logic_error("mutex hierarchy violated");
    }
  }

  void update_hierarchy_value() {
    previous_hierarchy_value_ = this_thread_hierarchy_value_;
    this_thread_hierarchy_value_ = hierarchy_value_;
  }
};

/// @name     threadsafe_queue
/// @brief    内部使用std::queue，外面包装了一层线程安全的接口
///
/// @author   Lijiancong, pipinstall@163.com
///           Taken from <C++ Concurrency In Action> 第四章：同步并发操作
///           清单4.5 使用条件变量的线程安全队列的完整类定义
///
/// @date     2020-01-13 14:32:30
/// @warning  线程安全
template <typename T>
class threadsafe_queue {
 public:
  threadsafe_queue() {}
  threadsafe_queue(threadsafe_queue const& other) {
    std::lock_guard<std::mutex> lk(other.mut);
    data_queue = other.data_queue;
  }

  void push(T new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(new_value);
    data_cond.notify_one();
  }

  void wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data.empty(); });
    value = data_queue.front();
    data_queue.pop();
  }

  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
  }

  bool try_pop(T& value) {
    std::lock_guard<std::mutex> lk(mut);
    if (data_queue.empty()) return false;
    value = data_queue.front();
    data_queue.pop();
    return true;
  }

  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lk(mut);
    if (data_queue.empty()) return std::shared_ptr<T>();
    std::shared_ptr<T> res(std::make_shared<T>(data_queue.front();));
    data_queue.pop();
    return res;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }

 private:
  mutable std::mutex mut;
  std::queue<T> data_queue;
  std::condition_variable data_cond;
};  /// end of class threadsafe_queue


}  // namespace thread_guard_
}  // namespace utility
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_THREAD_UTILITY_H_