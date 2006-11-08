/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: dac_pl_cal.h
**
** SAMPLE:  header file for dac_pl_cal.cpp
**
****************************************************************************/
#ifndef _Dac_Pl_Cal
#define _Dac_Pl_Cal

#ifdef __cplusplus
extern "C"
{
#endif

int dac_pl_cal(char *case_sn, char *alias, char *uid, char *upw, char *error_message);
int dac_pl_fnl(char *case_sn, char *alias, char *uid, char *upw, char *error_message);

#ifdef __cplusplus
}
#endif

#endif
