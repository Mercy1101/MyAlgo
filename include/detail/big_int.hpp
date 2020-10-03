///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   big_int.h
/// @brief  大整数类型的对象实现
///
/// @author lijiancong, pipinstall@163.com
/// @date  2020-10-03 12:44:50
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_BIG_INT_HPP_
#define INCLUDE_DETAIL_BIG_INT_HPP_

#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace lee {
inline namespace bigint {
class big_int {
 public:
  big_int() {}
  big_int(long long i) {
    positive_ = (i >= 0);
    while (i != 0) {
      num_.push_back(static_cast<int>(i % base_));
      i /= base_;
    }
  }

  big_int(const big_int &other)
      : num_(other.num_), positive_(other.positive_), base_(other.base_) {}

  big_int(const std::string &str) {
    positive_ = str.at(0) != '-';

    int size = static_cast<int>(str.size());
    while (true) {
      if (size == 0) {
        break;
      }
      if (!positive_ && size <= 1) {
        throw std::logic_error("invalid param in big_int!");
      }
      int length = 0;
      int num = 0;
      int prefix = 1;
      for (int i = size - 1; i >= 0 && i >= size - 9; --i) {
        if (str.at(i) < '0' || str.at(i) > '9') {
          throw std::logic_error("invalid param in big_int!");
        }
        num += (str.at(i) - '0') * prefix;
        prefix *= 10;
        ++length;
      }
      num_.push_back(num);
      size -= length;
    }
  }

  big_int &operator+(long long i) const {
    big_int c = *this;
    c += i;
    return c;
  }

  big_int &operator+=(long long i) {
    auto it = num_.begin();
    if (skip_ > num_.size() - 1) {
      num_.insert(num_.end(), skip_ - num_.size(), 0);
    }
    it += skip_;
    do {
      if (it != num_.end()) {
        *it += i % base_;
        i /= base_;
        i += *it / base_;  ///< i 加上进位
        *it %= base_;      ///< 当前值消除进位
        ++it;
      } else {
        num_.push_back(0);  ///< 添加一位，下一个循环再赋值
        it = std::prev(num_.end());  ///< 更新迭代器为有效状态
      }
    } while (i != 0);
    return *this;
  }

  big_int operator=(const long long &a) {
    num_.clear();
    long long t = a;
    do {
      num_.push_back(static_cast<int>(t % base_));
      t /= base_;
    } while (t != 0);

    return *this;
  }
  big_int &operator=(const big_int &other) {
    big_int temp(other.to_string());
    *this = temp;
    /// *this<<other;
    return *this;
  }

  big_int operator+(const big_int &b) const {
    big_int c = *this;
    c += b;

    return c;
  }

  big_int &operator+=(const big_int &b) {
    if (!b.positive_) {
      return *this -= b;
    }

    auto it1 = num_.begin();
    auto it2 = b.num_.begin();
    int sum = 0;
    while (it1 != num_.end() || it2 != num_.end()) {
      if (it1 != num_.end()) {
        sum += *it1;
      } else {
        num_.push_back(0);
        it1 = std::prev(num_.end());
      }
      if (it2 != b.num_.end()) {
        sum += *it2;
        ++it2;
      }
      *it1 = sum % base_;
      ++it1;
      sum /= base_;
    }
    if (sum != 0) {
      assert(sum != 1);
      num_.push_back(1);
    }
    return *this;
  }
  big_int operator-(const big_int &b) const {
    big_int c = *this;
    c -= b;

    return c;
  }

  big_int &operator-=(const big_int &b) {
    auto it1 = num_.begin();
    auto it2 = b.num_.begin();
    int dif = 0;
    while (it1 != num_.end() || it2 != b.num_.end()) {
      if (it1 != num_.end()) {
        dif += *it1;
        ++it1;
      }
      if (it2 != b.num_.end()) {
        dif -= *it2;
        ++it2;
      }
      if (dif < 0) {
        *(it1 - 1) = dif + base_;
        dif = -1;  ///< 向高位借位
      } else {
        *(it1 - 1) = dif % base_;
        dif /= base_;
      }

      if (dif < 0) positive_ = false;
      while (num_.size() > 1) {
        it1 = num_.end() - 1;
        if (*it1 == 0)
          num_.pop_back();
        else
          break;
      }
    }
    return *this;
  }

  big_int operator*(big_int const &b) {
    if (b.num_.size() == 1) return *this *= b.num_[0];
    std::vector<int>::iterator it1;
    std::vector<int>::const_iterator it2;
    big_int c;
    for (it1 = num_.begin(); it1 != num_.end(); ++it1) {
      for (it2 = b.num_.begin(); it2 != b.num_.end(); ++it2) {
        auto distance = std::distance(it1, num_.begin()) +
                        std::distance(it2, b.num_.begin());
        c.skip_ = static_cast<unsigned>(distance);
        c += static_cast<long long>(*it1) * (*it2);
      }
    }
    c.skip_ = 0;

    return c;
  }

  big_int &operator*=(const big_int &b) {
    *this = *this * b;

    return *this;
  }

  big_int operator*(const long long &b) {
    big_int c = *this;
    c *= b;

    return c;
  }

  big_int &operator*=(const long long &b) {
    std::vector<int>::iterator it = num_.begin();
    long long sum = 0;
    while (it != num_.end()) {
      sum += (long long)(*it) * b;
      *it = (int)(sum % base_);
      sum /= base_;
      ++it;
    }
    if (sum) num_.push_back((int)sum);

    return *this;
  }

  // Power
  big_int pow(int const &power, std::map<int, big_int> &lookup) {
    if (power == 1) return *this;
    if (lookup.count(power)) return lookup[power];

    int closestPower = 1;
    while (closestPower < power) closestPower <<= 1;
    closestPower >>= 1;

    if (power == closestPower)
      lookup[power] = pow(power / 2, lookup) * pow(power / 2, lookup);
    else
      lookup[power] =
          pow(closestPower, lookup) * pow(power - closestPower, lookup);

    return lookup[power];
  }

  big_int &pow(int const &power) {
    std::map<int, big_int> lookup;
    if (power % 2 == 0 && !positive_) {
      positive_ = true;
    }
    *this = pow(power, lookup);

    return *this;
  }

  // 0 this == a || -1 this < a || 1 this > a
  int compare(const big_int &a) const {
    /// 异号情况
    if (positive_ && !a.positive_) return 1;
    if (!positive_ && a.positive_) return -1;

    int check = 1;
    /// 同号情况转成正数判断，结果再通过符号转换。
    if (!positive_ && !a.positive_) check = -1;

    /// 位数多的数值大
    if (num_.size() < a.num_.size()) return -1 * check;
    if (num_.size() > a.num_.size()) return check;
    /// 位数相等的时候, 诸位比较大小
    for (size_t i = num_.size(); i > 0; --i) {
      if (num_[i - 1] < a.num_[i - 1]) return -1 * check;
      if (num_[i - 1] > a.num_[i - 1]) return check;
    }

    return 0;  // ==
  }

  bool operator<(big_int const &b) const { return compare(b) == -1; }

  bool operator<=(const big_int &b) const {
    int compared = compare(b);

    return compared == 0 || compared == -1;
  }

  bool operator>(const big_int &b) const { return compare(b) == 1; }

  bool operator>=(const big_int &b) const {
    int compared = compare(b);

    return compared == 0 || compared == 1;
  }

  bool operator==(const big_int &b) const { return compare(b) == 0; }

  bool operator!=(const big_int &b) const { return !(*this == b); }

  bool operator==(long long num) const {
    big_int temp(num);
    return temp == *this;
  }
  bool operator!=(long long num) const {
    big_int temp(num);
    return !(temp == *this);
  }

  bool operator==(const std::string &str) const {
    big_int temp(str);
    return temp == *this;
  }

  bool operator!=(const std::string &str) const {
    big_int temp(str);
    return !(temp == *this);
  }

  std::string to_string() const {
    std::ostringstream stream;
    stream << *this;

    return stream.str();
  }

  friend std::istream &operator>>(std::istream &in, big_int &a) {
    std::string str;
    in >> str;
    a = str;
    return in;
  }

  friend std::ostream &operator<<(std::ostream &out, const big_int &a) {
    if (a.num_.size() == 0) return out << 0;
    int length = static_cast<int>(a.num_.size()) - 1;
    for (; length >= 0 && a.num_[static_cast<size_t>(length)] == 0; --length) {
      ;  /// DONOTHING
    }

    if (length == -1) return out << 0;
    if (!a.positive_) out << '-';

    std::vector<int>::const_reverse_iterator it =
        a.num_.rbegin() + (a.num_.size() - length - 1);

    out << *it++;
    for (; it != a.num_.rend(); ++it) {
      for (int i = 0, len = a.segment_length(*it); i < 9 - len; ++i) {
        out << '0';
      }
      if (*it != 0) {
        out << *it;
      }
    }
    return out;
  }
  int segment_length(int segment) const {
    int length = 0;
    while (segment != 0) {
      segment /= 10;
      ++length;
    }

    return length;
  }

 private:
  static constexpr int BASE_NUM_ = static_cast<int>(1e9);
  std::vector<int> num_;
  bool positive_ = true;
  const int base_ = BASE_NUM_;
  unsigned skip_ = 0;
};

inline lee::big_int factorial(int n) {
  lee::big_int result = 1;
  if (n % 2) {
    result = n;
    --n;
  }
  int last = 0;
  for (; n >= 2; n -= 2) {
    result *= n + last;
    last += n;
  }

  return result;
}
}  // namespace bigint
}  // namespace lee

#endif  ///< end of INCLUDE_DETAIL_BIG_INT_HPP_