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
int dac_ploan_ev(char *case_sn, char *idn, int dac_sn, char *ole_db, char *return_msg)
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

/*******  First check NPV, then check principal is in valid range.
    if (ptrLoan->get_npv() < -1000)
       Message = "�ڵ� [NPV�p��NT$-1,000]";
    else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
       Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
    else {
       if (ptrLoan->get_principal() >= 2000000)
          Message = "�H�u�f�� [�ӶU���B�j��NT$200�U]";
       else {
          if (ptrLoan->get_segment() == "01" || ptrLoan->get_segment() == "02"){
             if (ptrLoan->get_principal() >= 1100000)
                Message = "�H�u�f�� [�u��/A1�ūȼh�F�ӶU���B����NT$110�U�MNT$200�U]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
          else if (ptrLoan->get_segment() == "03" || ptrLoan->get_segment() == "04") {
             if (ptrLoan->get_principal() >= 900000)
                Message = "�H�u�f�� [A2/A3�ūȼh�F�ӶU���B����NT$90�U�MNT$200�U]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
          else if (ptrLoan->get_segment() == "05") {
             if (ptrLoan->get_principal() >= 700000)
                Message = "�H�u�f�� [A4�ūȼh�F�ӶU���B����NT$70�U�MNT$200�U]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
          else {
             if (ptrLoan->get_principal() >= 450000)
                Message = "�H�u�f�� [B�ťH�U�ȼh�F�ӶU���B����NT$45�U�MNT$200�U]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
    }
*/
/*******  First check principal is in valid range, then check NPV. */
    if (ptrLoan->get_principal() >= 2000000)
       Message = "�H�u�f�� [�ӶU���B�j��NT$200�U]";
    else {
       if (ptrLoan->get_segment() == "01" || ptrLoan->get_segment() == "02"){
          if (ptrLoan->get_principal() >= 1100000)
             Message = "�H�u�f�� [�u��/A1�ūȼh�F�ӶU���B�j��NT$110�U]";
          else {
             if (ptrLoan->get_npv() < -1000)
                Message = "�ڵ� [NPV�p��NT$-1,000]";
             else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
                Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
       else if (ptrLoan->get_segment() == "03" || ptrLoan->get_segment() == "04") {
          if (ptrLoan->get_principal() >= 900000)
             Message = "�H�u�f�� [A2/A3�ūȼh�F�ӶU���B�j��NT$90�U]";
          else {
             if (ptrLoan->get_npv() < -1000)
                Message = "�ڵ� [NPV�p��NT$-1,000]";
             else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
                Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
       else if (ptrLoan->get_segment() == "05") {
          if (ptrLoan->get_principal() >= 700000)
             Message = "�H�u�f�� [A4�ūȼh�F�ӶU���B�j��NT$70�U]";
          else {
             if (ptrLoan->get_npv() < -1000)
                Message = "�ڵ� [NPV�p��NT$-1,000]";
             else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
                Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
       else {
          if (ptrLoan->get_principal() >= 450000)
             Message = "�H�u�f�� [B�ťH�U�ȼh�F�ӶU���B�j��NT$45�U]";
          else {
             if (ptrLoan->get_npv() < -1000)
                Message = "�ڵ� [NPV�p��NT$-1,000]";
             else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
                Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
    }
    // Assign NPV, PD, Risk score
    sprintf (sqlCommand, SQLCommands[Write_PLoan_Result],
                         ptrLoan->get_npv(), ptrLoan->get_pd(), ptrLoan->get_rscore(), 0.0);
    hostVars[0] = case_sn;
    hostVars[1] = idn;
    hostVars[2] = dac_sn;
    hostVars[3] = ptrLoan->get_application_date();
    hostVars[4] = Message;                  // return message
    dbhandle->ExecSQLCmd(sqlCommand, hostVars, 4);
    /* Drop all temporary tables before closing a connection to avoid connection creep problem.
       Without droping temp tables will not release system resource after connection is closed.
    */
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);

 } catch (Loan::DataEx &DE){
     strcpy (return_msg, DE.message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 } catch (Loan::RiskEx &RE){
     strcpy (return_msg, RE.message.c_str());
     if (DEBUG)
        dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
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
     strcpy (return_msg, E.Message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 }
 strcpy (return_msg, Message.c_str());
 delete ptrLoan;
 dbhandle->CloseDatabase();
 delete dbhandle;
 return (0);
}

int dac_ploan_ev_conn(char *case_sn, char *idn, int dac_sn, char *ole_db,
                 char *return_msg, TADOHandler *dbhandle)
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
/*******  First check NPV, then check principal is in valid range.
    if (ptrLoan->get_npv() < -1000)
       Message = "�ڵ� [NPV�p��NT$-1,000]";
    else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
       Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
    else {
       if (ptrLoan->get_principal() >= 2000000)
          Message = "�H�u�f�� [�ӶU���B�j��NT$200�U]";
       else {
          if (ptrLoan->get_segment() == "01" || ptrLoan->get_segment() == "02"){
             if (ptrLoan->get_principal() >= 1100000)
                Message = "�H�u�f�� [�u��/A1�ūȼh�F�ӶU���B����NT$110�U�MNT$200�U]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
          else if (ptrLoan->get_segment() == "03" || ptrLoan->get_segment() == "04") {
             if (ptrLoan->get_principal() >= 900000)
                Message = "�H�u�f�� [A2/A3�ūȼh�F�ӶU���B����NT$90�U�MNT$200�U]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
          else if (ptrLoan->get_segment() == "05") {
             if (ptrLoan->get_principal() >= 700000)
                Message = "�H�u�f�� [A4�ūȼh�F�ӶU���B����NT$70�U�MNT$200�U]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
          else {
             if (ptrLoan->get_principal() >= 450000)
                Message = "�H�u�f�� [B�ťH�U�ȼh�F�ӶU���B����NT$45�U�MNT$200�U]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
    }
*/
/*******  First check principal is in valid range, then check NPV. */
    if (ptrLoan->get_principal() >= 2000000)
       Message = "�H�u�f�� [�ӶU���B�j��NT$200�U]";
    else {
       if (ptrLoan->get_segment() == "01" || ptrLoan->get_segment() == "02"){
          if (ptrLoan->get_principal() >= 1100000)
             Message = "�H�u�f�� [�u��/A1�ūȼh�F�ӶU���B�j��NT$110�U]";
          else {
             if (ptrLoan->get_npv() < -1000)
                Message = "�ڵ� [NPV�p��NT$-1,000]";
             else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
                Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
       else if (ptrLoan->get_segment() == "03" || ptrLoan->get_segment() == "04") {
          if (ptrLoan->get_principal() >= 900000)
             Message = "�H�u�f�� [A2/A3�ūȼh�F�ӶU���B�j��NT$90�U]";
          else {
             if (ptrLoan->get_npv() < -1000)
                Message = "�ڵ� [NPV�p��NT$-1,000]";
             else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
                Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
       else if (ptrLoan->get_segment() == "05") {
          if (ptrLoan->get_principal() >= 700000)
             Message = "�H�u�f�� [A4�ūȼh�F�ӶU���B�j��NT$70�U]";
          else {
             if (ptrLoan->get_npv() < -1000)
                Message = "�ڵ� [NPV�p��NT$-1,000]";
             else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
                Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
       else {
          if (ptrLoan->get_principal() >= 450000)
             Message = "�H�u�f�� [B�ťH�U�ȼh�F�ӶU���B�j��NT$45�U]";
          else {
             if (ptrLoan->get_npv() < -1000)
                Message = "�ڵ� [NPV�p��NT$-1,000]";
             else if (ptrLoan->get_npv() >= -1000 && ptrLoan->get_npv() < 1000)
                Message = "�H�u�f�� [NPV����NT$-1,000�MNT$1,000]";
             else
                Message = "�֭� [NPV�j��NT$1,000]";
          }
       }
    }

    // Assign NPV, PD, Risk score
    sprintf (sqlCommand, SQLCommands[Write_PLoan_Result],
                         ptrLoan->get_npv(), ptrLoan->get_pd(), ptrLoan->get_rscore(), 0.0);
    hostVars[0] = case_sn;
    hostVars[1] = idn;
    hostVars[2] = dac_sn;
    hostVars[3] = ptrLoan->get_application_date();
    hostVars[4] = Message;                  // return message
    dbhandle->ExecSQLCmd(sqlCommand, hostVars, 4);
    /* Drop all temporary tables before closing a connection to avoid connection creep problem.
       Without droping temp tables will not release system resource after connection is closed.
    */
    dbhandle->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);

 } catch (Loan::DataEx &DE){
     strcpy (return_msg, DE.message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 } catch (Loan::RiskEx &RE){
     strcpy (return_msg, RE.message.c_str());
     if (DEBUG)
        dbhandle->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
     /* Drop all temporary tables before closing a connection to avoid connection creep problem.
        Without droping temp tables will not release system resource after connection is closed.
     */
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
     strcpy (return_msg, E.Message.c_str());
     delete ptrLoan;
     dbhandle->CloseDatabase();
     delete dbhandle;
     return (-1);
 }
 strcpy (return_msg, Message.c_str());
 delete ptrLoan;
 dbhandle->CloseDatabase();
 delete dbhandle;
 return (0);
}
