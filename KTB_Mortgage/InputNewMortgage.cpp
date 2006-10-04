//---------------------------------------------------------------------------
#include <time.h>
#include <vcl.h>
#pragma hdrstop

#include "InputNewMortgage.h"
#include "Exit.h"
#include "Reminder01.h"
#include "LoanTypeSelection.h"
#include "dm.h"
#include "cc_err.h"
#include "keyin_check.h"
#include "common.h"

#include <stdlib.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRegularInput *frmRegularInput;
extern AnsiString case_sn;
//---------------------------------------------------------------------------
__fastcall TfrmRegularInput::TfrmRegularInput(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmRegularInput::btnExit04Click(TObject *Sender)
{
   Application->CreateForm(__classid(TfrmExitDialog), &frmExitDialog);
   frmExitDialog->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TfrmRegularInput::FormCreate(TObject *Sender)
{
  btnClear->Click();
}
//---------------------------------------------------------------------------


void __fastcall TfrmRegularInput::FormShow(TObject *Sender)
{
  btnClear->Click();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegularInput::btnClearClick(TObject *Sender)
{
  cmbAppEducation->ItemIndex = -1;
  cmbAppMarriage->ItemIndex = -1;
  cmbAppQualified->ItemIndex = -1;
  edtAppName->Clear();
  medtAppAnnualIncome->Clear();
  medtAppBirthDate->Clear();
  medtAppBirthMonth->Clear();
  medtAppBirthYear->Clear();
  medtAppID->Clear();

  ckbCoApp->Checked = true;
  ckbCoApp->Checked = false;

  ckbGuarantor->Checked = true;
  ckbGuarantor->Checked = false;

  cmbInterestSections->ItemIndex = -1;
  medtAppAmount->Clear();
  medtAppFee->Clear();
  medtGracePeriod->Clear();
  medtMaturity->Clear();

  medtInterestOne->Enabled = true;
  medtInterestPeriodOne->Enabled = true;
  medtInterestTwo->Enabled = true;
  medtInterestPeriodTwo->Enabled = true;
  medtInterestThree->Enabled = true;
  medtInterestPeriodThree->Enabled = true;
  medtInterestOne->Clear();
  medtInterestPeriodOne->Clear();
  medtInterestTwo->Clear();
  medtInterestPeriodTwo->Clear();
  medtInterestThree->Clear();
  medtInterestPeriodThree->Clear();
  medtInterestOne->Enabled = false;
  medtInterestPeriodOne->Enabled = false;
  medtInterestTwo->Enabled = false;
  medtInterestPeriodTwo->Enabled = false;
  medtInterestThree->Enabled = false;
  medtInterestPeriodThree->Enabled = false;
  Label45->Enabled = false;
  Label46->Enabled = false;
  Label47->Enabled = false;
  Label48->Enabled = false;
  Label49->Enabled = false;
  Label50->Enabled = false;
  Label51->Enabled = false;
  Label52->Enabled = false;
  Label53->Enabled = false;
  
  cmbCollectralQualified->ItemIndex = -1;
  cmbPremiumCollectral->ItemIndex = -1;
  cmbRelation->ItemIndex = -1;
  edtLandSerial->Clear();
  edtOwnerName->Clear();
  medtGAV->Clear();
  medtNAV->Clear();
  medtOwnerID->Clear();

  medtBranch->Clear();
  medtEmployee->Clear();
  medtUnderwriter->Clear();
  if(this->Visible == true)
    edtAppName->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegularInput::ckbCoAppClick(TObject *Sender)
{
  if(ckbCoApp->Checked)
    set_coapp_input(true);
  else
    set_coapp_input(false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegularInput::ckbGuarantorClick(TObject *Sender)
{
  if(ckbGuarantor->Checked)
    set_guarantor_input(true);
  else
    set_guarantor_input(false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegularInput::btnScoreClick(TObject *Sender)
{
  AnsiString message;
  AnsiString unqualified_msg = "";
  try{
    AnsiString query_sn_applicant, query_sn_co_applicant, query_sn_guarantor;
    AnsiString jcic_inquiry_result;
    AnsiString applicant_jcic_inquiry_date = "";
    AnsiString coapplicant_jcic_inquiry_date = "";
    AnsiString guarantor_jcic_inquiry_date = "";
    int ejcic_error_code;
    unsigned int availability; //1:applicant; 2:co-applicant; 4:guarantor
    bool success = true;

    availability = check_keyin(unqualified_msg);

    if(availability & 0x1){
      if(!request_for_jcic_data(Data->ejcic_query, medtAppID->Text.Trim(), query_sn_applicant, jcic_inquiry_result, ejcic_error_code)){
        success = false;
        message = ("e JCIC 錯誤，代碼：" + static_cast<AnsiString>(ejcic_error_code) + " 訊息：" + jcic_inquiry_result + "\n");
        MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
      }
    }
    if(availability & 0x2){
      if(!request_for_jcic_data(Data->ejcic_query, medtCoAppID->Text.Trim(), query_sn_co_applicant, jcic_inquiry_result, ejcic_error_code)){
        success = false;
        message = ("e JCIC 錯誤，代碼：" + static_cast<AnsiString>(ejcic_error_code) + " 訊息：" + jcic_inquiry_result + "\n");
        MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
      }
    }
    if(availability & 0x4){
      if(!request_for_jcic_data(Data->ejcic_query, medtGuarantorID->Text.Trim(), query_sn_guarantor, jcic_inquiry_result, ejcic_error_code)){
        success = false;
        message = ("e JCIC 錯誤，代碼：" + static_cast<AnsiString>(ejcic_error_code) + " 訊息：" + jcic_inquiry_result + "\n");
        MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
      }
    };
    if(success){
      case_sn = generate_case_sn(Data->query);
      if(availability & 0x1){
        applicant_jcic_inquiry_date =
          get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn_applicant, medtAppID->Text, case_sn);
      }
      if(availability & 0x2){
        coapplicant_jcic_inquiry_date =
          get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn_co_applicant, medtCoAppID->Text, case_sn);
      }
      if(availability & 0x4){
        guarantor_jcic_inquiry_date =
         get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn_guarantor, medtGuarantorID->Text, case_sn);
      };
      store_input(Data->command, case_sn, applicant_jcic_inquiry_date, coapplicant_jcic_inquiry_date, guarantor_jcic_inquiry_date);
      frmRegularInput->Hide();
      Application->CreateForm(__classid(TfrmSNReminder1), &frmSNReminder1);
      frmSNReminder1->lblCase_SN->Caption = case_sn;
      frmSNReminder1->ShowModal();
    };
  }
  catch(keyin_error &Err){
    message = Err.ShowMessage();
    if(Err.error_code() < 43)
      MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
    else{
      try{
        case_sn = generate_case_sn(Data->query);
        store_input(Data->command, case_sn, "", "", "");
        store_decline_result(Data->command, case_sn, unqualified_msg);
        frmRegularInput->Hide();
        MessageDlg(unqualified_msg, mtInformation, TMsgDlgButtons() << mbOK, 0);
        frmSelection->Show();
      }
      catch(Exception &Err){
        message = Err.Message;
        MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
      }
    }
  }
  catch(Exception &Err){
    message = Err.Message;
    MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
  };
}
//---------------------------------------------------------------------------

unsigned int __fastcall TfrmRegularInput::check_keyin(AnsiString &msg)
{
  unsigned int return_value = 0x1;
  msg = "";
  unsigned int gender;
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int minute;
  unsigned int second;

  get_time(year, month, day, hour, minute, second);

  if(edtAppName->Text.Trim().IsEmpty())
    throw keyin_error(1);
  if(!check_id(medtAppID->Text, gender))
    throw keyin_error(2);
  if(check_birthday(medtAppBirthYear->Text, medtAppBirthMonth->Text,
                    medtAppBirthDate->Text, year, month, day) > 0)
    throw keyin_error(3);
  if(cmbAppMarriage->ItemIndex == -1)
    throw keyin_error(4);
  if(cmbAppEducation->ItemIndex == -1)
    throw keyin_error(5);
  if(medtAppAnnualIncome->Text.Trim().IsEmpty())
    throw keyin_error(6);
  if(cmbAppQualified->ItemIndex == -1)
    throw keyin_error(7);

  if(ckbCoApp->Checked){
    return_value |= 0x2;
    if(edtCoAppName->Text.Trim().IsEmpty())
      throw keyin_error(8);
    if(!check_id(medtCoAppID->Text, gender))
      throw keyin_error(9);
    if(check_birthday(medtCoAppBirthYear->Text, medtCoAppBirthMonth->Text,
                      medtCoAppBirthDate->Text, year, month, day) > 0)
      throw keyin_error(10);
    if(cmbCoAppMarriage->ItemIndex == -1)
      throw keyin_error(11);
    if(cmbCoAppEducation->ItemIndex == -1)
      throw keyin_error(12);
    if(medtCoAppAnnualIncome->Text.Trim().IsEmpty())
      throw keyin_error(13);
    if(cmbCoAppQualified->ItemIndex == -1)
      throw keyin_error(14);
  };
  if(ckbGuarantor->Checked){
    return_value |= 0x4;
    if(edtGuarantorName->Text.Trim().IsEmpty())
      throw keyin_error(15);
    if(!check_id(medtGuarantorID->Text, gender))
      throw keyin_error(16);
    if(check_birthday(medtGuarantorBirthYear->Text, medtGuarantorBirthMonth->Text,
                      medtGuarantorBirthDate->Text, year, month, day) > 0)
      throw keyin_error(17);
    if(cmbGuarantorMarriage->ItemIndex == -1)
      throw keyin_error(18);
    if(cmbGuarantorEducation->ItemIndex == -1)
      throw keyin_error(19);
    if(medtGuarantorAnnualIncome->Text.Trim().IsEmpty())
      throw keyin_error(20);
    if(cmbGuarantorQualified->ItemIndex == -1)
      throw keyin_error(21);
  };
  if(!check_amount(medtAppAmount->Text))
    throw keyin_error(22);
  if(!check_period(medtMaturity->Text, 12, 240))
    throw keyin_error(23);

  unsigned int grace_upper = medtMaturity->Text.Trim().ToInt() - 1 < 36 ? medtMaturity->Text.Trim().ToInt() - 1 : 36;
  if(!check_period(medtGracePeriod->Text, 0, grace_upper, false, false))
    throw keyin_error(24);
  if(medtAppFee->Text.Trim().IsEmpty())
    throw keyin_error(25);
  if(cmbInterestSections->ItemIndex == -1)
    throw keyin_error(26);
  if(!check_loan_periods(medtInterestPeriodOne->Text,
                         medtInterestPeriodTwo->Text,
                         medtInterestPeriodThree->Text,
                         medtMaturity->Text, cmbInterestSections->ItemIndex + 1))
    throw keyin_error(30);
  if(!check_period(medtInterestOne->Text, 0, 10))
    throw keyin_error(27);
  if(cmbInterestSections->ItemIndex > 0){
    if(!check_period(medtInterestTwo->Text, 0, 10))
      throw keyin_error(28);
    if(cmbInterestSections->ItemIndex > 1){
      if(!check_period(medtInterestThree->Text, 0, 10))
        throw keyin_error(29);
    };
  };
  if(edtOwnerName->Text.Trim().IsEmpty())
    throw keyin_error(32);
  if(!check_id(medtOwnerID->Text, gender))
    throw keyin_error(33);
  if(edtLandSerial->Text.Trim().IsEmpty())
    throw keyin_error(34);
  if(cmbRelation->ItemIndex == -1)
    throw keyin_error(35);
  if(medtGAV->Text.Trim().IsEmpty() || medtGAV->Text.Trim().ToInt() == 0)
    throw keyin_error(36);
  if(!check_period(medtNAV->Text, 0, medtGAV->Text.Trim().ToInt()))
    throw keyin_error(37);
  if(cmbCollectralQualified->ItemIndex == -1)
    throw keyin_error(38);
  if(cmbPremiumCollectral->ItemIndex == -1)
    throw keyin_error(39);
  if(!check_channel(medtBranch->Text.c_str()))
    throw keyin_error(41);
  if(medtEmployee->Text.Trim().IsEmpty())
    throw keyin_error(40);
  if(medtUnderwriter->Text.Trim().IsEmpty())
    throw keyin_error(42);
  if(cmbAppQualified->ItemIndex == 1)
    msg += "申請人基本資料不符合承做條件。";
  if(cmbCoAppQualified->ItemIndex == 1)
    msg += "共同申請人基本資料不符合承做條件。";
  if(cmbGuarantorQualified->ItemIndex == 1)
    msg += "保證人基本資料不符合承做條件。";
  if(cmbCollectralQualified->ItemIndex == 1)
    msg += "擔保品不符合承做條件。";
  if(msg.Length() > 0)
    throw keyin_error(43);

  return return_value;
};

void __fastcall TfrmRegularInput::cmbInterestSectionsChange(
      TObject *Sender)
{
  switch(cmbInterestSections->ItemIndex){
    case 0:
      medtInterestOne->Enabled = true;
      medtInterestPeriodOne->Enabled = true;
      medtInterestTwo->Enabled = true;
      medtInterestPeriodTwo->Enabled = true;
      medtInterestThree->Enabled = true;
      medtInterestPeriodThree->Enabled = true;
      medtInterestOne->Clear();
      medtInterestPeriodOne->Clear();
      medtInterestTwo->Clear();
      medtInterestPeriodTwo->Clear();
      medtInterestThree->Clear();
      medtInterestPeriodThree->Clear();
      medtInterestTwo->Enabled = false;
      medtInterestPeriodTwo->Enabled = false;
      medtInterestThree->Enabled = false;
      medtInterestPeriodThree->Enabled = false;
      Label45->Enabled = true;
      Label46->Enabled = true;
      Label47->Enabled = true;
      Label48->Enabled = false;
      Label49->Enabled = false;
      Label50->Enabled = false;
      Label51->Enabled = false;
      Label52->Enabled = false;
      Label53->Enabled = false;
      break;
    case 1:
      medtInterestOne->Enabled = true;
      medtInterestPeriodOne->Enabled = true;
      medtInterestTwo->Enabled = true;
      medtInterestPeriodTwo->Enabled = true;
      medtInterestThree->Enabled = true;
      medtInterestPeriodThree->Enabled = true;
      medtInterestOne->Clear();
      medtInterestPeriodOne->Clear();
      medtInterestTwo->Clear();
      medtInterestPeriodTwo->Clear();
      medtInterestThree->Clear();
      medtInterestPeriodThree->Clear();
      medtInterestThree->Enabled = false;
      medtInterestPeriodThree->Enabled = false;
      Label45->Enabled = true;
      Label46->Enabled = true;
      Label47->Enabled = true;
      Label48->Enabled = true;
      Label49->Enabled = true;
      Label50->Enabled = true;
      Label51->Enabled = false;
      Label52->Enabled = false;
      Label53->Enabled = false;
      break;
    case 2:
      medtInterestOne->Enabled = true;
      medtInterestPeriodOne->Enabled = true;
      medtInterestTwo->Enabled = true;
      medtInterestPeriodTwo->Enabled = true;
      medtInterestThree->Enabled = true;
      medtInterestPeriodThree->Enabled = true;
      medtInterestOne->Clear();
      medtInterestPeriodOne->Clear();
      medtInterestTwo->Clear();
      medtInterestPeriodTwo->Clear();
      medtInterestThree->Clear();
      medtInterestPeriodThree->Clear();
      Label45->Enabled = true;
      Label46->Enabled = true;
      Label47->Enabled = true;
      Label48->Enabled = true;
      Label49->Enabled = true;
      Label50->Enabled = true;
      Label51->Enabled = true;
      Label52->Enabled = true;
      Label53->Enabled = true;
      break;
  }
};

void __fastcall TfrmRegularInput::store_input(TADOCommand *command,
    const AnsiString &case_no, const AnsiString &applicant_inquiry_date,
    const AnsiString &coapplicant_inquiry_date,
    const AnsiString &guarantor_inquiry_date)
{
  AnsiString sql_stmt;
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int minute;
  unsigned int second;
  AnsiString str_month;
  AnsiString str_day;
  AnsiString str_hour;
  AnsiString str_minute;
  AnsiString str_second;
  AnsiString app_date;

  get_time(year, month, day, hour, minute, second);
  str_month = static_cast<AnsiString>(month);
  str_day = static_cast<AnsiString>(day);
  str_hour = static_cast<AnsiString>(hour);
  str_minute = static_cast<AnsiString>(minute);
  str_second = static_cast<AnsiString>(second);

  str_month = fill_in(str_month, 2);
  str_day = fill_in(str_day, 2);
  str_hour = fill_in(str_hour, 2);
  str_minute = fill_in(str_minute, 2);
  str_second = fill_in(str_second, 2);
  app_date = static_cast<AnsiString>(year) + str_month + str_day + str_hour + str_minute + str_second;

  sql_stmt ="INSERT INTO APP_INFO(CASE_NO, APP_DATE, PROD_TYPE, APP_ID, ";
  sql_stmt += "APP_NAME, APP_BIRTHDAY, APP_MARRIAGE, APP_EDUCATION, ";
  sql_stmt += "APP_INCOME, APP_QUALIFIED, APP_INQ_DATE, ";
  if(ckbCoApp->Checked){
    sql_stmt += "COS_ID, COS_NAME, COS_BIRTHDAY, COS_MARRIAGE, COS_EDUCATION, ";
    sql_stmt += "COS_INCOME, COS_QUALIFIED, COS_INQ_DATE, ";
  };
  if(ckbGuarantor->Checked){
    sql_stmt += "GUA_ID, GUA_NAME, GUA_BIRTHDAY, GUA_MARRIAGE, GUA_EDUCATION, ";
    sql_stmt += "GUA_INCOME, GUA_QUALIFIED, GUA_INQ_DATE, ";
  };
  sql_stmt += "APP_AMT, PERIOD, APR1, SEG1, ";
  if(cmbInterestSections->ItemIndex == 1)
    sql_stmt += "APR2, SEG2, ";
  else if(cmbInterestSections->ItemIndex == 2)
    sql_stmt += "APR2, SEG2, APR3, SEG3, ";
  sql_stmt += "GRACE_PERIOD, APP_FEE, OWNER_ID, OWNER_NAME, LAND_NUM, ";
  sql_stmt += "RELATIONSHIP, GAV, NAV, COL_QUALIFIED, PREMIUM_COL, ";
  sql_stmt += "BRANCH, EMP_ID, AUDITOR) VALUES( ";
  sql_stmt += ":case_no, :app_date, :product_type, :app_id, :app_name, ";
  sql_stmt += ":app_birthday, :app_marriage, :app_education, :app_income, ";
  sql_stmt += ":app_qualified, :app_inq_date, ";
  if(ckbCoApp->Checked){
    sql_stmt += ":cos_id, :cos_name, :cos_birthday, :cos_marriage, ";
    sql_stmt += ":cos_education, :cos_income, :cos_qualified, :cos_inq_date, ";
  };
  if(ckbGuarantor->Checked){
    sql_stmt += ":gua_id, :gua_name, :gua_birthday, :gua_marriage, ";
    sql_stmt += ":gua_education, :gua_income, :gua_qualified, :gua_inq_date, ";
  };
  sql_stmt += ":app_amt, :period, :apr1, :seg1, ";
  if(cmbInterestSections->ItemIndex == 1)
    sql_stmt += ":apr2, :seg2, ";
  else if(cmbInterestSections->ItemIndex == 2)
    sql_stmt += ":apr2, :seg2, :apr3, :seg3, ";
  sql_stmt += ":grace_period, :app_fee, :owner_id, :owner_name, :land_num, ";
  sql_stmt += ":relationship, :gav, :nav, :col_qualified, :premium_col, ";
  sql_stmt += ":branch, :emp_id, :auditor)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_no"] = case_no;
  command->Parameters->ParamValues["app_date"] = app_date;
  command->Parameters->ParamValues["product_type"] = 1;
  command->Parameters->ParamValues["app_id"] = medtAppID->Text.Trim();
  command->Parameters->ParamValues["app_name"] = edtAppName->Text.Trim();
  command->Parameters->ParamValues["app_inq_date"] = applicant_inquiry_date;

  AnsiString app_birthday = fill_in(medtAppBirthYear->Text, 3) +
                            fill_in(medtAppBirthMonth->Text, 2) +
                            fill_in(medtAppBirthDate->Text, 2);
  command->Parameters->ParamValues["app_birthday"] = app_birthday;
  command->Parameters->ParamValues["app_marriage"] = cmbAppMarriage->ItemIndex;
  command->Parameters->ParamValues["app_education"] = cmbAppEducation->ItemIndex;
  command->Parameters->ParamValues["app_income"] = medtAppAnnualIncome->Text.Trim().ToInt();
  command->Parameters->ParamValues["app_qualified"] = abs(cmbAppQualified->ItemIndex - 1);
  if(ckbCoApp->Checked){
    command->Parameters->ParamValues["cos_id"] = medtCoAppID->Text.Trim();
    command->Parameters->ParamValues["cos_name"] = edtCoAppName->Text.Trim();

    AnsiString coapp_birthday = fill_in(medtCoAppBirthYear->Text, 3) +
                                fill_in(medtCoAppBirthMonth->Text, 2) +
                                fill_in(medtCoAppBirthDate->Text, 2);
    command->Parameters->ParamValues["cos_birthday"] = coapp_birthday;
    command->Parameters->ParamValues["cos_marriage"] = cmbCoAppMarriage->ItemIndex;
    command->Parameters->ParamValues["cos_education"] = cmbCoAppEducation->ItemIndex;
    command->Parameters->ParamValues["cos_income"] = medtCoAppAnnualIncome->Text.Trim().ToInt();
    command->Parameters->ParamValues["cos_qualified"] = abs(cmbCoAppQualified->ItemIndex - 1);
    command->Parameters->ParamValues["cos_inq_date"] = coapplicant_inquiry_date;
  };
  if(ckbGuarantor->Checked){
    command->Parameters->ParamValues["gua_id"] = medtGuarantorID->Text.Trim().Trim();
    command->Parameters->ParamValues["gua_name"] = edtGuarantorName->Text.Trim().Trim();

    AnsiString guarantor_birthday = fill_in(medtGuarantorBirthYear->Text, 3) +
                                fill_in(medtGuarantorBirthMonth->Text, 2) +
                                fill_in(medtGuarantorBirthDate->Text, 2);
    command->Parameters->ParamValues["gua_birthday"] = guarantor_birthday;
    command->Parameters->ParamValues["gua_marriage"] = cmbGuarantorMarriage->ItemIndex;
    command->Parameters->ParamValues["gua_education"] = cmbGuarantorEducation->ItemIndex;
    command->Parameters->ParamValues["gua_income"] = medtGuarantorAnnualIncome->Text.Trim().ToInt();
    command->Parameters->ParamValues["gua_qualified"] = abs(cmbGuarantorQualified->ItemIndex - 1);
    command->Parameters->ParamValues["gua_inq_date"] = guarantor_inquiry_date;
  };
  command->Parameters->ParamValues["app_amt"] = medtAppAmount->Text.Trim().ToInt() * 10000;
  command->Parameters->ParamValues["period"] = medtMaturity->Text.Trim().ToInt();
  command->Parameters->ParamValues["apr1"] = medtInterestOne->Text.Trim().ToDouble() / 100.0;
  command->Parameters->ParamValues["seg1"] = medtInterestPeriodOne->Text.Trim().ToInt();
  if(cmbInterestSections->ItemIndex == 1){
    command->Parameters->ParamValues["apr2"] = medtInterestTwo->Text.Trim().ToDouble() / 100.0;
    command->Parameters->ParamValues["seg2"] = medtInterestPeriodTwo->Text.Trim().ToInt();
  }
  else if(cmbInterestSections->ItemIndex == 2){
    command->Parameters->ParamValues["apr2"] = medtInterestTwo->Text.Trim().ToDouble() / 100.0;
    command->Parameters->ParamValues["seg2"] = medtInterestPeriodTwo->Text.Trim().ToInt();
    command->Parameters->ParamValues["apr3"] = medtInterestThree->Text.Trim().ToDouble() / 100.0;
    command->Parameters->ParamValues["seg3"] = medtInterestPeriodThree->Text.Trim().ToInt();
  };
  command->Parameters->ParamValues["grace_period"] = medtGracePeriod->Text.Trim().ToInt();
  command->Parameters->ParamValues["app_fee"] = medtAppFee->Text.Trim().ToInt();
  command->Parameters->ParamValues["owner_id"] = medtOwnerID->Text.Trim();
  command->Parameters->ParamValues["owner_name"] = edtOwnerName->Text.Trim();
  command->Parameters->ParamValues["land_num"] = edtLandSerial->Text.Trim();
  command->Parameters->ParamValues["relationship"] = cmbRelation->ItemIndex;
  command->Parameters->ParamValues["nav"] = medtNAV->Text.Trim().ToInt() * 10000;
  command->Parameters->ParamValues["gav"] = medtGAV->Text.Trim().ToInt() * 10000;
  command->Parameters->ParamValues["col_qualified"] = abs(cmbCollectralQualified->ItemIndex - 1);
  command->Parameters->ParamValues["premium_col"] = abs(cmbPremiumCollectral->ItemIndex - 1);
  command->Parameters->ParamValues["branch"] = medtBranch->Text.Trim();
  command->Parameters->ParamValues["emp_id"] = medtEmployee->Text.Trim();
  command->Parameters->ParamValues["auditor"] = medtUnderwriter->Text.Trim();
  command->Execute();
};

void __fastcall TfrmRegularInput::store_decline_result(TADOCommand *command,
    const AnsiString &case_no, const AnsiString &msg)
{
  AnsiString sql_stmt;
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int minute;
  unsigned int second;
  AnsiString str_month;
  AnsiString str_day;
  AnsiString str_hour;
  AnsiString str_minute;
  AnsiString str_second;
  AnsiString decision_date;

  get_time(year, month, day, hour, minute, second);
  str_month = static_cast<AnsiString>(month);
  str_day = static_cast<AnsiString>(day);
  str_hour = static_cast<AnsiString>(hour);
  str_minute = static_cast<AnsiString>(minute);
  str_second = static_cast<AnsiString>(second);

  str_month = fill_in(str_month, 2);
  str_day = fill_in(str_day, 2);
  str_hour = fill_in(str_hour, 2);
  str_minute = fill_in(str_minute, 2);
  str_second = fill_in(str_second, 2);
  decision_date = static_cast<AnsiString>(year) + str_month + str_day + str_hour + str_minute + str_second;

  sql_stmt = "INSERT INTO APP_RESULT(CASE_NO, FINAL_DATE, SUGG_CODE, SUGG_MSG) ";
  sql_stmt += "VALUES(:case_no, :final_date, :sugg_code, :sugg_msg)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_no"] = case_no;
  command->Parameters->ParamValues["final_date"] = decision_date;
  command->Parameters->ParamValues["sugg_code"] = 2;
  command->Parameters->ParamValues["sugg_msg"] = msg;
  command->Execute();
};
//---------------------------------------------------------------------------
void __fastcall TfrmRegularInput::set_coapp_input(bool enable)
{
  if(!enable){
    cmbCoAppEducation->ItemIndex = -1;
    cmbCoAppMarriage->ItemIndex = -1;
    cmbCoAppQualified->ItemIndex = -1;
    edtCoAppName->Clear();
    medtCoAppAnnualIncome->Clear();
    medtCoAppBirthDate->Clear();
    medtCoAppBirthMonth->Clear();
    medtCoAppBirthYear->Clear();
    medtCoAppID->Clear();
  }
  else
    gbCoApp->Enabled = enable;
  cmbCoAppEducation->Enabled = enable;
  cmbCoAppMarriage->Enabled = enable;
  cmbCoAppQualified->Enabled = enable;
  edtCoAppName->Enabled = enable;
  medtCoAppAnnualIncome->Enabled = enable;
  medtCoAppBirthDate->Enabled = enable;
  medtCoAppBirthMonth->Enabled = enable;
  medtCoAppBirthYear->Enabled = enable;
  medtCoAppID->Enabled = enable;
  Label13->Enabled = enable;
  Label14->Enabled = enable;
  Label15->Enabled = enable;
  Label16->Enabled = enable;
  Label17->Enabled = enable;
  Label18->Enabled = enable;
  Label19->Enabled = enable;
  Label20->Enabled = enable;
  Label21->Enabled = enable;
  Label22->Enabled = enable;
  Label23->Enabled = enable;
  gbCoApp->Enabled = enable;
};

void __fastcall TfrmRegularInput::set_guarantor_input(bool enable)
{
  if(!enable){
    cmbGuarantorEducation->ItemIndex = -1;
    cmbGuarantorMarriage->ItemIndex = -1;
    cmbGuarantorQualified->ItemIndex = -1;
    edtGuarantorName->Clear();
    medtGuarantorAnnualIncome->Clear();
    medtGuarantorBirthDate->Clear();
    medtGuarantorBirthMonth->Clear();
    medtGuarantorBirthYear->Clear();
    medtGuarantorID->Clear();
  }
  else
    gbGuarantor->Enabled = enable;
  cmbGuarantorEducation->Enabled = enable;
  cmbGuarantorMarriage->Enabled = enable;
  cmbGuarantorQualified->Enabled = enable;
  edtGuarantorName->Enabled = enable;
  medtGuarantorAnnualIncome->Enabled = enable;
  medtGuarantorBirthDate->Enabled = enable;
  medtGuarantorBirthMonth->Enabled = enable;
  medtGuarantorBirthYear->Enabled = enable;
  medtGuarantorID->Enabled = enable;
  Label24->Enabled = enable;
  Label25->Enabled = enable;
  Label26->Enabled = enable;
  Label27->Enabled = enable;
  Label28->Enabled = enable;
  Label29->Enabled = enable;
  Label30->Enabled = enable;
  Label31->Enabled = enable;
  Label32->Enabled = enable;
  Label33->Enabled = enable;
  Label34->Enabled = enable;
  gbGuarantor->Enabled = enable;
};


