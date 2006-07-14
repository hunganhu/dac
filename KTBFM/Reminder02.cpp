//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Reminder02.h"
#include "ResultReload.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSNReminder2 *frmSNReminder2;
//---------------------------------------------------------------------------
__fastcall TfrmSNReminder2::TfrmSNReminder2(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmSNReminder2::btnScoringClick(TObject *Sender)
{
  frmReloadResult->Visible = true;
  frmReloadResult->btnScore->Click();
  frmSNReminder2->Close();
}
//---------------------------------------------------------------------------

