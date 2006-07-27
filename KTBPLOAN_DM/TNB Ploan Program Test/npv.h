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
#define RECOVERY_RATIO 0.02
#define DAYS_LATE_M7 180
#define LATE_FEE_RATE_M7 0.2
#define ACCT_MGMT_FEE 3



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
    double _ms093;
    double _ms094;
    double _ms105;
    double _ms047;
    double _ms080;
    double _wi006_9m;
    double _wi008_3m;
    double _wi008_9m;
    double _wi002_12m;
    double _wi004_12m;
    double _cpdaco_score;
    bool _ms093_isnull;
    bool _ms094_isnull;
    bool _ms105_isnull;
    bool _ms047_isnull;
    bool _ms080_isnull;
    bool _wi006_9m_isnull;
    bool _wi008_3m_isnull;
    bool _wi008_9m_isnull;
    bool _wi002_12m_isnull;
    bool _wi004_12m_isnull;
    double _pdaco_score;
    double _monthly_pb;
    double _ci_score;
    int _year_for_pd;

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
             unsigned int application_fee,
             double ms093, double ms094, double ms105, double ms047,
             double ms080, double wi006_9m, double wi008_3m, double wi008_9m,
             double wi002_12m, double wi004_12m, double pdaco_score,
             bool ms093_isnull, bool ms094_isnull, bool ms105_isnull, bool ms047_isnull,
             bool ms080_isnull, bool wi006_9m_isnull, bool wi008_3m_isnull,
             bool wi008_9m_isnull, bool wi002_12m_isnull, bool wi004_12m_isnull);
    ~ploan();         
    double npv(double &pb);
    double get_ci_score(){return _ci_score;};    
};