# 

```sh
group $osd {
    cpu {
        cpu.rt_period_us="1000000";
        cpu.rt_runtime_us="0";
        cpu.cfs_quota_us="-1";
        cpu.cfs_period_us="100000";
        cpu.shares="1024";
    }
    cpuset {
        cpuset.cpus = "0-23";
        cpuset.mems = "0";
    }
    memory {
        memory.limit_in_bytes=2G;
        memory.memsw.limit_in_bytes=8G;
        memory.swappiness=0;
        #memory.oom_control=1;
    }
}

group $mon {
    cpu {
        cpu.rt_period_us="1000000";
        cpu.rt_runtime_us="0";
        cpu.cfs_quota_us="-1";
        cpu.cfs_period_us="100000";
        cpu.shares="1024";
    }
    cpuset {
        cpuset.cpus = "0-23";
        cpuset.mems = "0";
    }
    memory {
        memory.limit_in_bytes=1G;
        memory.memsw.limit_in_bytes=4G;
        memory.swappiness=0;
        memory.oom_control=1;
    }
}
```
