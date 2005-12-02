/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
** SOURCE FILE NAME: error.h
**
**
****************************************************************************/
#ifndef Error_H
#define Error_H

#define DataErrorMsg(x)  tcbMsg[(x)].msg
#define DataErrorCode(x) tcbMsg[(x)].code

typedef struct _invalid_msg {
	int code;
	char msg[128];
} Invalid_Msg, *Invalid_Msg_Ptr;

enum ErrorCodes {
	DATA_ERR_1001,
	DATA_ERR_1002,
	DATA_ERR_1003,
	DATA_ERR_1004,
	DATA_ERR_1005,
	DATA_ERR_1006,
	DATA_ERR_1007,
	DATA_ERR_1008,
	DATA_ERR_1009,
	DATA_ERR_1010,
	DATA_ERR_1011,
	DATA_ERR_1012,
	DATA_ERR_1013,
	DATA_ERR_1014,
	DATA_ERR_1015,
	DATA_ERR_1016,
	DATA_ERR_1017,
	DATA_ERR_1018,
	DATA_ERR_1019,
	DATA_ERR_1020,
	DATA_ERR_1021,
	DATA_ERR_1022,
	DATA_ERR_1023,
	DATA_ERR_1024,
	DATA_ERR_1025,
	DATA_ERR_1026,
	DATA_ERR_1027,
	DATA_ERR_1028,
	EXIST_ERR_2001,
	EXIST_ERR_2002,
	EXIST_ERR_2003,
	EXIST_ERR_2004,
	EXIST_ERR_2005
};

#endif  //Error_H
