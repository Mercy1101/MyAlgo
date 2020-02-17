///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019 Lijiancong. All rights reserved.
///
/// @file   concurrency_utility.h
/// @brief  从 <C++ Concurrency In Action> 书中抄的代码
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-01-16 15:31:21
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_THREAD_POOL_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_THREAD_POOL_UTILITY_H_

#include <atomic>
#include <future>
#include <memory>
#include <queue>
#include <thread>
#include <vector>
#include <shared_mutex>

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

struct empty_stack : std::exception {
  const char* what() const throw();
};

/// @name     threadsafe_queue
/// @brief    使用锁来封装一个std::stack，因为大量使用了锁，所以可能有效率问题
///
/// @author   Lijiancong, pipinstall@163.com
///           Taken from <C++ Concurrency In Action> 第六章：基于锁的并发结构
///           清单6.6 使用细粒度锁的线程安全队列
/// @date     2020-01-15 09:45:31
/// @warning  线程不安全
template <typename T>
class threadsafe_queue {
 public:
  threadsafe_queue() : head(new node), tail(head.get()) {}
  threadsafe_queue(const threadsafe_queue& other) = delete;

  std::shared_ptr<T> try_pop() {
    std::unique_ptr<node> old_head = pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>();
  }

  void push(T new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
    std::unique_ptr<node> p(new node);
    node* const new_tail = p.get();
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    tail->data = new_data;
    tail->next = std::move(p);
    tail = new_tail;
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

  node* get_tail() {
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;
  }

  std::unique_ptr<node> pop_head() {
    std::lock_guard<std::mutex> head_lock(head_mutex);

    if (head.get() == get_tail()) {
      return nullptr;
    }
    std::unique_ptr<node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
  }

};  /// end of threadsafe_queue_ptr

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
          data.begin(), dta.end(),
          [&](bucket_value const& item) { return item.first == key; });
    }

   public:
    Value value_for(Key const& key, Value const& value) {
      std::shared_lock<std::shared_mutex> lock(mutex);
      bucket_iterator const found_entry = find_entry_for(key);
      return (found_entry == data.end()) ? default_value : found_entry->second;
    }

    void add_or_update_mapping(Key const& key, Value const& value) {
      std::unique_lock<std::shared_mutex> lock(mutex);
      bucket_iterator const found_entry = find_entry_for(key);
      if (found_entry == data.end()) {
        data.push_back(bucket_value(key, value));
      }
    }

    void remove_mapping(Key const& key) {
      std::unique_lock<std::shared_mutex> lock(mutex);
      bucket_iterator const found_entry = find_entry_for(key);
      if (found_entry != data.end()) {
        data.erase(found_entry);
      }
    }
  };  /// end of bucket_type

  std::vector<std::unique_ptr<bucket_type>> buckets;
  Hash hasher;

  bucket_type& get_bucket(Key const& key) const {
    std::size_t const bucket_index = hasher(key) % buckets.size();
    return *buckets[bucket_index];
  }

 public:
  typedef Key key_type;
  typedef Value mapped_type;

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

  void add_or_update_mapping(Key const& key, Value const& value) {
    get_bucket(key).add_or_update_mapping(key, value);
  }

  void remove_mapping(Key const& key) { get_bucket(key).remove_mapping(key); }
};  /// end of threadsafe_lookup_table

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
    remove_if([](node const&) { return true; });
  }

  threadsafe_list(threadsafe_list const& other) = delete;
  threadsafe_list& operator=(threadsafe_list const& other) = delete;

  void push_front(T const& value) {
    std::unique_ptr<node> new_node(new node(value));
    std::lock_guard<std::mutex> lk(head.m);
    new_node->next = std::move(new_node);
  }

  template <typename Function>
  void for_each(Function f) {
    node* current = &head;
    std::unique_lock<std::mutex> lk(head.m);
    while (node* const next = current->next.get()) {
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
      }
    }
  }
};  /// end of class threadsafe_list

template <typename T>
class lock_free_queue {
 private:
  struct node {
    std::shared_ptr<T> data;
    node* next;
    node() : next(nullptr) {}
  };

  std::atomic<node*> head;
  std::atomic<node*> tail;
  node* pop_head() {
    node* const old_head = head.load();
    if (old_head == tail.load()) {
      return nullptr;
    }
    head.store(old_head->next);
    return old_head;
  }

 public:
  lock_free_queue() : head(new node), tail(head.load()) {}
  lock_free_queue& operator=(const lock_free_queue& other) = delete;

  ~lock_free_queue() {
    while (node* const old_head = head.load()) {
      head.store(old_head->next);
      delete old_head;
    }
  }

  std::shared_ptr<T> pop() {
    node* old_head = pop_head();
    if (!old_head) {
      return std::shared_ptr<T>();
    }

    std::shared_ptr<T> const res(old_head->data);
    delete old_head;
    return res;
  }

  void push(T new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
    node* p = new node;
    node* const old_tail = tail.load();
    old_tail->data.swap(new_data);
    old_tail->next = p;
    tail.store(p);
  }
};  /// end of lock_free_queue


class function_wrapper {
  struct impl_base {
    virtual void call() = 0;
    virtual ~impl_base() {}
  };
  std::unique_ptr<impl_base> impl;
  template <typename F>
  struct impl_type : impl_base {
    F f;
    impl_type(F&& f_) : f(std::move(f_)) {}
    void call() { f(); }
  };

 public:
  template <typename F>
  function_wrapper(F&& f) : impl(new impl_type<F>(std::move(f))) {}

  void call() { impl->call(); }

  function_wrapper(function_wrapper&& other) : impl(std::move(other.impl)) {}

  function_wrapper& operator=(function_wrapper&& other) {
    impl = std::move(other.impl);
    return *this;
  }

  function_wrapper(const function_wrapper&) = delete;
  function_wrapper(function_wrapper&) = delete;
  function_wrapper& operator=(const function_wrapper&) = delete;
};

class work_stealing_queue {
 private:
  using data_type = Lee::function_wrapper;
  std::deque<data_type> the_queue;
  mutable std::mutex the_mutex;

 public:
  work_stealing_queue() {}

  work_stealing_queue(const work_stealing_queue& other) = delete;
  work_stealing_queue& operator=(const work_stealing_queue& other) = delete;

  void push(data_type data) {
    std::lock_guard<std::mutex> lock(the_mutex);
    the_queue.push_front(std::move(data));
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(the_mutex);
    return the_queue.empty();
  }

  bool try_pop(data_type& res) {
    std::lock_guard<std::mutex> lock(the_mutex);
    if (the_queue.empty()) {
      return false;
    }

    res = std::move(the_queue.front());
    the_queue.pop_front();
    return true;
  }

  bool try_steal(data_type& res) {
    std::lock_guard<std::mutex> lock(the_mutex);
    if (the_queue.empty()) {
      return false;
    }

    res = std::move(the_queue.back());
    the_queue.pop_back();
    return true;
  }
};


class join_threads {
  std::vector<std::thread>& threads;

 public:
  explicit join_threads(std::vector<std::thread>& threads_)
      : threads(threads_) {}
  ~join_threads() {
    for (unsigned long i = 0; i < threads.size(); ++i) {
      if (threads[i].joinable()) threads[i].join();
    }
  }
};

}  // namespace concurrency
}  // namespace utility
}  // namespace Lee

#endif