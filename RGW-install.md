# 安装CephRGW

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
rgw keystone admin token = aaaaaa
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
keystone endpoint-create --service-id $(keystone service-list | awk '/ swift / {print $2}') --publicurl http://172.16.197.31:8080/swift/v1 --internalurl http://172.16.197.31:8080/swift/v1 --adminurl http://172.16.197.31:8080/swift/v1 --region region150

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
