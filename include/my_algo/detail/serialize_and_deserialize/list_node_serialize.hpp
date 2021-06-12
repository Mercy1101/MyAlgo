///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   list_node_serialize.h
/// @brief  链表的序列化与反序列化
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-11 18:55:11
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_LIST_NODE_SERIALIZE_HPP_
#define INCLUDE_DETAIL_LIST_NODE_SERIALIZE_HPP_

#include <string>
#include <vector>

namespace lee
{
inline namespace serialize
{
struct ListNode
{
    int val = 0;
    ListNode* next = nullptr;
    ListNode(int num) : val(num), next(nullptr) {}
};
class list_node_serialize
{
public:
    std::string serialize(ListNode* root)
    {
        std::string res = "[";
        while (root != nullptr)
        {
            res += std::to_string(root->val) + "";
            root = root->next;
        }
        res += "]";
        return res;
    }

    ListNode* deserialize(std::string data)
    {
        data.erase(data.begin());
        data.erase(std::prev(data.end()));
        auto last_pos = data.find_first_not_of(',', 0);
        auto pos = data.find_first_of(',', last_pos);
        std::vector<int> vec;
        while (last_pos != std::string::npos || pos != std::string::npos)
        {
            vec.push_back(std::stoi(data.substr(last_pos, pos - last_pos)));
            last_pos = data.find_first_not_of(',', pos);
            pos = data.find_first_of(',', last_pos);
        }
        if (vec.empty())
            return nullptr;
        ListNode* head = new ListNode(vec.at(0));
        auto res = head;
        for (size_t i = 1; i < vec.size(); ++i)
        {
            head->next = new ListNode(vec.at(i));
            head = head->next;
        }
        return res;
    }
};
}  // namespace serialize
}  // namespace lee

#endif  ///< end of INCLUDE_DETAIL_LIST_NODE_SERIALIZE_HPP_