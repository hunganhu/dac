//---------------------------------------------------------------------------
#pragma hdrstop
#include <stdio.h>
#include "dac_ploan.h"
#include "loanApp.h"
#include "ploanSQL.h"
#ifndef _WRFLOW    //In Project->Options->directories/conditionals, append ";_WRFLOW" to conditinals
 #define DEBUG 0
#else
 #define DEBUG 1
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

int prescreen(char *app_sn, char *jcic_data_date, char *app_data_time,
              char *ole_db, char *error_msg)
{
 TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];
 int errCode = 0;

 try {
    strcpy (error_msg, "");      // return empty string if stop normally.
    dbhandle = new TADOHandler();
    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan(app_sn, app_data_time);
    ptrLoan->app_info_validate(app_sn, app_data_time, dbhandle);
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    errCode = ptrLoan->get_code();
    /*Write_Prescreen_Result*/
    hostVars[0] = app_sn;
    hostVars[1] = app_data_time;
    hostVars[2] = jcic_data_date;
    hostVars[3] = ptrLoan->get_product_type();
    hostVars[4] = errCode;
    hostVars[5] = ptrLoan->error();
    dbhandle->ExecSQLCmd(SQLCommands[Write_Prescreen_Result], hostVars, 5);
#ifdef _WRFLOW
    dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);

 } catch (Loan::DataEx &DE){
     strcpy (error_msg, DE.message.c_str());
     errCode = DE.code;
 } catch (Exception &E) {
     strcpy (error_msg, E.Message.c_str());
     errCode = -1;
 }
 delete ptrLoan;
 dbhandle->CloseDatabase();
 delete dbhandle;
 return (errCode);
}
//---------------------------------------------------------------------------
int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_msg)
{
 return(0);
}

//---------------------------------------------------------------------------
int optimal_cal_conn(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_msg,
                TADOHandler *dbhandle)
{
// TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];
 char  sqlCommand[256];
 String Message;
 int errCode = 0;

 try {
//    dbhandle = new TADOHandler();
//    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan (app_sn, app_data_time, ts_data_date, jcic_data_date, tsn);
    ptrLoan->app_info_validate(app_sn, app_data_time, dbhandle);
    ptrLoan->loan_validate(app_sn, tsn, dbhandle);
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    errCode = ptrLoan->get_code();
    if (errCode == 0) {
       ptrLoan->get_test_PB(app_sn, dbhandle);
       ptrLoan->calculate_npv(ptrLoan->get_principal());
       hostVars[0] = ptrLoan->get_pd();
       hostVars[1] = ptrLoan->get_npv();
       hostVars[2] = app_sn;
       dbhandle->ExecSQLCmd(SQLCommands[Write_Ploan_NPV], hostVars, 2);
    }
    else
       strcpy (error_msg, ptrLoan->error().c_str());

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
       strcpy (error_msg, ptrLoan->error().c_str());
*/
#ifdef _WRFLOW
     dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
 } catch (Loan::DataEx &DE){
     strcpy (error_msg, DE.message.c_str());
     errCode = DE.code;
 } catch (Exception &E) {
     strcpy (error_msg, E.Message.c_str());
     errCode = -1;
 }
 delete ptrLoan;
// dbhandle->CloseDatabase();
// delete dbhandle;
 return(errCode);
}

//---------------------------------------------------------------------------
int designated_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_msg)
{
 return(0);

}
//---------------------------------------------------------------------------
int decision_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_msg)
{
 return(0);

}


