//---------------------------------------------------------------------------

#ifndef Reminder03H
#define Reminder03H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmSNReminder3 : public TForm
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
  __fastcall TfrmSNReminder3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSNReminder3 *frmSNReminder3;
//---------------------------------------------------------------------------
#endif
