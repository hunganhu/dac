//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ResultReload.h"
#include "LoanTypeSelection.h"
#include "dm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmReloadResult *frmReloadResult;
extern AnsiString case_sn;
extern AnsiString connection_string_module;
//---------------------------------------------------------------------------
__fastcall TfrmReloadResult::TfrmReloadResult(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmReloadResult::btnNextClick(TObject *Sender)
{
  frmSelection->Show();
  frmReloadResult->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReloadResult::btnScoreClick(TObject *Sender)
{
  init(case_sn);
  char c_message[256];
  int return_code = 0;
  return_code = FM_Reload(case_sn.c_str(), connection_string_module.c_str(), c_message);
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
  frmReloadResult->Refresh();
};
//---------------------------------------------------------------------------

void __fastcall TfrmReloadResult::fill_result(TADOQuery *query, const AnsiString &case_sn)
{
  AnsiString sql_stmt;
  unsigned int amount;
  double rate1, rate2, rate3;
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
    amount = query->FieldValues["APPROVED_AMOUNT"].IsNull() ? 0 : query->FieldValues["APPROVED_AMOUNT"];
    amount /= 10000;
    if(amount == 0)
      lblAmount->Caption = "";
    else
      lblAmount->Caption = amount;

    rate1 = query->FieldValues["MIN_RATE1"].IsNull() ? 0 : query->FieldValues["MIN_RATE1"];
    rate1 *= 100;
    if(query->FieldValues["MIN_RATE1"].IsNull())
      lblRate1->Caption = "";
    else
      lblRate1->Caption = rate1;
    rate2 = query->FieldValues["MIN_RATE2"].IsNull() ? 0 : query->FieldValues["MIN_RATE2"];
    rate2 *= 100;
    if(query->FieldValues["MIN_RATE2"].IsNull())
      lblRate2->Caption = "";
    else
      lblRate2->Caption = rate2;
    rate3 = query->FieldValues["MIN_RATE3"].IsNull() ? 0 : query->FieldValues["MIN_RATE3"];
    rate3 *= 100;
    if(query->FieldValues["MIN_RATE3"].IsNull())
      lblRate3->Caption = "";
    else
      lblRate3->Caption = rate3;
    if(query->FieldValues["SEG1"].IsNull())
      lblPeriod1->Caption = "";
    else
      lblPeriod1->Caption = query->FieldValues["SEG1"];
    if(query->FieldValues["SEG2"].IsNull())
      lblPeriod2->Caption = "";
    else
      lblPeriod2->Caption = query->FieldValues["SEG2"];
    if(query->FieldValues["SEG3"].IsNull())
      lblPeriod3->Caption = "";
    else
      lblPeriod3->Caption = query->FieldValues["SEG3"];
    if(query->FieldValues["SUGG_MSG"].IsNull())
      lblSuggestion1->Caption = "";
    else
      lblSuggestion1->Caption = query->FieldValues["SUGG_MSG"];
    if(query->FieldValues["REASON_MSG"].IsNull())
      lblSuggestion2->Caption = "";
    else
      lblSuggestion2->Caption = query->FieldValues["REASON_MSG"];
  };
  frmReloadResult->Refresh();
};

void __fastcall TfrmReloadResult::init(const AnsiString &case_sn)
{
  lblSN->Caption = case_sn;
  lblAmount->Caption = "";
  lblPeriod1->Caption = "";
  lblPeriod2->Caption = "";
  lblPeriod3->Caption = "";
  lblRate1->Caption = "";
  lblRate2->Caption = "";
  lblRate3->Caption = "";
  lblSuggestion1->Caption = "";
  lblSuggestion2->Caption = "";
  lblExecution->Caption = "模組評分中\n";
  frmReloadResult->Refresh();
};
void __fastcall TfrmReloadResult::Button1Click(TObject *Sender)
{
  Application->Terminate();  
}
//---------------------------------------------------------------------------

