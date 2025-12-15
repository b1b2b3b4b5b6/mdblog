<!--
author: lumos
date: 2020-12-11
title:  
tags: 
category: 
status: draft
summary: 
-->


# 概念
##### 作用域操作符 ::
编译器应从操作符左侧名字所示的作用域中寻找右侧那个名字
```C++
std::cin	//使用命名空间std中的名字cin
```
一般情况下，编译器可以知道右侧名字是类型还是变量，但访问模板参数作用域的名字时，会假定不是类型。当需要访问类型时，必须通过typname来显式告知编译器该名字是一个类型
```C++
T::size_type * p;//这句是定义名为p的变量还是size_type和p相乘？
```
##### using声明
声明下面语句
```c++
using namespace::name;	
```
就可以直接访问命名空间`namespace`中的名字`name`
>每个名字都需要独立的的using声明
>头文件一般不使用using声明