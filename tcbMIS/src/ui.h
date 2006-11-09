//---------------------------------------------------------------------------

#ifndef uiH
#define uiH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <ADODB.hpp>
#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"
using namespace std;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TGroupBox *GroupBox1;
  TLabel *Label3;
  TLabel *Label4;
  TEdit *edtAccount;
  TEdit *edtPassword;
  TButton *Button1;
  TButton *Button2;
  TLabel *Label5;
  TLabel *Label6;
  TEdit *edtBeginDate;
  TEdit *edtEndDate;
  TLabel *lblMessage;
  TLabel *Label10;
  TButton *Button3;
  TLabel *Label9;
  TLabel *Label11;
  TGroupBox *GroupBox2;
  TListBox *lbDimensions;
  TLabel *Label7;
  TLabel *Label8;
  TLabel *lblDimension1;
  TLabel *lblDimension2;
  TButton *btnAddDimension1;
  TButton *btnRemoveDimension1;
  TButton *btnAddDimension2;
  TButton *btnRemoveDimension2;
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall btnAddDimension1Click(TObject *Sender);
  void __fastcall btnAddDimension2Click(TObject *Sender);
  void __fastcall btnRemoveDimension1Click(TObject *Sender);
  void __fastcall btnRemoveDimension2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
void clear(char *array, int size);
bool valid_date(const AnsiString &date);
void write_to_missing_application(TADOQuery *query);
AnsiString get_dimension(const AnsiString &dimension_Chinese, unsigned int &type);
void prepare_report(TADOCommand *command, TADOQuery *query,
                    const AnsiString &begin_date, const AnsiString &end_date,
                    const AnsiString &dimension_one, const AnsiString &dimension_two);
void fill_dimension(const AnsiString &dimension,
                    TADOQuery *query, vector<AnsiString> &dimension_content,
                    unsigned int dimension_type);
bool generate_report(TADOCommand *command, TADOQuery *query,
                     const AnsiString &report_date,
                     const AnsiString &begin_date, const AnsiString &end_date,
                     const AnsiString &dimension_one_name, const AnsiString &dimension_two_name,
                     const AnsiString &dimension_one, const AnsiString &dimension_two,
                     unsigned int dimension_one_type, unsigned int dimension_two_type,
                     const AnsiString &report_path, AnsiString &error);
void generate_type_one_report(TADOCommand *command,
                              const AnsiString &dimension_one,
                              const AnsiString &dimension_two,
                              unsigned int dimension_one_type,
                              unsigned int dimension_two_type,
                              const AnsiString &dimension_one_content,
                              const AnsiString &dimension_two_content);
void write_header(ofstream &report, const AnsiString &report_date,
                  const AnsiString &begin_date, const AnsiString &end_date,
                  const AnsiString &dimension_one,
                  const AnsiString &dimension_two,
                  const AnsiString &dimension_one_content,
                  const AnsiString &dimension_two_content);
void generate_summary_report(TADOCommand *command, const AnsiString &dimension,
                             unsigned int dimension_type,
                             const AnsiString &dimension_content);
void write_part_one(TADOQuery *query, ofstream &report);
void write_part_two(TADOQuery *query, ofstream &report);
void write_part_three(TADOQuery *query, ofstream &report);
void write_part_four(TADOQuery *query, ofstream &report);
void write_part_five(TADOQuery *query, ofstream &report);
void clean_up(TADOCommand *command);

