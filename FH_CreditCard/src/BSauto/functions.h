//---------------------------------------------------------------------------

#ifndef functionsH
#define functionsH

#include <vcl.h>
#include <set>
#include <time.h>
using namespace std;

//---------------------------------------------------------------------------
#define Is_White(x)    ((x) == ' ' || (x) == '\t' || (x) == '\n')
const int  MAX = 512;
const char ENDBUFFER = -1;
const char ENDFILE = 0;
const char LF = 10;
const char CR = 12;


int get_control_info();
int  get_filename (char *line, char *name, char *count);
int  get_linecount(char *filename);
bool validate_date(String date);
void Scan_Read_Buffer (FILE * fid);
char * CurrDateTime ();

#endif
 