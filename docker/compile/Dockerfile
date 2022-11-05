#docker build --no-cache=true -t "devhub.baymax.oppoer.me/80349971/ofdb-compile:v1.7.6" .    # 763MB
#docker tag devhub.baymax.oppoer.me/80349971/ofdb-compile:v1.7.6 devhub.baymax.oppoer.me/nodis/ofdb-compile:v1.7.6
#docker push devhub.baymax.oppoer.me/nodis/ofdb-compile:v1.7.6
FROM devhub.baymax.oppoer.me/80349971/ofdb-centos:v1.7.6

RUN yum install -y openssh-server openssl-devel zip unzip tar \
    && yum clean all \
    && rm -rf /var/cache/yum

### 开启ssh 服务配置
RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

RUN echo "export VISIBLE=now" >> /etc/profile

RUN useradd -ms /bin/bash dev 
RUN echo 'dev:dev' | chpasswd
RUN echo "dev ALL=(ALL) ALL" >> /etc/sudoers

RUN yum install -y lz4-devel snappy-devel bzip2-devel zlib-devel numactl-devel libasan protobuf-compiler protobuf-devel tbb-devel \
    gcc gcc-c++ gdb gdb-gdbserver rsync vim make git cmake cmake3 autoconf autogen automake libtool pkgconfig gflags-devel gtest-devel glog-devel \
    && yum clean all \
    && rm -rf /var/cache/yum
