#!/bin/bash

BBOX_HOME=`dirname $0`
SCRIPT_NAME=`basename $0`
cd ${BBOX_HOME}
BBOX_HOME=`pwd`

chmod u+x ${BBOX_HOME}/*.sh
${BBOX_HOME}/shutdown.sh

