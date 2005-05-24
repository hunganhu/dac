//---------------------------------------------------------------------------

#ifndef loanAppH
#define loanAppH

#include <set>
#include <list>
#include <time.h>
#include "AdoHandle.h"
using namespace std;
#pragma package(smart_init)
// Constants used in the program.
const int TERM = 120;

//---------------------------------------------------------------------------
class Rate {
  private:
    double int_rate;
    int    period;
    int    segment;
    int    int_rate_ind;
    int    period_ind;
  public:
    Rate (double interest, int month, int seg, int rate_ind, int month_ind):
     int_rate(interest), period(month), segment(seg), int_rate_ind(rate_ind), period_ind(month_ind) {}
    ~Rate() {};
    double get_rate() {return int_rate;}
    int    get_period() {return period;}
    int    get_segment() {return segment;}
    int    get_rate_ind() {return int_rate_ind;}
    int    get_period_ind() {return period_ind;}
};
//---------------------------------------------------------------------------
class Loan {
  private:
    /* application information */
    String case_sn;                 // 案件編號
    String idn;                     // 身份證號
    int dac_sn;                     // 序號
    int record_count;               // number of records read
    String application_date;        // 申請日期YYYYMMDD
    String inquiry_date;            // JCIC 查詢日期 YYYYMMDD
    String product_code;            // 性質別
    String project_source;          // 專案代碼
    String case_source;             // 案件來源
    String recommender;             // 引薦人(0: 無引薦人; 1: 有引薦人)
    String guarantor;               // 保人(0: 無保人; 1: 有保人)
    double principal;               // 貸款金額(新台幣元)
    String repayment;               // 還款方式(1; 一般法;2:本息法;3:本金法)
    int periods;                    // 貸款期數(月數)
    int grace_period;               // 寬限期(月數)
    int num_int_period;             // 利率時段個數，必須是1, 2 或3
    String appropriation;           // 撥款方式(1: 一次撥貸; 3: 分次撥貸; 4: 循環)，本次專案不包含3 和 4
    String zip;                     // 三位郵遞區號
    String segment;                 // 客層 "01"~"09", "99"
    int application_fee;            // 手續費+查詢費(新台幣元)
    int risk_mgmt_fee;              // 風險管理費(新台幣元)
    int acct_mgmt_fee;              // 帳戶管理費 (新台幣元)
    int bt_fee;                     // 代償費(新台幣元)
    int early_closing_period;       // 提早還款期間
    double max_apr;                 // max. of apr among each interest preiod
    TADODataSet *ds;

    /* Null indicator for each application feature */
    int application_date_ind;
    int inquiry_date_ind;
    int product_code_ind;
    int project_source_ind;
    int case_source_ind;
    int recommender_ind;
    int guarantor_ind;
    int principal_ind;
    int repayment_ind;
    int periods_ind;
    int grace_period_ind;
    int num_int_period_ind;
    int appropriation_ind;
    int zip_ind;
    int segment_ind;
    int application_fee_ind;
    int risk_mgmt_fee_ind;
    int acct_mgmt_fee_ind;
    int bt_fee_ind;
    int early_closing_period_ind;       // 提早還款期間 NULL indicator

    /* Prefilter and postfilter variables */
    int avail_flag;
    int jas002_defect;
    int krm021_hit;
    int krm023_hit;
    int fs044;
    int max_bucket;
    int cash_max_bucket;
    int cash_utilization;
    int ind001;

    /* cash flow arrays to calculate NPV */
    String Message;
    list<Rate> rateList;
    list<Rate>::iterator iter1;
    int district;
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
    double m1_recovery_ratio;            // Late fee recovery ratio - 4 days
    int    m1_avg_late_days;             // Average late days - 4 days
    double m1_penalty_rate;              // 違約率 - 4 days
    double m6_recovery_ratio;            // Late fee recovery ratio - 180+ days
    int    m6_avg_late_days;             // Average late days - 180+ days
    double m6_penalty_rate;              // 違約率 - 180+ days
//    int    early_closing_period;         // 提早還款期間 -- move to app_info
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
    double legal_exec_north;             //法務費用-北區（執行名義）
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
    void set_apr();
    void set_attrition();
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
    class RiskEx {
       public:
          String message;
          double pb;
          RiskEx (String msg, double code) {
             message = msg;
             pb = code;
          }
    };
    class DataEx {
       public:
          String message;
          DataEx (String msg) {
             message = msg;
          }
    };
    Loan (char * caseNo, char* pid, int seq, TADOHandler *handler);
    ~Loan ();
    void validate();
    String error();
    String get_application_date();
    void calculate_pd(TADOHandler *handler);
    void calculate_npv();
    double get_rscore ();
    double get_pd ();
    double get_npv ();
    double get_principal ();
    String get_segment ();
    void Init_Maintenance(TADOHandler *handler);
    double get_pd(char *idn, TADOHandler *handler);
    void postFilter();
};

#endif
