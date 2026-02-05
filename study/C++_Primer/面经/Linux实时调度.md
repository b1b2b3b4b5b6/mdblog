<!--
author: lumos
date: 2026-02-05
title:  
tags: 
category: 
status: draft
summary: 
-->


# Linux 实时调度

Linux 实时调度提供**确定性响应时间**，用于对时间敏感的任务。

```C++
#include <sched.h>
#include <pthread.h>
```

## 实时调度策略

| 策略                | 说明                                      | 优先级范围 |
| :----------------- | :--------------------------------------- | :-------- |
| SCHED_FIFO         | 先进先出，无时间片限制                     | 1-99      |
| SCHED_RR           | 轮转调度，有时间片（默认100ms）            | 1-99      |
| SCHED_DEADLINE     | 截止时间调度（EDF算法）                    | 最高      |
| SCHED_OTHER/NORMAL | 默认调度（CFS）                           | -         |
| SCHED_BATCH        | 批处理任务                                | -         |
| SCHED_IDLE         | 最低优先级                                | -         |

## 优先级关系
```
SCHED_DEADLINE > SCHED_FIFO/RR (1-99) > SCHED_OTHER > SCHED_IDLE
                                  ↑
                              99最高，1最低
```

## 设置方法

### 方法1：pthread API（推荐）

##### 创建线程前设置
```C++
pthread_t thread;
pthread_attr_t attr;

// 初始化属性
pthread_attr_init(&attr);

// 设置调度策略
pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

// 设置优先级
struct sched_param param;
param.sched_priority = 80;
pthread_attr_setschedparam(&attr, &param);

// 关键：使用显式调度（不继承父进程）
pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

// 创建线程
pthread_create(&thread, &attr, thread_func, nullptr);
pthread_attr_destroy(&attr);
```

##### 运行时设置
```C++
void* realtime_thread(void* arg) {
    struct sched_param param;
    param.sched_priority = 80;
    
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0) {
        perror("pthread_setschedparam failed");
        return nullptr;
    }
    
    // 实时任务代码
    while (true) {
        // 时间敏感工作
    }
    return nullptr;
}
```

### 方法2：sched_setscheduler

##### 设置当前进程
```C++
#include <sched.h>

struct sched_param param;
param.sched_priority = 50;

// 0 表示当前进程
if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
    perror("sched_setscheduler");
    return 1;
}
```

##### 设置指定进程
```C++
pid_t pid = 12345;
struct sched_param param;
param.sched_priority = 50;

sched_setscheduler(pid, SCHED_RR, &param);
```

### 方法3：命令行工具 chrt

##### 启动程序时设置
```bash
# FIFO 调度，优先级 80
chrt -f 80 ./my_program

# RR 调度，优先级 50
chrt -r 50 ./my_program
```

##### 修改运行中进程
```bash
# 修改进程调度策略
chrt -f -p 80 <pid>

# 查看进程调度信息
chrt -p <pid>
```

### 方法4：systemd 服务

##### 服务配置文件
```ini
# /etc/systemd/system/myservice.service
[Service]
ExecStart=/path/to/program
CPUSchedulingPolicy=fifo
CPUSchedulingPriority=80
CPUAffinity=0 1  # 可选：绑定 CPU
```

## 完整示例

##### 基础实时线程
```C++
#include <pthread.h>
#include <sched.h>
#include <iostream>
#include <cstring>

bool set_thread_realtime(int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0) {
        std::cerr << "设置实时调度失败: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

void* realtime_task(void* arg) {
    if (!set_thread_realtime(80)) {
        return nullptr;
    }
    
    std::cout << "实时线程运行，优先级 80" << std::endl;
    
    // 实时任务循环
    while (true) {
        // 执行时间敏感的工作
    }
    return nullptr;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, nullptr, realtime_task, nullptr);
    pthread_join(thread, nullptr);
    return 0;
}
```

##### 进程级实时调度 + 内存锁定
```C++
#include <sched.h>
#include <sys/mman.h>
#include <iostream>
#include <cstring>

bool enable_realtime(int priority) {
    // 设置实时调度
    struct sched_param param;
    param.sched_priority = priority;
    
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        std::cerr << "设置实时调度失败: " << strerror(errno) << std::endl;
        std::cerr << "需要 root 权限或 CAP_SYS_NICE" << std::endl;
        return false;
    }
    
    // 锁定内存，避免页面交换延迟
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
        std::cerr << "锁定内存失败: " << strerror(errno) << std::endl;
        // 不是致命错误，继续运行
    }
    
    std::cout << "实时调度已启用，优先级 " << priority << std::endl;
    return true;
}

int main() {
    if (!enable_realtime(80)) {
        return 1;
    }
    
    // 实时任务代码
    while (true) {
        // 时间敏感的工作
    }
    
    return 0;
}
```

## 权限管理

### 需要的权限
| 权限方式          | 说明                                      |
| :--------------- | :--------------------------------------- |
| root 用户        | 拥有所有权限                              |
| CAP_SYS_NICE     | 允许设置实时调度和优先级                   |
| ulimit           | 限制实时优先级范围                        |

### 设置 CAP_SYS_NICE（推荐）
```bash
# 赋予可执行文件权限（无需 root 运行）
sudo setcap 'cap_sys_nice=eip' ./my_program

# 查看权限
getcap ./my_program

# 删除权限
sudo setcap -r ./my_program
```

### 设置 ulimit
```bash
# 查看实时优先级限制
ulimit -r

# 设置限制（临时）
ulimit -r 99

# 永久设置：编辑 /etc/security/limits.conf
@realtime   -   rtprio    99
@realtime   -   memlock   unlimited
```

## 验证与调试

### 查看调度信息
```bash
# 查看进程调度策略和优先级
ps -e -o pid,class,rtprio,cmd | grep my_program

# 使用 chrt 查看
chrt -p <pid>

# 查看线程调度信息
ps -eLo pid,tid,class,rtprio,cmd | grep my_program
```

### 代码内查询
```C++
// 查询当前调度策略
int policy = sched_getscheduler(0);
switch (policy) {
    case SCHED_FIFO:   std::cout << "SCHED_FIFO" << std::endl; break;
    case SCHED_RR:     std::cout << "SCHED_RR" << std::endl; break;
    case SCHED_OTHER:  std::cout << "SCHED_OTHER" << std::endl; break;
    default:           std::cout << "Unknown" << std::endl; break;
}

// 查询优先级
struct sched_param param;
sched_getparam(0, &param);
std::cout << "优先级: " << param.sched_priority << std::endl;

// 查询优先级范围
int min_prio = sched_get_priority_min(SCHED_FIFO);
int max_prio = sched_get_priority_max(SCHED_FIFO);
std::cout << "FIFO 优先级范围: " << min_prio << "-" << max_prio << std::endl;
```

## 最佳实践

### 配合使用的技术
| 技术              | 作用                          | 示例                              |
| :--------------- | :--------------------------- | :------------------------------- |
| CPU 亲和性        | 绑定到特定 CPU，减少迁移开销    | `pthread_setaffinity_np()`       |
| 内存锁定          | 避免页面交换延迟               | `mlockall(MCL_CURRENT|MCL_FUTURE)` |
| 禁用抢占          | 减少内核抢占                   | 编译内核时配置                     |
| 中断亲和性        | 隔离中断到特定 CPU              | `/proc/irq/*/smp_affinity`       |

### CPU 亲和性设置
```C++
#include <pthread.h>

void set_cpu_affinity(int cpu_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);
    
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}

// 使用
set_cpu_affinity(2);  // 绑定到 CPU 2
```

### 完整实时配置
```C++
bool configure_realtime(int priority, int cpu_id) {
    // 1. 设置实时调度
    struct sched_param param;
    param.sched_priority = priority;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        return false;
    }
    
    // 2. 锁定内存
    mlockall(MCL_CURRENT | MCL_FUTURE);
    
    // 3. 设置 CPU 亲和性
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
    
    // 4. 预热内存（可选）
    // 触发内存分配，避免运行时分配延迟
    
    return true;
}
```

## 注意事项

##### 风险
- 实时任务可能饿死普通任务
- 死循环的实时任务会锁死系统
- 不当使用可能导致系统无响应

##### 建议
- 实时任务必须主动让出 CPU（`sched_yield()`）
- 设置看门狗监控
- 预分配资源，避免运行时动态分配
- 测试最坏情况执行时间（WCET）
- 使用 `SCHED_RR` 而非 `SCHED_FIFO`（更安全）

##### 编译与运行
```bash
# 编译
g++ -o rt_program rt_program.cpp -pthread

# 需要 root 或 CAP_SYS_NICE
sudo ./rt_program

# 或使用 setcap
sudo setcap cap_sys_nice=eip ./rt_program
./rt_program
```

## 参考
- `man sched(7)` - 调度策略概述
- `man pthread_setschedparam(3)` - pthread 调度设置
- `man sched_setscheduler(2)` - 系统调用
- `man capabilities(7)` - Linux 能力
