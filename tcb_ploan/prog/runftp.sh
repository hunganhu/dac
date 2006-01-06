#!/bin/sh
HOST_FROM='10.0.31.71'
HOST_TO='10.0.5.47'
USER='escorap1'
PASSWD='escorap1'

cd /home/escorap1/tmp

ftp -n $HOST_FROM > /tmp/ftp.worked1 2> /tmp/ftp.failed1 <<END_SCRIPT
quote USER $USER
quote PASS $PASSWD
prompt n
cd archive_prod
mget *.del
quit
END_SCRIPT

ftp -n $HOST_TO > /tmp/ftp.worked2 2> /tmp/ftp.failed2 <<END_SCRIPT
quote USER $USER
quote PASS $PASSWD
prompt n
cd load_prod
mput *.del
quit
END_SCRIPT

exit 0
