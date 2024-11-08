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

const int EXPIRATION_DATE = 20070131; // expiration date of this module
const char *EXPIRATION_MSG = "聯徵資料日期超過期限。"; // expiration message
const char *VERSION = "2.0"; // PDACO 6.1 for KTB PLOAN DM  the 1st release

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)
 int TNB_Ploan_AM_Campaign(char *msno, char *jcic_inquiry_date, char *app_input_time,
                          char *bank_code, char *ole_db_str, int gender, char *error);

unsigned int check_credit_card_block(TADOHandler *handler, const AnsiString &msn);
void write_npv(char *msn, char *input_time, double npv, TADOHandler *handler);
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
                  TADOHandler *handler);

#endif
