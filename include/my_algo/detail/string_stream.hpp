/**
 * @file            string_stream.hpp
 * @brief           ʵ��һ������ std::string ��������
 * @author          lijiancong(pipinstall@163.com)
 * @version         0.1
 * @date            2021-08-05 16:33:11
 */

#ifndef LEE_STRING_STREAM_HPP__
#define LEE_STRING_STREAM_HPP__

#include <sstream>
#include <vector>
#include <map>

namespace lee
{
class string_stream
{
public:
    string_stream() {}
    string_stream(const string_stream& other)
    {
        m_stream.clear();
        m_stream.str("");
        m_stream << other.m_stream.str();
    }

    string_stream& operator=(const lee::string_stream& other)
    {
        if (this != &other)
        {
            m_stream.clear();
            m_stream.str("");
            m_stream << other.m_stream.str();
        }
        return *this;
    }

    // ת��Ϊ std::string �ĺ���
    std::string str() const
    {
        return m_stream.str();
    }

    /// ��������
    void clear()
    {
        m_stream.clear();
        m_stream.str("");
    }

    /// Ϊ bool ��������
    string_stream& operator<<(const bool& data)
    {
        m_stream << (data ? "true" : "false");
        return *this;
    }

    /// Ϊԭ����������
    template <typename T, unsigned int N>
    string_stream& operator<<(const T (&arr)[N])
    {
        m_stream << "array: ";
        for (unsigned i = 0; i < N; ++i)
        {
            *this << arr[i];
            if (i != N - 1)
            {
                m_stream << ", ";
            }
        }
        return *this;
    }

    /// Ϊ char ��������
    template <unsigned int N>
    string_stream& operator<<(const char (&arr)[N])
    {
        m_stream << arr;
        return *this;
    }

    /// Ϊ vector ����
    template <typename T>
    string_stream& operator<<(const std::vector<T>& vec)
    {
        m_stream << "vector: \r\n";
        for (unsigned i = 0; i < vec.size(); i++)
        {
            *this << vec.at(i);
            if (i != vec.size() - 1)
            {
                m_stream << ", ";
            }
        }
        return *this;
    }

    /// Ϊ map ����
    template <typename Key, typename Value>
    string_stream& operator<<(const std::map<Key, Value>& map)
    {
        m_stream << "map: \r\n";
        typename std::map<Key, Value>::const_iterator it = map.begin();
        for (; it != map.end(); ++it)
        {
            *this << "Key: " << it->first << ", Value: " << it->second << "\r\n";
        }
        return *this;
    }

    /// Ϊָ����������
    /// template <typename T>
    /// string_stream& operator<<(const T* p)
    /// {
    ///     m_stream << lee::to_string(p);
    ///     return *this;
    /// }

    /// �������͵�����
    template <typename T>
    string_stream& operator<<(const T& data)
    {
        m_stream << data;
        return *this;
    }

private:
    std::stringstream m_stream;
};
}  // namespace lee

#endif  ///<  LEE_STRING_STREAM_HPP__