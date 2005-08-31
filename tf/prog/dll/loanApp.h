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
    String app_sn;                  // �ץ�s��
    String app_date;                // �ӽФ��YYYYMMDDHHmm
    String jcic_date;               // JCIC �d�ߤ��YYYYMMDD
    String ts_date;                 // Taishin internal credit data date YYYYMMDD
    String tsn;                     // trial seq #
    int product_type;               // ���~�N��(1 ������H�U; 2 ���d�n��)
    int gender;                     // 0���k��; 1���k��
    String zip;                     // �T��l���ϸ�
    int secretive;                  // 0���_; 1���O�A�O�_�K�a�H�A�Q�a�H�O�_���x�����U��
    int edu;                        // �Ш|�{�� �A1 ����s��(�t)�H�W ; 2 ���j��; 3 ���M��;
                                    // 4  ������(¾); 5 ���ꤤ�H�U; 6 ����L
    int marriage_status;            // �B�ê��p,1 ���w�B; 2 �����B; 3 �����B; 4 ����L
    int alien;                      // �O�_���~��H (0 ���_; 1���O)
    int age;                        // �~�֬O�_�j��56�� (0���_; 1���O)
    int cashcard_lock;              // �O�_���x�s�{���d�B�׳Q�� (0���_; 1���O)
    double cof;                     // �������
    double roe;                     // �ѪF�v�q���S
    double ts_tax_rate;             // �������
    double tf_tax_rate;             // �ѪF�v�q���S
    int info_processing_cost;       // �C��C����u�x�s�Ȧ��T�ǶO�� (�s�x����)
    int operation_cost;             // �C��C����u�x�s�Ȧ����@�~�ΫȪA�O�� (�s�x����)
    int hr_cost;                    // �C��C��H�Ʀ����O�� (�s�x����)

    int record_count;               // number of app_info records read
    int trial_count;                // number of Loan records read
    int code;                       // prescreen code
    /* loan information */
    double principal;               // �U�ڪ��B(�s�x����)
    double int_rate;                // �~�Q�v (e.g. 18% �� 0.18)
    double teaser_rate;             // �u�f�~�Q�v (e.g. 18% �� 0.18)
    int periods;                    // �U�ڴ���(���)
    int teaser_period;              // �u�f�� (���)
    int grace_period;               // �e����(���)
    int application_fee;            // �}��O (�s�x����)
    int credit_checking_fee;        // �x�H�d�߶O(�s�x����)
    int risk_mgmt_fee;              // �C�����I�޲z�O��(�s�x����)
    int risk_mgmt_fee_terms;        // ���I�޲z�O�Φ�������
    String sales_channel;           // �P��޹D
    int risk_level;                 // TF���I���� (1:�C 2:��)
//    TADODataSet *ds;
    double max_apr;                 // �~�Q�v (e.g. 18% �� 0.18)

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
