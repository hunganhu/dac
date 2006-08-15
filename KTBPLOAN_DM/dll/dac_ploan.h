/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: dac_fm.h
**
** Description:  header file for functions used for Kingtown Bank Ploan Campaign
**
****************************************************************************/
//---------------------------------------------------------------------------

#ifndef dac_ploanH
#define dac_ploanH

#include "AdoHandle.h"
#include "functions.h"
#include "errors.h"
#include "pdaco61.h"
#include "ploanNPV.h"

const int EXPIRATION_DATE = 20251231; // expiration date of this module
const char *EXPIRATION_MSG = "核准模組使用期限已過，請洽DAC解決使用權限問題。"; // expiration message

//---------------------------------------------------------------------------
//extern "C" __declspec(dllexport)
// int FM_New(char *case_no, char *ole_db, char *error_message);
extern "C" __declspec(dllexport)
 int TNB_Ploan_AM_Campaign(char *msno, char *jcic_inquiry_date, char *app_input_time,
                          char *bank_code, char *ole_db_str, int gender, char *error);

unsigned int check_credit_card_block(TADOHandler *handler, const AnsiString &msn);
                          
/*
int overall_lookup(int appStatus, int cosStatus, int guaStatus,
                 int appPSCode, int cosPSCode, int guaPSCode,
                 int *pdacoPath, int *incomePath, int *ms101Path,
                 String &dispositionMsg, String &finalMsg, TADOHandler *handler);
int final_lookup(int appStatus, int cosStatus, int guaStatus, int disp_code,
                 int appPSCode, int cosPSCode, int guaPSCode,
                 String &dispositionMsg, String &finalMsg, TADOHandler *handler);
void write_final_result(int &dispCode, String &suggMsg, String &reasonMsg,
                        Loan *prtLoan, PDACO *pdaco_app, PDACO *pdaco_cos, PDACO *pdaco_gua,
                        TADOHandler *handler);
void write_prescreen_result(int dispCode, String suggMsg, String reasonMsg,
                        Loan *prtLoan, PDACO *pdaco_app, PDACO *pdaco_cos, PDACO *pdaco_gua,
                        TADOHandler *dhandler);
void write_bal_transfer_result(Loan *ptrLoan, PDACO *pdaco_app, PDACO *pdaco_cos, PDACO *pdaco_gua,
                        TADOHandler *handler);
*/

#endif
