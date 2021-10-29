/**
 * @file            input_check_utility.hpp
 * @brief           输入字符串的检查工具函数实现
 * @author          lijiancong(pipinstall@163.com)
 * @version         0.1
 * @date            2021-09-01 16:27:04
 *
 */

#ifndef LEE_INPUT_CHECK_UTILITY_HPP__
#define LEE_INPUT_CHECK_UTILITY_HPP__

#include <string>
#include <vector>
#include <utility>
#include <cstdlib>
#include <cctype>

namespace lee
{
/// 判断字符是否是特殊符号
inline bool IsPunctChar(const char c)
{
    return ispunct(static_cast<int>(c)) != 0;
}

/// 判断字符是字母或数字（alphanumeric character）a-z A-Z 0-9 都返回 true
inline bool IsAlChar(const char c)
{
    return isalnum(static_cast<int>(c)) != 0;
}

/// 判断字符是数字
inline bool IsDigit(const char c)
{
    return isdigit(static_cast<int>(c)) != 0;
}

/// 判断字符是空格， '\r' '\n' '\t' 'space' 都返回 true
inline bool IsSpace(const char c)
{
    return isspace(static_cast<int>(c)) != 0;
}

/// 判断字符是 大写字母
inline bool IsUpper(const char c)
{
    return isupper(static_cast<int>(c)) != 0;
}

/// 判断字符是 小写字母
inline bool IsLower(const char c)
{
    return islower(static_cast<int>(c)) != 0;
}

/// 判断字符是数字或 '.'
inline bool IsDigitOrDot(const char c)
{
    return isdigit(static_cast<int>(c)) != 0 || c == '.';
}

/// 是否是十六进制的字符
inline bool IsHexChar(const char c)
{
    return IsDigit(c) || ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

/// 判断是否是特定字符
template <char C>
inline bool IsChar(const char c)
{
    return c == C;
}

/// 所有字符返回真
inline bool IsChar(const char c)
{
    return true;
}

/// 判断字符是物理地址
inline bool IsMacChar(const char c)
{
    return IsHexChar(c) || c == '-' || c == ':';
}

/// 检查字符是否复合规则函数指针定义
typedef bool (*check_char_func)(const char);

/// 所有字符，全是
inline bool all_of(const std::string& strInput, check_char_func p)
{
    return lee::all_of(strInput.begin(), strInput.end(), p);
}

/// 所有字符，全不是
inline bool none_of(const std::string& strInput, check_char_func p)
{
    return lee::none_of(strInput.begin(), strInput.end(), p);
}

/// 所有字符，部分是
inline bool any_of(const std::string& strInput, check_char_func p)
{
    return lee::any_of(strInput.begin(), strInput.end(), p);
}

/// 计数字符串内有多少符合规则的字符
inline bool count_equal(const std::string& str, check_char_func p, const unsigned& count)
{
    return count == (unsigned)std::count_if(str.begin(), str.end(), p);
}

/// 不等于
inline bool count_not_equal(const std::string& str, check_char_func p, const unsigned& count)
{
    return count != (unsigned)std::count_if(str.begin(), str.end(), p);
}

/// 大于
inline bool count_great(const std::string& str, check_char_func p, const unsigned& count)
{
    return (unsigned)std::count_if(str.begin(), str.end(), p) > count;
}

/// 大于等于
inline bool count_great_equal(const std::string& str, check_char_func p, const unsigned& count)
{
    return (unsigned)std::count_if(str.begin(), str.end(), p) >= count;
}

/// 小于
inline bool count_less(const std::string& str, check_char_func p, const unsigned& count)
{
    return (unsigned)std::count_if(str.begin(), str.end(), p) < count;
}

/// 小于等于
inline bool count_less_equal(const std::string& str, check_char_func p, const unsigned& count)
{
    return (unsigned)std::count_if(str.begin(), str.end(), p) <= count;
}

/// 检查字符串是否复合规则的函数指针
typedef bool (*input_check_function)(const std::string&, check_char_func);
/// 字符计数函数
typedef bool (*count_check_function)(const std::string&, check_char_func, const unsigned&);

class CountRules
{
public:
    CountRules(count_check_function count_f, check_char_func char_f, const unsigned& count)
        : m_count_f(count_f), m_check_char_f(char_f), m_count(count)
    {
    }

    CountRules& operator=(const CountRules& other)
    {
        if (&other != this)
        {
            m_count_f = other.m_count_f;
            m_check_char_f = other.m_check_char_f;
            m_count = other.m_count;
        }
        return *this;
    }

    count_check_function m_count_f;
    check_char_func m_check_char_f;
    unsigned m_count;
};

/**
 * @brief           输入字符串检查类，规则可叠加
 * @author          lijiancong(pipinstall@163.com)
 * @date            2021-09-02 09:31:00
 * @note
 */
class input_check
{
public:
    /**
     * @brief           添加过滤规则
     * @param           input    [in]   检查字符串的函数
     * @param           p        [in]   检查字符的函数
     * @return          input_check&
     * @author          lijiancong(pipinstall@163.com)
     * @date            2021-09-02 09:31:19
     * @note
     */
    input_check& AddRules(input_check_function input, check_char_func p)
    {
        std::pair<input_check_function, check_char_func> pair(input, p);
        vecRules.push_back(pair);
        return *this;
    }

    /**
     * @brief           添加计数规则
     * @param           input    [in]   计数函数
     * @param           p        [in]   字符检查规则函数
     * @param           count    [in]   计数数量
     * @return          input_check&
     * @author          lijiancong(pipinstall@163.com)
     * @date            2021-10-27 09:34:51
     * @note
     */
    input_check& AddRules(count_check_function input, check_char_func p, const unsigned& count)
    {
        CountRules temp(input, p, count);
        vecCountRules.push_back(temp);
        return *this;
    }

    /**
     * @brief           是否输入字符串是否符合所有规则
     * @param           strInput    [in]
     * @return          true    该字符串符合全部校验规则
     * @return          false   该字符串不符合全部校验规则
     * @author          lijiancong(pipinstall@163.com)
     * @date            2021-09-01 18:40:37
     * @note
     */
    bool IsValid(const std::string& strInput)
    {
        for (unsigned i = 0; i < vecRules.size(); ++i)
        {
            if (!vecRules.at(i).first(strInput, vecRules.at(i).second))
            {
                return false;
            }
        }

        for (unsigned i = 0; i < vecCountRules.size(); ++i)
        {
            if (!vecCountRules.at(i).m_count_f(strInput, vecCountRules.at(i).m_check_char_f,
                                               vecCountRules.at(i).m_count))
            {
                return false;
            }
        }

        return true;
    }

private:
    std::vector<std::pair<input_check_function, check_char_func> > vecRules;  ///< 过滤规则的集合
    std::vector<CountRules> vecCountRules;
};

/**
 * @brief           判断输入的用户名是否有效
 * @param           strInput    [in]
 * @return          true
 * @return          false
 * @author          lijiancong(pipinstall@163.com)
 * @date            2021-09-01 18:42:14
 * @note
 */
inline bool IsInputUserNameValid(const std::string& strInput)
{
    input_check filter;
    /// 这里的第二个规则不用是冗余的，写在这里是用来展示 input_check 的用法
    filter.AddRules(lee::all_of, lee::IsAlChar).AddRules(lee::none_of, lee::IsSpace);
    return filter.IsValid(strInput);
}

/**
 * @brief           判断输入的密码是否有效
 * @param           strInput    [in]
 * @return          true
 * @return          false
 * @author          lijiancong(pipinstall@163.com)
 * @date            2021-09-03 18:00:08
 * @note
 */
inline bool IsInputPasswordValid(const std::string& strInput)
{
    input_check filter;
    /// 这里的第二个规则不用是冗余的，写在这里是用来展示 input_check 的用法
    filter.AddRules(lee::none_of, lee::IsPunctChar).AddRules(lee::none_of, lee::IsSpace);

    return filter.IsValid(strInput);
}

/**
 * @brief           判断输入密码是强密码，即 有小写字母，大写字母和数字
 * @param           strInput    [in]
 * @return          true
 * @return          false
 * @author          lijiancong(pipinstall@163.com)
 * @date            2021-09-02 10:00:29
 * @note
 */
inline bool IsInputStrongPasswordValid(const std::string& strInput)
{
    input_check filter;
    filter.AddRules(lee::any_of, lee::IsUpper).AddRules(lee::any_of, lee::IsLower).AddRules(lee::any_of, lee::IsDigit);
    return filter.IsValid(strInput);
}

/**
 * @brief           判断输入字符串的 IP 是否有效
 * @param           strIP    [in]
 * @return          true    是有效的 IP 字符串
 * @return          false   不有效的 IP 字符串
 * @author          lijiancong(pipinstall@163.com)
 * @date            2021-09-02 09:42:26
 * @note
 */
inline bool IsInputIPV4Valid(const std::string& strIP)
{
    input_check obj;
    obj.AddRules(lee::all_of, lee::IsDigitOrDot)
        .AddRules(lee::count_equal, lee::IsChar<'.'>, 3)
        .AddRules(lee::count_great_equal, lee::IsDigitOrDot, 7)
        .AddRules(lee::count_less_equal, lee::IsDigitOrDot, 15);
    if (!obj.IsValid(strIP))
    {
        return false;
    }

    /// 判断每个数字应该为 0 ~ 255
    std::vector<std::string> vec;
    lee::string_split(strIP, vec, ".");
    for (unsigned i = 0; i < vec.size(); ++i)
    {
        int num = std::atoi(vec.at(i).c_str());
        if (num < 0 || num > 255)
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief           判断输入的物理地址是否正确
 * @param           strMac      [in]    输入 mac 地址
 * @param           split_char  [in]    物理地址的分隔符
 * @return          true    是有效的 mac 地址
 * @return          false   不是有效的 mac 地址
 * @author          lijiancong(pipinstall@163.com)
 * @date            2021-09-02 14:33:36
 * @note
 */
inline bool IsInputMacValid(const std::string& strMac, const char split_char = ':')
{
    /// 判断分隔符的有效性
    if (split_char != ':' && split_char != '-')
    {
        return false;
    }

    input_check filter;
    filter.AddRules(lee::all_of, lee::IsMacChar).AddRules(lee::count_equal, lee::IsChar, 17);
    if (split_char == ':')
    {
        filter.AddRules(lee::count_equal, lee::IsChar<':'>, 5);
    }
    else if (split_char == '-')
    {
        filter.AddRules(lee::count_equal, lee::IsChar<'-'>, 5);
    }

    return filter.IsValid(strMac);
}

/**
 * @brief           判断某个字符的数量正不正确
 * @param           str     [in]    需要检查的字符串
 * @param           count   [in]    需要几个
 * @return          true
 * @return          false
 * @author          lijiancong(pipinstall@163.com)
 * @date            2021-10-27 10:17:49
 * @note
 */
template <char C>
inline bool IsCharCountCorrect(const std::string& str, const unsigned& ulCount)
{
    input_check obj;
    obj.AddRules(lee::count_equal, lee::IsChar<C>, ulCount);
    return obj.IsValid(str);
}

}  // namespace lee

#endif  ///< LEE_INPUT_CHECK_UTILITY_HPP__