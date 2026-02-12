<!--
author: lumos
date: 2020-12-11
title:  
tags: 
category: 
status: draft
summary: 
-->


# const
- 被const修饰的变量，一但创建（编译后运行时）后**无法改变**
- const变量必须初始化
- 除了`引用`和`指针`外, const修饰的位置无影响
```cpp
const int a = 42;				//运行时初始化
int const a = 42;               //const位置无影响
const int b = get_size();		//编译时初始化
const int c;					//错误
```
- const默认仅在文件内有效
- **顶层const**：被修饰的变量本身无法改变
- **底层const**：其他的通过指针或引用等间接途径来限制目标对象内容不可变
##### const和引用
引用是复合类型, 故有两种const修饰位置
   * `&前面绑定的类型`遵循简单类型的const修饰规则, 故位置无影响
   * `&后面的引用`因引用不是对象, 故无法修饰
```cpp
const int &a = 42; //正确,修饰绑定的类型
int const &a = 42; //正确,修饰绑定的类型
int & const a = 42; //错误,引用无法用const修饰
```

##### const和指针

对于指针变量来说，有两个问题
 - 指针是const：顶层const
 - 指针指向的对象被当作const看待：底层const
```cpp
const int a = 0;
const int *const p = &a; //左边底层const，右边顶层const
```
>注：const指针可指向非const对象，非const指针不能指向const对象
```cpp
int n = 0;
const int *p = &n; //正确
```

##### constexpr
显式声明常量表达式，并且其初始值也必须为常量表达式（编译器验证）
```cpp
constexpr int a = 0;		//正确，因为0是常量表达式，a被声明为常量表达式
constexpr int b = a + 1;	//正确，因为a + 1是常量表达式，b被声明为常量表达式
constexpr int c = size();	//错误, size()不是常量表达式
```
在定义指针时，由以下效果
```cpp
int n; 								//必须定义在函数外，因为编译时，函数体内&n不确定
constexpr int *p1 = &n;				//正确，因为&n是常量表达式，p1被声明为指向int的常量表达式指针
constexpr const int *p2 = &n;		//正确，因为&n是常量表达式，p2被声明为指向const int（不可通过*赋值）的常量表达式指针
constexpr const int *const p2 = &n;	//正确，因为&n是常量表达式，p2被声明为指向const int（不可通过*赋值）的常量表达式指针（constexpr符合const的要求）
```