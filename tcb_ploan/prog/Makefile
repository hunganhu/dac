#############################################################################
# Licensed Materials - Property of DAC
#
# (C) COPYRIGHT Decision Analytics Consulting 2005
# All Rights Reserved.
#
#############################################################################
#
# MAKEFILE for C++ samples on AIX
#
# Enter one of the following commands
#
#   make app_name	   - Builds the program designated by app_name
#   make all		   - Builds all supplied sample programs
#   make srv		   - Builds all programs that that run on the server
#			      including routines (stored procedures and UDFs)
##############################################################################
#		   1 -- VARIABLES
##############################################################################

# Set up the DB2 instance path
DB2PATH = /home/db2inst1/sqllib

#Set up precompiler and C++ compiler
EMBPREP=embprep
CPP=g++
EXTRA_CFLAG= -g -Wno-deprecated

# To connect to a remote DAC P-Loan database cataloged on the client machine
# with another name, update the ALIAS variable.
ALIAS=dacdb
# Set UID and PWD if neccesary
UID=ejcicap1
PWD=ejcicap1

ERASE=rm -f


#****************************************************************************
#		   2a - make all (srv + all_client)
#****************************************************************************

all : \
	libdac.so java  main mainl libdac.a

java: Dac.class libdacploan.so

Dac.class: Dac.java
	javac Dac.java
Dac.h: Dac.java
	javah -jni Dac
libdacploan.so: dacploan.o
	gcc -shared -o libdacploan.so dacploan.o

dacploan.o: dacploan.c
	gcc -c -g -I/usr/java131/include dacploan.c

main: main.o utilemb.o risk_model.o dac_pl_cal.o
	$(CPP) $(EXTRA_CFLAG) -o main main.o utilemb.o risk_model.o dac_pl_cal.o -ldb2 -L$(DB2PATH)/lib

mainl: main.o 
	$(CPP) $(EXTRA_CFLAG) -o mainl main.o -ldac -L. -ldb2 -L$(DB2PATH)/lib

main.o: main.C
	$(CPP) $(EXTRA_CFLAG) -I$(DB2PATH)/include -c main.C

libdac.so: dac_pl_cal.o utilemb.o risk_model.o
	$(CPP) -shared -ldb2 -L$(DB2PATH)/lib -o libdac.so \
	dac_pl_cal.o utilemb.o risk_model.o

libdac.a: dac_pl_cal.o utilemb.o risk_model.o
	$(ERASE) $@
	ar cq $@ dac_pl_cal.o utilemb.o risk_model.o

dac_pl_cal.o: dac_pl_cal.C dac_pl_cal.h
#	$(EMBPREP) dac_pl_cal $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2PATH)/include -c dac_pl_cal.C

utilemb.o: utilemb.sqC utilemb.h
	$(EMBPREP) utilemb $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2PATH)/include -c utilemb.C

risk_model.o: risk_model.sqC
	$(EMBPREP) risk_model $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2PATH)/include -c risk_model.C

#****************************************************************************
# make clean
#****************************************************************************

clean :
	$(ERASE) *.o *.so *.bak
	$(ERASE) *.DEL *.TXT *.MSG *.bnd
	$(ERASE) dac_pl_cal.C
