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
    message = "本DAC核准模組僅供京城商銀使用。";
  case 101:
    message = "模組建議婉拒，曾經強停 / 拒往 / 退票 / 授信異常。";
    break;
  case 102:
    message = "模組建議婉拒，任一信用卡連續三個月以上循環繳款不正常。";
    break;
  case 103:
    message = "模組建議婉拒，任一現金卡最近一個月繳款延遲。";
    break;
  case 104:
    message = "模組建議婉拒，任一現金卡動用比率 >= 95%。";
    break;
  case 105:
    message = "模組建議婉拒，任一貸款逾期。";
    break;
  case 106:
    message = "模組建議婉拒，貢獻度過低。";
    break;
  case 107:
    message = "模組建議婉拒，任一貸款連續繳款不正常。";
    break;
  case 108:
    message = "模組建議婉拒，信用卡與現金卡餘額過高。";
    break;
  case 109:
    message = "模組建議婉拒，JCIC資料不足且查詢次數過多。";
    break;
  case 200:
    message = "模組建議人工審核，JCIC資料不足。";
    break;
  case 201:
    message = "模組建議人工審核，JCIC資料不足。建議核准額度不宜超過新台幣兩萬元。";
    break;
  case 203:
    message = "模組建議人工審核，信用卡循環金額 > 額度的75%。";
    break;
  case 204:
    message = "模組建議人工審核，信用卡額度數 <= 2且半數以上信用卡曾經延遲繳款。";
    break;
  default:
    message = "Unrecognized error code";
  };
  return message;
}

