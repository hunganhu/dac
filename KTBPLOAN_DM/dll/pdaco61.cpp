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
** Description: PDACO V6.1 risk model
**
****************************************************************************/
//---------------------------------------------------------------------------
// to do:
//
#pragma hdrstop

#include <Math.hpp>
#include <math.h>
#include "pdaco61.h"
#include "errors.h"
#define min(a,b)  ((a) > (b)) ? (b): (a)
#define max(a,b)  ((a) > (b)) ? (a): (b)

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)


PDACO::PDACO (char *msn, char* input_time):
    msn(msn), input_time(input_time), ps_code(0), ps_msg("")
{
}

//---------------------------------------------------------------------------
PDACO::~PDACO ()
{
}
int PDACO::input_npv_test(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    hostVars[0] = msn;
    hostVars[1] = input_time;
    handler->ExecSQLQry(SQLCommands[Get_AppInfo_Record], hostVars, 1, ds);
    ds->First();
    if (!ds->Eof) {
        LOAN_AMOUNT = ds->FieldValues["LOAN_AMOUNT"];
        TOTAL_TERM = ds->FieldValues["TOTAL_TERM"];
        period = TOTAL_TERM;
        apr = ds->FieldByName("APR")->AsFloat;
        app_fee = ds->FieldByName("APP_FEE")->AsFloat;
        pb = ds->FieldByName("PB")->AsFloat;
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
int PDACO::CreateWorkingTables(TADOHandler *handler)
{
 try {
    handler->ExecSQLCmd(SQLCommands[Drop_Working_Tables]);
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
int PDACO::PrepareJcicSourceTables(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;
 int record_count;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    hostVars[0] = msn;
    hostVars[1] = input_time;
    // add code to check if msn exists  first select then insert
    handler->ExecSQLQry(SQLCommands[Get_Applicant_Record], hostVars, 1, ds);
    record_count = ds->RecordCount;
    if (record_count > 0) {
       handler->ExecSQLCmd(SQLCommands[Insert_PDACO_V61], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Dedup_KRM021], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Dedup_KRM023], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Dedup_STM007], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Dedup_BAM086], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Insert_JAS002_DELINQUENT], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Insert_JAS002_BAD_CHECK], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Insert_JAS002_REJECT], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Insert_JAS002_STOP_CARD], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Dedup_KRM037], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Dedup_VAM102], hostVars, 1);
       handler->ExecSQLCmd(SQLCommands[Prepare_JCIC_Source_Tables]);
    }
    else {
       ps_code = PSCode(PSCODE_3);
       ps_msg = PSMsg(PSCODE_3);
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
int PDACO::PrepareJcicData(TADOHandler *handler)
{
 try {
    handler->ExecSQLCmd(SQLCommands[Prepare_JCIC_Data]);
 } catch (Exception &E) {
     throw;
 }
 return 0;
}
//---------------------------------------------------------------------------
int PDACO::GenerateScreenVars(TADOHandler *handler)
{
 PrepareJcicSourceTables(handler);
 PrepareJcicData(handler);
 return 0;
}
//---------------------------------------------------------------------------
int PDACO::get_scorecard(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
     hostVars[0] = msn;
     hostVars[1] = input_time;
     handler->ExecSQLQry(SQLCommands[Get_Prescreen_Result], hostVars, 1, ds);
     ds->First();
     if (!ds->Eof) {
        krm021_hit = ds->FieldValues["KRM021_HIT"];
        krm023_hit = ds->FieldValues["KRM023_HIT"];
        bam086_hit = ds->FieldValues["BAM086_HIT"];
        ind001     = ds->FieldValues["IND001"];
        jas002_defect = ds->FieldValues["JAS002_DEFECT"];  // ScreenOut rule #1 Major Derogatory within 3 year > 0
        app_max_bucket = ds->FieldValues["MAX_BUCKET"];    // ScreenOut rule #2 Max. bucket of credit lines > 3
        delinquent_months = ds->FieldValues["FS334"];      // ScreenOut rule #3 Max bucket of loans > 3
        fs044 = ds->FieldValues["FS044"];                  // ScreenOut rule #4 no. of loans w/ pass_due_amount > 0
        cash_max_bucket = ds->FieldValues["FS302"];        // ScreenOut rule #5 no. of delinquent cash cards > 0
        MS605 = ds->FieldValues["MS605"];
        CDEF_FLAG_1M = ds->FieldValues["CDEF_FLAG_1M"];
        if (ds->FieldValues["FS059_3M_1K"].IsNull())
          FS059_3M_1K = 0;
        else
           FS059_3M_1K = ds->FieldValues["FS059_3M_1K"];        // PostScreen #6 121- card delinquent in 3M
        FS334B_1M = ds->FieldValues["FS334B_1M"];               // PostScreen #4 119- bam delinquent in 1M
        card_force_stop = ds->FieldValues["CARD_FORCE_STOP"];   // PostScreen #5 120- card force stop
        cash_utilization = ds->FieldValues["CASH_UTILIZATION"]; // PostScreen #1 104- cash util >=.95
        revolving_amt = ds->FieldValues["REVOLVING_AMT"];       // PostScreen #2 108- double card >=500K
        REQUEST_AMT = ds->FieldValues["REQUEST_AMT"];
        TOTAL_TERM = ds->FieldValues["TOTAL_TERM"];
        period = TOTAL_TERM;
        apr = ds->FieldValues["APR"];
        app_fee = ds->FieldValues["APP_FEE"];
        debt_flag = ds->FieldValues["DEBT_FLAG"];               // PostScreen #7 122- debt note in krm037
        note_flag = ds->FieldValues["NOTE_FLAG"];               // PostScreen #8 123- note in vam102
        if (ds->FieldValues["MONTHLY_INCOME"].IsNull())
          monthly_income = 0;
        else
           monthly_income = ds->FieldValues["MONTHLY_INCOME"];
        MS606 = ds->FieldValues["MS606"];                       // PostScreen #3 116- unsecured loan >=1M
     }
     if (jas002_defect > 0 || app_max_bucket > 3 || fs044 > 0 || delinquent_months > 3 || cash_max_bucket > 0)
     	scorecard = 0;           // score card P0  meet screenout rule #1-5
     else if (!(krm021_hit == 1 && krm023_hit== 1 && ind001 == 0))  // score card P1  insufficient data (gray/write)
     	scorecard = 1;
     else if (MS605 <= 50.0)  // score card P2  credit card limit (MS605) <= 50k
     	scorecard = 2;
     else if (MS605 > 50.0 && MS605 <= 100.0)  // 50k < credit card limit (MS605) <= 100k
	if (CDEF_FLAG_1M != 1) // score card P3  50k < credit card limit (MS605) <= 100k, credit NOT revolve in latest month
	   scorecard = 3;
	else                   // score card P3  50k < credit card limit (MS605) <= 100k, credit revolve in latest month
	   scorecard = 4;
     else        // score card P5  100k < credit card limit (MS605)
	scorecard = 5;
 } catch (Exception &E) {
     ds->Close();
     delete ds;
     throw;
 }
 ds->Close();
 delete ds;
 return (scorecard);
}
/*
  For a table column with data type decimal(p, s),
    ds->FieldValues["m1_recovery_ratio"] will return 0.
    ds->FieldByName("m1_recovery_ratio")->AsFloat will return correct value.
  Conclusion:
    For a column with data type char, varchar, you can get the value with
     ds->FieldValues["m1_recovery_ratio"].
    For a column with number data type, you'd better use
     ds->FieldByName("m1_recovery_ratio")->AsFloat to get its value.
*/
//---------------------------------------------------------------------------
int PDACO::PDACO61P0Raw(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    handler->ExecSQLCmd(SQLCommands[PDACO61_P0]);
    hostVars[0] = msn;
    hostVars[1] = input_time;
    handler->ExecSQLQry(SQLCommands[Get_P0_Raw_Variables], hostVars, 1, ds);
    ds->First();
    if (!ds->Eof) {
        FS302_FG = ds->FieldByName("FS302_FG")->AsFloat;
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
int PDACO::PDACO61P1Raw(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    handler->ExecSQLCmd(SQLCommands[PDACO61_P1]);
    hostVars[0] = msn;
    hostVars[1] = input_time;
    handler->ExecSQLQry(SQLCommands[Get_P1_Raw_Variables], hostVars, 1, ds);
    ds->First();
    if (!ds->Eof) {
        GRAY2_FLAG = ds->FieldByName("GRAY2_FLAG")->AsFloat;
        krm023_hit = ds->FieldByName("KRM023_HIT")->AsFloat;
        card_force_stop = ds->FieldByName("CARD_FORCE_STOP")->AsFloat;
        fs059_12m_1k = ds->FieldByName("FS059_12M_1K")->AsFloat;
        FS031 = ds->FieldByName("FS031")->AsFloat;
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
int PDACO::PDACO61P2Raw(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    handler->ExecSQLCmd(SQLCommands[PDACO61_P2]);
    hostVars[0] = msn;
    hostVars[1] = input_time;
    handler->ExecSQLQry(SQLCommands[Get_P2_Raw_Variables], hostVars, 1, ds);
    ds->First();
    if (!ds->Eof) {
        FS016C_9M_T1 = ds->FieldByName("FS016C_9M_T1")->AsFloat;
        CDEF_FLAG_1M = ds->FieldByName("CDEF_FLAG_1M")->AsFloat;
        MS093 = ds->FieldByName("MS093")->AsFloat;
        MS094B = ds->FieldByName("MS094B")->AsFloat;
        MS105 = ds->FieldByName("MS105")->AsFloat;
        WI001_9M = ds->FieldByName("WI001_9M")->AsFloat;
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
int PDACO::PDACO61P3Raw(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    handler->ExecSQLCmd(SQLCommands[PDACO61_P3]);
    hostVars[0] = msn;
    hostVars[1] = input_time;
    handler->ExecSQLQry(SQLCommands[Get_P3_Raw_Variables], hostVars, 1, ds);
    ds->First();
    if (!ds->Eof) {
        LU_FLAG = ds->FieldByName("LU_FLAG")->AsFloat;
        FS021_9M = ds->FieldByName("FS021_9M")->AsFloat;
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
int PDACO::PDACO61P4Raw(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    handler->ExecSQLCmd(SQLCommands[PDACO61_P4]);
    hostVars[0] = msn;
    hostVars[1] = input_time;
    handler->ExecSQLQry(SQLCommands[Get_P4_Raw_Variables], hostVars, 1, ds);
    ds->First();
    if (!ds->Eof) {
        MS604_R = ds->FieldByName("MS604_R")->AsFloat;
        RS017_R_TRAN = ds->FieldByName("RS017_R_TRAN")->AsFloat;
        MS001_12M_1K_Q = ds->FieldByName("MS001_12M_1K_Q")->AsFloat;
        FS3036_T5 = ds->FieldByName("FS3036_T5")->AsFloat;
        FS031_TRAN4 = ds->FieldByName("FS031_TRAN4")->AsFloat;
        FS546_9M_TRAN = ds->FieldByName("FS546_9M_TRAN")->AsFloat;
        INT053_6_Q = ds->FieldByName("INT053_6_Q")->AsFloat;
        MS093 = ds->FieldByName("MS093")->AsFloat;
        MS094B = ds->FieldByName("MS094B")->AsFloat;
        MS105 = ds->FieldByName("MS105")->AsFloat;
        WI004_9M = ds->FieldByName("WI004_9M")->AsFloat;
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
int PDACO::PDACO61P5Raw(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
    handler->ExecSQLCmd(SQLCommands[PDACO61_P5]);
    hostVars[0] = msn;
    hostVars[1] = input_time;
    handler->ExecSQLQry(SQLCommands[Get_P5_Raw_Variables], hostVars, 1, ds);
    ds->First();
    if (!ds->Eof) {
        FS016F_12M = ds->FieldByName("FS016F_12M")->AsFloat;
        RS017_R_TRAN2 = ds->FieldByName("RS017_R_TRAN2")->AsFloat;
        MS074 = ds->FieldByName("MS074")->AsFloat;
        FS205_3M_1K_Q_TRAN2 = ds->FieldByName("FS205_3M_1K_Q_TRAN2")->AsFloat;
        FS031_1M_Q_TRAN2 = ds->FieldByName("FS031_1M_Q_TRAN2")->AsFloat;
        FS073B_12M_R = ds->FieldByName("FS073B_12M_R")->AsFloat;
        MS093 = ds->FieldByName("MS093")->AsFloat;
        MS094B = ds->FieldByName("MS094B")->AsFloat;
        MS105 = ds->FieldByName("MS105")->AsFloat;
        WI003_9M_T = ds->FieldByName("WI003_9M_T")->AsFloat;
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
double PDACO::PDACO61P0Score()
{
 rscore = 0.11920 +
	 FS302_FG	* 0.162590 +
	 jas002_defect  * 0.082360;
 rscore = static_cast<int> (rscore * 1000000)/1000000.0;
 if      (rscore <= 0.11920 ) twentile = 2;
 else if (rscore <= 0.201560) twentile = 3;
 else if (rscore >  0.201560) twentile = 4;

 if (twentile == 2) pb = 0.12;
 else if (twentile == 3) pb = 0.24;
 else if (twentile == 4) pb = 0.28;
 pb = pb / 2.0;
 if (jas002_defect > 0)
    {ps_code = PSCode(PSCODE_101); ps_msg = PSMsg(PSCODE_101);}
 else if (app_max_bucket > 3)
    {ps_code = PSCode(PSCODE_102); ps_msg = PSMsg(PSCODE_102);}
 else if (fs044 > 0)
    {ps_code = PSCode(PSCODE_105); ps_msg = PSMsg(PSCODE_105);}
 else if (delinquent_months > 3)
    {ps_code = PSCode(PSCODE_107); ps_msg = PSMsg(PSCODE_107);}
 else if (cash_max_bucket > 0)
    {ps_code = PSCode(PSCODE_103); ps_msg = PSMsg(PSCODE_103);}

 return (pb);
}
//---------------------------------------------------------------------------
double PDACO::PDACO61P1Score()
{
 rscore = pb = 1.1; //set a temporary pb (> 1.0) which will be set to null
 twentile = 0;

 postScreen();
 if (ps_code > 0 ) return(pb);

 if (krm023_hit == 1) {     // Gray 1   krm023_hit
    if (FS031 > 5) {ps_code = PSCode(PSCODE_111); ps_msg = PSMsg(PSCODE_111);}
    else if (fs059_12m_1k > 0) {ps_code = PSCode(PSCODE_112); ps_msg = PSMsg(PSCODE_112);}
    else if (card_force_stop > 0) {ps_code = PSCode(PSCODE_113); ps_msg = PSMsg(PSCODE_113);}
    else {ps_code = PSCode(PSCODE_201); ps_msg = PSMsg(PSCODE_201);}
 }
 else if (GRAY2_FLAG > 0) {  // Gray 2  krm021_hit or bam086_hit
    if (FS031 > 3) {ps_code = PSCode(PSCODE_114); ps_msg = PSMsg(PSCODE_114);}
    else if (card_force_stop > 0) {ps_code = PSCode(PSCODE_115); ps_msg = PSMsg(PSCODE_115);}
    else {ps_code = PSCode(PSCODE_201); ps_msg = PSMsg(PSCODE_201);}
 } else {       // White   no krm023 nor krm021 nor bam086
    ps_code = PSCode(PSCODE_201);
    ps_msg = PSMsg(PSCODE_201);
 }

 return (pb);
}
//---------------------------------------------------------------------------
double PDACO::PDACO61P2Score()
{
 double APR_N, Score_N, term_N, loan_N;

 // period should be the same as TOTAL_TERM, but can be changed when recalculating PB
 monthly_payment = Payment(apr / 12.0, period, -LOAN_AMOUNT, 0.0, ptEndOfPeriod);
 ln001_9m = (monthly_payment/1000.0 + MS093 + (MS094B + MS105)* 0.35)/ WI001_9M;
 ln001_9m_t2 = ln001_9m * (1-FS016C_9M_T1);

 rscore= -0.02923	+
   	 FS016C_9M_T1	*	0.15400 +
   	 ln001_9m_t2	*	0.00316 +
   	 CDEF_FLAG_1M	*	0.06766;
 rscore = static_cast<int> (rscore * 1000000)/1000000.0;

 if      (rscore <= -0.01551)  twentile = 1 ;
 else if (rscore <= -0.00549)  twentile = 2 ;
 else if (rscore <= 0.00562 )  twentile = 3 ;
 else if (rscore <= 0.01940 )  twentile = 4 ;
 else if (rscore <= 0.04620 )  twentile = 5 ;
 else if (rscore <= 0.05837 )  twentile = 6 ;
 else if (rscore <= 0.07083 )  twentile = 7 ;
 else if (rscore <= 0.07631 )  twentile = 8 ;
 else if (rscore <= 0.08300 )  twentile = 9 ;
 else if (rscore <= 0.09374 )  twentile = 10;
 else if (rscore <= 0.10974 )  twentile = 11;
 else if (rscore <= 0.12477 )  twentile = 12;
 else if (rscore <= 0.14445 )  twentile = 13;
 else if (rscore <= 0.18946 )  twentile = 14;
 else if (rscore <= 0.19243 )  twentile = 17;
 else if (rscore >  0.19243 )  twentile = 20;

 APR_N = apr * 100 / 30;
 Score_N = (rscore + 0.021881193 ) / (0.5958727095 + 0.021881193);
 if (Score_N < 0) Score_N = 0;
 else if (Score_N > 1.0)  Score_N = 1.0;
 term_N = period / 120.0;
 loan_N = LOAN_AMOUNT / 3000000.0;

 pb = (P2_X[13] + P2_X[1] * pow(APR_N, P2_X[2]) * Score_N + (P2_X[3] + P2_X[4] * APR_N + P2_X[5] * pow(APR_N, P2_X[2])) * pow(Score_N, P2_X[6]))
              * (1 + term_N * ( P2_X[7] + P2_X[8] * APR_N + P2_X[9] * Score_N))
              * (1 + loan_N * ( P2_X[10] + P2_X[11] * APR_N + P2_X[12] * Score_N ));
 pb = pb / 2.0;
 if (pb > 1.0) pb = 1.0;

 return (pb);
}

//---------------------------------------------------------------------------
double PDACO::PDACO61P3Score()
{
 double APR_N;

 if (LU_FLAG == 1) twentile = 1;
 else {   // LU_FLAG == 0
    if (FS021_9M <=2 ) twentile = 2;
    else twentile = 3;
 }

 APR_N = apr * 100.0 / 30.0;

 if (twentile == 1) rscore = pb = 0.008;
 else if (twentile == 2) rscore = pb = 0.044 + 0.083 * APR_N;
 else if (twentile == 3) rscore = pb = 0.017 + 0.41  * APR_N;
 pb = pb / 2.0;

 return (pb);
}
//---------------------------------------------------------------------------
double PDACO::PDACO61P4Score()
{
 double APR_N, Score_N, term_N, loan_N;

 monthly_payment = Payment(apr / 12.0, period, -LOAN_AMOUNT, 0.0, ptEndOfPeriod);
 ln004_9m = (monthly_payment/1000.0 + MS093 + (MS094B + MS105)* 0.35)/ WI004_9M;
 ln004_9m_q = ln004_9m * ln004_9m;

 rscore = 0.39859	+
   	    MS604_R		*	0.00364 +
   	    RS017_R_TRAN	*	-0.09671 +
   	    MS001_12M_1K_Q	*	0.00000207 +
   	    FS3036_T5		*	0.04757 +
   	    FS031_TRAN4		*	0.00872 +
   	    INT053_6_Q		*	0.09296 +
   	    ln004_9m_q		*	0.04015 +
   	    FS546_9M_TRAN	*	0.01141;
 rscore = static_cast<int> (rscore * 1000000)/1000000.0;

 if      (rscore <= -0.02452) twentile = 1;
 else if (rscore <= 0.00056)  twentile = 2;
 else if (rscore <= 0.01786)  twentile = 3;
 else if (rscore <= 0.03197)  twentile = 4;
 else if (rscore <= 0.04325)  twentile = 5;
 else if (rscore <= 0.05431)  twentile = 6;
 else if (rscore <= 0.06485)  twentile = 7;
 else if (rscore <= 0.07343)  twentile = 8;
 else if (rscore <= 0.08346)  twentile = 9;
 else if (rscore <= 0.09181)  twentile = 10;
 else if (rscore <= 0.10081)  twentile = 11;
 else if (rscore <= 0.11041)  twentile = 12;
 else if (rscore <= 0.12171)  twentile = 13;
 else if (rscore <= 0.13461)  twentile = 14;
 else if (rscore <= 0.14597)  twentile = 15;
 else if (rscore <= 0.16077)  twentile = 16;
 else if (rscore <= 0.18534)  twentile = 17;
 else if (rscore <= 0.20890)  twentile = 18;
 else if (rscore <= 0.25620)  twentile = 19;
 else if (rscore >  0.25620)  twentile = 20;

 APR_N = apr * 100.0 / 30.0;
 Score_N = (rscore + 0.07949958) / (2.864864072 + 0.07949958 );
 if (Score_N < 0) Score_N = 0;
 else if (Score_N > 1.0)  Score_N = 1.0;
 term_N = period / 120.0;
 loan_N =LOAN_AMOUNT / 3000000.0;

 pb = (P4_X[1] * (pow(APR_N, P4_X[2])) * Score_N + (P4_X[3] + P4_X[4] * APR_N + P4_X[5] * pow(APR_N, P4_X[2])) * pow(Score_N, P4_X[6]))
          * (1 + term_N * ( P4_X[7] + P4_X[8] * APR_N + P4_X[9] * Score_N))
          * (1 + loan_N * ( P4_X[10] + P4_X[11] * APR_N + P4_X[12] * Score_N));
 pb = pb / 2.0;
 if (pb > 1.0) pb = 1.0;

 return (pb);
}

//---------------------------------------------------------------------------
double PDACO::PDACO61P5Score()
{
 double APR_N, Score_N, term_N, loan_N;

 monthly_payment = Payment(apr / 12.0, period, -LOAN_AMOUNT, 0.0, ptEndOfPeriod);
 ln003_9m_t = (monthly_payment/1000.0 + MS093 + (MS094B + MS105)* 0.35)/ WI003_9M_T;
 MS074_T2  = LOAN_AMOUNT / 1000.0 + MS074;
//  remove Cap when in production
// if (MS074_T2 > 2250) MS074_T3 = 2250;
// else  MS074_T3 = MS074_T2;

 rscore= 0.25612	+
   	     FS016F_12M         *	0.00321   +
   	     RS017_R_TRAN2      *	-0.03459  +
   	     MS074_T2           *	0.00003294+
   	     FS205_3M_1K_Q_TRAN2*	0.00225   +
   	     ln003_9m_t         *	0.00003516+
   	     FS031_1M_Q_TRAN2   *	0.00171   +
   	     FS073B_12M_R       *	0.02631;
 rscore = static_cast<int> (rscore * 1000000)/1000000.0;

 if      (rscore <= 0.00317) twentile = 1;
 else if (rscore <= 0.01542) twentile = 2;
 else if (rscore <= 0.02500) twentile = 3;
 else if (rscore <= 0.03280) twentile = 4;
 else if (rscore <= 0.04108) twentile = 5;
 else if (rscore <= 0.04851) twentile = 6;
 else if (rscore <= 0.05467) twentile = 7;
 else if (rscore <= 0.06031) twentile = 8;
 else if (rscore <= 0.06764) twentile = 9;
 else if (rscore <= 0.07515) twentile = 10;
 else if (rscore <= 0.08255) twentile = 11;
 else if (rscore <= 0.09187) twentile = 12;
 else if (rscore <= 0.10131) twentile = 13;
 else if (rscore <= 0.11072) twentile = 14;
 else if (rscore <= 0.12268) twentile = 15;
 else if (rscore <= 0.13652) twentile = 16;
 else if (rscore <= 0.15577) twentile = 17;
 else if (rscore <= 0.17960) twentile = 18;
 else if (rscore <= 0.21736) twentile = 19;
 else if (rscore >  0.21736) twentile = 20;

 APR_N = apr * 100.0 / 30.0;
 Score_N = (rscore + 0.017201595) / (0.6799021583 + 0.017201595);
 if (Score_N < 0) Score_N = 0;
 else if (Score_N > 1.0)  Score_N = 1.0;
 term_N = period / 120.0;
 loan_N = LOAN_AMOUNT / 3000000.0;

 pb = (P5_X[1] * pow(APR_N, P5_X[2]) * Score_N + (P5_X[3] + P5_X[4] * APR_N + P5_X[5] * pow(APR_N, P5_X[2])) * pow(Score_N, P5_X[6]))
      * (1 + term_N * (P5_X[7] + P5_X[8] * APR_N + P5_X[9] * Score_N))
      * (1 + loan_N * (P5_X[10] + P5_X[11] * APR_N + P5_X[12] * Score_N));
 pb = pb / 2.0;
 if (pb > 1.0) pb = 1.0;

 return (pb);
}

//---------------------------------------------------------------------------
double PDACO::recal_Pdaco61Pb(double loan_amt, double newapr, int term)
{
 LOAN_AMOUNT = loan_amt;
 apr = newapr;
 period = term;
 switch (scorecard) {
 	case 0: PDACO61P0Score();  // Scorecard P0
     	        break;
     	case 1: PDACO61P1Score();  // Scorecard P1
     	        break;
     	case 2: PDACO61P2Score();  // Scorecard P2
     	        break;
     	case 3: PDACO61P3Score();  // Scorecard P3
     	        break;
     	case 4: PDACO61P4Score();  // Scorecard P4
     	        break;
     	case 5: PDACO61P5Score(); // Scorecard P5
     	        break;
 }
// if (pb > getPbCap()) {
//    ps_code = PSCode(PSCODE_109);
//    ps_msg = PSMsg(PSCODE_109);
// }

 return (pb);
}

//---------------------------------------------------------------------------
int PDACO::GeneratePdaco61Score(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;
 int card;
 char buf[1024];

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
     CreateWorkingTables(handler);
     GenerateScreenVars(handler);
     card = get_scorecard(handler);
     switch (card) {
     	case 0: PDACO61P0Raw(handler);  // Scorecard P0
                setLoanAmount();
     	        PDACO61P0Score();
                sprintf(buf, SQLCommands[Update_P0_Transform], twentile,
                        msn, input_time);
                handler->ExecSQLCmd(buf);
     	        break;
     	case 1: PDACO61P1Raw(handler);  // Scorecard P1
                setLoanAmount();
     	        PDACO61P1Score();
                sprintf(buf, SQLCommands[Update_P1_Transform], twentile,
                        msn, input_time);
                handler->ExecSQLCmd(buf);
     	        break;
     	case 2: PDACO61P2Raw(handler);  // Scorecard P2
                setLoanAmount();
     	        PDACO61P2Score();
                sprintf(buf, SQLCommands[Update_P2_Transform], twentile, ln001_9m,
                        ln001_9m_t2, monthly_payment, msn, input_time);
                handler->ExecSQLCmd(buf);
     	        break;
     	case 3: PDACO61P3Raw(handler);  // Scorecard P3
     	        PDACO61P3Score();
                setLoanAmount();
                sprintf(buf, SQLCommands[Update_P3_Transform], twentile,
                        msn, input_time);
                handler->ExecSQLCmd(buf);
     	        break;
     	case 4: PDACO61P4Raw(handler);  // Scorecard P4
                setLoanAmount();
     	        PDACO61P4Score();
                sprintf(buf, SQLCommands[Update_P4_Transform], twentile, ln004_9m,
                        ln004_9m_q, monthly_payment, msn, input_time);
                handler->ExecSQLCmd(buf);
     	        break;
     	case 5: PDACO61P5Raw(handler);  // Scorecard P5
                setLoanAmount();
     	        PDACO61P5Score();
                sprintf(buf, SQLCommands[Update_P5_Transform], twentile, ln003_9m_t,
                        MS074_T2, monthly_payment, msn, input_time);
                handler->ExecSQLCmd(buf);
     	        break;
     }

   if (card > 1) {
      if (LOAN_AMOUNT < 150000) {
         ps_code = PSCode(PSCODE_117);
         ps_msg = PSMsg(PSCODE_117);
      }
//      else if (pb > getPbCap()) {
//         ps_code = PSCode(PSCODE_109);
//         ps_msg = PSMsg(PSCODE_109);
//      }
   }
#ifdef _TRACE
     WriteTraceRecord(handler);
#endif
     DropWorkingTables(handler);
 } catch(cc_error &Err){
#ifdef _TRACE
     WriteTraceRecord(handler);
#endif
 } catch (Exception &E) {
     ds->Close();
     delete ds;
     throw;
 }
 ds->Close();
 delete ds;

 return (ps_code);
}
//---------------------------------------------------------------------------
int PDACO::WriteTraceRecord(TADOHandler *handler)
{
 char buf[1024];
 try {
    sprintf(buf, SQLCommands[Update_Audit_Table], LOAN_AMOUNT, FSC_AMOUNT,
                 scorecard, pb, ps_code, ps_msg, rscore, msn, input_time);
    handler->ExecSQLCmd(buf);

    handler->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
    sprintf(buf, SQLCommands[Update_P1_PB], msn, input_time);
    handler->ExecSQLCmd(buf);
 } catch (Exception &E) {
     throw;
 }
 return 0;
}
//---------------------------------------------------------------------------
double PDACO::getCapAmount()
{
 switch (scorecard) {
        case 0: cap_amount = 0.0;       // Scorecard P0
     	        break;
     	case 1: cap_amount = 0.0;       // Scorecard P1
     	        break;
     	case 2: cap_amount = 300000.0;  // Scorecard P2
     	        break;
     	case 3: if (twentile == 1) cap_amount = 700000.0;  // Scorecard P3
                else if (twentile == 2) cap_amount = 300000.0;
                else if (twentile == 3) cap_amount = 150000.0;
     	        break;
     	case 4: cap_amount = 500000.0;  // Scorecard P4
     	        break;
     	case 5: cap_amount = 1000000.0; // Scorecard P5
     	        break;
 }
 return(cap_amount);
}
//---------------------------------------------------------------------------
double PDACO::getPbCap()
{
 // APR        PB Cap   APR needs to be round to integer
 // 11% and plus   4%
 // 10%            3.75%
 // 9%             3.5%
 // 8%             3.25%
 // 7%             3%
 // 6%             2.75%
 // 5% and below   2.5%
 double pbCap;
 if (apr >= 0.105) pbCap = 0.04;       // 11% and plus
 else if (apr >= 0.095) pbCap = 0.0375; // 10%
 else if (apr >= 0.085) pbCap = 0.035;  // 9%
 else if (apr >= 0.075) pbCap = 0.0325; // 8%
 else if (apr >= 0.065) pbCap = 0.03;   // 7%
 else if (apr >= 0.055) pbCap = 0.0275; // 6%
 else pbCap = 0.025;                  // 5% and below

 return(pbCap);
}
//---------------------------------------------------------------------------
int PDACO::getFscCap()
{
 double fsc_lendable = monthly_income * 22 - MS606 * 1000;
 if (fsc_lendable < 0) fsc_lendable = 0;
 FSC_AMOUNT = (static_cast<int>(fsc_lendable / 10000)) * 10000;

 return(FSC_AMOUNT);
}
//---------------------------------------------------------------------------
double PDACO::setLoanAmount()
{
 LOAN_AMOUNT = principal = min(min(REQUEST_AMT, getCapAmount()),getFscCap());
 return(LOAN_AMOUNT);
}
//---------------------------------------------------------------------------
double PDACO::upperLendableAmount()
{
  return(min(getCapAmount(), getFscCap()));
}
//---------------------------------------------------------------------------
double PDACO::getLoanAmount()
{
 return(LOAN_AMOUNT);
}
//---------------------------------------------------------------------------
double PDACO::getUnsecuredBalance()
{
 return(MS606);
}
//---------------------------------------------------------------------------
double PDACO::getDoubleCardBalance()
{
 return(revolving_amt);
}
//---------------------------------------------------------------------------
double PDACO::getRequestAmount()
{
 return(REQUEST_AMT);
}
//---------------------------------------------------------------------------
double PDACO::getPdaco61Score()
{
 return rscore;
}
//---------------------------------------------------------------------------
double PDACO::getPdaco61PB()
{
 return pb;
}
//---------------------------------------------------------------------------
double PDACO::getPdaco61CapAmount()
{
 return cap_amount;
}
//---------------------------------------------------------------------------

double PDACO::monthly_debt()
{
 return ms101;
}
//---------------------------------------------------------------------------
int PDACO::getPsCode()
{
 return ps_code;
}
//---------------------------------------------------------------------------
void   PDACO::setPsCode(int code)
{
 ps_code = PSCode(code);
 ps_msg  = PSMsg(code);
}
//---------------------------------------------------------------------------
double PDACO::getScoreCard()
{
 return scorecard;
}
//---------------------------------------------------------------------------
double PDACO::getApr()
{
 return apr;
}
//---------------------------------------------------------------------------
int PDACO::getTerm()
{
 return TOTAL_TERM;
}
//---------------------------------------------------------------------------
int PDACO::getAppFee()
{
 return app_fee;
}
//---------------------------------------------------------------------------
void PDACO::postScreen()
{
//---------------------------------------------------------------
// FS334B_1M	> 0	OR	204   BAM_MAX_BUCKET
// DELINQUENT_FLAG	=1	OR	205   FS059_3M_1K
// DEBT_CODE	=1	OR	207
// DEBT_FLAG    =1
// NOTE_FLAG    =1
// STOP_CODE_FLAG	=1	OR	206   CARD_FORCE_STOP
// FS314B	>= 0.95	OR	201  cash_utilization
// MS602	>= 500	OR	202  REVOLVING_AMT
// MS606	>= 1000	OR	203  MS606
//---------------------------------------------------------------
 if (FS334B_1M > 0)
    {ps_code = PSCode(PSCODE_119); ps_msg = PSMsg(PSCODE_119);}
 else if (FS059_3M_1K >= 1)
    {ps_code = PSCode(PSCODE_121); ps_msg = PSMsg(PSCODE_121);}
 else if (debt_flag > 0)
    {ps_code = PSCode(PSCODE_122); ps_msg = PSMsg(PSCODE_122);}
 else if (note_flag > 0)
    {ps_code = PSCode(PSCODE_123); ps_msg = PSMsg(PSCODE_123);}
 else if (card_force_stop >= 1)
    {ps_code = PSCode(PSCODE_120); ps_msg = PSMsg(PSCODE_120);}
 else if (cash_utilization > 0)
    {ps_code = PSCode(PSCODE_104); ps_msg = PSMsg(PSCODE_104);}
 else if (revolving_amt >= 500)
    {ps_code = PSCode(PSCODE_108); ps_msg = PSMsg(PSCODE_108);}
 else if (MS606 >= 1000)
    {ps_code = PSCode(PSCODE_116); ps_msg = PSMsg(PSCODE_116);}
}

//---------------------------------------------------------------------------
String PDACO::vam102_message(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;
 String note = "";

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
     hostVars[0] = msn;
     hostVars[1] = input_time;
     handler->ExecSQLQry(SQLCommands[Get_VAM102_Note], hostVars, 1, ds);
     ds->First();
     while (!ds->Eof) {
        if(!ds->FieldValues["NOTE"].IsNull()){
          note += static_cast<String>(ds->FieldValues["NOTE"]).Trim();
          note += " ";
        };
        ds->Next();
     }
 } catch (Exception &E) {
     ds->Close();
     delete ds;
     throw;
 }
 ds->Close();
 delete ds;
 return (note);
};

