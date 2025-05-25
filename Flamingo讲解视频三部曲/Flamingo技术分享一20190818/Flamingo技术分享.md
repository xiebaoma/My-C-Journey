## Flamingo 技术讲座大纲



### 如何编译与调试

server 和 pc 使用 VS 2019，安卓端使用 Android Studio；

调试的使用 VS 或者 gdb；



### 整体架构，聊天服务、文件服务与图片服务

chatserver => mysql

fileserver、imgserver



### pc端的结构

UI 层 -> 数据加工层 -> 网络通信层



网络通信协议（技术范畴）

聊天协议（业务范畴）

```
{"msgText": "hello"}, {"faceID": 1},{"msgText": "hello"}
```



数据加工层 - hProxWnd - 目标窗口





### 断线重连的设计

技术上的重连

业务上的重连



### 从Session到Connection再到Channel后Socket

网络框架 Connection、Channel、Socket

TcpConnection - Channel - fd

TcpConnection  - fd

Session 不属于网络框架的。





### EventLoop与IO复用技术

one thread one loop

```
while (!m_bQuit)
{
    check_and_do_timers();
    
    epoll_or_select_func();
 
    handle_io_events();
 
    handle_other_things();
}
```

epollfd - fds



新连接来了，epoll检测到，调用Acceptor::handleRead，调用TcpServer::newConnection=>创建connection对象。



EventLoop-A listenfd 

EventLoop-B clientfd1 clientfd3

EventLoop-C clientfd2





https://blog.csdn.net/analogous_love/article/details/53426777



###   定时器的设计

std::priority_queue

小根堆

时间轮或者时间片





### 收发数据正确的做法

收，监听可读事件，事件触发后收取；

发，先尝试直接发，当send或者write函数表明当前tcp窗口太小发不出去再注册可写事件，下次可写事件触发后将发送缓冲区的剩余数据继续发出去后，移除监听可写事件。





### 如何阅读FlamingoServer代码

  



### 接收和发送缓冲区设计

- 每一路连接都有一个接收和发送缓冲区；

100 = 20 +  30

- 有序的

  char sendBuf[10 * 1024 * 1024];

  std::vector<char> sendBuf;

  std::string 

- 缓冲区大小要有个上限

  https://mp.weixin.qq.com/s/wUOrfxrO9C7tvvkXEkeQoA



 \-

  ### 协议格式分析

定长

以特定分隔符

包头 + 包体

Flamingo 的协议分析：

```
struct chat_msg_header
{
    char     compressflag;     //压缩标志，如果为1，则启用压缩，反之不启用压缩
    int32_t  originsize;       //包体压缩前大小
    int32_t  compresssize;     //包体压缩后大小
    char     reserved[16];
};

std::string 二进制内容 BinaryRead/WriteStream

TLV/TTLV Type Length Value / Tag Type Length Value

```

int32_t 

100

0~127

<stdint.h>

0111 1111

注意事项：

- 涉及到整数时一定要使用显式指定长度的类型；
- 涉及到浮点数要考虑精度问题，建议放大成整数或者使用字符串去传输；

 

### 心跳包的设计

https://mp.weixin.qq.com/s/CmtXhax60uBDrfZ-NWr7OA



 ### 断线重连的设计



### 自动升级功能如何设计



### 日志类

- 支持适用不同场景的日志级别；
- 可以打印日志文件名和行号；
- 支持rollSize
- 



   \- 数据库连接的管理与数据操作

   \- 缓存机制的设计

   \- 信号的处理

   \- 程序初始化

   \- 端口重用与地址重用

   \- Linux 与 Windows 平台在网络通信部件上的一些区别