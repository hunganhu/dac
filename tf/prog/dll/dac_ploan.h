//---------------------------------------------------------------------------

#ifndef dac_ploanH
#define dac_ploanH

#include "AdoHandle.h"
//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)
 int prescreen(char *app_sn, char *ts_data_date, char *jcic_data_date,
               char *ole_db, char *error_msg);
extern "C" __declspec(dllexport)
 int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, char *ole_db, char *error_msg);

 extern "C" __declspec(dllexport)
 int optimal_cal_conn(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_msg,
                TADOHandler *dbhandle);

extern "C" __declspec(dllexport)
int designated_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_msg);

extern "C" __declspec(dllexport)
 int conversion_cal(char *app_sn, char *tsn, char *ole_db, char *error_msg);

extern "C" __declspec(dllexport)
 int prescreen_gx_conn(char *app_sn, char *ts_data_date, char *jcic_data_date,
                  char *ole_db, char *error_msg, TADOHandler *dbhandle);
/*
extern "C" __declspec(dllexport)
 int dac_ploan_ev(char *case_sn, char *idn, int dac_sn, char *ole_db, char *error_msg);

extern "C" __declspec(dllexport)
 int dac_ploan_ev_conn(char *case_sn, char *idn, int dac_sn, char *ole_db,
                 char *error_msg, TADOHandler *dbhandle);
*/
#endif
