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


    int  get_filename (char *line, char *name, char *count);
    int  get_linecount(char *filename);
    void Scan_Read_Buffer (FILE * fid);

  public:
    int  get_control_info();
    int  bulk_insert(TADOHandler *dbhandle);
    ~ControlFile ();
    int ControlFile::check_bulk_insert_status(TADOHandler *dbhandle);
    int ControlFile::check_production_insert_status(TADOHandler *dbhandle);

};

bool validate_date(String date);
char * CurrDateTime ();
int  load_tables(TADOHandler *dbhandle);
#endif
