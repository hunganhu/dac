/*************************************************************************
 *              Copyright (C) 2005 Decision Analytics Consulting
 *         All rights reserved. Duplication or distribution prohibited
 *
 * Program: main.cpp
 * Description:  The program does the following tasks.
 *    Operation Process
 *    -	When the "Host" computer is ready to calculate the scores of the most recently cycled credit card accounts, it sends (by FTP) three files to the "Scoring" computer:
 *      o ACCOUNT: All accounts of the most recent cycle
 *      o STATEMENT: Most recent statement data of the accounts
 *      o CONTROL: Signal file that also contains the control information for the specific run of cycle
 *    -	Periodically (every 30 minutes), a batch program running on the "Scoring" compute checks the existence of the CONTROL file as the signal that the data is ready for calculation.
 *    -	When the batch program detects the CONTROL file, it starts to execute the sequence of data loading, checking, and scoring procedures.
 *    -	Upon finishing up, the batch program
 *      o Returns a code to indicate whether the program executes successfully
 *      o Writes log to the log table
 *      o Upload the scoring results to the "Host" computer
 *         "	PD: Scored accounts with PD values and assigned group ID's
 *         "	PROFILE: Numbers of accounts in each assigned group for the cycle
 *         "	CONTROL: Signal file that contains control information for the result of the specific run of cycle
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
 * Current Revision: Mon March 25 2005
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
#include "BSautoSQL.h"
#include "main.h"

//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{
  GetOpt getopt (argc, argv, "c:C:u:U:p:P:s:S:d:D:hHgG");
  int option_char, i, Debug = 0, returnCode;
  char *target_month, *config_file, *user, *password, *source, *database, *create_dtime;
  char connect_string[128], buf[20], syscmd[256], PD_file[32], Profile_file[32];
  TADOHandler *dbhandle;
  TADOQuery *Query;
  ControlFile *control;
  char start_time[20];

  strcpy(start_time, CurrDateTime());
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
    control  = new ControlFile();
    dbhandle->OpenDatabase(connect_string);

    dbhandle->ExecSQLCmd(SQLCommands[Clean_Temp_Tables]);
    returnCode = control->get_control_info();
    if (returnCode > 0) {
       write_log_table(dbhandle, control->get_cycledate(), start_time,
                       CurrDateTime(), returnCode, return_msgs[returnCode]);
       return (returnCode);
    }

    control->bulk_insert(dbhandle);
    returnCode = control->check_bulk_insert_status(dbhandle);
    if (returnCode > 0) {
       write_log_table(dbhandle, control->get_cycledate(), start_time,
                       CurrDateTime(), returnCode, return_msgs[returnCode]);
       return (returnCode);
    }

    returnCode = control->check_production_insert_status(dbhandle);
    if (returnCode > 0) {
       write_log_table(dbhandle, control->get_cycledate(), start_time,
                       CurrDateTime(), returnCode, return_msgs[returnCode]);
       return (returnCode);
    }

// Prepare system command to execute behavior scoring module advscore
    sprintf (syscmd, SQLCommands[SYSTEM_Exec_Advscore], control->get_cycledate(),
             user, password, source, database);
    system(syscmd);

   char *bs_data, curr_dir[128];
   current_directory(curr_dir);
   if ((bs_data = getenv("BSAUTO_DATA")) == NULL)
      bs_data = curr_dir;

   create_dtime =  Create_date();
// Prepare system command bcp to dump PDs of the accounts with specific cycle date
    sprintf (syscmd, SQLCommands[SYSTEM_Exec_Bcp_PD], database, control->get_cycledate(),
             bs_data, control->get_cycledate(), create_dtime, user, password, source);
    system(syscmd);

// Prepare system command bcp to dump profile of the specific cycle date
    sprintf (syscmd, SQLCommands[SYSTEM_Exec_Bcp_Profile], database, control->get_cycledate(),
             bs_data, control->get_cycledate(), create_dtime, user, password, source);
    system(syscmd);
    write_result (bs_data, control->get_cycledate(), create_dtime);
 } catch (Exception &E) {
     fprintf(stderr, E.Message.c_str());
     write_log_table(dbhandle, control->get_cycledate(), start_time,
                     CurrDateTime(), SQL_RUNTIME_ERROR, E.Message.c_str());
     dbhandle->CloseDatabase();
     delete control;
     delete dbhandle;
     return (SQL_RUNTIME_ERROR);
 }

 dbhandle->CloseDatabase();
 delete control;
 delete dbhandle;
 write_log_table(dbhandle, control->get_cycledate(), start_time,
                 CurrDateTime(), SUCCESS, return_msgs[SUCCESS]);
 return (0);
}

int write_result(char *bs_home, char *cycle_date, char *create_date)
{
 ofstream outfile;
 char control_path[128], line[80], file_path[128];

 sprintf (control_path, "%s\\BCRESULT.CTL", bs_home);
 outfile.open(control_path, ios::out);

 outfile << create_date << endl;
 outfile << cycle_date << endl;
 sprintf(file_path, "%s\\PD_%s_%s.csv", bs_home, cycle_date, create_date);
 sprintf(line, "PD_%s_%s.csv %d", cycle_date, create_date, get_linecount(file_path));
 outfile << line << endl;
 sprintf(file_path, "%s\\PROFILE_%s_%s.csv", bs_home, cycle_date, create_date);
 sprintf(line, "PROFILE_%s_%s.csv %d", cycle_date, create_date, get_linecount(file_path));
 outfile << line << endl;

 return(0);
}


void write_log_table(TADOHandler *dbhandle, char *cycle_date, char *start,
                     char *end, int retcode, char *msg)
{
  Variant hostVars[10];

  try {
     hostVars[0] = cycle_date;
     hostVars[1] = start;
     hostVars[2] = end;
     hostVars[3] = retcode;
     hostVars[4] = msg;
     dbhandle->ExecSQLCmd(SQLCommands[Write_Log], hostVars, 4);
  } catch (Exception &E) {
     throw;
  }
}
