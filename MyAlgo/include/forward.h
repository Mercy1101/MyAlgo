#ifndef MYALGO_INCLUDE_FORWARD
#define MYALGO_INCLUDE_FORWARD

template <typename T>
T&& forward(typename remove_reference<T>::type& arg);

#endif