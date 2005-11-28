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
#include "loan_app.h"
#include "functions.h"
#ifdef DEBUG
#define debug 1
#else
#define debug 0
#endif

char MESSAGE[1024];

int dac_pl_cal(char *case_sn, char *alias, char *uid, char *upw, char *error_message)
{
  DbEmb db;
  RiskModel rm;
  LoanApp app = LoanApp(case_sn);
  int rc = 0, qualified_guarantor = 0;
  char aID[12], gID[12];
  float aPB, gPB, cut_point;
  int periods, app_amount, productType;
  float max_apr;
  int  ps_code;
  char ps_msg[128];

/*
  Info("Enter dac_pl_cal()\n");
  Info("DB name= %s\n", alias);
  Info("User ID= %s\n", uid);
  Info("User PW= %s\n", upw);
  Info("Case SN= %s\n", case_sn);
*/
  memset(MESSAGE, '\0', sizeof(MESSAGE));

  db.setDb(alias, uid, upw);
  rc = db.Connect();
  if (rc != 0)  {
     strcpy (error_message, "與資料庫連線失敗。");
     return 1;
  }

  rc = app.initial();
  if (rc < 0) {
     strcpy (error_message, MESSAGE);
     return 1;
  } 
/*
  else if (rc > 0) {
     strcpy (error_message, MESSAGE);
     return rc;
  }
*/
/*
  rc = app.get_fin_info();
  if (rc != 0)  {
     strcpy (error_message, MESSAGE);
     return rc;
  }
*/
  strcpy(aID, app.Applicant_id());
  Info("Case SN= %s\n", case_sn);
  Info("IDN= %s\n", aID);

  rc = rm.Calculate_PB(case_sn, aID);
  if (rc < 0) {
     strcpy (error_message, MESSAGE);
     return 1;
  } 

  rc = rm.get_prescreen_status(case_sn, aID, &ps_code, ps_msg);
  app.set_ps_status_a(ps_code,ps_msg);

  max_apr = app.get_max_apr();
  periods = app.get_periods();
  app_amount = app.get_apply_amount();
  cut_point = get_risk_cut_point(max_apr, periods, app_amount);
  app.set_cutpoint(cut_point);

  if (rc < 0) {
     strcpy (error_message, MESSAGE);
     return 1;
  } 
  if (ps_code > 0) {
    // decline & output result
    rc = app.write_result_prescreen_failed();
    if (rc < 0)  {
       strcpy (error_message, MESSAGE);
       return 1;
    }
    strcpy (error_message, MESSAGE);
    return rc;
  }

  rc = rm.get_pb(case_sn, aID, &aPB);
  if (rc < 0) {
     strcpy (error_message, MESSAGE);
     return 1;
  } 
  else if (rc > 0) {
     strcpy (error_message, MESSAGE);
     return rc;
  }
  Info("pb= %f\n", aPB);
  app.set_applicant_pb(aPB);
  rc = rm.CleanTables(case_sn, aID);
  if (rc != 0)  return rc;

  // Calculate PB of guanrantor
  strcpy(gID, app.Guanrantor_id());

  if (gID[0] == '\0')
     qualified_guarantor = 0;
  else {
     rc = rm.Calculate_PB(case_sn, gID);
     if (rc < 0) {
        strcpy (error_message, MESSAGE);
        return 1;
     } 

     rc = rm.get_prescreen_status(case_sn, gID, &ps_code, ps_msg);
     app.set_ps_status_g(ps_code,ps_msg);
     if (rc < 0) {
        strcpy (error_message, MESSAGE);
        return 1;
     } 

     if (ps_code > 0) {
       qualified_guarantor = 0;
     } else {  //rc ==0
       rc = rm.get_pb(case_sn, gID, &gPB);
       app.set_guarantor_pb(gPB);
       if (gPB <= 0.02)
          qualified_guarantor = 1;
     }
     rc = rm.CleanTables(case_sn, gID);
     if (rc != 0)  return rc;
  }

  app.set_guarantor_pass(qualified_guarantor);
  if (qualified_guarantor)
     aPB = aPB /2.0;
  app.set_applicant_pb_adj(aPB);
     
  max_apr = app.get_max_apr();
  periods = app.get_periods();
  app_amount = app.get_apply_amount();
  cut_point = get_risk_cut_point(max_apr, periods, app_amount);
  app.set_cutpoint(cut_point);
  
  if ((productType = app.get_product_type())== 4) {
     rc = app.write_result_others();
  } else {
    if (aPB >= cut_point) {
       rc = app.write_result_declined();
    } else {
       rc = app.write_result_approved();
    }
  }
  
  if (rc < 0) {
     strcpy (error_message, MESSAGE);
     return 1;
  } 
  else if (rc > 0) {
     strcpy (error_message, MESSAGE);
     return rc;
  }


  rc = db.Disconnect();
  if (rc != 0)
  {
    strcpy (error_message, "切斷資料庫連線失敗。");
    return 1;
  }

  Info("Exit dac_pl_cal()\n");

  return 0;
}
