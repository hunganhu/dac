//---------------------------------------------------------------------------
#pragma hdrstop

#include "fmNPV.h"
// Constants used in the program.
//const char *expire_date = "21001231";
//---------------------------------------------------------------------------

#pragma package(smart_init)
double recovery_ratio[2] = {0.7, 0.8};

Loan::Loan (char *case_no):
    case_no(case_no)
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

int Loan::app_info_validate(char *case_no, TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);
 String sql_stmt;

 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 code = 0;
 Message = "";
 try {
    hostVars[0] = case_no;
    handler->ExecSQLQry(SQLCommands[Get_AppInfo_Record], hostVars, 0, ds);
    record_count = ds->RecordCount;

    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["APP_DATE"].IsNull())
          app_date = ds->FieldValues["APP_DATE"];
       else
          app_date_ind = -1;

       if (! ds->FieldValues["PROD_TYPE"].IsNull())
          prod_type = ds->FieldValues["PROD_TYPE"];
       else
          prod_type_ind = -1;

       if (! ds->FieldValues["APP_ID"].IsNull())
          app_id = ds->FieldValues["APP_ID"];
       else
          app_id_ind = -1;

       if (! ds->FieldValues["APP_INCOME"].IsNull())
          app_income = ds->FieldValues["APP_INCOME"];
       else
          app_income_ind = -1;

       if (! ds->FieldValues["COS_ID"].IsNull())
          cos_id = ds->FieldValues["COS_ID"];
       else
          cos_id_ind = -1;

       if (! ds->FieldValues["COS_INCOME"].IsNull())
          cos_income = ds->FieldValues["COS_INCOME"];
       else
          cos_income_ind = -1;

       if (! ds->FieldValues["GUA_ID"].IsNull())
          gua_id = ds->FieldValues["GUA_ID"];
       else
          gua_id_ind = -1;

       if (! ds->FieldValues["GUA_INCOME"].IsNull())
          gua_income = ds->FieldValues["GUA_INCOME"];
       else
          gua_income_ind = -1;

       if (! ds->FieldValues["APP_AMT"].IsNull())
          app_amt = ds->FieldValues["APP_AMT"];
       else
          app_amt_ind = -1;

       if (! ds->FieldValues["PERIOD"].IsNull())
          periods = ds->FieldValues["PERIOD"];
       else
          periods_ind = -1;

       if (! ds->FieldValues["APR1"].IsNull())
          apr1 = ds->FieldValues["APR1"];
       else
          apr1 = 0.0;

       if (! ds->FieldValues["SEG1"].IsNull())
          seg1 = ds->FieldValues["SEG1"];
       else
          seg1 = 0;

       if (! ds->FieldValues["APR2"].IsNull())
          apr2 = ds->FieldValues["APR2"];
       else
          apr2 = 0.0;

       if (! ds->FieldValues["SEG2"].IsNull())
          seg2 = ds->FieldValues["SEG2"];
       else
          seg2 = 0;

       if (! ds->FieldValues["APR3"].IsNull())
          apr3 = ds->FieldValues["APR3"];
       else
          apr3 = 0.0;

       if (! ds->FieldValues["SEG3"].IsNull())
          seg3 = ds->FieldValues["SEG3"];
       else
          seg3 = 0;

       if (! ds->FieldValues["GRACE_PERIOD"].IsNull())
          grace_period = ds->FieldValues["GRACE_PERIOD"];
       else
          grace_period = 0;

       if (! ds->FieldValues["APP_FEE"].IsNull())
          app_fee = ds->FieldValues["APP_FEE"];
       else
          app_fee = 0;

       if (! ds->FieldValues["GAV"].IsNull())
          gav = ds->FieldValues["GAV"];
       else
          gav = 0.0;

       if (!ds->FieldValues["NAV"].IsNull()) {
          nav = ds->FieldValues["NAV"];
       }
       else
          nav = 0.0;

       if (!ds->FieldValues["PREMIUM_COL"].IsNull())
          premium_col = ds->FieldValues["PREMIUM_COL"];
       else
          premium_col_ind = -1;

       if (!ds->FieldValues["MONTHLY_PAYMENT"].IsNull())
          monthly_payment = ds->FieldValues["MONTHLY_PAYMENT"];
       else
          monthly_payment_ind = -1;

//  if (trial_count == 0)  {
//     Message += "無申請件資料 "; code = 313;}
//  else {
//     if ((principal_ind == -1) || (product_type == 1 && (principal < 100000.0 || principal > 600000.0))) {
//        Message += TF_Messages[Loan_error_331_GX]; code = 331;}
//     else if ((principal_ind == -1) || (product_type == 2 && (principal < 70000.0 || principal > 200000.0))){
//        Message += TF_Messages[Loan_error_331_KHJ]; code = 331;}
//
//     if ((int_rate_ind == -1) || (int_rate < 0.0)||(int_rate > 0.20)) {
//          Message += TF_Messages[Loan_error_332]; code = 332;}
//
//     if ((periods_ind == -1) || (periods < 13) || (periods > 84)) {
//        Message += TF_Messages[Loan_error_333];  code = 333;}
//
//     if ((application_fee_ind == -1) || (application_fee < 0)) {
//        Message += TF_Messages[Loan_error_334];  code = 334;}
//
//     if ((credit_checking_fee_ind == -1) || (credit_checking_fee < 0)) {
//        Message += TF_Messages[Loan_error_335];  code = 335;}
//
//     if ((risk_mgmt_fee_ind == -1) || (risk_mgmt_fee < 0)) {
//        Message += TF_Messages[Loan_error_336];  code = 336;}
//
//     if ((risk_mgmt_fee_terms_ind == -1) || (risk_mgmt_fee_terms < 0) || (risk_mgmt_fee_terms > periods)) {
//        Message += TF_Messages[Loan_error_337];  code = 337;}
//
//     if ((teaser_period_ind == -1) || (teaser_period < 0) || (teaser_period >= periods)) {
//        Message +=TF_Messages[Loan_error_338];   code = 338;}
//
//     if ((grace_period_ind == -1) || (grace_period < 0) || (grace_period >= periods)) {
//        Message += TF_Messages[Loan_error_339];  code = 339;}
//
//     if ((teaser_rate_ind == -1) || (teaser_rate < 0.0) || (teaser_rate >= int_rate)) {
//          Message += TF_Messages[Loan_error_340];  code = 340;}
//
//     if (!validate_date(ts_date)) {
//        Message += TF_Messages[App_error_316]; code = 316;}
  }
 } catch (Exception &E) {
    ds->Close();  // close dataset before delete and drop an object outside the try block,
                // otherwise result in "too many consecutive exceptions"
    delete ds;
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
double Loan::secured_pb(double first_mortgage_amount, int pdaco_twentile, double nav)
{
 // =============VARAIBLE DEFINITION=============
 // First_mortgage_amount in K
 // NAV in K
 // pdaco twentile : use KTB's pdaco twentile cut
 
 double loan_ratio = (first_mortgage_amount / nav) * 100;
 double loan_ratio2_tran_q = loan_ratio * loan_ratio;
 double p_twen_ratio_tran_aq = (pdaco_twentile * pdaco_twentile) * loan_ratio2_tran_q;
 // double fm_score, fm_pb;   // use class variable
 
 // =============SCORING FORMULA=============
 fm_score = -0.01997 +
 	p_twen_ratio_tran_aq  * 1.657801E-8 +
 	first_mortgage_amount * 0.00000274  +
 	loan_ratio2_tran_q    * 0.00000156;
 
 // =============PB ASSIGNMENT===============
 if (fm_score <= 0.005265642)      fm_pb = 0.002;
 else if (fm_score <= 0.007482222) fm_pb = 0.003;
 else if (fm_score <= 0.008836151) fm_pb = 0.004;
 else if (fm_score <= 0.010255733) fm_pb = 0.005;
 else if (fm_score <= 0.011781021) fm_pb = 0.006;
 else if (fm_score <= 0.013190738) fm_pb = 0.007;
 else if (fm_score <= 0.014524572) fm_pb = 0.008;
 else if (fm_score <= 0.015589203) fm_pb = 0.009;
 else if (fm_score <= 0.016951404) fm_pb = 0.010;
 else if (fm_score <= 0.018088879) fm_pb = 0.011;
 else if (fm_score <= 0.019078364) fm_pb = 0.012;
 else if (fm_score <= 0.020291099) fm_pb = 0.013;
 else if (fm_score <= 0.02102386)  fm_pb = 0.014;
 else if (fm_score <= 0.022872703) fm_pb = 0.015;
 else if (fm_score <= 0.02423022)  fm_pb = 0.016;
 else if (fm_score <= 0.025368277) fm_pb = 0.017;
 else if (fm_score <= 0.027393801) fm_pb = 0.018;
 else if (fm_score <= 0.028823742) fm_pb = 0.019;
 else if (fm_score <= 0.030351926) fm_pb = 0.020;
 else if (fm_score <= 0.032277837) fm_pb = 0.021;
 else if (fm_score <= 0.034372936) fm_pb = 0.022;
 else if (fm_score <= 0.036023876) fm_pb = 0.023;
 else if (fm_score <= 0.038486192) fm_pb = 0.024;
 else if (fm_score <= 0.040347679) fm_pb = 0.025;
 else if (fm_score <= 0.043045046) fm_pb = 0.026;
 else if (fm_score <= 0.045361163) fm_pb = 0.027;
 else if (fm_score <= 0.047448938) fm_pb = 0.028;
 else if (fm_score <= 0.048981471) fm_pb = 0.029;
 else if (fm_score <= 0.051403843) fm_pb = 0.030;
 else if (fm_score <= 0.054500449) fm_pb = 0.031;
 else if (fm_score <= 0.057261352) fm_pb = 0.032;
 else if (fm_score <= 0.060483419) fm_pb = 0.033;
 else if (fm_score <= 0.064944046) fm_pb = 0.034;
 else if (fm_score <= 0.069040716) fm_pb = 0.035;
 else if (fm_score <= 0.08892801)  fm_pb = 0.036;
 else if (fm_score <= 0.121612092) fm_pb = 0.037;
 else fm_pb = 0.038;

 return fm_pb;
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
int Loan::exist_applicant()
{
 if (app_id_ind != -1) return 1;
 else return 0;
}
//---------------------------------------------------------------------------
int Loan::exist_coapplicant()
{
 if (cos_id_ind != -1) return 1;
 else return 0;
}
//---------------------------------------------------------------------------
int Loan::exist_guarantor()
{
 if (gua_id_ind != -1) return 1;
 else return 0;
}
//---------------------------------------------------------------------------
char * Loan::Applicant ()
{
 return app_id.c_str();
}
//---------------------------------------------------------------------------
char * Loan::Cosigner ()
{
 return cos_id.c_str();
}
//---------------------------------------------------------------------------
char * Loan::Guarantor ()
{
 return gua_id.c_str();
}
//---------------------------------------------------------------------------
String Loan::Inquiry_date()
{
 return inquiry_date;
}
//---------------------------------------------------------------------------
double Loan::get_pd()
{
 return fm_pb;
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
/*
int Loan::get_external_monthly_payment()
{
 return ms082;
}
*/
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
//      npv[i][1] = secured_pb(npv[i][0], handler);
      if ((npv[i][2] = calculate_npv(npv[i][0], npv[i][1])) > max_npv) {
           max_npv = npv[i][2];
           max_line = i;
      }
    }
    fm_pb = npv[max_line][1];
    total_npv = npv[max_line][2];
 } catch (Exception &E) {
     throw;
 }
 return (max_line);
}

//---------------------------------------------------------------------------
double Loan::calculate_npv(int line, double pb)
{
 double Interest_Revenue, Setup_Revenue, Late_Fee;
 double Interest_Cost, Commission, Setup_Cost, Acct_Mgmt_Cost, Late_Cost, Collection_Cost;
 double Credit_Loss, Working_Capital;

 try {
  npv_init();
  set_apr(0.0);
  set_attrition(pb);
  set_annuity(line);  // 本息法
  // Revenue
  Interest_Revenue = set_interest_revenue();
  Setup_Revenue = set_setup_fee();
  Late_Fee = set_late_fee(pb);
//  Open_Credit_Fee = set_open_credit_revenue();

  // Cost
  Interest_Cost = set_interest_cost();
  Commission =  calculate_commission();
  Setup_Cost = setup_cost();
  Acct_Mgmt_Cost = set_account_management_cost();
  Late_Cost = set_late_cost(pb);
  Collection_Cost = set_collection_cost();

  Working_Capital = set_working_capital();
  Credit_Loss = set_credit_loss(pb);

  total_npv = (Interest_Revenue + Setup_Revenue + Late_Fee)  // Revenue
               - (Interest_Cost + Commission + Setup_Cost + Acct_Mgmt_Cost
                  + Late_Cost + Collection_Cost )               // Cost
                  + (Working_Capital + Credit_Loss);       // Working Capital

 } catch (Exception &E) {
     throw;
 }
#ifdef _WRNPV
     fstream outf;
     outf.open("NPV_flows.txt", ios::app | ios::out);  // Open for ouput and append

     outf << "Case SN: " << app_sn.c_str() << "   appDate: " << app_date.c_str()
          << "   jcicDate: " << jcic_date.c_str() << "   Commission: " << commission
          << "   PB: " << pd << "   NPV: " << total_npv << endl;
     outf << "TERM       APR  Open_Atr   Vol_Atr Invol_Atr  "
          << "      OsPrinL        P_Repay        I_Repay         IntRev          LateF"
          << "        IntCost       AcctCost     Late_cost           Collect"
          << "      WorkCap     CreditLoss"  << endl;
     outf << "==============================================================================================="
          << "======================================================================"
          << "======================================================================" << endl;
     outf <<  showpoint
              << setw(89) << setprecision(8) << "  "
              << setw(15) << setprecision(8) << Interest_Revenue
              << setw(15) << setprecision(8) << Late_Fee
              << setw(15) << setprecision(8) << Interest_Cost
              << setw(15) << setprecision(8) << Acct_Mgmt_Cost
              << setw(15) << setprecision(8) << Late_cost
              << setw(15) << setprecision(8) << Collection_Cost
              << setw(15) << setprecision(8) << Working_Capital
              << setw(15) << setprecision(8) << Credit_Loss
              << endl;
     for (int i = 0; i <= periods; i++) {
         outf << setprecision(8) << showpoint
              << setw(4)  << i
              << setw(10) << setprecision(4) << apr[i]
              << setw(10) << setprecision(4) << open_attrition[i]
              << setw(10) << setprecision(4) << voluntary_attrition[i]
              << setw(10) << setprecision(4) << involuntary_attrition[i]
              << setw(15) << setprecision(8) << os_principal[i]
              << setw(15) << setprecision(8) << principal_repayment[i]
              << setw(15) << setprecision(8) << interest_repayment[i]
              << setw(15) << setprecision(8) << interest_revenue[i]
              << setw(15) << setprecision(8) << late_fee[i]
              << setw(15) << setprecision(8) << interest_cost[i]
              << setw(15) << setprecision(8) << account_management_cost[i]
              << setw(15) << setprecision(8) << late_cost[i]
              << setw(15) << setprecision(8) << collection_cost[i]
              << setw(15) << setprecision(8) << working_capital[i]
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
     vol_attrition_open[i] = 0.0;
//     open_credit_fee[i] = 0.0;
//     risk_mgmt_revenue[i] = 0.0;
     os_principal[i] = 0.0;
     principal_repayment[i] = 0.0;
     interest_repayment[i] = 0.0;
     monthly_repayment[i] = 0.0;
     interest_revenue[i] = 0.0;
     late_fee[i] = 0.0;
//     open_credit_fee[i] = 0.0;
//     risk_mgmt_revenue[i] = 0.0;
     interest_cost[i] = 0.0;
     account_management_cost[i] = 0.0;
     late_cost[i] = 0.0;
     collection_cost[i] = 0.0;
     working_capital[i] = 0.0;
     credit_loss[i] = 0.0;
  }
}

//---------------------------------------------------------------------------
void Loan::set_apr(double delta_apr)
{
  apr[0] = 0.0;
  int i, j;

  j = 0;
  apr[j] = 0.0;
  for (i = 0; i < seg1; i++)
      apr[++j] = (apr1 + delta_apr < 0 ? 0 : apr1 + delta_apr) / 12.0;

  for (i = 0; i < seg2; i++)
      apr[++j] = (apr2 + delta_apr < 0 ? 0 : apr2 + delta_apr) / 12.0;

  for (i = 0; i < seg3; i++)
      apr[++j] = (apr3 + delta_apr < 0 ? 0 : apr3 + delta_apr) / 12.0;

}

//---------------------------------------------------------------------------
void Loan::set_attrition(double pb)
{
  double monthly_pd = pb / 12.0;
  double M1_120_base = 0.0063;
  double M1_120_slope = 0.00004;
  double M120_base = 0.0125;
  double M120_slope = 0.0002;

  open_attrition[0] = 1.0;
  voluntary_attrition[0] = involuntary_attrition[0] = 0.0;
  for (int i = 1; i <= periods; i++) {
      if (i < 120) vol_attrition_open[i] = M1_120_base + i * M1_120_slope;
      else if (i == 120) vol_attrition_open[i] = 0.025;
      else if (i < 179) vol_attrition_open[i] = M120_base + (i - 120) * M120_slope;
      else if (i == 179) vol_attrition_open[i] = 0.05;
      else if (i == 180) vol_attrition_open[i] = 0.5;
      else vol_attrition_open[i] = 0.05;

      voluntary_attrition[i] = open_attrition[i-1] * vol_attrition_open[i];
      involuntary_attrition[i] = open_attrition[i-1] * monthly_pd;
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
      monthly_repayment[i] = principal_repayment[i] + interest_repayment[i];
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
  monthly_repayment [0] = 0.0;
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
     monthly_repayment[i] = principal_repayment[i] + interest_repayment[i];
  }
}

//---------------------------------------------------------------------------
double Loan::set_interest_revenue()
{
  for (int i = 1; i <= periods; i++)
     interest_revenue[i] = os_principal[i-1] * apr[i] * open_attrition[i];

  return (NetPresentValue(ROE / 12.0, interest_revenue + 1, periods, ptEndOfPeriod)
          + interest_revenue[0]);
}

//---------------------------------------------------------------------------
double Loan::set_setup_fee()
{
 return (SETUP_FEE);
}

//---------------------------------------------------------------------------
double Loan::set_late_fee(double pb)
{
  double late = pb / 12.0 * 2.0;

  for (int i = 3; i <= periods; i++)
     late_fee[i] = monthly_repayment[i] * open_attrition[i-1] * apr[i]
                   *  LATE_PENALTY_RATIO * late;

  return (NetPresentValue(ROE / 12.0, late_fee + 1, periods, ptEndOfPeriod)
          + late_fee[0]);
}

//---------------------------------------------------------------------------
// Cost
// Interest Cost:
double Loan::set_interest_cost()
{
  for (int i = 1; i <= periods; i++)
     interest_cost[i] = os_principal[i-1] * open_attrition[i-1] * LEVERAGE_RATIO * COF / 12.0;
  return (NetPresentValue(ROE / 12.0, interest_cost + 1, periods, ptEndOfPeriod)
          + interest_cost[0]);
}

//---------------------------------------------------------------------------
double Loan::calculate_commission()
{
  return(principal * COMMISSION_RATE + app_fee * FEE_COMMISSION_RATE);
}

//---------------------------------------------------------------------------
double Loan::setup_cost()
{
  return(ALLOGRAPH_FEE + LAND_ADM_FEE + DUEI_BAO_FEE);
}

//---------------------------------------------------------------------------
double Loan::set_account_management_cost()
{
  for (int i = 1; i <= periods; i++)
     account_management_cost[i] = ACCT_MGMT_COST * open_attrition[i-1];
  return (NetPresentValue(ROE / 12.0, account_management_cost + 1, periods, ptEndOfPeriod)
          + account_management_cost[0]);
}

//---------------------------------------------------------------------------
double Loan::set_late_cost(double pb)
{
  double late = pb / 12.0 * 2.0;

  for (int i = 3; i <= periods; i++)
     late_cost[i] = M2_3_EXPENSE * open_attrition[i-1] * late;

  return (NetPresentValue(ROE / 12.0, late_cost + 1, periods, ptEndOfPeriod)
          + late_fee[0]);
}

//---------------------------------------------------------------------------
// Collection Cost (legal)
double Loan::set_collection_cost()
{
 double monthly_pb = fm_pb / 12.0;
// double discount_rate = 1 / pow((1 + ROE / 12.0), TIME_TO_RECOVER);
 double recovery_amount = min(nav, principal * recovery_ratio[premium_col]);
 double wacc = 1.5 * (COF * LEVERAGE_RATIO + ROE * (1 - LEVERAGE_RATIO));

 for (int i=5; i <= periods; ++i)
    collection_cost[i] = monthly_pb *  open_attrition[i-1] *
                   (M4P_EXPENSE + (os_principal[i] * LEGAL_FEE_RATE) +
                   (wacc * recovery_amount));

  return (NetPresentValue(ROE / 12.0, collection_cost + 1, periods, ptEndOfPeriod)
          + collection_cost[0]);
}
//---------------------------------------------------------------------------
// Working Capital
double Loan::set_working_capital()
{
 working_capital [0] = -os_principal[0] * (1 - LEVERAGE_RATIO);
 for (int i = 1; i <= periods; i++) {
    working_capital [i] = (os_principal[i-1]* open_attrition[i-1] -
                           os_principal[i] * open_attrition[i]) *
                           (1- LEVERAGE_RATIO);
 }
 return (NetPresentValue(ROE / 12.0, working_capital + 1, periods, ptEndOfPeriod)
          + working_capital[0]);
}

//---------------------------------------------------------------------------
// Credit loss:
double Loan::set_credit_loss(double pb)
{
 double monthly_pd = pb / 12.0;
 double discount_rate = 1 / pow((1 + ROE / 12.0), 12);
 double recovery_amount = min(nav, principal * recovery_ratio[premium_col]);
 credit_loss [0] = 0.0;
 for (int i = 1; i <= periods; i++) {
    credit_loss [i] = min(0, (os_principal[i-1] - recovery_amount * discount_rate))
                      * open_attrition[i-1] * monthly_pd;
 }
  return (NetPresentValue(ROE / 12.0, credit_loss + 1, periods, ptEndOfPeriod)
          + credit_loss[0]);
}

//---------------------------------------------------------------------------

