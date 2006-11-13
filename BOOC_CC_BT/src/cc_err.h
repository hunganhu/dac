#ifndef VCL_H
#include <vcl.h>
#endif

#ifndef CCERR_H
#define CCERR_H
#endif

class cc_error{
  public:
    AnsiString ShowMessage();
    int error_code(){return _error_code;};
    AnsiString case_sn(){return _case_sn;};
    AnsiString idn(){return _idn;};
    AnsiString app_date(){return _app_date;};
    int product_type(){return _product_type;};
    int line(){if (_product_type == 1) return 0; else return _amount;};
    cc_error(int error_code, const AnsiString &case_sn, const AnsiString &idn,
             const AnsiString &app_date, int product_type, int amount,
             double cash_card_util_cap, double credit_card_util_cap, int cc_balance_cap);
  private:
    int _error_code;
    AnsiString _case_sn;
    AnsiString _idn;
    AnsiString _app_date;
    int _product_type;
    int _amount;
    double _cash_card_util_cap;
    double _credit_card_util_cap;
    int _cc_balance_cap;
};





