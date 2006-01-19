//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <system.hpp>
#include <time.h>
#include <math.h>
#include <String.h>

#pragma hdrstop
#include "GetOpt.h"
#include "AdoHandle.h"
#include "main.h"

//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{
  using namespace std;
  GetOpt getopt (argc, argv, "m:M:u:U:p:P:s:S:d:D:hHgG");
  int option_char, i, Debug = 0;
  char *target_month, *config_file, *user, *password, *source, *database;
  char connect_string[128], buf[20], yrmon[10], sqlStatement[256];
  int now, yyyymm;
  Variant hostVars[10];
  TADOHandler *dbhandle;
//  TADODataSet *ds;

  int twentile, group_count;
  String segment;

  target_month = config_file = user = password = source = database = (char *) NULL;
  while ((option_char = getopt ()) != EOF)
    switch (option_char)
      {
         case 'm':
         case 'M':
           target_month = getopt.optarg;
           DEBUG (stderr,"data_date: %s\n", target_month); break;
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
           fprintf (stderr,"使用方式: fbscore -m data_date ");
           fprintf (stderr,"-u user -p password -s source -d database\n\n");
           fprintf (stderr,"\tdata_date: :資料取得之日期.\n");
           fprintf (stderr,"\tuser: 連結SQL資料庫的使用者名稱.\n");
           fprintf (stderr,"\tpassword: 連結SQL資料庫的使用者密碼.\n");
           fprintf (stderr,"\tsource: SQL資料庫所在的伺服器名稱.\n");
           fprintf (stderr,"\tdatabase: 資料庫名稱.\n");
           fprintf (stderr,"\n\t-h, -?: 顯示此說明訊息.\n");
           return (0);
      }
 if (target_month == (char*) NULL || password == (char*) NULL ||
    user == (char*) NULL || database == (char*) NULL || source == (char*) NULL) {
     fprintf (stderr,"所有的命令選項皆為必要項目.\n");
     fprintf (stderr,"usage: fbscore -m data_date ");
     fprintf (stderr,"-u user -p password -s source -d database\n\n");
     fprintf (stderr,"\tdata_date: :資料取得之日期.\n");
     fprintf (stderr,"\tuser: 連結SQL資料庫的使用者名稱.\n");
     fprintf (stderr,"\tpassword: 連結SQL資料庫的使用者密碼.\n");
     fprintf (stderr,"\tsource: SQL資料庫所在的伺服器名稱.\n");
     fprintf (stderr,"\tdatabase: 資料庫名稱.\n");
     fprintf (stderr,"\n\t-h, -?: 顯示此說明訊息.\n");
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
 yyyymm = atoi(strncpy(yrmon,target_month, 6));
 now = (yyyymm /100 - 1911) * 12 + (yyyymm % 100);
 try {
 dbhandle = new TADOHandler();
 dbhandle->OpenDatabase(connect_string);
 fprintf(stderr, "%s: Calculating Response Model Ver.1 started.\n", CurrDateTime());
// ds = new TADODataSet(NULL);

 for (i = 0; i < NSTEPS; i++) {
     switch (step[i]) {
        case Update_Vintage:
        case Update_Demographics:
           sprintf (sqlStatement, SQLCommands[step[i]], target_month);
           DEBUG (stderr, "%s: [Step %d] %s %s\n", CurrDateTime(), i, SQLNames[step[i]], target_month);
           dbhandle->ExecSQLCmd(sqlStatement);
           break;
        case Execute_Proc_Update_Max_Cycle:
           hostVars[0] = target_month;
           DEBUG (stderr, "%s: [Step %d] %s %s\n", CurrDateTime(), i, SQLNames[step[i]], target_month);
           dbhandle->ExecSQLCmd(SQLCommands[step[i]], hostVars, 0);
           break;
        case Execute_Proc_Get_Prev_Stmt_Info:
        case Execute_Proc_Cal_BucketM_on_Stmt_6:
        case Cal_RS001:
           hostVars[0] = now;
           DEBUG (stderr, "%s: [Step %d] %s %s\n", CurrDateTime(), i, SQLNames[step[i]], target_month);
           dbhandle->ExecSQLCmd(SQLCommands[step[i]], hostVars, 0);
           break;
        case End_of_SQL:
           if (Debug == 1) {
              DEBUG (stderr, "%s: [Step %d] %s\n", CurrDateTime(), i, SQLNames[Duplicate_Working_Table]);
              dbhandle->ExecSQLCmd(SQLCommands[Duplicate_Working_Table]);
           }
           fprintf(stderr, "%s: Calculating Response Model Ver.1 completed.\n", CurrDateTime());
           /*
           fprintf (stderr, "\nFubon Response Model Profile \n");
           dbhandle->ExecSQLQry(SQLCommands[Generate_Summary], ds);
           ds->First();
           fprintf (stderr, "     類    別\t評    分   總  數\n");
           fprintf (stderr, "    =========\t========  =======\n");
           while (!ds->Eof) {
              segment = ds->FieldValues["segment"];
              group_count = ds->FieldValues["group_count"];
              if (ds->FieldValues["twentile"].IsNull()) {
                 fprintf (stderr, "    %s\t無法評分 = %d\n", segment.c_str(), group_count);
              }
              else {
                 twentile = ds->FieldValues["twentile"];
                 fprintf (stderr, "    %s\t%8d = %d\n", segment.c_str(), twentile, group_count);
              }
              ds->Next();
           }
           */
           break;
        default:
           DEBUG (stderr, "%s: [Step %d] %s\n", CurrDateTime(), i, SQLNames[step[i]]);
           dbhandle->ExecSQLCmd(SQLCommands[step[i]]);
     }
 }

 dbhandle->CloseDatabase();
 } catch (Exception &E) {
    fprintf(stderr, "%s: %s\n", CurrDateTime(), E.Message.c_str());
 }
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
