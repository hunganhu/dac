//---------------------------------------------------------------------------

#ifndef Reminder01H
#define Reminder01H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmSNReminder1 : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TLabel *lblCase_SN;
  TLabel *Label3;
  TButton *btnScoring;
  TLabel *Label4;
  void __fastcall btnScoringClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmSNReminder1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSNReminder1 *frmSNReminder1;
//---------------------------------------------------------------------------
#endif
