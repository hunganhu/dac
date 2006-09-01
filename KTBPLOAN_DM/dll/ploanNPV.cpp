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

#include "ploanNPV.h"
// Constants used in the program.
//const char *expire_date = "21001231";
//---------------------------------------------------------------------------

#pragma package(smart_init)
//double recovery_ratio[2] = {0.7, 0.8};
//double nav_ratio[2] = {0.8, 0.9};
//double duei_bao_rate[2] = {0.0, 2.25};   // DUEI_BAO_FEE ($K)

Loan::Loan (char *msn, unsigned int loan_amount, double apr, unsigned int terms,
            unsigned int application_fee, double pd):
            msn(msn), principal(loan_amount),apr1(apr), seg1(terms), app_fee(application_fee),
            pd(pd), apr2(0.0), seg2(0), apr3(0.0), seg3(0), grace_period(0)
{
 max_apr = max(max(apr1,apr2),apr3);
 periods = seg1 + seg2 + seg3;
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
String Loan::Case_no ()
{
 return msn;
}
//---------------------------------------------------------------------------
int Loan::appIncome()
{
 return app_income;
}
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
int Loan::term1()
{
 return seg1;
}
//---------------------------------------------------------------------------
int Loan::term2()
{
 return seg2;
}
//---------------------------------------------------------------------------
int Loan::term3()
{
 return seg3;
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

// max_apr = get_max_apr();
 delta_r = -0.01;
 npv = calculate_npv(delta_r);
 // Calculate all NPV between apr1 - 1% and 10% steps 0.25%
 while ((delta_r + apr1) <= 0.1 && npv < 2.0) {
    delta_r = delta_r + 0.0025;
    npv = calculate_npv(delta_r);
 }
 min_npv = npv;   // set to object variable
 return (delta_r);
}
//---------------------------------------------------------------------------
double Loan::calculate_npv(double delta_apr)
{
 double Interest_Revenue, Setup_Revenue, Late_Fee;
 double Interest_Cost, Commission, Setup_Cost, Acct_Mgmt_Cost, Late_Cost, Collection_Cost;
 double PreCollection_Cost, Credit_Loss, Working_Capital, Other_Fee;

 try {
  npv_init();
  set_apr(delta_apr);
  set_attrition();
  set_annuity(principal);  // 本息法
  // Revenue
  Interest_Revenue = set_interest_revenue();
  Setup_Revenue = set_setup_fee();
  Late_Fee = set_late_fee();

  // Cost
  Interest_Cost = set_interest_cost();
//  Commission =  calculate_commission();
  Acct_Mgmt_Cost = set_account_management_cost();
  PreCollection_Cost = set_precollection_cost();
//  Setup_Cost = setup_cost();
//  Late_Cost = set_late_cost(pd);
  Collection_Cost = set_collection_cost();

  Working_Capital = set_working_capital();
  Credit_Loss = set_credit_loss();

  total_npv = (Interest_Revenue + Setup_Revenue + Late_Fee)  // Revenue
               - (Interest_Cost + Acct_Mgmt_Cost
                  + PreCollection_Cost + Collection_Cost )   // Cost
                  + (Working_Capital - Credit_Loss);         // Working Capital

 } catch (Exception &E) {
     throw;
 }
#ifdef _WRFLOW
     fstream outf;
     outf.open("NPV_flows.txt", ios::app | ios::out);  // Open for ouput and append

     outf << "Case SN: " << msn.c_str()
          << "   PB: " << pd << "   NPV: " << total_npv << endl;
     outf << "TERM       APR  Open_Atr   Vol_Atr Invol_Atr    M1_Atr  Base_Atr        "
          << "OsPrinL        P_Repay        I_Repay         IntRev          LateF         "
          << "EarlyF        IntCost       AcctCost         PreCol        Collect             "
          << "WC        CreLoss" << endl;
     outf << "========================================================================================================="
          << "======================================================================"
          << "======================================================================" << endl;
     outf <<  showpoint
              << setw(79) << setprecision(8) << app_fee
              << setw(15) << setprecision(8) << Other_Fee
              << setw(15) << setprecision(8) << Commission
              << setw(15) << setprecision(8) << Interest_Revenue
              << setw(15) << setprecision(8) << Late_Fee
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
              << setw(15) << setprecision(8) << interest_cost[i]
              << setw(15) << setprecision(8) << account_management_cost[i]
              << setw(15) << setprecision(8) << precollection_cost[i]
              << setw(15) << setprecision(8) << collection_cost[i]
              << setw(15) << setprecision(8) << working_capital[i]
              << setw(15) << setprecision(8) << credit_loss[i]
              << endl;
     }
#endif
  return (total_npv);
}
//---------------------------------------------------------------------------
double Loan::recal_npv(double delta_apr, double loan_amt)
{
 double Interest_Revenue, Setup_Revenue, Late_Fee;
 double Interest_Cost, Commission, Setup_Cost, Acct_Mgmt_Cost, Late_Cost, Collection_Cost;
 double PreCollection_Cost, Credit_Loss, Working_Capital, Other_Fee;

 try {
  principal = loan_amt;
  npv_init();
  set_apr(delta_apr);
  set_attrition();
  set_annuity(principal);  // 本息法
  // Revenue
  Interest_Revenue = set_interest_revenue();
  Setup_Revenue = set_setup_fee();
  Late_Fee = set_late_fee();

  // Cost
  Interest_Cost = set_interest_cost();
//  Commission =  calculate_commission();
  Acct_Mgmt_Cost = set_account_management_cost();
  PreCollection_Cost = set_precollection_cost();
//  Setup_Cost = setup_cost();
//  Late_Cost = set_late_cost(pd);
  Collection_Cost = set_collection_cost();

  Working_Capital = set_working_capital();
  Credit_Loss = set_credit_loss();

  total_npv = (Interest_Revenue + Setup_Revenue + Late_Fee)  // Revenue
               - (Interest_Cost + Acct_Mgmt_Cost + PreCollection_Cost + Collection_Cost) // Cost
               + (Working_Capital - Credit_Loss);       // Working Capital

 } catch (Exception &E) {
     throw;
 }
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
     PD_attrition[i] = 0.0;
     m1_attrition[i] = 0.0;
     vol_attrition_open[i] = 0.0;
//     open_credit_fee[i] = 0.0;
//     risk_mgmt_revenue[i] = 0.0;
     os_principal[i] = 0.0;
     principal_repayment[i] = 0.0;
     interest_repayment[i] = 0.0;
     monthly_repayment[i] = 0.0;
     interest_revenue[i] = 0.0;
     late_fee[i] = 0.0;
     early_closing_fee [i] = 0.0;
//     open_credit_fee[i] = 0.0;
//     risk_mgmt_revenue[i] = 0.0;
     interest_cost[i] = 0.0;
     account_management_cost[i] = 0.0;
     late_cost[i] = 0.0;
     precollection_cost[i] = 0.0;
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
void Loan::set_attrition()
{
//  double monthly_pd = pd / 12.0;
  int cat, term;
  int pd_term;
  double Yfit;
  
  if (periods < 48) { // 3 year (4 * 12 month)
     if (max_apr <= 0.05)
        cat = 0;
     else if (max_apr > 0.05 && max_apr <= 0.09)
        cat = 1;
     else
        cat = 2;
     term = 36; // month
     slope = 1.0/6.0; // slope ratio
  } else if (periods >= 48 && periods < 72 ) { // 5 year (4*12 - 6*12 month)
     if (max_apr <= 0.05)
        cat = 3;
     else if (max_apr > 0.05 && max_apr <= 0.09)
        cat = 4;
     else
        cat = 5;
     term = 60; // month
     slope = 1.0/4.0; // slope ratio
  } else { //  7 year
     if (max_apr <= 0.05)
        cat = 6;
     else if (max_apr > 0.05 && max_apr <= 0.09)
        cat = 7;
     else
        cat = 8;
     term = 120; // month
     slope = 1.0/3.0; // slope ratio
  }

  for (int i = 0; i < term; i++)
      base_attrition[i] = Attrition_Table[cat][i];

  Yfit =  pow(pd, 3)* Yfit_coeff[cat][0] +
          pow(pd, 2)* Yfit_coeff[cat][1] +
          pd        * Yfit_coeff[cat][2] +
          Yfit_coeff[cat][3];

  for (int i = 0; i < periods; i++) {
      if (i < slope_turn_point)     // term <= 9
          PD_attrition[i] = Yfit * (i + 1) / slope_turn_point;
      else if (i >= slope_turn_point && i < slope_end_point) // 9 < term <= 25
          PD_attrition[i] = PD_attrition[i-1] + PD_attrition[i-1] * slope / slope_turn_point;
      else    // term > 25
          PD_attrition[i] = PD_attrition[i-1];
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

//---------------------------------------------------------------------------
void Loan::set_amortize(double line)
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
void Loan::set_annuity(double line)
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
 return (app_fee);
}

//---------------------------------------------------------------------------
double Loan::set_late_fee()
{
//  double late = pd / 12.0 * 2.0;
  for (int i = 1; i <= periods; i++)
     late_fee[i] = os_principal[i-1] * apr[i] * YearMonths / YearDays *
                  (m1_attrition[i] * m1_recovery_ratio * m1_avg_late_days * m1_penalty_rate +
                   involuntary_attrition[i] * m6_recovery_ratio * m6_avg_late_days * m6_penalty_rate);

  return (NetPresentValue(ROE / 12.0, late_fee + 1, periods, ptEndOfPeriod)
          + late_fee[0]);
}
//---------------------------------------------------------------------------
double Loan::set_early_closing_fee()
{
  for (int i = 1; i <= periods; i++)
     if (i > early_closing_period)
        early_closing_fee [i] = 0.0;
     else
        early_closing_fee [i] = os_principal[i-1] * voluntary_attrition[i] * early_closing_fee_pct
        		        * early_closing_fee_collectable_ratio;

  return (NetPresentValue(ROE / 12.0, early_closing_fee + 1, periods, ptEndOfPeriod)
          + early_closing_fee[0]);
}

//---------------------------------------------------------------------------
// Cost
// Interest Cost:
double Loan::set_interest_cost()
{
  for (int i = 1; i <= periods; i++)
     interest_cost[i] = os_principal[i-1] * open_attrition[i-1] * leverage_ratio * COF / 12.0;
  return (NetPresentValue(ROE / 12.0, interest_cost + 1, periods, ptEndOfPeriod)
          + interest_cost[0]);
}
//---------------------------------------------------------------------------
// Account Management Cost
double Loan::set_account_management_cost()
{
  for (int i = 1; i <= periods; i++)
     account_management_cost[i] = acct_mgmt_cost * open_attrition[i-1];
  return (NetPresentValue(ROE / 12.0, account_management_cost + 1, periods, ptEndOfPeriod)
          + account_management_cost[0]);
}
//---------------------------------------------------------------------------
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
  return (NetPresentValue(ROE / 12.0, precollection_cost + 1, periods+1, ptEndOfPeriod)
          + precollection_cost[0]);
}
//---------------------------------------------------------------------------
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
  return (NetPresentValue(ROE / 12.0, collection_cost + 1, periods+3, ptEndOfPeriod)
          + collection_cost[0]);
}
//---------------------------------------------------------------------------
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
  return (NetPresentValue(ROE / 12.0, working_capital + 1, periods, ptEndOfPeriod)
          + working_capital[0]);
}

//---------------------------------------------------------------------------
// Credit loss:
double Loan::set_credit_loss()
{
 double discount_rate = 1 / pow((1 + ROE / YearMonths),legal_action_period);
 credit_loss [0] = 0.0;
 for (int i = 1; i <= periods; i++) {
    credit_loss [i] = -os_principal[i-1]* involuntary_attrition[i] *
                      (1 - recovery_ratio * discount_rate);
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
     outf << "Case SN: " << msn.c_str() << " offset: " << offset
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

