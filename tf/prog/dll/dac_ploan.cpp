//---------------------------------------------------------------------------
#pragma hdrstop
#include <stdio.h>
#include <Math.hpp>
#include "dac_ploan.h"
#include "loanApp.h"
#include "ploanSQL.h"
#include "functions.h"
#ifndef _WRFLOW    //In Project->Options->directories/conditionals, append ";_WRFLOW" to conditinals
 #define DEBUG 0
#else
 #define DEBUG 1
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)
/*
int prescreen(char *app_sn, char *jcic_data_date, char *app_data_time,
              char *ole_db, char *error_message)
*/
int prescreen(char *app_sn, char *jcic_data_date, char *app_data_time,
              char *ole_db, char *error_message, TADOHandler *dbhandle)
{
// TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];
 int errCode = 0;

 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
 try {
    strcpy (error_message, "");      // return empty string if stop normally.
//    dbhandle = new TADOHandler();
//    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan(app_sn, app_data_time, jcic_data_date);
    ptrLoan->app_info_validate(app_sn, app_data_time, dbhandle);

    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);

#ifdef _WRFLOW
    dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
//    dbhandle->CloseDatabase();

 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
 delete ptrLoan;
 delete dbhandle;
 return (errCode);
}

//---------------------------------------------------------------------------
/*
int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message)
*/
int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message,
                TADOHandler *dbhandle)
{
// TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[15];
 char  sqlCommand[256];
 String Message = "";
 int errCode = 0;
 int reason_code = 0;
 double pb;
 double a2_npv[][3] = {{150000.0, 0.0, 0.0},
                       {200000.0, 0.0, 0.0},
                       {250000.0, 0.0, 0.0},
                       {300000.0, 0.0, 0.0},
                       {350000.0, 0.0, 0.0},
                       {400000.0, 0.0, 0.0}};  // initial values for GX A2
 double b2_npv[][3] = {{150000.0, 0.0, 0.0},
                       {200000.0, 0.0, 0.0}};  // initial values for GX B2
 double others_npv[1][3] = {0.0, 0.0, 0.0};    // initial values for GX B1, C
                                               // and KHJ A2, B1, B2, C
 int optimal, max_line;
 double optimal_line, optimal_pb, optimal_npv;


 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
 try {
//    dbhandle = new TADOHandler();
//    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan (app_sn, app_data_time, ts_data_date, jcic_data_date, tsn);
    ptrLoan->app_info_validate(app_sn, app_data_time, dbhandle);
    ptrLoan->loan_validate(app_sn, tsn, dbhandle);

    // write common fields
    hostVars[0] = app_sn;
    hostVars[1] = tsn;
    hostVars[2] = ts_data_date;
    hostVars[3] = jcic_data_date;
    hostVars[4] = app_data_time;
    hostVars[5] = ptrLoan->get_product_type();

    if (ptrLoan->get_code() != 0){
       // write data error to approval_cal.
       hostVars[6] = ptrLoan->get_principal();
       hostVars[7] = ptrLoan->get_code();
       hostVars[8] = ptrLoan->error();
       dbhandle->ExecSQLCmd(SQLCommands[Write_Specific_Result_Data_Error], hostVars, 8);
//    dbhandle->CloseDatabase();
       delete ptrLoan;
       return (0);
    }
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    errCode = ptrLoan->get_code();
    if (errCode == 0) {
       ptrLoan->calculate_rscore(dbhandle);
//    if (ptrLoan->get_test_PB(app_sn, dbhandle) > 0) {
        // check product type and card
        if (ptrLoan->get_product_type() == 1) { // product GX
            switch (ptrLoan->get_card()) {
               case 1:max_line = ptrLoan->calculate_optimal_line(6, a2_npv, dbhandle);
                      optimal_line = a2_npv[max_line][0];
                      optimal_pb = a2_npv[max_line][1];
                      optimal_npv = a2_npv[max_line][2];
                      optimal = 1;
                      break;
               case 3:max_line = ptrLoan->calculate_optimal_line(2, b2_npv, dbhandle);
                      optimal_line = b2_npv[max_line][0];
                      optimal_pb = b2_npv[max_line][1];
                      optimal_npv = b2_npv[max_line][2];
                      optimal = 1;
                      break;
               case 2:
               case 4:others_npv[0][0]  = ptrLoan->get_principal(); // set line to loan amount
                      max_line = ptrLoan->calculate_optimal_line(1, others_npv, dbhandle);
                      optimal_line = others_npv[max_line][0];
                      optimal_pb = others_npv[max_line][1];
                      optimal_npv = others_npv[max_line][2];
                      optimal = 0;
                      break;
            }
        } else if (ptrLoan->get_product_type() == 2) {   // product KHJ
             others_npv[0][0]  = ptrLoan->get_principal(); // set line to loan amount
             max_line = ptrLoan->calculate_optimal_line(1, others_npv, dbhandle);
             optimal_line = others_npv[max_line][0];
             optimal_pb = others_npv[max_line][1];
             optimal_npv = others_npv[max_line][2];
             optimal = 0;
        }
       // write_optimal result to approval_cal
       hostVars[9] = optimal_line;
       hostVars[10] = RoundTo(optimal_pb, -4);
       hostVars[11] = optimal_npv;
       if (optimal_npv >= 0){
          reason_code = 1;
          Message = TF_Messages[Normal_1];
       }
       else {
          reason_code = 0;
          Message = TF_Messages[Normal_0];
       }
       if (Days_between(jcic_data_date, app_data_time) > 30) {
          errCode = 2;
          reason_code = 201;
          Message = TF_Messages[Warning_201];
          strcpy (error_message, TF_Messages[Warning_201]);
       } else {
          if (optimal == 0) {
             Message = TF_Messages[Optimal_error_501];
             reason_code = 501;
          }
       }
       hostVars[6] = reason_code;
       hostVars[7] = Message;
       hostVars[8] = ptrLoan->get_external_monthly_payment();
       if (optimal) {
          dbhandle->ExecSQLCmd(SQLCommands[Write_Optimal_Result], hostVars, 11);
       } else {
          dbhandle->ExecSQLCmd(SQLCommands[Write_No_Optimal_Result], hostVars, 8);
       }
    }
    else {
       // write prescreen result to PRESECREEN
       errCode = 1;
       strcpy (error_message, ptrLoan->error().c_str());
    }
#ifdef _WRFLOW
     dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
//    dbhandle->CloseDatabase();
 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
 delete ptrLoan;
// delete dbhandle;
 return(errCode);
}

//---------------------------------------------------------------------------
/*
int specific_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message)
*/
int specific_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message,
                TADOHandler *dbhandle)
{
// TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[20];
 char  sqlCommand[256];
 String Message = "";
 int errCode = 0;
 int reason_code = 0;
 double pb;

 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
 try {
//    dbhandle = new TADOHandler();
//    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan (app_sn, app_data_time, ts_data_date, jcic_data_date, tsn);
    ptrLoan->app_info_validate(app_sn, app_data_time, dbhandle);
    ptrLoan->loan_validate(app_sn, tsn, dbhandle);
    hostVars[0] = app_sn;
    hostVars[1] = tsn;
    hostVars[2] = ts_data_date;
    hostVars[3] = jcic_data_date;
    hostVars[4] = app_data_time;
    hostVars[5] = ptrLoan->get_product_type();

    if (ptrLoan->get_code() != 0){
       // write data error to approval_cal.
       hostVars[6] = ptrLoan->get_principal();
       hostVars[7] = ptrLoan->get_code();
       hostVars[8] = ptrLoan->error();
       dbhandle->ExecSQLCmd(SQLCommands[Write_Specific_Result_Data_Error], hostVars, 8);
//    dbhandle->CloseDatabase();
       delete ptrLoan;
       return (0);
    }
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    errCode = ptrLoan->get_code();
    if (errCode == 0) {
       ptrLoan->calculate_rscore(dbhandle);
//    if (ptrLoan->get_test_PB(app_sn, dbhandle) > 0) {
       pb = ptrLoan->calculate_pd(ptrLoan->get_principal(), dbhandle);
       ptrLoan->calculate_npv(ptrLoan->get_principal(), pb);
       // write_specific result to approval_cal
       hostVars[6] = ptrLoan->get_principal();
       hostVars[7] = RoundTo(pb, -4);
       hostVars[8] = ptrLoan->get_npv();
       if (ptrLoan->get_npv() >= 0){
          reason_code = 1;
          Message = TF_Messages[Normal_1];
       }
       else {
          reason_code = 0;
          Message = TF_Messages[Normal_0];
       }
       if (Days_between(jcic_data_date, app_data_time) > 30) {
          errCode = 2;
          reason_code = 201;
          Message = TF_Messages[Warning_201];
          strcpy (error_message, TF_Messages[Warning_201]);
       } else {
          switch (ptrLoan->get_card()) {
             case 1:if (ptrLoan->get_principal() > A2_LIMIT) {
                       reason_code = 202;
                       Message = TF_Messages[Warning_202];
                    }
                    break;
             case 2:if (ptrLoan->get_principal() > B1_LIMIT) {
                       reason_code = 203;
                       Message = TF_Messages[Warning_203];
                    }
                    break;
             case 3:
             case 4:if (ptrLoan->get_principal() > B2_C_LIMIT) {
                       reason_code = 204;
                       Message = TF_Messages[Warning_204];
                    }
                    break;
          }
       }
       hostVars[9] = reason_code;
       hostVars[10] = Message;
       hostVars[11] = ptrLoan->get_external_monthly_payment();
       dbhandle->ExecSQLCmd(SQLCommands[Write_Specific_Result], hostVars, 11);
    }
    else {
       // write prescreen result to PRESECREEN
       errCode = 1;
       strcpy (error_message, ptrLoan->error().c_str());
    }
#ifdef _WRFLOW
     dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
//    dbhandle->CloseDatabase();
 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
 delete ptrLoan;
// delete dbhandle;
 return(errCode);

}
//---------------------------------------------------------------------------
/*
int decision_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, char *ole_db,
                 char *audit_userno1, char *change_code, char *major_deviation,
                 char *minor_deviation, char *decline_code, char *manual_code,
                 char *error_message)
*/
int decision_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, char *ole_db,
                 char *audit_userno1, char *change_code, char *major_deviation,
                 char *minor_deviation, char *decline_code, char *manual_code,
                 char *error_message, TADOHandler *dbhandle)
{
// TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[15];
 char  sqlCommand[256];
 String Message = "";
 int errCode = 0;
 int reason_code = 0;
 double pb;

 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
 try {
//    dbhandle = new TADOHandler();
//    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan (app_sn, app_data_time, ts_data_date, jcic_data_date, tsn);
    ptrLoan->app_info_validate(app_sn, app_data_time, dbhandle);
    ptrLoan->loan_validate(app_sn, tsn, dbhandle);
    hostVars[0] = app_sn;
    hostVars[1] = tsn;
    hostVars[2] = ts_data_date;
    hostVars[3] = jcic_data_date;
    hostVars[4] = app_data_time;
    hostVars[5] = ptrLoan->get_product_type();
    hostVars[6] = audit_userno1;
    hostVars[7] = change_code;
    hostVars[8] = major_deviation;
    hostVars[9] = minor_deviation;
    hostVars[10] = decline_code;
    hostVars[11] = manual_code;

    if (ptrLoan->get_code() != 0){
       // write data error to approval_cal.
       hostVars[12] = ptrLoan->get_principal();
       hostVars[13] = ptrLoan->get_code();
       hostVars[14] = ptrLoan->error();
       hostVars[15] = ExecutionTime();
       dbhandle->ExecSQLCmd(SQLCommands[Write_Decision_Result_Error], hostVars, 15);
//    dbhandle->CloseDatabase();
       delete ptrLoan;
       return (0);
    }
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    errCode = ptrLoan->get_code();
    if (errCode == 0) {
       ptrLoan->calculate_rscore(dbhandle);
       pb = ptrLoan->calculate_pd(ptrLoan->get_principal(), dbhandle);
       ptrLoan->calculate_npv(ptrLoan->get_principal(), pb);
       // write_decision result to decision_cal
       hostVars[12] = ptrLoan->get_principal();
       hostVars[17] = RoundTo(pb, -4);
       hostVars[18] = ptrLoan->get_npv();
       hostVars[13] = reason_code;
       hostVars[14] = Message;
       hostVars[15] = ExecutionTime();
       hostVars[16] = ptrLoan->get_external_monthly_payment();
       dbhandle->ExecSQLCmd(SQLCommands[Write_Decision_Result], hostVars, 18);
    }
    else {
       // write prescreen result to PRESECREEN
       errCode = 1;
       strcpy (error_message, ptrLoan->error().c_str());
    }
#ifdef _WRFLOW
     dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
//    dbhandle->CloseDatabase();
 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
 delete ptrLoan;
// delete dbhandle;
 return(errCode);

}


