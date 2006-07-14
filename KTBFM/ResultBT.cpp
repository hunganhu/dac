//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ResultBT.h"
#include "LoanTypeSelection.h"
#include "dm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmBTResult *frmBTResult;
extern AnsiString case_sn;
extern AnsiString connection_string_module;
//---------------------------------------------------------------------------
__fastcall TfrmBTResult::TfrmBTResult(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmBTResult::btnNextClick(TObject *Sender)
{
  frmSelection->Show();
  frmBTResult->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBTResult::btnScoreClick(TObject *Sender)
{
  init(case_sn);
  char c_message[256];
  int return_code = 0;

  return_code = FM_Transfer(case_sn.c_str(), connection_string_module.c_str(), c_message);
  if(return_code != 0)
    lblExecution->Caption += static_cast<AnsiString>(c_message);
  else{
    lblExecution->Caption += "評分完成\n";
    try{
      fill_result(Data->query, case_sn);
    }
    catch(Exception &Err){
      lblExecution->Caption += Err.Message;
    };
  };
  frmBTResult->Refresh();
};

void __fastcall TfrmBTResult::fill_result(TADOQuery *query, const AnsiString &case_sn)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM APP_RESULT AS A INNER JOIN APP_INFO AS B ";
  sql_stmt += "ON A.CASE_NO = B.CASE_NO WHERE A.CASE_NO = :case_sn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["case_sn"] = case_sn;
  query->Open();
  if(query->RecordCount == 0)
    lblExecution->Caption = "程式錯誤，沒有找到評分結果，請聯絡DAC\n";
  else{
    if(query->FieldValues["APP_NAME"].IsNull())
      lblAppName->Caption = "";
    else
      lblAppName->Caption = query->FieldValues["APP_NAME"];
    if(query->FieldValues["COS_NAME"].IsNull())
      lblCoAppName->Caption = "";
    else
      lblCoAppName->Caption = query->FieldValues["COS_NAME"];
    if(query->FieldValues["GUA_NAME"].IsNull())
      lblGuarantorName->Caption = "";
    else
      lblGuarantorName->Caption = query->FieldValues["GUA_NAME"];
    if(query->FieldValues["APP_SCRMSG"].IsNull())
      lblAppMsg->Caption = "";
    else
      lblAppMsg->Caption = query->FieldValues["APP_SCRMSG"];
    if(query->FieldValues["COS_SCRMSG"].IsNull())
      lblCoAppMsg->Caption = "";
    else
      lblCoAppMsg->Caption = query->FieldValues["COS_SCRMSG"];
    if(query->FieldValues["GUA_SCRMSG"].IsNull())
      lblGuarantorMsg->Caption = "";
    else
      lblGuarantorMsg->Caption = query->FieldValues["GUA_SCRMSG"];
  };
  frmBTResult->Refresh();
};

void __fastcall TfrmBTResult::init(const AnsiString &case_sn)
{
  lblSN->Caption = case_sn;
  lblAppName->Caption = "";
  lblCoAppName->Caption = "";
  lblGuarantorName->Caption = "";
  lblAppMsg->Caption = "";
  lblCoAppMsg->Caption = "";
  lblGuarantorMsg->Caption = "";
  lblExecution->Caption = "模組評分中\n";
  frmBTResult->Refresh();
};
//---------------------------------------------------------------------------

void __fastcall TfrmBTResult::btnEndClick(TObject *Sender)
{
  Application->Terminate();  
}
//---------------------------------------------------------------------------

