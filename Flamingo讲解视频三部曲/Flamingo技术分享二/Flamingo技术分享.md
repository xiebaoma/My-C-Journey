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



**Flamingo**包格式分析：

包头部分

```
#pragma pack(push, 1)
//协议头
struct chat_msg_header
{
    char     compressflag;     //压缩标志，如果为1，则启用压缩，反之不启用压缩
    int32_t  originsize;       //包体压缩前大小
    int32_t  compresssize;     //包体压缩后大小
    char     reserved[16];
};

#pragma pack(pop)
```

组包的过程：

```
包体： cmd  seq data(json) 
cmd = 1100, seq = 0, data:聊天内容 targetid(消息接受者)

L checksum V V LV V

6字节的空间（整个字符串的长度 + checksum） + 四字节int + 四字节int +（字符串压缩后的长度 + 字符串本身）
```

解包：

```
先解包头
再解包体
```

整型的压缩技巧

```
int32 => 1 ~ 5
0~127 1 字节
128 ~ 0x7FFF 2字节
0x7FFFFF 3字节

1111111111110000111111111
1.00000000=》0.999999999、1.0000001
```

30 40 50 =》 120



1234 5678 

BigEndian Little Endian

包体部分

握手的过程：

```
//客户端发给服务器的握手请求
GET ws://120.55.94.78:9988/ HTTP/1.1\r\n
Host: 120.55.94.78:9988\r\n
Connection: Upgrade\r\n
Upgrade: websocket\r\n
Origin: http://coolaf.com\r\n
Sec-WebSocket-Version: 13\r\n
Sec-WebSocket-Key: NWtO9QluAUwF/bxf0ekD+w==\r\n
//Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\n
\r\n
```

```
//服务器回应客户端的握手请求
HTTP/1.1 101 Switching Protocols\r\n
Upgrade: websocket\r\n
Sec-Websocket-Accept: AD5ZXYSaZhvPC+A3XbLjSiUNae0=\r\n
Server: BTCMEXWebsocketServer 1.0.0\r\n
Connection: Upgrade\r\n
Sec-WebSocket-Version: 13\r\n
Sec-WebSocket-Extensions: permessage-deflate; client_no_context_takeover\r\n
Date: 20190901100821\r\n
\r\n
```



```
      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-------+-+-------------+-------------------------------+
     |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
     |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
     |N|V|V|V|       |S|             |   (if payload len==126/127)   |
     | |1|2|3|       |K|             |                               |
     +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
     |     Extended payload length continued, if payload len == 127  |
     + - - - - - - - - - - - - - - - +-------------------------------+
     |                               |Masking-key, if MASK set to 1  |
     +-------------------------------+-------------------------------+
     | Masking-key (continued)       |          Payload Data         |
     +-------------------------------- - - - - - - - - - - - - - - - +
     :                     Payload Data continued ...                :
     + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
     |                     Payload Data continued ...                |
     +---------------------------------------------------------------+
```



FIN = 1 123456789

FIN = 0 456

FIN = 1 789



payload length 的说明：

```
0~127
0~125
126 接下来两个字节表示包体长度 0xFFFF（65535）
127 接下来的8个字节表示包长
```







### 网络编程相关的命令

```
ping
telnet
netstat 
nc 
lsof
curl
tcpdump
```



 

### 心跳包的设计

https://mp.weixin.qq.com/s/CmtXhax60uBDrfZ-NWr7OA



 ### 断线重连的设计



### 自动升级功能如何设计



### 日志类

- 支持适用不同场景的日志级别；
- 可以打印日志文件名和行号；
- 支持rollSize
- 





### 数据库连接的管理与数据操作

短连接

连接池

```
建立n个连接
执行一条sql
```



### 缓存机制的设计

```
网络请求（从远端服务器请求，远端从磁盘中读取）
从磁盘中读
从内存
```

缓存失效、缓存穿透、缓存雪崩





### 信号的处理



### 程序初始化

```
getopt
```





 ### 端口重用与地址重用



惊群效应

listensocket => addr+port 



listensocket 1

listensocket 2

listensocket 3



### Linux 与 Windows 平台在网络通信部件上的一些区别

https://blog.csdn.net/analogous_love/article/details/60761528



### 答疑环节

