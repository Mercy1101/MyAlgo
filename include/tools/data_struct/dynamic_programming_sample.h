///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   dynamic_programming_sample.h
/// @brief  动态规划算法的笔记
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-10 09:02:14
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_DYNAMIC_PROGRAMMING_SAMPLE_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_DYNAMIC_PROGRAMMING_SAMPLE_H_

#include <vector>

namespace Lee {
inline namespace data_struct {
inline namespace dynamic_programming_sample {
/// 递归过多会导致爆栈
int fib_low(int N) {
  if (N == 1 || N == 2) return 1;
  return fib(N - 1) + fib(N - 2);
}

/// 线性数组
int fib_mid(int n){
  std::vector<int> dp(N+1, 0);
  /// base case
  dp[1] = dp[2] = 1;
  for(int i = 3; i <= N; i++){
    dp[i] = dp[i - 1] + dp[i - 2];
  }
  return dp[N];
}

int fib_mid_2(int n){
  if(n == 2 || n == 1){
    return 1;
  }
  int prev = 1, curr = 1;
  for(int i = 3; i <= n; ++i){
    int sum = prev + curr;
    prev = curr;
    curr = sum;
  }
}

/// 把重复的计算用一个简单的数据结构保存下来
int fib_high(int n) {
  if (N < 1) return 0;
  /// 备忘录全初始化为0
  std::vector<int> memo(N + 1, 0);
  /// 初始化最简情况
  return helper(memo, N);
}
int helper(std::vector<int>& memo, int n) {
  // base case
  if (n == 1 || n == 2) return 1;
  /// 已经计算过
  if (memo[n] != 0) return memo[n];
  memo[n] = helper(memo, n - 1) + helper(memo, n - 2);
  return memo[n];
}


/// 找零钱问题
/// 伪代码
/// def coinChange(coins: List[int], amount: int):
///   #定义： 要凑出金额n，至少要dp(n)个硬币
///   def dp(n):
///     # 做选择， 选择需要硬币最少的那个结果
///     for coin in coins:
///       res = min(res, 1 + dp(n - coin))
///     return res
///     # 我们要求的问题是 dp(amount)
/// return dp(amount)

/// def coinChange(coins: List[int], amount: int):
///   def dp(n):
///     #base case
///     if n == 0: return 0
///     if n < 0: return -1
///     # 求最小值，所以初始化为正无穷
///     res = float('INF')
///     for coin in coins:
///     subproblem = dp(n - coin)
///     # 子问题无解， 跳过
///     if subproblem == -1: continue
///     res = min(res, 1 + subproblem)
///   return dp(amount)

/// 带备忘录的递归
/// def coinChange(coins: List[int], amount: int):
///   # 备忘录
///   memo = dict()
///   def dp(n):
///     # 查备忘录， 避免重复计算
///     if n in memo: return memo[n]
/// 
///     if n == 0: return 0
///     if n < 0: return 01
///     res = float('INF')
///     for coin in coins:
///       subproblem = dp(n - coin)
///       if subproblem == -1: continue
///       res = min(res, 1 + subproblem)
///     
///     # 计入备忘录
///     memo[n] = res if res != float('INF') else -1
///     return memo[n]
/// 
///   return dp(amount)

/// dp数组的迭代解法
/// dp[i] = x 表示， 当目标金额为i时，至少需要x枚硬币
int coinChange(std::vector<int>& coins, int amount){
  /// 数组大小为 amount + 1, 初始值也为 amount + 1
  std::vector<int> dp(amount + 1, amount + 1);
  /// base case
  dp[0] = 0;
  for (int i = 0; i < dp.size(); i++){
    /// 内层 for 在求所有子问题 +1 的最小值
    for (int coin : coins){
      if (i - coin < 0) continue;
      dp[i] = min(dp[i], 1 + dp[i - coin]);
    }
  }
  return (dp[amount] == amount + 1) ? -1 : dp[amount];
}


}  // namespace dynamic_programming_sample
}  // namespace data_struct
}  // namespace Lee

#endif