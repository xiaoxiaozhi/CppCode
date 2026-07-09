# poll 工作流程详解

## 一、什么是 poll？

poll 是 Linux 下 **I/O 多路复用** 的一种机制，是 **select 的改进版**，用于**同时监控多个文件描述符**（如 socket）。与 select 相比，poll 解决了以下痛点：

| select 的问题 | poll 的改进 |
|---------------|-------------|
| FD_SETSIZE 限制（默认 1024） | **无最大连接数限制** |
| fd_set 是位图，操作繁琐 | **pollfd 结构体数组，更直观** |
| 每次调用都要重新设置 fd_set | **events/revents 分离**，无需每次重新设置 |
| 跨平台但 Linux 效率一般 | **专为 UNIX/Linux 设计** |

---

## 二、poll 的核心数据结构

### `struct pollfd` — 描述一个要监控的文件描述符

```c
struct pollfd {
    int   fd;         // 要监控的文件描述符（设为 -1 则忽略该条目）
    short events;     // 输入参数：告诉内核要监控哪些事件
    short revents;    // 输出参数：内核返回实际发生的事件
};
```

**events（输入，由应用程序设置）：**
| 事件 | 含义 |
|------|------|
| `POLLIN` | 有数据可读（包括 TCP 连接断开） |
| `POLLOUT` | 可以写入数据 |
| `POLLRDHUP` | TCP 连接被对端关闭（Linux 2.6.17+） |
| `POLLPRI` | 有紧急数据可读（如 TCP 带外数据） |

**revents（输出，由内核填充）：**
| 事件 | 含义 |
|------|------|
| `POLLIN` | 数据可读 |
| `POLLOUT` | 可写 |
| `POLLERR` | 发生错误（**不需要在 events 中设置，内核会自动返回**） |
| `POLLHUP` | 对方关闭连接（**不需要设置，内核自动返回**） |
| `POLLNVAL` | 文件描述符未打开（fd 无效） |

**💡 关键设计：events 和 revents 分离**
- `events` 是**我们告诉内核**要监控什么
- `revents` 是**内核告诉我们**实际发生了什么
- 这意味着：我们只需要设置一次 events，内核不会覆盖它

> **对比 select**: select 的 fd_set 是输入输出共用，每次调用前都要重新设置，非常麻烦。

---

## 三、poll 的核心函数

### `poll()` — 等待事件就绪

```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

**参数：**
- `fds`：`struct pollfd` 数组的首地址
- `nfds`：数组中有效元素的数量（**注意：不是最大 fd 值 +1，而是数组大小**）
  - 对比 select: select 需要传入 `maxfd + 1`，因为它是从 0 到位图最大值遍历
  - poll 只需要传入有效条目数量，效率更高
- `timeout`：超时时间（毫秒）
  - `-1`：**阻塞等待**，直到有事件就绪才返回
  - `0`：**非阻塞**，立即返回
  - `>0`：等待指定的毫秒数后超时

**返回：**
- `>0`：就绪的文件描述符数量（每个 fd 可能有多个事件）
- `0`：超时
- `-1`：出错

---

## 四、poll 工作流程（服务器端）

```
启动
  │
  ├── 1. socket()           创建监听套接字 lfd
  ├── 2. bind()             绑定 IP 和端口
  ├── 3. listen()           开始监听
  │
  ├── 4. 创建 pollfd 数组 fds[MAX_EVENTS]
  │     初始化所有 fds[i].fd = -1
  │
  ├── 5. fds[0].fd = lfd
  │     fds[0].events = POLLIN    ← 将 lfd 加入监控
  │     max_index = 0
  │
  ├── 6. while (1) {
  │       ├── poll(fds, max_index + 1, -1)  ← 阻塞等待
  │       │
  │       ├── for (i = 0; i <= max_index; i++) {
  │       │     if (fds[i].fd == -1) continue
  │       │
  │       │     if (fds[i].revents & POLLIN) {
  │       │       ├── if (fds[i].fd == lfd) {
  │       │       │     // 新客户端连接
  │       │       │     cfd = accept()
  │       │       │     找空闲位置 fds[j].fd = cfd
  │       │       │     fds[j].events = POLLIN
  │       │       │     更新 max_index
  │       │       │
  │       │       └── else {
  │       │             // 已有客户端发数据
  │       │             read() / write()
  │       │             如果 len == 0: close() + fds[i].fd = -1
  │       │           }
  │       │     }
  │       │   }
  │     }
```

---

## 五、poll 与 select 的详细对比

| 对比项 | select | poll |
|--------|--------|------|
| 头文件 | `<sys/select.h>` | `<poll.h>` |
| 数据结构 | `fd_set` 位图 | `struct pollfd` 结构体数组 |
| 最大连接数 | 1024（可改但需重编译内核） | **无限制**（受系统内存限制） |
| 输入参数 | 每次必须重新设置 fd_set | **events 一次设置，内核不覆盖** |
| 遍历方式 | 遍历整个位图（0 ~ maxfd） | 遍历数组（0 ~ nfds-1） |
| 编码复杂度 | 较高（FD_SET/FD_CLR 宏） | 较低（直接操作结构体） |
| 移植性 | 几乎所有平台支持 | 主要在 UNIX/Linux 系统 |
| 性能 | 随连接数增长线性下降 | 同 select，也是 O(n) |

---

## 六、poll 的优缺点

### ✅ 优点
1. **无 1024 限制** → 可以监控更多文件描述符
2. **events/revents 分离** → 不用每次重新设置
3. **接口更清晰** → pollfd 结构体比 fd_set 更直观
4. **错误检测更准确** → revents 明确标识 POLLERR/POLLHUP

### ❌ 缺点
1. **仍然是 O(n) 遍历** → 每次 poll 都要遍历整个数组确认就绪状态
2. **用户态/内核态数据拷贝** → 每次调用都要将整个数组从用户态拷贝到内核态
3. **随着连接数增加，性能下降** → 不适合超高并发场景（此时应使用 epoll）
4. **不是真正的就绪事件驱动** → 内核只是标记 revents，仍需要应用层遍历

---

## 七、poll 适用场景

| 场景 | 是否推荐 | 原因 |
|------|----------|------|
| 连接数 < 1000 | ✅ 推荐 | 代码简单，足够用 |
| 连接数 1000 ~ 5000 | ⚠️ 可用 | 性能开始下降，建议考虑 epoll |
| 连接数 > 5000 | ❌ 不推荐 | 应使用 epoll |
| 跨平台应用 | ✅ 推荐 | 比 epoll 更可移植 |
| Linux 高并发服务器 | ❌ 不推荐 | epoll 是更好的选择 |

---

## 八、poll 编码要点

### 要点1：fd = -1 表示该位置空闲
```cpp
// 初始化
for (int i = 0; i < MAX_EVENTS; i++)
    fds[i].fd = -1;

// 关闭客户端后
close(fd);
fds[i].fd = -1;  // 标记为空闲
```

### 要点2：events 只需设置一次
```cpp
// 第一次加入时设置 events
fds[i].fd = cfd;
fds[i].events = POLLIN;     // 只需设置一次
fds[i].revents = 0;

// 后续每次 poll 返回后，events 仍然保持 POLLIN
// 所以不需要像 select 那样重新设置！
```

### 要点3：nfds 参数是数组大小，不是 maxfd+1
```cpp
// select 写法（麻烦）
FD_ZERO(&rset);
FD_SET(lfd, &rset);
select(maxfd + 1, &rset, NULL, NULL, NULL);

// poll 写法（简洁）
poll(fds, max_index + 1, -1);
// max_index + 1 表示从 0 到 max_index 共多少个有效条目
```

### 要点4：检查 revents 用位运算
```cpp
if (fds[i].revents & POLLIN)   // 检查是否可读
if (fds[i].revents & POLLOUT)  // 检查是否可写
if (fds[i].revents & POLLERR)  // 检查是否出错
if (fds[i].revents & POLLHUP)  // 检查是否挂断
```

### 要点5：处理完一个事件要递减计数器
```cpp
int nready = poll(fds, max_index + 1, -1);
for (int i = 0; i <= max_index && nready > 0; i++) {
    if (fds[i].revents & POLLIN) {
        // 处理事件 ...
        nready--;  // 处理完一个，减少剩余待处理计数
    }
}