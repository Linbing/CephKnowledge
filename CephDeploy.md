## ceph部署

### 使用ceph-deploy部署

#### 添加ceph源

```sh
vim /etc/yum.repos.d/ceph.repo

[ceph]
name=ceph-hammer-x86_64
baseurl=http://mirrors.163.com/ceph/rpm-hammer/el7/x86_64
gpgcheck=0
enabled=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-7

[ceph-noarch]
name=ceph-hammer-noarch
baseurl=http://mirrors.163.com/ceph/rpm-hammer/el7/noarch
gpgcheck=0
enabled=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-7

[ceph-source]
name=ceph-hammer-source
baseurl=http://mirrors.163.com/ceph/rpm-hammer/el7/SRPMS
gpgcheck=0
enabled=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-7
```

#### 设置主机名并创建无密码ssh登录

分别设置主机名
hostnamectl  set-hostname cephnode11
hostnamectl  set-hostname cephnode12
hostnamectl  set-hostname cephnode13

添加主机名和ip至/etc/hosts
192.168.104.11  cephnode11
192.168.104.12  cephnode12
192.168.104.13  cephnode13

创建公钥私钥对
ssh-keygen
ssh-copy-id cephnode12
ssh-copy-id cephnode13

#### 安装epel源

```sh
rpm -vih http://mirrors.sohu.com/fedora-epel/7/x86_64/e/epel-release-7-2.noarch.rpm
```
#### 安装ceph-deploy和ceph

```sh
yum install ceph-deploy ceph
```

### 磁盘初始化
安装sgdisk

```sh
yum install -y gdisk
```

mbr转为gpt
sgdisk -g /dev/sdx

清除之前的分区
sgdisk -o /dev/sdx

#### 创建ceph集群

```sh
mkdir ceph-cluster
cd ceph-cluster
```

新建集群

```sh
ceph-deploy new cephnode11 cephnode12 cephnode13
```

创建monitor
```sh
ceph-deploy mon create cephnode11 cephnode12 cephnode13
```

初始化mon
```sh
ceph-deploy mon create-initial
```

OSD部署
```sh
ceph-deploy osd prepare cephnode11:[data]:[journal] cephnode12:[data]:[journal] ...
ceph-deploy osd activate
```

复制配置文件和密钥
```sh
ceph-deploy admin cephnode11 cephnode12 cephnode13
```

