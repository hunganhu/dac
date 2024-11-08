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
#include "main.h"

//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{
  using namespace std;
  GetOpt getopt (argc, argv, "m:M:c:C:u:U:p:P:s:S:d:D:hHgG");
  int option_char, i, Debug = 0;
  char *target_month, *config_file, *user, *password, *source, *database;
  char connect_string[128], buf[20];
  Variant hostVars[10];
  TADOHandler *dbhandle;
  TADOQuery *Query;


  target_month = config_file = user = password = source = database = (char *) NULL;
  while ((option_char = getopt ()) != EOF)
    switch (option_char)
      {
         case 'm':
         case 'M':
           target_month = getopt.optarg;
           DEBUG (stderr,"cycle_date: %s\n", target_month); break;
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
//           fprintf (stderr,"usage: Advscore -m cycle_date -c config_file ");
           fprintf (stderr,"usage: Advscore -m cycle_date ");
           fprintf (stderr,"-u user -p password -s source -d database\n\n");
           fprintf (stderr,"\tcycle_date: the cycle date in which the PD are calculated.\n");
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
 if (target_month == (char*) NULL || password == (char*) NULL ||
    user == (char*) NULL || database == (char*) NULL || source == (char*) NULL) {
//     fprintf (stderr,"usage: Advscore -m cycle_date -c config_file ");
     fprintf (stderr,"All options are REQUIRED.\n");
     fprintf (stderr,"usage: Advscore -m cycle_date ");
     fprintf (stderr,"-u user -p password -s source -d database\n\n");
     fprintf (stderr,"\tcycle_date: the cycle date in which the PD are calculated.\n");
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

 dbhandle = new TADOHandler();
 if (! dbhandle->OpenDatabase(connect_string)) {
    fprintf(stderr, "Error: Could not connect to database.");
    delete dbhandle;
    return (1);
 }

 fprintf(stderr, "%s: Calculating Behavior Score Ver. 3.1 started.\n", CurrDateTime());
 for (i = 0; i < NSTEPS; i++) {
     switch (step[i]) {
        case Execute_Proc_Load_Input_Table:
        case Execute_Proc_Assign_SegCode:
        case Execute_Proc_Assign_AccountAge:
        case Execute_Proc_Load_Vars_Table:
        case Execute_Proc_Generate_Profile:
        case Execute_Proc_Generate_Score:
           hostVars[0] = target_month;
           DEBUG (stderr, "%s: [Step %d] %s %s\n", CurrDateTime(), i, SQLNames[step[i]], target_month);
           if (! dbhandle->ExecSQLCmd(SQLCommands[step[i]], hostVars, 0)) {
              delete dbhandle;
              return (1);
           }
           break;
        case End_of_SQL:
           fprintf(stderr, "%s: Calculating Behavior Score Ver. 3.1 completed.\n", CurrDateTime());
           fprintf (stderr, "\nFuHwa Holding Credit Card Risk Monthly Profile (%s)\n", target_month);
           int risk_group, group_count;
           try {
              Query = new TADOQuery(NULL);
              Query->ConnectionString = connect_string;
              Query->Close();
              Query->SQL->Clear();
              Query->SQL->Add("select risk_group, group_count from credit_card_monthly_profile_riskgroup where cycle_date=:v1 order by risk_group");
              Query->Parameters->ParamValues["v1"] = target_month;
              Query->Open();
              Query->First();
              while (!Query->Eof) {
                 risk_group = Query->FieldValues["risk_group"];
                 group_count = Query->FieldValues["group_count"];
                 fprintf (stderr, "   Group %3d = %d\n", risk_group, group_count);
                 Query->Next();
              }
           }
           catch (Exception &E){
               fprintf(stderr,"Error: %s, %s\n", AnsiString(E.ClassName()), E.Message);
               delete dbhandle;
               return (false);
           }
           break;
        default:
           DEBUG (stderr, "%s: [Step %d] %s\n", CurrDateTime(), i, SQLNames[step[i]]);
           if (! dbhandle->ExecSQLCmd(SQLCommands[step[i]])) {
              delete dbhandle;
              return (1);
           }
     }
 }

 dbhandle->CloseDatabase();
 delete dbhandle;
 return (0);
}
//---------------------------------------------------------------------------

char * CurrDateTime ()
{
 time_t timer;
 struct tm *tblock;
 static char buf[20];

 timer = time(NULL);
 tblock = localtime(&timer);
 sprintf (buf, "%04d/%02d/%02d %02d:%02d:%02d", tblock->tm_year+1900, tblock->tm_mon+1,
          tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
 return (buf);
}
