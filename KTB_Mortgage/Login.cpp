//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Login.h"
#include "LoanTypeSelection.h"
AnsiString input_user;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::btnExit01Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btnLoginClick(TObject *Sender)
{
  Data->connection->Close();
  AnsiString connection_string;
  connection_string = "Provider=SQLOLEDB.1;";
  connection_string += "Password=fm_user1;";
  connection_string += "Persist Security Info=True;User ID=fm_user1;";
  connection_string += "Initial Catalog=KTB_FM;Data Source=DAC-DB2";
//  connection_string = "Provider=SQLOLEDB.1;Password=dac;Persist Security Info=True;User ID=dac;Initial Catalog=KTB_FM;Data Source=oliver\\daisy";
  Data->connection->ConnectionString = connection_string;
  Data->connection->ConnectionTimeout = 5;
  static int login_error = 0;
  static int connection_error = 0;
  AnsiString message;
  try{
//    Data->connection->Open();
    if(login(Data->query, frmLogin->edtUser->Text, frmLogin->edtPassword->Text)){
      input_user = frmLogin->edtUser->Text.Trim();
      Data->query->Close();
      Data->query->Connection = Data->connection;
      Data->query->CommandTimeout = Command_time_out;
      Data->command->Connection = Data->connection;
      Data->command->CommandTimeout = Command_time_out;
      Data->command->CommandType = cmdText;
      frmLogin->Close();
      frmSelection->Show();
    }
    else{
      ++login_error;
      message = "使用者帳號或密碼錯誤，請重新輸入。";
      MessageDlg(message, mtWarning, TMsgDlgButtons() << mbOK, 0);
      edtPassword->Text = "";
      edtUser->Text = "";
      edtUser->SetFocus();
      if(login_error > 2){
        message = "登入錯誤已達三次，程式中止。";
        MessageDlg(message, mtError, TMsgDlgButtons() << mbOK, 0);
        btnExit01->Click();
      }
    }
  }
	catch(Exception &E){
    ++connection_error;
    message = "資料庫或網路連線失敗，請稍後再試。";
    MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
    btnLogin->SetFocus();
  };
  if (connection_error > 2){
    message = "資料庫或網路連線失敗已達三次，程式中止，請洽網路管理與資料庫管理人員。";
    MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
    btnExit01->Click();
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

void __fastcall TfrmLogin::FormCreate(TObject *Sender)
{
  frmLogin->ShowModal();
}

