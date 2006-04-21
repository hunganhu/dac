//---------------------------------------------------------------------------

#ifndef functionsH
#define functionsH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <set>
#include <time.h>
#include <string>
using namespace std;

bool check_id(const AnsiString &idn, unsigned int &gender);
unsigned int check_birthday(const AnsiString &birthyear,
                            const AnsiString &birthmonth,
                            const AnsiString &birthdate,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day,
                            bool primary_card_applicant);
bool validate_date(unsigned int year, unsigned int month, unsigned int day);

int calculate_age(unsigned int year_born, unsigned int month_born, unsigned int date_born,
                  unsigned int year, unsigned int month, unsigned int date);
bool check_zip(string zip);
bool check_channel(string channel_code);
bool check_phone(const AnsiString &region_code, const AnsiString &phone_no, bool celluar);
bool check_region_code(const AnsiString &region_code);
void get_time(unsigned int &year, unsigned int &month, unsigned int &day,
              unsigned int &hour, unsigned int &min);


#endif
 