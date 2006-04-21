//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "LoginUI.h"
#include "Main.h"
#include "Data_Module.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner)
  : TForm(Owner)
{
//  frmLogin->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btnCancelClick(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------
/*
void __fastcall TfrmLogin::FormCreate(TObject *Sender)
{
  frmLogin->ShowModal();
}
*/
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btnLoginClick(TObject *Sender)
{
  static int login_error = 0;
  static int connection_error = 0;
  bool succeed = false;
  AnsiString message;
  AnsiString sql_stmt;


  Data->connection->Close();
  AnsiString connection_string;
  connection_string = "Provider=SQLOLEDB.1;";
  connection_string += "Password=Emily1013;";
  connection_string += "Persist Security Info=True;User ID=sa;";
  connection_string += "Initial Catalog=KTB_SML;Data Source=oliver\\daisy";
  Data->connection->ConnectionString = connection_string;
  Data->connection->ConnectionTimeout = 5;

//  connection_string = "Provider=SQLOLEDB.1;";
//  connection_string += "Password=";
//  connection_string += edtPassword->Text + ";";
//  connection_string += ";Persist Security Info=True;User ID=";
//  connection_string += edtUser->Text + ";";
//  connection_string += "Initial Catalog=KTB_SML;Data Source=oliver\\daisy";
//  Data->connection->ConnectionString = connection_string;
//  Data->connection->ConnectionTimeout = 5;
  try{
    Data->connection->Open();

//    while (login_error < 3 && !succeed) {
       sql_stmt = "SELECT COUNT(*) AS CNT FROM AGENT WHERE userid = :idn and userpw=:pw;";
       sql_stmt = sql_stmt.UpperCase();
       Data->query->Close();
       Data->query->SQL->Clear();
       Data->query->SQL->Add(sql_stmt);
       Data->query->Parameters->ParamValues["idn"] = edtUser->Text;
       Data->query->Parameters->ParamValues["pw"] = edtPassword->Text;
       Data->query->Open();
       double score = Data->query->FieldValues["CNT"];
       Data->query->Close();
       if (score > 0)  //success
       	  succeed = true;
       else
          login_error++;
//    }
  }
  catch(Exception &E){
    string error_message = static_cast<string>(E.Message.c_str());
    int n = error_message.find("登入失敗");
    if(n > 0){
//      ++login_error;
      message = "使用者帳號或密碼錯誤，請重新輸入。";
      MessageDlg(message, mtWarning, TMsgDlgButtons() << mbOK, 0);
      edtPassword->Text = "";
      edtUser->Text = "";
      edtUser->SetFocus();
      succeed = false;
    }
    else {
      ++connection_error;
      message = "資料庫或網路連線失敗，請稍後再試。";
      MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
      btnLogin->SetFocus();
      succeed = false;
    }
  }
  if(succeed){
//    connection_string = "Provider=SQLOLEDB.1;";
//    connection_string += "Password=Emily1013;";
//    connection_string += "Persist Security Info=True;User ID=sa;";
//    connection_string += "Initial Catalog=TNB_SML;Data Source=oliver\\daisy";
//    Data->connection->Close();
//    Data->connection->ConnectionString = connection_string;
//    Data->connection->ConnectionTimeout = 5;
//    Data->connection->Open();
    Data->query->Close();
    Data->query->Connection = Data->connection;
    Data->query->CommandTimeout = Command_time_out;
    Data->command->Connection = Data->connection;
    Data->command->CommandTimeout = Command_time_out;
    Data->command->CommandType = cmdText;
    ModalResult = mrCancel;
    frmLogin->Close();
    formMain->Show();
  } else {
      message = "使用者帳號或密碼錯誤，請重新輸入。";
      MessageDlg(message, mtWarning, TMsgDlgButtons() << mbOK, 0);
      edtPassword->Text = "";
      edtUser->Text = "";
      edtUser->SetFocus();
      succeed = false;
  }

  if(login_error > 3){
    message = "登入錯誤超過三次，程式中止。";
    MessageDlg(message, mtError, TMsgDlgButtons() << mbOK, 0);
    btnCancel->Click();
  }
  else if (connection_error > 3){
    message = "資料庫或網路連線失敗超過三次，程式中止，請洽網路管理與資料庫管理人員。";
    MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
    btnCancel->Click();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::FormShow(TObject *Sender)
{
  edtUser->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btnLoginKeyPress(TObject *Sender, char &Key)
{
  if(Key == 0x0A)
    btnLoginClick(Sender);
}
//---------------------------------------------------------------------------






