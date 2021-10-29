/**
 * @file            erase_if.hpp
 * @brief           std::erase_if��ʵ��
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
 * @brief           ����vector���ض�ֵ��Ԫ��
 * @param           vec      [out]  Ҫ������vector
 * @param           value    [in]   Ҫ������ֵ
 * @return          size_t  ����Ԫ�صĸ���
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
 * @brief           ��vec�в���erase_value�д��ڵ�Ԫ��
 * @param           vec            [out] ��Ҫ����������
 * @param           erase_value    [in]
 * @return          size_t  ����Ԫ�صĸ���
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
 * @brief           ��һ��string�в�����Ӧ���ַ�
 * @param           str    [out]    ��Ҫ�������ַ���
 * @param           c      [in]     ��Ҫ�������ַ�
 * @return          size_t  �����ַ��ĸ���
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
 * @brief           ��һ��string�в�����Ӧ���ַ�
 * @param           str    [out]    ��Ҫ�������ַ���
 * @param           vec    [in]     ��Ҫ�������ַ��ļ���
 * @return          size_t  �����ַ��ĸ���
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
