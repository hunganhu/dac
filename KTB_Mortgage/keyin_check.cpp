#include <time.h>
#include "keyin_check.h"

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
  }
  else
    return result;
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

unsigned int check_birthday(const AnsiString &birthyear,
                            const AnsiString &birthmonth,
                            const AnsiString &birthdate,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day)
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

    if(month_born > 12 || month_born < 1)
      return_value |= 0x1;
    else if(!check_date(year_born, month_born, date_born))
      return_value |= 0x2;
    else{
      int age = calculate_age(year_born, month_born, date_born, year, month, day);
      if(age < 0)
        return_value |= 0x4;
    }
  };
  return return_value;
};

void get_time(unsigned int &year, unsigned int &month, unsigned int &day,
              unsigned int &hour, unsigned int &minute, unsigned int &second)
{
  int counter;
  tm *today;
  time_t ltime;
  ltime = time( NULL );
  today = localtime( &ltime );
  year = today->tm_year + 1900;
  month = today->tm_mon + 1;
  day = today->tm_mday;
  hour = today->tm_hour;
  minute = today->tm_min;
  second = today->tm_sec;
};

bool check_amount(const AnsiString &str_amount)
{
  bool return_value = true;
  unsigned int amount = 0;
  if(!str_amount.Trim().IsEmpty())
    amount = str_amount.Trim().ToInt();
  if(amount == 0)
    return_value = false;
  return return_value;
};

bool check_period(const AnsiString &str_period,
                  const unsigned int lower, const unsigned int upper, bool eq_lower, bool eq_upper)
{
  bool return_value = true;
  try{
    double period = 0;
    if(!str_period.Trim().IsEmpty()){
      period = str_period.Trim().ToDouble();
      if(eq_lower){
        if(period <= lower)
          return_value = false;
      }
      else{
        if(period < lower)
          return_value = false;
      };
      if(eq_upper){
        if(period >= upper)
          return_value = false;
      }
      else{
        if(period > upper)
          return_value = false;
      };
    }
    else
      return_value = false;
  }
  catch(Exception &E){
    return_value = false;
  };
  return return_value;
};

bool check_loan_periods(const AnsiString &str_period1,
                        const AnsiString &str_period2,
                        const AnsiString &str_period3,
                        const AnsiString &str_period_total_yr, int sections)
{
  bool return_value = true;
  unsigned int period1 = str_period1.Trim().IsEmpty() ? 0 : str_period1.Trim().ToInt();
  unsigned int period2 = str_period2.Trim().IsEmpty() ? 0 : str_period2.Trim().ToInt();
  unsigned int period3 = str_period3.Trim().IsEmpty() ? 0 : str_period3.Trim().ToInt();
  unsigned int period_total = str_period_total_yr.Trim().IsEmpty() ? 0 : str_period_total_yr.Trim().ToInt();
  if(period1 == 0)
    return_value = false;
  else if((sections == 2) && (period2 == 0))
    return_value = false;
  else if ((sections == 3) &&((period2 == 0) || (period3 == 0)))
    return_value = false;
  else if(period_total != (period1 + period2 + period3))
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
