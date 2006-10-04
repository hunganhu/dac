//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReportGen.h"
#include "dm.h"
#include "LoanTypeSelection.h"
#include "ReDirUI.h"
#include "DirUI.h"
#include "keyin_check.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmReportGen *frmReportGen;
AnsiString working_dir;
AnsiString report_dir;
AnsiString report_regen_dir;
AnsiString message;
//---------------------------------------------------------------------------
__fastcall TfrmReportGen::TfrmReportGen(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmReportGen::btnCloseClick(TObject *Sender)
{
  frmSelection->Show();
  frmReportGen->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReportGen::btnExit2Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReportGen::btnNormalRptDirClick(TObject *Sender)
{
  Application->CreateForm(__classid(TfrmDirUI), &frmDirUI);
  frmDirUI->dlbReport->Directory = report_dir;
  frmDirUI->edtDir->Text = report_dir;
  frmDirUI->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReportGen::FormCreate(TObject *Sender)
{
  lblMessage->Caption = "";
  message = "";
  report_dir = "";
  report_regen_dir = "";
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
    clear(buffer,2048);
    config.getline(buffer,2048);
    report_regen_dir = buffer;
  };
  config.clear();
  config.close();
}
//---------------------------------------------------------------------------

void clear(char *array, int size)
{
  for(int i=0; i<size; ++i)
    array[i] = ' ';
};

void __fastcall TfrmReportGen::btnRegenRptDirClick(TObject *Sender)
{
   Application->CreateForm(__classid(TfrmReDirUI), &frmReDirUI);
   frmReDirUI->dlbReReport->Directory = report_regen_dir;
   frmReDirUI->edtReDir->Text = report_regen_dir;
   frmReDirUI->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReportGen::btnRptGenClick(TObject *Sender)
{
  AnsiString dir_message = (report_dir.Length() == 0) ? static_cast<AnsiString>("程式所在目錄") : report_dir;
  dir_message += "\n";
  message = "報表將會儲存在: " + dir_message;
  lblMessage->Caption = message;
  lblMessage->Refresh();
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if(config){
    config << report_dir.c_str() << endl;
    config << report_regen_dir.c_str() << endl;
    config.clear();
    config.close();
  };
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  unsigned int second;
  get_time(year, month, day, hour, min, second);

  AnsiString report_year = static_cast<AnsiString>(year);
  AnsiString report_month = static_cast<AnsiString>(month);
  AnsiString report_date = static_cast<AnsiString>(day);
  AnsiString report_hour = static_cast<AnsiString>(hour);
  AnsiString report_min = static_cast<AnsiString>(min);

  report_month = report_month.Length() == 1 ? "0" + report_month : report_month;
  report_date = report_date.Length() == 1 ? "0" + report_date : report_date;
  report_hour = report_hour.Length() == 1 ? "0" + report_hour : report_hour;
  report_min = report_min.Length() == 1 ? "0" + report_min : report_min;

  AnsiString report_gen_time = report_year + report_month + report_date +
                               report_hour + report_min;



  prepare_report(Data->command, "", false);
  bool success = generate_report(Data->query, report_dir, report_gen_time, false);
  if(!success){
    message += "報表產生錯誤。\n";
    lblMessage->Caption = message;

  }
  else{
    message += "報表已產生。";
    lblMessage->Caption = message;
  };
  frmReportGen->Refresh();
  clean_report(Data->command);
}
//---------------------------------------------------------------------------

void prepare_report(TADOCommand *command, const AnsiString &report_gen_time, bool regen)
{
  AnsiString sql_stmt;

  try{
    sql_stmt = "DROP TABLE Report";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE Report ";
  sql_stmt += "(CASE_NO Char(14), SCORE_TIME char(14), PRODUCT_TYPE VarChar(10), ";
  sql_stmt += "APP_ID Char(10),  APP_NAME VarChar(30), APP_GENDER Char(2), ";
  sql_stmt += "APP_BIRTHDAY Char(7), APP_MARRIAGE VarChar(4), APP_EDUCATION VarChar(18), ";
  sql_stmt += "APP_INCOME INT, APP_QUALIFIED Char(2), ";
  sql_stmt += "COS_ID Char(10),  COS_NAME VarChar(30), COS_GENDER Char(2), ";
  sql_stmt += "COS_BIRTHDAY Char(7), COS_MARRIAGE VarChar(4), COS_EDUCATION VarChar(18), ";
  sql_stmt += "COS_INCOME INT, COS_QUALIFIED Char(2), ";
  sql_stmt += "GUA_ID Char(10),  GUA_NAME VarChar(30), GUA_GENDER Char(2), ";
  sql_stmt += "GUA_BIRTHDAY Char(7), GUA_MARRIAGE VarChar(4), GUA_EDUCATION VarChar(18), ";
  sql_stmt += "GUA_INCOME INT, GUA_QUALIFIED Char(2), ";
  sql_stmt += "APP_AMOUNT INT, PERIOD int, APR1 Decimal(8,5), SEG1 INT, ";
  sql_stmt += "APR2 Decimal(8,5), SEG2 INT, APR3 Decimal(8,5), SEG3 INT, ";
  sql_stmt += "GRACE_PERIOD INT, APP_FEE INT, OWNER_ID	CHAR(10), OWNER_NAME	VARCHAR(30), ";
  sql_stmt += "LAND_NUM	VARCHAR(30), RELATIONSHIP	CHAR(4), GAV FLOAT, NAV FLOAT, ";
  sql_stmt += "COL_QUALIFIED CHAR(2), PREMIUM_COL	CHAR(2), MONTHLY_PAYMENT INT,	";
  sql_stmt += "APP_SCRMSG VARCHAR(128), COS_SCRMSG VARCHAR(128), GUA_SCRMSG VARCHAR(128), ";
  sql_stmt += "PB Decimal(5,4), NPV INT, APPROVED_AMOUNT INT, ";
  sql_stmt += "MIN_RATE1 Decimal(8,5), MIN_RATE2 Decimal(8,5), MIN_RATE3 Decimal(8,5), ";
  sql_stmt += "SUGG_MSG VARCHAR(256), REASON_MSG VARCHAR(256), BRANCH VARCHAR(4), SUGG_CODE INT, CFC VARCHAR(60))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO Report ";
  sql_stmt += "(CASE_NO, SCORE_TIME, PRODUCT_TYPE, ";
  sql_stmt += "APP_ID, APP_NAME, APP_GENDER, ";
  sql_stmt += "APP_BIRTHDAY, APP_MARRIAGE, APP_EDUCATION, ";
  sql_stmt += "APP_INCOME, APP_QUALIFIED, ";
  sql_stmt += "COS_ID, COS_NAME, COS_GENDER, ";
  sql_stmt += "COS_BIRTHDAY, COS_MARRIAGE, COS_EDUCATION, ";
  sql_stmt += "COS_INCOME, COS_QUALIFIED, ";
  sql_stmt += "GUA_ID, GUA_NAME, GUA_GENDER, ";
  sql_stmt += "GUA_BIRTHDAY, GUA_MARRIAGE, GUA_EDUCATION, ";
  sql_stmt += "GUA_INCOME, GUA_QUALIFIED, ";
  sql_stmt += "APP_AMOUNT, PERIOD, APR1, SEG1, ";
  sql_stmt += "APR2, SEG2, APR3, SEG3, ";
  sql_stmt += "GRACE_PERIOD , APP_FEE, OWNER_ID, OWNER_NAME, ";
  sql_stmt += "LAND_NUM, RELATIONSHIP, GAV, NAV, ";
  sql_stmt += "COL_QUALIFIED, PREMIUM_COL, MONTHLY_PAYMENT, ";
  sql_stmt += "APP_SCRMSG, COS_SCRMSG, GUA_SCRMSG,  ";
  sql_stmt += "PB, NPV, APPROVED_AMOUNT, ";
  sql_stmt += "MIN_RATE1, MIN_RATE2, MIN_RATE3, ";
  sql_stmt += "SUGG_MSG, REASON_MSG, BRANCH, SUGG_CODE) ";
  sql_stmt += "SELECT A.CASE_NO, B.FINAL_DATE, (CASE WHEN A.PROD_TYPE = 1 ";
  sql_stmt += "THEN '一般房貸' WHEN A.PROD_TYPE = 2 THEN '增貸' WHEN A.PROD_TYPE = 3 ";
  sql_stmt += "THEN '原餘額轉貸' END), ";
  sql_stmt += "A.APP_ID, A.APP_NAME, (CASE WHEN SUBSTRING(A.APP_GENDER, 2, 1) = '1'  ";
  sql_stmt += "THEN '男' WHEN SUBSTRING(A.GUA_GENDER, 2, 1) = '2' '女' ELSE '無' END), ";
  sql_stmt += "A.APP_BIRTHDAY, (CASE WHEN A.APP_MARRIAGE = 0 THEN '未婚' ";
  sql_stmt += "WHEN A.APP_MARRIAGE = 1 THEN '已婚' ";
  sql_stmt += "WHEN A.APP_MARRIAGE = 2 THEN '離婚' END), ";
  sql_stmt += "(CASE WHEN A.APP_EDUCATION = 0 THEN '研究所' ";
  sql_stmt += "WHEN A.APP_EDUCATION = 1 THEN '大學' ";
  sql_stmt += "WHEN A.APP_EDUCATION = 2 THEN '專科' ";
  sql_stmt += "WHEN A.APP_EDUCATION = 3 THEN '高、國中(含以下)' END), ";
  sql_stmt += "A.APP_INCOME, (CASE WHEN A.APP_QUALIFIED = 0 THEN '否' ";
  sql_stmt += "WHEN A.APP_QUALIFIED = 1 THEN '是' ELSE '無' END), ";
  sql_stmt += "A.COS_ID, A.COS_NAME, (CASE WHEN SUBSTRING(A.COS_GENDER, 2, 1) = '1'  ";
  sql_stmt += "THEN '男' WHEN SUBSTRING(A.GUA_GENDER, 2, 1) = '2' '女' ELSE '無' END), ";
  sql_stmt += "A.COS_BIRTHDAY, (CASE WHEN A.COS_MARRIAGE = 0 THEN '未婚' ";
  sql_stmt += "WHEN A.COS_MARRIAGE = 1 THEN '已婚' ";
  sql_stmt += "WHEN A.COS_MARRIAGE = 2 THEN '離婚' END), ";
  sql_stmt += "(CASE WHEN A.COS_EDUCATION = 0 THEN '研究所' ";
  sql_stmt += "WHEN A.COS_EDUCATION = 1 THEN '大學' ";
  sql_stmt += "WHEN A.COS_EDUCATION = 2 THEN '專科' ";
  sql_stmt += "WHEN A.COS_EDUCATION = 3 THEN '高、國中(含以下)' END), ";
  sql_stmt += "A.COS_INCOME, (CASE WHEN A.COS_QUALIFIED = 0 THEN '否' ";
  sql_stmt += "WHEN A.COS_QUALIFIED = 1 THEN '是' ELSE '無' END), ";
  sql_stmt += "A.GUA_ID, A.GUA_NAME, (CASE WHEN SUBSTRING(A.GUA_GENDER, 2, 1) = '1'  ";
  sql_stmt += "THEN '男' WHEN SUBSTRING(A.GUA_GENDER, 2, 1) = '2' '女' ELSE '無' END), ";
  sql_stmt += "A.GUA_BIRTHDAY, (CASE WHEN A.GUA_MARRIAGE = 0 THEN '未婚' ";
  sql_stmt += "WHEN A.GUA_MARRIAGE = 1 THEN '已婚' ";
  sql_stmt += "WHEN A.GUA_MARRIAGE = 2 THEN '離婚' END), ";
  sql_stmt += "(CASE WHEN A.GUA_EDUCATION = 0 THEN '研究所' ";
  sql_stmt += "WHEN A.GUA_EDUCATION = 1 THEN '大學' ";
  sql_stmt += "WHEN A.GUA_EDUCATION = 2 THEN '專科' ";
  sql_stmt += "WHEN A.GUA_EDUCATION = 3 THEN '高、國中(含以下)' END), ";
  sql_stmt += "A.GUA_INCOME, (CASE WHEN A.GUA_QUALIFIED = 0 THEN '否' ";
  sql_stmt += "WHEN A.GUA_QUALIFIED = 1 THEN '是' ELSE '無' END), ";
  sql_stmt += "A.APP_AMOUNT, A.PERIOD, A.APR1, A.SEG1, ";
  sql_stmt += "A.APR2, A.SEG2, A.APR3, A.SEG3, ";
  sql_stmt += "A.GRACE_PERIOD, A.APP_FEE, A.OWNER_ID, A.OWNER_NAME, ";
  sql_stmt += "A.LAND_NUM, (CASE WHEN A.RELATIONSHIP = 0 THEN '本人' ";
  sql_stmt += "WHEN A.RELATIONSHIP = 1 THEN '父母' ";
  sql_stmt += "WHEN A.RELATIONSHIP = 2 THEN '配偶' ";
  sql_stmt += "WHEN A.RELATIONSHIP = 3 THEN '子女' ";
  sql_stmt += "WHEN A.RELATIONSHIP = 4 THEN '其他' END)";
  sql_stmt += "A.GAV, A.NAV, ";
  sql_stmt += "(CASE WHEN A.COL_QUALIFIED = 0 THEN '否' ELSE '是' END), ";
  sql_stmt += "(CASE WHEN A.PREMIUM_COL = 0 THEN '否' ELSE '是' END), ";
  sql_stmt += "A.MONTHLY_PAYMENT, ";
  sql_stmt += "B.APP_SCRMSG, B.COS_SCRMSG, B.GUA_SCRMSG,  ";
  sql_stmt += "B.PB, B.NPV, B.APPROVED_AMOUNT, ";
  sql_stmt += "B.MIN_RATE1, B.MIN_RATE2, B.MIN_RATE3, ";
  sql_stmt += "B.SUGG_MSG, B.REASON_MSG, B.BRANCH, B.SUGG_CODE ";
  sql_stmt += "FROM APP_INFO AS A INNER JOIN APP_RESULT AS B ";
  sql_stmt += "ON A.CASE_NO = B.CASE_NO ";
  if(!regen)
    sql_stmt += "WHERE B.Result_File_GEN_Time IS NULL ";
  else
    sql_stmt += "WHERE LEFT(B.Result_File_GEN_Time,10) = :report_gen_time ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(regen)
    command->Parameters->ParamValues["report_gen_time"] = report_gen_time;
  command->Execute();

  sql_stmt = "UPDATE REPORT SET CFC = C.CFC ";
  sql_stmt += "FROM REPORT AS A INNER JOIN BRANCH AS B ON A.BRANCH = B.BRANCH_CODE ";
  sql_stmt += "INNER JOIN CFC AS C ON C.CFC_CODE = B.CFC_CODE ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
};


//---------------------------------------------------------------------------
unsigned int check_rpt_time(const AnsiString &rpt_year,
                            const AnsiString &rpt_month,
                            const AnsiString &rpt_date,
                            const AnsiString &rpt_hour,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day, const unsigned int hour)
{
//0x0 for normal; 0x1 for month error; 0x2 for date error;
//0x4 for report in the future; 0x8 for hour error;
//0x20 for missing year;
  unsigned int return_value = 0;

  if(rpt_year.Trim().Length()==0)
    return_value |= 0x20;
  if(rpt_month.Trim().Length()==0)
    return_value |= 0x1;
  if(rpt_date.Trim().Length()==0)
    return_value |= 0x2;
  if(rpt_hour.Trim().Length()==0)
    return_value |= 0x8;
  if(return_value == 0){
    unsigned int year_rpt = rpt_year.Trim().ToInt();
    unsigned int month_rpt = rpt_month.Trim().ToInt();
    unsigned int date_rpt = rpt_date.Trim().ToInt();
    unsigned int hour_rpt = rpt_hour.Trim().ToInt();
    year_rpt += 1911;

    if(month_rpt > 12)
      return_value |= 0x1;
    else if(!check_date(year_rpt, month_rpt, date_rpt))
      return_value |= 0x2;
    else{
      bool first_report_in_future =
          in_future(year_rpt, month_rpt, date_rpt, hour_rpt, year, month, day, hour);
      if(first_report_in_future)
        return_value |= 0x4;
    };
  };
  return return_value;
};
//---------------------------------------------------------------------------
bool generate_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time, bool regen)
{
  AnsiString sql_stmt;

  try{
    AnsiString file_name_total = report_dir + "total_" + report_gen_time + ".csv";
    AnsiString file_name_individual = report_dir + "individual_" + report_gen_time + ".csv";
    AnsiString file_name_individual_formating = report_dir + "individual.csv";

    sql_stmt = "SELECT MIN(INPUT_TIME) AS MIN_TIME, MAX(INPUT_TIME) As MAX_TIME, COUNT(*) AS CNT FROM Report";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Open();
    if(query->FieldValues["CNT"] == 0){
      if(regen)
        message += "原檔案產生日期錯誤，報表產生中止。\n";
      else
        message += "沒有未產生報表的申請件。\n";
      frmReportGen->lblMessage->Caption = message;
      frmReportGen->Refresh();
      return true;
    };
    int records = query->FieldValues["CNT"];

    ofstream total(file_name_total.c_str(), ios_base::out);
    ofstream individual(file_name_individual.c_str(), ios_base::out);
    ofstream report_individual(file_name_individual_formating.c_str(), ios_base::out);

    AnsiString year = report_gen_time.SubString(1,4);
    year = static_cast<AnsiString>(year.ToInt() - 1911);
    AnsiString min_time = query->FieldByName("Min_Time")->AsString;
    AnsiString max_time = query->FieldByName("Max_Time")->AsString;
    AnsiString min_year = min_time.SubString(1,4);
    min_year = static_cast<AnsiString>(min_year.ToInt() - 1911);
    AnsiString max_year = max_time.SubString(1,4);
    max_year = static_cast<AnsiString>(max_year.ToInt() - 1911);


    if(total && individual && report_individual){
      total << "京城銀行房貸評分結果名單" << endl;
      total << "名單產生時間：" << ",";
      total << year.c_str() << "年 ";
      total << report_gen_time.SubString(5,2).c_str() << "月 ";
      total << report_gen_time.SubString(7,2).c_str() << "日 ";
      total << report_gen_time.SubString(9,2).c_str() << "時 ";
      total << report_gen_time.SubString(11,2).c_str() << "分 " << endl;
      total << "申請件輸入日期時間範圍：" << ",";
      total << min_year.c_str() << "年 ";
      total << min_time.SubString(5,2).c_str() << "月 ";
      total << min_time.SubString(7,2).c_str() << "日 ";
      total << min_time.SubString(9,2).c_str() << "時 ";
      total << min_time.SubString(11,2).c_str() << "分 ";
      total << "," << " _ " << ",";
      total << max_year.c_str() << "年 ";
      total << max_time.SubString(5,2).c_str() << "月 ";
      total << max_time.SubString(7,2).c_str() << "日 ";
      total << max_time.SubString(9,2).c_str() << "時 ";
      total << max_time.SubString(11,2).c_str() << "分 " << endl;
      total << "名單所含件數：" << "," << records;
      total << endl;
      total << "案件編號" << ",";
      total << "評分時間" << ",";
      total << "案件類別" << ",";
      total << "進件消金中心" << ",";

      total << "申請人身份證字號" << ",";
      total << "申請人姓名" << ",";
      total << "申請人生日" << ",";
      total << "申請人婚姻狀況" << ",";
      total << "申請人學歷" << ",";
      total << "申請人年收入(新台幣元)" << ",";
      total << "申請人符合承作條件" << ",";

      total << "共同申請人身份證字號" << ",";
      total << "共同申請人姓名" << ",";
      total << "共同申請人生日" << ",";
      total << "共同申請人婚姻狀況" << ",";
      total << "共同申請人學歷" << ",";
      total << "共同申請人年收入(新台幣元)" << ",";
      total << "共同申請人符合承作條件" << ",";

      total << "保證人身份證字號" << ",";
      total << "保證人姓名" << ",";
      total << "保證人生日" << ",";
      total << "保證人婚姻狀況" << ",";
      total << "保證人學歷" << ",";
      total << "保證人年收入(新台幣元)" << ",";
      total << "保證人符合承作條件" << ",";

      total << "申貸金額(新台幣元)" << ",";
      total << "申貸期間(月)" << ",";
      total << "第一段利率" << ",";
      total << "第一段期間(月)" << ",";
      total << "第二段利率" << ",";
      total << "第二段期間(月)" << ",";
      total << "第三段利率" << ",";
      total << "第三段期間(月)" << ",";
      total << "寬限期(月)" << ",";
      total << "開辦費(新台幣元)" << ",";

      total << "所有人身分證字號" << ",";
      total << "所有人姓名" << ",";
      total << "地號/建號" << ",";
      total << "與申請人關係" << ",";
      total << "房屋毛值(新台幣元)" << ",";
      total << "房屋淨值(新台幣元)" << ",";
      total << "擔保品符合承作條件" << ",";
      total << "擔保品區段良好且搭配優良建設公司" << ",";
      total << "目前房貸月付金(新台幣元)" << ",";

      total << "申請人 PRESCREEN訊息" << ",";
      total << "共同借款人 PRESCREEN訊息" << ",";
      total << "保證人 PRESCREEN訊息" << ",";
      total << "估計壞帳機率" << ",";
      total << "估計經濟價值(新台幣元)" << ",";
      total << "核貸金額(新台幣元)" << ",";
      total << "第一段最低可貸利率" << ",";
      total << "第二段最低可貸利率" << ",";
      total << "第三段最低可貸利率" << ",";
      total << "審核結果訊息" << ",";
      total << "審核附加訊息" << endl;

      report_individual << "Case_No" <<",";
      report_individual << "Scoring_time" <<",";
      report_individual << "product_type" <<",";
      report_individual << "CFC" <<",";
      report_individual << "report_gen_time" <<",";

      report_individual << "app_id" <<",";
      report_individual << "app_name" <<",";
      report_individual << "app_birthday" <<",";
      report_individual << "app_marraige" <<",";
      report_individual << "app_education" <<",";
      report_individual << "app_income" <<",";
      report_individual << "app_qualified" <<",";

      report_individual << "cos_id" <<",";
      report_individual << "cos_name" <<",";
      report_individual << "cos_birthday" <<",";
      report_individual << "cos_marraige" <<",";
      report_individual << "cos_education" <<",";
      report_individual << "cos_income" <<",";
      report_individual << "cos_qualified" <<",";

      report_individual << "gua_id" <<",";
      report_individual << "gua_name" <<",";
      report_individual << "gua_birthday" <<",";
      report_individual << "gua_marraige" <<",";
      report_individual << "gua_education" <<",";
      report_individual << "gua_income" <<",";
      report_individual << "gua_qualified" <<",";

      report_individual << "app_amt" <<",";
      report_individual << "period" <<",";
      report_individual << "apr1" <<",";
      report_individual << "seg1" <<",";
      report_individual << "apr2" <<",";
      report_individual << "seg2" <<",";
      report_individual << "apr3" <<",";
      report_individual << "seg3" <<",";
      report_individual << "grace_period" <<",";
      report_individual << "app_fee" <<",";

      report_individual << "owner_id" <<",";
      report_individual << "owner_name" <<",";
      report_individual << "land_num" <<",";
      report_individual << "relationship" <<",";
      report_individual << "gav" <<",";
      report_individual << "nav" <<",";
      report_individual << "col_qualified" <<",";
      report_individual << "premium_col" <<",";
      report_individual << "monthly_payment" <<",";

      report_individual << "app_scrmsg" <<",";
      report_individual << "cos_scrmsg" <<",";
      report_individual << "gua_scrmsg" <<",";
      report_individual << "pb" <<",";
      report_individual << "npv" <<",";
      report_individual << "approved_amount" <<",";
      report_individual << "min_rate1" <<",";
      report_individual << "min_rate2" <<",";
      report_individual << "min_rate3" <<",";
      report_individual << "sugg_msg" <<",";
      report_individual << "reason_msg" << endl;


      individual << "Case_No" <<",";
      individual << "Scoring_time" <<",";
      individual << "product_type" <<",";
      individual << "CFC" <<",";
      individual << "report_gen_time" <<",";

      individual << "app_id" <<",";
      individual << "app_name" <<",";
      individual << "app_birthday" <<",";
      individual << "app_marraige" <<",";
      individual << "app_education" <<",";
      individual << "app_income" <<",";
      individual << "app_qualified" <<",";

      individual << "cos_id" <<",";
      individual << "cos_name" <<",";
      individual << "cos_birthday" <<",";
      individual << "cos_marraige" <<",";
      individual << "cos_education" <<",";
      individual << "cos_income" <<",";
      individual << "cos_qualified" <<",";

      individual << "gua_id" <<",";
      individual << "gua_name" <<",";
      individual << "gua_birthday" <<",";
      individual << "gua_marraige" <<",";
      individual << "gua_education" <<",";
      individual << "gua_income" <<",";
      individual << "gua_qualified" <<",";

      individual << "app_amt" <<",";
      individual << "period" <<",";
      individual << "apr1" <<",";
      individual << "seg1" <<",";
      individual << "apr2" <<",";
      individual << "seg2" <<",";
      individual << "apr3" <<",";
      individual << "seg3" <<",";
      individual << "grace_period" <<",";
      individual << "app_fee" <<",";

      individual << "owner_id" <<",";
      individual << "owner_name" <<",";
      individual << "land_num" <<",";
      individual << "relationship" <<",";
      individual << "gav" <<",";
      individual << "nav" <<",";
      individual << "col_qualified" <<",";
      individual << "premium_col" <<",";
      individual << "monthly_payment" <<",";

      individual << "app_scrmsg" <<",";
      individual << "cos_scrmsg" <<",";
      individual << "gua_scrmsg" <<",";
      individual << "pb" <<",";
      individual << "npv" <<",";
      individual << "approved_amount" <<",";
      individual << "min_rate1" <<",";
      individual << "min_rate2" <<",";
      individual << "min_rate3" <<",";
      individual << "sugg_msg" <<",";
      individual << "reason_msg" << endl;


      query->Close();
      sql_stmt = "SELECT * FROM Report ORDER BY SCORE_TIME, CFC;";
      sql_stmt = sql_stmt.UpperCase();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Open();
      while(!query->Eof){
        total << query->FieldByName("CASE_NO")->AsString.c_str() << ",";
        total << query->FieldByName("SCORE_TIME")->AsString.c_str() << ",";
        total << query->FieldByName("PRODUCT_TYPE")->AsString.c_str() << ",";
        total << query->FieldByName("CFC")->AsString.c_str() << ",";

        total << query->FieldByName("APP_ID")->AsString.c_str() << ",";
        total << query->FieldByName("APP_NAME")->AsString.c_str() << ",";
        total << query->FieldByName("APP_BIRTHDAY")->AsString.c_str() << ",";
        total << query->FieldByName("APP_MARRIAGE")->AsString.c_str() << ",";
        total << query->FieldByName("APP_EDUCATION")->AsString.c_str() << ",";
        total << query->FieldByName("APP_INCOME")->AsInteger << ",";
        total << query->FieldByName("APP_QUALIFIED")->AsString.c_str() << ",";

        total << query->FieldByName("COS_ID")->AsString.c_str() << ",";
        total << query->FieldByName("COS_NAME")->AsString.c_str() << ",";
        total << query->FieldByName("COS_BIRTHDAY")->AsString.c_str() << ",";
        total << query->FieldByName("COS_MARRIAGE")->AsString.c_str() << ",";
        total << query->FieldByName("COS_EDUCATION")->AsString.c_str() << ",";
        total << query->FieldByName("COS_INCOME")->AsInteger << ",";
        total << query->FieldByName("COS_QUALIFIED")->AsString.c_str() << ",";

        total << query->FieldByName("GUA_ID")->AsString.c_str() << ",";
        total << query->FieldByName("GUA_NAME")->AsString.c_str() << ",";
        total << query->FieldByName("GUA_BIRTHDAY")->AsString.c_str() << ",";
        total << query->FieldByName("GUA_MARRIAGE")->AsString.c_str() << ",";
        total << query->FieldByName("GUA_EDUCATION")->AsString.c_str() << ",";
        total << query->FieldByName("GUA_INCOME")->AsInteger << ",";
        total << query->FieldByName("GUA_QUALIFIED")->AsString.c_str() << ",";

        total << query->FieldByName("APP_AMT")->AsInteger << ",";
        total << query->FieldByName("PERIOD")->AsInteger << ",";
        total << query->FieldByName("APR1")->AsFloat << ",";
        total << query->FieldByName("SEG1")->AsInteger << ",";
        total << query->FieldByName("APR2")->AsFloat << ",";
        total << query->FieldByName("SEG2")->AsInteger << ",";
        total << query->FieldByName("APR3")->AsFloat << ",";
        total << query->FieldByName("SEG3")->AsInteger << ",";
        total << query->FieldByName("GRACE_PERIOD")->AsInteger << ",";
        total << query->FieldByName("APP_FEE")->AsInteger << ",";

        total << query->FieldByName("OWNER_ID")->AsString.c_str() << ",";
        total << query->FieldByName("OWNER_NAME")->AsString.c_str() << ",";
        total << query->FieldByName("LAND_NUM")->AsString.c_str() << ",";
        total << query->FieldByName("RELATIONSHIP")->AsString.c_str() << ",";
        total << query->FieldByName("GAV")->AsInteger << ",";
        total << query->FieldByName("NAV")->AsInteger << ",";
        total << query->FieldByName("COL_QUALIFIED")->AsString.c_str() << ",";
        total << query->FieldByName("PREMIUM_COL")->AsString.c_str() << ",";
        total << query->FieldByName("MONTHLY_PAYMENT")->AsInteger << ",";


        total << query->FieldByName("APP_SCRMSG")->AsString.c_str() << ",";
        total << query->FieldByName("COS_SCRMSG")->AsString.c_str() << ",";
        total << query->FieldByName("GUA_SCRMSG")->AsString.c_str() << ",";
        total << query->FieldByName("PB")->AsFloat << ",";
        total << query->FieldByName("NPV")->AsInteger << ",";
        total << query->FieldByName("APPROVED_AMOUNT")->AsInteger << ",";
        total << query->FieldByName("MIN_RATE1")->AsFloat << ",";
        total << query->FieldByName("MIN_RATE2")->AsFloat << ",";
        total << query->FieldByName("MIN_RATE3")->AsFloat << ",";
        total << query->FieldByName("SUGG_MSG")->AsString.c_str() << ",";
        total << query->FieldByName("REASON_MSG")->AsString.c_str() << endl;

        report_individual << query->FieldByName("CASE_NO")->AsString.c_str() << ",";
        report_individual << query->FieldByName("SCORE_TIME")->AsString.c_str() << ",";
        report_individual << query->FieldByName("PRODUCT_TYPE")->AsString.c_str() << ",";
        report_individual << query->FieldByName("CFC")->AsString.c_str() << ",";
        report_individual << report_gen_time.c_str() << ",";

        report_individual << query->FieldByName("APP_ID")->AsString.c_str() << ",";
        report_individual << query->FieldByName("APP_NAME")->AsString.c_str() << ",";
        report_individual << query->FieldByName("APP_BIRTHDAY")->AsString.c_str() << ",";
        report_individual << query->FieldByName("APP_MARRIAGE")->AsString.c_str() << ",";
        report_individual << query->FieldByName("APP_EDUCATION")->AsString.c_str() << ",";
        report_individual << query->FieldByName("APP_INCOME")->AsInteger << ",";
        report_individual << query->FieldByName("APP_QUALIFIED")->AsString.c_str() << ",";

        report_individual << query->FieldByName("COS_ID")->AsString.c_str() << ",";
        report_individual << query->FieldByName("COS_NAME")->AsString.c_str() << ",";
        report_individual << query->FieldByName("COS_BIRTHDAY")->AsString.c_str() << ",";
        report_individual << query->FieldByName("COS_MARRIAGE")->AsString.c_str() << ",";
        report_individual << query->FieldByName("COS_EDUCATION")->AsString.c_str() << ",";
        report_individual << query->FieldByName("COS_INCOME")->AsInteger << ",";
        report_individual << query->FieldByName("COS_QUALIFIED")->AsString.c_str() << ",";

        report_individual << query->FieldByName("GUA_ID")->AsString.c_str() << ",";
        report_individual << query->FieldByName("GUA_NAME")->AsString.c_str() << ",";
        report_individual << query->FieldByName("GUA_BIRTHDAY")->AsString.c_str() << ",";
        report_individual << query->FieldByName("GUA_MARRIAGE")->AsString.c_str() << ",";
        report_individual << query->FieldByName("GUA_EDUCATION")->AsString.c_str() << ",";
        report_individual << query->FieldByName("GUA_INCOME")->AsInteger << ",";
        report_individual << query->FieldByName("GUA_QUALIFIED")->AsString.c_str() << ",";

        report_individual << query->FieldByName("APP_AMT")->AsInteger << ",";
        report_individual << query->FieldByName("PERIOD")->AsInteger << ",";
        report_individual << query->FieldByName("APR1")->AsFloat << ",";
        report_individual << query->FieldByName("SEG1")->AsInteger << ",";
        report_individual << query->FieldByName("APR2")->AsFloat << ",";
        report_individual << query->FieldByName("SEG2")->AsInteger << ",";
        report_individual << query->FieldByName("APR3")->AsFloat << ",";
        report_individual << query->FieldByName("SEG3")->AsInteger << ",";
        report_individual << query->FieldByName("GRACE_PERIOD")->AsInteger << ",";
        report_individual << query->FieldByName("APP_FEE")->AsInteger << ",";

        report_individual << query->FieldByName("OWNER_ID")->AsString.c_str() << ",";
        report_individual << query->FieldByName("OWNER_NAME")->AsString.c_str() << ",";
        report_individual << query->FieldByName("LAND_NUM")->AsString.c_str() << ",";
        report_individual << query->FieldByName("RELATIONSHIP")->AsString.c_str() << ",";
        report_individual << query->FieldByName("GAV")->AsInteger << ",";
        report_individual << query->FieldByName("NAV")->AsInteger << ",";
        report_individual << query->FieldByName("COL_QUALIFIED")->AsString.c_str() << ",";
        report_individual << query->FieldByName("PREMIUM_COL")->AsString.c_str() << ",";
        report_individual << query->FieldByName("MONTHLY_PAYMENT")->AsInteger << ",";

        report_individual << query->FieldByName("APP_SCRMSG")->AsString.c_str() << ",";
        report_individual << query->FieldByName("COS_SCRMSG")->AsString.c_str() << ",";
        report_individual << query->FieldByName("GUA_SCRMSG")->AsString.c_str() << ",";
        report_individual << query->FieldByName("PB")->AsFloat << ",";
        report_individual << query->FieldByName("NPV")->AsInteger << ",";
        report_individual << query->FieldByName("APPROVED_AMOUNT")->AsInteger << ",";
        report_individual << query->FieldByName("MIN_RATE1")->AsFloat << ",";
        report_individual << query->FieldByName("MIN_RATE2")->AsFloat << ",";
        report_individual << query->FieldByName("MIN_RATE3")->AsFloat << ",";
        report_individual << query->FieldByName("SUGG_MSG")->AsString.c_str() << ",";
        report_individual << query->FieldByName("REASON_MSG")->AsString.c_str() << endl;

        individual << query->FieldByName("CASE_NO")->AsString.c_str() << ",";
        individual << query->FieldByName("SCORE_TIME")->AsString.c_str() << ",";
        individual << query->FieldByName("PRODUCT_TYPE")->AsString.c_str() << ",";
        individual << query->FieldByName("CFC")->AsString.c_str() << ",";
        individual << report_gen_time.c_str() << ",";

        individual << query->FieldByName("APP_ID")->AsString.c_str() << ",";
        individual << query->FieldByName("APP_NAME")->AsString.c_str() << ",";
        individual << query->FieldByName("APP_BIRTHDAY")->AsString.c_str() << ",";
        individual << query->FieldByName("APP_MARRIAGE")->AsString.c_str() << ",";
        individual << query->FieldByName("APP_EDUCATION")->AsString.c_str() << ",";
        individual << query->FieldByName("APP_INCOME")->AsInteger << ",";
        individual << query->FieldByName("APP_QUALIFIED")->AsString.c_str() << ",";

        individual << query->FieldByName("COS_ID")->AsString.c_str() << ",";
        individual << query->FieldByName("COS_NAME")->AsString.c_str() << ",";
        individual << query->FieldByName("COS_BIRTHDAY")->AsString.c_str() << ",";
        individual << query->FieldByName("COS_MARRIAGE")->AsString.c_str() << ",";
        individual << query->FieldByName("COS_EDUCATION")->AsString.c_str() << ",";
        individual << query->FieldByName("COS_INCOME")->AsInteger << ",";
        individual << query->FieldByName("COS_QUALIFIED")->AsString.c_str() << ",";

        individual << query->FieldByName("GUA_ID")->AsString.c_str() << ",";
        individual << query->FieldByName("GUA_NAME")->AsString.c_str() << ",";
        individual << query->FieldByName("GUA_BIRTHDAY")->AsString.c_str() << ",";
        individual << query->FieldByName("GUA_MARRIAGE")->AsString.c_str() << ",";
        individual << query->FieldByName("GUA_EDUCATION")->AsString.c_str() << ",";
        individual << query->FieldByName("GUA_INCOME")->AsInteger << ",";
        individual << query->FieldByName("GUA_QUALIFIED")->AsString.c_str() << ",";

        individual << query->FieldByName("APP_AMT")->AsInteger << ",";
        individual << query->FieldByName("PERIOD")->AsInteger << ",";
        individual << query->FieldByName("APR1")->AsFloat << ",";
        individual << query->FieldByName("SEG1")->AsInteger << ",";
        individual << query->FieldByName("APR2")->AsFloat << ",";
        individual << query->FieldByName("SEG2")->AsInteger << ",";
        individual << query->FieldByName("APR3")->AsFloat << ",";
        individual << query->FieldByName("SEG3")->AsInteger << ",";
        individual << query->FieldByName("GRACE_PERIOD")->AsInteger << ",";
        individual << query->FieldByName("APP_FEE")->AsInteger << ",";

        individual << query->FieldByName("OWNER_ID")->AsString.c_str() << ",";
        individual << query->FieldByName("OWNER_NAME")->AsString.c_str() << ",";
        individual << query->FieldByName("LAND_NUM")->AsString.c_str() << ",";
        individual << query->FieldByName("RELATIONSHIP")->AsString.c_str() << ",";
        individual << query->FieldByName("GAV")->AsInteger << ",";
        individual << query->FieldByName("NAV")->AsInteger << ",";
        individual << query->FieldByName("COL_QUALIFIED")->AsString.c_str() << ",";
        individual << query->FieldByName("PREMIUM_COL")->AsString.c_str() << ",";
        individual << query->FieldByName("MONTHLY_PAYMENT")->AsInteger << ",";

        individual << query->FieldByName("APP_SCRMSG")->AsString.c_str() << ",";
        individual << query->FieldByName("COS_SCRMSG")->AsString.c_str() << ",";
        individual << query->FieldByName("GUA_SCRMSG")->AsString.c_str() << ",";
        individual << query->FieldByName("PB")->AsFloat << ",";
        individual << query->FieldByName("NPV")->AsInteger << ",";
        individual << query->FieldByName("APPROVED_AMOUNT")->AsInteger << ",";
        individual << query->FieldByName("MIN_RATE1")->AsFloat << ",";
        individual << query->FieldByName("MIN_RATE2")->AsFloat << ",";
        individual << query->FieldByName("MIN_RATE3")->AsFloat << ",";
        individual << query->FieldByName("SUGG_MSG")->AsString.c_str() << ",";
        individual << query->FieldByName("REASON_MSG")->AsString.c_str() << endl;
        query->Next();
      };
      total << "/EOF" << endl;
      total.clear();
      total.close();
      report_individual.clear();
      report_individual.close();
      individual.clear();
      individual.close();
      query->Close();
    }
    else
      return false;

    if (!regen){
      sql_stmt = "UPDATE Result SET Result_File_GEN_Time = :result_gen_time ";
      sql_stmt += "WHERE Result_File_GEN_Time IS NULL;";
      sql_stmt = sql_stmt.UpperCase();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["result_gen_time"] = report_gen_time;
      query->ExecSQL();
    };
    return true;
  }
  catch(Exception &E){
    frmReportGen->lblMessage->Caption = E.Message;
    frmReportGen->Refresh();
    return false;
  };
};

void clean_report(TADOCommand *command)
{
  AnsiString sql_stmt;
  try{
    sql_stmt = "DROP TABLE Report";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
};

bool in_future(unsigned int year_rpt, unsigned int month_rpt,
               unsigned int date_rpt, unsigned int hour_rpt,
               unsigned int year, unsigned int month,
               unsigned int day, unsigned int hour)
{
  bool return_value = false;
  int age_year = year - year_rpt;
  int age_month = month - month_rpt;
  int age_day = day - date_rpt;
  int age_hour = hour - hour_rpt;

  if(age_year < 0)
    return_value = true;
  else if (age_year > 0)
    return_value = false;
  else if (age_year == 0){
    if(age_month < 0)
      return_value = true;
    else if(age_month > 0)
      return_value = false;
    else if(age_month == 0){
      if(age_day < 0)
        return_value = true;
      else if(age_day > 0)
        return_value = false;
      else if(age_day == 0){
        if(age_hour < 0)
          return_value = true;
        else
          return_value = false;
      }
    }
  };

  return return_value;
};


void __fastcall TfrmReportGen::btnRptRegenClick(TObject *Sender)
{
  AnsiString dir_message = (report_regen_dir.Length() == 0) ? static_cast<AnsiString>("程式所在目錄") : report_regen_dir;
  dir_message += "\n";
  AnsiString message = "報表將會儲存在: " + dir_message;
  lblMessage->Caption = message;
  lblMessage->Refresh();
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if(config){
    config << report_dir.c_str() << endl;
    config << report_regen_dir.c_str() << endl;
    config.clear();
    config.close();
  };

  AnsiString report_gen_time;
  bool is_error = false;

  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  unsigned int second;

  get_time(year, month, day, hour, min, second);

//0x0 for normal; 0x1 for month error; 0x2 for date error;
//0x4 for report in the future; 0x8 for hour error;
//0x20 for missing year;
  unsigned int bad_date = check_rpt_time(medtRptYear->Text,
                                         medtRptMonth->Text,
                                         medtRptDate->Text,
                                         medtRptHour->Text,
                                         year, month, day, hour);
  if(bad_date > 0){
    is_error = true;
    if((bad_date & 0x1) == 0x1)
      message += "評分結果報表初次產生月份錯誤\n";
    if((bad_date & 0x2) == 0x2)
      message += "評分結果報表初次產生日期錯誤\n";
    if((bad_date & 0x4) == 0x4)
      message += "評分結果報表初次產生時間在未來\n";
    if((bad_date & 0x8) == 0x8)
      message += "評分結果報表初次產生時間錯誤\n";
    if((bad_date & 0x20) == 0x20)
      message += "評分結果報表初次產生年份錯誤\n";
    lblMessage->Caption = message;
    frmReportGen->Refresh();
  };
  if(!is_error){
    AnsiString report_gen_year = static_cast<AnsiString>(medtRptYear->Text.Trim().ToInt() + 1911);
    AnsiString report_gen_month = medtRptMonth->Text.Trim();
    report_gen_month = report_gen_month.Length() == 1 ? "0" + report_gen_month : report_gen_month;
    AnsiString report_gen_day = medtRptDate->Text.Trim();
    report_gen_day = report_gen_day.Length() == 1 ? "0" + report_gen_day : report_gen_day;
    AnsiString report_gen_hour = medtRptHour->Text.Trim();
    report_gen_hour = report_gen_hour.Length() == 1 ? "0" + report_gen_hour : report_gen_hour;
    AnsiString report_gen_time = report_gen_year + report_gen_month + report_gen_day + report_gen_hour;


    prepare_report(Data->command, report_gen_time, true);
    bool success = generate_report(Data->query, report_regen_dir, report_gen_time, true);
    if(!success){
      message += "審核報表產生錯誤。\n";
      lblMessage->Caption = message;
    }
    else{
      message += "審核報表已重新產生。\n";
      lblMessage->Caption = message;
    };
    frmReportGen->Refresh();
    clean_report(Data->command);
  };
}
//---------------------------------------------------------------------------

