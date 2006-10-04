//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LoanTypeSelection.h"
#include "InputNewMortgage.h"
#include "Login.h"
#include "InputReload.h"
#include "InputBT.h"
#include "ReportGen.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelection *frmSelection;
AnsiString case_sn;
AnsiString connection_string_module = "";
//---------------------------------------------------------------------------
__fastcall TfrmSelection::TfrmSelection(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelection::btnExit02Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelection::FormCreate(TObject *Sender)
{
  CoInitialize(NULL);
  cmbProduct->ItemIndex = -1;
  frmSelection->Hide();
//  connection_string_module = "Provider=SQLOLEDB.1;Password=test;Persist Security Info=True;User ID=test;Initial Catalog=KTB_FM;Data Source=NBCOMPUTER\\NBCOMPUTER2K";
  connection_string_module = "Provider=SQLOLEDB.1;";
  connection_string_module += "Password=fm_user2;";
  connection_string_module += "Persist Security Info=True;User ID=fm_user2;";
  connection_string_module += "Initial Catalog=KTB_FM;Data Source=DAC-DB2";
}
//---------------------------------------------------------------------------


void __fastcall TfrmSelection::cmbProductChange(TObject *Sender)
{
  switch(cmbProduct->ItemIndex){
    case 0:
      frmRegularInput->Show();
      frmSelection->Hide();
      break;
    case 1:
      frmReloadInput->Show();
      frmSelection->Hide();
      break;
    case 2:
      frmBTInput->Show();
      frmSelection->Hide();
      break;
  };
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelection::FormShow(TObject *Sender)
{
  cmbProduct->ItemIndex = -1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelection::btnReportGenClick(TObject *Sender)
{
  frmReportGen->Show();
  frmSelection->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelection::FormDestroy(TObject *Sender)
{
  CoUninitialize();
}
//---------------------------------------------------------------------------

