#!/bin/bash

BBOX_HOME=`dirname $0`
SCRIPT_NAME=`basename $0`
cd ${BBOX_HOME}
BBOX_HOME=`pwd`

BBOX_BIN=${BBOX_HOME}/bin
BBOX_CONF=${BBOX_HOME}/conf
BBOX_LIB=${BBOX_HOME}/lib
BBOX_LOG=${BBOX_HOME}/log
BBOX_TEMP=${BBOX_HOME}/temp
export LD_LIBRARY_PATH=${BBOX_LIB}

LOG_FILE="${BBOX_HOME}/logs/${SCRIPT_NAME}.log"

source ${BBOX_HOME}/../env.sh
${BBOX_BIN}/bbox -c ${BBOX_CONF}/bbox.conf >> $LOG_FILE 2>&1
if [ $? -eq 0 ]
then
    info_log "${BBOX_HOME}/bin/bbox start successfully."
else
    error_log "failed to start bbox"
    echo "failed to start bbox"
fi

sleep 1
exit 0
