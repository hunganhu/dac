/*************************************************************************
 *              Copyright (C) 2005 Decision Analytics Consulting
 *         All rights reserved. Duplication or distribution prohibited
 *
 * Program: main.cpp
 * Description:
 *
 *
 * To run bsauto -h for command usage.
 *   Usage: bsauto -u user -p password -s source -d database
 *           user: user name to connect to SQL server.
 *           password: password to connect to SQL server.
 *           source: source name of SQL server.
 *           database: database name in which tables are resided.
 *           -h, -?: disply this help message.
 *
 * First   Revision: Fri March 11 2005, by Oliver Hu <oliver.hu@burkelee.com>
 * Current Revision: Mon November 06 2000
 *
 *
 *
 *************************************************************************/
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <system.hpp>
#include <time.h>
#include <math.h>

#pragma hdrstop
#include "GetOpt.h"
#include "AdoHandle.h"
#include "functions.h"
#include "main.h"

//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{
  using namespace std;
  GetOpt getopt (argc, argv, "c:C:u:U:p:P:s:S:d:D:hHgG");
  int option_char, i, Debug = 0;
  char *target_month, *config_file, *user, *password, *source, *database;
  char connect_string[128], buf[20];
  AnsiString low, medium, high, not_scored;
  Variant hostVars[10];
  TADOHandler *dbhandle;
  TADOQuery *Query;


  config_file = user = password = source = database = (char *) NULL;
  while ((option_char = getopt ()) != EOF)
    switch (option_char)
      {
         case 'c':
         case 'C':
           config_file = getopt.optarg;
           DEBUG (stderr,"config: %s\n", config_file); break;
         case 'U':
         case 'u':
           user = getopt.optarg;
           DEBUG (stderr,"user: %s\n", user); break;
         case 'P':
         case 'p':
           password = getopt.optarg;
           DEBUG (stderr,"password: %s\n", password); break;
         case 'D':
         case 'd':
           database = getopt.optarg;
           DEBUG (stderr,"database: %s\n", database); break;
         case 'S':
         case 's':
           source = getopt.optarg;
           DEBUG (stderr,"source: %s\n", source); break;
         case 'g':
         case 'G':
           Debug=1; DEBUG (stderr,"Debug ON\n"); break;
         case 'H':
         case 'h':
         case '?':
         default:
           fprintf (stderr,"usage: bsauto -u user -p password -s source -d database\n\n");
//           fprintf (stderr,"\tconfig_file: the file contains the information to connect to SQL server.\n");
//           fprintf (stderr,"\t             default is ./AdoConnect.udl\n");
           fprintf (stderr,"\tuser: user name to connect to SQL server.\n");
           fprintf (stderr,"\tpassword: password to connect to SQL server.\n");
           fprintf (stderr,"\tsource: source name of SQL server.\n");
           fprintf (stderr,"\tdatabase: database name in which tables are resided.\n");
           fprintf (stderr,"\n\t-h, -?: disply this help message.\n");
           return (0);
      }
//  if (argc < 2 || target_month == (char*) NULL) {
 if (password == (char*) NULL || user == (char*) NULL ||
     database == (char*) NULL || source == (char*) NULL) {
//     fprintf (stderr,"usage: Advscore -m statement_month -c config_file ");
     fprintf (stderr,"All options are REQUIRED.\n");
     fprintf (stderr,"usage: bsauto -u user -p password -s source -d database\n\n");
//     fprintf (stderr,"\tconfig_file: the file contains the information to connect to SQL server.");
//     fprintf (stderr," default is ./AdoConnect.udl\n");
     fprintf (stderr,"\tuser: user name to connect to SQL server.\n");
     fprintf (stderr,"\tpassword: password to connect to SQL server.\n");
     fprintf (stderr,"\tsource: source name of SQL server.\n");
     fprintf (stderr,"\tdatabase: database name in which tables are resided.\n");
     fprintf (stderr,"\n\t-h, -?: disply this help message.\n");
     return (0);
  }

 connect_string[0] = '\0';
 if (config_file != (char*) NULL) {
    sprintf (connect_string, "FILE NAME=%s", config_file);
    DEBUG (stderr,"connection string: %s\n", connect_string);
 }
 else if (password != (char*) NULL && user != (char*) NULL && database != (char*) NULL && source != (char*) NULL) {
    sprintf (connect_string,
          "Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s",
           password, user, database, source);
    DEBUG (stderr,"connection string: %s\n", connect_string);
 }
 else {
    strcpy (connect_string, CONNECTION_STRING);
 }

 try {
    dbhandle = new TADOHandler();
    dbhandle->OpenDatabase(connect_string);
    get_control_info();
/*
    bulk_insert(account);
    bulk_insert(statement);
    load_account();
    load_statement();
    system(advscore);
    dump_monthly_pd();
    dump_monthly_profile();
*/    
    
 } catch (Exception &E) {
     fprintf(stderr, E.Message.c_str());
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 }


 dbhandle->CloseDatabase();
 delete dbhandle;
 return (0);
}

