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
extern AnsiString input_user;
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnExit1Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMainUI::btnClearClick(TObject *Sender)
{
  frmMainUI->pcUI->ActivePageIndex = 0;
  frmMainUI->edtName->Clear();
  frmMainUI->medtID->Clear();
  frmMainUI->medtBirthYear->Clear();
  frmMainUI->medtBirthMonth->Clear();
  frmMainUI->medtBirthDate->Clear();
//  frmMainUI->rbFixedLine->Checked = true;;
  frmMainUI->medtFixedPhoneRegion->Clear();
  frmMainUI->medtFixedPhoneNumber->Clear();
  frmMainUI->medtCellRegion->Clear();
  frmMainUI->medtCellNumber->Clear();
  frmMainUI->medtZIP->Clear();
/* frmMainUI->medtOccupationCode->Clear();
  frmMainUI->medtJobCode->Clear();
  frmMainUI->medtJobStartYear->Clear();
  frmMainUI->medtJobStartMonth->Clear();*/
  frmMainUI->medtAmountApplied->Clear();
  frmMainUI->medtInterestRate->Clear();
  frmMainUI->medtTerms->Clear();
  frmMainUI->edtChannel->Clear();
  frmMainUI->medtApplicationFee->Clear();
  frmMainUI->cmbProduct->ItemIndex = -1;
  frmMainUI->cmbEducation->ItemIndex = -1;
  frmMainUI->cmbChildren->ItemIndex = -1;
  frmMainUI->cmbMarriage->ItemIndex = -1;
  frmMainUI->medtAnnualIncome->Clear();
//  frmMainUI->edtBranch->Clear();  
  frmMainUI->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::pcUIEnter(TObject *Sender)
{
  btnClear->Click();
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

bool check_id(const AnsiString &idn, unsigned int &gender)
{
  bool result = false;
  int number;
  AnsiString id = idn.Trim();
  if(id.Length()<10)
    return result;
  if(id.SubString(2,1)!= "1" && id.SubString(2,1)!= "2"){
    gender = -1;
    return result;
  };

  gender = id.SubString(2,1).ToInt();
  gender -= 2;
  gender *= -1;
  

  if (id.SubString(1,1).UpperCase() == "A"){
    number = 1 + 0 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "B"){
    number = 1 + 1 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "C"){
    number = 1 + 2 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "D"){
    number = 1 + 3 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "E"){
    number = 1 + 4 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "F"){
    number = 1 + 5 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "G"){
    number = 1 + 6 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "H"){
    number = 1 + 7 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "I"){
    number = 3 + 4 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "J"){
    number = 1 + 8 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "K"){
    number = 1 + 9 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "L"){
    number = 2 + 0 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "M"){
    number = 2 + 1 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "N"){
    number = 2 + 2 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "O"){
    number = 3 + 5 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "P"){
    number = 2 + 3 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "Q"){
    number = 2 + 4 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "R"){
    number = 2 + 5 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "S"){
    number = 2 + 6 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "T"){
    number = 2 + 7 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "U"){
    number = 2 + 8 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "V"){
    number = 2 + 9 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "W"){
    number = 3 + 2 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "X"){
    number = 3 + 0 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "Y"){
    number = 3 + 1 * 9;
  }
  else if (id.SubString(1,1).UpperCase() == "Z"){
    number = 3 + 3 * 9;
  };
  for (int i = 2; i < 10; ++ i){
    AnsiString ck = id.SubString(i,1);
    number += id.SubString(i,1).ToInt() * (10-i);
  };
  number = number % 10;
  number = 10 - number;
  if(number == 10)
    number = 0;
  if (number == id.SubString(10,1).ToInt())
   result = true;

  return result;
}
//---------------------------------------------------------------------------

bool check_date(unsigned int year, unsigned int month, unsigned int date)
{
//true for valid; false for invalid
  bool return_value = true;
  if(date < 1)
    return_value = false;
  else{
    switch (month){
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
        if(date > 31)
          return_value = false;
        break;
      case 4:
      case 6:
      case 9:
      case 11:
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

bool check_occupation(string occupation)
{
 bool return_value = true;
 string valid_occupation_code[] =  {
  "0101", "0102", "0103", "0201", "0202",
  "0203", "0204", "0205", "0206", "0207",
  "0208", "0209", "0210", "0211", "0212",
  "0213", "0214", "0215", "0301", "0302",
  "0303", "0304", "0401", "0402", "0403",
  "0404", "0405", "0406", "0407", "0408",
  "0501", "0502", "0503", "0504", "0506",
  "0507", "0508", "0509", "0510", "0511",
  "0512", "0601", "0602", "0603", "0604",
  "0605", "0606", "0607", "0701", "0702",
  "0703", "0704", "0705", "0706", "0707",
  "0708", "0709", "0710", "0711", "0712",
  "0713", "0714", "0715", "0716", "0717",
  "0718", "0719", "0801", "0802", "0803",
  "0804", "0805", "0806", "0901", "0902",
  "0903", "0904", "0905", "0906", "0907",
  "0908", "0909", "0910", "0911", "0912",
  "0913", "0914", "0915", "0916", "0917",
  "0918", "1001", "1002", "1003", "1004",
  "1005", "1006"
  };

 int occupationCount = sizeof(valid_occupation_code) / sizeof (valid_occupation_code[0]);  // get no. of zip code
 // Declare a set with key only, Initial it with all zip code ascendingly
 set<string, less<string> > occupationSet (valid_occupation_code, valid_occupation_code+occupationCount+1);
 set<string, less<string> >::iterator iter;

 iter = occupationSet.find(occupation);
 if (iter == occupationSet.end())
    return_value = false;

 return return_value;
};

bool check_job(string job)
{
 bool return_value = true;
 string valid_job[] =  {    // Valid job codes
  "0101", "0102", "0103", "0104", "0105",
  "0106", "0201", "0202", "0203", "0204",
  "0205", "0301", "0302", "0303", "0304",
  "0305", "0306", "0307", "0308", "0309",
  "0310", "0311", "0401", "0402", "0403",
  "0404", "0405", "0406", "0407", "0408",
  "0409", "0501", "0502", "0503", "0504",
  "0505", "0506", "0507", "0508", "0509"
 };

 int jobCount = sizeof(valid_job) / sizeof (valid_job[0]);  // get no. of zip code
 // Declare a set with key only, Initial it with all zip code ascendingly
 set<string, less<string> > jobSet (valid_job, valid_job+jobCount+1);
 set<string, less<string> >::iterator iter;

 iter = jobSet.find(job);
 if (iter == jobSet.end())
    return_value = false;

 return return_value;
};

bool check_amount(const AnsiString &amount)
{
  bool return_value = true;
  int request_amount = amount.ToInt();
  request_amount *= 10000;
  if((request_amount < 150000) || (request_amount > 1000000))
    return_value = false;
  return return_value;
};

bool check_terms(const AnsiString &terms)
{
  bool return_value = true;
  int request_terms = terms.ToInt();
  if((request_terms < 24) || (request_terms > 84))
    return_value = false;
  return return_value;
};

bool check_rate(const AnsiString &rate)
{
  bool return_value = true;
  double request_rate = rate.ToDouble();
  if((request_rate <= 0) || (request_rate > 20))
    return_value = false;
  return return_value;
};

bool check_branch(TADOQuery *query, const AnsiString &branch)
{
  bool return_value = true;
  AnsiString sql_stmt = "SELECT COUNT(*) AS CNT FROM BRANCH WHERE BRANCH_CODE = :branch";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["branch"] = branch.Trim();
  query->Open();
  int count = query->FieldValues["CNT"];
  if(count == 0)
    return_value = false;
  return return_value;
};

//---------------------------------------------------------------------------
int prescreen(TADOQuery *query, const AnsiString &id, AnsiString &code)
{
  int return_value = 0;//0: run model; 1: directly approved; 2: directly declined
  int count = 0;
  code = "";
  AnsiString sql_stmt = "SELECT * FROM Black_List WHERE Code IN ('03', '04') ";
  sql_stmt += "AND IDN = :idn";
  sql_stmt = sql_stmt.UpperCase();

  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = id;
  query->Open();
  count = query->RecordCount;
  if (count > 0){
    return_value = 2;
    code = query->FieldValues["Code"];
  };
  query->Close();

  if(count == 0){
    sql_stmt = "SELECT MAX(Priority) AS Max_Pri FROM Customer_List AS A INNER JOIN ";
    sql_stmt += "Customer_List_Lookup ON A.Code = B.Code WHERE IDN = :idn ";
    sql_stmt += "HAVING MAX(Priority) <= 3;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = id;
    query->Open();
    count = query->RecordCount;

    if (count > 0){
      return_value = 2;
      int priority = query->FieldValues["Max_Pri"];
      query->Close();
      sql_stmt = "SELECT Code FROM Customer_List_Lookup WHERE Priority = :priority;";
      sql_stmt = sql_stmt.UpperCase();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["priority"] = priority;
      query->Open();
      code = query->FieldValues["Code"];
    };
  };

  if(count == 0){
    sql_stmt = "SELECT MAX(Priority) FROM Customer_List AS A INNER JOIN ";
    sql_stmt += "Customer_List_Lookup ON A.Code = B.Code WHERE IDN = :idn ";
    sql_stmt += "HAVING MAX(Priority) = 6;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = id;
    query->Open();
    count = query->RecordCount;
    query->Close();
    if (count > 0){
      return_value = 1;
    };
  };
  return return_value;
};

void init_UI()
{
  frmMainUI->pcUI->ActivePageIndex = 0;
  frmMainUI->edtName->Clear();
  frmMainUI->medtID->Clear();
  frmMainUI->medtBirthYear->Clear();
  frmMainUI->medtBirthMonth->Clear();
  frmMainUI->medtBirthDate->Clear();
//  frmMainUI->rbFixedLine->Checked = true;;
  frmMainUI->medtFixedPhoneRegion->Clear();
  frmMainUI->medtFixedPhoneNumber->Clear();
  frmMainUI->medtCellRegion->Clear();
  frmMainUI->medtCellNumber->Clear();
  frmMainUI->medtZIP->Clear();
/*  frmMainUI->medtOccupationCode->Clear();
  frmMainUI->medtJobCode->Clear();
  frmMainUI->medtJobStartYear->Clear();
  frmMainUI->medtJobStartMonth->Clear();*/
  frmMainUI->medtAmountApplied->Clear();
  frmMainUI->medtInterestRate->Clear();
  frmMainUI->medtTerms->Clear();
  frmMainUI->edtChannel->Clear();
  frmMainUI->edtBranch->Clear();
  frmMainUI->cmbProduct->ItemIndex = -1;
  frmMainUI->cmbMarriage->ItemIndex = -1;
  frmMainUI->cmbChildren->ItemIndex = -1;
  frmMainUI->cmbEducation->ItemIndex = -1;
  frmMainUI->medtAnnualIncome->Clear();
  frmMainUI->medtApplicationFee->Clear();
  frmMainUI->Refresh();
};

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

__fastcall TfrmMainUI::TfrmMainUI(TComponent* Owner)
  : TForm(Owner)
{
  CoInitialize(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnScoreClick(TObject *Sender)
{
  bool is_input_error = false;
  bool no_fixed_phone = false;
  unsigned int gender;
  unsigned int gender_secondary;
  unsigned int monthly_income;
  int loan_type;
  lblMessage->Caption = "";
  message = "";
  frmMainUI->Refresh();
//Check input
  if(!check_id(medtID->Text, gender)){
    message += "申請人身分證號錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(medtFixedPhoneRegion->Text.IsEmpty() && medtFixedPhoneNumber->Text.IsEmpty())
    no_fixed_phone = true;
  else if(!check_phone(medtFixedPhoneRegion->Text, medtFixedPhoneNumber->Text, false)){
    message += "固網電話號碼錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };

  if(medtCellRegion->Text.IsEmpty() && medtCellNumber->Text.IsEmpty() && no_fixed_phone){
    message += "必須輸入至少一個電話號碼\n";
    lblMessage->Caption = message;
    is_input_error = true;
  }
  else if(!check_phone(medtCellRegion->Text, medtCellNumber->Text, true)){
    message += "行動電話號碼錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
/*  if(!check_occupation(medtOccupationCode->Text.Trim().c_str())){
    message += "職業代碼錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!check_job(medtJobCode->Text.Trim().c_str())){
    message += "職稱代碼錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };*/
  if(cmbEducation->ItemIndex == -1){
    message += "教育程度錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!check_zip(medtZIP->Text.Trim().c_str())){
    message += "郵遞區號錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(cmbChildren->ItemIndex == -1){
    message += "扶養子女數錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(cmbMarriage->ItemIndex == -1){
    message += "婚姻狀態錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!check_amount(medtAmountApplied->Text.Trim())){
    message += "申貸金額錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!check_terms(medtTerms->Text.Trim())){
    message += "申貸期數錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!check_rate(medtInterestRate->Text.Trim())){
    message += "申貸利率錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!check_income(medtAnnualIncome->Text.Trim(), monthly_income)){
    message += "收入錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(cmbProduct->ItemIndex == -1){
    message += "專案種類錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  }
  else
    loan_type = cmbProduct->ItemIndex + 2; //1 is reserved for 農家樂, which is taken out of the applicable product


  if(!check_branch(Data->query, edtBranch->Text.Trim().c_str())){
    message += "進件管道錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  get_time(year, month, day, hour, min);

  unsigned int bad_date = check_birthday(medtBirthYear->Text,
                                         medtBirthMonth->Text,
                                         medtBirthDate->Text,
                                         year, month, day, true);
  if (bad_date > 0){
    is_input_error = true;
    if((bad_date & 0x1) == 0x1)
      message += "申請人出生月份錯誤\n";
    if((bad_date & 0x2) == 0x2)
      message += "申請人出生日期錯誤\n";
    if((bad_date & 0x4) == 0x4)
      message += "申請人出生於未來\n";
    if((bad_date & 0x8) == 0x8)
      message += "申請人未滿20歲\n";
    if((bad_date & 0x20) == 0x20)
      message += "申請人出生年份錯誤\n";
    if((bad_date & 0x40) == 0x40)
      message += "申請人超過55歲\n";
    lblMessage->Caption = message;
  };

/*  unsigned int bad_job_ym = check_job_ym(medtJobStartYear->Text,
                                         medtJobStartMonth->Text,
                                         year, month);
  if (bad_job_ym > 0){
    is_input_error = true;
    if((bad_job_ym & 0x1) == 0x1)
      message += "申請人就任現職月份錯誤\n";
    if((bad_job_ym & 0x4) == 0x4)
      message += "申請人就任現職於未來\n";
    if((bad_job_ym & 0x20) == 0x20)
      message += "申請人就任現職年份錯誤\n";
    lblMessage->Caption = message;
  };*/
//Get data for scoring
  unsigned int request_amount = medtAmountApplied->Text.Trim().ToInt();
  request_amount *= 10000;
  unsigned int request_terms = medtTerms->Text.Trim().ToInt();
  unsigned int application_fee = medtApplicationFee->Text.Trim().ToInt();
  double request_rate = medtInterestRate->Text.Trim().ToDouble();
  request_rate /= 100.0;

  frmMainUI->Refresh();

  if(!is_input_error){
    int prescreen_result;
//Use systemn time as input time
    AnsiString year_str = static_cast<AnsiString>(year);
    AnsiString month_str = static_cast<AnsiString>(month);
    AnsiString day_str = static_cast<AnsiString>(day);
    AnsiString hour_str = static_cast<AnsiString>(hour);
    AnsiString min_str = static_cast<AnsiString>(min);
//Format input time
    if(month_str.Length() == 1)
      month_str = "0" + month_str;
    if(day_str.Length() == 1)
      day_str = "0" + day_str;
    if(hour_str.Length() == 1)
      hour_str = "0" + hour_str;
    if(min_str.Length() == 1)
      min_str = "0" + min_str;

    AnsiString input_time = year_str + month_str + day_str + hour_str + min_str;

    try{
      AnsiString birth_year = static_cast<AnsiString>(medtBirthYear->Text.Trim().ToInt() + 1911);
      AnsiString birth_month = medtBirthMonth->Text.Trim();
      AnsiString birth_date =  medtBirthDate->Text.Trim();
      birth_month = birth_month.Length() == 1 ? "0" + birth_month : birth_month;
      birth_date = birth_date.Length() == 1 ? "0" + birth_date : birth_date;
      AnsiString birthday = birth_year + birth_month + birth_date;

//      AnsiString job_start_year = static_cast<AnsiString>(medtJobStartYear->Text.Trim().ToInt() + 1911);
//      AnsiString job_start_month = medtJobStartMonth->Text.Trim();
//      job_start_month = job_start_month.Length() == 1 ? "0" + job_start_month : job_start_month;
//      AnsiString job_start_ym = job_start_year + job_start_month;


      message += "申請件資料通過檢核，正在取回聯徵資料。\n";
      lblMessage->Caption = message;
      frmMainUI->Refresh();

      AnsiString query_sn;
      AnsiString jcic_inquiry_result;
      AnsiString jcic_inquiry_date;
//      AnsiString ole_db = "Provider=SQLOLEDB.1;Password=dac_usr1;Persist Security Info=True;User ID=dac_usr1;Initial Catalog=PLOAN;Data Source=DAC-DB2";
      AnsiString ole_db = Data->connection->ConnectionString;
      char error[256];
      int module_operation;
      bool success;
      int ejcic_error_code;
//Send request for JCIC data to e-JCIC
      success = request_for_jcic_data(Data->ejcic_query, medtID->Text.Trim(), query_sn, jcic_inquiry_result, ejcic_error_code);
        if(success){
          message += "聯徵資料已取回，正在儲存資料。\n";
          lblMessage->Caption = message;
          frmMainUI->Refresh();
//Store input data from GUI
          store_input(Data->command,
                      medtID->Text.Trim(), input_time, edtName->Text.Trim(),
                      birthday ,
                      medtFixedPhoneRegion->Text.Trim() + medtFixedPhoneNumber->Text.Trim(),
                      medtCellRegion->Text.Trim() + medtCellNumber->Text.Trim(),
                      medtZIP->Text.Trim(), cmbMarriage->ItemIndex + 1,
                      cmbChildren->ItemIndex + 1, cmbEducation->ItemIndex + 1,
                      /*job_start_ym,*/
                      request_amount, request_rate, request_terms,
                      application_fee, loan_type, monthly_income,
                      edtChannel->Text.Trim(), edtBranch->Text.Trim(), input_user);
//Store JCIC data
          jcic_inquiry_date =
            get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn, medtID->Text, input_time);

          message += "資料已儲存，正在評分中。\n";
          lblMessage->Caption = message;
          frmMainUI->Refresh();
//Calculate pb and npv
//The result will be store in table 'RESULT'
            module_operation =
                TNB_Ploan_AM(medtID->Text.c_str(), jcic_inquiry_date.c_str(),
                             input_time.c_str(), "054", ole_db.c_str(),
                             gender, error);

            if(module_operation != 0){
              lblMessage->Caption = static_cast<AnsiString>(error);
              frmMainUI->Refresh();
            };
        }
        else{
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
//0x0 for normal; 0x1 for month error; 0x2 for date error;
//0x4 for born in the future; 0x8 for primary card applicant under age;
//0x10 for secondary card applicant under age;
//0x20 for missing birth year;
//0x40 for age over 55
unsigned int check_birthday(const AnsiString &birth_year,
                            const AnsiString &birth_month,
                            const AnsiString &birth_date,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day,
                            bool primary_card_applicant)
{
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

    if(month_born > 12 || month_born < 1)
      return_value |= 0x1;
    else if(!check_date(year_born, month_born, date_born))
      return_value |= 0x2;
    else{
      int age = calculate_age(year_born, month_born, date_born, year, month, day);
      if(age < 0)
        return_value |= 0x4;
      if(age > 55)
        return_value |= 0x40;
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


unsigned int check_job_ym(const AnsiString &job_start_year,
                          const AnsiString &job_start_month,
                          const unsigned int year, const unsigned int month)
{
  unsigned int return_value = 0;

  if(job_start_year.Length()==0)
    return_value |= 0x20;
  if(job_start_month.Length()==0)
    return_value |= 0x1;

  if(return_value == 0){
    unsigned int year_start = job_start_year.ToInt();
    unsigned int month_start = job_start_month.ToInt();

    year_start += 1900;

    if(month_start > 12)
      return_value |= 0x1;
    else{
      int age = calculate_age(year_start, month_start, 1, year, month, 1);
      if(age < 0)
        return_value |= 0x4;
    };
  };
  return return_value;
};

bool check_phone(const AnsiString &region_code, const AnsiString &phone_no, bool celluar)
{
  bool return_value = true;
  if(celluar){
    if((phone_no.Trim().Length() != 6) || (region_code.Trim().SubString(1,2) != "09") ||
       (region_code.Trim().Length() != 4))
      return_value = false;
  }
  else{
    AnsiString long_phone_no = region_code.Trim() + phone_no.Trim();
    if(((long_phone_no.Length() != 10) && (long_phone_no.Length() != 9)) || (!check_region_code(region_code)))
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
   int code = region_code.Trim().ToInt();
   switch (code){
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 37:
    case 49:
    case 82:
    case 89:
    case 826:
    case 836:
      break;
    default:
      return_value = false;
   }
 };
 return return_value;
};

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
int prescreen(TADOQuery *query,
              const AnsiString &id, const AnsiString &input_time, int alien_bride)
{
  int return_value = 0;//0: run model; 1: directly approved; 2: directly declined

  AnsiString sql_stmt = "SELECT * FROM Primary_Card_Applicant WHERE IDN = :idn ";
  sql_stmt += " AND Input_Time = :input_time;";
  sql_stmt = sql_stmt.UpperCase();

  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = id;
  query->Parameters->ParamValues["input_time"] = input_time;
  query->Open();
  AnsiString black_list_code = "";
  if(!query->FieldValues["Black_List_Code"].IsNull())
    black_list_code = query->FieldValues["Black_List_Code"];
  AnsiString customer_list_code = "";
  if(!query->FieldValues["Customer_List_Code"].IsNull())
    customer_list_code =  query->FieldValues["Customer_List_Code"];

  if((black_list_code == "03") || (black_list_code == "04") ||
     (customer_list_code == "CC") || (customer_list_code == "28") ||
     (customer_list_code == "55") || (alien_bride == 1) ||
     (customer_list_code == "CD"))
    return_value = 2;
  else if(customer_list_code == "00")
    return_value = 1;

  return return_value;
};

void store_input(TADOCommand *command,
                 const AnsiString &id, const AnsiString &input_time,
                 const AnsiString &name, const AnsiString &birthday ,
                 const AnsiString &fixed_phone, const AnsiString &cell_phone, 
                 const AnsiString &zip,
                 int marriage_code, int children_code, int edu_code,
                 unsigned int request_amount, double request_rate,
                 unsigned int request_terms, unsigned int application_fee,
                 int product_type, unsigned int monthly_income,
                 const AnsiString &channel, const AnsiString &branch,
                 const AnsiString &input_user)
{
  AnsiString sql_stmt;
  sql_stmt = "INSERT INTO Applicant(IDN, INPUT_TIME, CNAME, ";
//  sql_stmt += "BIRTHDAY, PHONE, ZIP, OCCUPATION, JOB, ON_BOARD_YM, ";
  sql_stmt += "BIRTHDAY, ZIP, FIXED_LINE_PHONE, CELL_PHONE, ";
  sql_stmt += "APPLICATION_AMOUNT, APPLICATION_RATE, APPLICATION_TERMS, ";
  sql_stmt += "APPLICATION_FEE, CHANNEL, MONTHLY_INCOME, PRODUCT_TYPE, BRANCH, ";
  sql_stmt += "MARRIAGE, CHILDREN, EDUCATION, INPUT_USER) ";
//  sql_stmt += "VALUES(:idn, :input_time, :name, :birthday, :phone, :zip, ";
  sql_stmt += "VALUES(:idn, :input_time, :name, :birthday, :zip, :fixed_phone, :cell_phone, ";
//  sql_stmt += ":occupation, :job, :on_board_ym, :amount, :rate, ";
  sql_stmt += ":amount, :rate, ";
  sql_stmt += ":terms, :fee, :channel, :income, :product, :branch, :marriage, ";
  sql_stmt += ":children, :edu, :input_user)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = id.Trim();
  command->Parameters->ParamValues["input_time"] = input_time;
  command->Parameters->ParamValues["name"] = name;
  command->Parameters->ParamValues["birthday"] = birthday;
//  command->Parameters->ParamValues["phone"] = phone;
  command->Parameters->ParamValues["fixed_phone"] = fixed_phone;
  command->Parameters->ParamValues["cell_phone"] = cell_phone;
  command->Parameters->ParamValues["zip"] = zip;
//  command->Parameters->ParamValues["occupation"] = occupation_code;
//  command->Parameters->ParamValues["job"] = job_code;
//  command->Parameters->ParamValues["on_board_ym"] = job_start_ym;
  command->Parameters->ParamValues["amount"] = request_amount;
  command->Parameters->ParamValues["rate"] = request_rate;
  command->Parameters->ParamValues["terms"] = request_terms;
  command->Parameters->ParamValues["fee"] = application_fee;
  command->Parameters->ParamValues["channel"] = channel;
  command->Parameters->ParamValues["income"] = monthly_income;
  command->Parameters->ParamValues["product"] = product_type;
  command->Parameters->ParamValues["branch"] = branch;
  command->Parameters->ParamValues["edu"] = edu_code;
  command->Parameters->ParamValues["marriage"] = marriage_code;
  command->Parameters->ParamValues["children"] = children_code;
  command->Parameters->ParamValues["input_user"] = input_user;
  command->Execute();
};

AnsiString get_store_jcic_data(TADOConnection *ejcic_connection,
                         TADOQuery *ejcic_query, TADOCommand *command,
                         const AnsiString &query_sn, const AnsiString &idn,
                         const AnsiString &input_time)
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


  sql_stmt = "INSERT INTO KRM023(IDN, INQUIRY_DATE, YRMON, ISSUE, ISSUE_NAME, ";
  sql_stmt += "KR_CODE, LIMIT, PAYMENT, CASH, PAY_CODE, Input_Time) VALUES(:idn, ";
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


  sql_stmt = "INSERT INTO KRM021(IDN, INQUIRY_DATE, CARD_BRAND, CARD_TYPE, ISSUE, ";
  sql_stmt += "ISSUE_NAME, START_DATE, STOP_DATE, STOP_CODE, AB_CODE, ";
  sql_stmt += "M_S, RELA, LIMIT, RISK, CLEAR_DATE, IDN_PRI, CNAME, REMARK, INPUT_TIME) VALUES(:idn, ";
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
    command->Parameters->ParamValues["idn"] = idn.UpperCase();
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

  sql_stmt = "INSERT INTO BAM086(IDN, INQUIRY_DATE, DATA_YYY, DATA_MM, BANK_CODE, ";
  sql_stmt += "BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, ";
  sql_stmt += "LOAN_AMT, PASS_DUE_AMT, PAY_CODE_12, CO_LOAN, INPUT_TIME) VALUES (:idn, ";
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

  sql_stmt = "INSERT INTO JAS002(IDN, INQUIRY_DATE, EVER_DELINQUENT, DELINQUENT_DATE, ";
  sql_stmt += "EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, ";
  sql_stmt += "EVER_STOP_CARD, STOP_CARD_DATE, INPUT_TIME) VALUES (:idn, :inquiry_date, ";
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

  sql_stmt = "INSERT INTO STM007(IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, BANK_NAME, ";
  sql_stmt += "ITEM_LIST, INPUT_TIME, INQ_PURPOSE_1, INQ_PURPOSE) VALUES (:idn, :INQUIRY_DATE, :query_date, ";
  sql_stmt += ":bank_code, :bank_name, :item_list, :input_time, :inq_purpose_1, :inq_purpose);";
	command->CommandText = sql_stmt;
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_STM007 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["idn"] = idn;
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
  sql_stmt = "INSERT INTO KRM037 (";
  sql_stmt += "IDN, INQUIRY_DATE, BILL_DATE, ISSUE, ISSUE_NAME, CARD_TYPE, ";
  sql_stmt += "PERM_LIMIT, TEMP_LIMIT, CASH_LIMIT, PAYABLE, CASH_LENT, ";
  sql_stmt += "LAST_PAYA, REVOL_BAL, PAY_STAT, PAY_CODE, REVOL_RATE, ";
  sql_stmt += "PRE_OWED,DEBT_CODE, CLOSE_CODE, CLEAR_DATE, INPUT_TIME) VALUES(";
  sql_stmt += ":IDN, :INQUIRY_DATE, :BILL_DATE, :ISSUE, :ISSUE_NAME, :CARD_TYPE, ";
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

  sql_stmt = "INSERT INTO VAM102 (";
  sql_stmt += "IDN, INQUIRY_DATE, DATA_DATE, MAINCODE, MAINNOTE, SUBCODE, SUBNOTE, NOTE, INPUT_TIME) ";
  sql_stmt += "VALUES(:IDN, :INQUIRY_DATE, :DATA_DATE, :MAINCODE, :MAINNOTE, :SUBCODE, ";
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

/*  try{
    sql_stmt = "DROP TABLE First_Application";
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

  sql_stmt = "CREATE TABLE Report ";
  sql_stmt +="(INPUT_USER VARCHAR(40), IDN char(11), Input_Time Char(12), ";
  sql_stmt += "CName Char(20), Gender Char(2), Marriage Char(4), Children Char(8), Education Char(4), ";
  sql_stmt += "Fixed_Phone Char(10), Cell_Phone Char(10), ";
  sql_stmt += "Application_amount int, Approved_amount int, Application_terms int, ";
  sql_stmt += "Application_fee int, ";
  sql_stmt += "RATE Decimal(5,4), PB Decimal(6,5), NPV INT, MIN_RATE Decimal(5,4), ";
  sql_stmt += "SUGGESTION CHAR(60), Approved_amount_fsc int, Unsecured_balance int, CFC CHAR(20), SPECIAL_NOTE CHAR(256))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
/*
  sql_stmt = "CREATE TABLE First_Application ";
  sql_stmt += "(IDN CHAR(11), INPUT_TIME CHAR(12), ";
  sql_stmt += "CNAME VARCHAR(20), PHONE CHAR(10), CELL_PHONE CHAR(10), ";
  sql_stmt += "APPLICATION_AMOUNT INT, ";
  sql_stmt += "APPLICATION_RATE DECIMAL(6,5), APPLICATION_TERMS INT, ";
  sql_stmt += "APPLICATION_FEE INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO First_Application(IDN, Input_Time) ";
  sql_stmt += "SELECT IDN, MIN(Input_Time) FROM APPLICANT GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE First_Application SET ";
  sql_stmt += "CNAME = A.CNAME, PHONE =A.PHONE, ";
  sql_stmt += "APPLICATION_AMOUNT =A.APPLICATION_AMOUNT, ";
  sql_stmt += "APPLICATION_TERMS =A.APPLICATION_TERMS ";
  sql_stmt += "FROM APPLICANT AS A INNER JOIN First_Application AS B ";
  sql_stmt += "ON A.IDN = B.IDN AND A.Input_Time = B.Input_Time";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
*/
  sql_stmt = "INSERT INTO Report(INPUT_USER, IDN, Input_Time, CName, Gender, Fixed_Phone, Cell_Phone, Marriage, Children, Education, Suggestion, ";
  sql_stmt += "Application_amount, Approved_amount, Application_terms, ";
  sql_stmt += "Application_fee, Rate, PB, NPV, MIN_RATE, Approved_Amount_FSC, Unsecured_Balance, CFC, SPECIAL_NOTE) ";
  sql_stmt += "SELECT A.INPUT_USER, A.IDN, A.Input_Time, A.CName, ";
  sql_stmt += "(CASE WHEN SUBSTRING(A.IDN, 2, 1) = '1' THEN '男' ELSE '女' END), ";
  sql_stmt += "A.Fixed_Phone, A.Cell_Phone, E.DES, F.DES, G.DES, B.Suggestion, ";
  sql_stmt += "A.Application_amount, B.Approved_amount, A.Application_terms, ";
  sql_stmt += "A.Application_fee, B.PB, B.NPV, B.MIN_RATE, B.Approved_Amount_FSC, B.Unsecured_Balance,  D.CFC, B.SPECIAL_NOTE ";
  sql_stmt += "FROM Result AS B INNER JOIN Applicant AS A ";
  sql_stmt += "ON A.Input_Time = B.Input_Time AND A.IDN = B.IDN ";
  sql_stmt += "INNER JOIN CFC_BRANCH AS C ON A.BRANCH = C.BRANCH INNER JOIN CFC AS D ";
  sql_stmt += "ON C.CFC_CODE = D.CFC_CODE INNER JOIN MARRIAGE_LOOKUP AS E ";
  sql_stmt += "ON A.MARRIAGE = E.CODE INNER JOIN CHILDREN_LOOKUP AS F ";
  sql_stmt += "ON A.CHILDREN = F.CODE INNER JOIN EDUCATION_LOOKUP AS G ";
  sql_stmt += "ON A.EDUCATION = G.CODE";

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
    AnsiString year = report_gen_time.SubString(1,4);
    year = static_cast<AnsiString>(year.ToInt() - 1911);

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


    if(total && individual && report_individual && manual && report_manual){
      total << "京城銀行個人信貸評分結果名單" << endl;
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
      total << "進件消金中心" << ",";
      total << "申請人身分證號" << ",";
      total << "申請人姓名" << ",";
      total << "申請件輸入時間" << ",";
      total << "申貸金額(NT$)" << ",";
      total << "核准金額(NT$)" << ",";
      total << "收入22倍限制下核准金額(NT$)" << ",";
      total << "無擔保貸款餘額(NT$)" << ",";
      total << "申貸期數" << ",";
      total << "申貸利率" << ",";
      total << "模組建議" << ",";
      total << "壞帳機率" << ",";
      total << "經濟價值" << ",";
      total << "最低可貸利率" << ",";
      total << "聯徵特殊註記" <<endl;

      report_individual << "CFC" <<",";
      report_individual << "Input_staff" <<",";
      report_individual << "Input_time" <<",";
      report_individual << "Report_time" <<",";
      report_individual << "id" <<",";
      report_individual << "name" <<",";
      report_individual << "fixed_phone" <<",";
      report_individual << "cell_phone" <<",";
      report_individual << "marriage_status" << ",";
      report_individual << "children" << ",";
      report_individual << "education" << ",";
      report_individual << "amount" <<",";
      report_individual << "approved_amount" <<",";
      report_individual << "approved_amount_fsc" << ",";
      report_individual << "unsecured_balance" <<",";
      report_individual << "terms" <<",";
      report_individual << "rate" <<",";
      report_individual << "suggestion" <<",";
      report_individual << "pb" <<",";
      report_individual << "npv" << ",";
      report_individual << "min_rate" << ",";
      report_individual << "special_note" << endl;

      individual << "CFC" <<",";
      individual << "Input_staff" <<",";
      individual << "Input_time" <<",";
      individual << "Report_time" <<",";
      individual << "id" <<",";
      individual << "name" <<",";
      individual << "fixed_phone" <<",";
      individual << "cell_phone" <<",";
      individual << "marriage_status" << ",";
      individual << "children" << ",";
      individual << "education" << ",";
      individual << "amount" <<",";
      individual << "approved_amount" <<",";
      individual << "approved_amount_fsc" << ",";
      individual << "unsecured_balance" <<",";
      individual << "terms" <<",";
      individual << "rate" <<",";
      individual << "suggestion" <<",";
      individual << "pb" <<",";
      individual << "npv" << ",";
      individual << "min_rate" << ",";
      individual << "special_note" << endl;

      manual << "report_time" << ",";
      manual << "name" <<",";
      manual << "id" <<",";
      manual << "input_time" << ",";
      manual << "gender" << ",";
      manual << "fixed_phone" << ",";
      manual << "cell_phone" << ",";
      manual << "special_note" << endl;

      report_manual << "report_time" << ",";
      report_manual << "name" <<",";
      report_manual << "id" <<",";
      report_manual << "input_time" << ",";
      report_manual << "gender" << ",";
      report_manual << "fixed_phone" << ",";
      report_manual << "cell_phone" << ",";
      report_manual << "special_note" << endl;

      query->Close();
      sql_stmt = "SELECT * FROM Report ORDER BY CFC, Input_Time;";
      sql_stmt = sql_stmt.UpperCase();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Open();
      while(!query->Eof){
        total << query->FieldByName("CFC")->AsString.c_str() << ",";
        total << query->FieldByName("IDN")->AsString.c_str() << ",";
        total << query->FieldByName("CName")->AsString.c_str() << ",";
        total << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        total << query->FieldByName("Application_Amount")->AsInteger << ",";
        total << query->FieldByName("Approved_Amount")->AsInteger << ",";
        total << query->FieldByName("Approved_Amount_FSC")->AsInteger << ",";
        total << query->FieldByName("Unsecured_balance")->AsInteger << ",";
        total << query->FieldByName("Application_Terms")->AsInteger << ",";
        total << query->FieldByName("Rate")->AsFloat << ",";
        total << query->FieldByName("SUGGESTION")->AsString.c_str() << ",";
        total << query->FieldByName("PB")->AsFloat << ",";
        total << query->FieldByName("NPV")->AsFloat << ",";
        total << query->FieldByName("MIN_Rate")->AsFloat <<",";
        total << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;

        if(query->FieldByName("Suggestion")->AsString.SubString(1,16) == "模組建議人工審核"){
          manual << (year + report_gen_time.SubString(5,8)).c_str() << ",";
          manual << query->FieldByName("CName")->AsString.c_str() <<",";
          manual << query->FieldByName("IDN")->AsString.c_str() <<",";
          manual <<  query->FieldByName("Input_Time")->AsString.c_str() << ",";
          manual << query->FieldByName("Gender")->AsString.c_str() << ",";
          manual << query->FieldByName("Fixed_Phone")->AsString.c_str() << ",";
          manual << query->FieldByName("Cell_Phone")->AsString.c_str() <<",";
          manual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;

          report_manual << (year + report_gen_time.SubString(5,8)).c_str() << ",";
          report_manual << query->FieldByName("CName")->AsString.c_str() <<",";
          report_manual << query->FieldByName("IDN")->AsString.c_str() <<",";
          report_manual <<  query->FieldByName("Input_Time")->AsString.c_str() << ",";
          report_manual << query->FieldByName("Gender")->AsString.c_str() << ",";
          report_manual << query->FieldByName("Fixed_Phone")->AsString.c_str() << ",";
          report_manual << query->FieldByName("Cell_Phone")->AsString.c_str() <<",";
          report_manual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;
        };
        report_individual << query->FieldByName("CFC")->AsString.c_str() << ",";
        report_individual << query->FieldByName("INPUT_USER")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        report_individual << report_gen_time.c_str() << ",";
        report_individual << query->FieldByName("IDN")->AsString.c_str() << ",";
        report_individual << query->FieldByName("CName")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Fixed_Phone")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Cell_Phone")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Marriage")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Children")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Education")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Application_Amount")->AsInteger << ",";
        report_individual << query->FieldByName("Approved_Amount")->AsInteger << ",";
        report_individual << query->FieldByName("Approved_Amount_FSC")->AsInteger << ",";
        report_individual << query->FieldByName("Unsecured_balance")->AsInteger << ",";
        report_individual << query->FieldByName("Application_Terms")->AsInteger << ",";
        report_individual << query->FieldByName("Rate")->AsFloat << ",";
        report_individual << query->FieldByName("Suggestion")->AsString.c_str() << ",";
        report_individual << query->FieldByName("PB")->AsFloat << ",";
        report_individual << query->FieldByName("NPV")->AsFloat << ",";
        report_individual << query->FieldByName("MIN_RATE")->AsFloat <<",";
        report_individual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;

        individual << query->FieldByName("CFC")->AsString.c_str() << ",";
        individual << query->FieldByName("INPUT_USER")->AsString.c_str() << ",";
        individual << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        individual << report_gen_time.c_str() << ",";
        individual << query->FieldByName("IDN")->AsString.c_str() << ",";
        individual << query->FieldByName("CName")->AsString.c_str() << ",";
        individual << query->FieldByName("Fixed_Phone")->AsString.c_str() << ",";
        individual << query->FieldByName("Cell_Phone")->AsString.c_str() << ",";
        individual << query->FieldByName("Marriage")->AsString.c_str() << ",";
        individual << query->FieldByName("Children")->AsString.c_str() << ",";
        individual << query->FieldByName("Education")->AsString.c_str() << ",";
        individual << query->FieldByName("Application_Amount")->AsInteger << ",";
        individual << query->FieldByName("Approved_Amount")->AsInteger << ",";
        individual << query->FieldByName("Approved_Amount_FSC")->AsInteger << ",";
        individual << query->FieldByName("Unsecured_balance")->AsInteger << ",";
        individual << query->FieldByName("Application_Terms")->AsInteger << ",";
        individual << query->FieldByName("Rate")->AsFloat << ",";
        individual << query->FieldByName("Suggestion")->AsString.c_str() << ",";
        individual << query->FieldByName("PB")->AsFloat << ",";
        individual << query->FieldByName("NPV")->AsFloat << ",";
        individual << query->FieldByName("MIN_RATE")->AsFloat <<",";
        individual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << endl;

        query->Next();
      };
      total << "/EOF" << endl;
      total.clear();
      total.close();
      report_individual.clear();
      report_individual.close();
      individual.clear();
      individual.close();
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

//---------------------------------------------------------------------------

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
void __fastcall TfrmMainUI::medtEntryDateExit(TObject *Sender)
{
  btnGetSuggestionClick(NULL);
} */
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
  AnsiString input_date =  medtEntryDate->Text.Trim().Length() == 1 ? "0" + medtEntryDate->Text.Trim() : medtEntryDate->Text.Trim();
  AnsiString input_time = input_year + input_month + input_date;

  sql_stmt = "SELECT MAX(Input_Time) As Input_Time FROM Result WHERE IDN = :idn AND ";
  sql_stmt += "LEFT(Input_Time, 8) = :input_time;";
  sql_stmt = sql_stmt.UpperCase();
  try{
    Data->query->Close();
    Data->query->SQL->Clear();
    Data->query->SQL->Add(sql_stmt);
    Data->query->Parameters->ParamValues["idn"] = medtResultID->Text.Trim();
    Data->query->Parameters->ParamValues["input_time"] = input_time;
    Data->query->Open();
    input_time = Data->query->FieldValues["Input_Time"];
    Data->query->Close();

    sql_stmt = "INSERT INTO Approval_Decision(";
    sql_stmt += "IDN , Input_Time, Decision_Input_Time, Decision,  ";
    sql_stmt += "Approved_amount, Approved_Rate, Approved_Terms, ";
    sql_stmt += "Decline_Reason_Code)";
    sql_stmt += "VALUES(:idn, :input_time, :decision_time, :decision, ";
    sql_stmt += ":amount, rate, terms, ";
    sql_stmt += ":decline_reason)";
    sql_stmt = sql_stmt.UpperCase();
    Data->command->CommandText = sql_stmt;
    Data->command->Parameters->ParamValues["idn"] = medtResultID->Text;
    Data->command->Parameters->ParamValues["input_time"] = input_time;
    Data->command->Parameters->ParamValues["decision_time"] = system_time;
    if(rbApproval->Checked){
      Data->command->Parameters->ParamValues["decision"] = 1;
      Data->command->Parameters->ParamValues["amount"] = medtAmountApproved->Text.Trim().ToInt();
      Data->command->Parameters->ParamValues["rate"] = medtRateApproved->Text.Trim().ToDouble() / 100.0;
      Data->command->Parameters->ParamValues["terms"] = medtTermsApproved->Text.Trim().ToInt();
      Data->command->Parameters->ParamValues["Decline_Reason"] = 0;
    }
    else{
      Data->command->Parameters->ParamValues["Decision"] = 0;
      Data->command->Parameters->ParamValues["amount"] = 0;
      Data->command->Parameters->ParamValues["rate"] = 0;
      Data->command->Parameters->ParamValues["terms"] = 0;
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
} */
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

void __fastcall TfrmMainUI::btnRptGenKeyPress(TObject *Sender, char &Key)
{
 if(Key == 0x0A)
  btnRptGen->Click();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnRptRegenKeyPress(TObject *Sender, char &Key)
{
  if(Key == 0x0A)
    btnRptRegen->Click();
}
//---------------------------------------------------------------------------
/*
void __fastcall TfrmMainUI::btnGetSuggestionKeyPress(TObject *Sender,
      char &Key)
{
  if(Key == 0x0A)
    btnGetSuggestion->Click();
} */
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::FormDestroy(TObject *Sender)
{
  CoUninitialize();  
}
//---------------------------------------------------------------------------
/*
void __fastcall TfrmMainUI::rbApprovalClick(TObject *Sender)
{
  medtResultID->Enabled = true;
  medtRateApproved->Enabled = true;
  medtTermsApproved->Enabled = true;
  medtAmountApproved->Enabled = true;
  edtDeclineReason->Enabled = false;
} */
//---------------------------------------------------------------------------
/*
void __fastcall TfrmMainUI::rbDeclineClick(TObject *Sender)
{
  medtResultID->Enabled = true;
  medtRateApproved->Enabled = true;
  medtTermsApproved->Enabled = true;
  medtAmountApproved->Enabled = true;
  edtDeclineReason->Enabled = false;
} */
//---------------------------------------------------------------------------

bool check_income(const AnsiString &income_str, unsigned int &income)
{
  income = income_str.ToInt() / 12;
  bool return_value = true;
  if(income < 12500)
    return_value = false;
  return return_value;  
};
