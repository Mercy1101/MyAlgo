#include <iostream>
#include <vector>
#include <chrono>
#include "introsort.h"
#include <cassert>

using namespace std;

void TestIntroSort(int NumberLimit)
{
    std::cout << "Into test : " << NumberLimit << std::endl;
    vector<int> v(NumberLimit);

    for (int i = 0; i < NumberLimit; i++) {
        v[i] = rand() % 100'000;
    }

    const auto b = v;

    std::chrono::high_resolution_clock::time_point begin1 = std::chrono::high_resolution_clock::now();
    introsort(v.begin(), v.end());
    std::chrono::high_resolution_clock::time_point end1 = std::chrono::high_resolution_clock::now();
    std::cout << "introsort Time difference = "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1).count() << std::endl;
    assert(is_sorted(v.begin(), v.end()));

    v = b;

    begin1 = std::chrono::high_resolution_clock::now();
    sort(v.begin(), v.end());
    end1 = std::chrono::high_resolution_clock::now();
    std::cout << "std::sort Time difference = "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1).count() << std::endl;
    assert(is_sorted(v.begin(), v.end()));

    std::cout << "Out test : " << NumberLimit << std::endl << std::endl;
}

int main() {
//    gen_data();
    TestIntroSort(10);
    TestIntroSort(100);
    TestIntroSort(1'000);
    TestIntroSort(10'000);
    TestIntroSort(100'000);
    TestIntroSort(1'000'000);
    TestIntroSort(10'000'000);
    TestIntroSort(100'000'000);

    system("pause");

    return 0;
}