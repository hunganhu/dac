#ifndef NPVH
#define NPVH
#endif
#include <new>
#include <vector>
#include <math.h>
#include <Math>
#include <fstream>
#include <iostream>
#include <string.h>
#include <system.hpp>
#ifndef ADODBHPP
#include <ADODB.hpp>
#endif
#ifndef VCLH
#include <vcl>
#endif
#include <new>



#define TEST 10
#define PRODUCTION 7
//Assumptions for BSP SML NPV
#define LATE_FEE 1.2 //mutiple of interest rate applied
#define RECO_OTHERS 0.75   //recovery ratio for non-taipei area
#define RECO_TPE 0.8       //recovery ratio for taipei area
#define NPV_PERIODS 60     //5 years
#define LOC_TEASER1 0.0299
#define LOC_TEASER2 0.0599
#define LOC_GOTO 0.0959 //Change from 0.095 on Jan 1 2005
#define IL_APR1 0.0649
#define IL_APR2 0.0949
#define MGMT_FEE1 0.2
#define MGMT_FEE2 0.15
#define LEVERAGE 0.9406 //Change from 0.937 on Jan 1 2005
#define COF	0.0125 //Change from 0.01175 on Jan 1 2005
#define COE	0.1
#define ACQ_COST 1.176 //one time
#define COMMISSION 0.0003 //one time, of loan amount or line 0.03%
#define M2_3_EXPENSE 5.25 //NT$5.25K
#define M4P_EXPENSE 22.5  //NT$22500 per account per month
#define MAX_LENDING_SECURED 5000 //NT$500MM
#define MAX_LENDING_UNSECURED 1000 //NT$100MM
#define MIN_LENDING_AMOUNT 100 //NT$10K

#define Usage_peak 6 //6 months, for loc only
#define Extension_expense 0.2 // extension cost, once every year for loc
#define Time_to_recover 12 //months
#define Legal_fee_rate 0.008 //percentage of balance, taken from Teresa's model, for collection
#define Maintenance_cost 0.08 //NT$80 per account per year
#define Set_lien_fee 0
//#define Set_lien_fee 0.0012 percentage of line, governemnt fee
#define Default_recovery 0.15 //recovery rate for credit card, used for unsecured recovery

#define Loan_Base 500.0 //NT$500K, for unsecured IL risk
#define Max_Granting_Ratio 1.2 //120% of nav
#define Max_Granting_Ratio_CASHCARD 0.6 // 60% of nav
#define Max_Granting_Ratio_DATA 0.8 // 80%

#define MAX_CELL_NO 40

using namespace std;

// 0 for intro rate, 1 for go-to rate, 2 for intro period, 3 for loan period
// 4 for account mgmt fee, 5 for monthly account mgmt fee period
// 6 for loan type - 0 for LOC, 1 for IL
const double product[MAX_CELL_NO][7] = {
0.0299, 0.0959, 6, 0, 0, 0, 0,
0.0299, 0.0959, 6, 0, 0, 0, 0,
0.0299, 0.0959, 6, 0, 0.2, 24, 0,
0.0299, 0.0959, 6, 0, 0.2, 24, 0,
0.0299, 0.0959, 12, 0, 0, 0, 0,
0.0299, 0.0959, 12, 0, 0, 0, 0,
0.0299, 0.0959, 12, 0, 0.2, 24, 0,
0.0299, 0.0959, 12, 0, 0.2, 24, 0,
0.0599, 0.0959, 6, 0, 0, 0, 0,
0.0599, 0.0959, 6, 0, 0, 0, 0,
0.0599, 0.0959, 6, 0, 0.2, 24, 0,
0.0599, 0.0959, 6, 0, 0.2, 24, 0,
0.0599, 0.0959, 12, 0, 0, 0, 0,
0.0599, 0.0959, 12, 0, 0, 0, 0,
0.0599, 0.0959, 12, 0, 0.2, 24, 0,
0.0599, 0.0959, 12, 0, 0.2, 24, 0,
0, 0.0649, 0, 36, 0, 0, 1,
0, 0.0649, 0, 36, 0, 0, 1,
0, 0.0649, 0, 36, 0.15, 36, 1,
0, 0.0649, 0, 36, 0.15, 36, 1,
0, 0.0649, 0, 84, 0, 0, 1,
0, 0.0649, 0, 84, 0, 0, 1,
0, 0.0649, 0, 84, 0.15, 36, 1,
0, 0.0649, 0, 84, 0.15, 36, 1,
0, 0.0949, 0, 36, 0, 0, 1,
0, 0.0949, 0, 36, 0, 0, 1,
0, 0.0949, 0, 36, 0.15, 36, 1,
0, 0.0949, 0, 36, 0.15, 36, 1,
0, 0.0949, 0, 84, 0, 0, 1,
0, 0.0949, 0, 84, 0, 0, 1,
0, 0.0949, 0, 84, 0.15, 36, 1,
0, 0.0949, 0, 84, 0.15, 36, 1,
0.0399, 0.095, 12, 0, 0.2, 24, 0,
0.0399, 0.095,  6, 0, 0.2, 24, 0,
0.0499, 0.095, 12, 0, 0.2, 24, 0,
0.0499, 0.095,  6, 0, 0.2, 24, 0,
0, 0.0749, 0, 84, 0.15, 36, 1,
0, 0.0749, 0, 84, 0, 0, 1,
0, 0.0849, 0, 84, 0.15, 36, 1,
0, 0.0849, 0, 84, 0, 0, 1
};


class loc{
  private:
    double (*_series)[TEST+1];
//0 for interest revenue, 1 for late payment processing expense,
//2 for balance, 3 for interest rate applied
//4 for probability of stay, 5 for late fee,
//6 for credit losses, 7 for net income,
//8 for interest expense, 9 for collection cost

    AnsiString _msn;
    AnsiString _idn;
    AnsiString _time_stamp;
    int _test_cell;
    int _propensity_decile;
    double _gav;
    double _nav;
    double _existing_mortgage;
    double _leverage;
    double _coe;
    double _cof;
    double _commission;
    double _maintenance_cost;
    double _personal_risk;
    double _home_equity;
    int _periods;
    int _mgmt_fee_periods;
    double _mgmt_fee;
    double _principal;
    unsigned char _status; //0 not created
                           //1 for created
    double _recovery_ratio;
    double _outstanding_unsecured_bal;

    void get_recovery_ratio(AnsiString &);
    void get_product_feature();
    void attrition(double , bool =true);
    double secured_pb(double, unsigned int);
    double unsecured_pb(double);
    void propensity(double, int);
    void interest_revenue();
    void late_fee(double);
    void interest_cost();
    void late_cost(double);
    void collection_cost(double, double, bool);
    void credit_loss(double, double, bool);
    double discount(int, double);
    double max_npv_line(int, double &, double &, double [][3], unsigned int);


    bool dump(double, double, bool);
    bool dump_npv(int ,double [][3]);
    bool dump_series(int, int);

  public:
    loc(int, double, double, double, AnsiString, double, double, int, double,
        AnsiString, AnsiString, AnsiString, AnsiString *);
    loc();
    ~loc();
    unsigned char status();
    double npv(bool, double &, double &, unsigned int, double);
};

inline unsigned char loc::status()
{
  return _status;
};

inline void loc::get_recovery_ratio(AnsiString &Zip)
{
  int zip = Zip.ToInt();
  if(zip>253)
    _recovery_ratio = RECO_OTHERS;
  else if((zip>=200) && (zip<=206))
    _recovery_ratio = RECO_OTHERS;
  else
    _recovery_ratio = RECO_TPE;
};
//-----------------------------------------------
class il{
  private:
    double (*_series)[TEST+1];
//0 for interest repayment, 1 for principal repayment,
//2 for remaining principal, 3 for interest rate applied
//4 for probability of stay, 5 for late fee,
//6 for credit losses, 7 for net income,
//8 for interest cost, 9 for collection cost
//10 for late payment processing cost
//11 for change in working capital
//11 for credit loss calculation payment schedule ->No need

    unsigned char _status;
    AnsiString _msn;
    AnsiString _idn;
    AnsiString _time_stamp;
    int _test_cell;
    double _gav;
    double _existing_mortgage;
    double _nav;
    double _cof;
    double _coe;
    double _leverage;
    double _commission;
    double _maintenance_cost;
    double _personal_risk;
    double _home_equity;
    int _periods;
    int _mgmt_fee_periods;
    double _mgmt_fee;
    double _recovery_ratio;
    double _outstanding_unsecured_bal;
    double _principal;
//    double _zero_taken_multiple;

    void get_recovery_ratio(AnsiString &);
    void get_product_feature();
    void get_periods();
//    void get_propensity_parameters(int);
//    double effective_amount(double);
    double secured_pb(double, unsigned int);
    double unsecured_pb(double);
    void annunity(double, int);
    void attrition(double , bool =true);
    void late_fee(double, double);
    void interest_cost();
    void late_cost(double);
    void collection_cost(double, bool);
    void credit_loss(double, bool);
    double discount(int, double);

    double max_npv_amount(int, double &, double &, double [][3], unsigned int);

    void record_npv(TADOCommand *, double [5][4]);
    bool dump(double, double, bool);
    bool dump_series(int , int );
    bool dump_npv(int,double [][3]);
    bool dump_pb(int,double *);

  public:
    il(int, double, double, double, AnsiString, double, double, double , AnsiString,
       AnsiString, AnsiString, AnsiString*);
    il();
    ~il();
    unsigned char status();
    double npv(bool, double &, double &, unsigned int, double);
};

inline void il::get_recovery_ratio(AnsiString &Zip)
{
  int zip = Zip.ToInt();
  if(zip>253)
    _recovery_ratio = RECO_OTHERS;
  else if((zip>=200) && (zip<=206))
    _recovery_ratio = RECO_OTHERS;
  else
    _recovery_ratio = RECO_TPE;
};

inline unsigned char il::status()
{
  return _status;
};
