# CAS无锁编程

## Atomic类
![](https://raw.githubusercontent.com/b1b2b3b4b5b6/pic/main/PicGo/202307251830986.png)
### 原子操作种类
原子操作可以分为下面几种： 
* Store 操作
    * memory_order_relaxed,
    * memory_order_release
    * memory_order_seq_cst
* Load 操作
    * memory_order_relaxed
    * memory_order_consume（已废弃）
    * memory_order_acquire
    * memory_order_seq_cst
* Read-modify-write 操作
    * memory_order_relaxed
    * memory_order_consume（已废弃）
    * memory_order_acquire
    * memory_order_release
    * memory_order_acq_rel
    * memory_order_seq_cst

#### 内存序区别

#### memory_order_relaxed: 
* 无fencing作用, CPU和编译器 可以 重排序 指令 

#### memory_order_acquire（刷新读缓存）:
* 当前线程的 读写 不能**乱序到前面**
* 其他线程对这个原子变量的所有 **写** 操作对当前线程是 可见 的. 

#### memory_order_release（刷新写缓存）: 
* 当前线程的 读写 不能**乱序到后面**
* 当前线程的所有 **写** 操作对其他 **读** 这个 原子变量 的线程是 可见 的. 

#### memory_order_acq_rel（刷新读写缓存）
*  当前线程的 **读写** 不能 被 **重排序** 在这条操作之前以及之后
*  其他线程对这个 原子变量 的 **写** 对本线程是 可见 的. 
* 本线程的这条操作结果对其他 **读取** 该 原子变量 的线程是 可见 的

#### memory_order_seq_cst（刷新读写缓存，且不可并行）: 
* 读 操作在被影响的内存位置上执行 acquire 操作
* 写 操作在被影响的内存位置上执行 release 操作
* 对象为所有线程



### CAS（V，E，N）

* `V`:内存值（共享变量）
* `E`:旧的预期值（工作内存的值）
* `N`:要修改的新值（修改共享变量的值）

当且仅当`预期值E`==`内存值V`相，将`内存值V`=`N`，否则什么都不做。

#### 特性
* 通过硬件指令实现，具有`原子性`

#### 函数
```C++
class atomic {
    bool compare_exchange_strong(T& expect /*用来比较的值*/, T desired/*用来设置的值*/)
    bool compare_exchange_weak(T& expect, T desired)
}

```


> 所以weak和strong的区别在于，weak仍然在`*ptr == expected`的时候，执行依然会有小概率失败
>也就是说， 即使`*ptr == expected`，此时也不会发生值的设置，返回false。（不会是设置成功了，但是返回的是false）

