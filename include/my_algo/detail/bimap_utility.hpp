/**
 * @file            bimap_utility.hpp
 * @brief           ʵ��˫�� map ���ݽṹ
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
 * @brief           ˫�����ݽṹ
 * @author          lijiancong(pipinstall@163.com)
 * @date            2021-09-02 13:16:49
 * @note
 *
 */
template <class LEFT_KEY, class RIGHT_KEY, class VALUE>
class bimap
{
    /// ˫�����ݽṹ��ֵ���Ͷ���
    struct value_type
    {
        LEFT_KEY lk;   ///< ���
        RIGHT_KEY rk;  ///< �Ҽ�
        VALUE value;   ///< ֵ
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
    typedef typename std::list<value_type>::iterator iterator;  ///< ��������ݽṹ�ĵ���������

    /// ����Ԫ��
    bool insert(const LEFT_KEY& lKey, const RIGHT_KEY& rKey, const VALUE& value)
    {
        if (m_LeftKeyMap.find(lKey) != m_LeftKeyMap.end() || m_RightKeyMap.find(rKey) != m_RightKeyMap.end())
        {
            /// ����ظ����Ҽ��ظ�ֱ�ӷ���false
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

    /// �������
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

    /// �Ҽ�����
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

    /// �������
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

    /// �Ҽ�����
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

    /// �����ݽṹ�Ƿ�Ϊ��
    bool empty()
    {
        return m_Value.empty();
    }

    /// ��ձ����ݽṹ
    void clear()
    {
        m_Value.clear();
        m_LeftKeyMap.clear();
        m_RightKeyMap.clear();
    }

private:
    std::list<value_type> m_Value;                ///< ���ڴ��ֵ
    std::map<LEFT_KEY, iterator> m_LeftKeyMap;    ///< ���ڱ��������ֵ��ӳ���ϵ��map
    std::map<RIGHT_KEY, iterator> m_RightKeyMap;  ///< ���ڱ����Ҽ���ֵ��ӳ���ϵ��map
};
}  // namespace lee

#endif  ///< MY_ALGO_BIMAP_UTILITY_HPP__
