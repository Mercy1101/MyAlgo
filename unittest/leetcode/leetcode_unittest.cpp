#include "leetcode.h"

#include <catch2/catch.hpp>

#include "utility/utility.h"

/**
 * @brief Lee::Leetcode::isPalindrome()函数的简单测试
 */
TEST_CASE("isPalindrome简单测试", "[leetcode][isPalindrome]") {
  using namespace Lee;
  REQUIRE(Leetcode::isPalindrome("a"));
  REQUIRE(Leetcode::isPalindrome("aba"));
  REQUIRE(Leetcode::isPalindrome("ABa"));
  REQUIRE(Leetcode::isPalindrome("A man, a plan, a canal: Panama"));

  REQUIRE_FALSE(Leetcode::isPalindrome(","));
  REQUIRE_FALSE(Leetcode::isPalindrome(""));
  REQUIRE_FALSE(Leetcode::isPalindrome("race a car"));
}

/**
 * @brief Lee::Leetcode::IsValidParentheses()函数的简单测试
 */
TEST_CASE("IsValidParentheses简单测试", "[leetcode][IsValidParentheses]") {
  using namespace Lee;
  REQUIRE(Leetcode::IsValidParentheses("((){{}([[]{[]}]())})"));
  REQUIRE(Leetcode::IsValidParentheses("()"));
  REQUIRE(Leetcode::IsValidParentheses("()[]"));

  REQUIRE_FALSE(Leetcode::IsValidParentheses(""));
  REQUIRE_FALSE(Leetcode::IsValidParentheses("(]"));
  REQUIRE_FALSE(Leetcode::IsValidParentheses("([)]"));
  REQUIRE_FALSE(Leetcode::IsValidParentheses("("));
  REQUIRE_FALSE(Leetcode::IsValidParentheses(")"));
}

SCENARIO("ParseChanNumber 简单测试", "[leetcode][ParseChanNumber]") {
  std::vector<int> chan_number_vec;
  GIVEN("正确参数测试") {
    WHEN("入参为: 0-1-2-3-4-5") {
      std::string x1("0-1-2-3-4-5");
      THEN("ParseChanNumber应该返回真") {
        REQUIRE(Lee::Leetcode::ParseChanNumber(x1, &chan_number_vec));
        REQUIRE(chan_number_vec.size() == 6);
        REQUIRE(std::is_sorted(chan_number_vec.begin(), chan_number_vec.end()));
        for (size_t i = 0; i < chan_number_vec.size(); ++i) {
          REQUIRE(chan_number_vec.at(i) == i);
        }
        chan_number_vec.clear();
      }
    }
    WHEN("入参为: 11-12-13-14-15") {
      std::string x2("11-13-12-14-15");
      THEN("ParseChanNumber应该返回真") {
        REQUIRE(Lee::Leetcode::ParseChanNumber(x2, &chan_number_vec));
        REQUIRE(chan_number_vec.size() == 5);
        REQUIRE(std::is_sorted(chan_number_vec.begin(), chan_number_vec.end()));
        for (size_t i = 0; i < chan_number_vec.size(); ++i) {
          REQUIRE(chan_number_vec.at(i) == i + 11);
        }
        chan_number_vec.clear();
      }
    }
    WHEN("入参为: 单个有效通道") {
      std::string x3("12");
      THEN("ParseChanNumber应该返回真") {
        REQUIRE(Lee::Leetcode::ParseChanNumber(x3, &chan_number_vec));
        REQUIRE(chan_number_vec.size() == 1);
        REQUIRE(chan_number_vec.at(0) == 12);
        chan_number_vec.clear();
      }
    }
    GIVEN("无效参数测试") {
      WHEN("入参为: -12") {
        std::string x4("-12");
        THEN("ParseChanNumber应该返回真") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x4, &chan_number_vec));
          chan_number_vec.clear();
        }
      }
      WHEN("入参为: 12-") {
        std::string x4("12-");
        THEN("ParseChanNumber应该返回真") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x4, &chan_number_vec));
          chan_number_vec.clear();
        }
      }
      WHEN("入参为空") {
        std::string x5("12-");
        THEN("ParseChanNumber应该返回真") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x5, &chan_number_vec));
          chan_number_vec.clear();
        }
      }
      WHEN("入参为含有无效字母") {
        std::string x6("-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x6, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x7("ss-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x7, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x8("ss");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x8, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x9("-ss");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x9, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x10(".");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x10, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x11("--");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x11, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x12("---------------------------");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x12, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x13(".6");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x13, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x14(".7-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x14, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x15("-12-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x15, &chan_number_vec));
          chan_number_vec.clear();
        }
        std::string x16(",77-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x16, &chan_number_vec));
          chan_number_vec.clear();
        }
      }
      WHEN("入参含有浮点数") {
        std::string x17("1.1-2.2-3.3");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x17, &chan_number_vec));
        }
        std::string x18("1.1");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x18, &chan_number_vec));
        }
      }
      WHEN("入参含有重复通道") {
        std::string x19("7-7");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(Lee::Leetcode::ParseChanNumber(x19, &chan_number_vec));
        }
      }
    }
  }
}

SCENARIO("top_k_frequent 简单测试", "[leetcode][top_k_frequent]") {
  GIVEN("简单测试") {
    WHEN("Input: num = [1,1,1,2,2,3], k = 1") {
      THEN("Output: [1]") {
        std::vector<int> input{1, 1, 1, 2, 2, 3};
        std::vector<int> true_result{1};
        std::vector<int> result = Lee::Leetcode::top_k_frequent(input, 1);
        REQUIRE(Lee::compare_vector(result, true_result));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: num = [1], k = 1") {
      THEN("Output: [1]") {
        std::vector<int> input{1};
        std::vector<int> true_result{1};
        std::vector<int> result = Lee::Leetcode::top_k_frequent(input, 1);
        REQUIRE(Lee::compare_vector(result, true_result));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

SCENARIO("sort_string 简单测试", "[leetcode][sort_string]") {
  GIVEN("简单测试") {
    WHEN("Input: s = [\"aaaabbbbcccc\"]") {
      THEN("Output: [\"abccbaabccba\"]") {
        REQUIRE(Lee::Leetcode::sort_string("aaaabbbbcccc") == "abccbaabccba");
      } ///< THEN
    } ///< WHEN

    WHEN("Input: s = [\"rat\"]") {
      THEN("Output: [\"art\"]") {
        REQUIRE(Lee::Leetcode::sort_string("rat") == "art");
      } ///< THEN
    } ///< WHEN

    WHEN("Input: s = [\"leetcode\"]") {
      THEN("Output: [\"cdelotee\"]") {
        REQUIRE(Lee::Leetcode::sort_string("leetcode") == "cdelotee");
      } ///< THEN
    } ///< WHEN

    WHEN("Input: s = [\"ggggggg\"]") {
      THEN("Output: [\"ggggggg\"]") {
        REQUIRE(Lee::Leetcode::sort_string("ggggggg") == "ggggggg");
      } ///< THEN
    } ///< WHEN

    WHEN("Input: s = [\"spo\"]") {
      THEN("Output: [\"ops\"]") {
        REQUIRE(Lee::Leetcode::sort_string("spo") == "ops");
      } ///< THEN
    } ///< WHEN
  } ///< GIVEN
} ///< SCENARIO
