/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: fmNPV.h
**
** Description: header file of NPV model for first mortgage
**
****************************************************************************/
//---------------------------------------------------------------------------
#ifndef ploanNPVH
#define ploanNPVH

#include <set>
#include <list>
#include <time.h>
#include <Math.hpp>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "pdaco61SQL.h"
#include "functions.h"
#include "AdoHandle.h"
#include "errors.h"
using namespace std;
#pragma package(smart_init)
// Constants used in the program.
const int TERM = 244;
#define min(a,b)  ((a) > (b)) ? (b): (a)
#define max(a,b)  ((a) > (b)) ? (a): (b)
const double ApprovedNPV = 2.0;  // (K)$2000
const double Allowance = 0.01;   // (K)$10
const double m1_to_m7_ratio = 5.0;
const int slope_turn_point = 9;
const int slope_end_point  = 25;
const double YearMonths = 12.0;
const double YearDays = 365.0;


//---------------------------------------------------------------------------
class Loan {
  private:
    /* application information */
    String msn;                 // 案件編號
    String input_time;		    // 案件輸入時間 YYYYMMDDHHMMSS
//    int prod_type;		    // 產品類別 1:一般房貸, 2:增貸, 3:原餘額轉貸
    double pd;
//    String app_id;		    // 申請人身份證字號姓名
//    String app_name;		    // 申請人姓名
//    String app_birthday;	    // 申請人生日 YYYMMDD
//    int app_marriage;		    // 申請人婚姻 0:未婚 / 1:已婚 / 2:離婚
//    int app_education;		    // 申請人學歷 0:研究所 / 1:大學 / 2:專科 / 3:高、國中(含以下)
    float app_income;		    // 申請人年收入
    int periods;		    // 申貸期間
    float max_apr;		    // 申貸利率
    float slope;		    // Slope ratio
    float apr1;			    // 第一段利率
    int seg1;			    // 第一段期間
    float apr2;			    // 第二段利率
    int seg2;			    // 第二段期間
    float apr3;			    // 第三段利率
    int seg3;			    // 第三段期間
    int grace_period;		    // 寬限期
    float app_fee;		    // 開辦費
//    float monthly_payment;	    // 目前房貸月付金
    String branch;		    // 進件分行
    String emp_id;		    // 進件員工
    String auditor;		    // 徵審人員

    int record_count;               // number of app_info records read
    int code;                       // prescreen code

    /* Null indicator for each application feature */
    /* cash flow arrays to calculate NPV */
    String Message;
    double principal;
    double monthly_income;
    double monthly_debt;
//    double max_loan_capacity;
//    double weighted_apr;
//    double pb_adjustment;
//    double pdaco_score;
//    int    pdaco_twentile;
//    double fm_score;
//    double fm_pb;
    double lowest_delta;
    double min_apr1;
    double min_apr2;
    double min_apr3;
    double optimal_line;
    double total_npv;
    double min_npv;              // npv close to hurdle amount
    double apr[TERM+4];
    double PD_attrition[TERM+4];
    double m1_attrition[TERM+4];
    double base_attrition[TERM+4];

    double open_attrition[TERM+4];
    double voluntary_attrition[TERM+4];
    double involuntary_attrition[TERM+4];
    double vol_attrition_open[TERM+4];
    double open_credit_fee[TERM+4];
    double risk_mgmt_revenue[TERM+4];
    double os_principal[TERM+4];
    double principal_repayment[TERM+4];
    double interest_repayment[TERM+4];
    double monthly_repayment[TERM+4];
    double interest_revenue[TERM+4];
    double late_fee[TERM+4];
    double interest_cost[TERM+4];
    double account_management_cost[TERM+4];
    double precollection_cost[TERM+4];
    double early_closing_fee[TERM+4];

    double late_cost[TERM+4];
    double collection_cost[TERM+4];
    double working_capital[TERM+4];
    double credit_loss[TERM+4];

    /* functions to calculate cash flows */
    void npv_init();
    void set_apr(double delta_apr);
    void set_attrition();
    void set_amortize(double line);
    void set_annuity(double line);
    double set_interest_revenue();
    double set_setup_fee();
    double set_late_fee();
    double set_early_closing_fee();

    double set_interest_cost();
//    double calculate_commission();
//    double setup_cost();
    double set_account_management_cost();
//    double set_late_cost(double pb);
    double set_collection_cost();
    double set_working_capital();
    double set_credit_loss();
    double set_precollection_cost();

  public:

    Loan (char *msn, unsigned int loan_amount, double apr, unsigned int terms,
            unsigned int application_fee, double pd);
    ~Loan ();
    String error();
    double calculate_npv(double delta_apr);
    double recal_npv(double delta_apr, double loan_amt, double new_pd);
    int calculate_optimal_line(int loops, double npv[][3], TADOHandler *handler);
    String Case_no();
    int appIncome();

    void set_lowest_rate (double delta_rate);
    void set_npv (double npv_value);
    double find_lowest_rate (double offset, double delta_r);
    double get_max_apr();
    double Min_APR1();
    double Min_APR2();
    double Min_APR3();
    int term1();
    int term2();
    int term3();
    double Monthly_Income();
    double Monthly_Debt();
    double Max_Loan_Capacity();
    double Weighted_APR();
    double Lowest_npv();
    double calculate_optimal_npv();

    double get_rscore ();
    double get_pd();
    int get_test_PB(char *idn, TADOHandler *handler);
    double get_npv ();
    double get_principal ();
    void Init_Maintenance(TADOHandler *handler);
    // functions for test only
//    int app_validate_test(char *case_no, TADOHandler *handler);
//    int app_validate_bt(char *case_no, TADOHandler *handler);
};
    //Economic model constants -  maintenance variables
    const int district = 3;                            // south
    const double m1_recovery_ratio = 0.95;            // Late fee recovery ratio - 4 days
    const int    m1_avg_late_days = 6;                // Average late days - 4 days
    const double m1_penalty_rate = 0.1;               // 違約率 - 4 days
    const double m6_recovery_ratio = 0.02;            // Late fee recovery ratio - 180+ days
    const int    m6_avg_late_days = 180;              // Average late days - 180+ days
    const double m6_penalty_rate = 0.2;               // 違約率 - 180+ days
    const int    early_closing_period = 57;           // 提早還款期間 -- move to app_info
    const double early_closing_fee_pct = 0.0;         // 提早還款費率
    const double early_closing_fee_collectable_ratio = 0.5; //  提早還款費可徵收之比率
    const double leverage_ratio = 0.9;                //舉債比率（％）
    const double COF = 0.01;                          //資金成本（％）
    const double ROE = 0.1;                           //股東權益報酬（％）
    const double commission_ratio = 0.0;              //25~50%of 手續費
    const double acct_mgmt_cost = 3.0;                //臨櫃繳款
    const double short_message_expense = 1.36;        //簡訊費用
    const double phone_expense_north = 39.0;          //電話催收費用-北區
    const double phone_expense_south = 50.0;          //電話催收費用-南區
    const double phone_expense_central = 28.0;        //電話催收費用-中區
    const double legal_exec_north = 963.0;            //法務費用-北區（執行名義）
    const double legal_query_north = 618.0;           //法務費用-北區（查詢所得&財產）
    const double legal_auction_north = 800.0;         //法務費用-北區（拍賣）
    const double legal_staff_north = 1500.0;          //法務費用-北區（法務人員）
    const double legal_exec_south = 325.0;            //法務費用-南區（執行名義）
    const double legal_query_south = 535.0;           //法務費用-南區（查詢所得&財產）
    const double legal_auction_south = 600.0;         //法務費用-南區（拍賣）
    const double legal_staff_south = 1500.0;          //法務費用-南區（法務人員）
    const double legal_exec_central = 1600.0;         //法務費用-中區（執行名義）
    const double legal_query_central = 700.0;         //法務費用-中區（查詢所得&財產）
    const double legal_auction_central = 1000.0;      //法務費用-中區（拍賣）
    const double legal_staff_central = 1500.0;        //法務費用-中區（法務人員）
    const double legal_detain_ratio_north = 0.0056;   //假扣押&強制執行-北區 (% of balance)
    const double legal_detain_ratio_south = 0.0032;   //假扣押&強制執行-南區 (% of balance)
    const double legal_detain_ratio_central = 0.0080; //假扣押&強制執行-中區 (% of balance)
    const double recovery_ratio = 0.25;               //壞帳回收百分比, (1-資金損失 % (EAD) )
    const double legal_action_period = 12;            // Legal action usually lasts 12 months before write-off
//---------------------------------------------------------------------------------------
const double PDterm_coeff[][2] = {{6.0, 1.00},    // 0: dummy record
                                  {7.0, 7.85},    // 1: {peak_month, divisor}
                                  {8.0, 7.58},    // 2: {peak_month, divisor}
                                  {9.0, 7.31},    // 3: {peak_month, divisor}
                                  {10.0, 7.04},   // 4: {peak_month, divisor}
                                  {11.0, 6.77},   // 5: {peak_month, divisor}
                                  {12.0, 6.5},    // 6: {peak_month, divisor}
                                  {12.0, 6.5}};   // 7: {peak_month, divisor}
const double PD_FACTOR = 0.77;

const double Yfit_coeff[][4] = {{0.2647, 0.0975, 0.0986,  0.000008},   //[3-year, 5%]  
                                {0.3356, 0.0858, 0.1020, -0.000010},   //[3-year, 5-9%]
                                {0.3715, 0.0963, 0.1094, -0.000004},   //[3-year, 9%+] 
                                {0.2719, 0.0682, 0.0894, -0.000010},   //[5-year, 5%]  
                                {0.2849, 0.0727, 0.0913, -0.000009},   //[5-year, 5-9%]
                                {0.3445, 0.0803, 0.0997, -0.000010},   //[5-year, 9%+] 
                                {0.2232, 0.0623, 0.0794, -0.000002},   //[7-year, 5%]  
                                {0.2508, 0.0691, 0.0841, -0.000008},   //[7-year, 5-9%]
                                {0.2852, 0.0657, 0.0858, -0.000007}};  //[7-year, 9%+]  


/*
attrition_table[catagory][cycle]:
   The value of attrition is depended on the catagory and cycle. The catagory is combination of loan term and APR.
The term can be 3 year (term <= 3), 5 year (3 < term <= 5), and 7 year (5 < term < 10).
The APR can be 5% (apr <= 5%), 5-9% (5% < apr <=9%), and 9%+ (apr > 9%).
The cycle can be 1 to 120 (month).
attrition_table[0]: [3-year, 5%]
attrition_table[1]: [3-year, 5-9%]
attrition_table[2]: [3-year, 9%+]
attrition_table[3]: [5-year, 5%]
attrition_table[4]: [5-year, 5-9%]
attrition_table[5]: [5-year, 9%+]
attrition_table[6]: [7-year, 5%]
attrition_table[7]: [7-year, 5-9%]
attrition_table[8]: [7-year, 9%+]

*/
const double Attrition_Table [][120] = {
//attrition_table[0]: [3-year, 5%]
{0.005556738140205, 0.005556738140205, 0.005556738140205, 0.005556738140205, 0.005556738140205, 0.005556738140205,
 0.005556738140205, 0.005556738140205, 0.005556738140205, 0.005556738140205, 0.005556738140205, 0.027100271002710,
 0.032520325203252, 0.024390243902439, 0.024390243902439, 0.019512195121951, 0.019512195121951, 0.019512195121951,
 0.019512195121951, 0.019512195121951, 0.019512195121951, 0.019512195121951, 0.019512195121951, 0.019512195121951,
 0.019512195121951, 0.019512195121951, 0.019512195121951, 0.019512195121951, 0.019512195121951, 0.019512195121951,
 0.019512195121951, 0.019512195121951, 0.019512195121951, 0.019512195121951, 0.019512195121951, 0.019512195121951,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
//attrition_table[1]: [3-year, 5-9%]
{0.006697035361594, 0.006697035361594, 0.006697035361594, 0.006697035361594, 0.006697035361594, 0.006697035361594,
 0.006697035361594, 0.006697035361594, 0.006697035361594, 0.006697035361594, 0.006697035361594, 0.030581039755352,
 0.036697247706422, 0.027522935779817, 0.027522935779817, 0.022018348623853, 0.022018348623853, 0.022018348623853,
 0.022018348623853, 0.022018348623853, 0.022018348623853, 0.022018348623853, 0.022018348623853, 0.022018348623853,
 0.022018348623853, 0.022018348623853, 0.022018348623853, 0.022018348623853, 0.022018348623853, 0.022018348623853,
 0.022018348623853, 0.022018348623853, 0.022018348623853, 0.022018348623853, 0.022018348623853, 0.022018348623853,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
//attrition_table[2]: [3-year, 9%+]
{0.007653150338726, 0.007653150338726, 0.007653150338726, 0.007653150338726, 0.007653150338726, 0.007653150338726,
 0.007653150338726, 0.007653150338726, 0.007653150338726, 0.007653150338726, 0.007653150338726, 0.046948356807512,
 0.056338028169014, 0.042253521126761, 0.042253521126761, 0.033802816901408, 0.033802816901408, 0.033802816901408,
 0.033802816901408, 0.033802816901408, 0.033802816901408, 0.033802816901408, 0.033802816901408, 0.033802816901408,
 0.033802816901408, 0.033802816901408, 0.033802816901408, 0.033802816901408, 0.033802816901408, 0.033802816901408,
 0.033802816901408, 0.033802816901408, 0.033802816901408, 0.033802816901408, 0.033802816901408, 0.033802816901408,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
//attrition_table[3]: [5-year, 5%]
{0.002314426997546, 0.002314426997546, 0.002314426997546, 0.002314426997546, 0.002314426997546, 0.002314426997546,
 0.002314426997546, 0.002314426997546, 0.002314426997546, 0.002314426997546, 0.002314426997546, 0.021929824561404,
 0.026315789473684, 0.019736842105263, 0.019736842105263, 0.015789473684211, 0.015789473684211, 0.015789473684211,
 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211,
 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211,
 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211,
 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211,
 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211,
 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211,
 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211, 0.015789473684211,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
//attrition_table[4]: [5-year, 5-9%]
{0.005052044213783, 0.005052044213783, 0.005052044213783, 0.005052044213783, 0.005052044213783, 0.005052044213783,
 0.005052044213783, 0.005052044213783, 0.005052044213783, 0.005052044213783, 0.005052044213783, 0.020661157024793,
 0.024793388429752, 0.018595041322314, 0.018595041322314, 0.014876033057851, 0.014876033057851, 0.014876033057851,
 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851,
 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851,
 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851,
 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851,
 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851,
 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851,
 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851, 0.014876033057851,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
//attrition_table[5]: [5-year, 9%+]
{0.004985894370963, 0.004985894370963, 0.004985894370963, 0.004985894370963, 0.004985894370963, 0.004985894370963,
 0.004985894370963, 0.004985894370963, 0.004985894370963, 0.004985894370963, 0.004985894370963, 0.043103448275862,
 0.051724137931035, 0.038793103448276, 0.038793103448276, 0.031034482758621, 0.031034482758621, 0.031034482758621,
 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621,
 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621,
 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621,
 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621,
 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621,
 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621,
 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621, 0.031034482758621,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
//attrition_table[6]: [7-year, 5%]
{0.002623025553193, 0.002623025553193, 0.002623025553193, 0.002623025553193, 0.002623025553193, 0.002623025553193,
 0.002623025553193, 0.002623025553193, 0.002623025553193, 0.002623025553193, 0.002623025553193, 0.009128847157016,
 0.010954616588419, 0.008215962441315, 0.008215962441315, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052,
 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052, 0.006572769953052},

//attrition_table[7]: [7-year, 5-9%]
{0.003908973586443, 0.003908973586443, 0.003908973586443, 0.003908973586443, 0.003908973586443, 0.003908973586443,
 0.003908973586443, 0.003908973586443, 0.003908973586443, 0.003908973586443, 0.003908973586443, 0.019225081336883,
 0.023070097604259, 0.017302573203194, 0.017302573203194, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556,
 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556, 0.013842058562556},

//attrition_table[8]: [7-year, 9%+]
{0.005513949643733, 0.005513949643733, 0.005513949643733, 0.005513949643733, 0.005513949643733, 0.005513949643733,
 0.005513949643733, 0.005513949643733, 0.005513949643733, 0.005513949643733, 0.005513949643733, 0.019736842105263,
 0.023684210526316, 0.017763157894737, 0.017763157894737, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790,
 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790, 0.014210526315790}
};


#endif
