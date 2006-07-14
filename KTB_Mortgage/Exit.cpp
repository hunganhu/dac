//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Exit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmExitDialog *frmExitDialog;
//---------------------------------------------------------------------------
__fastcall TfrmExitDialog::TfrmExitDialog(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmExitDialog::btnExit03Click(TObject *Sender)
{
  Application->Terminate();  
}
//---------------------------------------------------------------------------

void __fastcall TfrmExitDialog::btnResumeClick(TObject *Sender)
{
  frmExitDialog->Close();  
}
//---------------------------------------------------------------------------

