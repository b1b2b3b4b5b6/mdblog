# swap
标准库实现
```cpp
namespace std{
    template<typename T>
    void swap(T & a, T & b){
        T temp(a);
        a = b;
        b = temp;
    }
};
```
缺点: 只做值拷贝, 效率低
对于下面的类, 只需交换`ptr`指针即可
```cpp
class Contour {
public:
    ...
private:
    ContourInfo* ptr;//指针，所指对象数据
};
```

尝试模板特例化std::swap
```cpp
namespace std{
    template<>
    void swap<Contour>(Contour&a, Contour&b){
        //交换ptr指针
    };
};
```
发现因为ptr是private变量,无法直接访问

需要`Contour`先实现个swap成员方法, 然后在模板特例化内调用
```cpp
class Contour{ 
public:
    ...
    void swap(Contour& other){
        using std::swap;     
        swap(ptr, other.ptr);
    }
}

namespace std{
    template<>
    void swap<Contour>(Contour&a, Contour& b){//no-memberde swap
        a.swap(b);//调用swap成员函数
    }
};
```


## swap
* swap不是必要的
* swap需要定义成友元函数
* 友元swap优先级高于std::swap

```cpp
class Foo{
    friend void swap(Foo &, Foo &);
}
```
