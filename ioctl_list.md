## ioctl命令列表

### BLKGETSIZE64,BLKGETSIZE —— 获取块设备的大小或者是扇区数

```plain
btrfs_ioc_test.c:83:	r = ioctl(to, BTRFS_IOC_CLONE_RANGE, &a);
btrfs_ioc_test.c:102:	r = ioctl(fd, BTRFS_IOC_SNAP_CREATE_ASYNC, &async_args);
btrfs_ioc_test.c:118:	r = ioctl(fd, BTRFS_IOC_SNAP_DESTROY, &volargs);
btrfs_ioc_test.c:128:	int r = ioctl(fd, BTRFS_IOC_WAIT_SYNC, &transid);
cephfs.cc:69:    err = ioctl(fd, CEPH_IOC_GET_LAYOUT, (unsigned long)&layout);
cephfs.cc:85:    err = ioctl(fd, CEPH_IOC_GET_DATALOC, (unsigned long)&location);
cephfs.cc:112:    err = ioctl(fd, ioctl_num, (unsigned long)&layout);
cephfs.cc:127:    err = ioctl(fd, CEPH_IOC_GET_LAYOUT, (unsigned long)&layout);
cephfs.cc:139:      err = ioctl(fd, CEPH_IOC_GET_DATALOC, (unsigned long)&location);
client/fuse_ll.cc:485:static void fuse_ll_ioctl(fuse_req_t req, fuse_ino_t ino, int cmd, void *arg, struct fuse_file_info *fi,
client/fuse_ll.cc:505:      fuse_reply_ioctl(req, 0, &l, sizeof(struct ceph_ioctl_layout));
client/test_ioctls.c:39:        err = ioctl(fd, CEPH_IOC_GET_LAYOUT, (unsigned long)&l);
client/test_ioctls.c:51:        err = ioctl(fd, CEPH_IOC_SET_LAYOUT, (unsigned long)&l);
client/test_ioctls.c:60:	err = ioctl(fd, CEPH_IOC_GET_LAYOUT, (unsigned long)&l);
client/test_ioctls.c:70:	err = ioctl(fd, CEPH_IOC_GET_DATALOC, (unsigned long)&dl);
client/test_ioctls.c:100:        err = ioctl(fd, CEPH_IOC_SET_LAYOUT_POLICY, (unsigned long)&l);
client/test_ioctls.c:114:        err = ioctl(fd, CEPH_IOC_GET_LAYOUT, (unsigned long)&l);
common/blkdev.cc:28:  int ret = ::ioctl(fd, BLKGETSIZE64, psize);
common/blkdev.cc:31:  int ret = ::ioctl(fd, BLKGETSIZE, &sectors);
common/blkdev.cc:161:  return ioctl(fd, BLKDISCARD, range);
common/blkdev.cc:170:  int ret = ::ioctl(fd, DKIOCGETBLOCKSIZE, &blocksize);
common/blkdev.cc:173:    ret = ::ioctl(fd, DKIOCGETBLOCKCOUNT, &nblocks);
common/blkdev.cc:196:  int ret = ::ioctl(fd, DIOCGMEDIASIZE, psize);
os/BtrfsFileStoreBackend.cc:81:      r = ::ioctl(fd, BTRFS_IOC_CLONE_RANGE, &clone_args);
os/BtrfsFileStoreBackend.cc:105:  r = ::ioctl(get_basedir_fd(), BTRFS_IOC_SUBVOL_CREATE, &vol_args);
os/BtrfsFileStoreBackend.cc:119:  r = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_CREATE, &vol_args);
os/BtrfsFileStoreBackend.cc:125:    r = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_DESTROY, &vol_args);
os/BtrfsFileStoreBackend.cc:155:  r = ::ioctl(get_basedir_fd(), BTRFS_IOC_START_SYNC, &transid);
os/BtrfsFileStoreBackend.cc:164:    r = ::ioctl(get_basedir_fd(), BTRFS_IOC_WAIT_SYNC, &transid);
os/BtrfsFileStoreBackend.cc:190:      r = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_DESTROY, &vol_args);
os/BtrfsFileStoreBackend.cc:197:    r = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_CREATE_V2, &async_args);
os/BtrfsFileStoreBackend.cc:204:      r = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_DESTROY, &vol_args);
os/BtrfsFileStoreBackend.cc:220:  r = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_DESTROY, &vol_args);
os/BtrfsFileStoreBackend.cc:281:  if (::ioctl(get_basedir_fd(), BTRFS_IOC_SUBVOL_CREATE, (unsigned long int)&volargs) < 0) {
os/BtrfsFileStoreBackend.cc:383:    int r = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_CREATE_V2, &async_args);
os/BtrfsFileStoreBackend.cc:400:    int r = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_CREATE, &vol_args);
os/BtrfsFileStoreBackend.cc:416:  int ret = ::ioctl(get_op_fd(), BTRFS_IOC_WAIT_SYNC, &transid);
os/BtrfsFileStoreBackend.cc:436:  int ret = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_DESTROY, &vol_args);
os/BtrfsFileStoreBackend.cc:457:  ret = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_CREATE, &vol_args);
os/BtrfsFileStoreBackend.cc:474:  int ret = ::ioctl(get_basedir_fd(), BTRFS_IOC_SNAP_DESTROY, &vol_args);
os/BtrfsFileStoreBackend.cc:487:  int ret = ::ioctl(get_op_fd(), BTRFS_IOC_SYNC);
os/BtrfsFileStoreBackend.cc:541:  err = ::ioctl(to, BTRFS_IOC_CLONE_RANGE, &a);
os/FileJournal.cc:1660:	dout(1) << __func__ << "ioctl(BLKDISCARD) error:" << cpp_strerror(errno) << dendl;
os/GenericFileStoreBackend.cc:231:  if (ioctl(fd, FS_IOC_FIEMAP, fiemap) < 0) {
os/GenericFileStoreBackend.cc:251:  if (ioctl(fd, FS_IOC_FIEMAP, fiemap) < 0) {
os/XfsFileStoreBackend.cc:58:  if (ioctl(fd, XFS_IOC_FSGETXATTR, &fsx) < 0) {
os/XfsFileStoreBackend.cc:75:  if (ioctl(fd, XFS_IOC_FSSETXATTR, &fsx) < 0) {
test/librbd/fsx.cc:666:	if (ioctl(ctx->krbd_fd, BLKDISCARD, &range) < 0) {
test/librbd/fsx.cc:681:	if (ioctl(ctx->krbd_fd, BLKGETSIZE64, &bytes) < 0) {
test/old/test_seek_read.c:21:  int r = ioctl(fd, BLKGETSIZE64, &bytes);
test/old/test_setlayout.c:16:	int r = ioctl(fd, CEPH_IOC_GET_LAYOUT, &l, sizeof(l));
test/old/test_setlayout.c:22:	r = ioctl(fd, CEPH_IOC_SET_LAYOUT, &l, sizeof(l));
test/old/test_short_seek_read.c:21:  int r = ioctl(fd, BLKGETSIZE64, &bytes);
```
