///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   threadsafe_stack.hpp
/// @brief  threadsafe_stack 的实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-11-02 10:15:14
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_THREADSAFE_STACK_H_
#define INCLUDE_DETAIL_THREADSAFE_STACK_H_

#include <exception>
#include <memory>
#include <mutex>
#include <stack>

namespace lee
{
inline namespace threadsafe
{
struct empty_stack : std::exception
{
    virtual const char* what() const throw()
    {
        return "empty stack!";
    };
};

template <typename T>
class threadsafe_stack
{
public:
    threadsafe_stack() = default;
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.data);
        data = other.data;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }

    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
        {
            throw empty_stack();
        }
        value = data.top();
        data.pop();
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
        {
            return std::shared_ptr<T>();
        }
        auto p = std::make_shared<T>(data.top());
        data.pop();
        return p;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }

private:
    std::stack<T> data;
    mutable std::mutex m;
};

}  // namespace threadsafe
}  // namespace lee

#endif  /// INCLUDE_DETAIL_THREADSAFE_STACK_H_
