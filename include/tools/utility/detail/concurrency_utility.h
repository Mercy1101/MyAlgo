﻿///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// @file   concurrency_utility.h
/// @brief  从 <C++ Concurrency In Action> 书中抄的代码
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-01-16 15:31:21
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_CONCURRENCY_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_CONCURRENCY_UTILITY_H_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <utility>
#include <vector>

namespace Lee {
inline namespace utility {
inline namespace concurrency {

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

  /// @name     lock
  /// @brief    用来锁定自己的函数，
  ///           锁定顺序依照本线程中小的数值先锁定，到锁定大的数值
  ///           如果顺序反过来则会抛出异常
  ///
  /// @param    NONE
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-02-17 13:22:50
  /// @warning  线程不安全
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
  inline static thread_local unsigned long this_thread_hierarchy_value_ = 0;

  void check_for_hierarchy_violation() {
    if (this_thread_hierarchy_value_ >= hierarchy_value_) {
      throw std::logic_error("mutex hierarchy violated");
    }
  }

  void update_hierarchy_value() {
    previous_hierarchy_value_ = this_thread_hierarchy_value_;
    this_thread_hierarchy_value_ = hierarchy_value_;
  }
};

/// @name     threadsafe_queue
/// @brief    使用条件变量来确保线程安全的队列
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-02-24 09:04:58
/// @warning  线程不安全
template <typename T>
class threadsafe_queue {
 public:
  threadsafe_queue() : head(new node), tail(head.get()) {}
  threadsafe_queue(const threadsafe_queue& other) = delete;
  threadsafe_queue& operator=(const threadsafe_queue& other) = delete;

  std::shared_ptr<T> try_pop() {
    std::unique_ptr<node> const old_head = try_pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>();
  }

  bool try_pop(T& value) {
    std::unique_ptr<node> const old_head = try_pop_head(value);
    return old_head;
  }

  std::shared_ptr<T> wait_and_pop() {
    std::unique_ptr<node> const old_head = wait_pop_head();
    return old_head->data;
  }

  void wait_and_pop(T& value) {
    std::unique_ptr<node> const old_head = wait_pop_head(value);
  }

  void push(T new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
    std::unique_ptr<node> p(new node);
    {
      std::lock_guard<std::mutex> tail_lock(tail_mutex);
      tail->data = new_data;
      node* const new_tail = p.get();
      tail->next = std::move(p);
      tail = new_tail;
    }
    data_cond.notify_one();
  }

  void empty() {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    return (head == get_tail());
  }

 private:
  struct node {
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;
  };

  std::mutex head_mutex;
  std::unique_ptr<node> head;
  std::mutex tail_mutex;
  node* tail;
  std::condition_variable data_cond;

  node* get_tail() {
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;
  }

  std::unique_ptr<node> pop_head() {
    std::unique_ptr<node> const old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
  }

  std::unique_lock<std::mutex> wait_for_data() {
    std::unique_lock<std::mutex> head_lock(head_mutex);
    data_cond.wait(head_lock, [&] { return head != get_tail(); });
    return std::move(head_lock);
  }

  std::unique_ptr<node> wait_pop_head() {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    return pop_head();
  }

  std::unique_ptr<node> wait_pop_head(T& value) {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    value = std::move(*head->data);
    return pop_head();
  }

  std::unique_ptr<node> try_pop_head() {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return std::unique_ptr<node>();
    }
    return pop_head();
  }

  std::unique_ptr<node> try_pop_head(T& value) {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return std::unique_ptr<node>();
    }
    value = std::move(*head->data);
    return pop_head();
  }
};

/// @name     threadsafe_lookup_table
/// @brief    细粒度锁的线程安全查找表
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-02-24 14:18:14
/// @warning  线程不安全
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lookup_table {
 private:
  class bucket_type {
   private:
    typedef std::pair<Key, Value> bucket_value;
    typedef std::list<bucket_value> bucket_data;
    typedef typename bucket_data::iterator bucket_iterator;

    bucket_data data;
    mutable std::shared_mutex mutex;

    bucket_iterator find_entry_for(Key const& key) const {
      return std::find_if(
          data.begin(), data.end(),
          [&](bucket_value const& item) { return item.first == key; });
    }

   public:
    Value value_for(Key const& key, Value const& default_value) const {
      std::shared_lock<std::shared_mutex> lock(mutex);
      bucket_iterator const found_entry = find_entry_for(key);
      return (found_entry == data.end() ? default_value : found_entry->second);
    }

    void add_or_updata_mapping(Key const& key, Value const& value) {
      std::unique_lock<std::shared_mutex> lock(mutex);
      bucket_iterator const found_entry = find_entry_for(key);
      if (found_entry == data.end()) {
        data.push_back(bucket_value(key, value));
      } else {
        found_entry->second = value;
      }
    }

    void remove_mapping(Key const& key) {
      std::unique_lock<std::shared_mutex> lock(mutex);
      bucket_iterator const found_entry = find_entry_for(key);
      if (found_entry != data.end()) {
        data.erase(found_entry);
      }
    }
  };

  std::vector<std::unique_ptr<bucket_type>> buckets;
  Hash hasher;

  bucket_type& get_bucket(Key const& key) const {
    std::size_t const bucket_index = hasher(key) % buckets.size();
    return *buckets[bucket_index];
  }

 public:
  typedef Key key_type;
  typedef Value mapped_type;
  typedef Hash hash_type;

  threadsafe_lookup_table(unsigned num_buckets = 19,
                          Hash const& hasher_ = Hash())
      : buckets(num_buckets), hasher(hasher_) {
    for (unsigned i = 0; i < num_buckets; ++i) {
      buckets[i].reset(new bucket_type);
    }
  }

  threadsafe_lookup_table(threadsafe_lookup_table const& other) = delete;
  threadsafe_lookup_table& operator=(threadsafe_lookup_table const& other) =
      delete;

  Value value_for(Key const& key, Value const& default_value = Value()) const {
    return get_bucket(key).value_for(key, default_value);
  }

  void add_or_updata_mapping(Key const& key, Value const& value) {
    get_bucket().add_or_update_mapping(key, value);
  }

  void remove_mapping(Key const& key) { get_bucket(key).remove_mapping(key); }
};

/// @name     threadsafe_list
/// @brief    一个具有细粒度锁的线程安全，可迭代的链表
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-02-24 15:04:19
/// @warning  线程不安全
template <typename T>
class threadsafe_list {
  struct node {
    std::mutex m;
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;

    node() : next() {}

    node(T const& value) : data(std::make_shared<T>(value)) {}
  };

  node head;

 public:
  threadsafe_list() {}

  ~threadsafe_list() {
    remove_if([](T const&) { return true; });
  }

  threadsafe_list(threadsafe_list const& other) = delete;
  threadsafe_list& operator=(threadsafe_list const& other) = delete;

  void push_front(T const& value) {
    std::unique_ptr<node> new_node(new node(value));
    std::lock_guard<std::mutex> lk(head.m);
    new_node->next = std::move(head.next);
    head.next = std::move(new_node);
  }

  template <typename Function>
  void for_each(Function f) {
    node* current = &head;
    std::unique_lock<std::mutex> lk(head.m);
    while (node* const next = current->next.get()) {
      std::unique_lock<std::mutex> next_lk(next->m);
      lk.unlock();
      f(*next->data);
      current = next;
      lk = std::move(next_lk);
    }
  }

  template <typename Predicate>
  std::shared_ptr<T> find_first_if(Predicate p) {
    node* current = &head;
    std::unique_lock<std::mutex> lk(head.m);
    while (node* const next = current->next.get()) {
      std::unique_lock<std::mutex> next_lk(next->m);
      lk.unlock();
      if (p(*next->data)) {
        return next->data;
      }

      current = next;
      lk = std::move(next_lk);
    }
    return std::shared_ptr<T>();
  }

  template <typename Predicate>
  void remove_if(Predicate p) {
    node* current = &head;
    std::unique_lock<std::mutex> lk(head.m);
    while (node* const next = current->next.get()) {
      std::unique_lock<std::mutex> next_lk(next->m);
      if (p(*next->data)) {
        std::unique_ptr<node> old_next = std::move(current->next);
        current->next = std::move(next->next);
        next_lk.unlock();
      } else {
        lk.unlock();
        current = next;
        lk = std::move(next_lk);
      }
    }
  }
};

}  // namespace concurrency
}  // namespace utility
}  // namespace Lee

#endif