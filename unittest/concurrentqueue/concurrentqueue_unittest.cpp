#include <cassert>
#include <thread>

#include "concurrentqueue/concurrentqueue.h"
#include "gtest/gtest.h"

TEST(ConcurrentqueueSample, HelloQueue) {
  moodycamel::ConcurrentQueue<int> q;

  for (int i = 0; i != 123; ++i) q.enqueue(i);

  int item;
  for (int i = 0; i != 123; ++i) {
    q.try_dequeue(item);
    ASSERT_TRUE(item == i);
  }
}

TEST(ConcurrentqueueSample, HelloConcurrency) {
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
    ASSERT_TRUE(dequeued[i] == 1);
  }
}

TEST(ConcurrentqueueSample, BulkUp) {
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
