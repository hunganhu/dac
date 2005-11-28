#############################################################################
# Licensed Materials - Property of DAC
#
# (C) COPYRIGHT Decision Analytics Consulting 2005
# All Rights Reserved.
#
#############################################################################
#
# MAKEFILE for C++ Approval module on AIX
#
# Enter one of the following commands
#
#   make app_name	   - Builds the program designated by app_name
#   make all		   - Builds all supplied sample programs
##############################################################################
#		   1 -- VARIABLES
##############################################################################

# Set up the DB2 instance path
DB2PATH = /home/db2inst1/sqllib
DB2INC = $(DB2PATH)/include
DB2LIB = $(DB2PATH)/lib

#Set up precompiler and C++ compiler
EMBPREP=embprep
CPP=g++
EXTRA_CFLAG= -g -Wno-deprecated -DDEBUG
#EXTRA_CFLAG= -g -Wno-deprecated

# To connect to a remote DAC P-Loan database cataloged on the client machine
# with another name, update the ALIAS variable.
ALIAS=dacdb
# Set UID and PWD if neccesary
UID=ejcicap1
PWD=ejcicap1

ERASE=rm -f

DLL_OBJ = dac_pl_cal.o utilemb.o risk_model.o dacploan.o loan_app.o functions.o

#****************************************************************************
#		   2a - make all (srv + all_client)
#****************************************************************************

all : libdac.so java  main 
.PHONY : all
	
java: Dac.class 

Dac.class: Dac.java
	javac Dac.java
Dac.h: Dac.class
	javah -jni Dac
#libdacploan.so: dacploan.o
#	gcc -shared -o libdacploan.so dacploan.o

dacploan.o: dacploan.c 
	gcc -g $(EXTRA_CFLAG) -I/usr/java131/include -c dacploan.c

main: main.o utilemb.o risk_model.o dac_pl_cal.o loan_app.o
	$(CPP) $(EXTRA_CFLAG) -o main -ldb2 -L$(DB2PATH)/lib \
	 main.o $(DLL_OBJ)

mainl: main.o 
	$(CPP) $(EXTRA_CFLAG) -o mainl main.o -ldac -L. -ldb2 -L$(DB2PATH)/lib

main.o: main.cpp
#	$(EMBPREP) main $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -c main.cpp

libdac.so: $(DLL_OBJ)
	$(CPP) -shared -ldb2 -L$(DB2LIB) -o libdac.so $(DLL_OBJ)

#libdac.a: dac_pl_cal.o utilemb.o risk_model.o
#	$(ERASE) $@
#	ar cq $@ dac_pl_cal.o utilemb.o risk_model.o

dac_pl_cal.o: dac_pl_cal.cpp dac_pl_cal.h
#	$(EMBPREP) dac_pl_cal $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -c dac_pl_cal.cpp

utilemb.o: utilemb.sqC utilemb.h
	$(EMBPREP) utilemb $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -c utilemb.C

risk_model.o: risk_model.sqC risk_model.h
	$(EMBPREP) risk_model $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -c risk_model.C

loan_app.o: loan_app.sqC loan_app.h error.h
	$(EMBPREP) loan_app $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -c loan_app.C

functions.o: functions.cpp functions.h
	$(CPP) $(EXTRA_CFLAG) -c functions.cpp
#****************************************************************************
# make clean
#****************************************************************************

clean :
	$(ERASE) *.o *.so *.bak *~
	$(ERASE) *.bnd *.C
