//---------------------------------------------------------------------------
#pragma hdrstop
#include <stdio.h>
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

int prescreen(char *app_sn, char *jcic_data_date, char *app_data_time,
              char *ole_db, char *error_message)
{
 TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];
 int errCode = 0;

 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
 try {
    strcpy (error_message, "");      // return empty string if stop normally.
    dbhandle = new TADOHandler();
    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan(app_sn, app_data_time, jcic_data_date);
    ptrLoan->app_info_validate(app_sn, app_data_time, dbhandle);

    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);

#ifdef _WRFLOW
    dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
    dbhandle->CloseDatabase();

    if (Days_between(jcic_data_date, app_data_time) > 30) {
       errCode = 2;
       strcpy (error_message, JCIC_EXPIRE);
    }
 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
 delete ptrLoan;
 delete dbhandle;
 return (errCode);
}
//---------------------------------------------------------------------------
int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message)
{
 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
 return(0);
}

//---------------------------------------------------------------------------
int optimal_cal_conn(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message,
                TADOHandler *dbhandle)
{
// TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];
 char  sqlCommand[256];
 String Message;
 int errCode = 0;

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
    if (ptrLoan->get_code() != 0){
       // write data error to ouput table.
       return 0;
    }

    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    errCode = ptrLoan->get_code();
    if (errCode == 0) {
       ptrLoan->get_test_PB(app_sn, dbhandle);
       ptrLoan->calculate_npv(ptrLoan->get_principal(), ptrLoan->get_pd());
       hostVars[0] = ptrLoan->get_pd();
       hostVars[1] = ptrLoan->get_npv();
       hostVars[2] = app_sn;
       dbhandle->ExecSQLCmd(SQLCommands[Write_Ploan_NPV], hostVars, 2);
    }
    else
       strcpy (error_message, ptrLoan->error().c_str());

/*
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    errCode = ptrLoan->get_code();
    if (errCode == 0) {
       ptrLoan->calculate_rscore(dbhandle);
       ptrLoan->calculate_pd(dbhandle);
//       ptrLoan->calculate_optimal_line(ptrLoan->get_principal(), ptrLoan->get_principal(), 1,
//                                    dbhandle);
    ptrLoan->calculate_npv(get_principal());
    }
    else
       strcpy (error_message, ptrLoan->error().c_str());
*/
#ifdef _WRFLOW
     dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
// dbhandle->CloseDatabase();
 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
 delete ptrLoan;
// delete dbhandle;
    if (Days_between(jcic_data_date, app_data_time) > 30) {
       errCode = 2;
       strcpy (error_message, JCIC_EXPIRE);
    }
 return(errCode);
}

//---------------------------------------------------------------------------
int specific_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
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
    if (ptrLoan->get_code() != 0){
       // write data error to approval_cal.
       return 0;
    }
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
//    ptrLoan->prescreen(jcic_data_date, dbhandle);
//    errCode = ptrLoan->get_code();
//    if (errCode == 0) {
//       ptrLoan->calculate_rscore(dbhandle);
    if (ptrLoan->get_test_PB(app_sn, dbhandle) > 0) {
       pb = ptrLoan->calculate_pd(ptrLoan->get_principal(), dbhandle);
       ptrLoan->calculate_npv(ptrLoan->get_principal(), pb);
       // write_specific result to approval_cal
       hostVars[0] = app_sn;
       hostVars[1] = tsn;
       hostVars[2] = ts_data_date;
       hostVars[3] = jcic_data_date;
       hostVars[4] = app_data_time;
       hostVars[5] = ptrLoan->get_product_type();
       hostVars[6] = ptrLoan->get_principal();
       hostVars[7] = pb;
       hostVars[8] = ptrLoan->get_npv();
       hostVars[9] = reason_code;
       hostVars[10] = Message;

       dbhandle->ExecSQLCmd(SQLCommands[Write_Specific_Result], hostVars, 10);

    }
    else {
       // write prescreen result to approval_cal  
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
    if (Days_between(jcic_data_date, app_data_time) > 30) {
       errCode = 2;
       strcpy (error_message, JCIC_EXPIRE);
    }
 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
 delete ptrLoan;
// delete dbhandle;
 return(errCode);

}
//---------------------------------------------------------------------------
int decision_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, char *ole_db,
                 char *audit_userno1, char *change_code, char *major_deviation,
                 char *minor_deviation, char *decline_code, char *manual_code,
                 char *error_message)
{
 int errCode = 0;
 if (check_expiration(EXPIRATION_DATE) == -1) {
    strcpy (error_message, EXPIRATION_MSG);
    return(-1);
 }
    if (Days_between(jcic_data_date, app_data_time) > 30) {
       errCode = 2;
       strcpy (error_message, JCIC_EXPIRE);
    }
 return(errCode);

}


