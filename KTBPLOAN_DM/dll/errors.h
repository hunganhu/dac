/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: errors.cpp
**
** Description: prescreen code and segment code
**
****************************************************************************/
//---------------------------------------------------------------------------

#ifndef errorsH
#define errorsH
//---------------------------------------------------------------------------
#ifndef VCL_H
#include <vcl.h>
#endif

#define PSMsg(x)  PRESCREEN_Msg[(x)].msg
#define PSCode(x) PRESCREEN_Msg[(x)].code

typedef struct _invalid_msg {
	int code;
	char msg[128];
} Invalid_Msg, *Invalid_Msg_Ptr;

enum ErrorCodes {
	PSCODE_0,    // 0
	PSCODE_1,    // 1
	PSCODE_2,    // 2
	PSCODE_3,    // 3
	PSCODE_101,  // 4
	PSCODE_102,  // 5
	PSCODE_103,  // 6
	PSCODE_104,  // 7
	PSCODE_105,  // 8
	PSCODE_106,  // 9
	PSCODE_107,  // 10
	PSCODE_108,  // 11
	PSCODE_109,  // 12
	PSCODE_110,  // 13
	PSCODE_111,  // 14
	PSCODE_112,  // 15
	PSCODE_113,  // 16
	PSCODE_114,  // 17
	PSCODE_115,  // 18
	PSCODE_116,  // 19
	PSCODE_117,  // 20
	PSCODE_118,  // 21
	PSCODE_119,  // 22
	PSCODE_120,  // 23
	PSCODE_121,  // 24
	PSCODE_122,  // 25
	PSCODE_123,  // 26
	PSCODE_201,  // 27
	PSCODE_301,  // 28
	PSCODE_302,  // 29
	PSCODE_303,  // 30
	PSCODE_304   // 31
};

extern Invalid_Msg PRESCREEN_Msg[];

class cc_error {
  public:
    char* ShowMessage();
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

#endif
