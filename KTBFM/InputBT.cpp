//---------------------------------------------------------------------------

#include <vcl.h>
#include <time.h>
#pragma hdrstop

#include "InputBT.h"
#include "Exit.h"
#include "common.h"
#include "keyin_check.h"
#include "dm.h"
#include "Reminder03.h"
#include "cc_err.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmBTInput *frmBTInput;
extern AnsiString case_sn;
//---------------------------------------------------------------------------
__fastcall TfrmBTInput::TfrmBTInput(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmBTInput::btnExitClick(TObject *Sender)
{
  Application->CreateForm(__classid(TfrmExitDialog), &frmExitDialog);
  frmExitDialog->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TfrmBTInput::FormShow(TObject *Sender)
{
  btnClear->Click();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBTInput::btnClearClick(TObject *Sender)
{
  edtAppName->Clear();
  medtAppID->Clear();

  ckbCoApp->Checked = true;
  ckbCoApp->Checked = false;

  ckbGuarantor->Checked = true;
  ckbGuarantor->Checked = false;

  medtBranch->Clear();
  medtEmployee->Clear();
  medtUnderwriter->Clear();
  if(this->Visible == true)
    edtAppName->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBTInput::ckbCoAppClick(TObject *Sender)
{
  if(ckbCoApp->Checked){
    gbCoApp->Enabled = true;
    edtCoAppName->Enabled = true;
    medtCoAppID->Enabled = true;
    Label13->Enabled = true;
    Label14->Enabled = true;
  }
  else{
    edtCoAppName->Clear();
    medtCoAppID->Clear();
    edtCoAppName->Enabled = false;
    medtCoAppID->Enabled = false;
    Label13->Enabled = false;
    Label14->Enabled = false;
    gbCoApp->Enabled = false;
  };
}
//---------------------------------------------------------------------------

void __fastcall TfrmBTInput::ckbGuarantorClick(TObject *Sender)
{
  if(ckbGuarantor->Checked){
    gbGuarantor->Enabled = true;
    edtGuarantorName->Enabled = true;
    medtGuarantorID->Enabled = true;
    Label24->Enabled = true;
    Label25->Enabled = true;
  }
  else{
    edtGuarantorName->Clear();
    medtGuarantorID->Clear();
    edtGuarantorName->Enabled = false;
    medtGuarantorID->Enabled = false;
    Label24->Enabled = false;
    Label25->Enabled = false;
    gbGuarantor->Enabled = false;
  };
}
//---------------------------------------------------------------------------

void __fastcall TfrmBTInput::btnScoreClick(TObject *Sender)
{
  AnsiString message;
  try{
    AnsiString query_sn_applicant, query_sn_co_applicant, query_sn_guarantor;
    AnsiString jcic_inquiry_result;
    AnsiString applicant_jcic_inquiry_date = "";
    AnsiString coapplicant_jcic_inquiry_date = "";
    AnsiString guarantor_jcic_inquiry_date = "";

    int ejcic_error_code;
    unsigned int availability; //1:applicant; 2:co-applicant; 4:guarantor
    bool success = true;

    availability = check_keyin();

    if(availability && 0x1){
/*      if(!request_for_jcic_data(Data->ejcic_query, medtAppID->Text.Trim(), query_sn_applicant, jcic_inquiry_result, ejcic_error_code)){
        success = false;
        message = ("e JCIC ¿ù»~¡A¥N½X¡G" + static_cast<AnsiString>(ejcic_error_code) + " °T®§¡G" + jcic_inquiry_result + "\n");
        MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
      }*/
    }
    else if(availability && 0x2){
/*      if(!request_for_jcic_data(Data->ejcic_query, medtAppID->Text.Trim(), query_sn_co_applicant, jcic_inquiry_result, ejcic_error_code)){
        success = false;
        message = ("e JCIC ¿ù»~¡A¥N½X¡G" + static_cast<AnsiString>(ejcic_error_code) + " °T®§¡G" + jcic_inquiry_result + "\n");
        MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
      }*/
    }
    else if(availability && 0x4){
/*      if(!request_for_jcic_data(Data->ejcic_query, medtAppID->Text.Trim(), query_sn_guarantor, jcic_inquiry_result, ejcic_error_code)){
        success = false;
        message = ("e JCIC ¿ù»~¡A¥N½X¡G" + static_cast<AnsiString>(ejcic_error_code) + " °T®§¡G" + jcic_inquiry_result + "\n");
        MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
      }*/
    };
    if(success){
      case_sn = generate_case_sn(Data->query);
      if(availability && 0x1){
//        applicant_jcic_inquiry_date =
//          get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn_applicant, medtAppID->Text, case_sn);
      }
      else if(availability && 0x2){
//        coapplicant_jcic_inquiry_date =
//          get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn_co_applicant, medtCoAppID->Text, case_sn);
      }
      else if(availability && 0x4){
//        guarantor_jcic_inquiry_date =
//         get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn_guarantor, medtGuarantorID->Text, case_sn);
      };
      store_input(Data->command, case_sn, applicant_jcic_inquiry_date, coapplicant_jcic_inquiry_date, guarantor_jcic_inquiry_date);
      frmBTInput->Hide();
      Application->CreateForm(__classid(TfrmSNReminder3), &frmSNReminder3);
      frmSNReminder3->lblCase_SN->Caption = case_sn;
      frmSNReminder3->ShowModal();
    };
  }
  catch(keyin_error &Err){
    message = Err.ShowMessage();
    MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
  }
  catch(Exception &Err){
    message = Err.Message;
    MessageDlg(message, mtInformation, TMsgDlgButtons() << mbOK, 0);
  };
};

unsigned int __fastcall TfrmBTInput::check_keyin()
{
  unsigned int return_value = 0x1;
  unsigned int gender;

  if(edtAppName->Text.Trim().IsEmpty())
    throw keyin_error(1);
  if(!check_id(medtAppID->Text, gender))
    throw keyin_error(2);

  if(ckbCoApp->Checked){
    return_value |= 0x2;
    if(edtCoAppName->Text.Trim().IsEmpty())
      throw keyin_error(8);
    if(!check_id(medtCoAppID->Text, gender))
      throw keyin_error(9);
  };
  if(ckbGuarantor->Checked){
    return_value |= 0x4;
    if(edtGuarantorName->Text.Trim().IsEmpty())
      throw keyin_error(15);
    if(!check_id(medtGuarantorID->Text, gender))
      throw keyin_error(16);
  };
  if(!check_channel(medtBranch->Text.c_str()))
    throw keyin_error(41);
  if(medtEmployee->Text.Trim().IsEmpty())
   throw keyin_error(40);
  if(medtUnderwriter->Text.Trim().IsEmpty())
    throw keyin_error(42);

  return return_value;
};

void __fastcall TfrmBTInput::store_input(TADOCommand *command,
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
  sql_stmt += "APP_NAME, APP_INQ_DATE, ";
  if(ckbCoApp->Checked){
    sql_stmt += "COS_ID, COS_NAME, COS_INQ_DATE, ";
  };
  if(ckbGuarantor->Checked){
    sql_stmt += "GUA_ID, GUA_NAME, GUA_INQ_DATE, ";
  };
  sql_stmt += "BRANCH, EMP_ID, AUDITOR) VALUES( ";
  sql_stmt += ":case_no, :app_date, :product_type, :app_id, :app_name, :app_inq_date, ";
  if(ckbCoApp->Checked){
    sql_stmt += ":cos_id, :cos_name, :cos_inq_date, ";
  };
  if(ckbGuarantor->Checked){
    sql_stmt += ":gua_id, :gua_name, :gua_inq_date, ";
  };
  sql_stmt += ":branch, :emp_id, :auditor)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_no"] = case_no;
  command->Parameters->ParamValues["app_date"] = app_date;
  command->Parameters->ParamValues["product_type"] = 3;
  command->Parameters->ParamValues["app_id"] = medtAppID->Text.Trim();
  command->Parameters->ParamValues["app_name"] = edtAppName->Text.Trim();
  command->Parameters->ParamValues["app_inq_date"] = applicant_inquiry_date;

  if(ckbCoApp->Checked){
    command->Parameters->ParamValues["cos_id"] = medtCoAppID->Text.Trim();
    command->Parameters->ParamValues["cos_name"] = edtCoAppName->Text.Trim();
    command->Parameters->ParamValues["cos_inq_date"] = coapplicant_inquiry_date;
  };
  if(ckbGuarantor->Checked){
    command->Parameters->ParamValues["gua_id"] = medtGuarantorID->Text.Trim().Trim();
    command->Parameters->ParamValues["gua_name"] = edtGuarantorName->Text.Trim().Trim();
    command->Parameters->ParamValues["gua_inq_date"] = guarantor_inquiry_date;
  };
  command->Parameters->ParamValues["branch"] = medtBranch->Text.Trim();
  command->Parameters->ParamValues["emp_id"] = medtEmployee->Text.Trim();
  command->Parameters->ParamValues["auditor"] = medtUnderwriter->Text.Trim();
  command->Execute();
};


//---------------------------------------------------------------------------

