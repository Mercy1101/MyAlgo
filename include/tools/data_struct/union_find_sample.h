
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019, 2020 Lijiancong. All rights reserved.
///
/// @file   union_find_sample.h
/// @brief  自己尝试实现一个智能指针
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-06-08 22:48:56
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_UNION_FIND_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_UNION_FIND_H_

namespace Lee {
inline namespace data_struct {
inline namespace union_find_sample{
  class UF{
    public:
     UF(int n){
       this->count_ = n;
       parent = new int[n];
       size = new int[n];
       for(int i = 0; i < n; i++){
         parent[i] = i;
         size[i] = 1;
       }
    }

    void union_(int p, int q){
      int rootP = find(p);
      int rootQ = find(q);
      if(rootP == rootQ){
        return;
      }

      /// 小树接到大树下面，较平衡
      if(size[rootP] > size[rootQ]){
        parent[rootQ] = rootP;
        size[rootP] += size[rootQ];
      } else {
        parent[rootP] = rootQ;
        size[rootQ] += size[rootP];
      }
      --count_;
    }

    bool connected(int p, int q){
      int rootP = find(p);
      int rootQ = find(q);
      return rootP == rootQ;
    }

    int find(int x){
      while(parent[x] != x){
        /// 进行路径压缩
        parent[x] = parent[parent[x]];
        x = parent[x];
      }
      return x;
    }

    int count(){
      return count_;
    }
    private:
    int count_;  ///< 连同分量个数
    int *parent; ///< 存储一棵树
    int *size; ///< 记录树的重量

  };
}
}  // namespace data_struct
}  // namespace Lee

#endif