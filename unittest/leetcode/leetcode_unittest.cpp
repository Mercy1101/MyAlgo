#include "leetcode.h"

#include <catch2/catch.hpp>

#include "utility/utility.h"

/**
 * @brief lee::leetcode::IsValidParentheses()函数的简单测试
 */
TEST_CASE("IsValidParentheses简单测试", "[leetcode][IsValidParentheses]") {
  using namespace lee;
  REQUIRE((leetcode::IsValidParentheses("((){{}([[]{[]}]())})")));
  REQUIRE((leetcode::IsValidParentheses("()")));
  REQUIRE((leetcode::IsValidParentheses("()[]")));

  REQUIRE_FALSE((leetcode::IsValidParentheses("")));
  REQUIRE_FALSE((leetcode::IsValidParentheses("(]")));
  REQUIRE_FALSE((leetcode::IsValidParentheses("([)]")));
  REQUIRE_FALSE((leetcode::IsValidParentheses("(")));
  REQUIRE_FALSE((leetcode::IsValidParentheses(")")));
}

SCENARIO("ParseChanNumber 简单测试", "[leetcode][ParseChanNumber]") {
  std::vector<int> chan_number_vec;
  GIVEN("正确参数测试") {
    WHEN("入参为: 0-1-2-3-4-5") {
      std::string x1("0-1-2-3-4-5");
      THEN("ParseChanNumber应该返回真") {
        REQUIRE((lee::leetcode::ParseChanNumber(x1, &chan_number_vec)));
        REQUIRE((chan_number_vec.size() == 6));
        REQUIRE(
            (std::is_sorted(chan_number_vec.begin(), chan_number_vec.end())));
        for (size_t i = 0; i < chan_number_vec.size(); ++i) {
          REQUIRE((chan_number_vec.at(i) == i));
        }
        chan_number_vec.clear();
      }
    }
    WHEN("入参为: 11-12-13-14-15") {
      std::string x2("11-13-12-14-15");
      THEN("ParseChanNumber应该返回真") {
        REQUIRE((lee::leetcode::ParseChanNumber(x2, &chan_number_vec)));
        REQUIRE((chan_number_vec.size() == 5));
        REQUIRE(
            (std::is_sorted(chan_number_vec.begin(), chan_number_vec.end())));
        for (size_t i = 0; i < chan_number_vec.size(); ++i) {
          REQUIRE((chan_number_vec.at(i) == i + 11));
        }
        chan_number_vec.clear();
      }
    }
    WHEN("入参为: 单个有效通道") {
      std::string x3("12");
      THEN("ParseChanNumber应该返回真") {
        REQUIRE((lee::leetcode::ParseChanNumber(x3, &chan_number_vec)));
        REQUIRE((chan_number_vec.size() == 1));
        REQUIRE((chan_number_vec.at(0) == 12));
        chan_number_vec.clear();
      }
    }
    GIVEN("无效参数测试") {
      WHEN("入参为: -12") {
        std::string x4("-12");
        THEN("ParseChanNumber应该返回真") {
          REQUIRE_FALSE((lee::leetcode::ParseChanNumber(x4, &chan_number_vec)));
          chan_number_vec.clear();
        }
      }
      WHEN("入参为: 12-") {
        std::string x4("12-");
        THEN("ParseChanNumber应该返回真") {
          REQUIRE_FALSE((lee::leetcode::ParseChanNumber(x4, &chan_number_vec)));
          chan_number_vec.clear();
        }
      }
      WHEN("入参为空") {
        std::string x5("12-");
        THEN("ParseChanNumber应该返回真") {
          REQUIRE_FALSE((lee::leetcode::ParseChanNumber(x5, &chan_number_vec)));
          chan_number_vec.clear();
        }
      }
      WHEN("入参为含有无效字母") {
        std::string x6("-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE((lee::leetcode::ParseChanNumber(x6, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x7("ss-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE((lee::leetcode::ParseChanNumber(x7, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x8("ss");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE((lee::leetcode::ParseChanNumber(x8, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x9("-ss");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE((lee::leetcode::ParseChanNumber(x9, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x10(".");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(
              (lee::leetcode::ParseChanNumber(x10, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x11("--");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(
              (lee::leetcode::ParseChanNumber(x11, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x12("---------------------------");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(
              (lee::leetcode::ParseChanNumber(x12, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x13(".6");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(
              (lee::leetcode::ParseChanNumber(x13, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x14(".7-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(
              (lee::leetcode::ParseChanNumber(x14, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x15("-12-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(
              (lee::leetcode::ParseChanNumber(x15, &chan_number_vec)));
          chan_number_vec.clear();
        }
        std::string x16(",77-");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(lee::leetcode::ParseChanNumber(x16, &chan_number_vec));
          chan_number_vec.clear();
        }
      }
      WHEN("入参含有浮点数") {
        std::string x17("1.1-2.2-3.3");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(lee::leetcode::ParseChanNumber(x17, &chan_number_vec));
        }
        std::string x18("1.1");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(lee::leetcode::ParseChanNumber(x18, &chan_number_vec));
        }
      }
      WHEN("入参含有重复通道") {
        std::string x19("7-7");
        THEN("ParseChanNumber应该返回假") {
          REQUIRE_FALSE(lee::leetcode::ParseChanNumber(x19, &chan_number_vec));
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
        std::vector<int> result = lee::leetcode::top_k_frequent(input, 1);
        REQUIRE((result == true_result));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: num = [1], k = 1") {
      THEN("Output: [1]") {
        std::vector<int> input{1};
        std::vector<int> true_result{1};
        std::vector<int> result = lee::leetcode::top_k_frequent(input, 1);
        REQUIRE((result == true_result));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

SCENARIO("sort_string 简单测试", "[leetcode][sort_string]") {
  GIVEN("简单测试") {
    WHEN("Input: s = [\"aaaabbbbcccc\"]") {
      THEN("Output: [\"abccbaabccba\"]") {
        REQUIRE(lee::leetcode::sort_string("aaaabbbbcccc") == "abccbaabccba");
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: s = [\"rat\"]") {
      THEN("Output: [\"art\"]") {
        REQUIRE(lee::leetcode::sort_string("rat") == "art");
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: s = [\"leetcode\"]") {
      THEN("Output: [\"cdelotee\"]") {
        REQUIRE(lee::leetcode::sort_string("leetcode") == "cdelotee");
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: s = [\"ggggggg\"]") {
      THEN("Output: [\"ggggggg\"]") {
        REQUIRE(lee::leetcode::sort_string("ggggggg") == "ggggggg");
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: s = [\"spo\"]") {
      THEN("Output: [\"ops\"]") {
        REQUIRE(lee::leetcode::sort_string("spo") == "ops");
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

SCENARIO("generate_pascal_trangel 简单测试",
         "[leetcode][generate_pascal_trangel]") {
  GIVEN("简单测试") {
    WHEN("Input: s = [5]") {
      THEN("Output: [ [1], [1,1], [1,2,1], [1,3,3,1], [1,4,6,4,1]]") {
        auto result = lee::leetcode::generate_pascal_trangel(5);
        std::vector<std::vector<int>> true_result;
        true_result.push_back({1});
        true_result.push_back({1, 1});
        true_result.push_back({1, 2, 1});
        true_result.push_back({1, 3, 3, 1});
        true_result.push_back({1, 4, 6, 4, 1});
        REQUIRE((result == true_result));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

SCENARIO(" equations_possible 简单测试", "[leetcode][equations_possible]") {
  GIVEN("简单测试") {
    WHEN("Input: [a==b, b!=a]") {
      THEN("Output: false") {
        std::vector<std::string> input;
        input.emplace_back("a==b");
        input.emplace_back("b!=a");
        REQUIRE(!lee::leetcode::equations_possible(input));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: [b==a, a==b]") {
      THEN("Output: true") {
        std::vector<std::string> input;
        input.emplace_back("a==b");
        input.emplace_back("b==a");
        REQUIRE(lee::leetcode::equations_possible(input));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: [a==b, b==c, a==c]") {
      THEN("Output: true") {
        std::vector<std::string> input;
        input.emplace_back("a==b");
        input.emplace_back("b==c");
        input.emplace_back("a==c");
        REQUIRE(lee::leetcode::equations_possible(input));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: [a==b, b!=c, c==a]") {
      THEN("Output: false") {
        std::vector<std::string> input;
        input.emplace_back("a==b");
        input.emplace_back("b!=c");
        input.emplace_back("c==a");
        REQUIRE(!lee::leetcode::equations_possible(input));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: [c==c, d==c, x!=z]") {
      THEN("Output: true") {
        std::vector<std::string> input;
        input.emplace_back("c==c");
        input.emplace_back("d==c");
        input.emplace_back("x!=z");
        REQUIRE(lee::leetcode::equations_possible(input));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

SCENARIO("min_window 简单测试", "[leetcode][min_window]") {
  GIVEN("简单测试") {
    WHEN("Input: S = ADOBECODEBANC, T = ABC") {
      THEN("Output: BANC") {
        auto str = lee::leetcode::min_window("ADOBECODEBANC", "ABC");
        REQUIRE((0 == str.compare("BANC")));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: S = ADOECODEANC, T = ABC") {
      THEN("Output: empty_string") {
        REQUIRE(lee::leetcode::min_window("ADOECODEANC", "ABC").empty());
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: S = AB, T = A") {
      THEN("Output: A") {
        REQUIRE((0 == lee::leetcode::min_window("AB", "A").compare("A")));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
  GIVEN("输入字符串比子串小") {
    WHEN("Innput: S = A, T = ABC") {
      THEN("Output: empty_string") {
        REQUIRE(lee::leetcode::min_window("A", "ABC").empty());
      }  ///< THEN
    }    ///< WHEN
    WHEN("Innput: S = A, T = AA") {
      THEN("Output: empty_string") {
        REQUIRE(lee::leetcode::min_window("A", "AA").empty());
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN

  GIVEN("子字符串有重复的") {
    WHEN("Innput: S = ABAA, T = AA") {
      THEN("Output: AA") {
        REQUIRE(0 == lee::leetcode::min_window("ABAA", "AA").compare("AA"));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Innput: S = ABACHKA, T = AA") {
      THEN("Output: ABA") {
        REQUIRE(0 == lee::leetcode::min_window("ABACHKA", "AA").compare("ABA"));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

SCENARIO("climb_statirs 简单测试", "[leetcode][climb_statirs]") {
  GIVEN("简单测试") {
    WHEN("Input: 2") {
      THEN("Output: 2") {
        REQUIRE((2 == lee::leetcode::climb_stairs(2)));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: 3") {
      THEN("Output: 3") {
        REQUIRE((3 == lee::leetcode::climb_stairs(3)));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

SCENARIO("myAtoi 简单测试", "[leetcode][myAtoi]") {
  GIVEN("简单测试") {
    WHEN("Input: 42") {
      THEN("Output: 42") {
        REQUIRE((42 == lee::leetcode::myAtoi("42")));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: -42") {
      THEN("Output: -42") {
        REQUIRE((-42 == lee::leetcode::myAtoi("-42")));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: [               42]") {
      THEN("Output: 42") {
        REQUIRE((42 == lee::leetcode::myAtoi("42")));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: [-42]") {
      THEN("Output: 0") {
        REQUIRE((-42 == lee::leetcode::myAtoi("                -42")));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: +-42") {
      THEN("Output: 0") {
        REQUIRE((0 == lee::leetcode::myAtoi("+-42")));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: -+42") {
      THEN("Output: 0") {
        REQUIRE((0 == lee::leetcode::myAtoi("-+42")));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: [4193 with words]") {
      THEN("Output: 4193") {
        REQUIRE((4193 == lee::leetcode::myAtoi("4193 with words")));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: [-91283492332]") {
      THEN("Output: INT_MIN") {
        REQUIRE((INT_MIN == lee::leetcode::myAtoi("-91283492332")));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: [2147483646]") {
      THEN("Output: 2147483646") {
        REQUIRE((2147483646 == lee::leetcode::myAtoi("2147483646")));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: [2147483648]") {
      THEN("Output:  2147483647") {
        REQUIRE((2147483647 == lee::leetcode::myAtoi("2147483648")));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

SCENARIO("strStr简单测试", "[leetcode][strStr]") {
  GIVEN("简单测试") {
    WHEN("Input: aaaa, bbba") {
      THEN("Output: -1") {
        REQUIRE((-1 == lee::leetcode::strStr("aaaaa", "bba")));
      }  ///< THEN
    }    ///< WHEN

  }  ///< GIVEN
}  ///< SCENARIO

SCENARIO("lengthOfLongestSubstring 简单测试",
         "[leetcode][lengthOfLongestSubstring]") {
  GIVEN("简单测试") {
    WHEN("Input: abcabcbb") {
      THEN("Output: 3") {
        REQUIRE((3 == lee::leetcode::lengthOfLongestSubstring("abcabcbb")));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: bbbbbbb") {
      THEN("Output: b") {
        REQUIRE((1 == lee::leetcode::lengthOfLongestSubstring("bbbbbbb")));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: pwwkew") {
      THEN("Output: 3") {
        REQUIRE((3 == lee::leetcode::lengthOfLongestSubstring("pwwkew")));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: ") {
      THEN("Output: 0") {
        REQUIRE((0 == lee::leetcode::lengthOfLongestSubstring("")));
      }  ///< THEN
    }    ///< WHEN

  }  ///< GIVEN
}  ///< SCENARIO

SCENARIO("checkInclusion 简单测试", "[leetcode][checkInclusion]") {
  GIVEN("简单测试") {
    WHEN("Input: s1 = ab s2 = eidbaooo") {
      THEN("Output: true") {
        REQUIRE(lee::leetcode::checkInclusion("ab", "eidbaooo"));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: s1 = ab s2 = eidboaooo") {
      THEN("Output: false") {
        REQUIRE(!lee::leetcode::checkInclusion("ab", "eidboaooo"));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

SCENARIO("multiply 简单测试", "[leetcode][multiply]") {
  GIVEN("简单测试") {
    WHEN("num1 = 2, num2 = 3") {
      THEN("Output: true") {
        REQUIRE("6" == lee::leetcode::multiply("2", "3"));
      }  ///< THEN
    }    ///< WHEN

    WHEN("Input: s1 = 123 s2 = 456") {
      THEN("Output: 56088") {
        REQUIRE(("56088" == lee::leetcode::multiply("123", "456")));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO

TEST_CASE("serialize 简单测试", "[leetcode][serialize]") {
  using namespace lee::leetcode;
  TreeNode* root = new TreeNode(5);
  TreeNode* left = nullptr;
  TreeNode* right = new TreeNode(7);
  root->left = left;
  root->right = right;
  right->left = new TreeNode(6);
  right->right = new TreeNode(8);

  auto temp = serialize(root);
  REQUIRE(temp == std::string("[5,null,7,null,null,6,8]"));

  TreeNode* root1 = new TreeNode(50);
  TreeNode* left1 = new TreeNode(20);
  TreeNode* right1 = new TreeNode(70);
  root1->left = left1;
  root1->right = right1;
  left1->left = new TreeNode(10);
  left1->right = nullptr;
  /// new TreeNode(30);
  right1->left = new TreeNode(60);
  right1->right = new TreeNode(80);
  auto left_left = left1->left;
  left_left->left = new TreeNode(5);
  left_left->right = new TreeNode(15);
  /// auto left_right = left1->right;
  /// left_right->left = nullptr;
  /// new TreeNode(25);
  /// left_right->right = new TreeNode(35);

  auto right_left = right1->left;
  right_left->left = new TreeNode(55);
  right_left->right = new TreeNode(65);
  auto right_right = right1->right;
  right_right->left = new TreeNode(75);
  right_right->right = new TreeNode(85);

  auto temp1 = serialize(root1);
  REQUIRE(temp1 ==
          std::string("[50,20,70,10,null,60,80,5,15,null,null,55,65,75,85]"));
}

TEST_CASE("deserialize 简单测试", "[leetcode][deserialize]") {
  using namespace lee::leetcode;
  auto result = deserialize("[5,null,7,null,null,6,8]");
  REQUIRE(serialize(result) == "[5,null,7,null,null,6,8]");

  auto string =
      "[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,"
      "27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,"
      "51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,"
      "75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,"
      "99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,"
      "117,118,119,120,121,122,123,124,125,126]";
  auto result3 = deserialize(string);
  REQUIRE(serialize(result3) == string);

}
