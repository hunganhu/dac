//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUI.h"
#include "Data_Module.h"
#include "LoginUI.h"
#include "DirUI.h"
#include "ReDirUI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMainUI *frmMainUI;
AnsiString working_dir;
AnsiString report_dir;
AnsiString report_regen_dir;
AnsiString message;
//---------------------------------------------------------------------------
__fastcall TfrmMainUI::TfrmMainUI(TComponent* Owner)
  : TForm(Owner)
{
  CoInitialize(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnExit1Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMainUI::cbSecondaryCardAppliedClick(TObject *Sender)
{
  if(cbSecondaryCardApplied->Checked){
    GroupBox2->Enabled = true;
    Label12->Enabled = true;
    Label2->Enabled = true;
    Label4->Enabled = true;
    Label8->Enabled = true;
    Label5->Enabled = true;
    Label9->Enabled = true;
    Label10->Enabled = true;
  }
  else{
    Label2->Enabled = false;
    Label12->Enabled = false;
    Label4->Enabled = false;
    Label8->Enabled = false;
    Label5->Enabled = false;
    Label9->Enabled = false;
    Label10->Enabled = false;
    GroupBox2->Enabled = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnClearClick(TObject *Sender)
{
  edtPrimaryName->Clear();
  medtPrimaryID->Clear();
  medtPrimaryBirthYear->Clear();
  medtPrimaryBirthMonth->Clear();
  medtPrimaryBirthDate->Clear();
  medtRegion->Clear();
  medtPhone->Clear();
  medtZIP->Clear();
  edtSecondaryName->Clear();
  medtSecondaryID->Clear();
  medtSecondaryBirthYear->Clear();
  medtSecondaryBirthMonth->Clear();
  medtSecondaryBirthYear->Clear();
  medtMonthlyIncome->Clear();
  edtChannel->Clear();
  edtPrimaryName->SetFocus();  
  lblMessage->Caption = "";
  message = "";
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

void __fastcall TfrmMainUI::btnScoreClick(TObject *Sender)
{
  bool is_input_error = false;
  unsigned int gender;
  unsigned int gender_secondary;
  unsigned int monthly_income;
  lblMessage->Caption = "";
  message = "";
  frmMainUI->Refresh();
//Check input
  if(!check_id(medtPrimaryID->Text, gender)){
    message += "主卡申請人身分證號錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!check_phone(medtRegion->Text, medtPhone->Text, rbMobile->Checked)){
    message += "電話號碼錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  if(!check_zip(medtZIP->Text.Trim().c_str())){
	message += "郵遞區號錯誤\n";
	lblMessage->Caption = message;
	is_input_error = true;
  };
  if(!check_branch(Data->query, edtChannel->Text.Trim())){
	message += "進件管道錯誤\n";
	lblMessage->Caption = message;
	is_input_error = true;
  };
  
  if(!check_income(medtMonthlyIncome->Text, monthly_income)){
    message += "月收入錯誤\n";
    lblMessage->Caption = message;
    is_input_error = true;
  };
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;

  get_time(year, month, day, hour, min);



  unsigned int bad_date = check_birthday(medtPrimaryBirthYear->Text,
                                         medtPrimaryBirthMonth->Text,
                                         medtPrimaryBirthDate->Text,
                                         year, month, day, true);
  if (bad_date > 0){
    is_input_error = true;
    if((bad_date & 0x1) == 0x1)
      message += "主卡申請人出生月份錯誤\n";
    if((bad_date & 0x2) == 0x2)
      message += "主卡申請人出生日期錯誤\n";
    if((bad_date & 0x4) == 0x4)
      message += "主卡申請人出生於未來\n";
    if((bad_date & 0x8) == 0x8)
      message += "主卡申請人未滿20歲\n";
    if((bad_date & 0x20) == 0x20)
      message += "主卡申請人出生年份錯誤\n";
    lblMessage->Caption = message;
  };
  frmMainUI->Refresh();

  if(cbSecondaryCardApplied->Checked){
    if(!check_id(medtSecondaryID->Text, gender_secondary)){
      message += "附卡申請人身分證號錯誤\n";
      lblMessage->Caption = message;
      is_input_error = true;
    };
    bad_date = check_birthday(medtSecondaryBirthYear->Text, medtSecondaryBirthMonth->Text,
                              medtSecondaryBirthDate->Text,
                              year, month, day, false);
    if(bad_date > 0){
      is_input_error = true;
      if((bad_date & 0x1) == 0x1)
        message += "附卡申請人出生月份錯誤\n";
      if((bad_date & 0x2) == 0x2)
        message += "附卡申請人出生日期錯誤\n";
      if((bad_date & 0x4) == 0x4)
        message += "附卡申請人出生出生於未來\n";
      if((bad_date & 0x10) == 0x10)
        message += "附卡申請人未滿15歲\n";
      if((bad_date & 0x20) == 0x20)
        message += "附卡申請人出生年份錯誤\n";
      lblMessage->Caption = message;
    };
  };
  frmMainUI->Refresh();

  if(!is_input_error){
    int prescreen_result;
//Format input time
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

    int alien_bride = cbForeignBride->Checked ? 1 : 0;
    int forced_scoring = cbForcedScoring->Checked ? 1 : 0;
    int manager_recommended = cbManagerRecommended->Checked ? 1: 0;
    int secondary_card = cbSecondaryCardApplied->Checked ? 1 : 0;

    try{
      AnsiString birth_year = static_cast<AnsiString>(medtPrimaryBirthYear->Text.Trim().ToInt() + 1911);
      AnsiString birth_month = medtPrimaryBirthMonth->Text.Trim();
      AnsiString birth_date =  medtPrimaryBirthDate->Text.Trim();
      birth_month = birth_month.Length() == 1 ? "0" + birth_month : birth_month;
      birth_date = birth_date.Length() == 1 ? "0" + birth_date : birth_date;
      AnsiString birthday = birth_year + birth_month + birth_date;

      store_input(Data->command, Data->query,
                  medtPrimaryID->Text.Trim(), medtPrimaryID->Text.Trim(), input_time,
                  edtPrimaryName->Text.Trim(),  birthday , monthly_income,
                  medtRegion->Text.Trim() + medtPhone->Text.Trim(), medtZIP->Text.Trim(),
                  alien_bride, manager_recommended, secondary_card, edtChannel->Text.Trim(), true);

      if(cbSecondaryCardApplied->Checked){
         birth_year = static_cast<AnsiString>(medtPrimaryBirthYear->Text.Trim().ToInt() + 1911);
         birth_month = medtPrimaryBirthMonth->Text.Trim();
         birth_date =  medtPrimaryBirthDate->Text.Trim();
         birth_month = birth_month.Length() == 1 ? "0" + birth_month : birth_month;
         birth_date = birth_date.Length() == 1 ? "0" + birth_date : birth_date;
         birthday = birth_year + birth_month + birth_date;
         store_input(Data->command, Data->query,
                      medtPrimaryID->Text.Trim(), medtSecondaryID->Text.Trim(), input_time,
                      edtSecondaryName->Text.Trim(),
                      birthday);
      };

      prescreen_result = prescreen(Data->query, medtPrimaryID->Text, input_time, alien_bride, forced_scoring);

      message += "申請件資料通過檢核，正在取回聯徵資料。\n";
      lblMessage->Caption = message;
      frmMainUI->Refresh();

      AnsiString query_sn;
      AnsiString jcic_inquiry_result;
      AnsiString jcic_inquiry_date;
      AnsiString ole_db = "Provider=SQLOLEDB.1;Password=dac_usr1;Persist Security Info=True;User ID=dac_usr1;Initial Catalog=TNB_CC;Data Source=DAC-DB1";
      char error[256];
      int module_operation;
      bool success;
      int ejcic_error_code;
      switch (prescreen_result){
        case 0:
        success = request_for_jcic_data(Data->ejcic_query, medtPrimaryID->Text.Trim(), query_sn, jcic_inquiry_result, ejcic_error_code);
          if(success){
            message += "聯徵資料已取回，正在儲存資料。\n";
            lblMessage->Caption = message;
            frmMainUI->Refresh();

            jcic_inquiry_date =
              get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn, medtPrimaryID->Text, input_time);
//              record_scoring_time(Data->command, medtPrimaryID->Text,
//                                  input_time, true);
            message += "資料已儲存，正在評分中。\n";
            lblMessage->Caption = message;
            frmMainUI->Refresh();

              module_operation =
                  TNB_CC_AM(medtPrimaryID->Text.c_str(),
                            jcic_inquiry_date.c_str(), input_time.c_str(),
                            "054", ole_db.c_str(), gender, error);
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
          if(cbSecondaryCardApplied->Checked){
            success = request_for_jcic_data(Data->ejcic_query, medtSecondaryID->Text.Trim(), query_sn, jcic_inquiry_result, ejcic_error_code);
            if(success){
              jcic_inquiry_date =
                get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn, medtSecondaryID->Text, input_time);
                module_operation =
                    TNB_CC_AM(medtSecondaryID->Text.c_str(),
                              jcic_inquiry_date.c_str(), input_time.c_str(),
                              "054", ole_db.c_str(), gender_secondary, error);
                if(module_operation != 0){
                  lblMessage->Caption = static_cast<AnsiString>(error);
                  frmMainUI->Refresh();
                };
            }
            else{
  //            log_error(Data->command, medtPrimaryID->Text, jcic_inquiry_result);
              message += ("e JCIC 錯誤，代碼：" + static_cast<AnsiString>(ejcic_error_code) + " 訊息：" + jcic_inquiry_result + "\n");
              lblMessage->Caption = message;
              frmMainUI->Refresh();
              module_operation = 999;
            };
          };
          break;
        case 1:
//        directly approved
          store_result(Data->command, medtPrimaryID->Text, input_time, 1);
          module_operation = 0;
          break;
        case 2:
//         directly decline
          store_result(Data->command, medtPrimaryID->Text, input_time, 2);
          module_operation = 0;
          break;
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


unsigned int check_birthday(const AnsiString &birthyear,
                            const AnsiString &birthmonth,
                            const AnsiString &birthdate,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day,
                            bool primary_card_applicant)
{
//0x0 for normal; 0x1 for month error; 0x2 for date error;
//0x4 for born in the future; 0x8 for primary card applicant under age;
//0x10 for secondary card applicant under age;
//0x20 for missing birth year;
  unsigned int return_value = 0;
  AnsiString birth_year = birthyear.Trim();
  AnsiString birth_month = birthmonth.Trim();
  AnsiString birth_date = birthdate.Trim();


  if(birth_year.Length()==0)
    return_value |= 0x20;
  if(birth_month.Length()==0)
    return_value |= 0x1;
  if(birth_date.Length()==0)
    return_value |= 0x2;
  if(return_value == 0){
    unsigned int year_born = birth_year.Trim().ToInt();
    unsigned int month_born = birth_month.Trim().ToInt();
    unsigned int date_born = birth_date.Trim().ToInt();
    year_born += 1911;

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

bool check_channel(string channel_code)
{
 bool return_value = true;
 string valid_channel[] =  {    // Valid channel code
"0010", "0021", "0032", "0043", "0054", "0065", "0076", "0087", "0098", "0102",
"0113", "0124", "0135", "0146", "0157", "0179", "0180", "0191", "0205", "0216",
"0227", "0238", "0249", "0250", "0261", "0272", "0283", "0294", "0308", "0319",
"0320", "0331", "0342", "0353", "0364", "0375", "0386", "0397", "0401", "0412",
"0434", "0445", "0467", "0489", "0504", "0515", "0526", "0537", "0548", "0559",
"0593", "0607", "0618", "0629", "0641", "0652", "0663", "0674", "0696", "0700",
"0711", "0733", "0744", "0755", "5110", "5120", "5130", "5140", "5150", "5160",
"5170", "5180", "5190", "5310", "5320", "5330", "5340", "5350", "5360", "5370",
"5380", "5390", "5400", "5410", "5510", "5530", "5700", "5810", "5820", "5830",
"5835", "5836", "5837", "5834", "5840", "5850", "5860", "6410", "6420", "7010",
"8010", "8150", "8170", "8180", "9980", "9001", "0777", "0766", "8886", "8887",
"8888", "8881", "8882", "8883", "8884", "8885", "0788"};

 int channelCount = sizeof(valid_channel) / sizeof (valid_channel[0]);  // get no. of channles
 // Declare a set with key only, Initial it with all zip code ascendingly
 set<string, less<string> > channelSet (valid_channel, valid_channel+channelCount+1);
 set<string, less<string> >::iterator iter;

 iter = channelSet.find(channel_code);
 if (iter == channelSet.end())
    return_value = false;

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
              const AnsiString &id, const AnsiString &input_time,
              int alien_bride, int forced_scoring)
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
  else if((forced_scoring == 0) && (customer_list_code == "00"))
    return_value = 1;

  return return_value;
};

void init_UI()
{
  frmMainUI->pcUI->ActivePageIndex = 0;
  frmMainUI->edtPrimaryName->Clear();
  frmMainUI->medtPrimaryID->Clear();
  frmMainUI->medtPrimaryBirthYear->Clear();
  frmMainUI->medtPrimaryBirthMonth->Clear();
  frmMainUI->medtPrimaryBirthDate->Clear();
  frmMainUI->rbFixedLine->Checked = true;;
  frmMainUI->medtRegion->Clear();
  frmMainUI->medtPhone->Clear();
  frmMainUI->medtZIP->Clear();
  frmMainUI->cbForeignBride->Checked = false;
  frmMainUI->cbSecondaryCardApplied->Checked = true;
  frmMainUI->edtSecondaryName->Clear();
  frmMainUI->medtSecondaryID->Clear();
  frmMainUI->medtSecondaryBirthYear->Clear();
  frmMainUI->medtSecondaryBirthMonth->Clear();
  frmMainUI->medtSecondaryBirthDate->Clear();
  frmMainUI->cbSecondaryCardApplied->Checked = false;
  frmMainUI->edtChannel->Clear();
  frmMainUI->medtMonthlyIncome->Clear();
//  frmMainUI->lblMessage->Caption = "";
//  message = "";
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

void store_input(TADOCommand *command, TADOQuery *query,
                 const AnsiString &pri_idn, const AnsiString &sec_idn, const AnsiString &input_time,
                 const AnsiString &cname, const AnsiString &birthday, unsigned int monthly_income,
                 const AnsiString &phone, const AnsiString &zip,
                 int alien_bride, int manager_recommended, int secondary_card, const AnsiString &channel, bool is_primary)
{
  AnsiString sql_stmt;
  if(is_primary){
    query->Close();
    sql_stmt = "SELECT A.Code FROM Black_List AS A INNER JOIN Black_List_Lookup AS B ";
    sql_stmt +="ON A.Code = B.Code WHERE IDN = :idn ORDER BY B.Priority;";
    sql_stmt = sql_stmt.UpperCase();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = pri_idn.Trim().UpperCase();
    query->Open();
    query->First();
    AnsiString black_list_code = "";
    if(query->RecordCount > 0)
      black_list_code = query->FieldValues["Code"];

    query->Close();
    sql_stmt = "SELECT A.Code FROM Customer_List AS A INNER JOIN Customer_List_Lookup AS B ";
    sql_stmt +="ON A.Code = B.Code WHERE IDN = :idn ORDER BY B.Priority DESC;";
    sql_stmt = sql_stmt.UpperCase();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = pri_idn.Trim().UpperCase();
    query->Open();
    query->First();
    AnsiString customer_list_code = "";
    if(query->RecordCount > 0)
      customer_list_code = query->FieldValues["Code"];

    query->Close();
    sql_stmt = "SELECT * FROM VIP_List WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = pri_idn.Trim().UpperCase();
    query->Open();
    query->First();
    int vip_list = 0;
    if(query->RecordCount > 0)
      vip_list = 1;

    sql_stmt = "INSERT INTO Primary_Card_Applicant(IDN, Input_Time, CName, ";
    sql_stmt += "Birthday, Phone, Zip, Alien_Bride, Manager_Recommended, Black_List_Code, ";
    sql_stmt += "Customer_List_Code, VIP_List, Secondary_Card, Channel, Monthly_Income) ";
    sql_stmt += "VALUES(:idn, :input_time, :name, :birthday, :phone, :zip, ";
    sql_stmt += ":alien_bride, :manager_recommended, :black_list, :customer_list, :vip, ";
    sql_stmt += ":second_card, :channel, :monthly_income);";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = pri_idn.Trim().UpperCase();
    command->Parameters->ParamValues["input_time"] = input_time;
    command->Parameters->ParamValues["name"] = cname;
    command->Parameters->ParamValues["birthday"] = birthday;
    command->Parameters->ParamValues["phone"] = phone;
    command->Parameters->ParamValues["zip"] = zip;
    command->Parameters->ParamValues["alien_bride"] = alien_bride;
    command->Parameters->ParamValues["manager_recommended"] = manager_recommended;
    command->Parameters->ParamValues["black_list"] = black_list_code;
    command->Parameters->ParamValues["customer_list"] = customer_list_code;
    command->Parameters->ParamValues["vip"] = vip_list;
    command->Parameters->ParamValues["second_card"] = secondary_card;
    command->Parameters->ParamValues["channel"] = channel;
    command->Parameters->ParamValues["monthly_income"] = monthly_income;
    command->Execute();
  }
  else{
    sql_stmt = "INSERT INTO Secondary_Card_Applicant(IDN, Primary_Applicant_ID, ";
    sql_stmt += "Primary_Input_Time, CName, Birthday) ";
    sql_stmt += "VALUES(:idn, :pri_idn, :input_time, :name, :birthday);";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = sec_idn.Trim().UpperCase();
    command->Parameters->ParamValues["pri_idn"] = pri_idn.Trim().UpperCase();
    command->Parameters->ParamValues["input_time"] = input_time;
    command->Parameters->ParamValues["name"] = cname;
    command->Parameters->ParamValues["birthday"] = birthday;
    command->Execute();
  };
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

/*  sql_stmt = "INSERT INTO KRM021(IDN, INQUIRY_DATE, CARD_BRAND, CARD_TYPE, ISSUE, ";
  sql_stmt += "ISSUE_NAME, START_DATE, STOP_DATE, STOP_CODE, AB_CODE, ";
  sql_stmt += "M_S, LIMIT, INPUT_TIME) VALUES(:idn, ";
  sql_stmt += ":inquiry_date, :card_brand, :card_type, :issue, ";
  sql_stmt += ":issue_name, :start_date, :stop_date, :stop_code, :ab_code, :m_s, :limit, :input_time);";
	command->CommandText = sql_stmt;  */

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
  sql_stmt = sql_stmt.UpperCase();
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
  sql_stmt = sql_stmt.UpperCase();
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


/*  sql_stmt = "INSERT INTO STM007(IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, BANK_NAME, ";
  sql_stmt += "ITEM_LIST, INQ_PURPOSE_1, INQ_PURPOSE, INPUT_TIME) VALUES (:idn, :INQUIRY_DATE, :query_date, ";
  sql_stmt += ":bank_code, :bank_name, :item_list, :inq_purpose_1, :inq_purose, :input_time);";
	command->CommandText = sql_stmt;*/

  sql_stmt = "INSERT INTO STM007(IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, BANK_NAME, ";
  sql_stmt += "ITEM_LIST, INPUT_TIME, INQ_PURPOSE_1, INQ_PURPOSE) VALUES (:idn, :INQUIRY_DATE, :query_date, ";
  sql_stmt += ":bank_code, :bank_name, :item_list, :input_time, :inq_purpose_1, :inq_purpose);";
  sql_stmt = sql_stmt.UpperCase();
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

void __fastcall TfrmMainUI::btnStoreDecisionKeyPress(TObject *Sender,
      char &Key)
{
  if(Key == 0x0A)
    btnStoreDecision->Click();
}
//---------------------------------------------------------------------------
void store_result(TADOCommand *command, const AnsiString &idn, const AnsiString &input_time, int type)
{
  AnsiString sql_stmt;
  sql_stmt = "INSERT INTO Result(IDN, Input_Time, Suggestion, Suggestion_Code) ";
  sql_stmt += "VALUES(:idn, :input_time, :suggestion, :suggestion_code);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn.Trim().UpperCase();
  command->Parameters->ParamValues["input_time"] = input_time;

  switch (type){
   case 1:
	   command->Parameters->ParamValues["suggestion_code"] = 4;
	   command->Parameters->ParamValues["suggestion"] = "直接核准";
     break;
   case 2:
	   command->Parameters->ParamValues["suggestion_code"] = 5;
     command->Parameters->ParamValues["suggestion"] = "直接拒絕";
     break;
  };
  command->Execute();
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
    message += "審核報表產生錯誤。\n";
    lblMessage->Caption = message;

  }
  else{
    message += "審核報表已產生。";
    lblMessage->Caption = message;
  };
  frmMainUI->Refresh();
//  clean_report(Data->command);
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
void clear(char *array, int size)
{
  for(int i=0; i<size; ++i)
    array[i] = ' ';
};
//---------------------------------------------------------------------------
void __fastcall TfrmMainUI::btnRegenRptDirClick(TObject *Sender)
{
   Application->CreateForm(__classid(TfrmReDirUI), &frmReDirUI);
   frmReDirUI->dlbReReport->Directory = report_regen_dir;
   frmReDirUI->edtReDir->Text = report_regen_dir;
   frmReDirUI->ShowModal();
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
  sql_stmt = "CREATE TABLE Report(IDN char(11), Input_Time Char(12), ";
  sql_stmt += "CName Char(20), Secondary_Applicant_ID Char(11), ";
  sql_stmt += "Secondary_Applicant_Name Char(20), Line int, Secondary_Applicant_Line int, ";
  sql_stmt += "Suggestion Char(100), Alien_Bride Char(2), Customer_List_Code Char(2), ";
  sql_stmt += "Black_List_Code Char(2), VIP_List int, Customer_List Char(20), ";
  sql_stmt += "Black_List Char(20), VIP Char(2), Follow_up Char(22), Secondary_Suggestion Char(100), Manager_Recommended Char(2), ";
  sql_stmt += "Monthly_Income INT, Unsecured_Balance INT, Flag_22 CHAR(2), ";
  sql_stmt += "RISK_GROUP INT, PROPENSITY_GROUP INT, PB Decimal(5,4), SPECIAL_NOTE CHAR(2048), DOLLAR_BAD Decimal(5,4), CHANNEL Char(20))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO Report(IDN, Input_Time, CName, Suggestion, Line, Alien_Bride, Manager_Recommended, ";
  sql_stmt += "Customer_List_Code, Black_List_Code, VIP_List, Monthly_Income, Unsecured_Balance, Flag_22, RISK_GROUP, PROPENSITY_GROUP, PB, SPECIAL_NOTE, DOLLAR_BAD, CHANNEL) ";
  sql_stmt += "SELECT A.IDN, A.Input_Time, B.CName, A.Suggestion, A.Suggested_Line, ";
  sql_stmt += "Alien_Bride = (CASE WHEN B.Alien_Bride = 1 THEN '是' ELSE '否' END), ";
  sql_stmt += "Manager_Recommended = (CASE WHEN B.Manager_Recommended = 1 THEN '是' ELSE '否' END), ";
  sql_stmt += "Customer_List_Code, Black_List_Code, VIP_List, Monthly_Income, Unsecured_Balance, ";
  sql_stmt += "(CASE WHEN Unsecured_Balance / (CASE WHEN CONVERT(FLOAT, Monthly_Income) = 0 THEN NULL ELSE CONVERT(FLOAT, Monthly_Income) END) > 22 THEN '是' ELSE '否' END), ";
  sql_stmt += "RISK_GROUP, PROPENSITY_GROUP, PB , SPECIAL_NOTE, DOLLAR_BAD, B.CHANNEL ";
  sql_stmt += "FROM Result AS A INNER JOIN Primary_Card_Applicant AS B ";
  sql_stmt += "ON A.IDN = B.IDN AND A.Input_Time = B.Input_Time ";
  sql_stmt = sql_stmt.UpperCase();
  if(!regen)
    sql_stmt += "WHERE A.Result_File_GEN_Time IS NULL ";
  else
    sql_stmt += "WHERE LEFT(A.Result_File_GEN_Time,10) = :report_gen_time ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  if(regen)
    command->Parameters->ParamValues["report_gen_time"] = report_gen_time;
  command->Execute();

  sql_stmt = "UPDATE Report SET ";
  sql_stmt += "Secondary_Applicant_Name = B.CName, ";
  sql_stmt += "Secondary_Applicant_ID = B.IDN ";
  sql_stmt += "FROM Report AS A INNER JOIN Secondary_Card_Applicant AS B ";
  sql_stmt += "ON B.Primary_Applicant_ID = A.IDN AND B.Primary_Input_Time = A.Input_Time ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE Report SET ";
  sql_stmt += "Secondary_Applicant_Line = B.Suggested_Line, ";
  sql_stmt += "Secondary_Suggestion = B.Suggestion ";
  sql_stmt += "FROM Report AS A INNER JOIN Result As B ";
  sql_stmt += "ON A.Secondary_Applicant_ID = B.IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE Report SET ";
  sql_stmt += "Customer_List = B.Des ";
  sql_stmt += "FROM Report AS A INNER JOIN Customer_List_Lookup As B ";
  sql_stmt += "ON A.Customer_List_Code = B.Code;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE Report SET ";
  sql_stmt += "Black_List = B.Des ";
  sql_stmt += "FROM Report AS A INNER JOIN Black_List_Lookup As B ";
  sql_stmt += "ON A.Black_List_Code = B.Code;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE Report SET ";
  sql_stmt += "VIP = (CASE WHEN VIP_List = 1 THEN '是' ELSE '否' END) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE Report ";
  sql_stmt += "SET Follow_Up = (CASE ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議核准%') THEN '依模組建議' ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議核准%') THEN '依照額度調整準則' ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議核准%') THEN '依照額度調整準則'  ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議核准%') THEN '依照額度調整準則' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議核准%') THEN '依照列名黑名單原因處理' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議核准%') THEN '依照列名黑名單原因處理' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議核准%') THEN '依照列名黑名單原因處理' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議核准%') THEN '依照列名黑名單原因處理'    ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議婉拒%') THEN '婉拒' ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議婉拒%') THEN '依照額度調整準則' ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議婉拒%') THEN '依照額度調整準則' ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議婉拒%') THEN '依照額度調整準則' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議婉拒%') THEN '婉拒' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議婉拒%') THEN '依照列名黑名單原因處理' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議婉拒%') THEN '婉拒' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議婉拒%') THEN '依照列名黑名單原因處理'    ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議人工審核%') THEN '依照現行人工審核流程' ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議人工審核%') THEN '依照現行人工審核流程' ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議人工審核%') THEN '依照現行人工審核流程' ";
  sql_stmt += "WHEN (BLACK_LIST IS NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議人工審核%') THEN '依照現行人工審核流程' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議人工審核%') THEN '婉拒' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議人工審核%') THEN '依照列名黑名單原因處理' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 0) AND (SUGGESTION LIKE '%建議人工審核%') THEN '依照列名黑名單原因處理' ";
  sql_stmt += "WHEN (BLACK_LIST IS NOT NULL) AND (CUSTOMER_LIST IS NOT NULL) AND (VIP_List = 1) AND (SUGGESTION LIKE '%建議人工審核%') THEN '依照列名黑名單原因處理' ";
  sql_stmt += "ELSE SUGGESTION ";
  sql_stmt += " END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
};

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
    sql_stmt = "SELECT COUNT(*) AS CNT FROM Report";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Open();
    if(query->FieldValues["CNT"] == 0){
      if(regen)
        message += "原檔案產生日期錯誤，報表產生中止。\n";
      else
        message += "沒有未產生報表的申請件，報表產生中止。\n";
      frmMainUI->lblMessage->Caption = message;
      frmMainUI->Refresh();
      return true;
    };

    sql_stmt = "SELECT MIN(Input_Time) AS Min_Time, MAX(Input_Time) AS Max_Time ";
    sql_stmt += "FROM Report";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Open();

    AnsiString min_time = query->FieldByName("Min_Time")->AsString;
    AnsiString max_time = query->FieldByName("Max_Time")->AsString;

    sql_stmt = "SELECT * FROM Report ORDER BY Input_Time;";
    sql_stmt = sql_stmt.UpperCase();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Open();

    AnsiString year = report_gen_time.SubString(1,4);
    year = static_cast<AnsiString>(year.ToInt() - 1911);

    AnsiString file_name_total = report_dir + report_gen_time + "_total.csv";
    AnsiString file_name_individual = report_dir + report_gen_time + "_individual.csv";
    AnsiString file_name_total_formating = report_dir + "total.csv";
    AnsiString file_name_individual_formating = report_dir + "individual.csv";

    ofstream report_total(file_name_total.c_str(), ios_base::out);
    ofstream report_individual(file_name_individual.c_str(), ios_base::out);

    ofstream total(file_name_total_formating.c_str(), ios_base::out);
    ofstream individual(file_name_individual_formating.c_str(), ios_base::out);


    if(report_total && report_individual && total && individual){
      report_total << "京城銀行信用卡評分結果名單" << endl;
      report_total << "名單產生時間：" << ",";
      report_total << year.c_str() << "年 ";
      report_total << report_gen_time.SubString(5,2).c_str() << "月 ";
      report_total << report_gen_time.SubString(7,2).c_str() << "日 ";
      report_total << report_gen_time.SubString(9,2).c_str() << "時 ";
      report_total << report_gen_time.SubString(11,2).c_str() << "分 " << endl;

      total << "京城銀行信用卡評分結果名單" << endl;
      total << "名單產生時間：" << ",";
      total << year.c_str() << "年 ";
      total << report_gen_time.SubString(5,2).c_str() << "月 ";
      total << report_gen_time.SubString(7,2).c_str() << "日 ";
      total << report_gen_time.SubString(9,2).c_str() << "時 ";
      total << report_gen_time.SubString(11,2).c_str() << "分 " << endl;

      report_total << "申請件輸入日期時間範圍：" << ",";
      total << "申請件輸入日期時間範圍：" << ",";
      year = min_time.SubString(1,4);
      year = static_cast<AnsiString>(year.ToInt() - 1911);
      report_total << year.c_str() << "年 ";
      report_total << min_time.SubString(5,2).c_str() << "月 ";
      report_total << min_time.SubString(7,2).c_str() << "日 ";
      report_total << min_time.SubString(9,2).c_str() << "時 ";
      report_total << min_time.SubString(11,2).c_str() << "分 ";
      report_total << "," << " _ " << ",";

      total << year.c_str() << "年 ";
      total << min_time.SubString(5,2).c_str() << "月 ";
      total << min_time.SubString(7,2).c_str() << "日 ";
      total << min_time.SubString(9,2).c_str() << "時 ";
      total << min_time.SubString(11,2).c_str() << "分 ";
      total << "," << " _ " << ",";

      year = max_time.SubString(1,4);
      year = static_cast<AnsiString>(year.ToInt() - 1911);
      report_total << year.c_str() << "年 ";
      report_total << max_time.SubString(5,2).c_str() << "月 ";
      report_total << max_time.SubString(7,2).c_str() << "日 ";
      report_total << max_time.SubString(9,2).c_str() << "時 ";
      report_total << max_time.SubString(11,2).c_str() << "分 " << endl;

      total << year.c_str() << "年 ";
      total << max_time.SubString(5,2).c_str() << "月 ";
      total << max_time.SubString(7,2).c_str() << "日 ";
      total << max_time.SubString(9,2).c_str() << "時 ";
      total << max_time.SubString(11,2).c_str() << "分 " << endl;

      report_total << "名單所含件數：" << "," << query->RecordCount;
      report_total << endl;

      total << "名單所含件數：" << "," << query->RecordCount;
      total << endl;

      report_total << "主卡申請人身分證字號" << ",";
      report_total << "主卡申請人姓名" << ",";
      report_total << "附卡申請人身分證字號姓名" << "," ;
      report_total << "附卡申請人姓名" << ",";
      report_total << "申請件輸入日期時間" << ",";
      report_total << "核准建議" << ",";
      report_total << "主卡建議額度(NT$)" << ",";
      report_total << "附卡建議額度(NT$)" << ",";
      report_total << "外籍新娘" << ",";
      report_total << "現有客戶狀況" << ",";
      report_total << "列名黑名單原因" << ",";
      report_total << "VIP" << ",";
      report_total << "主管推薦" << ",";
      report_total << "無擔貸款金額(NT$)" << ",";
      report_total << "月收入(NT$)" << ",";
      report_total << "是否超出22倍" << ",";
      report_total << "壞帳機率" << ",";
      report_total << "風險等級" << ",";
      report_total << "使用傾向等級" << ",";
      report_total << "聯徵補充註記" << ",";
      report_total << "預計壞帳金額比" << ",";
      report_total << "進件管道";
      report_total << endl;

      total << "主卡申請人身分證字號" << ",";
      total << "主卡申請人姓名" << ",";
      total << "附卡申請人身分證字號姓名" << "," ;
      total << "附卡申請人姓名" << ",";
      total << "申請件輸入日期時間" << ",";
      total << "核准建議" << ",";
      total << "主卡建議額度(NT$)" << ",";
      total << "附卡建議額度(NT$)" << ",";
      total << "外籍新娘" << ",";
      total << "現有客戶狀況" << ",";
      total << "列名黑名單原因" << ",";
      total << "VIP" << ",";
      total << "主管推薦" << ",";
      total << "無擔貸款金額(NT$)" << ",";
      total << "月收入(NT$)" << ",";
      total << "是否超出22倍" << ",";
      total << "壞帳機率" << ",";
      total << "風險等級" << ",";
      total << "使用傾向等級" << ",";
      total << "聯徵補充註記" << ",";
      total << "預計壞帳金額比" << ",";
      total << "進件管道";
      total << endl;

      report_individual << "Input_time" <<",";
      report_individual << "Report_time" <<",";
      report_individual << "Primary_id" <<",";
      report_individual << "Primary_name" <<",";
      report_individual << "Alien_bride" <<",";
      report_individual << "VIP" <<",";
      report_individual << "Black_list" <<",";
      report_individual << "Customer_list" <<",";
      report_individual << "Primary_suggestion" <<",";
      report_individual << "Primary_line" <<",";
      report_individual << "Follow_up" <<",";
      report_individual << "Secondary_ID" <<",";
      report_individual << "Secondary_name" <<",";
      report_individual << "Secondary_suggestion" <<",";
      report_individual << "Secondary_line" <<",";
      report_individual << "Manager_Recommended" <<",";
      report_individual << "Unsecured_amount_1" << ",";
      report_individual << "Unsecured_amount_2" << ",";
      report_individual << "PB" << ",";
      report_individual << "Risk_Group" << ",";
      report_individual << "Propensity_Group" << ",";
      report_individual << "Note" << ",";
      report_individual << "Dollar_Bad";
      report_individual << endl;

      individual << "Input_time" <<",";
      individual << "Report_time" <<",";
      individual << "Primary_id" <<",";
      individual << "Primary_name" <<",";
      individual << "Alien_bride" <<",";
      individual << "VIP" <<",";
      individual << "Black_list" <<",";
      individual << "Customer_list" <<",";
      individual << "Primary_suggestion" <<",";
      individual << "Primary_line" <<",";
      individual << "Follow_up" <<",";
      individual << "Secondary_ID" <<",";
      individual << "Secondary_name" <<",";
      individual << "Secondary_suggestion" <<",";
      individual << "Secondary_line" <<",";
      individual << "Manager_Recommended" << ",";
      individual << "Unsecured_amount_1" << ",";
      individual << "Unsecured_amount_2" << ",";
      individual << "PB" << ",";
      individual << "Risk_Group" << ",";
      individual << "Propensity_Group" << ",";
      individual << "Note" << ",";
      individual << "Dollar_Bad";
      individual << endl;

      while(!query->Eof){
        report_total << query->FieldByName("IDN")->AsString.c_str() << ",";
        report_total << query->FieldByName("CName")->AsString.c_str() << ",";
        report_total << query->FieldByName("Secondary_Applicant_ID")->AsString.c_str() << ",";
        report_total << query->FieldByName("Secondary_Applicant_Name")->AsString.c_str() << ",";
        report_total << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        report_total << query->FieldByName("Suggestion")->AsString.c_str() << ",";
        report_total << query->FieldByName("Line")->AsInteger << ",";
        report_total << query->FieldByName("Secondary_Applicant_Line")->AsInteger << ",";
        report_total << query->FieldByName("Alien_Bride")->AsString.c_str() << ",";
        report_total << query->FieldByName("Customer_List")->AsString.c_str() << ",";
        report_total << query->FieldByName("Black_List")->AsString.c_str() << ",";
        report_total << query->FieldByName("VIP_LIST")->AsInteger <<",";
        report_total << query->FieldByName("MANAGER_RECOMMENDED")->AsString.c_str()<<",";
        report_total << query->FieldByName("UNSECURED_BALANCE")->AsInteger <<",";
        report_total << query->FieldByName("MONTHLY_INCOME")->AsString.c_str() <<",";
        report_total << query->FieldByName("FLAG_22")->AsString.c_str()<<",";
        report_total << query->FieldByName("PB")->AsFloat <<",";
        report_total << query->FieldByName("RISK_GROUP")->AsInteger <<",";
        report_total << query->FieldByName("PROPENSITY_GROUP")->AsInteger <<",";
        report_total << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << ",";
        report_total << query->FieldByName("DOLLAR_BAD")->AsFloat << ",";
        report_total << query->FieldByName("CHANNEL")->AsString.c_str();
        report_total << endl;

        total << query->FieldByName("IDN")->AsString.c_str() << ",";
        total << query->FieldByName("CName")->AsString.c_str() << ",";
        total << query->FieldByName("Secondary_Applicant_ID")->AsString.c_str() << ",";
        total << query->FieldByName("Secondary_Applicant_Name")->AsString.c_str() << ",";
        total << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        total << query->FieldByName("Suggestion")->AsString.c_str() << ",";
        total << query->FieldByName("Line")->AsInteger << ",";
        total << query->FieldByName("Secondary_Applicant_Line")->AsInteger << ",";
        total << query->FieldByName("Alien_Bride")->AsString.c_str() << ",";
        total << query->FieldByName("Customer_List")->AsString.c_str() << ",";
        total << query->FieldByName("Black_List")->AsString.c_str() << ",";
        total << query->FieldByName("VIP_LIST")->AsInteger <<",";
        total << query->FieldByName("MANAGER_RECOMMENDED")->AsString.c_str()<<",";
        total << query->FieldByName("UNSECURED_BALANCE")->AsInteger <<",";
        total << query->FieldByName("MONTHLY_INCOME")->AsString.c_str() << ",";
        total << query->FieldByName("FLAG_22")->AsString.c_str()<< ",";
        total << query->FieldByName("PB")->AsFloat <<",";
        total << query->FieldByName("RISK_GROUP")->AsInteger <<",";
        total << query->FieldByName("PROPENSITY_GROUP")->AsInteger <<",";
        total << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << ",";
        total << query->FieldByName("DOLLAR_BAD")->AsFloat << ",";
        total << query->FieldByName("CHANNEL")->AsString.c_str();
        total << endl;

        report_individual << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        report_individual << report_gen_time.c_str() << ",";
        report_individual << query->FieldByName("IDN")->AsString.c_str() << ",";
        report_individual << query->FieldByName("CName")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Alien_Bride")->AsString.c_str() << ",";
        report_individual << query->FieldByName("VIP_LIST")->AsInteger << ",";
        report_individual << query->FieldByName("Black_List")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Customer_List")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Suggestion")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Line")->AsInteger << ",";
        report_individual << query->FieldByName("Follow_Up")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Secondary_Applicant_ID")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Secondary_Applicant_Name")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Secondary_Suggestion")->AsString.c_str() << ",";
        report_individual << query->FieldByName("Secondary_Applicant_Line")->AsInteger <<",";
        report_individual << query->FieldByName("MANAGER_RECOMMENDED")->AsString.c_str()<<",";
        if(query->FieldByName("FLAG_22")->AsString == "是"){
          report_individual << " " <<",";
          report_individual << query->FieldByName("UNSECURED_BALANCE")->AsString.c_str()<<",";
        }
        else{
          report_individual << query->FieldByName("UNSECURED_BALANCE")->AsString.c_str()<<",";
          report_individual << " " << ",";
        };
        report_individual << query->FieldByName("PB")->AsFloat <<",";
        report_individual << query->FieldByName("RISK_GROUP")->AsInteger <<",";
        report_individual << query->FieldByName("PROPENSITY_GROUP")->AsInteger<<",";
        report_individual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << ",";
        report_individual << query->FieldByName("DOLLAR_BAD")->AsFloat;
        report_individual << endl;

        individual << query->FieldByName("Input_Time")->AsString.c_str() << ",";
        individual << report_gen_time.c_str() << ",";
        individual << query->FieldByName("IDN")->AsString.c_str() << ",";
        individual << query->FieldByName("CName")->AsString.c_str() << ",";
        individual << query->FieldByName("Alien_Bride")->AsString.c_str() << ",";
        individual << query->FieldByName("VIP_LIST")->AsInteger << ",";
        individual << query->FieldByName("Black_List")->AsString.c_str() << ",";
        individual << query->FieldByName("Customer_List")->AsString.c_str() << ",";
        individual << query->FieldByName("Suggestion")->AsString.c_str() << ",";
        individual << query->FieldByName("Line")->AsInteger << ",";
        individual << query->FieldByName("Follow_Up")->AsString.c_str() << ",";
        individual << query->FieldByName("Secondary_Applicant_ID")->AsString.c_str() << ",";
        individual << query->FieldByName("Secondary_Applicant_Name")->AsString.c_str() << ",";
        individual << query->FieldByName("Secondary_Suggestion")->AsString.c_str() << ",";
        individual << query->FieldByName("Secondary_Applicant_Line")->AsInteger <<",";
        individual << query->FieldByName("MANAGER_RECOMMENDED")->AsString.c_str()<<",";
        if(query->FieldByName("FLAG_22")->AsString == "是"){
          individual << " " <<",";
          individual << query->FieldByName("UNSECURED_BALANCE")->AsString.c_str() <<",";
        }
        else{
          individual << query->FieldByName("UNSECURED_BALANCE")->AsString.c_str() <<",";
          individual << " "<<",";
        };
        individual << query->FieldByName("PB")->AsFloat <<",";
        individual << query->FieldByName("RISK_GROUP")->AsInteger <<",";
        individual << query->FieldByName("PROPENSITY_GROUP")->AsInteger <<",";
        individual << query->FieldByName("SPECIAL_NOTE")->AsString.c_str() << ",";
        individual << query->FieldByName("DOLLAR_BAD")->AsFloat;
        individual << endl;

        query->Next();
      };
      report_total << "/EOF" << endl;
      total << "/EOF" << endl;
      report_total.clear();
      report_total.close();
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
    frmMainUI->lblMessage->Caption = E.Message;
    frmMainUI->Refresh();
    return false;
  };
};


void __fastcall TfrmMainUI::rbApprovalClick(TObject *Sender)
{
  Label33->Enabled = true;
  Label34->Enabled = true;
  medtApprovedLine->Enabled = true;
  medtApprovedLine->Clear();
  Label35->Enabled = false;
  cmbDeclineReason->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnClearDecisionClick(TObject *Sender)
{
  medtID->Clear();
  medtEntryYear->Clear();
  medtEntryMonth->Clear();
  medtEntryDate->Clear();
  rbApproval->Checked = true;
  medtApprovedLine->Clear();
  rbApproval->Checked = false;
  cmbDeclineReason->ItemIndex = -1;
  rbApproval->Checked = true;
  medtID->SetFocus();
  btnStoreDecision->Enabled = false;
  lblMessage->Caption = "";
  message = "";
  frmMainUI->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::btnGetSuggestionClick(TObject *Sender)
{
  bool is_input_error = false;
  lblMessage->Caption = "";
  message = "";
  frmMainUI->Refresh();
  unsigned int gender; //This variable is not used
  if(!check_id(medtID->Text, gender)){
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
                                         medtEntryDate->Text,
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
    medtEntryDate->SetFocus();
  else if((bad_date & 0x4) == 0x4)
    medtEntryYear->SetFocus();


  if(!is_input_error){
    AnsiString input_year = static_cast<AnsiString>(medtEntryYear->Text.Trim().ToInt() + 1911);
    AnsiString input_month = medtEntryMonth->Text.Trim().Length() == 1 ? "0" + medtEntryMonth->Text.Trim() : medtEntryMonth->Text.Trim();
    AnsiString input_date =  medtEntryDate->Text.Trim().Length() == 1 ? "0" + medtEntryDate->Text.Trim() : medtEntryDate->Text.Trim();
    AnsiString input_time = input_year + input_month + input_date;

    AnsiString sql_stmt = "SELECT * FROM Result WHERE IDN = :idn AND ";
    sql_stmt += "LEFT(Input_Time, 8) = :input_time ORDER BY INPUT_TIME DESC;";
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
        medtEntryDate->Clear();
        medtID->SetFocus();
      }
      else{
        unsigned int suggestion_code = Data->query->FieldValues["Suggestion_Code"];
//Suggestion Code: 1: Module approved "模組建議核准" ; 2: Module declined "模組建議婉拒"; 3: Module manual "模組建議人工審核";
//                 4: Directly approved "直接核准"; 5: Directly declined "直接拒絕"
        switch (suggestion_code){
          case 1:
            rbApproval->Checked = true;
            medtApprovedLine->Enabled = true;
            cmbDeclineReason->Enabled = false;
            medtApprovedLine->Text = static_cast<AnsiString>(Data->query->FieldValues["Suggested_Line"]);
            break;
          case 2:
            rbDecline->Checked = true;
            cmbDeclineReason->Enabled = true;
            cmbDeclineReason->ItemIndex = -1;
            break;
          case 3:
            rbDecline->Checked = false;
            rbApproval->Checked = false;
            medtApprovedLine->Enabled = true;
            cmbDeclineReason->Enabled = true;
            medtApprovedLine->Clear();
            cmbDeclineReason->ItemIndex = -1;
            medtApprovedLine->Enabled = false;
            cmbDeclineReason->Enabled = false;
            break;
          case 4:
            rbApproval->Checked = true;
            medtApprovedLine->Enabled = true;
            cmbDeclineReason->Enabled = false;
            medtApprovedLine->Clear();
            break;
          case 5:
            rbDecline->Checked = true;
            medtApprovedLine->Enabled = false;
            cmbDeclineReason->Enabled = true;
            cmbDeclineReason->ItemIndex = -1;
            break;
        };
        btnStoreDecision->Enabled = true;
      };
      if(rbApproval->Checked)
        medtApprovedLine->SetFocus();
      else if(rbDecline->Checked)
        cmbDeclineReason->SetFocus();
    }
    catch(Exception &E){
     lblMessage->Caption = E.Message;
     frmMainUI->Refresh();
    };
  };
}
//---------------------------------------------------------------------------


void __fastcall TfrmMainUI::medtEntryDateExit(TObject *Sender)
{
  btnGetSuggestionClick(NULL);
};
//---------------------------------------------------------------------------



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
    Data->query->Parameters->ParamValues["idn"] = medtID->Text.Trim();
    Data->query->Parameters->ParamValues["input_time"] = input_time;
    Data->query->Open();
    input_time = Data->query->FieldValues["Input_Time"];
    Data->query->Close();

    sql_stmt = "INSERT INTO Approval_Decision(";
    sql_stmt += "IDN , Input_Time, Decision_Input_Time, Decision,  Line_Granted, ";
    sql_stmt += "Decline_Reason_Code)";
    sql_stmt += "VALUES(:idn, :input_time, :decision_time, :decision, :line, ";
    sql_stmt += ":decline_reason)";
    sql_stmt = sql_stmt.UpperCase();
    Data->command->CommandText = sql_stmt;
    Data->command->Parameters->ParamValues["idn"] = medtID->Text;
    Data->command->Parameters->ParamValues["input_time"] = input_time;
    Data->command->Parameters->ParamValues["decision_time"] = system_time;
    if(rbApproval->Checked){
      Data->command->Parameters->ParamValues["Decision"] = 1;
      Data->command->Parameters->ParamValues["Line"] = medtApprovedLine->Text.Trim().ToInt();
      Data->command->Parameters->ParamValues["Decline_Reason"] = 0;
    }
    else{
      Data->command->Parameters->ParamValues["Decision"] = 0;
      Data->command->Parameters->ParamValues["Line"] = 0;
      Data->command->Parameters->ParamValues["Decline_Reason"] = cmbDeclineReason->ItemIndex + 1;
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
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::tsDecisionEnter(TObject *Sender)
{
  btnClearDecision->Click();
}
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

void __fastcall TfrmMainUI::btnGetSuggestionKeyPress(TObject *Sender,
      char &Key)
{
  if(Key == 0x0A)
    btnGetSuggestion->Click();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMainUI::FormDestroy(TObject *Sender)
{
  CoUninitialize();
}
//---------------------------------------------------------------------------

bool request_for_jcic_data(TADOQuery *query, const AnsiString &id, AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no)
{
  bool return_value = true;

  WideString jcic_inquiry_package_code = "004";
  WideString branch_code = "5330";//"8160":資訊室; For testing machine:GSS
  WideString user_id = "DAC";//"01922":李政達; For testing machine:AUSTIN;
  WideString jcic_inquiry_criteria = id.Trim() + "^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^";
  WideString inquiry_style = "D"; //for data; "H" for html
  WideString AS400 = "N";
  WideString inquiry_reason = "A1A";
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
}

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
void __fastcall TfrmMainUI::rbDeclineClick(TObject *Sender)
{
  Label33->Enabled = false;
  Label34->Enabled = false;
  medtApprovedLine->Enabled = false;
  Label35->Enabled = true;
  cmbDeclineReason->Enabled = true;
  cmbDeclineReason->ItemIndex = -1;
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
};

bool check_income(const AnsiString &income_str, unsigned int &income)
{
  income = income_str.Trim().ToInt() / 12;
  bool return_value = true;
  if(income < 0)
    return_value = false;
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