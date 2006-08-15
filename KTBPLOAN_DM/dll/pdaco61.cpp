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
#pragma hdrstop

#include <Math.hpp>
#include <math.h>
#include "pdaco61.h"
#include "errors.h"
using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)


PDACO::PDACO (char *msn, char* input_time):
    msn(msn), input_time(input_time)
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
int PDACO::PrepareJcicSourceTables(TADOHandler *handler)
{
 Variant hostVars[5];
 try {
    hostVars[0] = msn;
    hostVars[1] = input_time;
    handler->ExecSQLCmd(SQLCommands[Prepare_JCIC_Source_Tables], hostVars, 1);
 } catch (Exception &E) {
     throw;
 }
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
        delinquent_months = ds->FieldValues["fs334"];      // ScreenOut rule #3 Max bucket of loans > 3
        fs044 = ds->FieldValues["fs044"];                  // ScreenOut rule #4 no. of loans w/ pass_due_amount > 0
        cash_max_bucket = ds->FieldValues["fs302"];        // ScreenOut rule #5 no. of delinquent cash cards > 0
        MS605 = ds->FieldValues["MS605"];
        CDEF_FLAG_1M = ds->FieldValues["CDEF_FLAG_1M"];
        fs059_1k_12m = ds->FieldValues["FS059_1K_12M"];
        card_force_stop = ds->FieldValues["CARD_FORCE_STOP"];
        cash_utilization = ds->FieldValues["CASH_UTILIZATION"];
        revolving_amt = ds->FieldValues["REVOLVING_AMT"];
        if (!ds->FieldValues["MS101"].IsNull())
           ms101 = ds->FieldByName("MS101")->AsFloat;
        else
           ms101 = 0.0;
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
        FS302_FG = ds->FieldValues["FS302_FG"];
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
        GRAY2_FLAG = ds->FieldValues["GRAY2_FLAG"];
        krm023_hit = ds->FieldValues["KRM023_HIT"];
        card_force_stop = ds->FieldValues["CARD_FORCE_STOP"];
        fs059_1k_12m = ds->FieldValues["FS059_1K_12M"];
        FS031 = ds->FieldValues["FS031"];
    }
    if (krm023_hit == 1) {     // Gray 1   krm023_hit
       if (FS031 > 5) throw cc_error(PSCODE_111, msn, input_time);
       else if (fs059_1k_12m > 0) throw cc_error(PSCODE_112, msn, input_time);
       else if (card_force_stop > 0) throw cc_error(PSCODE_113, msn, input_time);
    }
    else if (GRAY2_FLAG == 1) {  // Gray 2  krm021_hit or bam086_hit
       if (FS031 > 3) throw cc_error(PSCODE_114, msn, input_time);
       else if (card_force_stop > 0) throw cc_error(PSCODE_115, msn, input_time);
    } else {       // White   no krm023 nor krm021 nor bam086
       throw cc_error(PSCODE_201, msn, input_time);
    }

 } catch(cc_error &Err){
     ds->Close();
     delete ds;
     throw;
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
        FS016C_9M_T1 = ds->FieldValues["FS016C_9M_T1"];
        CDEF_FLAG_1M = ds->FieldValues["CDEF_FLAG_1M"];
        MS093 = ds->FieldValues["MS093"];
        MS094B = ds->FieldValues["MS094B"];
        MS105 = ds->FieldValues["MS105"];
        WI001_9M = ds->FieldValues["WI001_9M"];
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
        LU_FLAG = ds->FieldValues["LU_FLAG"];
        FS021_9M = ds->FieldValues["FS021_9M"];
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
        MS604_R = ds->FieldValues["MS604_R"];
        RS017_R_TRAN = ds->FieldValues["RS017_R_TRAN"];
        MS001_12M_1K_Q = ds->FieldValues["MS001_12M_1K_Q"];
        FS3036_T5 = ds->FieldValues["FS3036_T5"];
        FS031_TRAN4 = ds->FieldValues["FS031_TRAN4"];
        FS546_9M_TRAN = ds->FieldValues["FS546_9M_TRAN"];
        INT053_6_Q = ds->FieldValues["INT053_6_Q"];
        MS093 = ds->FieldValues["MS093"];
        MS094B = ds->FieldValues["MS094B"];
        MS105 = ds->FieldValues["MS105"];
        WI004_9M = ds->FieldValues["WI004_9M"];
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
        FS016F_12M = ds->FieldValues["FS016F_12M"];
        RS017_R_TRAN2 = ds->FieldValues["RS017_R_TRAN2"];
        MS074_T3 = ds->FieldValues["MS074_T3"];
        FS205_3M_1K_Q_TRAN2 = ds->FieldValues["FS205_3M_1K_Q_TRAN2"];
        FS031_1M_Q_TRAN2 = ds->FieldValues["FS031_1M_Q_TRAN2"];
        FS073B_12M_R = ds->FieldValues["FS073B_12M_R"];
        MS093 = ds->FieldValues["MS093"];
        MS094B = ds->FieldValues["MS094B"];
        MS105 = ds->FieldValues["MS105"];
        WI003_9M = ds->FieldValues["WI003_9M"];
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
double PDACO::PDACO61P2Score()
{
 double APR_N, Score_N, term_N, loan_N;

 monthly_payment = Payment(apr / 12.0, period, -LOAN_AMOUNT, 0.0, ptEndOfPeriod);
 ln001_9m = (monthly_payment/1000.0 + MS093 + (MS094B + MS105)* 0.35)/ WI001_9M;
 ln001_9m_t2 = ln001_9m * (1-FS016C_9M_T1);
 p2_score= -0.02923	+
   	   FS016C_9M_T1	*	0.15400 +
   	   ln001_9m_t2	*	0.00316 +
   	   CDEF_FLAG_1M	*	0.06766;

 if      (p2_score <= -0.01551)  twentile = 1 ;
 else if (p2_score <= -0.00549)  twentile = 2 ;
 else if (p2_score <= 0.00562 )  twentile = 3 ;
 else if (p2_score <= 0.01940 )  twentile = 4 ;
 else if (p2_score <= 0.04620 )  twentile = 5 ;
 else if (p2_score <= 0.05837 )  twentile = 6 ;
 else if (p2_score <= 0.07083 )  twentile = 7 ;
 else if (p2_score <= 0.07631 )  twentile = 8 ;
 else if (p2_score <= 0.08300 )  twentile = 9 ;
 else if (p2_score <= 0.09374 )  twentile = 10;
 else if (p2_score <= 0.10974 )  twentile = 11;
 else if (p2_score <= 0.12477 )  twentile = 12;
 else if (p2_score <= 0.14445 )  twentile = 13;
 else if (p2_score <= 0.18946 )  twentile = 14;
 else if (p2_score <= 0.19243 )  twentile = 17;
 else twentile = 20;

 APR_N = apr * 100 / 30;
 Score_N = (p2_score + 0.02188119 ) / (0.679902158 + 0.02188119 );
 term_N = TOTAL_TERM / 120.0;
 loan_N = LOAN_AMOUNT / 3000000.0;

 pb = (P2_X[13] + P2_X[1] * pow(APR_N, P2_X[2]) * Score_N + (P2_X[3] + P2_X[4] * APR_N + P2_X[5] * pow(APR_N, P2_X[2])) * pow(Score_N, P2_X[6]))
              * (1 + term_N * ( P2_X[7] + P2_X[8] * APR_N + P2_X[9] * Score_N))
              * (1 + loan_N * ( P2_X[10] + P2_X[11] * APR_N + P2_X[12] * Score_N ));
 if (pb > 1.0) pb = 1.0;

 return (pb);
}

//---------------------------------------------------------------------------
double PDACO::PDACO61P4Score()
{
 double monthly_payment, ln004_9m, ln004_9m_q, p4_score;
 double APR_N, Score_N, term_N, loan_N, pb;

 monthly_payment = Payment(apr / 12.0, period, -LOAN_AMOUNT, 0.0, ptEndOfPeriod);
 ln004_9m = (monthly_payment/1000.0 + MS093 + (MS094B + MS105)* 0.35)/ WI004_9M,
 ln004_9m_q = ln004_9m * ln004_9m;

 p4_score = 0.39859	+
   	    MS604_R		*	0.00364 +
   	    RS017_R_TRAN	*	-0.09671 +
   	    MS001_12M_1K_Q	*	0.00000207 +
   	    FS3036_T5		*	0.04757 +
   	    FS031_TRAN4		*	0.00872 +
   	    INT053_6_Q		*	0.09296 +
   	    ln004_9m_q		*	0.04015 +
   	    FS546_9M_TRAN	*	0.01141;

 if      (p4_score <= -0.02452) twentile = 1;
 else if (p4_score <= 0.00056)  twentile = 2;
 else if (p4_score <= 0.01786)  twentile = 3;
 else if (p4_score <= 0.03197)  twentile = 4;
 else if (p4_score <= 0.04325)  twentile = 5;
 else if (p4_score <= 0.05431)  twentile = 6;
 else if (p4_score <= 0.06485)  twentile = 7;
 else if (p4_score <= 0.07343)  twentile = 8;
 else if (p4_score <= 0.08346)  twentile = 9;
 else if (p4_score <= 0.09181)  twentile = 10;
 else if (p4_score <= 0.10081)  twentile = 11;
 else if (p4_score <= 0.11041)  twentile = 12;
 else if (p4_score <= 0.12171)  twentile = 13;
 else if (p4_score <= 0.13461)  twentile = 14;
 else if (p4_score <= 0.14597)  twentile = 15;
 else if (p4_score <= 0.16077)  twentile = 16;
 else if (p4_score <= 0.18534)  twentile = 17;
 else if (p4_score <= 0.20890)  twentile = 18;
 else if (p4_score <= 0.25620)  twentile = 19;
 else twentile = 20;

 APR_N = apr * 100 / 30;
 Score_N = (p4_score + 0.02188119 ) / (0.679902158 + 0.02188119 );
 term_N = TOTAL_TERM / 120.0;
 loan_N =LOAN_AMOUNT / 3000000.0;

 pb = (P2_X[1] * (pow(APR_N, P2_X[2])) * Score_N + (P2_X[3] + P2_X[4] * APR_N + P2_X[5] * pow(APR_N, P2_X[2])) * pow(Score_N, P2_X[6]))
          * ( 1 + term_N * ( P2_X[7] + P2_X[8] * APR_N + P2_X[9] * Score_N))
          * ( 1 + loan_N * ( P2_X[10] + P2_X[11] * APR_N + P2_X[12] * Score_N));
 if (pb > 1.0) pb = 1.0;

 return (pb);
}

//---------------------------------------------------------------------------
double PDACO::PDACO61P5Score()
{
 double monthly_payment, ln003_9m_t, p5_score;
 double APR_N, Score_N, term_N, loan_N, pb;

 monthly_payment = Payment(apr / 12.0, period, -LOAN_AMOUNT, 0.0, ptEndOfPeriod);
 ln003_9m_t = (monthly_payment/1000.0 + MS093 + (MS094B + MS105)* 0.35)/ WI003_9M_T,
 p5_score= 0.25612	+
   	     FS016F_12M         *	0.00321   +
   	     RS017_R_TRAN2      *	-0.03459  +
   	     MS074_T3           *	0.00003294+
   	     FS205_3M_1K_Q_TRAN2*	0.00225   +
   	     ln003_9m_t         *	0.00003516+
   	     FS031_1M_Q_TRAN2   *	0.00171   +
   	     FS073B_12M_R       *	0.02631;

 if      (p5_score <= 0.00317) twentile = 1;
 else if (p5_score <= 0.01542) twentile = 2;
 else if (p5_score <= 0.02500) twentile = 3;
 else if (p5_score <= 0.03280) twentile = 4;
 else if (p5_score <= 0.04108) twentile = 5;
 else if (p5_score <= 0.04851) twentile = 6;
 else if (p5_score <= 0.05467) twentile = 7;
 else if (p5_score <= 0.06031) twentile = 8;
 else if (p5_score <= 0.06764) twentile = 9;
 else if (p5_score <= 0.07515) twentile = 10;
 else if (p5_score <= 0.08255) twentile = 11;
 else if (p5_score <= 0.09187) twentile = 12;
 else if (p5_score <= 0.10131) twentile = 13;
 else if (p5_score <= 0.11072) twentile = 14;
 else if (p5_score <= 0.12268) twentile = 15;
 else if (p5_score <= 0.13652) twentile = 16;
 else if (p5_score <= 0.15577) twentile = 17;
 else if (p5_score <= 0.17960) twentile = 18;
 else if (p5_score <= 0.21736) twentile = 19;
 else twentile = 20;

 APR_N = apr * 100 / 30;
 Score_N = (p5_score + 0.02188119 ) / (0.679902158 + 0.02188119 );
 term_N = TOTAL_TERM / 120.0;
 loan_N = LOAN_AMOUNT / 3000000.0;

 pb = (P5_X[1] * pow(APR_N, P5_X[2]) * Score_N + (P5_X[3] + P5_X[4] * APR_N + P5_X[5] * pow(APR_N, P5_X[2])) * pow(Score_N, P5_X[6]))
      * (1 + term_N * (P5_X[7] + P5_X[8] * APR_N + P5_X[9] * Score_N))
      * (1 + loan_N * (P5_X[10] + P5_X[11] * APR_N + P5_X[12] * Score_N));

 return (pb);
}

//---------------------------------------------------------------------------
int PDACO::GeneratePdaco61Score(TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds;
 int card;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 try {
     card = get_scorecard(handler);
     switch (card) {
     	case 0: PDACO61P0Raw(handler);  // Scorecard P0
     	        break;
     	case 1:
     	        break;
     	case 2: PDACO61P2Raw(handler);  // Scorecard P2
     	        PDACO61P2Score();
     	        cap_amount = 300000.0;
     	        break;
     	case 3: PDACO61P3Raw(handler);  // Scorecard P3
     	        cap_amount = 700000.0;
     	        break;
     	case 4: PDACO61P4Raw(handler);  // Scorecard P4
     	        PDACO61P4Score();
     	        cap_amount = 500000.0;
     	        break;
     	case 5: PDACO61P5Raw(handler);  // Scorecard P5
     	        PDACO61P5Score();
     	        cap_amount = 1000000.0;
     	        break;
     }
/*
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
*/
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
    hostVars[0] = msn;
    hostVars[1] = input_time;
    handler->ExecSQLCmd(SQLCommands[Prepare_JCIC_Source_Tables], hostVars, 1);
    handler->ExecSQLCmd(SQLCommands[Prepare_JCIC_Data]);

 } catch (Exception &E) {
     throw;
 }
 return 0;
}
//---------------------------------------------------------------------------
double PDACO::getPdaco61Score()
{
 return score;
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
void PDACO::set_PS_code(int code)
{
 ps_code = code;
}

