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
    message = "��DAC�֭�ҲնȨѵع��Ȧ�ϥΡC";
  case 101:
    message = "��ĳ���ڡA���j�H�η岫�C";
    break;
  case 102:
    message = "��ĳ���ڡA�H�Υdú�ڰO�����}�C";
    break;
  case 103:
    message = "��ĳ���ڡA�{���d������O���C";
    break;
  case 104:
    message = "��ĳ���ڡA�{���d�ʤ��Ҥj��ʤ���" + static_cast<AnsiString>(_cash_card_util_cap * 100) + "�C";
    break;
  case 105:
    message = "��ĳ���ڡA�U��ú�ڰO�����}�C";
    break;
  case 108:
    message = "��ĳ���ڡA�H�Υd�P�{���d�l�B�j��s�x��" + static_cast<AnsiString>(_cc_balance_cap) + "���C";
    break;
  case 106:
    message = "��ĳ���ڡA��ع��Ȧ�^�m�׹L�C�C";
    break;
  case 201:
    message = "��ĳ�H�u�f�֡AJCIC��Ƥ����C";
    break;
  case 202:
    message = "��ĳ�H�u�f�֡A�l�B�N�v�ӽЪ��B�W�L����C";
    break;
  case 203:
    message = "��ĳ�H�u�f�֡A�H�Υd�`�����B�j���B�ת��ʤ���" + static_cast<AnsiString>(_credit_card_util_cap * 100) + "�C";
    break;
  case 204:
    message = "��ĳ�H�u�f�֡A�H�Υd�B�׼� <= 2�B�b�ƥH�W�H�Υd���g����ú�ڡC";
    break;
  case 301:
    message = "�L�k���Ѳ��~�ʽ�O�C";
    break;
  case 302:
    message = "�N�v���B�p��ε���s�C";
    break;
  case 303:
    message = "�N�v���Ƥp��ε���s�C";
    break;
  case 304:
    message = "�N�v�u�f����(��)�p��s�C";
    break;
  case 305:
    message = "�N�v�u�f�Q�v���ƥ[�`������N�v�u�f�`���ơC";
    break;
  case 306:
    message = "�L�k���ѥN�v����O�C";
    break;
  case 307:
    message = "�L�k���ѥN�v����O�����覡�C";
    break;
  case 308:
    message = "�C��N�v����O���~�C";
    break;
  case 309:
    message = "�N�v�~�Q�v�p��s�Τj��H�Υd�Q�v�C";
    break;
  case 310:
    message = "�H�Υd�~�Q�v���~�C";
    break;
  case 311:
    message = "�N�v����O��������(��)���~�C";
    break;
  case 312:
    message = "�L���ӽХ��ơC";
    break;
  case 313:
    message = "�ӽХ��ƭ��ơC";
    break;
  case 314:
    message = "�L�k���ѥd�O�C";
    break;
  default:
    message = "Unrecognized error code";
  };
  return message;
}



