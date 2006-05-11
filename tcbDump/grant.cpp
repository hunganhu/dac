//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "grant.h"
#include "update.h"
#include "AdoHandle.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
 AnsiString userName, passWord, connectString, sqlStatement;
 int record_count;

 Close();
 userName = Edit1->Text;
 passWord = Edit2->Text;
 connectString = "Provider=SQLOLEDB.1;Password=" + passWord +
                 ";Persist Security Info=True;User ID=" + userName +
                 ";Initial Catalog=escore;Data Source=OLIVER\\DAISY";
 try {
   Form1->dbhandle = new TADOHandler();
   Form1->dbhandle->OpenDatabase(connectString.c_str());
   Form1->ds = new TADODataSet(NULL);
   Form1->ds->EnableBCD = false;  // Decimal fields are mapped to float.
   Form1->message->Caption = "  ";
   Form1->connect->Visible = false;
   Form1->ApprovePay->Visible = true;
   Form1->ApproveDecline->Visible = true;
   Form1->ApproveUnclose->Visible = true;
   Form1->DeclinePay->Visible = true;
   Form1->ManualPay->Visible = true;
   Form1->All->Visible = true;
   Form1->OutputDir->Visible = true;
   Form1->Visible = true;
 }
 catch (Exception &E) {
     Form1->message->Caption = E.Message;
     Form1->Refresh();
 }
 catch (...) {
    ShowMessage("連線失敗！請檢查使用者名稱和密碼。");
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
