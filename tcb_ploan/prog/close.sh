#!/bin/sh
#***************************************************************************
# Licensed Materials - Property of DAC
# (C) COPYRIGHT Decision Analytics Consulting 2005,2006
# All Rights Reserved.
#
# FILE NAME: scorclean.sh
#
# Description: Delete data over 5 year
# 
# Setup UDB DB2.
if [ -f /home/db2inst1/sqllib/db2profile ]; then
    . /home/db2inst1/sqllib/db2profile
fi

ALIAS='escore'
USER='escorap1'
PASSWD='escorap1'

/home/escorap1/bin/closecases -u $USER -p $PASSWD -d $ALIAS

# The return code  of closecases command can be one of the following:
# Code   Description
# 0      DB2 command or SQL statement executed successfully 
# 100    SQL statement warning - Data not found 
# 1999   DB2 command or SQL statement error 

retcode=$?
echo "Return code = $retcode"
if [ $retcode -eq 0 ]; then
 echo " 成功 "
 exit 0
elif [ $retcode -eq 100 ]; then
 echo " 無待結案件 "
 exit 0
else
 echo " 失敗，請至目錄 /home/escorap1/log 檢查 LOG " 
 exit 1
fi
