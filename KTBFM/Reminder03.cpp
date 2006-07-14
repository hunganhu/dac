//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Reminder03.h"
#include "ResultBT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSNReminder3 *frmSNReminder3;
//---------------------------------------------------------------------------
__fastcall TfrmSNReminder3::TfrmSNReminder3(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSNReminder3::btnScoringClick(TObject *Sender)
{
  frmBTResult->Visible = true;
  frmBTResult->btnScore->Click();
  frmBTResult->Visible = true;
  frmSNReminder3->Close();
}
//---------------------------------------------------------------------------
