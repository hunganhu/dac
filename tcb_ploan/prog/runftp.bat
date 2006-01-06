cd c:\escorap1
:: get export data from production host to local host
:: Create the temporary script file
:: Change the user name
:: Change the password
:: Change the working directory if needed
> script.ftp ECHO escorap1
>>script.ftp ECHO escorap1
>>script.ftp ECHO cd archive_prod
>>script.ftp ECHO ascii
>>script.ftp ECHO prompt n
>>script.ftp ECHO mget *.del
>>script.ftp ECHO quit
:: Use the temporary script for unattended FTP
FTP -v -s:script.ftp 10.0.31.71
:: For the paranoid: overwrite the temporary file before deleting it
TYPE NUL >script.ftp
DEL script.ftp


:: put export data from local host to testing host
:: Create the temporary script file
:: Change the user name
:: Change the password
:: Change the working directory if needed
> script.ftp ECHO escorap1
>>script.ftp ECHO escorap1
>>script.ftp ECHO cd load_prod
>>script.ftp ECHO ascii
>>script.ftp ECHO prompt n
>>script.ftp ECHO mput *.del
>>script.ftp ECHO quit
:: Use the temporary script for unattended FTP
FTP -v -s:script.ftp 10.0.5.47
:: For the paranoid: overwrite the temporary file before deleting it
TYPE NUL >script.ftp
DEL script.ftp
