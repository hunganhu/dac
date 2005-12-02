/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: functions.h
**
** SAMPLE:  header file for functions.cpp
**
**         This is the header file for the functions.cpp utility
**         file.
**
**
****************************************************************************/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#ifdef __cplusplus
extern "C"
{
#endif

int validate_date(char *date);  // format of date is yyymmdd
int is_future(char *date);      // format of date is yyymmdd
int is_future_year(int year);   // format of year is yyy
float get_risk_cut_point(int loan_amt, int term, double max_apr);
void print_cut_point();
char *CurrTime_MinGuo();
int age (char *birthday);
int foreigner(char *idn);

#ifdef __cplusplus
}
#endif

#endif // FUNCTIONS_H

