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
   using namespace std; 
#else
   #include <iostream.h>
#endif
#include "dac_pl_cal.h"

int main(int argc, char *argv[])
{
  int rc = 0;
  DbEmb db;
  char alias[32], uid[32], upw[32], case_sn[16], msg[1024];

  // check the command line arguments
  strcpy (alias, "dacdb");
  strcpy (uid, "ejcicap1");
  strcpy (upw, "ejcicap1");
  strcpy (case_sn, "003000016373");

  cout <<"DB name= "<< alias << endl;  
  cout <<"User ID= "<< uid << endl;  
  cout <<"User PW= "<< upw << endl;  
  cout <<"Case SN= "<< case_sn << endl;  
  // connect to the database
  rc = dac_pl_cal(case_sn, alias, uid, upw, msg);
  if (rc != 0)
  {
    cout << msg << endl;
    return rc;
  }
  
  cout << "DB connected" << endl;
  return 0;
} //main
