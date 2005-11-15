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
    char case_sn[20];                 // �ץ�s��
    char aID[12];                     // �ӽФH�����Ҹ�
    char gID[12];                     // �O�ҤH�����Ҹ�
    int product_type;               // ���~�N��(1 ������H�U; 2 ���d�n��)
    int gender;                     // 0���k��; 1���k��
    char zip[4];                     // �T��l���ϸ�
    int edu;                        // �Ш|�{�� �A1 ����s��(�t)�H�W ; 2 ���j��; 3 ���M��;
                                    // 4  ������(¾); 5 ���ꤤ�H�U; 6 ����L
    int marriage_status;            // �B�ê��p,1 ���w�B; 2 �����B; 3 �����B; 4 ����L
    double cof;                     // �������
    double roe;                     // �ѪF�v�q���S
    int info_processing_cost;       // �C��C����u�x�s�Ȧ��T�ǶO�� (�s�x����)
    int operation_cost;             // �C��C����u�x�s�Ȧ����@�~�ΫȪA�O�� (�s�x����)
    int hr_cost;                    // �C��C��H�Ʀ����O�� (�s�x����)
    double commission;              // ���� (�s�x����)
    int other_initial_cost1;       // ��L�}�즨���@
    int other_initial_cost2;       // ��L�}�즨���G
    int other_initial_cost3;       // ��L�}�즨���T

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


#endif  //LoanApp_H
