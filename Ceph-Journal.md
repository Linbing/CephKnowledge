# Ceph Journal

![DoubleWrite](img/osd_double_write.jpg)

## 简介

与一般的日志文件系统类似，Ceph OSD的日志也是一种事务日志。它是基于文件系统的OSD的关键组成部分，提供存储系统
所需的一致性保证。

Ceph OSD使用日志有两个原因([CephDocument](http://docs.ceph.com/docs/jewel/rados/configuration/journal-ref/))：

1.速度：日志使得Ceph OSD Daemon进程能够快速的提交小IO。Ceph将小的随机IO顺序地写入日志，让后端文件系统有更多
时间来合并IO，提升系统的突发负载能力。然而，这可能带来剧烈的性能抖动，表现为一段时间内的高速写入，而这之后
一段时间内没有任何写入直至文件系统追上日志的进度

2.一致性：Ceph OSD Daemon进程需要一个文件系统接口来保证多个操作的原子性。TODO

dio: 指定了dio后，在FileJournal的_open函数中会指定用`O_DIRECT|O_DSYNC`方式打开journal文件

O_DIRECT: 直接IO，IO高速缓存开销最小化，会降低性能
O_DSYNC: 告诉内核，当向文件写入数据的时候，只有当数据写到磁盘上时，才算写入完成

Problem:
Journal机制使得一次写请求在OSD端变为两次写操作(同步写journal，异步写object)

Ceph有一个原则，就是所有存储的不管是块设备，对象存储还是文件存储，最后都转化成了底层的对象Object，这个Object
包含3个元素，data、xattr和omap。data就是保存对象的数据；xattr是保存对象的扩展属性，每个对象文件都可以设置文件
的属性，这是属性是一个Key/Value值对，但是受文件系统的限制，Key/Value对的个数和每个value的大小都进行了限制。
如果设置的对象的Key/Value不能存储在文件的扩展属性中，可以使用omap来保存。omap实际上是保存到了key/value值对
的数据库leveldb中，在这里的value值限制要比xattr中好的多。

日志写入流程：

日志写入的入口函数为：journal的submit_entry函数

1.通过`writeq_lock`和`completions_lock`首先把请求加入到writeq和completions两个队列中

2.FileJournal的内部线程`write_thread`，对应的run函数`write_thread_entry`从writeq队列中获取请求，通过aio写入
日志磁盘

3.`write_finish_thread`(这里分析aio情况)检查是否有aio请求完成，如果完成，则把请求从completions队列删除，并添加到
Finisher队列里

4.Finisher队列调用日志的回调函数完成

日志的同步：

在FileStore::mount方法中，会创建sync线程 `sync_thread.create()`，该线程的入口函数为：void FileStore::sync_entry()

该函数定期执行同步操作，当同步时，调用tp.pause使FileStore的op_wq的线程池停止，等待正在应用的日志完成。然后调用fsync
同步内存中的数据到数据盘，当同步完成后，就可以丢弃相应的日志，释放相应的日志空间。

日志数据：
一系列的事务被传递到日志。在日志处理过程中，首先用encoding.h中定义的事务函数编码这些事务。每种类型的事务都在
ObjecctStore.h中定义了自己的解码器。需要注意的是日志项中不仅包括元数据，还包含IO数据。也就是说，一个写事务包括
了它的IO数据。事务编码后，传递到日志也就被当作一个不透明的数据块。

Metadata Only Journal Mode:
RADOS - METADATA-ONLY JOURNAL MODE
Summary
Currently the Ceph community is thinking of eliminating the double write
penalty of write ahead logging, newstore is a great design which implements
create, append operations in an copy on read way, while maintaining all
the original semantics. This makes newstore a general purpose optimization,
especially suitable for the write once scenarios. Metadata-only journal mode
intends to do in a more aggressive way, that is, not journal object data at all.
This applies to two major kinds of situations, one is that the atomicity for
object data modification may not need, for example, RBD to simulate a disk
in cloud platform. The second is those double journaling situations, for example,
cache tiering, while cache pool has already provided the durability, when dirty
objects are written back, they theoretically need not go through the journaling
process of base pool, since the flusher could always replay the write operation.
Metadata-only journal mode, to some extent, resembles the data=ordered journal
mode in ext4. With such journal mode is on, object data are written directly to
their ultimate location, when data written finished, metadata are written into the
journal. It guarantees the consistency in terms of RADOS name space, and the data
consistency among object copies. However, the object data may not be correct.
Later we will demonstrate that this rarely happens.

Owners

Li Wang (liwang@ubuntukylin.com)
Yunchuan Wen (yunchuanwen@ubuntukylin.com)
Name

Interested Parties
If you are interested in contributing to this blueprint, or want to be a "speaker" during the Summit session, list your name here.
Name (Affiliation)
Name (Affiliation)
Name

Current Status
Please describe the current status of Ceph as it relates to this blueprint. Is there something that this replaces? Are there current features that are related?

Detailed Description
We have two options,
The first option

1 Write object data
2 Commit metadata to journal
3 Submit metadata

This option makes minimal revision to current framework, enjoy the simplicity. In most cases, this will not introduce problem relying
on the powerful peering and client resent mechanism. The only problematic situation is PG down as a whole, and client also down,
 in that case, the guest fs in the vm will possibly recover it to consistent by fsck and journal replaying. So it just to leave scrub to find
 and fix this by randomly synchronizing one of the copies to others.

The second option

1 Commit transaction A into journal, to add a UNSTABLE flag in object metadata, add a pglog record, and update the pglog version
2 Write data to object
3 Commit transaction B into journal, to update metadata, delete the flag, add a pglog record, and update the pglog version
4 Submit metadata

As long as one osd in the pg has succeeded, the pg will be recovered to
a consistent and correct state by peering; If the pg down as a whole,
there are the following situations, 
(1) None of the osds finishes step 1, nothing happen; 
(2) At least one of the osds finishes step 3, journaling and
peering will recover the pg to a consistent and correct state; 
(3) none of the osds has finished step (3), and at least one of the osds has 
finished step (1), this is the only potentially problematical situation, 
in this case, peering will synchronize the UNSTABLE flag to other osds in the pg. 
For object read and write, if found UNSTABLE flag in object metadata, it will let the operation wait and
start a recovery to randomly choose one osd to synchronize its 
content of written area to other copies. During scrub, it will also check the flag and do the recvery.

Work items
This section should contain a list of work tasks created by this blueprint. Please include engineering tasks as well as related build/release
and documentation work. If this blueprint requires cleanup of deprecated features, please list those tasks as well.
[blueprint](http://tracker.ceph.com/projects/ceph/wiki/Rados_-_metadata-only_journal_mode/16)

### Journal机制分析

### 模块调试方法

### 数据结构

### 接口、架构

### 流程

### 并发机制

### 参考链接

[Ceph IO模式分析](http://www.openstack.cn/?p=4270)
[Ceph OSD日志分析](http://bbs.ceph.org.cn/article/42)
[Ceph数据存储之路 Object的attr和omap操作](https://my.oschina.net/u/2460844/blog/604530)
[Ceph NewStore存储引擎介绍](https://www.cnblogs.com/wuhuiyuan/p/ceph-newstore-intro.html?hmsr=toutiao.io&utm_medium=toutiao.io&utm_source=toutiao.io)

[Ceph Journal](http://irq0.org/articles/ceph/journal)
