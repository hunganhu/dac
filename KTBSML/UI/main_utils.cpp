//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Data_Module.h"
#include "main_utils.h"
#include <fstream>
#include <iostream>
#include "functions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

extern AnsiString message;
char *Relation[5] = {"本人","父母","配偶","子女","其他"};
//---------------------------------------------------------------------------
void init_UI()
{
// Applicant Info
  formMain->edtPrimaryName->Clear();
  formMain->medtPrimaryID->Clear();
//  medtPrimaryBirthYear->Clear();
//  medtPrimaryBirthMonth->Clear();
//  medtPrimaryBirthDate->Clear();
//  marriage->ItemIndex = 0;
//  edtCareer->Clear();
//  child->ItemIndex = 0;
//  education->ItemIndex = 0;
  formMain->edtIncome->Clear();
  formMain->medtZip->Clear();
  formMain->rgAppQualified->ItemIndex = 1;

// Product Info
  formMain->cbPeriod->ItemIndex = 0;
  formMain->medtAppFee->Clear();
  formMain->edtAppAmount->Clear();
  formMain->edtAPR->Clear();

// Property 1
//  edtLienValue1->Clear();
  formMain->edtOwnerID1->Clear();
  formMain->edtLandNum1->Clear();
//  lien1->ItemIndex = 1;
//  location1->ItemIndex = 0;
  formMain->edtOwnerName1->Clear();
  formMain->relationship1->ItemIndex = 0;

// Property 2
//  edtLienValue2->Clear();
  formMain->edtOwnerID2->Clear();
  formMain->edtLandNum2->Clear();
//  lien2->ItemIndex = 1;
//  location2->ItemIndex = 0;
  formMain->edtOwnerName2->Clear();
  formMain->relationship2->ItemIndex = 0;

  formMain->edtBranch->Clear();
  formMain->edtAuditor->Clear();
  formMain->edtEmpID->Clear();

  formMain->medtPrimaryID->SetFocus();
//  formMain->lblMessage->Caption = "";
  message = "";
  formMain->Refresh();
};

//---------------------------------------------------------------------------
void init_UI_final()
{
  formMain->edtMSN->Clear();

  // Property 1
  formMain->gav1->Clear();
  formMain->nav1->Clear();

  // Property 2
  formMain->gav2->Clear();
  formMain->nav2->Clear();

  formMain->gav1->SetFocus();
  formMain->lblMessage->Caption = "";
  message = "";
  formMain->Refresh();
};
//---------------------------------------------------------------------------
bool request_for_jcic_data(TADOQuery *query, const AnsiString &id, AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no)
{
  bool return_value = true;

  WideString jcic_inquiry_package_code = "004";
  WideString branch_code = "5330";//"8160":資訊室; For testing machine:GSS
  WideString user_id = "DAC";//"01922":李政達; For testing machine:AUSTIN;
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
          formMain->lblMessage->Caption = "第" + static_cast<AnsiString>(i) + " 次嘗試。eJCIC尚未將查詢送給聯徵。";
          formMain->Refresh();
        }
        else if(ejcic_inquiry_status == "S"){
          formMain->lblMessage->Caption = "第" + static_cast<AnsiString>(i) + " 次嘗試。聯徵尚未回覆查詢。";
          formMain->Refresh();
        }
        else{
          formMain->lblMessage->Caption = "eJCIC 聯徵查詢狀態無法辨識。";
          formMain->Refresh();
        }
        Sleep(5000);
        ejcic_inquiry_status = get_ejcic_inquiry_result(query, query_sn, ejcic_inquiry_result, ejcic_inquiry_return_code);
      };
    };
    if(ejcic_inquiry_status != "R"){
      jcic_inquiry_result = "聯徵仍未回傳查詢結果，本申請件處理中止請聯絡e-JCIC維護人員與聯徵連絡人員。\n";
      return_value = false;
    }
    else{
      if(ejcic_inquiry_result == "S"){
        jcic_inquiry_result = "";
        return_value = true;
      }
      else if(ejcic_inquiry_result == "F"){
        jcic_inquiry_result = "聯徵查詢錯誤，e-JCIC訊息：" + ejcic_inquiry_return_code + " 請聯絡e-JCIC維護人員與聯徵連絡人員。\n";
        return_value = false;
      }
    }
  }
  else{
    jcic_inquiry_result = "查詢錯誤，未送出至聯徵，e-JCIC訊息：" + Data->ejcic->get_JC_ReturnCode() + " 請聯絡e-JCIC維護人員。\n";
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
     command->Parameters->ParamValues["CARD_TYPE"] = ejcic_query->FieldValues["CARD_TYPE"];
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

//  sql_stmt = "INSERT INTO VAM102 (";
//  sql_stmt += "MSN, IDN, INQUIRY_DATE, DATA_DATE, MAINCODE, MAINNOTE, SUBCODE, SUBNOTE, NOTE, INPUT_TIME) ";
//  sql_stmt += "VALUES(:MSN, :IDN, :INQUIRY_DATE, :DATA_DATE, :MAINCODE, :MAINNOTE, :SUBCODE, ";
//  sql_stmt += ":SUBNOTE, :NOTE, :INPUT_TIME)";

  sql_stmt = "INSERT INTO VAM102 ( "
             " MSN, IDN, INQUIRY_DATE, DATA_DATE, MAINCODE, MAINNOTE, SUBCODE, SUBNOTE, NOTE) "
             " VALUES(:MSN, :IDN, :INQUIRY_DATE, :DATA_DATE, :MAINCODE, :MAINNOTE, :SUBCODE, "
             " :SUBNOTE, :NOTE) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_VAM102 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
    command->Parameters->ParamValues["IDN"] = idn;
    command->Parameters->ParamValues["MSN"] = msn;
    command->Parameters->ParamValues["INQUIRY_DATE"] = INQUIRY_DATE;
    command->Parameters->ParamValues["DATA_DATE"] = ejcic_query->FieldValues["DATA_DATE"];
    command->Parameters->ParamValues["MAINCODE"] = ejcic_query->FieldValues["MAINCODE"];
    command->Parameters->ParamValues["MAINNOTE"] = ejcic_query->FieldValues["MAINNOTE"];
    command->Parameters->ParamValues["SUBCODE"] = ejcic_query->FieldValues["SUBCODE"];
    command->Parameters->ParamValues["SUBNOTE"] = ejcic_query->FieldValues["SUBNOTE"];
    command->Parameters->ParamValues["NOTE"] = ejcic_query->FieldValues["NOTE"];
//    command->Parameters->ParamValues["INPUT_TIME"] = input_time;
    command->Execute();
    ejcic_query->Next();
  };

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
void prepare_prelimitary_report(TADOCommand *command, const AnsiString &report_gen_time)
{
  AnsiString sql_stmt;
  try{
    sql_stmt = " IF OBJECT_ID('tempdb..#PRELIMITARY_REPORT') IS NOT NULL"
               " DROP TABLE #PRELIMITARY_REPORT";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();

  sql_stmt =  " CREATE TABLE #PRELIMITARY_REPORT ("
              " 	MSN CHAR (14),"
              " 	PREMIER_DATE CHAR (14),"
              " 	RSCORE DECIMAL(7, 4),"
              " 	PREMIER_CODE INT,"
              " 	PREMIER_MSG VARCHAR (256),"
              " 	SYSTEM_DATE CHAR (14),"
              " 	APPLICANT_ID CHAR (10),"
              " 	APPLICANT_NAME VARCHAR (30),"
//              " 	BIRTHDAY CHAR (7),"
//              " 	MARRIAGE INT,"
//              " 	CHILD INT,"
//              " 	EDUCATION INT,"
//              " 	CAREER VARCHAR (30),"
              " 	INCOME INT,"
              " 	APP_AMT INT,"
              " 	PERIOD INT,"
              " 	APR DECIMAL(7, 4),"
              " 	APP_FEE INT,"
              " 	BRANCH CHAR (4),"
              " 	BRANCH_NAME VARCHAR (20),"
              " 	REGION_NAME VARCHAR (20),"
              " 	AGENT CHAR (4),"
              " 	OWNER_ID1 CHAR (10),"
              " 	OWNER_NAME1 VARCHAR (30),"
              " 	LAND_NUM1 VARCHAR (30),"
//              " 	LIEN1 INT,"
              " 	FIRST_LIEN1 INT,"
              " 	RELATIONSHIP1 INT,"
//              " 	LOCATION1 INT,"
//              " 	LAND_FORBIT1 INT,"
//              " 	LAND_DEMOLISH1 INT,"
//              " 	LAND_COLLECT1 INT,"
//              " 	HOUSE_RAY1 INT,"
//              " 	HOUSE_SEASAND1 INT,"
//              " 	HOUSE_DANGEROUS1 INT,"
              " 	NAV1 FLOAT,"
              " 	GAV1 FLOAT,"
//              " 	HOUSE_DAMAGE1 INT,"
//              " 	HOUSE_LENT1 INT,"
//              " 	HOUSE_MISUSE1 INT,"
//              " 	HOUSE_COMPLEX1 INT,"
//              " 	HOUSE_BASEMENT1 INT,"
              " 	OWNER_ID2 CHAR (10),"
              " 	OWNER_NAME2 VARCHAR (30),"
              " 	LAND_NUM2 VARCHAR (30),"
//              " 	LIEN2 INT,"
              " 	FIRST_LIEN2 INT,"
              " 	RELATIONSHIP2 INT,"
//              " 	LOCATION2 INT,"
//              " 	LAND_FORBIT2 INT,"
//              " 	LAND_DEMOLISH2 INT,"
//              " 	LAND_COLLECT2 INT,"
//              " 	HOUSE_RAY2 INT,"
//              " 	HOUSE_SEASAND2 INT,"
//              " 	HOUSE_DANGEROUS2 INT,"
              " 	NAV2 FLOAT,"
              " 	GAV2 FLOAT,"
//              " 	HOUSE_DAMAGE2 INT,"
//              " 	HOUSE_LENT2 INT,"
//              " 	HOUSE_MISUSE2 INT,"
//              " 	HOUSE_COMPLEX2 INT,"
//              " 	HOUSE_BASEMENT2 INT,"
              " 	ZIP CHAR (3),"
              " 	INQUIRY_DATE CHAR (8)"
              " );";

  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
/*
  sql_stmt = " INSERT INTO #PRELIMITARY_REPORT (MSN, PREMIER_DATE, RSCORE, PREMIER_CODE, PREMIER_MSG, "
             "          SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME, BIRTHDAY, MARRIAGE, CHILD, EDUCATION, CAREER, "
             "          INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
             "          LIEN1, FIRST_LIEN1, RELATIONSHIP1, LOCATION1, LAND_FORBIT1, LAND_DEMOLISH1, LAND_COLLECT1, "
             "          HOUSE_RAY1, HOUSE_SEASAND1, HOUSE_DANGEROUS1, NAV1, GAV1, HOUSE_DAMAGE1, HOUSE_LENT1, "
             "          HOUSE_MISUSE1, HOUSE_COMPLEX1, HOUSE_BASEMENT1, OWNER_ID2, OWNER_NAME2, LAND_NUM2, LIEN2, "
             "          FIRST_LIEN2, RELATIONSHIP2, LOCATION2, LAND_FORBIT2, LAND_DEMOLISH2, LAND_COLLECT2, HOUSE_RAY2, "
             "          HOUSE_SEASAND2, HOUSE_DANGEROUS2, NAV2, GAV2, HOUSE_DAMAGE2, HOUSE_LENT2, HOUSE_MISUSE2, "
             "          HOUSE_COMPLEX2, HOUSE_BASEMENT2, ZIP, INQUIRY_DATE)"
             " SELECT A.MSN, PREMIER_DATE, RSCORE, PREMIER_CODE, PREMIER_MSG, "
             "          SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME, BIRTHDAY, MARRIAGE, CHILD, EDUCATION, CAREER, "
             "          INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
             "          LIEN1, FIRST_LIEN1, RELATIONSHIP1, LOCATION1, LAND_FORBIT1, LAND_DEMOLISH1, LAND_COLLECT1, "
             "          HOUSE_RAY1, HOUSE_SEASAND1, HOUSE_DANGEROUS1, NAV1, GAV1, HOUSE_DAMAGE1, HOUSE_LENT1, "
             "          HOUSE_MISUSE1, HOUSE_COMPLEX1, HOUSE_BASEMENT1, OWNER_ID2, OWNER_NAME2, LAND_NUM2, LIEN2, "
             "          FIRST_LIEN2, RELATIONSHIP2, LOCATION2, LAND_FORBIT2, LAND_DEMOLISH2, LAND_COLLECT2, HOUSE_RAY2, "
             "          HOUSE_SEASAND2, HOUSE_DANGEROUS2, NAV2, GAV2, HOUSE_DAMAGE2, HOUSE_LENT2, HOUSE_MISUSE2, "
             "          HOUSE_COMPLEX2, HOUSE_BASEMENT2, ZIP, INQUIRY_DATE "
             " FROM APP_PREMIER A, APP_INFO B "
             " WHERE A.MSN = B.MSN "
             " AND   LEFT(PREMIER_DATE, 8) = :report_gen_time; ";
*/
  sql_stmt = " INSERT INTO #PRELIMITARY_REPORT (MSN, PREMIER_DATE, RSCORE, PREMIER_CODE, PREMIER_MSG, "
             "          SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME,  "
             "          INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
             "          FIRST_LIEN1, RELATIONSHIP1, "
             "          NAV1, GAV1, "
             "          OWNER_ID2, OWNER_NAME2, LAND_NUM2, "
             "          FIRST_LIEN2, RELATIONSHIP2, "
             "          NAV2, GAV2, "
             "          ZIP, INQUIRY_DATE)"
             " SELECT A.MSN, PREMIER_DATE, RSCORE, PREMIER_CODE, PREMIER_MSG, "
             "          SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME, "
             "          INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
             "          FIRST_LIEN1, RELATIONSHIP1, "
             "          NAV1, GAV1, "
             "          OWNER_ID2, OWNER_NAME2, LAND_NUM2, "
             "          FIRST_LIEN2, RELATIONSHIP2, "
             "          NAV2, GAV2, "
             "          ZIP, INQUIRY_DATE "
             " FROM APP_PREMIER A, APP_INFO B "
             " WHERE A.MSN = B.MSN "
             " AND   LEFT(PREMIER_DATE, 8) = :report_gen_time; ";

  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["report_gen_time"] = report_gen_time.SubString(1,8);
  command->Execute();

  sql_stmt = "UPDATE #PRELIMITARY_REPORT SET "
             "  BRANCH_NAME = B.BRANCH, "
             "  REGION_NAME = C.CFC "
             "  FROM BRANCH AS B, CFC AS C "
             "  WHERE #PRELIMITARY_REPORT.BRANCH = B.BRANCH_CODE "
             "    AND B.CFC_CODE = C.CFC_CODE ";
  command->CommandText = sql_stmt;
  command->Execute();
  }
  catch(Exception &E){
     throw;
  };

};

//---------------------------------------------------------------------------
bool generate_prelimitary_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time)
{
  AnsiString sql_stmt, sql_stmt2;

//select a.msn, a.final_date, a.pb, a.npv, a.optimal_amount, a.audit_agent, a.final_code, a.final_msg,
// system_date, applicant_id, applicant_name, birthday, marriage, child, education, career, income, app_amt, period, apr, app_fee, branch, agent, owner_id1, owner_name1, land_num1, lien1, first_lien1, relationship1, location1, land_forbit1, land_demolish1, land_collect1, house_ray1, house_seasand1, house_dangerous1, nav1, gav1, house_damage1, house_lent1, house_misuse1, house_complex1, house_basement1, owner_id2, owner_name2, land_num2, lien2, first_lien2, relationship2, location2, land_forbit2, land_demolish2, land_collect2, house_ray2, house_seasand2, house_dangerous2, nav2, gav2, house_damage2, house_lent2, house_misuse2, house_complex2, house_basement2, zip, INQUIRY_DATE
//from app_final a left join app_info b
//  on a.msn = b.msn
//where left(a.final_date, 8) = '20060428'

AnsiString msn, premier_date, premier_code, premier_msg, system_date, applicant_id, applicant_name, birthday, career;
int marriage, child, education, income, app_amt, period, app_fee;
float rscore, apr;
AnsiString branch, branch_name, region_name, agent;
AnsiString owner_id1, owner_name1, land_num1;
int lien1, first_lien1, relationship1, location1, land_forbit1, land_demolish1, land_collect1;
int house_ray1, house_seasand1, house_dangerous1;
int nav1, gav1, house_damage1, house_lent1, house_misuse1, house_complex1, house_basement1;
AnsiString owner_id2, owner_name2, land_num2;
int lien2, first_lien2, relationship2, location2, land_forbit2, land_demolish2, land_collect2;
int house_ray2, house_seasand2, house_dangerous2;
int nav2, gav2, house_damage2, house_lent2, house_misuse2, house_complex2, house_basement2;
AnsiString head_region = "EMPTY";
AnsiString head_branch = "EMPTY";
int index, branch_count;
AnsiString file_name_individual, file_name_total;
ofstream report_individual, report_total;
AnsiString premier_year, reason;

unsigned int year;
unsigned int month;
unsigned int day;
unsigned int hour;
unsigned int min;
unsigned int sec;
char report_time[20];
char premier_time[20];
char birthday_str[20];

get_time(year, month, day, hour, min, sec);
sprintf (report_time, "%03d/%02d/%02d %02d:%02d:%02d", year-1911, month, day, hour, min, sec);

  try {
/*
    sql_stmt = " SELECT MSN, PREMIER_DATE, RSCORE, PREMIER_CODE, PREMIER_MSG, "
               "   SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME, BIRTHDAY, MARRIAGE, CHILD, EDUCATION, CAREER, "
               "   INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, BRANCH_NAME, REGION_NAME, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
               "   LIEN1, FIRST_LIEN1, RELATIONSHIP1, LOCATION1, LAND_FORBIT1, LAND_DEMOLISH1, LAND_COLLECT1, "
               "   HOUSE_RAY1, HOUSE_SEASAND1, HOUSE_DANGEROUS1, NAV1, GAV1, HOUSE_DAMAGE1, HOUSE_LENT1, "
               "   HOUSE_MISUSE1, HOUSE_COMPLEX1, HOUSE_BASEMENT1, OWNER_ID2, OWNER_NAME2, LAND_NUM2, LIEN2, "
               "   FIRST_LIEN2, RELATIONSHIP2, LOCATION2, LAND_FORBIT2, LAND_DEMOLISH2, LAND_COLLECT2, HOUSE_RAY2, "
               "   HOUSE_SEASAND2, HOUSE_DANGEROUS2, NAV2, GAV2, HOUSE_DAMAGE2, HOUSE_LENT2, HOUSE_MISUSE2, "
               "   HOUSE_COMPLEX2, HOUSE_BASEMENT2, ZIP, INQUIRY_DATE "
               " FROM #PRELIMITARY_REPORT "
               " ORDER BY REGION_NAME, BRANCH, MSN; ";
*/
    sql_stmt = " SELECT MSN, PREMIER_DATE, RSCORE, PREMIER_CODE, PREMIER_MSG, "
               "   SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME, "
               "   INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, BRANCH_NAME, REGION_NAME, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
               "   FIRST_LIEN1, RELATIONSHIP1, "
               "   NAV1, GAV1, "
               "   OWNER_ID2, OWNER_NAME2, LAND_NUM2, "
               "   FIRST_LIEN2, RELATIONSHIP2, "
               "   NAV2, GAV2, "
               "   ZIP, INQUIRY_DATE "
               " FROM #PRELIMITARY_REPORT "
               " ORDER BY REGION_NAME, BRANCH, MSN; ";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);

    query->Open();
    if(query->RecordCount == 0){
      message += "沒有未產生報表的申請件，報表產生中止。\n";
      formMain->lblMessage->Caption = message;
      formMain->Refresh();
      return true;
    }

    while (!query->Eof) {
       msn = query->FieldByName("MSN")->AsString;
       premier_date = query->FieldByName("PREMIER_DATE")->AsString;
//       rscore = query->FieldByName("RSCORE")->AsFloat;
       premier_code = query->FieldByName("PREMIER_CODE")->AsString;
       premier_msg = query->FieldByName("PREMIER_MSG")->AsString;
       system_date = query->FieldByName("SYSTEM_DATE")->AsString;
       applicant_id = query->FieldByName("APPLICANT_ID")->AsString;
       applicant_name = query->FieldByName("APPLICANT_NAME")->AsString;
//       birthday = query->FieldByName("BIRTHDAY")->AsString;
//       marriage = query->FieldByName("MARRIAGE")->AsInteger;
//       child = query->FieldByName("CHILD")->AsInteger;
//       education = query->FieldByName("EDUCATION")->AsInteger;
//       career = query->FieldByName("CAREER")->AsString;
//       income = query->FieldByName("INCOME")->AsInteger;
//       app_amt = query->FieldByName("APP_AMT")->AsInteger;
//       period = query->FieldByName("PERIOD")->AsInteger;
//       apr = query->FieldByName("APR")->AsFloat;
//       app_fee = query->FieldByName("APP_FEE")->AsInteger;
       branch = query->FieldByName("BRANCH")->AsString;
       branch_name = query->FieldByName("BRANCH_NAME")->AsString;
       region_name = query->FieldByName("REGION_NAME")->AsString;
//       agent = query->FieldByName("AGENT")->AsString;

       owner_id1 = query->FieldByName("OWNER_ID1")->AsString;
       owner_name1 = query->FieldByName("OWNER_NAME1")->AsString;
       land_num1 = query->FieldByName("LAND_NUM1")->AsString;
//       lien1 = query->FieldByName("LIEN1")->AsInteger;
//       first_lien1 = query->FieldByName("FIRST_LIEN1")->AsInteger;
       relationship1 = query->FieldByName("RELATIONSHIP1")->AsInteger;
//       location1 = query->FieldByName("LOCATION1")->AsInteger;
//       land_forbit1 = query->FieldByName("LAND_FORBIT1")->AsInteger;
//       land_demolish1 = query->FieldByName("LAND_DEMOLISH1")->AsInteger;
//       land_collect1 = query->FieldByName("LAND_COLLECT1")->AsInteger;
//       house_ray1 = query->FieldByName("HOUSE_RAY1")->AsInteger;
//       house_seasand1 = query->FieldByName("HOUSE_SEASAND1")->AsInteger;
//       house_dangerous1 = query->FieldByName("HOUSE_DANGEROUS1")->AsInteger;
//       nav1 = query->FieldByName("NAV1")->AsInteger;
//       gav1 = query->FieldByName("GAV1")->AsInteger;

       owner_id2 = query->FieldByName("OWNER_ID2")->AsString;
       owner_name2 = query->FieldByName("OWNER_NAME2")->AsString;
       land_num2 = query->FieldByName("LAND_NUM2")->AsString;
//       lien2 = query->FieldByName("LIEN2")->AsInteger;
//       first_lien2 = query->FieldByName("FIRST_LIEN2")->AsInteger;
       relationship2 = query->FieldByName("RELATIONSHIP2")->AsInteger;
//       location2 = query->FieldByName("LOCATION2")->AsInteger;
//       land_forbit2 = query->FieldByName("LAND_FORBIT2")->AsInteger;
//       land_demolish2 = query->FieldByName("LAND_DEMOLISH2")->AsInteger;
//       land_collect2 = query->FieldByName("LAND_COLLECT2")->AsInteger;
//       house_ray2 = query->FieldByName("HOUSE_RAY2")->AsInteger;
//       house_seasand2 = query->FieldByName("HOUSE_SEASAND2")->AsInteger;
//       house_dangerous2 = query->FieldByName("HOUSE_DANGEROUS2")->AsInteger;
//       nav2 = query->FieldByName("NAV2")->AsInteger;
//       gav2 = query->FieldByName("GAV2")->AsInteger;

       if (head_region != region_name) {
            if (head_region != "EMPTY") {
               // close current file
               report_total << "/EOF" << endl;
               report_total.clear();
               report_total.close();
//               report_individual << "/EOF" << endl;
               report_individual.clear();
               report_individual.close();
            }
           // open new file
           file_name_total = report_dir + "Pre_" + region_name + "_" + report_gen_time + "_total.csv";
           report_total.open (file_name_total.c_str(), ios_base::out);
           head_region = region_name;
           file_name_individual = report_dir + "Pre_" + region_name + "_" + report_gen_time + "_individual.csv";
           report_individual.open(file_name_individual.c_str(), ios_base::out);
           report_individual << "region_name,branch_name,premier_time,msn,report_time,applicant_name,applicant_id,";
           report_individual << "owner_name1,owner_id1,relationship1,land_num1,owner_name2,owner_id2,relationship2,land_num2,";
           report_individual << "applicant_status,property1_status,property2_status,permier_result,premier_msg" << endl;
       }
       if (head_branch != branch) {
           // get count of current branch
           // write header
           report_total << "京城銀行淨值房貸" << endl;
           report_total << "DAC核准模組初審結果回覆總表" << endl;
           report_total << "報表產生時間：" << ",";
           report_total << year-1911 << "年 " << month << "月 " << day << "日 " << hour << "時 " << min << "分 " << endl;
           report_total << "報表所屬區中心：" << "," << region_name.c_str() << ",";
           report_total << "所屬分行：" << "," << branch_name.c_str() << endl;

           sql_stmt2 = " SELECT COUNT(*) AS CNT "
                       " FROM #PRELIMITARY_REPORT "
                       " WHERE REGION_NAME = :REGION_NAME AND BRANCH = :BRANCH ";

           sql_stmt2 = sql_stmt2.UpperCase();
           Data->query2->Close();
           Data->query2->SQL->Clear();
           Data->query2->SQL->Add(sql_stmt2);
           Data->query2->Parameters->ParamValues["REGION_NAME"] = region_name;
           Data->query2->Parameters->ParamValues["BRANCH"] = branch;
           Data->query2->Open();
           branch_count = Data->query2->FieldValues["CNT"];
           Data->query2->Close();

           report_total << "報表申請件數：" << ","<< branch_count << endl;

           report_total << endl;
           report_total << "報表號碼,審核序號,申請人資料,,,擔保品資料,,,,,,審核狀況," << endl;
           report_total << ",,申請人姓名,身分證字號,申請人資格,擔保品1所有人姓名,擔保品1門牌號碼,擔保品1資格,";
           report_total << "擔保品2所有人姓名,擔保品2門牌號碼,擔保品2資格,初審結果,初審建議" << endl;

           head_branch = branch;
           index = 0;
       }
       report_total << ++index << ",";
       report_total << msn.c_str() << ",";
       report_total << applicant_name.c_str() << ",";
       report_total << applicant_id.c_str() << ",";
       if (premier_code == 0 || premier_code == 301)
          report_total << "合格" << ",";
       else
          report_total << "不合格" << ",";
       report_total << owner_name1.c_str() << ",";
       report_total << land_num1.c_str() << ",";
       if ((lien1 > 1) || (relationship1 > 3) || (location1 > 2) ||
           ((land_forbit1 + land_demolish1 + land_collect1 + house_ray1 +
             house_seasand1 + house_dangerous1) > 0) ) {
          report_total << "不合格" << ",";
       }
       else
          report_total << "合格" << ",";
       if (owner_name2 == "") {
          report_total << ",,,";
       } else {
          report_total << owner_name2.c_str() << ",";
          report_total << land_num2.c_str() << ",";
          if ((lien2 > 2) || (relationship2 > 3) || (location2 > 2) ||
              ((land_forbit2 + land_demolish2 + land_collect2 + house_ray2 +
                house_seasand2 + house_dangerous2) > 0) ) {
             report_total << "不合格" << ",";
          }
          else
             report_total << "合格" << ",";
       }
       report_individual << region_name.c_str() << ",";
       report_individual << branch_name.c_str() << ",";
       premier_year = premier_date.SubString(1,4);
       sprintf (premier_time, "%03d/%s/%s %s:%s:%s", premier_year.ToInt()-1911,
                                                     premier_date.SubString(5,2),
                                                     premier_date.SubString(7,2),
                                                     premier_date.SubString(9,2),
                                                     premier_date.SubString(11,2),
                                                     premier_date.SubString(13,2));
//       sprintf (birthday_str, "%s/%s/%s", birthday.SubString(1,3),
//                                          birthday.SubString(4,2),
//                                          birthday.SubString(6,2));
       report_individual << premier_time << ",";
       report_individual << msn.c_str() << ",";
       report_individual << report_time << ",";
       report_individual << applicant_name.c_str() << ",";
       report_individual << applicant_id.c_str() << ",";
//       report_individual << birthday_str << ",";
       report_individual << owner_name1.c_str() << ",";
       report_individual << owner_id1.c_str() << ",";
       report_individual << Relation[relationship1] << ",";
       report_individual << land_num1.c_str() << ",";
       if (owner_name2 == "") {
          report_individual << ",,,,";
       } else {
          report_individual << owner_name2.c_str() << ",";
          report_individual << owner_id2.c_str() << ",";
          report_individual << Relation[relationship2] << ",";
          report_individual << land_num2.c_str() << ",";
       }
       if (premier_code == 0 || premier_code == 301)
          report_individual << "合格" << ",";
       else
          report_individual << "不合格（" << premier_msg.c_str() << "）,";

//       if ((lien1 > 1) || (relationship1 > 3) || (location1 > 2) ||
//           ((land_forbit1 + land_demolish1 + land_collect1 + house_ray1 +
//             house_seasand1 + house_dangerous1) > 0) ) {
//          reason = "";
//          if (lien1 > 1) reason += "抵押順位為第二順位或以上 ";
//          if (relationship1 > 3) reason += "與申請人關係為其他 ";
//          if (location1 > 2) reason += "座落區域為其他 ";
//          if (land_forbit1) reason += "土地為禁建 ";
//          if (land_demolish1) reason += "都市計畫拆除對象 ";
//          if (land_collect1) reason += "都市計畫徵收對象 ";
//          if (house_ray1) reason += "輻射屋 ";
//          if (house_seasand1) reason += "海砂屋 ";
//          if (house_dangerous1) reason += "危樓 ";
//          report_individual << "不合格（" << reason.c_str() << "）,";
//       }
//       else
          report_individual << "合格" << ",";

       if (owner_name2 == "") {
          report_individual << ",";
       } else {
//          if ((lien2 > 2) || (relationship2 > 3) || (location2 > 2) ||
//              ((land_forbit2 + land_demolish2 + land_collect2 + house_ray2 +
//                house_seasand2 + house_dangerous2) > 0) ) {
//             reason = "";
//             if (lien2 > 2) reason += "抵押順位為第二順位或以上 ";
//             if (relationship2 > 3) reason += "與申請人關係為其他 ";
//             if (location2 > 2) reason += "座落區域為其他 ";
//             if (land_forbit2) reason += "土地為禁建 ";
//             if (land_demolish2) reason += "都市計畫拆除對象 ";
//             if (land_collect2) reason += "都市計畫徵收對象 ";
//             if (house_ray2) reason += "輻射屋 ";
//             if (house_seasand2) reason += "海砂屋 ";
//             if (house_dangerous2) reason += "危樓 ";
//             report_individual << "不合格（" << reason.c_str() << "）,";
//          }
//          else
             report_individual << "合格" << ",";
       }
       if (premier_code == 0 || premier_code == 301) {
          if (premier_code == 301) {
             report_total << "不合格" << "," << "建議轉個人信貸產品" << endl;
             report_individual << "不合格" << "," << "建議轉個人信貸產品" << endl;
          }
          else {
             if (gav1 > 0) {
                if (gav2 > 0) {
                    report_total << "合格" << "," << "票交查詢通過後，進行鑑價" << endl;
                    report_individual << "合格" << "," << "票交查詢通過後，進行鑑價" << endl;
                }
                else {
                    report_total << "合格" << "," << "票交查詢通過後，依第一擔保品進行鑑價" << endl;
                    report_individual << "合格" << "," << "票交查詢通過後，依第一擔保品進行鑑價" << endl;
                }
             }
             else {
                report_total << "合格" << "," << "票交查詢通過後，依第二擔保品進行鑑價" << endl;
                report_individual << "合格" << "," << "票交查詢通過後，依第二擔保品進行鑑價" << endl;
             }
          }
       }
       else {
          report_total << "不合格" << "," << "建議婉拒" << endl;
          report_individual << "不合格" << "," << "建議婉拒" << endl;
       }
      query->Next();
    }

//    report_individual << "/EOF" << endl;
    report_total << "/EOF" << endl;
    report_total.clear();
    report_total.close();
    report_individual.clear();
    report_individual.close();
    query->Close();
    return true;
  }
  catch(Exception &E){
    formMain->lblMessage->Caption = E.Message;
    formMain->Refresh();
    return false;
  }
}
//---------------------------------------------------------------------------
void prepare_final_report(TADOCommand *command, const AnsiString &report_gen_time)
{
  AnsiString sql_stmt;
  try{
    sql_stmt = " IF OBJECT_ID('tempdb..#FINAL_REPORT') IS NOT NULL"
               " DROP TABLE #FINAL_REPORT";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();

  sql_stmt =  " CREATE TABLE #FINAL_REPORT ("
              " 	MSN CHAR (14),"
              " 	FINAL_DATE CHAR (14),"
              " 	PB DECIMAL(7, 4),"
              " 	NPV INT,"
              " 	OPTIMAL_AMOUNT INT,"
              " 	AUDIT_AGENT CHAR (4),"
              " 	FINAL_CODE INT,"
              " 	FINAL_MSG VARCHAR (256),"
              " 	SYSTEM_DATE CHAR (14),"
              " 	APPLICANT_ID CHAR (10),"
              " 	APPLICANT_NAME VARCHAR (30),"
//              " 	BIRTHDAY CHAR (7),"
//              " 	MARRIAGE INT,"
//              " 	CHILD INT,"
//              " 	EDUCATION INT,"
//              " 	CAREER VARCHAR (30),"
              " 	INCOME INT,"
              " 	APP_AMT INT,"
              " 	PERIOD INT,"
              " 	APR DECIMAL(7, 4),"
              " 	APP_FEE INT,"
              " 	BRANCH CHAR (4),"
              " 	BRANCH_NAME VARCHAR (20),"
              " 	REGION_NAME VARCHAR (20),"
              " 	AGENT CHAR (4),"
              " 	OWNER_ID1 CHAR (10),"
              " 	OWNER_NAME1 VARCHAR (30),"
              " 	LAND_NUM1 VARCHAR (30),"
//              " 	LIEN1 INT,"
              " 	FIRST_LIEN1 INT,"
              " 	RELATIONSHIP1 INT,"
//              " 	LOCATION1 INT,"
//              " 	LAND_FORBIT1 INT,"
//              " 	LAND_DEMOLISH1 INT,"
//              " 	LAND_COLLECT1 INT,"
//              " 	HOUSE_RAY1 INT,"
//              " 	HOUSE_SEASAND1 INT,"
//              " 	HOUSE_DANGEROUS1 INT,"
              " 	NAV1 FLOAT,"
              " 	GAV1 FLOAT,"
//              " 	HOUSE_DAMAGE1 INT,"
//              " 	HOUSE_LENT1 INT,"
//              " 	HOUSE_MISUSE1 INT,"
//              " 	HOUSE_COMPLEX1 INT,"
//              " 	HOUSE_BASEMENT1 INT,"
              " 	OWNER_ID2 CHAR (10),"
              " 	OWNER_NAME2 VARCHAR (30),"
              " 	LAND_NUM2 VARCHAR (30),"
//              " 	LIEN2 INT,"
              " 	FIRST_LIEN2 INT,"
              " 	RELATIONSHIP2 INT,"
//              " 	LOCATION2 INT,"
//              " 	LAND_FORBIT2 INT,"
//              " 	LAND_DEMOLISH2 INT,"
//              " 	LAND_COLLECT2 INT,"
//              " 	HOUSE_RAY2 INT,"
//              " 	HOUSE_SEASAND2 INT,"
//              " 	HOUSE_DANGEROUS2 INT,"
              " 	NAV2 FLOAT,"
              " 	GAV2 FLOAT,"
//              " 	HOUSE_DAMAGE2 INT,"
//              " 	HOUSE_LENT2 INT,"
//              " 	HOUSE_MISUSE2 INT,"
//              " 	HOUSE_COMPLEX2 INT,"
//              " 	HOUSE_BASEMENT2 INT,"
              " 	ZIP CHAR (3),"
              " 	INQUIRY_DATE CHAR (8)"
              " );";

  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
/*
  sql_stmt = " INSERT INTO #FINAL_REPORT (MSN, FINAL_DATE, PB, NPV, OPTIMAL_AMOUNT, AUDIT_AGENT, FINAL_CODE, FINAL_MSG, "
             "          SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME, BIRTHDAY, MARRIAGE, CHILD, EDUCATION, CAREER, "
             "          INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
             "          LIEN1, FIRST_LIEN1, RELATIONSHIP1, LOCATION1, LAND_FORBIT1, LAND_DEMOLISH1, LAND_COLLECT1, "
             "          HOUSE_RAY1, HOUSE_SEASAND1, HOUSE_DANGEROUS1, NAV1, GAV1, HOUSE_DAMAGE1, HOUSE_LENT1, "
             "          HOUSE_MISUSE1, HOUSE_COMPLEX1, HOUSE_BASEMENT1, OWNER_ID2, OWNER_NAME2, LAND_NUM2, LIEN2, "
             "          FIRST_LIEN2, RELATIONSHIP2, LOCATION2, LAND_FORBIT2, LAND_DEMOLISH2, LAND_COLLECT2, HOUSE_RAY2, "
             "          HOUSE_SEASAND2, HOUSE_DANGEROUS2, NAV2, GAV2, HOUSE_DAMAGE2, HOUSE_LENT2, HOUSE_MISUSE2, "
             "          HOUSE_COMPLEX2, HOUSE_BASEMENT2, ZIP, INQUIRY_DATE)"
             " SELECT A.MSN, FINAL_DATE, PB, NPV, OPTIMAL_AMOUNT, AUDIT_AGENT, FINAL_CODE, FINAL_MSG, "
             "          SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME, BIRTHDAY, MARRIAGE, CHILD, EDUCATION, CAREER, "
             "          INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
             "          LIEN1, FIRST_LIEN1, RELATIONSHIP1, LOCATION1, LAND_FORBIT1, LAND_DEMOLISH1, LAND_COLLECT1, "
             "          HOUSE_RAY1, HOUSE_SEASAND1, HOUSE_DANGEROUS1, NAV1, GAV1, HOUSE_DAMAGE1, HOUSE_LENT1, "
             "          HOUSE_MISUSE1, HOUSE_COMPLEX1, HOUSE_BASEMENT1, OWNER_ID2, OWNER_NAME2, LAND_NUM2, LIEN2, "
             "          FIRST_LIEN2, RELATIONSHIP2, LOCATION2, LAND_FORBIT2, LAND_DEMOLISH2, LAND_COLLECT2, HOUSE_RAY2, "
             "          HOUSE_SEASAND2, HOUSE_DANGEROUS2, NAV2, GAV2, HOUSE_DAMAGE2, HOUSE_LENT2, HOUSE_MISUSE2, "
             "          HOUSE_COMPLEX2, HOUSE_BASEMENT2, ZIP, INQUIRY_DATE "
             " FROM APP_FINAL A, APP_INFO B "
             " WHERE A.MSN = B.MSN "
             " AND   LEFT(FINAL_DATE, 8) = :report_gen_time; ";
*/
  sql_stmt = " INSERT INTO #FINAL_REPORT (MSN, FINAL_DATE, PB, NPV, OPTIMAL_AMOUNT, AUDIT_AGENT, FINAL_CODE, FINAL_MSG, "
             "          SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME, "
             "          INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
             "          FIRST_LIEN1, RELATIONSHIP1, "
             "          NAV1, GAV1, "
             "          OWNER_ID2, OWNER_NAME2, LAND_NUM2, "
             "          FIRST_LIEN2, RELATIONSHIP2, "
             "          NAV2, GAV2, "
             "          ZIP, INQUIRY_DATE)"
             " SELECT A.MSN, FINAL_DATE, PB, NPV, OPTIMAL_AMOUNT, AUDIT_AGENT, FINAL_CODE, FINAL_MSG, "
             "          SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME "
             "          INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
             "          FIRST_LIEN1, RELATIONSHIP1, "
             "          NAV1, GAV1, "
             "          OWNER_ID2, OWNER_NAME2, LAND_NUM2, "
             "          FIRST_LIEN2, RELATIONSHIP2, "
             "          NAV2, GAV2, "
             "          ZIP, INQUIRY_DATE "
             " FROM APP_FINAL A, APP_INFO B "
             " WHERE A.MSN = B.MSN "
             " AND   LEFT(FINAL_DATE, 8) = :report_gen_time; ";

  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["report_gen_time"] = report_gen_time.SubString(1,8);
  command->Execute();

  sql_stmt = "UPDATE #FINAL_REPORT SET "
             "  BRANCH_NAME = B.BRANCH, "
             "  REGION_NAME = C.CFC "
             "  FROM BRANCH AS B, CFC AS C "
             "  WHERE #FINAL_REPORT.BRANCH = B.BRANCH_CODE "
             "    AND B.CFC_CODE = C.CFC_CODE ";
  command->CommandText = sql_stmt;
  command->Execute();
  }
  catch(Exception &E){
     throw;
  };
};
//---------------------------------------------------------------------------
bool generate_final_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time)
{
  AnsiString sql_stmt, sql_stmt2;
//select a.msn, a.final_date, a.pb, a.npv, a.optimal_amount, a.audit_agent, a.final_code, a.final_msg,
// system_date, applicant_id, applicant_name, birthday, marriage, child, education, career, income, app_amt, period, apr, app_fee, branch, agent, owner_id1, owner_name1, land_num1, lien1, first_lien1, relationship1, location1, land_forbit1, land_demolish1, land_collect1, house_ray1, house_seasand1, house_dangerous1, nav1, gav1, house_damage1, house_lent1, house_misuse1, house_complex1, house_basement1, owner_id2, owner_name2, land_num2, lien2, first_lien2, relationship2, location2, land_forbit2, land_demolish2, land_collect2, house_ray2, house_seasand2, house_dangerous2, nav2, gav2, house_damage2, house_lent2, house_misuse2, house_complex2, house_basement2, zip, INQUIRY_DATE
//from app_final a left join app_info b
//  on a.msn = b.msn
//where left(a.final_date, 8) = '20060428'

AnsiString msn, final_date, audit_agent,  final_msg, system_date, applicant_id, applicant_name, birthday, career;
int marriage, child, education, income, app_amt, period, app_fee, final_code;
float rscore, apr, pb, npv, optimal_amount;
AnsiString branch, branch_name, region_name, agent;
AnsiString owner_id1, owner_name1, land_num1;
int lien1, first_lien1, relationship1, location1, land_forbit1, land_demolish1, land_collect1;
int house_ray1, house_seasand1, house_dangerous1;
int nav1, gav1, house_damage1, house_lent1, house_misuse1, house_complex1, house_basement1;
AnsiString owner_id2, owner_name2, land_num2;
int lien2, first_lien2, relationship2, location2, land_forbit2, land_demolish2, land_collect2;
int house_ray2, house_seasand2, house_dangerous2;
int nav2, gav2, house_damage2, house_lent2, house_misuse2, house_complex2, house_basement2;
AnsiString head_region = "EMPTY";
AnsiString head_branch = "EMPTY";
int index, branch_count;
AnsiString file_name_individual, file_name_total;
ofstream report_individual, report_total;
AnsiString final_year, reason;

unsigned int year;
unsigned int month;
unsigned int day;
unsigned int hour;
unsigned int min;
unsigned int sec;
char report_time[20];
char final_time[20];
char birthday_str[20];

get_time(year, month, day, hour, min, sec);
sprintf (report_time, "%03d/%02d/%02d %02d:%02d:%02d", year-1911, month, day, hour, min, sec);

  try {
/*
    sql_stmt = " SELECT MSN, FINAL_DATE, PB, NPV, OPTIMAL_AMOUNT, AUDIT_AGENT, FINAL_CODE, FINAL_MSG, "
       "   SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME, BIRTHDAY, MARRIAGE, CHILD, EDUCATION, CAREER, "
       "   INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, BRANCH_NAME, REGION_NAME, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
       "   LIEN1, FIRST_LIEN1, RELATIONSHIP1, LOCATION1, LAND_FORBIT1, LAND_DEMOLISH1, LAND_COLLECT1, "
       "   HOUSE_RAY1, HOUSE_SEASAND1, HOUSE_DANGEROUS1, NAV1, GAV1, HOUSE_DAMAGE1, HOUSE_LENT1, "
       "   HOUSE_MISUSE1, HOUSE_COMPLEX1, HOUSE_BASEMENT1, OWNER_ID2, OWNER_NAME2, LAND_NUM2, LIEN2, "
       "   FIRST_LIEN2, RELATIONSHIP2, LOCATION2, LAND_FORBIT2, LAND_DEMOLISH2, LAND_COLLECT2, HOUSE_RAY2, "
       "   HOUSE_SEASAND2, HOUSE_DANGEROUS2, NAV2, GAV2, HOUSE_DAMAGE2, HOUSE_LENT2, HOUSE_MISUSE2, "
       "   HOUSE_COMPLEX2, HOUSE_BASEMENT2, ZIP, INQUIRY_DATE "
       " FROM #FINAL_REPORT "
       " ORDER BY REGION_NAME, BRANCH, MSN; ";
*/
    sql_stmt = " SELECT MSN, FINAL_DATE, PB, NPV, OPTIMAL_AMOUNT, AUDIT_AGENT, FINAL_CODE, FINAL_MSG, "
       "   SYSTEM_DATE, APPLICANT_ID, APPLICANT_NAME "
       "   INCOME, APP_AMT, PERIOD, APR, APP_FEE, BRANCH, BRANCH_NAME, REGION_NAME, AGENT, OWNER_ID1, OWNER_NAME1, LAND_NUM1, "
       "   FIRST_LIEN1, RELATIONSHIP1, "
       "   NAV1, GAV1, "
       "   OWNER_ID2, OWNER_NAME2, LAND_NUM2, "
       "   FIRST_LIEN2, RELATIONSHIP2, "
       "   NAV2, GAV2, "
       "   ZIP, INQUIRY_DATE "
       " FROM #FINAL_REPORT "
       " ORDER BY REGION_NAME, BRANCH, MSN; ";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);

    query->Open();
    if(query->RecordCount == 0){
      message += "沒有未產生報表的申請件，報表產生中止。\n";
      formMain->lblMessage->Caption = message;
      formMain->Refresh();
      return true;
    }

    while (!query->Eof) {
       msn = query->FieldByName("MSN")->AsString;
       final_date = query->FieldByName("FINAL_DATE")->AsString;
       pb = query->FieldByName("PB")->AsFloat;
       npv = query->FieldByName("NPV")->AsFloat;
       optimal_amount = query->FieldByName("OPTIMAL_AMOUNT")->AsFloat;
//       final_code = query->FieldByName("FINAL_CODE")->AsInteger;
       final_msg = query->FieldByName("FINAL_MSG")->AsString;
       system_date = query->FieldByName("SYSTEM_DATE")->AsString;
       applicant_id = query->FieldByName("APPLICANT_ID")->AsString;
       applicant_name = query->FieldByName("APPLICANT_NAME")->AsString;
//       birthday = query->FieldByName("BIRTHDAY")->AsString;
//       marriage = query->FieldByName("MARRIAGE")->AsInteger;
//       child = query->FieldByName("CHILD")->AsInteger;
//       education = query->FieldByName("EDUCATION")->AsInteger;
//       career = query->FieldByName("CAREER")->AsString;
//       income = query->FieldByName("INCOME")->AsInteger;
       app_amt = query->FieldByName("APP_AMT")->AsInteger;
       period = query->FieldByName("PERIOD")->AsInteger;
       apr = query->FieldByName("APR")->AsFloat;
       app_fee = query->FieldByName("APP_FEE")->AsInteger;
       branch = query->FieldByName("BRANCH")->AsString;
       branch_name = query->FieldByName("BRANCH_NAME")->AsString;
       region_name = query->FieldByName("REGION_NAME")->AsString;
//       agent = query->FieldByName("AGENT")->AsString;

       owner_id1 = query->FieldByName("OWNER_ID1")->AsString;
       owner_name1 = query->FieldByName("OWNER_NAME1")->AsString;
       land_num1 = query->FieldByName("LAND_NUM1")->AsString;
//       lien1 = query->FieldByName("LIEN1")->AsInteger;
//       first_lien1 = query->FieldByName("FIRST_LIEN1")->AsInteger;
       relationship1 = query->FieldByName("RELATIONSHIP1")->AsInteger;
//       location1 = query->FieldByName("LOCATION1")->AsInteger;
       nav1 = query->FieldByName("NAV1")->AsInteger;
       gav1 = query->FieldByName("GAV1")->AsInteger;
//       house_damage1 = query->FieldByName("HOUSE_DAMAGE1")->AsInteger;
//       house_lent1 = query->FieldByName("HOUSE_LENT1")->AsInteger;
//       house_misuse1 = query->FieldByName("HOUSE_MISUSE1")->AsInteger;
//       house_complex1 = query->FieldByName("HOUSE_COMPLEX1")->AsInteger;
//       house_basement1 = query->FieldByName("HOUSE_BASEMENT1")->AsInteger;

       owner_id2 = query->FieldByName("OWNER_ID2")->AsString;
       owner_name2 = query->FieldByName("OWNER_NAME2")->AsString;
       land_num2 = query->FieldByName("LAND_NUM2")->AsString;
//       lien2 = query->FieldByName("LIEN2")->AsInteger;
//       first_lien2 = query->FieldByName("FIRST_LIEN2")->AsInteger;
       relationship2 = query->FieldByName("RELATIONSHIP2")->AsInteger;
//       location2 = query->FieldByName("LOCATION2")->AsInteger;
       nav2 = query->FieldByName("NAV2")->AsInteger;
       gav2 = query->FieldByName("GAV2")->AsInteger;
//       house_damage2 = query->FieldByName("HOUSE_DAMAGE2")->AsInteger;
//       house_lent2 = query->FieldByName("HOUSE_LENT2")->AsInteger;
//       house_misuse2 = query->FieldByName("HOUSE_MISUSE2")->AsInteger;
//       house_complex2 = query->FieldByName("HOUSE_COMPLEX2")->AsInteger;
//       house_basement2 = query->FieldByName("HOUSE_BASEMENT2")->AsInteger;

       if (head_region != region_name) {
            if (head_region != "EMPTY") {
               // close current file
               report_total << "/EOF" << endl;
               report_total.clear();
               report_total.close();
//               report_individual << "/EOF" << endl;
               report_individual.clear();
               report_individual.close();
            }
           // open new file
           file_name_total = report_dir + "Final_" + region_name + "_" + report_gen_time + "_total.csv";
           report_total.open (file_name_total.c_str(), ios_base::out);
           head_region = region_name;
           file_name_individual = report_dir + "Final_" + region_name + "_" + report_gen_time + "_individual.csv";
           report_individual.open(file_name_individual.c_str(), ios_base::out);
           report_individual << "region_name,branch_name,final_date,msn,report_time,applicant_name,applicant_id,";
           report_individual << "app_amt,apr,period,app_fee,owner_name1,owner_id1,relationship1,land_num1,property1_status,gav1,nav1,";
           report_individual <<"owner_name2,owner_id2,relationship2,land_num2,property2_status,gav2,nav2,";
           report_individual << "optimal_amount,pb,npv,final_msg" << endl;
       }
       if (head_branch != branch) {
           // get count of current branch
           // write header
           report_total << "京城銀行淨值房貸" << endl;
           report_total << "DAC核准模組複審結果回覆總表" << endl;
           report_total << "報表產生時間：" << ",";
           report_total << year-1911 << "年 " << month << "月 " << day << "日 " << hour << "時 " << min << "分 " << endl;
           report_total << "報表所屬區中心：" << "," << region_name.c_str() << ",";
           report_total << "所屬分行：" << "," << branch_name.c_str() << endl;

           sql_stmt2 = " SELECT COUNT(*) AS CNT "
                       " FROM #FINAL_REPORT "
                       " WHERE REGION_NAME = :REGION_NAME AND BRANCH = :BRANCH ";

           sql_stmt2 = sql_stmt2.UpperCase();
           Data->query2->Close();
           Data->query2->SQL->Clear();
           Data->query2->SQL->Add(sql_stmt2);
           Data->query2->Parameters->ParamValues["REGION_NAME"] = region_name;
           Data->query2->Parameters->ParamValues["BRANCH"] = branch;
           Data->query2->Open();
           branch_count = Data->query2->FieldValues["CNT"];
           Data->query2->Close();

           report_total << "報表申請件數：" << ","<< branch_count << endl;

           report_total << endl;
           report_total << "報表號碼,審核序號,申請人資料,,擔保品資料暨鑑價狀況,,,,,,,,,,審核結果,,," << endl;
           report_total << ",,申請人姓名,身分證字號,擔保品1所有人姓名,擔保品1門牌號碼,擔保品1鑑價結果,擔保品1毛鑑價值,擔保品1淨鑑價值,";
           report_total << "擔保品2所有人姓名,擔保品2門牌號碼,擔保品2鑑價結果,擔保品2毛鑑價值,擔保品2淨鑑價值,最高可貸金額,PB,NPV,准駁建議" << endl;

           head_branch = branch;
           index = 0;
       }

       report_total << ++index << ",\"";
       report_total << msn.c_str() << "\",";
       report_total << applicant_name.c_str() << ",";
       report_total << applicant_id.c_str() << ",";
       report_total << owner_name1.c_str() << ",";
       report_total << land_num1.c_str() << ",";
//       if ((house_damage1 + house_lent1 + house_misuse1 + house_complex1 + house_basement1) > 0) {
//          report_total << "不合格" << ",";
//          report_total << 0 << "," << 0 << ",";
//       }
//       else {
          report_total << "合格" << ",\"";
          report_total << FloatToStrF(gav1, ffNumber, 9, 0).c_str() << "\",\"" << FloatToStrF(nav1, ffNumber, 9, 0).c_str() << "\",";
//       }
       if (owner_name2 == "") {
          report_total << ",,,,,";
       } else {
          report_total << owner_name2.c_str() << ",";
          report_total << land_num2.c_str() << ",";
//          if ((house_damage2 + house_lent2 + house_misuse2 + house_complex2 + house_basement2) > 0) {
//             report_total << "不合格" << ",";
//             report_total << 0 << "," << 0 << ",";
//          }
//          else {
             report_total << "合格" << ",\"";
             report_total << FloatToStrF(gav2, ffNumber, 9, 0).c_str() << "\",\"" << FloatToStrF(nav2, ffNumber, 9, 0).c_str() << "\",";
//          }
       }
       report_total << "\"" << FloatToStrF(optimal_amount, ffNumber, 9, 0).c_str() << "\"," << pb << ",\"" << FloatToStrF(npv, ffNumber, 9, 0).c_str() << "\",";
       if (optimal_amount > 100000 && npv >0)
          report_total << "建議核准" << endl;
       else
          report_total << "建議轉個人信貸產品" << endl;

       report_individual << region_name.c_str() << ",";
       report_individual << branch_name.c_str() << ",";
       final_year = final_date.SubString(1,4);
       sprintf (final_time, "%03d/%s/%s %s:%s:%s", final_year.ToInt()-1911,
                                                          final_date.SubString(5,2),
                                                          final_date.SubString(7,2),
                                                          final_date.SubString(9,2),
                                                          final_date.SubString(11,2),
                                                          final_date.SubString(13,2));
//       sprintf (birthday_str, "%s/%s/%s", birthday.SubString(1,3),
//                                          birthday.SubString(4,2),
//                                          birthday.SubString(6,2));
       report_individual << final_time << ",\"";
       report_individual << msn.c_str() << "\",";
       report_individual << report_time << ",";
       report_individual << applicant_name.c_str() << ",";
       report_individual << applicant_id.c_str() << ",";
//       report_individual << birthday_str << ",\"";
       report_individual << FloatToStrF(app_amt, ffNumber, 9, 0).c_str() << "\",";
       report_individual << apr << ",";
       report_individual << period << ",\"";
       report_individual << FloatToStrF(app_fee, ffNumber, 9, 0).c_str() << "\",";
       report_individual << owner_name1.c_str() << ",";
       report_individual << owner_id1.c_str() << ",";
       report_individual << Relation[relationship1] << ",";
       report_individual << land_num1.c_str() << ",";
//       if ((house_damage1 + house_lent1 + house_misuse1 + house_complex1 + house_basement1) > 0) {
//          reason = "";
//           if (house_damage1) reason += "屋況無人管理 ";
//           if (house_lent1) reason += "租賃契約超過五年或不定期者 ";
//           if (house_misuse1) reason += "非正常用途 ";
//           if (house_complex1) reason += "出入環境複雜 ";
//           if (house_basement1) reason += "地下室 ";
//          report_individual << "不合格（" << reason.c_str() << "）,";
//          report_individual << 0 << "," << 0 << ",";
//       }
//       else {
          report_individual << "合格" << ",\"";
          report_individual << FloatToStrF(gav1, ffNumber, 9, 0).c_str()<< "\",\"" << FloatToStrF(nav1, ffNumber, 9, 0).c_str()<< "\",";
//       }
       if (owner_name2 == "") {
          report_individual << ",,,,,,,";
       } else {
          report_individual << owner_name2.c_str() << ",";
          report_individual << owner_id2.c_str() << ",";
          report_individual << Relation[relationship2] << ",";
          report_individual << land_num2.c_str() << ",";
//          if ((house_damage2 + house_lent2 + house_misuse2 + house_complex2 + house_basement2) > 0) {
//              reason = "";
//              if (house_damage2) reason += "屋況無人管理 ";
//              if (house_lent2) reason += "租賃契約超過五年或不定期者 ";
//              if (house_misuse2) reason += "非正常用途 ";
//              if (house_complex2) reason += "出入環境複雜 ";
//              if (house_basement2) reason += "地下室 ";
//             report_individual << "不合格（" << reason.c_str() << "）,";
//             report_individual << 0 << "," << 0 << ",";
//          }
//          else {
             report_individual << "合格" << ",\"";
             report_individual << FloatToStrF(gav2, ffNumber, 9, 0).c_str()<< "\",\"" << FloatToStrF(nav2, ffNumber, 9, 0).c_str()<< "\",";
//          }
       }
       report_individual << "\"" << FloatToStrF(optimal_amount, ffNumber, 9, 0).c_str() << "\"," << pb << ",\"" << FloatToStrF(npv, ffNumber, 9, 0).c_str() << "\",";
       if (optimal_amount >= 100000 && npv > 0)
          report_individual << "建議核准" << endl;
       else
          report_individual << "建議轉個人信貸產品" << endl;

       query->Next();
    }

//    report_individual << "/EOF" << endl;
    report_total << "/EOF" << endl;
    report_total.clear();
    report_total.close();
    report_individual.clear();
    report_individual.close();
    query->Close();
    return true;
  }
  catch(Exception &E){
    formMain->lblMessage->Caption = E.Message;
    formMain->Refresh();
    return false;
  }
}
//---------------------------------------------------------------------------
AnsiString get_applicant_name(const AnsiString &msn, TADOQuery *query)
{
  AnsiString return_value = "";
  AnsiString sql_stmt = "SELECT CName FROM MAIL_LIST WHERE MSN = :msn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  if(!query->FieldValues["CNAME"].IsNull())
    return_value = query->FieldValues["CNAME"];
  return return_value;
}
//---------------------------------------------------------------------------
bool check_msn(const AnsiString &msn, TADOQuery *query)
{
  bool return_value = true;

  AnsiString sql_stmt;
  sql_stmt = "SELECT COUNT(*) AS CNT FROM MAIL_LIST WHERE MSN = :msn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  if(query->FieldValues["CNT"] == 0)
    return_value = false;
  return return_value;
};
//---------------------------------------------------------------------------
//Check on id based on the pre-loaded msn_idn table
bool check_id(const AnsiString &idn, const AnsiString &msn, TADOQuery *query)
{
  bool result = false;
  AnsiString idn_mail;

  AnsiString sql_stmt;
  sql_stmt = "SELECT IDN FROM MAIL_LIST WHERE MSN = :msn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  idn_mail = query->FieldValues["IDN"];
  if (idn_mail.Trim() == idn.Trim())
     result = true;

  return result;
};
//---------------------------------------------------------------------------
void get_product_feature(const AnsiString &product_code, TADOQuery *query, double &apr, unsigned int &application_fee)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM PRODUCT_LOOKUP WHERE CODE = :product_code";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["product_code"] = product_code;
  query->Open();

  apr = query->FieldValues["RATE"];
  application_fee = query->FieldValues["APPLICATION_FEE"];
  query->Close();
};

//---------------------------------------------------------------------------
AnsiString get_jcic_inquriy_date(const AnsiString &msn, TADOQuery *query)
{
  AnsiString sql_stmt;
  AnsiString jcic_inquiry_date;

  sql_stmt = "SELECT INQUIRY_DATE FROM MAIL_LIST WHERE MSN = :msn";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  jcic_inquiry_date = query->FieldValues["INQUIRY_DATE"];
  query->Close();
  return(jcic_inquiry_date);
};
//---------------------------------------------------------------------------
void get_product_code(const AnsiString &msn, AnsiString &product_code, TADOQuery *query)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT PRODUCT_CODE FROM MAIL_LIST WHERE MSN = :msn";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  product_code = query->FieldValues["PRODUCT_CODE"];
  query->Close();
};
//---------------------------------------------------------------------------
bool check_and_cleanup_result(const AnsiString &msn, TADOQuery *query)
{
  AnsiString sql_stmt;
  bool return_value = true;
  sql_stmt = "SELECT COUNT(*) AS CNT FROM APP_PREMIER WHERE MSN = :msn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  int results = query->FieldValues["CNT"];
  query->Close();
  if(results > 0){
    if(MessageDlg("本申請件已評分，是否要重新評分?",
               mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes){
      sql_stmt = "DELETE FROM APP_PREMIER WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM APP_FINAL WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM APP_INFO WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM BAM086 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM JAS002 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM KRM021 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM KRM023 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM KRM037 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM STM007 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
      sql_stmt = "DELETE FROM VAM102 WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
    }
    else
      return_value = false;
  }
  return return_value;
}
//---------------------------------------------------------------------------
bool check_and_cleanup_final_result(const AnsiString &msn, TADOQuery *query)
{
  AnsiString sql_stmt;
  bool return_value = true;
  sql_stmt = "SELECT COUNT(*) AS CNT FROM APP_FINAL WHERE MSN = :msn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["msn"] = msn;
  query->Open();
  int results = query->FieldValues["CNT"];
  query->Close();
  if(results > 0){
    if(MessageDlg("本申請件已評分，是否要重新評分?",
               mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes){
      sql_stmt = "DELETE FROM APP_FINAL WHERE MSN = :msn";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["msn"] = msn;
      query->ExecSQL();
    }
    else
      return_value = false;
  }
  return return_value;
}
//---------------------------------------------------------------------------

