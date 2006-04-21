//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Data_Module.h"
#include "functions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformMain *formMain;
AnsiString working_dir;
AnsiString report_dir;
AnsiString report_regen_dir;
AnsiString message;
AnsiString case_sn;
bool case_inserted;
//---------------------------------------------------------------------------
__fastcall TformMain::TformMain(TComponent* Owner)
        : TForm(Owner)
{
  CoInitialize(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btnExit1Click(TObject *Sender)
{
//  message = "確定要離開？";
//  MessageDlg(message, mtWarning, TMsgDlgButtons() << mbOK, 0);
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btnClearClick(TObject *Sender)
{
  message = "確定要清除所有的欄位？";
  MessageDlg(message, mtWarning, TMsgDlgButtons() << mbOK, 0);

// Applicant Info
  edtPrimaryName->Clear();
  medtPrimaryID->Clear();
  medtPrimaryBirthYear->Clear();
  medtPrimaryBirthMonth->Clear();
  medtPrimaryBirthDate->Clear();
  marriage->ItemIndex = 0;
  edtCareer->Clear();
  child->ItemIndex = 0;
  education->ItemIndex = 0;
  edtIncome->Clear();

// Product Info
  cbPeriod->Clear();
  edtAppFee->Clear();
  edtAppAmount->Clear();
  edtAPR->Clear();

// Property 1
  edtLienValue1->Clear();
  edtOwnerID1->Clear();
  edtLandNum1->Clear();
  lien1->ItemIndex = 1;
  location1->ItemIndex = 0;
  edtOwnerName1->Clear();
  relationship1->ItemIndex = 0;
  rgCollect1->ItemIndex = 0;
  rgDangerous1->ItemIndex = 0;
  rgDemolish1->ItemIndex = 0;
  rgForbit1->ItemIndex = 0;
  rgHouseRay1->ItemIndex = 0;
  rgSeasand1->ItemIndex = 0;

// Property 2
  edtLienValue2->Clear();
  edtOwnerID2->Clear();
  edtLandNum2->Clear();
  lien2->ItemIndex = 1;
  location2->ItemIndex = 0;
  edtOwnerName2->Clear();
  relationship2->ItemIndex = 0;
  rgCollect2->ItemIndex = 0;
  rgDangerous2->ItemIndex = 0;
  rgDemolish2->ItemIndex = 0;
  rgForbit2->ItemIndex = 0;
  rgHouseRay2->ItemIndex = 0;
  rgSeasand2->ItemIndex = 0;

  edtPrimaryName->SetFocus();
  lblMessage->Caption = "";
  message = "";

}

//---------------------------------------------------------------------------

void __fastcall TformMain::btnPrescreenClick(TObject *Sender)
{
// to be added
  bool is_input_error = false;
  unsigned int gender;
  AnsiString msn;
  lblMessage->Caption = "";
  message = "";
  formMain->Refresh();
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  double gav, nav;
  double gav1, nav1, gav2, nav2;
  int period;
  char header[10];
  char system_date[15], birthday[10];
  AnsiString sql_stmt;
  char error_msg[254];
  int status = 0;
  AnsiString ole_str = Data->connection->ConnectionString;

  gav1 = nav1 = gav2 = nav2 = 1; // initial to a non-zero value which means qualified property
  is_input_error = validate_application();
  try {
     if (!is_input_error) {
        // get case_sn
        get_time(year, month, day, hour, min);
        sprintf(header, "A%04d%02d%02d", year, month, day);
        msn = get_case_sn(header);
        sprintf(system_date, "%04d%02d%02d%02d%02d00", year, month, day, hour, min);
     // insert into app_info
        if (cbP2->Checked)
           sql_stmt = "insert into app_info (msn, system_date, applicant_id, applicant_name, "
                   " birthday, zip, marriage, child, education, career, income, app_amt, period, "
                   " apr, app_fee, branch, agent, owner_id1, "
                   " owner_name1, land_num1, lien1, first_lien1, relationship1, "
                   " location1, land_forbit1, land_demolish1, land_collect1, house_ray1, "
                   " house_seasand1, house_dangerous1, owner_id2, owner_name2, "
                   " land_num2, lien2, first_lien2, relationship2, location2, "
                   " land_forbit2, land_demolish2, land_collect2, house_ray2, house_seasand2, "
                   " house_dangerous2) values "
                   "(:msn, :system_date, :applicant_id, :applicant_name, "
                   " :birthday, :zip, :marriage, :child, :education, :career, :income, :app_amt, :period, "
                   " :apr, :app_fee, :branch, :agent, :owner_id1, "
                   " :owner_name1, :land_num1, :lien1, :first_lien1, :relationship1, "
                   " :location1, :land_forbit1, :land_demolish1, :land_collect1, :house_ray1, "
                   " :house_seasand1, :house_dangerous1, :owner_id2, :owner_name2, "
                   " :land_num2, :lien2, :first_lien2, :relationship2, :location2, "
                   " :land_forbit2, :land_demolish2, :land_collect2, :house_ray2, :house_seasand2, "
                   " :house_dangerous2);";
        else
           sql_stmt = "insert into app_info (msn, system_date, applicant_id, applicant_name, "
                   " birthday, zip, marriage, child, education, career, income, app_amt, period, "
                   " apr, app_fee, branch, agent, owner_id1, "
                   " owner_name1, land_num1, lien1, first_lien1, relationship1, "
                   " location1, land_forbit1, land_demolish1, land_collect1, house_ray1, "
                   " house_seasand1, house_dangerous1) values "
                   "(:msn, :system_date, :applicant_id, :applicant_name, "
                   " :birthday, :zip, :marriage, :child, :education, :career, :income, :app_amt, :period, "
                   " :apr, :app_fee, :branch, :agent, :owner_id1, "
                   " :owner_name1, :land_num1, :lien1, :first_lien1, :relationship1, "
                   " :location1, :land_forbit1, :land_demolish1, :land_collect1, :house_ray1, "
                   " :house_seasand1, :house_dangerous1);";

        Data->command->CommandText = sql_stmt;
        Data->command->Parameters->ParamValues["msn"] = msn;
        Data->command->Parameters->ParamValues["system_date"] = system_date;
        Data->command->Parameters->ParamValues["applicant_id"] = medtPrimaryID->Text;
        Data->command->Parameters->ParamValues["applicant_name"] = edtPrimaryName->Text;
        sprintf(birthday, "%03d%02d%02d", medtPrimaryBirthYear->Text.Trim().ToInt(),
                                       medtPrimaryBirthMonth->Text.Trim().ToInt(),
                                       medtPrimaryBirthDate->Text.Trim().ToInt());
        Data->command->Parameters->ParamValues["birthday"] = birthday;
        Data->command->Parameters->ParamValues["zip"] = edtZip->Text;
        Data->command->Parameters->ParamValues["marriage"] = marriage->ItemIndex -1;
        Data->command->Parameters->ParamValues["child"] = child->ItemIndex -1;
        Data->command->Parameters->ParamValues["education"] = education->ItemIndex - 1;
        Data->command->Parameters->ParamValues["career"] = edtCareer->Text;
        Data->command->Parameters->ParamValues["income"] = StrToInt(edtIncome->Text) * 10000;
        Data->command->Parameters->ParamValues["app_amt"] = StrToInt(edtAppAmount->Text) * 10000;
        switch(cbPeriod->ItemIndex) {
           case 1: period = 3 * 12; break;
           case 2: period = 5 * 12; break;
           case 3: period = 7 * 12; break;
        }
        Data->command->Parameters->ParamValues["period"] = period;
        Data->command->Parameters->ParamValues["apr"] = StrToFloat(edtAPR->Text) / 100.0;
        Data->command->Parameters->ParamValues["app_fee"] = StrToInt(edtAppFee->Text);
        Data->command->Parameters->ParamValues["branch"] = edtBranch->Text;
        Data->command->Parameters->ParamValues["agent"] = edtAgent->Text;
        Data->command->Parameters->ParamValues["owner_id1"] = edtOwnerID1->Text;
        Data->command->Parameters->ParamValues["owner_name1"] = edtOwnerName1->Text;
        Data->command->Parameters->ParamValues["land_num1"] = edtLandNum1->Text;
        Data->command->Parameters->ParamValues["lien1"] = lien1->ItemIndex - 1;
        Data->command->Parameters->ParamValues["first_lien1"] = StrToInt(edtLienValue1->Text);
        Data->command->Parameters->ParamValues["relationship1"] = relationship1->ItemIndex - 1;
        Data->command->Parameters->ParamValues["location1"] = location1->ItemIndex - 1;
        Data->command->Parameters->ParamValues["land_forbit1"] = rgForbit1->ItemIndex;
        Data->command->Parameters->ParamValues["land_demolish1"] = rgDemolish1->ItemIndex;
        Data->command->Parameters->ParamValues["land_collect1"] = rgCollect1->ItemIndex;
        Data->command->Parameters->ParamValues["house_ray1"] = rgHouseRay1->ItemIndex;
        Data->command->Parameters->ParamValues["house_seasand1"] = rgSeasand1->ItemIndex;
        Data->command->Parameters->ParamValues["house_dangerous1"] = rgSeasand1->ItemIndex;
        if ((lien1->ItemIndex > 2) || (relationship1->ItemIndex > 4) || (location1->ItemIndex > 3) ||
            ((rgHouseRay1->ItemIndex + rgSeasand1->ItemIndex + rgSeasand1->ItemIndex) > 0) ) {
            gav1 = nav1 = 0;
        }

        if (cbP2->Checked) {
           Data->command->Parameters->ParamValues["owner_id2"] = edtOwnerID2->Text;
           Data->command->Parameters->ParamValues["owner_name2"] = edtOwnerName2->Text;
           Data->command->Parameters->ParamValues["land_num2"] = edtLandNum2->Text;
           Data->command->Parameters->ParamValues["lien2"] = lien2->ItemIndex - 1;
           Data->command->Parameters->ParamValues["first_lien2"] = StrToInt(edtLienValue2->Text);
           Data->command->Parameters->ParamValues["relationship2"] = relationship2->ItemIndex - 1;
           Data->command->Parameters->ParamValues["location2"] = location2->ItemIndex - 1;
           Data->command->Parameters->ParamValues["land_forbit2"] = rgForbit2->ItemIndex;
           Data->command->Parameters->ParamValues["land_demolish2"] = rgDemolish2->ItemIndex;
           Data->command->Parameters->ParamValues["land_collect2"] = rgCollect2->ItemIndex;
           Data->command->Parameters->ParamValues["house_ray2"] = rgHouseRay2->ItemIndex;
           Data->command->Parameters->ParamValues["house_seasand2"] = rgSeasand2->ItemIndex;
           Data->command->Parameters->ParamValues["house_dangerous2"] = rgDangerous2->ItemIndex;
           if ((lien2->ItemIndex > 2) || (relationship2->ItemIndex > 4) || (location2->ItemIndex > 3) ||
               ((rgHouseRay2->ItemIndex + rgSeasand2->ItemIndex + rgSeasand2->ItemIndex) > 0) ) {
               gav2 = nav2 = 0;
           }
        }
        Data->command->Execute();
        message = "MSN " + msn + " 已新增。\n";

        gav = gav1;
        nav = nav1;
        if (cbP2->Checked) {
            gav += gav2;
            nav += nav2;
        }
        // call prescreen function
        status = DAC_SML_PRESCREEN(medtPrimaryID->Text.c_str(), msn.c_str(), system_date,
                                   ole_str.c_str(), gav, nav,
                                   error_msg);
        if (status < 0) {
            message += error_msg;
        } else if (status > 0) {
            message += error_msg;
        } else { // status == 0, i.e. prescreen pass (code = 0)
          // gav != 0 and nav != 0,
          if (gav1 == 0 || nav1 == 0) {
              message += error_msg;
              message += "，但第一擔保品資格不符。" ;
          }
          else if (cbP2->Checked && (gav2 == 0 || nav2 == 0)) {
              message += error_msg;
              message += "，但第二擔保品資格不符。" ;
          }
        }
        lblMessage->Caption = message;

     } // end of if(!is_input_error)
  } // end of try
  catch(Exception &E){
    lblMessage->Caption = E.Message;
    MessageDlg(E.Message, mtWarning, TMsgDlgButtons() << mbOK, 0);
  }
  formMain->Refresh();

}
//---------------------------------------------------------------------------
bool TformMain::validate_application()
{
  bool is_input_error = false;
  unsigned int gender;

  lblMessage->Caption = "";
  message = "";
  formMain->Refresh();
// CHECK APPLICANT INFO
// check Name
  if(edtPrimaryName->Text == ""){
    message += "申請人姓名未填。";
    is_input_error = true;
  };
// check ID
  if(medtPrimaryID->Text == ""){
    message += "申請人身分證字號未填。";
    is_input_error = true;
  }
  else if(!check_id(medtPrimaryID->Text, gender)){
    message += "申請人身分證號錯誤。";
    is_input_error = true;
  }
// check Channel
  if(!check_channel(edtBranch->Text.Trim().c_str())){
	message += "進件管道錯誤。";
	is_input_error = true;
  }
// check birthday
  if (medtPrimaryBirthYear->Text == "" ||
      medtPrimaryBirthMonth->Text == "" ||
      medtPrimaryBirthDate->Text == ""){
    message += "出生年月日輸入未填。";
    is_input_error = true;
  } 

  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;

  get_time(year, month, day, hour, min);

  unsigned int bad_date = check_birthday(medtPrimaryBirthYear->Text,
                                         medtPrimaryBirthMonth->Text,
                                         medtPrimaryBirthDate->Text,
                                         year, month, day, true);
  if (bad_date > 0){
    is_input_error = true;
    if((bad_date & 0x1) == 0x1)
      message += "申請人出生月份錯誤。";
    else if((bad_date & 0x2) == 0x2)
      message += "申請人出生日期錯誤。";
    else if((bad_date & 0x4) == 0x4)
      message += "申請人出生於未來。";
    else if((bad_date & 0x8) == 0x8)
      message += "申請人年齡資格不符。";
    else if((bad_date & 0x20) == 0x20)
      message += "申請人出生年份錯誤。";
  };
// check Zip
  if (!check_zip(edtZip->Text.Trim().c_str())){
     message += "郵遞區號錯誤\n";
     is_input_error = true;
  };
// check Marriage
  if(marriage->ItemIndex == 0){
    message += "婚姻狀況未填。";
    is_input_error = true;
  };
// check Children
  if(child->ItemIndex == 0){
    message += "撫養子女數未填。";
    is_input_error = true;
  };
// check Education
  if(education->ItemIndex == 0){
    message += "教育程度未填。";
    is_input_error = true;
  };
// check Career
  if(edtCareer->Text == ""){
    message += " 職業未填。";
    is_input_error = true;
  };
// check Income
  if(edtIncome->Text == ""){
    message += "年收入未填。";
    is_input_error = true;
  };

//CHECK PRODUCT INFO
// check App amount
  if(edtAppAmount->Text == ""){
    message += "申貸金額未填。";
    is_input_error = true;
  } else if (StrToInt(edtAppAmount->Text) < 10) {
    message += "申貸金額不能小於10萬。";
    is_input_error = true;
  }
// check APR
  if(edtAPR->Text == ""){
    message += "申貸利率未填。";
    is_input_error = true;
  } else if (StrToFloat(edtAPR->Text) < 0.0 || StrToFloat(edtAPR->Text) > 20.0) {
    message += "申貸利率輸入錯誤。";
    is_input_error = true;
  }
// check Period
  if(cbPeriod->ItemIndex == 0){
    message += "申貸期數未填。";
    is_input_error = true;
  };
// check App Fee
  if(edtAppFee->Text == ""){
    message += "開辦費未填。";
    is_input_error = true;
  } else if (StrToInt(edtAppFee->Text) < 0){
    message += "開辦費輸入錯誤。";
    is_input_error = true;
  }

//CHECK PROPERTY 1
// check owner name 1
  if(edtOwnerName1->Text == ""){
    message += "第一件擔保品所有人姓名未填。";
    is_input_error = true;
  };
// check owner id 1
  if(edtOwnerID1->Text == ""){
    message += "第一件擔保品所有人身分證字號未填。";
    is_input_error = true;
  }
  else if(!check_id(edtOwnerID1->Text, gender)){
    message += "第一件擔保品所有人身分證字號錯誤。";
    is_input_error = true;
  };
// check relationship with applicant 1
  if(relationship1->ItemIndex == 0){
    message += "第一件擔保品與申請人關係未填。";
    is_input_error = true;
  };
// check land and house num 1
  if(edtLandNum1->Text == ""){
    message += "第一件擔保品地號/建號未填。";
    is_input_error = true;
  };
// check location 1
  if(location1->ItemIndex == 0){
    message += "第一件擔保品座落區域未填。";
    is_input_error = true;
  };
// check lien 1
  if(lien1->ItemIndex == 0){
    message += "第一件擔保品目前抵押順位未填。";
    is_input_error = true;
  };
// check lien value 1
  if(edtLienValue1->Text == ""){
    message += "第一件擔保品總抵押金額未填。";
    is_input_error = true;
  } else if (StrToInt(edtLienValue1->Text) < 0){
    message += "第一件擔保品總抵押金額不能小於0。";
    is_input_error = true;
  }

//CHECK PROPERTY 2
if (cbP2->Checked == true)
{
// check owner name 2
  if(edtOwnerName2->Text == ""){
    message += "第二件擔保品所有人姓名未填。";
    is_input_error = true;
  };
// check owner id 2
  if(edtOwnerID2->Text == ""){
    message += "第二件擔保品所有人身分證字號未填。";
    is_input_error = true;
  } else if(!check_id(edtOwnerID2->Text, gender)){
    message += "第二件擔保品所有人身分證字號錯誤。";
    is_input_error = true;
  };
// check relationship with applicant 2
  if(relationship2->ItemIndex == 0){
    message += "第二件擔保品與申請人關係未填。";
    is_input_error = true;
  };
// check land and house num 2
  if(edtLandNum2->Text == ""){
    message += "第二件擔保品地號/建號未填。";
    is_input_error = true;
  };
// check location 2
  if(location2->ItemIndex == 0){
    message += "第二件擔保品座落區域未填。";
    is_input_error = true;
  };
// check lien 2
  if(lien2->ItemIndex == 0){
    message += "第二件擔保品目前抵押順位未填。";
    is_input_error = true;
  };
// check lien value 2
  if(edtLienValue2->Text == ""){
    message += "第二件擔保品總抵押金額未填。";
    is_input_error = true;
  } else if (StrToInt(edtLienValue2->Text) < 0){
    message += "第二件擔保品總抵押金額不能小於0。";
    is_input_error = true;
  }
}
// check Income
  if(edtBranch->Text == ""){
    message += "進件分行未填。";
    is_input_error = true;
  };
// check Income
  if(edtAgent->Text == ""){
    message += "進件員工未填。";
    is_input_error = true;
  };

  lblMessage->Caption = message;
  formMain->Refresh();

  return(is_input_error);
}
//---------------------------------------------------------------------------
bool TformMain::validate_property()
{
  bool is_input_error = false;
  unsigned int gender;

  lblMessage->Caption = "";
  message = "";
  formMain->Refresh();

//CHECK PRODUCT INFO
// check App amount
  if(edtAppAmount2->Text == ""){
    message += "申貸金額未填。";
    is_input_error = true;
  } else if (StrToInt(edtAppAmount2->Text) < 10) {
    message += "申貸金額不能小於10萬。";
    is_input_error = true;
  }
// check APR
  if(edtAPR2->Text == ""){
    message += "申貸利率未填。";
    is_input_error = true;
  } else if (StrToFloat(edtAPR2->Text) < 0.0 || StrToFloat(edtAPR2->Text) > 20.0) {
    message += "申貸利率輸入錯誤。";
    is_input_error = true;
  }
// check Period
  if(cbPeriod2->ItemIndex == 0){
    message += "申貸期數未填。";
    is_input_error = true;
  };
// check App Fee
  if(edtAppFee2->Text == ""){
    message += "開辦費未填。";
    is_input_error = true;
  } else if (StrToInt(edtAppFee2->Text) < 0){
    message += "開辦費輸入錯誤。";
    is_input_error = true;
  }

//CHECK PROPERTY 1
// check GAV 1
  if(gav1->Text == ""){
    message += "第一件擔保品毛值未填。";
    is_input_error = true;
  } else if (StrToInt(gav1->Text) < 0) {
    message += "第一件擔保品毛值輸入錯誤。";
    is_input_error = true;
  }
// check NAV 1
  if(nav1->Text == ""){
    message += "第一件擔保品淨值未填。";
    is_input_error = true;
  } else if (StrToInt(nav1->Text) < 0) {
    message += "第一件擔保品淨值輸入錯誤。";
    is_input_error = true;
  }
  if (StrToInt(nav1->Text) > StrToInt(gav1->Text)) {
    message += "第一件擔保品淨值大於毛值。";
    is_input_error = true;
  }

//CHECK PROPERTY 2
if (lblOwnerID2->Enabled == true)
{
// check GAV 2
  if(gav2->Text == ""){
    message += "第二件擔保品毛值未填。";
    is_input_error = true;
  } else if (StrToInt(gav2->Text) < 0) {
    message += "第二件擔保品毛值輸入錯誤。";
    is_input_error = true;
  }
// check NAV 2
  if(nav2->Text == ""){
    message += "第二件擔保品淨值未填。";
    is_input_error = true;
  } else if (StrToInt(nav2->Text) < 0) {
    message += "第二件擔保品淨值輸入錯誤。";
    is_input_error = true;
  }
  if (StrToInt(nav2->Text) > StrToInt(gav2->Text)) {
    message += "第二件擔保品淨值大於毛值。";
    is_input_error = true;
  }
}
  lblMessage->Caption = message;
  formMain->Refresh();

  return(is_input_error);
}
//---------------------------------------------------------------------------
AnsiString TformMain::get_case_sn(char *header)
{
  AnsiString msn, sql_stmt;
  char case_sn[15];
  int  seq_no;

  try {
       sql_stmt = "SELECT seq_no FROM COUNTER WHERE seq_type = :seq_type;";
       Data->query->Close();
       Data->query->SQL->Clear();
       Data->query->SQL->Add(sql_stmt);
       Data->query->Parameters->ParamValues["seq_type"] = header;
       Data->query->Open();
       if (Data->query->FieldValues["seq_no"].IsNull()) {
          // insert into counter COUNTER;
          seq_no = 1;
          sprintf(case_sn, "%s%05d", header, seq_no);
          seq_no++;
          msn = case_sn;
          sql_stmt = "INSERT INTO COUNTER (seq_type, seq_no) VALUES (:seq_type, :seq_no);";
          Data->command->CommandText =sql_stmt;
          Data->command->Parameters->ParamValues["seq_type"] = header;
          Data->command->Parameters->ParamValues["seq_no"] = seq_no;
          Data->command->Execute();
       } else {
          // add 1 to seq_no and write back to table COUNTER
          seq_no = Data->query->FieldValues["seq_no"];
          sprintf(case_sn, "%s%05d", header, seq_no);
          seq_no++;
          msn = case_sn;
          sql_stmt = "UPDATE COUNTER SET seq_no = :seq_no WHERE seq_type = :seq_type;";
          Data->command->CommandText = sql_stmt;
          Data->command->Parameters->ParamValues["seq_type"] = header;
          Data->command->Parameters->ParamValues["seq_no"] = seq_no;
          Data->command->Execute();
       }
  }
  catch(Exception &E){
    lblMessage->Caption = E.Message;
    MessageDlg(E.Message, mtWarning, TMsgDlgButtons() << mbOK, 0);
  }
  return(msn);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TformMain::cbP2Click(TObject *Sender)
{
// enable all the labels, input fields.
 if (cbP2->Checked) {
  Label36->Enabled = true;
  Label37->Enabled = true;
  Label38->Enabled = true;
  Label51->Enabled = true;
  Label52->Enabled = true;
  Label53->Enabled = true;
  Label54->Enabled = true;
  Label57->Enabled = true;
  Label59->Enabled = true;
  Label61->Enabled = true;
  Label63->Enabled = true;
  Label64->Enabled = true;
  Label65->Enabled = true;
  Label66->Enabled = true;
  edtLienValue2->Enabled = true;
  edtOwnerID2->Enabled = true;
  edtLandNum2->Enabled = true;
  lien2->Enabled = true;
  location2->Enabled = true;
  edtOwnerName2->Enabled = true;
  relationship2->Enabled = true;
  rgCollect2->Enabled = true;
  rgDangerous2->Enabled = true;
  rgDemolish2->Enabled = true;
  rgForbit2->Enabled = true;
  rgHouseRay2->Enabled = true;
  rgSeasand2->Enabled = true;
 } else {
  Label36->Enabled = false;
  Label37->Enabled = false;
  Label38->Enabled = false;
  Label51->Enabled = false;
  Label52->Enabled = false;
  Label53->Enabled = false;
  Label54->Enabled = false;
  Label57->Enabled = false;
  Label59->Enabled = false;
  Label61->Enabled = false;
  Label63->Enabled = false;
  Label64->Enabled = false;
  Label65->Enabled = false;
  Label66->Enabled = false;
  edtLienValue2->Enabled = false;
  edtOwnerID2->Enabled = false;
  edtLandNum2->Enabled = false;
  lien2->Enabled = false;
  location2->Enabled = false;
  edtOwnerName2->Enabled = false;
  relationship2->Enabled = false;
  rgCollect2->Enabled = false;
  rgDangerous2->Enabled = false;
  rgDemolish2->Enabled = false;
  rgForbit2->Enabled = false;
  rgHouseRay2->Enabled = false;
  rgSeasand2->Enabled = false;
 }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TformMain::finalReview_ClearClick(TObject *Sender)
{
  message = "確定要清除所有的欄位？";
  MessageDlg(message, mtWarning, TMsgDlgButtons() << mbOK, 0);

// Product Info
  cbPeriod2->ItemIndex = 0;
  edtAppFee2->Clear();
  edtAppAmount2->Clear();
  edtAPR2->Clear();

// Property 1
  gav1->Clear();
  nav1->Clear();
  rgBasement1->ItemIndex = 0;
  rgComplex1->ItemIndex = 0;
  rgDamage1->ItemIndex = 0;
  rgLent1->ItemIndex = 0;
  rgMisuse1->ItemIndex = 0;

// Property 2
  gav2->Clear();
  nav2->Clear();
  rgBasement2->ItemIndex = 0;
  rgComplex2->ItemIndex = 0;
  rgDamage2->ItemIndex = 0;
  rgLent2->ItemIndex = 0;
  rgMisuse2->ItemIndex = 0;

  edtPrimaryName->SetFocus();
  lblMessage->Caption = "";
  message = "";
  formMain->Refresh();

}
//---------------------------------------------------------------------------
void __fastcall TformMain::SelectClick(TObject *Sender)
{
  AnsiString sql_stmt;
  int period, index;

  lblMessage->Caption = "";
  message = "";
  formMain->Refresh();

  try {
     sql_stmt = "select msn, system_date, applicant_id, applicant_name, app_amt, period, apr, app_fee, "
                " owner_id1, owner_name1, land_num1, first_lien1, "
                " owner_id2, owner_name2, land_num2, first_lien2, zip "
                " from app_info where msn = :msn";
     Data->query->Close();
     Data->query->SQL->Clear();
     Data->query->SQL->Add(sql_stmt);
     Data->query->Parameters->ParamValues["msn"] = edtMSN->Text;
     Data->query->Open();
     if (!Data->query->FieldValues["msn"].IsNull()) {
        lblMSN->Caption = Data->query->FieldValues["msn"];
        hidden_SystemDate->Caption = Data->query->FieldValues["system_date"];
        hidden_Zip->Caption = Data->query->FieldValues["zip"];
        lblPrimaryName->Caption = Data->query->FieldValues["applicant_name"];
        lblPrimaryID->Caption = Data->query->FieldValues["applicant_id"];
        edtAppAmount2->Text = Data->query->FieldValues["owner_id1"];
        edtAPR2->Text = Data->query->FieldValues["apr"] * 100;
        edtAppAmount2->Text = Data->query->FieldValues["app_amt"] / 10000;
        period = Data->query->FieldValues["period"];
        switch (period) {
           case 36: index = 1; break;
           case 60: index = 2; break;
           case 84: index = 3; break;
        }
        cbPeriod2->ItemIndex =index;
        edtAppFee2->Text = Data->query->FieldValues["app_fee"];
        lblOwnerID1->Caption = Data->query->FieldValues["owner_id1"];
        lblLandHouseNum1->Caption = Data->query->FieldValues["land_num1"];
        hidden_FirstLien1->Caption = Data->query->FieldValues["first_lien1"];
        if (!Data->query->FieldValues["owner_id2"].IsNull()) {
           // enable Property 2
           lblOwnerID2->Caption = Data->query->FieldValues["owner_id2"];
           lblLandHouseNum2->Caption = Data->query->FieldValues["land_num2"];
           hidden_FirstLien2->Caption = Data->query->FieldValues["first_lien2"];
           lblOwnerID2->Enabled = true;
           lblLandHouseNum2->Enabled = true;
           gav2->Enabled = true;
           Label106->Enabled = true;
           Label76->Enabled = true;
           Label77->Enabled = true;
           Label78->Enabled = true;
           Label79->Enabled = true;
           Label80->Enabled = true;
           Label81->Enabled = true;
           Label82->Enabled = true;
           Label91->Enabled = true;
           Label94->Enabled = true;
           Label96->Enabled = true;
           nav2->Enabled = true;
           rgBasement2->Enabled = true;
           rgComplex2->Enabled = true;
           rgDamage2->Enabled = true;
           rgLent2->Enabled = true;
           rgMisuse2->Enabled = true;
           GroupBox4->Enabled = true;

           lblOwnerID2->Visible = true;
           lblLandHouseNum2->Visible = true;
           gav2->Visible = true;
           Label106->Visible = true;
           Label76->Visible = true;
           Label77->Visible = true;
           Label78->Visible = true;
           Label79->Visible = true;
           Label80->Visible = true;
           Label81->Visible = true;
           Label82->Visible = true;
           Label91->Visible = true;
           Label94->Visible = true;
           Label96->Visible = true;
           nav2->Visible = true;
           rgBasement2->Visible = true;
           rgComplex2->Visible = true;
           rgDamage2->Visible = true;
           rgLent2->Visible = true;
           rgMisuse2->Visible = true;
           GroupBox4->Visible = true;
        }
     } else {
        lblMessage->Caption = "案件編號不存在";
        formMain->Refresh();
     }
  }
  catch(Exception &E){
    lblMessage->Caption = E.Message;
    MessageDlg(E.Message, mtWarning, TMsgDlgButtons() << mbOK, 0);
  }

  formMain->Refresh();

}
//---------------------------------------------------------------------------


void __fastcall TformMain::finalReviewClick(TObject *Sender)
{
// to be added
  bool is_input_error = false;
  unsigned int gender;
  AnsiString msn;
  lblMessage->Caption = "";
  message = "";
  formMain->Refresh();
  int period;
  AnsiString sql_stmt;

  double gav, nav, lien_value;
  double gav_1, nav_1, gav_2, nav_2;
  char error_msg[254];
  int status = 0;
  AnsiString ole_str = Data->connection->ConnectionString;
  AnsiString system_date = hidden_SystemDate->Caption;
  AnsiString zip = hidden_Zip->Caption;
  double first_lien1 = hidden_FirstLien1->Caption.ToDouble();
  double first_lien2 = hidden_FirstLien2->Caption.ToDouble();
  double first_lien;

// check data error
  is_input_error = validate_property();

  try {
     if (!is_input_error) {
        // update app_info
        sql_stmt = "update app_info "
                   "  set app_amt = :app_amt, "
                   "  period = :period, "
                   "  apr = :apr, "
                   "  app_fee = :app_fee, "
                   "  nav1 = :nav1, "
                   "  gav1 = :gav1, "
                   "  house_damage1 = :house_damage1, "
                   "  house_lent1 = :house_lent1, "
                   "  house_misuse1 = :house_misuse1, "
                   "  house_complex1 = :house_complex1, "
                   "  house_basement1 = :house_basement1 "
                   "where msn = :msn;";
        Data->command->CommandText = sql_stmt;
        Data->command->Parameters->ParamValues["app_amt"] = StrToInt(edtAppAmount2->Text) * 10000;
        switch(cbPeriod2->ItemIndex) {
           case 1: period = 3 * 12; break;
           case 2: period = 5 * 12; break;
           case 3: period = 7 * 12; break;
        }
        Data->command->Parameters->ParamValues["period"] = period;
        Data->command->Parameters->ParamValues["apr"] = StrToFloat(edtAPR2->Text) / 100.0;
        Data->command->Parameters->ParamValues["app_fee"] = edtAppFee2->Text;
        Data->command->Parameters->ParamValues["nav1"] = nav1->Text.Trim().ToInt();
        Data->command->Parameters->ParamValues["gav1"] = gav1->Text.Trim().ToInt();
        gav_1 = gav1->Text.Trim().ToDouble();
        nav_1 = nav1->Text.Trim().ToDouble();
        Data->command->Parameters->ParamValues["house_damage1"] = rgDamage1->ItemIndex;
        Data->command->Parameters->ParamValues["house_lent1"] = rgLent1->ItemIndex;
        Data->command->Parameters->ParamValues["house_misuse1"] = rgMisuse1->ItemIndex;
        Data->command->Parameters->ParamValues["house_complex1"] = rgComplex1->ItemIndex;
        Data->command->Parameters->ParamValues["house_basement1"] = rgBasement1->ItemIndex;
        Data->command->Parameters->ParamValues["msn"] = lblMSN->Caption;
        Data->command->Execute();
        if ((rgDamage1->ItemIndex + rgLent1->ItemIndex + rgMisuse1->ItemIndex +
             rgComplex1->ItemIndex + rgBasement1->ItemIndex) > 0) {
           gav_1 = nav_1 = first_lien1 = 0;
        }

        if (lblOwnerID2->Enabled) { // if property 2 exists, update property 2 info
           sql_stmt = "update app_info "
                      "  set nav2 = :nav2, "
                      "  gav2 = :gav2, "
                      "  house_damage2 = :house_damage2, "
                      "  house_lent2 = :house_lent2, "
                      "  house_misuse2 = :house_misuse2, "
                      "  house_complex2 = :house_complex2, "
                      "  house_basement2 = :house_basement2 "
                      "where msn = :msn;";
           Data->command->CommandText = sql_stmt;
           Data->command->Parameters->ParamValues["nav2"] = nav2->Text.Trim().ToInt();
           Data->command->Parameters->ParamValues["gav2"] = gav2->Text.Trim().ToInt();
           gav_1 = gav1->Text.Trim().ToDouble();
           nav_1 = nav1->Text.Trim().ToDouble();
           Data->command->Parameters->ParamValues["house_damage2"] = rgDamage2->ItemIndex;
           Data->command->Parameters->ParamValues["house_lent2"] = rgLent2->ItemIndex;
           Data->command->Parameters->ParamValues["house_misuse2"] = rgMisuse2->ItemIndex;
           Data->command->Parameters->ParamValues["house_complex2"] = rgComplex2->ItemIndex;
           Data->command->Parameters->ParamValues["house_basement2"] = rgBasement2->ItemIndex;
           Data->command->Parameters->ParamValues["msn"] = lblMSN->Caption;
           Data->command->Execute();
           if ((rgDamage2->ItemIndex + rgLent2->ItemIndex + rgMisuse2->ItemIndex +
                rgComplex2->ItemIndex + rgBasement2->ItemIndex) > 0) {
               gav_2 = nav_2 = first_lien2 = 0;
           }
        }
     // call final review function
        gav = gav_1;
        nav = nav_1;
        first_lien = first_lien1;

        if (lblOwnerID2->Enabled) {
            gav += gav_2;
            nav += nav_2;
            first_lien += first_lien2;
        }
        // call final review function
        status = DAC_SML_NPV(medtPrimaryID->Text.c_str(), msn.c_str(), system_date.c_str(),
                             ole_str.c_str(),StrToFloat(edtAppAmount2->Text) * 10000,
                             StrToFloat(edtAPR2->Text) / 100.0, StrToFloat(edtAppFee2->Text),
                             period, gav, nav, zip.c_str(), first_lien, error_msg);
        if (status < 0) {
            message += error_msg;
        } else if (status > 0) {
            message += error_msg;
        } else { // status == 0, i.e. prescreen pass (code = 0)
          // gav != 0 and nav != 0,
          if (gav1 == 0 || nav1 == 0) {
              message += error_msg;
              message += "，但第一擔保品資格不符。" ;
          }
          else if (lblOwnerID2->Enabled && (gav2 == 0 || nav2 == 0)) {
              message += error_msg;
              message += "，但第二擔保品資格不符。" ;
          }
        }
        lblMessage->Caption = message;
     }
  }
  catch(Exception &E){
    lblMessage->Caption = E.Message;
    MessageDlg(E.Message, mtWarning, TMsgDlgButtons() << mbOK, 0);
  }
  formMain->Refresh();

}
//---------------------------------------------------------------------------


