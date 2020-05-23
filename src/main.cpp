#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>  // include after defining CATCH_CONFIG_RUNNER
#include <iostream>

#include "Version/Version.h"  // for print PrintVersion()
#include "init/init.h"
#include "utility/utility.h"
#include "profiler/Profiler.h"

// "算法-二分查找"
// "树-二叉树"
// "树-AVL树"
// "树-伸展树"
// "树-B-树"
// "散列-分离链接法"
// "散列-开放定址法"
// "散列-线性探测法"
// "散列-平方探测法"
// "散列-双散列"
// "散列-再散列"
// "散列-可扩散列"
// "二叉堆"
// "优先队列"
// "d-堆"
// "左式堆"
// "斜堆"
// "二项队列"
// "排序-希尔排序"
// "排序-堆排序"
// "排序-归并排序"
// "排序-快速排序"
// "排序-桶式排序"
// "排序-外部排序"
// "排序-多路合并"
// "排序-外部排序-替换选择"
// "不相交集"
// "Union/Find"
// "图论-"

// 不重复素数生成器
// RSA加密
// 打印非完全二叉树
// 二叉树的实现
int main(int argc, char** argv) {
  PrintVersion();
  CheckConfigFolder();

  Catch::Session().run(argc, argv);
  system("pause");
  return 0;
}
