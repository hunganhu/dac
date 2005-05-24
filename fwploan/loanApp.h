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
    String case_sn;                 // �ץ�s��
    String idn;                     // �����Ҹ�
    int dac_sn;                     // �Ǹ�
    int record_count;               // number of records read
    String application_date;        // �ӽФ��YYYYMMDD
    String inquiry_date;            // JCIC �d�ߤ�� YYYYMMDD
    String product_code;            // �ʽ�O
    String project_source;          // �M�ץN�X
    String case_source;             // �ץ�ӷ�
    String recommender;             // ���ˤH(0: �L���ˤH; 1: �����ˤH)
    String guarantor;               // �O�H(0: �L�O�H; 1: ���O�H)
    double principal;               // �U�ڪ��B(�s�x����)
    String repayment;               // �ٴڤ覡(1; �@��k;2:�����k;3:�����k)
    int periods;                    // �U�ڴ���(���)
    int grace_period;               // �e����(���)
    int num_int_period;             // �Q�v�ɬq�ӼơA�����O1, 2 ��3
    String appropriation;           // ���ڤ覡(1: �@�����U; 3: �������U; 4: �`��)�A�����M�פ��]�t3 �M 4
    String zip;                     // �T��l���ϸ�
    String segment;                 // �ȼh "01"~"09", "99"
    int application_fee;            // ����O+�d�߶O(�s�x����)
    int risk_mgmt_fee;              // ���I�޲z�O(�s�x����)
    int acct_mgmt_fee;              // �b��޲z�O (�s�x����)
    int bt_fee;                     // �N�v�O(�s�x����)
    int early_closing_period;       // �����ٴڴ���
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
    int early_closing_period_ind;       // �����ٴڴ��� NULL indicator

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
    double m1_penalty_rate;              // �H���v - 4 days
    double m6_recovery_ratio;            // Late fee recovery ratio - 180+ days
    int    m6_avg_late_days;             // Average late days - 180+ days
    double m6_penalty_rate;              // �H���v - 180+ days
//    int    early_closing_period;         // �����ٴڴ��� -- move to app_info
    double early_closing_fee_pct;        // �����ٴڶO�v
    double early_closing_fee_collectable_ratio; //  �����ٴڶO�i�x������v
    double leverage_ratio;               //�|�Ť�v�]�H�^
    double cof;                          //��������]�H�^
    double roe;                          //�ѪF�v�q���S�]�H�^
    double query_fee;                    //�d�߶O�B���O
    double commission_ratio;             //25~50%of ����O
    double acquisition_data_cost;        //Cost of Jcic data acquisition  �����ٴڴ���
    double acct_mgmt_cost;               //�{�dú��
    double short_message_expense;        //²�T�O��
    double phone_expense_north;          //�q�ܶʦ��O��-�_��
    double phone_expense_south;          //�q�ܶʦ��O��-�n��
    double phone_expense_central;        //�q�ܶʦ��O��-����
    double legal_exec_north;             //�k�ȶO��-�_�ϡ]����W�q�^
    double legal_query_north;            //�k�ȶO��-�_�ϡ]�d�ߩұo&�]���^
    double legal_auction_north;          //�k�ȶO��-�_�ϡ]���^
    double legal_staff_north;            //�k�ȶO��-�_�ϡ]�k�ȤH���^
    double legal_exec_south;             //�k�ȶO��-�n�ϡ]����W�q�^
    double legal_query_south;            //�k�ȶO��-�n�ϡ]�d�ߩұo&�]���^
    double legal_auction_south;          //�k�ȶO��-�n�ϡ]���^
    double legal_staff_south;            //�k�ȶO��-�n�ϡ]�k�ȤH���^
    double legal_exec_central;           //�k�ȶO��-���ϡ]����W�q�^
    double legal_query_central;          //�k�ȶO��-���ϡ]�d�ߩұo&�]���^
    double legal_auction_central;        //�k�ȶO��-���ϡ]���^
    double legal_staff_central;          //�k�ȶO��-���ϡ]�k�ȤH���^
    double legal_detain_ratio_north;     //������&�j�����-�_�� (% of balance)
    double legal_detain_ratio_south;     //������&�j�����-�n�� (% of balance)
    double legal_detain_ratio_central;   //������&�j�����-���� (% of balance)
    double recovery_ratio;               //�a�b�^���ʤ���, (����l�� % (EAD) )
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
