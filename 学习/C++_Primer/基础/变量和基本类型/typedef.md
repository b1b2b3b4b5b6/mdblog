<!--
author: lumos
date: 2020-12-11
title:  
tags: 
category: 
status: draft
summary: 
-->


# typedef
类型别名
```C++
typedef int (*fop)(int);	//定义函数指针类型别名
typedef int *foo(int);		//定义函数类型别名

fop p;						//声明并定义函数指针p
int a = (*p)(0);			//通过*p调用函数

foo f;						//声明并定义函数f
int *a = f(0);				//调用f函数
```