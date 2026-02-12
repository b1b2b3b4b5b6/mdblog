# decltype(auto)
作用：让编译器用decltype规则推导，常用于完美转发
```C++
// 场景1：转发函数返回值
template<typename F, typename... Args>
decltype(auto) wrapper(F&& f, Args&&... args) {
    // do something
    return std::forward<F>(f)(std::forward<Args>(args)...);
    // 如果f返回引用，这里也返回引用
    // 如果用auto，会丢失引用
}

// 场景2：变量声明
int x = 10;
int& get_ref() { return x; }

auto a = get_ref();           // int（拷贝）
decltype(auto) b = get_ref(); // int&（保留引用）
```
>为什么不直接用auto： 因为auto丢弃引用和顶层const

