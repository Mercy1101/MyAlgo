#include <iostream>
#include "list.h"

list::list()
{
    m_pstRoot = nullptr;
}

list::list(int iValue)
{
    m_pstRoot = nullptr;
    this->push_back(iValue);
}

list::~list()
{
    /* 如果是第一个节点 */
    if (nullptr == m_pstRoot)
    {
        return;
    }
    else
    {
        T* pstTemp = m_pstRoot;
        T* WillDeletePst = nullptr;
        while (pstTemp->pNext != nullptr)
        {
            WillDeletePst = pstTemp->pNext;
            delete pstTemp;
            pstTemp = WillDeletePst;
        }
        delete pstTemp;
    }
}

bool list::push_back(const int &iValue)
{
    /* 如果是第一个节点 */
    if (nullptr == m_pstRoot)
    {
        m_pstRoot = new T;
        m_pstRoot->pNext = nullptr;
        m_pstRoot->iValue = iValue;
    }
    else
    {
        T* pstTemp = m_pstRoot;
        while (pstTemp->pNext != nullptr)
        {
            pstTemp = pstTemp->pNext;
        }

        pstTemp->pNext = new T;
        pstTemp->pNext->iValue = iValue;
        pstTemp->pNext->pNext = nullptr;
    }
    return true;
}

/* 在第iIndex位置后插入元素 */
bool list::Insert(const int &iValue, const int &iIndex)
{
    if (iIndex < 0 || nullptr == m_pstRoot)
    {
        // assert(0);
        return false;
    }
    else
    {
        T* pTmp = m_pstRoot;
        for (int i = 0; i < iIndex; ++i)
        {
            if (pTmp == nullptr)
            {
                /* iIndex大于list总长度，就直接在list后面添加该元素 */
                this->push_back(iValue);
                return false;
            }
            pTmp = pTmp->pNext;
        }
        T* const pIndex = pTmp->pNext;
        pTmp->pNext = new T;
        pTmp->pNext->pNext = pIndex;
        pTmp->pNext->iValue = iValue;
        return true;
    }
}

const int& list::at(const int &iIndex) const
{
    /* Index 非法情況 */
    if (iIndex < 0 || nullptr == m_pstRoot)
    {
        // assert(0);
        return INVALID_VALUE;
    }

    T* pSt = m_pstRoot;
    if (0 == iIndex)
    {
        return pSt->iValue;
    }

    for (int i = 0; i < iIndex; i++)
    {
        if (nullptr == pSt->pNext)
        {
            // assert(0);
            return INVALID_VALUE;
        }
        pSt = pSt->pNext;
    }
    return pSt->iValue;
}