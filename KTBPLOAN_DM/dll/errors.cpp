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
	{1,   "本DAC核准模組僅供京城銀行使用。"},
	{2,   "The approval module has expired."},
	{3,   "無此申請件資料。"},
	{101, "曾經強停 / 拒往 / 退票 / 授信異常。"},
	{102, "任一信用卡連續三個月以上循環繳款不正常。"},
	{103, "任一現金卡最近一個月繳款延遲。"},
	{104, "最近一個月現金卡使用率超過95%(含)"},
	{105, "任一貸款逾期。"},
	{106, "貢獻度過低。"},
	{107, "任一貸款連續三個月以上繳款不正常。"},
	{108, "最近一個月現金卡加信用卡循環金額大於五十萬(含)。"},
	{109, "壞帳機率過高。"},
	{110, "京城銀行黑名單。"},
	{111, "JCIC資料不足且查詢次數 > 5。"},
	{112, "JCIC資料不足且有信用卡逾繳紀錄。"},
	{113, "JCIC資料不足且有強制停卡紀錄。"},
	{114, "JCIC資料不足且查詢次數 > 3。"},
	{115, "JCIC資料不足且有強制停卡紀錄。"},
	{116, "最近一個月現金卡加信用卡循環金額加無擔保貸款餘額大於一百萬(含)。"},
	{117, "收入22倍限制可借貸金額小於15萬。"},
	{118, "經濟價值過低。"},
	{119, "最近一個月任一貸款繳款不正常。"},
	{120, "有強制停卡紀錄。"},
	{121, "最近三個月有信用卡逾繳紀錄。"},
	{122, "有催收或呆帳註記。"},
	{123, "有補充/註記資訊。"},
	{201, "JCIC資料不足。"},
	{301, "模組建議核准。"},
	{302, "請核實收入確認後，模組建議核准。"},
	{303, "模組建議核准。"},
	{304, "請核實收入確認後，模組建議核准。"}
};

cc_error::cc_error(int error_code, const AnsiString &idn,
             const AnsiString &app_date)
      : _idn(idn), _app_date(app_date)
{
  _error_code = error_code;
};


char * cc_error::ShowMessage()
{
  return PSMsg(_error_code);
}


