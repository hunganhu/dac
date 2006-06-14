//---------------------------------------------------------------------------

#ifndef dac_fmH
#define dac_fmH

#include "AdoHandle.h"

const int EXPIRATION_DATE = 20251231; // expiration date of this module
const char *EXPIRATION_MSG = "核准模組使用期限已過，請洽DAC解決使用權限問題。"; // expiration message

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)
 int FM_New(char *case_no, char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
 int FM_Reload(char *case_no, char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
 int FM_Transfer(char *case_no, char *ole_db, char *error_message);

int overall_lookup(int appStatus, int cosStatus, int guaStatus,
                 int appPSCode, int cosPSCode, int guaPSCode,
                 String appMsg, String cosMsg, String guaMsg,
                 int *pdacoPath, int *incomePath, int *ms101Path,
                 String dispositionMsg, String finalMsg, TADOHandler *handler);
double Pb_adjustment(double score);
int final_lookup(int appStatus, int cosStatus, int guaStatus,
                 int appPSCode, int cosPSCode, int guaPSCode,
                 String appMsg, String cosMsg, String guaMsg,
                 int disp_code, String dispositionMsg, String finalMsg, TADOHandler *handler);

#endif
