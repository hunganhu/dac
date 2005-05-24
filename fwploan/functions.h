//---------------------------------------------------------------------------

#ifndef functionsH
#define functionsH

#include <vcl.h>
#include <set>
#include <time.h>
using namespace std;

//---------------------------------------------------------------------------
int validZIP (String zip);  // 0: not found, or the first digit of zip code
int yrmon_to_mon(String inquiry_month, bool time_lock, String lock);
int after_day15(String inquiry_month); // 1: day > 15, 0: day <= 15
bool validate_date(String date);
#endif
 