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

extern "C" __declspec(dllimport)int BOOC_CC_BT_AM(char *app_sn, char *idn, /*char *jcic_data_date, */
                      int product_type, char *bank_code, char *ole_db_str,
                      char *error);

