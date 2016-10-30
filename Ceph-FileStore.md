# Ceph FileStore

读写流程分析

根据日志的不同，FileStore有三种写的方式：

1.Writeahead方式：一般xfs采用这种日志，在这种情况下，数据先写入日志，并等待日志同步到日志磁盘上，然后再写到对象对应的
日志文件中

2.Parallel方式：Btrfs和ZFS采用这种方式，日志和数据是同时（并行）写入。由于Btrfs和ZFS支持checkpoint操作，如果数据
在写入过程中出错，可以rollback，保证数据的一致性

3.无日志的方式：在这种情况下，数据之间写入并同步到磁盘中。这种模式下只支持append模式的写入，不支持随机写。

[Ceph OS模块介绍及读写流程分析](http://www.voidcn.com/blog/changtao381/article/p-4364519.html)