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
    float        REQUEST_AMT;
    float        TOTAL_TERM;
    float        apr;
    float        period;
    float        principal;
    float        LOAN_AMOUNT;
    int          now;
    int          scorecard;
// prescreen variable
    int krm021_hit;
    int krm023_hit;
    int bam086_hit;
    int jas002_defect;
    int app_max_bucket;
    int fs044;
    int fs059_1k_12m;
    int delinquent_months;
    int card_force_stop;
    int cash_max_bucket;
    int cash_utilization;
    int ind001;
    float revolving_amt;
    int ps_code;
    int segment;
// PDACO 6.1 risk model raw variables
    float APP_MAX_BUCKET;
    float APPROVED_AMT;
    float APR;
    float BAM086_HIT;
    float CDEF_FLAG_1M;
    float FS002_3M_1K;
    float FS014_6M;
    float FS016C_9M;
    float FS016F_12M;
    float FS018_12M;
    float FS021_9M;
    float FS031;
    float FS031_1M;
    float FS036;
    float FS040;
    float FS044;
    float FS059_12M_1K;
    float FS059_3M_1K;
    float FS061_6M_1K;
    float FS073B_12M;
    float FS101_6M;
    float FS205_3M_1K;
    float FS302;
    float FS309;
    float FS314B;
    float FS334;
    float FS334B_1M;
    float FS546_9M;
    float MS001_12M_1K;
    float MS063;
    float MS074;
    float MS093;
    float MS094B;
    float MS105;
    float MS118_9M;
    float MS601;
    float MS605;
    float MS606;
    float RS017;
    float STOP_CODE_FLAG;
    float WI001_9M;
    float WI003_9M;
    float WI004_9M;
// transformed variables
    float FS302_FG;
    float LU_FLAG;
    float MS604_R;
    float RS017_R_TRAN;
    float MS001_12M_1K_Q;
    float FS3036_T5;
    float FS031_TRAN4;
    float INT053_6_Q;
    float ln004_9m_q;
    float FS546_9M_TRAN;
    float ln004_9m;
    float p4_score;
    float FS016C_9M_T1;
    float WI003_9M_T;
    float monthly_payment;
    float ln001_9m;
    float ln001_9m_t2;
    float p2_score;
    float RS017_R_TRAN2;
    float MS074_T3;
    float FS205_3M_1K_Q_TRAN2;
    float ln003_9m_t;
    float FS031_1M_Q_TRAN2;
    float FS073B_12M_R;

// pdaco score
    float ms101;
    int   twentile;
    double cap_amount;
    double score;
    double pb;
// internal member functions
    int CreateWorkingTables(TADOHandler *handler);
    int DropWorkingTables(TADOHandler *handler);
    int PrepareJcicSourceTables(TADOHandler *handler);
    int PrepareJcicData(TADOHandler *handler);
    int get_scorecard(TADOHandler *handler);
    int PDACO61P0Raw(TADOHandler *handler);
    int PDACO61P2Raw(TADOHandler *handler);
    int PDACO61P3Raw(TADOHandler *handler);
    int PDACO61P4Raw(TADOHandler *handler);
    int PDACO61P5Raw(TADOHandler *handler);
    double PDACO61P2Score();
    double PDACO61P4Score();
    double PDACO61P5Score();

  public:
    PDACO (char *msn, char* input_time);
    ~PDACO ();
    int GeneratePdaco61Score(TADOHandler *handler);
    int GenerateScreenVars(TADOHandler *handler);
    void set_PS_code(int code);
    double getPdaco61Score();
    double getPdaco61PB();
    double getPdaco61CapAmount();
    double monthly_debt();
};


const double P2_X[14] = {0.0, 0.123593000, 5.000000000, 0.195735000, 0.0, 6.235570000, 1.100000000, 2.052260000, -0.831907000, 0.788097000, 1.439830000, 4.125550000, -1.674490000, 0.000040250};
const double P4_X[12] = {0.0, 4.670740, 4.300610, 7.000060, 4.029140, 1.432320, 1.635670, -0.579372, 2.068420, 1.608720, 5.212910, 0.417489};
const double P5_X[14] = {0.0, 0.991190, 1.250800, 0.163415, 0.535809, -1.383160, 2.247320, 0.813351, 0.819610, -0.134986, 0.262729, 0.011975, -0.073090, 0.0};

#endif
