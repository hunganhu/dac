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
 int i, errCode = 0;
 int pass = 1;
 double pdaco_score, income, monthly_debt;
 double npv_value, pb_value, delta_apr, lowest_delta, orig_npv;
 double optimal_amount, optimal_pb, optimal_npv;
 double max_apr;
 String note;

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

 // new risk model and calculate jcic variables
    pdaco_app = new PDACO(msno, app_input_time);
    pass = pdaco_app->GeneratePdaco61Score(dbhandle);
    note = pdaco_app->vam102_message(dbhandle);
// npv test only, get loan info from table
//      pass = pdaco_app->input_npv_test(dbhandle);
 // calculate NPV
    if (pass == 0) {
       // calculate NPV
       ptrLoan = new Loan(msno, pdaco_app->getLoanAmount(), pdaco_app->getApr(),
                          pdaco_app->getTerm(), pdaco_app->getAppFee(), pdaco_app->getPdaco61PB());
       orig_npv = optimal_npv = ptrLoan->calculate_npv(0.0); // delta_apr = 0.0

       // initial optimal amount, npv, and corresponding pb
       optimal_amount = pdaco_app->getLoanAmount();
       optimal_pb = pdaco_app->getPdaco61PB();
       //write_npv(msno, app_input_time, orig_npv, dbhandle); // npv test code

       if (orig_npv <= 0) {
         store_result(msno, app_input_time, pdaco_app, optimal_amount, pdaco_app->getPsCode(),
                    optimal_npv, optimal_pb, note, VERSION, true, dbhandle);
       } else {
         if (pdaco_app->getPdaco61PB() < 0.01 &&
             pdaco_app->getScoreCard() == 5   &&
             pdaco_app->getLoanAmount() == pdaco_app->getRequestAmount()) {
             // upsell
             for (i = pdaco_app->getLoanAmount(); i <= pdaco_app->upperLendableAmount();
                  i = (i + 50000)/ 50000 * 50000) {
                pb_value = pdaco_app->recal_Pdaco61Pb(i, pdaco_app->getApr(), pdaco_app->getTerm());
                npv_value = ptrLoan->recal_npv(0.0, i); // delta_apr = 0.0
                if (npv_value > optimal_npv && pb_value < pdaco_app->getPbCap()) {
                   optimal_amount = i;
                   optimal_npv = npv_value;
                   optimal_pb = pb_value;
                }
             }
             if (i > pdaco_app->upperLendableAmount()) {
                i =  pdaco_app->upperLendableAmount();
                pb_value = pdaco_app->recal_Pdaco61Pb(i, pdaco_app->getApr(), pdaco_app->getTerm());
                npv_value = ptrLoan->recal_npv(0.0, i); // delta_apr = 0.0
                if (npv_value > optimal_npv && pb_value < pdaco_app->getPbCap()) {
                   optimal_amount = i;
                   optimal_npv = npv_value;
                   optimal_pb = pb_value;
                }
             }
         }   // end of upsell
         pdaco_app->postScreen ();
         if (pdaco_app->getPsCode() == 0) {   // pass post screen
            store_result(msno, app_input_time, pdaco_app, optimal_amount, pdaco_app->getPsCode(),
                    optimal_npv, optimal_pb, note, VERSION, true, dbhandle);
         }
         else {  // do NOT pass post screen
            store_result(msno, app_input_time, pdaco_app, 0, pdaco_app->getPsCode(),
                    0, pdaco_app->getPdaco61PB(), note, VERSION, false, dbhandle);
         }
       }

     delete ptrLoan;
    }
    else { // pdaco 6.1 fail
       store_result(msno, app_input_time, pdaco_app, 0, pdaco_app->getPsCode(),
                    0, pdaco_app->getPdaco61PB(), note, VERSION, false, dbhandle);
    }
 } catch(cc_error &Err){
   // Store screen-out result
     strcpy (error, Err.ShowMessage());
     errCode = -1;
 } catch (Exception &E) {
     strcpy (error, E.Message.c_str());
     errCode = -1;
 }
// delete ptrLoan;
 delete pdaco_app;
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

//---------------------------------------------------------------------------
void store_result(const char *idno,
                  const char *input_time,
                  PDACO *pdaco_app,
                  int optimal_amount,
                  int ps_code,
                  int npv,
                  double pb,
                  const AnsiString &note,
                  const char *version,
                  bool normal,
                  TADOHandler *handler)
{
 Variant hostVars[15];
 TADODataSet *ds;
 int result_output;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.

 AnsiString sql_stmt;
 AnsiString result_string;

 try {
  if(normal){
    int result_output;
    if(npv <=0) {
      result_output = 2;
      result_string = "模組建議婉拒：經濟價值過低。";
    }
    else {
       if ((pdaco_app->getRequestAmount() >= 500000) && (optimal_amount >= 500000)){
         result_output = 1;
         result_string = "請核實收入確認後，模組建議核准。";
       }
       else {
         result_output = 1;
         result_string = "模組建議核准。";
       }
     }
     hostVars[0] = idno;
     hostVars[1] = input_time;
     hostVars[2] = optimal_amount;
     hostVars[3] = pb;
     hostVars[4] = npv;
     hostVars[5] = result_string;
     hostVars[6] = result_output;
     hostVars[7] = pdaco_app->getUnsecuredBalance();
     hostVars[8] = pdaco_app->getFscCap();
     hostVars[9] = note;
     hostVars[10] = pdaco_app->getDoubleCardBalance();
     hostVars[11] = version;
     hostVars[12] = ps_code;
     handler->ExecSQLCmd(SQLCommands[Store_Result], hostVars, 12);
  }
  else {
    int result_output;
    switch (ps_code){
      case 101:
      case 102:
      case 103:
      case 105:
      case 107:
        result_output = 2;
        result_string = "模組建議婉拒，壞帳機率過高。";
        break;
      case 104:
      case 108:
      case 116:
        result_output = 2;
        result_string = "模組建議婉拒，負債過高。";
        break;
      case 119:
      case 120:
      case 121:
        result_output = 2;
        result_string = "模組建議婉拒，信用有瑕疵。";
        break;
      case 111:
      case 114:
        result_output = 2;
        result_string = "模組建議婉拒，查詢次數過高。";
        break;
      case 117:
        result_output = 2;
        result_string = "模組建議婉拒，負債過高。";
        break;
      case 112:
      case 113:
      case 115:
        result_output = 2;
        result_string = "模組建議婉拒，信用有瑕疵。";
        break;
      case 201:
        result_output = 3;
        result_string = "模組建議人工審核，JCIC資料不足。";
        break;
    }
     hostVars[0] = idno;
     hostVars[1] = input_time;
     hostVars[2] = optimal_amount;
     hostVars[3] = pb;
     hostVars[4] = npv;
     hostVars[5] = result_string;
     hostVars[6] = result_output;
     hostVars[7] = pdaco_app->getUnsecuredBalance();
     hostVars[8] = pdaco_app->getFscCap();
     hostVars[9] = note;
     hostVars[10] = pdaco_app->getDoubleCardBalance();
     hostVars[11] = version;
     hostVars[12] = ps_code;
     handler->ExecSQLCmd(SQLCommands[Store_Result], hostVars, 12);
  }
 } catch (Exception &E) {
     throw;
 }

}
//---------------------------------------------------------------------------
void write_npv(char *msn, char *input_time, double npv,
                        TADOHandler *handler)
{
 String sqlstmt;

 sqlstmt  = "INSERT INTO NPV_OUT (MSN, LOAN_AMOUNT, NPV) VALUES (" ;
 sqlstmt += "'" + static_cast<String>(msn) + "', " + static_cast<String>(input_time) + ", " + FloatToStr(npv) + ")";
 try {
    handler->ExecSQLCmd(sqlstmt.c_str());
 } catch (Exception &E) {
    throw;
 }
}
//---------------------------------------------------------------------------
/*
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


