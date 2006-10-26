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


//Financial assumptions
#define ROE 0.1
#define COF 0.01
#define LEVERAGE_RATIO 0.9

#define ATTRITION_ADJ 1
#define EAD 0.75
#define LEGAL_ACTION_PERIOD 12
#define RECOVERY_RATIO 0.02
#define M1_TO_M7_RATIO 5
#define PRE_COLLECTION_PHONE_COST 50
#define SMS_COST 1.36
#define EXECUTION_ORDER_COST 325
#define PROPERTY_INQUIRY_COST 535
#define AUDITION_COST 600
#define LEGAL_PERSONNEL 1500
#define EXECUTION_COST_RATE 0.0032
#define RECOVERY_RATIO_4D 0.95
#define DAYS_LATE 6
#define LATE_FEE_RATE 0.1
#define DAYS_LATE_M7 180
#define LATE_FEE_RATE_M7 0.2
#define ACCT_MGMT_FEE 3
#define BLT_RECOVERY 0.7



class ploan{
	private:
//attributes
	  double (*_npv)[14];
    bool _located;
	  unsigned int _loan_amount;
    double _monthly_rate;
    double _apr;
    unsigned int _terms;
    unsigned int _application_fee;
    double _pdaco_score;
    double _monthly_pb;
    int _year_for_pd;
    unsigned int _product_type;

//methods
    void probability_of_stay();
    void interest_cost(unsigned int i);
    void pre_collection_cost(unsigned int i);
    void collection_cost(unsigned int i);
    void late_fee(unsigned int i);
    void credit_loss(unsigned int i);
    void delta_working_capital(unsigned int i);
    void mgmt_fee(unsigned int i);
    void operation_expense(unsigned int i);
    double discount();
    bool dump_series(int type, int period);
    double calculate_pb();
    double assign_pb();
    void annunity();
    double assign_pd(unsigned int);

  public:
    ploan(unsigned int loan_amount, double apr, unsigned int terms,
          unsigned int application_fee, double pdaco_score,
          unsigned int product_type);
    ~ploan();         
    double npv(double &pb, double &min_rate);
};