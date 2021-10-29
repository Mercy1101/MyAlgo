/**
 * @file            erase_if.hpp
 * @brief           std::erase_if的实现
 * @author          lijiancong(pipinstall@163.com)
 * @version         0.1
 * @date            2021-07-14 15:13:37
 *
 */

#ifndef MY_ALGO_ERASE_IF_HPP__
#define MY_ALGO_ERASE_IF_HPP__

#include <vector>
#include <algorithm>

namespace lee
{
/**
 * @brief           擦除vector中特定值的元素
 * @param           vec      [out]  要操作的vector
 * @param           value    [in]   要擦除的值
 * @return          size_t  擦除元素的个数
 * @author          lijiancong(pipinstall@163.com)
 * @note
 */
template <class T>
inline size_t erase(std::vector<T>& vec, const T& value)
{
    typename std::vector<T>::iterator it = std::remove(vec.begin(), vec.end(), value);
    size_t count = static_cast<size_t>(std::distance(it, vec.end()));
    vec.erase(it, vec.end());
    return count;
}

/**
 * @brief           从vec中擦除erase_value中存在的元素
 * @param           vec            [out] 需要擦除的容器
 * @param           erase_value    [in]
 * @return          size_t  擦除元素的个数
 * @author          lijiancong(pipinstall@163.com)
 * @note
 */
template <class T>
inline size_t erase(std::vector<T>& vec, const std::vector<T>& erase_value)
{
    size_t count = 0;
    for (size_t i = 0; i < erase_value.size(); ++i)
    {
        typename std::vector<T>::iterator it = std::remove(vec.begin(), vec.end(), erase_value.at(i));
        count += static_cast<size_t>(std::distance(it, vec.end()));
        vec.erase(it, vec.end());
    }
    return count;
}

/**
 * @brief           从一个string中擦除对应的字符
 * @param           str    [out]    需要操作的字符串
 * @param           c      [in]     需要擦除的字符
 * @return          size_t  擦除字符的个数
 * @author          lijiancong(pipinstall@163.com)
 * @note
 */
inline size_t erase(std::string& str, const char& c)
{
    std::string::iterator it = std::remove(str.begin(), str.end(), c);
    size_t count = static_cast<size_t>(std::distance(it, str.end()));
    str.erase(it, str.end());
    return count;
}

/**
 * @brief           从一个string中擦除对应的字符
 * @param           str    [out]    需要操作的字符串
 * @param           vec    [in]     需要擦除的字符的集合
 * @return          size_t  擦除字符的个数
 * @author          lijiancong(pipinstall@163.com)
 * @note
 */
inline size_t erase(std::string& str, const std::vector<char>& vec)
{
    size_t count = 0;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::string::iterator it = std::remove(str.begin(), str.end(), vec.at(i));
        count += static_cast<size_t>(std::distance(it, str.end()));
        str.erase(it, str.end());
    }
    return count;
}

template <class... Args>
inline size_t erase(std::string& str, const char& head, Args... c)
{
    return erase(str, head) + erase(str, c...);
}
}  // namespace lee

#endif  ///< MY_ALGO_ERASE_IF_HPP__
