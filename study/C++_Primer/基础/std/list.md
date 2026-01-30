<!--
author: lumos
date: 2026-01-30
title:  
tags: 
category: 
status: draft
summary: 
-->


# 标准库类型list
```C++
#include <list>
using std::list;
```
- 双向链表，支持在任意位置高效插入/删除
- 不支持随机访问，只能顺序遍历
- 是类模板

##### 初始化方式
| 方式                      | 说明                           |
| :------------------------ | :---------------------------- |
| list<T> l1                | 默认初始化，空list             |
| list<T> l2(l1)            | 包含l1所有元素的副本            |
| list<T> l3 = l1           | 包含l1所有元素的副本            |
| list<T> l4(n)             | 包含n个默认初始化的元素          |
| list<T> l5(n, val)        | 包含n个值为val的元素             |
| list<T> l6{a,b,c...}      | 包含列表内的元素                |
| list<T> l7 = {a,b,c...}   | 包含列表内的元素                |

##### 对list的写入操作
| 方式                        | 说明                                          |
| :-------------------------- | :-------------------------------------------- |
| l.push_back(t)              | 向尾部添加元素                                 |
| l.push_front(t)             | 向头部添加元素                                 |
| l.emplace_back(args...)     | 尾部就地构造                                   |
| l.emplace_front(args...)    | 头部就地构造                                   |
| l.insert(pos, t)            | 在pos前插入元素                                |
| l.insert(pos, n, t)         | 在pos前插入n个元素                             |
| l.insert(pos, first, last)  | 在pos前插入区间[first,last)                    |
| l.erase(pos)                | 删除pos指向的元素                              |
| l.erase(first, last)        | 删除区间[first,last)                           |
| l.remove(val)               | 删除所有等于val的元素                          |
| l.remove_if(pred)           | 删除满足谓词的元素                             |
| l.unique()                  | 删除连续重复元素（可带谓词）                   |
| l.splice(pos, l2)           | 将l2全部元素转移到pos前                         |
| l.merge(l2)                 | 合并两个已排序的list                           |
| l.sort()                    | 排序（可带比较器）                             |
| l.reverse()                 | 反转元素顺序                                   |
| l.resize(ns)                | 调整大小，多出元素被默认构造或删除              |
| l.clear()                   | 删除所有元素                                   |
| l.swap(l2)                  | 交换两个list                                   |

##### 对list的读取操作
| 方式            | 说明                                                      |
| :-------------- | :-------------------------------------------------------- |
| l.empty()       | 如果l为空，返回真，否则返回假                               |
| l.size()        | 返回l中的元素个数                                         |
| l.front()       | 返回首元素的引用                                          |
| l.back()        | 返回尾元素的引用                                          |
| l1 == l2        | 元素完全相同且位置一致时返回真                               |
| l1 != l2        | 元素完全相同且位置一致时返回假，否则返回真                   |
| <, <=, >, >=    | 由第一对相异的元素值的大小决定（位置对应）                  |

>注：只有在T可比较的情况下，list<T>才可比较

##### 迭代器失效规则
- 插入不会使已有迭代器、引用、指针失效
- 仅被删除元素的迭代器、引用、指针失效

##### range-for迭代
拷贝方式，不可赋值
```C++
for(auto i : l)
	cout << i << endl;
```
引用方式，可赋值
```C++
for(auto &i : l)
	cout << i << endl;
```

##### 使用示例
```C++
list<int> l = {3, 1, 2, 2, 4};

l.push_front(0);            // {0,3,1,2,2,4}
l.push_back(5);             // {0,3,1,2,2,4,5}

l.remove(2);                // 删除所有2
l.sort();                   // {0,1,3,4,5}

list<int> l2 = {6, 7};
l.splice(l.begin(), l2);    // l2变空

for(const auto &v : l) {
    cout << v << " ";
}
```

##### list vs vector
| 特性          | list                     | vector                    |
| :----------- | :------------------------ | :------------------------ |
| 随机访问      | 不支持                    | 支持                       |
| 任意位置插入/删除 | O(1)（已知位置）       | O(n)                      |
| 迭代器稳定性  | 插入稳定，删除局部失效     | 插入/扩容可能失效           |
| 内存布局      | 非连续                    | 连续                       |
| 适用场景      | 频繁插入删除、无需随机访问 | 需要随机访问和局部性         |
