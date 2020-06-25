#include "concurrentqueue/concurrentqueue.h"
#include "utility/utility.h"
#include <cassert>
#include <catch2/catch.hpp>
#include <thread>

TEST_CASE("第一个例子", "[Concurrentqueue][Sample]") {
  moodycamel::ConcurrentQueue<int> q;

  for (int i = 0; i != 123; ++i) q.enqueue(i);

  int item;
  for (int i = 0; i != 123; ++i) {
    q.try_dequeue(item);
    REQUIRE(item == i);
  }
}

TEST_CASE("并行例子", "[Concurrentqueue][Sample]") {
  moodycamel::ConcurrentQueue<int> q;
  int dequeued[100] = {0};
  std::thread threads[20];

  // Producers
  for (int i = 0; i != 10; ++i) {
    threads[i] = std::thread(
        [&](int i) {
          for (int j = 0; j != 10; ++j) {
            q.enqueue(i * 10 + j);
          }
        },
        i);
  }

  // Consumers
  for (int i = 10; i != 20; ++i) {
    threads[i] = std::thread([&]() {
      int item;
      for (int j = 0; j != 20; ++j) {
        if (q.try_dequeue(item)) {
          ++dequeued[item];
        }
      }
    });
  }

  // Wait for all threads
  for (int i = 0; i != 20; ++i) {
    threads[i].join();
  }

  // Collect any leftovers (could be some if e.g. consumers finish before
  // producers)
  int item;
  while (q.try_dequeue(item)) {
    ++dequeued[item];
  }

  // Make sure everything went in and came back out!
  for (int i = 0; i != 100; ++i) {
    REQUIRE(dequeued[i] == 1);
  }
}
TEST_CASE("BulkUp批量例子", "[Concurrentqueue][Sample]") {
  moodycamel::ConcurrentQueue<int> q;
  int dequeued[100] = {0};
  std::thread threads[20];

  // Producers
  for (int i = 0; i != 10; ++i) {
    threads[i] = std::thread(
        [&](int i) {
          int items[10];
          for (int j = 0; j != 10; ++j) {
            items[j] = i * 10 + j;
          }
          q.enqueue_bulk(items, 10);
        },
        i);
  }

  // Consumers
  for (int i = 10; i != 20; ++i) {
    threads[i] = std::thread([&]() {
      int items[20];
      for (std::size_t count = q.try_dequeue_bulk(items, 20); count != 0;
           --count) {
        ++dequeued[items[count - 1]];
      }
    });
  }

  // Wait for all threads
  for (int i = 0; i != 20; ++i) {
    threads[i].join();
  }

  // Collect any leftovers (could be some if e.g. consumers finish before
  // producers)
  int items[10];
  std::size_t count;
  while ((count = q.try_dequeue_bulk(items, 10)) != 0) {
    for (std::size_t i = 0; i != count; ++i) {
      ++dequeued[items[i]];
    }
  }

  // Make sure everything went in and came back out!
  for (int i = 0; i != 100; ++i) {
    assert(dequeued[i] == 1);
  }
}

TEST_CASE("dequeue大于队列值的结果", "[Concurrentqueue][Sample]") {
  moodycamel::ConcurrentQueue<int> q;
  int items[5];
  for (int j = 0; j != lee::ArraySize(items); ++j) {
    items[j] = 10 + j;
  }
  /// 注意下面这句话的第二个参数,如果该数值超过了items所拥有的元素个数,将会将垃圾值入列
  q.enqueue_bulk(items, lee::ArraySize(items));
  int items2[11];
  auto count = q.try_dequeue_bulk(items2, 10);
  REQUIRE(count == lee::ArraySize(items));
}
