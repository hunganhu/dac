//---------------------------------------------------------------------------

#ifndef dac_fmH
#define dac_fmH

#include "AdoHandle.h"

const int EXPIRATION_DATE = 20251231; // expiration date of this module
const char *EXPIRATION_MSG = "�֭�Ҳըϥδ����w�L�A�Ь�DAC�ѨM�ϥ��v�����D�C"; // expiration message

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)
 int FM_New(char *case_no, char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
 int FM_Reload(char *case_no, char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
 int FM_Transfer(char *case_no, char *ole_db, char *error_message);
#endif
