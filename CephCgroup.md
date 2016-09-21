# Cgroup在Ceph中的应用

## 一、Cgroup简介

Cgroups是control groups的缩写，是Linux内核提供的一种可以限制、记录、隔离进程/进程组所使用的物理资源（如：CPU, Memory, IO等）的机制。
最初由Google的工程师提出，后来被整合进Linux内核。

Cgroups最初的目标是为资源管理提供的一个统一的框架，既整合现有的Cpuset等子系统，也为未来开发新的子系统提供接口。现在的Cgroups适用于多种应
用场景，从单个进程的资源控制，到实现操作系统层次的虚拟化（OS Level Virtualization）。Cgroups提供以下功能：

1. 限制进程组可以使用的资源数量（Resource limiting ）。比如：Memory子系统可以为进程组设定一个Memory使用上限，一旦进程组使用的内存达到限
额再申请内存，就会出发OOM（out of  memory）。

2. 进程组的优先级控制（Prioritization）。比如：可以使用CPU子系统为某个进程组分配特定CPUshare。

3. 进程组隔离（Isolation）。比如：使用ns子系统可以使不同的进程组使用不同的namespace，以达到隔离的目的，不同的进程组有各自的进程、网络、文件系统挂载空间。

4. 记录进程组使用的资源数量（Accounting）。比如：可以使用Cpuacct子系统记录某个进程组使用的CPU时间

5. 进程组控制（Control）。比如：使用freezer子系统可以将进程组挂起和恢复。
