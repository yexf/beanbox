#!/bin/bash

TARGET=ofdb
if [ $# -gt 0  ]
then
    TARGET=$1
fi
DEPLOY_PATH="."
cd $DEPLOY_PATH
DEPLOY_PATH=`pwd`
THIRD_PATH="${DEPLOY_PATH}/thirdparty"
RECALL_PATH="${DEPLOY_PATH}/recall"
BUILD_PATH="${DEPLOY_PATH}/build"

mkdir -p ${BUILD_PATH} >> /dev/null 2>&1
mkdir -p ${BUILD_PATH}/include >> /dev/null 2>&1
mkdir -p ${BUILD_PATH}/lib >> /dev/null 2>&1

# install snappy zlib bzip2 lz4 ASAN
echo "install glog"
if [ ! -f ${BUILD_PATH}/lib/libglog.so.0 ]
then
    cp -d /lib64/libglog.* ${BUILD_PATH}/lib/
    cp -d /lib64/libgflags.* ${BUILD_PATH}/lib/
    cp -d /lib64/libprotobuf.* ${BUILD_PATH}/lib/
    cp -d /lib64/libz.* ${BUILD_PATH}/lib/
    cp -d /lib64/libtbb.* ${BUILD_PATH}/lib/
fi

CPU_NUM=`lscpu | awk '/^CPU\(s\)/{print $2}'`
if [ $CPU_NUM -gt 2 ]
then
    COMPILE_CPU_NUM=`expr $CPU_NUM - 2`
else
    COMPILE_CPU_NUM=1
fi

cd ${BUILD_PATH}
cmake3 ..
make -j ${COMPILE_CPU_NUM}

# copy libs to deploy path
echo "copy libs to deploy path..."
mkdir -p ${DEPLOY_PATH}/package/conf >> /dev/null 2>&1
mkdir -p ${DEPLOY_PATH}/package/lib >> /dev/null 2>&1
cd ${DEPLOY_PATH}/package/lib
rm -r lib* >> /dev/null 2>&1
cp ${BUILD_PATH}/lib/*.* .
cp ${DEPLOY_PATH}/lib/libbean.so .
cp ${DEPLOY_PATH}/lib/libofdb.so .
mkdir -p ${DEPLOY_PATH}/package/bin >> /dev/null 2>&1
cp ${DEPLOY_PATH}/bin/bbox ${DEPLOY_PATH}/package/bin/

