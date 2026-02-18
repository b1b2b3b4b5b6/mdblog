# shared_mutex

## 概要
`std::shared_mutex` 是 C++17 引入的共享互斥锁（也称读写锁），支持**独占模式**（用于写操作）和**共享模式**（用于读操作），适用于读多写少的场景。

## 与 std::mutex 的区别

| 特性 | std::mutex | std::shared_mutex |
|------|-----------|------------------|
| 锁模式 | 仅独占 | 独占 + 共享 |
| 同时持有数 | 最多1个线程 | 独占时1个，共享时多个 |
| 适用场景 | 读写频率相近 | 读多写少 |
| 性能开销 | 较小 | 相对较大 |
| C++版本 | C++11 | C++17 |

## 核心接口

### 独占模式（写锁）
```cpp
void lock();              // 阻塞获取独占锁
bool try_lock();          // 尝试获取独占锁（非阻塞）
void unlock();            // 释放独占锁
```

### 共享模式（读锁）
```cpp
void lock_shared();       // 阻塞获取共享锁
bool try_lock_shared();   // 尝试获取共享锁（非阻塞）
void unlock_shared();     // 释放共享锁
```

## RAII 包装器

- **独占锁**：`std::lock_guard<std::shared_mutex>` 或 `std::unique_lock<std::shared_mutex>`
- **共享锁**：`std::shared_lock<std::shared_mutex>` (C++14)

## 使用示例

### 基本用法
```cpp
#include <shared_mutex>
#include <map>
#include <string>

class ThreadSafeCache {
    mutable std::shared_mutex mtx_;
    std::map<std::string, int> cache_;

public:
    // 读操作：多个线程可同时读
    int get(const std::string& key) const {
        std::shared_lock<std::shared_mutex> lock(mtx_);
        auto it = cache_.find(key);
        return it != cache_.end() ? it->second : -1;
    }

    // 写操作：独占访问
    void set(const std::string& key, int value) {
        std::unique_lock<std::shared_mutex> lock(mtx_);
        cache_[key] = value;
    }

    // 条件写：先读后写（升级锁）
    void increment(const std::string& key) {
        // 注意：不能直接从 shared_lock 升级到 unique_lock
        // 正确做法：先释放读锁，再获取写锁
        std::unique_lock<std::shared_mutex> lock(mtx_);
        ++cache_[key];
    }
};
```

### 锁升级陷阱
```cpp
// ❌ 错误：死锁风险
void wrong_upgrade() {
    std::shared_lock<std::shared_mutex> r_lock(mtx_);
    // ... 读取数据 ...
    
    // 不能直接升级！会导致死锁
    r_lock.unlock();
    std::unique_lock<std::shared_mutex> w_lock(mtx_);  // 危险：其他线程可能插入
    // ... 写入数据 ...
}

// ✅ 正确：直接使用独占锁
void correct_approach() {
    std::unique_lock<std::shared_mutex> lock(mtx_);
    // ... 读取并写入 ...
}
```

## 使用建议

1. **读多写少场景**：当读操作远多于写操作时使用 `shared_mutex`
2. **读写频率相近**：直接使用 `mutex`，避免额外开销
3. **避免锁升级**：C++ 不支持原子的锁升级（从共享到独占）
4. **注意饥饿**：频繁的读操作可能导致写操作饥饿（依赖实现）
5. **优先使用 RAII**：使用 `shared_lock` 和 `unique_lock` 管理锁的生命周期

## 相关类型

- `std::shared_timed_mutex` (C++14)：shared_mutex 的前身，额外支持超时操作
  - `try_lock_for()` / `try_lock_until()`
  - `try_lock_shared_for()` / `try_lock_shared_until()`

## 性能考虑

```cpp
// 示例：性能对比
struct Data {
    std::mutex mtx;              // 方案1
    std::shared_mutex smtx;      // 方案2
    std::vector<int> values;
};

// 读密集型：方案2 更优（10读:1写）
// 写密集型：方案1 更优（平均或写为主）
// 临界区很小：方案1 更优（锁开销占主导）
```

**经验法则**：当读写比 > 10:1 且临界区有一定大小时，`shared_mutex` 才能体现优势。

## 注意事项

1. **不可重入**：同一线程不能多次加锁（会死锁）
2. **共享锁不可写**：即使持有共享锁，也不能修改数据（语义错误，不是技术限制）
3. **锁粒度**：尽量缩小临界区范围，避免持锁时间过长
4. **平台差异**：不同操作系统的实现效率可能不同
