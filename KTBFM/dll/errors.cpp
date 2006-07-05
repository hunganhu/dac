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
** Description: Prescreen message for first mortgage
**
****************************************************************************/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "errors.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
Invalid_Msg PRESCREEN_Msg[] = {
	{0,   " "},
	{101, "三年內有重大信用瑕疵"},
	{102, "信用卡最近一年內曾逾期超過90天"},
	{103, "貸款最近一年內曾逾期超過90天"},
	{104, "現金卡最近一個月遲繳"},
	{105, "現金卡動支比例超過95%"},
	{106, "信用卡及現金卡循環餘額超過50萬"},
	{107, "個人風險過高"},
	{108, "行內黑名單"},
	{109, "信用卡有逾期紀錄"},
	{110, "有強制停卡紀錄"},
	{111, "貸款曾經延遲繳款"}
};

Invalid_Msg PRESCREEN_BT_Msg[] = {
	{0,   "無特殊個人風險提示"},
	{101, "三年內有重大信用瑕疵"},
	{102, "信用卡最近一年內曾逾期超過90天"},
	{103, "貸款最近一年內曾逾期超過90天"},
	{104, "現金卡最近一個月遲繳"},
	{105, "現金卡動支比例超過95%"},
	{106, "信用卡及現金卡循環餘額超過50萬"},
	{107, "個人風險過高"},
	{108, "行內黑名單"},
	{109, "信用卡有逾期紀錄"},
	{110, "有強制停卡紀錄"},
	{111, "貸款曾經延遲繳款"}
};

