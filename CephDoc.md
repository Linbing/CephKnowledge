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


