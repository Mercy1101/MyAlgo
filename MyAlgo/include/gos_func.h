#ifndef  SRC_LIST_H
#define SRC_LIST_H

const int INVALID_VALUE = 0xFFFFFFFF;

struct T
{
    T*  pNext;
    int iValue;
};

class list
{
public:
    explicit list();
    explicit list(int iValue);
    ~list();
    /*
    list(const list & rhs) = default;                   // Copy constructor
    list(list && rhs) = default;                        // Move constructor
    list & operator= (const list & rhs) = default;      // Copy assignment
    list & operator= (list && rhs) = default;           // Move assignment
    */

    bool push_back(const int &iValue);
    bool Insert(const int &iValue, const int &iIndex);
    const int& at(const int &k) const;

private:
    struct T m_T;
    T *m_pstRoot;
};

#endif