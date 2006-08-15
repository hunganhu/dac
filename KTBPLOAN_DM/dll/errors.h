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
	PSCODE_201,  // 19
	PSCODE_202,  // 20
	PSCODE_301,  // 21
	PSCODE_302,  // 22
	PSCODE_303,  // 23
	PSCODE_304,  // 24
	PSCODE_305,  // 25
	PSCODE_306,  // 26
	PSCODE_307,  // 27
	PSCODE_308,  // 28
	PSCODE_309,  // 29
	PSCODE_310,  // 30
	PSCODE_311,  // 31
	PSCODE_312,  // 32
	PSCODE_313,  // 33
	PSCODE_314,  // 34
	PSCODE_315   // 35
};
enum SegmentCodes {
        seg_N,     // 0 Null
	seg_S,     // 1 Success
	seg_F,     // 2 Fail
	seg_Ip,    // 3 Insufficient Pass
	seg_If     // 4 Insufficient Fail
};

extern Invalid_Msg PRESCREEN_Msg[];
extern Invalid_Msg PRESCREEN_BT_Msg[];

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
