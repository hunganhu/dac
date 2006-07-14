//---------------------------------------------------------------------------

#ifndef ReDirUIH
#define ReDirUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TfrmReDirUI : public TForm
{
__published:	// IDE-managed Components
  TDirectoryListBox *dlbReReport;
  TButton *Button1;
  TButton *btnDirCancel;
  TEdit *edtReDir;
  void __fastcall btnDirCancelClick(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall dlbReReportMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmReDirUI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReDirUI *frmReDirUI;
//---------------------------------------------------------------------------
#endif

extern AnsiString report_regen_dir;