#ifndef DAC_PLOAN_H
#define DAC_PLOAN_H
//---------------------------------------------------------------------------
#include <ADODB.hpp>
#include "AdoHandle.h"


class Loan {  
    char case_sn[13];
    char idn[11];	
    int dac_sn;
    char application_date[13];
    char product_code[11];
    char project_source[11];
    char case_source[11];
    char recommender;
    char guarantor;
    double principal;
    int repayment;
    int periods;
    int grace_period;
    int num_int_period;
    int appropriation;
    char zip[4];
    char occupation[11];
    char channel[11];
    int application_fee_ind;
    double application_fee_pct;
    double application_fee;
    int risk_ind;
    double risk_mgmt_pct;
    double risk_mgmt_fee;
    int acct_mgmt_ind;
    double acct_mgmt_pct;
    double acct_mgmt_fee;
    int bt_ind;
    double bt_pct;
    double bt_fee;
  public:
    Loan (ADOHandler & handler, char * case_sn, char* idn, int dac_sn);
    ~loan ();
    int validate(); 
}

#endif