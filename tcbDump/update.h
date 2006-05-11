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
#include <dir.h>
#include <fstream>
#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TButton *ApprovePay;
        TButton *End;
        TLabel *L_message;
        TLabel *title;
        TButton *connect;
        TLabel *message;
        TButton *ApproveDecline;
        TButton *ApproveUnclose;
        TButton *DeclinePay;
        TButton *ManualPay;
        TButton *All;
        TButton *OutputDir;
        void __fastcall EndClick(TObject *Sender);
        void __fastcall connectClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall OutputDirClick(TObject *Sender);
        void __fastcall ApprovePayClick(TObject *Sender);
        void __fastcall DeclinePayClick(TObject *Sender);
        void __fastcall ManualPayClick(TObject *Sender);
        void __fastcall ApproveUncloseClick(TObject *Sender);
        void __fastcall ApproveDeclineClick(TObject *Sender);
        void __fastcall AllClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
        TADOHandler *dbhandle;
        TADODataSet *ds;

        __fastcall TForm1(TComponent* Owner);
};
bool Generate_ApprovePay_Report(TADOHandler *dbhandle, TADODataSet *ds);
bool Generate_ApproveDecline_Report(TADOHandler *dbhandle, TADODataSet *ds);
bool Generate_ApproveUnclose_Report(TADOHandler *dbhandle, TADODataSet *ds);
bool Generate_DeclinePay_Report(TADOHandler *dbhandle, TADODataSet *ds);
bool Generate_ManualPay_Report(TADOHandler *dbhandle, TADODataSet *ds);
bool Generate_All_Report(TADOHandler *dbhandle, TADODataSet *ds);

//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
