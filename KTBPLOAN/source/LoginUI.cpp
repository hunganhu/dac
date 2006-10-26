//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "LoginUI.h"
#include "MainUI.h"
#include "Data_Module.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLogin *frmLogin;
AnsiString input_user = "";
//---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner)
  : TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btnCancelClick(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::FormCreate(TObject *Sender)
{
  frmLogin->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btnLoginClick(TObject *Sender)
{
  Data->connection->Close();
  AnsiString connection_string;
  connection_string = "Provider=SQLOLEDB.1;";
  connection_string += "Password=dac_ploan_1;";
  connection_string += "Persist Security Info=True;User ID=dac_ploan_1;";
  connection_string += "Initial Catalog=PLOAN;Data Source=DAC-DB2";
  Data->connection->ConnectionString = connection_string;
  Data->connection->ConnectionTimeout = 5;
  static int login_error = 0;
  static int connection_error = 0;
  AnsiString message;
  try{
    Data->connection->Open();
    if(login(Data->query, frmLogin->edtUser->Text, frmLogin->edtPassword->Text)){
      input_user = frmLogin->edtUser->Text.Trim();
      Data->query->Close();
      Data->query->Connection = Data->connection;
      Data->query->CommandTimeout = Command_time_out;
      Data->command->Connection = Data->connection;
      Data->command->CommandTimeout = Command_time_out;
      Data->command->CommandType = cmdText;
      frmLogin->Close();
      frmMainUI->Show();
    }
    else{
      ++login_error;
      message = "�ϥΪ̱b���αK�X���~�A�Э��s��J�C";
      MessageDlg(message, mtWarning, TMsgDlgButtons() << mbOK, 0);
      edtPassword->Text = "";
      edtUser->Text = "";
      edtUser->SetFocus();
      if(login_error > 2){
        message = "�n�J���~�w�F�T���A�{������C";
        MessageDlg(message, mtError, TMsgDlgButtons() << mbOK, 0);
        btnCancel->Click();
      }
    }
  }
	catch(Exception &E){
    ++connection_error;
    message = "��Ʈw�κ����s�u���ѡA�еy��A�աC";
    MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
    btnLogin->SetFocus();
  };
  if (connection_error > 2){
    message = "��Ʈw�κ����s�u���Ѥw�F�T���A�{������A�Ь������޲z�P��Ʈw�޲z�H���C";
    MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
    btnCancel->Click();
  };
};
//---------------------------------------------------------------------------
bool login(TADOQuery *query, const AnsiString &acct, const AnsiString &passwd)
{
  AnsiString sql_stmt = "SELECT COUNT(*) AS CNT FROM AGENT ";
  sql_stmt += "WHERE USERID = :acct AND USERPW = :passwd;";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["acct"] = acct.Trim();
  query->Parameters->ParamValues["passwd"] = passwd.Trim();
  query->Open();
  if(query->FieldValues["CNT"] > 0)
    return true;
  else
    return false;
};
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::FormShow(TObject *Sender)
{
  edtUser->SetFocus();
}
//---------------------------------------------------------------------------

