//---------------------------------------------------------------------------

#ifndef LoginUIH
#define LoginUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <string>
#include <ADODB.hpp>
//---------------------------------------------------------------------------
class TfrmLogin : public TForm
{
__published:	// IDE-managed Components
  TButton *btnLogin;
  TButton *btnCancel;
  TLabel *Label1;
  TEdit *edtUser;
  TLabel *Label2;
  TEdit *edtPassword;
  void __fastcall btnCancelClick(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall btnLoginClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif

using namespace std;
const int Command_time_out = 30;
bool login(TADOQuery *query, const AnsiString &acct, const AnsiString &passwd);
