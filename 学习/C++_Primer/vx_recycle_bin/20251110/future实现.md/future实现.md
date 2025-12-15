# C++ `std::future` 的实现

## 概要
本文简要说明 C++ 标准库中 `std::future`/`std::promise`（及相关类型）背后的典型实现思路：共享状态（shared state）、同步机制、异常传播与生命周期管理。重点在于概念与线程语义，而非某个具体库的源代码细节。

## 共享状态（shared state）
- 当创建 `std::promise`、`std::packaged_task` 或 `std::async` 时，会为该异步结果分配一个“共享状态”。该状态包含：可选的返回值/异常、就绪标志、用于等待/唤醒的同步对象，以及对该状态的引用计数。
- `std::future` 和 `std::shared_future` 持有对共享状态的引用（通常通过指针或 `shared_ptr`），`std::promise`/`packaged_task` 也持有引用以便在完成时设置值。

## 值与异常的传递
- 值通常存储在共享状态中的可选位置，`set_value` 将值写入此处。若任务抛出异常，库会捕获并通过 `std::exception_ptr` 存储到共享状态，随后 `future.get()` 会重新抛出该异常。
- `future.get()` 一般会移动（move）共享状态中的值并使未来对象失效（普通 `std::future` 只能 `get` 一次）。`std::shared_future` 则允许多次访问。

## 同步：等待与唤醒
- 共享状态包含用于同步的原语，常见实现为一个互斥量（mutex）+条件变量（condition_variable）。当调用 `wait()`/`get()` 时，如果结果尚未准备好，调用线程会阻塞在条件变量上；当 `set_value`/`set_exception` 执行时，会在设置就绪标志后调用 `notify_one()`/`notify_all()` 唤醒等待者。
- 超时等待（`wait_for`/`wait_until`）的实现通常基于条件变量的带时限等待，循环检查就绪标志以防伪唤醒。

## 生命周期与所有权
- 共享状态的生存期受持有它的对象影响：只要有 `future`/`promise`/`packaged_task` 等引用存在，共享状态就保留。实现上常通过引用计数（例如 `shared_ptr`）管理该内存。
- 当所有者都销毁后，共享状态被释放；如果在 `promise` 被销毁而未设置值的情况下，标准要求在 `future.get()` 时抛出 `std::future_error` 或将异常（如 `broken_promise`）传递给等待者。

## move 语义与有效性
- `std::future` 是不可复制但可移动的。移动语义通过转移对共享状态的引用实现；被移动后源 `future` 变为无效（`valid()` 返回 false）。

## `shared_future` 的实现要点
- `std::shared_future` 通过允许多个对象共享对相同共享状态的只读访问来支持多次读取结果。底层通常使用额外的引用计数或使得 `get()` 不转移值而是返回 `const`/`copy`。

## `std::async` 与延迟执行
- `std::async` 可按策略创建新线程（`std::launch::async`）或延迟执行（`std::launch::deferred`）。无论哪种，都会创建共享状态并返回 `std::future`。对于 deferred，直到调用 `get`/`wait` 时才在调用线程直接执行任务。

## 常见实现细节与优化（简明）
- 小对象优化：值的存放可采用可选（optional）或未构造缓冲区以避免多余拷贝。
- 原子与无锁：在轻量场景下，某些实现可能用原子标志减少锁的使用，但通常仍需条件变量来阻塞等待线程以避免忙等。
- 异常与状态一致性：设置值/异常时要保证原子性（先写入值/异常，再设置就绪标志，最后唤醒等待者）。

## 简短示例（说明流程）
```C++
// 大致伪代码，展示 promise/future 交互
struct SharedState {
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    std::any value;                // 存放返回值（示意）
    std::exception_ptr ex;
};

// promise.set_value(v):
{
    std::lock_guard<std::mutex> lg(shared->m);
    shared->value = std::move(v);
    shared->ready = true;
}
shared->cv.notify_all();

// future.get():
{
    std::unique_lock<std::mutex> lk(shared->m);
    shared->cv.wait(lk, [&]{ return shared->ready; });
    if (shared->ex) std::rethrow_exception(shared->ex);
    return std::any_cast<T>(std::move(shared->value));
}
```

## 注意与边界情况（简短）
- `future.get()` 只能被调用一次（普通 `future`），重复调用未定义/会抛异常。使用 `shared_future` 可多次访问。
- 如果 `promise` 被销毁而未设置值，等待的 `future` 应收到相应异常（`broken_promise`）。
- `get()` 在返回前会阻塞，注意不要在持有其它重要锁的情况下长时间阻塞，以免发生死锁。

## 小结
`std::future` 的实现以“共享状态 + 同步原语”为核心：共享状态保存值/异常与就绪标志，互斥量与条件变量保障等待/唤醒的正确性；引用计数管理生命周期；异常通过 `std::exception_ptr` 传播。理解这些要点有助于正确使用 futures 并调优并发设计。
