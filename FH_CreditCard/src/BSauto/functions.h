//---------------------------------------------------------------------------
#ifndef functionsH
#define functionsH

#include <vcl.h>
#include <set>
#include <time.h>
#include "AdoHandle.h"
using namespace std;

//---------------------------------------------------------------------------
#define Is_White(x)    ((x) == ' ' || (x) == '\t' || (x) == '\n')
const int  MAX = 512;
const char ENDBUFFER = -1;
const char ENDFILE = 0;
const char LF = 10;
const char CR = 12;

class ControlFile {
  private:
    char createDate[16];
    char cycleDate[16];
    char statementFile[64];
    char accountFile[64];
    int  statementCount;
    int  accountCount;
    int  statement_read_count;
    int  account_read_count;
    int  statement_temp_count;
    int  account_temp_count;
    int  statement_load_count;
    int  account_load_count;

    int  get_filename (char *line, char *name, char *count);
    int  get_linecount(char *filename);
    void Scan_Read_Buffer (FILE * fid);

  public:
    int  get_control_info();
    int  bulk_insert(TADOHandler *dbhandle);
    char * get_cycledate();
    int  check_bulk_insert_status(TADOHandler *dbhandle);
    int  check_production_insert_status(TADOHandler *dbhandle);

};

bool validate_date(String date);
char *CurrDateTime ();
char *current_directory(char *path);
char *Create_date ();
#endif
