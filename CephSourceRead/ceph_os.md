# Ceph对象存储源码分析

ObjectStore这个类中的create函数会根据配置的对象存储类型来初始化相应的实例。在hammer中，有三种存储
引擎（filestore,memstore和keyvaluestore）, 在jewel中，有四种存储引擎（filestore,memstore,bluestore
和kstore）。

## FileStore

## MemStore

## KeyValueStore

## BlueStore

## KStore
