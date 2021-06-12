///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   tree_node_serialize.h
/// @brief  二叉树的序列化与反序列化
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-11 15:13:24
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_TREE_NODE_SERIALIZE_HPP_
#define INCLUDE_DETAIL_TREE_NODE_SERIALIZE_HPP_

#include <queue>
#include <string>

namespace lee
{
inline namespace serialize
{
struct TreeNode
{
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class tree_node_serialize
{
public:
    // Encodes a tree to a single string.
    std::string serialize(TreeNode* root)
    {
        std::string res = "[";
        std::queue<TreeNode*> q;
        q.push(root);
        while (!q.empty())
        {
            auto temp = q.front();
            q.pop();
            if (temp == nullptr)
            {
                res += "null,";
            }
            else
            {
                res += std::to_string(temp->val) + ",";
                q.push(temp->left);
                q.push(temp->right);
            }
        }
        res += "]";
        return res;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(std::string data)
    {
        /// 去除首尾的'[]'
        data.erase(data.begin());
        data.erase(std::prev(data.end()));
        /// 把字符串解析成数组
        auto lastPos = data.find_first_not_of(',', 0);
        auto pos = data.find_first_of(',', lastPos);
        std::queue<std::string> q;
        while (pos != std::string::npos || std::string::npos != lastPos)
        {
            q.push(data.substr(lastPos, pos - lastPos));
            lastPos = data.find_first_not_of(',', pos);
            pos = data.find_first_of(',', lastPos);
        }
        /// 遍历数组建立树
        auto head = q.front();
        std::queue<TreeNode*> q_cur;
        std::queue<TreeNode*> q_next;
        TreeNode* r = nullptr;
        if (head != "null")
        {
            auto thead = new TreeNode(std::atoi(head.c_str()));
            q_cur.push(thead);
            r = thead;
        }
        q.pop();
        while (!q.empty())
        {
            while (!q_cur.empty())
            {
                auto t = q_cur.front();
                q_cur.pop();
                auto l_t = q.front();
                q.pop();
                auto r_t = q.front();
                q.pop();
                if (l_t != "null")
                {
                    t->left = new TreeNode(std::atoi(l_t.c_str()));
                    q_next.push(t->left);
                }
                if (r_t != "null")
                {
                    t->right = new TreeNode(std::atoi(r_t.c_str()));
                    q_next.push(t->right);
                }
            }
            while (!q_next.empty())
            {
                q_cur.push(q_next.front());
                q_next.pop();
            }
        }
        return r;
    }
};
}  // namespace serialize
}  // namespace lee

#endif  ///< end of INCLUDE_DETAIL_TREE_NODE_SERIALIZE_HPP_