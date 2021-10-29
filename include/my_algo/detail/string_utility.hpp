/**
 * @file            string_utility.hpp
 * @brief           字符串工具
 * @author          lijiancong(pipinstall@163.com)
 * @version         0.1
 * @date            2021-06-17 21:49:35
 */

#ifndef LEE_STRING_UTILITY_HPP__
#define LEE_STRING_UTILITY_HPP__

#include <stdlib.h>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>

namespace lee
{
/**
 * @brief           把字符串前后的字符串给去除
 * @param           s       [in]    要剪切的字符串
 * @param           chars   [in]    要去除什么的字符串
 * @return          std::string& 剪切后的字符串
 * @author          lijiancong(pipinstall@163.com)
 * @note
 */
inline std::string& string_strip(std::string& s, const std::string& chars = " ")
{
    s.erase(0, s.find_first_not_of(chars.c_str()));
    s.erase(s.find_last_not_of(chars.c_str()) + 1);
    return s;
}

/**
 * @brief           以特定符号为分隔符，切分字符串并放入vector里
 * @param           s           [in]    原字符串
 * @param           tokens      [out]   剪切后的子字符串
 * @param           delimiters  [in]    分隔符
 * @author          lijiancong(pipinstall@163.com)
 * @note
 *
 * @code
 * std::string str("temp_creator_1");
 * std::vector<std::string> tokens;
 * lee::string_split(str, tokens, "_");
 * for(unsigned i = 0; i < tokens.size(); ++i)
 * {
 *     std::cout << tokens.at(i) << std::endl;
 * }
 * @endcode
 *
 * 输出:
 * temp
 * creator
 * 1
 */
inline unsigned string_split(const std::string& s, std::vector<std::string>& tokens,
                             const std::string& delimiters = " ")
{
    std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    std::string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
    return (unsigned)tokens.size();
}

/**
 * @brief           擦除字符串中的子串
 * @param           sub_str    [in] 需要擦除的子串
 * @param           s          [in] 需要操作的字符串
 * @author          lijiancong(pipinstall@163.com)
 * @note
 * @code
 * std::string str("Hello World! ll Hi!");
 * lee::string_erase("ll", str);
 * std::cout << str;
 * @endcode
 * 输出 ：
 * Heo World!  Hi!
 */
inline void string_erase(const std::string& sub_str, std::string& s)
{
    if (sub_str.empty() || s.empty())
    {
        return;
    }

    int begin_pos = lee::kmp_search(s.c_str(), sub_str.c_str());
    while (begin_pos != -1 && begin_pos <= (int)s.size())
    {
        s.erase(begin_pos, sub_str.size());
        begin_pos = lee::kmp_search(s.c_str(), sub_str.c_str(), begin_pos);
    }
}

/**
 * @brief           替换指定字符串为新的字符串
 * @param           find_str        [in]    被替换的字符串
 * @param           replace_str     [in]    替换字符串
 * @param           s               [out]   需要操作的原字符串
 * @author          lijiancong(pipinstall@163.com)
 * @note
 * @code
 * std::string str("Hello World!");
 * lee::string_replace("Hello", "Hi", str);
 * std::cout << str << std::endl;
 * @endcode
 * 输出:
 * Hi World!
 */
inline void string_replace(const std::string& find_str, const std::string& replace_str, std::string& s)
{
    if (find_str == replace_str || s.empty() || find_str.empty())
    {
        return;
    }

    /// std::string::size_type begin_pos = s.find(find_str);
    size_t begin_pos = lee::kmp_search(s.c_str(), find_str.c_str());
    while (begin_pos != std::string::npos && begin_pos <= s.size())
    {
        /// if(begin_pos > s.size())
        /// {
        ///     LogDebug("s:\n%s\n begin_pos: %d\n s.size(): %u", s.c_str(), begin_pos, s.size());
        /// }
        s.erase(begin_pos, find_str.size());
        s.insert(begin_pos, replace_str);
        begin_pos += replace_str.size();
        begin_pos = lee::kmp_search(s.c_str(), find_str.c_str(), begin_pos);
    }
}

inline size_t string_find(const std::string& src_str, const std::string& find_str)
{
    if (src_str.size() <= 1024)
    {
        const char* pos = strstr(src_str.c_str(), find_str.c_str());
        if (pos)
        {
            return static_cast<size_t>(pos - src_str.c_str());
        }
        else
        {
            return std::string::npos;
        }
    }
    else
    {
        return lee::kmp_search(src_str.c_str(), find_str.c_str());
    }
}

inline size_t string_rfind(const std::string& src_str, const std::string& find_str)
{
    std::string src_reverse(src_str);
    std::string find_reverse(find_str);
    std::reverse(src_reverse.begin(), src_reverse.end());
    std::reverse(find_reverse.begin(), find_reverse.end());
    size_t begin_pos = kmp_search(src_reverse.c_str(), find_reverse.c_str());
    if (begin_pos != std::string::npos)
    {
        return (src_str.size() - 1) - (begin_pos - find_str.size());
    }
    return std::string::npos;
}

/**
 * @brief 把无符号整数转换为十六进制的字符串
 *
 * @param dec
 * @return std::string
 */
inline std::string to_hex(const size_t dec)
{
    char acText[32];

    sprintf(acText, "0x%X", dec);

    return acText;
}

/**
 * @brief 把传入的指针生成十六进制的形式返回字符串
 *
 * @tparam T
 * @param pointer
 * @return std::string
 */
template <typename T>
inline std::string pointer_to_hex(const T* pointer)
{
    return lee::to_hex(reinterpret_cast<size_t>(pointer));
}

/**
 * @brief           把不同类型的数据转换为 std::string
 * @param           str
 * @return          std::string
 * @author          lijiancong(pipinstall@163.com)
 * @note
 */
inline std::string to_string(const std::string& str)
{
    return str;
}

inline std::string to_string(int i)
{
    char s[20] = {0};
    sprintf(s, "%d", i);
    return std::string(s);
}

inline std::string to_string(unsigned i)
{
    char s[20] = {0};
    sprintf(s, "%u", i);
    return std::string(s);
}

inline std::string to_string(unsigned long i)
{
    char s[20] = {0};
    sprintf(s, "%lu", i);
    return std::string(s);
}

inline std::string to_string(unsigned long long x)
{
    char num[64] = {0};
    sprintf(num, "%llu", x);
    return std::string(num);
}

inline std::string to_string(long i)
{
    char s[20] = {0};
    sprintf(s, "%ld", i);
    return std::string(s);
}

inline std::string to_string(long long x)
{
    char num[64] = {0};
    sprintf(num, "%lld", x);
    return std::string(num);
}

template <typename T>
inline std::string to_string(T* x)
{
    return lee::pointer_to_hex(x);
}

inline std::string to_string(bool x)
{
    return x ? "true" : "false";
}

inline std::string to_string(char c)
{
    char a[2] = {0};
    sprintf(a, "%c", c);
    return std::string(a);
}

inline bool to_bool(const std::string& str)
{
    return str == "true";
}

}  // namespace lee

#endif  ///< LEE_STRING_UTILITY_HPP__
