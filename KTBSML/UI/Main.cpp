//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Data_Module.h"
#include "functions.h"
#include "LoginUI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformMain *formMain;
AnsiString working_dir;
AnsiString report_dir;
AnsiString report_regen_dir;
AnsiString message;
AnsiString case_sn;
AnsiString agent_id;
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
//  message = "�T�w�n���}�H";
//  MessageDlg(message, mtWarning, TMsgDlgButtons() << mbOK, 0);
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btnClearClick(TObject *Sender)
{
  message = "�T�w�n�M���Ҧ������H";
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
                   " house_dangerous2, inquiry_date) values "
                   "(:msn, :system_date, :applicant_id, :applicant_name, "
                   " :birthday, :zip, :marriage, :child, :education, :career, :income, :app_amt, :period, "
                   " :apr, :app_fee, :branch, :agent, :owner_id1, "
                   " :owner_name1, :land_num1, :lien1, :first_lien1, :relationship1, "
                   " :location1, :land_forbit1, :land_demolish1, :land_collect1, :house_ray1, "
                   " :house_seasand1, :house_dangerous1, :owner_id2, :owner_name2, "
                   " :land_num2, :lien2, :first_lien2, :relationship2, :location2, "
                   " :land_forbit2, :land_demolish2, :land_collect2, :house_ray2, :house_seasand2, "
                   " :house_dangerous2, :inquiry_date);";
        else
           sql_stmt = "insert into app_info (msn, system_date, applicant_id, applicant_name, "
                   " birthday, zip, marriage, child, education, career, income, app_amt, period, "
                   " apr, app_fee, branch, agent, owner_id1, "
                   " owner_name1, land_num1, lien1, first_lien1, relationship1, "
                   " location1, land_forbit1, land_demolish1, land_collect1, house_ray1, "
                   " house_seasand1, house_dangerous1, inquiry_date) values "
                   "(:msn, :system_date, :applicant_id, :applicant_name, "
                   " :birthday, :zip, :marriage, :child, :education, :career, :income, :app_amt, :period, "
                   " :apr, :app_fee, :branch, :agent, :owner_id1, "
                   " :owner_name1, :land_num1, :lien1, :first_lien1, :relationship1, "
                   " :location1, :land_forbit1, :land_demolish1, :land_collect1, :house_ray1, "
                   " :house_seasand1, :house_dangerous1, :inquiry_date);";

        sql_stmt = sql_stmt.UpperCase();
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

////////// Connect to GSS EJCIC SYSTEM ////////////////////////////////
      AnsiString query_sn;
      AnsiString jcic_inquiry_result;
      AnsiString jcic_inquiry_date;
      char error[256];
      int module_operation;
      bool success;
      int ejcic_error_code;

          success = request_for_jcic_data(Data->ejcic_query, medtPrimaryID->Text.Trim(), query_sn, jcic_inquiry_result, ejcic_error_code);
          if (success) {
            message += "�p�x��Ƥw���^�A���b�x�s�p�x��ơC ";
            lblMessage->Caption = message;
            formMain->Refresh();
            jcic_inquiry_date =
              get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn, msn, medtPrimaryID->Text, system_date);

            message += "�p�x��Ƥw�x�s�A���b������. . .\n";
            lblMessage->Caption = message;
            formMain->Refresh();

            // insert into app_info after JCIC data is successfully retrieved.
            Data->command->Parameters->ParamValues["inquiry_date"] = jcic_inquiry_date;
            Data->command->Execute();
            message += "�ץ� " + msn + " �w�s�W�C\n";
            lblMessage->Caption = message;
            formMain->Refresh();

            gav = gav1;
            nav = nav1;
            if (cbP2->Checked) {
                gav += gav2;
                nav += nav2;
            }
            // call prescreen function
            status = DAC_SML_PRESCREEN(medtPrimaryID->Text.c_str(), msn.c_str(), jcic_inquiry_date.c_str(),
                                       ole_str.c_str(), gav, nav,
                                       error_msg);
            if (status < 0) {
                message += error_msg;
            } else if (status > 0) {
                message += error_msg;
            } else { // status == 0, i.e. prescreen pass (code = 0)
              // gav != 0 and nav != 0,
              message += error_msg;
              if (gav1 == 0 || nav1 == 0) {
                  message += "�A���Ĥ@��O�~��椣�šC" ;
              }
              else if (cbP2->Checked && (gav2 == 0 || nav2 == 0)) {
                  message += "�A���ĤG��O�~��椣�šC" ;
              }
            }
            lblMessage->Caption = message;
            formMain->Refresh();
          }
          else {
  //        log_error(Data->command, medtPrimaryID->Text, jcic_inquiry_result, ejcic_error_code);
            message += ("e JCIC ���~�A�N�X�G" + static_cast<AnsiString>(ejcic_error_code) + " �T���G" + jcic_inquiry_result + "\n");
            lblMessage->Caption = message;
            formMain->Refresh();
            status = -1;
          }

      if(status >= 0){
        lblMessage->Caption = "���������C�i�H��J�U�@���C\n";
        formMain->Refresh();
      };

//////////////////////////////////////////
     } // end of if(!is_input_error)
  } // end of try
  catch(Exception &E){
    message += E.Message;
    MessageDlg(E.Message, mtWarning, TMsgDlgButtons() << mbOK, 0);
  }
  lblMessage->Caption = message;
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
    message += "�ӽФH�m�W����C";
    is_input_error = true;
  }
// check ID
  if(medtPrimaryID->Text == "          "){
    message += "�ӽФH�����Ҧr������C";
    is_input_error = true;
  }
  else if(!check_id(medtPrimaryID->Text, gender)){
    message += "�ӽФH�����Ҹ����~�C";
    is_input_error = true;
  }
// check birthday
  if (medtPrimaryBirthYear->Text == "" ||
      medtPrimaryBirthMonth->Text == "" ||
      medtPrimaryBirthDate->Text == ""){
    message += "�X�ͦ~����J����C";
    is_input_error = true;
  } else {
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
         message += "�X�ͦ~����J���~�C";
       else if((bad_date & 0x2) == 0x2)
         message += "�X�ͦ~����J���~�C";
       else if((bad_date & 0x4) == 0x4)
         message += "�ӽФH�X�ͩ󥼨ӡC";
       else if((bad_date & 0x8) == 0x8)
         message += "�ӽФH�~�ָ�椣�šC";
       else if((bad_date & 0x20) == 0x20)
         message += "�X�ͦ~����J���~�C";
     }
  }
// check Zip
  if(edtZip->Text == "  "){
    message += "�l���ϸ�����C";
    is_input_error = true;
  }
  if (!check_zip(edtZip->Text.Trim().c_str())){
     message += "�l���ϸ����~\n";
     is_input_error = true;
  }
// check Marriage
  if(marriage->ItemIndex == 0){
    message += "�B�ê��p����C";
    is_input_error = true;
  }
// check Children
  if(child->ItemIndex == 0){
    message += "���i�l�k�ƥ���C";
    is_input_error = true;
  }
// check Education
  if(education->ItemIndex == 0){
    message += "�Ш|�{�ץ���C";
    is_input_error = true;
  }
// check Career
  if(edtCareer->Text == ""){
    message += " ¾�~����C";
    is_input_error = true;
  }
// check Income
  if(edtIncome->Text == ""){
    message += "�~���J����C";
    is_input_error = true;
  } else if (StrToInt(edtIncome->Text) < 15) {
    message += "�~���J��J���~�C";
    is_input_error = true;
  }

//CHECK PRODUCT INFO
// check App amount
  if(edtAppAmount->Text == ""){
    message += "�ӶU���B����C";
    is_input_error = true;
  } else if (StrToInt(edtAppAmount->Text) < 10 ||StrToInt(edtAppAmount->Text) > 500) {
    message += "�ӶU���B��J���~�C";
    is_input_error = true;
  }
// check APR
  try {
     if (edtAPR->Text == ""){
        message += "�ӶU�Q�v����C";
        is_input_error = true;
     } else if (StrToFloat(edtAPR->Text) < 0.0 || StrToFloat(edtAPR->Text) > 20.0) {
        message += "�ӶU�Q�v��J���~�C";
        is_input_error = true;
     }
  }
  catch (...){
     message += "�ӶU�Q�v��J���~�C";
     is_input_error = true;
  }
// check Period
  if(cbPeriod->ItemIndex == 0){
    message += "�ӶU���ƥ���C";
    is_input_error = true;
  }
// check App Fee
  if(edtAppFee->Text == ""){
    message += "�}��O����C";
    is_input_error = true;
  } else if (StrToInt(edtAppFee->Text) < 0){
    message += "�}��O��J���~�C";
    is_input_error = true;
  }

//CHECK PROPERTY 1
// check owner name 1
  if(edtOwnerName1->Text == ""){
    message += "�Ĥ@���O�~�Ҧ��H�m�W����C";
    is_input_error = true;
  }
// check owner id 1
  if(edtOwnerID1->Text == "          "){
    message += "�Ĥ@���O�~�Ҧ��H�����Ҧr������C";
    is_input_error = true;
  }
  else if(!check_id(edtOwnerID1->Text, gender)){
    message += "�Ĥ@���O�~�Ҧ��H�����Ҧr�����~�C";
    is_input_error = true;
  }
// check relationship with applicant 1
  if(relationship1->ItemIndex == 0){
    message += "�Ĥ@���O�~�P�ӽФH���Y����C";
    is_input_error = true;
  }
// check land and house num 1
  if(edtLandNum1->Text == ""){
    message += "�Ĥ@���O�~�a��/�ظ�����C";
    is_input_error = true;
  }
// check location 1
  if(location1->ItemIndex == 0){
    message += "�Ĥ@���O�~�y���ϰ쥼��C";
    is_input_error = true;
  }
// check lien 1
  if(lien1->ItemIndex == 0){
    message += "�Ĥ@���O�~�ثe��㶶�쥼��C";
    is_input_error = true;
  }
// check lien value 1
  if(edtLienValue1->Text == ""){
    message += "�Ĥ@���O�~�`�����B����C";
    is_input_error = true;
  } else if (StrToInt(edtLienValue1->Text) < 0){
    message += "�Ĥ@���O�~�`�����B����p��0�C";
    is_input_error = true;
  }

//CHECK PROPERTY 2
if (cbP2->Checked == true)
{
// check owner name 2
  if(edtOwnerName2->Text == ""){
    message += "�ĤG���O�~�Ҧ��H�m�W����C";
    is_input_error = true;
  }
// check owner id 2
  if(edtOwnerID2->Text == "          "){
    message += "�ĤG���O�~�Ҧ��H�����Ҧr������C";
    is_input_error = true;
  } else if(!check_id(edtOwnerID2->Text, gender)){
    message += "�ĤG���O�~�Ҧ��H�����Ҧr�����~�C";
    is_input_error = true;
  }
// check relationship with applicant 2
  if(relationship2->ItemIndex == 0){
    message += "�ĤG���O�~�P�ӽФH���Y����C";
    is_input_error = true;
  }
// check land and house num 2
  if(edtLandNum2->Text == ""){
    message += "�ĤG���O�~�a��/�ظ�����C";
    is_input_error = true;
  }
// check location 2
  if(location2->ItemIndex == 0){
    message += "�ĤG���O�~�y���ϰ쥼��C";
    is_input_error = true;
  }
// check lien 2
  if(lien2->ItemIndex == 0){
    message += "�ĤG���O�~�ثe��㶶�쥼��C";
    is_input_error = true;
  }
// check lien value 2
  if(edtLienValue2->Text == ""){
    message += "�ĤG���O�~�`�����B����C";
    is_input_error = true;
  } else if (StrToInt(edtLienValue2->Text) < 0){
    message += "�ĤG���O�~�`�����B����p��0�C";
    is_input_error = true;
  }
}
// check Branch
  if(edtBranch->Text == ""){
    message += "�i����楼��C";
    is_input_error = true;
  }else {
     if (!check_channel(edtBranch->Text.Trim().c_str())){
	message += "����N����J���~�C";
	is_input_error = true;
     }
  }
// check Agent
  if(edtAgent->Text == ""){
    message += "�i����u����C";
    is_input_error = true;
  }

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
    message += "�ӶU���B����C";
    is_input_error = true;
  } else if (StrToInt(edtAppAmount2->Text) < 15 ||StrToInt(edtAppAmount2->Text) > 100) {
    message += "�ӶU���B��J���~�C";
    is_input_error = true;
  }
// check APR
  try {
     if (edtAPR2->Text == ""){
        message += "�ӶU�Q�v����C";
        is_input_error = true;
     } else if (StrToFloat(edtAPR2->Text) < 0.0 || StrToFloat(edtAPR2->Text) > 20.0) {
        message += "�ӶU�Q�v��J���~�C";
        is_input_error = true;
     }
  }
  catch (...){
     message += "�ӶU�Q�v��J���~�C";
     is_input_error = true;
  }
// check Period
  if(cbPeriod2->ItemIndex == 0){
    message += "�ӶU���ƥ���C";
    is_input_error = true;
  }
// check App Fee
  if(edtAppFee2->Text == ""){
    message += "�}��O����C";
    is_input_error = true;
  } else if (StrToInt(edtAppFee2->Text) < 0){
    message += "�}��O��J���~�C";
    is_input_error = true;
  }

//CHECK PROPERTY 1
// check GAV 1
  if(gav1->Text == ""){
    message += "�Ĥ@���O�~��ȥ���C";
    is_input_error = true;
  } else if (StrToInt(gav1->Text) < 0) {
    message += "�Ĥ@���O�~��ȿ�J���~�C";
    is_input_error = true;
  }
// check NAV 1
  if(nav1->Text == ""){
    message += "�Ĥ@���O�~�b�ȥ���C";
    is_input_error = true;
  } else if (StrToInt(nav1->Text) < 0) {
    message += "�Ĥ@���O�~�b�ȿ�J���~�C";
    is_input_error = true;
  } else if (StrToInt(nav1->Text) > StrToInt(gav1->Text)) {
    message += "�Ĥ@���O�~�b�Ȥj���ȡC";
    is_input_error = true;
  }

//CHECK PROPERTY 2
if (lblOwnerID2->Enabled == true)
{
// check GAV 2
  if(gav2->Text == ""){
    message += "�ĤG���O�~��ȥ���C";
    is_input_error = true;
  } else if (StrToInt(gav2->Text) < 0) {
    message += "�ĤG���O�~��ȿ�J���~�C";
    is_input_error = true;
  }
// check NAV 2
  if(nav2->Text == ""){
    message += "�ĤG���O�~�b�ȥ���C";
    is_input_error = true;
  } else if (StrToInt(nav2->Text) < 0) {
    message += "�ĤG���O�~�b�ȿ�J���~�C";
    is_input_error = true;
  } else if (StrToInt(nav2->Text) > StrToInt(gav2->Text)) {
    message += "�ĤG���O�~�b�Ȥj���ȡC";
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
       sql_stmt = sql_stmt.UpperCase();
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
          sql_stmt = sql_stmt.UpperCase();
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
          sql_stmt = sql_stmt.UpperCase();
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
  message = "�T�w�n�M���Ҧ������H";
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

  edtAppAmount2->SetFocus();
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
     sql_stmt = "select a.msn, system_date, applicant_id, applicant_name, app_amt, period, apr, app_fee, "
                " owner_id1, owner_name1, land_num1, first_lien1, "
                " owner_id2, owner_name2, land_num2, first_lien2, zip, inquiry_date "
                " from app_info a, app_premier b "
                " where a.msn = :msn and a.msn = b.msn and b.premier_code = 0;";
     sql_stmt = sql_stmt.UpperCase();
     Data->query->Close();
     Data->query->SQL->Clear();
     Data->query->SQL->Add(sql_stmt);
     Data->query->Parameters->ParamValues["msn"] = edtMSN->Text;
     Data->query->Open();
     if (!Data->query->FieldValues["MSN"].IsNull()) {
        lblMSN->Caption = Data->query->FieldValues["MSN"];
        hidden_SystemDate->Caption = Data->query->FieldValues["SYSTEM_DATE"];
        hidden_Zip->Caption = Data->query->FieldValues["ZIP"];
        hidden_InquiryDate->Caption = Data->query->FieldValues["INQUIRY_DATE"];
        lblPrimaryName->Caption = Data->query->FieldValues["APPLICANT_NAME"];
        lblPrimaryID->Caption = Data->query->FieldValues["APPLICANT_ID"];
        edtAppAmount2->Text = Data->query->FieldValues["OWNER_ID1"];
        edtAPR2->Text = Data->query->FieldValues["APR"] * 100;
        edtAppAmount2->Text = Data->query->FieldValues["APP_AMT"] / 10000;
        period = Data->query->FieldValues["PERIOD"];
        switch (period) {
           case 36: index = 1; break;
           case 60: index = 2; break;
           case 84: index = 3; break;
        }
        cbPeriod2->ItemIndex =index;
        edtAppFee2->Text = Data->query->FieldValues["APP_FEE"];
        lblOwnerID1->Caption = Data->query->FieldValues["OWNER_ID1"];
        lblLandHouseNum1->Caption = Data->query->FieldValues["LAND_NUM1"];
        hidden_FirstLien1->Caption = Data->query->FieldValues["FIRST_LIEN1"];
        if (!Data->query->FieldValues["OWNER_ID2"].IsNull()) {
           // enable Property 2
           lblOwnerID2->Caption = Data->query->FieldValues["OWNER_ID2"];
           lblLandHouseNum2->Caption = Data->query->FieldValues["LAND_NUM2"];
           hidden_FirstLien2->Caption = Data->query->FieldValues["FIRST_LIEN2"];
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
        lblMessage->Caption = "�ץ�s�����s�b";
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
  double first_lien2;
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
        sql_stmt = sql_stmt.UpperCase();
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
           first_lien2 = hidden_FirstLien2->Caption.ToDouble();
           sql_stmt = "update app_info "
                      "  set nav2 = :nav2, "
                      "  gav2 = :gav2, "
                      "  house_damage2 = :house_damage2, "
                      "  house_lent2 = :house_lent2, "
                      "  house_misuse2 = :house_misuse2, "
                      "  house_complex2 = :house_complex2, "
                      "  house_basement2 = :house_basement2 "
                      "where msn = :msn;";
           sql_stmt = sql_stmt.UpperCase();
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
        message += "������. . .\n";
        lblMessage->Caption = message;
        formMain->Refresh();

        gav = gav_1;
        nav = nav_1;
        first_lien = first_lien1;

        if (lblOwnerID2->Enabled) {
            gav += gav_2;
            nav += nav_2;
            first_lien += first_lien2;
        }
        // call final review function
        status = DAC_SML_NPV(lblPrimaryID->Caption.c_str(), lblMSN->Caption.c_str(), hidden_InquiryDate->Caption.c_str(),
                             ole_str.c_str(), StrToFloat(edtAppAmount2->Text) * 10000,
                             StrToFloat(edtAPR2->Text) / 100.0, period, StrToFloat(edtAppFee2->Text),
                             gav, nav, zip.c_str(), first_lien, error_msg);
        if (status < 0) {
            message += error_msg;
        } else if (status > 0) {
            message += error_msg;
        } else { // status == 0, i.e. prescreen pass (code = 0)
          // gav != 0 and nav != 0,
          message += error_msg;
          if (gav1 == 0 || nav1 == 0) {
              message += "�A���Ĥ@��O�~��椣�šC" ;
          }
          else if (lblOwnerID2->Enabled && (gav2 == 0 || nav2 == 0)) {
              message += "�A���ĤG��O�~��椣�šC" ;
          }
        }
        lblMessage->Caption = message;
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


void __fastcall TformMain::FormDestroy(TObject *Sender)
{
 CoUninitialize();
}
//---------------------------------------------------------------------------
bool request_for_jcic_data(TADOQuery *query, const AnsiString &id, AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no)
{
  bool return_value = true;

  WideString jcic_inquiry_package_code = "004";
  WideString branch_code = "5330";//"8160":��T��; For testing machine:GSS
  WideString user_id = "DAC";//"01922":���F�F; For testing machine:AUSTIN;
  WideString jcic_inquiry_criteria = id.Trim() + "^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^";
  WideString inquiry_style = "D"; //for data; "H" for html
  WideString AS400 = "N";
  WideString inquiry_reason = "A1A";
  WideString check_idn = "Y";
  WideString program_id = "GSSJCIC30";

  Data->ejcic->set_AP_ID(program_id);
  Data->ejcic->set_ConStrName(program_id);

  Data->ejcic->PutRequest(jcic_inquiry_package_code,
                          jcic_inquiry_criteria,
                          branch_code,
                          user_id,
                          inquiry_style,
                          AS400,
                          inquiry_reason,
                          check_idn);


  error_no = Data->ejcic->get_Error_Number();

  if(error_no == 0){
    query_sn = static_cast<AnsiString>(Data->ejcic->QSEQNO);

    AnsiString ejcic_inquiry_status = "";
    AnsiString ejcic_inquiry_result = "";
    AnsiString ejcic_inquiry_return_code = "";

    ejcic_inquiry_status = get_ejcic_inquiry_result(query, query_sn, ejcic_inquiry_result, ejcic_inquiry_return_code);

    if(ejcic_inquiry_status != "R"){
//      frmMainUI->timer->OnTimer;
//      frmMainUI->timer->Enabled = true;
      for(int i=1; i < 4; ++i){
        if(ejcic_inquiry_status == "R")
          break;
        else if(ejcic_inquiry_status == "P"){
          formMain->lblMessage->Caption = "��" + static_cast<AnsiString>(i) + " �����աCeJCIC�|���N�d�߰e���p�x�C";
          formMain->Refresh();
        }
        else if(ejcic_inquiry_status == "S"){
          formMain->lblMessage->Caption = "��" + static_cast<AnsiString>(i) + " �����աC�p�x�|���^�Ьd�ߡC";
          formMain->Refresh();
        }
        else{
          formMain->lblMessage->Caption = "eJCIC �p�x�d�ߪ��A�L�k���ѡC";
          formMain->Refresh();
        }
        Sleep(5000);
        ejcic_inquiry_status = get_ejcic_inquiry_result(query, query_sn, ejcic_inquiry_result, ejcic_inquiry_return_code);
      };
    };
    if(ejcic_inquiry_status != "R"){
      jcic_inquiry_result = "�p�x�����^�Ǭd�ߵ��G�A���ӽХ�B�z������p��e-JCIC���@�H���P�p�x�s���H���C\n";
      return_value = false;
    }
    else{
      if(ejcic_inquiry_result == "S"){
        jcic_inquiry_result = "";
        return_value = true;
      }
      else if(ejcic_inquiry_result == "F"){
        jcic_inquiry_result = "�p�x�d�߿��~�Ae-JCIC�T���G" + ejcic_inquiry_return_code + " ���p��e-JCIC���@�H���P�p�x�s���H���C\n";
        return_value = false;
      }
    }
  }
  else{
    jcic_inquiry_result = "�d�߿��~�A���e�X���p�x�Ae-JCIC�T���G" + Data->ejcic->get_JC_ReturnCode() + " ���p��e-JCIC���@�H���C\n";
    return_value = false;
  };

  return return_value;
}
//---------------------------------------------------------------------------

AnsiString get_store_jcic_data(TADOConnection *ejcic_connection,
                         TADOQuery *ejcic_query, TADOCommand *command,
                         const AnsiString &query_sn, const AnsiString &msn, const AnsiString &idn,
                         const AnsiString &input_time)
{
  if(!ejcic_connection->Connected)
    ejcic_connection->Open();
  ejcic_query->Close();

  AnsiString INQUIRY_DATE;
  AnsiString ejcic_sql_stmt;
  AnsiString sql_stmt;

  ejcic_sql_stmt = "SELECT RS_DTM_CHAR FROM TNBB_JCIC.JC_T_REQ WHERE QSEQNO = :query_sn";
  ejcic_sql_stmt = ejcic_sql_stmt.UpperCase();
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  INQUIRY_DATE = ejcic_query->FieldValues["RS_DTM_CHAR"];
  ejcic_query->Close();
  INQUIRY_DATE = INQUIRY_DATE.SubString(1,8);


//  sql_stmt = "INSERT INTO KRM023(IDN, INQUIRY_DATE, YRMON, ISSUE, ISSUE_NAME, ";
//  sql_stmt += "KR_CODE, LIMIT, PAYMENT, CASH, PAY_CODE, Input_Time) VALUES(:idn, ";
//  sql_stmt += ":inquiry_date, :yrmon, :issue, :issue_name, :kr_code, :limit, ";
//  sql_stmt += ":payment, :cash, :pay_code, :input_time);";
  sql_stmt = "INSERT INTO KRM023(MSN, IDN, INQUIRY_DATE, YRMON, ISSUE, ISSUE_NAME, "
             "KR_CODE, LIMIT, PAYMENT, CASH, PAY_CODE) VALUES(:msn, :idn, "
             ":inquiry_date, :yrmon, :issue, :issue_name, :kr_code, :limit, "
             ":payment, :cash, :pay_code);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_KRM023 WHERE QSEQNO = :query_sn";
  ejcic_sql_stmt = ejcic_sql_stmt.UpperCase();
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
     command->Parameters->ParamValues["msn"] = msn;
     command->Parameters->ParamValues["idn"] = idn;
     command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
     command->Parameters->ParamValues["yrmon"] = ejcic_query->FieldValues["YRMON"];
     command->Parameters->ParamValues["issue"] = ejcic_query->FieldValues["ISSUE"];
     command->Parameters->ParamValues["issue_name"] = ejcic_query->FieldValues["ISSUE_NAME"];
     command->Parameters->ParamValues["kr_code"] = ejcic_query->FieldValues["KR_CODE"];
     command->Parameters->ParamValues["limit"] = ejcic_query->FieldValues["LIMIT"];
     command->Parameters->ParamValues["payment"] = ejcic_query->FieldValues["PAYMENT"];
     command->Parameters->ParamValues["cash"] = ejcic_query->FieldValues["CASH"];
     command->Parameters->ParamValues["pay_code"] = ejcic_query->FieldValues["PAY_CODE"];
//	  command->Parameters->ParamValues["input_time"] = input_time;
     command->Execute();
     ejcic_query->Next();
  }


//  sql_stmt = "INSERT INTO KRM021(IDN, INQUIRY_DATE, CARD_BRAND, CARD_TYPE, ISSUE, ";
//  sql_stmt += "ISSUE_NAME, START_DATE, STOP_DATE, STOP_CODE, AB_CODE, ";
//  sql_stmt += "M_S, RELA, LIMIT, RISK, CLEAR_DATE, IDN_PRI, CNAME, REMARK, INPUT_TIME) VALUES(:idn, ";
//  sql_stmt += ":inquiry_date, :card_brand, :card_type, :issue, ";
//  sql_stmt += ":issue_name, :start_date, :stop_date, :stop_code, :ab_code, :m_s, :rela, :limit, ";
//  sql_stmt += ":risk, :clear_date, :pidn, :cname, :remark, :input_time)";

  sql_stmt = "INSERT INTO KRM021(MSN, IDN, INQUIRY_DATE, CARD_BRAND, CARD_TYPE, ISSUE, "
             "ISSUE_NAME, START_DATE, STOP_DATE, STOP_CODE, AB_CODE, "
             "M_S, RELA, LIMIT, RISK, CLEAR_DATE, IDN_PRI, CNAME, REMARK) VALUES(:msn, :idn, "
             ":inquiry_date, :card_brand, :card_type, :issue, "
             ":issue_name, :start_date, :stop_date, :stop_code, :ab_code, :m_s, :rela, :limit, "
             ":risk, :clear_date, :pidn, :cname, :remark)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;


  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_KRM021 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
     command->Parameters->ParamValues["msn"] = msn;
     command->Parameters->ParamValues["idn"] = idn.UpperCase();
     command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
     command->Parameters->ParamValues["card_brand"] = ejcic_query->FieldValues["CARD_BRAND"];
     command->Parameters->ParamValues["card_type"] = ejcic_query->FieldValues["CARD_TYPE"];
     command->Parameters->ParamValues["issue"] = ejcic_query->FieldValues["ISSUE"];
     command->Parameters->ParamValues["issue_name"] = ejcic_query->FieldValues["ISSUE_NAME"];
     command->Parameters->ParamValues["start_date"] = ejcic_query->FieldValues["START_DATE"];
     command->Parameters->ParamValues["stop_date"] = ejcic_query->FieldValues["STOP_DATE"];
     command->Parameters->ParamValues["stop_code"] = ejcic_query->FieldValues["STOP_CODE"];
     command->Parameters->ParamValues["ab_code"] = ejcic_query->FieldValues["AB_CODE"];
     command->Parameters->ParamValues["m_s"] = ejcic_query->FieldValues["M_S"];
     command->Parameters->ParamValues["rela"] = ejcic_query->FieldValues["RELA"];
     command->Parameters->ParamValues["limit"] = ejcic_query->FieldValues["LIMIT"];
     command->Parameters->ParamValues["risk"] = ejcic_query->FieldValues["RISK"];
     command->Parameters->ParamValues["clear_date"] = ejcic_query->FieldValues["CLEAR_DATE"];
     command->Parameters->ParamValues["pidn"] = ejcic_query->FieldValues["IDN"];
     command->Parameters->ParamValues["cname"] = ejcic_query->FieldValues["CNAME"];
     command->Parameters->ParamValues["remark"] = ejcic_query->FieldValues["REMARK"];
//    command->Parameters->ParamValues["input_time"] = input_time;
     command->Execute();
     ejcic_query->Next();
  };


//  sql_stmt = "INSERT INTO BAM086(IDN, INQUIRY_DATE, DATA_YYY, DATA_MM, BANK_CODE, ";
//  sql_stmt += "BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, ";
//  sql_stmt += "LOAN_AMT, PASS_DUE_AMT, PAY_CODE_12, CO_LOAN, INPUT_TIME) VALUES (:idn, ";
//  sql_stmt += ":inquiry_date, :data_yyy, :data_mm, :bank_code, :bank_name, ";
//  sql_stmt += ":account_code, :account_code2, :purpose_code, :contract_amt, ";
//  sql_stmt += ":loan_amt, :pass_due_amt, :pay_code_12, :co_loan, :input_time);";

  sql_stmt = "INSERT INTO BAM086(MSN, IDN, INQUIRY_DATE, DATA_YYY, DATA_MM, BANK_CODE, "
             "BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, "
             "LOAN_AMT, PASS_DUE_AMT, PAY_CODE_12, CO_LOAN) VALUES (:msn, :idn, "
             ":inquiry_date, :data_yyy, :data_mm, :bank_code, :bank_name, "
             ":account_code, :account_code2, :purpose_code, :contract_amt, "
             ":loan_amt, :pass_due_amt, :pay_code_12, :co_loan);";
  command->CommandText = sql_stmt;
  sql_stmt = sql_stmt.UpperCase();
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_BAM086 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
     command->Parameters->ParamValues["msn"] = msn;
     command->Parameters->ParamValues["idn"] = idn;
     command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
     command->Parameters->ParamValues["data_yyy"] = ejcic_query->FieldValues["DATA_YYY"];
     command->Parameters->ParamValues["data_mm"] = ejcic_query->FieldValues["DATA_MM"];
     command->Parameters->ParamValues["bank_code"] = ejcic_query->FieldValues["BANK_CODE"];
     command->Parameters->ParamValues["bank_name"] = ejcic_query->FieldValues["BANK_NAME"];
     command->Parameters->ParamValues["account_code"] = ejcic_query->FieldValues["ACCOUNT_CODE"];
     command->Parameters->ParamValues["account_code2"] = ejcic_query->FieldValues["ACCOUNT_CODE2"];
     command->Parameters->ParamValues["purpose_code"] = ejcic_query->FieldValues["PURPOSE_CODE"];
     command->Parameters->ParamValues["contract_amt"] = ejcic_query->FieldValues["CONTRACT_AMT"];
     command->Parameters->ParamValues["loan_amt"] = ejcic_query->FieldValues["LOAN_AMT"];
     command->Parameters->ParamValues["pass_due_amt"] = ejcic_query->FieldValues["PASS_DUE_AMT"];
     command->Parameters->ParamValues["pay_code_12"] = ejcic_query->FieldValues["PAY_CODE_12"];
     command->Parameters->ParamValues["co_loan"] = ejcic_query->FieldValues["CO_LOAN"];
//	  command->Parameters->ParamValues["input_time"] = input_time;
     command->Execute();
     ejcic_query->Next();
  };


//  sql_stmt = "INSERT INTO JAS002(IDN, INQUIRY_DATE, EVER_DELINQUENT, DELINQUENT_DATE, ";
//  sql_stmt += "EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, ";
//  sql_stmt += "EVER_STOP_CARD, STOP_CARD_DATE, INPUT_TIME) VALUES (:idn, :inquiry_date, ";
//  sql_stmt += ":ever_delinquent, :delinquent_date, :ever_bad_check, ";
//  sql_stmt += ":bad_check_date, :ever_reject, :reject_date, :ever_stop_card, ";
//  sql_stmt += ":stop_card_date, :input_time);";

  sql_stmt = "INSERT INTO JAS002(MSN, IDN, INQUIRY_DATE, EVER_DELINQUENT, DELINQUENT_DATE, "
             "EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, "
             "EVER_STOP_CARD, STOP_CARD_DATE) VALUES (:msn, :idn, :inquiry_date, "
             ":ever_delinquent, :delinquent_date, :ever_bad_check, "
             ":bad_check_date, :ever_reject, :reject_date, :ever_stop_card, "
             ":stop_card_date);";
  command->CommandText = sql_stmt;
  sql_stmt = sql_stmt.UpperCase();
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_JAS002 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
     command->Parameters->ParamValues["msn"] = msn;
     command->Parameters->ParamValues["idn"] = idn;
     command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
     command->Parameters->ParamValues["ever_delinquent"] = ejcic_query->FieldValues["EVER_DELINQUENT"];
     command->Parameters->ParamValues["delinquent_date"] = ejcic_query->FieldValues["DELINQUENT_DATE"];
     command->Parameters->ParamValues["ever_bad_check"] = ejcic_query->FieldValues["EVER_BAD_CHECK"];
     command->Parameters->ParamValues["bad_check_date"] = ejcic_query->FieldValues["BAD_CHECK_DATE"];
     command->Parameters->ParamValues["ever_reject"] = ejcic_query->FieldValues["EVER_REJECT"];
     command->Parameters->ParamValues["reject_date"] = ejcic_query->FieldValues["REJECT_DATE"];
     command->Parameters->ParamValues["ever_stop_card"] = ejcic_query->FieldValues["EVER_STOP_CARD"];
     command->Parameters->ParamValues["stop_card_date"] = ejcic_query->FieldValues["STOP_CARD_DATE"];
//	  command->Parameters->ParamValues["input_time"] = input_time;
     command->Execute();
     ejcic_query->Next();
  };


/*  sql_stmt = "INSERT INTO STM007(IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, BANK_NAME, ";
  sql_stmt += "ITEM_LIST, INQ_PURPOSE_1, INQ_PURPOSE, INPUT_TIME) VALUES (:idn, :INQUIRY_DATE, :query_date, ";
  sql_stmt += ":bank_code, :bank_name, :item_list, :inq_purpose_1, :inq_purose, :input_time);";
	command->CommandText = sql_stmt;*/

//  sql_stmt = "INSERT INTO STM007(IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, BANK_NAME, ";
//  sql_stmt += "ITEM_LIST, INPUT_TIME, INQ_PURPOSE_1, INQ_PURPOSE) VALUES (:idn, :INQUIRY_DATE, :query_date, ";
//  sql_stmt += ":bank_code, :bank_name, :item_list, :input_time, :inq_purpose_1, :inq_purpose);";

  sql_stmt = "INSERT INTO STM007(MSN, IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, BANK_NAME, "
             "ITEM_LIST, INQ_PURPOSE_1, INQ_PURPOSE) VALUES (:msn, :idn, :INQUIRY_DATE, :query_date, "
             ":bank_code, :bank_name, :item_list, :inq_purpose_1, :inq_purpose);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_STM007 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
     command->Parameters->ParamValues["msn"] = msn;
     command->Parameters->ParamValues["idn"] = idn;
     command->Parameters->ParamValues["query_date"] = ejcic_query->FieldValues["QUERY_DATE"];
     command->Parameters->ParamValues["INQUIRY_DATE"] = INQUIRY_DATE;
     command->Parameters->ParamValues["bank_code"] = ejcic_query->FieldValues["BANK_CODE"];
     command->Parameters->ParamValues["bank_name"] = ejcic_query->FieldValues["BANK_NAME"];
     command->Parameters->ParamValues["item_list"] = ejcic_query->FieldValues["ITEM_LIST"];
     command->Parameters->ParamValues["inq_purpose_1"] = ejcic_query->FieldValues["INQ_PURPOSE_1"];
     command->Parameters->ParamValues["inq_purpose"] = ejcic_query->FieldValues["INQ_PURPOSE"];
//	  command->Parameters->ParamValues["input_time"] = input_time;
     command->Execute();
     ejcic_query->Next();
  }

//  sql_stmt = "INSERT INTO KRM037 (";
//  sql_stmt += "IDN, INQUIRY_DATE, BILL_DATE, ISSUE, ISSUE_NAME, CARD_TYPE, ";
//  sql_stmt += "PERM_LIMIT, TEMP_LIMIT, CASH_LIMIT, PAYABLE, CASH_LENT, ";
//  sql_stmt += "LAST_PAYA, REVOL_BAL, PAY_STAT, PAY_CODE, REVOL_RATE, ";
//  sql_stmt += "PRE_OWED,DEBT_CODE, CLOSE_CODE, CLEAR_DATE, INPUT_TIME) VALUES(";
//  sql_stmt += ":IDN, :INQUIRY_DATE, :BILL_DATE, :ISSUE, :ISSUE_NAME, :CARD_TYPE, ";
//  sql_stmt += ":PERM_LIMIT, :TEMP_LIMIT, :CASH_LIMIT, :PAYABLE, :CASH_LENT, ";
//  sql_stmt += ":LAST_PAYA, :REVOL_BAL, :PAY_STAT, :PAY_CODE, :REVOL_RATE, ";
//  sql_stmt += ":PRE_OWED, :DEBT_CODE, :CLOSE_CODE, :CLEAR_DATE, :INPUT_TIME)";

  sql_stmt = "INSERT INTO KRM037 (MSN, "
             "IDN, INQUIRY_DATE, BILL_DATE, ISSUE, ISSUE_NAME, CARD_TYPE, "
             "PERM_LIMIT, TEMP_LIMIT, CASH_LIMIT, PAYABLE, CASH_LENT, "
             "LAST_PAYA, REVOL_BAL, PAY_STAT, PAY_CODE, REVOL_RATE, "
             "PRE_OWED,DEBT_CODE, CLOSE_CODE, CLEAR_DATE) VALUES("
             ":MSN, :IDN, :INQUIRY_DATE, :BILL_DATE, :ISSUE, :ISSUE_NAME, :CARD_TYPE, "
             ":PERM_LIMIT, :TEMP_LIMIT, :CASH_LIMIT, :PAYABLE, :CASH_LENT, "
             ":LAST_PAYA, :REVOL_BAL, :PAY_STAT, :PAY_CODE, :REVOL_RATE, "
             ":PRE_OWED, :DEBT_CODE, :CLOSE_CODE, :CLEAR_DATE)";
  sql_stmt = sql_stmt.UpperCase();
	command->CommandText = sql_stmt;
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_KRM037 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
     command->Parameters->ParamValues["MSN"] = msn;
     command->Parameters->ParamValues["IDN"] = idn;
     command->Parameters->ParamValues["INQUIRY_DATE"] = INQUIRY_DATE;
     command->Parameters->ParamValues["BILL_DATE"] = ejcic_query->FieldValues["BILL_DATE"];
     command->Parameters->ParamValues["ISSUE"] = ejcic_query->FieldValues["ISSUE"];
     command->Parameters->ParamValues["ISSUE_NAME"] = ejcic_query->FieldValues["ISSUE_NAME"];
     command->Parameters->ParamValues["PERM_LIMIT"] = ejcic_query->FieldValues["PERM_LIMIT"];
     command->Parameters->ParamValues["TEMP_LIMIT"] = ejcic_query->FieldValues["TEMP_LIMIT"];
     command->Parameters->ParamValues["CASH_LIMIT"] = ejcic_query->FieldValues["CASH_LIMIT"];
     command->Parameters->ParamValues["PAYABLE"] = ejcic_query->FieldValues["PAYABLE"];
     command->Parameters->ParamValues["CASH_LENT"] = ejcic_query->FieldValues["CASH_LENT"];
     command->Parameters->ParamValues["LAST_PAYA"] = ejcic_query->FieldValues["LAST_PAYA"];
     command->Parameters->ParamValues["REVOL_BAL"] = ejcic_query->FieldValues["REVOL_BAL"];
     command->Parameters->ParamValues["PAY_STAT"] = ejcic_query->FieldValues["PAY_STAT"];
     command->Parameters->ParamValues["PAY_CODE"] = ejcic_query->FieldValues["PAY_CODE"];
     command->Parameters->ParamValues["REVOL_RATE"] = ejcic_query->FieldValues["REVOL_RATE"];
     command->Parameters->ParamValues["PRE_OWED"] = ejcic_query->FieldValues["PRE_OWED"];
     command->Parameters->ParamValues["DEBT_CODE"] = ejcic_query->FieldValues["DEBT_CODE"];
     command->Parameters->ParamValues["CLOSE_CODE"] = ejcic_query->FieldValues["CLOSE_CODE"];
     command->Parameters->ParamValues["CLEAR_DATE"] = ejcic_query->FieldValues["CLEAR_DATE"];
//	  command->Parameters->ParamValues["INPUT_TIME"] = input_time;
     command->Execute();
     ejcic_query->Next();
  }
  return INQUIRY_DATE;
}
//---------------------------------------------------------------------------

AnsiString get_ejcic_inquiry_result(TADOQuery *query, const AnsiString &query_sn, AnsiString &result, AnsiString &result_code)
{
  AnsiString sql_stmt = "SELECT * FROM TNBB_JCIC.JC_T_REQ WHERE QSEQNO = :query_sn";
  AnsiString return_value = "";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["query_sn"] = query_sn;
  query->Open();
  if(!query->FieldValues["STATUS"].IsNull())
    return_value = query->FieldValues["STATUS"];
  if(!query->FieldValues["RESULT"].IsNull())
    result = query->FieldValues["RESULT"];
  if(!query->FieldValues["RT_CODE"].IsNull())
    result_code = query->FieldValues["RT_CODE"];
  return return_value;
}
//---------------------------------------------------------------------------


void __fastcall TformMain::FormCreate(TObject *Sender)
{
  lblMessage->Caption = "";
  formMain->Hide();
  message = "";
  report_dir = "";
  report_regen_dir = "";
  char working_drive;
  char working[2048];
  int drive = getdisk();
  working_drive = 'A' + drive;
  getcurdir(0, working);
  working_dir = working;
  working_dir = static_cast<AnsiString>(working_drive) + ":\\" + working_dir;
  if(working_dir.Length()>3)
    working_dir += "\\";

  char buffer[2048];
  ifstream config((working_dir + "config.cfg").c_str());
  if(config){
    config.getline(buffer,2048);
    report_dir = buffer;
  };
  config.clear();
  config.close();
}
//---------------------------------------------------------------------------


