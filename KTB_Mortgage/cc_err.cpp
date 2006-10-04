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
    message = "�ӽФH�m�W�|����g�C";
    break;
  case 2:
    message = "�ӽФH�����Ҹ��X���~�C";
    break;
  case 3:
    message = "�ӽФH�X�ͦ~�����~�C";
    break;
  case 4:
    message = "�ӽФH�B�ê��A�|����g�C";
    break;
  case 5:
    message = "�ӽФH�Ǿ��|����g�C";
    break;
  case 6:
    message = "�ӽФH�~���J�|����g�C";
    break;
  case 7:
    message = "���T�w�ӽФH�򥻸�ƬO�_�ŦX�Ӱ�����C";
    break;
  case 8:
    message = "�@�P�ɴڤH�m�W�|����g�C";
    break;
  case 9:
    message = "�@�P�ɴڤH�����Ҹ��X���~�C";
    break;
  case 10:
    message = "�@�P�ɴڤH�X�ͦ~�����~�C";
    break;
  case 11:
    message = "�@�P�ɴڤH�B�ê��A�|����g�C";
    break;
  case 12:
    message = "�@�P�ɴڤH�Ǿ��|����g�C";
    break;
  case 13:
    message = "�@�P�ɴڤH�~���J�|����g�C";
    break;
  case 14:
    message = "���T�w�@�P�ɴڤH�򥻸�ƬO�_�ŦX�Ӱ�����C";
    break;
  case 15:
    message = "�O�ҤH�m�W�|����g�C";
    break;
  case 16:
    message = "�O�ҤH�����Ҹ��X���~�C";
    break;
  case 17:
    message = "�O�ҤH�X�ͦ~�����~�C";
    break;
  case 18:
    message = "�O�ҤH�B�ê��A�|����g�C";
    break;
  case 19:
    message = "�O�ҤH�Ǿ��|����g�C";
    break;
  case 20:
    message = "�O�ҤH�~���J�|����g�C";
    break;
  case 21:
    message = "���T�{�O�ҤH�򥻸�ƬO�_�ŦX�Ӱ�����C";
    break;
  case 22:
    message = "�ӽЪ��B���~�C";
    break;
  case 23:
    message = "�ɴڴ������~�A���T�Ȭ�12��240�C";
    break;
  case 24:
    message = "�e�������~�C";
    break;
  case 25:
    message = "�}��O�|����g�C";
    break;
  case 26:
    message = "�Q�v���q�|����g�C";
    break;
  case 27:
    message = "�Ĥ@�q�Q�v���~�A���T�Ȭ�0��10�C";
    break;
  case 28:
    message = "�ĤG�q�Q�v���~�A���T�Ȭ�0��10�C";
    break;
  case 29:
    message = "�ĤT�q�Q�v���~�A���T�Ȭ�0��10�C";
    break;
  case 30:
    message = "�U�q�Q�v�����`�M������ɴڴ����A�ΧQ�v�������s�A�Э��s��J�C";
    break;
  case 31:
    message = "�C�����I�жU���B���~�C";
    break;
  case 32:
    message = "��O�~�Ҧ��H�m�W�|����g�C";
    break;
  case 33:
    message = "��O�~�Ҧ��H�����Ҹ��X���~�C";
    break;
  case 34:
    message = "��O�~�a��/�ظ��|����g�C";
    break;
  case 35:
    message = "��O�~�Ҧ��H�P�ӽФH���Y�|����g�C";
    break;
  case 36:
    message = "�ЫΤ�ȿ��~�C";
    break;
  case 37:
    message = "�Ыβb�ȿ��~�C";
    break;
  case 38:
    message = "���T�{��O�~�O�_�ŦX�Ӱ�����C";
    break;
  case 39:
    message = "���T�{��O�~�O�_���u���O�~�C";
    break;
  case 40:
    message = "�i����u�|����g�C";
    break;
  case 41:
    message = "�i�������~�C";
    break;
  case 42:
    message = "�x�f�H���|����g�C";
    break;
  case 43:
    message = "�ӽФH�򥻸�Ƥ��ŦX�Ӱ�����C";
    break;
  case 44:
    message = "�@�P�ӽФH�򥻸�Ƥ��ŦX�Ӱ�����C";
    break;
  case 45:
    message = "�O�ҤH�򥻸�Ƥ��ŦX�Ӱ�����C";
    break;
  case 46:
    message = "��O�~���ŦX�Ӱ�����C";
    break;
  default:
    message = "Unrecognized error code";
  };
  return message;
}
