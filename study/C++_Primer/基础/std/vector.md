<!--
author: lumos
date: 2020-12-11
title:  
tags: 
category: 
status: draft
summary: 
-->


# 标准库类型vector
```cpp
#include <vector>
using std::vector;
```
- 对象的集合，其中所有的对象类型都相同，对象拥有索引
- 是类模板

##### 初始化方式
| 方式                      | 说明                           |
| :------------------------ | :---------------------------- |
| vector<T> v1              | 默认初始化，空vector            |
| vector<T> v2(v1)          | 包含v1所有元素的副本            |
| vector<T> v3 = v1         | 包含v1所有元素的副本            |
| vector<T> v4(n)           | 包含n个重复执行了默认初始化的对象 |
| vector<T> v5(n, val)      | 包含n个元素，每个元素的值都是val |
| vector<T> v6{a,b,c...}    | 包含列表内的元素                |
| vector<T> v7 = {a,b,c...} | 包含列表内的元素                |

##### 对vector的写入操作
| 方式               | 说明                                                                                       |
| :---------------- | :----------------------------------------------------------------------------------------- |
| v.push_back(t)    | 向v的末端添加t元素，优先使用移动构造，其次拷贝构造                                            |
| v.emplace_back(t) | 就地构造                                                                                   |
| v.push_back(t)    | 向v的末端添加t元素                                                                          |
| v.resize(ns)      | ns < size:析构多余元素；ns> size:默认构造缺失元素；ns >capacity：触发重分配，默认构造缺失元素 |
| v.reserve(ns)     | ns > capacity，触发重分配                                                                   |
| v.clear()         | 等价于resize(0)                                                                            |
| v.shrink_to_fit() | size < capacity:触发重分配                                                                  |
| v.swap(v&)        | 交换内存首指针                                                                              |
| v1 = v2           | 用v2中元素的拷贝替换v1中的元素                                                              |
| v1 = {a,b,c...}   | 用列表中的元素替换v1中的元素                                                                |

##### 批量插入（insert）
| 方式                          | 说明                                          |
| :---------------------------- | :-------------------------------------------- |
| v.insert(pos, val)           | 在pos前插入1个val                              |
| v.insert(pos, n, val)        | 在pos前插入n个val                              |
| v.insert(pos, first, last)   | 在pos前插入迭代器区间[first,last)的元素         |
| v.insert(pos, {a,b,c})       | 在pos前插入初始化列表中的元素                  |

> insert可能触发重分配，并使插入点及其后的迭代器、引用、指针失效

##### 批量赋值（assign）
| 方式                          | 说明                                          |
| :---------------------------- | :-------------------------------------------- |
| v.assign(n, val)             | 将v替换为n个val（覆盖原有全部元素）             |
| v.assign(first, last)        | 将v替换为迭代器区间[first,last)的元素           |
| v.assign({a,b,c})            | 将v替换为初始化列表中的元素                    |

> assign会销毁原有元素并重新构造新元素；若新元素个数超过capacity可能触发重分配，从而使所有迭代器、引用、指针失效

>重分配总是会先拷贝构造元素到新内存，然后再析构原有内存的元素

##### 对vector的读取操作
| 方式            | 说明                                                      |
| :-------------- | :-------------------------------------------------------- |
| v.empty()       | 如果v为空，返回真，否则返回假                                |
| v.size()        | 返回v中的元素个数，size_type类型                                            |
| v[n]            | 返回v中第n个位置上元素的引用，从0计，n是size_type类型         |
| v1 == v2        | 元素完全相同且位置一致时返回真                               |
| v1 != v2        | 元素完全相同且位置一致时返回假，否则返回真                     |
| <, <=, >, >=    | 由第一对相异的元素值的大小决定（位置对应，长度不同时，较长更大） |
>注：只有在T可比较的情况下，vector<T>才可比较

##### 迭代器本质
- `vector<T>::iterator`通常为指针或轻量包装类（连续迭代器）
- 具体类型不保证是指针，但可做随机访问与指针算术

##### range-for迭代
拷贝方式，不可赋值
```cpp
for(auto i : v)			
	cout << i <<endl;
```
引用方式，可赋值
```cpp
for(auto &i : v)			//循环变量定义为引用
	cout << i <<endl;
```
##### vector截取
```cpp
vector<int>::const_iterator fist1 = Arrs.begin() + 2; // 第三个迭代器
vector<int>::const_iterator last1 = Arrs.end() - 1; // 倒数第二个迭代器
vector<int> SubArrs(first1, last1); // 创建整型的容器，并初始化为Arrs的第三个到第八个
```