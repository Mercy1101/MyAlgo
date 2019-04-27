#ifndef SRC_LIST_TEMPLATE_H
#define SRC_LIST_TEMPLATE_H

template<typename T>
struct ListNode
{
    T            stTemplate;
    ListNode<T>* pNextNode;

    ListNode() :pNextNode(nullptr) {}
    ListNode(T stInput, ListNode<T>* pInput = nullptr)
        :stTemplate(stInput), pNextNode(pInput) {}
};

template<typename T>
class List
{
private:
    ListNode<T>* m_pHead;
    ListNode<T>* m_pTail;
    int          m_iListSize;

public:
    explicit List() :m_pHead(nullptr), m_pTail(nullptr), m_iListSize(0) {}
    ~List();
    List(const List<T> & rhs) = default;                      // Copy constructor
    List(List<T> && rhs) = default;                           // Move constructor
    List<T> & operator= (const List<T> & rhs) = default;      // Copy assignment
    List<T> & operator= (List<T> && rhs) = default;           // Move assignment

    bool push_back(const ListNode<T> &tempInput);
    // ListNode<T> & at(int index);
};

#endif