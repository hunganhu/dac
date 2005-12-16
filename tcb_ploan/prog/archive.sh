#!/bin/sh
# The following three lines have been added by UDB DB2.
if [ -f /home/db2inst1/sqllib/db2profile ]; then
    . /home/db2inst1/sqllib/db2profile
fi

PATH=/opt/lotus/bin:/notesdata1:$PATH:/home/db2inst1/sqllib/function:/usr/java131/bin:/usr/bin:/etc:/usr/sbin:/usr/ucb:$HOME/bin:/usr/bin/X11:/sbin:/usr/local/bin:.

export PATH

CLASSPATH=/home/db2inst1/sqllib/java/db2java.zip:/home/db2inst1/sqllib/java/db2jcc_license_cisuz.jar:/home/db2inst1/sqllib/java/db2jcc_license_cu.jar:/home/db2inst1/sqllib/java/db2jcc.zip:/home/db2inst1/sqllib/java/runtime.zip:/opt/lotus/notes/60030/ibmpow/jsdk.jar:/opt/lotus/notes/60030/ibmpow/Notes.jar:$CLASSPATH

export CLASSPATH

NLSPATH=/usr/lib/nls/msg/%L/%N:/usr/lib/nls/msg/%L/%N.cat

DEBUG_ENABLE_CORE=1

export LANG=Zh_TW

db2 -svtf ~escorap1/archive_prod/export.sql
