//---------------------------------------------------------------------------

#include <dir.h>
#include <vcl.h>

#pragma hdrstop

#include "ui.h"
#include "dm.h"
#include "dirset.h"
AnsiString report_dir;
AnsiString working_dir;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
int err_no_drop = -204;
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
  AnsiString dir_message = (report_dir.Length() == 0) ? static_cast<AnsiString>("程式所在目錄") : report_dir;
  dir_message += "\n";
  AnsiString message = "報表將會儲存在: " + dir_message;
  lblMessage->Caption = message;
  lblMessage->Refresh();
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if(config){
//    config << edtDBServer->Text.c_str() << endl;
//    config << edtDB->Text.c_str() << endl;
    config << report_dir.c_str() << endl;
    config.clear();
    config.close();
  }

  AnsiString ConnectionString;
//  AnsiString DBServer = "ESCORE";
  AnsiString DBServer = "TCBSCORE";
//  AnsiString DB = edtDB->Text;
  AnsiString account = edtAccount->Text;
//  AnsiString location = "10.0.31.71:50000"; //Testing:10.0.5.47:50000; production should use:10.0.31.71:50000 Local:"\"\"";
//  AnsiString location = "10.0.5.47:50000"; //Testing:10.0.5.47:50000; production should use:10.0.31.71:50000 Local:"\"\"";
  AnsiString location = "192.168.1.13:50000"; //local test
  AnsiString ExtendedProperties = "\"\"";
  AnsiString passowrd = edtPassword->Text;
  ConnectionString = "Provider=IBMDADB2.1;Password=" + passowrd;
  ConnectionString += ";Persist Security Info=True;User ID=" + account;
//  ConnectionString += ";Initial Catalog=" + DB;
  ConnectionString += ";Data Source=" + DBServer;
  ConnectionString += ";Location=" + location;
  ConnectionString += ";Extended Properties=" + ExtendedProperties;
  data->connection->Close();
  data->connection->ConnectionString = ConnectionString;
  data->connection->ConnectionTimeout = 30;
  data->connection->LoginPrompt = false;
  data->command->Connection = data->connection;
  data->query->Connection = data->connection;
  data->query->CommandTimeout = 300;
  data->command->CommandTimeout = 300;
  data->query->EnableBCD = false;
  AnsiString error_message="";

  try{
    data->connection->Open();
    if((!valid_date(edtBeginDate->Text)) ||
       (!valid_date(edtEndDate->Text)) ||
       (edtEndDate->Text < edtBeginDate->Text)){
       message += "報表起迄日期錯誤。";
       message += "\n";
       lblMessage->Caption = message;
       lblMessage->Refresh();
    }
    else{
      tm *today;
      time_t ltime;
      ltime = time( NULL );
      today = localtime( &ltime );
      int year = today->tm_year + 1900 - 1911; //民國年
      int month = today->tm_mon + 1;
      int day = today->tm_mday;
      AnsiString str_year = static_cast<AnsiString>(year);
      AnsiString str_month = static_cast<AnsiString>(month);
      AnsiString str_day = static_cast<AnsiString>(day);
      str_year = str_year.Length() == 2 ? "0" + str_year : str_year;
      str_month = str_month.Length() == 1 ? "0" + str_month : str_month;
      str_day = str_day.Length() == 1 ? "0" + str_day : str_day;
      AnsiString report_date = str_year + str_month + str_day;

      AnsiString dimension1, dimension2;
      unsigned int dimension1_type, dimension2_type;
      dimension1 = get_dimension(lblDimension1->Caption, dimension1_type);
      dimension2 = get_dimension(lblDimension2->Caption, dimension2_type);
//      int product_code = rgProduct->ItemIndex + 1;

      prepare_report(data->command, data->query,
                     edtBeginDate->Text, edtEndDate->Text,
                     dimension1, dimension2);
      bool success =
          generate_report(data->command, data->query, report_date,
                          edtBeginDate->Text, edtEndDate->Text,
                          lblDimension1->Caption, lblDimension2->Caption,
                          dimension1, dimension2,
                          dimension1_type, dimension2_type,
                          report_dir, error_message);
      if(!success){
        message += error_message;
        message += "\n";
        message += "程式執行錯誤。";
        message += "\n";
      }
      else{
        message += "報表已成功產生。";
        message += "\n";
      }
      lblMessage->Caption = message;
      lblMessage->Refresh();
    }
  }
  catch(Exception &E){
    if(static_cast<AnsiString>(E.ClassName()) == "EConvertError"){
      message += "報表起迄日期格式錯誤。正確格式:YYYYMMDD";
      message += "\n";
      lblMessage->Caption = message;
      edtBeginDate->Text = "";
      edtEndDate->Text = "";
      edtBeginDate->Focused();
    }
    else{
      message += E.Message;
      message += "\n";
      lblMessage->Caption = message;
      lblMessage->Refresh();
//      edtDB->Text = "";
//      edtDBServer->Text = "";
      edtAccount->Text = "";
      edtPassword->Text = "";
//      edtDBServer->Focused();
    }
  }
  data->connection->Close();
}
//---------------------------------------------------------------------------
bool valid_date(const AnsiString &date)
{
  int year = date.SubString(1,3).ToInt();
  int month = date.SubString(4,2).ToInt();
  int day = date.SubString(6,2).ToInt();
  if((day > 31) || (month > 12) || (day <= 0) || (month <=0) || (year <=0))
    return false;
  if((month == 2) && (day == 29)){
    if(year % 4 != 0)
      return false;
    if((year % 100 == 0) && (year% 400 != 0))
      return false;
  }
  return true;
}

void write_header(ofstream &report, const AnsiString &report_date,
                  const AnsiString &begin_date, const AnsiString &end_date,
                  const AnsiString &dimension_one,
                  const AnsiString &dimension_two,
                  const AnsiString &dimension_one_content,
                  const AnsiString &dimension_two_content)
{
  AnsiString formatted_begin_date;
  AnsiString formatted_end_date;
  AnsiString formatted_report_date;
  AnsiString product = "";

  formatted_begin_date = begin_date.SubString(1,3) + "/" + begin_date.SubString(4,2) + "/" + begin_date.SubString(6,2);
  formatted_end_date = end_date.SubString(1,3) + "/" + end_date.SubString(4,2) + "/" + end_date.SubString(6,2);
  formatted_report_date = report_date.SubString(1,3) + "/" + report_date.SubString(4,2) + "/" + report_date.SubString(6,2);
  report << "合作金庫銀行個人信用貸款管理報表"<< endl;
  report << "報表產生日期: "  << formatted_report_date.c_str() << endl;
  report << "報表涵蓋期間: " << formatted_begin_date.c_str() << " - " << formatted_end_date.c_str() << endl;
  AnsiString dimension_one_description, dimension_two_description;
  if(dimension_one == "貸款金額"){
    if(dimension_one_content.Trim() == "1")
      dimension_one_description = "新台幣40萬元以下";
    else if(dimension_one_content.Trim() == "2")
      dimension_one_description = "新台幣40萬元 - 50萬元";
    else if(dimension_one_content.Trim() == "3")
      dimension_one_description = "新台幣50萬元 - 100萬元";
    else if(dimension_one_content.Trim() == "4")
      dimension_one_description = "新台幣100萬元以上";
  }
  else if(dimension_one == "產品"){
    if(dimension_one_content.Trim() == "1")
      dimension_one_description = "簡易小額信貸";
    else if(dimension_one_content.Trim() == "2")
      dimension_one_description = "公教人員貸款";
    else if(dimension_one_content.Trim() == "3")
      dimension_one_description = "U利貸款";
    else if(dimension_one_content.Trim() == "4")
      dimension_one_description = "其他";
    else if(dimension_one_content.Trim() == "5")
      dimension_one_description = "農家樂貸款";
    else if(dimension_one_content.Trim() == "6")
      dimension_one_description = "房貸";
  }
  else if(dimension_one == "利率"){
    if(dimension_one_content.Trim() == "1")
      dimension_one_description = "百分之六以下";
    else if(dimension_one_content.Trim() == "2")
      dimension_one_description = "百分之六以上";
  }
  else
    dimension_one_description = dimension_one_content;

  if(dimension_two == "貸款金額"){
    if(dimension_two_content.Trim() == "1")
      dimension_two_description = "新台幣40萬元以下";
    else if(dimension_two_content.Trim() == "2")
      dimension_two_description = "新台幣40萬元 - 50萬元";
    else if(dimension_two_content.Trim() == "3")
      dimension_two_description = "新台幣50萬元 - 100萬元";
    else if(dimension_two_content.Trim() == "4")
      dimension_two_description = "新台幣100萬元以上";
  }
  else if(dimension_two == "產品"){
    if(dimension_two_content.Trim() == "1")
      dimension_two_description = "簡易小額信貸";
    else if(dimension_two_content.Trim() == "2")
      dimension_two_description = "公教人員貸款";
    else if(dimension_two_content.Trim() == "3")
      dimension_two_description = "U利貸款";
    else if(dimension_two_content.Trim() == "4")
      dimension_two_description = "其他";
    else if(dimension_two_content.Trim() == "5")
      dimension_two_description = "農家樂貸款";
    else if(dimension_two_content.Trim() == "6")
      dimension_two_description = "房貸";
  }
  else if(dimension_two == "利率"){
    if(dimension_two_content.Trim() == "1")
      dimension_two_description = "百分之六以下";
    else if(dimension_two_content.Trim() == "2")
      dimension_two_description = "百分之六以上";
  }
  else
    dimension_two_description = dimension_two_content;


  if(dimension_one != "")
    report << dimension_one.c_str() << ": " << dimension_one_description.c_str() << endl;
  else
    report << endl;
  if(dimension_two != "")
    report << dimension_two.c_str() << ": " << dimension_two_description.c_str() << endl;
  else
    report << endl;
  report << "壞帳機率範圍" << "," << "無法評分" << "," << "<=0.5%" << "," << "0.5% - 1.0%" << "," << "1.0% - 1.5%" << "," << "1.5% - 2.0%" << "," << "2.0% - 2.5%" << "," << "2.5% - 3.0%" << "," << "3.0% - 3.5%" << "," << "3.5% - 4.0%" << "," << "4.0% - 4.5%" << "," << ">4.5%" << "," << "Total" << endl;
}

void prepare_report(TADOCommand *command, TADOQuery *query,
                    const AnsiString &begin_date, const AnsiString &end_date,
                    const AnsiString &dimension_one, const AnsiString &dimension_two)
{
  AnsiString sql_stmt;

  sql_stmt = "SELECT CASE_SN ";
  sql_stmt += "FROM APP_RESULT WHERE CASE_SN NOT IN ";
  sql_stmt += "(SELECT CASE_SN FROM APP_INFO ";
  sql_stmt += "WHERE ";
  sql_stmt += "(SYSTEM_DATE BETWEEN :beg_date AND :end_date)) ";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["beg_date"] = begin_date;
  query->Parameters->ParamValues["end_date"] = end_date;
  query->Open();
  if(query->RecordCount > 0)
    write_to_missing_application(query);
  query->Close();

  try{
    sql_stmt = "DROP TABLE REPORT_BASE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE REPORT_BASE(";
  sql_stmt += "CASE_SN VARCHAR(12), APPLICATION_AMOUNT FLOAT, CI_KEY CHAR(10), BRANCH VARCHAR(20), ";
  sql_stmt += "PRODUCT_TYPE_CODE INT, PB DECIMAL(7,4), MODULE_NPV FLOAT, MODULE_DECISION INT, ";
  sql_stmt += "APPROVAL_FINAL INT, APPROVAL_AMOUNT FLOAT, NPV_FINAL FLOAT, ";
  sql_stmt += "APP_AMT_CAT INT, APR1 FLOAT, APR2 FLOAT, APR3 FLOAT, ";
  sql_stmt += "APR_GROUP INT, PB_GROUP INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO REPORT_BASE(CASE_SN, APPLICATION_AMOUNT, CI_KEY, BRANCH, PRODUCT_TYPE_CODE, ";
  sql_stmt += "PB, MODULE_NPV, MODULE_DECISION, APPROVAL_FINAL, APPROVAL_AMOUNT, NPV_FINAL, ";
  sql_stmt += "APP_AMT_CAT, APR1, APR2, APR3, APR_GROUP, PB_GROUP) ";
  sql_stmt += "SELECT A.CASE_SN, A.APP_AMT, A.CI_KEY, A.BRANCH, A.PRODUCT_TYPE_CODE, ";
  sql_stmt += "B.APPLICANT_PB_adj, B.NPV, (CASE ";
  sql_stmt += "WHEN B.APPROVAL_CODE IN (5, 3104, 3110) THEN 3 ";
  sql_stmt += "WHEN B.APPROVAL_CODE > 1 THEN 2 ELSE B.APPROVAL_CODE END), "; // 1 for approved; 2 for decilined; 3 for manual
  sql_stmt += "(CASE WHEN A.APPROVAL_FINAL = 2 THEN 1 ELSE 0 END), ";// 1 for funds remitted; 0 for funds not remitted
  sql_stmt += "A.LOAN_AMT, A.NPV_FINAL, (CASE ";
  sql_stmt += "WHEN APP_AMT >= 0 AND APP_AMT < 400000 THEN 1 ";
  sql_stmt += "WHEN APP_AMT >= 400000 AND APP_AMT < 500000 THEN 2 ";
  sql_stmt += "WHEN APP_AMT >= 500000 AND APP_AMT < 1000000 THEN 3 ";
  sql_stmt += "WHEN APP_AMT >= 1000000 THEN 4 END), ";
  sql_stmt += "A.APR_1, A.APR_2, A.APR_3, (CASE ";
  sql_stmt += "WHEN A.APR_1 > 0.06 AND A.APR_2 > 0.06 AND A.APR_3 > 0.06 THEN 1 ";
  sql_stmt += "WHEN A.APR_1 > 0.06 AND A.APR_2 = 0.00 AND A.APR_3 = 0.00 THEN 1 ";
  sql_stmt += "ELSE 2 END), (CASE ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj IS NULL THEN 0 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj <= 0.005 THEN 1 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj > 0.005 AND B.APPLICANT_PB_adj <= 0.01 THEN 2 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj > 0.01 AND B.APPLICANT_PB_adj <= 0.015 THEN 3 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj > 0.015 AND B.APPLICANT_PB_adj <= 0.02 THEN 4 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj > 0.02 AND B.APPLICANT_PB_adj <= 0.025 THEN 5 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj > 0.025 AND B.APPLICANT_PB_adj <= 0.03 THEN 6 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj > 0.03 AND B.APPLICANT_PB_adj <= 0.035 THEN 7 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj > 0.035 AND B.APPLICANT_PB_adj <= 0.04 THEN 8 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj > 0.04 AND B.APPLICANT_PB_adj <= 0.045 THEN 9 ";
  sql_stmt += "WHEN B.APPLICANT_PB_adj > 0.045 THEN 10 END) ";
  sql_stmt += "FROM APP_INFO AS A INNER JOIN APP_RESULT AS B ";
  sql_stmt += "ON A.CASE_SN = B.CASE_SN ";
  sql_stmt += "WHERE (SYSTEM_DATE BETWEEN :BEG_DATE AND :END_DATE) AND ";
  sql_stmt += "PRODUCT_TYPE_CODE BETWEEN 1 AND 5 ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["BEG_DATE"] = begin_date;
  command->Parameters->ParamValues["END_DATE"] = end_date;
  command->Execute();

//Base table for report part one
  AnsiString category_create_string = "";
  if(dimension_one.Length() != 0){
    if (dimension_one == "CI_KEY")
      category_create_string += dimension_one + " char(10), ";
    else if (dimension_one == "APP_AMT_CAT")
      category_create_string += dimension_one + " int, ";
    else if (dimension_one == "BRANCH")
      category_create_string += dimension_one + " varchar(20), ";
    else if (dimension_one == "APR_GROUP")
      category_create_string += dimension_one + " int, ";
    else if (dimension_one == "PRODUCT_TYPE_CODE")
      category_create_string += dimension_one + " int, ";
  }
  if(dimension_two.Length() != 0){
    if (dimension_two == "CI_KEY")
      category_create_string += dimension_two + " char(10), ";
    else if (dimension_two == "APP_AMT_CAT")
      category_create_string += dimension_two + " int, ";
    else if (dimension_two == "BRANCH")
      category_create_string += dimension_two + " varchar(20), ";
    else if (dimension_two == "APR_GROUP")
      category_create_string += dimension_two + " int, ";
    else if (dimension_two == "PRODUCT_TYPE_CODE")
      category_create_string += dimension_two + " int, ";
  }
  AnsiString category_query_string_single = "";
  AnsiString category_query_string_multiple = "";
  AnsiString category_group_string = "";
  if(dimension_one.Length() != 0){
    category_query_string_single += dimension_one + ", ";
    category_query_string_multiple += "A." + dimension_one + ", ";
    category_group_string += dimension_one;
  }
  if(dimension_two.Length() != 0){
    if(dimension_one.Length() != 0)
      category_group_string += ", ";
    category_group_string += dimension_two;
    category_query_string_single += dimension_two + ", ";
    category_query_string_multiple += "A." + dimension_two + ", ";
  }

  try{
    sql_stmt = "DROP TABLE ONE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE ONE (";
  sql_stmt += category_create_string;
  sql_stmt += " NS float, I float, II float, III float, IV float, V float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO ONE ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string_single;
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 THEN 1 ELSE 0 END) AS NS, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 THEN 1 ELSE 0 END) AS I, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 THEN 1 ELSE 0 END) AS II, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 THEN 1 ELSE 0 END) AS III, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 THEN 1 ELSE 0 END) AS IV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 THEN 1 ELSE 0 END) AS V, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 THEN 1 ELSE 0 END) AS VI, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 THEN 1 ELSE 0 END) AS VII, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 THEN 1 ELSE 0 END) AS VIII, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 THEN 1 ELSE 0 END) AS IX, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 THEN 1 ELSE 0 END) AS X, ";
  sql_stmt += "       COUNT(*) AS XI, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 THEN APPLICATION_AMOUNT ELSE 0 END) AS NSD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS ID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 THEN APPLICATION_AMOUNT ELSE 0 END) AS IID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS IIID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 THEN APPLICATION_AMOUNT ELSE 0 END) AS IVD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 THEN APPLICATION_AMOUNT ELSE 0 END) AS VD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 THEN APPLICATION_AMOUNT ELSE 0 END) AS VID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIIID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 THEN APPLICATION_AMOUNT ELSE 0 END) AS IXD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 THEN APPLICATION_AMOUNT ELSE 0 END) AS XD, ";
  sql_stmt += "       SUM(APPLICATION_AMOUNT) AS XID ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Table for data in portion two of the report

  try{
    sql_stmt = "DROP TABLE TWO;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE TWO (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSMR float, IMR float, IIMR float, IIIMR float, IVMR float, VMR float, VIMR float, VIIMR float, VIIIMR float, IXMR float, XMR float, XIMR float, ";
  sql_stmt += " NSMRD float, IMRD float, IIMRD float, IIIMRD float, IVMRD float, VMRD float, VIMRD float, VIIMRD float, VIIIMRD float, IXMRD float, XMRD float, XIMRD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float)";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TWO ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string_single;
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IVA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS VA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS VIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS VIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS VIIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IXA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS XA, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS XIA, ";
  sql_stmt += "       0,        ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS IAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS IIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS IIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS IVAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS VAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS IXAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS XAD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN APPLICATION_AMOUNT ELSE 0 END) AS XIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS NSMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS IMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS IIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS IIIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS IVMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS VMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS VIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS VIIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS VIIIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS IXMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS XMR, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 3 THEN 1 ELSE 0 END) AS XIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS NSMRD,          ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS IMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS IIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS IIIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS IVMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS VMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIIIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS IXMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS XMRD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 3 THEN APPLICATION_AMOUNT ELSE 0 END) AS XIMRD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IVPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS VPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS VIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS VIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS VIIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IXPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS XPB, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN PB ELSE 0 END) AS XIPB,        ";
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS IPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS IIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS IIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS IVPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS VPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS VIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS VIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS VIIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS IXPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS XPBD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN (PB * APPLICATION_AMOUNT) ELSE 0 END) AS XIPBD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS INPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS IINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS IIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS IVNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS VNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS VINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS VIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS VIIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS IXNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS XNPV, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN MODULE_NPV ELSE 0 END) AS XINPV ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();

//Data for part three of the report
  try{
    sql_stmt = "DROP TABLE THREE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE THREE (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
//  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO THREE ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string_single;
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IVA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VIIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IXA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS XA, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS XIA, ";
  sql_stmt += "       0,        ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IVAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IXAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS XAD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS XIAD, ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IVPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VIIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IXPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS XPB, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS XIPB,        ";
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IVPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VIIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IXPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS XPBD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS XIPBD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS INPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IVNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VIIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IXNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS XNPV, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS XINPV ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();


//Data for part four of the report

  try{
    sql_stmt = "DROP TABLE FOUR_I;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE FOUR_I (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float) ";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FOUR_I ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string_single;
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS NSA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IVA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VIIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IXA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND Module_decision = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS XA, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS XIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS NSAD,               ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IVAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IXAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS XAD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS XIAD, ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IVPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VIIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IXPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS XPB, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS XIPB,        ";
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IVPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VIIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IXPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS XPBD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS XIPBD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS INPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IVNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VIIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IXNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS XNPV, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 2 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS XINPV ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();


  try{
    sql_stmt = "DROP TABLE FOUR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }


  sql_stmt = "CREATE TABLE FOUR (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float, ";
  sql_stmt += " NSAA float, IAA float, IIAA float, IIIAA float, IVAA float, VAA float, VIAA float, VIIAA float, VIIIAA float, IXAA float, XAA float, XIAA float, ";
  sql_stmt += " NSAAD float, IAAD float, IIAAD float, IIIAAD float, IVAAD float, VAAD float, VIAAD float, VIIAAD float, VIIIAAD float, IXAAD float, XAAD float, XIAAD float, ";
  sql_stmt += " NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XAPB float, XIAPB float, ";
  sql_stmt += " NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float,  ";
  sql_stmt += " NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FOUR ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string_multiple;
  sql_stmt += " B.NSA , B.IA , B.IIA , B.IIIA , B.IVA , B.VA , B.VIA , B.VIIA , B.VIIIA , B.IXA , B.XA , B.XIA , ";
  sql_stmt += " B.NSAD , B.IAD , B.IIAD , B.IIIAD , B.IVAD , B.VAD , B.VIAD , B.VIIAD , B.VIIIAD , B.IXAD , B.XAD , B.XIAD , ";
  sql_stmt += " B.NSPB , B.IPB , B.IIPB , B.IIIPB , B.IVPB , B.VPB , B.VIPB , B.VIIPB , B.VIIIPB , B.IXPB , B.XPB , B.XIPB , ";
  sql_stmt += " B.NSPBD , B.IPBD , B.IIPBD , B.IIIPBD , B.IVPBD , B.VPBD , B.VIPBD , B.VIIPBD , B.VIIIPBD , B.IXPBD , B.XPBD , B.XIPBD ,  ";
  sql_stmt += " B.NSNPV , B.INPV , B.IINPV , B.IIINPV , B.IVNPV , B.VNPV , B.VINPV , B.VIINPV , B.VIIINPV , B.IXNPV , B.XNPV , B.XINPV , ";
  sql_stmt += "(A.NSA + B.NSA), (A.IA + B.IA), (A.IIA + B.IIA), (A.IIIA + B.IIIA), (A.IVA + B.IVA), (A.VA + B.VA), (A.VIA + B.VIA), (A.VIIA + B.VIIA),  (A.VIIIA + B.VIIIA), (A.IXA + B.IXA), (A.XA + B.XA), (A.XIA + B.XIA), ";
  sql_stmt += "(A.NSAD + B.NSAD), (A.IAD + B.IAD), (A.IIAD + B.IIAD), (A.IIIAD + B.IIIAD), (A.IVAD + B.IVAD), (A.VAD + B.VAD), (A.VIAD + B.VIAD), (A.VIIAD + B.VIIAD), (A.VIIIAD + B.VIIIAD), (A.IXAD + B.IXAD), (A.XAD + B.XAD), (A.XIAD + B.XIAD), ";
  sql_stmt += "0, (A.IPB + B.IPB), (A.IIPB + B.IIPB), (A.IIIPB + B.IIIPB), (A.IVPB + B.IVPB), (A.VPB + B.VPB), (A.VIPB + B.VIPB), (A.VIIPB + B.VIIPB), (A.VIIIPB + B.VIIIPB), (A.IXPB + B.IXPB), (A.XPB + B.XPB), (A.XIPB + B.XIPB), ";
  sql_stmt += "0, (A.IPBD + B.IPBD), (A.IIPBD + B.IIPBD), (A.IIIPBD + B.IIIPBD), (A.IVPBD + B.IVPBD), (A.VPBD + B.VPBD), (A.VIPBD + B.VIPBD), (A.VIIPBD + B.VIIPBD), (A.VIIIPBD + B.VIIIPBD),  (A.IXPBD + B.IXPBD),  (A.XPBD + B.XPBD),  (A.XIPBD + B.XIPBD), ";
  sql_stmt += "(A.NSNPV + B.NSNPV), (A.INPV + B.INPV), (A.IINPV + B.IINPV), (A.IIINPV + B.IIINPV), (A.IVNPV + B.IVNPV), (A.VNPV + B.VNPV), (A.VINPV + B.VINPV), (A.VIINPV + B.VIINPV), (A.VIIINPV + B.VIIINPV), (A.IXNPV + B.IXNPV), (A.XNPV + B.XNPV), (A.XINPV + B.XINPV) ";
  sql_stmt += "FROM THREE AS A, FOUR_I AS B ";
  if(category_create_string != ""){
    sql_stmt += "WHERE ";
    if(dimension_one != ""){
    	sql_stmt += "A.";
    	sql_stmt += dimension_one;
    	sql_stmt += " = B.";
    	sql_stmt += dimension_one;
    }
    if(dimension_two != ""){
      if(dimension_one != "")
        sql_stmt += " AND ";
    	sql_stmt += "A.";
    	sql_stmt += dimension_two;
    	sql_stmt += " = B.";
    	sql_stmt += dimension_two;
    }
  }
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE FOUR_AR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE FOUR_AR(";
  sql_stmt += category_create_string;
  sql_stmt += "                      NS float, I float, II float, III float, IV float, V  float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += "                      NSD float, ID float, IID float, IIID float, IVD float, VD  float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FOUR_AR ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string_single;
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 THEN 1 ELSE 0 END) AS NS, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 THEN 1 ELSE 0 END) AS I, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 THEN 1 ELSE 0 END) AS II, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 THEN 1 ELSE 0 END) AS III, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 THEN 1 ELSE 0 END) AS IV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 THEN 1 ELSE 0 END) AS V, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 THEN 1 ELSE 0 END) AS VI, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 THEN 1 ELSE 0 END) AS VII, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 THEN 1 ELSE 0 END) AS VIII, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 THEN 1 ELSE 0 END) AS IX, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 THEN 1 ELSE 0 END) AS X, ";
  sql_stmt += "       COUNT(*) AS XI, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 THEN APPROVAL_AMOUNT ELSE 0 END) AS NSD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS ID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 THEN APPROVAL_AMOUNT ELSE 0 END) AS IID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 THEN APPROVAL_AMOUNT ELSE 0 END) AS IIID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 THEN APPROVAL_AMOUNT ELSE 0 END) AS IVD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 THEN APPROVAL_AMOUNT ELSE 0 END) AS VD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 THEN APPROVAL_AMOUNT ELSE 0 END) AS VID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIIID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 THEN APPROVAL_AMOUNT ELSE 0 END) AS IXD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 THEN APPROVAL_AMOUNT ELSE 0 END) AS XD, ";
  sql_stmt += "       SUM(APPROVAL_AMOUNT) AS XID ";
  sql_stmt += "FROM REPORT_BASE ";
  sql_stmt += "WHERE Module_decision = 2 ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();

//Data for part five of the report
  try{
    sql_stmt = "DROP TABLE FIVE_I;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

//Data for part five of the report

  sql_stmt = "CREATE TABLE FIVE_I (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FIVE_I ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string_single;
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS NSA,        ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IVA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS VIIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS IXA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS XA, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN 1 ELSE 0 END) AS XIA, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS NSAD,               ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IVAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS IXAD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS XAD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS XIAD, ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IVPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS VIIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS IXPB, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS XPB, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN PB ELSE 0 END) AS XIPB,        ";
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IVPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS VIIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS IXPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS XPBD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN (PB * APPROVAL_AMOUNT) ELSE 0 END) AS XIPBD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS INPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IVNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS VIIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS IXNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 AND MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS XNPV, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 3 AND APPROVAL_FINAL = 1 THEN NPV_FINAL ELSE 0 END) AS XINPV ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE FIVE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

//Data for part five of the report

  sql_stmt = "CREATE TABLE FIVE (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float, ";
  sql_stmt += " NSAA float, IAA float, IIAA float, IIIAA float, IVAA float, VAA float, VIAA float, VIIAA float, VIIIAA float, IXAA float, XAA float, XIAA float, ";
  sql_stmt += " NSAAD float, IAAD float, IIAAD float, IIIAAD float, IVAAD float, VAAD float, VIAAD float, VIIAAD float, VIIIAAD float, IXAAD float, XAAD float, XIAAD float, ";
  sql_stmt += " NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XAPB float, XIAPB float, ";
  sql_stmt += " NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float,  ";
  sql_stmt += " NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FIVE ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string_multiple;
  sql_stmt += " B.NSA , B.IA , B.IIA , B.IIIA , B.IVA , B.VA , B.VIA , B.VIIA , B.VIIIA , B.IXA , B.XA , B.XIA , ";
  sql_stmt += " B.NSAD , B.IAD , B.IIAD , B.IIIAD , B.IVAD , B.VAD , B.VIAD , B.VIIAD , B.VIIIAD , B.IXAD , B.XAD , B.XIAD , ";
  sql_stmt += " B.NSPB , B.IPB , B.IIPB , B.IIIPB , B.IVPB , B.VPB , B.VIPB , B.VIIPB , B.VIIIPB , B.IXPB , B.XPB , B.XIPB , ";
  sql_stmt += " B.NSPBD , B.IPBD , B.IIPBD , B.IIIPBD , B.IVPBD , B.VPBD , B.VIPBD , B.VIIPBD , B.VIIIPBD , B.IXPBD , B.XPBD , B.XIPBD ,  ";
  sql_stmt += " B.NSNPV , B.INPV , B.IINPV , B.IIINPV , B.IVNPV , B.VNPV , B.VINPV , B.VIINPV , B.VIIINPV , B.IXNPV , B.XNPV , B.XINPV , ";
  sql_stmt += "(A.NSAA + B.NSA), (A.IAA + B.IA), (A.IIAA + B.IIA), (A.IIIAA + B.IIIA), (A.IVAA + B.IVA), (A.VAA + B.VA), (A.VIAA + B.VIA), (A.VIIAA + B.VIIA),  (A.VIIIAA + B.VIIIA), (A.IXAA + B.IXA), (A.XAA + B.XA), (A.XIAA + B.XIA), ";
  sql_stmt += "(A.NSAAD + B.NSAD), (A.IAAD + B.IAD), (A.IIAAD + B.IIAD), (A.IIIAAD + B.IIIAD), (A.IVAAD + B.IVAD), (A.VAAD + B.VAD), (A.VIAAD + B.VIAD), (A.VIIAAD + B.VIIAD), (A.VIIIAAD + B.VIIIAD), (A.IXAAD + B.IXAD), (A.XAAD + B.XAD), (A.XIAAD + B.XIAD), ";
  sql_stmt += "0, (A.IAPB + B.IPB), (A.IIAPB + B.IIPB), (A.IIIAPB + B.IIIPB), (A.IVAPB + B.IVPB), (A.VAPB + B.VPB), (A.VIAPB + B.VIPB), (A.VIIAPB + B.VIIPB), (A.VIIIAPB + B.VIIIPB), (A.IXAPB + B.IXPB), (A.XAPB + B.XPB), (A.XIAPB + B.XIPB), ";
  sql_stmt += "0, (A.IAPBD + B.IPBD), (A.IIAPBD + B.IIPBD), (A.IIIAPBD + B.IIIPBD), (A.IVAPBD + B.IVPBD), (A.VAPBD + B.VPBD), (A.VIAPBD + B.VIPBD), (A.VIIAPBD + B.VIIPBD), (A.VIIIAPBD + B.VIIIPBD),  (A.IXAPBD + B.IXPBD),  (A.XAPBD + B.XPBD),  (A.XIAPBD + B.XIPBD), ";
  sql_stmt += "(A.NSANPV + B.NSNPV), (A.IANPV + B.INPV), (A.IIANPV + B.IINPV), (A.IIIANPV + B.IIINPV), (A.IVANPV + B.IVNPV), (A.VANPV + B.VNPV), (A.VIANPV + B.VINPV), (A.VIIANPV + B.VIINPV), (A.VIIIANPV + B.VIIINPV), (A.IXANPV + B.IXNPV), (A.XANPV + B.XNPV), (A.XIANPV + B.XINPV) ";
  sql_stmt += "FROM FOUR AS A, FIVE_I AS B ";
  if(category_create_string != ""){
    sql_stmt += "WHERE ";
    if(dimension_one != ""){
    	sql_stmt += "A.";
    	sql_stmt += dimension_one;
    	sql_stmt += " = B.";
    	sql_stmt += dimension_one;
    }
    if(dimension_two != ""){
      if(dimension_one != "")
        sql_stmt += " AND ";
    	sql_stmt += "A.";
    	sql_stmt += dimension_two;
    	sql_stmt += " = B.";
    	sql_stmt += dimension_two;
    }
  }
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE FIVE_AR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE FIVE_AR(";
  sql_stmt += category_create_string;
  sql_stmt += "                      NS float, I float, II float, III float, IV float, V  float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += "                      NSD float, ID float, IID float, IIID float, IVD float, VD  float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FIVE_AR ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string_single;
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 THEN 1 ELSE 0 END) AS NS, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 THEN 1 ELSE 0 END) AS I, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 THEN 1 ELSE 0 END) AS II, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 THEN 1 ELSE 0 END) AS III, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 THEN 1 ELSE 0 END) AS IV, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 THEN 1 ELSE 0 END) AS V, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 THEN 1 ELSE 0 END) AS VI, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 THEN 1 ELSE 0 END) AS VII, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 THEN 1 ELSE 0 END) AS VIII, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 THEN 1 ELSE 0 END) AS IX, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 THEN 1 ELSE 0 END) AS X, ";
  sql_stmt += "       COUNT(*) AS XI, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 0 THEN APPROVAL_AMOUNT ELSE 0 END) AS NSD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 1 THEN APPROVAL_AMOUNT ELSE 0 END) AS ID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 2 THEN APPROVAL_AMOUNT ELSE 0 END) AS IID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 3 THEN APPROVAL_AMOUNT ELSE 0 END) AS IIID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 4 THEN APPROVAL_AMOUNT ELSE 0 END) AS IVD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 5 THEN APPROVAL_AMOUNT ELSE 0 END) AS VD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 6 THEN APPROVAL_AMOUNT ELSE 0 END) AS VID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 7 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 8 THEN APPROVAL_AMOUNT ELSE 0 END) AS VIIID, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 9 THEN APPROVAL_AMOUNT ELSE 0 END) AS IXD, ";
  sql_stmt += "       SUM(CASE WHEN PB_GROUP = 10 THEN APPROVAL_AMOUNT ELSE 0 END) AS XD, ";
  sql_stmt += "       SUM(APPROVAL_AMOUNT) AS XID ";
  sql_stmt += "FROM REPORT_BASE ";
  sql_stmt += "WHERE Module_decision = 3 ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();

//Generate report tables
//First part
  try{
    sql_stmt = "DROP TABLE PART_ONE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE PART_ONE ";
  sql_stmt += "(NS float, I float, II float, III float, IV float, V float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

//Second part
  try{
    sql_stmt = "DROP TABLE PART_TWO;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE PART_TWO ";
  sql_stmt += "(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSMR float, IMR float, IIMR float, IIIMR float, IVMR float, VMR float, VIMR float, VIIMR float, VIIIMR float, IXMR float, XMR float, XIMR float, ";
  sql_stmt += " NSMRD float, IMRD float, IIMRD float, IIIMRD float, IVMRD float, VMRD float, VIMRD float, VIIMRD float, VIIIMRD float, IXMRD float, XMRD float, XIMRD float, ";
  sql_stmt += " NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float, ";
  sql_stmt += " NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

//Third part
  try{
    sql_stmt = "DROP TABLE PART_THREE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE PART_THREE ";
  sql_stmt += "(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
//  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float, ";
  sql_stmt += " NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

//Fourth part
  try{
    sql_stmt = "DROP TABLE PART_FOUR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }


  sql_stmt = "CREATE TABLE PART_FOUR ";
  sql_stmt += "(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
//  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float, ";
  sql_stmt += " NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float, ";
  sql_stmt += " NSAAR float, IAAR float, IIAAR float, IIIAAR float, IVAAR float, VAAR float, VIAAR float, VIIAAR float, VIIIAAR float, IXAAR float, XAAR float, XIAAR float, ";
  sql_stmt += " NSAARD float, IAARD float, IIAARD float, IIIAARD float, IVAARD float, VAARD float, VIAARD float, VIIAARD float, VIIIAARD float, IXAARD float, XAARD float, XIAARD float, ";
  sql_stmt += " NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XAPB float, XIAPB float, ";
  sql_stmt += " NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float,  ";
  sql_stmt += " NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE AR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE AR(NS float, I float, II float, III float, IV float, V  float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += "                 NSD float, ID float, IID float, IIID float, IVD float, VD  float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

//Fifth part
  try{
    sql_stmt = "DROP TABLE PART_FIVE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  sql_stmt = "CREATE TABLE PART_FIVE ";
  sql_stmt += "(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float, ";
  sql_stmt += " NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float, ";
  sql_stmt += " NSAAR float, IAAR float, IIAAR float, IIIAAR float, IVAAR float, VAAR float, VIAAR float, VIIAAR float, VIIIAAR float, IXAAR float, XAAR float, XIAAR float, ";
  sql_stmt += " NSAARD float, IAARD float, IIAARD float, IIIAARD float, IVAARD float, VAARD float, VIAARD float, VIIAARD float, VIIIAARD float, IXAARD float, XAARD float, XIAARD float, ";
  sql_stmt += " NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XAPB float, XIAPB float, ";
  sql_stmt += " NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float,  ";
  sql_stmt += " NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float);";
  command->CommandText = sql_stmt;
  command->Execute();
}

void write_to_missing_application(TADOQuery *query)
{
  ofstream log("App_info_missing.log",ios_base::app);
  query->First();
  while(!query->Eof){
    AnsiString app_sn = query->FieldValues["CASE_SN"];
//    AnsiString app_time = query->FieldValues["APP_DATA_TIME"];
    log << app_sn.c_str() /*<< "," << app_time.c_str() << ","*/ << endl;
    query->Next();
  }
  log.clear();
  log.close();
}

bool generate_report(TADOCommand *command, TADOQuery *query,
                     const AnsiString &report_date,
                     const AnsiString &begin_date, const AnsiString &end_date,
                     const AnsiString &dimension_one_name, const AnsiString &dimension_two_name,
                     const AnsiString &dimension_one, const AnsiString &dimension_two,
                     unsigned int dimension_one_type, unsigned int dimension_two_type,
                     const AnsiString &report_path, AnsiString &error)
{
  error = "";
  vector<AnsiString>dimension_one_contents;
  vector<AnsiString>dimension_two_contents;
  if(dimension_one != "")
    fill_dimension(dimension_one, query, dimension_one_contents, dimension_one_type);
  if(dimension_two != "")
    fill_dimension(dimension_two, query, dimension_two_contents, dimension_two_type);

  if(dimension_one_contents.empty() && dimension_two_contents.empty()){
    generate_summary_report(command, "", 0, "");
    AnsiString full_file_name = report_path + report_date + "_" + begin_date + "_" + end_date + "_" + "all" + ".csv";
    ofstream report(full_file_name.c_str(), ios_base::out);
    report.precision(15);
    if(!report){
      error += "無法產生報表檔。 \n";
      return false;
    }
    write_header(report, report_date, begin_date, end_date, dimension_one_name, dimension_two_name, "", "");
    write_part_one(query, report);
    write_part_two(query, report);
    write_part_three(query, report);
    write_part_four(query, report);
    write_part_five(query, report);
    report.clear();
    report.close();
  }
  else if(dimension_one_contents.empty()){
    vector<AnsiString>::iterator iter_dim2 = dimension_two_contents.begin();
    for(;iter_dim2 != dimension_two_contents.end(); ++iter_dim2){
      generate_summary_report(command, dimension_two,
                              dimension_two_type, (*iter_dim2));
      AnsiString full_file_name = report_path + report_date + "_" + begin_date + "_" + end_date + "_" + dimension_two + "_" + (*iter_dim2) + ".csv";
      ofstream report(full_file_name.c_str(), ios_base::out);
      report.precision(15);
      if(!report){
        error += "無法產生報表檔。 \n";
        return false;
      }
      write_header(report, report_date, begin_date, end_date, dimension_one_name, dimension_two_name, "", (*iter_dim2));
      write_part_one(query, report);
      write_part_two(query, report);
      write_part_three(query, report);
      write_part_four(query, report);
      write_part_five(query, report);
      report.clear();
      report.close();
    }
  }
  else if(dimension_two_contents.empty()){
    vector<AnsiString>::iterator iter_dim1 = dimension_one_contents.begin();
    for(;iter_dim1 != dimension_one_contents.end(); ++iter_dim1){
      generate_summary_report(command, dimension_one,
                              dimension_one_type, (*iter_dim1));
      AnsiString full_file_name = report_path + report_date + "_" + begin_date + "_" + end_date + "_" + dimension_one + "_" + (*iter_dim1) + ".csv";
      ofstream report(full_file_name.c_str(), ios_base::out);
      report.precision(15);
      if(!report){
        error += "無法產生報表檔。 \n";
        return false;
      }
      write_header(report, report_date, begin_date, end_date, dimension_one_name, dimension_two_name, (*iter_dim1), "");
      write_part_one(query, report);
      write_part_two(query, report);
      write_part_three(query, report);
      write_part_four(query, report);
      write_part_five(query, report);
      report.clear();
      report.close();
    }
  }
  else{
    vector<AnsiString>::iterator iter_dim1 = dimension_one_contents.begin();
    vector<AnsiString>::iterator iter_dim2 = dimension_two_contents.begin();
    for(;iter_dim1 != dimension_one_contents.end(); ++iter_dim1){
      iter_dim2 = dimension_two_contents.begin();
      for(;iter_dim2 != dimension_two_contents.end(); ++iter_dim2){
        generate_type_one_report(command, dimension_one, dimension_two,
                                 dimension_one_type, dimension_two_type,
                                 (*iter_dim1), (*iter_dim2));
        AnsiString full_file_name = report_path + report_date + "_" + begin_date + "_" + end_date + "_" + dimension_one + "_" + (*iter_dim1) + "_" + dimension_two + "_" + (*iter_dim2) + ".csv";
        ofstream report(full_file_name.c_str(), ios_base::out);
        report.precision(15);
        if(!report){
          error += "無法產生報表檔。 \n";
          return false;
        }
        write_header(report, report_date, begin_date, end_date, dimension_one_name, dimension_two_name, (*iter_dim1), (*iter_dim2));
        write_part_one(query, report);
        write_part_two(query, report);
        write_part_three(query, report);
        write_part_four(query, report);
        write_part_five(query, report);
        report.clear();
        report.close();
      }
    }
  }
//  clean_up(command);
  return true;
}

void generate_type_one_report(TADOCommand *command,
                              const AnsiString &dimension_one,
                              const AnsiString &dimension_two,
                              unsigned int dimension_one_type,
                              unsigned int dimension_two_type,
                              const AnsiString &dimension_one_content,
                              const AnsiString &dimension_two_content)
{
  bool dimension_one_exist = dimension_one.Length() == 0 ? false : true;
  bool dimension_two_exist = dimension_two.Length() == 0 ? false : true;
  AnsiString join_string = "";
  AnsiString condition_string = "";
  if(dimension_one_exist){
    condition_string += " WHERE A." + dimension_one + " = :DIM1 ";
    join_string += " ON A." + dimension_one + " = B." + dimension_one;
  }
  if(dimension_two_exist){
    if(dimension_one_exist){
      condition_string += " AND ";
      join_string += " AND ";
    }
    else{
      condition_string += " WHERE ";
      join_string += " ON ";
    }
    condition_string += "A." + dimension_two + " = :DIM2 ";
    join_string += "A." + dimension_two + " = B." + dimension_two;
  }

  AnsiString sql_stmt;

  sql_stmt = "DELETE FROM PART_ONE;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM PART_TWO;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM PART_THREE;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM PART_FOUR;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM PART_FIVE;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO PART_ONE ";
  sql_stmt += "SELECT  ";
  sql_stmt += "NS, I, II, III, IV, V, VI, VII, VIII, IX, X, XI, ";
  sql_stmt += "NSD, ID, IID, IIID, IVD, VD, VID, VIID, VIIID, IXD, XD, XID ";
  sql_stmt += "FROM ONE AS A ";
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_one_exist){
    if(dimension_one_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_one_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_one_content;
  }
  if(dimension_two_exist){
    if(dimension_two_type == 0)
      command->Parameters->ParamValues["DIM2"] = dimension_two_content.ToInt();
    else
      command->Parameters->ParamValues["DIM2"] = dimension_two_content;
  }
  command->Execute();

  sql_stmt = "INSERT INTO PART_TWO ";
  sql_stmt += "(NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "NSMR, IMR, IIMR, IIIMR, IVMR, VMR, VIMR, VIIMR, VIIIMR, IXMR, XMR, XIMR, ";
  sql_stmt += "NSMRD, IMRD, IIMRD, IIIMRD, IVMRD, VMRD, VIMRD, VIIMRD, VIIIMRD, IXMRD, XMRD, XIMRD, ";
  sql_stmt += "NSAR, IAR, IIAR, IIIAR, IVAR, VAR, VIAR, VIIAR, VIIIAR, IXAR, XAR, XIAR, ";
  sql_stmt += "NSARD, IARD, IIARD, IIIARD, IVARD, VARD, VIARD, VIIARD, VIIIARD, IXARD, XARD, XIARD, ";
  sql_stmt += "IPB, IIPB, IIIPB, IVPB, VPB, VIPB, VIIPB, VIIIPB, IXPB, XPB, XIPB, ";
  sql_stmt += "IPBD, IIPBD, IIIPBD, IVPBD, VPBD, VIPBD, VIIPBD, VIIIPBD, IXPBD, XPBD, XIPBD, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV) ";
  sql_stmt += "SELECT ";
  sql_stmt += "0, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "0, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "NSMR, IMR, IIMR, IIIMR, IVMR, VMR, VIMR, VIIMR, VIIIMR, IXMR, XMR, XIMR, ";
  sql_stmt += "NSMRD, IMRD, IIMRD, IIIMRD, IVMRD, VMRD, VIMRD, VIIMRD, VIIIMRD, IXMRD, XMRD, XIMRD, ";
  sql_stmt += "0, ";
  sql_stmt += "IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR, ";
  sql_stmt += "IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR, ";
  sql_stmt += "IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR, ";
  sql_stmt += "IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR, ";
  sql_stmt += "VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR, ";
  sql_stmt += "VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR, ";
  sql_stmt += "VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR, ";
  sql_stmt += "VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR, ";
  sql_stmt += "IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR, ";
  sql_stmt += "XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR, ";
  sql_stmt += "XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR, ";
  sql_stmt += "0, ";
  sql_stmt += "IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD, ";
  sql_stmt += "IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD, ";
  sql_stmt += "IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD, ";
  sql_stmt += "IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD, ";
  sql_stmt += "VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD, ";
  sql_stmt += "VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD, ";
  sql_stmt += "VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD, ";
  sql_stmt += "VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD, ";
  sql_stmt += "IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD, ";
  sql_stmt += "XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD, ";
  sql_stmt += "XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD, ";
//  sql_stmt += "NULL,  ";
  sql_stmt += "IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END) AS IPB,  ";
  sql_stmt += "IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END) AS IIPB,  ";
  sql_stmt += "IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIIA END) AS IIIPB,  ";
  sql_stmt += "IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END) AS IVPB,  ";
  sql_stmt += "VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END) AS VPB,  ";
  sql_stmt += "VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END) AS VIPB,  ";
  sql_stmt += "VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END) AS VIIPB,  ";
  sql_stmt += "VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END) AS VIIIPB,  ";
  sql_stmt += "IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END) AS IXPB,  ";
  sql_stmt += "XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END) AS XPB,  ";
  sql_stmt += "XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END) AS XIPB, ";
//  sql_stmt += "NULL,  ";
  sql_stmt += "IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END) AS IPBD,  ";
  sql_stmt += "IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END) AS IIPBD,  ";
  sql_stmt += "IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END) AS IIIPBD,  ";
  sql_stmt += "IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END) AS IVPBD,  ";
  sql_stmt += "VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END) AS VPBD,  ";
  sql_stmt += "VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END) AS VIPBD,  ";
  sql_stmt += "VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END) AS VIIPBD,  ";
  sql_stmt += "VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END) AS VIIIPBD,  ";
  sql_stmt += "IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END) AS IXPBD,  ";
  sql_stmt += "XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END) AS XPBD,  ";
  sql_stmt += "XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END) AS XIPBD,  ";
//  sql_stmt += "NULL, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV  ";
  sql_stmt += "FROM TWO AS B INNER JOIN ONE AS A  ";
  sql_stmt += join_string;
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_one_exist){
    if(dimension_one_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_one_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_one_content;
  }
  if(dimension_two_exist){
    if(dimension_two_type == 0)
      command->Parameters->ParamValues["DIM2"] = dimension_two_content.ToInt();
    else
      command->Parameters->ParamValues["DIM2"] = dimension_two_content;
  }
  command->Execute();


  sql_stmt = "INSERT INTO PART_THREE ";
  sql_stmt += "(NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "NSAR, IAR, IIAR, IIIAR, IVAR, VAR, VIAR, VIIAR, VIIIAR, IXAR, XAR, XIAR, ";
  sql_stmt += "NSARD, IARD, IIARD, IIIARD, IVARD, VARD, VIARD, VIIARD, VIIIARD, IXARD, XARD, XIARD, ";
  sql_stmt += "IPB, IIPB, IIIPB, IVPB, VPB, VIPB, VIIPB, VIIIPB, IXPB, XPB, XIPB, ";
  sql_stmt += "IPBD, IIPBD, IIIPBD, IVPBD, VPBD, VIPBD, VIIPBD, VIIIPBD, IXPBD, XPBD, XIPBD, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV) ";
  sql_stmt += "SELECT ";
  sql_stmt += "0, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "0, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "0, ";
  sql_stmt += "IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR, ";
  sql_stmt += "IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR, ";
  sql_stmt += "IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR, ";
  sql_stmt += "IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR, ";
  sql_stmt += "VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR, ";
  sql_stmt += "VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR, ";
  sql_stmt += "VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR, ";
  sql_stmt += "VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR, ";
  sql_stmt += "IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR, ";
  sql_stmt += "XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR, ";
  sql_stmt += "XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR, ";
  sql_stmt += "0, ";
  sql_stmt += "IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD, ";
  sql_stmt += "IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD, ";
  sql_stmt += "IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD, ";
  sql_stmt += "IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD, ";
  sql_stmt += "VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD, ";
  sql_stmt += "VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD, ";
  sql_stmt += "VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD, ";
  sql_stmt += "VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD, ";
  sql_stmt += "IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD, ";
  sql_stmt += "XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD, ";
  sql_stmt += "XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD, ";
//  sql_stmt += "NULL,  ";
  sql_stmt += "IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END) AS IPB,  ";
  sql_stmt += "IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END) AS IIPB,  ";
  sql_stmt += "IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIIA END) AS IIIPB,  ";
  sql_stmt += "IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END) AS IVPB,  ";
  sql_stmt += "VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END) AS VPB,  ";
  sql_stmt += "VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END) AS VIPB,  ";
  sql_stmt += "VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END) AS VIIPB,  ";
  sql_stmt += "VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END) AS VIIIPB,  ";
  sql_stmt += "IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END) AS IXPB,  ";
  sql_stmt += "XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END) AS XPB,  ";
  sql_stmt += "XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END) AS XIPB, ";
//  sql_stmt += "NULL,  ";
  sql_stmt += "IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END) AS IPBD,  ";
  sql_stmt += "IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END) AS IIPBD,  ";
  sql_stmt += "IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END) AS IIIPBD,  ";
  sql_stmt += "IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END) AS IVPBD,  ";
  sql_stmt += "VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END) AS VPBD,  ";
  sql_stmt += "VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END) AS VIPBD,  ";
  sql_stmt += "VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END) AS VIIPBD,  ";
  sql_stmt += "VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END) AS VIIIPBD,  ";
  sql_stmt += "IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END) AS IXPBD,  ";
  sql_stmt += "XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END) AS XPBD,  ";
  sql_stmt += "XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END) AS XIPBD,  ";
//  sql_stmt += "NULL, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV  ";
  sql_stmt += "FROM THREE AS B INNER JOIN ONE AS A  ";
  sql_stmt += join_string;
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_one_exist){
    if(dimension_one_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_one_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_one_content;
  }
  if(dimension_two_exist){
    if(dimension_two_type == 0)
      command->Parameters->ParamValues["DIM2"] = dimension_two_content.ToInt();
    else
      command->Parameters->ParamValues["DIM2"] = dimension_two_content;
  }
  command->Execute();


  sql_stmt = "INSERT INTO PART_FOUR ";
  sql_stmt += "(NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "NSAR, IAR, IIAR, IIIAR, IVAR, VAR, VIAR, VIIAR, VIIIAR, IXAR, XAR, XIAR, ";
  sql_stmt += "NSARD, IARD, IIARD, IIIARD, IVARD, VARD, VIARD, VIIARD, VIIIARD, IXARD, XARD, XIARD, ";
  sql_stmt += "IPB, IIPB, IIIPB, IVPB, VPB, VIPB, VIIPB, VIIIPB, IXPB, XPB, XIPB, ";
  sql_stmt += "IPBD, IIPBD, IIIPBD, IVPBD, VPBD, VIPBD, VIIPBD, VIIIPBD, IXPBD, XPBD, XIPBD, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV, ";
  sql_stmt += "NSAAR, IAAR, IIAAR, IIIAAR, IVAAR, VAAR, VIAAR, VIIAAR, VIIIAAR, IXAAR, XAAR, XIAAR, ";
  sql_stmt += "NSAARD, IAARD, IIAARD, IIIAARD, IVAARD, VAARD, VIAARD, VIIAARD, VIIIAARD, IXAARD, XAARD, XIAARD, ";
  sql_stmt += "IAPB, IIAPB, IIIAPB, IVAPB, VAPB, VIAPB, VIIAPB, VIIIAPB, IXAPB, XAPB, XIAPB, ";
  sql_stmt += "IAPBD, IIAPBD, IIIAPBD, IVAPBD, VAPBD, VIAPBD, VIIAPBD, VIIIAPBD, IXAPBD, XAPBD, XIAPBD, ";
  sql_stmt += "IANPV, IIANPV, IIIANPV, IVANPV, VANPV, VIANPV, VIIANPV, VIIIANPV, IXANPV, XANPV, XIANPV) ";
  sql_stmt += "SELECT ";
  sql_stmt += "NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "NSA / (CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END) AS NSAR, ";
  sql_stmt += "IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR, ";
  sql_stmt += "IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR, ";
  sql_stmt += "IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR, ";
  sql_stmt += "IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR, ";
  sql_stmt += "VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR, ";
  sql_stmt += "VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR, ";
  sql_stmt += "VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR, ";
  sql_stmt += "VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR, ";
  sql_stmt += "IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR, ";
  sql_stmt += "XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR, ";
  sql_stmt += "XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR, ";
  sql_stmt += "NSAD / (CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END) AS NSARD, ";
  sql_stmt += "IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD, ";
  sql_stmt += "IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD, ";
  sql_stmt += "IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD, ";
  sql_stmt += "IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD, ";
  sql_stmt += "VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD, ";
  sql_stmt += "VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD, ";
  sql_stmt += "VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD, ";
  sql_stmt += "VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD, ";
  sql_stmt += "IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD, ";
  sql_stmt += "XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD, ";
  sql_stmt += "XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD, ";
//  sql_stmt += "NULL,  ";
  sql_stmt += "IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END) AS IPB,  ";
  sql_stmt += "IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END) AS IIPB,  ";
  sql_stmt += "IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIIA END) AS IIIPB,  ";
  sql_stmt += "IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END) AS IVPB,  ";
  sql_stmt += "VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END) AS VPB,  ";
  sql_stmt += "VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END) AS VIPB,  ";
  sql_stmt += "VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END) AS VIIPB,  ";
  sql_stmt += "VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END) AS VIIIPB,  ";
  sql_stmt += "IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END) AS IXPB,  ";
  sql_stmt += "XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END) AS XPB,  ";
  sql_stmt += "XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END) AS XIPB, ";
//  sql_stmt += "NULL,  ";
  sql_stmt += "IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END) AS IPBD,  ";
  sql_stmt += "IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END) AS IIPBD,  ";
  sql_stmt += "IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END) AS IIIPBD,  ";
  sql_stmt += "IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END) AS IVPBD,  ";
  sql_stmt += "VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END) AS VPBD,  ";
  sql_stmt += "VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END) AS VIPBD,  ";
  sql_stmt += "VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END) AS VIIPBD,  ";
  sql_stmt += "VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END) AS VIIIPBD,  ";
  sql_stmt += "IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END) AS IXPBD,  ";
  sql_stmt += "XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END) AS XPBD,  ";
  sql_stmt += "XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END) AS XIPBD,  ";
//  sql_stmt += "NULL,";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV, ";
  sql_stmt += "NSAA /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END), ";
  sql_stmt += "IAA /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END), ";
  sql_stmt += "IIAA /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END), ";
  sql_stmt += "IIIAA /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END), ";
  sql_stmt += "IVAA /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END), ";
  sql_stmt += "VAA /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END), ";
  sql_stmt += "VIAA /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END), ";
  sql_stmt += "VIIAA /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END), ";
  sql_stmt += "VIIIAA /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END), ";
  sql_stmt += "IXAA /(CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END), ";
  sql_stmt += "XAA /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END), ";
  sql_stmt += "XIAA /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END), ";
  sql_stmt += "NSAAD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END), ";
  sql_stmt += "IAAD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END), ";
  sql_stmt += "IIAAD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END), ";
  sql_stmt += "IIIAAD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END), ";
  sql_stmt += "IVAAD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END), ";
  sql_stmt += "VAAD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END), ";
  sql_stmt += "VIAAD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END), ";
  sql_stmt += "VIIAAD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END), ";
  sql_stmt += "VIIIAAD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END), ";
  sql_stmt += "IXAAD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END), ";
  sql_stmt += "XAAD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END), ";
  sql_stmt += "XIAAD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END), ";
//  sql_stmt += "NULL, ";
  sql_stmt += "IAPB /(CASE WHEN IAA = 0 THEN NULL ELSE IAA END) AS IAPB, ";
  sql_stmt += "IIAPB /(CASE WHEN IIAA = 0 THEN NULL ELSE IIAA END) AS IIAPB, ";
  sql_stmt += "IIIAPB /(CASE WHEN IIIAA = 0 THEN NULL ELSE IIIAA END) AS IIIAPB, ";
  sql_stmt += "IVAPB /(CASE WHEN IVAA = 0 THEN NULL ELSE IVAA END) AS IVAPB, ";
  sql_stmt += "VAPB /(CASE WHEN VAA = 0 THEN NULL ELSE VAA END) AS VAPB, ";
  sql_stmt += "VIAPB /(CASE WHEN VIAA = 0 THEN NULL ELSE VIAA END) AS VIAPB, ";
  sql_stmt += "VIIAPB /(CASE WHEN VIIAA = 0 THEN NULL ELSE VIIAA END) AS VIIAPB, ";
  sql_stmt += "VIIIAPB /(CASE WHEN VIIIAA = 0 THEN NULL ELSE VIIIAA END) AS VIIIAPB, ";
  sql_stmt += "IXAPB /(CASE WHEN IXAA = 0 THEN NULL ELSE IXAA END) AS IXAPB, ";
  sql_stmt += "XAPB /(CASE WHEN XAA = 0 THEN NULL ELSE XAA END) AS XAPB, ";
  sql_stmt += "XIAPB /(CASE WHEN XIAA = 0 THEN NULL ELSE XIAA END) AS XIAPB, ";
//  sql_stmt += "NULL, ";
  sql_stmt += "IAPBD /(CASE WHEN IAAD = 0 THEN NULL ELSE IAAD END) AS IAPBD, ";
  sql_stmt += "IIAPBD /(CASE WHEN IIAAD = 0 THEN NULL ELSE IIAAD END) AS IIAPBD, ";
  sql_stmt += "IIIAPBD /(CASE WHEN IIIAAD = 0 THEN NULL ELSE IIIAAD END) AS IIIAPBD, ";
  sql_stmt += "IVAPBD /(CASE WHEN IVAAD = 0 THEN NULL ELSE IVAAD END) AS IXAPBD, ";
  sql_stmt += "VAPBD /(CASE WHEN VAAD = 0 THEN NULL ELSE VAAD END) AS VAPBD, ";
  sql_stmt += "VIAPBD /(CASE WHEN VIAAD = 0 THEN NULL ELSE VIAAD END) AS VIAPBD, ";
  sql_stmt += "VIIAPBD /(CASE WHEN VIIAAD = 0 THEN NULL ELSE VIIAAD END) AS VIIAPBD, ";
  sql_stmt += "VIIIAPBD /(CASE WHEN VIIIAAD = 0 THEN NULL ELSE VIIIAAD END) AS VIIIAPBD, ";
  sql_stmt += "IXAPBD /(CASE WHEN IXAAD = 0 THEN NULL ELSE IXAAD END) AS IXAPBD, ";
  sql_stmt += "XAPBD /(CASE WHEN XAAD = 0 THEN NULL ELSE XAAD END) AS XAPBD, ";
  sql_stmt += "XIAPBD /(CASE WHEN XIAAD = 0 THEN NULL ELSE XIAAD END) AS XIAPBD, ";
//  sql_stmt += "NULL, ";
  sql_stmt += "IANPV, IIANPV, IIIANPV, IVANPV, VANPV, VIANPV, VIIANPV, VIIIANPV, IXANPV, XANPV, XIANPV ";
  sql_stmt += "FROM FOUR AS B INNER JOIN ONE AS A  ";
  sql_stmt += join_string;
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_one_exist){
    if(dimension_one_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_one_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_one_content;
  }
  if(dimension_two_exist){
    if(dimension_two_type == 0)
      command->Parameters->ParamValues["DIM2"] = dimension_two_content.ToInt();
    else
      command->Parameters->ParamValues["DIM2"] = dimension_two_content;
  }
  command->Execute();

  sql_stmt = "INSERT INTO PART_FIVE ";
  sql_stmt += "(NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "NSAR, IAR, IIAR, IIIAR, IVAR, VAR, VIAR, VIIAR, VIIIAR, IXAR, XAR, XIAR, ";
  sql_stmt += "NSARD, IARD, IIARD, IIIARD, IVARD, VARD, VIARD, VIIARD, VIIIARD, IXARD, XARD, XIARD, ";
  sql_stmt += "IPB, IIPB, IIIPB, IVPB, VPB, VIPB, VIIPB, VIIIPB, IXPB, XPB, XIPB, ";
  sql_stmt += "IPBD, IIPBD, IIIPBD, IVPBD, VPBD, VIPBD, VIIPBD, VIIIPBD, IXPBD, XPBD, XIPBD, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV, ";
  sql_stmt += "NSAAR, IAAR, IIAAR, IIIAAR, IVAAR, VAAR, VIAAR, VIIAAR, VIIIAAR, IXAAR, XAAR, XIAAR, ";
  sql_stmt += "NSAARD, IAARD, IIAARD, IIIAARD, IVAARD, VAARD, VIAARD, VIIAARD, VIIIAARD, IXAARD, XAARD, XIAARD, ";
  sql_stmt += "IAPB, IIAPB, IIIAPB, IVAPB, VAPB, VIAPB, VIIAPB, VIIIAPB, IXAPB, XAPB, XIAPB, ";
  sql_stmt += "IAPBD, IIAPBD, IIIAPBD, IVAPBD, VAPBD, VIAPBD, VIIAPBD, VIIIAPBD, IXAPBD, XAPBD, XIAPBD, ";
  sql_stmt += "IANPV, IIANPV, IIIANPV, IVANPV, VANPV, VIANPV, VIIANPV, VIIIANPV, IXANPV, XANPV, XIANPV) ";
  sql_stmt += "SELECT ";
  sql_stmt += "NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "NSA / (CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END) AS NSAR, ";
  sql_stmt += "IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR, ";
  sql_stmt += "IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR, ";
  sql_stmt += "IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR, ";
  sql_stmt += "IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR, ";
  sql_stmt += "VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR, ";
  sql_stmt += "VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR, ";
  sql_stmt += "VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR, ";
  sql_stmt += "VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR, ";
  sql_stmt += "IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR, ";
  sql_stmt += "XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR, ";
  sql_stmt += "XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR, ";
  sql_stmt += "NSAD / (CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END) AS NSARD, ";
  sql_stmt += "IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD, ";
  sql_stmt += "IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD, ";
  sql_stmt += "IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD, ";
  sql_stmt += "IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD, ";
  sql_stmt += "VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD, ";
  sql_stmt += "VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD, ";
  sql_stmt += "VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD, ";
  sql_stmt += "VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD, ";
  sql_stmt += "IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD, ";
  sql_stmt += "XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD, ";
  sql_stmt += "XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD, ";
//  sql_stmt += "NULL,  ";
  sql_stmt += "IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END) AS IPB,  ";
  sql_stmt += "IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END) AS IIPB,  ";
  sql_stmt += "IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIIA END) AS IIIPB,  ";
  sql_stmt += "IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END) AS IVPB,  ";
  sql_stmt += "VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END) AS VPB,  ";
  sql_stmt += "VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END) AS VIPB,  ";
  sql_stmt += "VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END) AS VIIPB,  ";
  sql_stmt += "VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END) AS VIIIPB,  ";
  sql_stmt += "IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END) AS IXPB,  ";
  sql_stmt += "XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END) AS XPB,  ";
  sql_stmt += "XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END) AS XIPB, ";
//  sql_stmt += "NULL,  ";
  sql_stmt += "IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END) AS IPBD,  ";
  sql_stmt += "IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END) AS IIPBD,  ";
  sql_stmt += "IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END) AS IIIPBD,  ";
  sql_stmt += "IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END) AS IVPBD,  ";
  sql_stmt += "VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END) AS VPBD,  ";
  sql_stmt += "VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END) AS VIPBD,  ";
  sql_stmt += "VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END) AS VIIPBD,  ";
  sql_stmt += "VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END) AS VIIIPBD,  ";
  sql_stmt += "IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END) AS IXPBD,  ";
  sql_stmt += "XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END) AS XPBD,  ";
  sql_stmt += "XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END) AS XIPBD,  ";
//  sql_stmt += "NULL, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV, ";
  sql_stmt += "NSAA /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END), ";
  sql_stmt += "IAA /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END), ";
  sql_stmt += "IIAA /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END), ";
  sql_stmt += "IIIAA /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END), ";
  sql_stmt += "IVAA /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END), ";
  sql_stmt += "VAA /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END), ";
  sql_stmt += "VIAA /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END), ";
  sql_stmt += "VIIAA /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END), ";
  sql_stmt += "VIIIAA /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END), ";
  sql_stmt += "IXAA /(CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END), ";
  sql_stmt += "XAA /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END), ";
  sql_stmt += "XIAA /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END), ";
  sql_stmt += "NSAAD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END), ";
  sql_stmt += "IAAD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END), ";
  sql_stmt += "IIAAD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END), ";
  sql_stmt += "IIIAAD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END), ";
  sql_stmt += "IVAAD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END), ";
  sql_stmt += "VAAD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END), ";
  sql_stmt += "VIAAD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END), ";
  sql_stmt += "VIIAAD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END), ";
  sql_stmt += "VIIIAAD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END), ";
  sql_stmt += "IXAAD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END), ";
  sql_stmt += "XAAD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END), ";
  sql_stmt += "XIAAD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END), ";
//  sql_stmt += "NULL, ";
  sql_stmt += "IAPB /(CASE WHEN IAA = 0 THEN NULL ELSE IAA END) AS IAPB, ";
  sql_stmt += "IIAPB /(CASE WHEN IIAA = 0 THEN NULL ELSE IIAA END) AS IIAPB, ";
  sql_stmt += "IIIAPB /(CASE WHEN IIIAA = 0 THEN NULL ELSE IIIAA END) AS IIIAPB, ";
  sql_stmt += "IVAPB /(CASE WHEN IVAA = 0 THEN NULL ELSE IVAA END) AS IVAPB, ";
  sql_stmt += "VAPB /(CASE WHEN VAA = 0 THEN NULL ELSE VAA END) AS VAPB, ";
  sql_stmt += "VIAPB /(CASE WHEN VIAA = 0 THEN NULL ELSE VIAA END) AS VIAPB, ";
  sql_stmt += "VIIAPB /(CASE WHEN VIIAA = 0 THEN NULL ELSE VIIAA END) AS VIIAPB, ";
  sql_stmt += "VIIIAPB /(CASE WHEN VIIIAA = 0 THEN NULL ELSE VIIIAA END) AS VIIIAPB, ";
  sql_stmt += "IXAPB /(CASE WHEN IXAA = 0 THEN NULL ELSE IXAA END) AS IXAPB, ";
  sql_stmt += "XAPB /(CASE WHEN XAA = 0 THEN NULL ELSE XAA END) AS XAPB, ";
  sql_stmt += "XIAPB /(CASE WHEN XIAA = 0 THEN NULL ELSE XIAA END) AS XIAPB, ";
//  sql_stmt += "NULL, ";
  sql_stmt += "IAPBD /(CASE WHEN IAAD = 0 THEN NULL ELSE IAAD END) AS IAPBD, ";
  sql_stmt += "IIAPBD /(CASE WHEN IIAAD = 0 THEN NULL ELSE IIAAD END) AS IIAPBD, ";
  sql_stmt += "IIIAPBD /(CASE WHEN IIIAAD = 0 THEN NULL ELSE IIIAAD END) AS IIIAPBD, ";
  sql_stmt += "IVAPBD /(CASE WHEN IVAAD = 0 THEN NULL ELSE IVAAD END) AS IXAPBD, ";
  sql_stmt += "VAPBD /(CASE WHEN VAAD = 0 THEN NULL ELSE VAAD END) AS VAPBD, ";
  sql_stmt += "VIAPBD /(CASE WHEN VIAAD = 0 THEN NULL ELSE VIAAD END) AS VIAPBD, ";
  sql_stmt += "VIIAPBD /(CASE WHEN VIIAAD = 0 THEN NULL ELSE VIIAAD END) AS VIIAPBD, ";
  sql_stmt += "VIIIAPBD /(CASE WHEN VIIIAAD = 0 THEN NULL ELSE VIIIAAD END) AS VIIIAPBD, ";
  sql_stmt += "IXAPBD /(CASE WHEN IXAAD = 0 THEN NULL ELSE IXAAD END) AS IXAPBD, ";
  sql_stmt += "XAPBD /(CASE WHEN XAAD = 0 THEN NULL ELSE XAAD END) AS XAPBD, ";
  sql_stmt += "XIAPBD /(CASE WHEN XIAAD = 0 THEN NULL ELSE XIAAD END) AS XIAPBD, ";
//  sql_stmt += "NULL,";
  sql_stmt += "IANPV, IIANPV, IIIANPV, IVANPV, VANPV, VIANPV, VIIANPV, VIIIANPV, IXANPV, XANPV, XIANPV ";
  sql_stmt += "FROM FIVE AS B INNER JOIN ONE AS A  ";
  sql_stmt += join_string;
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_one_exist){
    if(dimension_one_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_one_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_one_content;
  }
  if(dimension_two_exist){
    if(dimension_two_type == 0)
      command->Parameters->ParamValues["DIM2"] = dimension_two_content.ToInt();
    else
      command->Parameters->ParamValues["DIM2"] = dimension_two_content;
  }
  command->Execute();
}



void generate_summary_report(TADOCommand *command,
                              const AnsiString &dimension,
                              unsigned int dimension_type,
                              const AnsiString &dimension_content)
{
  bool dimension_exist = dimension.Length() == 0 ? false : true;
  AnsiString condition_string = "";
  if(dimension_exist){
    condition_string += " WHERE " + dimension + " = :DIM1 ";
  }

  AnsiString trailing = "";

  AnsiString sql_stmt;
  sql_stmt = "DELETE FROM PART_ONE;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM PART_TWO;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM PART_THREE;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM PART_FOUR;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM AR;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM PART_FIVE;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO PART_ONE ";
  sql_stmt += "SELECT  ";
  sql_stmt += "SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI), ";
  sql_stmt += "SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID) ";
  sql_stmt += "FROM ONE ";
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_exist){
    if(dimension_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_content;
  }
  command->Execute();
  command->CommandText = sql_stmt;

  sql_stmt = "INSERT INTO PART_TWO ";
  sql_stmt += "(NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "NSMR, IMR, IIMR, IIIMR, IVMR, VMR, VIMR, VIIMR, VIIIMR, IXMR, XMR, XIMR, ";
  sql_stmt += "NSMRD, IMRD, IIMRD, IIIMRD, IVMRD, VMRD, VIMRD, VIIMRD, VIIIMRD, IXMRD, XMRD, XIMRD, NSAR, NSARD, ";
  sql_stmt += "IPB, IIPB, IIIPB, IVPB, VPB, VIPB, VIIPB, VIIIPB, IXPB, XPB, XIPB, ";
  sql_stmt += "IPBD, IIPBD, IIIPBD, IVPBD, VPBD, VIPBD, VIIPBD, VIIIPBD, IXPBD, XPBD, XIPBD, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV) ";
  sql_stmt += "SELECT ";
  sql_stmt += "0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA), ";
  sql_stmt += "0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD), ";
  sql_stmt += "SUM(NSMR), SUM(IMR), SUM(IIMR), SUM(IIIMR), SUM(IVMR), SUM(VMR), SUM(VIMR), SUM(VIIMR), SUM(VIIIMR), SUM(IXMR), SUM(XMR), SUM(XIMR), ";
  sql_stmt += "SUM(NSMRD), SUM(IMRD), SUM(IIMRD), SUM(IIIMRD), SUM(IVMRD), SUM(VMRD), SUM(VIMRD), SUM(VIIMRD), SUM(VIIIMRD), SUM(IXMRD), SUM(XMRD), SUM(XIMRD), ";
  sql_stmt += "0, ";
  sql_stmt += "0, ";
  sql_stmt += "SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB), ";
  sql_stmt += "SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD),  ";
  sql_stmt += "SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV)  ";
  sql_stmt += "FROM TWO ";
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_exist){
    if(dimension_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_content;
  }
  command->Execute();
  command->CommandText = sql_stmt;

  sql_stmt = "UPDATE PART_TWO ";
  sql_stmt += "SET (IAR, IIAR, IIIAR, IVAR, VAR, VIAR, VIIAR, VIIIAR, IXAR, XAR, XIAR, ";
  sql_stmt += "IARD, IIARD, IIIARD, IVARD, VARD, VIARD, VIIARD, VIIIARD, IXARD, XARD, XIARD) = ";
  sql_stmt +="(SELECT ";
  sql_stmt += "IA / (CASE WHEN I = 0 THEN NULL ELSE I END), ";
  sql_stmt += "IIA / (CASE WHEN II = 0 THEN NULL ELSE II END), ";
  sql_stmt += "IIIA / (CASE WHEN III = 0 THEN NULL ELSE III END), ";
  sql_stmt += "IVA / (CASE WHEN IV = 0 THEN NULL ELSE IV END), ";
  sql_stmt += "VA / (CASE WHEN V = 0 THEN NULL ELSE V END), ";
  sql_stmt += "VIA / (CASE WHEN VI = 0 THEN NULL ELSE VI END), ";
  sql_stmt += "VIIA / (CASE WHEN VII = 0 THEN NULL ELSE VII END), ";
  sql_stmt += "VIIIA / (CASE WHEN VIII = 0 THEN NULL ELSE VIII END), ";
  sql_stmt += "IXA / (CASE WHEN IX = 0 THEN NULL ELSE IX END), ";
  sql_stmt += "XA / (CASE WHEN X = 0 THEN NULL ELSE X END), ";
  sql_stmt += "XIA / (CASE WHEN XI = 0 THEN NULL ELSE XI END), ";
  sql_stmt += "IAD / (CASE WHEN ID = 0 THEN NULL ELSE ID END), ";
  sql_stmt += "IIAD / (CASE WHEN IID = 0 THEN NULL ELSE IID END), ";
  sql_stmt += "IIIAD / (CASE WHEN IIID = 0 THEN NULL ELSE IIID END), ";
  sql_stmt += "IVAD / (CASE WHEN IVD = 0 THEN NULL ELSE IVD END), ";
  sql_stmt += "VAD / (CASE WHEN VD = 0 THEN NULL ELSE VD END), ";
  sql_stmt += "VIAD / (CASE WHEN VID = 0 THEN NULL ELSE VID END), ";
  sql_stmt += "VIIAD / (CASE WHEN VIID = 0 THEN NULL ELSE VIID END), ";
  sql_stmt += "VIIIAD / (CASE WHEN VIIID = 0 THEN NULL ELSE VIIID END), ";
  sql_stmt += "IXAD / (CASE WHEN IXD = 0 THEN NULL ELSE IXD END), ";
  sql_stmt += "XAD / (CASE WHEN XD = 0 THEN NULL ELSE XD END), ";
  sql_stmt += "XIAD / (CASE WHEN XID = 0 THEN NULL ELSE XID END) ";
  sql_stmt += "FROM PART_ONE);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PART_TWO ";
  sql_stmt += "SET ";
  sql_stmt += "IPB = IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END), ";
  sql_stmt += "IIPB = IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END), ";
  sql_stmt += "IIIPB = IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIIA END), ";
  sql_stmt += "IVPB = IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END), ";
  sql_stmt += "VPB = VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END), ";
  sql_stmt += "VIPB = VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END), ";
  sql_stmt += "VIIPB = VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END), ";
  sql_stmt += "VIIIPB = VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END), ";
  sql_stmt += "IXPB = IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END), ";
  sql_stmt += "XPB = XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END),";
  sql_stmt += "XIPB = XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END), ";
  sql_stmt += "IPBD = IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END), ";
  sql_stmt += "IIPBD = IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END), ";
  sql_stmt += "IIIPBD = IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END), ";
  sql_stmt += "IVPBD = IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END), ";
  sql_stmt += "VPBD = VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END), ";
  sql_stmt += "VIPBD = VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END), ";
  sql_stmt += "VIIPBD = VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END), ";
  sql_stmt += "VIIIPBD = VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END), ";
  sql_stmt += "IXPBD = IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END), ";
  sql_stmt += "XPBD = XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END),";
  sql_stmt += "XIPBD = XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO PART_THREE ";
  sql_stmt += "(NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, NSAR, NSARD, ";
  sql_stmt += "IPB, IIPB, IIIPB, IVPB, VPB, VIPB, VIIPB, VIIIPB, IXPB, XPB, XIPB, ";
  sql_stmt += "IPBD, IIPBD, IIIPBD, IVPBD, VPBD, VIPBD, VIIPBD, VIIIPBD, IXPBD, XPBD, XIPBD, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV) ";
  sql_stmt += "SELECT ";
  sql_stmt += "0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA), ";
  sql_stmt += "0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD), ";
  sql_stmt += "0, ";
  sql_stmt += "0, ";
  sql_stmt += "SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB), ";
  sql_stmt += "SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD),  ";
  sql_stmt += "SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV)  ";
  sql_stmt += "FROM THREE ";
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_exist){
    if(dimension_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_content;
  }
  command->Execute();

  sql_stmt = "UPDATE PART_THREE ";
  sql_stmt += "SET (IAR, IIAR, IIIAR, IVAR, VAR, VIAR, VIIAR, VIIIAR, IXAR, XAR, XIAR, ";
  sql_stmt += "IARD, IIARD, IIIARD, IVARD, VARD, VIARD, VIIARD, VIIIARD, IXARD, XARD, XIARD) = ";
  sql_stmt +="(SELECT ";
  sql_stmt += "IA / (CASE WHEN I = 0 THEN NULL ELSE I END), ";
  sql_stmt += "IIA / (CASE WHEN II = 0 THEN NULL ELSE II END), ";
  sql_stmt += "IIIA / (CASE WHEN III = 0 THEN NULL ELSE III END), ";
  sql_stmt += "IVA / (CASE WHEN IV = 0 THEN NULL ELSE IV END), ";
  sql_stmt += "VA / (CASE WHEN V = 0 THEN NULL ELSE V END), ";
  sql_stmt += "VIA / (CASE WHEN VI = 0 THEN NULL ELSE VI END), ";
  sql_stmt += "VIIA / (CASE WHEN VII = 0 THEN NULL ELSE VII END), ";
  sql_stmt += "VIIIA / (CASE WHEN VIII = 0 THEN NULL ELSE VIII END), ";
  sql_stmt += "IXA / (CASE WHEN IX = 0 THEN NULL ELSE IX END), ";
  sql_stmt += "XA / (CASE WHEN X = 0 THEN NULL ELSE X END), ";
  sql_stmt += "XIA / (CASE WHEN XI = 0 THEN NULL ELSE XI END), ";
  sql_stmt += "IAD / (CASE WHEN ID = 0 THEN NULL ELSE ID END), ";
  sql_stmt += "IIAD / (CASE WHEN IID = 0 THEN NULL ELSE IID END), ";
  sql_stmt += "IIIAD / (CASE WHEN IIID = 0 THEN NULL ELSE IIID END), ";
  sql_stmt += "IVAD / (CASE WHEN IVD = 0 THEN NULL ELSE IVD END), ";
  sql_stmt += "VAD / (CASE WHEN VD = 0 THEN NULL ELSE VD END), ";
  sql_stmt += "VIAD / (CASE WHEN VID = 0 THEN NULL ELSE VID END), ";
  sql_stmt += "VIIAD / (CASE WHEN VIID = 0 THEN NULL ELSE VIID END), ";
  sql_stmt += "VIIIAD / (CASE WHEN VIIID = 0 THEN NULL ELSE VIIID END), ";
  sql_stmt += "IXAD / (CASE WHEN IXD = 0 THEN NULL ELSE IXD END), ";
  sql_stmt += "XAD / (CASE WHEN XD = 0 THEN NULL ELSE XD END), ";
  sql_stmt += "XIAD / (CASE WHEN XID = 0 THEN NULL ELSE XID END) ";
  sql_stmt += "FROM PART_ONE);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PART_THREE ";
  sql_stmt += "SET ";
  sql_stmt += "IPB = IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END), ";
  sql_stmt += "IIPB = IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END), ";
  sql_stmt += "IIIPB = IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIIA END), ";
  sql_stmt += "IVPB = IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END), ";
  sql_stmt += "VPB = VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END), ";
  sql_stmt += "VIPB = VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END), ";
  sql_stmt += "VIIPB = VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END), ";
  sql_stmt += "VIIIPB = VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END), ";
  sql_stmt += "IXPB = IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END), ";
  sql_stmt += "XPB = XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END),";
  sql_stmt += "XIPB = XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END), ";
  sql_stmt += "IPBD = IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END), ";
  sql_stmt += "IIPBD = IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END), ";
  sql_stmt += "IIIPBD = IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END), ";
  sql_stmt += "IVPBD = IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END), ";
  sql_stmt += "VPBD = VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END), ";
  sql_stmt += "VIPBD = VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END), ";
  sql_stmt += "VIIPBD = VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END), ";
  sql_stmt += "VIIIPBD = VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END), ";
  sql_stmt += "IXPBD = IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END), ";
  sql_stmt += "XPBD = XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END),";
  sql_stmt += "XIPBD = XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO AR ";
  sql_stmt += "SELECT  ";
  sql_stmt += "SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI), ";
  sql_stmt += "SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID) ";
  sql_stmt += "FROM FOUR_AR ";
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_exist){
    if(dimension_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_content;
  }
  command->Execute();
  command->CommandText = sql_stmt;

  sql_stmt = "INSERT INTO PART_FOUR ";
  sql_stmt += "(NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, ";
  sql_stmt += "IPB, IIPB, IIIPB, IVPB, VPB, VIPB, VIIPB, VIIIPB, IXPB, XPB, XIPB, ";
  sql_stmt += "IPBD, IIPBD, IIIPBD, IVPBD, VPBD, VIPBD, VIIPBD, VIIIPBD, IXPBD, XPBD, XIPBD, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV, ";
  sql_stmt += "NSAAR, IAAR, IIAAR, IIIAAR, IVAAR, VAAR, VIAAR, VIIAAR, VIIIAAR, IXAAR, XAAR, XIAAR, ";
  sql_stmt += "NSAARD, IAARD, IIAARD, IIIAARD, IVAARD, VAARD, VIAARD, VIIAARD, VIIIAARD, IXAARD, XAARD, XIAARD, ";
  sql_stmt += "IAPB, IIAPB, IIIAPB, IVAPB, VAPB, VIAPB, VIIAPB, VIIIAPB, IXAPB, XAPB, XIAPB, ";
  sql_stmt += "IAPBD, IIAPBD, IIIAPBD, IVAPBD, VAPBD, VIAPBD, VIIAPBD, VIIIAPBD, IXAPBD, XAPBD, XIAPBD, ";
  sql_stmt += "IANPV, IIANPV, IIIANPV, IVANPV, VANPV, VIANPV, VIIANPV, VIIIANPV, IXANPV, XANPV, XIANPV) ";
  sql_stmt += "SELECT ";
  sql_stmt += "SUM(NSA), SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA), ";
  sql_stmt += "SUM(NSAD), SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD), ";
  sql_stmt += "SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB), ";
  sql_stmt += "SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD),  ";
  sql_stmt += "SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV),  ";
  sql_stmt += "SUM(NSAA), SUM(IAA), SUM(IIAA), SUM(IIIAA), SUM(IVAA), SUM(VAA), SUM(VIAA), SUM(VIIAA), SUM(VIIIAA), SUM(IXAA), SUM(XAA), SUM(XIAA), ";
  sql_stmt += "SUM(NSAAD), SUM(IAAD), SUM(IIAAD), SUM(IIIAAD), SUM(IVAAD), SUM(VAAD), SUM(VIAAD), SUM(VIIAAD), SUM(VIIIAAD), SUM(IXAAD), SUM(XAAD), SUM(XIAAD), ";
  sql_stmt += "SUM(IAPB), SUM(IIAPB), SUM(IIIAPB), SUM(IVAPB), SUM(VAPB), SUM(VIAPB), SUM(VIIAPB), SUM(VIIIAPB), SUM(IXAPB), SUM(XPB), SUM(XIAPB), ";
  sql_stmt += "SUM(IAPBD), SUM(IIAPBD), SUM(IIIAPBD), SUM(IVAPBD), SUM(VAPBD), SUM(VIAPBD), SUM(VIIAPBD), SUM(VIIIAPBD), SUM(IXAPBD), SUM(XAPBD), SUM(XIAPBD), ";
  sql_stmt += "SUM(IANPV), SUM(IIANPV), SUM(IIIANPV), SUM(IVANPV), SUM(VANPV), SUM(VIANPV), SUM(VIIANPV), SUM(VIIIANPV), SUM(IXANPV), SUM(XANPV), SUM(XIANPV) ";
  sql_stmt += "FROM FOUR ";
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_exist){
    if(dimension_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_content;
  }
  command->Execute();
  command->CommandText = sql_stmt;

  sql_stmt = "UPDATE PART_FOUR ";
  sql_stmt += "SET (NSAR, IAR, IIAR, IIIAR, IVAR, VAR, VIAR, VIIAR, VIIIAR, IXAR, XAR, XIAR, ";
  sql_stmt += "NSARD, IARD, IIARD, IIIARD, IVARD, VARD, VIARD, VIIARD, VIIIARD, IXARD, XARD, XIARD) = ";
  sql_stmt += "(SELECT ";
  sql_stmt += "NSA / (CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END), ";
  sql_stmt += "IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END), ";
  sql_stmt += "IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END), ";
  sql_stmt += "IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END), ";
  sql_stmt += "IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END), ";
  sql_stmt += "VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END), ";
  sql_stmt += "VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END), ";
  sql_stmt += "VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END), ";
  sql_stmt += "VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END), ";
  sql_stmt += "IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END), ";
  sql_stmt += "XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END), ";
  sql_stmt += "XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END), ";
  sql_stmt += "NSAD / (CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END), ";
  sql_stmt += "IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END), ";
  sql_stmt += "IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END), ";
  sql_stmt += "IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END), ";
  sql_stmt += "IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END), ";
  sql_stmt += "VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END), ";
  sql_stmt += "VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END), ";
  sql_stmt += "VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END), ";
  sql_stmt += "VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END), ";
  sql_stmt += "IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END), ";
  sql_stmt += "XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END), ";
  sql_stmt += "XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) ";
  sql_stmt += "FROM PART_ONE AS A);";
  command->CommandText = sql_stmt;
  command->Execute();


  sql_stmt = "UPDATE PART_FOUR ";
  sql_stmt += "SET ";
  sql_stmt += "IAPB = IAPB /(CASE WHEN IAAR = 0 THEN NULL ELSE IAAR END), ";
  sql_stmt += "IIAPB = IIAPB /(CASE WHEN IIAAR = 0 THEN NULL ELSE IIAAR END), ";
  sql_stmt += "IIIAPB = IIIAPB /(CASE WHEN IIIAAR = 0 THEN NULL ELSE IIIAAR END), ";
  sql_stmt += "IVAPB = IVAPB /(CASE WHEN IVAAR = 0 THEN NULL ELSE IVAAR END), ";
  sql_stmt += "VAPB = VAPB /(CASE WHEN VAAR = 0 THEN NULL ELSE VAAR END), ";
  sql_stmt += "VIAPB = VIAPB /(CASE WHEN VIAAR = 0 THEN NULL ELSE VIAAR END), ";
  sql_stmt += "VIIAPB = VIIAPB /(CASE WHEN VIIAAR = 0 THEN NULL ELSE VIIAAR END), ";
  sql_stmt += "VIIIAPB = VIIIAPB /(CASE WHEN VIIIAAR = 0 THEN NULL ELSE VIIIAAR END), ";
  sql_stmt += "IXAPB = IXAPB /(CASE WHEN IXAAR = 0 THEN NULL ELSE IXAAR END), ";
  sql_stmt += "XAPB = XAPB /(CASE WHEN XAAR = 0 THEN NULL ELSE XAAR END), ";
  sql_stmt += "XIAPB = XIAPB /(CASE WHEN XIAAR = 0 THEN NULL ELSE XIAAR END), ";
  sql_stmt += "IAPBD = IAPBD /(CASE WHEN IAARD = 0 THEN NULL ELSE IAARD END), ";
  sql_stmt += "IIAPBD = IIAPBD /(CASE WHEN IIAARD = 0 THEN NULL ELSE IIAARD END), ";
  sql_stmt += "IIIAPBD = IIIAPBD /(CASE WHEN IIIAARD = 0 THEN NULL ELSE IIIAARD END), ";
  sql_stmt += "IVAPBD = IVAPBD /(CASE WHEN IVAARD = 0 THEN NULL ELSE IVAARD END), ";
  sql_stmt += "VAPBD = VAPBD /(CASE WHEN VAARD = 0 THEN NULL ELSE VAARD END), ";
  sql_stmt += "VIAPBD = VIAPBD /(CASE WHEN VIAARD = 0 THEN NULL ELSE VIAARD END), ";
  sql_stmt += "VIIAPBD = VIIAPBD /(CASE WHEN VIIAARD = 0 THEN NULL ELSE VIIAARD END), ";
  sql_stmt += "VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAARD = 0 THEN NULL ELSE VIIIAARD END), ";
  sql_stmt += "IXAPBD = IXAPBD /(CASE WHEN IXAARD = 0 THEN NULL ELSE IXAARD END), ";
  sql_stmt += "XAPBD = XAPBD /(CASE WHEN XAARD = 0 THEN NULL ELSE XAARD END), ";
  sql_stmt += "XIAPBD = XIAPBD /(CASE WHEN XIAARD = 0 THEN NULL ELSE XIAARD END);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PART_FOUR ";
  sql_stmt += "SET (NSAAR, IAAR, IIAAR, IIIAAR, IVAAR, VAAR, VIAAR, VIIAAR, VIIIAAR, IXAAR, XAAR, XIAAR, ";
  sql_stmt += "NSAARD, IAARD, IIAARD, IIIAARD, IVAARD, VAARD, VIAARD, VIIAARD, VIIIAARD, IXAARD, XAARD, XIAARD) = ";
  sql_stmt += "(SELECT ";
  sql_stmt += "NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END), ";
  sql_stmt += "IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END), ";
  sql_stmt += "IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END), ";
  sql_stmt += "IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END), ";
  sql_stmt += "IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END), ";
  sql_stmt += "VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END), ";
  sql_stmt += "VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END), ";
  sql_stmt += "VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END), ";
  sql_stmt += "VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END), ";
  sql_stmt += "IXAAR /(CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END), ";
  sql_stmt += "XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END), ";
  sql_stmt += "XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END), ";
  sql_stmt += "NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END), ";
  sql_stmt += "IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END), ";
  sql_stmt += "IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END), ";
  sql_stmt += "IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END), ";
  sql_stmt += "IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END), ";
  sql_stmt += "VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END), ";
  sql_stmt += "VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END), ";
  sql_stmt += "VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END), ";
  sql_stmt += "VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END), ";
  sql_stmt += "IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END), ";
  sql_stmt += "XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END), ";
  sql_stmt += "XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) ";
  sql_stmt += "FROM PART_ONE AS A)";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PART_FOUR ";
  sql_stmt += "SET ";
  sql_stmt += "IPB = IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END), ";
  sql_stmt += "IIPB = IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END), ";
  sql_stmt += "IIIPB = IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIIA END), ";
  sql_stmt += "IVPB = IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END), ";
  sql_stmt += "VPB = VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END), ";
  sql_stmt += "VIPB = VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END), ";
  sql_stmt += "VIIPB = VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END), ";
  sql_stmt += "VIIIPB = VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END), ";
  sql_stmt += "IXPB = IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END), ";
  sql_stmt += "XPB = XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END),";
  sql_stmt += "XIPB = XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END), ";
  sql_stmt += "IPBD = IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END), ";
  sql_stmt += "IIPBD = IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END), ";
  sql_stmt += "IIIPBD = IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END), ";
  sql_stmt += "IVPBD = IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END), ";
  sql_stmt += "VPBD = VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END), ";
  sql_stmt += "VIPBD = VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END), ";
  sql_stmt += "VIIPBD = VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END), ";
  sql_stmt += "VIIIPBD = VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END), ";
  sql_stmt += "IXPBD = IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END), ";
  sql_stmt += "XPBD = XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END),";
  sql_stmt += "XIPBD = XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM AR;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO AR ";
  sql_stmt += "SELECT  ";
  sql_stmt += "SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI), ";
  sql_stmt += "SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID) ";
  sql_stmt += "FROM FIVE_AR ";
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_exist){
    if(dimension_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_content;
  }
  command->Execute();
  command->CommandText = sql_stmt;

  sql_stmt = "INSERT INTO PART_FIVE ";
  sql_stmt += "(NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA, ";
  sql_stmt += "NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD, NSAR, NSARD, ";
  sql_stmt += "IPB, IIPB, IIIPB, IVPB, VPB, VIPB, VIIPB, VIIIPB, IXPB, XPB, XIPB, ";
  sql_stmt += "IPBD, IIPBD, IIIPBD, IVPBD, VPBD, VIPBD, VIIPBD, VIIIPBD, IXPBD, XPBD, XIPBD, ";
  sql_stmt += "INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV, ";
  sql_stmt += "NSAAR, IAAR, IIAAR, IIIAAR, IVAAR, VAAR, VIAAR, VIIAAR, VIIIAAR, IXAAR, XAAR, XIAAR, ";
  sql_stmt += "NSAARD, IAARD, IIAARD, IIIAARD, IVAARD, VAARD, VIAARD, VIIAARD, VIIIAARD, IXAARD, XAARD, XIAARD, ";
  sql_stmt += "IAPB, IIAPB, IIIAPB, IVAPB, VAPB, VIAPB, VIIAPB, VIIIAPB, IXAPB, XAPB, XIAPB, ";
  sql_stmt += "IAPBD, IIAPBD, IIIAPBD, IVAPBD, VAPBD, VIAPBD, VIIAPBD, VIIIAPBD, IXAPBD, XAPBD, XIAPBD, ";
  sql_stmt += "IANPV, IIANPV, IIIANPV, IVANPV, VANPV, VIANPV, VIIANPV, VIIIANPV, IXANPV, XANPV, XIANPV) ";
  sql_stmt += "SELECT ";
  sql_stmt += "SUM(NSA), SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA), ";
  sql_stmt += "SUM(NSAD), SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD), ";
  sql_stmt += "0, ";
  sql_stmt += "0, ";
  sql_stmt += "SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB), ";
  sql_stmt += "SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD),  ";
  sql_stmt += "SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV),  ";
  sql_stmt += "SUM(NSAA), SUM(IAA), SUM(IIAA), SUM(IIIAA), SUM(IVAA), SUM(VAA), SUM(VIAA), SUM(VIIAA), SUM(VIIIAA), SUM(IXAA), SUM(XAA), SUM(XIAA), ";
  sql_stmt += "SUM(NSAAD), SUM(IAAD), SUM(IIAAD), SUM(IIIAAD), SUM(IVAAD), SUM(VAAD), SUM(VIAAD), SUM(VIIAAD), SUM(VIIIAAD), SUM(IXAAD), SUM(XAAD), SUM(XIAAD), ";
  sql_stmt += "SUM(IAPB), SUM(IIAPB), SUM(IIIAPB), SUM(IVAPB), SUM(VAPB), SUM(VIAPB), SUM(VIIAPB), SUM(VIIIAPB), SUM(IXAPB), SUM(XAPB), SUM(XIAPB), ";
  sql_stmt += "SUM(IAPBD), SUM(IIAPBD), SUM(IIIAPBD), SUM(IVAPBD), SUM(VAPBD), SUM(VIAPBD), SUM(VIIAPBD), SUM(VIIIAPBD), SUM(IXAPBD), SUM(XAPBD), SUM(XIAPBD), ";
  sql_stmt += "SUM(IANPV), SUM(IIANPV), SUM(IIIANPV), SUM(IVANPV), SUM(VANPV), SUM(VIANPV), SUM(VIIANPV), SUM(VIIIANPV), SUM(IXANPV), SUM(XANPV), SUM(XIANPV) ";
  sql_stmt += "FROM FIVE ";
  sql_stmt += condition_string;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(dimension_exist){
    if(dimension_type == 0)
      command->Parameters->ParamValues["DIM1"] = dimension_content.ToInt();
    else
      command->Parameters->ParamValues["DIM1"] = dimension_content;
  }
  command->Execute();
  command->CommandText = sql_stmt;

  sql_stmt = "UPDATE PART_FIVE ";
  sql_stmt += "SET (NSAR, IAR, IIAR, IIIAR, IVAR, VAR, VIAR, VIIAR, VIIIAR, IXAR, XAR, XIAR, ";
  sql_stmt += "NSARD, IARD, IIARD, IIIARD, IVARD, VARD, VIARD, VIIARD, VIIIARD, IXARD, XARD, XIARD) = ";
  sql_stmt += "(SELECT ";
  sql_stmt += "NSA / (CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END), ";
  sql_stmt += "IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END), ";
  sql_stmt += "IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END), ";
  sql_stmt += "IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END), ";
  sql_stmt += "IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END), ";
  sql_stmt += "VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END), ";
  sql_stmt += "VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END), ";
  sql_stmt += "VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END), ";
  sql_stmt += "VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END), ";
  sql_stmt += "IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END), ";
  sql_stmt += "XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END), ";
  sql_stmt += "XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END), ";
  sql_stmt += "NSAD / (CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END), ";
  sql_stmt += "IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END), ";
  sql_stmt += "IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END), ";
  sql_stmt += "IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END), ";
  sql_stmt += "IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END), ";
  sql_stmt += "VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END), ";
  sql_stmt += "VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END), ";
  sql_stmt += "VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END), ";
  sql_stmt += "VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END), ";
  sql_stmt += "IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END), ";
  sql_stmt += "XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END), ";
  sql_stmt += "XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) ";
  sql_stmt += "FROM PART_ONE AS A);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PART_FIVE ";
  sql_stmt += "SET ";
  sql_stmt += "IAPB = IAPB /(CASE WHEN IAAR = 0 THEN NULL ELSE IAAR END), ";
  sql_stmt += "IIAPB = IIAPB /(CASE WHEN IIAAR = 0 THEN NULL ELSE IIAAR END), ";
  sql_stmt += "IIIAPB = IIIAPB /(CASE WHEN IIIAAR = 0 THEN NULL ELSE IIIAAR END), ";
  sql_stmt += "IVAPB = IVAPB /(CASE WHEN IVAAR = 0 THEN NULL ELSE IVAAR END), ";
  sql_stmt += "VAPB = VAPB /(CASE WHEN VAAR = 0 THEN NULL ELSE VAAR END), ";
  sql_stmt += "VIAPB = VIAPB /(CASE WHEN VIAAR = 0 THEN NULL ELSE VIAAR END), ";
  sql_stmt += "VIIAPB = VIIAPB /(CASE WHEN VIIAAR = 0 THEN NULL ELSE VIIAAR END), ";
  sql_stmt += "VIIIAPB = VIIIAPB /(CASE WHEN VIIIAAR = 0 THEN NULL ELSE VIIIAAR END), ";
  sql_stmt += "IXAPB = IXAPB /(CASE WHEN IXAAR = 0 THEN NULL ELSE IXAAR END), ";
  sql_stmt += "XAPB = XAPB /(CASE WHEN XAAR = 0 THEN NULL ELSE XAAR END), ";
  sql_stmt += "XIAPB = XIAPB /(CASE WHEN XIAAR = 0 THEN NULL ELSE XIAAR END), ";
  sql_stmt += "IAPBD = IAPBD /(CASE WHEN IAARD = 0 THEN NULL ELSE IAARD END), ";
  sql_stmt += "IIAPBD = IIAPBD /(CASE WHEN IIAARD = 0 THEN NULL ELSE IIAARD END), ";
  sql_stmt += "IIIAPBD = IIIAPBD /(CASE WHEN IIIAARD = 0 THEN NULL ELSE IIIAARD END), ";
  sql_stmt += "IVAPBD = IVAPBD /(CASE WHEN IVAARD = 0 THEN NULL ELSE IVAARD END), ";
  sql_stmt += "VAPBD = VAPBD /(CASE WHEN VAARD = 0 THEN NULL ELSE VAARD END), ";
  sql_stmt += "VIAPBD = VIAPBD /(CASE WHEN VIAARD = 0 THEN NULL ELSE VIAARD END), ";
  sql_stmt += "VIIAPBD = VIIAPBD /(CASE WHEN VIIAARD = 0 THEN NULL ELSE VIIAARD END), ";
  sql_stmt += "VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAARD = 0 THEN NULL ELSE VIIIAARD END), ";
  sql_stmt += "IXAPBD = IXAPBD /(CASE WHEN IXAARD = 0 THEN NULL ELSE IXAARD END), ";
  sql_stmt += "XAPBD = XAPBD /(CASE WHEN XAARD = 0 THEN NULL ELSE XAARD END), ";
  sql_stmt += "XIAPBD = XIAPBD /(CASE WHEN XIAARD = 0 THEN NULL ELSE XIAARD END);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PART_FIVE ";
  sql_stmt += "SET (NSAAR, IAAR, IIAAR, IIIAAR, IVAAR, VAAR, VIAAR, VIIAAR, VIIIAAR, IXAAR, XAAR, XIAAR, ";
  sql_stmt += "NSAARD, IAARD, IIAARD, IIIAARD, IVAARD, VAARD, VIAARD, VIIAARD, VIIIAARD, IXAARD, XAARD, XIAARD) = ";
  sql_stmt += "(SELECT ";
  sql_stmt += "NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END), ";
  sql_stmt += "IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END), ";
  sql_stmt += "IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END), ";
  sql_stmt += "IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END), ";
  sql_stmt += "IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END), ";
  sql_stmt += "VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END), ";
  sql_stmt += "VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END), ";
  sql_stmt += "VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END), ";
  sql_stmt += "VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END), ";
  sql_stmt += "IXAAR /(CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END), ";
  sql_stmt += "XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END), ";
  sql_stmt += "XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END), ";
  sql_stmt += "NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END), ";
  sql_stmt += "IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END), ";
  sql_stmt += "IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END), ";
  sql_stmt += "IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END), ";
  sql_stmt += "IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END), ";
  sql_stmt += "VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END), ";
  sql_stmt += "VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END), ";
  sql_stmt += "VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END), ";
  sql_stmt += "VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END), ";
  sql_stmt += "IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END), ";
  sql_stmt += "XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END), ";
  sql_stmt += "XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) ";
  sql_stmt += "FROM PART_ONE AS A)";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PART_FIVE ";
  sql_stmt += "SET ";
  sql_stmt += "IPB = IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END), ";
  sql_stmt += "IIPB = IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END), ";
  sql_stmt += "IIIPB = IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIIA END), ";
  sql_stmt += "IVPB = IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END), ";
  sql_stmt += "VPB = VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END), ";
  sql_stmt += "VIPB = VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END), ";
  sql_stmt += "VIIPB = VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END), ";
  sql_stmt += "VIIIPB = VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END), ";
  sql_stmt += "IXPB = IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END), ";
  sql_stmt += "XPB = XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END),";
  sql_stmt += "XIPB = XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END), ";
  sql_stmt += "IPBD = IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END), ";
  sql_stmt += "IIPBD = IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END), ";
  sql_stmt += "IIIPBD = IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END), ";
  sql_stmt += "IVPBD = IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END), ";
  sql_stmt += "VPBD = VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END), ";
  sql_stmt += "VIPBD = VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END), ";
  sql_stmt += "VIIPBD = VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END), ";
  sql_stmt += "VIIIPBD = VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END), ";
  sql_stmt += "IXPBD = IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END), ";
  sql_stmt += "XPBD = XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END),";
  sql_stmt += "XIPBD = XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END);";
  command->CommandText = sql_stmt;
  command->Execute();
}

void write_part_one(TADOQuery *query, ofstream &report)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM PART_ONE;";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Open();
  report << "申貸數" << ",";
  report << query->FieldByName("NS")->AsFloat << ",";
  report << query->FieldByName("I")->AsFloat << ",";
  report << query->FieldByName("II")->AsFloat << ",";
  report << query->FieldByName("III")->AsFloat << ",";
  report << query->FieldByName("IV")->AsFloat << ",";
  report << query->FieldByName("V")->AsFloat << ",";
  report << query->FieldByName("VI")->AsFloat << ",";
  report << query->FieldByName("VII")->AsFloat << ",";
  report << query->FieldByName("VIII")->AsFloat << ",";
  report << query->FieldByName("IX")->AsFloat << ",";
  report << query->FieldByName("X")->AsFloat << ",";
  report << query->FieldByName("XI")->AsFloat << endl;
  report << "申貸金額" << ",";
  report << query->FieldByName("NSD")->AsFloat << ",";
  report << query->FieldByName("ID")->AsFloat << ",";
  report << query->FieldByName("IID")->AsFloat << ",";
  report << query->FieldByName("IIID")->AsFloat << ",";
  report << query->FieldByName("IVD")->AsFloat << ",";
  report << query->FieldByName("VD")->AsFloat << ",";
  report << query->FieldByName("VID")->AsFloat << ",";
  report << query->FieldByName("VIID")->AsFloat << ",";
  report << query->FieldByName("VIIID")->AsFloat << ",";
  report << query->FieldByName("IXD")->AsFloat << ",";
  report << query->FieldByName("XD")->AsFloat << ",";
  report << query->FieldByName("XID")->AsFloat << endl;
  query->Close();
}

void write_part_two(TADOQuery *query, ofstream &report)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM PART_TWO;";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Open();
  report << "核准模組結果" << endl;
  report << "建議核准數" << ",";
  report << 0 << ",";
  report << query->FieldByName("IA")->AsFloat << ",";
  report << query->FieldByName("IIA")->AsFloat << ",";
  report << query->FieldByName("IIIA")->AsFloat << ",";
  report << query->FieldByName("IVA")->AsFloat << ",";
  report << query->FieldByName("VA")->AsFloat << ",";
  report << query->FieldByName("VIA")->AsFloat << ",";
  report << query->FieldByName("VIIA")->AsFloat << ",";
  report << query->FieldByName("VIIIA")->AsFloat << ",";
  report << query->FieldByName("IXA")->AsFloat << ",";
  report << query->FieldByName("XA")->AsFloat << ",";
  report << query->FieldByName("XIA")->AsFloat << endl;
  report << "建議核准金額" << ",";
  report << 0 << ",";
  report << query->FieldByName("IAD")->AsFloat << ",";
  report << query->FieldByName("IIAD")->AsFloat << ",";
  report << query->FieldByName("IIIAD")->AsFloat << ",";
  report << query->FieldByName("IVAD")->AsFloat << ",";
  report << query->FieldByName("VAD")->AsFloat << ",";
  report << query->FieldByName("VIAD")->AsFloat << ",";
  report << query->FieldByName("VIIAD")->AsFloat << ",";
  report << query->FieldByName("VIIIAD")->AsFloat << ",";
  report << query->FieldByName("IXAD")->AsFloat << ",";
  report << query->FieldByName("XAD")->AsFloat << ",";
  report << query->FieldByName("XIAD")->AsFloat << endl;
  report << "建議人工授信數" << ",";
  report << query->FieldByName("NSMR")->AsFloat << ",";
  report << query->FieldByName("IMR")->AsFloat << ",";
  report << query->FieldByName("IIMR")->AsFloat << ",";
  report << query->FieldByName("IIIMR")->AsFloat << ",";
  report << query->FieldByName("IVMR")->AsFloat << ",";
  report << query->FieldByName("VMR")->AsFloat << ",";
  report << query->FieldByName("VIMR")->AsFloat << ",";
  report << query->FieldByName("VIIMR")->AsFloat << ",";
  report << query->FieldByName("VIIIMR")->AsFloat << ",";
  report << query->FieldByName("IXMR")->AsFloat << ",";
  report << query->FieldByName("XMR")->AsFloat << ",";
  report << query->FieldByName("XIMR")->AsFloat << endl;
  report << "建議人工授信金額" << ",";
  report << query->FieldByName("NSMRD")->AsFloat << ",";
  report << query->FieldByName("IMRD")->AsFloat << ",";
  report << query->FieldByName("IIMRD")->AsFloat << ",";
  report << query->FieldByName("IIIMRD")->AsFloat << ",";
  report << query->FieldByName("IVMRD")->AsFloat << ",";
  report << query->FieldByName("VMRD")->AsFloat << ",";
  report << query->FieldByName("VIMRD")->AsFloat << ",";
  report << query->FieldByName("VIIMRD")->AsFloat << ",";
  report << query->FieldByName("VIIIMRD")->AsFloat << ",";
  report << query->FieldByName("IXMRD")->AsFloat << ",";
  report << query->FieldByName("XMRD")->AsFloat << ",";
  report << query->FieldByName("XIMRD")->AsFloat << endl;
  report << "申貸件核准率" << ",";
  report << 0 << ",";
  report << query->FieldByName("IAR")->AsFloat << ",";
  report << query->FieldByName("IIAR")->AsFloat << ",";
  report << query->FieldByName("IIIAR")->AsFloat << ",";
  report << query->FieldByName("IVAR")->AsFloat << ",";
  report << query->FieldByName("VAR")->AsFloat << ",";
  report << query->FieldByName("VIAR")->AsFloat << ",";
  report << query->FieldByName("VIIAR")->AsFloat << ",";
  report << query->FieldByName("VIIIAR")->AsFloat << ",";
  report << query->FieldByName("IXAR")->AsFloat << ",";
  report << query->FieldByName("XAR")->AsFloat << ",";
  report << query->FieldByName("XIAR")->AsFloat << endl;
  report << "申貸金額核准率" << ",";
  report << 0 << ",";
  report << query->FieldByName("IARD")->AsFloat << ",";
  report << query->FieldByName("IIARD")->AsFloat << ",";
  report << query->FieldByName("IIIARD")->AsFloat << ",";
  report << query->FieldByName("IVARD")->AsFloat << ",";
  report << query->FieldByName("VARD")->AsFloat << ",";
  report << query->FieldByName("VIARD")->AsFloat << ",";
  report << query->FieldByName("VIIARD")->AsFloat << ",";
  report << query->FieldByName("VIIIARD")->AsFloat << ",";
  report << query->FieldByName("IXARD")->AsFloat << ",";
  report << query->FieldByName("XARD")->AsFloat << ",";
  report << query->FieldByName("XIARD")->AsFloat << endl;
  report << "加權平均壞帳機率(戶數)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IPB")->AsFloat << ",";
  report << query->FieldByName("IIPB")->AsFloat << ",";
  report << query->FieldByName("IIIPB")->AsFloat << ",";
  report << query->FieldByName("IVPB")->AsFloat << ",";
  report << query->FieldByName("VPB")->AsFloat << ",";
  report << query->FieldByName("VIPB")->AsFloat << ",";
  report << query->FieldByName("VIIPB")->AsFloat << ",";
  report << query->FieldByName("VIIIPB")->AsFloat << ",";
  report << query->FieldByName("IXPB")->AsFloat << ",";
  report << query->FieldByName("XPB")->AsFloat << ",";
  report << query->FieldByName("XIPB")->AsFloat << endl;
  report << "加權平均壞帳機率(金額)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IPBD")->AsFloat << ",";
  report << query->FieldByName("IIPBD")->AsFloat << ",";
  report << query->FieldByName("IIIPBD")->AsFloat << ",";
  report << query->FieldByName("IVPBD")->AsFloat << ",";
  report << query->FieldByName("VPBD")->AsFloat << ",";
  report << query->FieldByName("VIPBD")->AsFloat << ",";
  report << query->FieldByName("VIIPBD")->AsFloat << ",";
  report << query->FieldByName("VIIIPBD")->AsFloat << ",";
  report << query->FieldByName("IXPBD")->AsFloat << ",";
  report << query->FieldByName("XPBD")->AsFloat << ",";
  report << query->FieldByName("XIPBD")->AsFloat << endl;
  report << "總核准經濟價值(NPV)" << ",";
  report << "---" << ",";
  report << query->FieldByName("INPV")->AsFloat << ",";
  report << query->FieldByName("IINPV")->AsFloat << ",";
  report << query->FieldByName("IIINPV")->AsFloat << ",";
  report << query->FieldByName("IVNPV")->AsFloat << ",";
  report << query->FieldByName("VNPV")->AsFloat << ",";
  report << query->FieldByName("VINPV")->AsFloat << ",";
  report << query->FieldByName("VIINPV")->AsFloat << ",";
  report << query->FieldByName("VIIINPV")->AsFloat << ",";
  report << query->FieldByName("IXNPV")->AsFloat << ",";
  report << query->FieldByName("XNPV")->AsFloat << ",";
  report << query->FieldByName("XINPV")->AsFloat << endl;
  query->Close();
}

void write_part_three(TADOQuery *query, ofstream &report)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM PART_THREE;";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Open();
  report << "核准且撥款(模組建議核准戶)" << endl;
  report << "撥款戶數" << ",";
  report << 0 << ",";
  report << query->FieldByName("IA")->AsFloat << ",";
  report << query->FieldByName("IIA")->AsFloat << ",";
  report << query->FieldByName("IIIA")->AsFloat << ",";
  report << query->FieldByName("IVA")->AsFloat << ",";
  report << query->FieldByName("VA")->AsFloat << ",";
  report << query->FieldByName("VIA")->AsFloat << ",";
  report << query->FieldByName("VIIA")->AsFloat << ",";
  report << query->FieldByName("VIIIA")->AsFloat << ",";
  report << query->FieldByName("IXA")->AsFloat << ",";
  report << query->FieldByName("XA")->AsFloat << ",";
  report << query->FieldByName("XIA")->AsFloat << endl;
/*  report << " declined cases" << ",";
  report << 0 << ",";
  report << query->FieldByName("ID")->AsFloat << ",";
  report << query->FieldByName("IID")->AsFloat << ",";
  report << query->FieldByName("IIID")->AsFloat << ",";
  report << query->FieldByName("IVD")->AsFloat << ",";
  report << query->FieldByName("VD")->AsFloat << ",";
  report << query->FieldByName("VID")->AsFloat << ",";
  report << query->FieldByName("VIID")->AsFloat << ",";
  report << query->FieldByName("VIIID")->AsFloat << ",";
  report << query->FieldByName("IXD")->AsFloat << ",";
  report << query->FieldByName("XD")->AsFloat << ",";
  report << query->FieldByName("XID")->AsFloat << endl; */
  report << "撥款金額" << ",";
  report << 0 << ",";
  report << query->FieldByName("IAD")->AsFloat << ",";
  report << query->FieldByName("IIAD")->AsFloat << ",";
  report << query->FieldByName("IIIAD")->AsFloat << ",";
  report << query->FieldByName("IVAD")->AsFloat << ",";
  report << query->FieldByName("VAD")->AsFloat << ",";
  report << query->FieldByName("VIAD")->AsFloat << ",";
  report << query->FieldByName("VIIAD")->AsFloat << ",";
  report << query->FieldByName("VIIIAD")->AsFloat << ",";
  report << query->FieldByName("IXAD")->AsFloat << ",";
  report << query->FieldByName("XAD")->AsFloat << ",";
  report << query->FieldByName("XIAD")->AsFloat << endl;
  report << "撥款件占申貸件百分比" << ",";
  report << 0 << ",";
  report << query->FieldByName("IAR")->AsFloat << ",";
  report << query->FieldByName("IIAR")->AsFloat << ",";
  report << query->FieldByName("IIIAR")->AsFloat << ",";
  report << query->FieldByName("IVAR")->AsFloat << ",";
  report << query->FieldByName("VAR")->AsFloat << ",";
  report << query->FieldByName("VIAR")->AsFloat << ",";
  report << query->FieldByName("VIIAR")->AsFloat << ",";
  report << query->FieldByName("VIIIAR")->AsFloat << ",";
  report << query->FieldByName("IXAR")->AsFloat << ",";
  report << query->FieldByName("XAR")->AsFloat << ",";
  report << query->FieldByName("XIAR")->AsFloat << endl;
  report << "撥款金額占申貸金額百分比" << ",";
  report << 0 << ",";
  report << query->FieldByName("IARD")->AsFloat << ",";
  report << query->FieldByName("IIARD")->AsFloat << ",";
  report << query->FieldByName("IIIARD")->AsFloat << ",";
  report << query->FieldByName("IVARD")->AsFloat << ",";
  report << query->FieldByName("VARD")->AsFloat << ",";
  report << query->FieldByName("VIARD")->AsFloat << ",";
  report << query->FieldByName("VIIARD")->AsFloat << ",";
  report << query->FieldByName("VIIIARD")->AsFloat << ",";
  report << query->FieldByName("IXARD")->AsFloat << ",";
  report << query->FieldByName("XARD")->AsFloat << ",";
  report << query->FieldByName("XIARD")->AsFloat << endl;
  report << "加權平均壞帳機率(戶數)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IPB")->AsFloat << ",";
  report << query->FieldByName("IIPB")->AsFloat << ",";
  report << query->FieldByName("IIIPB")->AsFloat << ",";
  report << query->FieldByName("IVPB")->AsFloat << ",";
  report << query->FieldByName("VPB")->AsFloat << ",";
  report << query->FieldByName("VIPB")->AsFloat << ",";
  report << query->FieldByName("VIIPB")->AsFloat << ",";
  report << query->FieldByName("VIIIPB")->AsFloat << ",";
  report << query->FieldByName("IXPB")->AsFloat << ",";
  report << query->FieldByName("XPB")->AsFloat << ",";
  report << query->FieldByName("XIPB")->AsFloat << endl;
  report << "加權平均壞帳機率(金額)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IPBD")->AsFloat << ",";
  report << query->FieldByName("IIPBD")->AsFloat << ",";
  report << query->FieldByName("IIIPBD")->AsFloat << ",";
  report << query->FieldByName("IVPBD")->AsFloat << ",";
  report << query->FieldByName("VPBD")->AsFloat << ",";
  report << query->FieldByName("VIPBD")->AsFloat << ",";
  report << query->FieldByName("VIIPBD")->AsFloat << ",";
  report << query->FieldByName("VIIIPBD")->AsFloat << ",";
  report << query->FieldByName("IXPBD")->AsFloat << ",";
  report << query->FieldByName("XPBD")->AsFloat << ",";
  report << query->FieldByName("XIPBD")->AsFloat << endl;
  report << "總撥款經濟價值(NPV)" << ",";
  report << "---" << ",";
  report << query->FieldByName("INPV")->AsFloat << ",";
  report << query->FieldByName("IINPV")->AsFloat << ",";
  report << query->FieldByName("IIINPV")->AsFloat << ",";
  report << query->FieldByName("IVNPV")->AsFloat << ",";
  report << query->FieldByName("VNPV")->AsFloat << ",";
  report << query->FieldByName("VINPV")->AsFloat << ",";
  report << query->FieldByName("VIINPV")->AsFloat << ",";
  report << query->FieldByName("VIIINPV")->AsFloat << ",";
  report << query->FieldByName("IXNPV")->AsFloat << ",";
  report << query->FieldByName("XNPV")->AsFloat << ",";
  report << query->FieldByName("XINPV")->AsFloat << endl;
  query->Close();
}

void write_part_four(TADOQuery *query, ofstream &report)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM PART_FOUR;";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Open();
  report << "核准且撥款(模組建議婉拒戶)" << endl;
  report << "撥款戶數" << ",";
  report << query->FieldByName("NSA")->AsFloat << ",";
  report << query->FieldByName("IA")->AsFloat << ",";
  report << query->FieldByName("IIA")->AsFloat << ",";
  report << query->FieldByName("IIIA")->AsFloat << ",";
  report << query->FieldByName("IVA")->AsFloat << ",";
  report << query->FieldByName("VA")->AsFloat << ",";
  report << query->FieldByName("VIA")->AsFloat << ",";
  report << query->FieldByName("VIIA")->AsFloat << ",";
  report << query->FieldByName("VIIIA")->AsFloat << ",";
  report << query->FieldByName("IXA")->AsFloat << ",";
  report << query->FieldByName("XA")->AsFloat << ",";
  report << query->FieldByName("XIA")->AsFloat << endl;
/*  report << " declined cases" << ",";
  report << query->FieldByName("NSD")->AsFloat << ",";
  report << query->FieldByName("ID")->AsFloat << ",";
  report << query->FieldByName("IID")->AsFloat << ",";
  report << query->FieldByName("IIID")->AsFloat << ",";
  report << query->FieldByName("IVD")->AsFloat << ",";
  report << query->FieldByName("VD")->AsFloat << ",";
  report << query->FieldByName("VID")->AsFloat << ",";
  report << query->FieldByName("VIID")->AsFloat << ",";
  report << query->FieldByName("VIIID")->AsFloat << ",";
  report << query->FieldByName("IXD")->AsFloat << ",";
  report << query->FieldByName("XD")->AsFloat << ",";
  report << query->FieldByName("XID")->AsFloat << endl;  */
  report << "撥款金額" << ",";
  report << query->FieldByName("NSAD")->AsFloat << ",";
  report << query->FieldByName("IAD")->AsFloat << ",";
  report << query->FieldByName("IIAD")->AsFloat << ",";
  report << query->FieldByName("IIIAD")->AsFloat << ",";
  report << query->FieldByName("IVAD")->AsFloat << ",";
  report << query->FieldByName("VAD")->AsFloat << ",";
  report << query->FieldByName("VIAD")->AsFloat << ",";
  report << query->FieldByName("VIIAD")->AsFloat << ",";
  report << query->FieldByName("VIIIAD")->AsFloat << ",";
  report << query->FieldByName("IXAD")->AsFloat << ",";
  report << query->FieldByName("XAD")->AsFloat << ",";
  report << query->FieldByName("XIAD")->AsFloat << endl;
  report << "撥款件占申貸件百分比" << ",";
  report << query->FieldByName("NSAR")->AsFloat << ",";
  report << query->FieldByName("IAR")->AsFloat << ",";
  report << query->FieldByName("IIAR")->AsFloat << ",";
  report << query->FieldByName("IIIAR")->AsFloat << ",";
  report << query->FieldByName("IVAR")->AsFloat << ",";
  report << query->FieldByName("VAR")->AsFloat << ",";
  report << query->FieldByName("VIAR")->AsFloat << ",";
  report << query->FieldByName("VIIAR")->AsFloat << ",";
  report << query->FieldByName("VIIIAR")->AsFloat << ",";
  report << query->FieldByName("IXAR")->AsFloat << ",";
  report << query->FieldByName("XAR")->AsFloat << ",";
  report << query->FieldByName("XIAR")->AsFloat << endl;
  report << "撥款金額占申貸金額百分比" << ",";
  report << query->FieldByName("NSARD")->AsFloat << ",";
  report << query->FieldByName("IARD")->AsFloat << ",";
  report << query->FieldByName("IIARD")->AsFloat << ",";
  report << query->FieldByName("IIIARD")->AsFloat << ",";
  report << query->FieldByName("IVARD")->AsFloat << ",";
  report << query->FieldByName("VARD")->AsFloat << ",";
  report << query->FieldByName("VIARD")->AsFloat << ",";
  report << query->FieldByName("VIIARD")->AsFloat << ",";
  report << query->FieldByName("VIIIARD")->AsFloat << ",";
  report << query->FieldByName("IXARD")->AsFloat << ",";
  report << query->FieldByName("XARD")->AsFloat << ",";
  report << query->FieldByName("XIARD")->AsFloat << endl;
  report << "加權平均壞帳機率(戶數)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IPB")->AsFloat << ",";
  report << query->FieldByName("IIPB")->AsFloat << ",";
  report << query->FieldByName("IIIPB")->AsFloat << ",";
  report << query->FieldByName("IVPB")->AsFloat << ",";
  report << query->FieldByName("VPB")->AsFloat << ",";
  report << query->FieldByName("VIPB")->AsFloat << ",";
  report << query->FieldByName("VIIPB")->AsFloat << ",";
  report << query->FieldByName("VIIIPB")->AsFloat << ",";
  report << query->FieldByName("IXPB")->AsFloat << ",";
  report << query->FieldByName("XPB")->AsFloat << ",";
  report << query->FieldByName("XIPB")->AsFloat << endl;
  report << "加權平均壞帳機率(金額)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IPBD")->AsFloat << ",";
  report << query->FieldByName("IIPBD")->AsFloat << ",";
  report << query->FieldByName("IIIPBD")->AsFloat << ",";
  report << query->FieldByName("IVPBD")->AsFloat << ",";
  report << query->FieldByName("VPBD")->AsFloat << ",";
  report << query->FieldByName("VIPBD")->AsFloat << ",";
  report << query->FieldByName("VIIPBD")->AsFloat << ",";
  report << query->FieldByName("VIIIPBD")->AsFloat << ",";
  report << query->FieldByName("IXPBD")->AsFloat << ",";
  report << query->FieldByName("XPBD")->AsFloat << ",";
  report << query->FieldByName("XIPBD")->AsFloat << endl;
  report << "總撥款經濟價值(NPV)" << ",";
  report << "---" << ",";
  report << query->FieldByName("INPV")->AsFloat << ",";
  report << query->FieldByName("IINPV")->AsFloat << ",";
  report << query->FieldByName("IIINPV")->AsFloat << ",";
  report << query->FieldByName("IVNPV")->AsFloat << ",";
  report << query->FieldByName("VNPV")->AsFloat << ",";
  report << query->FieldByName("VINPV")->AsFloat << ",";
  report << query->FieldByName("VIINPV")->AsFloat << ",";
  report << query->FieldByName("VIIINPV")->AsFloat << ",";
  report << query->FieldByName("IXNPV")->AsFloat << ",";
  report << query->FieldByName("XNPV")->AsFloat << ",";
  report << query->FieldByName("XINPV")->AsFloat << endl;

//  report << "Cumulative" << endl;
/*  report << "% case approval(cumulative)" << ",";
  report << query->FieldByName("NSAAR")->AsFloat << ",";
  report << query->FieldByName("IAAR")->AsFloat << ",";
  report << query->FieldByName("IIAAR")->AsFloat << ",";
  report << query->FieldByName("IIIAAR")->AsFloat << ",";
  report << query->FieldByName("IVAAR")->AsFloat << ",";
  report << query->FieldByName("VAAR")->AsFloat << ",";
  report << query->FieldByName("VIAAR")->AsFloat << ",";
  report << query->FieldByName("VIIAAR")->AsFloat << ",";
  report << query->FieldByName("VIIIAAR")->AsFloat << ",";
  report << query->FieldByName("IXAAR")->AsFloat << ",";
  report << query->FieldByName("XAAR")->AsFloat << ",";
  report << query->FieldByName("XIAAR")->AsFloat << endl;
  report << "% amount approval(cumulative)" << ",";
  report << query->FieldByName("NSAARD")->AsFloat << ",";
  report << query->FieldByName("IAARD")->AsFloat << ",";
  report << query->FieldByName("IIAARD")->AsFloat << ",";
  report << query->FieldByName("IIIAARD")->AsFloat << ",";
  report << query->FieldByName("IVAARD")->AsFloat << ",";
  report << query->FieldByName("VAARD")->AsFloat << ",";
  report << query->FieldByName("VIAARD")->AsFloat << ",";
  report << query->FieldByName("VIIAARD")->AsFloat << ",";
  report << query->FieldByName("VIIIAARD")->AsFloat << ",";
  report << query->FieldByName("IXAARD")->AsFloat << ",";
  report << query->FieldByName("XAARD")->AsFloat << ",";
  report << query->FieldByName("XIAARD")->AsFloat << endl;
  report << "weighted avg annual PB(cumulative)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IAPB")->AsFloat << ",";
  report << query->FieldByName("IIAPB")->AsFloat << ",";
  report << query->FieldByName("IIIAPB")->AsFloat << ",";
  report << query->FieldByName("IVAPB")->AsFloat << ",";
  report << query->FieldByName("VAPB")->AsFloat << ",";
  report << query->FieldByName("VIAPB")->AsFloat << ",";
  report << query->FieldByName("VIIAPB")->AsFloat << ",";
  report << query->FieldByName("VIIIAPB")->AsFloat << ",";
  report << query->FieldByName("IXAPB")->AsFloat << ",";
  report << query->FieldByName("XAPB")->AsFloat << ",";
  report << query->FieldByName("XIAPB")->AsFloat << endl;
  report << "weighted avg annual %$ bad(cumulative)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IAPBD")->AsFloat << ",";
  report << query->FieldByName("IIAPBD")->AsFloat << ",";
  report << query->FieldByName("IIIAPBD")->AsFloat << ",";
  report << query->FieldByName("IVAPBD")->AsFloat << ",";
  report << query->FieldByName("VAPBD")->AsFloat << ",";
  report << query->FieldByName("VIAPBD")->AsFloat << ",";
  report << query->FieldByName("VIIAPBD")->AsFloat << ",";
  report << query->FieldByName("VIIIAPBD")->AsFloat << ",";
  report << query->FieldByName("IXAPBD")->AsFloat << ",";
  report << query->FieldByName("XAPBD")->AsFloat << ",";
  report << query->FieldByName("XIAPBD")->AsFloat << endl;
  report << "Total NPV (follow + override)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IANPV")->AsFloat << ",";
  report << query->FieldByName("IIANPV")->AsFloat << ",";
  report << query->FieldByName("IIIANPV")->AsFloat << ",";
  report << query->FieldByName("IVANPV")->AsFloat << ",";
  report << query->FieldByName("VANPV")->AsFloat << ",";
  report << query->FieldByName("VIANPV")->AsFloat << ",";
  report << query->FieldByName("VIIANPV")->AsFloat << ",";
  report << query->FieldByName("VIIIANPV")->AsFloat << ",";
  report << query->FieldByName("IXANPV")->AsFloat << ",";
  report << query->FieldByName("XANPV")->AsFloat << ",";
  report << query->FieldByName("XIANPV")->AsFloat << endl;*/
  query->Close();
}

void write_part_five(TADOQuery *query, ofstream &report)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM PART_FIVE;";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Open();
  report << "核准且撥款(模組建議人工授信戶)" << endl;
  report << "撥款戶數" << ",";
  report << query->FieldByName("NSA")->AsFloat << ",";
  report << query->FieldByName("IA")->AsFloat << ",";
  report << query->FieldByName("IIA")->AsFloat << ",";
  report << query->FieldByName("IIIA")->AsFloat << ",";
  report << query->FieldByName("IVA")->AsFloat << ",";
  report << query->FieldByName("VA")->AsFloat << ",";
  report << query->FieldByName("VIA")->AsFloat << ",";
  report << query->FieldByName("VIIA")->AsFloat << ",";
  report << query->FieldByName("VIIIA")->AsFloat << ",";
  report << query->FieldByName("IXA")->AsFloat << ",";
  report << query->FieldByName("XA")->AsFloat << ",";
  report << query->FieldByName("XIA")->AsFloat << endl;
/*  report << " declined cases(manual only)" << ",";
  report << query->FieldByName("NSD")->AsFloat << ",";
  report << query->FieldByName("ID")->AsFloat << ",";
  report << query->FieldByName("IID")->AsFloat << ",";
  report << query->FieldByName("IIID")->AsFloat << ",";
  report << query->FieldByName("IVD")->AsFloat << ",";
  report << query->FieldByName("VD")->AsFloat << ",";
  report << query->FieldByName("VID")->AsFloat << ",";
  report << query->FieldByName("VIID")->AsFloat << ",";
  report << query->FieldByName("VIIID")->AsFloat << ",";
  report << query->FieldByName("IXD")->AsFloat << ",";
  report << query->FieldByName("XD")->AsFloat << ",";
  report << query->FieldByName("XID")->AsFloat << endl;*/
  report << "撥款金額" << ",";
  report << query->FieldByName("NSAD")->AsFloat << ",";
  report << query->FieldByName("IAD")->AsFloat << ",";
  report << query->FieldByName("IIAD")->AsFloat << ",";
  report << query->FieldByName("IIIAD")->AsFloat << ",";
  report << query->FieldByName("IVAD")->AsFloat << ",";
  report << query->FieldByName("VAD")->AsFloat << ",";
  report << query->FieldByName("VIAD")->AsFloat << ",";
  report << query->FieldByName("VIIAD")->AsFloat << ",";
  report << query->FieldByName("VIIIAD")->AsFloat << ",";
  report << query->FieldByName("IXAD")->AsFloat << ",";
  report << query->FieldByName("XAD")->AsFloat << ",";
  report << query->FieldByName("XIAD")->AsFloat << endl;
  report << "撥款件占申貸件百分比" << ",";
  report << query->FieldByName("NSAR")->AsFloat << ",";
  report << query->FieldByName("IAR")->AsFloat << ",";
  report << query->FieldByName("IIAR")->AsFloat << ",";
  report << query->FieldByName("IIIAR")->AsFloat << ",";
  report << query->FieldByName("IVAR")->AsFloat << ",";
  report << query->FieldByName("VAR")->AsFloat << ",";
  report << query->FieldByName("VIAR")->AsFloat << ",";
  report << query->FieldByName("VIIAR")->AsFloat << ",";
  report << query->FieldByName("VIIIAR")->AsFloat << ",";
  report << query->FieldByName("IXAR")->AsFloat << ",";
  report << query->FieldByName("XAR")->AsFloat << ",";
  report << query->FieldByName("XIAR")->AsFloat << endl;
  report << "撥款金額占申貸金額百分比" << ",";
  report << query->FieldByName("NSARD")->AsFloat << ",";
  report << query->FieldByName("IARD")->AsFloat << ",";
  report << query->FieldByName("IIARD")->AsFloat << ",";
  report << query->FieldByName("IIIARD")->AsFloat << ",";
  report << query->FieldByName("IVARD")->AsFloat << ",";
  report << query->FieldByName("VARD")->AsFloat << ",";
  report << query->FieldByName("VIARD")->AsFloat << ",";
  report << query->FieldByName("VIIARD")->AsFloat << ",";
  report << query->FieldByName("VIIIARD")->AsFloat << ",";
  report << query->FieldByName("IXARD")->AsFloat << ",";
  report << query->FieldByName("XARD")->AsFloat << ",";
  report << query->FieldByName("XIARD")->AsFloat << endl;
  report << "加權平均壞帳機率(戶數)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IPB")->AsFloat << ",";
  report << query->FieldByName("IIPB")->AsFloat << ",";
  report << query->FieldByName("IIIPB")->AsFloat << ",";
  report << query->FieldByName("IVPB")->AsFloat << ",";
  report << query->FieldByName("VPB")->AsFloat << ",";
  report << query->FieldByName("VIPB")->AsFloat << ",";
  report << query->FieldByName("VIIPB")->AsFloat << ",";
  report << query->FieldByName("VIIIPB")->AsFloat << ",";
  report << query->FieldByName("IXPB")->AsFloat << ",";
  report << query->FieldByName("XPB")->AsFloat << ",";
  report << query->FieldByName("XIPB")->AsFloat << endl;
  report << "加權平均壞帳機率(金額)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IPBD")->AsFloat << ",";
  report << query->FieldByName("IIPBD")->AsFloat << ",";
  report << query->FieldByName("IIIPBD")->AsFloat << ",";
  report << query->FieldByName("IVPBD")->AsFloat << ",";
  report << query->FieldByName("VPBD")->AsFloat << ",";
  report << query->FieldByName("VIPBD")->AsFloat << ",";
  report << query->FieldByName("VIIPBD")->AsFloat << ",";
  report << query->FieldByName("VIIIPBD")->AsFloat << ",";
  report << query->FieldByName("IXPBD")->AsFloat << ",";
  report << query->FieldByName("XPBD")->AsFloat << ",";
  report << query->FieldByName("XIPBD")->AsFloat << endl;
  report << "總撥款經濟價值(NPV)" << ",";
  report << "---" << ",";
  report << query->FieldByName("INPV")->AsFloat << ",";
  report << query->FieldByName("IINPV")->AsFloat << ",";
  report << query->FieldByName("IIINPV")->AsFloat << ",";
  report << query->FieldByName("IVNPV")->AsFloat << ",";
  report << query->FieldByName("VNPV")->AsFloat << ",";
  report << query->FieldByName("VINPV")->AsFloat << ",";
  report << query->FieldByName("VIINPV")->AsFloat << ",";
  report << query->FieldByName("VIIINPV")->AsFloat << ",";
  report << query->FieldByName("IXNPV")->AsFloat << ",";
  report << query->FieldByName("XNPV")->AsFloat << ",";
  report << query->FieldByName("XINPV")->AsFloat << endl;
  report << "所有核准且撥款(模組建議核准、婉拒、及人工授信戶)" << endl;
  report << "撥款件占申貸件百分比" << ",";
  report << query->FieldByName("NSAAR")->AsFloat << ",";
  report << query->FieldByName("IAAR")->AsFloat << ",";
  report << query->FieldByName("IIAAR")->AsFloat << ",";
  report << query->FieldByName("IIIAAR")->AsFloat << ",";
  report << query->FieldByName("IVAAR")->AsFloat << ",";
  report << query->FieldByName("VAAR")->AsFloat << ",";
  report << query->FieldByName("VIAAR")->AsFloat << ",";
  report << query->FieldByName("VIIAAR")->AsFloat << ",";
  report << query->FieldByName("VIIIAAR")->AsFloat << ",";
  report << query->FieldByName("IXAAR")->AsFloat << ",";
  report << query->FieldByName("XAAR")->AsFloat << ",";
  report << query->FieldByName("XIAAR")->AsFloat << endl;
  report << "撥款金額占申貸金額百分比" << ",";
  report << query->FieldByName("NSAARD")->AsFloat << ",";
  report << query->FieldByName("IAARD")->AsFloat << ",";
  report << query->FieldByName("IIAARD")->AsFloat << ",";
  report << query->FieldByName("IIIAARD")->AsFloat << ",";
  report << query->FieldByName("IVAARD")->AsFloat << ",";
  report << query->FieldByName("VAARD")->AsFloat << ",";
  report << query->FieldByName("VIAARD")->AsFloat << ",";
  report << query->FieldByName("VIIAARD")->AsFloat << ",";
  report << query->FieldByName("VIIIAARD")->AsFloat << ",";
  report << query->FieldByName("IXAARD")->AsFloat << ",";
  report << query->FieldByName("XAARD")->AsFloat << ",";
  report << query->FieldByName("XIAARD")->AsFloat << endl;
  report << "加權平均壞帳機率(戶數)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IAPB")->AsFloat << ",";
  report << query->FieldByName("IIAPB")->AsFloat << ",";
  report << query->FieldByName("IIIAPB")->AsFloat << ",";
  report << query->FieldByName("IVAPB")->AsFloat << ",";
  report << query->FieldByName("VAPB")->AsFloat << ",";
  report << query->FieldByName("VIAPB")->AsFloat << ",";
  report << query->FieldByName("VIIAPB")->AsFloat << ",";
  report << query->FieldByName("VIIIAPB")->AsFloat << ",";
  report << query->FieldByName("IXAPB")->AsFloat << ",";
  report << query->FieldByName("XAPB")->AsFloat << ",";
  report << query->FieldByName("XIAPB")->AsFloat << endl;
  report << "加權平均壞帳機率(金額)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IAPBD")->AsFloat << ",";
  report << query->FieldByName("IIAPBD")->AsFloat << ",";
  report << query->FieldByName("IIIAPBD")->AsFloat << ",";
  report << query->FieldByName("IVAPBD")->AsFloat << ",";
  report << query->FieldByName("VAPBD")->AsFloat << ",";
  report << query->FieldByName("VIAPBD")->AsFloat << ",";
  report << query->FieldByName("VIIAPBD")->AsFloat << ",";
  report << query->FieldByName("VIIIAPBD")->AsFloat << ",";
  report << query->FieldByName("IXAPBD")->AsFloat << ",";
  report << query->FieldByName("XAPBD")->AsFloat << ",";
  report << query->FieldByName("XIAPBD")->AsFloat << endl;
  report << "總撥款經濟價值(NPV)" << ",";
  report << "---" << ",";
  report << query->FieldByName("IANPV")->AsFloat << ",";
  report << query->FieldByName("IIANPV")->AsFloat << ",";
  report << query->FieldByName("IIIANPV")->AsFloat << ",";
  report << query->FieldByName("IVANPV")->AsFloat << ",";
  report << query->FieldByName("VANPV")->AsFloat << ",";
  report << query->FieldByName("VIANPV")->AsFloat << ",";
  report << query->FieldByName("VIIANPV")->AsFloat << ",";
  report << query->FieldByName("VIIIANPV")->AsFloat << ",";
  report << query->FieldByName("IXANPV")->AsFloat << ",";
  report << query->FieldByName("XANPV")->AsFloat << ",";
  report << query->FieldByName("XIANPV")->AsFloat << endl;
  query->Close();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
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
//    config.getline(buffer,2048);
//    edtDBServer->Text = buffer;
//    clear(buffer,2048);
//    config.getline(buffer,2048);
//    edtDB->Text = buffer;
//    clear(buffer,2048);
    config.getline(buffer,2048);
    report_dir = buffer;
  }
  config.clear();
  config.close();
}
//---------------------------------------------------------------------------
void clear(char *array, int size)
{
  for(int i=0; i<size; ++i)
    array[i] = ' ';
}
void __fastcall TForm1::Button3Click(TObject *Sender)
{
   Application->CreateForm(__classid(TForm2), &Form2);
   Form2->dlbReport->Directory = report_dir;
   Form2->edtDir->Text = report_dir;
   Form2->ShowModal();
}
//---------------------------------------------------------------------------
void clean_up(TADOCommand *command)
{
  AnsiString sql_stmt;
/*  try{
    sql_stmt = "DROP TABLE REPORT_BASE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }*/

  try{
    sql_stmt = "DROP TABLE ONE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE TWO;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE THREE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE FOUR_I;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  }

  try{
    sql_stmt = "DROP TABLE FOUR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE FOUR_AR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE FIVE_I;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  }

  try{
    sql_stmt = "DROP TABLE FIVE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE FIVE_AR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }
  
  try{
    sql_stmt = "DROP TABLE PART_ONE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE PART_TWO;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE PART_THREE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE PART_FOUR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }

  try{
    sql_stmt = "DROP TABLE AR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  }

  try{
    sql_stmt = "DROP TABLE PART_FIVE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == err_no_drop)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::btnAddDimension1Click(TObject *Sender)
{
  if(lbDimensions->ItemIndex != -1){
    if(lblDimension1->Caption != "")
      lbDimensions->Items->Add(lblDimension1->Caption);
    lblDimension1->Caption = lbDimensions->Items->operator [](lbDimensions->ItemIndex);
    lbDimensions->Items->Delete(lbDimensions->ItemIndex);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnAddDimension2Click(TObject *Sender)
{
  if(lbDimensions->ItemIndex != -1){
    if(lblDimension2->Caption != "")
      lbDimensions->Items->Add(lblDimension2->Caption);
    lblDimension2->Caption = lbDimensions->Items->operator [](lbDimensions->ItemIndex);
    lbDimensions->Items->Delete(lbDimensions->ItemIndex);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnRemoveDimension1Click(TObject *Sender)
{
  if(lblDimension1->Caption != ""){
    lbDimensions->Items->Add(lblDimension1->Caption);
    lblDimension1->Caption = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnRemoveDimension2Click(TObject *Sender)
{
  if(lblDimension2->Caption != ""){
    lbDimensions->Items->Add(lblDimension2->Caption);
    lblDimension2->Caption = "";
  }
}
//---------------------------------------------------------------------------
//type: 0 for int; 1 for string; 2 for float
AnsiString get_dimension(const AnsiString &dimension_Chinese, unsigned int &type)
{
  AnsiString return_message = "";
  type = 1;
  if(dimension_Chinese == "銷售通路")
    return_message = "CI_KEY";
  else if(dimension_Chinese == "貸款金額"){
    return_message = "APP_AMT_CAT";
    type = 0;
  }
  else if(dimension_Chinese == "分行")
    return_message = "BRANCH";
  else if(dimension_Chinese == "利率"){
    return_message = "APR_GROUP";
    type = 0;
  }
  else if(dimension_Chinese == "產品"){
    return_message = "PRODUCT_TYPE_CODE";
    type = 0;
  }
  return return_message;
}
//---------------------------------------------------------------------------
void fill_dimension(const AnsiString &dimension,
                    TADOQuery *query, vector<AnsiString> &dimension_content,
                    unsigned int dimension_type)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT DISTINCT " + dimension + " AS COL FROM REPORT_BASE ";
  if(dimension_type == 1)
    sql_stmt += "WHERE " + dimension + " <> ''";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Open();
  while(!query->Eof){
    dimension_content.push_back(query->FieldValues["COL"]);
    query->Next();
  }
}
//---------------------------------------------------------------------------

