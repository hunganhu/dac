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
    double APR_3;			//�Q�v(�ĤT��
    int   Grace_period;			//�e����
    int   Fee_1;			//�}��O(�T�w���B����)
    double Fee_2;			//�}��O(�ʤ��񳡤�)

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

    void set_applicant_pb(double pb);
    void set_guarantor_pb(double pb);
    void set_applicant_pb_adj(double pb);
    void set_guarantor_pass(int flag);
    void set_cutpoint(double cut);
    void set_ps_status_a(int code, char *msg);
    void set_ps_status_g(int code, char *msg);
    int  update_final_npv();
    int  write_result_prescreen_failed();
//    int  write_result_others(int decision);
//    int  write_result_declined(int decision);
//    int  write_result_approved(int decision);
    int  write_result(int decision, int revolving_amt);
};


#endif  //LoanApp_H
