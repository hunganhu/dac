#include <time.h>
#include "common.h"
#include "dm.h"

bool request_for_jcic_data(TADOQuery *query, const AnsiString &id, AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no)
{
  bool return_value = true;
  WideString jcic_inquiry_package_code = "001";
  WideString branch_code = "5400";//"8160":資訊室; For testing machine:GSS
  WideString user_id = "DAC_1";//"01922":李政達; For testing machine:AUSTIN;
  WideString jcic_inquiry_criteria = id.Trim() + "^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^^|^";
  WideString inquiry_style = "D"; //for data; "H" for html
  WideString AS400 = "N";
  WideString inquiry_reason = "A4A";
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
      for(int i=1; i < 4; ++i){
        if(ejcic_inquiry_status == "R")
          break;
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
};

AnsiString generate_case_sn(TADOQuery *query)
{
  AnsiString sql_stmt;
  AnsiString return_value;
  int year;
  int month;
  int day;
  int counter;
  tm *today;
  time_t ltime;
  ltime = time( NULL );
  today = localtime( &ltime );
  year = today->tm_year + 1900;
  month = today->tm_mon + 1;
  day = today->tm_mday;
  AnsiString str_year = static_cast<AnsiString>(year);
  AnsiString str_month = static_cast<AnsiString>(month);
  AnsiString str_date = static_cast<AnsiString>(day);
  str_month = str_month.Length() == 1 ? "0" + str_month : str_month;
  str_date = str_date.Length() == 1 ? "0" + str_date : str_date;
  return_value = str_year + str_month + str_date;

  sql_stmt = "SELECT COUNT(*) AS CNT FROM APP_INFO WHERE LEFT(CASE_NO, 8) = :app_date;";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["APP_DATE"] = return_value;
  query->Open();
  counter = query->FieldValues["CNT"] + 1;

  AnsiString serial = static_cast<AnsiString>(counter);
  int length = serial.Length();
  for(int i = 0; i < 5 - length; ++i)
     serial = "0" + serial;
  return_value = return_value + serial;
  return return_value;
};

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
};

AnsiString get_store_jcic_data(TADOConnection *ejcic_connection,
                               TADOQuery *ejcic_query, TADOCommand *command,
                               const AnsiString &query_sn, const AnsiString &idn,
                               const AnsiString &case_no)
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

  unsigned int mg_year = INQUIRY_DATE.SubString(1,4).ToInt() - 1911;
  AnsiString str_mg_year = static_cast<AnsiString>(mg_year);
  str_mg_year = str_mg_year.Length() == 2 ? "0" + str_mg_year : str_mg_year;
  INQUIRY_DATE = str_mg_year + "/" + INQUIRY_DATE.SubString(5,2) + "/" + INQUIRY_DATE.SubString(7,2);


  sql_stmt = "INSERT INTO KRM023(CASE_NO, IDN, INQUIRY_DATE, YRMON, ISSUE, ISSUE_NAME, ";
  sql_stmt += "KR_CODE, LIMIT, PAYMENT, CASH, PAY_CODE) VALUES(:case_no, :idn, ";
  sql_stmt += ":inquiry_date, :yrmon, :issue, :issue_name, :kr_code, :limit, ";
  sql_stmt += ":payment, :cash, :pay_code);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_KRM023 WHERE QSEQNO = :query_sn";
  ejcic_sql_stmt = ejcic_sql_stmt.UpperCase();
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["idn"] = idn;
  	command->Parameters->ParamValues["case_no"] = case_no;
	  command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["yrmon"] = ejcic_query->FieldValues["YRMON"];
	  command->Parameters->ParamValues["issue"] = ejcic_query->FieldValues["ISSUE"];
 	  command->Parameters->ParamValues["issue_name"] = ejcic_query->FieldValues["ISSUE_NAME"];
	  command->Parameters->ParamValues["kr_code"] = ejcic_query->FieldValues["KR_CODE"];
	  command->Parameters->ParamValues["limit"] = ejcic_query->FieldValues["LIMIT"];
	  command->Parameters->ParamValues["payment"] = ejcic_query->FieldValues["PAYMENT"];
	  command->Parameters->ParamValues["cash"] = ejcic_query->FieldValues["CASH"];
	  command->Parameters->ParamValues["pay_code"] = ejcic_query->FieldValues["PAY_CODE"];
    command->Execute();
    ejcic_query->Next();
  };


  sql_stmt = "INSERT INTO KRM021(CASE_NO, IDN, INQUIRY_DATE, CARD_BRAND, CARD_TYPE, ISSUE, ";
  sql_stmt += "ISSUE_NAME, START_DATE, STOP_DATE, STOP_CODE, AB_CODE, ";
  sql_stmt += "M_S, RELA, LIMIT, RISK, CLEAR_DATE, IDN_PRI, CNAME, REMARK) VALUES(:case_no, :idn, ";
  sql_stmt += ":inquiry_date, :card_brand, :card_type, :issue, ";
  sql_stmt += ":issue_name, :start_date, :stop_date, :stop_code, :ab_code, :m_s, :rela, :limit, ";
  sql_stmt += ":risk, :clear_date, :pidn, :cname, :remark)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_KRM021 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
    command->Parameters->ParamValues["idn"] = idn;
    command->Parameters->ParamValues["case_no"] = case_no;
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
    command->Execute();
    ejcic_query->Next();
  };

  sql_stmt = "INSERT INTO KRM037 (";
  sql_stmt += "CASE_NO, IDN, INQUIRY_DATE, BILL_DATE, ISSUE, ISSUE_NAME, CARD_TYPE, ";
  sql_stmt += "PERM_LIMIT, TEMP_LIMIT, CASH_LIMIT, PAYABLE, CASH_LENT, ";
  sql_stmt += "LAST_PAYA, REVOL_BAL, PAY_STAT, PAY_CODE, REVOL_RATE, ";
  sql_stmt += "PRE_OWED, DEBT_CODE, CLOSE_CODE, CLEAR_DATE) VALUES(";
  sql_stmt += ":CASE_NO, :IDN, :INQUIRY_DATE, :BILL_DATE, :ISSUE, :ISSUE_NAME, :CARD_TYPE, ";
  sql_stmt += ":PERM_LIMIT, :TEMP_LIMIT, :CASH_LIMIT, :PAYABLE, :CASH_LENT, ";
  sql_stmt += ":LAST_PAYA, :REVOL_BAL, :PAY_STAT, :PAY_CODE, :REVOL_RATE, ";
  sql_stmt += ":PRE_OWED, :DEBT_CODE, :CLOSE_CODE, :CLEAR_DATE)";
  sql_stmt = sql_stmt.UpperCase();
	command->CommandText = sql_stmt;
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_KRM037 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["CASE_NO"] = case_no;
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
    command->Execute();
    ejcic_query->Next();
  };

  sql_stmt = "INSERT INTO BAM086(CASE_NO, IDN, INQUIRY_DATE, DATA_YYY, DATA_MM, BANK_CODE, ";
  sql_stmt += "BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, ";
  sql_stmt += "LOAN_AMT, PASS_DUE_AMT, PAY_CODE_12, CO_LOAN) VALUES (:case_no, :idn, ";
  sql_stmt += ":inquiry_date, :data_yyy, :data_mm, :bank_code, :bank_name, ";
  sql_stmt += ":account_code, :account_code2, :purpose_code, :contract_amt, ";
  sql_stmt += ":loan_amt, :pass_due_amt, :pay_code_12, :co_loan);";
	command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_BAM086 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["idn"] = idn;
  	command->Parameters->ParamValues["case_no"] = case_no;
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
    command->Execute();
    ejcic_query->Next();
  };


  sql_stmt = "INSERT INTO JAS002(CASE_NO, IDN, INQUIRY_DATE, EVER_DELINQUENT, DELINQUENT_DATE, ";
  sql_stmt += "EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, ";
  sql_stmt += "EVER_STOP_CARD, STOP_CARD_DATE) VALUES (:case_no, :idn, :inquiry_date, ";
  sql_stmt += ":ever_delinquent, :delinquent_date, :ever_bad_check, ";
  sql_stmt += ":bad_check_date, :ever_reject, :reject_date, :ever_stop_card, ";
  sql_stmt += ":stop_card_date);";
	command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_JAS002 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["idn"] = idn;
  	command->Parameters->ParamValues["case_no"] = case_no;
	  command->Parameters->ParamValues["inquiry_date"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["ever_delinquent"] = ejcic_query->FieldValues["EVER_DELINQUENT"];
	  command->Parameters->ParamValues["delinquent_date"] = ejcic_query->FieldValues["DELINQUENT_DATE"];
 	  command->Parameters->ParamValues["ever_bad_check"] = ejcic_query->FieldValues["EVER_BAD_CHECK"];
	  command->Parameters->ParamValues["bad_check_date"] = ejcic_query->FieldValues["BAD_CHECK_DATE"];
	  command->Parameters->ParamValues["ever_reject"] = ejcic_query->FieldValues["EVER_REJECT"];
	  command->Parameters->ParamValues["reject_date"] = ejcic_query->FieldValues["REJECT_DATE"];
	  command->Parameters->ParamValues["ever_stop_card"] = ejcic_query->FieldValues["EVER_STOP_CARD"];
	  command->Parameters->ParamValues["stop_card_date"] = ejcic_query->FieldValues["STOP_CARD_DATE"];
    command->Execute();
    ejcic_query->Next();
  };

  sql_stmt = "INSERT INTO STM007(CASE_NO, IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, BANK_NAME, ";
  sql_stmt += "ITEM_LIST, INQ_PURPOSE_1, INQ_PURPOSE) VALUES (:case_no, :idn, :INQUIRY_DATE, :query_date, ";
  sql_stmt += ":bank_code, :bank_name, :item_list, :inq_purpose_1, :inq_purpose);";
	command->CommandText = sql_stmt;

  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_STM007 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["idn"] = idn;
  	command->Parameters->ParamValues["case_no"] = case_no;
	  command->Parameters->ParamValues["query_date"] = ejcic_query->FieldValues["QUERY_DATE"];
    command->Parameters->ParamValues["INQUIRY_DATE"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["bank_code"] = ejcic_query->FieldValues["BANK_CODE"];
	  command->Parameters->ParamValues["bank_name"] = ejcic_query->FieldValues["BANK_NAME"];
 	  command->Parameters->ParamValues["item_list"] = ejcic_query->FieldValues["ITEM_LIST"];
	  command->Parameters->ParamValues["inq_purpose_1"] = ejcic_query->FieldValues["INQ_PURPOSE_1"];
	  command->Parameters->ParamValues["inq_purpose"] = ejcic_query->FieldValues["INQ_PURPOSE"];
    command->Execute();
    ejcic_query->Next();
  };

  sql_stmt = "INSERT INTO VAM102 (";
  sql_stmt += "CASE_NO, IDN, INQUIRY_DATE, DATA_DATE, MAINCODE, MAINNOTE, SUBCODE, SUBNOTE, NOTE) ";
  sql_stmt += "VALUES(:CASE_NO, :IDN, :INQUIRY_DATE, :DATA_DATE, :MAINCODE, :MAINNOTE, :SUBCODE, ";
  sql_stmt += ":SUBNOTE, :NOTE)";
  sql_stmt = sql_stmt.UpperCase();
	command->CommandText = sql_stmt;
  ejcic_sql_stmt = "SELECT * FROM TNBB_JCIC.MQ_T_VAM102 WHERE QSEQNO = :query_sn";
  ejcic_query->SQL->Clear();
  ejcic_query->SQL->Add(ejcic_sql_stmt);
  ejcic_query->Parameters->ParamValues["query_sn"] = query_sn;
  ejcic_query->Open();
  while (!ejcic_query->Eof){
  	command->Parameters->ParamValues["IDN"] = idn;
  	command->Parameters->ParamValues["CASE_NO"] = case_no;
    command->Parameters->ParamValues["INQUIRY_DATE"] = INQUIRY_DATE;
	  command->Parameters->ParamValues["DATA_DATE"] = ejcic_query->FieldValues["DATA_DATE"];
	  command->Parameters->ParamValues["MAINCODE"] = ejcic_query->FieldValues["MAINCODE"];
	  command->Parameters->ParamValues["MAINNOTE"] = ejcic_query->FieldValues["MAINNOTE"];
 	  command->Parameters->ParamValues["SUBCODE"] = ejcic_query->FieldValues["SUBCODE"];
	  command->Parameters->ParamValues["SUBNOTE"] = ejcic_query->FieldValues["SUBNOTE"];
	  command->Parameters->ParamValues["NOTE"] = ejcic_query->FieldValues["NOTE"];
    command->Execute();
    ejcic_query->Next();
  };

  return INQUIRY_DATE;
};

AnsiString fill_in(const AnsiString &source, int digits)
{
  AnsiString return_value;
  int current = source.Trim().Length();
  return_value = source.Trim();
  for(int i = 0; i < digits - current; ++i)
    return_value = ("0" + return_value);
  return return_value;
}
