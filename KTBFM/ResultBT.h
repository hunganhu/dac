//---------------------------------------------------------------------------

#ifndef ResultBTH
#define ResultBTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
//---------------------------------------------------------------------------
class TfrmBTResult : public TForm
{
__published:	// IDE-managed Components
  TButton *btnEnd;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label1;
  TLabel *lblSN;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *lblAppName;
  TLabel *lblAppMsg;
  TLabel *lblCoAppMsg;
  TLabel *lblGuarantorMsg;
  TLabel *lblGuarantorName;
  TLabel *Label8;
  TLabel *Label10;
  TLabel *lblCoAppName;
  TLabel *Label12;
  TButton *btnNext;
  TButton *btnScore;
  TLabel *lblExecution;
  void __fastcall btnNextClick(TObject *Sender);
  void __fastcall btnScoreClick(TObject *Sender);
  void __fastcall btnEndClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmBTResult(TComponent* Owner);
void __fastcall TfrmBTResult::fill_result(TADOQuery *query, const AnsiString &case_sn);
void __fastcall TfrmBTResult::init(const AnsiString &case_sn);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBTResult *frmBTResult;
//---------------------------------------------------------------------------
#endif

extern "C" __declspec(dllexport)
 int FM_Transfer(char *case_no, char *ole_db, char *error_message);
