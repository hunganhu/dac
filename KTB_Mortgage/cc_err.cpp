#include "cc_err.h"

keyin_error::keyin_error(int error_code/*, const AnsiString &idn,
             const AnsiString &app_date*/)
//      : _idn(idn), _app_date(app_date)
{
  _error_code = error_code;
};


AnsiString keyin_error::ShowMessage()
{
  AnsiString message = "";
  switch (_error_code){
  case 1:
    message = "申請人姓名尚未填寫。";
    break;
  case 2:
    message = "申請人身分證號碼錯誤。";
    break;
  case 3:
    message = "申請人出生年月日錯誤。";
    break;
  case 4:
    message = "申請人婚姻狀態尚未填寫。";
    break;
  case 5:
    message = "申請人學歷尚未填寫。";
    break;
  case 6:
    message = "申請人年收入尚未填寫。";
    break;
  case 7:
    message = "未確定申請人基本資料是否符合承做條件。";
    break;
  case 8:
    message = "共同借款人姓名尚未填寫。";
    break;
  case 9:
    message = "共同借款人身分證號碼錯誤。";
    break;
  case 10:
    message = "共同借款人出生年月日錯誤。";
    break;
  case 11:
    message = "共同借款人婚姻狀態尚未填寫。";
    break;
  case 12:
    message = "共同借款人學歷尚未填寫。";
    break;
  case 13:
    message = "共同借款人年收入尚未填寫。";
    break;
  case 14:
    message = "未確定共同借款人基本資料是否符合承做條件。";
    break;
  case 15:
    message = "保證人姓名尚未填寫。";
    break;
  case 16:
    message = "保證人身分證號碼錯誤。";
    break;
  case 17:
    message = "保證人出生年月日錯誤。";
    break;
  case 18:
    message = "保證人婚姻狀態尚未填寫。";
    break;
  case 19:
    message = "保證人學歷尚未填寫。";
    break;
  case 20:
    message = "保證人年收入尚未填寫。";
    break;
  case 21:
    message = "未確認保證人基本資料是否符合承做條件。";
    break;
  case 22:
    message = "申請金額錯誤。";
    break;
  case 23:
    message = "借款期間錯誤，正確值為12至240。";
    break;
  case 24:
    message = "寬限期錯誤。";
    break;
  case 25:
    message = "開辦費尚未填寫。";
    break;
  case 26:
    message = "利率分段尚未填寫。";
    break;
  case 27:
    message = "第一段利率錯誤，正確值為0至10。";
    break;
  case 28:
    message = "第二段利率錯誤，正確值為0至10。";
    break;
  case 29:
    message = "第三段利率錯誤，正確值為0至10。";
    break;
  case 30:
    message = "各段利率期間總和不等於借款期間，或利率期間為零，請重新輸入。";
    break;
  case 31:
    message = "每月應付房貸金額錯誤。";
    break;
  case 32:
    message = "擔保品所有人姓名尚未填寫。";
    break;
  case 33:
    message = "擔保品所有人身分證號碼錯誤。";
    break;
  case 34:
    message = "擔保品地號/建號尚未填寫。";
    break;
  case 35:
    message = "擔保品所有人與申請人關係尚未填寫。";
    break;
  case 36:
    message = "房屋毛值錯誤。";
    break;
  case 37:
    message = "房屋淨值錯誤。";
    break;
  case 38:
    message = "未確認擔保品是否符合承做條件。";
    break;
  case 39:
    message = "未確認擔保品是否為優質擔保品。";
    break;
  case 40:
    message = "進件員工尚未填寫。";
    break;
  case 41:
    message = "進件分行錯誤。";
    break;
  case 42:
    message = "徵審人員尚未填寫。";
    break;
  case 43:
    message = "申請人基本資料不符合承做條件。";
    break;
  case 44:
    message = "共同申請人基本資料不符合承做條件。";
    break;
  case 45:
    message = "保證人基本資料不符合承做條件。";
    break;
  case 46:
    message = "擔保品不符合承做條件。";
    break;
  default:
    message = "Unrecognized error code";
  };
  return message;
}
