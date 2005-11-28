/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: main.C 
**    
** Description: How to modify a database
**
**         This is a tutorial sample showing basic database
**         modification. For a more comprehensive sample, see dbuse.sqC.
**           
****************************************************************************/
#include <string.h>
#include <sqlenv.h>
#include <sqlutil.h>
#include "utilemb.h"
#if ((__cplusplus >= 199711L) && !defined DB2HP) || \
    (DB2LINUX && (__LP64__ || (__GNUC__ >= 3)) )
   #include <iostream>
   #include <fstream>
   using namespace std; 
#else
   #include <iostream.h>
   #include <fstream.h>
#endif
#include "dac_pl_cal.h"


int main(int argc, char *argv[])
{
  int rc = 0;
  char alias[32], uid[32], upw[32], case_sn[16], msg[1024];
  ifstream infile("case_list");
//  DbEmb db;
//  struct sqlca sqlca;
  int index = 0;
/*
  EXEC SQL BEGIN DECLARE SECTION;
    char Case_Sn[20];
  EXEC SQL END DECLARE SECTION;
*/
  // check the command line arguments
  strcpy (alias, "dacdb");
  strcpy (uid, "ejcicap1");
  strcpy (upw, "ejcicap1");
  strcpy (case_sn, "000000002950");

//  rc = dac_pl_cal(case_sn, alias, uid, upw, msg);
  if (rc != 0) cout << CurrDateTime() << ": " << msg << endl; 

  while (!infile.eof()) { 
     infile.getline(case_sn, 16);
     index++;
     cout << CurrDateTime() << ": " << index << "- " << case_sn << endl; 
     rc = dac_pl_cal(case_sn, alias, uid, upw, msg);
     if (rc != 0) cout << CurrDateTime() << ": " << msg << endl; 
  }

  return 0;
} //main
