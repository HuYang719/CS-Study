# 计算机网络

### 什么是TCP协议？

TCP是传输控制协议，Transmission Control Protocol, 是一种面向连接，可靠的，基于字节流的传输层通信协议。

### TCP的运作方式是什么样的？

TCP的运行可以分为三个阶段：连接创建(connection establishment)、数据传送(data transfer)和连接终止(connection termination)。
- TCP通过三次握手创建连接。
- 在数据传送部分，有很多机制来保障TCP的可靠性和鲁棒性。它们包括：使用序号，对收到的TCP报文段进行排序以及检测重复的数据；使用校验和检测报文段的错误，即无错传输；使用确认和计时器来检测和纠正丢包或延时；流控制（Flow control）；拥塞控制（Congestion control）；丢失包的重传。
- 连接终止使用了四次握手(four-way handshake)，连接的每一侧都独立地被终止。




### 什么是三次握手 (three-way handshake)？
![三次握手](Pictures/handshake.png)

- 第一次握手：Client将SYN置1，随机产生一个初始序列号seq发送给Server，进入SYN_SENT状态；
- 第二次握手：Server收到Client的SYN=1之后，知道客户端请求建立连接，将自己的SYN置1，ACK置1，产生一个acknowledge number=sequence number+1，并随机产生一个自己的初始序列号，发送给客户端；进入SYN_RCVD状态；
- 第三次握手：客户端检查acknowledge number是否为序列号+1，ACK是否为1，检查正确之后将自己的ACK置为1，产生一个acknowledge number=服务器发的序列号+1，发送给服务器；进入ESTABLISHED状态；服务器检查ACK为1和acknowledge number为序列号+1之后，也进入ESTABLISHED状态；完成三次握手，连接建立。

### 三次握手中，如果服务器没有收到客户端的ACK会怎么样？

如果服务器端接到了客户端发的SYN后回了SYN-ACK后客户端掉线了，服务器端没有收到客户端回来的ACK，那么，这个连接处于一个中间状态，即没成功，也没失败。于是，服务器端如果在一定时间内没有收到的TCP会**重发SYN-ACK**。在Linux下，默认重试次数为5次，重试的间隔时间从1s开始每次都翻倍，5次的重试时间间隔为1s, 2s, 4s, 8s, 16s，总共31s，**第5次发出后还要等32s才知道第5次也超时了**，所以，总共需要 1s + 2s + 4s+ 8s+ 16s + 32s = 63s，TCP才会断开这个连接。使用三个TCP参数来调整行为：tcp_synack_retries 减少重试次数；tcp_max_syn_backlog，增大SYN连接数；tcp_abort_on_overflow决定超出能力时的行为。

### 什么说TCP协议是可靠的？