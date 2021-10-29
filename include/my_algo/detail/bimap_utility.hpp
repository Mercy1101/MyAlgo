/**
 * @file            bimap_utility.hpp
 * @brief           实现双键 map 数据结构
 * @author          lijiancong(pipinstall@163.com)
 * @version         0.1
 * @date            2021-08-21 09:43:13
 *
 */

#ifndef MY_ALGO_BIMAP_UTILITY_HPP__
#define MY_ALGO_BIMAP_UTILITY_HPP__

#include <map>
#include <list>

namespace lee
{
/**
 * @brief           双键数据结构
 * @author          lijiancong(pipinstall@163.com)
 * @date            2021-09-02 13:16:49
 * @note
 *
 */
template <class LEFT_KEY, class RIGHT_KEY, class VALUE>
class bimap
{
    /// 双键数据结构的值类型定义
    struct value_type
    {
        LEFT_KEY lk;   ///< 左键
        RIGHT_KEY rk;  ///< 右键
        VALUE value;   ///< 值
        value_type() {}
        value_type(const LEFT_KEY& left_key, const RIGHT_KEY& right_key, const VALUE& v)
            : lk(left_key), rk(right_key), value(v)
        {
        }

        value_type(const value_type& other)
        {
            lk = other.lk;
            rk = other.rk;
            value = other.value;
        }

        value_type& operator=(const value_type& other)
        {
            if (&other != this)
            {
                lk = other.lk;
                rk = other.rk;
                value = other.value;
            }
            return *this;
        }
    };

public:
    typedef typename std::list<value_type>::iterator iterator;  ///< 定义该数据结构的迭代器类型

    /// 插入元素
    bool insert(const LEFT_KEY& lKey, const RIGHT_KEY& rKey, const VALUE& value)
    {
        if (m_LeftKeyMap.find(lKey) != m_LeftKeyMap.end() || m_RightKeyMap.find(rKey) != m_RightKeyMap.end())
        {
            /// 左键重复或右键重复直接返回false
            return false;
        }
        else
        {
            value_type st(lKey, rKey, value);
            iterator it = m_Value.insert(m_Value.end(), st);
            m_LeftKeyMap[lKey] = it;
            m_RightKeyMap[rKey] = it;
            return true;
        }
    }

    /// 左键擦除
    bool left_erase(const LEFT_KEY& lKey)
    {
        if (m_LeftKeyMap.find(lKey) == m_LeftKeyMap.end())
        {
            return false;
        }
        else
        {
            m_RightKeyMap.erase(m_LeftKeyMap[lKey]->rk);
            m_Value.erase(m_LeftKeyMap.find(lKey)->second);
            m_LeftKeyMap.erase(lKey);
            return true;
        }
    }

    /// 右键擦除
    bool right_erase(const RIGHT_KEY& rKey)
    {
        if (m_RightKeyMap.find(rKey) == m_RightKeyMap.end())
        {
            return false;
        }
        else
        {
            m_LeftKeyMap.erase(m_RightKeyMap[rKey]->lk);
            m_Value.erase(m_RightKeyMap.find(rKey)->second);
            m_RightKeyMap.erase(rKey);
            return true;
        }
    }

    /// 左键查找
    iterator left_find(const LEFT_KEY& lk)
    {
        if (m_LeftKeyMap.find(lk) == m_LeftKeyMap.end())
        {
            return m_Value.end();
        }
        else
        {
            return m_LeftKeyMap[lk];
        }
    }

    /// 右键查找
    iterator right_find(const RIGHT_KEY& rk)
    {
        if (m_RightKeyMap.find(rk) == m_RightKeyMap.end())
        {
            return m_Value.end();
        }
        else
        {
            return m_RightKeyMap[rk];
        }
    }

    iterator end()
    {
        return m_Value.end();
    }

    /// 本数据结构是否为空
    bool empty()
    {
        return m_Value.empty();
    }

    /// 清空本数据结构
    void clear()
    {
        m_Value.clear();
        m_LeftKeyMap.clear();
        m_RightKeyMap.clear();
    }

private:
    std::list<value_type> m_Value;                ///< 用于存放值
    std::map<LEFT_KEY, iterator> m_LeftKeyMap;    ///< 用于保存左键与值得映射关系的map
    std::map<RIGHT_KEY, iterator> m_RightKeyMap;  ///< 用于保存右键与值得映射关系的map
};
}  // namespace lee

#endif  ///< MY_ALGO_BIMAP_UTILITY_HPP__
