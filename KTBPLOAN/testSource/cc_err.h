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
    AnsiString idn(){return _idn;};
    AnsiString app_date(){return _app_date;};
    cc_error(int error_code, const AnsiString &idn,
             const AnsiString &app_date);
  private:
    int _error_code;
    AnsiString _idn;
    AnsiString _app_date;
};





