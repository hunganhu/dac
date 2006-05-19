//--- -----------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "update.h"
#include "grant.h"
#include "dirui.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//#define CONNECTION_STRING  "AdoConnect.udl"
#define CONNECTION_STRING_PROD  "Provider=IBMDADB2.1;Password=escorap1;Persist Security Info=True;User ID=escorap1;Data Source=ESCORE;Location=10.0.31.71:50000;Extended Properties=\"\""
#define CONNECTION_STRING_TEST  "Provider=IBMDADB2.1;Password=escorap1;Persist Security Info=True;User ID=escorap1;Data Source=ESCORE;Location=10.0.5.47:50000;Extended Properties=\"\""
AnsiString working_dir;
AnsiString report_dir;
AnsiString return_msg;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
enum SQLCodes {Approve_and_Appropriate, 
               Decline_and_Appropriate,
               Manual_and_Appropriate,
               Approve_But_Decline,
               Approve_and_Unclose
              };

char *SQLCommands[] = {
/* Approve_and_Appropriate*/
" select a.system_date, a.case_sn, a.applicant_ID, a.applicant_name, a.branch, c.branch_name,"
"        a.app_amt, a.apr_1, a.apr_2, a.apr_3, a.loan_amt, a.account_no, a.open_date"
" from app_result b, app_info a left join branch c"
"   on a.branch = c.branch"
" where a.case_sn = b.case_sn"
"   and b.approval_code = 1"
"   and a.approval_final = 2"
"   and a.account_no is not null"
" order by a.case_sn;",

/* Decline_and_Appropriate*/
" select a.system_date, a.case_sn, a.applicant_ID, a.applicant_name, a.branch, c.branch_name, a.app_amt, a.apr_1, a.apr_2, a.apr_3, a.loan_amt, a.account_no, a.open_date, b.approval_msg"
" from app_result b, app_info a left join branch c"
"   on a.branch = c.branch"
" where a.case_sn = b.case_sn"
"   and b.approval_code not in (1, 3, 4, 5, 3104, 3110)"
"   and a.approval_final = 2"
"   and a.account_no is not null"
" order by a.case_sn;",

/* Manual_and_Appropriate*/
" select a.system_date, a.case_sn, a.applicant_ID, a.applicant_name, a.branch, c.branch_name, a.app_amt, a.apr_1, a.apr_2, a.apr_3, a.loan_amt, a.account_no, a.open_date"
" from app_result b, app_info a left join branch c"
"   on a.branch = c.branch"
" where a.case_sn = b.case_sn"
"   and b.approval_code in (3, 4, 5, 3104, 3110)"
"   and a.approval_final = 2"
"   and a.account_no is not null"
" order by a.case_sn;",

/* Approve_But_Decline*/
" select a.system_date, a.case_sn, a.applicant_ID, a.applicant_name, a.branch, c.branch_name, a.app_amt"
" from app_result b, app_info a left join branch c"
"   on a.branch = c.branch"
" where a.case_sn = b.case_sn"
"   and b.approval_code = 1"
"   and a.approval_final = 0"
" order by a.case_sn;",

/* Approve_and_Unclose*/
" select a.system_date, a.case_sn, a.applicant_ID, a.applicant_name, a.branch, c.branch_name, a.app_amt"
" from app_result b, app_info a left join branch c"
"   on a.branch = c.branch"
" where a.case_sn = b.case_sn"
"   and b.approval_code = 1"
"   and a.approval_final is null"
" order by a.case_sn;"

};

void __fastcall TForm1::EndClick(TObject *Sender)
{
//  delete ds;
//  dbhandle->CloseDatabase();
//  delete dbhandle;

  Application->Terminate();
}
//---------------------------------------------------------------------------

bool Generate_ApprovePay_Report(TADOHandler *dbhandle, TADODataSet *ds)
{
  AnsiString file_name = report_dir + "Approve_and_Appropriate.csv";
  ofstream report_total(file_name.c_str(), ios_base::out);

  AnsiString sql_stmt;

  try {
     dbhandle->ExecSQLQry(SQLCommands[Approve_and_Appropriate], ds);
     ds->First();
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
    if (report_total) {
       report_total << "申貸日,申貸編號,申貸人ID,申請人姓名,分行編號,分行名,"
                       "申貸金額,APR_1,APR_2,APR_3,撥款金額,撥款帳號,撥貸日" << endl;
       while (!ds->Eof) {
          if (! ds->FieldValues["system_date"].IsNull())
             report_total << ds->FieldByName("system_date")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["case_sn"].IsNull())
             report_total << ds->FieldByName("case_sn")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_ID"].IsNull())
             report_total << ds->FieldByName("applicant_ID")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_name"].IsNull())
             report_total << ds->FieldByName("applicant_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch"].IsNull())
             report_total << ds->FieldByName("branch")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch_name"].IsNull())
             report_total << ds->FieldByName("branch_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["app_amt"].IsNull())
             report_total << ds->FieldByName("app_amt")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["apr_1"].IsNull())
             report_total << ds->FieldByName("apr_1")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["apr_2"].IsNull())
             report_total << ds->FieldByName("apr_2")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["apr_3"].IsNull())
             report_total << ds->FieldByName("apr_3")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["loan_amt"].IsNull())
             report_total << ds->FieldByName("loan_amt")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["account_no"].IsNull())
             report_total << ds->FieldByName("account_no")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["open_date"].IsNull())
             report_total << ds->FieldByName("open_date")->AsString.c_str() <<endl;
          else
             report_total <<endl;

          ds->Next();
       }
       report_total.clear();
       report_total.close();
       ds->Close();
       return_msg += "資料匯出至 " + file_name +"\n";
       return true;
    }
    else {
      return_msg += "檔案 " + file_name + " 開啟失敗";
      return false;
    }
 }
 catch (Exception &E) {
     return_msg += E.Message;
     return false;
 }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::connectClick(TObject *Sender)
{
 Form2->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  message->Caption = "";
  report_dir = "";
  char working_drive;
  char working[2048];
  int drive = getdisk();
  working_drive = 'A' + drive;
  getcurdir(0, working);
  working_dir = working;
  working_dir = static_cast<AnsiString>(working_drive) + ":\\" + working_dir;
  if(working_dir.Length()>3)
    working_dir += "\\";

  char buffer[2048];
  ifstream config((working_dir + "config.cfg").c_str());
  if(config){
    config.getline(buffer,2048);
    report_dir = buffer;
  };
  config.clear();
  config.close();

}
//---------------------------------------------------------------------------

bool Generate_ApproveDecline_Report(TADOHandler *dbhandle, TADODataSet *ds)
{
  AnsiString file_name = report_dir + "Approve_But_Decline.csv";
  ofstream report_total(file_name.c_str(), ios_base::out);

  AnsiString sql_stmt;

  try {
    dbhandle->ExecSQLQry(SQLCommands[Approve_But_Decline], ds);
    ds->First();

    if (report_total) {
       report_total << "申貸日,申貸編號,申貸人ID,申請人姓名,分行編號,分行名,申貸金額" << endl;
       while (!ds->Eof) {
          if (! ds->FieldValues["system_date"].IsNull())
             report_total << ds->FieldByName("system_date")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["case_sn"].IsNull())
             report_total << ds->FieldByName("case_sn")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_ID"].IsNull())
             report_total << ds->FieldByName("applicant_ID")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_name"].IsNull())
             report_total << ds->FieldByName("applicant_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch"].IsNull())
             report_total << ds->FieldByName("branch")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch_name"].IsNull())
             report_total << ds->FieldByName("branch_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["app_amt"].IsNull())
             report_total << ds->FieldByName("app_amt")->AsString.c_str() <<endl;
          else
             report_total <<endl;

          ds->Next();
       }
       report_total.clear();
       report_total.close();
       ds->Close();
       return_msg += "資料匯出至 " + file_name +"\n";
       return true;
    }
    else {
       return_msg += "檔案 " + file_name + " 開啟失敗";
       return false;
    }
 }
 catch (Exception &E) {
     return_msg += E.Message;
     return false;
 }
}
//---------------------------------------------------------------------------

bool Generate_ApproveUnclose_Report(TADOHandler *dbhandle, TADODataSet *ds)
{
  AnsiString file_name = report_dir + "Approve_and_Unclose.csv";
  ofstream report_total(file_name.c_str(), ios_base::out);

  AnsiString sql_stmt;

  try {
     dbhandle->ExecSQLQry(SQLCommands[Approve_and_Unclose], ds);
     ds->First();

    if (report_total) {
       report_total << "申貸日,申貸編號,申貸人ID,申請人姓名,分行編號,分行名,申貸金額" << endl;
       while (!ds->Eof) {
          if (! ds->FieldValues["system_date"].IsNull())
             report_total << ds->FieldByName("system_date")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["case_sn"].IsNull())
             report_total << ds->FieldByName("case_sn")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_ID"].IsNull())
             report_total << ds->FieldByName("applicant_ID")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_name"].IsNull())
             report_total << ds->FieldByName("applicant_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch"].IsNull())
             report_total << ds->FieldByName("branch")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch_name"].IsNull())
             report_total << ds->FieldByName("branch_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["app_amt"].IsNull())
             report_total << ds->FieldByName("app_amt")->AsString.c_str() <<endl;
          else
             report_total <<endl;

          ds->Next();
       }
       report_total.clear();
       report_total.close();
       ds->Close();
       return_msg += "資料匯出至 " + file_name +"\n";
       return true;
    }
    else {
      return_msg += "檔案 " + file_name + " 開啟失敗";
      return false;
    }
 }
 catch (Exception &E) {
     return_msg += E.Message;
     return false;
 }

}
//---------------------------------------------------------------------------

bool Generate_DeclinePay_Report(TADOHandler *dbhandle, TADODataSet *ds)
{
  AnsiString file_name = report_dir + "Decline_and_Appropriate.csv";
  ofstream report_total(file_name.c_str(), ios_base::out);

  AnsiString sql_stmt;

  try {
    dbhandle->ExecSQLQry(SQLCommands[Decline_and_Appropriate], ds);
    ds->First();

    if (report_total) {
       report_total << "申貸日,申貸編號,申貸人ID,申請人姓名,分行編號,分行名,"
                       "申貸金額,APR_1,APR_2,APR_3,撥款金額,撥款帳號,撥貸日" << endl;
       while (!ds->Eof) {
          if (! ds->FieldValues["system_date"].IsNull())
             report_total << ds->FieldByName("system_date")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["case_sn"].IsNull())
             report_total << ds->FieldByName("case_sn")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_ID"].IsNull())
             report_total << ds->FieldByName("applicant_ID")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_name"].IsNull())
             report_total << ds->FieldByName("applicant_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch"].IsNull())
             report_total << ds->FieldByName("branch")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch_name"].IsNull())
             report_total << ds->FieldByName("branch_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["app_amt"].IsNull())
             report_total << ds->FieldByName("app_amt")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["apr_1"].IsNull())
             report_total << ds->FieldByName("apr_1")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["apr_2"].IsNull())
             report_total << ds->FieldByName("apr_2")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["apr_3"].IsNull())
             report_total << ds->FieldByName("apr_3")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["loan_amt"].IsNull())
             report_total << ds->FieldByName("loan_amt")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["account_no"].IsNull())
             report_total << ds->FieldByName("account_no")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["open_date"].IsNull())
             report_total << ds->FieldByName("open_date")->AsString.c_str() <<endl;
          else
             report_total <<endl;

          ds->Next();
       }
       report_total.clear();
       report_total.close();
       ds->Close();
       return_msg += "資料匯出至 " + file_name +"\n";
       return true;
    }
    else {
      return_msg += "檔案 " + file_name + " 開啟失敗";
      return false;
    }
 }
 catch (Exception &E) {
     return_msg += E.Message;
     return false;
 }

}
//---------------------------------------------------------------------------

bool Generate_ManualPay_Report(TADOHandler *dbhandle, TADODataSet *ds)
{
  AnsiString file_name = report_dir + "Manual_and_Appropriate.csv";
  ofstream report_total(file_name.c_str(), ios_base::out);

  AnsiString sql_stmt;

  try {
     dbhandle->ExecSQLQry(SQLCommands[Manual_and_Appropriate], ds);
     ds->First();
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
    if (report_total) {
       report_total << "申貸日,申貸編號,申貸人ID,申請人姓名,分行編號,分行名,"
                       "申貸金額,APR_1,APR_2,APR_3,撥款金額,撥款帳號,撥貸日" << endl;
       while (!ds->Eof) {
          if (! ds->FieldValues["system_date"].IsNull())
             report_total << ds->FieldByName("system_date")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["case_sn"].IsNull())
             report_total << ds->FieldByName("case_sn")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_ID"].IsNull())
             report_total << ds->FieldByName("applicant_ID")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["applicant_name"].IsNull())
             report_total << ds->FieldByName("applicant_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch"].IsNull())
             report_total << ds->FieldByName("branch")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["branch_name"].IsNull())
             report_total << ds->FieldByName("branch_name")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["app_amt"].IsNull())
             report_total << ds->FieldByName("app_amt")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["apr_1"].IsNull())
             report_total << ds->FieldByName("apr_1")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["apr_2"].IsNull())
             report_total << ds->FieldByName("apr_2")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["apr_3"].IsNull())
             report_total << ds->FieldByName("apr_3")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["loan_amt"].IsNull())
             report_total << ds->FieldByName("loan_amt")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["account_no"].IsNull())
             report_total << ds->FieldByName("account_no")->AsString.c_str() <<",";
          else
             report_total <<",";

          if (! ds->FieldValues["open_date"].IsNull())
             report_total << ds->FieldByName("open_date")->AsString.c_str() <<endl;
          else
             report_total <<endl;

          ds->Next();
       }
       report_total.clear();
       report_total.close();
       ds->Close();
       return_msg += "資料匯出至 " + file_name +"\n";
       return true;
    }
    else {
      return_msg += "檔案 " + file_name + " 開啟失敗";
      return false;
    }
 }
 catch (Exception &E) {
     return_msg += E.Message;
     return false;
 }

}
//---------------------------------------------------------------------------

bool Generate_All_Report(TADOHandler *dbhandle, TADODataSet *ds)
{
return true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OutputDirClick(TObject *Sender)
{
   Application->CreateForm(__classid(TfrmDirUI), &frmDirUI);
   frmDirUI->dlbReport->Directory = report_dir;
   frmDirUI->edtDir->Text = report_dir;
   frmDirUI->ShowModal();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ApprovePayClick(TObject *Sender)
{
//  AnsiString dir_message = (report_dir.Length() == 0) ? static_cast<AnsiString>("程式所在目錄") : report_dir;
//  dir_message += "\n";
//  return_msg = "報表將會儲存在: " + dir_message;
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if(config){
    config << report_dir.c_str() << endl;
    config.clear();
    config.close();
  };
  return_msg = "";
  Generate_ApprovePay_Report(dbhandle, ds);
  message->Caption = return_msg;
  Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DeclinePayClick(TObject *Sender)
{
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if(config){
    config << report_dir.c_str() << endl;
    config.clear();
    config.close();
  };
  return_msg = "";
  Generate_DeclinePay_Report(dbhandle, ds);
  message->Caption = return_msg;
  Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ManualPayClick(TObject *Sender)
{
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if(config){
    config << report_dir.c_str() << endl;
    config.clear();
    config.close();
  };
  return_msg = "";
  Generate_ManualPay_Report(dbhandle, ds);
  message->Caption = return_msg;
  Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ApproveUncloseClick(TObject *Sender)
{
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if(config){
    config << report_dir.c_str() << endl;
    config.clear();
    config.close();
  };
  return_msg = "";
  Generate_ApproveUnclose_Report(dbhandle, ds);
  message->Caption = return_msg;
  Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ApproveDeclineClick(TObject *Sender)
{
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if(config){
    config << report_dir.c_str() << endl;
    config.clear();
    config.close();
  };
  return_msg = "";
  Generate_ApproveDecline_Report(dbhandle, ds);
  message->Caption = return_msg;
  Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AllClick(TObject *Sender)
{
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if(config){
    config << report_dir.c_str() << endl;
    config.clear();
    config.close();
  };
  return_msg = "";
  Generate_ApprovePay_Report(dbhandle, ds);
  Generate_ApproveDecline_Report(dbhandle, ds);
  Generate_ApproveUnclose_Report(dbhandle, ds);
  Generate_DeclinePay_Report(dbhandle, ds);
  Generate_ManualPay_Report(dbhandle, ds);
  message->Caption = return_msg;
  Refresh();
}
//---------------------------------------------------------------------------

