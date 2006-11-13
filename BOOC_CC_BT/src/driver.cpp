//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <ADODB.hpp>
#include "driver.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
  Application->Terminate();  
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  char app_sn[12], idn[11], time_stamp[9], ole_db[256], error_msg[256];
  int status;
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM DRIVER ORDER BY IDN;";
  CoInitialize(NULL);
  TADOConnection *connection = new TADOConnection(NULL);
  TADOQuery *query = new TADOQuery(NULL);

  connection->Close();
  connection->ConnectionString = edtConnectionString->Text;
  connection->Open();
  strcpy(ole_db, edtConnectionString->Text.c_str());

  query->Close();
  query->Connection = connection;
  query->CommandTimeout = 3000;
  query->EnableBCD = false;

  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Open();
  query->First();
  while(!query->Eof){
    strcpy(app_sn, query->FieldByName("APP_SN")->AsString.c_str());
    strcpy(idn, query->FieldByName("IDN")->AsString.c_str());
    strcpy(time_stamp, query->FieldByName("Inquiry_Date")->AsString.c_str());
    int gender = query->FieldValues["gender"];
    int product_type = (cmbProduct->ItemIndex + 1); //1 for credit card; 2 for BT
    status = BOOC_CC_BT_AM(app_sn, idn, time_stamp, product_type,
                           "010", ole_db, error_msg);

//          status = BOOC_CC_BT_AM(app_sn, idn,
//                                 product_type, "010", ole_db, error_msg);

    if(status < -1) {
      Application->MessageBox(error_msg, app_sn, MB_OK);
      break;
    };
    query->Next();
  };
  query->Close();
  connection->Close();
  delete connection;
  delete query;
  CoUninitialize();
}
//---------------------------------------------------------------------------

