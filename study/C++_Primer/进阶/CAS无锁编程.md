# CAS无锁编程

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

