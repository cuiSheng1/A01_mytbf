# mytbf --kmist

#### 介绍
实现简单的流量控制

#### 软件架构
软件包括 
libmystb.so   //封装库
libmystb.c    //源码
libmystb.h    //接口文件

tcpClient.c    //测试文件 客户端
tcpServer.c    //测试文件 服务端

makefile       //编译makefile

测试说明：
    执行./server 8888 指定8888端口
    另打开一个窗口执行 ./client 127.0.0.1 8888 用来跟服务器通信
    命令：客户端发送 time 就能向服务器请求当前时间，服务器返回当前时间

    流量控制： 每个客户端初始token数量为5个，每次向服务器发送一次请求消耗5个token，每10秒恢复5个token。
    也就是每10秒可以访问一次服务器。


#### 安装教程

1.  在linux环境中，执行make，make自动编译
2.   ./server 8888 启动服务器
3.   ./client 127.0.0.1 8888 启动客户端
4.    客户端每10秒能向服务器请求1次数据

#### 使用说明

1.  xxxx
2.  xxxx
3.  xxxx


