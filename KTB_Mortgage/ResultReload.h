//---------------------------------------------------------------------------

#ifndef ResultReloadH
#define ResultReloadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
//---------------------------------------------------------------------------
class TfrmReloadResult : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label3;
  TLabel *Label1;
  TGroupBox *GroupBox1;
  TLabel *Label49;
  TShape *Shape1;
  TLabel *Label51;
  TLabel *Label53;
  TLabel *Label54;
  TLabel *Label56;
  TLabel *Label57;
  TLabel *Label59;
  TLabel *Label60;
  TLabel *Label62;
  TLabel *Label64;
  TLabel *Label65;
  TLabel *Label67;
  TLabel *Label69;
  TButton *Button1;
  TButton *btnNext;
  TLabel *lblExecution;
  TLabel *lblSN;
  TLabel *lblSuggestion2;
  TLabel *lblSuggestion1;
  TLabel *lblRate3;
  TLabel *lblRate2;
  TLabel *lblRate1;
  TLabel *lblPeriod3;
  TLabel *lblPeriod2;
  TLabel *lblPeriod1;
  TLabel *lblAmount;
  TButton *btnScore;
  void __fastcall btnNextClick(TObject *Sender);
  void __fastcall btnScoreClick(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmReloadResult(TComponent* Owner);
  void __fastcall fill_result(TADOQuery *query, const AnsiString &case_sn);
  void __fastcall TfrmReloadResult::init(const AnsiString &case_sn);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReloadResult *frmReloadResult;
//---------------------------------------------------------------------------
#endif

extern "C" __declspec(dllexport)
 int FM_Reload(char *case_no, char *ole_db, char *error_message);
