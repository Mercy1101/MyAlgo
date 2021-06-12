///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   bimap.hpp
/// @brief  双向查找的map的实�?
///
/// @author lijiancong, pipinstall@163.com
/// @date   2021-05-12 17:26:24
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_BIMAP_HPP_
#define INCLUDE_DETAIL_BIMAP_HPP_

#include <cassert>
#include <cstdlib>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <tuple>

namespace lee
{
namespace bimap
{
template <typename A, typename B>
class bijection
{
    using data_type = std::list<std::tuple<A const, B const> >;

public:
    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

private:
    using direct_mapping_type = std::map<std::reference_wrapper<A const>, iterator, std::less<A const&> >;
    using inverse_mapping_type = std::map<std::reference_wrapper<B const>, iterator, std::less<B const&> >;
    using direct_iterator = typename direct_mapping_type::iterator;
    using inverse_iterator = typename inverse_mapping_type::iterator;

public:
    auto size() const
    {
        return data_.size();
    }

    iterator find(A const& a)
    {
        auto const direct = direct_mapping_.find(a);
        if (direct == direct_mapping_.end())
        {
            return data_.end();
        }
        else
        {
            return direct->second;
        }
    }

    iterator find(B const& b)
    {
        auto const inverse = inverse_mapping_.find(b);
        if (inverse == inverse_mapping_.end())
        {
            return data_.end();
        }
        else
        {
            return inverse->second;
        }
    }

    auto erase(iterator pos)
    {
        auto const& element = *pos;
        direct_mapping_.erase(std::get<0>(element));
        inverse_mapping_.erase(std::get<1>(element));
        return data_.erase(pos);
    }

    template <typename X, typename Y>
    std::tuple<iterator, bool, bool> insert(X&& x, Y&& y)
    {
        direct_iterator direct = direct_mapping_.find(x);
        inverse_iterator inverse = inverse_mapping_.find(y);
        bool const d = (direct != direct_mapping_.end());
        bool const i = (inverse != inverse_mapping_.end());
        if (d && i)
        {
            iterator ix = inverse->second;
            iterator iy = direct->second;
            inverse_mapping_.erase(inverse);
            direct_mapping_.erase(direct);
            if (ix != iy)
            {
                inverse_mapping_.erase(std::get<1>(*iy));
                direct_mapping_.erase(std::get<0>(*ix));
                data_.erase(iy);
                data_.erase(ix);
            }
            else
            {
                data_.erase(ix);  // iy
            }
        }
        else if (d)
        {
            iterator iy = direct->second;
            direct_mapping_.erase(direct);
            inverse_mapping_.erase(std::get<1>(*iy));
            data_.erase(iy);
        }
        else if (i)
        {
            iterator ix = inverse->second;
            inverse_mapping_.erase(inverse);
            direct_mapping_.erase(std::get<0>(*ix));
            data_.erase(ix);
        }
        data_.emplace_back(std::forward<X>(x), std::forward<Y>(y));
        auto const& element = data_.back();
        iterator it = --data_.end();
        direct_mapping_.emplace(std::get<0>(element), it);
        inverse_mapping_.emplace(std::get<1>(element), it);
        return std::make_tuple(it, d, i);
    }

private:
    data_type data_;
    direct_mapping_type direct_mapping_;
    inverse_mapping_type inverse_mapping_;
};

int test_bimap()
{
    using A = std::size_t;
    using B = std::string;
    using M = bijection<A, B>;
    M m;
    assert(1 == (m.insert(A(111), B("111")), m.size()));
    assert(1 == (m.insert(A(111), B("111")), m.size()));
    assert(2 == (m.insert(A(222), B("222")), m.size()));
    assert(3 == (m.insert(A(333), B("333")), m.size()));
    assert(2 == (m.insert(A(222), B("111")), m.size()));
    assert(3 == (m.insert(A(111), B("222")), m.size()));
    assert(2 == (m.insert(A(111), B("111")), m.size()));
    assert(1 == (m.insert(A(333), B("111")), m.size()));
    assert(1 == (m.insert(A(333), B("333")), m.size()));
    assert(1 == (m.insert(A(111), B("333")), m.size()));
    assert(0 == (m.erase(m.find(A(111))), m.size()));
    return EXIT_SUCCESS;
}

}  // namespace bimap
}  // namespace lee

#endif  ///< INCLUDE_DETAIL_BIMAP_HPP_