//---------------------------------------------------------------------------
#include "loanApp.h"
#include <math.h>
// Constants used in the program.
const char *expire_date = "21001231";
const double m1_to_m7_ratio = 5.0;
const double YearMonths = 12.0;
const double YearDays = 365.0;

//---------------------------------------------------------------------------


Loan::Loan (char * caseNo, char* pid, int seq, TADOHandler *handler):
    case_sn(caseNo), idn(pid), dac_sn(seq), application_date_ind(0),
    product_code_ind(0), project_source_ind(0), case_source_ind(0),
    recommender_ind(0), guarantor_ind(0), principal_ind(0),
    repayment_ind(0), periods_ind(0), grace_period_ind(0),
    num_int_period_ind(0), appropriation_ind(0), zip_ind(0),
    segment_ind(0), application_fee_ind(0), risk_mgmt_fee_ind(0),
    acct_mgmt_fee_ind(0), bt_fee_ind(0), inquiry_date_ind(0), early_closing_period_ind(0)
{

}


Loan::~Loan ()
{
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


void Loan::calculate_npv()
{
  double revenue, cost, wc;
  double Interest_Revenue, Late_Fee, Early_Closing_Fee, Application_Fee;
  double Other_Fee, Interest_Cost, Acct_Mgmt_Cost, PreCollection_Cost;
  double Collection_Cost, Commission, Working_Capital, Credit_Loss;

  npv_init();
  set_apr();
  set_attrition();
  if ((repayment == '1') || (repayment == '3'))    //(1:一般法;2:本息法;3:本金法)
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
//  double monthly_pd = pd / 12.0;
  int cat, term;
  int pd_term;

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

  if (periods < 7)
     pd_term = 0;
  else if (periods > 84)
     pd_term = 7;
  else
     pd_term = (periods + 6) / 12;     // rounding

  for (int i = 0; i < term; i++)
      base_attrition[i] = Attrition_Table[cat][i];

  for (int i = 0; i < periods; i++) {
      if (pd_term == 0)
          PD_attrition[i] = pd * (i + 1) / (periods + 1) / 6;
          /* (2*PD*periods)/(12*(periods+1)*(i+1)/periods */
      else
         if (i < PDterm_coeff[pd_term][0])
            PD_attrition[i] = pd * (i + 1) / PDterm_coeff[pd_term][0] / PDterm_coeff[pd_term][1];
         else
            PD_attrition[i] = pd * PD_FACTOR / PDterm_coeff[pd_term][1];
  }    

  open_attrition[0] = 1.0;
  voluntary_attrition[0] = involuntary_attrition[0] = m1_attrition[0] = 0.0;
  for (int i = 1; i <= periods; i++) {
      voluntary_attrition[i] = open_attrition[i-1] * base_attrition[i-1];
      involuntary_attrition[i] = open_attrition[i-1] * PD_attrition[i-1];
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

  return (NPV(roe / 12.0, interest_revenue + 1, periods, ptEndOfPeriod)
          + interest_revenue[0]);
}
double Loan::set_late_fee()
{
  for (int i = 1; i <= periods; i++)
     late_fee[i] = os_principal[i-1] * apr[i] * YearMonths / YearDays *
                  (m1_attrition[i] * m1_recovery_ratio * m1_avg_late_days * m1_penalty_rate +
                   involuntary_attrition[i] * m6_recovery_ratio * m6_avg_late_days * m6_penalty_rate);

  return (NPV(roe / 12.0, late_fee + 1, periods, ptEndOfPeriod)
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

  return (NPV(roe / 12.0, early_closing_fee + 1, periods, ptEndOfPeriod)
          + early_closing_fee[0]);
}
// Cost
// Interest Cost:
double Loan::set_interest_cost()
{
  for (int i = 1; i <= periods; i++)
     interest_cost[i] = os_principal[i-1] * open_attrition[i-1] * leverage_ratio * cof / YearMonths;
  return (NPV(roe / 12.0, interest_cost + 1, periods, ptEndOfPeriod)
          + interest_cost[0]);
}

// Commission:
// Account Management Cost
double Loan::set_account_management_cost()
{
  for (int i = 1; i <= periods; i++)
     account_management_cost[i] = acct_mgmt_cost * open_attrition[i-1];
  return (NPV(roe / 12.0, account_management_cost + 1, periods, ptEndOfPeriod)
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
  return (NPV(roe / 12.0, precollection_cost + 1, periods+1, ptEndOfPeriod)
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
  return (NPV(roe / 12.0, collection_cost + 1, periods+3, ptEndOfPeriod)
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
  return (NPV(roe / 12.0, working_capital + 1, periods, ptEndOfPeriod)
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
  return (NPV(roe / 12.0, credit_loss + 1, periods, ptEndOfPeriod)
          + credit_loss[0]);
}

