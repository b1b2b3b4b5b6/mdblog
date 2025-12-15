<!--
author: lumos
date: 2020-12-11
title:  Clang入门【2】：使用LibTooling
tags: Clang，静态分析
category: 
status: publish
summary: LibTooling静态分析示例
-->

>注：本文翻译自[BITS,BUYES,BOOS](https://kevinaboos.wordpress.com/2013/07/23/clang-tutorial-part-ii-libtooling-example/)，可自行参考。
# Clang入门【2】：使用LibTooling
#### LibTooling示例
我们将从LIbtooling的例子开始介绍，因为个人觉得它是Clang最好用的接口，当然，这部分的代码略经修改也能作为插件方式使用。

以分析一个简单的C文件(test.c)为例：
```C++
void do_math(int *x) {
    *x += 5;
}
 
int main(void) {
    int result = -1, val = 4;
    do_math(&val);
    return result;
}
```

我们的目标是在test.c上做一些修改：

- 将函数定义`do_math`改成`add5`
- 将所有的`do_math`调用改成`add5`
- 将`main`函数的返回值从`val`改成`result`

首先，我们需要做一些准备。Clang自带很好的插件示例，而不是LibTooling示例，但是没有关系，下面介绍如何创建一个LigTooling程序（[示例代码库](https://github.com/kevinaboos/LibToolingExample)）

#### 从Main函数开始
Example.cpp的内容如下：
```C++
int main(int argc, const char **argv) {
    // parse the command-line args passed to your code
    CommonOptionsParser op(argc, argv);
    // create a new Clang Tool instance (a LibTooling environment)
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());
 
    // run the Clang Tool, creating a new FrontendAction (explained below)
    int result = Tool.run(newFrontendActionFactory<ExampleFrontendAction>());
 
    errs() << "\nFound " << numFunctions << " functions.\n\n";
    // print out the rewritten source code ("rewriter" is a global var.)
    rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());
    return result;
}
```
上面的代码基本是自注释的，除了`rewriter`（将在下文解释），代码在首先设置了Clang Tool（**L5**），传递命令行参数（op.getCompilations()）和待解析文件列表（op.getSourcePathList()），然后调用run方法（**L8**）。使用LibTooling的好处在于你可以在解析前（**L6**）后（**L9**）做一些事情，比如打印输出，修改代码（**L12**），统计函数总数（**L10**）。在Clang插件的情形下是无法做到的。
同时我们需要定义一些全局变量。
```C++
Rewriter rewriter;
int numFunctions = 0;
```

#### 创建一个FrontendAction
现在，我们可以创建自定义的[FrontAction](http://clang.llvm.org/doxygen/classclang_1_1FrontendAction.html)，具体就是一个能够在Clang前端执行操作的类，我们选择[ASTFrontAction](http://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html)去分析test.c的AST。
```C++
class ExampleFrontendAction : public ASTFrontendAction {
public:
    virtual ASTConsumer *CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        return new ExampleASTConsumer(&CI); // pass CI pointer to ASTConsumer
    }
};
```
上面的代码创建了一个`ASTFrontendAction `的子类，并且重写了`CreateASTConsumer `方法，这样就能返回我们自己的`ASTConsumer`, 并且传递了指向[CompilerInstance](http://clang.llvm.org/doxygen/classclang_1_1CompilerInstance.html)的指针，因为这个指针包含了我们分析时所需的信息。

#### 创建一个ASTConsumer
[ASTConsumer](http://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html)能够消费（读取）Clang parser产生的AST。你可以重写这个类里面许多方法，当一个特定种类的AST节点被解析时，重写的方法就会被调用。首先，然我们重写[HandleTopLevelDecl()](http://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html#a856744773798bd97057ccbc2768b21ad)，这个方法会在顶层定义（全局变量，函数定义）被解析时被调用。
```C++
class ExampleASTConsumer : public ASTConsumer {
private:
    ExampleVisitor *visitor; // doesn't have to be private
 
public:
    // override the constructor in order to pass CI
    explicit ExampleASTConsumer(CompilerInstance *CI)
        : visitor(new ExampleVisitor(CI)) // initialize the visitor
        { }
 
    // override this to call our ExampleVisitor on each top-level Decl
    virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
            Decl *D = *i;
            visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
        }
        return true;
    }
};
```
这样一来，我们就解析出test.c中的两个函数定义：`do_math`和`main`，代码中的`ExampleVisitor`将会在下文介绍。

#### ASTConsumer的更好实现
当然，`HandleTopLevelDecl()`会在声明被解析时立即调用，而不是在所有解析完成之后，重写这个函数后，是无法得知已完成的解析内容。
幸运的是，ASTConsumer还有一个更好的方法去重写：[ HandleTranslationUnit()](http://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html#a2bea2db1d0e8af16c60ee7847f0d46ff)，这个方法会在所有源文件解析之后被调用，并且传入一个[ASTContext](http://clang.llvm.org/doxygen/classclang_1_1ASTContext.html)引用，其包含了源文件的AST。
将`HandleTopLevelDecl()`替换成`HandleTranslationUnit()`，我们可以得到下面的代码
```C++
// this replaces "HandleTopLevelDecl"
// override this to call our ExampleVisitor on the entire source file
virtual void HandleTranslationUnit(ASTContext &Context) {
    /* we can use ASTContext to get the TranslationUnitDecl, which is
       a single Decl that collectively represents the entire source file */
    visitor->TraverseDecl(Context.getTranslationUnitDecl());
}
```
在大多数情况下，我们应该使用`HandleTranslationUnit()`，特别是配合使用[RecursiveASTVisitor](http://clang.llvm.org/doxygen/classclang_1_1RecursiveASTVisitor.html)时。

#### 创建一个RecursiveASTVisitor
终于，我们要做真正有用的事了。前面两节我们搭了基础的框架，[RecursiveASTVisitor](http://clang.llvm.org/doxygen/classclang_1_1RecursiveASTVisitor.html)这个类可以遍历任意AST节点，比如函数定义，表达式，只需要重写它的VisitFunctionDecl和VisitStmt就可实现。其他的类也是相似的方法。Clang也提供了一个完整简介的[official tutorial](http://clang.llvm.org/docs/RAVFrontendAction.html)。

形如`Visit*`这样的方法，我们重写它的时候必须返回`true`来使AST继续进行下去，因为返回`false`会使遍历结束。我们不能直接调用形如`Visit*`这样的方法，而是通过调用`TraverseDecl `来实现遍历（就像我们在ExampleASTConsumer里做的一样）。

在我们需要修改函数定义和表达式，所以我们只需要去重写`VisitFunctionDecl`和`VisitStmt`，下面是具体代码。
```C++
class ExampleVisitor : public RecursiveASTVisitor {
private:
    ASTContext *astContext; // used for getting additional AST info
 
public:
    explicit ExampleVisitor(CompilerInstance *CI)
        : astContext(&(CI->getASTContext())) // initialize private members
    {
        rewriter.setSourceMgr(astContext->getSourceManager(),
            astContext->getLangOpts());
    }
 
    virtual bool VisitFunctionDecl(FunctionDecl *func) {
        numFunctions++;
        string funcName = func->getNameInfo().getName().getAsString();
        if (funcName == "do_math") {
            rewriter.ReplaceText(func->getLocation(), funcName.length(), "add5");
            errs() << "** Rewrote function def: " << funcName << "\n";         
        }         
        return true;     
    }     
     
    virtual bool VisitStmt(Stmt *st) {
        if (ReturnStmt *ret = dyn_cast(st)) {
            rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
            errs() << "** Rewrote ReturnStmt\n";
        }
        if (CallExpr *call = dyn_cast(st)) {
            rewriter.ReplaceText(call->getLocStart(), 7, "add5");
            errs() << "** Rewrote function call\n";
        }
        return true;
    }
};
```
上面的代码中，值得注意的是`rewriter`这个类，操作这个类可以对源码做文本上的修改，推荐用于重构或者简单的代码改变。最后也可以通过这个类来打印修改后的源码。

使用`rewriter`的时候要注意获取正确的要进行插入或替换代码的[SouceLocation](http://clang.llvm.org/doxygen/classclang_1_1SourceLocation.html)

当然，注意`dyn_casts`（L24，L28）的用法，它会检查Stmt的类型。[关于dyn_cast的介绍](http://llvm.org/docs/ProgrammersManual.html#the-isa-cast-and-dyn-cast-templates)

最终，`error()`就是简单的标准错误流，被LLVM/Clang用来打印调试信息

#### Visit* Functions的细节
在test.c的代码中，我们只需要修改返回语句（L8）和函数调用于语句（L7），我们是通过重写`VisitStmt`是实现的，这种办法比较底层，可以分别重写`VisitReturnStmt`和`VisitCallExpr`来作为替代，更为简单。
```C++
// this replaces the VisitStmt function above
virtual bool VisitReturnStmt(ReturnStmt *ret) {
    rewriter.ReplaceText(ret->getLocStart(), 6, "val");
    errs() << "** Rewrote ReturnStmt\n";
    return true;
}
virtual bool VisitCallExpr(CallExpr *call) {
    rewriter.ReplaceText(call->getLocStart(), 7, "add5");
    errs() << "** Rewrote function call\n";
    return true;
}
```

#### 整合所有代码并编译
[点击获取完整示例库](https://github.com/kevinaboos/LibToolingExample)。里面有写好的makefile，相比与Clang的编译链，要简单的多，可以参考。
要注意的是，clone下来的示例库需要放在`llvm/tools/clang/tools`目录

#### 进行分析
我们将对文章开始的test.c文件进行分析，执行以下脚本

```bash
#!/bin/bash

LLVM_DIR=~/static_analysis/llvm/  #the location of your llvm dir
$LLVM_DIR/Debug+Asserts/bin/example test.c --
```

也许你想让程序分析多个文件，传递自定义的分析参数，或者使用Gcc的编译选项：

```bash
#!/bin/bash
 
LLVM_DIR=~/static_analysis/llvm/  #the location of your llvm dir
$LLVM_DIR/Debug+Asserts/bin/example \
    file1.c file2.c file3.c \
    -myCmdLineArg argument1 \
    -- \
    -Wall -Isome/include/dir
```

值得注意的是在`--`前的参数都是传递给LibTooling程序的，而`--`后的是传递给Clang的参数。

#### 结论
在成功进行上述步骤后，你将得到以下输出
```C++
** Rewrote function def: do_math
** Rewrote function call
** Rewrote ReturnStmt
 
Found 2 functions.
 
void add5(int *x) {
    *x += 5;
}
 
int main(void) {
    int result = -1, val = 4;
    add5(&val);
    return val;
}
```
希望你能享受整个Clang例程的过程！