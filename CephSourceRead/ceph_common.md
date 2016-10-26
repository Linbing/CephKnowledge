# 公共库源码分析

## BackTrace

* iosfwd: 为输入输出对象提供向前的声明

## CephContext

CephContext表示单个library使用者所拥有的上下文，在同一个进程中可以有多个CephContext。对于daemon和utility程序来
说，只能有一个CephContext。CephContext包含配置，dout对象和其他任何每次函数调用需要传递给libcommon的东西。

引用计数: `atomic_t nref` libatomic
配置信息: `md_config_t *_conf`


## Object
