<!--
author: lumos
date: 2024-12-16
title:  
tags: 
category: 
status: draft
summary: 
-->


# 标准库类型queue
```cpp
#include <queue>
using std::queue;
```
- 队列容器，先进先出（FIFO）
- 是容器适配器，默认基于deque实现
- 只能从队尾插入，从队首删除
- 是类模板

##### 初始化方式
| 方式                      | 说明                           |
| :------------------------ | :---------------------------- |
| queue<T> q1               | 默认初始化，空queue             |
| queue<T> q2(q1)           | 包含q1所有元素的副本            |
| queue<T> q3 = q1          | 包含q1所有元素的副本            |
| queue<T, Container> q4    | 使用指定的底层容器类型          |

##### 对queue的写入操作
| 方式               | 说明                                      |
| :---------------- | :--------------------------------------- |
| q.push(t)         | 向队尾添加t元素，优先使用移动构造，其次拷贝构造 |
| q.emplace(args...) | 在队尾就地构造元素                         |
| q.pop()           | 删除队首元素（不返回值）                    |
| q.swap(q2)        | 交换两个queue的内容                        |
| q1 = q2           | 用q2中元素的拷贝替换q1中的元素              |

##### 对queue的读取操作
| 方式            | 说明                                      |
| :-------------- | :--------------------------------------- |
| q.empty()       | 如果q为空，返回真，否则返回假               |
| q.size()        | 返回q中的元素个数，size_type类型            |
| q.front()       | 返回队首元素的引用                         |
| q.back()        | 返回队尾元素的引用                         |
| q1 == q2        | 元素完全相同且位置一致时返回真              |
| q1 != q2        | 元素完全相同且位置一致时返回假，否则返回真    |
| <, <=, >, >=    | 字典序比较                                |

>注：queue不支持迭代器，不能使用range-for循环遍历

##### 使用示例
```cpp
queue<int> q;

// 入队
q.push(1);
q.push(2);
q.push(3);

// 访问队首和队尾
cout << q.front() << endl;  // 输出1
cout << q.back() << endl;   // 输出3

// 出队
q.pop();  // 删除1
cout << q.front() << endl;  // 输出2

// 遍历（需要逐个弹出）
while (!q.empty()) {
    cout << q.front() << " ";
    q.pop();
}
```

##### 指定底层容器
```cpp
// 使用list作为底层容器
queue<int, list<int>> q1;

// 使用deque作为底层容器（默认）
queue<int, deque<int>> q2;
```

>注：底层容器必须支持front()、back()、push_back()和pop_front()操作

##### 常见应用场景
- 广度优先搜索（BFS）
- 任务调度
- 消息队列
- 缓冲区管理
- 打印队列
