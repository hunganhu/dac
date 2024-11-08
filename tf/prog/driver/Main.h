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
/*
extern "C" __declspec(dllexport)
int prescreen(char *app_sn, char *jcic_data_date, char *app_data_time,
               char *ole_db, char *error_message, TADOHandler *dbhandle);

extern "C" __declspec(dllexport)
int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message,
                TADOHandler *dbhandle);

extern "C" __declspec(dllexport)
int specific_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message,
                TADOHandler *dbhandle);

extern "C" __declspec(dllexport)
int decision_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, int decision, char *ole_db,
                 char *audit_userno1, char *change_code, char *major_deviation,
                 char *minor_deviation, char *decline_code, char *manual_code,
                 char *error_message, TADOHandler *dbhandle);
*/
extern "C" __declspec(dllexport)
int prescreen(char *app_sn, char *jcic_data_date, char *app_data_time,
               char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
 int optimal_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
int specific_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message);

extern "C" __declspec(dllexport)
int decision_cal(char *app_sn, char *ts_data_date, char *jcic_data_date,
                 char *app_data_time, char *tsn, int decision, char *ole_db,
                 char *audit_userno1, char *change_code, char *major_deviation,
                 char *minor_deviation, char *decline_code, char *manual_code,
                 char *error_message);

extern "C" __declspec(dllexport)
int specific_cal_test(char *app_sn, char *ts_data_date, char *jcic_data_date,
                char *app_data_time, char *tsn, char *ole_db, char *error_message,
                TADOHandler *dbhandle);

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *Specific;
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
        TBitBtn *Optimal;
        TBitBtn *Decision;
        TBitBtn *Prescreen;
        TEdit *Edit5;
        TLabel *Label14;
        TEdit *Edit6;
        TLabel *Label15;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall OptimalClick(TObject *Sender);
        void __fastcall DecisionClick(TObject *Sender);
        void __fastcall PrescreenClick(TObject *Sender);
        void __fastcall SpecificClick(TObject *Sender);
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
