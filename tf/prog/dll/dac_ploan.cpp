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

int prescreen(char *app_sn, char *app_data_date, char *jcic_data_date,
              char *ole_db, char *error_msg)
{
 TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];

 try {
    dbhandle = new TADOHandler();
    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan(app_sn, app_data_date, dbhandle);
    ptrLoan->app_info_validate(app_sn, app_data_date, dbhandle);
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    /*Write_Prescreen_Result*/
    hostVars[0] = app_sn;
    hostVars[1] = app_data_date;
    hostVars[2] = jcic_data_date;
    hostVars[3] = ptrLoan->get_product_type();
    hostVars[4] = ptrLoan->get_code();
    hostVars[5] = ptrLoan->error();
    dbhandle->ExecSQLCmd(SQLCommands[Write_Prescreen_Result], hostVars, 5);
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);

 } catch (Loan::DataEx &DE){
     strcpy (error_msg, DE.message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 } catch (Loan::RiskEx &RE){
     strcpy (error_msg, RE.message.c_str());
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
     dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 } catch (Exception &E) {
     strcpy (error_msg, E.Message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 }
 strcpy (error_msg, "");      // return empty string if stop normally.
 delete ptrLoan;
 dbhandle->CloseDatabase();
 delete dbhandle;
 return (0);
}
//---------------------------------------------------------------------------
int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, int tsn, char *ole_db, char *error_msg)
{
 TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];
 char  sqlCommand[256];
 String Message;

 try {
    dbhandle = new TADOHandler();
    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan(app_sn, app_data_time, dbhandle);
    ptrLoan->app_info_validate(app_sn, app_data_time, dbhandle);
    ptrLoan->loan_validate(app_sn, tsn, dbhandle);
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    ptrLoan->calculate_pd(ts_data_date, dbhandle);
//    ptrLoan->get_pd(idn, dbhandle);
//    ptrLoan->calculate_npv();
//    ptrLoan->postFilter();
 } catch (Loan::DataEx &DE){
     strcpy (error_msg, DE.message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 } catch (Loan::RiskEx &RE){
     strcpy (error_msg, RE.message.c_str());

#ifdef _WRFLOW
     dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
     dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-RE.pb);
 } catch (Exception &E) {
     strcpy (error_msg, E.Message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 }
 strcpy (error_msg, Message.c_str());
 delete ptrLoan;
 dbhandle->CloseDatabase();
 delete dbhandle;
 return(0);
}

//---------------------------------------------------------------------------
int optimal_cal_conn(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, int tsn, char *ole_db, char *error_msg,
                TADOHandler *dbhandle)
{
// TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];
 char  sqlCommand[256];
 String Message;

 try {
//    dbhandle = new TADOHandler();
//    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan(app_sn, app_data_time, dbhandle);
//    ptrLoan->app_info_validate(app_sn, app_data_time, dbhandle);
    ptrLoan->loan_validate(app_sn, tsn, dbhandle);
    dbhandle->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    ptrLoan->prescreen(jcic_data_date, dbhandle);
    ptrLoan->calculate_pd(ts_data_date, dbhandle);
//    ptrLoan->get_pd(idn, dbhandle);
//    ptrLoan->calculate_npv();
//    ptrLoan->postFilter();
 } catch (Loan::DataEx &DE){
     strcpy (error_msg, DE.message.c_str());
     delete ptrLoan;
//     dbhandle->CloseDatabase();
//     delete dbhandle;
     return (-1);
 } catch (Loan::RiskEx &RE){
     strcpy (error_msg, RE.message.c_str());

#ifdef _WRFLOW
     dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
     dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
     delete ptrLoan;
//     dbhandle->CloseDatabase();
//     delete dbhandle;
     return (-RE.pb);
 } catch (Exception &E) {
     strcpy (error_msg, E.Message.c_str());
     delete ptrLoan;
//     dbhandle->CloseDatabase();
//     delete dbhandle;
     return (-1);
 }
 strcpy (error_msg, Message.c_str());
 delete ptrLoan;
// dbhandle->CloseDatabase();
// delete dbhandle;
 return(0);
}

//---------------------------------------------------------------------------
int designated_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, int tsn, char *ole_db, char *error_msg)
{
 return(0);

}
//---------------------------------------------------------------------------
int conversion_cal(char *app_sn, int tsn, char *ole_db, char *error_msg)
{
 return(0);

}

//---------------------------------------------------------------------------
/*
int dac_ploan_ev(char *case_sn, char *idn, int dac_sn, char *ole_db, char *error_msg)
{
 TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];
 char  sqlCommand[256];
 String Message;

 try {
    dbhandle = new TADOHandler();
    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan(case_sn, idn, dac_sn, dbhandle);
    ptrLoan->validate();
    ptrLoan->Init_Maintenance(dbhandle);
    ptrLoan->calculate_pd(dbhandle);
//    ptrLoan->get_pd(idn, dbhandle);
    ptrLoan->calculate_npv();
    ptrLoan->postFilter();

    // Assign NPV, PD, Risk score
    sprintf (sqlCommand, SQLCommands[Write_PLoan_Result],
                         ptrLoan->get_npv(), ptrLoan->get_pd(), ptrLoan->get_rscore(), 0.0);
    hostVars[0] = case_sn;
    hostVars[1] = idn;
    hostVars[2] = dac_sn;
    hostVars[3] = ptrLoan->get_application_date();
    hostVars[4] = Message;                  // return message
    dbhandle->ExecSQLCmd(sqlCommand, hostVars, 4);
    /-* Drop all temporary tables before closing a connection to avoid connection creep problem.
       Without droping temp tables will not release system resource after connection is closed.
    *-/
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);

 } catch (Loan::DataEx &DE){
     strcpy (error_msg, DE.message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 } catch (Loan::RiskEx &RE){
     strcpy (error_msg, RE.message.c_str());
     if (DEBUG)
        dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
     /-* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     *-/
     dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
     int exCode = static_cast <int> (RE.pb);
     switch (exCode) {
        case 101:
        case 102:
             hostVars[0] = case_sn;
             hostVars[1] = idn;
             hostVars[2] = dac_sn;
             hostVars[3] = ptrLoan->get_application_date();
             hostVars[4] = RE.message;            // return message
             hostVars[5] = RE.pb;                 // checksum1 (rscore)
                                                  // NPV & PD is assigned to NULL
             dbhandle->ExecSQLCmd(SQLCommands[Write_PreFilter_Result], hostVars, 5);
             break;
        case 103:
        case 104:
        case 105:
        case 106:
        case 107:
             sprintf (sqlCommand, SQLCommands[Write_PLoan_Result],
                      ptrLoan->get_npv(), ptrLoan->get_pd(), ptrLoan->get_rscore(), RE.pb);
             hostVars[0] = case_sn;
             hostVars[1] = idn;
             hostVars[2] = dac_sn;
             hostVars[3] = ptrLoan->get_application_date();
             hostVars[4] = RE.message;                  // return message
             dbhandle->ExecSQLCmd(sqlCommand, hostVars, 4);
             break;
     }
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-RE.pb);
 } catch (Exception &E) {
     strcpy (error_msg, E.Message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 }
 strcpy (error_msg, Message.c_str());
 delete ptrLoan;
 dbhandle->CloseDatabase();
 delete dbhandle;
 return (0);
}
*/
/*
int dac_ploan_ev_conn(char *case_sn, char *idn, int dac_sn, char *ole_db,
                 char *error_msg, TADOHandler *dbhandle)
{
// TADOHandler *dbhandle;
 Loan *ptrLoan;
 Variant hostVars[10];
 char  sqlCommand[256];
 String Message;

 try {
//    dbhandle = new TADOHandler();
//    dbhandle->OpenDatabase(ole_db);
    ptrLoan = new Loan(case_sn, idn, dac_sn, dbhandle);
    ptrLoan->validate();
    ptrLoan->Init_Maintenance(dbhandle);
    ptrLoan->calculate_pd(dbhandle);
//    ptrLoan->get_pd(idn, dbhandle);
    ptrLoan->calculate_npv();
    ptrLoan->postFilter();
    // Assign NPV, PD, Risk score
    sprintf (sqlCommand, SQLCommands[Write_PLoan_Result],
                         ptrLoan->get_npv(), ptrLoan->get_pd(), ptrLoan->get_rscore(), 0.0);
    hostVars[0] = case_sn;
    hostVars[1] = idn;
    hostVars[2] = dac_sn;
    hostVars[3] = ptrLoan->get_application_date();
    hostVars[4] = Message;                  // return message
    dbhandle->ExecSQLCmd(sqlCommand, hostVars, 4);
    /-* Drop all temporary tables before closing a connection to avoid connection creep problem.
       Without droping temp tables will not release system resource after connection is closed.
    *-/
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);

 } catch (Loan::DataEx &DE){
     strcpy (error_msg, DE.message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 } catch (Loan::RiskEx &RE){
     strcpy (error_msg, RE.message.c_str());
     if (DEBUG)
        dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
     /-* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     *-/
     dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
     int exCode = static_cast <int> (RE.pb);
     switch (exCode) {
        case 101:
        case 102:
             hostVars[0] = case_sn;
             hostVars[1] = idn;
             hostVars[2] = dac_sn;
             hostVars[3] = ptrLoan->get_application_date();
             hostVars[4] = RE.message;            // return message
             hostVars[5] = RE.pb;                 // checksum1 (rscore)
                                                  // NPV & PD is assigned to NULL
             dbhandle->ExecSQLCmd(SQLCommands[Write_PreFilter_Result], hostVars, 5);
             break;
        case 103:
        case 104:
        case 105:
        case 106:
        case 107:
             sprintf (sqlCommand, SQLCommands[Write_PLoan_Result],
                      ptrLoan->get_npv(), ptrLoan->get_pd(), ptrLoan->get_rscore(), RE.pb);
             hostVars[0] = case_sn;
             hostVars[1] = idn;
             hostVars[2] = dac_sn;
             hostVars[3] = ptrLoan->get_application_date();
             hostVars[4] = RE.message;                  // return message
             dbhandle->ExecSQLCmd(sqlCommand, hostVars, 4);
             break;
     }
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-RE.pb);
 } catch (Exception &E) {
     strcpy (error_msg, E.Message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 }
 strcpy (error_msg, Message.c_str());
 delete ptrLoan;
 dbhandle->CloseDatabase();
 delete dbhandle;
 return (0);
}
*/
