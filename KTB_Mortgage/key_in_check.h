#ifndef KEYINCHK_H
#define KEYINCHK_H
#endif

bool check_id(const AnsiString &idn, unsigned int &gender);
void get_date(unsigned int &year, unsigned int &month, unsigned int &day);
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
                  const unsigned int lower, const unsigned int upper);
 