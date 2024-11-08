//--- -----------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "update.h"
#include "grant.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//#define CONNECTION_STRING  "AdoConnect.udl"
#define CONNECTION_STRING_PROD  "Provider=IBMDADB2.1;Password=escorap1;Persist Security Info=True;User ID=escorap1;Data Source=ESCORE;Location=10.0.31.71:50000;Extended Properties=\"\""
#define CONNECTION_STRING_TEST  "Provider=IBMDADB2.1;Password=escorap1;Persist Security Info=True;User ID=escorap1;Data Source=ESCORE;Location=10.0.5.47:50000;Extended Properties=\"\""
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
enum SQLCodes { Get_Maintenance_Record, Update_Fin_Info};

char *SQLCommands[] = {
/* Get_Maintenance_Record*/
" select fund_deposit_pct, fund_bank_pct, fund_self_pct, fund_free_pct, "
"   cof_deposit, cof_bank, roe, commission, npv_hurdle "
"  from tcb_fin_info;" ,

/* Update_Fin_Info*/
"update tcb_fin_info "
"   set fund_deposit_pct = %s,"
"       fund_bank_pct = %s,"
"       fund_self_pct = %s,"
"       fund_free_pct = %s,"
"       cof_deposit = %s,"
"       cof_bank = %s,"
"       roe = %s,"
"       commission = %s,"
"       npv_hurdle = %s;"

};

void __fastcall TForm1::EndClick(TObject *Sender)
{
//  delete ds;
//  dbhandle->CloseDatabase();
//  delete dbhandle;

  Application->Terminate();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::UpdateClick(TObject *Sender)
{
 Variant hostVars[10];
// float deposit_pct, bank_pct, self_pct, free_pct;
// float deposit_cof, bank_cof, roe_rate, comm_rate;
 char  cmd[1024];

 message->Caption = "  ";
 Form1->Refresh();
 try {
    sprintf (cmd, SQLCommands[Update_Fin_Info], fund_deposit_pct->Text.c_str(),
             fund_bank_pct->Text.c_str(), fund_self_pct->Text.c_str(), fund_free_pct->Text.c_str(),
             cof_deposit->Text.c_str(), cof_bank->Text.c_str(), roe->Text.c_str(), commission->Text.c_str(),
             npv->Text.c_str());
    dbhandle->ExecSQLCmd(cmd);

    message->Caption = "維護作業更新完成！";
    displayInfo();
 } catch (Exception &E) {
     message->Caption = E.Message;
     Form1->Refresh();
//     delete ds;
//     dbhandle->CloseDatabase();
//     delete dbhandle;
 }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::connectClick(TObject *Sender)
{
 Form2->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::disconnectClick(TObject *Sender)
{
  dbhandle->CloseDatabase();
  fund_deposit_pct->Text = "";
  fund_bank_pct->Text ="";
  fund_self_pct->Text ="";
  fund_free_pct->Text ="";
  cof_deposit->Text ="";
  cof_bank->Text ="";
  roe->Text = "";
  commission->Text ="";
}
//---------------------------------------------------------------------------

