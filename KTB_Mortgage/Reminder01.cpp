//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Reminder01.h"
#include "ResultNewMortgage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSNReminder1 *frmSNReminder1;
//---------------------------------------------------------------------------
__fastcall TfrmSNReminder1::TfrmSNReminder1(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmSNReminder1::btnScoringClick(TObject *Sender)
{
  frmRegularResult->Visible = true;
  frmRegularResult->btnScore->Click();
  frmSNReminder1->Close();
}
//---------------------------------------------------------------------------

