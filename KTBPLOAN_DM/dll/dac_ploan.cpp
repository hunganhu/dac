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
#include "dac_ploan.h"
#ifndef _WRFLOW    //In Project->Options->directories/conditionals, append ";_WRFLOW" to conditinals
 #define DEBUG 0
#else
 #define DEBUG 1
#endif
//---------------------------------------------------------------------------

//#pragma package(smart_init)
#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"

//---------------------------------------------------------------------------
int TNB_Ploan_AM_Campaign(char *msno, char *jcic_inquiry_date, char *app_input_time,
                          char *bank_code, char *ole_db_str, int gender, char *error)
{
 TADOHandler *dbhandle;    // commemt if past from argument
 Variant hostVars[5];
 Loan *ptrLoan;
 PDACO *pdaco_app;
 char input_time[20];
 String bank = bank_code;
// int now;
 int errCode = 0;
 int pass = 1;
 double pdaco_score, income, monthly_debt;
 double npv_value, delta_apr, lowest_delta;
 double max_apr;

 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error, EXPIRATION_MSG);
    return(-1);
 }
 try {
    strcpy (error, "");      // return empty string if stop normally.
    if (bank != "054")               // bank code of KingTown bank
       throw cc_error(PSCODE_1, msno, app_input_time);

    dbhandle = new TADOHandler();    // commemt if past from argument
    dbhandle->OpenDatabase(ole_db_str);  // commemt if past from argument

 // Check credit card black list
    if (check_credit_card_block(dbhandle, msno) > 0)
       throw cc_error(PSCODE_110, msno, app_input_time);
 // Get application info: amount, term, apr, and fee
 //   ptrLoan = new Loan(msno, app_input_time);
 //   ptrLoan->app_info_validate(msno, dbhandle);
 // new risk model and calculate jcic variables
    pdaco_app = new PDACO(msno, app_input_time);
    pass = pdaco_app->GeneratePdaco61Score(dbhandle);
 // calculate NPV
    if (pass) {
       if (pdaco_app->getPdaco61PB() > pdaco_app->GetPbCap())
          throw cc_error(PSCODE_109, msno, app_input_time);
       // calculate NPV
         
    }
 // upsell
 //

 } catch(cc_error &Err){
   // Store screen-out result
     strcpy (error, Err.ShowMessage());
     errCode = -1;
 } catch (Exception &E) {
     strcpy (error, E.Message.c_str());
     errCode = -1;
 }
// delete ptrLoan;
 delete dbhandle;
 return (errCode);
}

unsigned int check_credit_card_block(TADOHandler *handler, const AnsiString &msn)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);
 String sql_stmt;
 unsigned int return_value = 0x0;

 try {
     sql_stmt = "SELECT * FROM CREDIT_BLOCK_LIST WHERE MSN = :msn ";
     hostVars[0] = msn;
     handler->ExecSQLQry(sql_stmt.c_str(), hostVars, 0, ds);
     if (ds->RecordCount > 0)
        return_value = 0x800;
 } catch (Exception &E) {
     ds->Close();
     delete ds;
     throw;
 }
 ds->Close();
 delete ds;
 return (return_value);
};

/*
int FM_New(char *msn, char *ole_db, char *error_message)
{
 TADOHandler *dbhandle;    // commemt if past from argument
 Variant hostVars[20];
 Loan *ptrLoan;
 PDACO *pdaco_app, *pdaco_cos, *pdaco_gua;
 char input_time[20];
 int app_seg, cos_seg, gua_seg;
 int app_pscode, cos_pscode, gua_pscode;
 int pdacoPath, incomePath, ms101Path;
 String appMsg, cosMsg, guaMsg, suggMsg, reasonMsg;
// int now;
 int errCode = 0;
 int dispCode;
 double pdaco_score, income, monthly_debt;
 double npv_value, delta_apr, lowest_delta;
 double max_apr;

 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
 try {
    strcpy (error_message, "");      // return empty string if stop normally.
    dbhandle = new TADOHandler();    // commemt if past from argument
    dbhandle->OpenDatabase(ole_db);  // commemt if past from argument

    ptrLoan = new Loan(msn);
    ptrLoan->app_info_validate(msn, dbhandle);
//    now = yrmon_to_mon(ptrLoan->Inquiry_date(), false, "");
    // determine people
    if (ptrLoan->exist_applicant()) {
//       now = yrmon_to_mon(ptrLoan->App_inquiry_date(), false, "");
       pdaco_app = new PDACO(msn, input_time);
       pdaco_app->Prescreen_New(dbhandle);
       app_seg = pdaco_app->Segment();
       app_pscode = pdaco_app->PS_code();
    } else {
       strcpy (error_message, "Applicant ID does not exist.");
       return (-1);
    }

    if (ptrLoan->exist_coapplicant()) {
//       now = yrmon_to_mon(ptrLoan->Cos_inquiry_date(), false, "");
       pdaco_cos = new PDACO(msn, input_time);
       pdaco_cos->Prescreen_New(dbhandle);
       cos_seg =  pdaco_cos->Segment();
       cos_pscode = pdaco_cos->PS_code();
    } else {   // co-applicant does not exist
       cos_seg = seg_N;
       cos_pscode = -1;
    }
    if (ptrLoan->exist_guarantor()) {
//       now = yrmon_to_mon(ptrLoan->Gua_inquiry_date(), false, "");
       pdaco_gua = new PDACO(msn, input_time);
       pdaco_gua->Prescreen_New(dbhandle);
       gua_seg = pdaco_gua->Segment();
       gua_pscode = pdaco_gua->PS_code();
       if (gua_seg > seg_Ip) {
           gua_seg  = seg_Ip;
           if (gua_pscode >= PSCODE_109) { // ignore segment Insufficient-failure message (109,110)
               gua_pscode = PSCODE_0;
           }
       }
       ptrLoan->set_pb_adjustment(gua_seg, pdaco_gua->Pdaco_score());
    } else {  // guarantor does not exist
       gua_seg = seg_N;
       ptrLoan->set_pb_adjustment(gua_seg, 1.0);
       gua_pscode = -1;
    }
    dispCode = overall_lookup( app_seg, cos_seg, gua_seg, app_pscode, cos_pscode, gua_pscode,
                 &pdacoPath, &incomePath, &ms101Path, suggMsg, reasonMsg, dbhandle);
    if (dispCode == 0) {
      // calculate NPV, lowest APR
      switch (pdacoPath) {
        case 1: pdaco_score = pdaco_app->Pdaco_score(); break;
        case 2: pdaco_score = pdaco_cos->Pdaco_score(); break;
        case 3: pdaco_score = (pdaco_app->Pdaco_score()+ pdaco_cos->Pdaco_score())/2.0; break;
      }
      switch (incomePath) {
        case 1: income = ptrLoan->appIncome() / 12.0; break;
        case 2: income = ptrLoan->cosIncome() / 12.0; break;
        case 3: income = (ptrLoan->appIncome()+ ptrLoan->cosIncome()) / 12.0; break;
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

      // calculate NPV with no interest rate bias
      npv_value = ptrLoan->calculate_npv(0.0);
      // find the lowest rate to make NPV > Approved_line (2.0K)
      if (npv_value > 2.0) dispCode = 1;
      else dispCode = 2;

      if (dispCode == 1) {
         lowest_delta = ptrLoan->calculate_optimal_npv();
         ptrLoan->set_lowest_rate(lowest_delta);
      }
      ptrLoan->set_npv(npv_value);  // restore npv based on application
#ifdef _TRACE
     fstream outf;

     outf.open("NPV_trace.txt", ios::app | ios::out);  // Open for ouput and append
     outf << "Case SN: " << msn << " min_apr1: " << ptrLoan->Min_APR1()
          << " min_apr2: " << ptrLoan->Min_APR2()
          << " min_apr3: " << ptrLoan->Min_APR3() << " NPV: " << ptrLoan->Lowest_npv() << endl;
#endif

      final_lookup(app_seg, cos_seg, gua_seg, dispCode, app_pscode, cos_pscode, gua_pscode,
                   suggMsg, reasonMsg, dbhandle);

      // write approve or decline result to db
      write_final_result(dispCode, suggMsg, reasonMsg,
                         ptrLoan, pdaco_app, pdaco_cos, pdaco_gua, dbhandle);
    }
    else {
      // write decline or manual result to db
      write_prescreen_result(dispCode, suggMsg, reasonMsg,
                             ptrLoan, pdaco_app, pdaco_cos, pdaco_gua, dbhandle);
    }


    dbhandle->CloseDatabase();     // commemt if past from argument
 } catch (Loan::DataEx &DE){
     strcpy (error_message, DE.message.c_str());
     errCode = -1;
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
*/
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------
int overall_lookup(int appStatus, int cosStatus, int guaStatus,
                 int appPSCode, int cosPSCode, int guaPSCode,
                 int *pdacoPath, int *incomePath, int *ms101Path,
                 String &dispositionMsg, String &finalMsg, TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);
 int dispCode = 0;
 String appMsg, cosMsg, guaMsg;

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
             if (guaStatus >= seg_Ip) // Insufficient data
                guaMsg = "";
             else
                guaMsg = guaMsg + "(" + PSMsg(guaPSCode) + ")";
       }
       else
          guaMsg = "";
       finalMsg += guaMsg + "。";

       if (! ds->FieldValues["PDACO_CODE"].IsNull())
          *pdacoPath = ds->FieldValues["PDACO_CODE"];
       else
          *pdacoPath = 0;

       if (! ds->FieldValues["INCOME_CODE"].IsNull())
          *incomePath = ds->FieldValues["INCOME_CODE"];
       else
          *incomePath = 0;

       if (! ds->FieldValues["MS101_CODE"].IsNull())
          *ms101Path = ds->FieldValues["MS101_CODE"];
       else
          *ms101Path = 0;
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
                 String &dispositionMsg, String &finalMsg, TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);
 String appMsg, cosMsg, guaMsg;

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

       if (! ds->FieldValues["MESSAGE_GUARANTOR"].IsNull()) {
          guaMsg = ds->FieldValues["MESSAGE_GUARANTOR"];
          if (guaPSCode > 0)
             if (guaStatus >= seg_Ip) // Insufficient data
                guaMsg = "";
             else
                guaMsg = guaMsg + "(" + PSMsg(guaPSCode) + ")";
       }
       else
          guaMsg = "";
       finalMsg += guaMsg;

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

void write_final_result(int &dispCode, String &suggMsg, String &reasonMsg,
                        Loan *ptrLoan, PDACO *pdaco_app, PDACO *pdaco_cos, PDACO *pdaco_gua,
                        TADOHandler *handler)
{
 String sqlstmt;

 if (ptrLoan->get_principal() > 8000)
     suggMsg += "因貸款金額超過800萬元，建議送消金審查部核實擔保品價值及申請人/共同貸款人收入。";
 else if (ptrLoan->get_principal() <= 0) {
     suggMsg = "模組建議婉拒:收入不及負債。";
     dispCode = 2; // decline
 }

 sqlstmt = "INSERT INTO APP_RESULT (CASE_NO, FINAL_DATE, "
           " APP_RSCORE, APP_PB, APP_SCRCODE, APP_SCRMSG,"
           " COS_RSCORE, COS_PB, COS_SCRCODE, COS_SCRMSG,"
           " GUA_RSCORE, GUA_PB, GUA_SCRCODE, GUA_SCRMSG,"
           " INCOME_CONSIDER, LOAN_AMOUNT_CONSIDER, WEIGHTED_APR, MAX_LOAN_CAPACITY, MS101,"
           " FM_PB, NPV, APPROVED_AMOUNT, MIN_RATE1, MIN_RATE2, MIN_RATE3,"
           " SUGG_CODE, SUGG_MSG, REASON_MSG) VALUES(";
 sqlstmt += "'" + ptrLoan->Case_no()+ "','" + ExecutionTime()+ "',";
 if (pdaco_app->Segment() == seg_S)
    sqlstmt += FloatToStr(pdaco_app->Pdaco_score()) + "," +  FloatToStr(pdaco_app->Pdaco_pb());
 else
    sqlstmt +=  " NULL, NULL";
 sqlstmt += "," +IntToStr(PSCode(pdaco_app->PS_code())) + ",'" + PSMsg(pdaco_app->PS_code())+ "'";

 if (ptrLoan->exist_coapplicant()) {  // add co-applicant risk info if exists
    if (pdaco_cos->Segment() == seg_S)
       sqlstmt += "," +  FloatToStr(pdaco_cos->Pdaco_score()) + "," +  FloatToStr(pdaco_cos->Pdaco_pb());
    else
       sqlstmt +=  ", NULL, NULL";
    sqlstmt += "," + IntToStr(PSCode(pdaco_cos->PS_code())) + ",'" + PSMsg(pdaco_cos->PS_code())+ "'";
 }
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";

 if (ptrLoan->exist_guarantor()) {  // add guarantor risk info if exists
    if (pdaco_gua->Segment() == seg_S)
       sqlstmt += "," +  FloatToStr(pdaco_gua->Pdaco_score()) + "," +  FloatToStr(pdaco_gua->Pdaco_pb());
    else
       sqlstmt +=  ", NULL, NULL";
    if (pdaco_gua->Segment() >= seg_Ip)   // insufficient data
       sqlstmt += "," + IntToStr(PSCode(PSCODE_0)) + ",'" + PSMsg(PSCODE_0)+ "'";
    else
       sqlstmt += "," + IntToStr(PSCode(pdaco_gua->PS_code())) + ",'" + PSMsg(pdaco_gua->PS_code())+ "'";
 }
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";

 sqlstmt += "," + FloatToStr(ptrLoan->Monthly_Income() * 1000) + ","+ FloatToStr(ptrLoan->get_principal()* 1000)
          + "," + FloatToStr(ptrLoan->Weighted_APR()) + "," + FloatToStr(ptrLoan->Max_Loan_Capacity())
          + "," + FloatToStr(ptrLoan->Monthly_Debt() * 1000);

 if (ptrLoan->get_principal() <= 0)  // if principal <=0 donot calculate pd and npv
     sqlstmt += ", NULL, NULL";
 else
     sqlstmt += "," + FloatToStr(ptrLoan->get_pd()) + "," + FloatToStr(ptrLoan->get_npv() * 1000);

 if (dispCode == 2) // declined case, set approved amount to 0, and no min APR
     sqlstmt += ", 0, NULL, NULL, NULL";
 else { // approved case
     sqlstmt += "," + FloatToStr(ptrLoan->get_principal() * 1000)
          + "," + FloatToStr(ptrLoan->Min_APR1());
     if (ptrLoan->term2() > 0)
         sqlstmt += "," +  FloatToStr(ptrLoan->Min_APR2());
     else
         sqlstmt += ", NULL";
     if (ptrLoan->term3() > 0)
         sqlstmt += "," +  FloatToStr(ptrLoan->Min_APR3());
     else
         sqlstmt += ", NULL";
 }
 sqlstmt += "," + IntToStr(dispCode) + ",'" + suggMsg + "','" + reasonMsg + "')"; // add suggestion message
#ifdef _TRACE
     fstream outf;
     outf.open("PATH_trace.txt", ios::app | ios::out);  // Open for ouput and append
     outf << "Case SN:" <<  ptrLoan->Case_no().c_str() <<": " << sqlstmt.c_str() << endl;
#endif

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
 sqlstmt += "'" + ptrLoan->Case_no()+ "','" + ExecutionTime()+ "',";
 if (pdaco_app->Segment() == seg_S)
    sqlstmt += FloatToStr(pdaco_app->Pdaco_score()) + "," +  FloatToStr(pdaco_app->Pdaco_pb());
 else
    sqlstmt +=  " NULL, NULL";
 sqlstmt += "," + IntToStr(PSCode(pdaco_app->PS_code())) + ",'" + PSMsg(pdaco_app->PS_code())+ "'";

 if (ptrLoan->exist_coapplicant()) {  // add co-applicant risk info if exists
    if (pdaco_cos->Segment() == seg_S)
       sqlstmt += "," +  FloatToStr(pdaco_cos->Pdaco_score()) + "," +  FloatToStr(pdaco_cos->Pdaco_pb());
    else
       sqlstmt +=  ", NULL, NULL";
    sqlstmt += "," + IntToStr(PSCode(pdaco_cos->PS_code())) + ",'" + PSMsg(pdaco_cos->PS_code())+ "'";
 }
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";

 if (ptrLoan->exist_guarantor()) {  // add guarantor risk info if exists
    if (pdaco_gua->Segment() == seg_S)
       sqlstmt += "," +  FloatToStr(pdaco_gua->Pdaco_score()) + "," +  FloatToStr(pdaco_gua->Pdaco_pb());
    else
       sqlstmt +=  ", NULL, NULL";
    if (pdaco_gua->Segment() >= seg_Ip)   // insufficient data
       sqlstmt += "," + IntToStr(PSCode(PSCODE_0)) + ",'" + PSMsg(PSCODE_0)+ "'";
    else
       sqlstmt += "," + IntToStr(PSCode(pdaco_gua->PS_code())) + ",'" + PSMsg(pdaco_gua->PS_code())+ "'";
 }
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";

 sqlstmt += "," + IntToStr(dispCode) + ",'" + suggMsg + "','" + reasonMsg + "')"; // add suggestion message
#ifdef _TRACE
     fstream outf;
     outf.open("PATH_trace.txt", ios::app | ios::out);  // Open for ouput and append
     outf << "Case SN:" << ptrLoan->Case_no().c_str() <<": " << sqlstmt.c_str() << endl;
#endif

 try {
    handler->ExecSQLCmd(sqlstmt.c_str());
 } catch (Exception &E) {
    throw;
 }
}
//---------------------------------------------------------------------------
void write_bal_transfer_result(Loan *ptrLoan, PDACO *pdaco_app, PDACO *pdaco_cos, PDACO *pdaco_gua,
                        TADOHandler *handler)
{
 String sqlstmt;

 sqlstmt = "INSERT INTO APP_RESULT (CASE_NO, FINAL_DATE, "
           " APP_RSCORE, APP_PB, APP_SCRCODE, APP_SCRMSG,"
           " COS_RSCORE, COS_PB, COS_SCRCODE, COS_SCRMSG,"
           " GUA_RSCORE, GUA_PB, GUA_SCRCODE, GUA_SCRMSG) VALUES (";
 sqlstmt += "'" + ptrLoan->Case_no()+ "','" + ExecutionTime()+ "',";
 if (pdaco_app->Segment() == seg_S)
    sqlstmt += FloatToStr(pdaco_app->Pdaco_score()) + "," +  FloatToStr(pdaco_app->Pdaco_pb());
 else
    sqlstmt +=  " NULL, NULL";
 if (pdaco_app->Segment() >= seg_Ip)  // insufficient data, show normal message
    sqlstmt += "," + IntToStr(PSCode_BT(PSCODE_0)) + ",'" + PSMsg_BT(PSCODE_0)+ "'";
 else
    sqlstmt += "," + IntToStr(PSCode_BT(pdaco_app->PS_code())) + ",'" + PSMsg_BT(pdaco_app->PS_code())+ "'";

 if (ptrLoan->exist_coapplicant()) {  // add co-applicant risk info if exists
    if (pdaco_cos->Segment() == seg_S)
       sqlstmt += "," +  FloatToStr(pdaco_cos->Pdaco_score()) + "," +  FloatToStr(pdaco_cos->Pdaco_pb());
    else
       sqlstmt +=  ", NULL, NULL";
    if (pdaco_cos->Segment() >= seg_Ip)  // insufficient data, show normal message
       sqlstmt += "," + IntToStr(PSCode_BT(PSCODE_0)) + ",'" + PSMsg_BT(PSCODE_0)+ "'";
    else
       sqlstmt += "," + IntToStr(PSCode_BT(pdaco_cos->PS_code())) + ",'" + PSMsg_BT(pdaco_cos->PS_code())+ "'";
 }
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";

 if (ptrLoan->exist_guarantor()) {  // add guarantor risk info if exists
    if (pdaco_gua->Segment() == seg_S)
       sqlstmt += "," +  FloatToStr(pdaco_gua->Pdaco_score()) + "," +  FloatToStr(pdaco_gua->Pdaco_pb());
    else
       sqlstmt +=  ", NULL, NULL";
    if (pdaco_gua->Segment() >= seg_Ip)  // insufficient data, show normal message
       sqlstmt += "," + IntToStr(PSCode_BT(PSCODE_0)) + ",'" + PSMsg_BT(PSCODE_0)+ "'";
    else
       sqlstmt += "," + IntToStr(PSCode_BT(pdaco_gua->PS_code())) + ",'" + PSMsg_BT(pdaco_gua->PS_code())+ "'";
 }
 else
    sqlstmt +=  ", NULL, NULL, NULL, NULL";
 sqlstmt +=  ")";
#ifdef _TRACE
     fstream outf;
     outf.open("PATH_trace.txt", ios::app | ios::out);  // Open for ouput and append
     outf << "Case SN:" << ptrLoan->Case_no().c_str() <<": " << sqlstmt.c_str() << endl;
#endif

 try {
    handler->ExecSQLCmd(sqlstmt.c_str());
 } catch (Exception &E) {
    throw;
 }
}
*/
//---------------------------------------------------------------------------


