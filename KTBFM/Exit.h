//---------------------------------------------------------------------------

#ifndef ExitH
#define ExitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmExitDialog : public TForm
{
__published:	// IDE-managed Components
  TButton *btnExit03;
  TButton *btnResume;
  TLabel *Label1;
  void __fastcall btnExit03Click(TObject *Sender);
  void __fastcall btnResumeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmExitDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmExitDialog *frmExitDialog;
//---------------------------------------------------------------------------
#endif
