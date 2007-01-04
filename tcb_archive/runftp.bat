:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Licensed Materials - Property of DAC
:: (C) COPYRIGHT Decision Analytics Consulting 2005,2006
:: All Rights Reserved.
::
:: FILE NAME: runftp.bat
::
:: Description: Get exported files from database server.
:: 

cd c:\escore
:: get export data from production host to local host
:: Create the temporary script file
:: Change the user name
:: Change the password
:: Change the working directory if needed
> script.ftp ECHO escorap1
>>script.ftp ECHO escorap1
>>script.ftp ECHO cd history
>>script.ftp ECHO ascii
>>script.ftp ECHO prompt n
>>script.ftp ECHO mget *.del
>>script.ftp ECHO quit
:: Use the temporary script for unattended FTP
FTP -v -s:script.ftp 10.0.31.72
:: For the paranoid: overwrite the temporary file before deleting it
TYPE NUL >script.ftp
DEL script.ftp
