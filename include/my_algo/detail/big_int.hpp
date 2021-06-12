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

#include <algorithm>
#include <cassert>
#include <exception>
#include <functional>
#include <limits>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace lee
{
inline namespace bigint
{
class big_int
{
public:
    big_int()
    {
        num_.push_back(0);
    }
    big_int(long long i)
    {
        if (i == 0)
        {
            num_.push_back(0);
        }
        else
        {
            positive_ = (i >= 0);
            if (!positive_ && i != std::numeric_limits<long long>::lowest())
            {
                i *= -1;
            }
            while (i != 0)
            {
                auto temp = static_cast<int>(i % base_);
                if (temp < 0)
                    temp *= -1;
                num_.push_back(temp);
                i /= base_;
                if (i < 0)
                    i *= -1;
            }
        }
    }

    big_int(const big_int &other) : num_(other.num_), positive_(other.positive_), base_(other.base_) {}

    big_int(const std::string &str)
    {
        if (str.empty() || str == "0" || str == "-0")
        {
            num_.push_back(0);
        }
        else
        {
            positive_ = str.at(0) != '-';

            int size = static_cast<int>(str.size());
            while (true)
            {
                if (size <= 0)
                {
                    break;
                }
                if (!positive_ && size < 1)
                {
                    throw std::logic_error("invalid param in big_int!");
                }
                int length = 0;
                int num = 0;
                int prefix = 1;
                for (int i = size - 1; i >= 0 && i >= size - 9; --i)
                {
                    if (str.at(i) < '0' || str.at(i) > '9')
                    {
                        if (i == 0 && str.at(i) == '-' && !positive_)
                        {
                            size = 0;
                            break;  ///< 跳过第一个负号
                        }
                        else
                        {
                            throw std::logic_error("invalid param in big_int!");
                        }
                    }
                    num += (str.at(i) - '0') * prefix;
                    prefix *= 10;
                    ++length;
                }
                num_.push_back(num);
                size -= length;
            }
        }
        while (true && num_.size() > 1)
        {
            /// 去除头部多余的零
            if (num_.back() == 0)
            {
                num_.pop_back();
            }
            else
            {
                break;
            }
        }
    }

    big_int operator+(long long i) const
    {
        big_int c = *this;
        c += i;
        return c;
    }

    big_int &operator+=(long long i)
    {
        if ((i >= 0) ^ positive_)
        {
            /// 符号不同则计算减法
            i *= -1;
            *this -= i;
            return *this;
        }
        else
        {
            auto it = num_.begin();
            if (skip_ > num_.size() - 1)
            {
                num_.insert(num_.end(), skip_ - num_.size(), 0);
            }
            it += skip_;
            do
            {
                if (it != num_.end())
                {
                    *it += i % base_;
                    i /= base_;
                    i += *it / base_;  ///< i 加上进位
                    *it %= base_;      ///< 当前值消除进位
                    ++it;
                }
                else
                {
                    num_.push_back(0);           ///< 添加一位，下一个循环再赋值
                    it = std::prev(num_.end());  ///< 更新迭代器为有效状态
                }
            } while (i != 0);
            return *this;
        }
    }

    big_int operator=(const long long &a)
    {
        num_.clear();
        long long t = a;
        do
        {
            num_.push_back(static_cast<int>(t % base_));
            t /= base_;
        } while (t != 0);

        return *this;
    }
    big_int &operator=(const big_int &other)
    {
        if (&other != this)
        {
            base_ = other.base_;
            num_ = other.num_;
            positive_ = other.positive_;
        }
        return *this;
    }

    big_int operator+(const big_int &b) const
    {
        big_int c = *this;
        c += b;

        return c;
    }

    big_int &operator+=(const big_int &temp)
    {
        auto b = *this;
        if (&temp != this)
        {
            b = temp;
        }

        if (positive_ ^ b.positive_)
        {
            /// 异号相减
            b *= -1;
            return *this -= b;
        }

        auto it1 = num_.begin();
        auto it2 = b.num_.begin();
        int sum = 0;
        while (it1 != num_.end() || it2 != b.num_.end())
        {
            if (it1 != num_.end())
            {
                sum += *it1;
            }
            else
            {
                num_.push_back(0);
                it1 = std::prev(num_.end());
            }
            if (it2 != b.num_.end())
            {
                sum += *it2;
                ++it2;
            }
            *it1 = sum % base_;
            ++it1;
            sum /= base_;
        }
        if (sum != 0)
        {
            assert(sum != 1);
            num_.push_back(1);
        }
        return *this;
    }

    big_int operator-(long long l) const
    {
        big_int b(l);
        big_int c = *this;
        c -= b;
        return c;
    }
    big_int &operator-=(long long l)
    {
        big_int b(l);
        *this -= b;
        return *this;
    }

    big_int operator-(const big_int &b) const
    {
        big_int c = *this;
        c -= b;
        return c;
    }

    big_int &operator-=(const big_int &temp)
    {
        auto b = *this;
        if (&temp != this)
        {
            b = temp;
        }
        if (temp.positive_ ^ positive_)
        {
            b *= -1;
            /// 异号
            *this += b;
        }
        else
        {
            auto it1 = num_.begin();
            auto it2 = b.num_.begin();
            int dif = 0;
            while (it1 != num_.end() || it2 != b.num_.end())
            {
                if (it1 != num_.end())
                {
                    dif += *it1;
                    ++it1;
                }
                if (it2 != b.num_.end())
                {
                    dif -= *it2;
                    ++it2;
                }
                if (dif < 0)
                {
                    *std::prev(it1) = dif + base_;
                    dif = -1;  ///< 向高位借位
                }
                else
                {
                    *std::prev(it1) = dif % base_;
                    dif /= base_;
                }
            }
            if (dif < 0)
                positive_ = false;
            while (num_.size() > 1)
            {
                it1 = num_.end() - 1;
                if (*it1 == 0)
                    num_.pop_back();
                else
                    break;
            }
        }
        return *this;
    }

    big_int operator*(const big_int &temp)
    {
        big_int c = *this;
        big_int b = temp;
        if (b == big_int(-1))
        {
            c.positive_ = !c.positive_;
        }
        else
        {
            c.positive_ = !(c.positive_ ^ b.positive_);
            if (b.num_.size() == 1)
                return c *= b.num_[0];
            for (auto it1 = c.num_.begin(); it1 != c.num_.end(); ++it1)
            {
                for (auto it2 = b.num_.begin(); it2 != b.num_.end(); ++it2)
                {
                    auto distance = std::distance(c.num_.begin(), it1) + std::distance(b.num_.begin(), it2);
                    c.skip_ = static_cast<unsigned>(distance);
                    c += (static_cast<long long>(*it1) * (*it2));
                }
            }
            c.skip_ = 0;
        }
        return c;
    }

    big_int &operator*=(const big_int &b)
    {
        *this = *this * b;
        return *this;
    }

    big_int operator*(const long long &b)
    {
        big_int c = *this;
        c *= b;
        return c;
    }

    big_int &operator*=(const long long &b)
    {
        if (b == -1)
        {
            positive_ = !positive_;
        }
        else if (b == 1)
        {
            /// DONOTHING
        }
        else if (b == 0)
        {
            positive_ = true;
            num_.clear();
            num_.push_back(0);
        }
        else
        {
            positive_ = !(positive_ ^ (b >= 0));
            std::vector<int>::iterator it = num_.begin();
            long long sum = 0;
            while (it != num_.end())
            {
                sum += (long long)(*it) * b;
                *it = (int)(sum % base_);
                sum /= base_;
                ++it;
            }
            if (sum)
                num_.push_back((int)sum);
        }
        return *this;
    }

    big_int &pow(const int &power)
    {
        if (power < 0)
        {
            throw std::logic_error("invalid param in big_int::pow()");
        }
        std::map<int, big_int> lookup;
        lookup.insert({0, big_int(1)});
        lookup.insert({1, *this});
        if (power % 2 == 0 && !positive_)
        {
            positive_ = true;
        }
        std::function<big_int(const int)> func = [&, this](const int n)
        {
            if (n == 1)
                return lookup[n];
            if (lookup.count(n))
                return lookup[n];

            int closestPower = 1;
            while (closestPower < n) closestPower <<= 1;
            closestPower >>= 1;

            if (n == closestPower)
                lookup[n] = func(n / 2) * func(n / 2);
            else
                lookup[n] = func(closestPower) * func(n - closestPower);

            return lookup[n];
        };
        *this = func(power);

        return *this;
    }

    // 0 this == a || -1 this < a || 1 this > a
    int compare(const big_int &a) const
    {
        /// 比较对象是自己
        if (&a == this)
        {
            return 0;
        }
        /// 异号情况
        if (positive_ && !a.positive_)
            return 1;
        if (!positive_ && a.positive_)
            return -1;

        int check = 1;
        /// 同号情况转成正数判断，结果再通过符号转换。
        if (!positive_ && !a.positive_)
            check = -1;

        /// 位数多的数值大
        if (num_.size() < a.num_.size())
            return -1 * check;
        if (num_.size() > a.num_.size())
            return check;
        /// 位数相等的时候, 诸位比较大小
        for (size_t i = num_.size(); i > 0; --i)
        {
            if (num_[i - 1] < a.num_[i - 1])
                return -1 * check;
            if (num_[i - 1] > a.num_[i - 1])
                return check;
        }

        return 0;  // ==
    }

    bool operator<(big_int const &b) const
    {
        return compare(b) == -1;
    }

    bool operator<=(const big_int &b) const
    {
        int compared = compare(b);

        return compared == 0 || compared == -1;
    }

    bool operator>(const big_int &b) const
    {
        return compare(b) == 1;
    }

    bool operator>=(const big_int &b) const
    {
        int compared = compare(b);

        return compared == 0 || compared == 1;
    }

    bool operator==(const big_int &b) const
    {
        return compare(b) == 0;
    }

    bool operator!=(const big_int &b) const
    {
        return !(*this == b);
    }

    bool operator==(long long num) const
    {
        big_int temp(num);
        return temp == *this;
    }
    bool operator!=(long long num) const
    {
        big_int temp(num);
        return !(temp == *this);
    }

    bool operator==(const std::string &str) const
    {
        big_int temp(str);
        return temp == *this;
    }

    bool operator!=(const std::string &str) const
    {
        big_int temp(str);
        return !(temp == *this);
    }

    std::string to_string() const
    {
        std::ostringstream stream;
        stream << *this;

        return stream.str();
    }

    friend std::istream &operator>>(std::istream &in, big_int &a)
    {
        std::string str;
        in >> str;
        a = str;
        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, const big_int &a)
    {
        if (a.num_.size() == 0)
            return out << 0;
        int length = static_cast<int>(a.num_.size()) - 1;
        for (; length >= 0 && a.num_[static_cast<size_t>(length)] == 0; --length)
        {
            ;  /// DONOTHING
        }

        if (length == -1)
            return out << 0;
        if (!a.positive_)
            out << '-';

        std::vector<int>::const_reverse_iterator it = a.num_.rbegin() + (a.num_.size() - length - 1);

        out << *it++;
        for (; it != a.num_.rend(); ++it)
        {
            for (int i = 0, len = a.segment_length(*it); i < 9 - len; ++i)
            {
                out << '0';
            }
            if (*it != 0)
            {
                out << *it;
            }
        }
        return out;
    }
    bool get_positive()
    {
        return positive_;
    }

private:
    static constexpr int BASE_NUM_ = static_cast<int>(1e9);
    std::vector<int> num_;
    bool positive_ = true;
    int base_ = BASE_NUM_;
    unsigned skip_ = 0;

    int segment_length(int segment) const
    {
        int length = 0;
        while (segment != 0)
        {
            segment /= 10;
            ++length;
        }
        return length;
    }

#if false
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
#endif
};  // namespace bigint

inline lee::big_int factorial(long long n)
{
    lee::big_int result = 1;
    if (n % 2)
    {
        result = n;
        --n;
    }
    long long last = 0;
    for (; n >= 2; n -= 2)
    {
        result *= n + last;
        last += n;
    }

    return result;
}
}  // namespace bigint
}  // namespace lee

#endif  ///< end of INCLUDE_DETAIL_BIG_INT_HPP_
