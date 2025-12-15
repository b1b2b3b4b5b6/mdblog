<!--
author: lumos
date: 2020-12-11
title:  Clang入门【3】：使用Plugin
tags: Clang，静态分析
category: 
status: publish
summary: Clang Plugin静态分析示例
-->

>注：本文翻译自[BITS,BUYES,BOOS](https://kevinaboos.wordpress.com/2013/07/29/clang-tutorial-part-iii-plugin-example/)，可自行参考。
# Clang入门【3】：使用Plugin

#### Clang Plugin的介绍
在Calng入门【1】，我们介绍过Clang Plugin和LibToolong相比，使用起来是相似的，除了Clang Plugin不能同时分析多个文件（不同点之一）。

但是Clang Plugin作为语法检查或者格式化助手是可行的，下面介绍如何使用。

#### Plugins 和LibTooling的重要区别
- Clang Plugins和LibTooling执行路径不同
- 通过向Calng传递命令行参数，Clang Plugins可以成为编译链的一部分
- Clang Plugins可以控制编译过程（通过报警告或停止编译链）

#### 创建一个Clang Plugin
[点击参考官方文档](http://clang.llvm.org/docs/ClangPlugins.html)。
[点击参考官方例程](http://clang.llvm.org/docs/ClangPlugins.html)。
首先clone我的例程库放在`llvm/tools/clang/examples/`下（llvm自带的例程也在这里）

```bash
cd llvm/tools/clang/examples
git clone https://github.com/kevinaboos/PluginExample.git PluginExample
```

关注`PluginExample.cpp`，可以看出它和之前LibTooling例程文件略有差异，但是`ASTConsumer`和`RecursiveASTVisitor`的实现是一样。

Plugins没有`main()`，所有唯一的调用入口是注册过的[PluginASTAction](http://clang.llvm.org/doxygen/classclang_1_1PluginASTAction.html)。

#### 创建一个PluginASTAction
[PluginASTAction](http://clang.llvm.org/doxygen/classclang_1_1PluginASTAction.html)看起来像[ASTFrontendAction](http://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html)，是我们唯一和ASTConsumer产生联系的方法，顾名思义，它只能在Clang Plugin使用。

下面的代码替换了LibTooling例程的`ASTFrontendAction`
```C++
class PluginExampleAction : public PluginASTAction {
protected:
    // this gets called by Clang when it invokes our Plugin
    ASTConsumer *CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        return new ExampleASTConsumer(&CI);
    }
 
    // implement this function if you want to parse custom cmd-line args
    bool ParseArgs(const CompilerInstance &CI, const vector<string> &args) {
        return true;
    }
};
```

#### 将Plugin注册到Clang
我们需要将Plugin注册到Clang，这样Clang才能在编译过程中调用到Plugin，就像下面一样：

```C++
static FrontendPluginRegistry::Add<PluginExampleAction>
       X("-example-plugin", "simple Plugin example");

```

这是所有Clang Plugins都要在程序最后做的事，注册Plugin需要两个必要参数：
- 一个和插件绑定的命令行参数，比如`-example-plugin`
- Plugin的描述，比如`simple Plugin example`

#### 编译Plugin例程
下面是Plugin的makefile，相比LibTooling要简单的多
```makefile
CLANG_LEVEL := ../..
LIBRARYNAME = PluginExample
 
SOURCES := PluginExample.cpp
 
LINK_LIBS_IN_SHARED = 0
SHARED_LIBRARY = 1
 
include $(CLANG_LEVEL)/Makefile
```

#### 运行Plugin例程
我们还是使用LibTooling例程里的test.c作为输入，下面的执行脚本：  

```bash
#!/bin/bash
 
clang -Xclang -load \
      -Xclang ~/static_analysis/llvm/Debug+Asserts/lib/libPluginExample.so \
      -Xclang -plugin -Xclang -example-plugin \
      -Wall -c test.c
```

指令看起来复杂，但分解后就很直白。首先，`-Xclang`后面跟着的参数都会被传递至Clang的`cc1 process`（类似预处理器），上面指令的`-Xclang`参数组合起来的完整形式是：

```bash
clang -load [YourLibrary.so] -plugin -example-plugin [-Options] -c [-SourceFiles]
```

其余的`-`连接的参数就是标准的GCC编译参数，最后是我们要输入的文件名或路径。

#### 结论
由于Plugin是配置Clang编译使用的，所以我们不希望其在终端输出太多不必要的信息，所有可以将Plugin的信息输出到文件。