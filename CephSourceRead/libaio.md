# Linux下异步IO(libaio)的使用

Linux异步IO是Linux内核中提供的一个相当新的增强功能。它是2.6内核的一个标准特性，在2.4的补丁
中也可以找到它。AIO的背后思想是允许进程发起很多IO操作，而不用阻塞或等待任何操作完成。稍后
或在接收到IO操作完成的通知时，进程就可以检索IO操作的结果。

`aio_*`系列的调用是glibc提供的，是glibc用线程＋阻塞调用来模拟的，性能很差。所以推荐使用
[libaio](http://oss.oracle.com/projects/libaio-oracle/)。在CentOS中只需通过`yum install
libaio-devel`即可安装。


# 参考链接

[libaio的使用及性能](http://blog.yufeng.info/archives/741)

[使用异步IO提高应用程序的性能](https://www.ibm.com/developerworks/cn/linux/l-async/)
