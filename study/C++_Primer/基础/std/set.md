<!--
author: lumos
date: 2020-12-11
title:  
tags: 
category: 
status: draft
summary: 
-->


# 标准库类型set/multiset
```C++
#include <set>
using std::set;
using std::multiset;
```
- set: 有序集合，元素唯一，基于红黑树实现
- multiset: 有序集合，允许重复元素
- 元素自动按键值排序
- 是类模板

##### 初始化方式
| 方式                       | 说明                          |
| :------------------------- | :---------------------------- |
| set<T> s1                  | 默认初始化，空set              |
| set<T> s2(s1)              | 包含s1所有元素的副本           |
| set<T> s3 = s1             | 包含s1所有元素的副本           |
| set<T> s4{a,b,c...}        | 包含列表内的元素               |
| set<T> s5 = {a,b,c...}     | 包含列表内的元素               |
| set<T, Cmp> s6             | 使用自定义比较器Cmp           |
| multiset<T> ms1            | 允许重复元素的集合             |

##### 对set的写入操作
| 方式                    | 说明                                      |
| :---------------------- | :---------------------------------------- |
| s.insert(t)             | 插入元素t，返回pair<iterator, bool>        |
| s.insert(first, last)   | 插入迭代器范围内的元素                     |
| s.emplace(args...)      | 就地构造元素                              |
| s.erase(t)              | 删除值为t的元素，返回删除的元素个数         |
| s.erase(iter)           | 删除迭代器指向的元素，返回下一个迭代器      |
| s.erase(first, last)    | 删除迭代器范围内的元素                     |
| s.clear()               | 删除所有元素                              |
| s.swap(s2)              | 交换两个set的内容                         |
| s1 = s2                 | 用s2中元素的拷贝替换s1中的元素             |

##### 对set的读取操作
| 方式              | 说明                                        |
| :--------------- | :----------------------------------------- |
| s.empty()        | 如果s为空，返回真，否则返回假                |
| s.size()         | 返回s中的元素个数                           |
| s.find(t)        | 查找值为t的元素，返回迭代器，未找到返回end() |
| s.count(t)       | 返回值为t的元素个数（set为0或1）             |
| s.lower_bound(t) | 返回第一个>=t的元素的迭代器                |
| s.upper_bound(t) | 返回第一个>t的元素的迭代器                   |
| s.equal_range(t) | 返回pair，包含lower_bound和upper_bound      |
| s1 == s2         | 元素完全相同时返回真                        |
| s1 != s2         | 元素不完全相同时返回真                       |
| <, <=, >, >=     | 字典序比较                                  |

##### 迭代器本质
- `set<T>::iterator`为类类型（非指针）
- 迭代器是**双向迭代器**，不支持随机访问

##### range-for迭代
拷贝方式（推荐，set元素不可修改）
```C++
for(auto i : s)			
	cout << i << endl;
```
引用方式（只读）
```C++
for(const auto &i : s)			
	cout << i << endl;
```

##### 自定义比较器
```C++
// 方法1: 函数对象
struct cmp {
    bool operator()(int a, int b) const {
        return a > b;  // 降序
    }
};
set<int, cmp> s;

// 方法2: lambda表达式（C++11）
auto cmp = [](int a, int b) { return a > b; };
set<int, decltype(cmp)> s(cmp);
```

##### multiset特性
```C++
multiset<int> ms{1, 2, 2, 3, 3, 3};
ms.count(3);           // 返回3
ms.erase(3);           // 删除所有值为3的元素
ms.erase(ms.find(3));  // 只删除一个值为3的元素
```

##### 常用操作示例
```C++
set<int> s = {3, 1, 4, 1, 5};  // 自动去重排序: {1, 3, 4, 5}

// 插入
auto [iter, success] = s.insert(2);  // C++17
if (success) cout << "插入成功" << endl;

// 查找
if (s.find(3) != s.end()) {
    cout << "找到元素3" << endl;
}

// 获取最小和最大元素
int minVal = *s.begin();
int maxVal = *s.rbegin();
```
