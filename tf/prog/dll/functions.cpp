//---------------------------------------------------------------------------
#pragma hdrstop

#include "functions.h"
#include <dateutils.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

/* return code:
   0: not found,
  and the first digit of zip code */
int validZIP (String zipcode)
{
 String valid_TW_Zip_3[] =  {    // Valid Taiwan 3 digit zip codes
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
 set<String, less<String> > zipSet (valid_TW_Zip_3, valid_TW_Zip_3+zipCount+1);
 set<String, less<String> >::iterator iter;

 iter = zipSet.find(zipcode);
 if (iter == zipSet.end())
    return 0;

 return (StrToInt(zipcode)/100);
}
//---------------------------------------------------------------------------

int yrmon_to_mon(String inquiry_month, bool time_lock, String lock)
{
  int year;
  int month;
  int day;
  String current;

  if (inquiry_month == ""){
     tm *today;
     time_t ltime;
     ltime = time( NULL );
     today = localtime( &ltime );
     year = today->tm_year + 1900;
     month = today->tm_mon + 1;
     day = today->tm_mday;
     current = (year * 100 + month) * 100 + day;

     if (time_lock && (lock.Length()==8)) {
       if (current > lock)
        return -1;
     }
  }
  else {
    year = StrToInt(inquiry_month.SubString(1,4));
    month = StrToInt(inquiry_month.SubString(5,2));
    day = StrToInt(inquiry_month.SubString(7,2));
  }
  if (day <= 15)
     month --;
  if (month == 0) {
     month = 12;
     year--;
  }
  return (year - 1911) * 12 + month;
}

static char daytab[2][13] = {
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
bool validate_date(String date)
{
 int year, month, day;
 int leap;

 year = StrToInt(date.SubString(1,4));
 month = StrToInt(date.SubString(5,2));
 day = StrToInt(date.SubString(7,2));

 leap = (year%4 == 0 && year%100 != 0 || year%400 == 0);
 if (month > 12) return false;
 if (day > daytab[leap][month]) return false;

 return(true);
}
//---------------------------------------------------------------------------
/*
 * Function: check_expiration(int lock)
 * Description: Check if current date greater than the lock date.
 */
int check_expiration(int lock)
{
  int year;
  int month;
  int day;
  String current;
  tm *today;
  time_t ltime;

  ltime = time( NULL );
  today = localtime( &ltime );
  year = today->tm_year + 1900;
  month = today->tm_mon + 1;
  day = today->tm_mday;
  current = (year * 100 + month) * 100 + day;

  if (current > lock)
     return (-1);
  else
     return (0);
}

//---------------------------------------------------------------------------
/*
 * Function: Days_between (String begin, String end)
 * Description: Input two date strings, find the number of whole days between
 *              the two dates. The date format is yyyymmdd.
 */
int Days_between (String begin, String end)
{
 TDateTime beginDate, endDate;
 String beginString, endString;

 ShortDateFormat = "yyyymmdd";
 beginString = begin.SubString(1, 4) + "/" +  begin.SubString(5, 2)
               + "/" +  begin.SubString(7, 2);
 endString = end.SubString(1, 4) + "/" +  end.SubString(5, 2)
               + "/" +  end.SubString(7, 2);

 beginDate = StrToDate(beginString);
 endDate = StrToDate(endString);
 return(DaysBetween(endDate, beginDate));
}
//---------------------------------------------------------------------------
/*
 * Function: ExecutionTime ()
 * Description: Return current date and time. The datetime format is yyyymmddHHMi.
 */
char * ExecutionTime()
{
 time_t timer;
 struct tm *tblock;
 static char buf[20];

 timer = time(NULL);
 tblock = localtime(&timer);
 sprintf (buf, "%04d%02d%02d%02d%02d%2d", tblock->tm_year+1900, tblock->tm_mon+1,
          tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
 return (buf);
}
