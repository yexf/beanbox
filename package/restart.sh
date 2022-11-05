#!/bin/bash

BBOX_HOME=`dirname $0`
SCRIPT_NAME=`basename $0`
cd ${BBOX_HOME}
BBOX_HOME=`pwd`

source ${BBOX_HOME}/../env.sh
chmod 644 /home/service/var/corefiles/*
${BBOX_HOME}/shutdown.sh
sleep 25 # 增加本地缓存后停止服务所需时间增加，延迟20秒后再启动
${BBOX_HOME}/startup.sh

