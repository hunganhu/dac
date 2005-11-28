/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: LoanApp.h
**
** SAMPLE:  header file for LoanApp.sqC
**
**         This is the header file for the LoanApp.sqC
**         file.
**
****************************************************************************/
#ifndef LoanApp_H
#define LoanApp_H

#include <string>

class LoanApp {
  private:
    /* application information */
    char  case_sn[20];			//案件編號
    char  aID[12];			//申請人身分證號
    char  gID[12];			//保證人身分證號
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
    double APR_3;			//利率(第三期
    int   Grace_period;			//寬限期
    int   Fee_1;			//開辦費(固定金額部分)
    double Fee_2;			//開辦費(百分比部分)

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

    /* NPV variables */
    double applicantPB;
    double guarantorPB;
    double applicantPB_adjust;
    int   ps_code_a;
    char  ps_msg_a[128];
    int   ps_code_g;
    char  ps_msg_g[128];
    int   Guarantor_pass;
  public:
    LoanApp (char *caseSN);
    ~LoanApp();
    int initial();
    int get_fin_info();
    char * Applicant_id();
    char * Guanrantor_id();
    int  get_product_type();
    double get_max_apr();
    int get_periods();
    int get_apply_amount();

    void set_applicant_pb(double pb);
    void set_guarantor_pb(double pb);
    void set_applicant_pb_adj(double pb);
    void set_guarantor_pass(int flag);
    void set_cutpoint(double cut);
    void set_ps_status_a(int code, char *msg);
    void set_ps_status_g(int code, char *msg);
    int  write_result_prescreen_failed();
    int  write_result_others();
    int  write_result_declined();
    int  write_result_approved();
};


#endif  //LoanApp_H
