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
                      double gav, double nav, char *zip, double first_lien_value, char *error);

extern "C" __declspec(dllexport)
int DAC_SML_PRESCREEN(char *idn, char *msn, char *time_stamp, char *ole_db,
                      double gav, double nav, char *error);

bool request_for_jcic_data(TADOQuery *query, const AnsiString &id,
             AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no);
AnsiString get_store_jcic_data(TADOConnection *ejcic_connection,
                         TADOQuery *ejcic_query, TADOCommand *command,
                         const AnsiString &query_sn, const AnsiString &msn, const AnsiString &idn,
                         const AnsiString &input_time);
AnsiString get_ejcic_inquiry_result(TADOQuery *query, const AnsiString &query_sn,
                             AnsiString &result, AnsiString &result_code);
void prepare_prelimitary_report(TADOCommand *command, const AnsiString &report_gen_time);
void prepare_final_report(TADOCommand *command, const AnsiString &report_gen_time);
bool generate_prelimitary_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time);
bool generate_final_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time);


//---------------------------------------------------------------------------
class TformMain : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControlMain;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBoxApp;
        TLabel *Label15;
        TLabel *Label1;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label7;
        TLabel *Label14;
        TMaskEdit *medtPrimaryID;
        TMaskEdit *medtPrimaryBirthYear;
        TMaskEdit *medtPrimaryBirthMonth;
        TMaskEdit *medtPrimaryBirthDate;
        TEdit *edtPrimaryName;
        TGroupBox *GroupBoxProd;
        TLabel *Label3;
        TLabel *Label4;
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
        TLabel *Label6;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TComboBox *marriage;
        TComboBox *child;
        TComboBox *education;
        TEdit *edtCareer;
        TLabel *Label2;
        TGroupBox *GroupBox_p1;
        TLabel *Label23;
        TLabel *Label56;
        TLabel *Label58;
        TLabel *Label60;
        TLabel *Label62;
        TEdit *edtOwnerName1;
        TEdit *edtLandNum1;
        TLabel *Label39;
        TComboBox *lien1;
        TComboBox *relationship1;
        TComboBox *location1;
        TLabel *Label46;
        TLabel *Label47;
        TMaskEdit *edtIncome;
        TMaskEdit *edtAppAmount;
        TComboBox *cbPeriod;
        TMaskEdit *edtAppFee;
        TMaskEdit *edtOwnerID1;
        TLabel *Label48;
        TMaskEdit *edtLienValue1;
        TLabel *Label13;
        TLabel *Label49;
        TLabel *Label50;
        TRadioGroup *rgForbit1;
        TRadioGroup *rgCollect1;
        TRadioGroup *rgDemolish1;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label28;
        TRadioGroup *rgHouseRay1;
        TRadioGroup *rgSeasand1;
        TRadioGroup *rgDangerous1;
        TGroupBox *GroupBox_p2;
        TLabel *Label36;
        TLabel *Label37;
        TLabel *Label38;
        TLabel *Label51;
        TLabel *Label52;
        TLabel *Label53;
        TLabel *Label54;
        TLabel *Label57;
        TLabel *Label59;
        TLabel *Label61;
        TLabel *Label63;
        TLabel *Label64;
        TLabel *Label65;
        TLabel *Label66;
        TEdit *edtOwnerName2;
        TEdit *edtLandNum2;
        TComboBox *lien2;
        TComboBox *relationship2;
        TComboBox *location2;
        TMaskEdit *edtOwnerID2;
        TMaskEdit *edtLienValue2;
        TRadioGroup *rgForbit2;
        TRadioGroup *rgCollect2;
        TRadioGroup *rgDemolish2;
        TRadioGroup *rgHouseRay2;
        TRadioGroup *rgSeasand2;
        TRadioGroup *rgDangerous2;
        TLabel *Label67;
        TLabel *Label68;
        TMaskEdit *edtBranch;
        TMaskEdit *edtAgent;
        TCheckBox *cbP2;
        TMaskEdit *edtMSN;
        TGroupBox *GroupBox1;
        TLabel *Label69;
        TLabel *Label75;
        TGroupBox *GroupBox2;
        TLabel *Label83;
        TLabel *Label84;
        TLabel *Label85;
        TLabel *Label86;
        TLabel *Label87;
        TLabel *Label88;
        TLabel *Label89;
        TLabel *Label90;
        TMaskEdit *edtAppAmount2;
        TComboBox *cbPeriod2;
        TMaskEdit *edtAppFee2;
        TGroupBox *GroupBox3;
        TLabel *Label92;
        TLabel *Label93;
        TLabel *Label95;
        TLabel *Label97;
        TLabel *Label98;
        TLabel *Label99;
        TLabel *Label100;
        TLabel *Label101;
        TLabel *Label102;
        TLabel *Label103;
        TMaskEdit *nav1;
        TRadioGroup *rgDamage1;
        TRadioGroup *rgLent1;
        TRadioGroup *rgBasement1;
        TRadioGroup *rgMisuse1;
        TRadioGroup *rgComplex1;
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
        TLabel *Label82;
        TLabel *Label91;
        TLabel *Label94;
        TLabel *Label96;
        TLabel *lblOwnerID2;
        TLabel *lblLandHouseNum2;
        TLabel *Label106;
        TMaskEdit *nav2;
        TRadioGroup *rgDamage2;
        TRadioGroup *rgLent2;
        TRadioGroup *rgBasement2;
        TRadioGroup *rgMisuse2;
        TRadioGroup *rgComplex2;
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
        TEdit *edtAPR;
        TEdit *edtAPR2;
        TLabel *hidden_SystemDate;
        TLabel *hidden_FirstLien1;
        TLabel *hidden_FirstLien2;
        TLabel *Label70;
        TMaskEdit *edtZip;
        TLabel *hidden_Zip;
        TLabel *hidden_InquiryDate;
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
