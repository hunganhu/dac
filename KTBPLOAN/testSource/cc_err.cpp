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
    message = "��DAC�֭�ҲնȨѥx�n���ȨϥΡC";
  case 101:
    message = "�Ҳի�ĳ���ڡA�T�~�����g�j�� / �ک� / �h�� / �«H���`�C";
    break;
  case 102:
    message = "�Ҳի�ĳ���ڡA���@�H�Υd�s��T�Ӥ�H�W�`��ú�ڤ����`�C";
    break;
  case 103:
    message = "�Ҳի�ĳ���ڡA���@�{���d�̪�@�Ӥ�ú�ک���C";
    break;
  case 104:
    message = "�Ҳի�ĳ���ڡA���@�{���d�ʥΤ�v >= 100%�C";
    break;
  case 105:
    message = "�Ҳի�ĳ���ڡA���@�U�ڹO���C";
    break;
  case 106:
    message = "�Ҳի�ĳ���ڡA�^�m�׹L�C�C";
    break;
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
    message = "�Ҳի�ĳ���ڡA����H�Υd�H�Τ��}�C";
    break;
  case 201:
    message = "�Ҳի�ĳ�H�u�f�֡AJCIC��Ƥ����C";
    break;
  case 202:
    message = "�Ҳի�ĳ�H�u�f�֡A�ӽЪ��B�W�L����C";
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
    message = "�ӶU����(��)���~�C";
    break;
  case 305:
    message = "�N�v�u�f�Q�v���ƥ[�`������N�v�u�f�`���ơC";
    break;
  case 306:
    message = "�ӶU����O���~�C";
    break;
  case 307:
    message = "�L�k���ѥN�v����O�����覡�C";
    break;
  case 308:
    message = "�C��N�v����O�ʤ���p��s�C";
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

