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

#define PSMsg(x)  PRESCREEN_Msg[(x)].msg
#define PSCode(x) PRESCREEN_Msg[(x)].code
#define PSMsg_BT(x)  PRESCREEN_BT_Msg[(x)].msg
#define PSCode_BT(x) PRESCREEN_BT_Msg[(x)].code

#define SYSTEM_ERROR 1999

typedef struct _invalid_msg {
	int code;
	char msg[128];
} Invalid_Msg, *Invalid_Msg_Ptr;

enum ErrorCodes {
	PSCODE_0,    // 0
	PSCODE_101,  // 1
	PSCODE_102,  // 2
	PSCODE_103,  // 3
	PSCODE_104,  // 4
	PSCODE_105,  // 5
	PSCODE_106,  // 6
	PSCODE_107,  // 7
	PSCODE_108,  // 8
	PSCODE_109,  // 9
	PSCODE_110,  // 10
	PSCODE_111   // 11
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

#endif
