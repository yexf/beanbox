#docker build --no-cache=true -t "devhub.baymax.oppoer.me/80349971/ofdb-centos:v1.7.6" . 
FROM centos:7.6.1810

RUN mkdir -p /etc/yum.repos.d/repo.bak
RUN mv /etc/yum.repos.d/*.repo /etc/yum.repos.d/repo.bak
ADD *.repo /etc/yum.repos.d/

RUN yum install -y wget vim sudo \
    && yum clean all \
    && rm -rf /var/cache/yum

### 开放端口
EXPOSE 22 6699 8081
