//---------------------------------------------------------------------------

#ifndef LoginH
#define LoginH
//---------------------------------------------------------------------------
#include "dm.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmLogin : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TEdit *edtUser;
  TEdit *edtPassword;
  TButton *btnLogin;
  TButton *btnExit01;
  void __fastcall btnExit01Click(TObject *Sender);
  void __fastcall btnLoginClick(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif
const int Command_time_out = 300;

bool login(TADOQuery *query, const AnsiString &acct, const AnsiString &passwd);
