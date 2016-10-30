# Daily Notes

## 2016-10-26

去除"journal_force_aio=true"后4k随机写性能提高原因

## 2016-10-27

 熟悉几个新的api
* posix_fallocate: allocate file space
* posix_memalign: posix_memalign, aligned_alloc, memalign, valloc, pvalloc - allocate aligned(对齐的) memory
* pread/pwrite: read from or write to a file descriptor at a given offset

了解了CephContext的初始化流程和日志输出