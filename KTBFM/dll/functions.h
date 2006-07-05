/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: functions.h
**
** Description: header file for common functions
**
****************************************************************************/
//---------------------------------------------------------------------------

#ifndef functionsH
#define functionsH

#include <vcl.h>
#include <set>
#include <time.h>
using namespace std;

//---------------------------------------------------------------------------
int validZIP (String zip);  // 0: not found, or the first digit of zip code
int yrmon_to_mon(String inquiry_month, bool time_lock, String lock);
bool validate_date(String date);
int check_expiration(int lock);
int Days_between (String begin, String end);
char * ExecutionTime();
#endif
