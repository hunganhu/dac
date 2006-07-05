/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: pdaco.cpp
**
** Description: PDACO V1.0 risk model
**
****************************************************************************/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "pdaco.h"
#include "errors.h"
using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)


PDACO::PDACO (char *case_no, char *idn, int now):
    case_no(case_no), idn(idn), now(now)
{
}

//---------------------------------------------------------------------------
PDACO::~PDACO ()
{
}
//---------------------------------------------------------------------------
int PDACO::CreateWorkingTables(TADOHandler *handler)
{
 try {
    handler->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
 } catch (Exception &E) {
     throw;
 }
 return 0;
}
//---------------------------------------------------------------------------
int PDACO::DropWorkingTables(TADOHandler *handler)
{
 try {
    handler->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
 } catch (Exception &E) {
     throw;
 }
 return 0;
}
//---------------------------------------------------------------------------
int PDACO::Prescreen_New(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
     CreateWorkingTables(handler);
     GenerateScreenVars(handler);
     hostVars[0] = case_no;
     handler->ExecSQLQry(SQLCommands[Get_Prescreen_Result], hostVars, 0, ds);
     ds->First();
     if (!ds->Eof) {
        krm021_hit = ds->FieldValues["KRM021_HIT"];
        krm023_hit = ds->FieldValues["KRM023_HIT"];
        bam086_hit = ds->FieldValues["BAM086_HIT"];
        ind001     = ds->FieldValues["IND001"];
        jas002_defect = ds->FieldValues["JAS002_DEFECT"];
        app_max_bucket = ds->FieldValues["MAX_BUCKET"];
        fs044 = ds->FieldValues["fs044"];
        fs059_1k_12m = ds->FieldValues["FS059_1K_12M"];
        delinquent_months = ds->FieldValues["fs334"];
        card_force_stop = ds->FieldValues["CARD_FORCE_STOP"];
        cash_max_bucket = ds->FieldValues["fs302"];
        cash_utilization = ds->FieldValues["CASH_UTILIZATION"];
        revolving_amt = ds->FieldValues["REVOLVING_AMT"];
     }
     // Generate Segmentation and Prescreen message
     ps_code = PSCODE_0;     // initial to no error
     segment = seg_S;        // initial to Scorable
     if (jas002_defect > 0) {
        ps_code = PSCODE_101; }
     else if (app_max_bucket > 3) {
        ps_code = PSCODE_102; }
     else if (delinquent_months > 3) {
        ps_code = PSCODE_103; }
     else if (cash_max_bucket > 0) {
        ps_code = PSCODE_104; }
     else if (cash_utilization > 0) {
        ps_code = PSCODE_105; }
     else if (revolving_amt > 500) {
        ps_code = PSCODE_106; }

     if (ps_code > 0)
        segment = seg_F;
     else {
        if (krm021_hit == 1 && krm023_hit== 1 && ind001 == 0) {    // Scorable
           segment = seg_S;
           GeneratePdacoScore(handler);
           if (score > 0.06647) {
              ps_code = PSCODE_107;
              segment = seg_F;
           }
        }
        else {  // Non-scorable
           if (card_force_stop > 0)
              ps_code = PSCODE_110;
           else if (fs059_1k_12m > 0)
              ps_code = PSCODE_109;
           if (ps_code > 0)
              segment = seg_If;
           else
              segment = seg_Ip;
        }
     }
#ifdef _TRACE
     handler->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     DropWorkingTables(handler);
 } catch (Exception &E) {
     ds->Close();
     delete ds;
     throw;
 }
 ds->Close();
 delete ds;

 return 0;
}
//---------------------------------------------------------------------------
int PDACO::Prescreen_Reload(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
     CreateWorkingTables(handler);
     GenerateScreenVars(handler);
     hostVars[0] = case_no;
     handler->ExecSQLQry(SQLCommands[Get_Prescreen_Result], hostVars, 0, ds);
     ds->First();
     if (!ds->Eof) {
        krm021_hit = ds->FieldValues["KRM021_HIT"];
        krm023_hit = ds->FieldValues["KRM023_HIT"];
        bam086_hit = ds->FieldValues["BAM086_HIT"];
        ind001     = ds->FieldValues["IND001"];
        jas002_defect = ds->FieldValues["JAS002_DEFECT"];
        app_max_bucket = ds->FieldValues["MAX_BUCKET"];
        fs044 = ds->FieldValues["fs044"];
        fs059_1k_12m = ds->FieldValues["FS059_1K_12M"];
        delinquent_months = ds->FieldValues["fs334"];
        card_force_stop = ds->FieldValues["CARD_FORCE_STOP"];
        cash_max_bucket = ds->FieldValues["fs302"];
        cash_utilization = ds->FieldValues["CASH_UTILIZATION"];
        revolving_amt = ds->FieldValues["REVOLVING_AMT"];
     }
     // Generate Segmentation and Prescreen message
     ps_code = PSCODE_0;     // initial to no error
     segment = seg_S;        // initial to Scorable
     if (jas002_defect > 0) {
        ps_code = PSCODE_101; }
     else if (app_max_bucket > 3) {
        ps_code = PSCODE_102; }
     else if (delinquent_months > 0) {  // Reload only
        ps_code = PSCODE_111; }
     else if (cash_max_bucket > 0) {
        ps_code = PSCODE_104; }
     else if (cash_utilization > 0) {
        ps_code = PSCODE_105; }
     else if (revolving_amt > 500) {
        ps_code = PSCODE_106; }

     if (ps_code > 0)
        segment = seg_F;
     else {
        if (krm021_hit == 1 && krm023_hit== 1 && ind001 == 0) {    // Scorable
           segment = seg_S;
           GeneratePdacoScore(handler);
           if (score > 0.06647) {
              ps_code = PSCODE_107;
              segment = seg_F;
           }
        }
        else {  // Non-scorable
           if (card_force_stop > 0)
              ps_code = PSCODE_110;
           else if (fs059_1k_12m > 0)
              ps_code = PSCODE_109;
           if (ps_code > 0)
              segment = seg_If;
           else
              segment = seg_Ip;
        }
     }
#ifdef _TRACE
     handler->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     DropWorkingTables(handler);
 } catch (Exception &E) {
     ds->Close();
     delete ds;
     throw;
 }
 ds->Close();
 delete ds;

 return 0;
}
//---------------------------------------------------------------------------
int PDACO::Prescreen_Transfer(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
     CreateWorkingTables(handler);
     GenerateScreenVars(handler);
     hostVars[0] = case_no;
     handler->ExecSQLQry(SQLCommands[Get_Prescreen_Result], hostVars, 0, ds);
     ds->First();
     if (!ds->Eof) {
        krm021_hit = ds->FieldValues["KRM021_HIT"];
        krm023_hit = ds->FieldValues["KRM023_HIT"];
        bam086_hit = ds->FieldValues["BAM086_HIT"];
        ind001     = ds->FieldValues["IND001"];
        jas002_defect = ds->FieldValues["JAS002_DEFECT"];
        app_max_bucket = ds->FieldValues["MAX_BUCKET"];
        fs044 = ds->FieldValues["fs044"];
        fs059_1k_12m = ds->FieldValues["FS059_1K_12M"];
        delinquent_months = ds->FieldValues["fs334"];
        card_force_stop = ds->FieldValues["CARD_FORCE_STOP"];
        cash_max_bucket = ds->FieldValues["fs302"];
        cash_utilization = ds->FieldValues["CASH_UTILIZATION"];
        revolving_amt = ds->FieldValues["REVOLVING_AMT"];
     }
     // Generate Segmentation and Prescreen message
     ps_code = PSCODE_0;     // initial to no error
     segment = seg_S;        // initial to Scorable
     if (jas002_defect > 0) {
        ps_code = PSCODE_101; }
     else if (app_max_bucket > 3) {
        ps_code = PSCODE_102; }
     else if (delinquent_months > 3) {
        ps_code = PSCODE_103; }
     else if (cash_max_bucket > 0) {
        ps_code = PSCODE_104; }
     else if (cash_utilization > 0) {
        ps_code = PSCODE_105; }
     else if (revolving_amt > 500) {
        ps_code = PSCODE_106; }

#ifdef _TRACE
     handler->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
     DropWorkingTables(handler);
 } catch (Exception &E) {
     ds->Close();
     delete ds;
     throw;
 }
 ds->Close();
 delete ds;

 return 0;
}
//---------------------------------------------------------------------------
int PDACO::GenerateScreenVars(TADOHandler *handler)
{
 Variant hostVars[5];
 try {
    handler->ExecSQLCmd(SQLCommands[Drop_PROCEDURE_PREPARE_JCIC_DATA]);
    handler->ExecSQLCmd(SQLCommands[Create_PROCEDURE_PREPARE_JCIC_DATA]);
    hostVars[0] = case_no;
    hostVars[1] = idn;
    hostVars[2] = now;
    handler->ExecSQLCmd(SQLCommands[Exec_PROCEDURE_PREPARE_JCIC_DATA], hostVars, 2);
    handler->ExecSQLCmd(SQLCommands[Drop_PROCEDURE_PREPARE_JCIC_DATA]);

 } catch (Exception &E) {
     throw;
 }
 return 0;
}
//---------------------------------------------------------------------------
int PDACO::GeneratePdacoScore(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    handler->ExecSQLCmd(SQLCommands[Drop_PROCEDURE_GENERATE_PDACO_SCORE]);
    handler->ExecSQLCmd(SQLCommands[Create_PROCEDURE_GENERATE_PDACO_SCORE]);
    hostVars[0] = now;
    handler->ExecSQLCmd(SQLCommands[Exec_PROCEDURE_GENERATE_PDACO_SCORE], hostVars, 0);
    handler->ExecSQLCmd(SQLCommands[Drop_PROCEDURE_GENERATE_PDACO_SCORE]);

    hostVars[0] = case_no;
    handler->ExecSQLQry(SQLCommands[Get_PDACO_Score], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
        score = ds->FieldValues["PDACO_SCORE"];
        pb = ds->FieldValues["PB_IN"];
        twentile = ds->FieldValues["PDACO_TWEN"];
        if (!ds->FieldValues["MS101"].IsNull()) {
           ms101 = ds->FieldByName("MS101")->AsFloat;
        }
        else
           ms101 = 0.0;
    }
 } catch (Exception &E) {
     ds->Close();
     delete ds;
     throw;
 }
 ds->Close();
 delete ds;
 return 0;
}
//---------------------------------------------------------------------------
int PDACO::Segment()
{
 return segment;
}
//---------------------------------------------------------------------------
int PDACO::PS_code()
{
 return ps_code;
}
//---------------------------------------------------------------------------
double PDACO::Pdaco_score()
{
 return score;
}
//---------------------------------------------------------------------------
double PDACO::Pdaco_pb()
{
 return pb;
}
//---------------------------------------------------------------------------

double PDACO::monthly_debt()
{
 return ms101;
}
//---------------------------------------------------------------------------
void PDACO::set_PS_code(int code)
{
 ps_code = code;
}

