<!--
author: lumos
date: 2020-12-11
title:  
tags: 
category: 
status: draft
summary: 
-->

# auto
类型说明符，通过初始值来推算并声明和初始化对象
```c++
auto n = 3.14 + 1;	//n的类型是double，初始值为4.14
```
auto一般会忽略顶层const，保留底层const
```C++
const int ci = 0;
const int &cr = ci;
auto a = ci;  //int
auto b = cr;  //int，引用视作别名
auto c = &ci; //指向const int（保留底层const）的 int *
```

auto的类型推导作用和模版函数中的类型推导是等价的
