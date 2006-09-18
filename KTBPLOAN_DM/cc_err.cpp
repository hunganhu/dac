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
    message = "��DAC�֭�ҲնȨѨʫ��Ȧ�ϥΡC";
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
/*  case 106:
    message = "�Ҳի�ĳ���ڡA�^�m�׹L�C�C";
    break;*/
  case 107:
    message = "�Ҳի�ĳ���ڡA���@�U�ڳs��T�Ӥ�H�Wú�ڤ����`�C";
    break;
  case 108:
    message = "�Ҳի�ĳ���ڡA�H�Υd�P�{���d�l�B�L���C";
    break;
  case 109:
    message = "�Ҳի�ĳ���ڡA�a�b���v�L���C";
    break;
  case 110:
    message = "����H�Υd�H�Τ��}�C";
    break;
  case 111:
    message = "�Ҳի�ĳ���ڡAJCIC��Ƥ����B�d�ߦ��ƹL�h�C";
    break;
  case 112:
    message = "�Ҳի�ĳ���ڡAJCIC��Ƥ����B���H�Υd�Oú�����C";
    break;
  case 113:
    message = "�Ҳի�ĳ���ڡAJCIC��Ƥ����B���j��d�����C";
    break;
  case 114:
    message = "�Ҳի�ĳ���ڡAJCIC��Ƥ����B�d�ߦ��ƹL�h�C";
    break;
  case 115:
    message = "�Ҳի�ĳ���ڡAJCIC��Ƥ����B���j��d�����C";
    break;
  case 116:
    message = "�Ҳի�ĳ���ڡA�t�ŹL���C";
    break;
  case 201:
    message = "�Ҳի�ĳ�H�u�f�֡AJCIC��Ƥ����C";
    break;
  default:
    message = "Unrecognized error code";
  };
  return message;
}

