#ifndef INTROSORT_SORTING_NETWORKS_H
#define INTROSORT_SORTING_NETWORKS_H

#include <algorithm>

namespace introsort_implementation {

    #define sort_net2(x, y) if (comp(y, x)) std::swap(x, y)

    template<typename T, typename Comparator>
    void sort2(T &a, T &b, Comparator comp) {
        sort_net2(a, b);
    };

    template<typename T, typename Comparator>
    void sort3(T &a, T &b, T &c, Comparator comp) {
        sort_net2(b, c);
        sort_net2(a, c);
        sort_net2(a, b);
    };

    template<typename T, typename Comparator>
    void sort4(T &a, T &b, T &c, T &d, Comparator comp) {
        sort_net2(a, b);
        sort_net2(c, d);
        sort_net2(a, c);
        sort_net2(b, d);
        sort_net2(b, c);
    };

    template<typename T, typename Comparator>
    void sort5(T &a, T &b, T &c, T &d, T &e, Comparator comp) {
        sort_net2(a, b);
        sort_net2(d, e);
        sort_net2(c, e);
        sort_net2(c, d);
        sort_net2(b, e);
        sort_net2(a, d);
        sort_net2(a, c);
        sort_net2(b, d);
        sort_net2(b, c);
    };

    template<typename T, typename Comparator>
    void sort6(T &a, T &b, T &c, T &d, T &e, T &f, Comparator comp) {
        sort_net2(b, c);
        sort_net2(e, f);
        sort_net2(a, c);
        sort_net2(d, f);
        sort_net2(a, b);
        sort_net2(d, e);
        sort_net2(c, f);
        sort_net2(a, d);
        sort_net2(b, e);
        sort_net2(c, e);
        sort_net2(b, d);
        sort_net2(c, d);
    };
}

#endif //INTROSORT_SORTING_NETWORKS_H
