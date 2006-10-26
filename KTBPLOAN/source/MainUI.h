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
#include <ExtCtrls.hpp>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <time.h>
#include <dir.h>
#include <ADODB.hpp>
#include <utilcls.h>
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
  TLabel *Label11;
  TGroupBox *GroupBox1;
  TLabel *Label15;
  TLabel *Label1;
  TLabel *Label16;
  TLabel *Label17;
  TLabel *Label18;
  TLabel *Label6;
  TGroupBox *GroupBox2;
        TLabel *Label7;
        TEdit *edtChannel;
  TMaskEdit *medtID;
  TMaskEdit *medtBirthYear;
  TMaskEdit *medtBirthMonth;
        TMaskEdit *medtZIP;
  TMaskEdit *medtBirthDate;
        TButton *btnScore;
        TButton *btnClear;
  TLabel *Label13;
  TLabel *Label12;
  TLabel *Label14;
  TEdit *edtName;
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
  TMaskEdit *medtRptHour;
  TLabel *Label25;
  TLabel *Label28;
  TLabel *Label2;
  TLabel *Label4;
  TMaskEdit *medtAmountApplied;
  TLabel *Label5;
  TLabel *Label39;
  TMaskEdit *medtTerms;
  TLabel *Label44;
  TLabel *Label45;
  TLabel *Label46;
  TMaskEdit *medtInterestRate;
  TLabel *lblMessage;
  TMaskEdit *medtApplicationFee;
  TLabel *Label43;
  TComboBox *cmbProduct;
  TLabel *Label47;
  TLabel *Label50;
  TEdit *edtBranch;
  TLabel *Label3;
  TLabel *Label8;
  TLabel *Label10;
  TLabel *Label24;
  TMaskEdit *medtAnnualIncome;
  TLabel *Label9;
  TComboBox *cmbChildren;
  TComboBox *cmbMarriage;
  TComboBox *cmbEducation;
        TGroupBox *GroupBox5;
        TLabel *Label26;
        TLabel *Label27;
        TMaskEdit *medtFixedPhoneRegion;
        TMaskEdit *medtFixedPhoneNumber;
        TGroupBox *GroupBox6;
        TLabel *Label29;
        TLabel *Label30;
        TMaskEdit *medtCellRegion;
        TMaskEdit *medtCellNumber;
  void __fastcall btnExit1Click(TObject *Sender);
  void __fastcall btnClearClick(TObject *Sender);
  void __fastcall pcUIEnter(TObject *Sender);
  void __fastcall btnExit2Click(TObject *Sender);
  void __fastcall btnExit3Click(TObject *Sender);
  void __fastcall btnScoreClick(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
//  void __fastcall btnStoreDecisionKeyPress(TObject *Sender, char &Key);
  void __fastcall btnRptGenClick(TObject *Sender);
  void __fastcall btnNormalRptDirClick(TObject *Sender);
  void __fastcall btnRegenRptDirClick(TObject *Sender);
  void __fastcall btnRptRegenClick(TObject *Sender);
//  void __fastcall btnClearDecisionClick(TObject *Sender);
//  void __fastcall btnGetSuggestionClick(TObject *Sender);
//  void __fastcall medtEntryDateExit(TObject *Sender);
//  void __fastcall btnStoreDecisionClick(TObject *Sender);
//  void __fastcall tsDecisionEnter(TObject *Sender);
  void __fastcall tsDataEntryEnter(TObject *Sender);
  void __fastcall tsReportGenEnter(TObject *Sender);
  void __fastcall btnScoreKeyPress(TObject *Sender, char &Key);
  void __fastcall btnRptGenKeyPress(TObject *Sender, char &Key);
  void __fastcall btnRptRegenKeyPress(TObject *Sender, char &Key);
//  void __fastcall btnGetSuggestionKeyPress(TObject *Sender, char &Key);
  void __fastcall FormDestroy(TObject *Sender);
//  void __fastcall rbApprovalClick(TObject *Sender);
//  void __fastcall rbDeclineClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmMainUI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMainUI *frmMainUI;
//---------------------------------------------------------------------------
#endif
using namespace std;

bool check_id(const AnsiString &idn, unsigned int &gender);
void get_time(unsigned int &year, unsigned int &month, unsigned int &day,
              unsigned int &hour, unsigned int &min);
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
bool check_occupation(string occupation);
bool check_job(string job);
bool check_amount(const AnsiString &amount);
bool check_terms(const AnsiString &terms);
bool check_rate(const AnsiString &rate);
bool check_region_code(const AnsiString &region_code);
bool check_branch(TADOQuery *query, const AnsiString &branch);
bool check_phone(const AnsiString &region_code, const AnsiString &phone_no, bool celluar);
unsigned int check_job_ym(const AnsiString &job_start_year,
                          const AnsiString &job_start_month,
                          const unsigned int year, const unsigned int month);
int prescreen(TADOQuery *query, const AnsiString &id, AnsiString &code);

void init_UI();
void store_input(TADOCommand *command,
                 const AnsiString &id, const AnsiString &input_time,
                 const AnsiString &name, const AnsiString &birthday ,
                 const AnsiString &fixed_phone, const AnsiString &cell_phone, 
                 const AnsiString &zip,
                 int marriage_code, int children_code, int edu_code,
                 unsigned int request_amount, double request_rate,
                 unsigned int request_terms, unsigned int application_fee,
                 int product_type, unsigned int monthly_income,
                 const AnsiString &channel, const AnsiString &branch,
                 const AnsiString &input_user);

//The following codes are from credit card, for edit to fit into personal loan





bool check_region_code(const AnsiString &region_code);
bool check_phone(const AnsiString &region_code, const AnsiString &phone_no, bool celluar);

AnsiString get_store_jcic_data(TADOConnection *ejcic_connection,
                               TADOQuery *ejcic_query, TADOCommand *command,
                               const AnsiString &query_sn, const AnsiString &idn,
                               const AnsiString &input_time);
void clear(char *array, int size);
void prepare_report(TADOCommand *command, const AnsiString &report_gen_time, bool regen);
unsigned int check_rpt_time(const AnsiString &rpt_year,
                            const AnsiString &rpt_month,
                            const AnsiString &rpt_date,
                            const AnsiString &rpt_hour,
                            const unsigned int year, const unsigned int month,
                            const unsigned int day, const unsigned int hour);
bool generate_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time, bool regen);

bool request_for_jcic_data(TADOQuery *query, const AnsiString &id, AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no);
AnsiString get_ejcic_inquiry_result(TADOQuery *query, const AnsiString &query_sn, AnsiString &result, AnsiString &result_code);
void clean_report(TADOCommand *command);
bool in_future(unsigned int year_rpt, unsigned int month_rpt,
               unsigned int date_rpt, unsigned int hour_rpt,
               unsigned int year, unsigned int month,
               unsigned int day, unsigned int hour);

void record_scoring_time(TADOCommand *command, const AnsiString &idn,
                         const AnsiString &input_time, bool begin);
bool check_income(const AnsiString &income_str, unsigned int &income);
