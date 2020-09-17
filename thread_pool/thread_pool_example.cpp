#include <iostream>

#include "thread_pool.hpp"

int print_int(int num) {
  /// 模拟执行时间3秒
  std::this_thread::sleep_for(std::chrono::seconds(3));
  return num;
}
std::string print_string(std::string s) {
  /// 模拟执行时间1秒
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return s;
}
double print_double(double num) {
  /// 模拟执行时间5秒
  std::this_thread::sleep_for(std::chrono::seconds(5));
  return num;
}

int main() {
  ThreadPool pool;
  auto result_int = pool.enqueue(print_int, 111);
  auto result_double = pool.enqueue(print_double, 0.2222);
  auto result_string = pool.enqueue(print_string, "this is test!");
  /// 阻塞式获取异步执行的函数的返回值
  std::cout << "result_string() return " << result_string.get() << std::endl;

  /// 每500毫秒查看一次函数是否执行完毕
  while (result_int.wait_for(std::chrono::milliseconds(500)) !=
         std::future_status::ready) {
    std::cout << "waitting for result_int" << std::endl;
  }
  std::cout << "result_int() return " << result_int.get() << std::endl;

  if (result_double.wait_for(std::chrono::milliseconds(500)) !=
      std::future_status::ready) {
    /// 如果等待一个500毫秒后，函数还是没有执行完毕，则打印获取失败。
    std::cout << "excute function print_double time_out." << std::endl;
  }

  std::cout << "finally result_double return " << result_double.get()
            << std::endl;
  system("pause");
}
