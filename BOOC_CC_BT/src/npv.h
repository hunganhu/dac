#ifndef NPVH
#define NPVH
#endif

#include <vector>
#include <math.h>
/* The following include files are for debugging only*/
#include <fstream>
#include <iostream>
#include <string.h>

#ifndef VCLH
#include <vcl>
#endif

using namespace std;

#define NPV_PERIODS 36

#define IN_TEASER_VOLUTARY_ATTRITION 0.0086
#define OUT_OF_TEASER_VOLUTARY_ATTRITION 0.0172 //Twice as much as IN_TEASER


//Financial assumptions
#define ROE 0.1
#define COF 0.013
#define LEVERAGE_RATIO 0.956
#define COMMISSION_RATE 0.035
#define MONTHLY_OPERATING_EXPENSE 35 //NT$35
#define SETUP_COST 50 //NT$50
#define DPD_6_90_COST 60 //NT$60
#define DPD_6_90_ROLL_BACK_RATE 2.25 //2.25 times of monthly pb
#define DPD_90_270_AVERAGE_MONTH 6 //Average recovery month
#define DPD_90_270_OUTSOURCING_RATE 0.8 //80% of DPD 90-270 accounts are out-sourced
#define DPD_90_270_RECOVERY_RATE_OUTSOURCING 0.19 //19% of DPD 90-270 balance can be recovered via out-sourcing
#define DPD_90_270_RECOVERY_DATE_INHOUSE 0.08
#define DPD_90_270_COST_OUTSOURCING 0.16 //of recovery amount
#define DPD_90_270_COST_INHOUSE 210 //NT$210
#define DPD_COURT_ORDER_COST 1000 //NT$1000
#define DPD_COURT_FORCED_EXEC_COST 3000 //NT$3000
#define DPD_COURT_FORCED_EXEC_RATE 0.05 //5% of accounts in DPD 270 plus have court forced execution
#define DPD_270P_AVERAGE_MONTH 10.5
#define DPD_270P_RECOVERY_RATE 0.0224
#define DPD_270P_COST 0.25 //of recovery amount
#define DPD_270P_ROLL_RATE 0.1 //10% of monthly pb will fall into DPD270 plus

class bt{
	private:
//attributes  
	  double _npv[NPV_PERIODS + 10][14];
	  int _bt_counts;
	  int _total_bt_amount;
	  double _monthly_fee;
	  int _monthly_fee_type;
	  int _months_charging_fee;
	  double _monthly_pb;
    int _bt_period;
    double _monthly_regular_apr;
//methods
    void remaining_balance_percentage();
    void probability_of_stay();
    void remaining_balance(int i);
    void interest_revenue(int i);
    void interest_cost(int i);
    void pre_collection_cost(int i);
    void collection_cost(int i);
    void final_collection_cost(int i);
    void credit_loss(int i);
    void delta_working_capital(int i);
    void mgmt_fee(int i);
    void operation_expense(int i);
    double discount();
    bool dump_series(int type, int period);

  public:
    bt(int bt_counts, int total_bt_amount, int bt_fee_type,
       double monthly_bt_fee, int months_charging_bt_fee, int bt_period,
       double pb, const vector<double> &interest_scheme, double regular_apr);
    double npv();
};