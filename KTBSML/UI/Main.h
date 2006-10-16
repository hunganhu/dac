//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include <stdio>
#include <ADODB.hpp>
#include <dir.h>
#include <fstream>
#include <iostream>

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)
int DAC_SML_NPV(char *idn, char *msn, char *time_stamp, char *ole_db,
                double principal, double apr, int period, double app_fee,
                double gav, double nav, char *zip, double first_lien_value,
                int monthly_income, char *error);

extern "C" __declspec(dllexport)
int DAC_SML_PRESCREEN(char *idn, char *msn, char *time_stamp, char *ole_db,
                      int monthly_income, char *error);

//---------------------------------------------------------------------------
class TformMain : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControlMain;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBoxApp;
        TLabel *Label15;
        TLabel *Label1;
        TLabel *Label14;
        TMaskEdit *medtPrimaryID;
        TEdit *edtPrimaryName;
        TGroupBox *GroupBoxProd;
        TLabel *Label3;
        TLabel *Label5;
        TButton *btnPrescreen;
        TButton *btnClear;
        TButton *btnExit1;
        TLabel *lblMessage;
        TTabSheet *TabSheet2;
        TLabel *Label55;
        TButton *Select;
        TButton *finalReview;
        TTabSheet *TabSheet3;
        TGroupBox *GroupBox9;
        TLabel *Label41;
        TLabel *Label42;
        TLabel *Label43;
        TLabel *Label44;
        TButton *btnFinalreviewDir;
        TMaskEdit *final_year;
        TMaskEdit *final_month;
        TMaskEdit *final_day;
        TButton *btnFinalreviewReport;
        TButton *btnExit3;
        TButton *btnExit2;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label20;
        TLabel *Label2;
        TGroupBox *GroupBox_p1;
        TLabel *Label23;
        TLabel *Label56;
        TLabel *Label58;
        TEdit *edtOwnerName1;
        TEdit *edtLandNum1;
        TLabel *Label39;
        TComboBox *relationship1;
        TMaskEdit *edtIncome;
        TMaskEdit *edtAppAmount;
        TComboBox *cbPeriod;
        TMaskEdit *edtOwnerID1;
        TGroupBox *GroupBox_p2;
        TLabel *Label36;
        TLabel *Label37;
        TLabel *Label38;
        TLabel *Label53;
        TEdit *edtOwnerName2;
        TEdit *edtLandNum2;
        TComboBox *relationship2;
        TMaskEdit *edtOwnerID2;
        TCheckBox *cbP2;
        TMaskEdit *edtMSN;
        TGroupBox *GroupBox1;
        TLabel *Label69;
        TLabel *Label75;
        TGroupBox *GroupBox3;
        TLabel *Label92;
        TLabel *Label93;
        TLabel *Label95;
        TLabel *Label97;
        TLabel *Label98;
        TLabel *Label99;
        TMaskEdit *nav1;
        TRadioGroup *rgQualified1;
        TLabel *lblPrimaryID;
        TLabel *lblPrimaryName;
        TLabel *Label72;
        TLabel *lblMSN;
        TMaskEdit *gav1;
        TLabel *lblOwnerID1;
        TLabel *lblLandHouseNum1;
        TLabel *Label74;
        TGroupBox *GroupBox4;
        TLabel *Label76;
        TLabel *Label77;
        TLabel *Label78;
        TLabel *Label79;
        TLabel *Label80;
        TLabel *Label81;
        TLabel *lblOwnerID2;
        TLabel *lblLandHouseNum2;
        TLabel *Label106;
        TMaskEdit *nav2;
        TRadioGroup *rgQualified2;
        TMaskEdit *gav2;
        TGroupBox *GroupBox5;
        TLabel *Label40;
        TLabel *Label45;
        TLabel *Label107;
        TLabel *Label108;
        TButton *btnPreviewDir;
        TMaskEdit *primier_year;
        TMaskEdit *primier_month;
        TMaskEdit *primier_day;
        TButton *btnPreviewReport;
        TButton *finalReview_Clear;
        TLabel *hidden_SystemDate;
        TLabel *hidden_Zip;
        TLabel *Label13;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TMaskEdit *edtLienValue1;
        TMaskEdit *edtLienValue2;
        TLabel *Label4;
        TLabel *hidden_appAmount;
        TLabel *hidden_APR;
        TLabel *hidden_appFee;
        TLabel *hidden_InquiryDate;
        TLabel *hidden_monthly_income;
        TMaskEdit *medtZip;
        TLabel *Label6;
        TEdit *edtAPR;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TMaskEdit *medtAppFee;
        void __fastcall btnExit1Click(TObject *Sender);
        void __fastcall btnClearClick(TObject *Sender);
        void __fastcall btnPrescreenClick(TObject *Sender);
        void __fastcall cbP2Click(TObject *Sender);
        void __fastcall finalReview_ClearClick(TObject *Sender);
        void __fastcall SelectClick(TObject *Sender);
        void __fastcall finalReviewClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnPreviewDirClick(TObject *Sender);
        void __fastcall btnPreviewReportClick(TObject *Sender);
        void __fastcall btnFinalReportClick(TObject *Sender);
        void __fastcall btnFinalreviewDirClick(TObject *Sender);
        void __fastcall medtPrimaryIDExit(TObject *Sender);
private:	// User declarations
public:		// User declarations
        bool validate_application();
        bool validate_property();
        AnsiString get_case_sn(char *header);

        __fastcall TformMain(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TformMain *formMain;
//---------------------------------------------------------------------------
#endif
