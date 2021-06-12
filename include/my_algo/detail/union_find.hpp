///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   union_find.h
/// @brief  并查集示例
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-02 12:16:00
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_UNION_FIND_HPP_
#define INCLUDE_DETAIL_UNION_FIND_HPP_

#include <vector>

namespace lee
{
inline namespace union_find
{
/// 并查集模板
struct union_find_example
{
    std::vector<int> ancestor;

    union_find_example(int n)
    {
        ancestor.resize(n);
        for (int i = 0; i < n; ++i)
        {
            ancestor[i] = i;
        }
    }

    int find(int index)
    {
        return index == ancestor[index] ? index : ancestor[index] = find(ancestor[index]);
    }

    void merge(int u, int v)
    {
        ancestor[find(u)] = find(v);
    }
};

}  // namespace union_find
}  // namespace lee
#endif  ///< end of INCLUDE_DETAIL_UNION_FIND_HPP_