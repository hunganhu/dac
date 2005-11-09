/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2004
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: dac_pl_cal.sqC 
**    
** Description: 
**
**           
****************************************************************************/

#include <string.h>
#include <stdio.h>
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
#include "risk_model.h"
char MESSAGE[1024];

int dac_pl_cal(char *case_sn, char *alias, char *uid, char *upw, char *error_message)
{
  DbEmb db;
  RiskModel rm;
  int rc = 0;
  
  Info("Enter dac_pl_cal()\n");
  Info("DB name= %s\n", alias);  
  Info("User ID= %s\n", uid);  
  Info("User PW= %s\n", upw);  
  Info("Case SN= %s\n", case_sn);  
  memset(MESSAGE, '\0', sizeof(MESSAGE));
  
  db.setDb(alias, uid, upw);
  rc = db.Connect();
  if (rc != 0)  {
     strcpy (error_message, "Connect to DB failed");
     return rc;
  }
/*
  rc = rm.CreateWorkingTables();
  if (rc != 0)  {
     strcpy (error_message, MESSAGE);
     return rc;
  }
*/
  rc = rm.PrepareJcicTables(case_sn);
  if (rc != 0)  {
     strcpy (error_message, MESSAGE);
     return rc;
  }
  rc = rm.Prescreen(case_sn);
  if (rc != 0)  {
     strcpy (error_message, MESSAGE);
     return rc;
  }
  rc = rm.GeneratePdacoScore();
  if (rc != 0)  {
     strcpy (error_message, MESSAGE);
     return rc;
  }

  rc = db.Disconnect();
  if (rc != 0)
  {
    strcpy (error_message, "Disconnect to DB failed");
    return rc;
  }

  Info("Exit dac_pl_cal()\n");

  return 0;
}
