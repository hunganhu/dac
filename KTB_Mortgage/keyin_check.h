#ifndef VCL_H
#include <vcl.h>
#endif

#include <set>
#include <string>

#ifndef CHECK_KEYIN_H
#define CHECK_KEYIN_H
#endif

using namespace std;

bool check_id(const AnsiString &idn, unsigned int &gender);
void get_time(unsigned int &year, unsigned int &month, unsigned int &day,
              unsigned int &hour, unsigned int &minute, unsigned int &second);
bool check_date(unsigned int year, unsigned int month, unsigned int date);
bool check_leap_year(unsigned int year);
int calculate_age(unsigned int year_born, unsigned int month_born, unsigned int date_born,
                  unsigned int year, unsigned int month, unsigned int date);
unsigned int check_birthday(const AnsiString &birthyear,
                            const AnsiString &birthmonth,
                            const AnsiString &birthdate,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day);
bool check_amount(const AnsiString &str_amount);
bool check_period(const AnsiString &str_period,
                  const unsigned int lower, const unsigned int upper,
                  bool eq_lower=false, bool eq_upper=false);
bool check_loan_periods(const AnsiString &str_period1,
                        const AnsiString &str_period2,
                        const AnsiString &str_period3,
                        const AnsiString &str_period_total_yr, int sections);
bool check_channel(string channel_code);
