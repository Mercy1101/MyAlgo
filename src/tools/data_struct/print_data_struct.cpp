#include "data_struct/print_data_struct.h"
#include <iostream>
#include "utility/utility.h"

namespace Lee {
inline namespace data_struct {
inline namespace print_data_struct {
const char g_letter[36] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                           '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                           'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                           'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

void PrintSpace(const int &space_number) {
  for (int i = 0; i < space_number - 1; ++i) {
    std::cout << ' ';
  }
}

void PrintElement(size_t element_number) {
  while (element_number > Lee::ArraySize(g_letter) - 1) {
    element_number -= (Lee::ArraySize(g_letter) - 1);
  }
  std::cout << g_letter[element_number];
}

void PrintLine(const int &line_number) {
  for (int i = 0; i < line_number - 1; ++i) {
    std::cout << '_';
  }
}

void PrintCompleteTree(const int &depth) {
  std::cout << "\n";
  if (depth == 0 || depth > 6) {
    std::cout << "Invalid Depth Param!"
              << "\n";
    return;
  }

  for (int i = 0; i < depth + 1; ++i) {  /// 深度循环
    int element_count = Lee::power(2, i);
    int last_offset = 0;
    for (int count = 0; count < element_count; ++count) {  /// 单行元素循环
      auto off_set = (((count + 1) * 2) - 1) * Lee::power(2, (depth - i));
      if (count != 0) {
        PrintLine(off_set - last_offset);
      } else {
        PrintSpace(off_set - last_offset);
      }
      last_offset = off_set;
      PrintElement(count);
    }
    std::cout << "\n";
    std::cout << "\n";
  }
  std::cout << "\n";
}
}  // namespace print_data_struct
}  // namespace data_struct
}  // namespace Lee