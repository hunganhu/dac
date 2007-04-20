//---------------------------------------------------------------------------

#ifndef updateH
#define updateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "AdoHandle.h"
#include <ADODB.hpp>
#include <DB.hpp>
using namespace std;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TButton *Update;
        TButton *End;
        TLabel *L_message;
        TLabel *title;
        TButton *connect;
        TGroupBox *GroupBox1;
        TLabel *Label2;
        TLabel *Label4;
        TLabel *Label7;
        TLabel *Label8;
        TEdit *fund_deposit_pct;
        TEdit *fund_bank_pct;
        TEdit *fund_self_pct;
        TEdit *fund_free_pct;
        TLabel *Label11;
        TLabel *Label14;
        TLabel *Label16;
        TLabel *Label17;
        TGroupBox *GroupBox2;
        TLabel *Label5;
        TLabel *Label19;
        TEdit *cof_deposit;
        TEdit *cof_bank;
        TLabel *Label20;
        TLabel *Label21;
        TGroupBox *GroupBox3;
        TLabel *Label1;
        TLabel *Label9;
        TEdit *roe;
        TEdit *commission;
        TLabel *Label12;
        TLabel *Label15;
        TLabel *message;
        TGroupBox *GroupBox4;
        TLabel *Label3;
        TEdit *npv;
        TLabel *Label6;
        void __fastcall EndClick(TObject *Sender);
        void __fastcall UpdateClick(TObject *Sender);
        void __fastcall connectClick(TObject *Sender);
        void __fastcall disconnectClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
        TADOHandler *dbhandle;
        TADODataSet *ds;

        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
