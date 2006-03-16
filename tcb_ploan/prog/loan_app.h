/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: LoanApp.h
**
** SAMPLE:  header file for LoanApp.sqC
**
****************************************************************************/
#ifndef LoanApp_H
#define LoanApp_H

#include <string>

enum DecisionCodes {
	TYPE_I,
	TYPE_II,
	TYPE_III,
	TYPE_IV,
	TYPE_V
};
const int TERM = 120;
const double m1_to_m7_ratio = 5.0;
const double YearMonths = 12.0;
const double YearDays = 365.0;
const double ApprovedNPV = 2000.0;
const double Allowance = 1.0;

class LoanApp {
  private:
    /* application information */
    char  case_sn[20];			//案件編號
    char  aID[12];			//申請人身分證號
    char  gID[12];			//保證人身分證號
    char  System_date[8];		//系統日期
    char  aName[31];			//申請人姓名
    int   Loan_purpose;			//資金用途
    int   Education;			//教育程度
    char  Birthday[8];			//申請人生日
    int   Marriage_status;		//申請人婚姻狀態
    int   Annual_Income;		//申請人年所得收入
    int   Home_ownership;		//申請人現住房屋狀況
    int   Year_at_current_address;	//申請人現址已居住年數
    int   Year_at_current_job;		//申請人現職服務年資
    int   Car_year;			//申請人車籍資料(年份)
    int   Product_type_code;		//產品種類碼
    int   App_amt;			//申貸金額
    int   Term_1;			//期數(第一期)
    int   Term_2;			//期數(第二期)
    int   Term_3;			//期數(第三期)
    double APR_1;			//利率(第一期)
    double APR_2;			//利率(第二期)
    double APR_3;			//利率(第三期)
    int   Grace_period;			//寬限期
    int   Fee_1;			//開辦費(固定金額部分)
    double Fee_2;			//開辦費(百分比部分)
    double Lowest_rate_1;               //最低可承做利率(第一期)
    double Lowest_rate_2;               //最低可承做利率(第二期)
    double Lowest_rate_3;               //最低可承做利率(第三期)

    /* Financial Information*/
    double Fund_deposit_pct;		//資金來源來自存款百分比
    double Fund_bank_pct;		//資金來源來自同業拆款百分比
    double Fund_self_pct;		//資金來源自有百分比
    double Fund_free_pct;		//資金來源無息百分比
    double COF_deposit;			//存款平均利率
    double COF_bank;			//同業拆款利率
    double ROE;				//股東權益報酬率
    double Commission;			//業務獎金

    int record_count;               // number of app_info records read
    int trial_count;                // number of Loan records read
    int code;                       // prescreen code
    /* loan information */
    double principal;               // 貸款金額(新台幣元)
    double int_rate;                // 年利率 (e.g. 18% 為 0.18)
    int periods;                    // 貸款期數(月數)
    int application_fee;            // 開辦費 (新台幣元)
    int risk_mgmt_fee;              // 每期風險管理費用(新台幣元)
    double max_apr;                 // 年利率 (e.g. 18% 為 0.18)
    double cutpoint;                // PB cut point

    char System_date_final[8];		// 結案輸入系統日期yyymmdd
    char System_time_final[7];		// 結案輸入系統時間hhmiss
    int Approval_final;			// 最終准駁/撥款決定
    char Account_No[21];		// 撥款帳號
    char Open_date[8];			// 撥款日期
    int Loan_amt;			// 撥款金額
    double Loan_APR;			// 撥款利率
    int Loan_term;			// 最終貸款期數
    int Fee_1_final;			// 實收開辦費(固定金額部分)
    double Fee_2_final;			// 實收開辦費(百分比部分)

    /* NPV variables */
    double applicantPB;			// 
    double guarantorPB;			// 
    double applicantPB_adjust;		// 
    int   ps_code_a;			// 
    char  ps_msg_a[128];		// 
    int   ps_code_g;			// 
    char  ps_msg_g[128];		// 
    int   Guarantor_pass;		// 

    /* cash flow arrays to calculate NPV */
    double pd;
    double rscore;
    double total_npv;
    double apr[TERM+4];
    double open_attrition[TERM+4];
    double PD_attrition[TERM+4];
    double voluntary_attrition[TERM+4];
    double involuntary_attrition[TERM+4];
    double m1_attrition[TERM+4];
    double base_attrition[TERM+4];
    double os_principal[TERM+4];
    double principal_repayment[TERM+4];
    double interest_repayment[TERM+4];
    double interest_revenue[TERM+4];
    double late_fee[TERM+4];
    double early_closing_fee[TERM+4];
    double interest_cost[TERM+4];
    double account_management_cost[TERM+4];
    double precollection_cost[TERM+4];
    double collection_cost[TERM+4];
    double working_capital[TERM+4];
    double credit_loss[TERM+4];

    /* maintenance variables*/
    int district;
    double m1_recovery_ratio;            // Late fee recovery ratio - 4 days
    int    m1_avg_late_days;             // Average late days - 4 days
    double m1_penalty_rate;              // 違約率 - 4 days
    double m6_recovery_ratio;            // Late fee recovery ratio - 180+ days
    int    m6_avg_late_days;             // Average late days - 180+ days
    double m6_penalty_rate;              // 違約率 - 180+ days
    int    early_closing_period;         // 提早還款期間 -- move to app_info
    double early_closing_fee_pct;        // 提早還款費率
    double early_closing_fee_collectable_ratio; //  提早還款費可徵收之比率
    double leverage_ratio;               //舉債比率（％）
    double cof;                          //資金成本（％）
    double roe;                          //股東權益報酬（％）
    double query_fee;                    //查詢費、文件費
    double commission_ratio;             //25~50%of 手續費
    double acquisition_data_cost;        //Cost of Jcic data acquisition  提早還款期間
    double acct_mgmt_cost;               //臨櫃繳款
    double short_message_expense;        //簡訊費用
    double phone_expense_north;          //電話催收費用-北區
    double phone_expense_south;          //電話催收費用-南區
    double phone_expense_central;        //電話催收費用-中區
    double legal_exec_north;         //法務費用-北區（執行名義）支付命令 + 假扣押裁定 + 1.1% * 申貸金額
    double legal_query_north;            //法務費用-北區（查詢所得&財產）
    double legal_auction_north;          //法務費用-北區（拍賣）
    double legal_staff_north;            //法務費用-北區（法務人員）
    double legal_exec_south;             //法務費用-南區（執行名義）
    double legal_query_south;            //法務費用-南區（查詢所得&財產）
    double legal_auction_south;          //法務費用-南區（拍賣）
    double legal_staff_south;            //法務費用-南區（法務人員）
    double legal_exec_central;           //法務費用-中區（執行名義）
    double legal_query_central;          //法務費用-中區（查詢所得&財產）
    double legal_auction_central;        //法務費用-中區（拍賣）
    double legal_staff_central;          //法務費用-中區（法務人員）
    double legal_detain_ratio_north;     //假扣押&強制執行-北區 (% of balance)
    double legal_detain_ratio_south;     //假扣押&強制執行-南區 (% of balance)
    double legal_detain_ratio_central;   //假扣押&強制執行-中區 (% of balance)
    double recovery_ratio;               //壞帳回收百分比, (資金損失 % (EAD) )
    double legal_action_period;          // Legal action usually lasts 12 months before write-off

    /* functions to calculate cash flows */
    void npv_init();
    void set_apr(double delta_apr);
    void set_attrition(double delta_apr);
    void set_amortize();
    void set_annuity();
    double set_interest_revenue();
    double set_late_fee();
    double set_early_closing_fee();
    double set_interest_cost();
    double set_account_management_cost();
    double set_precollection_cost();
    double set_collection_cost();
    double set_working_capital();
    double set_credit_loss();

  public:
    LoanApp (char *caseSN);
    ~LoanApp();
    int initial();
    int get_fin_info();
    int get_loan_info();
    char * Applicant_id();
    char * Guarantor_id();
    int  get_product_type();
    double get_max_apr();
    int get_periods();
    int get_apply_amount();
    char * get_birthday();
    char * get_sysdate();
    double get_npv();

    void set_applicant_pb(double pb);
    void set_guarantor_pb(double pb);
    void set_applicant_pb_adj(double pb);
    void set_guarantor_pass(int flag);
    void set_cutpoint(double cut);
    void set_ps_status_a(int code, char *msg);
    void set_ps_status_g(int code, char *msg);
    void set_npv(double npv_value);
    int  update_final_npv();
    int  write_result_prescreen_failed();
//    int  write_result_others(int decision);
//    int  write_result_declined(int decision);
//    int  write_result_approved(int decision);
    int  write_result(int decision, int npv_value, int unsecured_amt);
    int  write_npv(double npv);
    int  write_delta_rate(double lowest_delta, double npv);
    double calculate_npv(double delta_apr);
    double find_lowest_rate (double offset, double delta_r);
    void set_lowest_rate (double delta_rate);

};


#endif  //LoanApp_H
