//---------------------------------------------------------------------------

#ifndef ResultNewMortgageH
#define ResultNewMortgageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
//---------------------------------------------------------------------------
class TfrmRegularResult : public TForm
{
__published:	// IDE-managed Components
  TButton *btnEnd;
  TLabel *Label1;
  TLabel *lblSN;
  TLabel *Label3;
  TLabel *lblExecution;
  TGroupBox *GroupBox1;
  TLabel *Label49;
  TShape *Shape1;
  TLabel *lblSuggestion1;
  TLabel *Label51;
  TLabel *lblAmount;
  TLabel *Label53;
  TLabel *Label54;
  TLabel *lblPeriod1;
  TLabel *Label56;
  TLabel *Label57;
  TLabel *lblRate1;
  TLabel *Label59;
  TLabel *Label60;
  TLabel *lblPeriod2;
  TLabel *Label62;
  TLabel *lblRate2;
  TLabel *Label64;
  TLabel *Label65;
  TLabel *lblPeriod3;
  TLabel *Label67;
  TLabel *lblRate3;
  TLabel *Label69;
  TLabel *lblSuggestion2;
  TButton *btnNext;
  TButton *btnScore;
  void __fastcall btnEndClick(TObject *Sender);
  void __fastcall btnNextClick(TObject *Sender);
  void __fastcall btnScoreClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmRegularResult(TComponent* Owner);
  void __fastcall TfrmRegularResult::fill_result(TADOQuery *query, const AnsiString &case_sn);
  void __fastcall TfrmRegularResult::init(const AnsiString &case_sn);  
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRegularResult *frmRegularResult;
//---------------------------------------------------------------------------
#endif
extern "C" __declspec(dllexport)
 int FM_New(char *case_no, char *ole_db, char *error_message);


