//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdio.h>
#include <Math.hpp>
#include "dac_fm.h"
#include "functions.h"
#include "errors.h"
#include "pdaco.h"
#include "fmNPV.h"
#ifndef _WRFLOW    //In Project->Options->directories/conditionals, append ";_WRFLOW" to conditinals
 #define DEBUG 0
#else
 #define DEBUG 1
#endif
//---------------------------------------------------------------------------

//#pragma package(smart_init)
#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"

int FM_New(char *case_no, char *ole_db, char *error_message)
{
 TADOHandler *dbhandle;    // commemt if past from argument
 String Message = "";
 Variant hostVars[20];
 Loan *ptrLoan;
 PDACO *pdaco_app, *pdaco_cos, *pdaco_gua;
 int app_seg, cos_seg, gua_seg;
 int app_pscode, cos_pscode, gua_pscode;
 int now;
 int errCode = 0;

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
    }

    if (ptrLoan->exist_coapplicant()) {
       pdaco_cos = new PDACO(case_no, ptrLoan->Cosigner(), now);
       pdaco_cos->Prescreen_New(dbhandle);
       cos_seg =  pdaco_cos->Segment();
       cos_pscode = pdaco_cos->PS_code();
    } else {
       cos_seg = seg_N;
    }
    if (ptrLoan->exist_guarantor()) {
       pdaco_gua = new PDACO(case_no, ptrLoan->Guarantor(), now);
       pdaco_gua->Prescreen_New(dbhandle);
       gua_seg = pdaco_gua->Segment();
       gua_pscode = pdaco_gua->PS_code();
    } else {
       gua_seg = seg_N;
    }


    dbhandle->CloseDatabase();     // commemt if past from argument
 } catch (Exception &E) {
     strcpy (error_message, E.Message.c_str());
     errCode = -1;
 }
 delete dbhandle;
 return (errCode);
}

int FM_Reload(char *case_no, char *ole_db, char *error_message)
{
 int errCode = 0;

 return (errCode);
}

int FM_Transfer(char *case_no, char *ole_db, char *error_message)
{
 int errCode = 0;

 return (errCode);
}



