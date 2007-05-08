/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: closecases.h
**
** SAMPLE:  header file for closecases.sqC
**
****************************************************************************/
#ifndef _CloseCases
#define _CloseCases

#ifdef __cplusplus
extern "C"
{
#endif

int call_final_npv();
char * CurrMonth();
const char *DEFAULT_LOG_DIRECTORY = "/home/escorap1/log"; // default log directory

#ifdef __cplusplus
}
#endif

#endif
