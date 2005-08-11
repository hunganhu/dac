//---------------------------------------------------------------------------
#pragma hdrstop

#include "loanApp.h"
#include <Math.hpp>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "ploanSQL.h"
#include "functions.h"
#include "economic.h"
// Constants used in the program.
const char *expire_date = "21001231";
const double m1_to_m7_ratio = 5.0;

//---------------------------------------------------------------------------

#pragma package(smart_init)

Loan::Loan (char *appSN, char *appDate):
    app_sn(appSN), app_date(appDate), product_type_ind(0),
    gender_ind(0), zip_ind(0), secretive_ind(0), edu_ind(0),
    marriage_status_ind(0), alien_ind(0), age_ind(0), cashcard_lock_ind(0),
    cof_ind(0), roe_ind(0), ts_tax_rate_ind(0), tf_tax_rate_ind(0),
    info_processing_cost_ind(0), operation_cost_ind(0), hr_cost_ind(0),
    principal_ind(0), int_rate_ind(0), teaser_rate_ind(0), periods_ind(0),
    teaser_period_ind(0), grace_period_ind(0), application_fee_ind(0),
    credit_checking_fee_ind(0), risk_mgmt_fee_ind(0), risk_mgmt_fee_terms_ind(0),
    sales_channel_ind(0), risk_level_ind(0), code(0)
{
}
//---------------------------------------------------------------------------
Loan::Loan (char *appSN, char *appDate, char *tsDate, char *jcicDate, char *tsn):
    app_sn(appSN), app_date(appDate), ts_date(tsDate), jcic_date(jcicDate),tsn(tsn),product_type_ind(0),
    gender_ind(0), zip_ind(0), secretive_ind(0), edu_ind(0),
    marriage_status_ind(0), alien_ind(0), age_ind(0), cashcard_lock_ind(0),
    cof_ind(0), roe_ind(0), ts_tax_rate_ind(0), tf_tax_rate_ind(0),
    info_processing_cost_ind(0), operation_cost_ind(0), hr_cost_ind(0),
    principal_ind(0), int_rate_ind(0), teaser_rate_ind(0), periods_ind(0),
    teaser_period_ind(0), grace_period_ind(0), application_fee_ind(0),
    credit_checking_fee_ind(0), risk_mgmt_fee_ind(0), risk_mgmt_fee_terms_ind(0),
    sales_channel_ind(0), risk_level_ind(0), code(0)
{
}
//---------------------------------------------------------------------------
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
void Loan::app_info_validate(char * appNo, char* appDate, TADOHandler *handler)
{
 Variant hostVars[5];
 bool success = true;
 TADODataSet *ds = new TADODataSet(NULL);

 ds->EnableBCD = false;  // Decimal fields are mapped to float.
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

       if (! ds->FieldValues["alien"].IsNull())
          alien = ds->FieldValues["alien"];
       else
          alien_ind = -1;

       if (! ds->FieldValues["age_over_limit"].IsNull())
          age = ds->FieldValues["age_over_limit"];
       else
          age_ind = -1;

       if (! ds->FieldValues["ts_cashcard_restricted"].IsNull())
          cashcard_lock = ds->FieldValues["ts_cashcard_restricted"];
       else
         cashcard_lock_ind = -1;

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

       if (!ds->FieldValues["risk_level"].IsNull())
          risk_level = ds->FieldValues["risk_level"];
       else
          risk_level_ind = -1;

       if (!ds->FieldValues["sales_channel"].IsNull())
          sales_channel = ds->FieldValues["sales_channel"];
       else
          sales_channel_ind = -1;
    }

  if (record_count == 0) {
     throw DataEx("無申請件資料。", -1);
  }

  if ((product_type_ind == -1) || (product_type < 1) || (product_type > 2)) {
     Message += "305 - 產品代號無法辨識 (正確值為0 或 1)。"; code = 305;
     success = false;
  }

  if ((gender_ind == -1) || (gender < 0) || (gender > 1)) {
     Message += "304 - 無法辨識申請人性別(正確值為0 或 1)。"; code = 304;
     success = false;
  }

  if ( validZIP(zip.c_str()) == 0) {
     Message += "三碼郵遞區號錯誤。";  code = 306;
     success = false;
  }

  if ((secretive_ind == -1) || (secretive < 0) || (secretive > 1)) {
     Message += "307 - 無法辨識是否要密家人(正確值為0 或 1)。"; code = 307;
     success = false;
  }

  if ((edu_ind == -1) || (edu < 1) || (edu > 6)) {
     Message += "309 - 教育程度無法辨識 (正確值為1, 2, 3, 4, 5, 6)。"; code = 309;
     success = false;
  }

  if ((marriage_status_ind == -1) || (marriage_status < 1) || (marriage_status > 4)) {
     Message += "308 - 無法辨識婚姻狀況(正確值為 1, 2, 3, 4)。";  code = 308;
     success = false;
  }

  if ((alien_ind == -1) || (alien < 0) || (alien > 1)) {
     Message += "301 - 無法辨識申請人是否為外國人(正確值為0 或 1)。"; code = 301;
     success = false;
  }

  if ((age_ind == -1) || (age < 0) || (age > 1)) {
     Message += "302 - 無法辨識申請人年紀是否超過55歲(正確值為0 或 1)。"; code = 302;
     success = false;
  }

  if ((cashcard_lock_ind == -1) || (cashcard_lock < 0) || (cashcard_lock > 1)) {
     Message += "303 - 無法辨識是否申請人本行現金卡額度被鎖(正確值為0 或 1)。"; code = 303;
     success = false;
  }

  if ((cof_ind == -1) || (cof < 0.0) || (cof > 1.0)) {
     Message += "322 - 台新金控資金成本小於或等於零。"; code = 322;
     success = false;
  }

  if ((roe_ind == -1) || (roe < 0.0) || (roe > 1.0)) {
     Message += "321 - ROE小於或等於零。"; code = 321;
     success = false;
  }

  if ((ts_tax_rate_ind == -1) || (ts_tax_rate < 0.0) || (ts_tax_rate > 1.0)) {
     Message += "323 - 台新銀行營業稅小於或等於零。"; code = 323;
     success = false;
  }

  if ((tf_tax_rate_ind == -1) || (tf_tax_rate < 0.0) || (tf_tax_rate > 1.0)) {
     Message += "324 - 台新資融營業稅小於或等於零。"; code = 324;
     success = false;
  }

  if ((info_processing_cost_ind == -1) || (info_processing_cost < 0.0)) {
     Message += "325 - 台新資訊室費用小於或等於零。"; code = 325;
     success = false;
  }

  if ((operation_cost_ind == -1) || (operation_cost < 0.0)) {
     Message += "326 - 台新作業與客服費用小於或等於零。"; code = 326;
     success = false;
  }

  if ((hr_cost_ind == -1) || (hr_cost < 0.0)) {
     Message += "327 - 人事成本小於或等於零。"; code = 327;
     success = false;
  }

  if ((risk_level == -1) || (risk_level < 0) || (risk_level > 1) ) {
     Message += "312 - 風險等級無法辨識(正確值為1 或 2)。"; code = 312;
     success = false;
  }

  if ((sales_channel == -1) || (sales_channel < "001") || (sales_channel > "009") ) {
     Message += "311 - 銷售管道無法辨識(正確值為001, 002, 003, 004, 005, 006, 007, 008, 009)。";
     code = 311;
     success = false;
  }
  delete ds;
  if (!success) throw DataEx(Message, code);
 } catch (Exception &E) {
    throw;
 }
}
//---------------------------------------------------------------------------
void Loan::loan_validate(char * appNo, char *tsn, TADOHandler *handler)
{
 Variant hostVars[5];
 bool success = true;
 TADODataSet *ds = new TADODataSet(NULL);

 ds->EnableBCD = false;  // Decimal fields are mapped to float.
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
     }
  if (trial_count == 0) {
     throw DataEx("無貸款資料。", -1);
  }
  if ((principal_ind == -1) || (principal < 10000.0)) {
     Message += "331 - 申貸金額小於或等於零。"; code = 331;
     success = false;
  }

  if ((int_rate_ind == -1) || (int_rate < 0.0)
        ||(int_rate > 0.20)) {
       Message += "332 - 年利率小於或等於零。"; code = 332;
       success = false;
  }

  if ((periods_ind == -1) || (periods < 1) || (periods > 120)) {
     Message += "333 - 期數小於或等於零。";  code = 333;
     success = false;
  }

  if ((application_fee_ind == -1) || (application_fee < 0)) {
     Message += "334 - 開辦費小於零。";  code = 334;
     success = false;
  }

  if ((credit_checking_fee_ind == -1) || (credit_checking_fee < 0)) {
     Message += "335 - 徵信查詢費小於零。";  code = 335;
     success = false;
  }

  if ((risk_mgmt_fee_ind == -1) || (risk_mgmt_fee < 0)) {
     Message += "336 - 每期風險管理費用小於零。";  code = 336;
     success = false;
  }

  if ((risk_mgmt_fee_terms_ind == -1) || (risk_mgmt_fee_terms < 0) || (risk_mgmt_fee_terms > periods)) {
     Message += "337 - 風險管理費用收取期數(月)，必須介於0和貸款期數。";  code = 337;
     success = false;
  }

  if ((teaser_rate_ind == -1) || (teaser_rate < 0.0)
        ||(teaser_rate > int_rate)) {
       Message += "340 - 優惠年利率必須介於0和貸款利率。";  code = 340;
       success = false;
  }

  if ((teaser_period_ind == -1) || (teaser_period < 0) || (teaser_period > periods)) {
     Message += "338 - 優惠期 (月)必須介於0和貸款期數。";   code = 338;
     success = false;
  }

  if ((grace_period_ind == -1) || (grace_period < 0) || (grace_period > periods)) {
     Message += "339 - 寬限期 (月)必須介於0和貸款期數。";  code = 339;
     success = false;
  }

  delete ds;
//  if (!success) throw DataEx(Message, code);
 } catch (Exception &E) {
    throw;
 }
}

//---------------------------------------------------------------------------
Loan::~Loan ()
{
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
 TADODataSet *ds = new TADODataSet(NULL);

 ds->EnableBCD = false;  // Decimal fields are mapped to float.

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
    handler->ExecSQLQry(SQLCommands[Get_Prescreen_Result], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
       jas002_defect = ds->FieldValues["jas002_defect"];
       app_max_bucket = ds->FieldValues["app_max_bucket"];
       fs044 = ds->FieldValues["fs044"];
       delinquent_months = ds->FieldValues["fs334"];
       cash_max_bucket = ds->FieldValues["fs302"];
    }
    if (age == 1) {
       Message = "拒絕 [申請人年齡大於55歲]"; code = 101; }
    else if (alien == 1) {
       Message = "拒絕 [申請人為外國人]"; code = 102; }
    else if ((cashcard_lock == 1) && (product_type == 2)) {
       Message = "拒絕 [申請人為本行現金卡額度被鎖 (僅適用於卡好借)]"; code = 103; }
    else if (jas002_defect > 0) {
       Message = "拒絕 [有退票強停拒往授信異常等記錄]"; code = 104; }
    else if (app_max_bucket > 3) {
       Message = "拒絕 [信用卡有90天以上遲繳記錄]"; code = 105; }
    else if (fs044 > 0) {
       Message = "拒絕 [貸款有遲繳記錄]"; code = 106; }
    else if (cash_max_bucket > 0) {
       Message = "拒絕 [現金卡前期有遲繳記錄]"; code = 107; }
    else if (delinquent_months > 3) {
       Message = "拒絕 [貸款有90天以上遲繳記錄]"; code = 108; }
    delete ds;
 } catch (Exception &E) {
     throw;
   }
}

//---------------------------------------------------------------------------
void Loan::calculate_rscore(TADOHandler *handler)
{
 Variant hostVars[5];
 float krm001_hit, krm023_hit, bam085_hit, ind001, ms080;
 TADODataSet *ds = new TADODataSet(NULL);

 ds->EnableBCD = false;  // Decimal fields are mapped to float.

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
       bam085_hit = ds->FieldValues["bam085_hit"];
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
 double mp_r, amortization_rate, ln001, npv;
 int    score_card, index, jindex;
 double ms082, wi001_12m;
 int    ms082_ind;
 TADODataSet *ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.

 try {
    optimal_line = 0.0;
    hostVars[0] = app_sn;
    handler->ExecSQLQry(SQLCommands[Get_PB_test], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
       score_card = ds->FieldValues["score_card"];
       switch (score_card) {
          case 0: // screen out
                 break;
          case 1: // A2, full JCIC
                 index = ds->FieldValues["twentile_a2"];
                 if (! ds->FieldValues["ms082"].IsNull())
                    ms082 = ds->FieldValues["ms082"];
                 else
                    ms082 = 0.0;
                 wi001_12m = ds->FieldValues["WI001_12m"];
                 break;
          case 2: // B1 ms080 > 0
                 index = ds->FieldValues["twentile_b1"];
                 break;
          case 3: // B2 ms080 <= 0
                 index = ds->FieldValues["brmp_twentile"];
                 break;
          case 4: // Demographic
                 break;
       }
    }
    if (int_rate == 0.0) amortization_rate = 1.0 / periods;
    else amortization_rate =  pow ((1 + (int_rate / 12.0 )), periods) * (int_rate / 12.0 )
                              / (pow ((1 + (int_rate / 12.0 )), periods) - 1);

    int line = principal;
    //    for (int line = start_amount; line <= end_amount; line += increment) {
       switch (score_card) {
          case 0: // screen out
                 pd = 0.9;
                 break;
          case 1: // A2, full JCIC
                 ln001 = pow ((line * amortization_rate + ms082 * 1000.0)
                               / wi001_12m, 0.5);
                 if (ln001 <= 0.0) jindex = 0;
                 else if (ln001 <= 21.3540000000) jindex = 1;
                 else if (ln001 <= 27.3980000000) jindex = 2;
                 else if (ln001 <= 32.2840000000) jindex = 3;
                 else if (ln001 <= 36.1610000000) jindex = 4;
                 else if (ln001 <= 39.9150000000) jindex = 5;
                 else if (ln001 <= 43.5220000000) jindex = 6;
                 else if (ln001 <= 47.1000000000) jindex = 7;
                 else if (ln001 <= 50.6460000000) jindex = 8;
                 else if (ln001 <= 55.1270000000) jindex = 9;
                 else if (ln001 <= 58.8450000000) jindex = 10;
                 else if (ln001 <= 63.0650000000) jindex = 11;
                 else if (ln001 <= 67.7100000000) jindex = 12;
                 else if (ln001 <= 72.8690000000) jindex = 13;
                 else if (ln001 <= 78.8130000000) jindex = 14;
                 else if (ln001 <= 86.7410000000) jindex = 15;
                 else if (ln001 <= 95.9160000000) jindex = 16;
                 else if (ln001 <= 109.3390000000) jindex = 17;
                 else if (ln001 <= 134.5470000000) jindex = 18;
                 else if (ln001 <= 170.8902000000) jindex = 19;
                 else if (ln001 <= 229.0276537120) jindex = 20;
                 else if (ln001 <= 319.9872318933) jindex = 21;
                 else if (ln001 <= 460.4174726738) jindex = 22;
                 else if (ln001 <= 673.9418808429) jindex = 23;
                 else if (ln001 <= 993.5307920375) jindex = 24;
                 else if (ln001 <= 1464.5116054610) jindex = 25;
                 else jindex = 26;
                 pd =  GXa2_PB[index][jindex];
                 break;
          case 2: // B1 ms080 > 0
                 pd =  GXb1_PB[index];
                 break;
          case 3: // B2 ms080 <= 0
                 mp_r = pow((line * amortization_rate + ms082 * 1000.0), 0.5);

                 if (mp_r <= 0 ) jindex = 0;
                 else if (mp_r <= 40.517 ) jindex = 1;
                 else if (mp_r <= 44.789 ) jindex = 2;
                 else if (mp_r <= 49.477 ) jindex = 3;
                 else if (mp_r <= 50.075 ) jindex = 4;
                 else if (mp_r <= 56.812 ) jindex = 5;
                 else if (mp_r <= 57.822 ) jindex = 6;
                 else if (mp_r <= 58.481 ) jindex = 7;
                 else if (mp_r <= 64.647 ) jindex = 8;
                 else if (mp_r <= 64.712 ) jindex = 9;
                 else if (mp_r <= 73.973 ) jindex = 10;
                 else if (mp_r <= 78.992 ) jindex = 11;
                 else if (mp_r <= 86.741 ) jindex = 12;
                 else if (mp_r <= 104.614 ) jindex = 13;
                 else if (mp_r <= 131.054 ) jindex = 14;
                 else if (mp_r <= 175.6339 ) jindex = 15;
                 else if (mp_r <= 248.435144 ) jindex = 16;
                 else if (mp_r <= 364.6396332 ) jindex = 17;
                 else if (mp_r <= 545.8185649 ) jindex = 18;
                 else if (mp_r <= 821.9282779 ) jindex = 19;
                 else if (mp_r <= 1233.779339 ) jindex = 20;
                 else jindex = 21;
                 pd =  GXb2_PB[index][jindex];
                 break;
          case 4: // Demographic
                 pd =  GXdemo_PB[index];
                 break;
       }
// KHJ
       switch (score_card) {
          case 0: // screen out
                 pd = 0.9;
                 break;
          case 1: // A2, full JCIC
                 ln001 = pow ((line * amortization_rate + ms082 * 1000.0)
                               / wi001_12m, 0.5);
                 if      (ln001 <= 0.0)            jindex = 0;
                 else if (ln001 <= 21.3540000000)  jindex = 1;
                 else if (ln001 <= 27.3980000000)  jindex = 2;
                 else if (ln001 <= 32.2840000000)  jindex = 3;
                 else if (ln001 <= 36.1610000000)  jindex = 4;
                 else if (ln001 <= 39.9150000000)  jindex = 5;
                 else if (ln001 <= 43.5220000000)  jindex = 6;
                 else if (ln001 <= 47.1000000000)  jindex = 7;
                 else if (ln001 <= 50.6460000000)  jindex = 8;
                 else if (ln001 <= 55.1270000000)  jindex = 9;
                 else if (ln001 <= 58.8450000000)  jindex = 10;
                 else if (ln001 <= 63.0650000000)  jindex = 11;
                 else if (ln001 <= 67.7100000000)  jindex = 12;
                 else if (ln001 <= 72.8690000000)  jindex = 13;
                 else if (ln001 <= 78.8130000000)  jindex = 14;
                 else if (ln001 <= 86.7410000000)  jindex = 15;
                 else if (ln001 <= 95.9160000000)  jindex = 16;
                 else if (ln001 <= 109.3390000000) jindex = 17;
                 else if (ln001 <= 134.5470000000) jindex = 18;
                 else if (ln001 <= 170.8902000000) jindex = 19;
                 else if (ln001 <= 229.0276537120) jindex = 20;
                 else if (ln001 <= 319.9872318933) jindex = 21;
                 else if (ln001 <= 460.4174726738) jindex = 22;
                 else if (ln001 <= 673.9418808429) jindex = 23;
                 else if (ln001 <= 993.5307920375) jindex = 24;
                 else if (ln001 <= 1464.511605461) jindex = 25;
                 else jindex = 26;
                 pd =  KHJa2_PB[index][jindex];
                 break;
          case 2: // B1 ms080 > 0
                 pd =  KHJb1_PB[index];
                 break;
          case 3: // B2 ms080 <= 0
                 mp_r = pow((line * amortization_rate + ms082 * 1000.0), 0.5);

                 if      (mp_r <= 0.0000000000   ) jindex = 0;
                 else if (mp_r <= 40.5170000000  ) jindex = 1;
                 else if (mp_r <= 44.7890000000  ) jindex = 2;
                 else if (mp_r <= 49.4770000000  ) jindex = 3;
                 else if (mp_r <= 50.0750000000  ) jindex = 4;
                 else if (mp_r <= 56.8120000000  ) jindex = 5;
                 else if (mp_r <= 57.8220000000  ) jindex = 6;
                 else if (mp_r <= 58.4810000000  ) jindex = 7;
                 else if (mp_r <= 64.6470000000  ) jindex = 8;
                 else if (mp_r <= 64.7120000000  ) jindex = 9;
                 else if (mp_r <= 73.9730000000  ) jindex = 10;
                 else if (mp_r <= 78.9920000000  ) jindex = 11;
                 else if (mp_r <= 86.7410000000  ) jindex = 12;
                 else if (mp_r <= 104.6140000000 ) jindex = 13;
                 else if (mp_r <= 131.0540000000 ) jindex = 14;
                 else if (mp_r <= 175.6339000000 ) jindex = 15;
                 else if (mp_r <= 248.4351439635 ) jindex = 16;
                 else if (mp_r <= 364.6396331652 ) jindex = 17;
                 else if (mp_r <= 545.8185648775 ) jindex = 18;
                 else if (mp_r <= 821.9282779142 ) jindex = 19;
                 else if (mp_r <= 1233.7793392206) jindex = 20;
                 else jindex = 21;
                 pd =  KHJb2_PB[index][jindex];
                 break;
          case 4: // Demographic
                 pd =  KHJdemo_PB[index];
                 break;
       }
//     hostVars[0] = pd;
//     hostVars[1] = app_sn;
//     handler->ExecSQLCmd(SQLCommands[Write_PB_Result], hostVars, 1);
//    }
  delete ds;
 } catch (Exception &E) {
     throw;
 }
}

//---------------------------------------------------------------------------
int Loan::calculate_optimal_line(int start_amount, int end_amount, int increment,
                                    TADOHandler *handler)
{
 Variant hostVars[5];
 double mp_r, amortization_rate, ln001, npv;
 int    score_card, index, jindex;
 double ms082, wi001_12m;
 int    ms082_ind;
 TADODataSet *ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.

 try {
    optimal_line = 0.0;
    index = jindex = 0;
    hostVars[0] = app_sn;
    handler->ExecSQLQry(SQLCommands[Get_PB_test], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
       score_card = ds->FieldValues["score_card"];
       switch (score_card) {
          case 0: // screen out
                 break;
          case 1: // A2, full JCIC
                 index = ds->FieldValues["rtwentile_a2"];
                 if (! ds->FieldValues["ms082"].IsNull())
                    ms082 = ds->FieldValues["ms082"];
                 else
                    ms082 = 0.0;
                 wi001_12m = ds->FieldValues["WI001_12m"];
                 break;
          case 2: // B1 ms080 > 0
                 index = ds->FieldValues["rtwentile_b1"];
                 break;
          case 3: // B2 ms080 <= 0
                 index = ds->FieldValues["brmp_twentile"];
                 break;
          case 4: // Demographic
                 index = ds->FieldValues["rtwentile_c"];
                 break;
       }
       index--; // index start at 0
    }
    if (int_rate == 0.0) amortization_rate = 1.0 / periods;
    else amortization_rate =  pow ((1 + (int_rate / 12.0 )), periods) * (int_rate / 12.0 )
                              / (pow ((1 + (int_rate / 12.0 )), periods) - 1);

    for (int line = start_amount; line <= end_amount; line += increment) {
       switch (score_card) {
          case 0: // screen out
                 pd = 0.9;
                 break;
          case 1: // A2, full JCIC
                 ln001 = pow ((line * amortization_rate + ms082 * 1000.0)
                               / wi001_12m, 0.5);
                 if (ln001 <= 0.0) jindex = 0;
                 else if (ln001 <= 21.3540000000) jindex = 1;
                 else if (ln001 <= 27.3980000000) jindex = 2;
                 else if (ln001 <= 32.2840000000) jindex = 3;
                 else if (ln001 <= 36.1610000000) jindex = 4;
                 else if (ln001 <= 39.9150000000) jindex = 5;
                 else if (ln001 <= 43.5220000000) jindex = 6;
                 else if (ln001 <= 47.1000000000) jindex = 7;
                 else if (ln001 <= 50.6460000000) jindex = 8;
                 else if (ln001 <= 55.1270000000) jindex = 9;
                 else if (ln001 <= 58.8450000000) jindex = 10;
                 else if (ln001 <= 63.0650000000) jindex = 11;
                 else if (ln001 <= 67.7100000000) jindex = 12;
                 else if (ln001 <= 72.8690000000) jindex = 13;
                 else if (ln001 <= 78.8130000000) jindex = 14;
                 else if (ln001 <= 86.7410000000) jindex = 15;
                 else if (ln001 <= 95.9160000000) jindex = 16;
                 else if (ln001 <= 109.3390000000) jindex = 17;
                 else if (ln001 <= 134.5470000000) jindex = 18;
                 else if (ln001 <= 170.8902000000) jindex = 19;
                 else if (ln001 <= 229.0276537120) jindex = 20;
                 else if (ln001 <= 319.9872318933) jindex = 21;
                 else if (ln001 <= 460.4174726738) jindex = 22;
                 else if (ln001 <= 673.9418808429) jindex = 23;
                 else if (ln001 <= 993.5307920375) jindex = 24;
                 else if (ln001 <= 1464.5116054610) jindex = 25;
                 else jindex = 26;
                 pd =  GXa2_PB[index][jindex];
                 break;
          case 2: // B1 ms080 > 0
                 pd =  GXb1_PB[index];
                 break;
          case 3: // B2 ms080 <= 0
                 mp_r = pow((line * amortization_rate + ms082 * 1000.0), 0.5);

                 if (mp_r <= 0 ) jindex = 0;
                 else if (mp_r <= 40.517 ) jindex = 1;
                 else if (mp_r <= 44.789 ) jindex = 2;
                 else if (mp_r <= 49.477 ) jindex = 3;
                 else if (mp_r <= 50.075 ) jindex = 4;
                 else if (mp_r <= 56.812 ) jindex = 5;
                 else if (mp_r <= 57.822 ) jindex = 6;
                 else if (mp_r <= 58.481 ) jindex = 7;
                 else if (mp_r <= 64.647 ) jindex = 8;
                 else if (mp_r <= 64.712 ) jindex = 9;
                 else if (mp_r <= 73.973 ) jindex = 10;
                 else if (mp_r <= 78.992 ) jindex = 11;
                 else if (mp_r <= 86.741 ) jindex = 12;
                 else if (mp_r <= 104.614 ) jindex = 13;
                 else if (mp_r <= 131.054 ) jindex = 14;
                 else if (mp_r <= 175.6339 ) jindex = 15;
                 else if (mp_r <= 248.435144 ) jindex = 16;
                 else if (mp_r <= 364.6396332 ) jindex = 17;
                 else if (mp_r <= 545.8185649 ) jindex = 18;
                 else if (mp_r <= 821.9282779 ) jindex = 19;
                 else if (mp_r <= 1233.779339 ) jindex = 20;
                 else jindex = 21;
                 pd =  GXb2_PB[index][jindex];
                 break;
          case 4: // Demographic
                 pd =  GXdemo_PB[index];
                 break;
       }
     if (score_card > 0)
        if ((npv = calculate_npv(line)) > optimal_line)
           optimal_line = npv;
    }
  delete ds;
  hostVars[0] = pd;
  hostVars[1] = npv;
  hostVars[2] = index;
  hostVars[3] = jindex;
  hostVars[4] = app_sn;
  handler->ExecSQLQry(SQLCommands[Write_NPV_Result], hostVars, 4, ds);
 } catch (Exception &E) {
     throw;
 }
 return (0);
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
double Loan::calculate_npv(int line)
{
 Variant hostVars[5];
 double revenue, cost, wc;
 double Interest_Revenue, Late_Fee, Open_Credit_Fee, Risk_Management_Fee;
 double Interest_Cost, Acct_Mgmt_Cost, Taishin_Corp_Tax, TF_Corp_Tax;
 double Collection_Cost, Commission, Credit_Loss;

 try {
  npv_init();
  set_apr();
  set_attrition();
  set_annuity(line);  // 本息法
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
  Collection_Cost = set_collection_cost();
  Credit_Loss = set_credit_loss();
  total_npv = (Interest_Revenue + Late_Fee + Open_Credit_Fee
               + Risk_Management_Fee)                            // Revenue
               - (Interest_Cost + Acct_Mgmt_Cost + Taishin_Corp_Tax + TF_Corp_Tax
                  + Collection_Cost + Commission + Credit_Loss);         // Cost

 } catch (Exception &E) {
     throw;
 }
#ifdef _WRFLOW
     fstream outf;
     outf.open("NPV_flows.txt", ios::app | ios::out);  // Open for ouput and append

     outf << "Case SN: " << app_sn.c_str() << "   appDate: " << app_date.c_str()
          << "   jcicDate: " << jcic_date.c_str() << "   Commission: " << Commission
          << "   PB: " << pd << "   NPV: " << total_npv << endl;
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
              << setw(15) << setprecision(8) << Taishin_Corp_Tax
              << setw(15) << setprecision(8) << TF_Corp_Tax
              << setw(15) << setprecision(8) << Collection_Cost
              << setw(15) << setprecision(8) << Credit_Loss
              << endl;
     for (int i = 0; i <= periods; i++) {
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
  return (total_npv);
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
void Loan::set_amortize(int line)
{
  os_principal [0] = line;
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
void Loan::set_annuity(int line)
{
  int Grace_Period = ((grace_period == periods)? grace_period - 1: grace_period);
  int after_grace_period = periods - Grace_Period;
  double repayment = principal / periods;     // assume apr = 0.0%
  int remaining_period;

  os_principal [0] = line;
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
                      / LATE_30D_RATIO * LATE_PENALTY_RATIO;

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


//---------------------------------------------------------------------------
// Credit loss:
double Loan::set_credit_loss()
{
 double discount_rate = 1.0 / pow((1 + roe / 12.0), 10);
 credit_loss [0] = 0.0;
 for (int i = 1; i <= periods; i++) {
    credit_loss [i] = (os_principal[i-1]+ interest_revenue[i-1] * 3)* open_attrition[i-1] *
                      bad_per_open[i] * (1 - recovery_ratio * discount_rate);
 }
  return (NetPresentValue(roe / 12.0, credit_loss + 1, periods, ptEndOfPeriod)
          + credit_loss[0]);
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
