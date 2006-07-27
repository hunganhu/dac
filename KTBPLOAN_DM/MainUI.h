//---------------------------------------------------------------------------

#ifndef MainUIH
#define MainUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
//#include <ExtCtrls.hpp>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <time.h>
#include <dir.h>
#include <ADODB.hpp>
//#include <utilcls.h>
#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"
//---------------------------------------------------------------------------
class TfrmMainUI : public TForm
{
__published:	// IDE-managed Components
  TPageControl *pcUI;
  TTabSheet *tsDataEntry;
  TTabSheet *tsReportGen;
        TButton *btnExit1;
        TButton *btnExit2;
  TGroupBox *GroupBox1;
  TLabel *Label15;
  TGroupBox *GroupBox2;
  TMaskEdit *medtID;
        TButton *btnScore;
        TButton *btnClear;
  TLabel *lblMessage;
  TLabel *Label13;
  TLabel *Label12;
  TLabel *Label14;
  TGroupBox *GroupBox3;
  TGroupBox *GroupBox4;
  TButton *btnNormalRptDir;
  TButton *btnRegenRptDir;
  TButton *btnRptGen;
  TLabel *Label19;
  TLabel *Label20;
  TMaskEdit *medtRptYear;
  TLabel *Label21;
  TMaskEdit *medtRptMonth;
  TLabel *Label22;
  TMaskEdit *medtRptDate;
  TLabel *Label23;
  TButton *btnRptRegen;
  TGroupBox *GroupBox5;
  TLabel *Label3;
  TMaskEdit *medtPhone;
  TLabel *Label24;
  TMaskEdit *medtRegion;
  TMaskEdit *medtRptHour;
  TLabel *Label25;
  TLabel *Label28;
  TLabel *Label4;
  TMaskEdit *medtAmountApplied;
  TLabel *Label5;
  TLabel *Label45;
  TGroupBox *GroupBox7;
  TLabel *Label8;
  TLabel *Label9;
  TMaskEdit *medtCellPhone;
  TMaskEdit *medtCellRegion;
  TLabel *Label1;
  TMaskEdit *medtMSN;
  TComboBox *cmbAppliedTerms;
  TEdit *edtName;
  TLabel *Label2;
  TMaskEdit *medtMonthlyIncome;
  TLabel *Label6;
  void __fastcall btnExit1Click(TObject *Sender);
        void __fastcall btnClearClick(TObject *Sender);
  void __fastcall pcUIEnter(TObject *Sender);
  void __fastcall btnExit2Click(TObject *Sender);
  void __fastcall btnExit3Click(TObject *Sender);
  void __fastcall btnScoreClick(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall btnRptGenClick(TObject *Sender);
  void __fastcall medtIDExit(TObject *Sender);
//  void __fastcall btnStoreDecisionKeyPress(TObject *Sender, char &Key);
  void __fastcall btnNormalRptDirClick(TObject *Sender);
  void __fastcall btnRegenRptDirClick(TObject *Sender);
  void __fastcall btnRptRegenClick(TObject *Sender);
//  void __fastcall rbApprovalClick(TObject *Sender);
//  void __fastcall btnClearDecisionClick(TObject *Sender);
//  void __fastcall btnGetSuggestionClick(TObject *Sender);
//  void __fastcall btnStoreDecisionClick(TObject *Sender);
//  void __fastcall tsDecisionEnter(TObject *Sender);
  void __fastcall tsDataEntryEnter(TObject *Sender);
  void __fastcall tsReportGenEnter(TObject *Sender);
  void __fastcall btnScoreKeyPress(TObject *Sender, char &Key);
  void __fastcall btnRptRegenKeyPress(TObject *Sender, char &Key);
  void __fastcall btnRptGenKeyPress(TObject *Sender, char &Key);
  void __fastcall FormDestroy(TObject *Sender);
//  void __fastcall rbDeclineClick(TObject *Sender);
  void __fastcall medtMSNKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmMainUI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMainUI *frmMainUI;
//---------------------------------------------------------------------------
#endif
using namespace std;

bool check_msn(const AnsiString &msn, TADOQuery *query);
bool check_id(const AnsiString &idn, const AnsiString &msn,
              unsigned int &gender, TADOQuery *query);
AnsiString get_applicant_name(const AnsiString &msn, TADOQuery *query);

bool check_phone(const AnsiString &region_code, const AnsiString &phone_no, bool celluar);
bool check_region_code(const AnsiString &region_code);
bool check_income(const AnsiString &income_str, unsigned int &income);
void init_UI();

void get_time(unsigned int &year, unsigned int &month, unsigned int &day,
              unsigned int &hour, unsigned int &min);

void get_product_feature(const AnsiString &product_code, TADOQuery *query, double &apr, unsigned int &application_fee);
void store_input(TADOCommand *command, TADOQuery *query,
                 const AnsiString &msn, const AnsiString &idn,
                 const AnsiString &input_time, const AnsiString &name,
                 unsigned int monthly_income, const AnsiString &product_code, 
                 const AnsiString &phone, const AnsiString &cell_phone,
                 unsigned int application_amount, unsigned int application_terms,
                 double apr, unsigned int application_fee);

AnsiString get_store_jcic_data(TADOConnection *ejcic_connection,
                               TADOQuery *ejcic_query, TADOCommand *command,
                               const AnsiString &query_sn, const AnsiString &idn,
                               const AnsiString &input_time,
                               const AnsiString &msn);



void prepare_report(TADOCommand *command, const AnsiString &report_gen_time, bool regen);
unsigned int check_rpt_time(const AnsiString &rpt_year,
                            const AnsiString &rpt_month,
                            const AnsiString &rpt_date,
                            const AnsiString &rpt_hour,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day, const unsigned int hour);

bool generate_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time, bool regen);

//Functions not used

unsigned int check_birthday(const AnsiString &birth_year,
                            const AnsiString &birth_month,
                            const AnsiString &birth_date,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day,
                            bool primary_card_applicant);
bool check_date(unsigned int year, unsigned int month, unsigned int date);
bool check_leap_year(unsigned int year);
int calculate_age(unsigned int year_born, unsigned int month_born, unsigned int date_born,
                  unsigned int year, unsigned int month, unsigned int date);
bool check_zip(string zip);
int prescreen(TADOQuery *query, const AnsiString &id, AnsiString &code);
//The following codes are from credit card, for edit to fit into personal loan





bool check_region_code(const AnsiString &region_code);
bool check_phone(const AnsiString &region_code, const AnsiString &phone_no, bool celluar);
int prescreen(TADOQuery *query, const AnsiString &id, const AnsiString &input_time, int alien_bride);



void clear(char *array, int size);

bool request_for_jcic_data(TADOQuery *query, const AnsiString &id, AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no);
AnsiString get_ejcic_inquiry_result(TADOQuery *query, const AnsiString &query_sn, AnsiString &result, AnsiString &result_code);
bool check_channel(string channel_code);
void clean_report(TADOCommand *command);
bool in_future(unsigned int year_rpt, unsigned int month_rpt,
               unsigned int date_rpt, unsigned int hour_rpt,
               unsigned int year, unsigned int month,
               unsigned int day, unsigned int hour);

void record_scoring_time(TADOCommand *command, const AnsiString &idn,
                         const AnsiString &input_time, bool begin);
void get_product_code(const AnsiString &msn, AnsiString &product_code, TADOQuery *query);
bool check_and_cleanup_result(const AnsiString &msn, TADOQuery *query);

