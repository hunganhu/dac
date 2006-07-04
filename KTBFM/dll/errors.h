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
	PSCODE_0,
	PSCODE_101,
	PSCODE_102,
	PSCODE_103,
	PSCODE_104,
	PSCODE_105,
	PSCODE_106,
	PSCODE_107,
	PSCODE_108,
	PSCODE_109,
	PSCODE_110
};

enum SegmentCodes {
        seg_N,     // Null
	seg_S,     // Success
	seg_F,     // Fail
	seg_Ip,    // Insufficient Pass
	seg_If     // Insufficient Fail
};

extern Invalid_Msg PRESCREEN_Msg[];
extern Invalid_Msg PRESCREEN_BT_Msg[];

#endif
