//---------------------------------------------------------------------------
#ifndef functionsH
#define functionsH

#include <vcl.h>
#include <set>
#include <time.h>
#include "AdoHandle.h"
using namespace std;

//---------------------------------------------------------------------------
class ControlFile {
  private:
    char createDate[16];        // create date indicated in the control file
    char cycleDate[16];         // cycle date indicated in the control file
    char statementFile[64];     // file name of statement data indicated in the control file
    char accountFile[64];       // file name of account data indicated in the control file
    int  statementCount;        // #rec of statement indicated in the control file
    int  accountCount;          // #rec of account indicated in the control file
    int  statement_read_count;  // #rec read from statement file
    int  account_read_count;    // #rec read from account file
    int  statement_temp_count;  // #rec of statement load to statement temp table
    int  account_temp_count;    // #rec of account load to account temp table
    int  statement_load_count;  // #rec load from statement temp table to production table
    int  account_load_count;    // #rec load from account temp table to production table
    TADODataSet *ds;

    int  get_filename (char *line, char *name, char *count);

  public:
    ControlFile();
    int  get_control_info();
    void  bulk_insert(TADOHandler *dbhandle);
    char * get_cycledate();
    int  check_bulk_insert_status(TADOHandler *dbhandle);
    int  check_production_insert_status(TADOHandler *dbhandle);

};

//---------------------------------------------------------------------------
#define Is_White(x)    ((x) == ' ' || (x) == '\t' || (x) == '\n')
const int  MAX = 512;
const char ENDBUFFER = -1;
const char ENDFILE = 0;
const char LF = 10;
const char CR = 12;

int  get_linecount(char *filename);
void Scan_Read_Buffer (FILE * fid);
bool validate_date(String date);
char *CurrDateTime ();
char *current_directory(char *path);
char *Create_date ();
#endif
