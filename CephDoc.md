# [Ceph Storage Cluster](http://docs.ceph.com/docs/master/rados/)

## Configuration

### Disk and Filesystems

#### Hard Drive Prep

Ceph aims for data safety, which means that when the Ceph Client receives notice
that data was written to a storage drive, that data was actually written to the
storage drive. For old kernels (<2.6.33), disable the write cache if the journal
is on a raw drive. Newer kernels should work fine.

Use hdparm to disable write caching on the hard disk:

```sh
sudo hdparm -W 0 /dev/hda 0
```

#### Filesystems

Ceph OSD Daemons rely heavily upon the stability and performance of the
underlying filesystem.

We currently recommend XFS for production deployments.

We used to recommend btrfs for testing, development, and any non-critical
deployments becuase it has the most promising set of features. However, we now
plan to avoid using a kernel file system entirely with the new BlueStore backend.
btrfs is still supported and has a comparatively compelling set of features,
but be mindful of its stability and support status in your Linux distribution.

We recommend against using ext4 due to limitations in the size of xattrs it can
store, and the problems this causes with the way Ceph handles long RADOS object
names. Although these issues will generally not surface with Ceph clusters using
only short object names (e.g., an RBD workload that does not include long RBD
image names), other users like RGW make extensive use of long object names and
can break.

### Configuration Ceph

For your convenience, each daemon has a series of default values(common/config_opts.h)

#### The Configuration File

The Ceph Configuration file defines:

* Cluster Identify
* Authentication settings
* Cluster membership
* Host names
* Host addresses
* Paths to keyrings
* Paths to journals
* Paths to data
* Other runtime options

The default Ceph Configuration file locations in sequential order include:

1. $CEPH_CONF
2. -c path/path(the -c command line argument)
3. /etc/ceph/ceph.conf
4. ~/.ceph/config
5. /ceph.conf

Config Sections

[global] 影响ceph集群中所有的守护进程

[osd] 影响所有ceph-osd进程，会覆盖global中相同的设置

[mon] 影响所有ceph-mon进程，会覆盖global中相同的设置

[mds] 影响所有ceph-mds进程，会覆盖global中相同的设置

[client] 影响所有的ceph客户端(已经挂载的ceph文件系统和块设备)

如果指定的是一个ceph对象网关客户端，则要指定进程和实例名称，并用句号分隔，如

[client.radosgw.instance-name]

Metavariables(元变量)

元变量简化了ceph的配置，当配置文件中设置了一个元变量的时候，ceph会展开成一个确切的值。ceph支持以下元变量：

$cluster：展开为ceph集群的名称，当在同样的硬件上运行多个ceph集群时很有用，默认值是`ceph`

$type: 展开为osd、mon或者mds，由实例的类型决定

$id: 展开为进程标示符，比如osd.0，标示符为0，mds.a的标识符为a

$host: 展开为实例的主机名

$name: 展开为$type.$id

$pid: 展开为进程id

#### OS RECOMMENDATIONS

Linux Kernel

Ceph Kernel Client

* 4.1.4 or later
* 3.16.3 or later(rbd deadlock regression in 3.16.[0-2])
* `NOT 3.15.*(rbd deadlock regression)`
* 3.14

These are considered pretty old, but if you must:

* `3.10.*`

B-tree File system(btrfs)

* If you use the btrfs file system with ceph, we recommend using a recent linux kernel(3.14 or later)

#### Journal Setting(Journal Config Reference)

Ceph使用journal的两个原因：速度和稳定性

* 速度：journal使得osd进程能够更快地提交小块数据的写入操作。将小块随机IO合并为顺序IO

* 稳定性：

Ceph OSD支持下列journal配置：

* journal dio: 启用journal的直接写入模式，需要`journal block align`设置为true

* journal aio: 启用libaio异步写journal，需要`journal dio`设置为true

* journal max write bytes: journal每次写入的最大字节数

* journal max write entries: journal每次写入的最大条目数

* journal queue max ops: 每次允许在队列中的最大操作数


## Development

下载ceph源码后编译，执行`./install-deps.sh`安装依赖包，安装完成后执行`do_cmake.sh`生成Makefile，再
执行make进行编译。

编译后的文件在build目录下，运行测试集群：

```sh
cd build
../src/vstart.sh -d -x -n -l
```

生成rpm包，先执行`make-srpm.sh`来生成rpm源码包，安装该源码包。再安装rpm-build

```sh
yum install rpm-build
```

之后在rpmbuild/SPECS中执行`rpm -ba ceph.spec`

### 安装编译生成的rpm包

```sh
yum install java boost-date-time boost-python

rpm -ivh ceph-base* ceph-osd* ceph-mds* ceph-mgr* ceph-mon* ceph-common* \
ceph-selinux* librbd* librados* libcephfs* librgw* python-cephfs* python-rados* python-rbd*
```

### 删除ceph包

```sh
yum remove ceph ceph-common ceph-base ceph-deploy 
yum remove python-cephfs python-rados 
yum remove librados2 librbd1 libcephfs1 libradosstriper1
```


[OS RECOMMENDATIONS](http://docs.ceph.com/docs/master/start/os-recommendations/)

#### Ceph CPU Profiling

[ceph cpu-profiling](http://docs.ceph.com/docs/master/rados/troubleshooting/cpu-profiling/)
