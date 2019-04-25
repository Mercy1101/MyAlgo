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
    void print(std::ostream &os = std::cout) const;

    class iteraor
    {
    private:
        ListNode<T> *m_ptr;
    public:
        explicit iterator(ListNode<T> *pInput = nullptr) : m_ptr(pInput) {}
        ~iterator() = default;
        iteraor(const iteraor & rhs) = default;
        iteraor(iteraor && rhs) = default;
        iteraor & operator= (const iteraor &rhs) = default;
        iteraor & operator= (iteraor && rhs) = default;

        const T& operator*() const;
        ListNode<T>* operator->() const;
        List<T>::iteraor& operator++() const;
        List<T>::iterator operator++(int) const;
        bool operator==(const List<T>::iterator &arg) const;
        bool operator!=(const List<T>::iterator &art) const;
    };

public:
    List<T>::iterator cbegin() const;
    List<T>::iterator cend() const;
};

#endif