//---------------------------------------------------------------------------
#pragma hdrstop

#include "loanApp.h"
#include <Math.hpp>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "ploanSQL.h"
#include "functions.h"
#include "attrition.h"
#include "economic.h"
// Constants used in the program.
const char *expire_date = "21001231";
const double m1_to_m7_ratio = 5.0;

//---------------------------------------------------------------------------

#pragma package(smart_init)

Loan::Loan (char * appSN, char* appDate, TADOHandler *handler):
    app_sn(appSN), app_date(appDate), product_type_ind(0),
    gender_ind(0), zip_ind(0), secretive_ind(0), edu_ind(0),
    principal_ind(0), int_rate_ind(0), teaser_rate_ind(0), periods_ind(0),
    teaser_period_ind(0), grace_period_ind(0), application_fee_ind(0),
    credit_checking_fee_ind(0), risk_mgmt_fee_ind(0), risk_mgmt_fee_terms_ind(0),
    sales_channel_ind(0), risk_level_ind(0)
{
 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
}
//---------------------------------------------------------------------------
Loan::Loan (char * appSN, char* appDate, char* tsDate, char *jcicDate, int tsn, TADOHandler *handler):
    app_sn(appSN), app_date(appDate), ts_date(tsDate), jcic_date(jcicDate),tsn(tsn),product_type_ind(0),
    gender_ind(0), zip_ind(0), secretive_ind(0), edu_ind(0),
    principal_ind(0), int_rate_ind(0), teaser_rate_ind(0), periods_ind(0),
    teaser_period_ind(0), grace_period_ind(0), application_fee_ind(0),
    credit_checking_fee_ind(0), risk_mgmt_fee_ind(0), risk_mgmt_fee_terms_ind(0),
    sales_channel_ind(0), risk_level_ind(0)
{
 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
}
//---------------------------------------------------------------------------
void Loan::app_info_validate(char * appNo, char* appDate, TADOHandler *handler)
{
 Variant hostVars[5];
 bool success = true;
 try {
    hostVars[0] = appNo;
    hostVars[1] = appDate;
    handler->ExecSQLQry(SQLCommands[Get_AppInfo_Record], hostVars, 1, ds);
    record_count = ds->RecordCount;

    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["product_type"].IsNull())
          product_type = ds->FieldValues["product_type"];
       else
          product_type_ind = -1;

       if (! ds->FieldValues["gender"].IsNull())
          gender = ds->FieldValues["gender"];
       else
          gender_ind = -1;
       if (! ds->FieldValues["zip"].IsNull())
          zip = ds->FieldValues["zip"];
       else
          zip_ind = -1;

       if (! ds->FieldValues["secretive"].IsNull())
          secretive = ds->FieldValues["secretive"];
       else
          secretive_ind = -1;

       if (! ds->FieldValues["edu"].IsNull())
          edu = ds->FieldValues["edu"];
       else
          edu_ind = -1;

       if (! ds->FieldValues["marriage_status"].IsNull())
          marriage_status = ds->FieldValues["marriage_status"];
       else
          marriage_status_ind = -1;
    }

  if (record_count == 0) {
     throw DataEx("�L�ӽХ��ơC");
  }

  if ((product_type_ind == -1) || (product_type < 1) || (product_type > 2)) {
     Message += "���~�N�����~�A�����O1(����H�U), 2(�d�n��)�C";
     success = false;
  }

  if ((gender_ind == -1) || (gender < 0) || (gender > 1)) {
     Message += "�ʧO���~�A�����O0(�k��), 1(�k��)�C";
     success = false;
  }

  if ((secretive_ind == -1) || (secretive < 0) || (secretive > 1)) {
     Message += "�K�a�H���~�A�����O0(�_), 1(�O)�C";
     success = false;
  }

  if ((edu_ind == -1) || (edu < 1) || (edu > 6)) {
     Message += "�Ш|�{�׿��~�A�����O1(��s��(�t)�H�W), 2(�j��), 3(�M��), 4(����(¾)), 5(�ꤤ�H�U), 6(��L)�C";
     success = false;
  }

  if ((marriage_status_ind == -1) || (marriage_status < 1) || (marriage_status > 4)) {
     Message += "�B�ê��p���~�A�����O1(�w�B), 2(���B), 3(���B), 4(��L)�C";
     success = false;
  }

  if (!success) throw DataEx(Message);
 } catch (Exception &E) {
    throw;
 }
}
//---------------------------------------------------------------------------
void Loan::loan_validate(char * appNo, int tsn, TADOHandler *handler)
{
 Variant hostVars[5];
 bool success = true;
 try {
    hostVars[0] = appNo;
    hostVars[1] = tsn;
    handler->ExecSQLQry(SQLCommands[Get_Loan_Record], hostVars, 1, ds);
    trial_count = ds->RecordCount;

    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["loan_amount"].IsNull())
          principal = ds->FieldValues["loan_amount"];
       else
          principal_ind = -1;

       if (! ds->FieldValues["apr"].IsNull())
          int_rate = ds->FieldValues["apr"];
       else
          int_rate_ind = -1;

       if (! ds->FieldValues["terms"].IsNull())
          periods = ds->FieldValues["terms"];
       else
          periods_ind = -1;

       if (! ds->FieldValues["grace_period"].IsNull())
          grace_period = ds->FieldValues["grace_period"];
       else
          grace_period_ind = -1;

       if (! ds->FieldValues["teaser_rate"].IsNull())
          teaser_rate = ds->FieldValues["teaser_rate"];
       else
          teaser_rate_ind = -1;

       if (! ds->FieldValues["teaser_period"].IsNull())
          teaser_period = ds->FieldValues["teaser_period"];
       else
          teaser_period_ind = -1;

       if (! ds->FieldValues["application_fee"].IsNull())
          application_fee = ds->FieldValues["application_fee"];
       else
          application_fee_ind = -1;

       if (! ds->FieldValues["credit_checking_fee"].IsNull())
          credit_checking_fee = ds->FieldValues["credit_checking_fee"];
       else
          credit_checking_fee_ind = -1;

       if (! ds->FieldValues["risk_mgmt_fee"].IsNull())
          risk_mgmt_fee = ds->FieldValues["risk_mgmt_fee"];
       else
          risk_mgmt_fee_ind = -1;

       if (! ds->FieldValues["risk_mgmt_fee_terms"].IsNull())
          risk_mgmt_fee_terms = ds->FieldValues["risk_mgmt_fee_terms"];
       else
          risk_mgmt_fee_terms_ind = -1;

       if (! ds->FieldValues["sales_channel"].IsNull())
          sales_channel = ds->FieldValues["sales_channel"];
       else
          sales_channel_ind = -1;
/*
       if (!ds->FieldValues["risk_level"].IsNull())
          risk_level = ds->FieldValues["risk_level"];
       else
          risk_level_ind = -1;
*/
    }
  if (trial_count == 0) {
     throw DataEx("�L�U�ڸ�ơC");
  }
  if ((principal_ind == -1) || (principal < 10000.0) || (principal > 9000000.0)) {
     Message += "�ӶU���B��������10,000�M9,000,000�]���^�C";
     success = false;
  }

  if ((int_rate_ind == -1) || (int_rate < 0.0)
        ||(int_rate > 0.20)) {
       Message += "�U�ڧQ�v���~�A��������0�M20(%)�C";
       success = false;
  }

  if ((periods_ind == -1) || (periods < 1) || (periods > 120)) {
     Message += "�U�ڴ��ƿ��~�A��������1�M120�C";
     success = false;
  }

  if ((application_fee_ind == -1) || (application_fee < 0) || (application_fee > 100000)) {
     Message += "�}��O��������0�M100000�]���^�C";
     success = false;
  }

  if ((credit_checking_fee_ind == -1) || (credit_checking_fee < 0) || (credit_checking_fee > 100000)) {
     Message += "�x�H�d�߶O��������0�M100000�]���^�C";
     success = false;
  }

  if ((risk_mgmt_fee_ind == -1) || (risk_mgmt_fee < 0) || (risk_mgmt_fee > 100000)) {
     Message += "�C�����I�޲z�O��������0�M100000�]���^�C";
     success = false;
  }

  if ((risk_mgmt_fee_terms_ind == -1) || (risk_mgmt_fee_terms < 0) || (risk_mgmt_fee_terms > periods)) {
     Message += "���I�޲z�O�Φ������ƿ��~�A��������0�M�U�ڴ��ơC";
     success = false;
  }

  if ((teaser_rate_ind == -1) || (teaser_rate < 0.0)
        ||(teaser_rate > int_rate)) {
       Message += "�u�f�~�Q�v���~�A��������0�M�U�ڧQ�v�C";
       success = false;
  }

  if ((teaser_period_ind == -1) || (teaser_period < 0) || (teaser_period > periods)) {
     Message += "�u�f�����~�A��������0�M�U�ڴ��ơC";
     success = false;
  }

  if ((grace_period_ind == -1) || (grace_period < 0) || (grace_period > periods)) {
     Message += "�e�������~�A��������0�M�U�ڴ��ơC";
     success = false;
  }

//  if (!success) throw DataEx(Message);
 } catch (Exception &E) {
    throw;
 }
}

//---------------------------------------------------------------------------
Loan::~Loan ()
{
 ds->Close();
 delete ds;
}

//---------------------------------------------------------------------------
String Loan::error ()
{
 return Message;
}
//---------------------------------------------------------------------------
int Loan::get_product_type ()
{
 return product_type;
}
//---------------------------------------------------------------------------
int Loan::get_code ()
{
 return code;
}
//---------------------------------------------------------------------------
double Loan::get_pd ()
{
 return pd;
}
//---------------------------------------------------------------------------
double Loan::get_npv ()
{
 return total_npv;
}
//---------------------------------------------------------------------------
double Loan::get_principal ()
{
 return principal;
}
//---------------------------------------------------------------------------
void Loan::prescreen(char *inquiry_date, TADOHandler *handler)
{
 Variant hostVars[5];

 jcic_date = inquiry_date;
 avail_flag = jas002_defect = krm001_hit = krm023_hit = fs044 = 0;
 try {
    hostVars[0] = app_sn;
    hostVars[1] = app_date;
    handler->ExecSQLCmd(SQLCommands[Insert_Daco_Table], hostVars, 1);

    hostVars[0] = jcic_date;
    handler->ExecSQLCmd(SQLCommands[Update_Inquiry_Date], hostVars, 0);

    handler->ExecSQLCmd(SQLCommands[Drop_Procedure_TF_prepare_jcic_data]);
    handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_prepare_jcic_data]);
    hostVars[0] = app_sn;
    hostVars[1] = jcic_date;
    handler->ExecSQLCmd(SQLCommands[Exec_Procedure_TF_prepare_jcic_data], hostVars, 1);
    handler->ExecSQLCmd(SQLCommands[Drop_Procedure_TF_prepare_jcic_data]);

    handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_loan_prescreen]);

    hostVars[0] = app_sn;
    handler->ExecSQLQry(SQLCommands[Get_Prescreen_Result], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
       jas002_defect = ds->FieldValues["jas002_defect"];
       krm001_hit = ds->FieldValues["krm001_hit"];
       krm023_hit = ds->FieldValues["krm023_hit"];
       bam085_hit = ds->FieldValues["krm023_hit"];
       app_max_bucket = ds->FieldValues["app_max_bucket"];
       fs044 = ds->FieldValues["fs044"];
       delinquent_months = ds->FieldValues["fs334"];
       cash_max_bucket = ds->FieldValues["fs302"];
       ind001 = ds->FieldValues["ind001"];
       ms080 = ds->FieldValues["ms080"];
    }

    if (jas002_defect > 0) {
       Message = "�ڵ� [���h���j���ک��«H���`���O��]"; code = 103;
    }
    if (app_max_bucket > 3) {
       Message = "�ڵ� [�H�Υd��90�ѥH�W��ú�O��]"; code = 104;
    }
    if (fs044 > 0) {
       Message = "�ڵ� [�U�ڦ���ú�O��]"; code = 105;
    }
    if (cash_max_bucket > 0) {
       Message = "�ڵ� [�{���d�e������ú�O��]"; code = 106;
    }
    if (delinquent_months > 3) {
       Message = "�ڵ� [�U�ڦ�90�ѥH�W��ú�O��]"; code = 107;
    }
 } catch (Exception &E) {
     throw;
   }
}

//---------------------------------------------------------------------------
void Loan::calculate_rscore(TADOHandler *handler)
{
 Variant hostVars[5];
 try {
    hostVars[0] = app_sn;
    hostVars[1] = ts_date;
    handler->ExecSQLCmd(SQLCommands[Calculate_Loan_Del_Number], hostVars, 1);
//    handler->ExecSQLCmd(SQLCommands[Update_Loan_Del_Number]);
    if (krm001_hit == 1 && krm023_hit == 1 && ind001 == 0)
       handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_ploan_model]);
    else if (bam085_hit == 1 && ms080 <= 0)
             handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_BAM_no_payment]);
         else if (bam085_hit == 1 && ms080 > 0)
                 handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_BAM_with_payment]);
              else
                 handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_demographic_model]);

#ifdef _WRFLOW
    handler->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);
#endif
 } catch (Exception &E) {
     throw;
   }
}
//---------------------------------------------------------------------------
void Loan::calculate_pd(TADOHandler *handler)
{
/*
 Variant hostVars[5];
 try {
    hostVars[0] = app_sn;
    hostVars[1] = ts_date;
    handler->ExecSQLCmd(SQLCommands[Update_Loan_Del_Number], hostVars, 1);
    handler->ExecSQLCmd(SQLCommands[Calculate_Loan_Del_Number]);
    if (krm001_hit == 1 && krm023_hit == 1 && ind001 == 0)
       handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_ploan_model]);
    else if (bam085_hit == 1 && ms080 <= 0)
             handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_BAM_no_payment]);
         else if (bam085_hit == 1 && ms080 > 0)
                 handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_BAM_with_payment]);
              else
                 handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_demographic_model]);
 } catch (Exception &E) {
     throw;
   }
   */
}
//---------------------------------------------------------------------------
void Loan::postFilter()
{
 if (jas002_defect > 0)
    throw (RiskEx ("�ڵ� [���h���j���ک��«H���`���O��]", 103));
 else if (app_max_bucket > 3)
    throw (RiskEx ("�ڵ� [�H�Υd��90�ѥH�W��ú�O��]", 104));
 else if (fs044 > 0)
    throw (RiskEx ("�ڵ� [�U�ڦ���ú�O��]", 105));
 else if (cash_max_bucket > 0)
    throw (RiskEx ("�ڵ� [�{���d�e������ú�O��]", 106));
 else if (delinquent_months > 3)
    throw (RiskEx ("�ڵ� [�U�ڦ�90 �ѥH�W��ú�O��]", 107));
}

//---------------------------------------------------------------------------
void Loan::calculate_npv()
{
/*
  double revenue, cost, wc;
  double Interest_Revenue, Late_Fee, Early_Closing_Fee, Application_Fee;
  double Other_Fee, Interest_Cost, Acct_Mgmt_Cost, PreCollection_Cost;
  double Collection_Cost, Commission, Working_Capital, Credit_Loss;

  npv_init();
  set_apr();
  set_attrition();
  if ((repayment == '1') || (repayment == '3'))    //(1:�@��k;2:�����k;3:�����k)
     set_amortize();
  else
     set_annuity();
  Application_Fee = application_fee - query_fee;
  Commission =  Application_Fee * commission_ratio;
  if (Commission < 0) Commission = 0;
  Interest_Revenue = set_interest_revenue();
  Late_Fee = set_late_fee();
  Early_Closing_Fee = set_early_closing_fee();
  Other_Fee = risk_mgmt_fee + acct_mgmt_fee + bt_fee;
  Interest_Cost = set_interest_cost();
  Acct_Mgmt_Cost = set_account_management_cost();
  PreCollection_Cost = set_precollection_cost();
  Collection_Cost = set_collection_cost();
  Working_Capital = set_working_capital();
  Credit_Loss = set_credit_loss();
  total_npv = (Interest_Revenue + Late_Fee + Early_Closing_Fee + Application_Fee
               + Other_Fee)                            // Revenue
              - (Interest_Cost + Acct_Mgmt_Cost + PreCollection_Cost
               + Collection_Cost + Commission)         // Cost
              + (Working_Capital + Credit_Loss);       // Working Capital
#ifdef _WRFLOW
     fstream outf;
     outf.open("NPV_flows.txt", ios::app | ios::out);  // Open for ouput and append

     outf << "Case SN: " << case_sn.c_str() << "   IDN: " << idn.c_str()
          << "   Dac SN : " << dac_sn << endl;
     outf << "TERM       APR  Open_Atr   Vol_Atr Invol_Atr    M1_Atr  Base_Atr        "
          << "OsPrinL        P_Repay        I_Repay         IntRev          LateF         "
          << "EarlyF        IntCost       AcctCost         PreCol        Collect             "
          << "WC        CreLoss" << endl;
     outf << "========================================================================================================="
          << "======================================================================"
          << "======================================================================" << endl;
     outf <<  showpoint
              << setw(79) << setprecision(8) << Application_Fee
              << setw(15) << setprecision(8) << Other_Fee
              << setw(15) << setprecision(8) << Commission
              << setw(15) << setprecision(8) << Interest_Revenue
              << setw(15) << setprecision(8) << Late_Fee
              << setw(15) << setprecision(8) << Early_Closing_Fee
              << setw(15) << setprecision(8) << Interest_Cost
              << setw(15) << setprecision(8) << Acct_Mgmt_Cost
              << setw(15) << setprecision(8) << PreCollection_Cost
              << setw(15) << setprecision(8) << Collection_Cost
              << setw(15) << setprecision(8) << Working_Capital
              << setw(15) << setprecision(8) << Credit_Loss
              << endl;
     for (int i = 0; i < periods + 4; i++) {
         outf << setprecision(8) << showpoint
              << setw(4)  << i
              << setw(10) << setprecision(4) << apr[i]
              << setw(10) << setprecision(4) << open_attrition[i]
              << setw(10) << setprecision(4) << voluntary_attrition[i]
              << setw(10) << setprecision(4) << involuntary_attrition[i]
              << setw(10) << setprecision(4) << m1_attrition[i]
              << setw(10) << setprecision(4) << base_attrition[i]
              << setw(15) << setprecision(8) << os_principal[i]
              << setw(15) << setprecision(8) << principal_repayment[i]
              << setw(15) << setprecision(8) << interest_repayment[i]
              << setw(15) << setprecision(8) << interest_revenue[i]
              << setw(15) << setprecision(8) << late_fee[i]
              << setw(15) << setprecision(8) << early_closing_fee[i]
              << setw(15) << setprecision(8) << interest_cost[i]
              << setw(15) << setprecision(8) << account_management_cost[i]
              << setw(15) << setprecision(8) << precollection_cost[i]
              << setw(15) << setprecision(8) << collection_cost[i]
              << setw(15) << setprecision(8) << working_capital[i]
              << setw(15) << setprecision(8) << credit_loss[i]
              << endl;
     }
#endif
*/
}
//---------------------------------------------------------------------------
void Loan::npv_init()
{
  for (int i = 0; i < TERM + 4; i ++) {
     apr[i] = 0.0;
     open_attrition[i] = 0.0;
     voluntary_attrition[i] = 0.0;
     involuntary_attrition[i] = 0.0;
     m1_attrition[i] = 0.0;
     base_attrition[i] = 0.0;
     os_principal[i] = 0.0;
     principal_repayment[i] = 0.0;
     interest_repayment[i] = 0.0;
     interest_revenue[i] = 0.0;
     late_fee[i] = 0.0;
     early_closing_fee[i] = 0.0;
     interest_cost[i] = 0.0;
     account_management_cost[i] = 0.0;
     precollection_cost[i] = 0.0;
     collection_cost[i] = 0.0;
     working_capital[i] = 0.0;
     credit_loss[i] = 0.0;
  }
}

//---------------------------------------------------------------------------
void Loan::set_apr()
{
  apr[0] = 0.0;
  for (int i = 1; i <= periods; i++) {
      if (i > teaser_period)
          apr[i] = int_rate / 12.0;
      else
          apr[i] = teaser_rate / 12.0;  // convert APR to monthly rate
  }
}

//---------------------------------------------------------------------------
void Loan::set_attrition()
{
  double monthly_pd = pd / 12.0;
  int cat, term;

  if (periods < 48) { // 3 year (4 * 12 month)
     if (max_apr <= 0.05)
        cat = 0;
     else if (max_apr > 0.05 && max_apr <= 0.09)
        cat = 1;
     else
        cat = 2;
     term = 36; // month
  } else if (periods >= 48 && periods < 72 ) { // 5 year (4*12 - 6*12 month)
     if (max_apr <= 0.05)
        cat = 3;
     else if (max_apr > 0.05 && max_apr <= 0.09)
        cat = 4;
     else
        cat = 5;
     term = 60; // month
  } else { //  7 year
     if (max_apr <= 0.05)
        cat = 6;
     else if (max_apr > 0.05 && max_apr <= 0.09)
        cat = 7;
     else
        cat = 8;
     term = 120; // month
  }

  for (int i = 0; i < term; i++)
      base_attrition[i] = Attrition_Table[cat][i];

  open_attrition[0] = 1.0;
  voluntary_attrition[0] = involuntary_attrition[0] = m1_attrition[0] = 0.0;
  for (int i = 1; i <= periods; i++) {
      voluntary_attrition[i] = open_attrition[i-1] * base_attrition[i-1];
      involuntary_attrition[i] = open_attrition[i-1] * monthly_pd;
      open_attrition[i] = open_attrition[i-1] - voluntary_attrition[i] - involuntary_attrition[i];
      m1_attrition[i] = involuntary_attrition[i] * m1_to_m7_ratio;
  }
}

//---------------------------------------------------------------------------
void Loan::set_amortize()
{
  os_principal [0] = principal;
  interest_repayment [0] = 0.0;
  principal_repayment [0] = 0.0;
//  monthly_repayment [0] = 0.0;
  grace_period = ((grace_period == periods)? grace_period - 1: grace_period);
  double fix_payment = principal / (periods - grace_period);

  for (int i = 1; i<= periods; i++) {
      interest_repayment[i] = os_principal[i-1] * apr[i];
      if (i > grace_period)
         principal_repayment [i] = fix_payment;
      else
         principal_repayment [i] = 0.0;
      os_principal [i] = os_principal[i-1] - principal_repayment[i];
//      monthly_repayment[i] = principal_repayment[i] + interest_repayment[i];
  }
}

//---------------------------------------------------------------------------
void Loan::set_annuity()
{
  int Grace_Period = ((grace_period == periods)? grace_period - 1: grace_period);
  int after_grace_period = periods - Grace_Period;
  double repayment = principal / periods;     // assume apr = 0.0%
  int remaining_period;

  os_principal [0] = principal;
  interest_repayment [0] = 0.0;
  principal_repayment [0] = 0.0;
//  monthly_repayment [0] = 0.0;
  for (int i = 1; i <= periods; i++) {
     if (apr[i] != apr[i-1]) {
        remaining_period = periods - i + 1;
        remaining_period = (remaining_period > after_grace_period ? after_grace_period : remaining_period);
        if (apr[i] == 0.0)
           repayment = os_principal[i-1] / remaining_period;
        else
           repayment = Payment(apr[i], remaining_period, -os_principal[i-1], 0.0, ptEndOfPeriod);
     }
     interest_repayment[i] = os_principal[i-1] * apr[i];
     if (i > grace_period)
        principal_repayment[i] = repayment - interest_repayment[i];
     else
        principal_repayment[i] = 0.0;

     os_principal[i] = os_principal[i-1] - principal_repayment[i];
//     monthly_repayment[i] = principal_repayment[i] + interest_repayment[i];
  }
}

//---------------------------------------------------------------------------
double Loan::set_interest_revenue()
{
  for (int i = 1; i <= periods; i++)
     interest_revenue[i] = os_principal[i-1] * apr[i] * open_attrition[i];

  return (NetPresentValue(roe / 12.0, interest_revenue + 1, periods, ptEndOfPeriod)
          + interest_revenue[0]);
}
//---------------------------------------------------------------------------
double Loan::set_late_fee()
{
  for (int i = 1; i <= periods; i++)
     late_fee[i] = os_principal[i-1] * apr[i] * 12.0 / 365.0 *
                  (m1_attrition[i] * m1_recovery_ratio * m1_avg_late_days * m1_penalty_rate +
                   involuntary_attrition[i] * m6_recovery_ratio * m6_avg_late_days * m6_penalty_rate);

  return (NetPresentValue(roe / 12.0, late_fee + 1, periods, ptEndOfPeriod)
          + late_fee[0]);
}
/*
double Loan::set_early_closing_fee()
{
  for (int i = 1; i <= periods; i++)
     if (i > early_closing_period)
        early_closing_fee [i] = 0.0;
     else
        early_closing_fee [i] = os_principal[i-1] * voluntary_attrition[i] * early_closing_fee_pct
        		        * early_closing_fee_collectable_ratio;

  return (NetPresentValue(roe / 12.0, early_closing_fee + 1, periods, ptEndOfPeriod)
          + early_closing_fee[0]);
}
*/
//---------------------------------------------------------------------------
// Cost
// Interest Cost:
double Loan::set_interest_cost()
{
  for (int i = 1; i <= periods; i++)
     interest_cost[i] = os_principal[i-1] * open_attrition[i-1] * leverage_ratio * cof / 12.0;
  return (NetPresentValue(roe / 12.0, interest_cost + 1, periods, ptEndOfPeriod)
          + interest_cost[0]);
}

// Commission:
double Loan::calculate_commission()
{
  for (int i = 1; i <= periods; i++)
     account_management_cost[i] = acct_mgmt_cost * open_attrition[i-1];
  return (NetPresentValue(roe / 12.0, account_management_cost + 1, periods, ptEndOfPeriod)
          + account_management_cost[0]);
}
// Account Management Cost
double Loan::set_account_management_cost()
{
  for (int i = 1; i <= periods; i++)
     account_management_cost[i] = acct_mgmt_cost * open_attrition[i-1];
  return (NetPresentValue(roe / 12.0, account_management_cost + 1, periods, ptEndOfPeriod)
          + account_management_cost[0]);
}

// Pre-collection Cost
/* 4 days late will send message to customer.
   30 days late will call to customer.
   30 days late is 2 payments behind.
*/
//---------------------------------------------------------------------------
double Loan::set_precollection_cost()
{
  double monthly_pd = pd / 12.0;
  double m1Open = monthly_pd * m1_to_m7_ratio;
  double d4Open = m1Open * m1_to_m7_ratio;
  double phone_expense;

  switch (district) {
    case 1: phone_expense =  phone_expense_north;  break;
    case 2: phone_expense =  phone_expense_central;  break;
    case 3: phone_expense =  phone_expense_south;  break;
  }
  for (int i = 1; i <= periods + 1; i++)
     if (i < 2)
        precollection_cost[i] = 0.0;
     else {
       if (os_principal[i-2] > 0)
          precollection_cost[i] = short_message_expense * open_attrition[i-2] * d4Open +
                             phone_expense * open_attrition[i-2] * m1Open;
       else
          precollection_cost[i] = 0.0;
     }
  return (NetPresentValue(roe / 12.0, precollection_cost + 1, periods+1, ptEndOfPeriod)
          + precollection_cost[0]);
}
//---------------------------------------------------------------------------
// Collection Cost (legal)
/* 90+ days delinquent will tirgger legal action to collect outstanding principal
   90+ days delinquent is 4 payments behind
*/
double Loan::set_collection_cost()
{
  double monthly_pd = pd / 12.0;
  double legal_expense;
  double legal_detain_ratio;

  switch (district) {
    case 1: legal_expense = legal_exec_north + legal_query_north
                            + legal_auction_north + legal_staff_north;
            legal_detain_ratio = legal_detain_ratio_north;
            break;
    case 2: legal_expense =  legal_exec_central + legal_query_central
                            + legal_auction_central + legal_staff_central;
            legal_detain_ratio = legal_detain_ratio_central;
            break;
    case 3: legal_expense =  legal_exec_south + legal_query_south
                            + legal_auction_south + legal_staff_south;
            legal_detain_ratio = legal_detain_ratio_south;
            break;
  }
  for (int i = 1; i <= periods + 3 ; i++)
     if (i < 4)
        collection_cost [i] = 0.0;
     else {
       if (os_principal[i-4] > 0)
          collection_cost[i] = open_attrition[i-4] * monthly_pd *
                       (legal_expense + os_principal[i-4] * legal_detain_ratio);
       else
          collection_cost[i] = 0.0;
     }
  return (NetPresentValue(roe / 12.0, collection_cost + 1, periods+3, ptEndOfPeriod)
          + collection_cost[0]);
}


// Working Capacital
/*
    working_capital [i] = (os_principal[i-1]* open_attrition[i-1] -
                           os_principal[i] * open_attrition[i]) *
                           (os_principal[i-1]* open_attrition[i-1] * leverage_ratio -
                           os_principal[i] * open_attrition[i] * leverage_ratio)
*/
//---------------------------------------------------------------------------
double Loan::set_working_capital()
{
 working_capital [0] = -os_principal[0] * (1 - leverage_ratio);
 for (int i = 1; i <= periods; i++) {
    working_capital [i] = (os_principal[i-1]* open_attrition[i-1] -
                           os_principal[i] * open_attrition[i]) *
                           (1- leverage_ratio);
 }
  return (NetPresentValue(roe / 12.0, working_capital + 1, periods, ptEndOfPeriod)
          + working_capital[0]);
}

//---------------------------------------------------------------------------
// Credit loss:
double Loan::set_credit_loss()
{
 double discount_rate = 1 / pow((1 + roe / 12.0),legal_action_period);
 credit_loss [0] = 0.0;
 for (int i = 1; i <= periods; i++) {
    credit_loss [i] = -os_principal[i-1]* involuntary_attrition[i] *
                      (1 - recovery_ratio * discount_rate);
 }
  return (NetPresentValue(roe / 12.0, credit_loss + 1, periods, ptEndOfPeriod)
          + credit_loss[0]);
}

//---------------------------------------------------------------------------
void Loan::Init_Maintenance(TADOHandler *handler)
{
 try {
    handler->ExecSQLQry(SQLCommands[Get_Maintenance_Record], ds);
    record_count = ds->RecordCount;
    if (record_count != 1)
       throw DataEx("���@���楲���u���@����ơC");
    ds->First();
/*
  For a table column with data type decimal(p, s),
    ds->FieldValues["m1_recovery_ratio"] will return 0.
    ds->FieldByName("m1_recovery_ratio")->AsFloat will return correct value.
  Conclusion:
    For a column with data type char, varchar, you can get the value with
     ds->FieldValues["m1_recovery_ratio"].
    For a column with number data type, you'd better use
     ds->FieldByName("m1_recovery_ratio")->AsFloat to get its value.
*/
    if (!ds->Eof) {
       if (! ds->FieldValues["m1_recovery_ratio"].IsNull())
          m1_recovery_ratio = ds->FieldByName("m1_recovery_ratio")->AsFloat / 100.0;

       if (! ds->FieldValues["m1_avg_late_days"].IsNull())
          m1_avg_late_days = ds->FieldByName("m1_avg_late_days")->AsInteger;

       if (! ds->FieldValues["m1_penalty_rate"].IsNull())
          m1_penalty_rate = ds->FieldByName("m1_penalty_rate")->AsFloat / 100.0;

       if (! ds->FieldValues["m6_recovery_ratio"].IsNull())
          m6_recovery_ratio = ds->FieldByName("m6_recovery_ratio")->AsFloat / 100.0;

       if (! ds->FieldValues["m6_avg_late_days"].IsNull())
          m6_avg_late_days = ds->FieldByName("m6_avg_late_days")->AsInteger;

       if (! ds->FieldValues["m6_penalty_rate"].IsNull())
          m6_penalty_rate = ds->FieldByName("m6_penalty_rate")->AsFloat / 100.0;
/*
       if (! ds->FieldValues["early_closing_period"].IsNull())
          early_closing_period = ds->FieldByName("early_closing_period")->AsInteger;
*/
       if (! ds->FieldValues["early_closing_fee_pct"].IsNull())
          early_closing_fee_pct = ds->FieldByName("early_closing_fee_pct")->AsFloat / 100.0;

       if (! ds->FieldValues["early_closing_fee_collectable_ratio"].IsNull())
          early_closing_fee_collectable_ratio =
            ds->FieldByName("early_closing_fee_collectable_ratio")->AsFloat / 100.0;

       if (! ds->FieldValues["leverage_ratio"].IsNull())
          leverage_ratio = ds->FieldByName("leverage_ratio")->AsFloat / 100.0;

       if (! ds->FieldValues["cof"].IsNull())
          cof = ds->FieldByName("cof")->AsFloat / 100.0;

       if (! ds->FieldValues["roe"].IsNull())
          roe = ds->FieldByName("roe")->AsFloat / 100.0;

       if (! ds->FieldValues["query_fee"].IsNull())
          query_fee = ds->FieldByName("query_fee")->AsFloat;

       if (! ds->FieldValues["commission_ratio"].IsNull())
          commission_ratio = ds->FieldByName("commission_ratio")->AsFloat / 100.0;

       if (! ds->FieldValues["acquisition_data_cost"].IsNull())
          acquisition_data_cost = ds->FieldByName("acquisition_data_cost")->AsFloat;

       if (! ds->FieldValues["acct_mgmt_cost"].IsNull())
          acct_mgmt_cost = ds->FieldByName("acct_mgmt_cost")->AsFloat;

       if (! ds->FieldValues["short_message_expense"].IsNull())
          short_message_expense = ds->FieldByName("short_message_expense")->AsFloat;

       if (! ds->FieldValues["phone_expense_north"].IsNull())
          phone_expense_north = ds->FieldByName("phone_expense_north")->AsFloat;

       if (! ds->FieldValues["phone_expense_south"].IsNull())
          phone_expense_south = ds->FieldByName("phone_expense_south")->AsFloat;

       if (!ds->FieldValues["phone_expense_central"].IsNull())
          phone_expense_central = ds->FieldByName("phone_expense_central")->AsFloat;

       if (! ds->FieldValues["legal_exec_north"].IsNull())
          legal_exec_north = ds->FieldByName("legal_exec_north")->AsFloat;

       if (! ds->FieldValues["legal_query_north"].IsNull())
          legal_query_north = ds->FieldByName("legal_query_north")->AsFloat;

       if (! ds->FieldValues["legal_auction_north"].IsNull())
          legal_auction_north = ds->FieldByName("legal_auction_north")->AsFloat;

       if (! ds->FieldValues["legal_staff_north"].IsNull())
          legal_staff_north = ds->FieldByName("legal_staff_north")->AsFloat;

       if (! ds->FieldValues["legal_exec_south"].IsNull())
          legal_exec_south = ds->FieldByName("legal_exec_south")->AsFloat;

       if (! ds->FieldValues["legal_query_south"].IsNull())
          legal_query_south = ds->FieldByName("legal_query_south")->AsFloat;

       if (! ds->FieldValues["legal_auction_south"].IsNull())
          legal_auction_south = ds->FieldByName("legal_auction_south")->AsFloat;

       if (! ds->FieldValues["legal_staff_south"].IsNull())
          legal_staff_south = ds->FieldByName("legal_staff_south")->AsFloat;

       if (! ds->FieldValues["legal_exec_central"].IsNull())
          legal_exec_central = ds->FieldByName("legal_exec_central")->AsFloat;

       if (! ds->FieldValues["legal_query_central"].IsNull())
          legal_query_central = ds->FieldByName("legal_query_central")->AsFloat;

       if (! ds->FieldValues["legal_auction_central"].IsNull())
          legal_auction_central = ds->FieldByName("legal_auction_central")->AsFloat;

       if (! ds->FieldValues["legal_staff_central"].IsNull())
          legal_staff_central = ds->FieldByName("legal_staff_central")->AsFloat;

       if (!ds->FieldValues["legal_detain_ratio_north"].IsNull())
          legal_detain_ratio_north = ds->FieldByName("legal_detain_ratio_north")->AsFloat / 100.0;

       if (!ds->FieldValues["legal_detain_ratio_south"].IsNull())
          legal_detain_ratio_south = ds->FieldByName("legal_detain_ratio_south")->AsFloat / 100.0;

       if (!ds->FieldValues["legal_detain_ratio_central"].IsNull())
          legal_detain_ratio_central = ds->FieldByName("legal_detain_ratio_central")->AsFloat / 100.0;

       if (! ds->FieldValues["recovery_ratio"].IsNull())
          recovery_ratio = ds->FieldByName("recovery_ratio")->AsFloat / 100.0;

       if (!ds->FieldValues["legal_action_period"].IsNull())
          legal_action_period = ds->FieldByName("legal_action_period")->AsFloat;
    }
 } catch (Exception &E) {
    throw;
 }
}
//---------------------------------------------------------------------------
/*
double Loan::get_pd(char *idn, TADOHandler *handler)
{
 Variant hostVars[5];
 double pb;

  try {
    hostVars[0] = idn;
    handler->ExecSQLQry(SQLCommands[Get_Ploan_PD], hostVars, 0, ds);
    record_count = ds->RecordCount;

    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["pd"].IsNull())
          pb = ds->FieldByName("pd")->AsFloat;
       if (! ds->FieldValues["rscore"].IsNull())
          rscore = ds->FieldByName("rscore")->AsFloat;
    }
    if (rscore == 101)
        throw (RiskEx ("�H�u�f�� [�LJCIC���]", 101));
    else if (rscore == 103)
        throw (RiskEx ("�ڵ� [���j�H�η岫�O��]", 103));
    else if (rscore == 104)
        throw (RiskEx ("�ڵ� [���j�H�η岫�O��]", 104));
    else if (rscore == 105)
        throw (RiskEx ("�ڵ� [���j�H�η岫�O��]", 105));
    else if (rscore == 106)
        throw (RiskEx ("�ڵ� [���j�H�η岫�O��]", 106));
    else if (rscore == 107)
        throw (RiskEx ("�ڵ� [���j�H�η岫�O��]", 107));
    else if (rscore == 102)
        throw (RiskEx ("�H�u�f�� [JCIC��Ƥ���]", 102));
    pd = pb;
 } catch (Exception &E) {
    throw;
 }
 return (pb);
}
*/