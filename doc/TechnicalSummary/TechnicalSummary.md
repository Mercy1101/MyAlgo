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

### 排序算法比较

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

