#ifndef INCLUDE_ALGORITHM_IMP_H
#define INCLUDE_ALGORITHM_IMP_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include "gos_typedef.h"

namespace Lee { namespace STL {

/*
* @name               : ShowVector
* @function           : ���ڴ�ӡvector�е�Ԫ�أ����ڵ���
* @class              : NONE
* @return             : NONE
* @author             : Lijiancong, 316, lijiancong@fritt.com.cn
* @createtime         : 2019-06-12 09:14:13
* @modifytime         : 2019-06-12 09:14:13
* @note               : 
*/
template<typename I>
void ShowVector(std::vector<I> &vInput)
{
    for (std::vector<I>::const_iterator cit = vInput.cbegin(); cit != vInput.cend(); ++cit)
    {
        std::cout << *cit << std::endl;
    }
    std::cout << std::endl;
}

/*
* @name               : unique
* @function           : ����ȥ��vector���ظ�Ԫ��(������)
* @return             : NONE
* @author             : Lijiancong, 316, lijiancong@fritt.com.cn
* @createtime         : 2019-06-12 09:15:48
* @modifytime         : 2019-06-12 09:15:48
* @note               : ���˼·���ڰ�vector�е�Ԫ����ȡ����������ʱ����map��
                                  �����ø�map��дvector�е�Ԫ�أ�
                                  �Դﵽ�����ظ�Ԫ�ص�Ŀ�ġ�

                        ԭ������std::map��Insert��Ա������
                              ��û���ظ�����(std::map.first)��Ԫ��ʱ����룬
                              ���ظ�������Ԫ��ʱ���д��������ֵ(std::map.second).

                        ע�⣺�޷���C�����ַ�����char*��char[] ������ָ��ָ���ֵ����ȥ�ء�

                        ���ð汾��VS2010����ͨ����C++Builder��ʱδ���ԡ�
*/
template<typename I>
void unique(std::vector<I> &vInput  // [in/out]
            )
{
    std::map<I, int> mTemp;
    for (std::vector<I>::const_iterator it = vInput.cbegin(); it != vInput.cend(); ++it)
    {
        mTemp.insert(std::map<I, int>::value_type(*it, 1));
    }

    vInput.clear();
    for (std::map <I, int>::const_iterator it = mTemp.cbegin(); it != mTemp.cend(); ++it)
    {
        vInput.push_back(it->first);
    }
}

/********************************quicksort**************************************/
#define RANDOM_INIT()   srand(time(NULL))
#define RANDOM(L, R)    (L + rand() % ((R) - (L) + 1)) // gen a random integer in [L, R]

/**
* swap 2-element, orignal value
*/
template<typename T>
static inline void swap(T &x, T &y)
{
    T _t(x);
    x = y;
    y = _t;
}

/**
* the quick-sort partition routine
*/
template<typename T, typename Compare>
static int partition_(T list[], int begin, int end, Compare Comp)
{
    int pivot_idx = RANDOM(begin, end);
    T pivot = list[pivot_idx];
    swap(list[begin], list[pivot_idx]);

    int i = begin + 1;
    int j = end;

    while(i <= j)
    {
        while((i <= end) && (Comp(pivot, list[i])))
            i++;
        while((j >= begin) && (Comp(list[j], pivot)))
            j--;
        if(i < j)
            swap(list[i], list[j]);
    }

    swap(list[begin], list[j]);
    return j; // final pivot position
}

/**
* quick sort an array of range [begin, end]
*/
template<typename T, typename Compare>
static void quicksort(T list[], int begin, int end, Compare Comp)
{
    if( begin < end)
    {
        int pivot_idx = partition_<T, Compare>(list, begin, end, Comp);
        quicksort(list, begin, pivot_idx-1, Comp);
        quicksort(list, pivot_idx+1, end, Comp);
    }
}

/***************************************************************************/

/**************************sort*******************************************/
template<class RandomIterator, class BinaryPredicate>
typename std::iterator_traits<RandomIterator>::value_type
mid3(RandomIterator first, RandomIterator last, BinaryPredicate pred)
{//[first, last]
    auto mid = first + (last + 1 - first) / 2;
    if (pred(*mid, *first))
    {
        swap(*mid, *first);
    }
    if (pred(*last, *mid))
    {
        swap(*last, *mid);
    }
    if (pred(*last, *first))
    {
        swap(*last, *first);
    }
    auto ret = *mid;
    swap(*mid, *(last - 1));//��mid item��λ��Ϊ�ڱ�
    return ret;
}

template<class RandomIterator, class BinaryPredicate>
void bubble_sort(RandomIterator first, RandomIterator last, BinaryPredicate pred)
{
    auto len = last - first;
    for (auto i = len; i != 0; --i)
    {
        bool swaped = false;
        for (auto p = first; p != (first + i - 1); ++p)
        {
            if (pred(*(p + 1), *p))
            {
                swap(*(p + 1), *p);
                swaped = true;
            }
        }
        if (!swaped)
            break;
    }
}

template<class T>
struct less
{
    typedef T First_Iteraor;
    typedef T Second_Iteraor;
    typedef bool Return_Value;
    Return_Value operator()(const First_Iteraor& x, const Second_Iteraor& y)
    {
        return x < y;
    }
};

template<class RandomIterator>
void sort(RandomIterator first, RandomIterator last)
{
    return sort(first, last, less<typename std::iterator_traits<RandomIterator>::value_type>());
}

template<class RandomIterator, class BinaryPredicate>
void sort(RandomIterator first, RandomIterator last, BinaryPredicate pred)
{
    if (first >= last || first + 1 == last)
        return;
    if (last - first <= 20)//���䳤��С�ڵ���20�Ĳ���ð���������
        return bubble_sort(first, last, pred);
    auto mid = mid3(first, last - 1, pred);
    auto p1 = first, p2 = last - 2;
    while (p1 < p2)
    {
        while (pred(*p1, mid) && (p1 < p2)) ++p1;
        while (!pred(*p2, mid) && (p1 < p2)) --p2;
        if (p1 < p2)
        {
            swap(*p1, *p2);
        }
    }
    swap(*p1, *(last - 2));//����Ϊ�ڱ���mid item����ԭ����λ��
    sort(first, p1, pred);
    sort(p1 + 1, last, pred);
}
/***************************************************************************/

/***********************[max]***********************************************/
//********* [Algorithm Complexity: O(1)] ****************
template <class T> 
const T& max(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

template <class T, class Compare>
const T& max(const T& a, const T& b, Compare comp)
{
    return (comp(a, b)) ? a : b;
}
/***************************************************************************/

/***********************[min]*************************************************/
//********* [Algorithm Complexity: O(1)] ****************
template <class T> 
const T& min(const T& a, const T& b)
{
    return !(b < a) ? a : b;
}

template <class T, class Compare>
const T& min(const T& a, const T& b, Compare comp)
{
    return !comp(b, a) ? a : b;
}
/***************************************************************************/


/*
* @name               : ROUND_UP
* @function           : ����ALGN����������������ȡ����
* @class              : 
* @globalparam        : NONE
* @classparam         : NONE
* @database           : NONE
* @return             : 
* @author             : Lijiancong, 316, lijiancong@fritt.com.cn
* @createtime         : 2019-06-13 15:48:57
* @modifytime         : 2019-06-13 15:48:57
* @note               : 
*/
static size_t ROUND_UP(size_t bytes)
{
    const int ALIGN = 8;
    auto x = ((bytes) + ALIGN-1);
    auto y = ~(ALIGN-1);
    return (x & y);
}



}  // end of namespace STL
}  // end of namespace Lee

#endif