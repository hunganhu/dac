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
    String case_sn;
    String idn;
    int dac_sn;
    int record_count;
    String application_date;
    String inquiry_date;
    String product_code;
    String project_source;
    String case_source;
    String recommender;
    String guarantor;
    double principal;
    String repayment;
    int periods;
    int grace_period;
    int num_int_period;
    String appropriation;
    String zip;
    String segment;
    int application_fee;
    int risk_mgmt_fee;
    int acct_mgmt_fee;
    int bt_fee;
    double max_apr;
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
    int    early_closing_period;         // 提早還款期間
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
