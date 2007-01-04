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


ALIAS='escore'
USER='escorap1'
PASSWD='escorap1'
BASEYEAR=`date +%Y`
BASEYEAR=`expr $BASEYEAR - 1915` # 4 years ago and convert to MinGuo year

WS='SCOR'
JS='002'
JN='db2'
LogName='/tmp/'$WS$JS'_'$JN'.log'

db2 -svz $LogName <<END_SCRIPT
connect to $ALIAS user $USER using $PASSWD

delete from aas001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from aps001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from bam086 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from bam087 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from bam302 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from bam303 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from bam304 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from bas001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from dam103 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from dam203 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from das006 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from jas002 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from krm001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from krm023 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from krm037 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from stm001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from stm007 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from vam102 where cast(left(inquiry_date,3) as int) < $BASEYEAR
delete from app_info where cast(left(system_date,3) as int) < $BASEYEAR
delete from app_result where cast(left(system_time,3) as int) < $BASEYEAR

connect reset
quit
END_SCRIPT

# The return code  of db2 command can be one of the following:
# Code   Description
# 0    DB2 command or SQL statement executed successfully 
# 1    SELECT or FETCH statement returned no rows 
# 2    DB2 command or SQL statement warning 
# 4    DB2 command or SQL statement error 
# 8    Command line processor system error 

retcode=$?
echo $retcode
if [ $retcode -le "2" ]
then
 echo OK
 exit 0
else
 echo Fail
 exit 1
fi













