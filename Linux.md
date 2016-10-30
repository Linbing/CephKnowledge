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

### 7. generate_randfile

### 8. hping3

### 9. ulimit

[增大linux系统的openfiles上限](http://www.chengweiyang.cn/2015/11/14/how-to-enlarge-linux-open-files-upper-cell/)

### 10. tcprewrite

[tcprewrite](http://momomoxiaoxi.com/2016/01/25/Tcpreplay/)
