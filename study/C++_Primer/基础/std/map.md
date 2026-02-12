<!--
author: lumos
date: 2020-12-11
title:  
tags: 
category: 
status: draft
summary: 
-->


# 标准库类型map
```cpp
#include <map>
using std::map;
```
- 关联式容器
- 有序无重复
- 红黑树

### 初始化方式
| 方式                  | 说明                       |
| :------------------------ | :---------------------------- |
| map<Key, value> m; | 默认初始化，空map         |
| map<key, value> m(iterator first, iterator last); | 用迭代器[frist, last)范围内的元素初始化map |
| map<key, value> m(cosnt map& m2); | 用m2构造m |
| map<key, value> m{initializer_list<value_type> il}：<br />map<key, value> m{{key k1, value v1},{k2, v2}, …}<br />map<key, value> m{make_pair(key k1, value v1), make_pair(k2, v2), …}<br />map<key, value> m{pair<key, value>(k1, v1), pair<key, value>(k2, v2), …} | 通过初始化列表构造<br /> |

### 迭代器

| 迭代器            | 说明      |
| ----------------- | --------- |
| begin() end()     | 正序      |
| rbegin() rend()   | 逆序      |
| cbegin() cend()   | 正序const |
| crbegin() crend() | 逆序const |

> 支持`++`，`--`操作, 不支持`+i`操作
>
> 通过`->`来访问`first`和`second`
>
> 对`iter`解引用会得到`pair`的引用

##### 迭代器本质
- `map<Key, T>::iterator`为类类型（非指针）
- 迭代器是**双向迭代器**，不支持随机访问



### 成员函数

| 函数                     | 说明                                                 |
| ------------------------ | ---------------------------------------------------- |
| empty()                  | 是否为空                                             |
| size()                   | 元素的个数                                           |
| max_size()               | map可能包含的最大元素个数, 不保证一定能达到          |
| find(const key_type &k)  | 查找key为k的键值对, 返回迭代器, 不存在则返回map::end |
| count(const key_type &k) | 统计key为k的键值对个数, 返回值不是1就是0             |

### 元素访问

| 方式  | 说明                                                         |
| ----- | ------------------------------------------------------------ |
| [k]   | 返回map 中 Key为 k的元素的value的引用。<br />如果 k不存在，那么将会插入一个 key为 k的元素，并返回其默认 value。 |
| at(k) | 返回map中Key为k的元素的value的引用。<br />如果k不存在，抛一个 out_of_range 异常 |

### 修改

| 方式                                                         | 说明                                     |
| ------------------------------------------------------------ | ---------------------------------------- |
| pair<iterator, bool> insert(const pair<key, value> &val);    | 插入键值对                               |
| iterator insert(const_iterator pos, const pair<key, value> &val); | 指定位置位置插入键值对(仍然会排序)       |
| void insert(iterator first, iterator second);                | 范围插入键值对                           |
| iterator erase(const_iterator position);                     | 删除迭代器指向的内容, 函数 返回 nullptr  |
| iterator erase(const_iterator first, const_iterator last);   | 删除迭代器范 围内的内容，函数返回nullptr |
| swap()                                                       | 交换map对象内存                          |
| clear()                                                      | 清空容器所有元素                         |



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
##### map截取
```cpp
map<int>::const_iterator fist1 = Arrs.begin(); // 第三个迭代器
map<int>::const_iterator last1 = Arrs.end() - 1; // 倒数第二个迭代器
vector<int> SubArrs(first1, last1); // 创建整型的容器
```