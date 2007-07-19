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

#define SYSTEM_ERROR 1999

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
	DATA_ERR_1029,
	DATA_ERR_1030,
	DATA_ERR_1031,
	DATA_ERR_1032,
	DATA_ERR_1033,
	DATA_ERR_1034,
	DATA_ERR_1035,
	DATA_ERR_1036,
	DATA_ERR_1037,
	DATA_ERR_1038,
	EXIST_ERR_1101,
	EXIST_ERR_1102,
	EXIST_ERR_1103,
	EXIST_ERR_1104,
	EXIST_ERR_1105
};

#endif  //Error_H
