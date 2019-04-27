#include <iostream>
#include "list_template.h"

template<typename T>
List<T>::~List()
{
    if (nullptr != m_pHead)
    {
        for (int i = 0; i < m_iListSize; i++)
        {
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

template<typename T>
void List<T>::print(std::ostream &os = std::cout) const
{}

template<typename T>
const T& List<T>::iteraor::operator*() const
{
    return m_ptr->stTemplate;
}

template<typename T>
ListNode<T>* List<T>::iteraor::operator->() const
{
    return m_ptr;
}

template<typename T>
List<T>::iteraor& List<T>::iteraor::operator++() const
{
    m_ptr = m_ptr->pNextNode;
    // What is returned here, need to be considered,
    // '*' Is it the overloaded symbol or what?
    return *this;
}

/* This function does not understand,
the input after the symbol '++' do not know where to call */
template<typename T>
List<T>::iterator List<T>::iteraor::operator++(int) const
{
    ListNode<T>* tmp = m_ptr;
    m_ptr = m_ptr->next;
    return iterator(tmp);
}

template<typename T>
bool List<T>::iteraor::operator==(const List<T>::iterator &arg) const
{
    return arg.m_ptr == this->m_ptr;
}

template<typename T>
bool List<T>::iteraor::operator!=(const List<T>::iterator &art) const
{
    return arg.m_ptr != this->m_ptr;
}

template<typename T>
List<T>::iterator List<T>::cbegin() const
{
    return iterator(m_pHead);
}

template<typename T>
List<T>::iterator List<T>::cend() const
{
    return iterator(m_pTail);
}