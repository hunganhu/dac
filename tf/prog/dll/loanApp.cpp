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

       if (! ds->FieldValues["cof"].IsNull())
          cof = ds->FieldValues["cof"];
       else
          cof_ind = -1;

       if (! ds->FieldValues["roe"].IsNull())
          roe = ds->FieldValues["roe"];
       else
          roe_ind = -1;

       if (! ds->FieldValues["ts_tax_rate"].IsNull())
          ts_tax_rate = ds->FieldValues["ts_tax_rate"];
       else
          ts_tax_rate_ind = -1;

       if (! ds->FieldValues["tf_tax_rate"].IsNull())
          tf_tax_rate = ds->FieldValues["tf_tax_rate"];
       else
          tf_tax_rate_ind = -1;

       if (! ds->FieldValues["info_processing_cost"].IsNull())
          info_processing_cost = ds->FieldValues["info_processing_cost"];
       else
          info_processing_cost_ind = -1;

       if (! ds->FieldValues["operation_cost"].IsNull())
          operation_cost = ds->FieldValues["operation_cost"];
       else
          operation_cost_ind = -1;

       if (! ds->FieldValues["hr_cost"].IsNull())
          hr_cost = ds->FieldValues["hr_cost"];
       else
          hr_cost_ind = -1;
    }

  if (record_count == 0) {
     throw DataEx("無申請件資料。");
  }

  if ((product_type_ind == -1) || (product_type < 1) || (product_type > 2)) {
     Message += "產品代號錯誤，必須是1(國民信貸), 2(卡好借)。";
     success = false;
  }

  if ((gender_ind == -1) || (gender < 0) || (gender > 1)) {
     Message += "性別錯誤，必須是0(女性), 1(男性)。";
     success = false;
  }

  if ((secretive_ind == -1) || (secretive < 0) || (secretive > 1)) {
     Message += "密家人錯誤，必須是0(否), 1(是)。";
     success = false;
  }

  if ((edu_ind == -1) || (edu < 1) || (edu > 6)) {
     Message += "教育程度錯誤，必須是1(研究所(含)以上), 2(大學), 3(專科), 4(高中(職)), 5(國中以下), 6(其他)。";
     success = false;
  }

  if ((marriage_status_ind == -1) || (marriage_status < 1) || (marriage_status > 4)) {
     Message += "婚姻狀況錯誤，必須是1(已婚), 2(未婚), 3(離婚), 4(其他)。";
     success = false;
  }

  if ((cof_ind == -1) || (cof < 0.0) || (cof > 1.0)) {
     Message += "資金成本必須介於 0% 和 100%。";
     success = false;
  }

  if ((roe_ind == -1) || (roe < 0.0) || (roe > 1.0)) {
     Message += "股東權益報酬必須介於 0% 和 100%。";
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

       if (!ds->FieldValues["risk_level"].IsNull())
          risk_level = ds->FieldValues["risk_level"];
       else
          risk_level_ind = -1;

    }
  if (trial_count == 0) {
     throw DataEx("無貸款資料。");
  }
  if ((principal_ind == -1) || (principal < 10000.0) || (principal > 9000000.0)) {
     Message += "申貸金額必須介於10,000和9,000,000（元）。";
     success = false;
  }

  if ((int_rate_ind == -1) || (int_rate < 0.0)
        ||(int_rate > 0.20)) {
       Message += "貸款利率錯誤，必須介於0和20(%)。";
       success = false;
  }

  if ((periods_ind == -1) || (periods < 1) || (periods > 120)) {
     Message += "貸款期數錯誤，必須介於1和120。";
     success = false;
  }

  if ((application_fee_ind == -1) || (application_fee < 0) || (application_fee > 100000)) {
     Message += "開辦費必須介於0和100000（元）。";
     success = false;
  }

  if ((credit_checking_fee_ind == -1) || (credit_checking_fee < 0) || (credit_checking_fee > 100000)) {
     Message += "徵信查詢費必須介於0和100000（元）。";
     success = false;
  }

  if ((risk_mgmt_fee_ind == -1) || (risk_mgmt_fee < 0) || (risk_mgmt_fee > 100000)) {
     Message += "每期風險管理費必須介於0和100000（元）。";
     success = false;
  }

  if ((risk_mgmt_fee_terms_ind == -1) || (risk_mgmt_fee_terms < 0) || (risk_mgmt_fee_terms > periods)) {
     Message += "風險管理費用收取期數錯誤，必須介於0和貸款期數。";
     success = false;
  }

  if ((teaser_rate_ind == -1) || (teaser_rate < 0.0)
        ||(teaser_rate > int_rate)) {
       Message += "優惠年利率錯誤，必須介於0和貸款利率。";
       success = false;
  }

  if ((teaser_period_ind == -1) || (teaser_period < 0) || (teaser_period > periods)) {
     Message += "優惠期錯誤，必須介於0和貸款期數。";
     success = false;
  }

  if ((grace_period_ind == -1) || (grace_period < 0) || (grace_period > periods)) {
     Message += "寬限期錯誤，必須介於0和貸款期數。";
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
 int jas002_defect, fs044, app_max_bucket, cash_max_bucket, delinquent_months;

 jcic_date = inquiry_date;
// avail_flag = jas002_defect = krm001_hit = krm023_hit = fs044 = 0;
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
    handler->ExecSQLQry("select jas002_defect, app_max_bucket, fs044, fs334, fs302 from tf_ploan_cal",
                        ds);
    ds->First();
    if (!ds->Eof) {
       jas002_defect = ds->FieldValues["jas002_defect"];
       app_max_bucket = ds->FieldValues["app_max_bucket"];
       fs044 = ds->FieldValues["fs044"];
       delinquent_months = ds->FieldValues["fs334"];
       cash_max_bucket = ds->FieldValues["fs302"];
    }

    if (jas002_defect > 0) {
       Message = "拒絕 [有退票強停拒往授信異常等記錄]"; code = 103;
    }
    if (app_max_bucket > 3) {
       Message = "拒絕 [信用卡有90天以上遲繳記錄]"; code = 104;
    }
    if (fs044 > 0) {
       Message = "拒絕 [貸款有遲繳記錄]"; code = 105;
    }
    if (cash_max_bucket > 0) {
       Message = "拒絕 [現金卡前期有遲繳記錄]"; code = 106;
    }
    if (delinquent_months > 3) {
       Message = "拒絕 [貸款有90天以上遲繳記錄]"; code = 107;
    }
 } catch (Exception &E) {
     throw;
   }
}

//---------------------------------------------------------------------------
void Loan::calculate_rscore(TADOHandler *handler)
{
 Variant hostVars[5];
 float krm001_hit, krm023_hit, bam085_hit, ind001, ms080;

 try {
    hostVars[0] = app_sn;
    hostVars[1] = ts_date;
    handler->ExecSQLCmd(SQLCommands[Calculate_Loan_Del_Number], hostVars, 1);

    hostVars[0] = app_sn;
    handler->ExecSQLQry(SQLCommands[Get_Branch_Ind], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
       ind001 = ds->FieldValues["ind001"];
       krm001_hit = ds->FieldValues["krm001_hit"];
       krm023_hit = ds->FieldValues["krm023_hit"];
       bam085_hit = ds->FieldValues["krm023_hit"];
       ms080 = ds->FieldValues["ms080"];
    }
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
}
//---------------------------------------------------------------------------
void Loan::calculate_pd(TADOHandler *handler)
{
 Variant hostVars[5];
 double mp_r, amortization_rate, ln001;
 int    ms082, score_card, index, jindex;
 try {
    hostVars[0] = app_sn;
    handler->ExecSQLQry(SQLCommands[Get_PB_Input], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
       score_card = ds->FieldValues["score_card"];
       switch (score_card) {
          case 0: // screen out
                 pd = 0.9;
                 break;
          case 1: // A2, full JCIC
                 index = ds->FieldValues["twentile"];
                 ms082 = ds->FieldValues["ms082"];
                 ln001 = ds->FieldValues["LN001_12m_r_tran3"];
                 if (ln001 <= 0)            jindex = 0;
                 else if (ln001 <= 15.61)   jindex = 1;
                 else if (ln001 <= 31.497)  jindex = 2;
                 else if (ln001 <= 38.199)  jindex = 3;
                 else if (ln001 <= 43.051)  jindex = 4;
                 else if (ln001 <= 47.65)   jindex = 5;
                 else if (ln001 <= 51.463)  jindex = 6;
                 else if (ln001 <= 55.759)  jindex = 7;
                 else if (ln001 <= 59.639)  jindex = 8;
                 else if (ln001 <= 63.923)  jindex = 9;
                 else if (ln001 <= 68.975)  jindex = 10;
                 else if (ln001 <= 74.229)  jindex = 11;
                 else if (ln001 <= 79.992)  jindex = 12;
                 else if (ln001 <= 86.552)  jindex = 13;
                 else if (ln001 <= 96.399)  jindex = 14;
                 else if (ln001 <= 110.635) jindex = 15;
                 else if (ln001 <= 133.281) jindex = 16;
                 else                       jindex = 17;
                 pd =  a2_PB[index][jindex];
                 break;
          case 2: // B1 ms080 > 0
                 index = ds->FieldValues["twentile"];
                 pd =  b1_PB[index];
                 break;
          case 3: // B2 ms080 <= 0
                 index = ds->FieldValues["twentile"];
                 ms082 = ds->FieldValues["ms082"];
                 amortization_rate = ds->FieldValues["amoritization_rate"];
                 mp_r = pow((principal * amortization_rate + ms082 * 1000.0), 0.5);
                 if (mp_r <= 0)            jindex = 0;
                 else if (mp_r <= 34.986)  jindex = 1;
                 else if (mp_r <= 56.385)  jindex = 2;
                 else if (mp_r <= 62.896)  jindex = 3;
                 else if (mp_r <= 63.04)   jindex = 4;
                 else if (mp_r <= 69.507)  jindex = 5;
                 else if (mp_r <= 72.327)  jindex = 6;
                 else if (mp_r <= 74.065)  jindex = 7;
                 else if (mp_r <= 74.663)  jindex = 8;
                 else if (mp_r <= 81.244)  jindex = 9;
                 else if (mp_r <= 83.254)  jindex = 10;
                 else if (mp_r <= 87.461)  jindex = 11;
                 else if (mp_r <= 101.572) jindex = 12;
                 else                      jindex = 13;
                 pd =  b2_PB[index][jindex];
                 break;
          case 4: // Demographic
                 index = ds->FieldValues["twentile"];
                 pd =  demo_PB[index];
                 break;
       }
    hostVars[0] = pd;
    hostVars[1] = app_sn;
    handler->ExecSQLCmd(SQLCommands[Write_PB_Result], hostVars, 1);
    }
 } catch (Exception &E) {
     throw;
 }
}
//---------------------------------------------------------------------------
double Loan::calculate_pb(int line, int index, double amortization_rate, int ms082, int score_card)
{
 double mp_r, pb;
 int    jindex;
 try {
       switch (score_card) {
          case 0: // screen out
                 pb = 0.9;
                 break;
          case 1: // A2, full JCIC
                 mp_r = pow((line * amortization_rate + ms082 * 1000.0), 0.5);
                 if (mp_r <= 0)            jindex = 0;
                 else if (mp_r <= 15.61)   jindex = 1;
                 else if (mp_r <= 31.497)  jindex = 2;
                 else if (mp_r <= 38.199)  jindex = 3;
                 else if (mp_r <= 43.051)  jindex = 4;
                 else if (mp_r <= 47.65)   jindex = 5;
                 else if (mp_r <= 51.463)  jindex = 6;
                 else if (mp_r <= 55.759)  jindex = 7;
                 else if (mp_r <= 59.639)  jindex = 8;
                 else if (mp_r <= 63.923)  jindex = 9;
                 else if (mp_r <= 68.975)  jindex = 10;
                 else if (mp_r <= 74.229)  jindex = 11;
                 else if (mp_r <= 79.992)  jindex = 12;
                 else if (mp_r <= 86.552)  jindex = 13;
                 else if (mp_r <= 96.399)  jindex = 14;
                 else if (mp_r <= 110.635) jindex = 15;
                 else if (mp_r <= 133.281) jindex = 16;
                 else                      jindex = 17;
                 pb =  a2_PB[index][jindex];
                 break;
          case 2: // B1 ms080 > 0
                 pb =  b1_PB[index];
                 break;
          case 3: // B2 ms080 <= 0
                 mp_r = pow((line * amortization_rate + ms082 * 1000.0), 0.5);
                 if (mp_r <= 0)            jindex = 0;
                 else if (mp_r <= 34.986)  jindex = 1;
                 else if (mp_r <= 56.385)  jindex = 2;
                 else if (mp_r <= 62.896)  jindex = 3;
                 else if (mp_r <= 63.04)   jindex = 4;
                 else if (mp_r <= 69.507)  jindex = 5;
                 else if (mp_r <= 72.327)  jindex = 6;
                 else if (mp_r <= 74.065)  jindex = 7;
                 else if (mp_r <= 74.663)  jindex = 8;
                 else if (mp_r <= 81.244)  jindex = 9;
                 else if (mp_r <= 83.254)  jindex = 10;
                 else if (mp_r <= 87.461)  jindex = 11;
                 else if (mp_r <= 101.572) jindex = 12;
                 else                      jindex = 13;
                 pb =  b2_PB[index][jindex];
                 break;
          case 4: // Demographic
                 pb =  demo_PB[index];
                 break;
       }
 } catch (Exception &E) {
     throw;
 }
 return (pb);
}
//---------------------------------------------------------------------------
/*
void Loan::postFilter()
{
 if (jas002_defect > 0)
    throw (RiskEx ("拒絕 [有退票強停拒往授信異常等記錄]", 103));
 else if (app_max_bucket > 3)
    throw (RiskEx ("拒絕 [信用卡有90天以上遲繳記錄]", 104));
 else if (fs044 > 0)
    throw (RiskEx ("拒絕 [貸款有遲繳記錄]", 105));
 else if (cash_max_bucket > 0)
    throw (RiskEx ("拒絕 [現金卡前期有遲繳記錄]", 106));
 else if (delinquent_months > 3)
    throw (RiskEx ("拒絕 [貸款有90 天以上遲繳記錄]", 107));
}
*/
//---------------------------------------------------------------------------
void Loan::calculate_npv()
{
  double revenue, cost, wc;
  double Interest_Revenue, Late_Fee, Open_Credit_Fee, Risk_Management_Fee;
  double Interest_Cost, Acct_Mgmt_Cost, Taishin_Corp_Tax, TF_Corp_Tax;
  double Collection_Cost, Commission, Working_Capital, Credit_Loss;

  npv_init();
  set_apr();
  set_attrition();
  set_annuity();  // 本息法
  Commission =  calculate_commission();
  if (Commission < 0) Commission = 0;
  // Revenue
  Interest_Revenue = set_interest_revenue();
  Late_Fee = set_late_fee();
  Open_Credit_Fee = set_open_credit_revenue();
  Risk_Management_Fee = set_risk_mgmt_revenue();

  // Cost
  Interest_Cost = set_interest_cost();
  Acct_Mgmt_Cost = set_account_management_cost();
  Taishin_Corp_Tax  = set_taishin_tax();
  TF_Corp_Tax = set_tf_tax();
//  PreCollection_Cost = set_precollection_cost();
  Collection_Cost = set_collection_cost();
//  Working_Capital = set_working_capital();
  Credit_Loss = set_credit_loss();
  total_npv = (Interest_Revenue + Late_Fee + Open_Credit_Fee
               + Risk_Management_Fee)                            // Revenue
               - (Interest_Cost + Acct_Mgmt_Cost + Taishin_Corp_Tax + TF_Corp_Tax
                  + Collection_Cost + Commission + Credit_Loss);         // Cost

#ifdef _WRFLOW
     fstream outf;
     outf.open("NPV_flows.txt", ios::app | ios::out);  // Open for ouput and append

     outf << "Case SN: " << app_sn.c_str() << "   appDate: " << app_date.c_str()
          << "   jcicDate: " << jcic_date.c_str() << "   Commission: " << Commission << endl;
     outf << "TERM       APR  Open_Atr   Vol_Atr Invol_Atr  bad/open"
          << "        OsPrinL        P_Repay        I_Repay         IntRev          LateF"
          << " O_R_Fee  RiskF        IntCost       AcctCost     TaishinTax          TFTax"
          << "        Collect     CreditLoss"  << endl;
     outf << "==============================================================================================="
          << "======================================================================"
          << "======================================================================" << endl;
     outf <<  showpoint
              << setw(99) << setprecision(8) << "  "
              << setw(15) << setprecision(8) << Interest_Revenue
              << setw(15) << setprecision(8) << Late_Fee
              << setw(7)  << setprecision(6) << Open_Credit_Fee
              << setw(8)  << setprecision(7) << Risk_Management_Fee
              << setw(15) << setprecision(8) << Interest_Cost
              << setw(15) << setprecision(8) << Acct_Mgmt_Cost
              << setw(15) << setprecision(8) << Acct_Mgmt_Cost
              << setw(15) << setprecision(8) << Taishin_Corp_Tax
              << setw(15) << setprecision(8) << TF_Corp_Tax
              << setw(15) << setprecision(8) << Collection_Cost
              << setw(15) << setprecision(8) << Credit_Loss
              << endl;
     for (int i = 0; i < periods; i++) {
         outf << setprecision(8) << showpoint
              << setw(4)  << i
              << setw(10) << setprecision(4) << apr[i]
              << setw(10) << setprecision(4) << open_attrition[i]
              << setw(10) << setprecision(4) << voluntary_attrition[i]
              << setw(10) << setprecision(4) << involuntary_attrition[i]
              << setw(10) << setprecision(4) << bad_per_open[i]
              << setw(15) << setprecision(8) << os_principal[i]
              << setw(15) << setprecision(8) << principal_repayment[i]
              << setw(15) << setprecision(8) << interest_repayment[i]
              << setw(15) << setprecision(8) << interest_revenue[i]
              << setw(15) << setprecision(8) << late_fee[i]
              << setw(7)  << setprecision(6) << open_credit_fee[i]
              << setw(8)  << setprecision(7) << risk_mgmt_revenue[i]
              << setw(15) << setprecision(8) << interest_cost[i]
              << setw(15) << setprecision(8) << account_management_cost[i]
              << setw(15) << setprecision(8) << taishin_tax[i]
              << setw(15) << setprecision(8) << tf_tax[i]
              << setw(15) << setprecision(8) << collection_cost[i]
              << setw(15) << setprecision(8) << credit_loss[i]
              << endl;
     }

#endif

}
//---------------------------------------------------------------------------
void Loan::npv_init()
{
  for (int i = 0; i < TERM + 4; i ++) {
     apr[i] = 0.0;
     open_attrition[i] = 0.0;
     voluntary_attrition[i] = 0.0;
     involuntary_attrition[i] = 0.0;
     open_credit_fee[i] = 0.0;
     risk_mgmt_revenue[i] = 0.0;
     os_principal[i] = 0.0;
     principal_repayment[i] = 0.0;
     interest_repayment[i] = 0.0;
     interest_revenue[i] = 0.0;
     late_fee[i] = 0.0;
     open_credit_fee[i] = 0.0;
     risk_mgmt_revenue[i] = 0.0;
     taishin_tax[i] = 0.0;
     tf_tax[i] = 0.0;
     interest_cost[i] = 0.0;
     account_management_cost[i] = 0.0;
     collection_cost[i] = 0.0;
//     working_capital[i] = 0.0;
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
double Loan::get_GX_adjustment(double annual_pb)
{
  return ( 17.901 * pow(annual_pb, 6) +
	  -29.601 * pow(annual_pb, 5) +
	   19.651 * pow(annual_pb, 4) +
	  -5.4726 * pow(annual_pb, 3) +
	   1.3014 * pow(annual_pb, 2) +
	    0.701 * annual_pb + 1.5354);
}

//---------------------------------------------------------------------------
double Loan::get_KHJ_adjustment(double annual_pb)
{
  return (-24.366 * pow(annual_pb, 6) +
	   44.271 * pow(annual_pb, 5) +
	   -29.34 * pow(annual_pb, 4) +
	   9.6136 * pow(annual_pb, 3) +
	  -0.9023 * pow(annual_pb, 2) +
	   0.7614 * annual_pb + 1.4702);

}
//---------------------------------------------------------------------------
void Loan::set_attrition()
{
  double monthly_pd = pd / 12.0;
  double adjustment;

  // product_type: 1 for GX, 2 for KHJ
  switch (product_type - 1) {
     case GX: adjustment = get_GX_adjustment(pd);
              break;
     case KHJ: adjustment = get_KHJ_adjustment(pd);
  };

  for (int i = 0; i <= periods; i++) {
      if (i >= 8)
         bad_per_open[i] = monthly_pd * adjustment;
      else
         bad_per_open[i] = monthly_pd * adjustment * i / 8.0;
  }

  open_attrition[0] = 1.0;
  voluntary_attrition[0] = involuntary_attrition[0] = 0.0;
  for (int i = 1; i <= periods; i++) {
      voluntary_attrition[i] = open_attrition[i-1] * vol_attrition_open[product_type - 1][i];
      involuntary_attrition[i] = open_attrition[i-1] * bad_per_open[i];
      open_attrition[i] = open_attrition[i-1] - voluntary_attrition[i] - involuntary_attrition[i];
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
double Loan::set_open_credit_revenue()
{
 open_credit_fee[0] = application_fee + credit_checking_fee;
 return (open_credit_fee[0]);
}
//---------------------------------------------------------------------------
double Loan::set_risk_mgmt_revenue()
{
  if (risk_mgmt_fee_terms == 0)
     risk_mgmt_revenue[0] = risk_mgmt_fee;
  else
     for (int i = 1; i <= risk_mgmt_fee_terms; i++)
         risk_mgmt_revenue[i] = risk_mgmt_fee;

  return (NetPresentValue(roe / 12.0, risk_mgmt_revenue + 1, risk_mgmt_fee_terms, ptEndOfPeriod)
          + risk_mgmt_revenue[0]);
}
//---------------------------------------------------------------------------
double Loan::set_taishin_tax()
{
  for (int i = 0; i <= periods; i++)
     taishin_tax[i] = (interest_revenue[i] + open_credit_fee[i] + risk_mgmt_revenue[i]
                       + late_fee[i]) * ts_tax_rate;

  return (NetPresentValue(roe / 12.0, taishin_tax + 1, periods, ptEndOfPeriod)
          + taishin_tax[0]);
}
//---------------------------------------------------------------------------
double Loan::set_tf_tax()
{
  for (int i = 0; i <= periods; i++)
     tf_tax[i] = (interest_revenue[i] + open_credit_fee[i] + risk_mgmt_revenue[i]
              + late_fee[i] - interest_cost[i] - account_management_cost[i]
              - taishin_tax[i]) * tf_tax_rate;

  return (NetPresentValue(roe / 12.0, tf_tax + 1, periods, ptEndOfPeriod)
          + tf_tax[0]);
}
//---------------------------------------------------------------------------

double Loan::set_late_fee()
{
  double monthly_pd = pd / 12.0;

  for (int i = 1; i <= periods; i++)
     if (i < 2)
        late_fee[i] = 0.0;
     else
        late_fee[i] = interest_revenue[i-1] * open_attrition[i] *  monthly_pd
                      * LATE_30D_RATIO * LATE_PENALTY_RATIO;

  return (NetPresentValue(roe / 12.0, late_fee + 1, periods, ptEndOfPeriod)
          + late_fee[0]);
}

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
 int channel = sales_channel.ToInt();
 int apr_group, line_grp;
 double point_cost;       //記點成本
 double transfer_bonus;   //轉介獎金
 double out_source_fee;   //委外佣金
 double out_source_bonus; //委外銷售獎金
 double head_bonus;       //主管手續獎金
 double sales_bonus;      //業務手續獎金

 if (int_rate <= 0.15) apr_group = 10;
 else if (int_rate <= 0.155) apr_group = 9;
 else if (int_rate <= 0.16)  apr_group = 8;
 else if (int_rate <= 0.165) apr_group = 7;
 else if (int_rate <= 0.17)  apr_group = 6;
 else if (int_rate <= 0.175) apr_group = 5;
 else if (int_rate <= 0.18)  apr_group = 4;
 else if (int_rate <= 0.185) apr_group = 3;
 else if (int_rate <= 0.19)  apr_group = 2;
 else if (int_rate <= 0.195) apr_group = 1;
 else apr_group = 0;

 switch (product_type - 1) {
    case GX: if (principal <= 150000) line_grp = 0;
             else if (principal <= 200000) line_grp = 1;
             else if (principal <= 250000) line_grp = 2;
             else if (principal <= 300000) line_grp = 3;
             else if (principal <= 350000) line_grp = 4;
             else if (principal <= 400000) line_grp = 5;
             else if (principal <= 450000) line_grp = 6;
             else if (principal <= 500000) line_grp = 7;
             else if (principal <= 550000) line_grp = 8;
             else line_grp = 9;
             point_cost = VariableCommission[GX][channel][apr_group] //記點成本
                          * risk_mgmt_fee
                          / GX_RiskMgmtFee[line_grp][risk_level - 1]
                          * SalesPointCost[GX][channel];
             if (channel == 3 || channel == 4 || channel == 5) // 轉介獎金
                transfer_bonus = GX_TRANSFER_BONUS;
             else
                transfer_bonus = 0.0;

             if (channel == 9 ) {
                out_source_fee = principal * GX_OUT_SOURCE_RATE; // 委外佣金
                out_source_bonus = GX_OUT_SOURCE_BONUS; //委外銷售獎金
             }
             else
                out_source_fee = out_source_bonus = 0.0;

             if (HeadBonusDiscount [GX][channel] == 1)       // 主管手續獎金
                head_bonus = application_fee / GX_APP_FEE_RECEIVABLE
                             * HeadFeeBonus [GX][channel];
             else head_bonus = 0.0;

             if (SalesBonusDiscount [GX][channel] == 1)      // 業務手續獎金
                sales_bonus = application_fee / GX_APP_FEE_RECEIVABLE
                             * SalesFeeBonus [GX][channel];
             else sales_bonus = 0.0;

             break;
    case KHJ:if (principal <= 100000) line_grp = 0;
             else if (principal <= 120000) line_grp = 1;
             else line_grp = 2;
             point_cost = VariableCommission[KHJ][channel][apr_group] //記點成本
                          * risk_mgmt_fee
                          / KHJ_RiskMgmtFee[line_grp]
                          * SalesPointCost[KHJ][channel];
             if (channel == 3 || channel == 4 || channel == 5) // 轉介獎金
                transfer_bonus = KHJ_TRANSFER_BONUS;
             else
                transfer_bonus = 0.0;

             if (channel == 9 ) {
                out_source_fee = principal * KHJ_OUT_SOURCE_RATE; // 委外佣金
                out_source_bonus = KHJ_OUT_SOURCE_BONUS; //委外銷售獎金
             }
             else
                out_source_fee = out_source_bonus = 0.0;

             if (HeadBonusDiscount [KHJ][channel] == 1)       // 主管手續獎金
                head_bonus = application_fee / KHJ_APP_FEE_RECEIVABLE
                             * HeadFeeBonus [KHJ][channel];
             else head_bonus = 0.0;

             if (SalesBonusDiscount [KHJ][channel] == 1)      // 業務手續獎金
                sales_bonus = application_fee / KHJ_APP_FEE_RECEIVABLE
                             * SalesFeeBonus [KHJ][channel];
             else sales_bonus = 0.0;

             break;
 }

  return (point_cost + transfer_bonus + out_source_fee + out_source_bonus
          + head_bonus + sales_bonus);
}
// Account Management Cost
double Loan::set_account_management_cost()
{
  for (int i = 1; i <= periods; i++)
     account_management_cost[i] = (info_processing_cost + operation_cost + hr_cost)
                                  * open_attrition[i-1];
  return (NetPresentValue(roe / 12.0, account_management_cost + 1, periods, ptEndOfPeriod)
          + account_management_cost[0]);
}

//---------------------------------------------------------------------------
// Collection Cost (legal)
double Loan::set_collection_cost()
{
  double monthly_pd = pd / 12.0;

  for (int i = 1; i <= periods; i++)
      collection_cost[i] = D1_FIX_COST * open_attrition[i] * monthly_pd *
                           D1_WRITEOFF;
  return (NetPresentValue(roe / 12.0, collection_cost + 1, periods, ptEndOfPeriod)
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
/*
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
*/
//---------------------------------------------------------------------------
// Credit loss:
double Loan::set_credit_loss()
{
 double discount_rate = 1.0 / pow((1 + roe / 12.0), 10);
 credit_loss [0] = 0.0;
 for (int i = 1; i <= periods; i++) {
    credit_loss [i] = (os_principal[i-1]+ interest_revenue[i-1] * 3)* involuntary_attrition[i] *
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
       throw DataEx("維護表格必須只有一筆資料。");
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
/*
       if (! ds->FieldValues["leverage_ratio"].IsNull())
          leverage_ratio = ds->FieldByName("leverage_ratio")->AsFloat / 100.0;
*/
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
/*
       if (! ds->FieldValues["acct_mgmt_cost"].IsNull())
          acct_mgmt_cost = ds->FieldByName("acct_mgmt_cost")->AsFloat;
*/
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
/*
       if (! ds->FieldValues["recovery_ratio"].IsNull())
          recovery_ratio = ds->FieldByName("recovery_ratio")->AsFloat / 100.0;
*/
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
        throw (RiskEx ("人工審核 [無JCIC資料]", 101));
    else if (rscore == 103)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 103));
    else if (rscore == 104)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 104));
    else if (rscore == 105)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 105));
    else if (rscore == 106)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 106));
    else if (rscore == 107)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 107));
    else if (rscore == 102)
        throw (RiskEx ("人工審核 [JCIC資料不足]", 102));
    pd = pb;
 } catch (Exception &E) {
    throw;
 }
 return (pb);
}
*/
