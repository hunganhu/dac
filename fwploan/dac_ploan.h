//---------------------------------------------------------------------------

#ifndef dac_ploanH
#define dac_ploanH

#include "AdoHandle.h"
//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)
 int dac_ploan_ev(char *case_sn, char *idn, int dac_sn, char *ole_db, char *error_msg);

extern "C" __declspec(dllexport)
 int dac_ploan_ev_conn(char *case_sn, char *idn, int dac_sn, char *ole_db,
                 char *return_msg, TADOHandler *dbhandle);

#endif
