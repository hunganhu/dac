//---------------------------------------------------------------------------

#ifndef dirH
#define dirH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
  TDirectoryListBox *dlbReport;
  TEdit *edtDir;
  TButton *Button1;
  TButton *btnCancel;
  void __fastcall btnCancelClick(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall dlbReportMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
  __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif

