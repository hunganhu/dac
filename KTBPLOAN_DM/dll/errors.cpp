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
	{1,   "��DAC�֭�ҲնȨѨʫ��Ȧ�ϥΡC"},
	{2,   "The approval module has expired."},
	{101, "���g�j�� / �ک� / �h�� / �«H���`�C"},
	{102, "���@�H�Υd�s��T�Ӥ�H�W�`��ú�ڤ����`�C"},
	{103, "���@�{���d�̪�@�Ӥ�ú�ک���C"},
	{104, "�̪�@�Ӥ�{���d�ϥβv�W�L95%(�t)"},
	{105, "���@�U�ڹO���C"},
	{106, "�^�m�׹L�C�C"},
	{107, "���@�U�ڳs��T�Ӥ�H�Wú�ڤ����`�C"},
	{108, "�̪�@�Ӥ�{���d�[�H�Υd�`�����B�j�󤭤Q�U(�t)"},
	{109, "�a�b���v�L���C"},
	{110, "�ʫ��Ȧ�¦W��C"},
	{111, "JCIC��Ƥ����B�d�ߦ��� > 5 �C"},
	{112, "JCIC��Ƥ����B���H�Υd�Oú�����C"},
	{113, "JCIC��Ƥ����B���j��d�����C"},
	{114, "JCIC��Ƥ����B�d�ߦ��� > 3 �C"},
	{115, "JCIC��Ƥ����B���j��d�����C"},
	{116, "�̪�@�Ӥ�{���d�[�H�Υd�`�����B�[�L��O�U�ھl�B�j��@�ʸU(�t)"},
	{117, "���J22������i�ɶU���B�p��15�U�C"},
	{118, "�g�ٻ��ȹL�C�C"},
	{119, "�̪�@�Ӥ���@�U��ú�ڤ����`�C"},
	{120, "���j��d�����C"},
	{121, "�̪�T�Ӥ릳�H�Υd�Oú�����C"},
	{122, "���ʦ��Χb�b���O�C"},
	{123, "���ɥR/���O��T�C"},
	{201, "JCIC��Ƥ����C"},
	{301, "�Ҳի�ĳ�֭�C"},
	{302, "�Юֹꦬ�J�T�{��A�Ҳի�ĳ�֭�C"},
	{303, "�Ҳի�ĳ�֭�C"},
	{304, "�Юֹꦬ�J�T�{��A�Ҳի�ĳ�֭�C"}
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


