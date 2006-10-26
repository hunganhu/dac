#include "cc_err.h"

cc_error::cc_error(int error_code, const AnsiString &idn,
             const AnsiString &app_date)
      : _idn(idn), _app_date(app_date)
{
  _error_code = error_code;
};


AnsiString cc_error::ShowMessage()
{
  AnsiString message = "";
  switch (_error_code){
  case 0:
    message = "The approval module has expired.";
    break;
  case 1:
    message = "本DAC核准模組僅供台南企銀使用。";
  case 101:
    message = "模組建議婉拒，三年內曾經強停 / 拒往 / 退票 / 授信異常。";
    break;
  case 102:
    message = "模組建議婉拒，任一信用卡連續三個月以上循環繳款不正常。";
    break;
  case 103:
    message = "模組建議婉拒，任一現金卡最近一個月繳款延遲。";
    break;
  case 104:
    message = "模組建議婉拒，任一現金卡動用比率 >= 100%。";
    break;
  case 105:
    message = "模組建議婉拒，任一貸款逾期。";
    break;
  case 106:
    message = "模組建議婉拒，貢獻度過低。";
    break;
  case 107:
    message = "模組建議婉拒，任一貸款連續三個月以上繳款不正常。";
    break;
  case 108:
    message = "模組建議婉拒，信用卡與現金卡餘額過高。";
    break;
  case 109:
    message = "模組建議婉拒，壞帳機率過高。";
    break;
  case 110:
    message = "模組建議婉拒，本行信用卡信用不良。";
    break;
  case 201:
    message = "模組建議人工審核，JCIC資料不足。";
    break;
  case 202:
    message = "模組建議人工審核，申請金額超過限制。";
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
    message = "申貸期數(月)錯誤。";
    break;
  case 305:
    message = "代償優惠利率期數加總不等於代償優惠總期數。";
    break;
  case 306:
    message = "申貸手續費錯誤。";
    break;
  case 307:
    message = "無法辨識代償手續費收取方式。";
    break;
  case 308:
    message = "每月代償手續費百分比小於零。";
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

