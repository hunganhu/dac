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
    message = "��DAC�֭�ҲնȨѨʫ��ӻȨϥΡC";
  case 101:
    message = "�Ҳի�ĳ���ڡA���g�j�� / �ک� / �h�� / �«H���`�C";
    break;
  case 102:
    message = "�Ҳի�ĳ���ڡA���@�H�Υd�s��T�Ӥ�H�W�`��ú�ڤ����`�C";
    break;
  case 103:
    message = "�Ҳի�ĳ���ڡA���@�{���d�̪�@�Ӥ�ú�ک���C";
    break;
  case 104:
    message = "�Ҳի�ĳ���ڡA���@�{���d�ʥΤ�v >= 95%�C";
    break;
  case 105:
    message = "�Ҳի�ĳ���ڡA���@�U�ڹO���C";
    break;
  case 106:
    message = "�Ҳի�ĳ���ڡA�^�m�׹L�C�C";
    break;
  case 107:
    message = "�Ҳի�ĳ���ڡA���@�U�ڳs��ú�ڤ����`�C";
    break;
  case 108:
    message = "�Ҳի�ĳ���ڡA�H�Υd�P�{���d�l�B�L���C";
    break;
  case 109:
    message = "�Ҳի�ĳ���ڡAJCIC��Ƥ����B�d�ߦ��ƹL�h�C";
    break;
  case 200:
    message = "�Ҳի�ĳ�H�u�f�֡AJCIC��Ƥ����C";
    break;
  case 201:
    message = "�Ҳի�ĳ�H�u�f�֡AJCIC��Ƥ����C��ĳ�֭��B�פ��y�W�L�s�x����U���C";
    break;
  case 203:
    message = "�Ҳի�ĳ�H�u�f�֡A�H�Υd�`�����B > �B�ת�75%�C";
    break;
  case 204:
    message = "�Ҳի�ĳ�H�u�f�֡A�H�Υd�B�׼� <= 2�B�b�ƥH�W�H�Υd���g����ú�ڡC";
    break;
  default:
    message = "Unrecognized error code";
  };
  return message;
}

