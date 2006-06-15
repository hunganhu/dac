//---------------------------------------------------------------------------

#ifndef pdacoH
#define pdacoH
//---------------------------------------------------------------------------
#include "AdoHandle.h"
#include "pdacoSQL.h"

// classes and methods
class PDACO
{
  private:	// User declarations
    String       case_no;
    String       idn;
    int          now;
// prescreen variable
    int krm021_hit;
    int krm023_hit;
    int bam086_hit;
    int jas002_defect;
    int app_max_bucket;
    int fs044;
    int fs059_1k_12m;
    int delinquent_months;
    int card_force_stop;
    int cash_max_bucket;
    int cash_utilization;
    int ind001;
    float revolving_amt;
    int ps_code;
    int segment;
// pdaco score
    float ms101;
    int twentile;
    float score;
    float pb;

  public:
    PDACO (char *case_no, char *idn, int now);
    ~PDACO ();
    int CreateWorkingTables(TADOHandler *handler);
    int DropWorkingTables(TADOHandler *handler);
    int Prescreen_New(TADOHandler *handler);
    int Prescreen_Reload(TADOHandler *handler);
    int Prescreen_Transfer(TADOHandler *handler);
    int GenerateScreenVars(TADOHandler *handler);
    int GeneratePdacoScore(TADOHandler *handler);
    int Segment();
    int PS_code();
    double Pdaco_score();
    double Pdaco_pb();
    double monthly_debt();
};

#endif
