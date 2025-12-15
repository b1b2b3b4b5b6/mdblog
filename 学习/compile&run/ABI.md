# ABI
* 是一套二进制接口规则
* 由编译器，操作系统，硬件共同决定
* 决定了二进制兼容性

# 具体规则
## C
* 对象布局(Object layout)
* 数据类型的大小和对齐(Size and default alignment of data types)
* 函数调用方式(Calling Convention)
* 寄存器使用(Register usage convention)
* 目标文件的格式（这里的格式指ELF / COFF等，不是产生的内容的格式）

## C++
* 名称修饰/重整(Name mangling)：C++具有函数重载、模板、名称空间等，他们在目标文件中应该具有不同的名称，来让可执行文件可以调用到唯一的函数。将函数的名称变换为另一个唯一名称的过程称为名称修饰/重整；例如，对于函数 namespace Namespace {int function(int x);} ，在GCC中会修饰为_ZN9NameSpace8functionEi，而在MSVC中会修饰为?function@NameSpace@@YAHH@Z。
* 异常处理(Exception handling)：例如在遇到异常时，栈如何展开(unwind)。
* 调用构造/析构函数(Invoking ctor & dtor)：规定了一个类的成员如何构造/析构，例如如何构造成员中的C数组。
* class的布局和对齐，例如多继承中成员变量的排布。
* 虚表的布局和对齐，例如虚函数在虚表中的顺序。