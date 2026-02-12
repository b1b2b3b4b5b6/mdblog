# CRTP
## 示例
```cpp
template <class T>
class Base
{
public:
  void interface()
  {
    static_cast<T*>(this)->implementation();
  }
};
 
class Derived : public Base<Derived>
{
public:
  void implementation() {}
};

template<class Derived>
void draw_with_color(const Base<Derived>& base)
{
  base.interface();
}
```
**优点**
* 可以用基类的接口调用派生类的实现方法
* 搭配模板函数可以接受各种基类对象的入参
* 不需要查表跳转，实现静态多态
**缺点**
* 各个基类本质时不同的类型，无法用vector之类统一管理

## std::enable_shared_from_this
使用方法
```cpp
class Book: public std::enable_shared_from_this<Book> {
public:
  shared_ptr<Book> shared_this() {
    return std::shared_from_this();
}

int main()
{
  auto b = std::make_shared<Book>();
  auto bb = b.shard_this(); //bb也是共享指针
}
```

`std::enable_shared_from_this`内部定义简化如下
```cpp
 template<typename _Tp>
    class enable_shared_from_this
    {
    protected:

      enable_shared_from_this(const enable_shared_from_this&) noexcept { }
      ~enable_shared_from_this() { }

    public:
      shared_ptr<_Tp>
      shared_from_this()
      { return shared_ptr<_Tp>(this->_M_weak_this); }

      shared_ptr<const _Tp>
      shared_from_this() const
      { return shared_ptr<const _Tp>(this->_M_weak_this); }

    private:
      mutable weak_ptr<_Tp>  _M_weak_this;
    };
```

实现原理：
* 派生类需要拿到共享指针的信息：通过集成特定基类，而后make_shared识别到托管对象继承了特定基类时，将自身weak_ptr赋值给基类
* 基类还需要拿到派生类类型，才能得到共享指针：将派生类型作为模板参数传递给基类


