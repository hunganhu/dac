//---------------------------------------------------------------------------

#ifndef LoanTypeSelectionH
#define LoanTypeSelectionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmSelection : public TForm
{
__published:	// IDE-managed Components
  TButton *btnExit02;
  TLabel *Label1;
  TComboBox *cmbProduct;
  TButton *btnReportGen;
  TLabel *Label2;
  void __fastcall btnExit02Click(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall cmbProductChange(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall btnReportGenClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmSelection(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelection *frmSelection;
//---------------------------------------------------------------------------
#endif


