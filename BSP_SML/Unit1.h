//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TButton *Button1;
  TButton *Button2;
  TEdit *edtConnectionString;
  TCheckBox *ckbGender;
  TLabel *Label1;
  TLabel *Label2;
  TEdit *edtInqMonth;
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
extern "C" __declspec(dllimport)int DAC_SML_NPV(char *idn, char *idn1, char *idn2,
  char *msn, char *time_stamp, char *ole_db, int gender, int sc, double principal, int test_cell,
  double gav, double nav, char *zip, double first_lien_value, char *error);
//int DAC_SML_NPV(char *idn, char *idn1, char *idn2,
//  char *msn, char *time_stamp, char *ole_db, double principal, int test_cell,
//  double gav, double nav, char *zip, double first_lien_value, char *error);
void capture_error(TADOCommand *command, AnsiString idn, AnsiString error);
void record_data(TADOCommand *command,AnsiString idn,AnsiString error_string);
void connect(TADOConnection *connection, AnsiString connection_string="",
             int connection_timeout = 30,
             int command_timeout = 300);
void initialize_ado_objects(TADOConnection *connection, TADOQuery *query,
                            TADOQuery * query2, TADOCommand *command,
                            int command_timeout = 300);
