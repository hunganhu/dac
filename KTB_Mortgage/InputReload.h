//---------------------------------------------------------------------------
#ifndef VCL_H
#include <vcl.h>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Mask.hpp>
#include <StdCtrls.hpp>
#endif
#include <Classes.hpp>
#include <Controls.hpp>
#include <Mask.hpp>
#include <StdCtrls.hpp>

#ifndef InputReloadH
#define InputReloadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <ADODB.hpp>
#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"

//---------------------------------------------------------------------------
class TfrmReloadInput : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label9;
  TLabel *Label61;
  TLabel *Label62;
  TGroupBox *gbApp;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *Label7;
  TLabel *Label8;
  TLabel *Label10;
  TLabel *Label11;
  TLabel *Label12;
  TEdit *edtAppName;
  TMaskEdit *medtAppID;
  TMaskEdit *medtAppBirthYear;
  TMaskEdit *medtAppBirthMonth;
  TMaskEdit *medtAppBirthDate;
  TComboBox *cmbAppMarriage;
  TComboBox *cmbAppEducation;
  TMaskEdit *medtAppAnnualIncome;
  TComboBox *cmbAppQualified;
  TGroupBox *gbCoApp;
  TLabel *Label13;
  TLabel *Label14;
  TLabel *Label15;
  TLabel *Label16;
  TLabel *Label17;
  TLabel *Label18;
  TLabel *Label19;
  TLabel *Label20;
  TLabel *Label21;
  TLabel *Label22;
  TLabel *Label23;
  TEdit *edtCoAppName;
  TMaskEdit *medtCoAppID;
  TMaskEdit *medtCoAppBirthYear;
  TMaskEdit *medtCoAppBirthMonth;
  TMaskEdit *medtCoAppBirthDate;
  TComboBox *cmbCoAppMarriage;
  TComboBox *cmbCoAppEducation;
  TMaskEdit *medtCoAppAnnualIncome;
  TComboBox *cmbCoAppQualified;
  TGroupBox *gbGuarantor;
  TLabel *Label24;
  TLabel *Label25;
  TLabel *Label26;
  TLabel *Label27;
  TLabel *Label28;
  TLabel *Label29;
  TLabel *Label30;
  TLabel *Label31;
  TLabel *Label32;
  TLabel *Label33;
  TLabel *Label34;
  TEdit *edtGuarantorName;
  TMaskEdit *medtGuarantorID;
  TMaskEdit *medtGuarantorBirthYear;
  TMaskEdit *medtGuarantorBirthMonth;
  TMaskEdit *medtGuarantorBirthDate;
  TComboBox *cmbGuarantorMarriage;
  TComboBox *cmbGuarantorEducation;
  TMaskEdit *medtGuarantorAnnualIncome;
  TComboBox *cmbGuarantorQualified;
  TGroupBox *gbApplication;
  TLabel *Label35;
  TLabel *Label36;
  TLabel *Label37;
  TLabel *Label38;
  TLabel *Label39;
  TLabel *Label40;
  TLabel *Label41;
  TLabel *Label42;
  TLabel *Label43;
  TLabel *Label44;
  TLabel *Label45;
  TLabel *Label46;
  TLabel *Label47;
  TLabel *Label48;
  TLabel *Label49;
  TLabel *Label50;
  TLabel *Label51;
  TLabel *Label52;
  TLabel *Label53;
  TMaskEdit *medtAppAmount;
  TMaskEdit *medtMaturity;
  TMaskEdit *medtGracePeriod;
  TMaskEdit *medtAppFee;
  TComboBox *cmbInterestSections;
  TMaskEdit *medtInterestPeriodOne;
  TMaskEdit *medtInterestOne;
  TMaskEdit *medtInterestPeriodTwo;
  TMaskEdit *medtInterestTwo;
  TMaskEdit *medtInterestPeriodThree;
  TMaskEdit *medtInterestThree;
  TGroupBox *gbCollectral;
  TLabel *Label54;
  TLabel *Label55;
  TLabel *Label56;
  TLabel *Label57;
  TLabel *Label59;
  TLabel *Label60;
  TLabel *Label58;
  TLabel *Label63;
  TLabel *Label64;
  TLabel *Label65;
  TEdit *edtOwnerName;
  TMaskEdit *medtOwnerID;
  TComboBox *cmbRelation;
  TComboBox *cmbCollectralQualified;
  TComboBox *cmbPremiumCollectral;
  TMaskEdit *medtGAV;
  TMaskEdit *medtNAV;
  TMaskEdit *medtBranch;
  TMaskEdit *medtEmployee;
  TMaskEdit *medtUnderwriter;
  TButton *btnScore;
  TButton *btnClear;
  TButton *btnExit;
  TLabel *Label66;
  TMaskEdit *medtMonthlyMortgagePayment;
  TLabel *Label67;
  TCheckBox *ckbCoApp;
  TCheckBox *ckbGuarantor;
  TEdit *edtLandSerial;
  void __fastcall btnExitClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall btnClearClick(TObject *Sender);
  void __fastcall ckbCoAppClick(TObject *Sender);
  void __fastcall ckbGuarantorClick(TObject *Sender);
  void __fastcall cmbInterestSectionsChange(TObject *Sender);
  void __fastcall btnScoreClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmReloadInput(TComponent* Owner);
  unsigned int __fastcall TfrmReloadInput::check_keyin(AnsiString &msg);
  void __fastcall TfrmReloadInput::store_input(TADOCommand *command,
    const AnsiString &case_no, const AnsiString &applicant_inquiry_date,
    const AnsiString &coapplicant_inquiry_date,
    const AnsiString &guarantor_inquiry_date);
  void __fastcall TfrmReloadInput::store_decline_result(TADOCommand *command,
    const AnsiString &case_no, const AnsiString &msg);
  void __fastcall TfrmReloadInput::set_coapp_input(bool enable);
  void __fastcall TfrmReloadInput::set_guarantor_input(bool enable);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReloadInput *frmReloadInput;
//---------------------------------------------------------------------------
#endif







