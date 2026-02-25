<!--
author: lumos
date: 2026-02-25
title:  
tags: 
category: 
status: draft
summary: 
-->


# std::bind
```cpp
#include <functional>
using std::bind;
using namespace std::placeholders;  // _1, _2, _3...
```
- 函数适配器，用于绑定函数参数
- 返回一个可调用对象，将部分参数固定
- C++11引入，C++14后建议优先使用lambda

## 基本用法

##### 语法
```cpp
auto f = bind(callable, arg1, arg2, ...);
```
- `callable`: 可调用对象（函数、函数指针、成员函数、lambda等）
- `arg`: 参数值或占位符（`_1`, `_2`, `_3`...）
- 占位符表示新函数的第n个参数

##### 常见场景
| 场景 | 说明 |
| :-- | :-- |
| 参数绑定 | 固定某些参数值 |
| 参数重排 | 改变参数顺序 |
| 成员函数绑定 | 绑定类成员函数到对象 |
| 配合算法 | 将多参数函数适配为单参数 |

## 使用示例

### 1. 固定参数值
```cpp
int add(int a, int b) { return a + b; }

auto add5 = bind(add, 5, _1);    // 固定第一个参数为5
cout << add5(10);                 // 输出15，相当于add(5, 10)
```

### 2. 参数重排
```cpp
int divide(int a, int b) { return a / b; }

auto flip = bind(divide, _2, _1);  // 交换参数顺序
cout << flip(2, 10);               // 输出5，相当于divide(10, 2)
```

### 3. 绑定成员函数
```cpp
struct Foo {
    void print(int x) { cout << x << endl; }
};

Foo obj;
// 第一个参数必须是对象指针或引用
auto f = bind(&Foo::print, &obj, _1);
f(42);  // 调用obj.print(42)
```

**使用ref/cref绑定引用**
```cpp
Foo obj;
// 避免拷贝对象，使用std::ref
auto f = bind(&Foo::print, std::ref(obj), _1);
```

### 4. 配合标准算法
```cpp
bool less_than(int a, int b) { return a < b; }

vector<int> vec{3, 1, 4, 1, 5};
// 查找第一个大于3的元素
auto it = find_if(vec.begin(), vec.end(), 
                  bind(less_than, 3, _1));
```

### 5. 嵌套bind
```cpp
int add3(int a, int b, int c) { return a + b + c; }

auto f = bind(add3, _1, _2, bind(add, _1, 5));
// f(x, y) = add3(x, y, add(x, 5))
cout << f(10, 20);  // 输出45，即10 + 20 + (10+5)
```

## 引用传递

##### 默认行为
- bind默认**拷贝**所有参数
- 如需引用语义，使用`std::ref`或`std::cref`

```cpp
void increment(int& x) { x++; }

int n = 0;
auto f1 = bind(increment, n);      // ❌ 拷贝n
f1();                               // n仍为0

auto f2 = bind(increment, ref(n)); // ✅ 引用n
f2();                               // n变为1
```

## 与lambda对比

| 特性 | bind | lambda |
| :-- | :-- | :-- |
| 可读性 | 较差（占位符不直观） | 优秀（语义清晰） |
| 性能 | 可能有额外开销 | 通常更优 |
| 灵活性 | 有限 | 极高 |
| 适用场景 | 简单适配 | 复杂逻辑 |

##### 推荐做法
```cpp
// ❌ 不推荐：bind可读性差
auto f1 = bind(add, 5, _1);

// ✅ 推荐：lambda更清晰
auto f2 = [](int x) { return add(5, x); };
```

## 注意事项

1. **返回类型推导**
   - bind返回未指定的函数对象类型
   - 通常用`auto`或`std::function`接收

2. **占位符命名空间**
   ```cpp
   using namespace std::placeholders;  // 必须导入
   ```

3. **成员函数指针语法**
   ```cpp
   bind(&Class::method, ...);  // 注意&和::
   ```

4. **避免悬空引用**
   ```cpp
   auto make_bind() {
       int local = 42;
       return bind(foo, ref(local));  // ❌ 危险！local已销毁
   }
   ```

5. **性能考虑**
   - bind可能引入额外间接调用
   - C++14+优先使用lambda

## 实际应用场景

### 线程创建
```cpp
void task(int id, const string& msg) {
    cout << id << ": " << msg << endl;
}

thread t(bind(task, 1, "Hello"));
t.join();
```

### 回调函数注册
```cpp
class Button {
public:
    void onClick(function<void()> callback) {
        callback_ = callback;
    }
private:
    function<void()> callback_;
};

class Handler {
public:
    void handle(int x) { /* ... */ }
};

Handler h;
Button btn;
btn.onClick(bind(&Handler::handle, &h, 42));
```

### STL算法适配
```cpp
struct Person {
    string name;
    int age;
};

vector<Person> people = { /* ... */ };

// 按年龄排序
sort(people.begin(), people.end(),
     [](const Person& a, const Person& b) { 
         return a.age < b.age; 
     });

// 使用bind（不推荐，仅作示例）
bool cmpAge(const Person& a, const Person& b) {
    return a.age < b.age;
}
sort(people.begin(), people.end(), cmpAge);
```

## 总结

- `bind`用于固定或重排函数参数
- 现代C++优先使用lambda替代
- 绑定成员函数时注意对象生命周期
- 需要引用语义时使用`std::ref`
