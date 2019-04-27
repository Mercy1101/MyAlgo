#include <iostream>
#include "list_template.h"

template<typename T>
List<T>::~List()
{
    if (nullptr != m_pHead)
    {
        List<T>* temp = m_pHead->pNextNode;
        free(m_pHead);
        temp = temp->pNextNode;
        for (int i = 0; i < m_iListSize; i++)
        {
            if (nullptr == temp->next)
            {
                List<T>* temp2 = temp->next;
                free(temp);
                temp = temp2;
            }
            else
            {
                break;
            }
        }
    }
}

template<typename T>
bool List<T>::push_back(const ListNode<T> &tempInput)
{
    if (nullptr == m_Head)
    {
        m_Head = new List<T>(tempInput);
        m_pTail = m_pHead;
        m_iListSize = 1;
    }
    else
    {
        m_pTail->pNextNode = new List<T>(tempInput);
        m_pTail = m_pTail->pNextNode;
        m_iListSize++;
    }
}

#if 0
// TODO
template<typename T>
ListNode<T> & List<T>::at(int index)
{
    if (index < 0 || index > INT_MAX || index > m_iListSize - 1)
};
#endif