//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ADODB.hpp>
#include <DB.hpp>

#include "AdoHandle.h"
//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)
 int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_date, char *ole_db, char *return_msg);
extern "C" __declspec(dllexport)
 int optimal_cal_conn(char *app_sn, char *ts_data_date, char *jcic_data_date,
                  char *app_data_date, char *ole_db, char *return_msg, TADOHandler *dbhandle);
/*
extern "C" __declspec(dllexport)
 int dac_ploan_ev(char *case_sn, char *idn, int dac_sn, char *ole_db, char *error_msg);

extern "C" __declspec(dllexport)
 int dac_ploan_ev_conn(char *case_sn, char *idn, int dac_sn, char *ole_db,
                 char *return_msg, TADOHandler *dbhandle);
*/
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *Validate;
        TEdit *Edit1;
        TLabel *Label1;
        TCheckBox *CheckBox1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TADOConnection *ADOConnection1;
        TADOQuery *ADOQuery1;
        TADOQuery *ADOQuery2;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *Edit2;
        TEdit *Edit3;
        TButton *UnitTest;
        TEdit *Edit4;
        TLabel *Label7;
        TLabel *Start_time;
        TLabel *End_time;
        TButton *Button1;
        TLabel *count;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TBitBtn *Validate_NoConn;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ValidateNoConnClick(TObject *Sender);
        void __fastcall ValidateClick(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall UnitTestClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
