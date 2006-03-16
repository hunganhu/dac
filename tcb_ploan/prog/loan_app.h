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
    char  case_sn[20];			//�ץ�s��
    char  aID[12];			//�ӽФH�����Ҹ�
    char  gID[12];			//�O�ҤH�����Ҹ�
    char  System_date[8];		//�t�Τ��
    char  aName[31];			//�ӽФH�m�W
    int   Loan_purpose;			//����γ~
    int   Education;			//�Ш|�{��
    char  Birthday[8];			//�ӽФH�ͤ�
    int   Marriage_status;		//�ӽФH�B�ê��A
    int   Annual_Income;		//�ӽФH�~�ұo���J
    int   Home_ownership;		//�ӽФH�{��ЫΪ��p
    int   Year_at_current_address;	//�ӽФH�{�}�w�~��~��
    int   Year_at_current_job;		//�ӽФH�{¾�A�Ȧ~��
    int   Car_year;			//�ӽФH���y���(�~��)
    int   Product_type_code;		//���~�����X
    int   App_amt;			//�ӶU���B
    int   Term_1;			//����(�Ĥ@��)
    int   Term_2;			//����(�ĤG��)
    int   Term_3;			//����(�ĤT��)
    double APR_1;			//�Q�v(�Ĥ@��)
    double APR_2;			//�Q�v(�ĤG��)
    double APR_3;			//�Q�v(�ĤT��)
    int   Grace_period;			//�e����
    int   Fee_1;			//�}��O(�T�w���B����)
    double Fee_2;			//�}��O(�ʤ��񳡤�)
    double Lowest_rate_1;               //�̧C�i�Ӱ��Q�v(�Ĥ@��)
    double Lowest_rate_2;               //�̧C�i�Ӱ��Q�v(�ĤG��)
    double Lowest_rate_3;               //�̧C�i�Ӱ��Q�v(�ĤT��)

    /* Financial Information*/
    double Fund_deposit_pct;		//����ӷ��Ӧۦs�ڦʤ���
    double Fund_bank_pct;		//����ӷ��ӦۦP�~��ڦʤ���
    double Fund_self_pct;		//����ӷ��ۦ��ʤ���
    double Fund_free_pct;		//����ӷ��L���ʤ���
    double COF_deposit;			//�s�ڥ����Q�v
    double COF_bank;			//�P�~��ڧQ�v
    double ROE;				//�ѪF�v�q���S�v
    double Commission;			//�~�ȼ���

    int record_count;               // number of app_info records read
    int trial_count;                // number of Loan records read
    int code;                       // prescreen code
    /* loan information */
    double principal;               // �U�ڪ��B(�s�x����)
    double int_rate;                // �~�Q�v (e.g. 18% �� 0.18)
    int periods;                    // �U�ڴ���(���)
    int application_fee;            // �}��O (�s�x����)
    int risk_mgmt_fee;              // �C�����I�޲z�O��(�s�x����)
    double max_apr;                 // �~�Q�v (e.g. 18% �� 0.18)
    double cutpoint;                // PB cut point

    char System_date_final[8];		// ���׿�J�t�Τ��yyymmdd
    char System_time_final[7];		// ���׿�J�t�ήɶ�hhmiss
    int Approval_final;			// �̲׭��/���ڨM�w
    char Account_No[21];		// ���ڱb��
    char Open_date[8];			// ���ڤ��
    int Loan_amt;			// ���ڪ��B
    double Loan_APR;			// ���ڧQ�v
    int Loan_term;			// �̲׶U�ڴ���
    int Fee_1_final;			// �ꦬ�}��O(�T�w���B����)
    double Fee_2_final;			// �ꦬ�}��O(�ʤ��񳡤�)

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
    double m1_penalty_rate;              // �H���v - 4 days
    double m6_recovery_ratio;            // Late fee recovery ratio - 180+ days
    int    m6_avg_late_days;             // Average late days - 180+ days
    double m6_penalty_rate;              // �H���v - 180+ days
    int    early_closing_period;         // �����ٴڴ��� -- move to app_info
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
    double legal_exec_north;         //�k�ȶO��-�_�ϡ]����W�q�^��I�R�O + ��������w + 1.1% * �ӶU���B
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
