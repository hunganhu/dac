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
DESDIR = /home/escorap1
DESLIB = $(DESDIR)/lib
DESBIN = $(DESDIR)/bin

#Set up precompiler and C++ compiler
EMBPREP=embprep
CPP=g++
###
#  3 compiler options are used to show debug information 
#  -DDEBUG: trace risk model steps
#  -DTRACE: save risk model variables to permanent table
#  -DNPV_FLOWS: print cash flows of every term when calculating NPV
#EXTRA_CFLAG= -g -Wno-deprecated -DDEBUG -DTRACE -DNPV_FLOWS
#EXTRA_CFLAG= -g -Wno-deprecated -DTRACE -DNPV_FLOWS
EXTRA_CFLAG= -g -Wno-deprecated -DTRACE
#EXTRA_CFLAG= -g -Wno-deprecated 

# To connect to a remote DAC P-Loan database cataloged on the client machine
# with another name, update the ALIAS variable.
#ALIAS=dacdb
ALIAS=escore
# Set UID and PWD if neccesary
#UID=ejcicap1
#PWD=ejcicap1
UID=escorap1
PWD=escorap1

ERASE=rm -f

DLL_OBJ = dac_pl_cal.o utilemb.o risk_model.o dacploan.o loan_app.o functions.o financial.o

#****************************************************************************
#		   2a - make all (srv + all_client)
#****************************************************************************

all : libdac_1_7.so java  main mainfnl closecases
.PHONY : all
	
java: Dac.class 

Dac.class: Dac.java
	javac Dac.java
Dac.h: Dac.class
	javah -jni Dac

dacploan.o: dacploan.c 
	gcc -g -I/usr/java131/include -c dacploan.c

main: main.o $(DLL_OBJ)
	$(CPP) $(EXTRA_CFLAG) -o main -ldb2 -L$(DB2PATH)/lib \
	 main.o $(DLL_OBJ)

mainfnl: mainfnl.o $(DLL_OBJ)
	$(CPP) $(EXTRA_CFLAG) -o mainfnl -ldb2 -L$(DB2PATH)/lib \
	 mainfnl.o $(DLL_OBJ)

closecases: closecases.o GetOpt.o $(DLL_OBJ)
	$(CPP) $(EXTRA_CFLAG) -o closecases -ldb2 -L$(DB2PATH)/lib \
	 closecases.o GetOpt.o utilemb.o loan_app.o functions.o financial.o
closecases1: closecases.o GetOpt.o
	$(CPP) $(EXTRA_CFLAG) -o closecases1 -L$(DB2PATH)/lib -ldb2 -L. -ldac \
	closecases.o GetOpt.o

closecases.o: closecases.sqC closecases.h loan_app.h utilemb.h error.h
	$(EMBPREP) closecases $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -c closecases.C

main.o: main.cpp
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -c main.cpp

mainfnl.o: mainfnl.cpp
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -c mainfnl.cpp

cutpoint: cutpoint.o $(DLL_OBJ)
	$(CPP) $(EXTRA_CFLAG) -o cutpoint -ldb2 -L$(DB2PATH)/lib \
	 cutpoint.o $(DLL_OBJ)
#	gcc  -o cutpoint cutpoint.o functions.o

cutpoint.o: cutpoint.cpp
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -c cutpoint.cpp
#	gcc -g  -c cutpoint.c
	
libdac_1_7.so: $(DLL_OBJ)
	$(ERASE) libdac_1_7.so
	$(CPP) -shared -ldb2 -L$(DB2LIB) -o libdac_1_7.so $(DLL_OBJ)
#libdac.a: $(DLL_OBJ)
#	$(ERASE) $@
#	ar cq $@ $(DLL_OBJ)

dac_pl_cal.o: dac_pl_cal.sqC dac_pl_cal.h loan_app.h error.h functions.h financial.h
	$(EMBPREP) dac_pl_cal $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -fPIC -c dac_pl_cal.C

utilemb.o: utilemb.sqC utilemb.h
	$(EMBPREP) utilemb $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -fPIC -c utilemb.C

risk_model.o: risk_model.sqC risk_model.h functions.h error.h
	$(EMBPREP) risk_model $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -fPIC -c risk_model.C

loan_app.o: loan_app.sqC loan_app.h error.h attrition.h functions.h financial.h
	$(EMBPREP) loan_app $(ALIAS) $(UID) $(PWD)
	$(CPP) $(EXTRA_CFLAG) -I$(DB2INC) -fPIC -c loan_app.C

functions.o: functions.cpp functions.h
	$(CPP) $(EXTRA_CFLAG) -fPIC -c functions.cpp

financial.o: financial.cpp financial.h
	$(CPP) $(EXTRA_CFLAG) -fPIC -c financial.cpp

GetOpt.o: GetOpt.cpp GetOpt.h
	$(CPP) $(EXTRA_CFLAG) -fPIC -c GetOpt.cpp

#****************************************************************************
# make clean
#****************************************************************************

clean :
	$(ERASE) *.o *.so *.bak *~
	$(ERASE) *.bnd *.C main mainfnl closecases

install: libdac_1_7.so closecases
	 cp -f libdac_1_7.so $(DESLIB)
	 cp -f closecases $(DESBIN)
	 touch install
	 cd $(DESLIB)
	 ln -fs libdac_1_7.so libdac.so
