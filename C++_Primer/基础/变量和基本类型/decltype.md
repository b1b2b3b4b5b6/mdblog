# decltype
类型指示符，通过expr表达式声明avl的类型
```C++
decltype(expr) avl;	//avl的类型是expr的类型
```
###### decltype不会忽略顶层const
```c++
const int *const p = nullptr;
decltype(p) x = nullptr;		//x的类型是const int *const
```
##### 根据expr的具体情况，有两种情况
- 一个对象：返回对象的类型
```C++
const int &cr = 0;
const int *p = nullptr;
decltype(cr) x = 0;		//x的类型是const int&
decltype(*p) y = 0;		//y的类型是const int&，因为*p是指向对象的引用
```
>注：decltype中处的引用是个例外，不作为别名出现

如果变量被()包围，如decltype((n))，那返回的结果总是引用

```C++
int n;
decltype((n)) x = 0;	//x的类型是int &，必须进行初始化
```

- 不是一个对象：返回表达式的结果对应类型
```c++
const int &r = 0;
int *p = nullptr;
decltype(r + 0) x = 0;	//加法的结果是int，所以x的类型是int
decltype(*p) y = x;		//y的类型是int&，绑定到x对象
```