//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUI.h"
#include "Data_Module.h"
#include "LoginUI.h"
#include "DirUI.h"
#include "ReDirUI.h"
#include "CPDACO.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMainUI *frmMainUI;
AnsiString working_dir;
AnsiString report_dir;
AnsiString report_regen_dir;
AnsiString message;
bool is_input_error;
//---------------------------------------------------------------------------
__fastcall TfrmMainUI::TfrmMainUI(TComponent* Owner)
  : TForm(Owner)
{
  CoInitialize(NULL);
};
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::FormCreate(TObject *Sender)
{
  lblMessage->Caption = "";
  message = "";
  frmMainUI->Hide();
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

};
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnExit1Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnScoreClick(TObject *Sender)
{
  is_input_error = false;
  unsigned int gender;
  int application_amount, application_terms;
  lblMessage->Caption = "";
  message = "";
  frmMainUI->Refresh();
  unsigned int no_phone_flag = 0;
  unsigned int unsecured_balance_opt = 0;                                             \
  unsigned int unsecured_balance_per = 0;
  unsigned int max_line = 0;
  unsigned int monthly_income;
  AnsiString msn, product_code;
  msn = medtMSN->Text;

  if(!check_income(medtMonthlyIncome->Text.Trim(), monthly_income)){
    message += "收入錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if((medtRegion->Text.Trim().Length()>0) || (medtPhone->Text.Trim().Length() > 0)){
    if(!check_phone(medtRegion->Text.Trim(), medtPhone->Text.Trim(), false)){
      message += "聯絡電話號碼錯誤\n";
      lblMessage->Caption = message;
      is_input_error = true;
    };
  }
  else
    no_phone_flag |= 0x1;
  if((medtCellRegion->Text.Trim().Length()>0) || (medtCellPhone->Text.Trim().Length() > 0)){
    if(!check_phone(medtCellRegion->Text.Trim(), medtCellPhone->Text.Trim(), true)){
      message += "行動電話號碼錯誤\n";
      lblMessage->Caption = message;
      is_input_error = true;
    };
  }
  else
    no_phone_flag |= 0x2;

  if(no_phone_flag == 3){
    message += "電話號碼未輸入\n";
    lblMessage->Caption = message;
    is_input_error = true;
  }
  application_amount = medtAmountApplied->Text.Trim().ToInt() * 10000;
  if((application_amount < 150000) || (application_amount > 1000000)){
    message += "申貸金額錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  switch (cmbAppliedTerms->ItemIndex){
    case 0:
      application_terms = 24;
      break;
    case 1:
      application_terms = 36;
      break;
    case 2:
      application_terms = 60;
      break;
    case 3:
      application_terms = 84;
      break;
    default:
      message += "未選擇貸款期數\n";
      lblMessage->Caption = message;
      is_input_error = true;
  };

  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;

  get_time(year, month, day, hour, min);

  frmMainUI->Refresh();

  if(!is_input_error){
    AnsiString id = medtID->Text.Trim();
    if(id.SubString(2,1)!= "1" && id.SubString(2,1)!= "2")
      gender = -1;
    else{
      gender = id.SubString(2,1).ToInt();
      gender -= 2;
      gender *= -1;
    };
    AnsiString year_str = static_cast<AnsiString>(year);
    AnsiString month_str = static_cast<AnsiString>(month);
    AnsiString day_str = static_cast<AnsiString>(day);
    AnsiString hour_str = static_cast<AnsiString>(hour);
    AnsiString min_str = static_cast<AnsiString>(min);

    if(month_str.Length() == 1)
      month_str = "0" + month_str;
    if(day_str.Length() == 1)
      day_str = "0" + day_str;
    if(hour_str.Length() == 1)
      hour_str = "0" + hour_str;
    if(min_str.Length() == 1)
      min_str = "0" + min_str;

    AnsiString input_time = year_str + month_str + day_str + hour_str + min_str;

    double apr;
    unsigned int application_fee;


    try{
      get_product_code(msn, product_code, Data->query);
      get_product_feature(product_code, Data->query, apr, application_fee);
      bool contiune = check_and_cleanup_result(msn, Data->query);
      if(!contiune)
        throw Exception("申請件重複，使用者選擇終止評分。");

      message += "申請件資料通過檢核，正在取回聯徵資料。\n";
      lblMessage->Caption = message;
      frmMainUI->Refresh();

      AnsiString query_sn;
      AnsiString jcic_inquiry_result;
      AnsiString jcic_inquiry_date;
      AnsiString ole_db = "Provider=SQLOLEDB.1;Password=dac_ploan_1;Persist Security Info=True;User ID=dac_ploan_1;Initial Catalog=PLOAN_MKT;Data Source=DAC-DB2";
//      ole_db = "Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=PLOAN_MKT;Data Source=NBCOMPUTER\\NBCOMPUTER2K";
      char error[256];
      int module_operation;
      bool success;
      int ejcic_error_code;

      success = request_for_jcic_data(Data->ejcic_query, medtID->Text.Trim(), query_sn, jcic_inquiry_result, ejcic_error_code);
      if(success){
        message += "聯徵資料已取回，正在儲存資料。\n";
        lblMessage->Caption = message;
        frmMainUI->Refresh();
        store_input(Data->command, Data->query,
                    msn, medtID->Text.Trim(),
                    input_time,
                    edtName->Text, monthly_income, product_code,
                    medtRegion->Text.Trim() + medtPhone->Text.Trim(),
                    medtCellRegion->Text.Trim() + medtCellPhone->Text.Trim(),
                    application_amount, application_terms, apr, application_fee);

        jcic_inquiry_date =
          get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn, medtID->Text, input_time, medtMSN->Text);
//              record_scoring_time(Data->command, medtPrimaryID->Text,
//                                  input_time, true);
        message += "資料已儲存，正在評分中。\n";
        lblMessage->Caption = message;
        frmMainUI->Refresh();
        module_operation =
            TNB_Ploan_AM_Campaign(medtMSN->Text.c_str(), jcic_inquiry_date.c_str(), input_time.c_str(),
                                  "054", ole_db.c_str(), gender, error);

//            TNB_CC_AM(medtPrimaryID->Text.c_str(),
//                      jcic_inquiry_date.c_str(), input_time.c_str(),
//                      "054", ole_db.c_str(), gender, error);
//              record_scoring_time(Data->command, medtPrimaryID->Text,
//                                  input_time, false);
        if(module_operation != 0){
          lblMessage->Caption = static_cast<AnsiString>(error);
          frmMainUI->Refresh();
        };
      }
      else{
  //        log_error(Data->command, medtPrimaryID->Text, jcic_inquiry_result, ejcic_error_code);
        message += ("e JCIC 錯誤，代碼：" + static_cast<AnsiString>(ejcic_error_code) + " 訊息：" + jcic_inquiry_result + "\n");
        lblMessage->Caption = message;
        frmMainUI->Refresh();
        module_operation = 999;
      };
      if(module_operation == 0){

        lblMessage->Caption = "評分完成。可以輸入下一筆。\n";
        frmMainUI->Refresh();
      };
    }
    catch(Exception &E){
      message += E.Message;
      lblMessage->Caption = message;
      frmMainUI->Refresh();
    };
    if(lblMessage->Caption == "評分完成。可以輸入下一筆。\n")
      init_UI();
  };
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnClearClick(TObject *Sender)
{
  medtID->Clear();
  edtName->Clear();
  medtMSN->Clear();
  medtPhone->Clear();
  medtRegion->Clear();
  medtCellPhone->Clear();
  medtCellRegion->Clear();
  medtMonthlyIncome->Clear();
  medtAmountApplied->Clear();
  cmbAppliedTerms->ItemIndex = -1;
  medtMSN->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::pcUIEnter(TObject *Sender)
{
  edtName->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnExit2Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnExit3Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------
//Check on id based on the pre-loaded msn_idn table
bool check_id(const AnsiString &idn, const AnsiString &msn,
              TADOQuery *query)
{
  bool result = false;   
  AnsiString sql_stmt;
  sql_stmt = "SELECT IDN FROM MAIL_LIST WHERE MSN = :msn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  if(query->FieldValues["IDN"] == idn)
    result = true;

  return result;
};
//---------------------------------------------------------------------------


unsigned int check_birthday(const AnsiString &birth_year,
                            const AnsiString &birth_month,
                            const AnsiString &birth_date,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day,
                            bool primary_card_applicant)
{
//0x0 for normal; 0x1 for month error; 0x2 for date error;
//0x4 for born in the future; 0x8 for primary card applicant under age;
//0x10 for secondary card applicant under age;
//0x20 for missing birth year;
  unsigned int return_value = 0;

  if(birth_year.Length()==0)
    return_value |= 0x20;
  if(birth_month.Length()==0)
    return_value |= 0x1;
  if(birth_date.Length()==0)
    return_value |= 0x2;
  if(return_value == 0){
    unsigned int year_born = birth_year.ToInt();
    unsigned int month_born = birth_month.ToInt();
    unsigned int date_born = birth_date.ToInt();
    year_born += 1900;

    if(month_born > 12)
      return_value |= 0x1;
    else if(!check_date(year_born, month_born, date_born))
      return_value |= 0x2;
    else{
      int age = calculate_age(year_born, month_born, date_born, year, month, day);
      if(age < 0)
        return_value |= 0x4;
      if(primary_card_applicant){
        if(age < 20)
          return_value |= 0x8;
      }
      else{
        if(age < 15)
          return_value |= 0x10;
      }
    }
  };
  return return_value;
};

bool check_date(unsigned int year, unsigned int month, unsigned int date)
{
//true for valid; false for invalid
  bool return_value = true;
  switch (month){
    case 1, 3, 5, 7, 8, 10, 12:
      if(date > 31)
        return_value = false;
      break;
    case 4, 6, 9, 11:
      if(date > 30)
        return_value = false;
      break;
    case 2:
      bool leap_year = check_leap_year(year);
      if(!leap_year && (date > 28))
        return_value = false;
      else if(leap_year && (date > 29))
        return_value = false;
      break;
  };
  return return_value;
};

bool check_leap_year(unsigned int year)
{
  bool return_value;
  if((year%4) == 0){
    if((year%100) == 0){
      if((year%400) == 0){
        if((year%4000) == 0)
          return_value = false;
        else
          return_value = true;
      }
      else
        return_value = false;
    }
    else
      return_value = true;
  }
  else
    return_value = false;
  return return_value;
};

int calculate_age(unsigned int year_born, unsigned int month_born, unsigned int date_born,
                           unsigned int year, unsigned int month, unsigned int date)
{
  int age_year = year - year_born;
  int age_month = month - month_born;
  int age_day = date - date_born;
  if(age_day < 0)
    --age_month;
  if(age_month < 0)
    --age_year;
  return age_year;
};

bool check_zip(string zip)
{
 bool return_value = true;
 string valid_TW_Zip_3[] =  {    // Valid Taiwan 3 digit zip codes
     "100", "302", "424", "614", "743", "922",
     "103", "303", "426", "615", "744", "923",
     "104", "304", "427", "616", "745", "924",
     "105", "305", "428", "621", "800", "925",
     "106", "306", "429", "622", "801", "926",
     "108", "307", "432", "623", "802", "927",
     "110", "308", "433", "624", "803", "928",
     "111", "310", "434", "625", "804", "929",
     "112", "311", "435", "630", "805", "931",
     "114", "312", "436", "631", "806", "932",
     "115", "313", "437", "632", "807", "940",
     "116", "314", "438", "633", "811", "941",
     "200", "315", "439", "634", "812", "942",
     "201", "320", "500", "635", "813", "943",
     "202", "324", "502", "636", "814", "944",
     "203", "325", "503", "637", "815", "945",
     "204", "326", "504", "638", "820", "946",
     "205", "327", "505", "640", "821", "947",
     "206", "328", "506", "643", "822", "950",
     "207", "330", "507", "646", "823", "951",
     "208", "333", "508", "647", "824", "952",
     "220", "334", "509", "648", "825", "953",
     "221", "335", "510", "649", "826", "954",
     "222", "336", "511", "651", "827", "955",
     "223", "337", "512", "652", "828", "956",
     "224", "338", "513", "653", "829", "957",
     "226", "350", "514", "654", "830", "958",
     "227", "351", "515", "655", "831", "959",
     "228", "352", "516", "700", "832", "961",
     "231", "353", "520", "701", "833", "962",
     "232", "354", "521", "702", "840", "963",
     "233", "356", "522", "704", "842", "964",
     "234", "357", "523", "708", "843", "965",
     "235", "358", "524", "709", "844", "966",
     "236", "360", "525", "710", "845", "970",
     "237", "361", "526", "711", "846", "971",
     "238", "362", "527", "712", "847", "972",
     "239", "363", "528", "713", "848", "973",
     "241", "364", "530", "714", "849", "974",
     "242", "365", "540", "715", "851", "975",
     "243", "366", "541", "716", "852", "976",
     "244", "367", "542", "717", "880", "977",
     "247", "368", "544", "718", "881", "978",
     "248", "369", "545", "719", "882", "979",
     "249", "400", "546", "720", "883", "981",
     "251", "401", "551", "721", "884", "982",
     "252", "402", "552", "722", "885", "983",
     "253", "403", "553", "723", "900", "890",
     "260", "404", "555", "724", "901", "891",
     "261", "406", "556", "725", "902", "892",
     "262", "407", "557", "726", "903", "893",
     "263", "408", "558", "727", "904", "894",
     "264", "411", "600", "730", "905", "896",
     "265", "412", "602", "731", "906", "209",
     "266", "413", "603", "732", "907", "210",
     "267", "414", "604", "733", "908", "211",
     "268", "420", "605", "734", "909", "212",
     "269", "421", "606", "735", "911", "817",
     "270", "422", "607", "736", "912", "819",
     "272", "423", "608", "737", "913", "290",
     "300",        "611", "741", "920",
                   "612", "742", "921",
                   "613", "703"
     };

 int zipCount = sizeof(valid_TW_Zip_3) / sizeof (valid_TW_Zip_3[0]);  // get no. of zip code
 // Declare a set with key only, Initial it with all zip code ascendingly
 set<string, less<string> > zipSet (valid_TW_Zip_3, valid_TW_Zip_3+zipCount+1);
 set<string, less<string> >::iterator iter;

 iter = zipSet.find(zip);
 if (iter == zipSet.end())
    return_value = false;

 return return_value;
};

bool check_phone(const AnsiString &region_code, const AnsiString &phone_no, bool celluar)
{
  bool return_value = true;
  if(celluar){
    if((phone_no.Length() != 6) || (region_code.SubString(1,2) != "09") ||
       (region_code.Length() != 4))
      return_value = false;
  }
  else{
    AnsiString long_phone_no = region_code.Trim() + phone_no.Trim();
    int len = long_phone_no.Length();
    if((long_phone_no.Length() != 10) && (long_phone_no.Length() != 9) && (!check_region_code(region_code.Trim())))
      return_value = false;
  };
  return return_value;
};

bool check_region_code(const AnsiString &region_code)
{
 bool return_value = true;
 if (region_code.SubString(1,1) != "0")
  return_value = false;
 else{
   int code = region_code.ToInt();
   switch (code){
    case 2,3,4,5,6,7,8,37,49,82,89,826,836:
      break;
    default:
      return_value = false;
   }
 };
 return return_value;
};

//---------------------------------------------------------------------------

void init_UI()
{
  frmMainUI->pcUI->ActivePageIndex = 0;
  frmMainUI->medtID->Clear();
  frmMainUI->medtMSN->Clear();
  frmMainUI->edtName->Clear();
  frmMainUI->medtCellPhone->Clear();
  frmMainUI->medtCellRegion->Clear();
  frmMainUI->medtPhone->Clear();
  frmMainUI->medtRegion->Clear();
  frmMainUI->medtMonthlyIncome->Clear();
  frmMainUI->medtAmountApplied->Clear();
  frmMainUI->cmbAppliedTerms->ItemIndex = -1;
//  frmMainUI->lblMessage->Caption = "";
  frmMainUI->Refresh();
};
//---------------------------------------------------------------------------

void get_time(unsigned int &year, unsigned int &month, unsigned int &day,
              unsigned int &hour, unsigned int &min)
{
    tm *today;
    time_t ltime;
    ltime = time( NULL );
    today = localtime( &ltime );
    year = today->tm_year + 1900;
    month = today->tm_mon + 1;
    day = today->tm_mday;
    hour = today->tm_hour;
    min = today->tm_min;
};
//---------------------------------------------------------------------------

void store_input(TADOCommand *command, TADOQuery *query,
                 const AnsiString &msn, const AnsiString &idn,
                 const AnsiString &input_time, const AnsiString &name,
                 unsigned int monthly_income, const AnsiString &product_code,
                 const AnsiString &phone, const AnsiString &cell_phone,
                 unsigned int application_amount, unsigned int application_terms,
                 double apr, unsigned int application_fee)
{
  AnsiString sql_stmt;

  sql_stmt = "INSERT INTO APPLICANT(MSN, IDN, CNAME, INPUT_TIME, PHONE, ";
  sql_stmt += "CELL_PHONE, APPLICATION_AMOUNT, APPLICATION_RATE, ";
  sql_stmt += "APPLICATION_TERMS, APPLICATION_FEE, MONTHLY_INCOME, PRODUCT_CODE) VALUES";
  sql_stmt += "(:msn, :idn, :cname, :input_time, :phone, :cell_phone, ";
  sql_stmt += ":application_amount, :apr, :application_terms, ";
  sql_stmt += ":application_fee, :income, :product_code)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn;
  command->Parameters->ParamValues["idn"] = idn;
  command->Parameters->ParamValues["cname"] = name;
  command->Parameters->ParamValues["input_time"] = input_time;
  command->Parameters->ParamValues["phone"] = phone;
  command->Parameters->ParamValues["cell_phone"] = cell_phone;
  command->Parameters->ParamValues["application_amount"] = application_amount;
  command->Parameters->ParamValues["product_code"] = product_code;
  command->Parameters->ParamValues["apr"] = apr;
  command->Parameters->ParamValues["application_terms"] = application_terms;
  command->Parameters->ParamValues["application_fee"] = application_fee;
  command->Parameters->ParamValues["income"] = monthly_income;
//  command->Parameters->ParamValues["dm_cell"] = dm_cell;
//  command->Parameters->ParamValues["dm_batch"] = dm_batch;
  command->Execute();                   

//Codes to handle unicode, comment out due to c++'s inability to write unicode file
/*  int pcount = 0;
  AnsiString w_sql_stmt;
  w_sql_stmt = "UPDATE APPLICANT SET CNAME = CONVERT(NVARCHAR(10), ?) WHERE MSN = ? AND INPUT_TIME = ?";
  command->ParamCheck = false;
  command->CommandText = w_sql_stmt;
  command->Parameters->AddParameter();
  pcount = command->Parameters->Count - 1;
  command->Parameters->Items[pcount]->Direction = pdInput;
  command->Parameters->Items[pcount]->DataType = ftVariant;
  command->Parameters->Items[pcount]->Value =  name;

  command->Parameters->AddParameter();
  pcount = command->Parameters->Count - 1;
  command->Parameters->Items[pcount]->Direction = pdInput;
  command->Parameters->Items[pcount]->DataType = ftString;
  command->Parameters->Items[pcount]->Value =  msn;


  command->Parameters->AddParameter();
  pcount = command->Parameters->Count - 1;
  command->Parameters->Items[pcount]->Direction = pdInput;
  command->Parameters->Items[pcount]->DataType = ftString;
  command->Parameters->Items[pcount]->Value = input_time;

  command->Execute();
  command->ParamCheck = true;*/
};
//The following is code from credit card, for edit to fit into personal loan

//---------------------------------------------------------------------------

AnsiString get_store_jcic_data(TADOConnection *ejcic_connection,
                               TADOQuery *ejcic_query, TADOCommand *command,
                               const AnsiString &query_sn, const AnsiString &idn,
                               const AnsiString &input_time,
                               const AnsiString &msn)
{
  if(!ejcic_connection->Connected)
    ejcic_connection->Open();
  ejcic_query->Close();

  AnsiString INQUIRY_DATE;
  AnsiString ejcic_sql_stmt;
  AnsiString sql_stmt;

  ejcic_sql_stmt = "SELECT RS_DTM_CHAR FROM TNBB_JCIC.JC_T_REQ WHERE QSEQNO = :query_sn";
  ejcic_sql_stmt = ejcic_sql_stmt.UpperCase();
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  INQUIRY_DATE = ejcic_query->FieldValues["RS_DTM_CHAR"];
  ejcic_query->Close();
  INQUIRY_DATE = INQUIRY_DATE.SubString(1,8);


  sql_stmt = "INSERT INTO KRM023(MSN, IDN, INQUIRY_DATE, YRMON, ISSUE, ISSUE_NAME, ";
  sql_stmt += "KR_CODE, LIMIT, PAYMENT, CASH, PAY_CODE, Input_Time) VALUES(:msn, :idn, ";
  sql_stmt += ":inquiry_date, :yrmon, :issue, :issue_name, :kr_code, :limit, ";
  sql_stmt += ":payment, :cash, :pay_code, :input_time);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_KRM023 WHERE QSEQNO = :query_sn";
  ejcic_sql_stmt = ejcic_sql_stmt.UpperCase();
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["idn"] = idn;
  	command->Parameters->ParamValues["msn"] = msn;
	  command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["yrmon"] = ejcic_query->FieldValues["YRMON"];
	  command->Parameters->ParamValues["issue"] = ejcic_query->FieldValues["ISSUE"];
 	  command->Parameters->ParamValues["issue_name"] = ejcic_query->FieldValues["ISSUE_NAME"];
	  command->Parameters->ParamValues["kr_code"] = ejcic_query->FieldValues["KR_CODE"];
	  command->Parameters->ParamValues["limit"] = ejcic_query->FieldValues["LIMIT"];
	  command->Parameters->ParamValues["payment"] = ejcic_query->FieldValues["PAYMENT"];
	  command->Parameters->ParamValues["cash"] = ejcic_query->FieldValues["CASH"];
	  command->Parameters->ParamValues["pay_code"] = ejcic_query->FieldValues["PAY_CODE"];
	  command->Parameters->ParamValues["input_time"] = input_time;
    command->Execute();
    ejcic_query->Next();
  };


  sql_stmt = "INSERT INTO KRM021(MSN, IDN, INQUIRY_DATE, CARD_BRAND, CARD_TYPE, ISSUE, ";
  sql_stmt += "ISSUE_NAME, START_DATE, STOP_DATE, STOP_CODE, AB_CODE, ";
  sql_stmt += "M_S, RELA, LIMIT, RISK, CLEAR_DATE, IDN_PRI, CNAME, REMARK, INPUT_TIME) VALUES(:msn, :idn, ";
  sql_stmt += ":inquiry_date, :card_brand, :card_type, :issue, ";
  sql_stmt += ":issue_name, :start_date, :stop_date, :stop_code, :ab_code, :m_s, :rela, :limit, ";
  sql_stmt += ":risk, :clear_date, :pidn, :cname, :remark, :input_time)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_KRM021 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
    command->Parameters->ParamValues["idn"] = idn;
    command->Parameters->ParamValues["msn"] = msn;
    command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
    command->Parameters->ParamValues["card_brand"] = ejcic_query->FieldValues["CARD_BRAND"];
    command->Parameters->ParamValues["card_type"] = ejcic_query->FieldValues["CARD_TYPE"];
    command->Parameters->ParamValues["issue"] = ejcic_query->FieldValues["ISSUE"];
    command->Parameters->ParamValues["issue_name"] = ejcic_query->FieldValues["ISSUE_NAME"];
    command->Parameters->ParamValues["start_date"] = ejcic_query->FieldValues["START_DATE"];
    command->Parameters->ParamValues["stop_date"] = ejcic_query->FieldValues["STOP_DATE"];
    command->Parameters->ParamValues["stop_code"] = ejcic_query->FieldValues["STOP_CODE"];
    command->Parameters->ParamValues["ab_code"] = ejcic_query->FieldValues["AB_CODE"];
    command->Parameters->ParamValues["m_s"] = ejcic_query->FieldValues["M_S"];
    command->Parameters->ParamValues["rela"] = ejcic_query->FieldValues["RELA"];
    command->Parameters->ParamValues["limit"] = ejcic_query->FieldValues["LIMIT"];
    command->Parameters->ParamValues["risk"] = ejcic_query->FieldValues["RISK"];
    command->Parameters->ParamValues["clear_date"] = ejcic_query->FieldValues["CLEAR_DATE"];
    command->Parameters->ParamValues["pidn"] = ejcic_query->FieldValues["IDN"];
    command->Parameters->ParamValues["cname"] = ejcic_query->FieldValues["CNAME"];
    command->Parameters->ParamValues["remark"] = ejcic_query->FieldValues["REMARK"];
    command->Parameters->ParamValues["input_time"] = input_time;
    command->Execute();
    ejcic_query->Next();
  };

  sql_stmt = "INSERT INTO KRM037 (";
  sql_stmt += "MSN, IDN, INQUIRY_DATE, BILL_DATE, ISSUE, ISSUE_NAME, CARD_TYPE, ";
  sql_stmt += "PERM_LIMIT, TEMP_LIMIT, CASH_LIMIT, PAYABLE, CASH_LENT, ";
  sql_stmt += "LAST_PAYA, REVOL_BAL, PAY_STAT, PAY_CODE, REVOL_RATE, ";
  sql_stmt += "PRE_OWED, DEBT_CODE, CLOSE_CODE, CLEAR_DATE, INPUT_TIME) VALUES(";
  sql_stmt += ":MSN, :IDN, :INQUIRY_DATE, :BILL_DATE, :ISSUE, :ISSUE_NAME, :CARD_TYPE, ";
  sql_stmt += ":PERM_LIMIT, :TEMP_LIMIT, :CASH_LIMIT, :PAYABLE, :CASH_LENT, ";
  sql_stmt += ":LAST_PAYA, :REVOL_BAL, :PAY_STAT, :PAY_CODE, :REVOL_RATE, ";
  sql_stmt += ":PRE_OWED, :DEBT_CODE, :CLOSE_CODE, :CLEAR_DATE, :INPUT_TIME)";
  sql_stmt = sql_stmt.UpperCase();
	command->CommandText = sql_stmt;
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_KRM037 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["MSN"] = msn;
  	command->Parameters->ParamValues["IDN"] = idn;
    command->Parameters->ParamValues["INQUIRY_DATE"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["BILL_DATE"] = ejcic_query->FieldValues["BILL_DATE"];
	  command->Parameters->ParamValues["ISSUE"] = ejcic_query->FieldValues["ISSUE"];
	  command->Parameters->ParamValues["ISSUE_NAME"] = ejcic_query->FieldValues["ISSUE_NAME"];
	  command->Parameters->ParamValues["CARD_TYPE"] = ejcic_query->FieldValues["CARD_TYPE"];
 	  command->Parameters->ParamValues["PERM_LIMIT"] = ejcic_query->FieldValues["PERM_LIMIT"];
	  command->Parameters->ParamValues["TEMP_LIMIT"] = ejcic_query->FieldValues["TEMP_LIMIT"];
	  command->Parameters->ParamValues["CASH_LIMIT"] = ejcic_query->FieldValues["CASH_LIMIT"];
	  command->Parameters->ParamValues["PAYABLE"] = ejcic_query->FieldValues["PAYABLE"];
	  command->Parameters->ParamValues["CASH_LENT"] = ejcic_query->FieldValues["CASH_LENT"];
 	  command->Parameters->ParamValues["LAST_PAYA"] = ejcic_query->FieldValues["LAST_PAYA"];
	  command->Parameters->ParamValues["REVOL_BAL"] = ejcic_query->FieldValues["REVOL_BAL"];
	  command->Parameters->ParamValues["PAY_STAT"] = ejcic_query->FieldValues["PAY_STAT"];
 	  command->Parameters->ParamValues["PAY_CODE"] = ejcic_query->FieldValues["PAY_CODE"];
	  command->Parameters->ParamValues["REVOL_RATE"] = ejcic_query->FieldValues["REVOL_RATE"];
	  command->Parameters->ParamValues["PRE_OWED"] = ejcic_query->FieldValues["PRE_OWED"];
 	  command->Parameters->ParamValues["DEBT_CODE"] = ejcic_query->FieldValues["DEBT_CODE"];
	  command->Parameters->ParamValues["CLOSE_CODE"] = ejcic_query->FieldValues["CLOSE_CODE"];
	  command->Parameters->ParamValues["CLEAR_DATE"] = ejcic_query->FieldValues["CLEAR_DATE"];
	  command->Parameters->ParamValues["INPUT_TIME"] = input_time;
    command->Execute();
    ejcic_query->Next();
  };

  sql_stmt = "INSERT INTO BAM086(MSN, IDN, INQUIRY_DATE, DATA_YYY, DATA_MM, BANK_CODE, ";
  sql_stmt += "BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, ";
  sql_stmt += "LOAN_AMT, PASS_DUE_AMT, PAY_CODE_12, CO_LOAN, INPUT_TIME) VALUES (:msn, :idn, ";
  sql_stmt += ":inquiry_date, :data_yyy, :data_mm, :bank_code, :bank_name, ";
  sql_stmt += ":account_code, :account_code2, :purpose_code, :contract_amt, ";
  sql_stmt += ":loan_amt, :pass_due_amt, :pay_code_12, :co_loan, :input_time);";
	command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_BAM086 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["idn"] = idn;
  	command->Parameters->ParamValues["msn"] = msn;
	  command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["data_yyy"] = ejcic_query->FieldValues["DATA_YYY"];
	  command->Parameters->ParamValues["data_mm"] = ejcic_query->FieldValues["DATA_MM"];
 	  command->Parameters->ParamValues["bank_code"] = ejcic_query->FieldValues["BANK_CODE"];
	  command->Parameters->ParamValues["bank_name"] = ejcic_query->FieldValues["BANK_NAME"];
	  command->Parameters->ParamValues["account_code"] = ejcic_query->FieldValues["ACCOUNT_CODE"];
	  command->Parameters->ParamValues["account_code2"] = ejcic_query->FieldValues["ACCOUNT_CODE2"];
	  command->Parameters->ParamValues["purpose_code"] = ejcic_query->FieldValues["PURPOSE_CODE"];
	  command->Parameters->ParamValues["contract_amt"] = ejcic_query->FieldValues["CONTRACT_AMT"];
	  command->Parameters->ParamValues["loan_amt"] = ejcic_query->FieldValues["LOAN_AMT"];
	  command->Parameters->ParamValues["pass_due_amt"] = ejcic_query->FieldValues["PASS_DUE_AMT"];
	  command->Parameters->ParamValues["pay_code_12"] = ejcic_query->FieldValues["PAY_CODE_12"];
	  command->Parameters->ParamValues["co_loan"] = ejcic_query->FieldValues["CO_LOAN"];
	  command->Parameters->ParamValues["input_time"] = input_time;
    command->Execute();
    ejcic_query->Next();
  };


  sql_stmt = "INSERT INTO JAS002(MSN, IDN, INQUIRY_DATE, EVER_DELINQUENT, DELINQUENT_DATE, ";
  sql_stmt += "EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, ";
  sql_stmt += "EVER_STOP_CARD, STOP_CARD_DATE, INPUT_TIME) VALUES (:msn, :idn, :inquiry_date, ";
  sql_stmt += ":ever_delinquent, :delinquent_date, :ever_bad_check, ";
  sql_stmt += ":bad_check_date, :ever_reject, :reject_date, :ever_stop_card, ";
  sql_stmt += ":stop_card_date, :input_time);";
	command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_JAS002 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["idn"] = idn;
  	command->Parameters->ParamValues["msn"] = msn;
	  command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["ever_delinquent"] = ejcic_query->FieldValues["EVER_DELINQUENT"];
	  command->Parameters->ParamValues["delinquent_date"] = ejcic_query->FieldValues["DELINQUENT_DATE"];
 	  command->Parameters->ParamValues["ever_bad_check"] = ejcic_query->FieldValues["EVER_BAD_CHECK"];
	  command->Parameters->ParamValues["bad_check_date"] = ejcic_query->FieldValues["BAD_CHECK_DATE"];
	  command->Parameters->ParamValues["ever_reject"] = ejcic_query->FieldValues["EVER_REJECT"];
	  command->Parameters->ParamValues["reject_date"] = ejcic_query->FieldValues["REJECT_DATE"];
	  command->Parameters->ParamValues["ever_stop_card"] = ejcic_query->FieldValues["EVER_STOP_CARD"];
	  command->Parameters->ParamValues["stop_card_date"] = ejcic_query->FieldValues["STOP_CARD_DATE"];
	  command->Parameters->ParamValues["input_time"] = input_time;
    command->Execute();
    ejcic_query->Next();
  };


/*  sql_stmt = "INSERT INTO STM007(IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, BANK_NAME, ";
  sql_stmt += "ITEM_LIST, INQ_PURPOSE_1, INQ_PURPOSE, INPUT_TIME) VALUES (:idn, :INQUIRY_DATE, :query_date, ";
  sql_stmt += ":bank_code, :bank_name, :item_list, :inq_purpose_1, :inq_purose, :input_time);";
	command->CommandText = sql_stmt;*/

  sql_stmt = "INSERT INTO STM007(MSN, IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, BANK_NAME, ";
  sql_stmt += "ITEM_LIST, INPUT_TIME, INQ_PURPOSE_1, INQ_PURPOSE) VALUES (:msn, :idn, :INQUIRY_DATE, :query_date, ";
  sql_stmt += ":bank_code, :bank_name, :item_list, :input_time, :inq_purpose_1, :inq_purpose);";
	command->CommandText = sql_stmt;


  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_STM007 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["idn"] = idn;
  	command->Parameters->ParamValues["msn"] = msn;
	  command->Parameters->ParamValues["query_date"] = ejcic_query->FieldValues["QUERY_DATE"];
    command->Parameters->ParamValues["INQUIRY_DATE"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["bank_code"] = ejcic_query->FieldValues["BANK_CODE"];
	  command->Parameters->ParamValues["bank_name"] = ejcic_query->FieldValues["BANK_NAME"];
 	  command->Parameters->ParamValues["item_list"] = ejcic_query->FieldValues["ITEM_LIST"];
	  command->Parameters->ParamValues["inq_purpose_1"] = ejcic_query->FieldValues["INQ_PURPOSE_1"];
	  command->Parameters->ParamValues["inq_purpose"] = ejcic_query->FieldValues["INQ_PURPOSE"];
	  command->Parameters->ParamValues["input_time"] = input_time;
    command->Execute();
    ejcic_query->Next();
  };

  sql_stmt = "INSERT INTO VAM102 (";
  sql_stmt += "MSN, IDN, INQUIRY_DATE, DATA_DATE, MAINCODE, MAINNOTE, SUBCODE, SUBNOTE, NOTE, INPUT_TIME) ";
  sql_stmt += "VALUES(:MSN, :IDN, :INQUIRY_DATE, :DATA_DATE, :MAINCODE, :MAINNOTE, :SUBCODE, ";
  sql_stmt += ":SUBNOTE, :NOTE, :INPUT_TIME)";
  sql_stmt = sql_stmt.UpperCase();
	command->CommandText = sql_stmt;
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_VAM102 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["IDN"] = idn;
  	command->Parameters->ParamValues["MSN"] = msn;
    command->Parameters->ParamValues["INQUIRY_DATE"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["DATA_DATE"] = ejcic_query->FieldValues["DATA_DATE"];
	  command->Parameters->ParamValues["MAINCODE"] = ejcic_query->FieldValues["MAINCODE"];
	  command->Parameters->ParamValues["MAINNOTE"] = ejcic_query->FieldValues["MAINNOTE"];
 	  command->Parameters->ParamValues["SUBCODE"] = ejcic_query->FieldValues["SUBCODE"];
	  command->Parameters->ParamValues["SUBNOTE"] = ejcic_query->FieldValues["SUBNOTE"];
	  command->Parameters->ParamValues["NOTE"] = ejcic_query->FieldValues["NOTE"];
	  command->Parameters->ParamValues["INPUT_TIME"] = input_time;
    command->Execute();
    ejcic_query->Next();
  };

  return INQUIRY_DATE;
};

//---------------------------------------------------------------------------
void clear(char *array, int size)
{
  for(int i=0; i<size; ++i)
    array[i] = ' ';
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void prepare_report(TADOCommand *command, const AnsiString &report_gen_time, bool regen)
{
  AnsiString sql_stmt;
  try{
    sql_stmt = "DROP TABLE Inbound_Report";
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

/*  try{
    sql_stmt = "DROP TABLE Outbound_Report";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };*/

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

/*  try{
    sql_stmt = "DROP TABLE Last_Application";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };*/

  sql_stmt = "CREATE TABLE Inbound_Report ";
  sql_stmt += "(MSN Char(14), IDN char(11), Input_Time Char(12))";/*, ";
  sql_stmt += "CName Char(20), Application_amount int, Application_terms int, ";
  sql_stmt += "Address Char(60), Product_type varchar(3))";*/
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
/*
  sql_stmt = "CREATE TABLE Outbound_Report ";
  sql_stmt += "(MSN Char(14), IDN char(11), Input_Time Char(12), Gender Varchar(2), ";
  sql_stmt += "Phone Char(10), Cell_Phone Char(10), Product_type varchar(3), ";
  sql_stmt += "CName Char(20), Application_amount int, Application_terms int) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
*/
  sql_stmt = "CREATE TABLE Report ";
  sql_stmt +="(MSN Char(14), IDN char(11), Input_Time Char(12), ";
  sql_stmt += "CName Char(20),  Gender Char(2), Product_type varchar(5), ";
  sql_stmt += "Application_amount int, Approved_amount int, Application_terms int, ";
  sql_stmt += "PB Decimal(6,5), NPV INT, SUGGESTION_CODE INT, ";
  sql_stmt += "PHONE CHAR(10), CELL_PHONE CHAR(10), ";
  sql_stmt += "SUGGESTION CHAR(60), Approved_amount_fsc int, Unsecured_balance int, Special_note char(2048))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*  sql_stmt = "CREATE TABLE Last_Application ";
  sql_stmt += "(MSN CHAR(14), IDN CHAR(11), INPUT_TIME CHAR(12), ";
  sql_stmt += "CNAME VARCHAR(20), PHONE CHAR(10), CELL_PHONE CHAR(10), ";
  sql_stmt += "APPLICATION_AMOUNT INT, PRODUCT_CODE CHAR(3), ";
  sql_stmt += "APPLICATION_RATE DECIMAL(6,5), APPLICATION_TERMS INT, ";
  sql_stmt += "APPLICATION_FEE INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO Last_Application(MSN, IDN, Input_Time) ";
  sql_stmt += "SELECT MSN, IDN, MAX(Input_Time) FROM APPLICANT ";
  sql_stmt += "WHERE IDN IN (SELECT IDN FROM RESULT WHERE ";
  if(!regen)
    sql_stmt += "RESULT_FILE_GEN_TIME IS NULL ";
  else
    sql_stmt += "LEFT(Result_File_GEN_Time,10) = :report_gen_time ";
  sql_stmt += "GROUP BY MSN, IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(regen)
    command->Parameters->ParamValues["report_gen_time"] = report_gen_time;
  command->Execute(); */

  sql_stmt = "INSERT INTO InBound_Report(MSN, IDN, Input_Time) ";
  sql_stmt += "SELECT MSN, IDN, MIN(Input_Time) FROM APPLICANT ";
  sql_stmt += "WHERE MSN IN (SELECT MSN FROM RESULT WHERE ";
  if(!regen)
    sql_stmt += "RESULT_FILE_GEN_TIME IS NULL) ";
  else
    sql_stmt += "LEFT(Result_File_GEN_Time,10) = :report_gen_time) ";
  sql_stmt += "GROUP BY MSN, IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(regen)
    command->Parameters->ParamValues["report_gen_time"] = report_gen_time;
  command->Execute();

/*  sql_stmt = "UPDATE Last_Application SET ";
  sql_stmt += "CNAME = A.CNAME, PHONE =A.PHONE, CELL_PHONE =A.CELL_PHONE, ";
  sql_stmt += "APPLICATION_AMOUNT =A.APPLICATION_AMOUNT, ";
  sql_stmt += "PRODUCT_CODE =A.PRODUCT_CODE, ";
  sql_stmt += "APPLICATION_TERMS =A.APPLICATION_TERMS ";
  sql_stmt += "FROM APPLICANT AS A INNER JOIN Last_Application AS B ";
  sql_stmt += "ON A.MSN = B.MSN AND A.IDN = B.IDN AND A.Input_Time = B.Input_Time";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute(); */

/*  sql_stmt = "INSERT INTO Inbound_Report(MSN, IDN, Input_Time, CName, ";
  sql_stmt += "Application_amount, Application_terms, Address, Product_type) ";
  sql_stmt += "SELECT A.MSN, A.IDN, B.Input_Time, (CASE WHEN B.CName IS NULL THEN A.CName ELSE B.CName END), ";
  sql_stmt += "B.Application_amount, B.Application_terms, A.Address, LEFT(A.MSN,3) ";
  sql_stmt += "FROM MAIL_LIST AS A LEFT JOIN First_Application AS B ON A.MSN = B.MSN AND ";
  sql_stmt += "A.IDN = B.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();*/
/*
  sql_stmt = "INSERT INTO Inbound_Report(MSN, IDN, Input_Time) ";
  sql_stmt += "SELECT A.MSN, A.IDN, A.Input_Time ";
  sql_stmt += "FROM First_Application_Time AS A INNER JOIN RESULT AS B ON ";
  sql_stmt += "A.MSN = B.MSN AND A.Input_Time = B.Input_Time ";
  sql_stmt += "WHERE B.Suggestion_Code IN (3,4) ";
  if(!regen)
    sql_stmt += "AND B.Result_File_GEN_Time IS NULL ";
  else
    sql_stmt += "AND LEFT(B.Result_File_GEN_Time,10) = :report_gen_time ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(regen)
    command->Parameters->ParamValues["report_gen_time"] = report_gen_time;
  command->Execute();  */
/*
  sql_stmt = "INSERT INTO Outbound_Report(MSN, IDN, Input_Time, Gender, Phone, ";
  sql_stmt += "Cell_Phone, Product_Type, CName, Application_amount, ";
  sql_stmt += "Application_terms) SELECT A.MSN, A.IDN, A.Input_Time, ";
  sql_stmt += "(CASE WHEN SUBSTRING(A.IDN,2,1) = '1' THEN '男' ELSE '女' END), ";
  sql_stmt += "Phone, Cell_Phone, Product_code, CName, Application_amount, Application_terms ";
  sql_stmt += "FROM First_Application AS A INNER JOIN RESULT AS B ON ";
  sql_stmt += "A.MSN = B.MSN AND A.Input_Time = B.Input_Time ";
  sql_stmt += "WHERE B.Suggestion_Code IN (3,4) ";
  if(!regen)
    sql_stmt += "AND B.Result_File_GEN_Time IS NULL ";
  else
    sql_stmt += "AND LEFT(B.Result_File_GEN_Time,10) = :report_gen_time ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(regen)
    command->Parameters->ParamValues["report_gen_time"] = report_gen_time;
  command->Execute();  */

  sql_stmt = "INSERT INTO Report(MSN, IDN, Input_Time, CName, Gender, Suggestion_code, Suggestion, ";
  sql_stmt += "Product_Type, Application_amount, Approved_amount, Application_terms, PB, NPV, ";
  sql_stmt += "Approved_amount_fsc, Unsecured_Balance, SPECIAL_NOTE, PHONE, CELL_PHONE) ";
  sql_stmt += "SELECT A.MSN, A.IDN, A.Input_Time, A.CName, ";
  sql_stmt += "(CASE WHEN SUBSTRING(A.IDN,2,1) = '1' THEN '男' ELSE '女' END), ";
  sql_stmt += "B.Suggestion_Code, B.Suggestion, A.Product_Code, ";
  sql_stmt += "A.Application_amount, B.Approved_amount, A.Application_terms, B.PB, B.NPV, ";
  sql_stmt += "B.Approved_amount_fsc, B.Unsecured_balance, B.special_note, A.PHONE, A.CELL_PHONE ";
  sql_stmt += "FROM Result AS B INNER JOIN Applicant AS A ";
  sql_stmt += "ON A.Input_Time = B.Input_Time AND A.MSN = B.MSN ";
  sql_stmt = sql_stmt.UpperCase();
  if(!regen)
    sql_stmt += "WHERE B.Result_File_GEN_Time IS NULL ";
  else
    sql_stmt += "WHERE LEFT(B.Result_File_GEN_Time,10) = :report_gen_time ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(regen)
    command->Parameters->ParamValues["report_gen_time"] = report_gen_time;
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
    sql_stmt = "SELECT MIN(Input_Time) AS Min_Time, MAX(Input_Time) AS Max_Time, COUNT(*) AS CNT ";
    sql_stmt += "FROM InBound_Report";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Open();

    AnsiString min_time_inbound = query->FieldByName("Min_Time")->AsString;
    AnsiString max_time_inbound = query->FieldByName("Max_Time")->AsString;


    AnsiString year = report_gen_time.SubString(1,4);
    year = static_cast<AnsiString>(year.ToInt() - 1911);
    AnsiString min_year_inbound = min_time_inbound.SubString(1,4);
    if(min_year_inbound != "")
      min_year_inbound = static_cast<AnsiString>(min_year_inbound.ToInt() - 1911);
    AnsiString max_year_inbound = max_time_inbound.SubString(1,4);
    if(max_year_inbound != "")
      max_year_inbound = static_cast<AnsiString>(max_year_inbound.ToInt() - 1911);

    AnsiString file_name_inbound = report_dir + "inbound_" + report_gen_time + ".csv";
    ofstream inbound(file_name_inbound.c_str(), ios_base::out);
    if(inbound){
      query->Close();
      sql_stmt = "SELECT * FROM Inbound_Report ORDER BY IDN;";
      sql_stmt = sql_stmt.UpperCase();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Open();

      inbound << "台南企銀個人信貸行銷測試活動，網訊電通Inbound作業用名單" << endl;
      inbound << "名單產生時間：" << ",";
      inbound << year.c_str() << "年 ";
      inbound << report_gen_time.SubString(5,2).c_str() << "月 ";
      inbound << report_gen_time.SubString(7,2).c_str() << "日 ";
      inbound << report_gen_time.SubString(9,2).c_str() << "時 ";
      inbound << report_gen_time.SubString(11,2).c_str() << "分 " << endl;
      inbound << "申請件輸入日期時間範圍：" << ",";
      inbound << min_year_inbound.c_str() << "年 ";
      inbound << min_time_inbound.SubString(5,2).c_str() << "月 ";
      inbound << min_time_inbound.SubString(7,2).c_str() << "日 ";
      inbound << min_time_inbound.SubString(9,2).c_str() << "時 ";
      inbound << min_time_inbound.SubString(11,2).c_str() << "分 ";
      inbound << "," << " _ " << ",";
      inbound << max_year_inbound.c_str() << "年 ";
      inbound << max_time_inbound.SubString(5,2).c_str() << "月 ";
      inbound << max_time_inbound.SubString(7,2).c_str() << "日 ";
      inbound << max_time_inbound.SubString(9,2).c_str() << "時 ";
      inbound << max_time_inbound.SubString(11,2).c_str() << "分 " << endl;
      inbound << "名單所含件數：" << "," << query->RecordCount;
      inbound << endl;
      inbound << "MSN" << ",";
      inbound << "身分證字號" << ",";
//      inbound << "姓名" << "," ;
//      inbound << "地址" << ",";
      inbound << "進件處理時間" << endl;
//      inbound << "產品別" << ",";
//      inbound << "申貸年限" << ",";
//      inbound << "申貸金額(NT$)" << ",";
//      inbound << "本日Inbound次數(Telexpress填寫)" << ",";
//      inbound << "補寄時間(Telexpress填寫，填寫”年月日”即可)" << endl;
      while(!query->Eof){
        inbound << query->FieldByName("MSN")->AsString.c_str() << ",";
        inbound << query->FieldByName("IDN")->AsString.c_str() << ",";
//        inbound << query->FieldByName("CName")->AsString.c_str() << ",";
//      inbound << query->FieldByName("ADDRESS")->AsString.c_str() << ",";
        inbound << query->FieldByName("Input_Time")->AsString.c_str() << endl;
//        inbound << query->FieldByName("Product_Type")->AsString.c_str() << ",";
//        inbound << query->FieldByName("Application_Terms")->AsInteger << ",";
//        inbound << query->FieldByName("Application_Amount")->AsInteger << endl;
        query->Next();
      };
      inbound << "/EOF" << endl;
    }
    else
      return false;


/*    sql_stmt = "SELECT COUNT(*) AS CNT, MAX(INPUT_TIME) AS MAX_TIME, MIN(INPUT_TIME) AS MIN_TIME FROM Outbound_Report";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Open();
    if(query->FieldValues["CNT"] == 0){
      if(regen)
        message += "原檔案產生日期錯誤，報表產生中止。\n";
      else
        message += "今天沒有要致電的申請件。\n";
      frmMainUI->lblMessage->Caption = message;
      frmMainUI->Refresh();
    }
    AnsiString file_name_outbound = report_dir + "outbound_" + report_gen_time + ".csv";
    ofstream outbound(file_name_outbound.c_str(), ios_base::out);

    AnsiString min_time_outbound, max_time_outbound, min_year_outbound, max_year_outbound;

    if(query->FieldValues["CNT"] == 0){
      min_time_outbound = "";
      max_time_outbound = "";
      min_year_outbound = "";
      max_year_outbound = "";
    }
    else{
      min_time_outbound = query->FieldByName("Min_Time")->AsString;
      max_time_outbound = query->FieldByName("Max_Time")->AsString;
      min_year_outbound = min_time_outbound.SubString(1,4);
      min_year_outbound = static_cast<AnsiString>(min_year_outbound.ToInt() - 1911);
      max_year_outbound = max_time_outbound.SubString(1,4);
      max_year_outbound = static_cast<AnsiString>(max_year_outbound.ToInt() - 1911);
    };

    outbound << "台南企銀個人信貸行銷測試活動，Outbound作業用名單" << endl;
    outbound << "名單產生時間：" << ",";
    outbound << year.c_str() << "年 ";
    outbound << report_gen_time.SubString(5,2).c_str() << "月 ";
    outbound << report_gen_time.SubString(7,2).c_str() << "日 ";
    outbound << report_gen_time.SubString(9,2).c_str() << "時 ";
    outbound << report_gen_time.SubString(11,2).c_str() << "分 " << endl;
    outbound << "申請件輸入日期時間範圍：" << ",";
    outbound << min_year_outbound.c_str() << "年 ";
    outbound << min_time_outbound.SubString(5,2).c_str() << "月 ";
    outbound << min_time_outbound.SubString(7,2).c_str() << "日 ";
    outbound << min_time_outbound.SubString(9,2).c_str() << "時 ";
    outbound << min_time_outbound.SubString(11,2).c_str() << "分 ";
    outbound << "," << " _ " << ",";
    outbound << max_year_outbound.c_str() << "年 ";
    outbound << max_time_outbound.SubString(5,2).c_str() << "月 ";
    outbound << max_time_outbound.SubString(7,2).c_str() << "日 ";
    outbound << max_time_outbound.SubString(9,2).c_str() << "時 ";
    outbound << max_time_outbound.SubString(11,2).c_str() << "分 " << endl;
    outbound << "名單所含件數：" << "," << query->FieldByName("CNT")->AsInteger;
    outbound << endl;
    outbound << "姓名" << ",";
    outbound << "性別" << ",";
    outbound << "聯絡電話1" << "," ;
    outbound << "聯絡電話2" << ",";
    outbound << "進件時間" << ",";
    outbound << "產品別" << ",";
    outbound << "身分證字號(如果留白請收集)" << ",";
    outbound << "申貸年限(如果留白請收集)" << ",";
    outbound << "申貸金額(如果留白請收集)" << ",";
    outbound << "公司名稱(請收集)" << ",";
    outbound << "職稱(請收集)" << ",";
    outbound << "年資(請收集)" << endl;

    query->Close();
    sql_stmt = "SELECT * FROM Outbound_Report ORDER BY Input_Time;";
    sql_stmt = sql_stmt.UpperCase();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Open();
    while(!query->Eof){
      outbound << query->FieldByName("CName")->AsString.c_str() << ",";
      outbound << query->FieldByName("Gender")->AsString.c_str() << ",";
      outbound << query->FieldByName("Phone")->AsString.c_str() << ",";
      outbound << query->FieldByName("Cell_Phone")->AsString.c_str() << ",";
      outbound << query->FieldByName("Input_Time")->AsString.c_str() << ",";
      outbound << query->FieldByName("Product_Type")->AsString.c_str() << ",";
      outbound << query->FieldByName("IDN")->AsString.c_str() << ",";
      outbound << (query->FieldByName("Application_Terms")->AsInteger / 12) << ",";
      outbound << query->FieldByName("Application_Amount")->AsInteger << endl;
      query->Next();
    };
    outbound << "/EOF" << endl;
    outbound.clear();
    outbound.close();*/

    AnsiString file_name_total = report_dir + "total_" + report_gen_time + ".csv";
    AnsiString file_name_individual = report_dir + "individual_" + report_gen_time + ".csv";
    AnsiString file_name_individual_formating = report_dir + "individual.csv";
    AnsiString file_name_manual = report_dir + "manual_" + report_gen_time + ".csv";
    AnsiString file_name_manual_formating = report_dir + "manual.csv";

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
      frmMainUI->lblMessage->Caption = message;
      frmMainUI->Refresh();
      return true;
    };
    int records = query->FieldValues["CNT"];

    ofstream total(file_name_total.c_str(), ios_base::out);
    ofstream individual(file_name_individual.c_str(), ios_base::out);
    ofstream report_individual(file_name_individual_formating.c_str(), ios_base::out);
    ofstream manual(file_name_manual.c_str(), ios_base::out);
    ofstream report_manual(file_name_manual_formating.c_str(), ios_base::out);

    AnsiString min_time = query->FieldByName("Min_Time")->AsString;
    AnsiString max_time = query->FieldByName("Max_Time")->AsString;
    AnsiString min_year = min_time.SubString(1,4);
    min_year = static_cast<AnsiString>(min_year.ToInt() - 1911);
    AnsiString max_year = max_time.SubString(1,4);
    max_year = static_cast<AnsiString>(max_year.ToInt() - 1911);


    if(total && individual && report_individual){
      total << "台南企銀個人信貸評分結果名單 (行銷測試活動版)" << endl;
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
      total << "申請人郵寄編號" << ",";
      total << "申請人姓名" << ",";
      total << "產品別" << "," ;
      total << "申請件輸入時間" << ",";
      total << "申貸金額(NT$)" << ",";
      total << "核准金額(NT$)" << ",";
      total << "收入22倍限制下核准金額(NT$)" << ",";
      total << "無擔保貸款餘額(NT$)" << ",";
      total << "申貸期數" << ",";
      total << "模組建議" << ",";
      total << "壞帳機率" << ",";
      total << "經濟價值" << ",";
      total << "聯徵特殊註記" << endl;

      report_individual << "Input_time" <<",";
      report_individual << "Report_time" <<",";
      report_individual << "id" <<",";
      report_individual << "name" <<",";
      report_individual << "product_type" <<",";
      report_individual << "amount" <<",";
      report_individual << "approved_amount" <<",";
      report_individual << "approved_amount_fsc" <<",";
      report_individual << "unsecured_balance" <<",";
      report_individual << "terms" <<",";
      report_individual << "suggestion" <<",";
      report_individual << "pb" <<",";
      report_individual << "npv" <<",";
      report_individual << "special_note" << endl;

      individual << "Input_time" <<",";
      individual << "Report_time" <<",";
      individual << "id" <<",";
      individual << "name" <<",";
      individual << "product_type" <<",";
      individual << "amount" <<",";
      individual << "approved_amount" <<",";
      individual << "approved_amount_fsc" <<",";
      individual << "unsecured_balance" <<",";
      individual << "terms" <<",";
      individual << "suggestion" <<",";
      individual << "pb" <<",";
      individual << "npv" <<",";
      individual << "special_note" << endl;

      report_manual << "Input_time" << ",";
      report_manual << "Report_time" << ",";
      report_manual << "id" << ",";
      report_manual << "name" << ",";
      report_manual << "gender" << ",";
      report_manual << "fixed_phone" << ",";
      report_manual << "cell_phone" << ",";
      report_manual << "special_note" << endl;

      manual << "Input_time" << ",";
      manual << "Report_time" << ",";
      manual << "id" << ",";
      manual << "name" << ",";
      manual << "gender" << ",";
      manual << "fixed_phone" << ",";
      manual << "cell_phone" << ",";
      manual << "special_note" << endl;

      query->Close();
      sql_stmt = "SELECT * FROM Report ORDER BY Input_Time;";
      sql_stmt = sql_stmt.UpperCase();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Open();
      while(!query->Eof){
        total << query->FieldByName("MSN")->AsString.c_str() << ",";
        total << query->FieldByName("CName")->AsString.c_str() << ",";
        total << query->FieldByName("Product_Type")->AsString.c_str() << ",";
        total << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        total << query->FieldByName("Application_Amount")->AsInteger << ",";
        total << query->FieldByName("Approved_Amount")->AsInteger << ",";
        total << query->FieldByName("Approved_Amount_FSC")->AsInteger << ",";
        total << query->FieldByName("Unsecured_balance")->AsInteger << ",";
        total << query->FieldByName("Application_Terms")->AsInteger << ",";
        total << query->FieldByName("SUGGESTION")->AsString.c_str() << ",";
        total << query->FieldByName("PB")->AsFloat << ",";
        total << query->FieldByName("NPV")->AsFloat << ",";
        total << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;

        if(query->FieldByName("Suggestion_Code")->AsInteger == 3){
          report_manual << query->FieldByName("Input_time")->AsString.c_str() << ",";
          report_manual << report_gen_time.c_str() << ",";
          report_manual << query->FieldByName("IDN")->AsString.c_str() << ",";
          report_manual << query->FieldByName("CNAME")->AsString.c_str() << ",";
          report_manual << query->FieldByName("gender")->AsString.c_str() << ",";
          report_manual << query->FieldByName("phone")->AsString.c_str() << ",";
          report_manual << query->FieldByName("cell_phone")->AsString.c_str() << ",";
          report_manual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;

          manual << query->FieldByName("Input_time")->AsString.c_str() << ",";
          manual << report_gen_time.c_str() << ",";
          manual << query->FieldByName("IDN")->AsString.c_str() << ",";
          manual << query->FieldByName("CNAME")->AsString.c_str() << ",";
          manual << query->FieldByName("gender")->AsString.c_str() << ",";
          manual << query->FieldByName("phone")->AsString.c_str() << ",";
          manual << query->FieldByName("cell_phone")->AsString.c_str() << ",";
          manual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;
        };

        report_individual << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        report_individual << report_gen_time.c_str() << ",";
        report_individual << query->FieldByName("IDN")->AsString.c_str() << ",";
        report_individual << query->FieldByName("CName")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Product_Type")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Application_Amount")->AsInteger << ",";
        report_individual << query->FieldByName("Approved_Amount")->AsInteger << ",";
        report_individual << query->FieldByName("Approved_Amount_FSC")->AsInteger << ",";
        report_individual << query->FieldByName("Unsecured_balance")->AsInteger << ",";
        report_individual << query->FieldByName("Application_Terms")->AsInteger << ",";
        report_individual << query->FieldByName("Suggestion")->AsString.c_str() << ",";
        report_individual << query->FieldByName("PB")->AsFloat << ",";
        report_individual << query->FieldByName("NPV")->AsFloat << ",";
        report_individual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;

        individual << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        individual << report_gen_time.c_str() << ",";
        individual << query->FieldByName("IDN")->AsString.c_str() << ",";
        individual << query->FieldByName("CName")->AsString.c_str() << ",";
        individual << query->FieldByName("Product_Type")->AsString.c_str() << ",";
        individual << query->FieldByName("Application_Amount")->AsInteger << ",";
        individual << query->FieldByName("Approved_Amount")->AsInteger << ",";
        individual << query->FieldByName("Approved_Amount_FSC")->AsInteger << ",";
        individual << query->FieldByName("Unsecured_balance")->AsInteger << ",";
        individual << query->FieldByName("Application_Terms")->AsInteger << ",";
        individual << query->FieldByName("Suggestion")->AsString.c_str() << ",";
        individual << query->FieldByName("PB")->AsFloat << ",";
        individual << query->FieldByName("NPV")->AsFloat << ",";
        individual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;

        query->Next();
      };
      total << "/EOF" << endl;
      inbound.clear();
      inbound.close();
      total.clear();
      total.close();
      report_individual.clear();
      report_individual.close();
      individual.clear();
      individual.close();
      report_manual.clear();
      report_manual.close();
      manual.clear();
      manual.close();
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
    frmMainUI->lblMessage->Caption = E.Message;
    frmMainUI->Refresh();
    return false;
  };
};
//---------------------------------------------------------------------------

bool request_for_jcic_data(TADOQuery *query, const AnsiString &id, AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no)
{
  bool return_value = true;

  WideString jcic_inquiry_package_code = "001";
  WideString branch_code = "5400";//"8160":資訊室; For testing machine:GSS
  WideString user_id = "DAC_1";//"01922":李政達; For testing machine:AUSTIN;
  WideString jcic_inquiry_criteria = id.Trim() + "^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^";
  WideString inquiry_style = "D"; //for data; "H" for html
  WideString AS400 = "N";
  WideString inquiry_reason = "A4A";
  WideString check_idn = "Y";
  WideString program_id = "GSSJCIC30";

  Data->ejcic->set_AP_ID(program_id);
  Data->ejcic->set_ConStrName(program_id);

  Data->ejcic->PutRequest(jcic_inquiry_package_code,
                          jcic_inquiry_criteria,
                          branch_code,
                          user_id,
                          inquiry_style,
                          AS400,
                          inquiry_reason,
                          check_idn);


  error_no = Data->ejcic->get_Error_Number();

  if(error_no == 0){
    query_sn = static_cast<AnsiString>(Data->ejcic->QSEQNO);

    AnsiString ejcic_inquiry_status = "";
    AnsiString ejcic_inquiry_result = "";
    AnsiString ejcic_inquiry_return_code = "";

    ejcic_inquiry_status = get_ejcic_inquiry_result(query, query_sn, ejcic_inquiry_result, ejcic_inquiry_return_code);

    if(ejcic_inquiry_status != "R"){
//      frmMainUI->timer->OnTimer;
//      frmMainUI->timer->Enabled = true;
      for(int i=1; i < 4; ++i){
        if(ejcic_inquiry_status == "R")
          break;
        else if(ejcic_inquiry_status == "P"){
          frmMainUI->lblMessage->Caption = "第" + static_cast<AnsiString>(i) + " 次嘗試。eJCIC尚未將查詢送給聯徵。";
          frmMainUI->Refresh();
        }
        else if(ejcic_inquiry_status == "S"){
          frmMainUI->lblMessage->Caption = "第" + static_cast<AnsiString>(i) + " 次嘗試。聯徵尚未回覆查詢。";
          frmMainUI->Refresh();
        }
        else{
          frmMainUI->lblMessage->Caption = "eJCIC 聯徵查詢狀態無法辨識。";
          frmMainUI->Refresh();
        }
        Sleep(5000);
        ejcic_inquiry_status = get_ejcic_inquiry_result(query, query_sn, ejcic_inquiry_result, ejcic_inquiry_return_code);
      };
    };
    if(ejcic_inquiry_status != "R"){
      jcic_inquiry_result = "聯徵仍未回傳查詢結果，本申請件處理中止請聯絡e-JCIC維護人員與聯徵聯絡人員。\n";
      return_value = false;
    }
    else{
      if(ejcic_inquiry_result == "S"){
        jcic_inquiry_result = "";
        return_value = true;
      }
      else if(ejcic_inquiry_result == "F"){
        jcic_inquiry_result = "聯徵查詢錯誤，e-JCIC訊息：" + ejcic_inquiry_return_code + " 請聯絡e-JCIC維護人員與聯徵聯絡人員。\n";
        return_value = false;
      }
    }
  }
  else{
    jcic_inquiry_result = "查詢錯誤，未送出至聯徵，e-JCIC訊息：" + Data->ejcic->get_JC_ReturnCode() + " 請聯絡e-JCIC維護人員。\n";
    return_value = false;
  };

  return return_value;
};

//---------------------------------------------------------------------------
AnsiString get_ejcic_inquiry_result(TADOQuery *query, const AnsiString &query_sn, AnsiString &result, AnsiString &result_code)
{
  AnsiString sql_stmt = "SELECT * FROM TNBB_JCIC.JC_T_REQ WHERE QSEQNO = :query_sn";
  AnsiString return_value = "";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["query_sn"] = query_sn;
  query->Open();
  if(!query->FieldValues["STATUS"].IsNull())
    return_value = query->FieldValues["STATUS"];
  if(!query->FieldValues["RESULT"].IsNull())
    result = query->FieldValues["RESULT"];
  if(!query->FieldValues["RT_CODE"].IsNull())
    result_code = query->FieldValues["RT_CODE"];
  return return_value;
}

//---------------------------------------------------------------------------

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

void record_scoring_time(TADOCommand *command, const AnsiString &idn,
                         const AnsiString &input_time, bool begin)
{
  AnsiString sql_stmt;
  if(begin){
    sql_stmt = "INSERT INTO SCORING_TIME_RECORDER(IDN, INPUT_TIME, BEGIN_TIME) ";
    sql_stmt += "VALUES (:idn, :input_time, GETDATE())";
    sql_stmt = sql_stmt.UpperCase();
  }
  else{
    sql_stmt = "UPDATE SCORING_TIME_RECORDER ";
    sql_stmt += "SET END_TIME = GETDATE() WHERE IDN = :idn AND INPUT_TIME = :input_time;";
    sql_stmt = sql_stmt.UpperCase();
  }
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn.Trim().UpperCase();
  command->Parameters->ParamValues["input_time"] = input_time;
  command->Execute();
}

bool check_msn(const AnsiString &msn, TADOQuery *query)
{
  bool return_value = true;

  AnsiString sql_stmt;
  sql_stmt = "SELECT COUNT(*) AS CNT FROM MAIL_LIST WHERE MSN = :msn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  if(query->FieldValues["CNT"] == 0)
    return_value = false;
  return return_value;
};


void __fastcall TfrmMainUI::btnRptGenClick(TObject *Sender)
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
  get_time(year, month, day, hour, min);

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
  frmMainUI->Refresh();
  clean_report(Data->command);

}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::medtIDExit(TObject *Sender)
{
  if(!check_msn(medtMSN->Text, Data->query)){
    message += "郵寄編號號錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!check_id(medtID->Text, medtMSN->Text, Data->query)){
    message += "申請人身分證號錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!is_input_error)
     edtName->Text = get_applicant_name(medtMSN->Text, Data->query);
  else{
    frmMainUI->btnClearClick(NULL);
    is_input_error = false;
  };  
}
//---------------------------------------------------------------------------

AnsiString get_applicant_name(const AnsiString &msn, TADOQuery *query)
{
  AnsiString return_value = "";
  AnsiString sql_stmt = "SELECT CName FROM MAIL_LIST WHERE MSN = :msn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  if(!query->FieldValues["CNAME"].IsNull())
    return_value = query->FieldValues["CNAME"];
  return return_value;  
};
/*void __fastcall TfrmMainUI::btnStoreDecisionKeyPress(TObject *Sender,
      char &Key)
{
  if(Key == 0x0A)
    btnStoreDecision->Click();
} */
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnNormalRptDirClick(TObject *Sender)
{
   Application->CreateForm(__classid(TfrmDirUI), &frmDirUI);
   frmDirUI->dlbReport->Directory = report_dir;
   frmDirUI->edtDir->Text = report_dir;
   frmDirUI->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnRegenRptDirClick(TObject *Sender)
{
   Application->CreateForm(__classid(TfrmReDirUI), &frmReDirUI);
   frmReDirUI->dlbReReport->Directory = report_regen_dir;
   frmReDirUI->edtReDir->Text = report_regen_dir;
   frmReDirUI->ShowModal();  
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnRptRegenClick(TObject *Sender)
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

  get_time(year, month, day, hour, min);

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
    frmMainUI->Refresh();
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
    frmMainUI->Refresh();
    clean_report(Data->command);
  };  
}
//---------------------------------------------------------------------------
/*
void __fastcall TfrmMainUI::rbApprovalClick(TObject *Sender)
{
  Label33->Enabled = true;
  Label34->Enabled = true;
  medtAmountApproved->Enabled = true;
  medtAmountApproved->Clear();
  Label35->Enabled = false;
  edtDeclineReason->Enabled = false;
}*/
//---------------------------------------------------------------------------
/*
void __fastcall TfrmMainUI::btnClearDecisionClick(TObject *Sender)
{
  medtResultMSN->Clear();
  medtEntryYear->Clear();
  medtEntryMonth->Clear();
  medtEntryDay->Clear();
  rbApproval->Checked = true;
  medtAmountApproved->Clear();
  medtTermsApproved->Clear();
  rbApproval->Checked = false;
  edtDeclineReason->Clear();
  rbApproval->Checked = true;
  medtResultMSN->SetFocus();
  btnStoreDecision->Enabled = false;
  lblMessage->Caption = "";
  message = "";
  frmMainUI->Refresh();
}*/
//---------------------------------------------------------------------------
/*
void __fastcall TfrmMainUI::btnGetSuggestionClick(TObject *Sender)
{
  bool is_input_error = false;
  lblMessage->Caption = "";
  message = "";
  frmMainUI->Refresh();
  unsigned int gender; //This variable is not used
  if(!check_msn(medtResultMSN->Text, Data->query)){
    message += "主卡申請人身分證號錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };

  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;

  get_time(year, month, day, hour, min);



  unsigned int bad_date = check_birthday(medtEntryYear->Text,
                                         medtEntryMonth->Text,
                                         medtEntryDay->Text,
                                         year, month, day, true);
  if ((bad_date > 0) && ((bad_date & 0x8) != 0x8))
    is_input_error = true;
  if((bad_date & 0x1) == 0x1)
    message += "資料輸入月份錯誤\n";
  if((bad_date & 0x2) == 0x2)
    message += "資料輸入日期錯誤\n";
  if((bad_date & 0x4) == 0x4)
    message += "資料輸入於未來\n";
  if((bad_date & 0x20) == 0x20)
    message += "資料輸入年份錯誤\n";
  lblMessage->Caption = message;
  frmMainUI->Refresh();
  if((bad_date & 0x20) == 0x20)
    medtEntryYear->SetFocus();
  else if((bad_date & 0x1) == 0x1)
    medtEntryMonth->SetFocus();
  else if((bad_date & 0x2) == 0x2)
    medtEntryDay->SetFocus();
  else if((bad_date & 0x4) == 0x4)
    medtEntryYear->SetFocus();


  if(!is_input_error){
    AnsiString input_year = static_cast<AnsiString>(medtEntryYear->Text.Trim().ToInt() + 1911);
    AnsiString input_month = medtEntryMonth->Text.Trim().Length() == 1 ? "0" + medtEntryMonth->Text.Trim() : medtEntryMonth->Text.Trim();
    AnsiString input_date =  medtEntryDay->Text.Trim().Length() == 1 ? "0" + medtEntryDay->Text.Trim() : medtEntryDay->Text.Trim();
    AnsiString input_time = input_year + input_month + input_date;

    AnsiString sql_stmt = "SELECT * FROM Result WHERE IDN = :idn AND ";
    sql_stmt += "LEFT(Input_Time, 8) = :input_time;";
    sql_stmt = sql_stmt.UpperCase();
    try{
      Data->query->Close();
      Data->query->SQL->Clear();
      Data->query->SQL->Add(sql_stmt);
      Data->query->Parameters->ParamValues["idn"] = medtID->Text;
      Data->query->Parameters->ParamValues["input_time"] = input_time;
      Data->query->Open();
      if(Data->query->RecordCount == 0){
        message += "無此申請件\n";
        lblMessage->Caption = message;
        frmMainUI->Refresh();
        medtID->Clear();
        medtEntryYear->Clear();
        medtEntryMonth->Clear();
        medtEntryDay->Clear();
        medtID->SetFocus();
      }
      else{
        unsigned int suggestion_code = Data->query->FieldValues["Suggestion_Code"];
//Suggestion Code: 1: Module approved "模組建議核准" ; 2: Module declined "模組建議婉拒"; 3: Module manual "模組建議人工審核";
//                 4: Directly approved "模組建議人工試算";
        switch (suggestion_code){
          case 1:
            rbApproval->Checked = true;
            medtAmountApproved->Enabled = true;
            edtDeclineReason->Enabled = false;
            medtAmountApproved->Text = static_cast<AnsiString>(Data->query->FieldValues["Suggested_Line"]);
            break;
          case 2:
            rbDecline->Checked = true;
            edtDeclineReason->Enabled = true;
            edtDeclineReason->Clear();
            break;
          case 3:
            rbDecline->Checked = false;
            rbApproval->Checked = false;
            medtAmountApproved->Enabled = true;
            edtDeclineReason->Enabled = true;
            medtAmountApproved->Clear();
            edtDeclineReason->Clear();
            medtAmountApproved->Enabled = false;
            edtDeclineReason->Enabled = false;
            break;
          case 4:
            rbDecline->Checked = false;
            rbApproval->Checked = false;
            medtAmountApproved->Enabled = true;
            edtDeclineReason->Enabled = true;
            medtAmountApproved->Clear();
            edtDeclineReason->Clear();
            medtAmountApproved->Enabled = false;
            edtDeclineReason->Enabled = false;
            break;
        };
        btnStoreDecision->Enabled = true;
      };
      if(rbApproval->Checked)
        medtAmountApproved->SetFocus();
      else if(rbDecline->Checked)
        edtDeclineReason->SetFocus();
    }
    catch(Exception &E){
     lblMessage->Caption = E.Message;
     frmMainUI->Refresh();
    };
  };
}*/
//---------------------------------------------------------------------------

/*
void __fastcall TfrmMainUI::btnStoreDecisionClick(TObject *Sender)
{
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  get_time(year, month, day, hour, min);

  AnsiString system_year;
  AnsiString system_month;
  AnsiString system_day;
  AnsiString system_hour;
  AnsiString system_min;
  system_year = static_cast<AnsiString>(year);
  system_month = static_cast<AnsiString>(month);
  system_day = static_cast<AnsiString>(day);
  system_hour = static_cast<AnsiString>(hour);
  system_min = static_cast<AnsiString>(min);
  system_month = system_month.Length() == 1 ? "0" + system_month : system_month;
  system_day = system_day.Length() == 1 ? "0" + system_day : system_day;
  system_hour = system_hour.Length() == 1 ? "0" + system_hour : system_hour;
  system_min = system_min.Length() == 1 ? "0" + system_min : system_min;
  AnsiString system_time = system_year + system_month + system_day + system_hour + system_min;

  AnsiString sql_stmt;
  AnsiString input_year = static_cast<AnsiString>(medtEntryYear->Text.Trim().ToInt() + 1911);
  AnsiString input_month = medtEntryMonth->Text.Trim().Length() == 1 ? "0" + medtEntryMonth->Text.Trim() : medtEntryMonth->Text.Trim();
  AnsiString input_date =  medtEntryDay->Text.Trim().Length() == 1 ? "0" + medtEntryDay->Text.Trim() : medtEntryDay->Text.Trim();
  AnsiString input_time = input_year + input_month + input_date;

  sql_stmt = "SELECT MAX(Input_Time) As Input_Time FROM Result WHERE MSN = :msn AND ";
  sql_stmt += "LEFT(Input_Time, 8) = :input_time;";
  sql_stmt = sql_stmt.UpperCase();
  try{
    Data->query->Close();
    Data->query->SQL->Clear();
    Data->query->SQL->Add(sql_stmt);
    Data->query->Parameters->ParamValues["msn"] = medtResultMSN->Text.Trim();
    Data->query->Parameters->ParamValues["input_time"] = input_time;
    Data->query->Open();
    input_time = Data->query->FieldValues["Input_Time"];
    Data->query->Close();

    sql_stmt = "INSERT INTO Approval_Decision(";
    sql_stmt += "MSN , Input_Time, Decision_Time, Decision,  Amount, Terms, ";
    sql_stmt += "Decline_Reason)";
    sql_stmt += "VALUES(:msn, :input_time, :decision_time, :decision, :amount, :terms, ";
    sql_stmt += ":decline_reason)";
    sql_stmt = sql_stmt.UpperCase();
    Data->command->CommandText = sql_stmt;
    Data->command->Parameters->ParamValues["msn"] = medtResultMSN->Text;
    Data->command->Parameters->ParamValues["input_time"] = input_time;
    Data->command->Parameters->ParamValues["decision_time"] = system_time;
    if(rbApproval->Checked){
      Data->command->Parameters->ParamValues["Decision"] = 1;
      Data->command->Parameters->ParamValues["Amount"] = medtAmountApproved->Text.Trim().ToInt();
      Data->command->Parameters->ParamValues["Terms"] = medtTermsApproved->Text.Trim().ToInt();
      Data->command->Parameters->ParamValues["Decline_Reason"] = "";
    }
    else{
      Data->command->Parameters->ParamValues["Decision"] = 0;
      Data->command->Parameters->ParamValues["Amount"] = 0;
      Data->command->Parameters->ParamValues["Terms"] = 0;
      Data->command->Parameters->ParamValues["Decline_Reason"] = edtDeclineReason->Text;
    };
    Data->command->Execute();
    btnClearDecisionClick(NULL);
    message += "審查結果已經儲存。可以進行下一筆輸入。";
    lblMessage->Caption = message;
    frmMainUI->Refresh();

  }
  catch(Exception &E){
    message += E.Message;
    lblMessage->Caption = message;
    frmMainUI->Refresh();
  };
} */
//---------------------------------------------------------------------------
/*
void __fastcall TfrmMainUI::tsDecisionEnter(TObject *Sender)
{
  btnClearDecision->Click();
}*/
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::tsDataEntryEnter(TObject *Sender)
{
  btnClear->Click();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::tsReportGenEnter(TObject *Sender)
{
  medtRptYear->Clear();
  medtRptMonth->Clear();
  medtRptDate->Clear();
  medtRptHour->Clear();
  lblMessage->Caption = "";
  message = "";
  frmMainUI->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnScoreKeyPress(TObject *Sender, char &Key)
{
  if(Key == 0x0A)
    btnScore->Click();  
}
//---------------------------------------------------------------------------


void __fastcall TfrmMainUI::btnRptRegenKeyPress(TObject *Sender, char &Key)
{
  if(Key == 0x0A)
    btnRptRegen->Click();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnRptGenKeyPress(TObject *Sender, char &Key)
{
  if(Key == 0x0A)
    btnRptGen->Click();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::FormDestroy(TObject *Sender)
{
  CoUninitialize();
}
//---------------------------------------------------------------------------
/*
void __fastcall TfrmMainUI::rbDeclineClick(TObject *Sender)
{
  Label33->Enabled = false;
  Label34->Enabled = false;
  medtAmountApproved->Enabled = false;
  medtTermsApproved->Enabled = false;
  Label35->Enabled = true;
  edtDeclineReason->Enabled = true;
  edtDeclineReason->Clear();
}*/
//---------------------------------------------------------------------------
void get_product_feature(const AnsiString &product_code, TADOQuery *query, double &apr, unsigned int &application_fee)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM PRODUCT_LOOKUP WHERE CODE = :product_code";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["product_code"] = product_code;
  query->Open();

  apr = query->FieldValues["RATE"];
  application_fee = query->FieldValues["APPLICATION_FEE"];
  query->Close();
};

void get_product_code(const AnsiString &msn, AnsiString &product_code, TADOQuery *query)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT PRODUCT_CODE FROM MAIL_LIST WHERE MSN = :msn";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  product_code = query->FieldValues["PRODUCT_CODE"];
  query->Close();
};


void __fastcall TfrmMainUI::medtMSNKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(medtMSN->Text.Length() == 1)
    lblMessage->Caption = "";
}
//---------------------------------------------------------------------------

bool check_income(const AnsiString &income_str, unsigned int &income)
{
  income = income_str.ToInt() / 12;
  bool return_value = true;
  if(income < 12500)
    return_value = false;
  return return_value;
};


bool check_and_cleanup_result(const AnsiString &msn, TADOQuery *query)
{
  AnsiString sql_stmt;
  bool return_value = true;
  sql_stmt = "SELECT COUNT(*) AS CNT FROM RESULT WHERE MSN = :msn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  int results = query->FieldValues["CNT"];
  query->Close();
  if(results > 0){
    if(MessageDlg("本申請件已評分，是否要重新評分?",
               mtWarning	, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes){
      sql_stmt = "DELETE FROM RESULT WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM APPLICANT WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM BAM086 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM JAS002 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM KRM021 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM KRM023 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM KRM037 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM STM007 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM VAM102 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
    }
    else
      return_value = false;
  };
  return return_value;
};

//---------------------------------------------------------------------------

