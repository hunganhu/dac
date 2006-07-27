//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Driver.h"
#include "dmcpp.h"
#include "CPDACO.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  Application->Terminate();  
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
  Data->connection->Close();
  Data->connection->ConnectionString = Edit1->Text;
  Data->connection->CommandTimeout = 300;
  Data->connection->ConnectionTimeout = 30;
  Data->connection->Open();

  AnsiString sql_stmt;
  char error_msg[256];
  sql_stmt = "SELECT * FROM DRIVER ORDER BY MSN";
  Data->query->Close();
  Data->query->SQL->Clear();
  Data->query->SQL->Add(sql_stmt);
  Data->query->Open();
  while(!Data->query->Eof){
    AnsiString msn = Data->query->FieldValues["MSN"];
    AnsiString input_time = Data->query->FieldValues["INPUT_TIME"];
    AnsiString inquiry_date = Data->query->FieldValues["INQUIRY_DATE"];
    int gender = Data->query->FieldValues["GENDER"];
    int value = TNB_Ploan_AM_Campaign(msn.c_str(), inquiry_date.c_str(),
                                      input_time.c_str(), "054",
                                      Edit1->Text.c_str(), gender, error_msg);
    Data->query->Next();
  }
  Application->MessageBox("Process finished.", "Done", MB_OK);
}
//---------------------------------------------------------------------------

