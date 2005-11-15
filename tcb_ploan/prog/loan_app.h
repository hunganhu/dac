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
    char case_sn[20];                 // 案件編號
    char aID[12];                     // 申請人身分證號
    char gID[12];                     // 保證人身分證號
    int product_type;               // 產品代號(1 為國民信貸; 2 為卡好借)
    int gender;                     // 0為女性; 1為男性
    char zip[4];                     // 三位郵遞區號
    int edu;                        // 教育程度 ，1 為研究所(含)以上 ; 2 為大學; 3 為專科;
                                    // 4  為高中(職); 5 為國中以下; 6 為其他
    int marriage_status;            // 婚姻狀況,1 為已婚; 2 為未婚; 3 為離婚; 4 為其他
    double cof;                     // 資金成本
    double roe;                     // 股東權益報酬
    int info_processing_cost;       // 每戶每月分攤台新銀行資訊室費用 (新台幣元)
    int operation_cost;             // 每戶每月分攤台新銀行分行作業及客服費用 (新台幣元)
    int hr_cost;                    // 每戶每月人事成本費用 (新台幣元)
    double commission;              // 佣金 (新台幣元)
    int other_initial_cost1;       // 其他開辦成本一
    int other_initial_cost2;       // 其他開辦成本二
    int other_initial_cost3;       // 其他開辦成本三

    int record_count;               // number of app_info records read
    int trial_count;                // number of Loan records read
    int code;                       // prescreen code
    /* loan information */
    double principal;               // 貸款金額(新台幣元)
    double int_rate;                // 年利率 (e.g. 18% 為 0.18)
    double teaser_rate;             // 優惠年利率 (e.g. 18% 為 0.18)
    int periods;                    // 貸款期數(月數)
    int teaser_period;              // 優惠期 (月數)
    int grace_period;               // 寬限期(月數)
    int application_fee;            // 開辦費 (新台幣元)
    int credit_checking_fee;        // 徵信查詢費(新台幣元)
    int risk_mgmt_fee;              // 每期風險管理費用(新台幣元)
    double max_apr;                 // 年利率 (e.g. 18% 為 0.18)

    /* Null indicator for each application feature */


  public:


    LoanApp (char *caseSN);
    ~LoanApp();
    int initial();
    int validate();
    char * Applicant_id();
    char * Guanrantor_id();
};


#endif  //LoanApp_H
