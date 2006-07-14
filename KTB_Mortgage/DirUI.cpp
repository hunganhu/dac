//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DirUI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmDirUI *frmDirUI;
//---------------------------------------------------------------------------
__fastcall TfrmDirUI::TfrmDirUI(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDirUI::btnDirCancelClick(TObject *Sender)
{
  frmDirUI->Close();  
}
//---------------------------------------------------------------------------


void __fastcall TfrmDirUI::dlbReportMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{         
  frmDirUI->edtDir->Text = frmDirUI->dlbReport->Directory;
}
//---------------------------------------------------------------------------



void __fastcall TfrmDirUI::btnDirSetClick(TObject *Sender)
{
  report_dir = frmDirUI->edtDir->Text;
  if(report_dir[report_dir.Length()] != '\\')
    report_dir += "\\";
  frmDirUI->Close();
}
//---------------------------------------------------------------------------

