//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Data_Module.h"
#include "functions.h"
#include "LoginUI.h"
#include "dirui.h"
#include "redirui.h"
#include "main_utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformMain *formMain;
AnsiString working_dir;
AnsiString report_dir;
AnsiString report_final_dir;
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
  message = "確定要離開？";
  if (MessageDlg(message, mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
     Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btnClearClick(TObject *Sender)
{
  message = "確定要清除所有的欄位？";
  if (MessageDlg(message, mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes) {

   // Applicant Info
     edtPrimaryName->Clear();
     medtPrimaryID->Clear();
   //  medtPrimaryBirthYear->Clear();
   //  medtPrimaryBirthMonth->Clear();
   //  medtPrimaryBirthDate->Clear();
   //  marriage->ItemIndex = 0;
   //  edtCareer->Clear();
   //  child->ItemIndex = 0;
   //  education->ItemIndex = 0;
     edtIncome->Clear();
     medtZip->Clear();
     rgAppQualified->ItemIndex = 1;

   // Product Info
     cbPeriod->ItemIndex = 0;
     medtAppFee->Clear();
     edtAppAmount->Clear();
     edtAPR->Clear();

   // Property 1
   //  edtLienValue1->Clear();
     edtOwnerID1->Clear();
     edtLandNum1->Clear();
   //  lien1->ItemIndex = 1;
   //  location1->ItemIndex = 0;
     edtOwnerName1->Clear();
     relationship1->ItemIndex = 0;

   // Property 2
   //  edtLienValue2->Clear();
     edtOwnerID2->Clear();
     edtLandNum2->Clear();
   //  lien2->ItemIndex = 1;
   //  location2->ItemIndex = 0;
     edtOwnerName2->Clear();
     relationship2->ItemIndex = 0;

     edtBranch->Clear();
     edtAuditor->Clear();
     edtEmpID->Clear();

     medtPrimaryID->SetFocus();
     lblMessage->Caption = "";
     message = "";
     }
}

//---------------------------------------------------------------------------
void __fastcall TformMain::medtPrimaryIDExit(TObject *Sender)
{
 bool is_input_error;

//  if(!check_msn(edtCaseNo->Text, Data->query)){
//    message += "郵寄編號號錯誤\n";
//    lblMessage->Caption = message;
//    is_input_error = true;
//  }
//  if(!check_id(medtPrimaryID->Text, edtCaseNo->Text, Data->query)){
//    message += "申請人身分證號錯誤\n";
//    lblMessage->Caption = message;
//    is_input_error = true;
//  }
//  if(!is_input_error)
//     edtPrimaryName->Text = get_applicant_name(edtCaseNo->Text, Data->query);
//  else{
//    formMain->btnClearClick(NULL);
//    is_input_error = false;
//  }
}
//---------------------------------------------------------------------------
void __fastcall TformMain::btnPrescreenClick(TObject *Sender)
{
// to be added
  bool is_input_error = false;
  unsigned int gender;
  AnsiString msn, product_code;
  lblMessage->Caption = "";
  message = "";
  formMain->Refresh();
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  unsigned int sec;
  int income;
//  double gav, nav;
//  double gav1, nav1, gav2, nav2;
  double apr;
  unsigned int application_fee;
  int period;
  char header[10];
  char system_date[15], birthday[10];
  AnsiString sql_stmt;
  char error_msg[254];
  int status = 0;
  AnsiString ole_str = Data->connection->ConnectionString;

  AnsiString query_sn;
  AnsiString jcic_inquiry_result;
  AnsiString jcic_inquiry_date;
  char error[256];
  int module_operation;
  bool success;
  int ejcic_error_code;

//  gav1 = nav1 = gav2 = nav2 = 1; // initial to a non-zero value which means qualified property
  is_input_error = validate_application();
  try {
     if (!is_input_error) {
        // get case_sn
        get_time(year, month, day, hour, min, sec);
        sprintf(header, "A%04d%02d%02d", year, month, day);
        msn = get_case_sn(header);
        msn = edtCaseNo->Text;     // comment out after test
        sprintf(system_date, "%04d%02d%02d%02d%02d%02d", year, month, day, hour, min, sec);
     // insert into app_info
        if (cbP2->Checked)
           sql_stmt = "insert into app_info (msn, system_date, applicant_id, applicant_name, "
                   " zip, income, app_qualified, app_amt, period, "
                   " apr, app_fee, branch, emp_id, auditor,"
                   " owner_id1, owner_name1, land_num1, relationship1, "
                   " owner_id2, owner_name2, land_num2, relationship2, "
                   " inquiry_date) values "
                   "(:msn, :system_date, :applicant_id, :applicant_name, "
                   " :zip, :income, :app_qualified, :app_amt, :period, "
                   " :apr, :app_fee, :branch, :emp_id, :auditor, "
                   " :owner_id1, :owner_name1, :land_num1, :relationship1, "
                   " :owner_id2, :owner_name2, :land_num2, :relationship2, "
                   " :inquiry_date);";
        else
           sql_stmt = "insert into app_info (msn, system_date, applicant_id, applicant_name, "
                   " zip, income, app_qualified, app_amt, period, "
                   " apr, app_fee, branch, emp_id, auditor, "
                   " owner_id1, owner_name1, land_num1, relationship1, "
                   " inquiry_date) values "
                   "(:msn, :system_date, :applicant_id, :applicant_name, "
                   " :zip, :income, :app_qualified, :app_amt, :period, "
                   " :apr, :app_fee, :branch, :emp_id, :auditor, "
                   " :owner_id1, :owner_name1, :land_num1, :relationship1, "
                   " :inquiry_date);";

        sql_stmt = sql_stmt.UpperCase();
        Data->command->CommandText = sql_stmt;
        Data->command->Parameters->ParamValues["msn"] = msn;
        Data->command->Parameters->ParamValues["system_date"] = system_date;
        Data->command->Parameters->ParamValues["applicant_id"] = medtPrimaryID->Text;
        Data->command->Parameters->ParamValues["applicant_name"] = edtPrimaryName->Text;
        Data->command->Parameters->ParamValues["zip"] = medtZip->Text;
        income = StrToInt(edtIncome->Text) * 10000;
        Data->command->Parameters->ParamValues["income"] = income;
        Data->command->Parameters->ParamValues["app_qualified"] = rgAppQualified->ItemIndex;
        Data->command->Parameters->ParamValues["app_amt"] = StrToInt(edtAppAmount->Text) * 10000;
//        switch(cbPeriod->ItemIndex) {
//           case 1: period = 3 * 12; break;
//           case 2: period = 5 * 12; break;
//           case 3: period = 7 * 12; break;
//        }
        period = cbPeriod->ItemIndex * 12;
        Data->command->Parameters->ParamValues["period"] = period;
        Data->command->Parameters->ParamValues["branch"] = edtBranch->Text;
        Data->command->Parameters->ParamValues["auditor"] = edtAuditor->Text;
        Data->command->Parameters->ParamValues["emp_id"] = edtEmpID->Text;
        Data->command->Parameters->ParamValues["owner_id1"] = edtOwnerID1->Text;
        Data->command->Parameters->ParamValues["owner_name1"] = edtOwnerName1->Text;
        Data->command->Parameters->ParamValues["land_num1"] = edtLandNum1->Text;
        Data->command->Parameters->ParamValues["relationship1"] = relationship1->ItemIndex - 1;

        if (cbP2->Checked) {
           Data->command->Parameters->ParamValues["owner_id2"] = edtOwnerID2->Text;
           Data->command->Parameters->ParamValues["owner_name2"] = edtOwnerName2->Text;
           Data->command->Parameters->ParamValues["land_num2"] = edtLandNum2->Text;
           Data->command->Parameters->ParamValues["relationship2"] = relationship2->ItemIndex - 1;
        }
//        get_product_code(msn, product_code, Data->query);
//        get_product_feature(product_code, Data->query, apr, application_fee);
//        Data->command->Parameters->ParamValues["apr"] = apr;
//        Data->command->Parameters->ParamValues["app_fee"] = application_fee;
        Data->command->Parameters->ParamValues["apr"] = StrToFloat(edtAPR->Text) / 100.0;
        Data->command->Parameters->ParamValues["app_fee"] = StrToInt(medtAppFee->Text);
        bool contiune = check_and_cleanup_result(msn, Data->query);
        if (!contiune)
           throw Exception("申請件重複，使用者選擇終止評分。");
/*
////////// Connect to GSS EJCIC SYSTEM ////////////////////////////////
        message += "申請件資料通過檢核，正在取回聯徵資料。\n";
        lblMessage->Caption = message;
        formMain->Refresh();


        success = request_for_jcic_data(Data->ejcic_query, medtPrimaryID->Text.Trim(), query_sn, jcic_inquiry_result, ejcic_error_code);
        if (success) {
           message += "聯徵資料已取回，正在儲存聯徵資料。 ";
           lblMessage->Caption = message;
           formMain->Refresh();
           jcic_inquiry_date =
             get_store_jcic_data(Data->ejcic_connection, Data->ejcic_query, Data->command, query_sn, msn, medtPrimaryID->Text, system_date);
*/
           message += "聯徵資料已儲存，正在評分中. . .\n";
           lblMessage->Caption = message;
           formMain->Refresh();

           jcic_inquiry_date =
             get_jcic_inquriy_date(msn, Data->query);  //**** temp function
           // insert into app_info after JCIC data is successfully retrieved.
           Data->command->Parameters->ParamValues["inquiry_date"] = jcic_inquiry_date;
           Data->command->Execute();   // save to app_info
           message += "案件 " + msn + " 已新增。\n";
           lblMessage->Caption = message;
           formMain->Refresh();

           // call prescreen function
           status = DAC_SML_PRESCREEN(medtPrimaryID->Text.c_str(), msn.c_str(), jcic_inquiry_date.c_str(),
                                      ole_str.c_str(), income/12, error_msg);
           if (status < 0) {
              message = message + error_msg + "\n";
           } else if (status > 0) {
              message = message + error_msg + "\n";
              lblMessage->Caption = message;
              formMain->Refresh();
          }
/*
          else {
  //        log_error(Data->command, medtPrimaryID->Text, jcic_inquiry_result, ejcic_error_code);
            message += ("e JCIC 錯誤，代碼：" + static_cast<AnsiString>(ejcic_error_code) + " 訊息：" + jcic_inquiry_result + "\n");
            lblMessage->Caption = message;
            formMain->Refresh();
            status = -1;
          }
*/
      if(status >= 0){
        message += "評分完成。可以輸入下一筆。\n";
        lblMessage->Caption = message;
        formMain->Refresh();
        init_UI();
      }
//////////////////////////////////////////
     } // end of if(!is_input_error)
  } // end of try
  catch(Exception &E){
    message += E.Message;
    lblMessage->Caption = message;
    formMain->Refresh();
    MessageDlg(E.Message, mtWarning, TMsgDlgButtons() << mbOK, 0);
  }
//    if(lblMessage->Caption == "評分完成。可以輸入下一筆。\n")
//      init_UI();

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
// check ID
  if (medtPrimaryID->Text.Trim() == ""){
     message += "申請人身分證字號未填。";
     is_input_error = true;
  }
  else if(!check_valid_id(medtPrimaryID->Text, gender)){
     message += "申請人身分證號錯誤。";
     is_input_error = true;
  }
// check Name
  if (edtPrimaryName->Text.Trim() == ""){
     message += "申請人姓名未填。";
     is_input_error = true;
  }
// check Income
  if (edtIncome->Text.Trim() == ""){
     message += "年收入未填。";
     is_input_error = true;
  } else if (StrToInt(edtIncome->Text) < 15) {
     message += "年收入輸入錯誤。";
     is_input_error = true;
  }
// check Zip
  if (medtZip->Text.Trim() == ""){
     message += "郵遞區號未填。";
     is_input_error = true;
  }
  else if (!check_zip(medtZip->Text.Trim().c_str())){
     message += "郵遞區號錯誤\n";
     is_input_error = true;
  }

//CHECK PRODUCT INFO
// check App amount, capped at $3M by 20060826 jeff's mail
  if (edtAppAmount->Text.Trim() == ""){
     message += "申貸金額未填。";
     is_input_error = true;
  } else if (StrToInt(edtAppAmount->Text) < 10 ||StrToInt(edtAppAmount->Text) > 300) {
     message += "申貸金額輸入錯誤。";
     is_input_error = true;
  }
// check APR
  if (edtAPR->Text.Trim() == ""){
     message += "申貸利率未填。";
     is_input_error = true;
  } else if (StrToFloat(edtAPR->Text) < 0.0 || StrToFloat(edtAPR->Text) > 20.0) {
     message += "申貸利率輸入錯誤。";
     is_input_error = true;
  }
// check Period
  if (cbPeriod->ItemIndex == 0){
     message += "申貸期數未填。";
     is_input_error = true;
  }
// check App Fee
  if(medtAppFee->Text.Trim() == ""){
    message += "開辦費未填。";
    is_input_error = true;
  } else if (StrToInt(medtAppFee->Text) < 0){
    message += "開辦費輸入錯誤。";
    is_input_error = true;
  }

//CHECK PROPERTY 1
// check owner name 1
  if (edtOwnerName1->Text.Trim() == ""){
     message += "第一件擔保品所有人姓名未填。";
     is_input_error = true;
  }
// check owner id 1
  if (edtOwnerID1->Text.Trim() == ""){
     message += "第一件擔保品所有人身分證字號未填。";
     is_input_error = true;
  }
  else if (!check_valid_id(edtOwnerID1->Text, gender)){
     message += "第一件擔保品所有人身分證字號錯誤。";
     is_input_error = true;
  }
// check relationship with applicant 1
  if (relationship1->ItemIndex == 0){
     message += "第一件擔保品與申請人關係未填。";
     is_input_error = true;
  }
// check land and house num 1
  if (edtLandNum1->Text.Trim() == ""){
     message += "第一件擔保品門牌號碼未填。";
     is_input_error = true;
  }
// check lien value 1
//  if (edtLienValue1->Text == ""){
//     message += "第一件擔保品總抵押金額未填。";
//     is_input_error = true;
//  } else if (StrToInt(edtLienValue1->Text) < 0){
//     message += "第一件擔保品總抵押金額不能小於0。";
//     is_input_error = true;
//  }

//CHECK PROPERTY 2
if (cbP2->Checked == true)
{
// check owner name 2
  if (edtOwnerName2->Text.Trim() == ""){
     message += "第二件擔保品所有人姓名未填。";
     is_input_error = true;
  }
// check owner id 2
  if (edtOwnerID2->Text.Trim() == ""){
     message += "第二件擔保品所有人身分證字號未填。";
     is_input_error = true;
  } else if(!check_valid_id(edtOwnerID2->Text, gender)){
     message += "第二件擔保品所有人身分證字號錯誤。";
     is_input_error = true;
  }
// check relationship with applicant 2
  if (relationship2->ItemIndex == 0){
     message += "第二件擔保品與申請人關係未填。";
     is_input_error = true;
  }
// check land and house num 2
  if (edtLandNum2->Text.Trim() == ""){
     message += "第二件擔保品門牌號碼未填。";
     is_input_error = true;
  }
//// check lien value 2
//  if(edtLienValue2->Text == ""){
//    message += "第二件擔保品總抵押金額未填。";
//    is_input_error = true;
//  } else if (StrToInt(edtLienValue2->Text) < 0){
//    message += "第二件擔保品總抵押金額不能小於0。";
//    is_input_error = true;
//  }
}
// check Branch
  if (edtBranch->Text.Trim() == ""){
     message += "進件分行未填。";
     is_input_error = true;
  } else {
     if (!check_channel(edtBranch->Text.Trim().c_str())){
	message += "分行代號輸入錯誤。";
	is_input_error = true;
     }
  }
// check Auditor
  if(edtAuditor->Text.Trim() == ""){
    message += "徵審人員未填。";
    is_input_error = true;
  }
// check EmpID
  if(edtEmpID->Text.Trim() == ""){
    message += "進件員工未填。";
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

//CHECK PROPERTY 1
// check GAV 1
  if(gav1->Text.Trim() == ""){
    message += "第一件擔保品鑑估總值未填。";
    is_input_error = true;
  } else if (StrToInt(gav1->Text) < 0) {
    message += "第一件擔保品鑑估總值輸入錯誤。";
    is_input_error = true;
  }
// check NAV 1
  if(nav1->Text.Trim() == ""){
    message += "第一件擔保品鑑估淨值未填。";
    is_input_error = true;
  } else if (StrToInt(nav1->Text) < 0) {
    message += "第一件擔保品鑑估淨值輸入錯誤。";
    is_input_error = true;
  } else if (StrToInt(nav1->Text) > StrToInt(gav1->Text)) {
    message += "第一件擔保品鑑估淨值大於鑑估總值。";
    is_input_error = true;
  }
// check lien value 1
  if(edtLienValue1->Text.Trim() == ""){
    message += "第一件擔保品總抵押金額未填。";
    is_input_error = true;
  } else if (StrToInt(edtLienValue1->Text) < 0){
    message += "第一件擔保品總抵押金額不能小於0。";
    is_input_error = true;
  }

//CHECK PROPERTY 2
if (lblOwnerID2->Enabled == true)
{
// check GAV 2
  if(gav2->Text.Trim() == ""){
    message += "第二件擔保品鑑估總值未填。";
    is_input_error = true;
  } else if (StrToInt(gav2->Text) < 0) {
    message += "第二件擔保品鑑估總值輸入錯誤。";
    is_input_error = true;
  }
// check NAV 2
  if(nav2->Text.Trim() == ""){
    message += "第二件擔保品鑑估淨值未填。";
    is_input_error = true;
  } else if (StrToInt(nav2->Text) < 0) {
    message += "第二件擔保品鑑估淨值輸入錯誤。";
    is_input_error = true;
  } else if (StrToInt(nav2->Text) > StrToInt(gav2->Text)) {
    message += "第二件擔保品鑑估淨值大於鑑估總值。";
    is_input_error = true;
  }
// check lien value 2
  if(edtLienValue2->Text.Trim() == ""){
    message += "第二件擔保品總抵押金額未填。";
    is_input_error = true;
  } else if (StrToInt(edtLienValue2->Text) < 0){
    message += "第二件擔保品總抵押金額不能小於0。";
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
void __fastcall TformMain::cbP2Click(TObject *Sender)
{
// enable all the labels, input fields.
 if (cbP2->Checked) {
    Label36->Enabled = true;
    Label37->Enabled = true;
    Label38->Enabled = true;
    Label53->Enabled = true;
    edtOwnerID2->Enabled = true;
    edtLandNum2->Enabled = true;
    edtOwnerName2->Enabled = true;
    relationship2->Enabled = true;
 } else {
    Label36->Enabled = false;
    Label37->Enabled = false;
    Label38->Enabled = false;
    Label53->Enabled = false;
    edtOwnerID2->Enabled = false;
    edtLandNum2->Enabled = false;
    edtOwnerName2->Enabled = false;
    relationship2->Enabled = false;
 }
}
//---------------------------------------------------------------------------
void __fastcall TformMain::finalReview_ClearClick(TObject *Sender)
{
  message = "確定要清除所有的欄位？";
//  MessageDlg(message, mtWarning, TMsgDlgButtons() << mbOK, 0);
  if (MessageDlg(message, mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes) {

   //// Product Info
   //  cbPeriod2->ItemIndex = 0;
   //  edtAppFee2->Clear();
   //  edtAppAmount2->Clear();
   //  edtAPR2->Clear();

   // Property 1
     gav1->Clear();
     nav1->Clear();
     edtLienValue1->Clear();
     rgQualified1->ItemIndex = 1;
   // Property 2
     gav2->Clear();
     nav2->Clear();
     edtLienValue2->Clear();
     rgQualified2->ItemIndex = 1;

     gav1->SetFocus();
     lblMessage->Caption = "";
     message = "";
     formMain->Refresh();
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMain::SelectClick(TObject *Sender)
{
  AnsiString sql_stmt;
  int period, index;
  int monthly_income;

  lblMessage->Caption = "";
  message = "";
  formMain->Refresh();

  try {
     sql_stmt = "select a.msn, system_date, applicant_id, applicant_name, app_amt, period, apr, app_fee, "
                " owner_id1, owner_name1, land_num1, relationship1, income"
                " owner_id2, owner_name2, land_num2, relationship2, zip, inquiry_date "
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
        if (!Data->query->FieldValues["INQUIRY_DATE"].IsNull())
           hidden_InquiryDate->Caption = Data->query->FieldValues["INQUIRY_DATE"];
        else
           hidden_InquiryDate->Caption = "";
        hidden_monthly_income->Caption = Data->query->FieldValues["INCOME"]/12;
        lblPrimaryName->Caption = Data->query->FieldValues["APPLICANT_NAME"];
        lblPrimaryID->Caption = Data->query->FieldValues["APPLICANT_ID"];
        lblAppFee->Caption = Data->query->FieldValues["APP_FEE"];
        lblAPR->Caption = Data->query->FieldValues["APR"] * 100;
        lblAppAmount->Caption = Data->query->FieldValues["APP_AMT"];
        lblPeriod->Caption = Data->query->FieldValues["PERIOD"] / 12;
        lblincome->Caption = Data->query->FieldValues["INCOME"];

        lblOwnerID1->Caption = Data->query->FieldValues["OWNER_ID1"];
        lblLandHouseNum1->Caption = Data->query->FieldValues["LAND_NUM1"];
        if (!Data->query->FieldValues["OWNER_ID2"].IsNull()) {
           // enable Property 2
           lblOwnerID2->Caption = Data->query->FieldValues["OWNER_ID2"];
           lblLandHouseNum2->Caption = Data->query->FieldValues["LAND_NUM2"];
           lblOwnerID2->Enabled = true;
           lblLandHouseNum2->Enabled = true;
           edtLienValue2->Enabled = true;
           gav2->Enabled = true;
           Label106->Enabled = true;
           Label76->Enabled = true;
           Label77->Enabled = true;
           Label78->Enabled = true;
           Label79->Enabled = true;
           Label80->Enabled = true;
           Label81->Enabled = true;
           Label25->Enabled = true;
           Label27->Enabled = true;
           nav2->Enabled = true;
           rgQualified2->Enabled = true;
           GroupBox4->Enabled = true;
        }
        finalReview->Enabled = true;
        finalReview_Clear->Enabled = true;
     } else {
        lblMessage->Caption = "案件編號不存在或未通過初審。";
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
  double first_lien1;
  double first_lien2;
  double first_lien;

// check data error
  is_input_error = validate_property();

  try {
     if (!is_input_error) {
        // update app_info
        sql_stmt = "update app_info "
                   "  set nav1 = :nav1, "
                   "  gav1 = :gav1, "
                   "  firstlien1 = :firstlien1, "
                   "where msn = :msn;";
        sql_stmt = sql_stmt.UpperCase();
        Data->command->CommandText = sql_stmt;
        first_lien1 = edtLienValue1->Text.Trim().ToDouble();
        Data->command->Parameters->ParamValues["firstlien1"] = edtLienValue1->Text.Trim().ToInt();
        Data->command->Parameters->ParamValues["nav1"] = nav1->Text.Trim().ToInt();
        Data->command->Parameters->ParamValues["gav1"] = gav1->Text.Trim().ToInt();
        Data->command->Parameters->ParamValues["msn"] = lblMSN->Caption;
        Data->command->Execute();
        gav_1 = gav1->Text.Trim().ToDouble();
        nav_1 = nav1->Text.Trim().ToDouble();
        if (rgQualified1->ItemIndex == 0) {
           gav_1 = nav_1 = first_lien1 = 0;
        }

        if (lblOwnerID2->Enabled) { // if property 2 exists, update property 2 info
           first_lien2 = edtLienValue2->Text.Trim().ToDouble();
           sql_stmt = "update app_info "
                      "  set nav2 = :nav2, "
                      "  gav2 = :gav2, "
                      "  firstlien2 = :firstlien2 "
                      "where msn = :msn;";
           sql_stmt = sql_stmt.UpperCase();
           Data->command->CommandText = sql_stmt;
           Data->command->Parameters->ParamValues["firstlien2"] = edtLienValue2->Text.Trim().ToInt();
           Data->command->Parameters->ParamValues["nav2"] = nav2->Text.Trim().ToInt();
           Data->command->Parameters->ParamValues["gav2"] = gav2->Text.Trim().ToInt();
           gav_1 = gav1->Text.Trim().ToDouble();
           nav_1 = nav1->Text.Trim().ToDouble();
           Data->command->Parameters->ParamValues["msn"] = lblMSN->Caption;
           Data->command->Execute();
        if (rgQualified2->ItemIndex == 0) {
           gav_2 = nav_2 = first_lien2 = 0;
        }
        }
     // call final review function
        message += "評分中. . .\n";
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
                             ole_str.c_str(), StrToFloat(lblAppAmount->Caption)/1000.0,
                             StrToFloat(lblAPR->Caption)/100.0, period, StrToFloat(lblAppFee->Caption)/1000.0,
                             gav/1000.0, nav/1000.0, zip.c_str(), first_lien/1000.0, StrToFloat(hidden_monthly_income->Caption), error_msg);
        if (status < 0) {
            message += error_msg;
        } else if (status > 0) {
            message += error_msg;
        } else { // status == 0, i.e. prescreen pass (code = 0)
          // gav != 0 and nav != 0,
          message += error_msg;
          if (gav1 == 0 || nav1 == 0) {
              message += "，但第一擔保品資格不符。" ;
          }
          else if (lblOwnerID2->Enabled && (gav2 == 0 || nav2 == 0)) {
              message += "，但第二擔保品資格不符。" ;
          }
        }
        message += "\n評分完成。可以輸入下一筆。\n" ;
        lblMessage->Caption = message;
        formMain->Refresh();
        init_UI_final();
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


void __fastcall TformMain::FormCreate(TObject *Sender)
{
  lblMessage->Caption = "";
  formMain->Hide();
  message = "";
  report_dir = "";
  report_final_dir = "";
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
    config.getline(buffer,2048);
    report_final_dir = buffer;
  };
  config.clear();
  config.close();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::btnPreviewDirClick(TObject *Sender)
{
   Application->CreateForm(__classid(TfrmDirUI), &frmDirUI);
   frmDirUI->dlbReport->Directory = report_dir;
   frmDirUI->edtDir->Text = report_dir;
   frmDirUI->ShowModal();

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TformMain::btnPreviewReportClick(TObject *Sender)
{
  AnsiString dir_message = (report_final_dir.Length() == 0) ? static_cast<AnsiString>("程式所在目錄") : report_final_dir;
  dir_message += "\n";
  message = "報表將會儲存在: " + dir_message;
  lblMessage->Caption = message;
  lblMessage->Refresh();
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if (config){
     config << report_dir.c_str() << endl;
     config << report_final_dir.c_str() << endl;
     config.clear();
     config.close();
  };
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  unsigned int sec;
//  get_time(year, month, day, hour, min, sec);

  AnsiString report_year = primier_year->Text.Trim();
  AnsiString report_month = primier_month->Text.Trim();
  AnsiString report_date = primier_day->Text.Trim();
  year = report_year.ToInt() + 1911;
  report_year = year;
  report_month = report_month.Length() == 1 ? "0" + report_month : report_month;
  report_date = report_date.Length() == 1 ? "0" + report_date : report_date;

  AnsiString report_gen_time = report_year + report_month + report_date;

  prepare_prelimitary_report(Data->command, report_gen_time);
  bool success = generate_prelimitary_report(Data->query, report_dir, report_gen_time);
  if (!success){
     message += "審核報表產生錯誤。\n";
     lblMessage->Caption = message;

  }
  else{
     message += "審核報表已產生。";
     lblMessage->Caption = message;
  };
  formMain->Refresh();
//  clean_report(Data->command);

}

//---------------------------------------------------------------------------
void __fastcall TformMain::btnFinalReportClick(TObject *Sender)
{
  AnsiString dir_message = (report_final_dir.Length() == 0) ? static_cast<AnsiString>("程式所在目錄") : report_final_dir;
  dir_message += "\n";
  message = "報表將會儲存在: " + dir_message;
  lblMessage->Caption = message;
  lblMessage->Refresh();
  ofstream config((working_dir + "config.cfg").c_str(), ios_base::out);
  if (config){
     config << report_dir.c_str() << endl;
     config << report_final_dir.c_str() << endl;
     config.clear();
     config.close();
  };
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  unsigned int sec;
//  get_time(year, month, day, hour, min, sec);

  AnsiString report_year = final_year->Text.Trim();
  AnsiString report_month = final_month->Text.Trim();
  AnsiString report_date = final_day->Text.Trim();
  year = report_year.ToInt() + 1911;
  report_year = year;
  report_month = report_month.Length() == 1 ? "0" + report_month : report_month;
  report_date = report_date.Length() == 1 ? "0" + report_date : report_date;

  AnsiString report_gen_time = report_year + report_month + report_date;

  prepare_final_report(Data->command, report_gen_time);
  bool success = generate_final_report(Data->query, report_dir, report_gen_time);
  if (!success){
     message += "審核報表產生錯誤。\n";
     lblMessage->Caption = message;
  }
  else{
     message += "審核報表已產生。";
     lblMessage->Caption = message;
  };
  formMain->Refresh();
//  clean_report(Data->command);

}
//---------------------------------------------------------------------------
void __fastcall TformMain::btnFinalreviewDirClick(TObject *Sender)
{
   Application->CreateForm(__classid(TfrmReDirUI), &frmReDirUI);
   frmReDirUI->dlbReReport->Directory = report_final_dir;
   frmReDirUI->edtReDir->Text = report_final_dir;
   frmReDirUI->ShowModal();
}
//---------------------------------------------------------------------------



