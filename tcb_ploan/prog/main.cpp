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
  strcpy (case_sn, "003000016373");

  rc = dac_pl_cal(case_sn, alias, uid, upw, msg);
/*
  while (!infile.eof()) { 
     infile.getline(case_sn, 16);
     index++;
     cout << CurrDateTime() << ": " << index << "- " << case_sn << endl; 
     rc = dac_pl_cal(case_sn, alias, uid, upw, msg);
     if (rc != 0) cout << CurrDateTime() << ": " << msg << endl; 
  }
*/
/*
  db.setDb(alias, uid, upw);
  rc = db.Connect();
  if (rc != 0)  {
     strcpy (msg, "Connect to DB failed");
     return rc;
  }
  // declare cursor
  EXEC SQL DECLARE c1 CURSOR FOR
     select case_sn from db2inst1.app_info order by case_sn;

  // open cursor
  EXEC SQL OPEN c1;
  EMB_SQL_CHECK("cursor -- open");

  // fetch cursor
  EXEC SQL FETCH c1 INTO :Case_Sn;
  EMB_SQL_CHECK("cursor -- fetch");

  while (sqlca.sqlcode != 100) {
    index++;
    cout << CurrDateTime() << ": " << index << "- " << Case_Sn << endl; 
    rc = dac_pl_cal(Case_Sn, alias, uid, upw, msg);
    if (rc != 0) cout << CurrDateTime() << ": " << msg << endl; 
    
    EXEC SQL FETCH c1 INTO :Case_Sn;
    Info("SQLCODE= %d\n", sqlca.sqlcode);  
    EMB_SQL_CHECK("cursor -- fetch in loop");
    Info("SQLCODE= %d\n", sqlca.sqlcode);  

  }

  // close cursor
  EXEC SQL CLOSE c1;
  EMB_SQL_CHECK("cursor -- close");

  rc = db.Disconnect();
  if (rc != 0)
  {
    strcpy (msg, "Disconnect to DB failed");
    return rc;
  }
*/
  return 0;
} //main
