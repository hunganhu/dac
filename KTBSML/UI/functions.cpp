//---------------------------------------------------------------------------


#pragma hdrstop

#include "functions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
bool check_valid_id(const AnsiString &idn, unsigned int &gender)
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
    else if(!validate_date(year_born, month_born, date_born))
      return_value |= 0x2;
    else{
      int age = calculate_age(year_born, month_born, date_born, year, month, day);
      if(age < 0)
        return_value |= 0x4;
      if(primary_card_applicant){
        if(age < 20 || age > 65)
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
//---------------------------------------------------------------------------
static unsigned int daytab[2][13] = {
 {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
 {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

//---------------------------------------------------------------------------
/*
 * Function: validate_date(String date)
 * Description: Check input date is a valid date. The year part should be 0001 ~ 9999.
 *              The month part should be 01 ~ 12. The day part should be 01 ~ 31.
 *              The leap year is also checked.
 *
 */
bool validate_date(unsigned int year, unsigned int month, unsigned int day)
{
 int leap;

 leap = (year%4 == 0 && year%100 != 0 || year%400 == 0);
 if (month > 12) return false;
 if (day > daytab[leap][month]) return false;

 return(true);
}
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
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
"8010", "8150", "8170", "8180", "9980", "9001", "0777", "0766"};

 int channelCount = sizeof(valid_channel) / sizeof (valid_channel[0]);  // get no. of channles
 // Declare a set with key only, Initial it with all zip code ascendingly
 set<string, less<string> > channelSet (valid_channel, valid_channel+channelCount+1);
 set<string, less<string> >::iterator iter;

 iter = channelSet.find(channel_code);
 if (iter == channelSet.end())
    return_value = false;

 return return_value;
};
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
void get_time(unsigned int &year, unsigned int &month, unsigned int &day,
              unsigned int &hour, unsigned int &min, unsigned int &sec)
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
    sec = today->tm_sec;
};
//---------------------------------------------------------------------------

