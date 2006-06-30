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
#ifndef fmNPVH
#define fmNPVH

#include <set>
#include <list>
#include <time.h>
#include <Math.hpp>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "pdacoSQL.h"
#include "functions.h"
#include "AdoHandle.h"
using namespace std;
#pragma package(smart_init)
// Constants used in the program.
const int TERM = 244;
#define min(a,b)  ((a) > (b)) ? (b): (a)
#define max(a,b)  ((a) > (b)) ? (a): (b)
const double ApprovedNPV = 2.0;  // (K)$2000
const double Allowance = 0.01;   // (K)$10

//---------------------------------------------------------------------------
class Loan {
  private:
    /* application information */
    String case_no;                 // 案件編號
    String app_date;		    // 案件輸入時間 YYYYMMDDHHMMSS
    int prod_type;		    // 產品類別 1:一般房貸, 2:增貸, 3:原餘額轉貸
    String app_id;		    // 申請人身份證字號姓名
    String app_name;		    // 申請人姓名
    String app_birthday;	    // 申請人生日 YYYMMDD
    int app_marriage;		    // 申請人婚姻 0:未婚 / 1:已婚 / 2:離婚
    int app_education;		    // 申請人學歷 0:研究所 / 1:大學 / 2:專科 / 3:高、國中(含以下)
    int app_income;		    // 申請人年收入
    int app_qualified;		    // 申請人符合承作條件 0: NO / 1:YES
    String cos_id;		    // 共同借款人身份證字號姓名
    String cos_name;		    // 共同借款人姓名
    String cos_birthday;	    // 共同借款人生日 YYYMMDD
    int cos_marriage;		    // 共同借款人婚姻 0:未婚 / 1:已婚 / 2:離婚
    int cos_education;		    // 共同借款人學歷 0:研究所 / 1:大學 / 2:專科 / 3:高、國中(含以下)
    int cos_income;		    // 共同借款人年收入
    int cos_qualified;		    // 共同借款人符合承作條件 0: NO / 1:YES		
    String gua_id;		    // 保證人身份證字號姓名  
    String gua_name;		    // 保證人姓名
    String gua_birthday;	    // 保證人生日 YYYMMDD
    int gua_marriage;		    // 保證人婚姻 0:未婚 / 1:已婚 / 2:離婚               
    int gua_education;		    // 保證人學歷 0:研究所 / 1:大學 / 2:專科 / 3:高、國中(含以下)
    int gua_income;		    // 保證人年收入
    int gua_qualified;		    // 保證人符合承作條件 0: NO / 1:YES-- l                      oan info
    int app_amt;		    // 申貸金額
    int periods;		    // 申貸期間
    float apr1;			    // 第一段利率
    int seg1;			    // 第一段期間
    float apr2;			    // 第二段利率
    int seg2;			    // 第二段期間
    float apr3;			    // 第三段利率
    int seg3;			    // 第三段期間
    int grace_period;		    // 寬限期
    int app_fee;		    // 開辦費
    String owner_id;		    // 所有人身分證字號
    String owner_name;		    // 所有人姓名
    String land_num;		    // 地號/建號
    int relationship;		    // 與申請人關係 0:本人 / 1:父母 / 2;配偶 / 3:子女 / 4:其他
    float gav;			    // 房屋毛值
    float nav;			    // 房屋淨值
    float existing_mortgage;	    // 房屋鑑價值
    int col_qualified;		    // 擔保品符合承作條件 0: NO / 1:YES
    int premium_col;		    // 擔保品區段良好且搭配優良建設公司 0: NO / 1:YES
    int monthly_payment;	    // 目前房貸月付金
    String inquiry_date;	    // JCIC 查詢日期 YYYYMMDD
    String branch;		    // 進件分行
    String emp_id;		    // 進件員工
    String auditor;		    // 徵審人員

    int record_count;               // number of app_info records read
    int code;                       // prescreen code

    /* Null indicator for each application feature */
    int prod_type_ind;
    int app_date_ind;
    int app_id_ind;
    int app_name_ind;
    int app_birthday_ind;
    int app_marriage_ind;
    int app_education_ind;
    int app_income_ind;
    int app_qualified_ind;
    int cos_id_ind;
    int cos_name_ind;
    int cos_birthday_ind;
    int cos_marriage_ind;
    int cos_education_ind;		 
    int cos_income_ind;		 
    int cos_qualified_ind;		 
    int gua_id_ind;		 
    int gua_name_ind;		 
    int gua_birthday_ind;
    int gua_marriage_ind;
    int gua_education_ind;		 
    int gua_income_ind;
    int gua_qualified_ind;		 
    int app_amt_ind;
    int periods_ind;			 
    int apr1_ind;
    int seg1_ind;			 
    int apr2_ind;			 
    int seg2_ind;
    int apr3_ind;
    int seg3_ind;
    int grace_period_ind;		 
    int app_fee_ind;		 
    int owner_id_ind;		 
    int owner_name_ind;		 
    int land_num_ind;
    int relationship_ind;		 
    int gav_ind;			 
    int nav_ind;			 
    int col_qualified_ind;		 
    int premium_col_ind;		 
    int monthly_payment_ind;
    int inquiry_date_ind;
    int branch_ind;
    int emp_id_ind;		 
    int auditor_ind;		 

    /* cash flow arrays to calculate NPV */
    String Message;
    double principal;
    double monthly_income;
    double monthly_debt;
    double max_loan_capacity;
    double weighted_apr;
    double pb_adjustment;
    double pdaco_score;
    int    pdaco_twentile;
    double fm_score;
    double fm_pb;
    double lowest_delta;
    double min_apr1;
    double min_apr2;
    double min_apr3;
    double optimal_line;
    double total_npv;
    double min_npv;              // npv close to hurdle amount
    double apr[TERM+4];
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
    double late_cost[TERM+4];
    double collection_cost[TERM+4];
    double working_capital[TERM+4];
    double credit_loss[TERM+4];

    /* functions to calculate cash flows */
    void npv_init();
    void set_apr(double delta_apr);
    void set_attrition(double pb);
    void set_amortize(int line);
    void set_annuity(int line);
    double set_interest_revenue();
    double set_setup_fee();
    double set_late_fee(double pb);
    double set_interest_cost();
    double calculate_commission();
    double setup_cost();
    double set_account_management_cost();
    double set_late_cost(double pb);
    double set_collection_cost();
    double set_working_capital();
    double set_credit_loss(double pb);

  public:

    class RiskEx {
       public:
          String message;
          int code;
          RiskEx (String msg, int err_code) {
             message = msg;
             code = err_code;
          }
    };
    class DataEx {
       public:
          String message;
          int code;
          DataEx (String msg, int err_code) {
             message = msg;
             code = err_code;
          }
    };

    Loan (char *case_no);
    ~Loan ();
    String error();
    int app_info_validate(char *case_no, TADOHandler *handler);
    double secured_pb();
    double calculate_npv(double delta_apr);
    int calculate_optimal_line(int loops, double npv[][3], TADOHandler *handler);
    int exist_applicant();
    int exist_coapplicant();
    int exist_guarantor();
    String Case_no ();
    char * Applicant();
    char * Cosigner();
    char * Guarantor();
    String Inquiry_date();
    int appIncome();
    int cosIncome();
    int guaIncome();
    void set_risk_score (double score);
    void set_monthly_income(double income);
    void set_monthly_debt(double debt);
    void set_risk_twentile (double score);
    void set_principal();
    void set_principal_reload();
    void set_pb_adjustment(double score);

    void set_lowest_rate (double delta_rate);
    void set_npv (double npv_value);
    double find_lowest_rate (double offset, double delta_r);
    double get_max_apr();
    double Min_APR1();
    double Min_APR2();
    double Min_APR3();
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
    int app_validate_test(char *case_no, TADOHandler *handler);
    int app_validate_bt(char *case_no, TADOHandler *handler);
};
//Economic model constants
const double LEVERAGE_RATIO = 0.9;     // 舉債比率（％）
const double COMMISSION_RATE = 0.0015; // 佣金比率（％）
const double FEE_COMMISSION_RATE = 0.1;// 開辦費佣金比率（％）
const double ALLOGRAPH_FEE = 0.0;      // 代書費
const double LAND_ADM_FEE = 0.0;       // 地政規費
const double DUEI_BAO_FEE = 2.25;      // 對保
const double SETUP_FEE = 3.0;          // 開辦費
const double COF = 0.01;               // 資金成本
const double ROE = 0.1;                // 股東權益報酬
const double ACCT_MGMT_COST = 0.08;    // Account Management Cost
const double LATE_PENALTY_RATIO = 1.2; // mutiple of interest rate applied
const double M2_3_EXPENSE = 5.25;      // NT$5.25K
const double M4P_EXPENSE = 22.5;       // NT$22500 per account per month
const double TIME_TO_RECOVER = 12;     // months
const double LEGAL_FEE_RATE = 0.008;   // percentage of balance, taken from Teresa's model, for collection


#endif
