/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: dac_fm.cpp
**
** Description:  functions used for Kingtown Bank First Mortgage
**
****************************************************************************/
//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdio.h>
#include <Math.hpp>
#include "dac_fm.h"
#ifndef _WRFLOW    //In Project->Options->directories/conditionals, append ";_WRFLOW" to conditinals
 #define DEBUG 0
#else
 #define DEBUG 1
#endif
//---------------------------------------------------------------------------

//#pragma package(smart_init)
#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"

//---------------------------------------------------------------------------
int FM_New(char *case_no, char *ole_db, char *error_message)
{
 TADOHandler *dbhandle;    // commemt if past from argument
 String Message = "";
 Variant hostVars[20];
 Loan *ptrLoan;
 PDACO *pdaco_app, *pdaco_cos, *pdaco_gua;
 int app_seg, cos_seg, gua_seg;
 int app_pscode, cos_pscode, gua_pscode;
 int pdacoPath, incomePath, ms101Path;
 String appMsg, cosMsg, guaMsg, suggMsg, reasonMsg;
 int now;
 int errCode = 0;
 int dispCode;
 double pdaco_score, income, monthly_debt;
 double npv_value, max_apr, delta_apr, lowest_delta;
 
 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
 try {
    strcpy (error_message, "");      // return empty string if stop normally.
    dbhandle = new TADOHandler();    // commemt if past from argument
    dbhandle->OpenDatabase(ole_db);  // commemt if past from argument

    ptrLoan = new Loan(case_no);
    ptrLoan->app_info_validate(case_no, dbhandle);
    now = yrmon_to_mon(ptrLoan->Inquiry_date(), false, "");
    // determine people
    if (ptrLoan->exist_applicant()) {
       pdaco_app = new PDACO(case_no, ptrLoan->Applicant(), now);
       pdaco_app->Prescreen_New(dbhandle);
       app_seg = pdaco_app->Segment();
       app_pscode = pdaco_app->PS_code();
    } else {
       strcpy (error_message, "Applicant ID does not exist.");
       return (-1);
    }

    if (ptrLoan->exist_coapplicant()) {
       pdaco_cos = new PDACO(case_no, ptrLoan->Cosigner(), now);
       pdaco_cos->Prescreen_New(dbhandle);
       cos_seg =  pdaco_cos->Segment();
       cos_pscode = pdaco_cos->PS_code();
    } else {   // co-applicant does not exist
       cos_seg = seg_N;
       cos_pscode = -1;
    }
    if (ptrLoan->exist_guarantor()) {
       pdaco_gua = new PDACO(case_no, ptrLoan->Guarantor(), now);
       pdaco_gua->Prescreen_New(dbhandle);
       gua_seg = pdaco_gua->Segment();
       if (gua_seg > seg_Ip) gua_seg  = seg_Ip;
       ptrLoan->set_pb_adjustment(pdaco_gua->Pdaco_score());
       gua_pscode = pdaco_gua->PS_code();
    } else {  // guarantor does not exist
       gua_seg = seg_N;
       ptrLoan->set_pb_adjustment(1.0);
       gua_pscode = -1;
    }
    dispCode = overall_lookup( app_seg, cos_seg, gua_seg, app_pscode, cos_pscode, gua_pscode,
                 appMsg, cosMsg, guaMsg, &pdacoPath, &incomePath, &ms101Path,
                 suggMsg, reasonMsg, dbhandle);
    if (dispCode == 0) {
      // calculate NPV, lowest APR
      switch (pdacoPath) {
        case 1: pdaco_score = pdaco_app->Pdaco_score(); break;
        case 2: pdaco_score = pdaco_cos->Pdaco_score(); break;
        case 3: pdaco_score = (pdaco_app->Pdaco_score()+ pdaco_cos->Pdaco_score())/2.0; break;
      }
      switch (incomePath) {
        case 1: income = ptrLoan->appIncome(); break;
        case 2: income = ptrLoan->cosIncome(); break;
        case 3: income = ptrLoan->appIncome()+ ptrLoan->cosIncome(); break;
      }
      switch (ms101Path) {
        case 1: monthly_debt = pdaco_app->monthly_debt(); break;
        case 2: monthly_debt = pdaco_cos->monthly_debt(); break;
        case 3: monthly_debt = pdaco_app->monthly_debt()+ pdaco_cos->monthly_debt(); break;
      }
      ptrLoan->set_risk_score (pdaco_score);
      ptrLoan->set_monthly_income(income);
      ptrLoan->set_monthly_debt(monthly_debt);
      ptrLoan->set_risk_twentile (pdaco_score);
      ptrLoan->set_principal();
      max_apr = ptrLoan->get_max_apr();

      /* calculate NPV with no interest rate bias*/
      npv_value = ptrLoan->calculate_npv(0.0);
      /* find the lowest rate to make NPV > Approved_line (2.0K) */
      if (npv_value > 2.0) dispCode = 1;
      else dispCode = 2;

      if (npv_value > (ApprovedNPV + Allowance)) {
         delta_apr = 0.0 - max_apr;
         lowest_delta = ptrLoan->find_lowest_rate(0.0, delta_apr);
      }
      else if (npv_value < ApprovedNPV) {
         delta_apr = max_apr + 0.3;
         lowest_delta = ptrLoan->find_lowest_rate(0.0, delta_apr);
      }
      else
         lowest_delta = 0.0;
      ptrLoan->set_npv(npv_value);
      ptrLoan->set_lowest_rate(lowest_delta);

      final_lookup(app_seg, cos_seg, gua_seg, dispCode, app_pscode, cos_pscode, gua_pscode,
                 appMsg, cosMsg, guaMsg, suggMsg, reasonMsg, dbhandle);

      // write approve or decline result to db
      write_final_result(dispCode, suggMsg, reasonMsg,
                         ptrLoan, pdaco_app, pdaco_cos, pdaco_gua,dbhandle);
    }
    else {
      // write decline or manual result to db
      write_prescreen_result(dispCode, suggMsg, reasonMsg,
                             ptrLoan, pdaco_app, pdaco_cos, pdaco_gua, dbhandle);
    }


    dbhandle->CloseDatabase();     // commemt if past from argument
 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
 if (ptrLoan->exist_applicant()) delete pdaco_app;
 if (ptrLoan->exist_coapplicant()) delete pdaco_cos;
 if (ptrLoan->exist_guarantor()) delete pdaco_gua;
 delete ptrLoan;
 delete dbhandle;
 return (errCode);
}

//---------------------------------------------------------------------------
int FM_Reload(char *case_no, char *ole_db, char *error_message)
{
 TADOHandler *dbhandle;    // commemt if past from argument
 String Message = "";
 Variant hostVars[20];
 Loan *ptrLoan;
 PDACO *pdaco_app, *pdaco_cos, *pdaco_gua;
 int app_seg, cos_seg, gua_seg;
 int app_pscode, cos_pscode, gua_pscode;
 int pdacoPath, incomePath, ms101Path;
 String appMsg, cosMsg, guaMsg, suggMsg, reasonMsg;
 int now;
 int errCode = 0;
 int dispCode;
 double pdaco_score, income, monthly_debt;
 double npv_value, max_apr, delta_apr, lowest_delta;

 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
 try {
    strcpy (error_message, "");      // return empty string if stop normally.
    dbhandle = new TADOHandler();    // commemt if past from argument
    dbhandle->OpenDatabase(ole_db);  // commemt if past from argument

    ptrLoan = new Loan(case_no);
    ptrLoan->app_validate_test(case_no, dbhandle);
/*
    now = yrmon_to_mon(ptrLoan->Inquiry_date(), false, "");
    // determine people
    if (ptrLoan->exist_applicant()) {
       pdaco_app = new PDACO(case_no, ptrLoan->Applicant(), now);
       pdaco_app->Prescreen_New(dbhandle);
       app_seg = pdaco_app->Segment();
       app_pscode = pdaco_app->PS_code();
    } else {
       strcpy (error_message, "Applicant ID does not exist.");
       return (-1);
    }

    if (ptrLoan->exist_coapplicant()) {
       pdaco_cos = new PDACO(case_no, ptrLoan->Cosigner(), now);
       pdaco_cos->Prescreen_New(dbhandle);
       cos_seg =  pdaco_cos->Segment();
       cos_pscode = pdaco_cos->PS_code();
    } else {   // co-applicant does not exist
       cos_seg = seg_N;
       cos_pscode = -1;
    }
    if (ptrLoan->exist_guarantor()) {
       pdaco_gua = new PDACO(case_no, ptrLoan->Guarantor(), now);
       pdaco_gua->Prescreen_New(dbhandle);
       gua_seg = pdaco_gua->Segment();
       if (gua_seg > seg_Ip) gua_seg  = seg_Ip;
       ptrLoan->set_pb_adjustment(pdaco_gua->Pdaco_score());
       gua_pscode = pdaco_gua->PS_code();
    } else {  // guarantor does not exist
       gua_seg = seg_N;
       ptrLoan->set_pb_adjustment(1.0);
       gua_pscode = -1;
    }
    dispCode = overall_lookup( app_seg, cos_seg, gua_seg, app_pscode, cos_pscode, gua_pscode,
                 appMsg, cosMsg, guaMsg, &pdacoPath, &incomePath, &ms101Path,
                 suggMsg, reasonMsg, dbhandle);
*/
/*
    if (dispCode == 0) {
      // calculate NPV, lowest APR
      switch (pdacoPath) {
        case 1: pdaco_score = pdaco_app->Pdaco_score(); break;
        case 2: pdaco_score = pdaco_cos->Pdaco_score(); break;
        case 3: pdaco_score = (pdaco_app->Pdaco_score()+ pdaco_cos->Pdaco_score())/2.0; break;
      }
      switch (incomePath) {
        case 1: income = ptrLoan->appIncome(); break;
        case 2: income = ptrLoan->cosIncome(); break;
        case 3: income = ptrLoan->appIncome()+ ptrLoan->cosIncome(); break;
      }
      switch (ms101Path) {
        case 1: monthly_debt = pdaco_app->monthly_debt(); break;
        case 2: monthly_debt = pdaco_cos->monthly_debt(); break;
        case 3: monthly_debt = pdaco_app->monthly_debt()+ pdaco_cos->monthly_debt(); break;
      }
      ptrLoan->set_risk_score (pdaco_score);
      ptrLoan->set_monthly_income(income);
      ptrLoan->set_monthly_debt(monthly_debt);
      ptrLoan->set_risk_twentile (pdaco_score);
      ptrLoan->set_principal();
*/
      max_apr = ptrLoan->get_max_apr();
      // calculate NPV with no interest rate bias
      npv_value = ptrLoan->calculate_npv(0.0);
      // find the lowest rate to make NPV > Approved_line (2.0K)
      if (npv_value > 2.0) dispCode = 1;
      else dispCode = 2;

      if (npv_value > (ApprovedNPV + Allowance)) {
         delta_apr = 0.0 - max_apr;
         lowest_delta = ptrLoan->find_lowest_rate(0.0, delta_apr);
      }
      else if (npv_value < ApprovedNPV) {
         delta_apr = max_apr + 0.3;
         lowest_delta = ptrLoan->find_lowest_rate(0.0, delta_apr);
      }
      else
         lowest_delta = 0.0;
      ptrLoan->set_npv(npv_value);
      ptrLoan->set_lowest_rate(lowest_delta);
/*
      final_lookup(app_seg, cos_seg, gua_seg, dispCode, app_pscode, cos_pscode, gua_pscode,
                 appMsg, cosMsg, guaMsg, suggMsg, reasonMsg, dbhandle);

      // write approve or decline result to db
      write_final_result(dispCode, suggMsg, reasonMsg,
                         ptrLoan, pdaco_app, pdaco_cos, pdaco_gua,dbhandle);
    }
    else {
      // write decline or manual result to db
      write_prescreen_result(dispCode, suggMsg, reasonMsg,
                             ptrLoan, pdaco_app, pdaco_cos, pdaco_gua, dbhandle);
    }
*/

    dbhandle->CloseDatabase();     // commemt if past from argument
 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
// if (ptrLoan->exist_applicant()) delete pdaco_app;
// if (ptrLoan->exist_coapplicant()) delete pdaco_cos;
// if (ptrLoan->exist_guarantor()) delete pdaco_gua;
 delete ptrLoan;
 delete dbhandle;
 return (errCode);
}

//---------------------------------------------------------------------------
int FM_Transfer(char *case_no, char *ole_db, char *error_message)
{
 int errCode = 0;

 return (errCode);
}

//---------------------------------------------------------------------------
int overall_lookup(int appStatus, int cosStatus, int guaStatus,
                 int appPSCode, int cosPSCode, int guaPSCode,
                 String appMsg, String cosMsg, String guaMsg,
                 int *pdacoPath, int *incomePath, int *ms101Path,
                 String dispositionMsg, String finalMsg, TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);
 int dispCode = 0;

 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    finalMsg = "";       // initialize to empty string
    hostVars[0] = appStatus;
    hostVars[1] = cosStatus;
    hostVars[2] = guaStatus;
    handler->ExecSQLQry(SQLCommands[Get_Overall_Lookup], hostVars, 2, ds);

    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["DISPOSITION_CODE"].IsNull())
          dispCode = ds->FieldValues["DISPOSITION_CODE"];

       if (! ds->FieldValues["DISPOSITION_MESSAGE"].IsNull())
          dispositionMsg = ds->FieldValues["DISPOSITION_MESSAGE"];
       else
          dispositionMsg = "";

       if (! ds->FieldValues["APPLICANT_MESSAGE"].IsNull()) {
          appMsg = ds->FieldValues["APPLICANT_MESSAGE"];
          if (appPSCode > 0)
              appMsg = appMsg + "(" + PSMsg(appPSCode) + ")";
       }
       else
          appMsg = "";
       finalMsg += appMsg;

       if (! ds->FieldValues["CO_APPLICANT_MESSAGE"].IsNull()) {
          cosMsg = ds->FieldValues["CO_APPLICANT_MESSAGE"];
          if (cosPSCode > 0)
              cosMsg = cosMsg + "(" + PSMsg(cosPSCode) + ")";
       }
       else
          cosMsg = "";
       finalMsg += cosMsg;

       if (! ds->FieldValues["GUARANTOR_MESSAGE"].IsNull()) {
          guaMsg = ds->FieldValues["GUARANTOR_MESSAGE"];
          if (guaPSCode > 0)
              guaMsg = guaMsg + "(" + PSMsg(guaPSCode) + ")";
       }
       else
          guaMsg = "";
       finalMsg += guaMsg + "。";

       if (! ds->FieldValues["PDACO_CODE"].IsNull())
          pdacoPath = ds->FieldValues["PDACO_CODE"];
       else
          pdacoPath = 0;

       if (! ds->FieldValues["INCOME_CODE"].IsNull())
          incomePath = ds->FieldValues["INCOME_CODE"];
       else
          incomePath = 0;

       if (! ds->FieldValues["MS101_CODE"].IsNull())
          ms101Path = ds->FieldValues["MS101_CODE"];
       else
          ms101Path = 0;
    }
 } catch (Exception &E) {
    ds->Close();
    delete ds;
    throw;
 }
  ds->Close();  // close dataset before delete and drop an object outside the try block,
                // otherwise result in "too many consecutive exceptions"
  delete ds;
  return (dispCode);
}

//---------------------------------------------------------------------------
int final_lookup(int appStatus, int cosStatus, int guaStatus, int disp_code,
                 int appPSCode, int cosPSCode, int guaPSCode,
                 String appMsg, String cosMsg, String guaMsg,
                 String dispositionMsg, String finalMsg, TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);

 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    finalMsg = "";       // initialize to empty string
    hostVars[0] = appStatus;
    hostVars[1] = cosStatus;
    hostVars[2] = guaStatus;
    hostVars[3] = disp_code;
    handler->ExecSQLQry(SQLCommands[Get_Final_Lookup], hostVars, 3, ds);

    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["MESSAGE_DISPOSITION"].IsNull())
          dispositionMsg = ds->FieldValues["MESSAGE_DISPOSITION"];
       else
          dispositionMsg = "";

       if (! ds->FieldValues["APPLICANT_MESSAGE"].IsNull()) {
          appMsg = ds->FieldValues["APPLICANT_MESSAGE"];
          if (appPSCode > 0)
              appMsg = appMsg + "(" + PSMsg(appPSCode) + ")";
       }
       else
          appMsg = "";
       finalMsg += appMsg;

       if (! ds->FieldValues["CO_APPLICANT_MESSAGE"].IsNull()) {
          cosMsg = ds->FieldValues["CO_APPLICANT_MESSAGE"];
          if (cosPSCode > 0)
              cosMsg = cosMsg + "(" + PSMsg(cosPSCode) + ")";
       }
       else
          cosMsg = "";
       finalMsg += cosMsg;

       if (! ds->FieldValues["GUARANTOR_MESSAGE"].IsNull()) {
          guaMsg = ds->FieldValues["GUARANTOR_MESSAGE"];
          if (guaPSCode > 0)
              guaMsg = guaMsg + "(" + PSMsg(guaPSCode) + ")";
       }
       else
          guaMsg = "";
       finalMsg += guaMsg + "。";

    }
 } catch (Exception &E) {
    ds->Close();
    delete ds;
    throw;
 }
  ds->Close();  // close dataset before delete and drop an object outside the try block,
                // otherwise result in "too many consecutive exceptions"
  delete ds;
  return (0);
}
//---------------------------------------------------------------------------

void write_final_result(int dispCode, String suggMsg, String reasonMsg,
                        Loan *ptrLoan, PDACO *pdaco_app, PDACO *pdaco_cos, PDACO *pdaco_gua,
                        TADOHandler *handler)
{
 String sqlstmt;

 if (ptrLoan->get_principal() > 8000)
     suggMsg += "，因貸款金額超過800萬元，建議送消金審查部核實擔保品價值及申請人/共同貸款人收入。";

 sqlstmt = "INSERT INTO APP_RESULT (CASE_NO, FINAL_DATE, "
           " APP_RSCORE, APP_PB, APP_SCRCODE, APP_SCRMSG,"
           " COS_RSCORE, COS_PB, COS_SCRCODE, COS_SCRMSG,"
           " GUA_RSCORE, GUA_PB, GUA_SCRCODE, GUA_SCRMSG,"
           " INCOME_CONSIDER, LOAN_AMOUNT_CONSIDER, WEIGHTED_APR, MAX_LOAN_CAPACITY, MS101,"
           " FM_PB, NPV, APPROVED_AMOUNT, MIN_RATE1, MIN_RATE2, MIN_RATE3,"
           " SUGG_CODE, SUGG_MSG, REASON_MSG) VALUES(";
 sqlstmt += "'" + ptrLoan->Case_no()+ "','" + ExecutionTime()+ "'," +
            pdaco_app->Pdaco_score()+ "," + pdaco_app->Pdaco_pb()+ "," +
            pdaco_app->PS_code() + ",'" + PSMsg(pdaco_app->PS_code())+ "'";

 if (ptrLoan->exist_coapplicant())  // add co-applicant risk info if exists
    sqlstmt += "," + FloatToStr(pdaco_cos->Pdaco_score()) + "," +  FloatToStr(pdaco_cos->Pdaco_pb())+ "," +
            IntToStr(pdaco_cos->PS_code()) + ",'" + PSMsg(pdaco_cos->PS_code())+ "'";
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";

 if (ptrLoan->exist_guarantor())  // add guarantor risk info if exists
    sqlstmt += "," +  FloatToStr(pdaco_cos->Pdaco_score()) + "," +  FloatToStr(pdaco_cos->Pdaco_pb())+ "," +
            IntToStr(pdaco_cos->PS_code()) + ",'" + PSMsg(pdaco_cos->PS_code())+ "'";
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";

 sqlstmt += "," + FloatToStr(ptrLoan->Monthly_Income() * 1000) + ","+ FloatToStr(ptrLoan->get_principal()* 1000)
          + "," + FloatToStr(ptrLoan->Weighted_APR()) + "," + FloatToStr(ptrLoan->Max_Loan_Capacity() * 1000)
          + "," + FloatToStr(ptrLoan->Monthly_Debt() * 1000) +  "," + FloatToStr(ptrLoan->get_pd())
          + "," + FloatToStr(ptrLoan->get_npv() * 1000) + "," + FloatToStr(ptrLoan->get_principal() * 1000)
          + "," + FloatToStr(ptrLoan->Min_APR1()) + "," +  FloatToStr(ptrLoan->Min_APR1())
          + "," + FloatToStr(ptrLoan->Min_APR1());
 sqlstmt += "," + IntToStr(dispCode) + ",'" + suggMsg + "','" + reasonMsg + "')"; // add suggestion message

 try {
    handler->ExecSQLCmd(sqlstmt.c_str());
 } catch (Exception &E) {
    throw;
 }
}
//---------------------------------------------------------------------------
void write_prescreen_result(int dispCode, String suggMsg, String reasonMsg,
                        Loan *ptrLoan, PDACO *pdaco_app, PDACO *pdaco_cos, PDACO *pdaco_gua,
                        TADOHandler *handler)
{
 String sqlstmt;

 sqlstmt = "INSERT INTO APP_RESULT (CASE_NO, FINAL_DATE, "
           " APP_RSCORE, APP_PB, APP_SCRCODE, APP_SCRMSG,"
           " COS_RSCORE, COS_PB, COS_SCRCODE, COS_SCRMSG,"
           " GUA_RSCORE, GUA_PB, GUA_SCRCODE, GUA_SCRMSG,"
           " SUGG_CODE, SUGG_MSG, REASON_MSG) VALUES (";
 sqlstmt += "'" + ptrLoan->Case_no()+ "','" + ExecutionTime()+ "'," +
            pdaco_app->Pdaco_score()+ "," + pdaco_app->Pdaco_pb()+ "," +
            pdaco_app->PS_code() + ",'" + PSMsg(pdaco_app->PS_code())+ "'";

 if (ptrLoan->exist_coapplicant())  // add co-applicant risk info if exists
    sqlstmt += "," + FloatToStr(pdaco_cos->Pdaco_score()) + "," +  FloatToStr(pdaco_cos->Pdaco_pb())+ "," +
            IntToStr(pdaco_cos->PS_code()) + ",'" + PSMsg(pdaco_cos->PS_code())+ "'";
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";

 if (ptrLoan->exist_guarantor())  // add guarantor risk info if exists
    sqlstmt += "," +  FloatToStr(pdaco_cos->Pdaco_score()) + "," +  FloatToStr(pdaco_cos->Pdaco_pb())+ "," +
            IntToStr(pdaco_cos->PS_code()) + ",'" + PSMsg(pdaco_cos->PS_code())+ "'";
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";

 sqlstmt += "," + IntToStr(dispCode) + ",'" + suggMsg + "','" + reasonMsg + "')"; // add suggestion message

 try {
    handler->ExecSQLCmd(sqlstmt.c_str());
 } catch (Exception &E) {
    throw;
 }
}
//---------------------------------------------------------------------------


