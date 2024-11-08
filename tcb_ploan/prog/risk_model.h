/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: risk_model.h
**
** SAMPLE:  header file for risk_model.sqC
**
****************************************************************************/

#ifndef RISK_MODEL_H
#define RISK_MODEL_H

#ifdef __cplusplus
extern "C"
{
#endif

// classes and methods
class RiskModel
{

  public:
    int PrepareJcicTables(char *case_sn, char *idn);
    int Prescreen(char *case_sn, char *idn, int product_type_code);
    int GenerateScreenVars(char *case_sn, char *idn, char *birthday, char *sysdate);
    int GeneratePdacoScore(char *case_sn, char *idn);
    int SaveScore(char *case_sn, char *idn);
    int CleanTables(char *case_sn, char *idn);
    int Calculate_PB(char *case_sn, char *idn, char *birthday, char *sysdate, int product_type_code);
    int get_pb(char *case_sn, char *idn, float *pb);
    int get_prescreen_status (char *case_sn, char *idn, int *code, char *msg, int *unsecured_bal);
};


#ifdef __cplusplus
}
#endif

#endif // RISK_MODEL_H

