### 1. rhino

invokes the JavaScript shell for running scripts in batch mode or interactive

### 2. getconf

获取cpu数目

```sh
getconf _NPROCESSORS_ONLN
```

### 3. blockdev —— 通过命令行来调用块设备的ioctl指令

e.g 获取磁盘的物理扇区和逻辑扇区大小

```sh
blockdev --getss /dev/sdx
blockdev --getpbsz /dev/sdx
```

### 4. blkdiscard —— 

### 5. abrt-cli list

### 6. sosreport

### Redhat Linux 7
Oct 19 12:34:39 Installed: 1:perl-Error-0.17020-2.el7.noarch
Oct 19 12:34:40 Installed: libgnome-keyring-3.8.0-3.el7.x86_64
Oct 19 12:34:40 Installed: perl-TermReadKey-2.30-20.el7.x86_64
Oct 19 12:34:41 Installed: git-1.8.3.1-6.el7_2.1.x86_64
Oct 19 12:34:41 Installed: perl-Git-1.8.3.1-6.el7_2.1.noarch
Oct 19 13:44:07 Installed: python-crypto-2.6.1-1.1.el7.x86_64
Oct 19 13:44:07 Installed: sshpass-1.05-5.el7.x86_64
Oct 19 13:44:07 Installed: python-httplib2-0.9.1-2.1.el7ost.noarch
Oct 19 13:44:07 Installed: python-pyasn1-0.1.6-2.el7.noarch
Oct 19 13:44:07 Installed: python-keyczar-0.71c-2.el7aos.noarch
Oct 19 13:44:07 Installed: python-ecdsa-0.11-4.el7ost.noarch
Oct 19 13:44:07 Installed: python-paramiko-1.15.2-1.el7aos.noarch
Oct 19 13:44:08 Installed: libyaml-0.1.4-11.el7_0.x86_64
Oct 19 13:44:08 Installed: PyYAML-3.10-11.el7.x86_64
Oct 19 13:44:08 Installed: ansible-1.9.4-1.el7aos.noarch
Oct 19 13:44:09 Installed: ceph-ansible-1.0.5-32.el7scon.noarch
