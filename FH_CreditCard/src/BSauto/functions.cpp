//---------------------------------------------------------------------------
#pragma hdrstop

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include "functions.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
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

String createDate;
String cycleDate;
char statementFile[64];
char accountFile[64];
int    statementCount;
int    accountCount;


/*
  4.1.	CONTROL File (from "Host" computer to "Scoring" computer)

4.1.1.	Naming convention: FHBSAUTO.CTL

4.1.2.	Contents: 4 lines (records)
Line	Description	                                        Format
1	Date and time (in hour) when the file is created	yyyymmddhh
2	Cycle date	Yyyymmdd
3	STATEMENT file: <file name> <space><number of records>	STATEMENT_cycledate_creationdate.csv #######
4	ACCOUNT file: <file name> <space><number of records>	ACCOUNT_cycledate_creationdate.csv #######
*/
bool get_control_info()
{
 char rec[MAX], *name, count[10];
 ifstream controlFile("FHBSAUTO.CTL");
 int stmtCNT, acctCNT;

 if (!controlFile.eof()) {
    controlFile.getline(rec, MAX);
    createDate = rec;
 }
 if (!controlFile.eof()) {
    controlFile.getline(rec, MAX);
    cycleDate = rec;
 }
 if (!controlFile.eof()) {
    controlFile.getline(rec, MAX);
    name = statementFile;
    get_filename(rec, name, count);
    statementCount = atoi(count);
 }
 if (!controlFile.eof()) {
    controlFile.getline(rec, MAX);
    name = accountFile;
    get_filename(rec, name, count);
    accountCount = atoi(count);
 }

 stmtCNT = get_linecount(statementFile);
 acctCNT = get_linecount(accountFile);

 return(true);
}


int get_linecount(char *filename)
{
 int linecount = 0;
 char buffer[MAX];
 FILE *fid;

/* Very slow process, try to use 8k block read (written in C).
 ifstream infile(filename);
 while (!infile.eof()) {
    infile.getline(buffer, MAX);
    linecount++;
 }
*/
 if ((fid = fopen(filename, "r")) == NULL) {
    fprintf (stderr, "%s:Can't open file: %s\n", CurrDateTime(), filename);
    return (1);
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

int get_filename (char *line, char *name, char *count)
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
