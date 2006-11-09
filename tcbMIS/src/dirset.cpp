//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ui.h"
#include "dirset.h"
extern AnsiString report_dir;
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
void __fastcall TForm2::btnCancelClick(TObject *Sender)
{
  Form2->Close();  
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
  report_dir = Form2->edtDir->Text;
  if(report_dir[report_dir.Length()] != '\\')
    report_dir += "\\";
  Form2->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::dlbReportMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  Form2->edtDir->Text = Form2->dlbReport->Directory;   
}
//---------------------------------------------------------------------------

