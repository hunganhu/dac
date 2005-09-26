//---------------------------------------------------------------------------

#ifndef dac_ploanH
#define dac_ploanH

#include "AdoHandle.h"

const int EXPIRATION_DATE = 20101231; // expiration date of this module
const char *EXPIRATION_MSG = "使用期限已過";      // expiration message

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)
int prescreen(char *app_sn, char *jcic_data_date, char *app_data_time,
               char *ole_db, char *error_message, TADOHandler *dbhandle);

extern "C" __declspec(dllexport)
int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message,
                TADOHandler *dbhandle);

extern "C" __declspec(dllexport)
int specific_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message,
                TADOHandler *dbhandle);

extern "C" __declspec(dllexport)
int decision_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, char *ole_db,
                 char *audit_userno1, char *change_code, char *major_deviation,
                 char *minor_deviation, char *decline_code, char *manual_code,
                 char *error_message, TADOHandler *dbhandle);
/*
extern "C" __declspec(dllexport)
int prescreen(char *app_sn, char *jcic_data_date, char *app_data_time,
               char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
 int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
int specific_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
int decision_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, char *ole_db,
                 char *audit_userno1, char *change_code, char *major_deviation,
                 char *minor_deviation, char *decline_code, char *manual_code,
                 char *error_message);
*/
#endif
