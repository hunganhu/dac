//---------------------------------------------------------------------------

#ifndef InputBTH
#define InputBTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <ADODB.hpp>
//---------------------------------------------------------------------------
class TfrmBTInput : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label9;
  TLabel *Label61;
  TLabel *Label62;
  TGroupBox *gbApp;
  TLabel *Label1;
  TLabel *Label2;
  TEdit *edtAppName;
  TMaskEdit *medtAppID;
  TGroupBox *gbCoApp;
  TLabel *Label13;
  TLabel *Label14;
  TEdit *edtCoAppName;
  TMaskEdit *medtCoAppID;
  TCheckBox *ckbCoApp;
  TGroupBox *gbGuarantor;
  TLabel *Label24;
  TLabel *Label25;
  TEdit *edtGuarantorName;
  TMaskEdit *medtGuarantorID;
  TCheckBox *ckbGuarantor;
  TMaskEdit *medtBranch;
  TMaskEdit *medtEmployee;
  TMaskEdit *medtUnderwriter;
  TButton *btnScore;
  TButton *btnClear;
  TButton *btnExit;
  TLabel *Label3;
  TLabel *Label4;
  void __fastcall btnExitClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall btnClearClick(TObject *Sender);
  void __fastcall ckbCoAppClick(TObject *Sender);
  void __fastcall ckbGuarantorClick(TObject *Sender);
  void __fastcall btnScoreClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmBTInput(TComponent* Owner);
  unsigned int __fastcall TfrmBTInput::check_keyin();
  void __fastcall TfrmBTInput::store_input(TADOCommand *command,
    const AnsiString &case_no, const AnsiString &applicant_inquiry_date,
    const AnsiString &coapplicant_inquiry_date,
    const AnsiString &guarantor_inquiry_date);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBTInput *frmBTInput;
//---------------------------------------------------------------------------
#endif
