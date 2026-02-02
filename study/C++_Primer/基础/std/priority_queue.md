<!--
author: lumos
date: 2020-12-11
title:  
tags: 
category: 
status: draft
summary: 
-->


# 标准库类型priority_queue
```C++
#include <queue>
using std::priority_queue;
```
- 优先队列，基于堆实现
- 默认为最大堆（大顶堆）
- 元素自动按优先级排序

##### 初始化方式
| 方式                                  | 说明                          |
| :------------------------------------ | :---------------------------- |
| priority_queue<T> pq1                 | 默认初始化，空优先队列，最大堆  |
| priority_queue<T, Container, Cmp> pq2 | 指定底层容器和比较器           |
| priority_queue<T> pq3(first, last)    | 用迭代器范围初始化             |
| priority_queue<T> pq4(cmp)            | 使用自定义比较器               |
| priority_queue<T> pq5(cmp, cont)      | 使用自定义比较器和底层容器      |

##### 常用比较器声明
```C++
// 最大堆（默认）
priority_queue<int> maxHeap;
priority_queue<int, vector<int>, less<int>> maxHeap2;

// 最小堆
priority_queue<int, vector<int>, greater<int>> minHeap;
```

##### 对priority_queue的写入操作
| 方式              | 说明                                |
| :---------------- | :---------------------------------- |
| pq.push(t)        | 插入元素t，优先使用移动构造         |
| pq.emplace(args...) | 就地构造元素                      |
| pq.pop()          | 删除堆顶元素（不返回值）            |

##### 对priority_queue的读取操作
| 方式         | 说明                              |
| :----------- | :-------------------------------- |
| pq.empty()   | 如果pq为空，返回真，否则返回假     |
| pq.size()    | 返回pq中的元素个数                |
| pq.top()     | 返回堆顶元素的引用（最高优先级元素）|

>注意：priority_queue不支持遍历，不支持迭代器

##### 自定义比较器
方法1: 函数对象
```C++
struct cmp {
    bool operator()(int a, int b) const {
        return a > b;  // 最小堆
    }
};
priority_queue<int, vector<int>, cmp> pq;
```

方法2: lambda表达式
```C++
auto cmp = [](int a, int b) { return a > b; };
priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
```

方法3: 自定义类型比较
```C++
struct Node {
    int val;
    int priority;
    // 重载<运算符（用于最大堆）
    bool operator<(const Node& other) const {
        return priority < other.priority;
    }
};
priority_queue<Node> pq;
```

##### 指针类型元素的比较
```C++
struct ListNode {
    int val;
    ListNode *next;
};

// 比较指针指向对象的值
struct cmp {
    bool operator()(ListNode* a, ListNode* b) const {
        return a->val > b->val;  // 最小堆
    }
};
priority_queue<ListNode*, vector<ListNode*>, cmp> pq;
```

##### 常用操作示例
```C++
// 最大堆示例
priority_queue<int> maxHeap;
maxHeap.push(3);
maxHeap.push(1);
maxHeap.push(4);
cout << maxHeap.top() << endl;  // 输出4
maxHeap.pop();
cout << maxHeap.top() << endl;  // 输出3

// 最小堆示例
priority_queue<int, vector<int>, greater<int>> minHeap;
minHeap.push(3);
minHeap.push(1);
minHeap.push(4);
cout << minHeap.top() << endl;  // 输出1

// 自定义类型示例
struct Task {
    string name;
    int priority;
    bool operator<(const Task& other) const {
        return priority < other.priority;  // 优先级高的在前
    }
};
priority_queue<Task> taskQueue;
taskQueue.push({"task1", 3});
taskQueue.push({"task2", 1});
taskQueue.push({"task3", 2});
cout << taskQueue.top().name << endl;  // 输出task1
```

##### 从数组构造优先队列
```C++
vector<int> nums = {3, 1, 4, 1, 5, 9};
priority_queue<int> pq(nums.begin(), nums.end());

// 或使用初始化列表（C++11）
priority_queue<int> pq2({3, 1, 4, 1, 5, 9});
```

##### 常见应用场景
- Top K 问题：使用最小堆维护K个最大元素
- 合并K个有序链表：使用最小堆
- 任务调度：按优先级处理任务
- Dijkstra算法：最短路径问题
- 中位数维护：双堆结构
