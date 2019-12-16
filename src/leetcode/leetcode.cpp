#include "leetcode.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>
#include "utility/utility.h"

namespace Lee {
namespace Leetcode {

bool isPalindrome(std::string s) {
  if (s.empty()) return false;
  if (s.length() == 1 && !::isalnum(*s.begin())) return false;
#pragma warning(disable : 4244)  // 屏蔽tolower的警告
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  auto left = s.begin();
  auto right = std::prev(s.end());
  while (left < right) {
    if (!::isalnum(*left)) {
      ++left;
    } else if (!::isalnum(*right)) {
      --right;
    } else if (*left != *right) {
      return false;
    } else {
      ++left;
      --right;
    }
  }
  return true;
}

bool IsValidParentheses(std::string const& s) {
  if (s.empty()) return false;

  std::string left = "([{";
  std::string right = ")]}";
  std::stack<char> stk;
  for (const auto& c : s) {
    if (left.find(c) != std::string::npos) {
      stk.push(c);
    } else {
      if (stk.empty() || stk.top() != left.at(right.find(c)))
        return false;
      else
        stk.pop();
    }
  }
  return stk.empty();
}

bool ParseChanNumber(const std::string& str,
                            std::vector<int>* chan_number_vec) {
  /// 通道分割符
  static std::string trim_char("-");
  /// 非法字符集
  static std::string invalid_char(".*!@#$%^()");
  try {
    if (str.empty()) return false;

    for (auto it : invalid_char) {
      if (std::string::npos != str.find(it)) {
        return false;
      }
    }
    size_t last_pos = 0;
    for (auto pos = str.find(trim_char); pos != std::string::npos;
         pos = str.find(trim_char, last_pos)) {
      auto chan_number = std::stoi(str.substr(last_pos, pos));
      if (chan_number < 0 || chan_number > 15) {
        throw std::invalid_argument("Chan number out of range");
      } else {
        chan_number_vec->emplace_back(chan_number);
      }
      last_pos = pos + 1;
    }

    auto chan_number = std::stoi(str.substr(last_pos, str.length()));
    if (chan_number < 0 || chan_number > 15) {
      throw std::invalid_argument("Chan number out of range");
    } else {
      chan_number_vec->emplace_back(chan_number);
      std::sort(chan_number_vec->begin(), chan_number_vec->end());
      if (chan_number_vec->end() !=
          std::unique(chan_number_vec->begin(), chan_number_vec->end())) {
        throw std::invalid_argument("duplication chan number");
      }
      return true;
    }
  } catch (const std::invalid_argument& e) {
    Lee::ignore_unused(e);
    return false;
  } catch (const std::out_of_range& e) {
    Lee::ignore_unused(e);
    return false;
  } catch (const std::exception& e) {
    Lee::ignore_unused(e);
    return false;
  }
}

}  // end of namespace Leetcode
}  // end of namespace Lee
