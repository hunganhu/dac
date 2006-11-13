#include "cc_err.h"

cc_error::cc_error(int error_code, const AnsiString &case_sn, const AnsiString &idn,
                   const AnsiString &app_date, int product_type, int amount,
                   double cash_card_util_cap, double credit_card_util_cap, int cc_balance_cap)
      : _case_sn(case_sn), _idn(idn), _app_date(app_date)
{
  _amount = amount;
  _product_type = product_type;
  _error_code = error_code;
  _cash_card_util_cap = cash_card_util_cap;
  _credit_card_util_cap = credit_card_util_cap;
  _cc_balance_cap = cc_balance_cap;
};


AnsiString cc_error::ShowMessage()
{
  AnsiString message = "";
  switch (_error_code){
  case 0:
    message = "The approval module has expired.";
    break;
  case 1:
    message = "本DAC核准模組僅供華僑銀行使用。";
  case 101:
    message = "建議婉拒，重大信用瑕疵。";
    break;
  case 102:
    message = "建議婉拒，信用卡繳款記錄不良。";
    break;
  case 103:
    message = "建議婉拒，現金卡近期有逾期。";
    break;
  case 104:
    message = "建議婉拒，現金卡動支比例大於百分之" + static_cast<AnsiString>(_cash_card_util_cap * 100) + "。";
    break;
  case 105:
    message = "建議婉拒，貸款繳款記錄不良。";
    break;
  case 108:
    message = "建議婉拒，信用卡與現金卡餘額大於新台幣" + static_cast<AnsiString>(_cc_balance_cap) + "元。";
    break;
  case 106:
    message = "建議婉拒，對華僑銀行貢獻度過低。";
    break;
  case 201:
    message = "建議人工審核，JCIC資料不足。";
    break;
  case 202:
    message = "建議人工審核，餘額代償申請金額超過限制。";
    break;
  case 203:
    message = "建議人工審核，信用卡循環金額大於額度的百分之" + static_cast<AnsiString>(_credit_card_util_cap * 100) + "。";
    break;
  case 204:
    message = "建議人工審核，信用卡額度數 <= 2且半數以上信用卡曾經延遲繳款。";
    break;
  case 301:
    message = "無法辨識產品性質別。";
    break;
  case 302:
    message = "代償金額小於或等於零。";
    break;
  case 303:
    message = "代償筆數小於或等於零。";
    break;
  case 304:
    message = "代償優惠期數(月)小於零。";
    break;
  case 305:
    message = "代償優惠利率期數加總不等於代償優惠總期數。";
    break;
  case 306:
    message = "無法辨識代償手續費。";
    break;
  case 307:
    message = "無法辨識代償手續費收取方式。";
    break;
  case 308:
    message = "每月代償手續費錯誤。";
    break;
  case 309:
    message = "代償年利率小於零或大於信用卡利率。";
    break;
  case 310:
    message = "信用卡年利率錯誤。";
    break;
  case 311:
    message = "代償手續費收取期數(月)錯誤。";
    break;
  case 312:
    message = "無此申請件資料。";
    break;
  case 313:
    message = "申請件資料重複。";
    break;
  case 314:
    message = "無法辨識卡別。";
    break;
  default:
    message = "Unrecognized error code";
  };
  return message;
}



