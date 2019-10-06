
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/67fcc66a98f14a84b1ba7af8dd3b4a36)](https://app.codacy.com/app/AlexandruValeanu/Introsort?utm_source=github.com&utm_medium=referral&utm_content=AlexandruValeanu/Introsort&utm_campaign=Badge_Grade_Settings)

Simple implementation of [introsort](https://en.wikipedia.org/wiki/Introsort) (introspective sort) algorithm, hybrid of 
quicksort, heapsort, and insertion sort that has particularly 
good runtime behavior.  It is one of the fastest comparison sorting, algorithms in use today, and is the usual implementation of the std::sort algorithm provided with the C++ STL.

Key attributes of this implementation:
* 3-way partition 
* median of three (using 3 randomly chosen elements) for pivot selection
* sorting networks for 1 &le; N &le; 6
* insertion sort for 7 &le; N &le; 25
* quicksort uses only one recursive call (the other one is transformed into tail recursion)
* std::heap_sort if quicksort degenerates (depth &gt; ```2 * log(n)```)
