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
/*
// TCB production
 connectString = "Provider=IBMDADB2.1;Password=" + passWord +
                 ";Persist Security Info=True;User ID=" + userName +
                 ";Data Source=ESCORE;Location=10.0.31.71:50000;Extended Properties=\"\"";
// TCB testing
 connectString = "Provider=IBMDADB2.1;Password=" + passWord +
                 ";Persist Security Info=True;User ID=" + userName +
                 ";Data Source=ESCORE;Location=10.0.5.47:50000;Extended Properties=\"\"";
// local 
 connectString = "Provider=IBMDADB2.1;Password=" + passWord +
                 ";Persist Security Info=True;User ID=" + userName +
                 ";Data Source=SAMPLE;Location=192.168.1.20:50000;Extended Properties=\"\"";
*/
 connectString = "Provider=IBMDADB2.1;Password=" + passWord +
                 ";Persist Security Info=True;User ID=" + userName +
                 ";Data Source=ESCORE;Location=10.0.31.71:50000;Extended Properties=\"\"";

 try {
   Form1->dbhandle = new TADOHandler();
   Form1->dbhandle->OpenDatabase(connectString.c_str());
   Form1->ds = new TADODataSet(NULL);
   Form1->ds->EnableBCD = false;  // Decimal fields are mapped to float.

   displayInfo();
/*
   sqlStatement = " select fund_deposit_pct, fund_bank_pct, fund_self_pct, fund_free_pct, "
                  "   cof_deposit, cof_bank, roe, commission, npv_hurdle "
                  "  from tcb_fin_info;";

   Form1->dbhandle->ExecSQLQry(sqlStatement.c_str(), Form1->ds);
   record_count = Form1->ds->RecordCount;
   if (record_count != 1)
      throw DataEx("維護表格必須只有一筆資料。");
   Form1->ds->First();

    if (!Form1->ds->Eof) {
       if (! Form1->ds->FieldValues["fund_deposit_pct"].IsNull())
          Form1->fund_deposit_pct->Text = FloatToStrF(Form1->ds->FieldByName("fund_deposit_pct")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["fund_bank_pct"].IsNull())
          Form1->fund_bank_pct->Text =FloatToStrF(Form1->ds->FieldByName("fund_bank_pct")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["fund_self_pct"].IsNull())
          Form1->fund_self_pct->Text =FloatToStrF(Form1->ds->FieldByName("fund_self_pct")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["fund_free_pct"].IsNull())
          Form1->fund_free_pct->Text =FloatToStrF(Form1->ds->FieldByName("fund_free_pct")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["cof_deposit"].IsNull())
          Form1->cof_deposit->Text =FloatToStrF(Form1->ds->FieldByName("cof_deposit")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["cof_bank"].IsNull())
          Form1->cof_bank->Text =FloatToStrF(Form1->ds->FieldByName("cof_bank")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["roe"].IsNull())
          Form1->roe->Text = FloatToStrF(Form1->ds->FieldByName("roe")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["commission"].IsNull())
          Form1->commission->Text = FloatToStrF(Form1->ds->FieldByName("commission")->AsFloat, ffNumber, 8, 3);

       if (! Form1->ds->FieldValues["npv_hurdle"].IsNull())
          Form1->npv->Text = FloatToStrF(Form1->ds->FieldByName("npv_hurdle")->AsFloat, ffFixed, 8, 1);
    }
*/
 }
 catch (Exception &E) {
     Form1->message->Caption = E.Message;
     Form1->Refresh();
 }
 catch (...) {
    ShowMessage("連線失敗！請檢查使用者名稱和密碼。");
 }
 Form1->message->Caption = "  ";
 Form1->connect->Visible = false;
 Form1->Update->Visible = true;
 Form1->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------

void displayInfo()
{
   String sqlStatement;
   int record_count;

   sqlStatement = " select fund_deposit_pct, fund_bank_pct, fund_self_pct, fund_free_pct, "
                  "   cof_deposit, cof_bank, roe, commission, npv_hurdle "
                  "  from tcb_fin_info;";

   Form1->dbhandle->ExecSQLQry(sqlStatement.c_str(), Form1->ds);
   record_count = Form1->ds->RecordCount;
   if (record_count != 1)
      throw DataEx("維護表格必須只有一筆資料。");
   Form1->ds->First();
/*
  For a table column with data type decimal(p, s),
    Query->FieldValues["m1_recovery_ratio"] will return 0.
    Query->FieldByName("m1_recovery_ratio")->AsFloat will return correct value.
  Conclusion:
    For a column with data type char, varchar, you can get the value with
     Query->FieldValues["m1_recovery_ratio"].
    For a column with number data type, you'd better use
     Query->FieldByName("m1_recovery_ratio")->AsFloat to get its value.
*/
    if (!Form1->ds->Eof) {
       if (! Form1->ds->FieldValues["fund_deposit_pct"].IsNull())
          Form1->fund_deposit_pct->Text = FloatToStrF(Form1->ds->FieldByName("fund_deposit_pct")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["fund_bank_pct"].IsNull())
          Form1->fund_bank_pct->Text =FloatToStrF(Form1->ds->FieldByName("fund_bank_pct")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["fund_self_pct"].IsNull())
          Form1->fund_self_pct->Text =FloatToStrF(Form1->ds->FieldByName("fund_self_pct")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["fund_free_pct"].IsNull())
          Form1->fund_free_pct->Text =FloatToStrF(Form1->ds->FieldByName("fund_free_pct")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["cof_deposit"].IsNull())
          Form1->cof_deposit->Text =FloatToStrF(Form1->ds->FieldByName("cof_deposit")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["cof_bank"].IsNull())
          Form1->cof_bank->Text =FloatToStrF(Form1->ds->FieldByName("cof_bank")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["roe"].IsNull())
          Form1->roe->Text = FloatToStrF(Form1->ds->FieldByName("roe")->AsFloat, ffFixed, 8, 3);

       if (! Form1->ds->FieldValues["commission"].IsNull())
          Form1->commission->Text = FloatToStrF(Form1->ds->FieldByName("commission")->AsFloat, ffNumber, 8, 3);

       if (! Form1->ds->FieldValues["npv_hurdle"].IsNull())
          Form1->npv->Text = FloatToStrF(Form1->ds->FieldByName("npv_hurdle")->AsFloat, ffFixed, 8, 0);
    }
}
