<!--
author: lumos
date: 2026-01-20
title:  
tags: 
category: 
status: draft
summary: 
-->


# 标准库类型stringstream
```C++
#include <sstream>
using std::stringstream;
using std::istringstream;
using std::ostringstream;
```
- 字符串流，用于字符串和其他类型之间的转换
- 支持string的输入输出操作
- 三种类型：istringstream（输入）、ostringstream（输出）、stringstream（输入输出）

##### 初始化方式
| 方式                        | 说明                           |
| :-------------------------- | :---------------------------- |
| stringstream ss             | 默认初始化，空流               |
| stringstream ss(s)          | 保存string s的拷贝             |
| istringstream iss           | 输入字符串流，只读             |
| istringstream iss(s)        | 保存string s的拷贝，只读       |
| ostringstream oss           | 输出字符串流，只写             |
| ostringstream oss(s)        | 保存string s的拷贝，只写       |

##### stringstream特有操作
| 方式           | 说明                                                  |
| :------------- | :--------------------------------------------------- |
| ss.str()       | 返回ss中保存的string的拷贝                            |
| ss.str(s)      | 将string s拷贝到ss中，返回void                        |
| ss.clear()     | 清除流的状态标志（不清空内容）                         |
| ss << value    | 将value写入ss                                        |
| ss >> value    | 从ss读取值到value                                    |

##### 使用场景
**字符串分割**
```C++
string line = "hello world test";
istringstream iss(line);
string word;
while (iss >> word) {
    cout << word << endl;  // 按空格分割
}
```

**类型转换**
```C++
// 数字转字符串
int num = 123;
ostringstream oss;
oss << num;
string str = oss.str();  // "123"

// 字符串转数字
string s = "456";
istringstream iss(s);
int n;
iss >> n;  // n = 456
```

**格式化输出**
```C++
stringstream ss;
ss << "Name: " << name << ", Age: " << age;
string result = ss.str();
```

**多值解析**
```C++
string data = "10 20.5 hello";
istringstream iss(data);
int i;
double d;
string s;
iss >> i >> d >> s;  // i=10, d=20.5, s="hello"
```

##### 状态标志
| 标志         | 说明                          |
| :----------- | :---------------------------- |
| good()       | 流处于有效状态                 |
| eof()        | 到达流末尾                     |
| fail()       | IO操作失败                     |
| bad()        | 流已崩溃                       |
| clear()      | 重置所有状态标志                |

##### 注意事项
* stringstream对象可以重复使用，但需要clear()清除状态并str("")清空内容
* 使用>>读取时会忽略前导空白字符
* 使用getline可以读取整行包括空格
* 类型转换失败时需要检查流状态

##### 清空stringstream
```C++
stringstream ss;
ss << "old data";

// 重用前清空
ss.str("");        // 清空内容
ss.clear();        // 清空状态标志

ss << "new data";  // 写入新数据
```
