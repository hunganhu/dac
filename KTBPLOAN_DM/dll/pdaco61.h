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
** Description: header file for PDACO V6.1 risk model
**
****************************************************************************/
//---------------------------------------------------------------------------

#ifndef pdaco61H
#define pdaco61H
//---------------------------------------------------------------------------
#include "AdoHandle.h"
#include "pdaco61SQL.h"

// classes and methods
class PDACO
{
  private:	// User declarations
    String       msn;
    String       idn;
    String       input_time;
    double        REQUEST_AMT;
    int          TOTAL_TERM;
    double       apr;
    int          period;
    int          app_fee;
    double        principal;
    double        LOAN_AMOUNT;
    double        FSC_AMOUNT;
    int          now;
    int          scorecard;
    int          monthly_income;
// prescreen variable
    double krm021_hit;
    double krm023_hit;
    double bam086_hit;
    double jas002_defect;
    double app_max_bucket;
    double fs044;
    double fs059_12m_1k;
    double delinquent_months;
    double card_force_stop;
    double cash_max_bucket;
    double cash_utilization;
    double ind001;
    double revolving_amt;
    int    debt_flag;
    int    note_flag;
    int    ps_code;
    String ps_msg;
    int segment;
// PDACO 6.1 risk model raw variables
    double GRAY2_FLAG;
    double APP_MAX_BUCKET;
    double APPROVED_AMT;
    double APR;
//    double BAM086_HIT;
    double CDEF_FLAG_1M;
    double FS002_3M_1K;
    double FS014_6M;
    double FS016C_9M;
    double FS016F_12M;
    double FS018_12M;
    double FS021_9M;
    double FS031;
    double FS031_1M;
    double FS036;
    double FS040;
    double FS044;
    double FS059_12M_1K;
    double FS059_3M_1K;
    double FS061_6M_1K;
    double FS073B_12M;
    double FS101_6M;
    double FS205_3M_1K;
    double FS302;
    double FS309;
    double FS314B;
    double FS334;
    double FS334B_1M;
    double FS546_9M;
    double MS001_12M_1K;
    double MS063;
    double MS074;
    double MS093;
    double MS094B;
    double MS105;
    double MS118_9M;
    double MS601;
    double MS604;
    double MS605;
    double MS606;
    double RS017;
    double STOP_CODE_FLAG;
    double WI001_9M;
    double WI003_9M;
    double WI004_9M;
// transformed variables
    double FS302_FG;
    double LU_FLAG;
    double MS604_R;
    double RS017_R_TRAN;
    double MS001_12M_1K_Q;
    double FS3036_T5;
    double FS031_TRAN4;
    double INT053_6_Q;
    double ln004_9m_q;
    double FS546_9M_TRAN;
    double ln004_9m;
    double FS016C_9M_T1;
    double WI003_9M_T;
    double monthly_payment;
    double ln001_9m;
    double ln001_9m_t2;
    double RS017_R_TRAN2;
    double MS074_T2;
    double MS074_T3;
    double FS205_3M_1K_Q_TRAN2;
    double ln003_9m_t;
    double FS031_1M_Q_TRAN2;
    double FS073B_12M_R;

// pdaco score
    double ms101;
    int   twentile;
    double cap_amount;
    double rscore;
    double pb;
// internal member functions
    int CreateWorkingTables(TADOHandler *handler);
    int DropWorkingTables(TADOHandler *handler);
    int PrepareJcicSourceTables(TADOHandler *handler);
    int PrepareJcicData(TADOHandler *handler);
    int get_scorecard(TADOHandler *handler);
    int PDACO61P0Raw(TADOHandler *handler);
    int PDACO61P1Raw(TADOHandler *handler);
    int PDACO61P2Raw(TADOHandler *handler);
    int PDACO61P3Raw(TADOHandler *handler);
    int PDACO61P4Raw(TADOHandler *handler);
    int PDACO61P5Raw(TADOHandler *handler);
    int WriteTraceRecord(TADOHandler *handler);
    double PDACO61P0Score();
    double PDACO61P1Score();
    double PDACO61P2Score();
    double PDACO61P3Score();
    double PDACO61P4Score();
    double PDACO61P5Score();
    double getCapAmount();

  public:
    PDACO (char *msn, char* input_time);
    ~PDACO ();
    int GeneratePdaco61Score(TADOHandler *handler);
    double recal_Pdaco61Pb(double loan_amount, double apr, int term);
    double getPbCap();
    double setLoanAmount();
    double upperLendableAmount();
    double getLoanAmount();
    double getRequestAmount();
    int GenerateScreenVars(TADOHandler *handler);
    int getPsCode();
    int getFscCap();
    double getUnsecuredBalance();
    double getDoubleCardBalance();
    double getPdaco61Score();
    double getScoreCard();
    double getApr();
    int getTerm();
    int getAppFee();
    double getPdaco61PB();
    double getPdaco61CapAmount();
    double monthly_debt();
    void   postScreen();
    String vam102_message(TADOHandler *handler);

    int input_npv_test(TADOHandler *handler);

};


const double P2_X[14] = {0.0, 0.123593000, 5.000000000, 0.195735000, 0.0, 6.235570000, 1.100000000, 2.052260000, -0.831907000, 0.788097000, 1.439830000, 4.125550000, -1.674490000, 0.000040250};
const double P4_X[13] = {0.0, 2.7866900, 6.1972900, 5.7875000, 17.9352000, 0.0103210, 1.7034700, -0.9714930, 1.2490100, 1.6176200, 2.8300900, 1.8354100, -1.7777300};
const double P5_X[14] = {0.0, 0.991190, 1.250800, 0.163415, 0.535809, -1.383160, 2.247320, 0.813351, 0.819610, -0.134986, 0.262729, 0.011975, -0.073090, 0.0};

#endif
