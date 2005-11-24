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
    char  case_sn[20];			//�ץ�s��
    char  aID[12];			//�ӽФH�����Ҹ�
    char  gID[12];			//�O�ҤH�����Ҹ�
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
    float APR_1;			//�Q�v(�Ĥ@��)
    float APR_2;			//�Q�v(�ĤG��)
    float APR_3;			//�Q�v(�ĤT��
    int   Grace_period;			//�e����
    int   Fee_1;			//�}��O(�T�w���B����)
    float Fee_2;			//�}��O(�ʤ��񳡤�)

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
    double max_apr;                 // �~�Q�v (e.g. 18% �� 0.18)

    /* Null indicator for each application feature */


  public:


    LoanApp (char *caseSN);
    ~LoanApp();
    int initial();
    int validate();
    char * Applicant_id();
    char * Guanrantor_id();
};

int validate_date(char *date);  // format of date is yyymmdd

#endif  //LoanApp_H
