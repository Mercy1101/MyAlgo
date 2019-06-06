#include "forward.h"

template <typename T>
T&& forward(typename remove_reference<T>::type& arg)
{
    return static_cast<T&&>(arg);
}