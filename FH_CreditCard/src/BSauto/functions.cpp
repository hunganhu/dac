//---------------------------------------------------------------------------
#pragma hdrstop

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include "AdoHandle.h"
#include "functions.h"
#include "BSautoSQL.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
/*
4.1.	CONTROL File (from "Host" computer to "Scoring" computer)
4.1.1.	Naming convention: FHBSAUTO.CTL
4.1.2.	Contents: 4 lines (records)
Line	Description	                                        Format
====    ======================================================  ============================================
1	Date and time (in hour) when the file is created	yyyymmddhh
2	Cycle date	                                        yyyymmdd
3	STATEMENT file: <file name> <space><number of records>	STATEMENT_cycledate_creationdate.csv #######
4	ACCOUNT file: <file name> <space><number of records>	ACCOUNT_cycledate_creationdate.csv #######

5.	Return Codes
Code	Message
0	Success
1	Control file does not exist
2	Statement file does not exist
3	Account file does not exist
4	Statement - number of records mismatch between text file and temp table
5	Account - number of records mismatch between text file and temp table
6	Statement - number of records mismatch between temp and production table
7	Account - number of records mismatch between temp and production table

*/
#define Char_Value()        (* ptrbuf)
#define Char_Addr()         (ptrbuf)
#define Char_Next()         (* (ptrbuf + 1))
#define Char_Move_Ptr()     (++ ptrbuf)
#define Char_Back_Ptr(n)    (ptrbuf -= (n))

/* Definitions for the input buffer */
#define BUF_ZERO_POINT    256
#define BUF_BLOCK_READ    8192
#define BUF_SIZE          (BUF_ZERO_POINT + BUF_BLOCK_READ + 1)

char buffer[BUF_SIZE], *ptrbuf, *begin_line;

ControlFile::ControlFile():
             statementCount(0), accountCount(0), statement_read_count(0),
             account_read_count(0), statement_temp_count(0), account_temp_count(0),
             statement_load_count(0), account_load_count(0)
{
 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
}

//---------------------------------------------------------------------------
/*
  function : get_control_info
  parameter: none
  return :  -1 - file not exist.
            ## - number of records of the file.
*/
int ControlFile::get_control_info()
{
 char rec[MAX], *name, count[10];
 ifstream control_file;
 int stmtCNT, acctCNT;
 char curr_dir[128], control_path[128], syscmd[512];
 char *bs_data;

 current_directory(curr_dir);
 if ((bs_data = getenv("BSAUTO_DATA")) == NULL)
    bs_data = curr_dir;
 sprintf (control_path, "%s\\FHBSAUTO.CTL", bs_data);
 control_file.open(control_path, ios::in);
 if (!control_file)
    return (1);    // Control file does not exist

 if (!control_file.eof()) {
    control_file.getline(rec, MAX);
    strcpy(createDate, rec);
 }
 if (!control_file.eof()) {
    control_file.getline(rec, MAX);
    strcpy(cycleDate, rec);
 }
 if (!control_file.eof()) {
    control_file.getline(rec, MAX);
    name = statementFile;
    get_filename(rec, name, count);     // get file name, line count of statement
    statementCount = atoi(count);
 }
 if (!control_file.eof()) {
    control_file.getline(rec, MAX);
    name = accountFile;
    get_filename(rec, name, count);     // get file name, line count of account
    accountCount = atoi(count);
 }

 sprintf (control_path, "%s\\%s", bs_data, statementFile);
 statement_read_count = get_linecount(control_path);
 if (statement_read_count == -1)
    return 2;    // statement file does not exist
 else if (statement_read_count != statementCount)
    return 4;    // #rec of statement indicated in control file and actual #rec mismatch.

 sprintf (control_path, "%s\\%s", bs_data, accountFile);
 account_read_count = get_linecount(control_path);
 if (account_read_count == -1)
    return 3;    // account file does not exist
 else if (account_read_count != accountCount)
    return 5;    // #rec of account indicated in control file and actual #rec mismatch.


 return(0);
}
//---------------------------------------------------------------------------

void ControlFile::bulk_insert(TADOHandler *dbhandle)
{
 char curr_dir[32];
 char *bs_home, *bs_data;
 char  sqlcmd[512];

 current_directory(curr_dir);
 if ((bs_home = getenv("BSAUTO_HOME")) == NULL)
    bs_home = curr_dir;
 if ((bs_data = getenv("BSAUTO_DATA")) == NULL)
    bs_data = curr_dir;

 try {
  sprintf (sqlcmd, SQLCommands[Bulk_Insert_Data_STMT],
          bs_data, statementFile, bs_home);
  dbhandle->ExecSQLCmd(sqlcmd);
  sprintf (sqlcmd, SQLCommands[Bulk_Insert_Data_ACCT],
          bs_data, accountFile, bs_home);
  dbhandle->ExecSQLCmd(sqlcmd);
 } catch (Exception &E) {
    throw;
 }

 }
//---------------------------------------------------------------------------
char * ControlFile::get_cycledate()
{
  return(cycleDate);
}
//---------------------------------------------------------------------------
char * ControlFile::get_statementFile()
{
  return(statementFile);
}
//---------------------------------------------------------------------------
char * ControlFile::get_accountFile()
{
  return(accountFile);
}
//---------------------------------------------------------------------------
int ControlFile::check_bulk_insert_status(TADOHandler *dbhandle)
{
  Variant hostVars[10];

 try {
    dbhandle->ExecSQLQry(SQLCommands[Check_Statement_Loaded], ds);
    ds->First();
    if (!ds->Eof)
       statement_temp_count = ds->FieldValues["load_count"];

    dbhandle->ExecSQLQry(SQLCommands[Check_Account_Loaded], ds);
    ds->First();
    if (!ds->Eof)
       account_temp_count = ds->FieldValues["load_count"];

    hostVars[0] = cycleDate;
    dbhandle->ExecSQLQry(SQLCommands[Check_Cycledate_Loaded], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof)
       account_cycle_count = ds->FieldValues["load_count"];

    if (statement_read_count != (statement_temp_count + 1))
       return 6;    // #rec of statement read and #rec in temp mismatch.

    if (account_read_count != (account_temp_count + 1))
       return 7;    // #rec of account read and  #rec in temp mismatch.

    if (account_cycle_count != (account_temp_count))
       return 8;    // #rec of account of specific cycle data read and  #rec in temp mismatch.
 } catch (Exception &E) {
    throw;
 }
 return(0);
}
//---------------------------------------------------------------------------
int ControlFile::check_production_insert_status(TADOHandler *dbhandle)
{
 char  sqlcmd[512];

 try {
    dbhandle->ExecSQLCmd(SQLCommands[DROP_PROCEDURE_Load_to_Account]);
    dbhandle->ExecSQLCmd(SQLCommands[CREATE_PROCEDURE_Load_to_Account]);
    dbhandle->ExecSQLCmd(SQLCommands[EXEC_PROCEDURE_Load_to_Account]);
    dbhandle->ExecSQLQry(SQLCommands[Check_Production_Account_Loaded], ds);
    ds->First();
    if (!ds->Eof)
       account_load_count = ds->FieldValues["row_affected"];
    dbhandle->ExecSQLCmd(SQLCommands[DROP_PROCEDURE_Load_to_Account]);

    if (account_temp_count != account_load_count)
       return 10;    // #rec of account in temp and  #rec loaded in production mismatch.

    dbhandle->ExecSQLCmd(SQLCommands[DROP_PROCEDURE_Load_to_Statement]);
    dbhandle->ExecSQLCmd(SQLCommands[CREATE_PROCEDURE_Load_to_Statement]);
    sprintf (sqlcmd, SQLCommands[EXEC_PROCEDURE_Load_to_Statement], cycleDate);
    dbhandle->ExecSQLCmd(sqlcmd);
    dbhandle->ExecSQLQry(SQLCommands[Check_Production_Statement_Loaded], ds);
    ds->First();
    if (!ds->Eof)
       statement_load_count = ds->FieldValues["row_affected"];
    dbhandle->ExecSQLCmd(SQLCommands[DROP_PROCEDURE_Load_to_Statement]);

    if (statement_temp_count != statement_load_count)
       return 9;    // #rec of statement in temp and #rec loaded in production mismatch.

 } catch (Exception &E) {
    throw;
 }

 return(0);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int ControlFile::get_filename (char *line, char *name, char *count)
{
  char *ptr1, *ptr2;
  int  length;

  length = strlen (line);
  ptr1 = line;

  while (Is_White(*ptr1) && length) {  /* Skip spaces, tabs */
    length--;
    ptr1++;
  }
  ptr2 = name;  /* get file name */
  *ptr2 = '\0'; /* initialized */
  while (! Is_White(*ptr1) && length) {
    *ptr2++ = *ptr1++;
    length--;
  }
  *ptr2 = '\0';

  while (Is_White(*ptr1) && length) {  /* Skip spaces, tabs */
    length--;
    ptr1++;
  }

  ptr2 = count;  /* get host */
  *ptr2 = '\0'; /* initialized */
  while (! Is_White(*ptr1) && length) {
    *ptr2++ = *ptr1++;
    length--;
  }
  *ptr2 = '\0';

  return (0);
}

/*
  function : get_linecount
  parameter: filename - name of input file.
  return :  -1 - file not exist.
            ## - number of records of the file.
*/
int get_linecount(char *filename)
{
 int linecount = 0;
 char buffer[MAX];
 FILE *fid;

// Very slow process, try to use 8k block read (written in C).
/*
 ifstream infile;
 infile.open(filename, ios::in);
 if (!infile)
    return (-1);
 infile.getline(buffer, MAX);
 while (!infile.eof()) {
    linecount++;
    infile.getline(buffer, MAX);
 }
 return (linecount);
*/

 if ((fid = fopen(filename, "r")) == NULL) {
    fprintf (stderr, "%s:Can't open file: %s\n", CurrDateTime(), filename);
    return (-1);
 }
 Scan_Read_Buffer (fid);
 while (Char_Value() != ENDFILE) {
    switch (Char_Value()) {
       case ENDBUFFER:
            Scan_Read_Buffer (fid);
            break;
       case LF:
            linecount++;
            Char_Move_Ptr();
            break;
       case CR:
            if (Char_Next() == LF) {
               Char_Move_Ptr();
               linecount++;
               Char_Move_Ptr();
            } else {
               Char_Move_Ptr();
            }
            break;
       default:
            Char_Move_Ptr();
    }
 }
 return (linecount);

}
//---------------------------------------------------------------------------
int  chars_read = 0;
char old_sentinel;

/*
 * This procedure is called when the input buffer has to be refilled.
 * When the input comes from either a file or a pipe, a block discipline
 * (block mode) is used, but when the input comes from the terminal, a
 * line discipline (mode) is used. In both cases the input is placed on
 * array buffer at position BUF_ZERO_POINT. The portion to the left to
 * this point is used in block mode to store the beginning of the partial
 * line read at the end of the last read. In both block and line modes,
 * an ENDBUFFER character is inserted at the end of the last complete
 * line in the buffer. This makes it very easy to detect when more input
 * is needed.
 */
void Scan_Read_Buffer (FILE * fid)

   {int   tail, inx;
    char *ptr, *qtr, *out;

    	/* Move the last incomplete line to the right of BUF_ZERO_POINT */
        /* This is done only in block mode */
    if (chars_read != 0)
       {Char_Value () = old_sentinel;
        tail = (int) buffer - (int) Char_Addr () + BUF_ZERO_POINT
                                                 + chars_read;
        qtr = Char_Addr ();
        begin_line = ptrbuf = ptr = buffer + (BUF_ZERO_POINT - tail);
        for (inx = 0;  inx < tail;  inx ++)
           * ptr ++ = * qtr ++;

       }
    else /* line mode or first time block mode */
        begin_line = ptrbuf = buffer + BUF_ZERO_POINT;

    chars_read = fread (buffer+BUF_ZERO_POINT, sizeof (char),
                            BUF_BLOCK_READ, fid);

    if (chars_read == 0)  /* If end of file, insert ENDFILE token */
        buffer[BUF_ZERO_POINT] = ENDFILE;
    else
       {ptr = buffer + (BUF_ZERO_POINT + chars_read - 1);
        while (*ptr != '\n' && ptr != ptrbuf) ptr --;
        if (ptr == ptrbuf)
           {buffer[BUF_ZERO_POINT + chars_read]     = '\n';
            buffer[BUF_ZERO_POINT + chars_read + 1] = ENDBUFFER;
            chars_read ++;
	   }
        else
           {old_sentinel = * (ptr + 1);
            * (ptr + 1) = ENDBUFFER;
	   }
       }

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
//---------------------------------------------------------------------------
char * Create_date ()
{
 time_t timer;
 struct tm *tblock;
 static char buf[20];

 timer = time(NULL);
 tblock = localtime(&timer);
 sprintf (buf, "%04d%02d%02d%02d", tblock->tm_year+1900, tblock->tm_mon+1,
          tblock->tm_mday, tblock->tm_hour);
 return (buf);
}
//---------------------------------------------------------------------------
static char daytab[2][13] = {
 {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
 {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

bool validate_date(String date)
{
 int year, month, day;
 int leap;

 year = StrToInt(date.SubString(1,4));
 month = StrToInt(date.SubString(5,2));
 day = StrToInt(date.SubString(7,2));

 leap = (year%4 == 0 && year%100 != 0 || year%400 == 0);
 if (month > 12) return false;
 if (day > daytab[leap][month]) return false;

 return(true);
}

//---------------------------------------------------------------------------
char *current_directory(char *path)
{
  strcpy(path, "X:\\");      /* fill string with form of response: X:\ */
  path[0] = 'A' + getdisk();    /* replace X with current drive letter */
  getcurdir(0, path+3);  /* fill rest of string with current directory */
  return(path);
}



