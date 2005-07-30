//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//#include "dac_ploan.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
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
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
/*  TADOConnection *ADOConnection1 = TaipeiBank->ADOConnection1;

  try {
     //Disconnect to local databases
     ADOConnection1->Connected=false;
  }
  catch(Exception &E) {
     ShowMessage(AnsiString(E.ClassName())+ E.Message);
  }*/
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ValidateClick(TObject *Sender)
{
  TADOQuery *Query = ADOQuery1;
  TADOQuery *QueryW = ADOQuery2;
  TADOHandler *dbhandle;             // pass connection object to function

  AnsiString sql_stmt, sql_stmt2;
  AnsiString app_sn, app_date, ts_date, jcic_date;
  char errMsg[257];
  char ole_str[256];
  AnsiString oledbString;
  int status = -1;
  char * CurrDateTime ();

  CoInitialize(NULL);
  ADOConnection1->ConnectionTimeout = 30; // 30 seconds
  ADOConnection1->CommandTimeout = 300;  //5 minutes; 20 minutes for AnShin

  // Connect to Local DB
  try {
    if (ADOConnection1->Connected == false)
      ADOConnection1->Open();

    Start_time->Caption = CurrDateTime();
    // Empty Validation Result table
    sql_stmt = "delete from test_out;";
    Query->Close();
    Query->SQL->Clear();
    Query->SQL->Add(sql_stmt);
    Query->ExecSQL();

    // get data from input table, call validation function,
    // and write result to validation_result table.
    oledbString = Edit1->Text;
    strcpy(ole_str, oledbString.c_str());
    dbhandle = new TADOHandler();     // pass connection object to function
    dbhandle->OpenDatabase(ole_str);  // pass connection object to function

    sql_stmt = "select app_sn, app_date, ts_date, jcic_date from test_in order by app_sn";
    Query->Close();
    Query->SQL->Clear();
    Query->SQL->Add(sql_stmt);
    Query->Open();
    Query->First();
    int seq = 0;
    while (!Query->Eof) {
       app_sn = Query->FieldValues["app_sn"];
       app_date = Query->FieldValues["app_date"];
       ts_date = Query->FieldValues["ts_date"];
       jcic_date = Query->FieldValues["jcic_date"];

       Label2->Caption = app_sn;
       Label3->Caption = app_date;
       Label4->Caption = jcic_date;
       count->Caption = ++seq;
       Form1->Refresh();

       *errMsg = '\0';
       // call the validation function
       status = optimal_cal_conn(app_sn.c_str(), ts_date.c_str(), jcic_date.c_str(), app_date.c_str(),
                            1, ole_str, errMsg, dbhandle);

          if (status < 0) {
             sql_stmt2 = "insert into test_out (idn, return_msg) values ";
             sql_stmt2 += "(:app_sn, :errMsg); ";
             QueryW->Close();
             QueryW->SQL->Clear();
             QueryW->SQL->Add(sql_stmt2);
             QueryW->Parameters->ParamValues["app_sn"] = app_sn;
             QueryW->Parameters->ParamValues["errMsg"] = errMsg;
             QueryW->ExecSQL();
          }

       Query->Next();
    }
    End_time->Caption = CurrDateTime();
  }
  catch (Exception &E) {
     ShowMessage(AnsiString(E.ClassName())+ E.Message);
  }

  dbhandle->CloseDatabase();    // pass connection object to function
  delete  dbhandle;             // pass connection object to function
  CoUninitialize();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ValidateNoConnClick(TObject *Sender)
{
  TADOQuery *Query = ADOQuery1;
  TADOQuery *QueryW = ADOQuery2;
//  TADOHandler *dbhandle;             // pass connection object to function

  CoInitialize(NULL);
  AnsiString sql_stmt, sql_stmt2;
  AnsiString app_sn, app_date, ts_date, jcic_date;
  char errMsg[257];
  char ole_str[256];
  AnsiString oledbString;
  int status = -1;
  char * CurrDateTime ();

  ADOConnection1->ConnectionTimeout = 30; // 30 seconds
  ADOConnection1->CommandTimeout = 300;  //5 minutes; 20 minutes for AnShin

  // Connect to Local DB
  try {
    if (ADOConnection1->Connected == false)
      ADOConnection1->Open();

    Start_time->Caption = CurrDateTime();
    // Empty Validation Result table
    sql_stmt = "delete from test_out;";
    Query->Close();
    Query->SQL->Clear();
    Query->SQL->Add(sql_stmt);
    Query->ExecSQL();

    // get data from input table, call validation function,
    // and write result to validation_result table.
    oledbString = Edit1->Text;
    strcpy(ole_str, oledbString.c_str());
//    dbhandle = new TADOHandler();     // pass connection object to function
//    dbhandle->OpenDatabase(ole_str);  // pass connection object to function

    sql_stmt = "select app_sn, app_date, ts_date, jcic_date from test_in order by app_sn";
    Query->Close();
    Query->SQL->Clear();
    Query->SQL->Add(sql_stmt);
    Query->Open();
    Query->First();
    int seq = 0;
    while (!Query->Eof) {
       app_sn = Query->FieldValues["app_sn"];
       app_date = Query->FieldValues["app_date"];
       ts_date = Query->FieldValues["ts_date"];
       jcic_date = Query->FieldValues["jcic_date"];

       Label2->Caption = app_sn;
       Label3->Caption = app_date;
       Label4->Caption = jcic_date;
       count->Caption = ++seq;
       Form1->Refresh();

       *errMsg = '\0';
       // call the validation function
       status = optimal_cal(app_sn.c_str(), ts_date.c_str(), jcic_date.c_str(), app_date.c_str(),
                            1, ole_str, errMsg);

          if (status < 0) {
             sql_stmt2 = "insert into test_out (idn, return_msg) values ";
             sql_stmt2 += "(:app_sn, :errMsg); ";
             QueryW->Close();
             QueryW->SQL->Clear();
             QueryW->SQL->Add(sql_stmt2);
             QueryW->Parameters->ParamValues["app_sn"] = app_sn;
             QueryW->Parameters->ParamValues["errMsg"] = errMsg;
             QueryW->ExecSQL();
          }

       Query->Next();
    }
    End_time->Caption = CurrDateTime();
  }
  catch (Exception &E) {
     ShowMessage(AnsiString(E.ClassName())+ E.Message);
  }

//  dbhandle->CloseDatabase();    // pass connection object to function
//  delete  dbhandle;             // pass connection object to function
  CoUninitialize();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
  ADOConnection1->ConnectionTimeout = 30; // 30 seconds
  ADOConnection1->CommandTimeout = 300;  //5 minutes; 20 minutes for AnShin

  if (CheckBox1->Checked){
     try {
        //Connect to local databases
        ADOConnection1->Connected=true;
     }
     catch(Exception &E) {
        ShowMessage(AnsiString(E.ClassName())+ E.Message);
     }
   }
   else {
     try {
        //DisConnect to local databases
        ADOConnection1->Connected=false;
     }
     catch(Exception &E) {
        ShowMessage(AnsiString(E.ClassName())+ E.Message);
     }
    }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::UnitTestClick(TObject *Sender)
{
  TADOQuery *QueryW = ADOQuery2;
  AnsiString sql_stmt, sql_stmt2;
  AnsiString app_sn, app_date, ts_date, jcic_date;
  int tsn=1;
  char errMsg[257];
  char ole_str[256];
  AnsiString oledbString;
  int status;

  CoInitialize(NULL);
  ADOConnection1->ConnectionTimeout = 30; // 30 seconds
  ADOConnection1->CommandTimeout = 300;  //5 minutes

  // Connect to Local DB
  // get data from input table, call validation function,
  // and write result to validation_result table.
  try {
    if (ADOConnection1->Connected == false)
      ADOConnection1->Open();

    oledbString = Edit1->Text;
    strcpy(ole_str, oledbString.c_str());
    app_sn = Edit2->Text;
    app_date = Edit3->Text;
    jcic_date = Edit4->Text;
    ts_date = app_date;
    *errMsg = '\0';

    status = optimal_cal(app_sn.c_str(), ts_date.c_str(), jcic_date.c_str(), app_date.c_str(),
                          tsn, ole_str, errMsg);

    Label2->Caption = Edit2->Text;
    Label3->Caption = Edit3->Text;
    Label4->Caption = Edit4->Text;
    Form1->Refresh();

    if (status < 0) {
       sql_stmt2 = "insert into test_out (idn, return_msg) values ";
       sql_stmt2 += "(:app_sn, :errMsg); ";
       QueryW->Close();
       QueryW->SQL->Clear();
       QueryW->SQL->Add(sql_stmt2);
       QueryW->Parameters->ParamValues["app_sn"] = app_sn;
       QueryW->Parameters->ParamValues["errMsg"] = errMsg;
       QueryW->ExecSQL();
    }
    ADOConnection1->Connected=false;
  }
  catch (Exception &E) {
     ShowMessage(AnsiString(E.ClassName())+ E.Message);
  }
  CoUninitialize();

}
//---------------------------------------------------------------------------
char * CurrDateTime ()
{
 time_t timer;
 struct tm *tblock;
 static char buf[20];

 timer = time(NULL);
 tblock = localtime(&timer);
 sprintf (buf, "%04d/%02d/%02d %02d:%02d:%02d", tblock->tm_year+1900, tblock->tm_mon+1,
          tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
 return (buf);
}
void __fastcall TForm1::Button1Click(TObject *Sender)
{
 Application->Terminate();
}
//---------------------------------------------------------------------------




