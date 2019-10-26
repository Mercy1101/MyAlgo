### STL算法速览

***注意***：如某容器的成员函数有对应的功能函数，应**优先选用成员函数**而非以下STL算法函数。因为特定容器的成员函数为该容器的实现提供了更好的算法，相比较普适性较强的STL算法函数拥有更好的时间复杂度和空间复杂度。

STL算法：

+ 非更易型算法（Nonmodifying algorithms）

+ 更易型算法（Modifying algorithms）

+ 移除型算法（Removing algorithms）

+ 变序型算法（Mutating algorithms）

+ 排序算法（Sorting algorithms）

+ 已排序区间算法（Sorted-range algorithms）

+ 数值算法（Numeric algorithms）

#### 非更易型算法（Nonmodifying algorithms）

| 名称                      | 效果                                                         |
| ------------------------- | ------------------------------------------------------------ |
| for_each()                | 对每个元素执行某操作                                         |
| count()                   | 返回元素个数                                                 |
| count_if()                | 返回满足某一准则(条件)的元素个数                             |
| min_element()             | 返回最小值元素                                               |
| max_element()             | 返回最大值元素                                               |
| minmax_element()          | 返回最小值和最大值元素                                       |
| find()                    | 查找“与被传入值相等”的第一个元素                             |
| find_if()                 | 查找“满足某个准则”的第一个元素                               |
| find_if_not()             | 查找“不满足某个准则”的第一个元素                             |
| search_n()                | 查找”具备某特性“之前n个连续元素                              |
| search()                  | 查找某个子区间的第一次出现位置                               |
| find_end()                | 查找某个子区间的最后一次出现的位置                           |
| find_first_of()           | 查找”数个可能元素中的第一个出现者“                           |
| adjacent_find()           | 查找连续两个相等（或者说符合特定准则）的元素                 |
| equal()                   | 判断两区间是否相等                                           |
| is_permutation()          | 连个不定序区间是否含有相等元素                               |
| mismatch()                | 返回两序列的各组对应元素中的第一对不相等元素                 |
| lexicographical_compare() | 判断在”字典顺寻“(lexicographically)下某序列是否小于另一序列  |
| is_sorted()               | 返回”是否区间内的元素已排序“                                 |
| is_sorted_until()         | 返回"区间内的元素是否基于某准则被分割为两组"                 |
| partition_point()         | 返回区间内的一个分割元素， 它把元素切割为两组，其中一组满足某个predicate，另一组则不然 |
| is_heap()                 | 返回”是否区间内的元素形成一个heap“                           |
| is_heap_until()           | 返回”是否所有元素都吻合某准则的元素“                         |
| all_of()                  | 返回”是否所有元素都吻合某准则“                               |
| any_of()                  | 返回”是否至少一个元素吻合某准则“                             |
| none_of()                 | 返回”是否五任何元素吻合某准则“                               |

#### 更易型算法（Modifying algorithms）

| 名称              | 效果                                               |
| ----------------- | -------------------------------------------------- |
| for_each()        | 针对每个元素执行某项操作                           |
| copy()            | 从某个元素开始，复制某个区间                       |
| copy_if()         | 复制那些”符合某个给定准则“的元素                   |
| copy_n()          | 复制n个元素                                        |
| copy_backward()   | 从最后一个元素开始，复制某个区间                   |
| move()            | 从第一个元素开始，搬移某个区间                     |
| move_backward()   | 从最后一个元素开始，搬移某个区间                   |
| transform()       | 改动（并复制）元素，将两个区间的元素合并           |
| merge()           | 合并两个区间                                       |
| swap_ranges()     | 交换两区间的元素                                   |
| fill()            | 以给定值替换每一个元素                             |
| fill_n()          | 以给定值替换n个元素                                |
| generate()        | 以某项操作的结果替换每一个元素                     |
| generate_n()      | 以某项操作的结果替换n个元素                        |
| iota()            | 将所有元素以一系列的递增值取代                     |
| replace()         | 将具有某特定值的元素替换为另一个值                 |
| replace_copy()    | 复制整个区间，并将具有某特定值的元素替换为另一个值 |
| replace_copy_if() | 复制整个区间，并将符合某准则的元素替换为另一个值   |

#### 移除型算法（Removing algorithms）

| 名称             | 效果                                   |
| ---------------- | -------------------------------------- |
| remove()         | 将“等于某特定值”的元素全部移除         |
| remove_if()      | 将“满足某准则”的元素全部移除           |
| remove_copy()    | 将“不等于某特定值”的元素全部复制到他处 |
| remove_copy_if() | 将“不满足某准则”的元素全部复制到他出   |
| unique()         | 一处毗邻的重复元素                     |
| unique_copy()    | 一处毗邻的重复元素，并复制到他处       |

#### 变序型算法（Mutating algorithms）

| 名称               | 效果                                                         |
| ------------------ | ------------------------------------------------------------ |
| reverse()          | 将元素的次序逆转                                             |
| reverse_copy()     | 复制的同时，逆转元素顺序                                     |
| rotate()           | 旋转元素顺序                                                 |
| rotate_copy()      | 复制的同时，旋转元素次序                                     |
| next_permutation() | 得到元素的下一个排列次序                                     |
| prev_permutation() | 得到元素的上一个排列次序                                     |
| shuffle()          | 将元素的次序随机打乱                                         |
| random_shuffle()   | 将元素的次序随机打乱                                         |
| partition()        | 改变元素次序，是“符合某准则”者移到前面                       |
| stable_partition() | 和partition()类似，但保持“与准则相符”和“与准则不符”之各个元素之间的相对位置 |
| partition_copy()   | 改变元素次序，使“符合某准则”者移到前面，过程中会复制元素     |

#### 排序算法（Sorting algorithms）

| 名称                | 效果                                                       |
| ------------------- | ---------------------------------------------------------- |
| sort()              | 对所有元素排序                                             |
| stable_sort()       | 对于所有元素排序，并保持相等元素之间的相对次序             |
| partial_sort()      | 排序，知道前n个元素就位                                    |
| partial_sort_copy() | 排序，直到前n个元素九尾；将结果复制于他处                  |
| nth_element()       | 根据第n个位置进行排序                                      |
| partition()         | 改变元素次序，是“符合某准则”者一道前面，过程中还会复制元素 |
| make_heap()         | 将某个区间转换成一个heap                                   |
| push_heap()         | 将元素加入一个heap                                         |
| pop_heap()          | 从heap移除一个元素                                         |
| sort_heap()         | 对heap进行排序（完成后就不再是个heap了）                   |

**检验是否排序算法**

| 名称              | 效果                                                         |
| ----------------- | ------------------------------------------------------------ |
| is_sort()         | 检验区间内的元素是否都已排序                                 |
| is_sorted_until() | 返回区间内第一个“破坏排序状态”的元素                         |
| is_partitioned()  | 检验区间内的元素是否根据某个准则被分为两组                   |
| partition_point() | 返回区间内的分割点，他把区间分割为“满足”和“不满足”某predicate的两组 |
| is_heap()         | 检验区间内的元素是否都排序成为一个heap                       |
| is_heap_until()   | 返回区间内第一个“破坏heap排序状态”的元素                     |

#### 已排序区间算法（Sorted-range algorithms）

| 名称                       | 效果                                                         |
| -------------------------- | ------------------------------------------------------------ |
| binary_search()            | 判断某区间内是否包含某个元素                                 |
| includes()                 | 判断某区间内的每一个元素是否都涵盖于另一个区间中             |
| lower_bound()              | 查找第一个“大于等于某给定值”的元素                           |
| upper_bound()              | 查找第一个“大于某给定值“的元素                               |
| equal_range()              | 返回”等于某给定值“的所有元素构成的区间                       |
| merge()                    | 将两个区间的元素合并                                         |
| set_union()                | 求两个区间的并集                                             |
| set_intersection()         | 求两个区间的交集                                             |
| set_difference()           | 求”位于第一区间“但”不位于第二区间“的所有元素，形成一个已排序区间 |
| set_symmetric_difference() | 找出”只出现两区间之一“的所有元素，形成一个已排序区间         |
| inplace_merge()            | 将两个连贯的已排序区间合并                                   |
| partition_point()          | 用一个判断式分割区间，返回分割元素                           |

#### 数值算法（Numeric algorithms）

| 名称                  | 效果                             |
| --------------------- | -------------------------------- |
| accumulate()          | 结合所有元素（求总和、求乘积）   |
| inner_product()       | 结合两区间内的所有元素           |
| adjacent_difference() | 将每个元素和其前以元素结合       |
| partial_sum()         | 将每个元素和其之前的所有元素结合 |

### std::optional 介绍

类模板 `std::optional` 管理一个*可选*的容纳值，即可以存在也可以不存在的值。

```c++
#include <string>
#include <functional>
#include <iostream>
#include <optional>

// optional 可用作可能失败的工厂返回类型
std::optional<std::string> create(bool b)
{
    if (b)
        return "Godzilla";
    else
        return {};
}

// 能用std::nullopt创建任何（空的）std::optional
auto create2(bool b)
{
    return b ? std::optional<std::string>{"Godzilla"} : std::nullopt;
}

// std::reference_wrapper可用于返回引用
auto create_ref(bool b)
{
    static std::string value = "Godzillasdfsdf";
    return b ? std::optional<std::reference_wrapper<std::string>>{value}
             : std::nullopt;
}

int main ()
{
    std::cout << "" << create(false).value_or("empty") << '\n';
    std::cout << "" << create(true).value_or("empty") << '\n';

    // 返回optional 的工厂函数可用作while和if的条件
    if (auto str = create2(true))
    {
        std::cout << "create2(true) returned" << *str << '\n';
    }

    auto falsestr = create2(false);
    if (!falsestr)
    {
        // falsestr被视为布尔值：假。且不可被输出，下面这句话编译错误
        //std::cout << "Fail to see: " << falsestr << std::endl;
    }

    if (auto str = create_ref(true))
    {
        // 用get()访问reference_wrapper 的值
        std::cout << "create_ref(true) returned" << str->get() << '\n';
        str->get() = "Mothra";
        std::cout << "modifying it changed it to " << str->get() << '\n';
    }
}
```

### STL排序算法比较

**sort()**传统上是采用quicksort算法。因此保证了很好的平均效能，复杂度为***n x log(n)***, 但最差情况下可能为n^2。如果“避免最差情况”对你是一件重要的事，你应该采用其他算法，如partial_sort()或stable_sort()。

**partial_sort()**传统上采用heapsort算法。因此它在任何情况下保证***n x log(n)***复杂度。

虽然partial_sort()拥有较好的复杂度，但sort()在多数情况下却拥有较好的运行期效能。partial_sort()的优点是它在任何时候都保证***n x log(n)***复杂度，绝不会变成二次复杂度。

partial_sort()还有一种特殊能力：如果你只需前n个元素排序，它可在完成任务后立刻停止。

**stable_sort()**传统上采用mergesort。它对所有元素进行排序。然而只有在内存充足的前提下它才有***n x log(n)***复杂度，否则其复杂度为***n x log(n) x log(n)***。stable_sort()的有点是会保持先庚元素之间的相对次序。



如果你只需要前n个排序元素，或只需要令最先或最后的n个元素（未排序）就位，可以使用nth_element()。你可以利用nth_element()将元素按照某排序准则分割成两个子集，也可以利用partition()或stable_partition()达到相同效果。三者区别如下：

+ 对于**nth_element()**, 在第一子集中指出元素个数(当然也就确定了第二子集的元素个数)例如：

  ```C++
  // move the four lowes elements to the front
  std::nth_element(coll.begin(),	// beginning of range
                   coll.begin()+3	// position between first and second part
                   coll.end()		// end of range);
  ```

  然而调用后你并不精确制导第一子集和第二子集之间有什么不同。两部分都可能包含"与第n个元素相等"的元素。

+ 对于**partition()**，你必须传入“将第一子集和第二子集区别开”的精确排序准则：

  ```c++
  // move all elements less than seven to the front
  vector<int>::iterator pos;
  pos = partition(coll1.begin(), coll1.end(),
                  [](int elem) { return elem<7;});
  ```

  调用之后你并不知道第一和第二子集内各有多少元素。返回的pos用来指出第二子集七点。第二子集的所有元素都不满足上述（被指出的）准则。

+ **stable_partition()** 的行为类似partition()，不过更具额外能力，保证两子集内的元素的相对次序维持不变。

### std::sort介绍

以升序排序范围 `[first, last)` 中的元素。不保证维持相等元素的顺序。

复杂度：O(N·log(N))* 次比较，其中N = std::distance(first, last)（C++11起）

```c++
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
 
int main()
{
    std::array<int, 10> s = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3}; 
 
    // 用默认的 operator< 排序
    std::sort(s.begin(), s.end());
    for (auto a : s) {
        std::cout << a << " ";
    }   
    std::cout << '\n';
 
    // 用标准库比较函数对象排序
    std::sort(s.begin(), s.end(), std::greater<int>());
    for (auto a : s) {
        std::cout << a << " ";
    }   
    std::cout << '\n';
 
    // 用自定义函数对象排序
    struct {
        bool operator()(int a, int b) const
        {   
            return a < b;
        }   
    } customLess;
    std::sort(s.begin(), s.end(), customLess);
    for (auto a : s) {
        std::cout << a << " ";
    }   
    std::cout << '\n';
 
    // 用 lambda 表达式排序
    std::sort(s.begin(), s.end(), [](int a, int b) {
        return b < a;   
    });
    for (auto a : s) {
        std::cout << a << " ";
    } 
    std::cout << '\n';
}
```

输出：

```
0 1 2 3 4 5 6 7 8 9 
9 8 7 6 5 4 3 2 1 0 
0 1 2 3 4 5 6 7 8 9 
9 8 7 6 5 4 3 2 1 0
```

### std::stable_sort

以升序排序范围 `[first, last)` 中的元素。保证保持等价元素的顺序。

复杂度：

应用 `cmp` *O(N·log(N)2)* 次，其中 N = std::distance(first, last) 。若额外内存可用，则复杂度为 *O(N·log(N))* 。

注意：

此函数试图分配等于待排序序列长度的临时缓冲区。若分配失败，则选择较低效的算法。

```c++
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
 
struct Employee {
    int age;
    std::string name;  // 不参与比较
};
 
bool operator<(const Employee &lhs, const Employee &rhs) {
    return lhs.age < rhs.age;
}
 
int main()
{
    std::vector<Employee> v = { 
        {108, "Zaphod"},
        {32, "Arthur"},
        {108, "Ford"},
    };  
 
    std::stable_sort(v.begin(), v.end());
 
    for (const auto &e : v) {
        std::cout << e.age << ", " << e.name << '\n';
    }   
}
```

输出：

```
32, Arthur
108, Zaphod
108, Ford
```

### std::partial_sort

重排元素，使得范围 `[first, middle)` 含有范围 `[first, last)` 中已排序的 `middle - first` 个最小元素。不保证保持相等的元素顺序。范围 `[middle, last)` 中剩余的元素顺序未指定。

复杂度：

约 *(last-first)log(middle-first)* 次应用 `cmp` 。

```c++
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
 
int main()
{
    std::array<int, 10> s{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
 
    std::partial_sort(s.begin(), s.begin() + 3, s.end());
    for (int a : s) {
        std::cout << a << " ";
    } 
}
```

可能的输出：

```
0 1 2 7 8 6 5 9 4 3
```

### std::nth_element

`nth_element` 是部分排序算法，它重排 `[first, last)` 中元素，使得：

- `nth` 所指向的元素被更改为假如 `[first, last)` 已排序则该位置会出现的元素。
- 这个新的 `nth` 元素前的所有元素小于或等于新的 `nth` 元素后的所有元素。

更正式而言， `nth_element` 以升序部分排序范围 `[first, last)` ，使得对于任何范围 `[first, nth)` 中的 `i` 和任何范围 `[nth, last)` 中的 `j` ，都满足条件 !(*j < *i) 。置于 `nth` 位置的元素则准确地是假如完全排序范围则应出现于此位置的元素。

简单的来说就是给定一个数值，比这个数值大的放该元素后面，比这个数值小的放前面。

复杂度：

平均与 std::distance(first, last) 成线性。

```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
 
int main()
{
    std::vector<int> v{5, 6, 4, 3, 2, 6, 7, 9, 3};
 
    std::nth_element(v.begin(), v.begin() + v.size()/2, v.end());
    std::cout << "The median is " << v[v.size()/2] << '\n';
 
    std::nth_element(v.begin(), v.begin()+1, v.end(), std::greater<int>());
    std::cout << "The second largest element is " << v[1] << '\n';
}
```

输出：

```
The median is 5
The second largest element is 7
```



## 编程名词和概念解释

### 容器类别

![STL Container Types](.\picture\STL Container Types.png)

#### 序列式容器（Sequence container）

这是一种有序(ordered)集合，其内每个元素均有确凿的位置----取决于插入时机和地点，与元素值无关。如果你以追加方式对一个集和置入6个元素，他们的排列次序将和置入次序一致。STL提供了5个定义好的序列式容器：array、vector、deque、list和forward_list。

#### 关联式容器(Associative container)

这是一种已排序(sorted)集合，元素位置取决于其value(或key----如果元素是个key/value pair)和给定的某个排序准则。如果将六个元素置入这样的集合中，他们的值将决定他们的次序，和插入次序无关。STL提供了4个关联式容器：set、multiset、map和multimap。

#### 无序容器（Unordered (associative) container）

这是一种无序集合(unordered collection), 其内每个元素的每个位置无关紧要，唯一重要的是某特定元素是否位于此集合内。元素值或其安插顺序，都不影响元素的位置，而且元素的位置有可能在容器生命周期中被改变。如果你放6个元素到这种集合内，它们的次序不明确，并且可能随时间而改变。STL内含4个预定义的无序容器：unordered_set、unordered_multiset、unordered_map和unordered_multimap。

+ **Sequence**容器通常被实现为array或linked list

+ **Associative**容器通常被实现为binary tree

+ **Unordered**容器通常被实现为hash table

### 各种容器使用时机

  ![ContainerSelect](.\picture\ContainerSelect.png)

+ 默认情况下应该使用vector。Vector的内部构造最简单，并允许随机访问，所以数据的访问十分方便灵活，数据的处理也够快。

+ 如果经常要在序列头部和尾部安插和一处元素，应该采用deque。如果你希望元素被移除时，容器能够自动缩减内部用量，那么也该使用deque。此外，由于vector通常采用一个内存区块来存放元素，而deque采用多个区块，所以后者可内含更多元素。

+ 如果需要经常在容器中段执行元素安插、移除和移动，可考虑使用list。List提供特殊的成员函数，可在常量时间内将元素从A容器转移到B容器。但由于list不支持随机访问，所以如果只知道list的头部却要造访list的中端元素，效能会大打折扣。和所有“以节点为基础”的容器相似，只要元素仍是容器的一部分，list就不会令只想那些元素的迭代器失效。Vector则不然，一旦超过其容量，它的所有iterator、pointer和reference失效。至于deque，当它的大小改变，所有iterator、pointer和reference都会失效。

+ 如果你要的容器对异常处理使得“每次操作若不成功便无任何作用”，那么应该选用list(但是不调用其assignment操作符和sort(), 而且如果元素比较过程中会抛出异常，就不要调用merge()、remove()、remove_if()和unique()，或选用associative/unordered容器（但不调用多元素安插动作，而且如果比较准则的复制/赋值动作可能抛出异常，就不要调用swap()或erase()）)。

+ 如果你经常需要根据某个准则查找元素，应当使用“依据该准则进行hash”的unordered set 或multiset。然而，hash容器内是无序的，所以如果你必须以来元素的次序(order),应该使用set或multiset，他们根据查找准则对元素排序。

+ 如果想处理key/value pair，请采用unordered (multi)map。如果元素次序很重要，可采用(multi)map。

+ 如果需要关联式数组(associative array), 应采用unordered map。如果元素次序很重要，可采用map。

+ 如果需要字典结构，应采用unordered multimap。如果元素次序很重要，可采用multimap。

![ContainerTypes](.\picture\ContainerTypes.png)

### 严格弱序（strict weak ordering）

关联式容器（set、multiset、map和multimap）的排序准则的定义，和std::sort的排序准则定义必须遵守严格弱序，详细描述见官方解释([strict weak ordering.pdf](./pdf/strict weak ordering.pdf))。

**严格弱序的定义**：

**简单的来说就是a<b返回true，a=b和a>b返回false。**

详细定义：

> 1. 必须是**非对称的**（antisymmetric）。
>
> ​       对`operator< `而言， 如果x < y为true， 则y < x为false。
>
> ​       对判断式(predicate) `op()`而言，如果op(x, y)为true，则op(y, x)为false。
>
> 2. 必须是**可传递的**（transitive）。
> 	
> 	对`operator< `而言，如果x < y 为true且y < z为true， 则x < z 为false。
> 	
> 	对判断式(predicate) `op()`而言，如果op(x, y)为true且op(y, z)为tru，则op(x, z)为true。
> 	
> 3. 必须是**非自反的**（irreflexive）
>
> ​       对`operator< `而言，x < x 永远是false
>
> ​       对判断式(predicate) `op()`而言，op(x, x)永远是false。
>
> 4. 必须有**等效传递性**（transitivity of equivalence）
>
>    对`operator< `而言，假如 !(a<b) && !(b<a) 为true且 !(b<c) && !(c<b) 为 true
>    那么!(a<c) && !(c<a) 也为true.
>    对判断式(predicate) `op()`而言， 假如 op(a,b), op(b,a), op(b,c), 和op(c,b) 都为
>    false, 那么op(a,c) and op(c,a) 也为false.



```c++
// 一个定义std::set<struct>的例子
#include <set>
#include <iostream>

struct ORDERING_EXAMPLE
{
    int x;
    int y;
    int z;

    /// 重载遵循严格弱序的运算符<
    bool operator < (const ORDERING_EXAMPLE& OtherStruct) const
    {
        if (this->x < OtherStruct.x)
            return true;
        if (OtherStruct.x < this->x)
            return false;

        // x == x则比较y
        if (this->y < OtherStruct.y)
            return true;
        if (OtherStruct.y < this->y)
            return false;

        // y == y则比较z
        if (this->z < OtherStruct.z)
            return true;

        return false;
    }
};

int main()
{
    std::set<ORDERING_EXAMPLE> setOrderingExample;
    
    ORDERING_EXAMPLE stOrderingExample0 = { 0, 0, 0 };
    ORDERING_EXAMPLE stOrderingExample1 = { 0, 1, 2 };
    ORDERING_EXAMPLE stOrderingExample2 = { 0, 1, 3 };
    ORDERING_EXAMPLE stOrderingExample3 = { 0, 1, 3 };
    
    setOrderingExample.insert(stOrderingExample0);
    setOrderingExample.insert(stOrderingExample1);
    setOrderingExample.insert(stOrderingExample2);
    setOrderingExample.insert(stOrderingExample3);

    return 0;
}
```

### 迭代器种类

根据能力的不同，迭代器被划分为物种不同类别。STL预先定义好的所有容器，其迭代器均属于一下三种分类：

1. **前向迭代器（Forward iterator）**	只能够以累加操作符（increment operator） 向前迭代。Class forward_list的迭代器就属此类。其他容器如unordered_set、unordered_multiset、unordered_map和unordered_multimap也都至少是此类别（但标准库其实为他们提供的是双向迭代器）

2. **双向迭代器（Bidirectional iterator）** 顾名思义它可以双向行进：以递增（increment）运算前进或以递减(decrement)运算后退。list、set、multiset、map和multimap提供的迭代器都属此类。

3. **随机访问迭代器（Random-access iterator）** 它不但具备双向迭代器的所有属性，还具备随机访问能力。更明确的说，他们提供了迭代器算数运算的必要操作符（和寻常指针的算数运算完全对应）。你可以对迭代器增加或减少一个偏移量、计算两迭代器间的距离，或使用`<`和`>`之类的relational（相对关系）操作符进行比较。vector、deque、array和string提供的迭代器都属此类。

除此之外，STL还定义了两个类别：

+ **输入型迭代器（Input iterator）** 向前迭代时能够读取/处理value。Input stream迭代器就是这样的例子。
+ **输出型迭代器（Output iterator）** 向前迭代时能够涂写value。Inserter和output stream迭代器都属此类。

