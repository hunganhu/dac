//---------------------------------------------------------------------------

#ifndef driverH
#define driverH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TButton *Button1;
  TButton *Button2;
  TLabel *Label1;
  TEdit *edtConnectionString;
  TLabel *Label2;
  TComboBox *cmbProduct;
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

extern "C" __declspec(dllimport)int TNB_CC_AM(char *idn, char *jcic_inquiry_date, char *app_input_time,
              char *bank_code, char *ole_db_str, int gender, char *error);
