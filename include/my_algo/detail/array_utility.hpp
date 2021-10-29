/**
 * @file            array_utility.hpp
 * @brief           实现自己的std::array
 * @author          lijiancong(pipinstall@163.com)
 * @version         0.1
 * @date            2021-05-28 10:43:48
 *
 * @copyright       Copyright (c) 2021 GBCOM
 *
 */

#ifndef LEE_UTILITY_ARRAY_UTILITY_HPP__
#define LEE_UTILITY_ARRAY_UTILITY_HPP__

#include <algorithm>
#include <stddef.h>
#include <cassert>
#include <exception>
#include <stdexcept>

namespace lee
{
inline namespace array
{
template <typename T, std::size_t N>
class array
{
public:
    T elems[N];
    // type definitions
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    array() {}

    // iterator support
    iterator begin()
    {
        return elems;
    }
    const_iterator begin() const
    {
        return elems;
    }
    const_iterator cbegin() const
    {
        return elems;
    }

    iterator end()
    {
        return elems + N;
    }
    const_iterator end() const
    {
        return elems + N;
    }
    const_iterator cend() const
    {
        return elems + N;
    }

    // operator[]
    reference operator[](size_type i)
    {
        assert(i < N);
        return elems[i];
    }

    const_reference operator[](size_type i) const
    {
        assert(i < N);
        return elems[i];
    }

    // at() with range check
    reference at(size_type i)
    {
        rangecheck(i);
        return elems[i];
    }
    const_reference at(size_type i) const
    {
        rangecheck(i);
        return elems[i];
    }

    // front() and back()
    reference front()
    {
        return elems[0];
    }

    const_reference front() const
    {
        return elems[0];
    }

    reference back()
    {
        return elems[N - 1];
    }

    const_reference back() const
    {
        return elems[N - 1];
    }

    // size is constant
    static size_type size()
    {
        return N;
    }
    static bool empty()
    {
        return false;
    }
    static size_type max_size()
    {
        return N;
    }
    enum
    {
        static_size = N
    };

    // swap (note: linear complexity)
    void swap(std::array<T, N>& y)
    {
        for (size_type i = 0; i < N; ++i) std::swap(elems[i], y.elems[i]);
    }

    // direct access to data (read-only)
    const T* data() const
    {
        return elems;
    }
    T* data()
    {
        return elems;
    }

    // use array as C array (direct read/write access to data)
    T* c_array()
    {
        return elems;
    }

    // assignment with type conversion
    template <typename T2>
    std::array<T, N>& operator=(const std::array<T2, N>& rhs)
    {
        std::copy(rhs.begin(), rhs.end(), begin());
        return *this;
    }

    std::array<T, N>& operator=(const std::array<T, N>& rhs)
    {
        if (&rhs != this)
        {
            std::copy(rhs.begin(), rhs.end(), begin());
        }
        return *this;
    }

    bool operator==(const std::array<T, N>& rhs) const
    {
        return std::equal(begin(), end(), rhs.begin());
    }

    array(const std::array<T, N>& rhs)
    {
        std::copy(rhs.begin(), rhs.end(), begin());
    }

    // assign one value to all elements
    void assign(const T& value)
    {
        fill(value);
    }  // A synonym for fill
    void fill(const T& value)
    {
        std::fill_n(begin(), size(), value);
    }

    // check range (may be private because it is static)
    static bool rangecheck(size_type i)
    {
        if (i >= size())
        {
            throw std::out_of_range("std::array<>: index out of range");
            return false;
        }
        else
        {
            return true;
        }
    }
};
}  // namespace array
}  // namespace lee

#endif  ///< LEE_UTILITY_ARRAY_UTILITY_HPP__
