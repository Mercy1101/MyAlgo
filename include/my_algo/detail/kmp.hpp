///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   kmp.h
/// @brief  kmp字符串匹配算法
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-05 14:59:35
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_KMP_HPP_
#define INCLUDE_DETAIL_KMP_HPP_

#include <string>
#include <vector>

namespace lee
{
inline namespace kmp_algo
{
class kmp
{
private:
    std::vector<std::vector<int>> dp_;
    std::string pattern_;

public:
    kmp(std::string pat)
    {
        this->pattern_ = pat;
        // dp[状态][字符] = 下个状态
        dp_ = std::vector<std::vector<int>>(pattern_.size(), std::vector<int>(256, 0));
        // base case
        dp_[0][pattern_.at(0)] = 1;
        // 影子状态 X 初始为 0
        int X = 0;
        // 构建状态转移图（稍改的更紧凑了）
        for (size_t j = 1; j < pattern_.size(); j++)
        {
            for (int c = 0; c < 256; c++) dp_[j][c] = dp_[X][c];
            dp_[j][pattern_.at(j)] = j + 1;
            // 更新影子状态
            X = dp_[X][pattern_.at(j)];
        }
    }

    int search(std::string txt)
    {
        int M = static_cast<int>(pattern_.size());
        int N = static_cast<int>(txt.size());
        // pat 的初始态为 0
        int j = 0;
        for (int i = 0; i < N; i++)
        {
            // 计算 pat 的下一个状态
            j = dp_[j][txt.at(i)];
            // 到达终止态，返回结果
            if (j == M)
                return i - M + 1;
        }
        // 没到达终止态，匹配失败
        return -1;
    }
};
}  // namespace kmp_algo
}  // namespace lee

#endif  ///< end of INCLUDE_DETAIL_KMP_HPP_