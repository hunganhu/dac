//---------------------------------------------------------------------------

#ifndef Reminder02H
#define Reminder02H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmSNReminder2 : public TForm
{
__published:	// IDE-managed Components
  TLabel *lblCase_SN;
  TLabel *Label4;
  TLabel *Label3;
  TLabel *Label1;
  TButton *btnScoring;
  void __fastcall btnScoringClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmSNReminder2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSNReminder2 *frmSNReminder2;
//---------------------------------------------------------------------------
#endif
