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
**         This is the header file for the risk_model.sqC utility
**         file. The risk_model.sqC file is compiled and linked in as an object 
**         module with embedded SQL sample programs by the supplied makefile 
**         and build files.
**
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
	
  float risk_cut[][][12];
	
  public:
    int CreateWorkingTables();
    int PrepareJcicTables(char *case_sn, char *idn);
    int Prescreen(char *case_sn, char *idn);
    int GeneratePdacoScore(char *case_sn, char *idn);
    int SaveScore(char *case_sn, char *idn);
    int CleanTables(char *case_sn, char *idn);
    float get_risk_cut_point(float apr, int term, int loan_amount);
};


#ifdef __cplusplus
}
#endif

#endif // RISK_MODEL_H

