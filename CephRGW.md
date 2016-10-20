# 部署多个rgw实例

为了解决单个rgw无法满足并发测试的需求，需要部署多个rgw实例供测试。可以在单机上部署多个rgw实例，然后使用nginx做负载均衡，
如果还达不到要求，可以在多台服务器上部署rgw实例。

## 在单个服务器上部署多个rgw实例

### 1. 部署第一个rgw实例

这里需要将httpd监听的端口改为8081，8080端口供nginx使用

```sh
#!/bin/bash
yum clean all
yum install -y httpd openssl ceph-radosgw radosgw-agent
rpm -ivh ftp://195.220.108.108/linux/centos/7.2.1511/os/x86_64/Packages/mod_fcgid-2.3.9-4.el7.x86_64.rpm
HOSTNAME=192.168.89.200
cat >> /etc/httpd/conf.d/swift.conf<<EOF
ServerName ${HOSTNAME}
Listen ${HOSTNAME}:8080
<IfModule !proxy_fcgi_module>
LoadModule proxy_fcgi_module modules/mod_proxy_fcgi.so
</IfModule>
EOF
systemctl restart httpd.service
#为radosgw添加key
cat >>/dev/null <<EOF
ceph-authtool --create-keyring /etc/ceph/ceph.client.radosgw.keyring
chmod +r /etc/ceph/ceph.client.radosgw.keyring
ceph-authtool /etc/ceph/ceph.client.radosgw.keyring -n client.radosgw.gateway --gen-key
ceph-authtool -n client.radosgw.gateway --cap osd 'allow rwx' --cap mon 'allow rwx' /etc/ceph/ceph.client.radosgw.keyring
ceph -k /etc/ceph/ceph.client.admin.keyring auth add client.radosgw.gateway -i /etc/ceph/ceph.client.radosgw.keyring
EOF
#修改ceph.conf添加radosgw配置
cat >> /etc/ceph/ceph.conf <<EOF
[client.radosgw.gateway]
host = controller
keyring = /etc/ceph/ceph.client.radosgw.keyring
# rgw socket path = /var/run/ceph/ceph.radosgw.gateway.fastcgi.sock
rgw frontends = fastcgi socket_port=9000 socket_host=0.0.0.0
log file = /var/log/radosgw/client.radosgw.gateway.log
rgw print continue = false
rgw keystone url = http://${HOSTNAME}:35357
rgw keystone admin token = jjjjjj
rgw keystone accepted roles = admin, _member_
rgw keystone token cache size = 500
rgw keystone revocation interval = 500
rgw s3 auth use keystone = true
nss db path = /var/ceph/nss
EOF
mkdir -p /var/log/radosgw/
mkdir -p /var/ceph/nss
#创建公钥供keystone操作nss db
openssl x509 -in /etc/keystone/ssl/certs/ca.pem -pubkey | certutil -d /var/ceph/nss -A -n ca -t "TCu,Cu,Tuw"
openssl x509 -in /etc/keystone/ssl/certs/signing_cert.pem -pubkey | certutil -A -d /var/ceph/nss -n signing_cert -t "P,P,P"
#创建swift服务
keystone service-create --name swift --type object-store --description "OpenStack object store"
#keystone endpoint-create --service-id $(keystone service-list | awk '/ swift / {print $2}') --publicurl http://172.16.197.31:8080/swift/v1 --internalurl http://172.16.197.31:8080/swift/v1 --adminurl http://172.16.197.31:8080/swift/v1 --region region150
keystone endpoint-create --service-id $(keystone service-list | awk '/ swift/ {print $2}') --publicurl http://172.16.50.25:8080/swift/v1 --internalurl http://172.16.50.25:8080/swift/v1 --adminurl http://172.16.50.25:8080/swift/v1 --region region150
mkdir -p /var/run/ceph
mkdir -p /var/lib/ceph/radosgw/ceph-radosgw.gateway
chown apache:apache /var/run/ceph
#添加rgw.conf文件
cat >> /etc/httpd/conf.d/rgw.conf <<EOF
<VirtualHost *:8080>
ServerName localhost
DocumentRoot /var/www/html
ErrorLog /var/log/httpd/rgw_error.log
CustomLog /var/log/httpd/rgw_access.log combined
# LogLevel debug
RewriteEngine On
RewriteRule .* - [E=HTTP_AUTHORIZATION:%{HTTP:Authorization},L]
SetEnv proxy-nokeepalive 1
ProxyPass / fcgi://${HOSTNAME}:9000/
</VirtualHost>
EOF
systemctl restart httpd.service
#启动radosgw:
radosgw -c /etc/ceph/ceph.conf -n client.radosgw.gateway
```

### 2. 在已有rgw的服务器上新增节点

由于之前已经部署过rgw实例，所以9000端口已经被占用，可以使用新的端口供rgw使用

在`/etc/ceph/ceph.conf`中新增新实例的配置，如这里使用9001端口

```plain
[client.radosgw.gateway1]
host = controller1
keyring = /etc/ceph/ceph.client.radosgw1.keyring
rgw frontends = fastcgi socket_port=9001 socket_host=0.0.0.0
log file = /var/log/radosgw/client.radosgw.gateway1.log
rgw print continue = false
rgw keystone url = http://172.159.6.11:35357
rgw keystone admin token = aaaaaa
rgw keystone accepted roles = admin, _member_
rgw keystone token cache size = 500
rgw keystone revocation interval = 500
rgw s3 auth use keystone = true
nss_db_path = /var/ceph/nss
rgw_cache_enabled = true
rgw_cache_lru_size = 100000
rgw_thread_pool_size = 256
```

在`/etc/httpd/conf.d`中新增`rgw1.conf`和`swift.conf`

```plain
rgw1.conf

<VirtualHost *:8082>
ServerName localhost
DocumentRoot /var/www/html
ErrorLog /var/log/httpd/rgw1_error.log
CustomLog /var/log/httpd/rgw1_access.log combined
# LogLevel debug
RewriteEngine On
RewriteRule .* - [E=HTTP_AUTHORIZATION:%{HTTP:Authorization},L]
SetEnv proxy-nokeepalive 1
ProxyPass / fcgi://172.159.6.11:9001/
</VirtualHost>
```

```plain
swift1.conf

ServerName 172.159.6.11
Listen 172.159.6.11:8082
<IfModule !proxy_fcgi_module>
LoadModule proxy_fcgi_module modules/mod_proxy_fcgi.so
</IfModule>
```

现在httpd已经有两个virtualhost了，之前部署的端口为8081，现在部署的为8082。

安装nginx做负载均衡

新建`/etc/yum.repos.d/nginx.repo`文件，内容为：

```plain
[nginx]
name=nginx repo
baseurl=http://nginx.org/packages/mainline/centos/7/$basearch/
gpgcheck=0
enabled=1
```

然后使用`yum install nginx`安装，然后修改配置`/etc/nginx/nginx.conf`，添加如下内容:

```plain
user  nginx;
worker_processes  4;
worker_rlimit_nofile 800000;

error_log  /var/log/nginx/error.log warn;
pid        /var/run/nginx.pid;


events {
    use epoll;
    worker_connections  4096;
}

http {

    fastcgi_buffers 4 64k;
    fastcgi_buffer_size 64;
    client_max_body_size 1000m;
    upstream bk_radosgw {
        server 172.159.6.11:8081;
        server 172.159.6.11:8082;
    }
    server
    {
        listen *:8080;
        location / {
            proxy_pass http://bk_radosgw;
        }
    }

    include       /etc/nginx/mime.types;
    default_type  application/octet-stream;

    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for"'
                      '"$upstream_addr" "$upstream_response_time"';

    access_log  /var/log/nginx/access.log  main;

    sendfile        on;
    #tcp_nopush     on;

    keepalive_timeout  65;
    include /etc/nginx/conf.d/*.conf;
}
```

重启nginx服务器和httpd

```sh
systemctl restart httpd
systemctl restart nginx
```

## 在多台服务器上部署rgw实例

上述步骤中已有配置步骤。使用相同方法配置成功后在nginx的配置文件中的upstream中加入就可以了。