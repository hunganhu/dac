#!/bin/sh
#***************************************************************************
# Licensed Materials - Property of DAC
# (C) COPYRIGHT Decision Analytics Consulting 2005,2006
# All Rights Reserved.
#
# FILE NAME: scordump.sh
#
# Description: Export data over 5 year to ascii files.
# 
ALIAS='escore'
USER='escorap1'
PASSWD='escorap1'
BASEYEAR=`date +%Y`
BASEYEAR=`expr $BASEYEAR - 1915` # 4 years ago and convert to MinGuo year

WS='SCOR'
JS='001'
JN='db2'
LogName='/tmp/'$WS$JS'_'$JN'.log'

cp /dev/null $LogName
#db2 -svt -z $LogName -f <<END_SCRIPT
db2 -svz $LogName <<END_SCRIPT
connect to $ALIAS user $USER using $PASSWD

export to /home/escorap1/history/aas001_$BASEYEAR.del of del select * from aas001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/aps001_$BASEYEAR.del of del select * from aps001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/bam086_$BASEYEAR.del of del select * from bam086 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/bam087_$BASEYEAR.del of del select * from bam086 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/bam302_$BASEYEAR.del of del select * from bam302 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/bam303_$BASEYEAR.del of del select * from bam303 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/bam304_$BASEYEAR.del of del select * from bam304 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/bas001_$BASEYEAR.del of del select * from bas001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/dam103_$BASEYEAR.del of del select * from dam103 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/dam203_$BASEYEAR.del of del select * from dam203 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/das006_$BASEYEAR.del of del select * from das006 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/jas002_$BASEYEAR.del of del select * from jas002 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/krm001_$BASEYEAR.del of del select * from krm001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/krm023_$BASEYEAR.del of del select * from krm023 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/krm037_$BASEYEAR.del of del select * from krm037 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/stm001_$BASEYEAR.del of del select * from stm001 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/stm007_$BASEYEAR.del of del select * from stm007 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/vam102_$BASEYEAR.del of del select * from vam102 where cast(left(inquiry_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/app_info_$BASEYEAR.del of del select * from app_info where cast(left(system_date,3) as int) < $BASEYEAR
export to /home/escorap1/history/app_result_$BASEYEAR.del of del select * from app_result where cast(left(system_time,3) as int) < $BASEYEAR

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

