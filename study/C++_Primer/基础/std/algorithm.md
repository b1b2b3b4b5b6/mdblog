<!--
author: lumos
date: 2026-01-23
title:  
tags: 
category: 
status: draft
summary: 
-->


# 常用算法（accumulate/min_element/max_element/distance）
```cpp
#include <numeric>    // accumulate
#include <algorithm>  // min_element/max_element
#include <iterator>   // distance
```

## std::accumulate
```cpp
using std::accumulate;
```
- 对区间[first, last)做“累加/折叠”
- 默认使用`operator+`，也可传入自定义二元操作

##### 基本用法
| 方式 | 说明 |
| :-- | :-- |
| accumulate(first, last, init) | 返回`init + *first + ...` |
| accumulate(first, last, init, op) | 返回`op(op(init, *first), *second)...` |

##### 返回值与类型
- 返回类型由`init`决定（模板参数`T`）
- `init`决定计算精度/溢出风险

##### 复杂度
- $O(n)$，n为区间长度

##### 常见用法
**求和**
```cpp
vector<int> v{1,2,3};
int s = accumulate(v.begin(), v.end(), 0); // 6
```

**避免整型溢出**
```cpp
long long s = accumulate(v.begin(), v.end(), 0LL);
```

**自定义操作（乘积）**
```cpp
int prod = accumulate(v.begin(), v.end(), 1, [](int a, int b){ return a * b; });
```

##### 注意事项
- `op`不满足结合性时结果与顺序相关
- 浮点累加会有误差，必要时用更高精度类型作为`init`


## std::min_element
```cpp
using std::min_element;
```
- 在区间[first, last)中找最小元素
- 若有多个最小，返回第一个

##### 基本用法
| 方式 | 说明 |
| :-- | :-- |
| min_element(first, last) | 使用`operator<`比较 |
| min_element(first, last, comp) | 使用自定义比较器`comp(a,b)` |

##### 返回值
- 区间非空：返回最小元素迭代器
- 区间为空：返回`last`

##### 复杂度
- $O(n)$，比较次数约`n-1`

##### 示例
```cpp
vector<int> v{3,1,2};
auto it = min_element(v.begin(), v.end());
if (it != v.end()) cout << *it; // 1
```


## std::max_element
```cpp
using std::max_element;
```
- 在区间[first, last)中找最大元素
- 若有多个最大，返回第一个

##### 基本用法
| 方式 | 说明 |
| :-- | :-- |
| max_element(first, last) | 使用`operator<`比较 |
| max_element(first, last, comp) | 使用自定义比较器`comp(a,b)` |

##### 返回值
- 区间非空：返回最大元素迭代器
- 区间为空：返回`last`

##### 复杂度
- $O(n)$，比较次数约`n-1`

##### 示例
```cpp
vector<string> v{"a", "abcd", "bb"};
auto it = max_element(v.begin(), v.end(), [](const string& a, const string& b){
    return a.size() < b.size();
});
```

##### 注意事项
- `comp`需要满足严格弱序（strict weak ordering）
- 同时求最小和最大可用`std::minmax_element`


## std::distance
```cpp
using std::distance;
```
- 计算两个迭代器之间的“步数距离”
- 结果类型：`iterator_traits<It>::difference_type`（有符号）

##### 基本用法
| 方式 | 说明 |
| :-- | :-- |
| distance(first, last) | 返回从first走到last需要`++`多少次 |

##### 复杂度
- RandomAccessIterator：$O(1)$（等价于`last - first`）
- Input/Forward/Bidirectional：$O(n)$

##### 示例
```cpp
vector<int> v{1,2,3,4};
auto it = find(v.begin(), v.end(), 3);
if (it != v.end()) {
    auto idx = distance(v.begin(), it); // 2
}
```

##### 注意事项
- 一般要求`last`可从`first`到达
- RandomAccessIterator可直接`it2 - it1`，但`distance`对所有迭代器统一接口
