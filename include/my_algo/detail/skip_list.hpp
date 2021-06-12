///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   skip_list.h
/// @brief  跳表的简单实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-03 09:41:10
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_SKIP_LIST_HPP_
#define INCLUDE_DETAIL_SKIP_LIST_HPP_

#include <random>

namespace lee
{
inline namespace skiplist
{
class skip_list
{
    struct Node
    {
        Node(int i, Node *r, Node *d) : val(i), right(r), down(d) {}
        int val;
        Node *right;
        Node *down;
    };

public:
    skip_list() {}

    bool search(int target)
    {
        auto cur = head;
        while (cur)
        {
            /// 如果目标值大于当前节点，则向前步进查找
            while (cur->right && cur->right->val < target)
            {
                cur = cur->right;
            }
            if (cur->right && cur->right->val == target)
            {
                return true;
            }
            /// 目标值小于该节点的右节点则从该节开始降级查找
            cur = cur->down;
        }
        return false;
    }

    void add(int num)
    {
        int r_level = 1;
        /// 当层级小于最大层级，则继续随机增加层级
        while (r_level <= max_level && ((std::rand() & 1) == 0))
        {
            ++r_level;
        }

        if (r_level > max_level)
        {
            max_level = r_level;
            /// 头节点创建一个新节点
            head = new Node(num, nullptr, head);
        }

        Node *cur = head;
        Node *last = nullptr;
        for (int l = max_level; l >= 1; --l)
        {
            /// 当右边的节点值大于当前值，则向右步进
            while (cur->right && cur->right->val < num)
            {
                cur = cur->right;
            }
            /// 当前层级小于该节点的层级则创建一个节点，用于保存目标值。
            if (l <= r_level)
            {
                /// 创建同级右边节点
                cur->right = new Node(num, cur->right, nullptr);
                if (last)
                {
                    /// 上一个节点的右节点不为空时，其下级节点是当前节点的右节点
                    last->down = cur->right;
                }
                /// 保留这个节点做个下一次层的父节点
                last = cur->right;
            }
            /// 降级
            cur = cur->down;
        }
    }

    bool erase(int num)
    {
        auto cur = head;
        bool seen = false;
        for (int l = max_level; l >= 1; --l)
        {
            while (cur->right && cur->right->val < num)
            {
                cur = cur->right;
            }
            if (cur->right && cur->right->val == num)
            {
                seen = true;
                Node *temp = cur->right;
                /// 右节点指针指向删除节点后的节点
                cur->right = cur->right->right;
                delete temp;
            }
            cur = cur->down;
        }
        return seen;
    }

private:
    int max_level = 0;     ///< 最大的层级数，动态生成
    Node *head = nullptr;  ///< 头节点
};

}  // namespace skiplist
}  // namespace lee

#endif  ///< end of INCLUDE_DETAIL_SKIP_LIST_HPP_