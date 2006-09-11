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
	PSCODE_101,  // 3
	PSCODE_102,  // 4
	PSCODE_103,  // 5
	PSCODE_104,  // 6
	PSCODE_105,  // 7
	PSCODE_106,  // 8
	PSCODE_107,  // 9
	PSCODE_108,  // 10
	PSCODE_109,  // 11
	PSCODE_110,  // 12
	PSCODE_111,  // 13
	PSCODE_112,  // 14
	PSCODE_113,  // 15
	PSCODE_114,  // 16
	PSCODE_115,  // 17
	PSCODE_116,  // 18
	PSCODE_117,  // 19
	PSCODE_118,  // 20
	PSCODE_119,  // 21
	PSCODE_120,  // 22
	PSCODE_121,  // 23
	PSCODE_122,  // 24
	PSCODE_123,  // 25
	PSCODE_201,  // 26
	PSCODE_301,  // 27
	PSCODE_302,  // 28
	PSCODE_303,  // 29
	PSCODE_304   // 30
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
