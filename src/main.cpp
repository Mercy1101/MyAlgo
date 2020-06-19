#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>  // include after defining CATCH_CONFIG_RUNNER
#include <iostream>
#include <limits>

#include "Version/Version.h"  // for print PrintVersion()
#include "init/init.h"
#include "leetcode.h"
#include "profiler/Profiler.h"
#include "utility/utility.h"


int main(int argc, char** argv) {
  PrintVersion();
  CheckConfigFolder();
  Catch::Session().run(argc, argv);
  /// 2, 4, 3 5, 6, 4
    auto node1 = new Lee::Leetcode::ListNode(2);
    node1->next = new Lee::Leetcode::ListNode(4);
    node1->next->next = new Lee::Leetcode::ListNode(3);

    auto node2 = new Lee::Leetcode::ListNode(5);
    node2->next = new Lee::Leetcode::ListNode(6);
    node2->next->next = new Lee::Leetcode::ListNode(4);
    auto it = Lee::Leetcode::addTwoNumbers(node1, node2);
    (void)it;

    std::vector<int> vec{9};

   auto vo = Lee::Leetcode::plusOne(vec);
   auto vo1 = Lee::Leetcode::plusOne(vec);
   (void)vo;
   (void)vo1;
    
  system("pause");
  return 0;
}
