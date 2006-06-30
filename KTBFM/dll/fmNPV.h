/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: fmNPV.h
**
** Description: header file of NPV model for first mortgage
**
****************************************************************************/
//---------------------------------------------------------------------------
#ifndef fmNPVH
#define fmNPVH

#include <set>
#include <list>
#include <time.h>
#include <Math.hpp>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "pdacoSQL.h"
#include "functions.h"
#include "AdoHandle.h"
using namespace std;
#pragma package(smart_init)
// Constants used in the program.
const int TERM = 244;
#define min(a,b)  ((a) > (b)) ? (b): (a)
#define max(a,b)  ((a) > (b)) ? (a): (b)
const double ApprovedNPV = 2.0;  // (K)$2000
const double Allowance = 0.01;   // (K)$10

//---------------------------------------------------------------------------
class Loan {
  private:
    /* application information */
    String case_no;                 // �ץ�s��
    String app_date;		    // �ץ��J�ɶ� YYYYMMDDHHMMSS
    int prod_type;		    // ���~���O 1:�@��жU, 2:�W�U, 3:��l�B��U
    String app_id;		    // �ӽФH�����Ҧr���m�W
    String app_name;		    // �ӽФH�m�W
    String app_birthday;	    // �ӽФH�ͤ� YYYMMDD
    int app_marriage;		    // �ӽФH�B�� 0:���B / 1:�w�B / 2:���B
    int app_education;		    // �ӽФH�Ǿ� 0:��s�� / 1:�j�� / 2:�M�� / 3:���B�ꤤ(�t�H�U)
    int app_income;		    // �ӽФH�~���J
    int app_qualified;		    // �ӽФH�ŦX�ӧ@���� 0: NO / 1:YES
    String cos_id;		    // �@�P�ɴڤH�����Ҧr���m�W
    String cos_name;		    // �@�P�ɴڤH�m�W
    String cos_birthday;	    // �@�P�ɴڤH�ͤ� YYYMMDD
    int cos_marriage;		    // �@�P�ɴڤH�B�� 0:���B / 1:�w�B / 2:���B
    int cos_education;		    // �@�P�ɴڤH�Ǿ� 0:��s�� / 1:�j�� / 2:�M�� / 3:���B�ꤤ(�t�H�U)
    int cos_income;		    // �@�P�ɴڤH�~���J
    int cos_qualified;		    // �@�P�ɴڤH�ŦX�ӧ@���� 0: NO / 1:YES		
    String gua_id;		    // �O�ҤH�����Ҧr���m�W  
    String gua_name;		    // �O�ҤH�m�W
    String gua_birthday;	    // �O�ҤH�ͤ� YYYMMDD
    int gua_marriage;		    // �O�ҤH�B�� 0:���B / 1:�w�B / 2:���B               
    int gua_education;		    // �O�ҤH�Ǿ� 0:��s�� / 1:�j�� / 2:�M�� / 3:���B�ꤤ(�t�H�U)
    int gua_income;		    // �O�ҤH�~���J
    int gua_qualified;		    // �O�ҤH�ŦX�ӧ@���� 0: NO / 1:YES-- l                      oan info
    int app_amt;		    // �ӶU���B
    int periods;		    // �ӶU����
    float apr1;			    // �Ĥ@�q�Q�v
    int seg1;			    // �Ĥ@�q����
    float apr2;			    // �ĤG�q�Q�v
    int seg2;			    // �ĤG�q����
    float apr3;			    // �ĤT�q�Q�v
    int seg3;			    // �ĤT�q����
    int grace_period;		    // �e����
    int app_fee;		    // �}��O
    String owner_id;		    // �Ҧ��H�����Ҧr��
    String owner_name;		    // �Ҧ��H�m�W
    String land_num;		    // �a��/�ظ�
    int relationship;		    // �P�ӽФH���Y 0:���H / 1:���� / 2;�t�� / 3:�l�k / 4:��L
    float gav;			    // �ЫΤ��
    float nav;			    // �Ыβb��
    float existing_mortgage;	    // �Ы�Ų����
    int col_qualified;		    // ��O�~�ŦX�ӧ@���� 0: NO / 1:YES
    int premium_col;		    // ��O�~�Ϭq�}�n�B�f�t�u�}�س]���q 0: NO / 1:YES
    int monthly_payment;	    // �ثe�жU��I��
    String inquiry_date;	    // JCIC �d�ߤ�� YYYYMMDD
    String branch;		    // �i�����
    String emp_id;		    // �i����u
    String auditor;		    // �x�f�H��

    int record_count;               // number of app_info records read
    int code;                       // prescreen code

    /* Null indicator for each application feature */
    int prod_type_ind;
    int app_date_ind;
    int app_id_ind;
    int app_name_ind;
    int app_birthday_ind;
    int app_marriage_ind;
    int app_education_ind;
    int app_income_ind;
    int app_qualified_ind;
    int cos_id_ind;
    int cos_name_ind;
    int cos_birthday_ind;
    int cos_marriage_ind;
    int cos_education_ind;		 
    int cos_income_ind;		 
    int cos_qualified_ind;		 
    int gua_id_ind;		 
    int gua_name_ind;		 
    int gua_birthday_ind;
    int gua_marriage_ind;
    int gua_education_ind;		 
    int gua_income_ind;
    int gua_qualified_ind;		 
    int app_amt_ind;
    int periods_ind;			 
    int apr1_ind;
    int seg1_ind;			 
    int apr2_ind;			 
    int seg2_ind;
    int apr3_ind;
    int seg3_ind;
    int grace_period_ind;		 
    int app_fee_ind;		 
    int owner_id_ind;		 
    int owner_name_ind;		 
    int land_num_ind;
    int relationship_ind;		 
    int gav_ind;			 
    int nav_ind;			 
    int col_qualified_ind;		 
    int premium_col_ind;		 
    int monthly_payment_ind;
    int inquiry_date_ind;
    int branch_ind;
    int emp_id_ind;		 
    int auditor_ind;		 

    /* cash flow arrays to calculate NPV */
    String Message;
    double principal;
    double monthly_income;
    double monthly_debt;
    double max_loan_capacity;
    double weighted_apr;
    double pb_adjustment;
    double pdaco_score;
    int    pdaco_twentile;
    double fm_score;
    double fm_pb;
    double lowest_delta;
    double min_apr1;
    double min_apr2;
    double min_apr3;
    double optimal_line;
    double total_npv;
    double min_npv;              // npv close to hurdle amount
    double apr[TERM+4];
    double open_attrition[TERM+4];
    double voluntary_attrition[TERM+4];
    double involuntary_attrition[TERM+4];
    double vol_attrition_open[TERM+4];
    double open_credit_fee[TERM+4];
    double risk_mgmt_revenue[TERM+4];
    double os_principal[TERM+4];
    double principal_repayment[TERM+4];
    double interest_repayment[TERM+4];
    double monthly_repayment[TERM+4];
    double interest_revenue[TERM+4];
    double late_fee[TERM+4];
    double interest_cost[TERM+4];
    double account_management_cost[TERM+4];
    double late_cost[TERM+4];
    double collection_cost[TERM+4];
    double working_capital[TERM+4];
    double credit_loss[TERM+4];

    /* functions to calculate cash flows */
    void npv_init();
    void set_apr(double delta_apr);
    void set_attrition(double pb);
    void set_amortize(int line);
    void set_annuity(int line);
    double set_interest_revenue();
    double set_setup_fee();
    double set_late_fee(double pb);
    double set_interest_cost();
    double calculate_commission();
    double setup_cost();
    double set_account_management_cost();
    double set_late_cost(double pb);
    double set_collection_cost();
    double set_working_capital();
    double set_credit_loss(double pb);

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

    Loan (char *case_no);
    ~Loan ();
    String error();
    int app_info_validate(char *case_no, TADOHandler *handler);
    double secured_pb();
    double calculate_npv(double delta_apr);
    int calculate_optimal_line(int loops, double npv[][3], TADOHandler *handler);
    int exist_applicant();
    int exist_coapplicant();
    int exist_guarantor();
    String Case_no ();
    char * Applicant();
    char * Cosigner();
    char * Guarantor();
    String Inquiry_date();
    int appIncome();
    int cosIncome();
    int guaIncome();
    void set_risk_score (double score);
    void set_monthly_income(double income);
    void set_monthly_debt(double debt);
    void set_risk_twentile (double score);
    void set_principal();
    void set_principal_reload();
    void set_pb_adjustment(double score);

    void set_lowest_rate (double delta_rate);
    void set_npv (double npv_value);
    double find_lowest_rate (double offset, double delta_r);
    double get_max_apr();
    double Min_APR1();
    double Min_APR2();
    double Min_APR3();
    double Monthly_Income();
    double Monthly_Debt();
    double Max_Loan_Capacity();
    double Weighted_APR();
    double Lowest_npv();
    double calculate_optimal_npv();

    double get_rscore ();
    double get_pd();
    int get_test_PB(char *idn, TADOHandler *handler);
    double get_npv ();
    double get_principal ();
    void Init_Maintenance(TADOHandler *handler);
    // functions for test only
    int app_validate_test(char *case_no, TADOHandler *handler);
    int app_validate_bt(char *case_no, TADOHandler *handler);
};
//Economic model constants
const double LEVERAGE_RATIO = 0.9;     // �|�Ť�v�]�H�^
const double COMMISSION_RATE = 0.0015; // ������v�]�H�^
const double FEE_COMMISSION_RATE = 0.1;// �}��O������v�]�H�^
const double ALLOGRAPH_FEE = 0.0;      // �N�ѶO
const double LAND_ADM_FEE = 0.0;       // �a�F�W�O
const double DUEI_BAO_FEE = 2.25;      // ��O
const double SETUP_FEE = 3.0;          // �}��O
const double COF = 0.01;               // �������
const double ROE = 0.1;                // �ѪF�v�q���S
const double ACCT_MGMT_COST = 0.08;    // Account Management Cost
const double LATE_PENALTY_RATIO = 1.2; // mutiple of interest rate applied
const double M2_3_EXPENSE = 5.25;      // NT$5.25K
const double M4P_EXPENSE = 22.5;       // NT$22500 per account per month
const double TIME_TO_RECOVER = 12;     // months
const double LEGAL_FEE_RATE = 0.008;   // percentage of balance, taken from Teresa's model, for collection


#endif
