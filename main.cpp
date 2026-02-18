#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <mutex>
#include <list>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <future>
#include <fstream>
#include <string>
#include <stdexcept>
#include <typeindex>

using namespace std;

/* 共享指针实现  */
template <typename T>
class SharedPtr
{
public:
    // 构造函数：接受原始指针
    explicit SharedPtr(T *ptr = nullptr)
        : data(ptr), count(ptr ? new std::atomic<int>(1) : nullptr) {}

    // 析构函数
    ~SharedPtr()
    {
        release();
    }

    // 拷贝构造
    SharedPtr(const SharedPtr &other)
        : data(other.data), count(other.count)
    {
        if (count)
            count->fetch_add(1);
    }

    // 拷贝赋值
    SharedPtr &operator=(const SharedPtr &other)
    {
        if (this != &other)
        { // 自赋值检查
            release();
            data = other.data;
            count = other.count;
            if (count)
                count->fetch_add(1);
        }
        return *this;
    }

    // 移动构造
    SharedPtr(SharedPtr &&other) noexcept
        : data(other.data), count(other.count)
    {
        other.data = nullptr;
        other.count = nullptr;
    }

    // 移动赋值
    SharedPtr &operator=(SharedPtr &&other) noexcept
    {
        if (this != &other)
        { // 自赋值检查
            release();
            data = other.data;
            count = other.count;
            other.data = nullptr;
            other.count = nullptr;
        }
        return *this;
    }

    // 解引用
    T &operator*() const
    {
        if (data)
            return *data;
        throw std::runtime_error();
    }

    // 箭头运算符
    T *operator->() const
    {
        if (data)
            return data;
        throw std::runtime_error();
    }

    // 获取原始指针
    T *get() const { return data; }

    // 引用计数
    int use_count() const { return count ? *count : 0; }

    void reset()
    {
        release();
    }

    bool operator bool() const
    {
        return data != nullptr;
    }

private:
    void release()
    {
        if (count)
        {
            if (count->fetch_sub(1) == 1)
            {
                delete data;
                delete count;
            }
            data = nullptr;
            count = nullptr;
        }
    }

    T *data;
    atomic<int> *count;
};

/* 判断元素是否存在  */
template <typename Container, typename Value>
bool contains(const Container &c, const Value &val)
{
    for (auto it = c.cbegin(); it != c.cend(); ++it)
    {
        if (*it == val)
            return true;
    }
    return false;
}

/* 带锁计数器  */
class Counter
{
    int count = 0;
    std::mutex mtx;

public:
    void increment()
    {
        std::lock_guard<std::mutex> lock(mtx);
        count++;
    }

    int get()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return count; // 没有加锁
    }

    void reset()
    {
        std::lock_guard<std::mutex> lock(mtx);
        count = 0;
    }
};

/* 文件IO管理  */
class File
{
    std::fstream fs_;
    std::string path_;

public:
    // 构造函数：explicit，打开失败抛异常
    explicit File(const std::string &path,
                  std::ios::openmode mode = std::ios::in | std::ios::out)
        : path_(path), fs_(path, mode)
    {
        if (!fs_.is_open())
        {
            throw std::runtime_error("Failed to open file: " + path);
        }
    }

    // 析构：自动关闭
    ~File()
    {
        if (fs_.is_open())
        {
            fs_.close();
        }
    }

    // 禁止拷贝
    File(const File &) = delete;
    File &operator=(const File &) = delete;

    // 移动构造
    File(File &&other) noexcept
        : fs_(std::move(other.fs_)), path_(std::move(other.path_))
    {
    }

    // 移动赋值
    File &operator=(File &&other) noexcept
    {
        if (this != &other)
        {
            if (fs_.is_open())
            {
                fs_.close();
            }
            fs_ = std::move(other.fs_);
            path_ = std::move(other.path_);
        }
        return *this;
    }

    // 写入
    void write(const std::string &data)
    {
        if (!fs_.is_open())
        {
            throw std::runtime_error("File not open");
        }
        fs_ << data;
        if (fs_.fail())
        {
            throw std::runtime_error("Write failed");
        }
    }

    // 读取一行
    std::string read_line()
    {
        if (!fs_.is_open())
        {
            throw std::runtime_error("File not open");
        }
        std::string line;
        std::getline(fs_, line);
        return line;
    }

    // 读取全部
    std::string read_all()
    {
        if (!fs_.is_open())
        {
            throw std::runtime_error("File not open");
        }
        std::string content((std::istreambuf_iterator<char>(fs_)),
                            std::istreambuf_iterator<char>());
        return content;
    }

    // 检查是否打开
    bool is_open() const
    {
        return fs_.is_open();
    }
};

class obj
{
    int operator()()
    {
        n++;
        return n;
    }
    int n = 0;
};

obj make_counter()
{
    return obj();
}

// 递归终止
void print()
{
    cout << endl;
}

// 递归展开
template <typename T, typename... Args>
void print(const T &first, const Args &...args)
{
    cout << first;
    if (sizeof...(args) > 0)
    {
        cout << " ";
    }
    print(args...); // 注意：args...
}

/* LRU  */
class LRUCache
{
private:
    int capacity;
    unordered_map<int, list<vector<int>>::iterator> m;
    list<vector<int>> l;

public:
    LRUCache(int capacity)
    {
        this->capacity = capacity;
    }

    int get(int key)
    {
        if (m.count(key) == 0)
            return -1;

        refresh(key);
        return (*m[key])[1];
    }

    void put(int key, int value)
    {
        if (m.count(key))
        {
            (*m[key])[1] = value;
            refresh(key);
        }
        else
        {
            l.push_back({key, value});
            m[key] = --l.end();
            check();
        }
    }

    void refresh(int key)
    {
        auto iter = m[key];
        auto v = std::move(*iter);
        l.erase(iter);
        l.push_back(std::move(v));
        m[key] = --l.end(); // ✅ 必须更新迭代器
    }
    void check()
    {
        if (m.size() > capacity)
        {
            auto first = l.begin();
            auto k = (*first)[0];
            l.erase(first);
            m.erase(k);
        }
    }
};

/* 单例模式  */
class singleton
{
private:
    singleton() = default;
    singleton(const singleton &) = delete;
    singleton &operator=(const singleton &) = delete;

public:
    static singleton *get_instance()
    {
        static singleton t;
        return &t;
    }
};

/* 带锁对象池  */
template <typename T>
class ObjectPool
{
public:
    explicit ObjectPool(size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            objects_.push_back(std::make_unique<T>());
            free_objects_.push(objects_.back().get());
        }
    }

    T *acquire()
    {
        std::unique_lock<std::mutex> lock(mtx_);

        // 等待直到有空闲对象
        cv_.wait(lock, [this]
                 { return !free_objects_.empty(); });

        T *obj = free_objects_.front();
        free_objects_.pop();
        return obj;
    }

    void release(T *obj)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        free_objects_.push(obj);
        cv_.notify_one(); // 唤醒等待的线程
    }

private:
    std::vector<std::unique_ptr<T>> objects_; // 持有对象所有权
    std::queue<T *> free_objects_;            // 空闲对象队列
    std::mutex mtx_;
    std::condition_variable cv_;
};

/* 多线程日志库  */
class mylog
{
public:
    mylog()
    {
        t = thread([this]
                   { work_loop(); });
    }

    ~mylog()
    {
        is_running = false;
        if (t.joinable())
            t.join();

        lock_guard lk(m);

        while (q.size() > 0)
        {
            std::cout << q.front();
            q.pop();
        }
    }

    void write(int log_level, string &s)
    {
        if (is_running == false)
            return;

        lock_guard lk(m);
        q.push(std::move(format_log(log_level, s)));
    }

private:
    std::string format_log(int level, const std::string &msg)
    {
        // 添加时间戳、级别、线程ID
        // ...
    }

    void work_loop()
    {
        is_running = true;
        while (is_running)
        {
            queue<string> temp_q;

            {
                lock_guard lk(m);
                if (q.size() > 100)
                {
                    std::swap(temp_q, q);
                }
            }

            while (temp_q.size() > 0)
            {
                std::cout << temp_q.front();
                temp_q.pop();
            }

            this_thread::sleep_for(chrono::microseconds(500));
        }
    }
    bool is_running;
    queue<string> q;
    mutex m;
    thread t;
};

int main()
{
    auto f = std::async(std::launch::async, []()
                        { return 1 + 1; });
    auto a = f.get();
}

/* RAII  */
class ResourceManager
{
public:
    ResourceManager(const std::string &name)
    {
        file_.reset(fopen(name.c_str(), "r"));
        if (!file_)
        {
            throw std::runtime_error("Failed to open file");
        }

        buffer_ = std::make_unique<char[]>(1024);

        connection_.reset(connect_to_database());
        if (!connection_)
        {
            throw std::runtime_error("Failed to connect");
        }
    }

    ~ResourceManager()
    {
    }

    void process()
    {
        // 使用资源进行处理
        fread(buffer_.get(), 1, 1024, file_.get());
        // ... 处理数据
    }

private:
    void *
    connect_to_database();       // 假设这个函数已实现
    void disconnect(void *conn); // 假设这个函数已实现

    unique_ptr<FILE, decltype(&fclose)> file_{nullptr, &fclose};
    unique_ptr<char[]> buffer_;
    unique_ptr<void, decltype(&disconnect)> connection_{nullptr, &disconnect};
};

/* 阻塞队列  */
template <typename T>
class BlockingQueue
{

public:
    BlockingQueue(int n = 100)
    {
        cap = n;
    }

    ~BlockingQueue()
    {
        while (!q.empty())
        {
            q.pop();
        }
    }

    void push(T value)
    {
        std::unique_lock lk(m);
        write_cv.wait(lk, [this]()
                      { return q.size() < cap; }); // 直接访问，更简洁

        q.push(std::move(value));
        read_cv.notify_one();
    }

    T pop()
    {
        std::unique_lock lk(m);
        read_cv.wait(lk, [this]()
                     { return !q.empty(); }); // 直接访问，更简洁
        auto res = q.front();
        q.pop();
        write_cv.notify_one();
        return res;
    }

private:
    int cap;
    queue<T> q;
    mutex m;
    condition_variable read_cv;
    condition_variable write_cv;
};

/* 无锁对象池简单实现  */
template <typename T>
class CASPool
{
    struct alignas(64) Block
    {
        T data;
        Block *next;
    };
    struct VersionCtl
    {
        Block *ptr;
        uint64_t version;

        bool operator==(const VersionCtl &other) const
        {
            if (ptr == other.ptr && version == other.version)
                return true;
            return false;
        }
    };

public:
    CASPool()
    {
        free_ctl.store(VersionCtl{nullptr, 0});
    }

    T *malloc()
    {
        VersionCtl exp = free_ctl.load(std::memory_order_acquire);

        VersionCtl new_ctl;
        do
        {
            if (exp.ptr == nullptr)
            {
                lager();
                exp = free_ctl.load(std::memory_order_acquire);
            }

            if (exp.ptr == nullptr)
                return nullptr;

            new_ctl.ptr = exp.ptr->next;
            new_ctl.version = exp.version + 1;
        } while (free_ctl.compare_exchange_weak(exp, new_ctl, std::memory_order_release, std::memory_order_acquire) == false);

        return &(exp.ptr->data);
    }

    void free(T *data_ptr)
    {
        auto block_ptr = reinterpret_cast<Block *>(reinterpret_cast<char *>(data_ptr) - offsetof(Block, data));

        VersionCtl exp = free_ctl.load(std::memory_order_acquire);
        VersionCtl new_ctl;
        new_ctl.ptr = block_ptr;
        do
        {
            block_ptr->next = exp.ptr;
            new_ctl.version = exp.version + 1;
        } while (free_ctl.compare_exchange_weak(exp, new_ctl, std::memory_order_release, std::memory_order_acquire) == false);
    }

    void lager()
    {
        std::lock_guard lk(lager_m);

        size_t count = obj_list.empty() ? 100 : obj_list.size();
        for (size_t i = 0; i < count; ++i)
        {
            obj_list.push_back(std::make_unique<Block>());
            free(&(obj_list.back()->data));
        }
    }

private:
    vector<std::unique_ptr<Block>> obj_list;
    atomic<VersionCtl> free_ctl;
    mutex lager_m;
};

/* Any简单实现  */
struct Any
{
    Any(void) : ty(std::type_index(typeid(void))) {}
    Any(const Any &that) : m_ptr(that.Clone()), ty(that.ty) {}
    Any(Any &&that) : m_ptr(std::move(that.m_ptr)), ty(that.ty) {}

    template <typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, void>::type>
    Any(U &&value) : m_ptr(new Derived<typename std::decay<U>::type>(forward<U>(value))),
                     ty(typeid(typename std::decay<U>::type)) {}

    bool IsNull() const { return m_ptr == nullptr; }

    template <class U>
    bool Is() const
    {
        return ty == type_index(typeid(U));
    }

    template <class U>
    U &AnyCast()
    {
        if (!Is<U>())
        {
            throw std::bad_cast();
        }
        auto derived = dynamic_cast<Derived<U> *>(m_ptr.get());
        return derived->m_value;
    }

    Any &operator=(const Any &a)
    {
        if (m_ptr == a.m_ptr)
            return *this;

        m_ptr = a.Clone();
        ty = a.ty;
        return *this;
    }

private:
    struct Base
    {
        virtual ~Base() {}
        virtual std::unique_ptr<Base> Clone() const = 0;
    };

    template <typename T>
    struct Derived : Base
    {
        template <typename U>
        Derived(U &&value) : m_value(forward<U>(value)) {}

        std::unique_ptr<Base> Clone() const
        {
            return std::unique_ptr<Base>(new Derived<T>(m_value));
        }

        T m_value;
    };

    std::unique_ptr<Base> Clone() const
    {
        if (m_ptr != nullptr)
            return m_ptr->Clone();

        return nullptr;
    }

    std::unique_ptr<Base> m_ptr;
    std::type_index ty;
};
