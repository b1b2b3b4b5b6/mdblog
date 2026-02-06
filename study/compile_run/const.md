# const
## 修饰变量
* 被const修饰的变量，一但创建（编译后运行时）后无法改变
* const修饰的变量默认符号类型为`local`,不参与链接决议
* 定义const变量必须初始化
    * 定义类时不用赋值, 因为类本身就会初始化
    * 类内成员变量不用，因为类内成员属于声明
* 除了引用和指针外, const修饰的位置无影响


### 变量位置
#### 类外
* 修饰的是class, 总是放在`.bss`
* 修饰的是基本类型
    * 初始化值编译时确定, 且后续无取地址操作(被引用也算), 不分配内存, 当作`立即数`编译
    * 在上面基础上, 有后续取地址操作(被引用也算), 放在`.rodata`
    * 初始化值不确定, 放在`.bss`
>修饰static局部变量时, 不会当作立即数编译, 其余符合上述规则

#### 类内
* 修饰的成员变量(除了static)总是被分配内存, 具体位置视类实例内存位置
* 若修饰的成员变量被static修饰
    * 若未定义, 且初始值确定, 则当作`立即数`编译
    * 若自己定义, 且初始值确定, 则放在`.rodata`
    * 若自己定义, 且初始值不确定, 则放在`.bss`
> 有趣的是, 当`static const 成员变量`在声明时指定初始值时, 对读操作, 会直接使用初始值; 
> 对`取地址`的相关操作时, 仍会使用符号(自己或其他文件导出的)
```C++
class OBJ {
public:
  static const int k = 2;

public:
  OBJ() {}
};
const int OBJ::k;
int main() {
  cout << OBJ::k << std::endl;
  cout << *(&OBJ::k) << std::endl;
  return 0;
}
//输出
//2
//5(链接决议得到的符号所代表的值)
```
## 修饰成员函数
```C++
class OBJ{
    void ConstFoo() const
    {
       ...;
    }
}
```
* const修饰的成员函数可以接受指向`const OBJ`的指针（一般成员函数不能）
* const修改的成员函数只可修改mutable和static成员变量

## 其他作用
### 延长临时对象生命周期
如下
```C++
#include <iostream>
#include <string>

class MyClass {
public:
    MyClass(int val) : value(val) {
        std::cout << "构造: " << value << std::endl;
    }
    ~MyClass() {
        std::cout << "析构: " << value << std::endl;
    }
    int value;
};

MyClass createTemp() {
    return MyClass(42);  // 返回临时对象
}

int main() {
    std::cout << "=== 情况1: 没有引用接收 ===" << std::endl;
    {
        createTemp();  // 临时对象立即销毁
        std::cout << "表达式结束后" << std::endl;
    }
    
    std::cout << "\n=== 情况2: const引用接收 ===" << std::endl;
    {
        const MyClass& ref = createTemp();  // 临时对象生命周期延长
        std::cout << "可以使用: " << ref.value << std::endl;
        std::cout << "作用域结束前..." << std::endl;
    }  // 这里才析构
    
    std::cout << "\n=== 情况3: 非const引用（编译错误）===" << std::endl;
    // MyClass& ref2 = createTemp();  // 错误！非const引用不能绑定临时对象
    
    std::cout << "\n=== 情况4: 实际应用 ===" << std::endl;
    {
        // 常见场景：函数参数
        const std::string& s = std::string("Hello") + " World";
        std::cout << s << std::endl;  // 临时string对象仍然存活
    }
    
    return 0;
}
```