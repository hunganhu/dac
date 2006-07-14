//---------------------------------------------------------------------------

#ifndef ReportGenH
#define ReportGenH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <fstream>
#include <iostream>
#include <dir.h>
#include <ADODB.hpp>
#include <time.h>
#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"
using namespace std;

//---------------------------------------------------------------------------
class TfrmReportGen : public TForm
{
__published:	// IDE-managed Components
  TButton *btnExit2;
  TGroupBox *GroupBox4;
  TLabel *Label19;
  TLabel *Label20;
  TLabel *Label21;
  TLabel *Label22;
  TLabel *Label23;
  TLabel *Label25;
  TButton *btnRegenRptDir;
  TMaskEdit *medtRptYear;
  TMaskEdit *medtRptMonth;
  TMaskEdit *medtRptDate;
  TButton *btnRptRegen;
  TMaskEdit *medtRptHour;
  TGroupBox *GroupBox3;
  TButton *btnNormalRptDir;
  TButton *btnRptGen;
  TLabel *Label13;
  TLabel *lblMessage;
  TButton *btnClose;
  void __fastcall btnCloseClick(TObject *Sender);
  void __fastcall btnExit2Click(TObject *Sender);
  void __fastcall btnNormalRptDirClick(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall btnRegenRptDirClick(TObject *Sender);
  void __fastcall btnRptGenClick(TObject *Sender);
  void __fastcall btnRptRegenClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmReportGen(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReportGen *frmReportGen;
//---------------------------------------------------------------------------
#endif

void clear(char *array, int size);
void prepare_report(TADOCommand *command,
                    const AnsiString &report_gen_time, bool regen);
unsigned int check_rpt_time(const AnsiString &rpt_year,
                            const AnsiString &rpt_month,
                            const AnsiString &rpt_date,
                            const AnsiString &rpt_hour,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day, const unsigned int hour);
bool generate_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time, bool regen);
void clean_report(TADOCommand *command);
bool in_future(unsigned int year_rpt, unsigned int month_rpt,
               unsigned int date_rpt, unsigned int hour_rpt,
               unsigned int year, unsigned int month,
               unsigned int day, unsigned int hour);

              

