///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   thread_pool.h
/// @brief  线程池的实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-09-17 20:35:11
/// @note   C++11、C++14、C++17都可以编译, 不能使用C++20编译
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef LEE_THREAD_POOL_H_
#define LEE_THREAD_POOL_H_

#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  /// @name     ThreadPool
  /// @brief 用于创建若干个线程，如果没有设置几个线程，默认设置硬件支持的线程数
  ///
  /// @param    threads   [in]    要创建的线程数量
  ///
  /// @return   NONE
  ///
  /// @date     2020-09-17 20:08:20
  /// @warning  线程安全
  ThreadPool(size_t threads = std::thread::hardware_concurrency())
      : stop(false) {
    for (size_t i = 0; i < threads; ++i)
      workers.emplace_back([this] {
        for (;;) {
          std::function<void()> task;

          {
            /// 获取同步锁
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            /// 阻塞等待获取任务，直到任务队列不为空
            this->condition.wait(
                lock, [this] { return this->stop || !this->tasks.empty(); });
            /// 如果stop标志位为true，且任务列表都执行完毕后，该线程退出
            if (this->stop && this->tasks.empty()) return;
            /// 从任务队列中拿出来一个任务
            task = std::move(this->tasks.front());
            this->tasks.pop();
          }  ///< 这里释放锁

          /// 执行该任务函数
          task();
        }
      });
  }

  /// @name     enqueue
  /// @brief    用于添加任务函数到任务队列中
  ///
  /// @param    f     [in]    任务函数
  /// @param    args  [in]    任务函数的入参列表
  ///
  /// @return   取决于任务函数的返回值
  ///
  /// @date     2020-09-17 21:30:34
  /// @warning  线程安全
  template <class F, class... Args>
  auto enqueue(F&& f, Args&&... args)
      -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    /// 这里封装一个异步的线程并执行刚刚传入的函数，这个函数通过bind改类型为void()
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    /// 创建一个这个函数的未来的值， 这个未来值不获取就不会进行计算
    std::future<return_type> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(queue_mutex);

      /// 假如说没有让这个线程停止则继续，否则抛出异常阻止线程池结束后在入列
      if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
      /// 这个封装好的函数放入任务列表中
      tasks.emplace([task]() { (*task)(); });
    }
    /// 通知一个阻塞中的线程，任务队列中有任务了
    condition.notify_one();
    return res;
  }

  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      stop = true;
    }
    /// 在stop位,
    /// 置为true后通知所有线程执行一次，然后等待所有线程处理完任务后join()
    condition.notify_all();
    for (std::thread& worker : workers) worker.join();
  }

 private:
  /// 所有的工作线程
  std::vector<std::thread> workers;
  /// 任务队列
  std::queue<std::function<void()>> tasks;

  std::mutex queue_mutex;  ///< 用于同步的互斥锁和条件变量
  std::condition_variable condition;  ///< 信号量
  bool stop;  ///< 用于判断所有线程是否需要结束
};

#endif