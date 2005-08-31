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
const int TERM = 60;

//---------------------------------------------------------------------------
class Loan {
  private:
    /* application information */
    String app_sn;                  // 案件編號
    String app_date;                // 申請日期YYYYMMDDHHmm
    String jcic_date;               // JCIC 查詢日期YYYYMMDD
    String ts_date;                 // Taishin internal credit data date YYYYMMDD
    String tsn;                     // trial seq #
    int product_type;               // 產品代號(1 為國民信貸; 2 為卡好借)
    int gender;                     // 0為女性; 1為男性
    String zip;                     // 三位郵遞區號
    int secretive;                  // 0為否; 1為是，是否密家人，被家人是否知悉此筆貸款
    int edu;                        // 教育程度 ，1 為研究所(含)以上 ; 2 為大學; 3 為專科;
                                    // 4  為高中(職); 5 為國中以下; 6 為其他
    int marriage_status;            // 婚姻狀況,1 為已婚; 2 為未婚; 3 為離婚; 4 為其他
    int alien;                      // 是否為外國人 (0 為否; 1為是)
    int age;                        // 年齡是否大於56歲 (0為否; 1為是)
    int cashcard_lock;              // 是否為台新現金卡額度被鎖 (0為否; 1為是)
    double cof;                     // 資金成本
    double roe;                     // 股東權益報酬
    double ts_tax_rate;             // 資金成本
    double tf_tax_rate;             // 股東權益報酬
    int info_processing_cost;       // 每戶每月分攤台新銀行資訊室費用 (新台幣元)
    int operation_cost;             // 每戶每月分攤台新銀行分行作業及客服費用 (新台幣元)
    int hr_cost;                    // 每戶每月人事成本費用 (新台幣元)

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
    int risk_mgmt_fee_terms;        // 風險管理費用收取期數
    String sales_channel;           // 銷售管道
    int risk_level;                 // TF風險分級 (1:低 2:高)
//    TADODataSet *ds;
    double max_apr;                 // 年利率 (e.g. 18% 為 0.18)

    /* Null indicator for each application feature */
    int product_type_ind;
    int gender_ind;
    int zip_ind;
    int secretive_ind;
    int edu_ind;
    int marriage_status_ind;
    int alien_ind;
    int age_ind;
    int cashcard_lock_ind;
    int cof_ind;
    int roe_ind;
    int ts_tax_rate_ind;
    int tf_tax_rate_ind;
    int info_processing_cost_ind;
    int operation_cost_ind;
    int hr_cost_ind;
    int sales_channel_ind;
    int risk_level_ind;

    int principal_ind;
    int int_rate_ind;
    int teaser_rate_ind;
    int periods_ind;
    int teaser_period_ind;
    int grace_period_ind;
    int application_fee_ind;
    int credit_checking_fee_ind;
    int risk_mgmt_fee_ind;
    int risk_mgmt_fee_terms_ind;

    /* Prefilter and postfilter variables */
/*
    int jas002_defect;
    int krm001_hit;
    int krm023_hit;
    int bam085_hit;
    int fs044;
    int app_max_bucket;
    int cash_max_bucket;
    int delinquent_months;
    int ind001;
    int ms080;
*/
    /* cash flow arrays to calculate NPV */
    String Message;
    int district;
    double pd;
    double optimal_line;
    double rscore;
    double total_npv;
    double apr[TERM+4];
    double open_attrition[TERM+4];
    double voluntary_attrition[TERM+4];
    double involuntary_attrition[TERM+4];
    double bad_per_open[TERM+4];
//    double m1_attrition[TERM+4];
//    double base_attrition[TERM+4];
    double open_credit_fee[TERM+4];
    double risk_mgmt_revenue[TERM+4];
    double taishin_tax[TERM+4];
    double tf_tax[TERM+4];
    double os_principal[TERM+4];
    double principal_repayment[TERM+4];
    double interest_repayment[TERM+4];
    double interest_revenue[TERM+4];
    double late_fee[TERM+4];
//    double early_closing_fee[TERM+4];
    double interest_cost[TERM+4];
    double account_management_cost[TERM+4];
//    double precollection_cost[TERM+4];
    double collection_cost[TERM+4];
//    double working_capital[TERM+4];
    double credit_loss[TERM+4];

    /*maintenance variables*/

    /* functions to calculate cash flows */
    void npv_init();
    void set_apr();
    void set_attrition();
    void set_amortize(int line);
    void set_annuity(int line);
    double set_interest_revenue();
    double set_open_credit_revenue();
    double set_risk_mgmt_revenue();
    double set_late_fee();
    double set_taishin_tax();
    double set_tf_tax();
    double set_interest_cost();
    double calculate_commission();
    double set_account_management_cost();
    double set_precollection_cost();
    double set_collection_cost();
    double set_working_capital();
    double set_credit_loss();
    double get_GX_adjustment(double annual_pb);
    double get_KHJ_adjustment(double annual_pb);
    double cal_GXa2_pb(int lending_amt, double apr, int period, double risk_score,
                    int ms082, double wi001_12m);

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
    Loan (char *appSN, char *appDate);
    Loan (char *appSN, char *appDate, char *tsDate, char *jcicDate, char *tsn);
    ~Loan ();
    void app_info_validate(char * appNo, char* appDate, TADOHandler *handler);
    void loan_validate(char * appNo, char *tsn, TADOHandler *handler);
    String error();
    void prescreen(char *inquiry_date, TADOHandler *handler);
    void calculate_rscore(TADOHandler *handler);
    void calculate_pd(TADOHandler *handler);
    double calculate_npv(int line);
    int  calculate_optimal_line(int start_amount, int end_amount, int increment,
                                TADOHandler *handler);
    int  get_product_type();
    int  get_code();
    double get_rscore ();
    double get_pd();
    double get_test_PB(char *idn, TADOHandler *handler);
    double get_npv ();
    double get_principal ();
    void Init_Maintenance(TADOHandler *handler);
//    double get_pd(char *idn, TADOHandler *handler);
    void postFilter();
};

#endif
