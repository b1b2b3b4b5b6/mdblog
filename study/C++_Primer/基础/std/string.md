<!--
author: lumos
date: 2020-12-11
title:  
tags: 
category: 
status: draft
summary: 
-->


# string 
```C++
#include <string.h>
using std::string;
```
##### 初始化方式
| 方式                 | 说明                        |
| :------------------- | :-------------------------- |
| string s1            | 默认初始化，s1是空串          |
| string s2(s1)        | 直接初始化                   |
| string s3("value")   | 直接初始化，不包括结束字符     |
| string s4 = (n, 'c') | 直接初始化，连续n个字符c组成串 |
| string s5 = s1       | 拷贝初始化                   |
| string s6 = "value"  | 拷贝初始化                   |

##### 对string的操作
| 方式           | 说明                                                                                 |
| :------------- | :---------------------------------------------------------------------------------- |
| os<<s          | 将s写到输出流os中，返回os                                                             |
| is>>s          | 从is中读取字符串赋给s，忽略起始空白，到空白为止，返回is                                   |
| getline(is, s) | 从is中读取一行赋给s，保留空白，直到换行，不保存换行（被getline丢弃），返回is               |
| s.empty()      | s为空返回true，否则为false                                                            |
| s.size()       | 返回s中字符的个数，类型为string::size_type,无符号整型                                   |
| s.substr(start, len)       | 返回从start开始，共len个字符的string                                   |
| s[n]           | 返回s中第n个字符的引用，n为string::size_type类型，位置n从0计起，返回该位置字符的引用       |
| s1+s2          | 返回s1和s2连接后的结果                                                                |
| s1=s2          | 用s2的副本代替s1中原来的字符                                                           |
| s1==s2         | 完全一样返回真，大小写敏感                                                             |
| s1!=s2         | 不完全一样返回真，大小写敏感                                                           |
| <, <=, >, >=   | 利用字符在字典中的顺序（ascii码大小）进行比较，大小写敏感。（位置对应，长度不同时，较长更大） |
##### range-for迭代
拷贝方式，不可赋值
```C++
for(auto c : str)			
	cout << c <<endl;
```
引用方式，可赋值
```C++
for(auto &c : str)			//循环变量定义为引用
	cout << c <<endl;
```
