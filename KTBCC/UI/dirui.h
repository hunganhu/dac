//---------------------------------------------------------------------------

#ifndef DirUIH
#define DirUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TfrmDirUI : public TForm
{
__published:	// IDE-managed Components
  TButton *btnDirCancel;
  TButton *btnDirSet;
  TEdit *edtDir;
  TDirectoryListBox *dlbReport;
  void __fastcall btnDirCancelClick(TObject *Sender);
  void __fastcall dlbReportMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall btnDirSetClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmDirUI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDirUI *frmDirUI;
//---------------------------------------------------------------------------
#endif

extern AnsiString report_dir;
