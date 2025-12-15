# lambda

## 表达式语法

```C++
[捕获列表] (形参列表) mutable 异常列表-> 返回类型
{
    函数体
}
```

- **捕获列表**：捕获外部变量，捕获的变量可以在函数体中使用，可以省略，即不捕获外部变量。
- **形参列表**：和普通函数的形参列表一样。可省略，即无参数列表
- **mutable**：mutable 关键字，如果有，则表示在函数体中可以修改捕获变量，根据具体需求决定是否需要省略。
- **异常列表**：noexcept / throw(...),和普通函数的异常列表一样，可省略，即代表可能抛出任何类型的异常。
- **返回类型**：和函数的返回类型一样。可省略，如省略，编译器将自动推导返回类型。
- **函数体**：代码实现。可省略，但是没意义。



## 特性

- 实现闭包, 即显示明确**引用环境**
- 每处lambda表达式都会定义一个**独特**的lambda类型, 故已定义的lambda类型无法再赋值
- 全局定义的lambda无法**捕获任何对象**
- 全局定义的lambda**不导出符号**



## 编译器实现原理

编译器实现 **lambda 表达式**大致分为一下几个步骤

1. 创建 **lambda 匿名类**，实现构造函数，使用 lambda 表达式的函数体重载 **operator()**（所以 lambda 表达式 也叫匿名函数对象）
2. 创建 lambda 对象
3. 通过对象调用 **operator()**

编译器将 lambda 表达式翻译后的代码：

```cpp
class lambda_xxxx
{
private:
    int a;
public:
    lambda_xxxx(int _a) :a(_a){}
    lambda_xxxx(const lambda_xxxx &) noexcept = default;
    lambda_xxxx(lambda_xxxx &&) noexcept = default;
    bool operator()(int x, int y) throw() {}
};
void LambdaDemo()
{
    int a = 1;
    lambda_xxxx lambda = lambda_xxxx(a);
    bool ret = lambda.operator()(3, 4);
}

```

> lamda的重载的`()`成员函数导出为 `text`段的`local`类型符号, 符号名称按一定规则变化递增
>
> lamda匿名类可以进行拷贝构造和移动构造

## lambda_xxxx 与 lambda 表达式 的对应关系

- **捕获列表**，对应  **private 成员**
- **形参列表**，对应 成员函数 **operator() 的形参列表**
- **mutable**，对应 成员函数 **operator() 的常属性 const**，即是否是 **常成员函数**
- **返回类型**，对应 成员函数 **operator() 的返回类型**
- **函数体**，对应 成员函数 **operator() 的函数体**



lambda 表达 捕获列表的捕获方式，也影响 对应 lambda_xxxx 类的 private 成员 的类型

- **值捕获**：private 成员 的类型与捕获变量的类型一致
- **引用捕获**：private 成员 的类型是捕获变量的引用类型



## 到函数指针的转换

如果 lambda 表达式不**捕获任何变量**，则可以转换为  **函数指针**, 如下

```cpp
void (*p)() = []() {};
```

#### 实现原理

- 在 lambda_xxxx 类中生成一个**静态函数**，静态函数的函数签名**与 operator() 一致**，在这个静态函数中，通过一个**空指针**去调用该类的 operator()
- 在 lambda_xxxx 重载与**函数指针**的**类型转换操作符**，在这个函数中，返回第 1 步中静态函数的地址。

实现代码如下

```cpp
class lambda_xxxx 
{
private:
	//没有捕获任何外部变量，所有没有成员
public:
        /*...省略其他代码...*/
	int operator()(){}
	static int _stdcall lambda_invoker_stdcall(int i)
	{
		return ((lambda_xxxx *)nullptr)->operator()(i);
	}

	operator void(*)() const
	{
		return &lambda_invoker_stdcall;
	}
};

```

> lamda的重载的`类型转换运算符`成员函数导出为 `text`段的`local`类型符号

