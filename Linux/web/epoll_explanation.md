# epoll 工作流程详解

## 一、什么是 epoll？

epoll 是 Linux 下 **I/O 多路复用** 的一种机制，用于**同时监控多个文件描述符**（如 socket），当某个描述符就绪（可读/可写/异常）时通知应用程序处理。相比于 select/poll，epoll 效率更高，尤其适合**高并发**场景。

---

## 二、epoll 的三大核心函数

### 1. `epoll_create()` — 创建 epoll 实例

```c
int epoll_create(int size);
```

- **作用**：在内核中创建一颗**红黑树** + 一个**就绪链表**
- **参数**：size（Linux 2.6.8 后忽略，传 >0 即可）
- **返回**：epoll 文件描述符（红黑树的根节点）

### 2. `epoll_ctl()` — 管理监控的文件描述符

```c
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```

- **作用**：向 epoll 实例中**添加/修改/删除**要监控的文件描述符
- **参数**：
  - `epfd`：epoll_create 返回的句柄
  - `op`：操作类型
    - `EPOLL_CTL_ADD` — 添加
    - `EPOLL_CTL_MOD` — 修改
    - `EPOLL_CTL_DEL` — 删除
  - `fd`：要监控的文件描述符
  - `event`：告诉内核要监听什么事件

**struct epoll_event 结构：**
```c
struct epoll_event {
    uint32_t     events;  /* 要监听的事件（位掩码） */
    epoll_data_t data;    /* 用户自定义数据（联合体） */
};

typedef union epoll_data {
    void    *ptr;
    int      fd;       // 最常用：存储文件描述符
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;
```

**常用事件：**
| 事件 | 含义 |
|------|------|
| `EPOLLIN` | 读事件：缓冲区有数据可读 |
| `EPOLLOUT` | 写事件：缓冲区可写 |
| `EPOLLERR` | 异常事件 |
| `EPOLLET` | **边缘触发模式**（默认是水平触发 LT） |

---

### 3. `epoll_wait()` — 等待事件就绪（核心！）

```c
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

- **作用**：**阻塞等待** epoll 实例中有文件描述符就绪
- **参数**：
  - `epfd`：epoll 句柄
  - `events`：**传出参数**，内核把就绪的事件复制到这个数组中
  - `maxevents`：events 数组大小
  - `timeout`：超时时间（毫秒）
    - `-1`：一直阻塞直到有事件就绪
    - `0`：不阻塞，立即返回
    - `>0`：最多阻塞多少毫秒
- **返回**：
  - `>0`：就绪的文件描述符数量
  - `0`：超时
  - `-1`：出错

---

## 三、epoll 完整工作流程（服务器端）

```
启动
  │
  ├── 1. socket()      创建监听套接字 lfd
  ├── 2. bind()        绑定 IP 和端口
  ├── 3. listen()      开始监听
  │
  ├── 4. epoll_create()  创建 epoll 实例 efd
  │
  ├── 5. epoll_ctl(ADD, lfd, EPOLLIN)  将 lfd 加入 epoll 监控
  │
  ├── 6. while (1) {
  │       ├── epoll_wait(efd, events, MAX_EVENTS, -1)  ← 阻塞等待
  │       │
  │       ├── for (i = 0; i < nready; i++) {
  │       │     ├── if (events[i].data.fd == lfd) {
  │       │     │     // 有新客户端连接
  │       │     │     accept() 得到 cfd
  │       │     │     epoll_ctl(ADD, cfd, EPOLLIN)  ← 将新连接加入监控
  │       │     │
  │       │     └── else {
  │       │           // 已有客户端发数据
  │       │           read() 读取数据
  │       │           处理业务逻辑
  │       │         }
  │       │   }
  │     }
```

---

## 四、LT（水平触发）vs ET（边缘触发）

| 特性 | LT (Level Triggered) | ET (Edge Triggered) |
|------|---------------------|---------------------|
| 默认模式 | ✅ 是 | ❌ 需加上 `EPOLLET` 标志 |
| 行为 | 只要缓冲区还有数据，每次 `epoll_wait` 都会通知 | 数据从无到有**变化时**才通知一次 |
| 编码难度 | 简单 | 较复杂，需循环读直到 EAGAIN |
| 效率 | 相对低（重复通知） | 高（减少系统调用） |

**ET 模式下必须：**
1. 将文件描述符设为**非阻塞**
2. `read()`/`write()` 循环直到返回 `EAGAIN` 错误

---

## 五、epoll 与 select/poll 对比

| 特性 | select | poll | epoll |
|------|--------|------|-------|
| 最大连接数 | 1024（FD_SETSIZE） | 无限制 | 无限制 |
| 遍历方式 | 线性遍历所有 fd | 线性遍历所有 fd | 回调机制，只遍历就绪的 fd |
| 数据拷贝 | 每次都要将 fd 集合从用户态拷贝到内核态 | 同 select | 使用 mmap，减少拷贝 |
| 工作模式 | LT | LT | LT + ET |
| 性能随连接数增长 | 线性下降 | 线性下降 | 基本不变 |