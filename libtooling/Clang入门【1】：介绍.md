<!--
author: lumos
date: 2020-12-11
title:  Clang入门【1】：介绍
tags: Clang，静态分析
category: 
status: publish
summary: Clang和静态分析方式介绍
-->

>注：本文翻译自[BITS,BUYES,BOOS](https://kevinaboos.wordpress.com/2013/07/23/clang-tutorial-part-i-introduction/)，可自行参考。

# Clang入门【1】：介绍

#### 什么是Clang？
我已经花了一段时间在研究使用[Clang](http://clang.llvm.org/)（[LLVM compiler project](http://llvm.org/)的前端组件）上，Clang可以分析任何属于C语言家族（C, C++, ObjectiveC, etc…）的代码，并且拥有非常棒的模块化设计，容易使用。如果你在寻找分析C代码的工具，我强烈推荐Clang，因为它比其他类似的工具更好，文档规范。

#### Clang如何工作？
在大多数情况下，Clang会运行预处理器（展开所有宏），将代码转换成抽象语法树（AST），AST相比于源码更容易操作，但是转换成源码也是轻松的。事实上，每种Clang用来表达源码的数据结构（AST，CFG，etc），都能还原到到原始的代码，这对于分析源码非常有用（refactoring, etc）

如果你想在源码层面去分析和修改，Clang比LLVM更好：因为使用LLVM做分析，要使用LLVM底层的方法，几乎等同与汇编（==）

#### Clang AST
几乎每种编译器和静态语法分析工具都会使用AST去表达源码，Clang的AST非常的详细和复杂，但是在学习各种各样的类和AST成员过程中，你将会乐在其中。参考[introduction to the Clang AST](http://clang.llvm.org/docs/IntroductionToTheClangAST.html)，最简单的学习AST的方法就是dump一个简单的源文件，看看它是如何产生的。

大体上，一个Clang AST由两部分组成：Decl和Stmt。他们有很多的子类，下面是一些例子：

- [FunctionDecl](http://clang.llvm.org/doxygen/classclang_1_1FunctionDecl.html) 函数声明或定义
- [BinaryOperator](https://clang.llvm.org/doxygen/classclang_1_1BinaryOperator.html) 表达式，比如 a+b
- [CallExpr](http://clang.llvm.org/doxygen/classclang_1_1CallExpr.html) 函数调用，比如 foo(x)

大多数AST中的类都是自注释的，比如ForStmt，IfStmt，ReturnStmt，几分钟就能熟悉AST，同样百度“*Clang FunctionDecl*”也能找到类的文档。

#### 如何使用Clang？
Clang可以完全替代gcc，并且它提供一些非常酷的静态分析工具。作为一个开发者，通过使用使用它的库，你可以充分发挥Clang的全部威力。

首先，参考一下[Clang’s own description](http://clang.llvm.org/docs/Tooling.html)，上面有Clang的接口使用。我也将简单讲一下他们之间的差异。

##### Clang Plugin
作为插件（由开发者使用Clang库编译生成的插件）使用的情况下，将只能一个文件一个文件的分析，不同文件之间不能分享变量。就像GCC的优化参数一样 (e.g., “-O1”)，你只能向gcc传递编译参数，gcc将调用插件给入参数，整个过程中不能运行任何自定义任务

##### LibTooling(Clang Tool)
作为独立的C++程序运行，和普通程序一样有main函数做为进入点。LibTooling通常用于分析多个源文件，这个过程是和编译链分离的。对于每个源文件，程序都会为之生成新的AST，类似Clang Plugin，但是在最后，你可以使用所有这些生成的AST，因为他们在程序内是共享的。由于存在main函数，你也可以在分析前后运行自定义的任务。

##### LibClang
如果你需要稳定的API，LibClang是最棒的选择。Clang经常更像迭代，如果你使用Plugin或者LIbTooling，你可能需要经常去更新你的静态分析插件（程序）代码，来适应Clang的更新（挺麻烦的）。如果你需要通过其他语言（比如PYTHON）来使用Clang的库，那么你必须使用LibClang。

>注：LibClang相比于LibTooling和Clang Plugin，是阉割版，无法提供AST的所有访问路径。

如果你还是无法决定，我建议你参考[Clang’s own description](http://clang.llvm.org/docs/Tooling.html)，这是最简单的方法。它甚至提供了Clang Plugin解析多个文件时，将结果报错至全局变量的方法。当然，用LIbTooling其实更简单。

#### Getting Started with Clang
现在你已经了解了基础知识，让我们下载LLVM（Clang被包含其中）搞起吧。
参考[Clang’s official installation instructions](http://clang.llvm.org/get_started.html)

- 安装ninja cmake clang git make 
- 找个合适的地方放工程：git clone https://github.com/llvm/llvm-project.git
- 执行以下命令
```bash
cd ~/clang-llvm
mkdir build && cd build
cmake -G Ninja ../llvm -DLLVM_ENABLE_PROJECTS=clang -DLLVM_BUILD_TESTS=ON  # Enable tests; default is off.
ninja
```
>注：这里编译了clang，如果想要编译自己的LibTooling工具，需要将自己的工程加入cmake，可参考[使用Clang作为库 —— 使用 LibTooling 和 LibASTMatchers 构建工具的教程](https://blog.csdn.net/qq_23599965/article/details/90697162)