#ifndef MYALGO_INCLUDE_BINARY_SEARCH_H
#define MYALGO_INCLUDE_BINARY_SEARCH_H

template<class ForwardIt, class T>
bool binary_search(ForwardIt first, ForwardIt last, const T& value);

template<class ForwardIt, class T, class Compare>
bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp);

#endif