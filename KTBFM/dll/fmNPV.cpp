/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: fmNPV.cpp
**
** Description: NPV model for first mortgage
**
****************************************************************************/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "fmNPV.h"
// Constants used in the program.
//const char *expire_date = "21001231";
//---------------------------------------------------------------------------

#pragma package(smart_init)
double recovery_ratio[2] = {0.7, 0.8};
double nav_ratio[2] = {0.8, 0.9};
double duei_bao_rate[2] = {0.0, 2.25};   // DUEI_BAO_FEE ($K)

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
          app_income = ds->FieldByName("APP_INCOME")->AsFloat / 1000.0;
       else
          app_income_ind = -1;

       if (! ds->FieldValues["COS_ID"].IsNull())
          cos_id = ds->FieldValues["COS_ID"];
       else
          cos_id_ind = -1;

       if (! ds->FieldValues["COS_INCOME"].IsNull())
          cos_income = ds->FieldByName("COS_INCOME")->AsFloat / 1000.0;
       else
          cos_income_ind = -1;

       if (! ds->FieldValues["GUA_ID"].IsNull())
          gua_id = ds->FieldValues["GUA_ID"];
       else
          gua_id_ind = -1;

       if (! ds->FieldValues["GUA_INCOME"].IsNull())
          gua_income = ds->FieldByName("GUA_INCOME")->AsFloat / 1000.0;
       else
          gua_income_ind = -1;

       if (! ds->FieldValues["APP_AMT"].IsNull())
          app_amt = ds->FieldByName("APP_AMT")->AsFloat / 1000.0;
       else
          app_amt = 0;

       if (! ds->FieldValues["PERIOD"].IsNull())
          periods = ds->FieldValues["PERIOD"];
       else
          periods = 0;

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
          app_fee = ds->FieldByName("APP_FEE")->AsFloat / 1000.0;
       else
          app_fee = 0;

       if (! ds->FieldValues["GAV"].IsNull())
          gav = ds->FieldByName("GAV")->AsFloat / 1000.0;
       else
          gav = 0.0;

       if (!ds->FieldValues["NAV"].IsNull()) {
          nav = ds->FieldByName("NAV")->AsFloat / 1000.0;
       }
       else
          nav = 0.0;

       if (!ds->FieldValues["PREMIUM_COL"].IsNull())
          premium_col = ds->FieldValues["PREMIUM_COL"];
       else
          premium_col = 0;

       if (!ds->FieldValues["MONTHLY_PAYMENT"].IsNull())
          monthly_payment = ds->FieldByName("MONTHLY_PAYMENT")->AsFloat / 1000.0;
       else
          monthly_payment = 0;

    }
 } catch (Exception &E) {
    ds->Close();
    delete ds;
    return -1;
 }
  ds->Close();  // close dataset before delete and drop an object outside the try block,
                // otherwise result in "too many consecutive exceptions"
  delete ds;
  return (0);
}

//---------------------------------------------------------------------------
double Loan::secured_pb()
{
 // =============VARAIBLE DEFINITION=============
 // First_mortgage_amount in K (principal)
 // NAV in K
 // pdaco twentile : use KTB's pdaco twentile cut

 double loan_ratio = (principal / nav) * 100;
 double loan_ratio2_tran_q = loan_ratio * loan_ratio;
 double p_twen_ratio_tran_aq = (pdaco_twentile * pdaco_twentile) * loan_ratio2_tran_q;
 // double fm_score, fm_pb;   // use object variable

 // =============SCORING FORMULA=============
 fm_score = -0.01997 +
 	p_twen_ratio_tran_aq  * 1.657801E-8 +
 	principal             * 0.00000274  +
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

#ifdef _TRACE
     fstream outf;

     outf.open("PDACO_trace.txt", ios::app | ios::out);  // Open for ouput and append
     outf << case_no.c_str() << "," << pdaco_score << "," << pdaco_twentile
          << "," << loan_ratio << "," << loan_ratio2_tran_q << "," << p_twen_ratio_tran_aq
          << "," << fm_score << "," << fm_pb << endl;
#endif
 fm_pb = fm_pb * pb_adjustment;

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
String Loan::Case_no ()
{
 return case_no;
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
int Loan::appIncome()
{
 return app_income;
}
//---------------------------------------------------------------------------
int Loan::cosIncome()
{
 return cos_income;
}
//---------------------------------------------------------------------------
int Loan::guaIncome()
{
 return gua_income;
}

//---------------------------------------------------------------------------
void Loan::set_risk_score (double score)
{
 pdaco_score = score;
}
//---------------------------------------------------------------------------
void Loan::set_monthly_income(double income)
{
 monthly_income = income;
}
//---------------------------------------------------------------------------
void Loan::set_monthly_debt(double debt)
{
 monthly_debt = debt;
}
//---------------------------------------------------------------------------
void Loan::set_risk_twentile (double score)
{
 if      (score <= -0.03231) pdaco_twentile=  1;
 else if (score <= -0.02275) pdaco_twentile=  2;
 else if (score <= -0.01479) pdaco_twentile=  3;
 else if (score <= -0.00919) pdaco_twentile=  4;
 else if (score <= -0.00438) pdaco_twentile=  5;
 else if (score <=  0.00101) pdaco_twentile=  6;
 else if (score <=  0.00624) pdaco_twentile=  7;
 else if (score <=  0.01245) pdaco_twentile=  8;
 else if (score <=  0.01836) pdaco_twentile=  9;
 else if (score <=  0.02482) pdaco_twentile= 10;
 else if (score <=  0.03219) pdaco_twentile= 11;
 else if (score <=  0.03963) pdaco_twentile= 12;
 else if (score <=  0.04759) pdaco_twentile= 13;
 else if (score <=  0.05585) pdaco_twentile= 14;
 else if (score <=  0.06657) pdaco_twentile= 15;
 else if (score <=  0.07865) pdaco_twentile= 16;
 else if (score <=  0.09435) pdaco_twentile= 17;
 else if (score <=  0.11509) pdaco_twentile= 18;
 else if (score <=  0.15002) pdaco_twentile= 19;
 else if (score  >  0.15002) pdaco_twentile= 20;
}
//---------------------------------------------------------------------------
void Loan::set_principal()
{
 double allowance;
 
 weighted_apr = (apr1 * seg1 + apr2 * seg2 + apr3 * seg3) / (seg1 + seg2 + seg3);
 allowance = monthly_income * 0.7 - monthly_debt;
 max_loan_capacity = -PresentValue(weighted_apr/12.0, (seg1 + seg2 + seg3), allowance, 0, ptEndOfPeriod);
 principal = min(min(nav*nav_ratio[premium_col],app_amt),max_loan_capacity);
 if (0 < (app_amt - principal ) && (app_amt - principal ) <= 300)
    principal = app_amt;
}
//---------------------------------------------------------------------------
void Loan::set_pb_adjustment(double score)
{
 if (score <= -0.00919) pb_adjustment = 0.8;
 else if (score <= 0.01836) pb_adjustment = 0.9;
 else pb_adjustment = 1.0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void Loan::set_npv (double npv_value)
{
 total_npv = npv_value;
}
//--------------------------------------------------------------------------------------------------
void Loan::set_lowest_rate (double delta_rate)
{
 lowest_delta = delta_rate;
 if (seg1 > 0)
    min_apr1 = (apr1 + delta_rate < 0 ? 0 : apr1 + delta_rate);
 else
    min_apr1 = 0.0;
 if (seg2 > 0)
    min_apr2 = (apr2 + delta_rate < 0 ? 0 : apr2 + delta_rate);
 else
    min_apr2 = 0.0;
 if (seg3 > 0)
    min_apr3 = (apr3 + delta_rate < 0 ? 0 : apr3 + delta_rate);
 else
    min_apr3 = 0.0;
}
//---------------------------------------------------------------------------
double Loan::get_max_apr()
{
 return (max(max(apr1,apr2),apr3));
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
double Loan::Min_APR1()
{
 return min_apr1;
}
//---------------------------------------------------------------------------
double Loan::Min_APR2()
{
 return min_apr2;
}
//---------------------------------------------------------------------------
double Loan::Min_APR3()
{
 return min_apr3;
}
//---------------------------------------------------------------------------
double Loan::Monthly_Income()
{
 return monthly_income;
}
//---------------------------------------------------------------------------
double Loan::Monthly_Debt()
{
 return monthly_debt;
}
//---------------------------------------------------------------------------
double Loan::Max_Loan_Capacity()
{
 return max_loan_capacity;
}
//---------------------------------------------------------------------------
double Loan::Weighted_APR()
{
 return weighted_apr;
}
//---------------------------------------------------------------------------
double Loan::Lowest_npv()
{
 return min_npv;
}
//---------------------------------------------------------------------------
double Loan::calculate_optimal_npv()
{
 double npv, max_apr;
 double delta_r;

 max_apr = get_max_apr();
 delta_r = -0.01;
 npv = calculate_npv(delta_r);
 // Calculate all NPV between apr - 1% and 10% steps 0.25%
 while ((delta_r + max_apr) <= 0.1 && npv < 2.0) {
    delta_r = delta_r + 0.0025;
    npv = calculate_npv(delta_r);
 }
 min_npv = npv;   // set to class variable
 return (delta_r);
}
//---------------------------------------------------------------------------
double Loan::calculate_npv(double delta_apr)
{
 double Interest_Revenue, Setup_Revenue, Late_Fee;
 double Interest_Cost, Commission, Setup_Cost, Acct_Mgmt_Cost, Late_Cost, Collection_Cost;
 double Credit_Loss, Working_Capital;

 try {
  npv_init();
//  secured_pb();  // comment for test only
  set_apr(delta_apr);
  set_attrition(fm_pb);
  set_annuity(principal);  // ¥»®§ªk
  // Revenue
  Interest_Revenue = set_interest_revenue();
  Setup_Revenue = set_setup_fee();
  Late_Fee = set_late_fee(fm_pb);
//  Open_Credit_Fee = set_open_credit_revenue();

  // Cost
  Interest_Cost = set_interest_cost();
  Commission =  calculate_commission();
  Setup_Cost = setup_cost();
  Acct_Mgmt_Cost = set_account_management_cost();
  Late_Cost = set_late_cost(fm_pb);
  Collection_Cost = set_collection_cost();

  Working_Capital = set_working_capital();
  Credit_Loss = set_credit_loss(fm_pb);

  total_npv = (Interest_Revenue + Setup_Revenue + Late_Fee)  // Revenue
               - (Interest_Cost + Commission + Setup_Cost + Acct_Mgmt_Cost
                  + Late_Cost + Collection_Cost )               // Cost
                  + (Working_Capital - Credit_Loss);       // Working Capital

 } catch (Exception &E) {
     throw;
 }
#ifdef _WRFLOW
     fstream outf;
     outf.open("NPV_flows.txt", ios::app | ios::out);  // Open for ouput and append

     outf << "Case SN: " << case_no.c_str()
          << "   Commission: " << Commission << "   Setup Cost: " <<  Setup_Cost
          << "   PB: " << fm_pb << "   NPV: " << total_npv << endl;
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
              << setw(15) << setprecision(8) << Late_Cost
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
     late_fee[i] = monthly_repayment[i-1] * open_attrition[i] * apr[i]
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
  return(ALLOGRAPH_FEE + LAND_ADM_FEE + duei_bao_rate[premium_col]);
}

//---------------------------------------------------------------------------
double Loan::set_account_management_cost()
{
  for (int i = 1; i <= periods; i++)
     account_management_cost[i] = ACCT_MGMT_COST / 12.0 * open_attrition[i-1]
         * (os_principal[i] >= 1.0? 1: 0);
  return (NetPresentValue(ROE / 12.0, account_management_cost + 1, periods, ptEndOfPeriod)
          + account_management_cost[0]);
}

//---------------------------------------------------------------------------
double Loan::set_late_cost(double pb)
{
  double late = pb / 12.0 * 2.0;

  for (int i = 3; i <= periods; i++)
     late_cost[i] = M2_3_EXPENSE * open_attrition[i] * late;

  return (NetPresentValue(ROE / 12.0, late_cost + 1, periods, ptEndOfPeriod)
          + late_fee[0]);
}

//---------------------------------------------------------------------------
// Collection Cost (legal)
double Loan::set_collection_cost()
{
 double monthly_pb = fm_pb / 12.0;
 double discount_rate = 1 / pow((1 + ROE / 12.0), TIME_TO_RECOVER);
 double recovery_amount = min(nav, principal * recovery_ratio[premium_col]);
 double wacc = 1.5 * (COF * LEVERAGE_RATIO + ROE * (1 - LEVERAGE_RATIO));

 for (int i=5; i <= periods; ++i)
    collection_cost[i] = monthly_pb *  open_attrition[i] * (os_principal[i] >= 1.0? 1: 0)
                  * (M4P_EXPENSE + (os_principal[i] * LEGAL_FEE_RATE) +
                    (wacc / 12.0 * discount_rate * recovery_amount));

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
    credit_loss [i] = max(0, (os_principal[i-1] - recovery_amount * discount_rate))
                      * open_attrition[i-1] * monthly_pd;
 }
  return (NetPresentValue(ROE / 12.0, credit_loss + 1, periods, ptEndOfPeriod)
          + credit_loss[0]);
}

//---------------------------------------------------------------------------
double Loan::find_lowest_rate (double offset, double delta_r)
{
 double npv, target_r, offset_r;

 target_r = offset + delta_r;

 npv = calculate_npv(target_r);

#ifdef _TRACE
     fstream outf;

     outf.open("NPV_trace.txt", ios::app | ios::out);  // Open for ouput and append
     outf << "Case SN: " << case_no.c_str() << " offset: " << offset
          << " delta: " << delta_r << " NPV: " << npv << endl;
#endif

 if ((delta_r < 0.000001 && delta_r > -0.000001) ||                 // abs(delta_r) < 0.000001
     (npv >= ApprovedNPV && npv <= (ApprovedNPV + Allowance))) {  // or  2000 =< npv <= 2010
    min_npv = npv;
    return (ceil(target_r * 10000) / 10000.0); // carry to 4th decimal
 }
 else if (npv > (ApprovedNPV + Allowance)) {
    if (delta_r > 0)
       offset_r = offset;
    else
       offset_r = offset + delta_r;
 }
 else if(npv < ApprovedNPV) {
    if (delta_r > 0)
       offset_r = offset + delta_r;
    else
       offset_r = offset;
 }
 return (find_lowest_rate(offset_r, delta_r / 2.0));
}
//---------------------------------------------------------------------------
int Loan::app_validate_test(char *case_no, TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);
 String sql_stmt;

 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 code = 0;
 Message = "";
 try {
    hostVars[0] = case_no;
    handler->ExecSQLQry(SQLCommands[Get_AppInfo_Test], hostVars, 0, ds);
    record_count = ds->RecordCount;

    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["Loan amount"].IsNull())
          principal = ds->FieldByName("Loan amount")->AsFloat;
       else
          principal = 0;

       if (! ds->FieldValues["Original APR_1"].IsNull())
          apr1 = ds->FieldValues["Original APR_1"];
       else
          apr1 = 0.0;

       if (! ds->FieldValues["Term_1"].IsNull())
          seg1 = ds->FieldValues["Term_1"];
       else
          seg1 = 0;

       if (! ds->FieldValues["Original APR_2"].IsNull())
          apr2 = ds->FieldValues["Original APR_2"];
       else
          apr2 = 0.0;

       if (! ds->FieldValues["Term_2"].IsNull())
          seg2 = ds->FieldValues["Term_2"];
       else
          seg2 = 0;

       if (! ds->FieldValues["Original APR_3"].IsNull())
          apr3 = ds->FieldValues["Original APR_3"];
       else
          apr3 = 0.0;

       if (! ds->FieldValues["Term_3"].IsNull())
          seg3 = ds->FieldValues["Term_3"];
       else
          seg3 = 0;
       periods = seg1 + seg2 + seg3;

       if (! ds->FieldValues["Grace_period"].IsNull())
          grace_period = ds->FieldValues["Grace_period"];
       else
          grace_period = 0;

       app_fee = 3.0;

       if (!ds->FieldValues["NAV"].IsNull()) {
          nav = ds->FieldByName("NAV")->AsFloat;
       }
       else
          nav = 0.0;

       if (!ds->FieldValues["Property_flag"].IsNull())
          premium_col = ds->FieldValues["Property_flag"];
       else
          premium_col = 0;

       if (!ds->FieldValues["PB"].IsNull())
          fm_pb = ds->FieldByName("PB")->AsFloat;
       else
          fm_pb = 0;

    }
 } catch (Exception &E) {
    ds->Close();
    delete ds;
    return -1;
 }
  ds->Close();  // close dataset before delete and drop an object outside the try block,
                // otherwise result in "too many consecutive exceptions"
  delete ds;
  return (0);
}
//---------------------------------------------------------------------------
int Loan::app_validate_bt(char *case_no, TADOHandler *handler)
{
 Variant hostVars[5];
 TADODataSet *ds = new TADODataSet(NULL);

 ds->EnableBCD = false;  // Decimal fields are mapped to float.
 code = 0;
 Message = "";
 try {
    hostVars[0] = case_no;
    handler->ExecSQLQry(SQLCommands[Get_AppInfo_BT], hostVars, 0, ds);
    record_count = ds->RecordCount;

    ds->First();
    if (!ds->Eof) {
       if (! ds->FieldValues["IDN"].IsNull())
          app_id = ds->FieldValues["IDN"];
       else
          app_id_ind = -1;

       if (! ds->FieldValues["LOAN_AMT"].IsNull())
          principal = ds->FieldByName("LOAN_AMT")->AsFloat;
       else
          principal = 0;

       if (!ds->FieldValues["NAV"].IsNull()) {
          nav = ds->FieldByName("NAV")->AsFloat;
       }
       else
          nav = 0.0;

       if (!ds->FieldValues["INQUIRY_DATE"].IsNull())
          inquiry_date = ds->FieldValues["INQUIRY_DATE"];
       else
          inquiry_date_ind = -1;
    }
 } catch (Exception &E) {
    ds->Close();
    delete ds;
    return -1;
 }
  ds->Close();  // close dataset before delete and drop an object outside the try block,
                // otherwise result in "too many consecutive exceptions"
  delete ds;
  return (0);
}
//---------------------------------------------------------------------------

