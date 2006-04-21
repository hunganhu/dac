//---------------------------------------------------------------------------

#ifndef NPVH
#define NPVH
//---------------------------------------------------------------------------
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

//#ifndef VCLH
//#include <vcl>
//#endif
#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"


#define TEST 10
#define PRODUCTION 7
//Assumptions for BSP SML NPV
#define LATE_FEE 1.2 //mutiple of interest rate applied
#define RECO_OTHERS 0.75   //recovery ratio for non-taipei area
#define RECO_TPE 0.8       //recovery ratio for taipei area
#define NPV_PERIODS 60     //5 years
#define LEVERAGE 0.9491 //0.937(Dec 31, 2004), 0.9406(Oct. 14, 2005), 0.9455(Jan 31, 2006)
#define COF	0.0156 //0.011759(Dec 31, 2004), 0.0125(Oct. 14, 2005),  0.0149(Jan 31, 2006)
#define COE	0.07 //0.1 (Oct. 14, 2005)
#define ACQ_COST 1.176 //one time
#define COMMISSION 0.0004 //one time, of loan amount or line 0.03% (Jan 31, 2006)
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
#define Max_Granting_Ratio 1.3 //120% of nav(Oct. 14, 2005)
#define Max_Granting_Ratio_CASHCARD 0.6 // 60% of nav
#define Max_Granting_Ratio_DATA 0.8 // 80%
#define min(a,b)  ((a) > (b)) ? (b): (a)
#define max(a,b)  ((a) > (b)) ? (a): (b)

#define MAX_CELL 52

using namespace std;

// 0 for intro rate, 1 for go-to rate, 2 for intro period, 3 for loan period
// 4 for account mgmt fee, 5 for monthly account mgmt fee period
// 6 for loan type - 0 for LOC, 1 for IL, 7 for test cell
const double product[MAX_CELL][8] = {
0.0299, 0.0959, 6, 0, 0, 0, 0, 1,
0.0299, 0.0959, 6, 0, 0, 0, 0, 2,
0.0299, 0.0959, 6, 0, 0.2, 24, 0, 3,
0.0299, 0.0959, 6, 0, 0.2, 24, 0, 4,
0.0299, 0.0959, 12, 0, 0, 0, 0, 5,
0.0299, 0.0959, 12, 0, 0, 0, 0, 6,
0.0299, 0.0959, 12, 0, 0.2, 24, 0, 7,
0.0299, 0.0959, 12, 0, 0.2, 24, 0, 8,
0.0599, 0.0959, 6, 0, 0, 0, 0, 9,
0.0599, 0.0959, 6, 0, 0, 0, 0, 10,
0.0599, 0.0959, 6, 0, 0.2, 24, 0, 11,
0.0599, 0.0959, 6, 0, 0.2, 24, 0, 12,
0.0599, 0.0959, 12, 0, 0, 0, 0, 13,
0.0599, 0.0959, 12, 0, 0, 0, 0, 14,
0.0599, 0.0959, 12, 0, 0.2, 24, 0, 15,
0.0599, 0.0959, 12, 0, 0.2, 24, 0, 16,
0, 0.0649, 0, 36, 0, 0, 1, 17,
0, 0.0649, 0, 36, 0, 0, 1, 18,
0, 0.0649, 0, 36, 0.15, 36, 1, 19,
0, 0.0649, 0, 36, 0.15, 36, 1, 20,
0, 0.0649, 0, 84, 0, 0, 1, 21,
0, 0.0649, 0, 84, 0, 0, 1, 22,
0, 0.0649, 0, 84, 0.15, 36, 1, 23,
0, 0.0649, 0, 84, 0.15, 36, 1, 24,
0, 0.0949, 0, 36, 0, 0, 1, 25,
0, 0.0949, 0, 36, 0, 0, 1, 26,
0, 0.0949, 0, 36, 0.15, 36, 1, 27,
0, 0.0949, 0, 36, 0.15, 36, 1, 28,
0, 0.0949, 0, 84, 0, 0, 1, 29,
0, 0.0949, 0, 84, 0, 0, 1, 30,
0, 0.0949, 0, 84, 0.15, 36, 1, 31,
0, 0.0949, 0, 84, 0.15, 36, 1, 32,
0.0399, 0.095, 12, 0, 0.2, 24, 0, 33,
0.0399, 0.095,  6, 0, 0.2, 24, 0, 34,
0.0499, 0.095, 12, 0, 0.2, 24, 0, 35,
0.0499, 0.095,  6, 0, 0.2, 24, 0, 36,
0, 0.0749, 0, 84, 0.15, 36, 1, 37,
0, 0.0749, 0, 84, 0, 0, 1, 38,
0, 0.0849, 0, 84, 0.15, 36, 1, 39,
0, 0.0849, 0, 84, 0, 0, 1, 40,
0, 0.068, 0, 84, 0, 0, 1, 41,
0, 0.0549, 0, 84, 0, 0, 1, 42,
0, 0.0599, 0, 84, 0.2, 24, 1, 43,
0, 0.0599, 0, 84, 0.2, 24, 1, 44,
0, 0.0499, 0, 84, 0.2, 24, 1, 45,
0, 0.0499, 0, 84, 0, 0, 1, 46,
0.078, 0.095, 12, 0, 0, 0, 0, 71,
0.0649, 0.095, 12, 0, 0, 0, 0, 72,
0.0699, 0.095, 12, 0, 0.2, 24, 0, 73,
0.0699, 0.095, 12, 0, 0.2, 24, 0, 74,
0.0599, 0.095, 12, 0, 0.2, 24, 0, 75,
0.0599, 0.095, 12, 0, 0, 0, 0, 76
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
    int _teaser_periods;
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
    double _app_fee;
//    double _zero_taken_multiple;

    void get_recovery_ratio(AnsiString &);
    void get_product_feature(int);
    int get_periods();
    void set_apr(double apr);
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
    il(double apr, int period, double gav, double nav, double existing_mortgage,
       AnsiString zip, double principal, double app_fee, double balance,
       double risk, AnsiString msn, AnsiString idn, AnsiString time_stamp,
       AnsiString *error_message);
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
