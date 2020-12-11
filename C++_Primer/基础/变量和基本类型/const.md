# const
- 被const修饰的变量，一但创建（编译后运行时）后**无法改变**
- const变量必须初始化
```C++
 const int a = 42;				//运行时初始化
 const int b = get_size();		//编译时初始化
 const int c;					//错误
```
- const默认仅在文件内有效
- **顶层const**：被修饰的变量本身无法改变
- **底层const**：其他的通过指针或引用等间接途径来限制目标对象内容不可变
##### const和引用
正常情况下，引用的类型必须和其所引用对象的类型一致，但对const的引用是例外
```C++
int i = 42;
const int &r1 = i;			//cosnt int 绑定了int对象上
const int &r2 = 3.14;		//正确，绑定了临时对象3
const int &r2 = r1 * 2;		//正确
```
>注：const引用可绑定非const对象，非const引用不能绑定const对象
```C++
int n = 0;
const int &p = n; //正确
```

##### const和指针

对于指针变量来说，有两个问题
 - 指针是const：顶层const
 - 指针指向的对象被当作const看待：底层const
```c++
const int a = 0;
const int *const p = &a; //左边顶层const，右边底层const
```
>注：const指针可指向非const对象，非const指针不能指向const对象
```C++
int n = 0;
const int *p = &n; //正确
```

##### constexpr
显式声明常量表达式，并且其初始值也必须为常量表达式（编译器验证）
```C++
constexpr int a = 0;		//正确，因为0是常量表达式，a被声明为常量表达式
constexpr int b = a + 1;	//正确，因为a + 1是常量表达式，b被声明为常量表达式
constexpr int c = size();	//错误, size()不是常量表达式
```
在定义指针时，由以下效果
```C++
int n; 								//必须定义在函数外，因为编译时，函数体内&n不确定
constexpr int *p1 = &n;				//正确，因为&n是常量表达式，p1被声明为指向int的常量表达式指针
constexpr const int *p2 = &n;		//正确，因为&n是常量表达式，p2被声明为指向const int（不可通过*赋值）的常量表达式指针
constexpr const int *const p2 = &n;	//正确，因为&n是常量表达式，p2被声明为指向const int（不可通过*赋值）的常量表达式指针（constexpr符合const的要求）
```