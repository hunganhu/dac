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
#ifndef _WRFLOW
 #define DEBUG 0
#else
 #define DEBUG 1
#endif
// Constants used in the program.
const char *expire_date = "21001231";
const double m1_to_m7_ratio = 5.0;
const double YearMonths = 12.0;
const double YearDays = 365.0;

//---------------------------------------------------------------------------

#pragma package(smart_init)

Loan::Loan (char * caseNo, char* pid, int seq, TADOHandler *handler):
    case_sn(caseNo), idn(pid), dac_sn(seq), application_date_ind(0),
    product_code_ind(0), project_source_ind(0), case_source_ind(0),
    recommender_ind(0), guarantor_ind(0), principal_ind(0),
    repayment_ind(0), periods_ind(0), grace_period_ind(0),
    num_int_period_ind(0), appropriation_ind(0), zip_ind(0),
    segment_ind(0), application_fee_ind(0), risk_mgmt_fee_ind(0),
    acct_mgmt_fee_ind(0), bt_fee_ind(0), inquiry_date_ind(0)
{
 Variant hostVars[5];
 ds = new TADODataSet(NULL);
 ds->EnableBCD = false;  // Decimal fields are mapped to float.

 try {
    hostVars[0] = case_sn;
    hostVars[1] = idn;
    hostVars[2] = dac_sn;
    handler->ExecSQLQry(SQLCommands[Get_AppInfo_Record], hostVars, 2, ds);
    record_count = ds->RecordCount;

    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["application_date"].IsNull())
          application_date = ds->FieldValues["application_date"];
       else
          application_date_ind = -1;

       if (! ds->FieldValues["inquiry_date"].IsNull())
          inquiry_date = ds->FieldValues["inquiry_date"];
       else
          inquiry_date_ind = -1;
/*
       if (! ds->FieldValues["product_code"].IsNull())
          product_code = ds->FieldValues["product_code"];
       else
          product_code_ind = -1;
       if (! ds->FieldValues["project_source"].IsNull())
          project_source = ds->FieldValues["project_source"];
       else
          project_source_ind = -1;
       if (! ds->FieldValues["case_source"].IsNull())
          case_source = ds->FieldValues["case_source"];
       else
          case_source_ind = -1;
       if (! ds->FieldValues["recommender"].IsNull())
          recommender = ds->FieldValues["recommender"];
       else
          recommender_ind = -1;
*/
       if (! ds->FieldValues["guarantor"].IsNull())
          guarantor = ds->FieldValues["guarantor"];
       else
          guarantor_ind = -1;

       if (! ds->FieldValues["principal"].IsNull())
          principal = ds->FieldValues["principal"];
       else
          principal_ind = -1;

       if (! ds->FieldValues["repayment"].IsNull())
          repayment = ds->FieldValues["repayment"];
       else
          repayment_ind = -1;

       if (! ds->FieldValues["periods"].IsNull())
          periods = ds->FieldValues["periods"];
       else
          periods_ind = -1;

       if (! ds->FieldValues["grace_period"].IsNull())
          grace_period = ds->FieldValues["grace_period"];
       else
          grace_period_ind = -1;

       if (! ds->FieldValues["num_int_period"].IsNull())
          num_int_period = ds->FieldValues["num_int_period"];
       else
          num_int_period_ind = -1;

       if (! ds->FieldValues["appropriation"].IsNull())
          appropriation = ds->FieldValues["appropriation"];
       else
          appropriation_ind = -1;

       if (! ds->FieldValues["zip"].IsNull())
          zip = ds->FieldValues["zip"];
       else
          zip_ind = -1;

       if (! ds->FieldValues["segment"].IsNull())
          segment = ds->FieldValues["segment"];
       else
          segment_ind = -1;

       if (! ds->FieldValues["application_fee"].IsNull())
          application_fee = ds->FieldValues["application_fee"];
       else
          application_fee_ind = -1;

       if (! ds->FieldValues["risk_mgmt_fee"].IsNull())
          risk_mgmt_fee = ds->FieldValues["risk_mgmt_fee"];
       else
          risk_mgmt_fee_ind = -1;

       if (! ds->FieldValues["acct_mgmt_fee"].IsNull())
          acct_mgmt_fee = ds->FieldValues["acct_mgmt_fee"];
       else
          acct_mgmt_fee_ind = -1;

       if (!ds->FieldValues["bt_fee"].IsNull())
          bt_fee = ds->FieldValues["bt_fee"];
//          bt_fee = ds->FieldByName("bt_fee")->AsFloat;
       else
          bt_fee_ind = -1;
    }
    handler->ExecSQLQry(SQLCommands[Get_AppI_Record], hostVars, 2, ds);

    ds->First();
    int seg;
    while (!ds->Eof) {
       double apr = 0.0;
       int period = 0, apr_ind = 0, period_ind = 0;

       if (!ds->FieldValues["apr"].IsNull())
          apr = ds->FieldValues["apr"];
       else
          apr_ind = -1;
       seg = ds->FieldValues["seq"];
       if (!ds->FieldValues["periods"].IsNull())
          period = ds->FieldValues["periods"];
       else
          period_ind = -1;

       rateList.push_back(Rate(apr / 100.0, period, seg, apr_ind, period_ind));
       ds->Next();
    }
 } catch (Exception &E) {
    throw;
 }
}

void Loan::validate()
{
  int sum_periods = 0;
  int dist;
  bool success = true;

  if (record_count == 0) {
     throw DataEx("無申請件資料。");
  }
  if ((application_date_ind == -1) || !validate_date(application_date)) {
     Message += "申請日期格式錯誤。";
     success = false;
  }
  if ((inquiry_date_ind == -1) || !validate_date(inquiry_date)) {
     Message += "JCIC查詢日期格式錯誤。";
     success = false;
  }
  if ((principal_ind == -1) || (principal < 10000.0) || (principal > 9000000.0)) {
     Message += "申貸金額必須介於10,000和9,000,000（元）。";
     success = false;
  }

  if ((periods_ind == -1) || (periods < 1) || (periods > 120)) {
     Message += "貸款期數錯誤，必須介於1和120。";
     success = false;
  }

  if ((repayment_ind == -1) || (repayment < "1") || (repayment > "3")) {
     Message += "還款方式錯誤，必須是1(一般法)，2(本息法) 或3(本金法)。";
     success = false;
  }

  if ((grace_period_ind == -1) || (grace_period < 0) || (grace_period > periods)) {
     Message += "寬限期錯誤，必須介於0和貸款期數。";
     success = false;
  }
  if ((repayment == "1") && (periods != grace_period)) {
     Message += "錯誤:還款方式為1(一般法)時，寬限期必須等於貸款期數。";
     success = false;
  }

  if ((guarantor_ind == -1) || (guarantor != "1") && (guarantor != "0")) {
     Message += "保人錯誤，必須是0(無保人) 或1(有保人)。";
     success = false;
  }
  if ((segment_ind == -1) || ((segment != "99") && ((segment < "00") ||(segment > "08")))) {
     Message += "客層錯誤，必須是01 ~ 08 或99。";
     success = false;
  }

  if ((application_fee_ind == -1) || (application_fee < 0) || (application_fee > 100000)) {
     Message += "手續費必須介於0和100000（元）。";
     success = false;
  }

  if ((risk_mgmt_fee_ind == -1) || (risk_mgmt_fee < 0) || (risk_mgmt_fee > 100000)) {
     Message += "風險管理費必須介於0和100000（元）。";
     success = false;
  }

  if ((acct_mgmt_fee_ind == -1) || (acct_mgmt_fee < 0) || (acct_mgmt_fee > 100000)) {
     Message += "帳戶管理費必須介於0和100000（元）。";
     success = false;
  }

  if ((bt_fee_ind == -1) || (bt_fee < 0) || (bt_fee > 100000)) {
     Message += "代償費必須介於0和100000（元）。";
     success = false;
  }

  if ((appropriation_ind == -1) || appropriation != "1") {
     Message += "撥款方式錯誤, 必須是1(一次撥貸)。";
     success = false;
  }

  if ((num_int_period_ind == -1) || ((num_int_period < 1) || (num_int_period > 3))) {
     Message += "利率時段個數錯誤，必須是1, 2, 或 3。";
     success = false;
  }
  if (static_cast <int> (rateList.size()) != num_int_period) {
     Message += "貸款利率表時段個數錯誤。";
     success = false;
  }
  max_apr = 0.0;
  int seq = 0;
  for (iter1 = rateList.begin(); iter1 != rateList.end(); ++iter1) {
    if (++seq != (*iter1).get_segment()) {
       Message = Message + "第" + seq + "期貸款利率表時段錯誤。";
       success = false;
    }
    if (((*iter1).get_rate_ind() == -1) || ((*iter1).get_rate() < 0.0)
        ||((*iter1).get_rate() > 0.20)) {
       Message = Message + "第" + seq + "期貸款利率錯誤，必須介於0和20(%)。";
       success = false;
    }
    else {
       if ((*iter1).get_rate() > max_apr) max_apr = (*iter1).get_rate();
    }
    if (((*iter1).get_period_ind() == -1) || ((*iter1).get_period() < 0)
        || ((*iter1).get_period() > periods)) {
       Message = Message + "第" + seq + "期貸款利率表期數錯誤。";
       success = false;
    }
    sum_periods += (*iter1).get_period();
    if (! success) break;
  }

  if (sum_periods != periods) {
     Message += "貸款利率表總期數錯誤。";
     success = false;
  }

  if ((dist = validZIP(zip.c_str())) == 0) {
     Message += "郵遞區號錯誤。";
     success = false;
  }
  else {
     if (dist >= 1 && dist <= 3)         // 1: North, the 1st digit of zip code
        district = 1;                    //    is 1, 2, or 3.
     else if (dist >= 4 && dist<= 6)     // 2: Central, the 1st digit of zip code
        district = 2;                    //    is 4, 5, or 6.
     else if (dist >= 7 && dist<= 9)     // 3: South, the 1st digit of zip code
        district = 3;                    //    is 7, 8, or 9.
  }

  if (!success ) throw DataEx(Message);
}

Loan::~Loan ()
{
 ds->Close();
 delete ds;
}

String Loan::error ()
{
 return Message;
}
String Loan::get_application_date ()
{
 return application_date;
}
double Loan::get_rscore ()
{
 return rscore;
}
double Loan::get_pd ()
{
 return pd;
}
double Loan::get_npv ()
{
 return total_npv;
}
double Loan::get_principal ()
{
 return principal;
}
String Loan::get_segment ()
{
 return segment;
}

void Loan::calculate_pd(TADOHandler *handler)
{
 Variant hostVars[5];
// int avail_flag, jas002_defect, krm021_hit, krm023_hit, fs044;
// int max_bucket, cash_max_bucket, cash_utilization, ind001;
 int now;
 // bool success = true;
 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 now = yrmon_to_mon(inquiry_date, false, expire_date);
 avail_flag = jas002_defect = krm021_hit = krm023_hit = fs044 = 0;
 try {
    handler->ExecSQLCmd(SQLCommands[Create_Working_Tables]);
    hostVars[0] = case_sn;
    hostVars[1] = idn;
    hostVars[2] = dac_sn;
    hostVars[3] = now;
    handler->ExecSQLCmd(SQLCommands[Insert_Daco_Table], hostVars, 3);

    handler->ExecSQLCmd(SQLCommands[Drop_Procedure_Prepare_Jcic_Data]);
    handler->ExecSQLCmd(SQLCommands[Create_Procedure_Prepare_Jcic_Data]);
    hostVars[0] = case_sn;
    hostVars[1] = idn;
    hostVars[2] = dac_sn;
    hostVars[3] = now;
    handler->ExecSQLCmd(SQLCommands[Exec_Procedure_Prepare_Jcic_Data], hostVars, 3);
    handler->ExecSQLCmd(SQLCommands[Drop_Procedure_Prepare_Jcic_Data]);
    handler->ExecSQLCmd(SQLCommands[Update_Base]);
    handler->ExecSQLCmd(SQLCommands[Update_Inquiry_Date]);

    hostVars[0] = idn;
    handler->ExecSQLQry(SQLCommands[Get_Filter_Result], hostVars, 0, ds);
    ds->First();
    if (!ds->Eof) {
       avail_flag = ds->FieldValues["avail_flag"];
       jas002_defect = ds->FieldValues["jas002_defect"];
       krm021_hit = ds->FieldValues["krm021_hit"];
       krm023_hit = ds->FieldValues["krm023_hit"];
       max_bucket = ds->FieldValues["max_bucket"];
       fs044 = ds->FieldValues["fs044"];
       cash_max_bucket = ds->FieldValues["cash_max_bucket"];
       cash_utilization = ds->FieldValues["cash_utilization"];
       ind001 = ds->FieldValues["ind001"];
    }
    if (avail_flag == 0)
        throw (RiskEx ("人工審核 [無JCIC資料]", 101));
/*    else if (jas002_defect > 0)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 103));
    else if (max_bucket >= 4)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 104));
    else if (fs044 > 0)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 105));
    else if (cash_max_bucket >= 0.5)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 106));
    else if (cash_utilization >= 1)
        throw (RiskEx ("拒絕 [重大信用瑕疵記錄]", 107));
*/
    else if (krm021_hit ==0 || krm023_hit == 0 || ind001 == 1)
        throw (RiskEx ("人工審核 [JCIC資料不足]", 102));

    handler->ExecSQLCmd(SQLCommands[Update_Base]);

    handler->ExecSQLCmd(SQLCommands[Drop_Procedure_Generate_Daco32_Score]);
    handler->ExecSQLCmd(SQLCommands[Create_Procedure_Generate_Daco32_Score]);

    hostVars[0] = now;
    handler->ExecSQLCmd(SQLCommands[Exec_Procedure_Generate_Daco32_Score], hostVars, 0);

    handler->ExecSQLCmd(SQLCommands[Drop_Procedure_Generate_Ploan_Score]);
    handler->ExecSQLCmd(SQLCommands[Create_Procedure_Generate_Ploan_Score]);

    hostVars[0] = now;
    handler->ExecSQLCmd(SQLCommands[Exec_Procedure_Generate_Ploan_Score], hostVars, 0);
    handler->ExecSQLCmd(SQLCommands[Drop_Procedure_Generate_Ploan_Score]);

//    handler->ExecSQLCmd(SQLCommands[Insert_Intermediate_Table]);
    if (DEBUG)
       handler->ExecSQLCmd(SQLCommands[Insert_Audit_Table]);

    handler->ExecSQLQry(SQLCommands[Get_PD], ds);
    ds->First();
    if (!ds->Eof) {
       rscore = ds->FieldValues["rscore"];
       pd = ds->FieldValues["pd"];
    }

 } catch (Exception &E) {
     throw;
   }
}
void Loan::postFilter()
{
 if (jas002_defect > 0)
    throw (RiskEx ("拒絕 [有退票強停拒往授信異常等記錄]", 103));
 else if (max_bucket >= 4)
    throw (RiskEx ("拒絕 [信用卡有90天以上遲繳記錄]", 104));
 else if (fs044 > 0)
    throw (RiskEx ("拒絕 [貸款有逾期記錄]", 105));
 else if (cash_max_bucket >= 0.5)
    throw (RiskEx ("拒絕 [現金卡前期有逾期記錄]", 106));
 else if (cash_utilization >= 1)
    throw (RiskEx ("拒絕 [現金卡放款額度大於或等於訂約額度]", 107));
}

void Loan::calculate_npv()
{
  double revenue, cost, wc;
  double Interest_Revenue, Late_Fee, Early_Closing_Fee, Application_Fee;
  double Other_Fee, Interest_Cost, Acct_Mgmt_Cost, PreCollection_Cost;
  double Collection_Cost, Commission, Working_Capital, Credit_Loss;

  npv_init();
  set_apr();
  set_attrition();
  if ((repayment == '1') || (repayment == '3'))
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
  if (DEBUG) {
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
  }
}


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

void Loan::set_apr()
{
  int j = 0;
  apr[0] = 0.0;
  for (iter1 = rateList.begin(); iter1 != rateList.end(); ++iter1) {
      for (int i = 0; i < (*iter1).get_period(); i++)
          apr[++j] = (*iter1).get_rate() / 12.0;           // convert APR to monthly rate.
  }
}

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

double Loan::set_interest_revenue()
{
  for (int i = 1; i <= periods; i++)
     interest_revenue[i] = os_principal[i-1] * apr[i] * open_attrition[i];

  return (NetPresentValue(roe / 12.0, interest_revenue + 1, periods, ptEndOfPeriod)
          + interest_revenue[0]);
}
double Loan::set_late_fee()
{
  for (int i = 1; i <= periods; i++)
     late_fee[i] = os_principal[i-1] * apr[i] * YearMonths / YearDays *
                  (m1_attrition[i] * m1_recovery_ratio * m1_avg_late_days * m1_penalty_rate +
                   involuntary_attrition[i] * m6_recovery_ratio * m6_avg_late_days * m6_penalty_rate);

  return (NetPresentValue(roe / 12.0, late_fee + 1, periods, ptEndOfPeriod)
          + late_fee[0]);
}
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
// Cost
// Interest Cost:
double Loan::set_interest_cost()
{
  for (int i = 1; i <= periods; i++)
     interest_cost[i] = os_principal[i-1] * open_attrition[i-1] * leverage_ratio * cof / YearMonths;
  return (NetPresentValue(roe / 12.0, interest_cost + 1, periods, ptEndOfPeriod)
          + interest_cost[0]);
}

// Commission:
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
double Loan::set_precollection_cost()
{
  double monthly_pd = pd / YearMonths;
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
// Collection Cost (legal)
/* 90+ days delinquent will tirgger legal action to collect outstanding principal
   90+ days delinquent is 4 payments behind
*/
double Loan::set_collection_cost()
{
  double monthly_pd = pd / YearMonths;
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

// Credit loss:
double Loan::set_credit_loss()
{
 double discount_rate = 1 / pow((1 + roe / YearMonths),legal_action_period);
 credit_loss [0] = 0.0;
 for (int i = 1; i <= periods; i++) {
    credit_loss [i] = -os_principal[i-1]* involuntary_attrition[i] *
                      (1 - recovery_ratio * discount_rate);
 }
  return (NetPresentValue(roe / 12.0, credit_loss + 1, periods, ptEndOfPeriod)
          + credit_loss[0]);
}

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

       if (! ds->FieldValues["early_closing_period"].IsNull())
          early_closing_period = ds->FieldByName("early_closing_period")->AsInteger;

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

/*
 pd = 0.01;
 rscore = 0.02;
 return (pd);
*/
}
