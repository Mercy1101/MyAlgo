#include "leetcode.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>


namespace Lee {
namespace Leetcode {

bool isPalindrome(std::string s) {
  if (s.empty()) return false;
  if (s.length() == 1 && !::isalnum(*s.begin())) return false;
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  auto left = s.begin();
  auto right = std::prev(s.end());
  while (left < right) {
    if (!::isalnum(*left))
      ++left;
    else if (!::isalnum(*right))
      --right;
    else if (*left != *right)
      return false;
    else {
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

}  // end of namespace Leetcode
}  // end of namespace Lee