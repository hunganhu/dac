//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReDirUI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmReDirUI *frmReDirUI;
//---------------------------------------------------------------------------
__fastcall TfrmReDirUI::TfrmReDirUI(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmReDirUI::btnDirCancelClick(TObject *Sender)
{
  frmReDirUI->Close();    
}
//---------------------------------------------------------------------------
void __fastcall TfrmReDirUI::Button1Click(TObject *Sender)
{
  report_regen_dir = frmReDirUI->edtReDir->Text;
  if(report_regen_dir[report_regen_dir.Length()] != '\\')
    report_regen_dir += "\\";
  frmReDirUI->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReDirUI::dlbReReportMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  frmReDirUI->edtReDir->Text = frmReDirUI->dlbReReport->Directory;
}
//---------------------------------------------------------------------------

