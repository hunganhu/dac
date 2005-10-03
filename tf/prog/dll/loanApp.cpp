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
char *TF_Messages[]= {
     "通過 ",                  // Prescreen_0,
     "建議核准 ",              // Normal_1,
     "建議婉拒 ",              // Normal_0,
     "JCIC資料時間超過30 天 ", // Warning_201,
     "申貸金額超出模組適用範圍，模組輸出僅供參考。(國民信貸A2超過40萬) ",     // Warning_202,
     "申貸金額超出模組適用範圍，模組輸出僅供參考。(國民信貸B1超過25萬) ",     // Warning_203,
     "申貸金額超出模組適用範圍，模組輸出僅供參考。(國民信貸B2 / C超過20萬) ", // Warning_204,
     // Prescreen Errors
     "申請人年齡大於55歲 ",         // Prescreen_101,
     "申請人為外國人 ",             // Prescreen_102,
     "申請人為本行現金卡額度被鎖 ", // Prescreen_103,
     "重大信用瑕疵 ",               // Prescreen_104,
     "信用卡連續三個月以上未繳足最低應繳金額 ", // Prescreen_105,
     "貸款有逾期 ",                             // Prescreen_106,
     "最近一個月有現金卡逾期 ",                 // Prescreen_107,
     "其他非信用卡貸款連續三期以上繳款異常 ",   // Prescreen_108,
     // Application Data Errors
     "無法辨識申請人是否為外國人 ",             // App_error_301,
     "無法辨識申請人年紀是否超過55歲 ",         // App_error_302,
     "無法辨識是否申請人本行現金卡額度被鎖 ",   // App_error_303,
     "無法辨識申請人性別 ",                     // App_error_304,
     "產品代號無法辨識 ",                       // App_error_305,
     "三碼郵遞區號錯誤 ",                       // App_error_306,
     "無法辨識是否要密家人 ",                   // App_error_307,
     "無法辨識婚姻狀況 ",                       // App_error_308,
     "教育程度無法辨識 ",                       // App_error_309,
     "佣金為負數 ",                             // App_error_310,
     "銷售管道無法辨識 ",                       // App_error_311,
     "風險等級無法辨識 ",                       // App_error_312,
     // Financial Data Errors
     "ROE小於或等於零 ", // Fin_error_321,
     "台新金控資金成本小於或等於零 ",           // Fin_error_322,
     "台新銀行營業稅小於或等於零 ",             // Fin_error_323,
     "台新資融營業稅小於或等於零 ",             // Fin_error_324,
     "台新資訊室費用小於或等於零 ",             // Fin_error_325,
     "台新作業與客服費用小於或等於零 ",         // Fin_error_326,
     "人事成本小於或等於零 ",                   // Fin_error_327,
     // Loan Conditions Data Errors
     "國民信貸貸款金額大於600000或小於100000 ", // Loan_error_331_GX,
     "卡好借貸款金額大於200000或小於70000 ",    // Loan_error_331_KHJ,
     "年利率小於15%或大於20% ",                 // Loan_error_332,
     "期數大於84或小於13 ",                     // Loan_error_333,
     "開辦費小於零 ",                           // Loan_error_334,
     "徵信查詢費小於零 ",                                                                                                                                  // Loan_error_335,
     "每期風險管理費用小於零 ",                   // Loan_error_336,
     "風險管理費用收取期數(月)小於零或大於期數 ", // Loan_error_337,
     "優惠期 (月) 小於零或大於等於期數 ",         // Loan_error_338,
     "寬限期 (月) 小於零或大於等於期數 ",         // Loan_error_339,
     "優惠利率小於零或大於等於年利率 ",             // Loan_error_340,
     "不適用於最適貸款金額計算 (信用卡資料不足或產品為卡好借) " // Optimal_error_501
};

//---------------------------------------------------------------------------

#pragma package(smart_init)

Loan::Loan (char *appSN, char *appDate, char *jcicDate):
    app_sn(appSN), app_date(appDate), jcic_date(jcicDate), product_type_ind(0),
    gender_ind(0), zip_ind(0), secretive_ind(0), edu_ind(0), Message(""),
    marriage_status_ind(0), alien_ind(0), age_ind(0), cashcard_lock_ind(0),
    cof_ind(0), roe_ind(0), ts_tax_rate_ind(0), tf_tax_rate_ind(0),
    info_processing_cost_ind(0), operation_cost_ind(0), hr_cost_ind(0),
    principal_ind(0), int_rate_ind(0), teaser_rate_ind(0), periods_ind(0),
    teaser_period_ind(0), grace_period_ind(0), application_fee_ind(0),
    credit_checking_fee_ind(0), risk_mgmt_fee_ind(0), risk_mgmt_fee_terms_ind(0),
    sales_channel_ind(0), risk_level_ind(0), code(0), commission_ind(0)
{
}
//---------------------------------------------------------------------------
Loan::Loan (char *appSN, char *appDate, char *tsDate, char *jcicDate, char *tsn):
    app_sn(appSN), app_date(appDate), ts_date(tsDate), jcic_date(jcicDate),tsn(tsn),product_type_ind(0),
    gender_ind(0), zip_ind(0), secretive_ind(0), edu_ind(0), Message(""),
    marriage_status_ind(0), alien_ind(0), age_ind(0), cashcard_lock_ind(0),
    cof_ind(0), roe_ind(0), ts_tax_rate_ind(0), tf_tax_rate_ind(0),
    info_processing_cost_ind(0), operation_cost_ind(0), hr_cost_ind(0),
    principal_ind(0), int_rate_ind(0), teaser_rate_ind(0), periods_ind(0),
    teaser_period_ind(0), grace_period_ind(0), application_fee_ind(0),
    credit_checking_fee_ind(0), risk_mgmt_fee_ind(0), risk_mgmt_fee_terms_ind(0),
    sales_channel_ind(0), risk_level_ind(0), code(0), commission_ind(0)
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
int Loan::app_info_validate(char * appNo, char* appDate, TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);

 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 code = 0;
 Message = "";
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

       if (! ds->FieldValues["education_level"].IsNull())
          edu = ds->FieldValues["education_level"];
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

       if (!ds->FieldValues["commission"].IsNull())
          commission = ds->FieldValues["commission"];
       else
          commission_ind = -1;
    }

  if (record_count == 0) {
     Message += "無申請件資料 "; code = 313;}
  else {
     if ((alien_ind == -1) || (alien < 0) || (alien > 1)) {
        Message += TF_Messages[App_error_301]; code = 301;}
   
     if ((age_ind == -1) || (age < 0) || (age > 1)) {
        Message += TF_Messages[App_error_302]; code = 302;}
   
     if ((cashcard_lock_ind == -1) || (cashcard_lock < 0) || (cashcard_lock > 1)) {
        Message += TF_Messages[App_error_303]; code = 303;}
   
     if ((gender_ind == -1) || (gender < 0) || (gender > 1)) {
        Message += TF_Messages[App_error_304]; code = 304;}
   
     if ((product_type_ind == -1) || (product_type < 1) || (product_type > 2)) {
        Message += TF_Messages[App_error_305]; code = 305;}

     if ( validZIP(zip.c_str()) == 0) {
        Message += TF_Messages[App_error_306];  code = 306;}
   
     if ((secretive_ind == -1) || (secretive < 0) || (secretive > 1)) {
        Message += TF_Messages[App_error_307]; code = 307;}
   
     if ((marriage_status_ind == -1) || (marriage_status < 1) || (marriage_status > 4)) {
        Message += TF_Messages[App_error_308];  code = 308;}
   
     if ((edu_ind == -1) || (edu < 1) || (edu > 6)) {
        Message += TF_Messages[App_error_309]; code = 309;}

     // if commission is null then calculate ourselves, otherwise get it from this field
     if ((commission_ind == 0) && (commission < 0)) {
        Message += TF_Messages[App_error_310]; code = 310;}

     if ((sales_channel_ind == -1) || (sales_channel < "001") || (sales_channel > "009") ) {
        Message += TF_Messages[App_error_311]; code = 311;}
   
     if ((risk_level_ind == -1) || (risk_level < 1) || (risk_level > 2) ) {
        Message += TF_Messages[App_error_312]; code = 312;}
   
     if ((roe_ind == -1) || (roe <= 0.0)) {
        Message += TF_Messages[Fin_error_321]; code = 321;}
   
     if ((cof_ind == -1) || (cof <= 0.0)) {
        Message += TF_Messages[Fin_error_322]; code = 322;}
   
     if ((ts_tax_rate_ind == -1) || (ts_tax_rate <= 0.0)) {
        Message += TF_Messages[Fin_error_323]; code = 323;}

     if ((tf_tax_rate_ind == -1) || (tf_tax_rate <= 0.0)) {
        Message += TF_Messages[Fin_error_324]; code = 324;}
   
     if ((info_processing_cost_ind == -1) || (info_processing_cost <= 0.0)) {
        Message += TF_Messages[Fin_error_325]; code = 325;}
   
     if ((operation_cost_ind == -1) || (operation_cost <= 0.0)) {
        Message += TF_Messages[Fin_error_326]; code = 326;}
   
     if ((hr_cost_ind == -1) || (hr_cost <= 0.0)) {
        Message += TF_Messages[Fin_error_327]; code = 327;}
  }   
 } catch (Exception &E) {
    throw;
 }
  ds->Close();  // close dataset before delete and drop an object outside the try block,
                // otherwise result in "too many consecutive exceptions"
  delete ds;
  if (code == 313)
     return (-1);
  else
     return (0);
}
//---------------------------------------------------------------------------
int Loan::loan_validate(char * appNo, char *tsn, TADOHandler *handler)
{
 Variant hostVars[5];
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
  if (trial_count == 0)  {
     Message += "無申請件資料 "; code = 313;}
  else {
     if ((principal_ind == -1) || (product_type == 1 && (principal < 100000.0 || principal > 600000.0))) {
        Message += TF_Messages[Loan_error_331_GX]; code = 331;}
     else if ((principal_ind == -1) || (product_type == 2 && (principal < 70000.0 || principal > 200000.0))){
        Message += TF_Messages[Loan_error_331_KHJ]; code = 331;}
   
     if ((int_rate_ind == -1) || (int_rate < 0.15)||(int_rate > 0.20)) {
          Message += TF_Messages[Loan_error_332]; code = 332;}
   
     if ((periods_ind == -1) || (periods < 13) || (periods > 84)) {
        Message += TF_Messages[Loan_error_333];  code = 333;}
   
     if ((application_fee_ind == -1) || (application_fee < 0)) {
        Message += TF_Messages[Loan_error_334];  code = 334;}
   
     if ((credit_checking_fee_ind == -1) || (credit_checking_fee < 0)) {
        Message += TF_Messages[Loan_error_335];  code = 335;}
   
     if ((risk_mgmt_fee_ind == -1) || (risk_mgmt_fee < 0)) {
        Message += TF_Messages[Loan_error_336];  code = 336;}
   
     if ((risk_mgmt_fee_terms_ind == -1) || (risk_mgmt_fee_terms < 0) || (risk_mgmt_fee_terms > periods)) {
        Message += TF_Messages[Loan_error_337];  code = 337;}
   
     if ((teaser_period_ind == -1) || (teaser_period < 0) || (teaser_period >= periods)) {
        Message +=TF_Messages[Loan_error_338];   code = 338;}
   
     if ((grace_period_ind == -1) || (grace_period < 0) || (grace_period >= periods)) {
        Message += TF_Messages[Loan_error_339];  code = 339;}
   
     if ((teaser_rate_ind == -1) || (teaser_rate < 0.0) || (teaser_rate >= int_rate)) {
          Message += TF_Messages[Loan_error_340];  code = 340;}
  }
 } catch (Exception &E) {
    throw;
 }
  ds->Close();
  delete ds;
  if (code == 313)
     return (-1);
  else
     return (0);
}
int Loan::loan_validate_no_principal(char * appNo, char *tsn, TADOHandler *handler)
{
 Variant hostVars[5];
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
  if (trial_count == 0)  {
     Message += "無申請件資料 "; code = 313;}
  else {
  /*
     if ((principal_ind == -1) || (product_type == 1 && (principal < 100000.0 || principal > 600000.0))) {
        Message += TF_Messages[Loan_error_331_GX]; code = 331;}
     else if ((principal_ind == -1) || (product_type == 2 && (principal < 70000.0 || principal > 200000.0))){
        Message += TF_Messages[Loan_error_331_KHJ]; code = 331;}
  */
     if ((int_rate_ind == -1) || (int_rate < 0.15)||(int_rate > 0.20)) {
          Message += TF_Messages[Loan_error_332]; code = 332;}

     if ((periods_ind == -1) || (periods < 13) || (periods > 84)) {
        Message += TF_Messages[Loan_error_333];  code = 333;}

     if ((application_fee_ind == -1) || (application_fee < 0)) {
        Message += TF_Messages[Loan_error_334];  code = 334;}

     if ((credit_checking_fee_ind == -1) || (credit_checking_fee < 0)) {
        Message += TF_Messages[Loan_error_335];  code = 335;}

     if ((risk_mgmt_fee_ind == -1) || (risk_mgmt_fee < 0)) {
        Message += TF_Messages[Loan_error_336];  code = 336;}

     if ((risk_mgmt_fee_terms_ind == -1) || (risk_mgmt_fee_terms < 0) || (risk_mgmt_fee_terms > periods)) {
        Message += TF_Messages[Loan_error_337];  code = 337;}

     if ((teaser_period_ind == -1) || (teaser_period < 0) || (teaser_period >= periods)) {
        Message +=TF_Messages[Loan_error_338];   code = 338;}
   
     if ((grace_period_ind == -1) || (grace_period < 0) || (grace_period >= periods)) {
        Message += TF_Messages[Loan_error_339];  code = 339;}

     if ((teaser_rate_ind == -1) || (teaser_rate < 0.0) || (teaser_rate >= int_rate)) {
          Message += TF_Messages[Loan_error_340];  code = 340;}
  }
 } catch (Exception &E) {
    throw;
 }
  ds->Close();
  delete ds;
  if (code == 313)
     return (-1);
  else
     return (0);
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
double Loan::get_pd()
{
 return pd;
}
//---------------------------------------------------------------------------
double Loan::get_npv()
{
 return total_npv;
}
//---------------------------------------------------------------------------
double Loan::get_principal()
{
 return principal;
}
//---------------------------------------------------------------------------
int Loan::get_card()
{
 return card;
}
int Loan::get_external_monthly_payment()
{
 return ms082;
}
//---------------------------------------------------------------------------
void Loan::prescreen(char *inquiry_date, TADOHandler *handler)
{
 Variant hostVars[10];
// int jas002_defect, fs044, app_max_bucket, cash_max_bucket, delinquent_months;
 TADODataSet *ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 code = 0;
 jcic_date = inquiry_date;
 Message = TF_Messages[Prescreen_0];
 
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
       ms082 = ds->FieldValues["ms082"];
    }
 } catch (Exception &E) {
     throw;
 }
    ds->Close();
    delete ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.

    if (age == 1) {
       Message = TF_Messages[Prescreen_101]; code = 101; }
    else if (alien == 1) {
       Message = TF_Messages[Prescreen_102]; code = 102; }
    else if (cashcard_lock == 1) {
       Message = TF_Messages[Prescreen_103]; code = 103; }
    else if (jas002_defect > 0) {
       Message = TF_Messages[Prescreen_104]; code = 104; }
    else if (app_max_bucket > 3) {
       Message = TF_Messages[Prescreen_105]; code = 105; }
    else if (fs044 > 0) {
       Message = TF_Messages[Prescreen_106]; code = 106; }
    else if (cash_max_bucket > 0) {
       Message = TF_Messages[Prescreen_107]; code = 107; }
    else if (delinquent_months > 3) {
       Message = TF_Messages[Prescreen_108]; code = 108; }
    /* Output_Prescreen_Output to working table */
 try {
    hostVars[0] = Message;
    hostVars[1] = app_sn;
    handler->ExecSQLCmd(SQLCommands[Update_Prescreen_Output], hostVars, 1);

    /*Write_Prescreen_Result*/
    hostVars[0] = app_sn;
    hostVars[1] = app_date;
    hostVars[2] = jcic_date;
    handler->ExecSQLQry(SQLCommands[Get_Prescreen_Record], hostVars, 2, ds);
    record_count = ds->RecordCount;
    /* if PRESCREEN table has a record with the same key then skip writing*/
    if (record_count == 0) {
       hostVars[0] = app_sn;
       hostVars[1] = jcic_date;
       hostVars[2] = app_date;
       hostVars[3] = product_type;
       hostVars[4] = code;
       hostVars[5] = Message;
       handler->ExecSQLCmd(SQLCommands[Write_Prescreen_Result], hostVars, 5);
    }
 } catch (Exception &E) {
     throw;
 }
    ds->Close();
    delete ds;
}

//---------------------------------------------------------------------------
void Loan::prescreen_only(char *inquiry_date, TADOHandler *handler)
{
 Variant hostVars[10];
// int jas002_defect, fs044, app_max_bucket, cash_max_bucket, delinquent_months;
 TADODataSet *ds;
 int check_jcic_expire = 0;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 code = 0;
 jcic_date = inquiry_date;
 Message = TF_Messages[Prescreen_0];

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
       ms082 = ds->FieldValues["ms082"];
    }
 } catch (Exception &E) {
     throw;
 }
    ds->Close();
    delete ds;

 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.

    if (Days_between(jcic_date, app_date) > 30) {
       code = 201;
       check_jcic_expire = 1;
    }
    if (age == 1) {
       Message = TF_Messages[Prescreen_101]; code = 101; }
    else if (alien == 1) {
       Message = TF_Messages[Prescreen_102]; code = 102; }
    else if (cashcard_lock == 1) {
       Message = TF_Messages[Prescreen_103]; code = 103; }
    else if (jas002_defect > 0) {
       Message = TF_Messages[Prescreen_104]; code = 104; }
    else if (app_max_bucket > 3) {
       Message = TF_Messages[Prescreen_105]; code = 105; }
    else if (fs044 > 0) {
       Message = TF_Messages[Prescreen_106]; code = 106; }
    else if (cash_max_bucket > 0) {
       Message = TF_Messages[Prescreen_107]; code = 107; }
    else if (delinquent_months > 3) {
       Message = TF_Messages[Prescreen_108]; code = 108; }
    /* Output_Prescreen_Output to working table */
 try {
    hostVars[0] = Message;
    hostVars[1] = app_sn;
    handler->ExecSQLCmd(SQLCommands[Update_Prescreen_Output], hostVars, 1);

    /*Write_Prescreen_Result*/
    hostVars[0] = app_sn;
    hostVars[1] = app_date;
    hostVars[2] = jcic_date;
    handler->ExecSQLQry(SQLCommands[Get_Prescreen_Record], hostVars, 2, ds);
    record_count = ds->RecordCount;
    /* if PRESCREEN table has a record with the same key then skip writing*/
    if (record_count == 0) {
       hostVars[0] = app_sn;
       hostVars[1] = jcic_date;
       hostVars[2] = app_date;
       hostVars[3] = product_type;
       hostVars[4] = code;
       if (check_jcic_expire == 1)
          Message += TF_Messages[Warning_201];
       hostVars[5] = Message;
       handler->ExecSQLCmd(SQLCommands[Write_Prescreen_Result], hostVars, 5);
    }
 } catch (Exception &E) {
     throw;
 }
    ds->Close();
    delete ds;
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

    hostVars[0] = ts_date;
    hostVars[1] = app_sn;
    handler->ExecSQLCmd(SQLCommands[Update_TS_Date], hostVars, 1);

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
    if (krm001_hit == 1 && krm023_hit == 1 && ind001 == 0) {
       handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_ploan_model]);
       card = 1;
    }
    else if (bam085_hit == 1 && ms080 <= 0) {
             handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_BAM_no_payment]);
             card = 3;
         }
         else if (bam085_hit == 1 && ms080 > 0) {
                 handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_BAM_with_payment]);
                 card = 2;
              }
              else {
                 handler->ExecSQLCmd(SQLCommands[Create_Procedure_TF_demographic_model]);
                 card = 4;
              }
 } catch (Exception &E) {
     throw;
   }
}
//---------------------------------------------------------------------------
double Loan::cal_GXa2_pb(int lending_amt, double apr, int period, double risk_score,
                double ms082, double wi001_12m)
{
 double ln001_12m_r, ln001_12m_r_n, partial_score_n, pb;
 ln001_12m_r = pow((Payment(apr / 12.0, period, -lending_amt, 0.0, ptEndOfPeriod)
                    + ms082 * 1000.0)/ wi001_12m , 0.5);
 ln001_12m_r_n = ((ln001_12m_r - MIN_LN001_12M_R) > 0.0? (ln001_12m_r - MIN_LN001_12M_R): 0.0)
                 / (MAX_LN001_12M_R - MIN_LN001_12M_R) + TRIVIAL_NUM;
 partial_score_n = ((risk_score - MIN_PARTIAL_SCORE) > 0.0? (risk_score - MIN_PARTIAL_SCORE): 0.0)
                   / (MAX_PARTIAL_SCORE - MIN_PARTIAL_SCORE) + TRIVIAL_NUM;
 pb =  a_a2[0] + a_a2[1] * partial_score_n + a_a2[2] * pow (partial_score_n, a_a2[3])
     + (b_a2[0] + b_a2[1] * partial_score_n + b_a2[2] * pow (partial_score_n, b_a2[3])) * ln001_12m_r_n
     + (c_a2[0] + c_a2[1] * partial_score_n + c_a2[2] * pow (partial_score_n, c_a2[3]))
     * pow(ln001_12m_r_n, (d_a2[0] + d_a2[1] * partial_score_n + d_a2[2] * pow (partial_score_n, d_a2[3])));

 return (pb);
}

//---------------------------------------------------------------------------
double Loan::cal_GXb1_pb(double risk_score)
{
 double pb;

 if      (risk_score <= 0.16277000000) pb =0.0850017500669239; 
 else if (risk_score <= 0.20156500000) pb =0.1447272118939650; 
 else if (risk_score <= 0.21722227265) pb =0.1668043016186280; 
 else if (risk_score <= 0.23324727265) pb =0.1809447578598680; 
 else if (risk_score <= 0.24526152055) pb =0.1931855549658770; 
 else if (risk_score <= 0.26604152055) pb =0.2021941066136120; 
 else if (risk_score <= 0.27716500000) pb =0.2100478269856190; 
 else if (risk_score <= 0.28608836955) pb =0.2183503684425780; 
 else if (risk_score <= 0.30174564220) pb =0.2243303993174850;
 else if (risk_score <= 0.30981298495) pb =0.2310321403196800; 
 else if (risk_score <= 0.31018071230) pb =0.2367873991108270; 
 else if (risk_score <= 0.31120500000) pb =0.2416443136296220; 
 else if (risk_score <= 0.31859439955) pb =0.2466326232893190; 
 else if (risk_score <= 0.32924592010) pb =0.2476218971230850; 
 else if (risk_score <= 0.33751379320) pb =0.2515519111045310; 
 else if (risk_score <= 0.34664727265) pb =0.2585207795898800; 
 else if (risk_score <= 0.35276500000) pb =0.2623365749832010; 
 else if (risk_score <= 0.35965652055) pb =0.2659262561421880; 
 else if (risk_score <= 0.36857989010) pb =0.2733758815054040; 
 else if (risk_score <= 0.37870836955) pb =0.2801435216142030; 
 else if (risk_score <= 0.38677571230) pb =0.2851904892831920; 
 else if (risk_score <= 0.38777071230) pb =0.2903532136976970; 
 else pb =0.3089703987428730; 
 
 return (pb);
}
//---------------------------------------------------------------------------
double Loan::cal_GXb2_pb(int lending_amt, double apr, int period, double risk_score,
                double ms082)
{
 double mp_r, mpr_n, brmp_score_n, pb;

 mp_r = pow((Payment(apr / 12.0, period, -lending_amt, 0.0, ptEndOfPeriod) + ms082 * 1000.0), 0.5);
 mpr_n = ((mp_r - MIN_MPR) > 0.0? (mp_r - MIN_MPR): 0.0) / (MAX_MPR - MIN_MPR) + TRIVIAL_NUM;
 brmp_score_n = ((risk_score - MIN_BRMP_SCORE) > 0.0? (risk_score - MIN_BRMP_SCORE): 0.0)
                   / (MAX_BRMP_SCORE - MIN_BRMP_SCORE) + TRIVIAL_NUM;
 pb =  a_b2[0] + a_b2[1] * brmp_score_n + a_b2[2] * pow (brmp_score_n, a_b2[3])
     + (b_b2[0] + b_b2[1] * brmp_score_n + b_b2[2] * pow (brmp_score_n, b_b2[3])) * mpr_n
     + (c_b2[0] + c_b2[1] * brmp_score_n + c_b2[2] * pow (brmp_score_n, c_b2[3]))
     * pow(mpr_n, (d_b2[0] + d_b2[1] * brmp_score_n + d_b2[2] * pow (brmp_score_n, d_b2[3])));

 return (pb);
}

double Loan::cal_GXc_pb(double risk_score)
{
 double pb;

 if      (risk_score <= 0.06348) pb = 0.0649831232981667;
 else if (risk_score <= 0.09329) pb = 0.0707807385668779;
 else if (risk_score <= 0.09731) pb = 0.0792251813444537;
 else if (risk_score <= 0.09823) pb = 0.0910745257376597;
 else if (risk_score <= 0.14058) pb = 0.098517428813551;
 else if (risk_score <= 0.16891) pb = 0.106108567922379;
 else if (risk_score <= 0.16983) pb = 0.117738902172291;
 else if (risk_score <= 0.17533) pb = 0.125299997076995;
 else if (risk_score <= 0.17935) pb = 0.143698844513559;
 else if (risk_score <= 0.20505) pb = 0.170805868713081;
 else if (risk_score <= 0.24693) pb = 0.180681774160762;
 else if (risk_score <= 0.25095) pb = 0.241228269260367;
 else if (risk_score <= 0.25645) pb = 0.339295046226114;
 else pb =  0.385068531172599;

 return (pb);
}
//---------------------------------------------------------------------------
double Loan::cal_KHJa2_pb(double risk_score)
{
 double rscore_n, pb;
 double a, b, c, d, min_score, length;

 a = 0.0000;
 b = 0.0000;
 c = 1.0000;
 d = 1.0616;
 min_score = 0.103105;
 length = 1.1570411836;

 rscore_n = ((risk_score + min_score) > 0.0? (risk_score + min_score): 0.0) / length;
 pb =  a + b * rscore_n + c * pow (rscore_n, d);

 return (pb);
}
//---------------------------------------------------------------------------
double Loan::cal_KHJb1_pb(double risk_score)
{
 double rscore_n, pb;
 double a, b, c, d, min_score, length;

 a = 0.056448;
 b = 0.000000;
 c = 0.734695;
 d = 1.563120;
 min_score = -0.12497;
 length = 0.41403;

 rscore_n = ((risk_score + min_score) > 0.0? (risk_score + min_score): 0.0) / length;
 pb =  a + b * rscore_n + c * pow (rscore_n, d);

 return (pb);
}
//---------------------------------------------------------------------------
double Loan::cal_KHJb2_pb(double risk_score)
{
 double rscore_n, pb;
 double a, b, c, d, e, min_score, length;

 a = 0.159427;
 b = 0.285207;
 c = 0.273425;
 d = 0.682826;
 e = 0.944249;
 min_score = -0.08517;
 length = 0.60995;

 rscore_n = ((risk_score + min_score) > 0.0? (risk_score + min_score): 0.0) / length;
 pb =  (a + b * rscore_n + c * pow (rscore_n, d)) * e;

 return (pb);
}
//---------------------------------------------------------------------------
double Loan::cal_KHJc_pb(double risk_score)
{
 double rscore_n, pb;
 double a, b, c, d, min_score, length;

 a = 0.035741;
 b = 0.403862;
 c = 0.179088;
 d = 9.618170;
 min_score = 0.08398;
 length = 0.476;

 rscore_n = ((risk_score + min_score) > 0.0? (risk_score + min_score): 0.0) / length;
 pb =  a + b * rscore_n + c * pow (rscore_n, d);

 return (pb);
}
//---------------------------------------------------------------------------
double Loan::calculate_pd(int line, TADOHandler *handler)
{
 Variant hostVars[5];
 int    card;
 double ms082, wi001_12m, risk_score;
 double pb_original, pb_original_1, pb_original_2, pb_original_3;
 double pb_1, pb_2, pb_3;
 int    ms082_ind;
 TADODataSet *ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.

 try {
    hostVars[0] = app_sn;
    handler->ExecSQLQry(SQLCommands[Get_PB_Parameters], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
       if (product_type == 1) { // product GX
          card = ds->FieldValues["score_card"];
          switch (card) {
             case 0: // screen out
                    break;
             case 1: // A2, full JCIC
                    risk_score = ds->FieldValues["full_score"];
                    if (! ds->FieldValues["ms082"].IsNull())
                       ms082 = ds->FieldValues["ms082"];
                    else
                       ms082 = 0.0;
                    wi001_12m = ds->FieldValues["WI001_12m"];
                    pb_original = cal_GXa2_pb(line, int_rate, periods, risk_score,
                                           ms082, wi001_12m);
                    pb_original_1 = cal_GXa2_pb(AMOUNT_1, int_rate, periods, risk_score,
                                           ms082, wi001_12m);
                    pb_original_2 = cal_GXa2_pb(AMOUNT_2, int_rate, periods, risk_score,
                                           ms082, wi001_12m);
                    pb_original_3 = cal_GXa2_pb(AMOUNT_3, int_rate, periods, risk_score,
                                           ms082, wi001_12m);
                    pb_1 = pb_original_1;
                    pb_2 = (pb_original_2 - pb_original_1) * INFLAT_1 + pb_1;
                    pb_3 = (pb_original_3 - pb_original_2) * INFLAT_2 + pb_2;
                    if (line > AMOUNT_3)
                        pd = (pb_original - pb_original_3) * INFLAT_3 + pb_3;
                    else if (line > AMOUNT_2)
                        pd = (pb_original - pb_original_2) * INFLAT_2 + pb_2;
                    else if (line > AMOUNT_1)
                        pd = (pb_original - pb_original_1) * INFLAT_1 + pb_1;
                    else
                        pd = pb_original;
                    if (pd > 0.7) pd = 0.7; // cap pd at 100%, 20051003:cap at 70%
                    break;
             case 2: // B1 ms080 > 0
                    risk_score = ds->FieldValues["b1_score"];
                    pd = cal_GXb1_pb(risk_score);
                    break;
             case 3: // B2 ms080 <= 0
                    risk_score = ds->FieldValues["b2_score"];
                    if (! ds->FieldValues["ms082"].IsNull())
                       ms082 = ds->FieldValues["ms082"];
                    else
                       ms082 = 0.0;
                    pb_original = cal_GXb2_pb(line, int_rate, periods, risk_score,
                                           ms082);
                    pb_original_1 = cal_GXb2_pb(AMOUNT_1, int_rate, periods, risk_score,
                                           ms082);
                    pb_original_2 = cal_GXb2_pb(AMOUNT_2, int_rate, periods, risk_score,
                                           ms082);
                    pb_original_3 = cal_GXb2_pb(AMOUNT_3, int_rate, periods, risk_score,
                                           ms082);
                    pb_1 = pb_original_1;
                    pb_2 = (pb_original_2 - pb_original_1) * INFLAT_1 + pb_1;
                    pb_3 = (pb_original_3 - pb_original_2) * INFLAT_2 + pb_2;
                    if (line > AMOUNT_3)
                        pd = (pb_original - pb_original_3) * INFLAT_3 + pb_3;
                    else if (line > AMOUNT_2)
                        pd = (pb_original - pb_original_2) * INFLAT_2 + pb_2;
                    else if (line > AMOUNT_1)
                        pd = (pb_original - pb_original_1) * INFLAT_1 + pb_1;
                    else
                        pd = pb_original;
                    if (pd > 0.7) pd = 0.7; // cap pd at 100%, 20051003:cap at 70%
                    break;
             case 4: // Demographic
                    risk_score = ds->FieldValues["demo_score"];
                    pd = cal_GXc_pb(risk_score);
                    break;
          } //End of switch
       } // End of product GX
       else if (product_type == 2) { // product KHJ
          card = ds->FieldValues["score_card"];
          switch (card) {
             case 0: // screen out
                    break;
             case 1: // A2, full JCIC
                    risk_score = ds->FieldValues["full_score"];
                    pd = cal_KHJa2_pb(risk_score);
                    if (pd > 0.7) pd = 0.7; // cap pd at 100%, 20051003:cap at 70%
                    break;
             case 2: // B1 ms080 > 0
                    risk_score = ds->FieldValues["b1_score"];
                    pd = cal_KHJb1_pb(risk_score);
                    if (pd > 0.7) pd = 0.7; // cap pd at 100%, 20051003:cap at 70%
                    break;
             case 3: // B2 ms080 <= 0
                    risk_score = ds->FieldValues["b2_score"];
                    pd = cal_KHJb2_pb(risk_score);
                    if (pd > 0.7) pd = 0.7; // cap pd at 100%, 20051003:cap at 70%
                    break;
             case 4: // Demographic
                    risk_score = ds->FieldValues["demo_score"];
                    pd = cal_KHJc_pb(risk_score);
                    if (pd > 0.7) pd = 0.7; // cap pd at 100%, 20051003:cap at 70%
                    break;
          } // End of switch
       } // End of product KHJ
    }
//     hostVars[0] = pd;
//     hostVars[1] = app_sn;
//     handler->ExecSQLCmd(SQLCommands[Write_PB_Result], hostVars, 1);
//    }
 } catch (Exception &E) {
     throw;
 }
  ds->Close();
  delete ds;
 return(pd);
}

//---------------------------------------------------------------------------
int Loan::calculate_optimal_line(int loops, double npv[][3], TADOHandler *handler)
/*
  npv[][0]: lending amount (input)
  npv[][1]: PB (output)
  npv[][2]: NPV (output)
*/
{
 double max_npv;
 int max_line;

 try {
    max_npv =-999999999;   // set max_npv to a very small negative number
    for (int i = 0; i < loops; i++) {
      npv[i][1] = calculate_pd(npv[i][0], handler);
      if ((npv[i][2] = calculate_npv(npv[i][0], npv[i][1])) > max_npv) {
           max_npv = npv[i][2];
           max_line = i;
      }
    }
    pd = npv[max_line][1];
    total_npv = npv[max_line][2];
 } catch (Exception &E) {
     throw;
 }
 return (max_line);
}
//---------------------------------------------------------------------------
double Loan::calculate_npv(int line, double pb)
{
 Variant hostVars[5];
 double revenue, cost, wc;
 double Interest_Revenue, Late_Fee, Open_Credit_Fee, Risk_Management_Fee;
 double Interest_Cost, Acct_Mgmt_Cost, Taishin_Corp_Tax, TF_Corp_Tax;
 double Collection_Cost, Credit_Loss;

 try {
  npv_init();
  set_apr();
  set_attrition(pb);
  set_annuity(line);  // 本息法
//  if (commission_ind == -1)
     commission =  calculate_commission(line);
  if (commission < 0) commission = 0;
  // Revenue
  Interest_Revenue = set_interest_revenue();
  Late_Fee = set_late_fee(pb);
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
                  + Collection_Cost + commission + Credit_Loss);         // Cost

 } catch (Exception &E) {
     throw;
 }
#ifdef _WRFLOW
     fstream outf;
     outf.open("NPV_flows.txt", ios::app | ios::out);  // Open for ouput and append

     outf << "Case SN: " << app_sn.c_str() << "   appDate: " << app_date.c_str()
          << "   jcicDate: " << jcic_date.c_str() << "   Commission: " << commission
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
void Loan::set_attrition(double pb)
{
  double monthly_pd = pb / 12.0;
  double adjustment;

  // product_type: 1 for GX, 2 for KHJ
  switch (product_type - 1) {
     case GX: adjustment = get_GX_adjustment(pb);
              break;
     case KHJ: adjustment = get_KHJ_adjustment(pb);
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
  double fix_payment = line / (periods - grace_period);

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
  double repayment = line / periods;     // assume apr = 0.0%
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

double Loan::set_late_fee(double pb)
{
  double monthly_pd = pb / 12.0;

  for (int i = 1; i <= periods; i++)
     if (i < 2)
        late_fee[i] = 0.0;
     else
       if (product_type ==1)
          late_fee[i] = interest_revenue[i-1] * open_attrition[i] *  monthly_pd
                      / GX_LATE_30D_RATIO * LATE_PENALTY_RATIO;
       else if (product_type ==2)
          late_fee[i] = interest_revenue[i-1] * open_attrition[i] *  monthly_pd
                      / KHJ_LATE_30D_RATIO * LATE_PENALTY_RATIO;


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
double Loan::calculate_commission(int line)
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
    case GX: if (line <= 150000) line_grp = 0;
             else if (line <= 200000) line_grp = 1;
             else if (line <= 250000) line_grp = 2;
             else if (line <= 300000) line_grp = 3;
             else if (line <= 350000) line_grp = 4;
             else if (line <= 400000) line_grp = 5;
             else if (line <= 450000) line_grp = 6;
             else if (line <= 500000) line_grp = 7;
             else if (line <= 550000) line_grp = 8;
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
                out_source_fee = line * GX_OUT_SOURCE_RATE; // 委外佣金
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
    case KHJ:if (line <= 100000) line_grp = 0;
             else if (line <= 120000) line_grp = 1;
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
                out_source_fee = line * KHJ_OUT_SOURCE_RATE; // 委外佣金
                out_source_bonus = KHJ_OUT_SOURCE_BONUS; //委外銷售獎金
             }
             else
                out_source_fee = out_source_bonus = 0.0;

             if (HeadBonusDiscount [KHJ][channel] == 1)       // 主管手續獎金
                head_bonus = application_fee / KHJ_APP_FEE_RECEIVABLE
                             * HeadFeeBonus [KHJ][channel];
             else head_bonus = HeadFeeBonus [KHJ][channel];

             if (SalesBonusDiscount [KHJ][channel] == 1)      // 業務手續獎金
                sales_bonus = application_fee / KHJ_APP_FEE_RECEIVABLE
                             * SalesFeeBonus [KHJ][channel];
             else sales_bonus = SalesFeeBonus [KHJ][channel];

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
     if (product_type ==1)  // GX
        collection_cost[i] = GX_D1_FIX_COST * open_attrition[i] * monthly_pd *
                             GX_D1_WRITEOFF;
     else if (product_type == 2)  // KHJ
        collection_cost[i] = KHJ_D1_FIX_COST * open_attrition[i] * monthly_pd *
                             KHJ_D1_WRITEOFF;
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

int Loan::get_test_PB(char *idn, TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);
// double pb;

 ds->EnableBCD = false;  // Decimal fields are mapped to float.

  try {
    hostVars[0] = app_sn;
    handler->ExecSQLQry(SQLCommands[Get_PB_test], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["card"].IsNull())
          card = ds->FieldByName("card")->AsFloat;
//       if (! ds->FieldValues["rscore"].IsNull())
//          rscore = ds->FieldByName("rscore")->AsFloat;
    }
//    pd = pb;
 } catch (Exception &E) {
    throw;
 }
 return (card);
}

