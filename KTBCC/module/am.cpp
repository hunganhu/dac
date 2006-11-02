//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#include <time.h>
#include "AM.h"
#pragma hdrstop
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  return 1;
}
//---------------------------------------------------------------------------
int TNB_CC_AM(char *idn, char *jcic_inquiry_date, char *app_input_time,
              char *bank_code, char *ole_db_str, int gender, char *error)
{

  AnsiString idno = static_cast<AnsiString>(idn);
  AnsiString jcic_date = static_cast<AnsiString>(jcic_inquiry_date);
  AnsiString input_time = static_cast<AnsiString>(app_input_time);
  AnsiString error_message = "";
  AnsiString connection_string = static_cast<AnsiString>(ole_db_str);
  int status = 0;
  AnsiString special_note= "";
  AnsiString version = "1.53";

  AnsiString bank = static_cast<AnsiString>(bank_code);
  if(bank!= "054")
    throw cc_error(1, idno, jcic_date);

  CoInitialize(NULL);
  TADOConnection *connection = new TADOConnection(NULL);
  TADOQuery *query = new TADOQuery(NULL);
  TADOQuery *query2 = new TADOQuery(NULL);
  TADOCommand *command = new TADOCommand(NULL);
  int command_timeout = 300;
  bool timelock = true;
  bool archive = true;
  bool no_archive = archive ? false : true;
  bool use_krm037 = true;
  unsigned int unsecured_balance_opt = 0;
  unsigned int unsecured_balance_per = 0;
  unsigned int highest_line = 0;
  bool p1 = false;



  try{

    int now = yrmon_to_mon(jcic_date, timelock, "20070131", no_archive, use_krm037);

    if(now > 0){
      connect(connection, connection_string);
      initialize_ado_objects(connection, query, query2, command, command_timeout);

      copy_table(command, "KRM021", KRM001, idno, jcic_date, input_time);
      copy_table(command, "KRM023", KRM023, idno, jcic_date, input_time);
      copy_table(command, "KRM037", KRM037, idno, jcic_date, input_time);
//      copy_table(command, "KRM034", KRM034, idno, jcic_date, input_time);
      copy_table(command, "BAM086", BAM086, idno, jcic_date, input_time);
      copy_table(command, "STM007", STM001, idno, jcic_date, input_time);
      copy_table(command, "JAS002", JAS002, idno, jcic_date, input_time);

      vam102_message(query, idno, jcic_date, input_time, special_note);

//      prepare_KRM023(command, KRM023, now);
      merge_prepare_KRM023_KRM037(command, KRM023, KRM037, now);
//      merge_prepare_KRM023_KRM034(command, KRM023, KRM034, now);
      prepare_KRM001(command, KRM001, now);
      prepare_BAM086(command, BAM086, now);
      prepare_STM001(command, STM001, "054");
      prepare_JAS002(command, JAS002, JAS002_T);

      get_idn_list(command, KRM001, KRM023, BAM086, STM001, JAS002_T);
      create_common_working_tables(command);
      prepare_2xx_infra(command, now);
      unsigned int data_flag = get_data_availability(query, idno);
      unsigned int filter_flag = in_daco41(query, KRM023, KRM001, JAS002_T, BAM086, STM001, idno, data_flag, now, unsecured_balance_opt, unsecured_balance_per, highest_line, p1);

//      if (filter_flag > 7) //for data insufficiency
      if (filter_flag > 0)
        prescreen_out(filter_flag, idno, input_time);
      else{
        double drisk_score;
        double propensity_score;
        double pb = 0;
        int optimal_line = 0;
        double dollar_bad = 0;
        int result_code = 0;
        int risk_cut;
        int propensity_cut;
        AnsiString result_message;
        if(filter_flag == 0){
     	    drisk_score = daco41(query, command, KRM023, KRM001, JAS002_T, BAM086, STM001, idno,
                               gender, now, filter_flag, data_flag);
          propensity_score = propensity(command, query, idno, now, filter_flag, data_flag);
          optimal_line = credit_card_line(query, drisk_score, propensity_score, highest_line, risk_cut, propensity_cut, pb, dollar_bad, p1);
        }
//Handle data insufficiency
        else
          data_insufficiency_model(query, command,
                                   KRM023, STM001, BAM086, idno, now,
                                   data_flag, pb, optimal_line, dollar_bad);
        if(pb == 0 && dollar_bad == 0)
             throw cc_error(201, idno, input_time);
        if(optimal_line == 0){
          result_message = "模組建議婉拒，貢獻度過低。";
          result_code = 106;
        }
        else{
          result_message = "模組建議核准。";
          result_code = 0;
        };
        store_result(command, idno, input_time, result_code, result_message,
                     optimal_line, unsecured_balance_opt, pb,
                     risk_cut, propensity_cut, dollar_bad, special_note, version, true); //normal output
      };
    }
    else{
      throw cc_error(0, idno, input_time);
    };
  }
  catch(cc_error &Err){
    status = -1;
    error_message += Err.ShowMessage();
    if(Err.error_code() > 100)
      store_result(command, Err.idn(), Err.app_date(),
                   Err.error_code(), Err.ShowMessage(),
                   0, unsecured_balance_opt, 0, 0, 0, 0, special_note, version, false);
  }
  catch(Exception &E){
  	error_message += E.Message;
  	status = -1;
	};
  strcpy(error,error_message.c_str());

//House keeping
  clean_up(command);
  query->Close();
  query2->Close();
  connection->Close();
  delete query;
  delete query2;
  delete command;
  delete connection;
  CoUninitialize();
  return status;
};

void connect(TADOConnection *connection, const AnsiString &connection_string,
             int connection_timeout, int command_timeout)
{
  if(connection->Connected)
    connection->Close();
  connection->CommandTimeout = command_timeout;
  connection->ConnectionTimeout = connection_timeout;

  if(connection_string != "")
    connection->ConnectionString = connection_string;

  connection->Connected = true;
};

void initialize_ado_objects(TADOConnection *connection, TADOQuery *query,
                            TADOQuery * query2, TADOCommand *command,
                            int command_timeout)
{
  if(query->Active)
    query->Close();
  query->Connection = connection;
  query->CommandTimeout = command_timeout;
  query->EnableBCD = false;
  query->CacheSize = 60;

  if(query2->Active)
    query2->Close();
  query2->Connection = connection;
  query2->CommandTimeout = command_timeout;
  query2->EnableBCD = false;

  command->Connection = connection;
  command->CommandTimeout = command_timeout;
  command->CommandType = cmdText;
};

int yrmon_to_mon(const AnsiString &inquiry_month,
                 bool time_lock, const AnsiString &lock,
                 bool archive, bool use_krm037)
{
  int year;
  int month;
  int day;
  if (inquiry_month == ""){
    tm *today;
    time_t ltime;
    ltime = time( NULL );
    today = localtime( &ltime );
    year = today->tm_year + 1900;
    month = today->tm_mon + 1;
    day = today->tm_mday;
  }
  else{
    year = inquiry_month.SubString(1,4).ToInt();
    month = inquiry_month.SubString(5,2).ToInt();
    day = inquiry_month.SubString(7,2).ToInt();
  };
  if(time_lock && (lock.Length()==8)){
    int end_year = lock.SubString(1,4).ToInt();
    int end_month = lock.SubString(5,2).ToInt();
    int end_day = lock.SubString(7,2).ToInt();
    int end_days = end_year * 365 + end_month * 30 + end_day;
    int days = year * 365 + month * 30 + day;
    if (days > end_days)
      return -1;
  };
  if((!archive) && (!use_krm037)){
    if(day < 16)
      --month;
    if(month == 0){
      month = 12;
      --year;
    };
  }
  else if ((!archive) && (use_krm037)){
    --month;
    if(month == 0){
      month = 12;
      --year;
    };
  };
  return (year - 1911) * 12 + month;
};


void copy_table(TADOCommand *command,
                const AnsiString &source_table, const AnsiString &destination_table,
                const AnsiString &idno, const AnsiString &jcic_date,
                const AnsiString &input_time)
{
    AnsiString sql_stmt;
try{
  sql_stmt = "DROP TABLE " + destination_table + ";";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
};
  sql_stmt = "SELECT * INTO " + destination_table;
  sql_stmt+= " FROM " + source_table + " WHERE INQUIRY_DATE = :jcic_date ";
  sql_stmt +="AND idn = :idno AND Input_Time = :input_time ";

  if(source_table == "KRM021"){
    sql_stmt += "GROUP BY IDN, INQUIRY_DATE, CARD_BRAND, CARD_TYPE, ISSUE, ";
    sql_stmt += "ISSUE_NAME, START_DATE, STOP_DATE, STOP_CODE, AB_CODE, M_S, ";
    sql_stmt += "LIMIT, Input_Time, RELA, RISK, CLEAR_DATE, IDN_PRI, ";
    sql_stmt += "CNAME, REMARK";
  }
  else if(source_table == "KRM023"){
    sql_stmt += "GROUP BY IDN, INQUIRY_DATE, YRMON, ISSUE, ISSUE_NAME, ";
    sql_stmt += "KR_CODE, LIMIT, PAYMENT, CASH, PAY_CODE, Input_Time";
  }
  else if(source_table == "BAM087"){
    sql_stmt += "GROUP BY IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ";
    sql_stmt += "ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT1, ";
    sql_stmt += "CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, PAY_CODE_12, CO_LOAN, ";
    sql_stmt += "Input_Time, ACT_Y_MARK, CONTRACT_AMT_Y, Inquiry_Date";
  }
  else if(source_table == "BAM086"){
    sql_stmt += "GROUP BY IDN, INQUIRY_DATE, DATA_YYY, DATA_MM, BANK_CODE, ";
    sql_stmt += "BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, ";
    sql_stmt += "CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, PAY_CODE_12, CO_LOAN, ";
    sql_stmt += "Input_Time";
  }
  else if(source_table == "STM007"){
    sql_stmt += "GROUP BY IDN, INQUIRY_DATE, QUERY_DATE, BANK_CODE, ";
    sql_stmt += "BANK_NAME, ITEM_LIST, Input_Time, INQ_PURPOSE_1, INQ_PURPOSE";
  }
  else if(source_table == "JAS002"){
    sql_stmt += "GROUP BY IDN, INQUIRY_DATE, EVER_DELINQUENT, ";
    sql_stmt += "DELINQUENT_DATE, EVER_BAD_CHECK, BAD_CHECK_DATE, ";
    sql_stmt += "EVER_REJECT, REJECT_DATE, EVER_STOP_CARD, STOP_CARD_DATE, ";
    sql_stmt += "Input_Time";
  }
  else if(source_table == "KRM037"){
    sql_stmt += "AND ISSUE <> 'TOT' ";
    sql_stmt += "GROUP BY IDN, INQUIRY_DATE, INPUT_TIME, BILL_DATE, ISSUE, ";
    sql_stmt += "ISSUE_NAME, CARD_TYPE, PERM_LIMIT, TEMP_LIMIT, CASH_LIMIT, ";
    sql_stmt += "PAYABLE, CASH_LENT, LAST_PAYA, REVOL_BAL, PAY_STAT, PAY_CODE, ";
    sql_stmt += "REVOL_RATE, PRE_OWED, DEBT_CODE, CLOSE_CODE, CLEAR_DATE";
  }
  else if(source_table == "KRM034"){
    sql_stmt += "AND ISSUE <> 'TOT' ";
    sql_stmt += "GROUP BY IDN, INQUIRY_DATE, INPUT_TIME, BILL_DATE, ISSUE, ";
    sql_stmt += "ISSUE_NAME, CARD_TYPE, PERM_LIMIT, CASH_YN, ";
    sql_stmt += "LAST_PAYA, PAY_STAT, PAY_CODE, ";
    sql_stmt += "DEBT_CODE, CLOSE_CODE, CLEAR_DATE";
  };  
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["jcic_date"] = jcic_date;
  command->Parameters->ParamValues["idno"] = idno;
  command->Parameters->ParamValues["input_time"] = input_time;
  command->Execute();
};

void prepare_KRM023(TADOCommand *command, const AnsiString &table, int now)
{
  AnsiString sql_stmt;

  sql_stmt = "UPDATE " + table + " ";
  sql_stmt += "SET ";
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
  sql_stmt += "YRMON = (CASE WHEN YRMON = '' THEN NULL ELSE YRMON END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "KR_CODE = (CASE WHEN KR_CODE = '' THEN NULL ELSE KR_CODE END), ";
  sql_stmt += "LIMIT = (CASE WHEN LIMIT = '' THEN NULL ELSE LIMIT END), ";
  sql_stmt += "PAYMENT = (CASE WHEN PAYMENT = '' THEN NULL ELSE PAYMENT END), ";
  sql_stmt += "CASH = (CASE WHEN CASH = '' THEN NULL ELSE CASH END), ";
  sql_stmt += "PAY_CODE = (CASE WHEN PAY_CODE = '' THEN NULL ELSE PAY_CODE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + table + " ";
  sql_stmt += "ADD PAYMENT_AMT FLOAT, BUCKET_DEF_1K INT, BUCKET_EF_1K INT, BUCKET_F_1K INT, MON_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Change YRMON to proper format so that year of 99 will be smaller than year of 100
  sql_stmt = "UPDATE " + table + " SET YRMON = ";
  sql_stmt += "(CASE WHEN LEFT(LTRIM(RTRIM(YRMON)),1) NOT IN ('1', '0', '*') ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(YRMON)) ELSE LTRIM(RTRIM(YRMON))  END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Put value INTO MON_SINCE, MON_SINCE means that month since the first month
  //of the establishment of ROC; actually it IS 12 larger than it logically should be
  //But since our use on the column IS to calculate the difference, it IS OK
  sql_stmt = "UPDATE " + table + " SET MON_SINCE = ";
  sql_stmt += "(CASE WHEN LEFT(YRMON,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(YRMON, 3) * 12 + RIGHT(LEFT(YRMON, 5), 2) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
//For two columns payment
/*  sql_stmt = "UPDATE " + table + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE PAYMENT2 WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(PAYMENT,0)-1) / 1000.0;";
  command->CommandText = sql_stmt;
  command->Execute();*/
//For one column payment
  sql_stmt = "UPDATE " + table + " SET PAYMENT = ";
  sql_stmt += "(CASE WHEN RIGHT(LEFT(PAYMENT,2),1) IN ('H', 'M', 'L') THEN '0' + (LEFT(PAYMENT,2)) ELSE PAYMENT END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
  sql_stmt = "UPDATE " + table + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE RIGHT(PAYMENT,1) WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(LEFT(PAYMENT,2),0)-1) / 1000.0;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Calculate the buckets, first initialize the columns
  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "BUCKET_DEF_1K = 0, BUCKET_EF_1K = 0, BUCKET_F_1K = 0;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The second phase of columns initialization for bucket assignment
  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "BUCKET_DEF_1K = (CASE WHEN PAY_CODE in ('D', 'E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K = (CASE WHEN PAY_CODE in ('E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K = (CASE WHEN PAY_CODE = 'F' AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The bucket assignment process
  sql_stmt = "DECLARE @i INT SET @i=12 ";
  sql_stmt += "WHILE @i > 0 BEGIN UPDATE " + table + " SET ";
  sql_stmt += "BUCKET_DEF_1K =(CASE WHEN " + table + ".PAY_CODE IN ('D', 'E', 'F') ";
  sql_stmt += " AND " + table + ".PAYMENT_AMT > 1 THEN A.BUCKET_DEF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K =(CASE WHEN " + table + ".PAY_CODE IN ('E', 'F') ";
  sql_stmt += "AND " + table + ".PAYMENT_AMT > 1 THEN A.BUCKET_EF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K =(CASE WHEN " + table + ".PAY_CODE = 'F' ";
  sql_stmt += "AND " + table + ".PAYMENT_AMT > 1 THEN A.BUCKET_F_1K + 1 ELSE 0 END) ";
  sql_stmt += "FROM " + table + " AS A INNER JOIN " + table + " ON ";
  sql_stmt += "A.idn = " + table + ".idn AND A.ISSUE = " + table + ".ISSUE ";
  sql_stmt += "AND A.MON_SINCE = (" + table + ".MON_SINCE - 1) ";
  sql_stmt += "WHERE (:inq_mon - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["inq_mon"] = now;
  command->Execute();
};

void merge_prepare_KRM023_KRM037(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm037, int now)
{
  AnsiString sql_stmt;
  sql_stmt = "UPDATE " + krm037 + " SET ";
  sql_stmt += "BILL_DATE = (CASE WHEN BILL_DATE = '' THEN NULL ELSE LTRIM(RTRIM(BILL_DATE)) END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "CARD_TYPE = (CASE WHEN CARD_TYPE = '' THEN NULL ELSE CARD_TYPE END), ";
  sql_stmt += "PAYABLE = (CASE WHEN PAYABLE = '' THEN NULL ELSE PAYABLE END), ";
  sql_stmt += "LAST_PAYA = (CASE WHEN LAST_PAYA = '' THEN NULL ELSE LAST_PAYA END), ";
  sql_stmt += "PAY_STAT = (CASE WHEN PAY_STAT = '' THEN NULL ELSE PAY_STAT END), ";
  sql_stmt += "PAY_CODE = (CASE WHEN PAY_CODE = '' THEN NULL ELSE PAY_CODE END), ";
  sql_stmt += "DEBT_CODE = (CASE WHEN DEBT_CODE = '' THEN NULL ELSE DEBT_CODE END), ";
  sql_stmt += "CLOSE_CODE = (CASE WHEN CLOSE_CODE = '' THEN NULL ELSE CLOSE_CODE END), ";
  sql_stmt += "CLEAR_DATE = (CASE WHEN CLEAR_DATE = '' THEN NULL ELSE CLEAR_DATE END) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE KRM023_TMP";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  try{
    sql_stmt = "DROP TABLE KRM023_TMP1";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE KRM023_TMP ";
  sql_stmt += "(IDN CHAR(11), ";
  sql_stmt += "MON_SINCE INT, ISSUE CHAR(3), ISSUE_NAME CHAR(40), LIMIT CHAR(5),";
  sql_stmt += "PAYMENT CHAR(3), PAYMENT_AMT FLOAT, CASH CHAR, PAY_CODE CHAR, SPREAD_PAYMENT FLOAT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO KRM023_TMP (IDN, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, PAYMENT_AMT, ";
  sql_stmt += "CASH, PAY_CODE, SPREAD_PAYMENT) SELECT  IDN, ";
  sql_stmt += "(CASE WHEN LEFT(BILL_DATE,1) = '*' THEN NULL ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 4 AND LEFT(BILL_DATE,1) BETWEEN '1' AND '9' ";
  sql_stmt += "THEN CONVERT(INT, LEFT(BILL_DATE, 2)) * 12 + CONVERT(INT, RIGHT(BILL_DATE, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 5 AND LEFT(BILL_DATE,1) BETWEEN '0' AND '9' THEN ";
  sql_stmt += "CONVERT(INT, LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, RIGHT(BILL_DATE, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 7 ";//AND CONVERT(INT, RIGHT(BILL_DATE,2)) > 15 ";
  sql_stmt += "THEN CONVERT(INT, LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) ";
//  sql_stmt += "WHEN LEN(BILL_DATE) = 7 THEN CONVERT(INT,LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) - 1 ";
  sql_stmt += "ELSE NULL END), (CASE WHEN ISSUE = '021' AND CARD_TYPE = 'V' THEN 'CTV' ";
  sql_stmt += "WHEN ISSUE = '021' AND CARD_TYPE = 'M' THEN 'CTM' ";
  sql_stmt += "WHEN ISSUE = '021' AND CARD_TYPE = 'D' THEN 'CTD' ";
  sql_stmt += "WHEN ISSUE = 'A82' AND CARD_TYPE = 'A' THEN 'AEA' ";
  sql_stmt += "WHEN ISSUE = 'A82' AND CARD_TYPE = 'E' THEN 'AEE' ELSE ISSUE END),";
  sql_stmt += "ISSUE_NAME, PERM_LIMIT, LAST_PAYA, REVOL_BAL / 1000.0, ";
  sql_stmt += "(CASE WHEN CASH_LENT > 0 THEN 'Y' ELSE 'N' END), ";
  sql_stmt += "(CASE WHEN PAY_STAT = 'X' AND PAY_CODE = 'X' THEN 'X' ";
  sql_stmt += "WHEN PAY_STAT = '1' AND PAY_CODE = 'N' THEN 'A' ";
  sql_stmt += "WHEN PAY_STAT = '1' AND PAY_CODE = '0' THEN 'B' ";
  sql_stmt += "WHEN PAY_STAT = '2' AND PAY_CODE = 'N' THEN 'C' ";
  sql_stmt += "WHEN PAY_STAT = '2' AND PAY_CODE = '0' THEN 'D' ";
  sql_stmt += "WHEN PAY_STAT = '3' AND PAY_CODE BETWEEN '1' AND '7' THEN 'E' ";
  sql_stmt += "WHEN PAY_STAT = '4' AND PAY_CODE BETWEEN '1' AND '7' THEN 'F' ";
  sql_stmt += "WHEN DEBT_CODE IN ('A', 'B') THEN 'F' ELSE NULL END), ";
  sql_stmt += "PRE_OWED / 1000.0 ";
  sql_stmt += "FROM " + krm037;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE KRM023_TMP SET PAYMENT_AMT = ";
  sql_stmt += "(CASE RIGHT(PAYMENT,1) WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(LEFT(PAYMENT,2),0)-1) / 1000.0 ";
  sql_stmt += "WHERE ((PAYMENT_AMT IS NULL) OR (PAYMENT_AMT = 0))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE KRM023_TMP1 ";
  sql_stmt += "(IDN CHAR(11), ";
  sql_stmt += "MON_SINCE INT, ISSUE CHAR(3), ISSUE_NAME CHAR(40), LIMIT CHAR(5),";
  sql_stmt += "PAYMENT_AMT FLOAT, CASH CHAR, PAY_CODE CHAR, SPREAD_PAYMENT FLOAT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO KRM023_TMP1 (IDN, MON_SINCE, ISSUE, ISSUE_NAME, ";
  sql_stmt += "LIMIT, PAYMENT_AMT, CASH, PAY_CODE, SPREAD_PAYMENT) ";
  sql_stmt += "SELECT  IDN, MON_SINCE, ISSUE, ISSUE_NAME, SUM(CONVERT(INT, LIMIT)), ";
  sql_stmt += "SUM(PAYMENT_AMT), MAX(CASH), ";
  sql_stmt += "MAX(CASE WHEN PAY_CODE = 'X' THEN '0' ELSE PAY_CODE END), ";
  sql_stmt += "SUM(SPREAD_PAYMENT) FROM KRM023_TMP ";
  sql_stmt += "GROUP BY IDN, MON_SINCE, ISSUE, ISSUE_NAME";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE KRM023_TMP1 SET ";
  sql_stmt += "CASH = A.CASH FROM KRM023_TMP1 AS A INNER JOIN KRM023_TMP1 ";
  sql_stmt += "ON KRM023_TMP1.IDN = A.IDN AND ";
  sql_stmt += "KRM023_TMP1.MON_SINCE = A.MON_SINCE + 1 AND ";
  sql_stmt += "KRM023_TMP1.ISSUE = A.ISSUE ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + krm023 + " ";
  sql_stmt += "ADD PAYMENT_AMT FLOAT, BUCKET_DEF_1K INT, BUCKET_EF_1K INT, BUCKET_F_1K INT, MON_SINCE INT, SPREAD_PAYMENT FLOAT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + krm023 + "(IDN, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT_AMT, ";
  sql_stmt += "CASH, PAY_CODE, SPREAD_PAYMENT) SELECT IDN, MON_SINCE, ";
  sql_stmt += "ISSUE, ISSUE_NAME, LIMIT, PAYMENT_AMT, CASH, ";
  sql_stmt += "(CASE WHEN PAY_CODE = '0' THEN 'X' ELSE PAY_CODE END), SPREAD_PAYMENT ";
  sql_stmt += "FROM KRM023_TMP1 WHERE MON_SINCE > 1140"; //1140 = 94 * 12 + 12
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
  sql_stmt += "YRMON = (CASE WHEN YRMON = '' THEN NULL ELSE YRMON END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "KR_CODE = (CASE WHEN KR_CODE = '' THEN NULL ELSE KR_CODE END), ";
  sql_stmt += "LIMIT = (CASE WHEN LIMIT = '' THEN NULL ELSE LIMIT END), ";
  sql_stmt += "PAYMENT = (CASE WHEN PAYMENT = '' THEN NULL ELSE PAYMENT END), ";
  sql_stmt += "CASH = (CASE WHEN CASH = '' THEN NULL ELSE CASH END), ";
  sql_stmt += "PAY_CODE = (CASE WHEN PAY_CODE = '' THEN NULL ELSE PAY_CODE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Change YRMON to proper format so that year of 99 will be smaller than year of 100
  sql_stmt = "UPDATE " + krm023 + " SET YRMON = ";
  sql_stmt += "(CASE WHEN LEFT(LTRIM(RTRIM(YRMON)),1) NOT IN ('1', '0', '*') ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(YRMON)) ELSE LTRIM(RTRIM(YRMON))  END) ";
  sql_stmt += "WHERE YRMON IS NOT NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Put value INTO MON_SINCE, MON_SINCE means that month since the first month
  //of the establishment of ROC; actually it IS 12 larger than it logically should be
  //But since our use on the column IS to calculate the difference, it IS OK
  sql_stmt = "UPDATE " + krm023 + " SET MON_SINCE = ";
  sql_stmt += "(CASE WHEN LEFT(YRMON,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(YRMON, 3) * 12 + RIGHT(LEFT(YRMON, 5), 2) END) ";
  sql_stmt += "WHERE MON_SINCE IS NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
//For two columns payment
/*  sql_stmt = "UPDATE " + table + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE PAYMENT2 WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(PAYMENT,0)-1) / 1000.0;";
  command->CommandText = sql_stmt;
  command->Execute();*/
//For one column payment
  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT = ";
  sql_stmt += "(CASE WHEN RIGHT(LEFT(PAYMENT,2),1) IN ('H', 'M', 'L') THEN '0' + (LEFT(PAYMENT,2)) ELSE PAYMENT END) ";
  sql_stmt += "WHERE PAYMENT IS NOT NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE RIGHT(PAYMENT,1) WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(LEFT(PAYMENT,2),0)-1) / 1000.0 ";
  sql_stmt += "WHERE ((PAYMENT_AMT IS NULL) OR (PAYMENT_AMT = 0))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE WHEN LIMIT = 0 THEN PAYMENT_AMT ";
  sql_stmt += "WHEN PAYMENT_AMT > LIMIT THEN LIMIT ELSE PAYMENT_AMT END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Calculate the buckets, first initialize the columns
  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K = 0, BUCKET_EF_1K = 0, BUCKET_F_1K = 0;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The second phase of columns initialization for bucket assignment
  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K = (CASE WHEN PAY_CODE in ('D', 'E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K = (CASE WHEN PAY_CODE in ('E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K = (CASE WHEN PAY_CODE = 'F' AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The bucket assignment process
  sql_stmt = "DECLARE @i INT SET @i=12 ";
  sql_stmt += "WHILE @i > 0 BEGIN UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K =(CASE WHEN " + krm023 + ".PAY_CODE IN ('D', 'E', 'F') ";
  sql_stmt += " AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_DEF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K =(CASE WHEN " + krm023 + ".PAY_CODE IN ('E', 'F') ";
  sql_stmt += "AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_EF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K =(CASE WHEN " + krm023 + ".PAY_CODE = 'F' ";
  sql_stmt += "AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_F_1K + 1 ELSE 0 END) ";
  sql_stmt += "FROM " + krm023 + " AS A INNER JOIN " + krm023 + " ON ";
  sql_stmt += "A.idn = " + krm023 + ".idn AND A.ISSUE = " + krm023 + ".ISSUE ";
  sql_stmt += "AND A.MON_SINCE = (" + krm023 + ".MON_SINCE - 1) ";
  sql_stmt += "WHERE (:inq_mon - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["inq_mon"] = now;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "PAYMENT_AMT = (CASE WHEN PAYMENT_AMT > LIMIT THEN LIMIT ";
  sql_stmt += "ELSE PAYMENT_AMT END) WHERE LIMIT > 0 ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
};

void merge_prepare_KRM023_KRM034(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm034, int now)
{
  AnsiString sql_stmt;
  sql_stmt = "UPDATE " + krm034 + " SET ";
  sql_stmt += "BILL_DATE = (CASE WHEN BILL_DATE = '' THEN NULL ELSE BILL_DATE END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "CARD_TYPE = (CASE WHEN CARD_TYPE = '' THEN NULL ELSE CARD_TYPE END), ";
  sql_stmt += "CASH_YN = (CASE WHEN CASH_YN = '' THEN NULL ELSE CASH_YN END), ";
//  sql_stmt += "PAYABLE = (CASE WHEN PAYABLE = '' THEN NULL ELSE PAYABLE END), ";
  sql_stmt += "LAST_PAYA = (CASE WHEN LAST_PAYA = '' THEN NULL ELSE LAST_PAYA END), ";
  sql_stmt += "PAY_STAT = (CASE WHEN PAY_STAT = '' THEN NULL ELSE PAY_STAT END), ";
  sql_stmt += "PAY_CODE = (CASE WHEN PAY_CODE = '' THEN NULL ELSE PAY_CODE END), ";
  sql_stmt += "DEBT_CODE = (CASE WHEN DEBT_CODE = '' THEN NULL ELSE DEBT_CODE END), ";
  sql_stmt += "CLOSE_CODE = (CASE WHEN CLOSE_CODE = '' THEN NULL ELSE CLOSE_CODE END), ";
  sql_stmt += "CLEAR_DATE = (CASE WHEN CLEAR_DATE = '' THEN NULL ELSE CLEAR_DATE END) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE KRM023_TMP";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
  

  sql_stmt = "CREATE TABLE KRM023_TMP ";
  sql_stmt += "(IDN CHAR(11), INPUT_TIME CHAR(12), INQUIRY_DATE CHAR(8), ";
  sql_stmt += "MON_SINCE INT, ISSUE CHAR(3), ISSUE_NAME CHAR(40), LIMIT CHAR(5),";
  sql_stmt += "PAYMENT CHAR(3), PAYMENT_AMT FLOAT, CASH CHAR, PAY_CODE CHAR)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO KRM023_TMP (IDN, INPUT_TIME, INQUIRY_DATE, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, ";
  sql_stmt += "CASH, PAY_CODE) SELECT IDN, INPUT_TIME, INQUIRY_DATE, ";
  sql_stmt += "(CASE WHEN LEFT(BILL_DATE,1) = '*' THEN NULL ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 4 AND LEFT(BILL_DATE,1) BETWEEN '1' AND '9' ";
  sql_stmt += "THEN CONVERT(INT, LEFT(BILL_DATE, 2)) * 12 + CONVERT(INT, RIGHT(BILL_DATE, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 5 AND LEFT(BILL_DATE,1) = '0' THEN ";
  sql_stmt += "CONVERT(INT, LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, RIGHT(BILL_DATE, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 7 AND CONVERT(INT, RIGHT(BILL_DATE,2)) > 15 ";
  sql_stmt += "THEN CONVERT(INT, LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 7 THEN CONVERT(INT,LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) - 1 ";
  sql_stmt += "(CASE WHEN ISSUE = '021' AND CARD_TYPE = 'D' THEN 'CTD' ";
  sql_stmt += "WHEN ISSUE = '021' AND CARD_TYPE = 'M' THEN 'CTM' ";
  sql_stmt += "WHEN ISSUE = '021' AND CARD_TYPE = 'V' THEN 'CTV' ";
  sql_stmt += "WHEN ISSUE = 'A82' AND CARD_TYPE = 'A' THEN 'AEA' ";
  sql_stmt += "WHEN ISSUE = 'A82' AND CARD_TYPE = 'E' THEN 'AEE' ";
  sql_stmt += "ELSE ISSUE END), ";
  sql_stmt += "ELSE NULL END), ";
  sql_stmt += "ISSUE_NAME, PERM_LIMIT, LAST_PAYA, CASH_YN, ";
  sql_stmt += "(CASE WHEN PAY_STAT = 'X' AND PAY_CODE = 'X' THEN 'X' ";
  sql_stmt += "WHEN PAY_STAT = '1' AND PAY_CODE = 'N' THEN 'A' ";
  sql_stmt += "WHEN PAY_STAT = '1' AND PAY_CODE = '0' THEN 'B' ";
  sql_stmt += "WHEN PAY_STAT = '2' AND PAY_CODE = 'N' THEN 'C' ";
  sql_stmt += "WHEN PAY_STAT = '2' AND PAY_CODE = '0' THEN 'D' ";
  sql_stmt += "WHEN PAY_STAT = '3' AND PAY_CODE BETWEEN '1' AND '7' THEN 'E' ";
  sql_stmt += "WHEN PAY_STAT = '4' AND PAY_CODE BETWEEN '1' AND '7' THEN 'F' ";
  sql_stmt += "WHEN DEBT_CODE IN ('A', 'B') THEN 'F' ELSE NULL END) ";
  sql_stmt += "FROM " + krm034;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + krm023 + " ";
  sql_stmt += "ADD PAYMENT_AMT FLOAT, BUCKET_DEF_1K INT, BUCKET_EF_1K INT, BUCKET_F_1K INT, MON_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + krm023 + "(IDN, INPUT_TIME, INQUIRY_DATE, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, CASH, ";
  sql_stmt += "PAY_CODE) SELECT IDN, INPUT_TIME, INQUIRY_DATE, MON_SINCE, ";
  sql_stmt += "ISSUE, ISSUE_NAME, LIMIT, PAYMENT, CASH, PAY_CODE ";
  sql_stmt += "FROM KRM023_TMP WHERE MON_SINCE > 1140"; //1140 = 94 * 12 + 12
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
  sql_stmt += "YRMON = (CASE WHEN YRMON = '' THEN NULL ELSE YRMON END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "KR_CODE = (CASE WHEN KR_CODE = '' THEN NULL ELSE KR_CODE END), ";
  sql_stmt += "LIMIT = (CASE WHEN LIMIT = '' THEN NULL ELSE LIMIT END), ";
  sql_stmt += "PAYMENT = (CASE WHEN PAYMENT = '' THEN NULL ELSE PAYMENT END), ";
  sql_stmt += "CASH = (CASE WHEN CASH = '' THEN NULL ELSE CASH END), ";
  sql_stmt += "PAY_CODE = (CASE WHEN PAY_CODE = '' THEN NULL ELSE PAY_CODE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Change YRMON to proper format so that year of 99 will be smaller than year of 100
  sql_stmt = "UPDATE " + krm023 + " SET YRMON = ";
  sql_stmt += "(CASE WHEN LEFT(LTRIM(RTRIM(YRMON)),1) NOT IN ('1', '0', '*') ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(YRMON)) ELSE LTRIM(RTRIM(YRMON))  END) ";
  sql_stmt += "WHERE YRMON IS NOT NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Put value INTO MON_SINCE, MON_SINCE means that month since the first month
  //of the establishment of ROC; actually it IS 12 larger than it logically should be
  //But since our use on the column IS to calculate the difference, it IS OK
  sql_stmt = "UPDATE " + krm023 + " SET MON_SINCE = ";
  sql_stmt += "(CASE WHEN LEFT(YRMON,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(YRMON, 3) * 12 + RIGHT(LEFT(YRMON, 5), 2) END) ";
  sql_stmt += "WHERE MON_SINCE IS NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
//For two columns payment
/*  sql_stmt = "UPDATE " + table + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE PAYMENT2 WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(PAYMENT,0)-1) / 1000.0;";
  command->CommandText = sql_stmt;
  command->Execute();*/
//For one column payment
  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT = ";
  sql_stmt += "(CASE WHEN RIGHT(LEFT(PAYMENT,2),1) IN ('H', 'M', 'L') THEN '0' + (LEFT(PAYMENT,2)) ELSE PAYMENT END) ";
  sql_stmt += "WHERE PAYMENT IS NOT NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE RIGHT(PAYMENT,1) WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(LEFT(PAYMENT,2),0)-1) / 1000.0 ";
  sql_stmt += "WHERE ((PAYMENT_AMT IS NULL) OR (PAYMENT_AMT = 0))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE WHEN LIMIT = 0 THEN PAYMENT_AMT ";
  sql_stmt += "WHEN PAYMENT_AMT > LIMIT THEN LIMIT ELSE PAYMENT_AMT END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Calculate the buckets, first initialize the columns
  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K = 0, BUCKET_EF_1K = 0, BUCKET_F_1K = 0;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The second phase of columns initialization for bucket assignment
  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K = (CASE WHEN PAY_CODE in ('D', 'E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K = (CASE WHEN PAY_CODE in ('E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K = (CASE WHEN PAY_CODE = 'F' AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The bucket assignment process
  sql_stmt = "DECLARE @i INT SET @i=12 ";
  sql_stmt += "WHILE @i > 0 BEGIN UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K =(CASE WHEN " + krm023 + ".PAY_CODE IN ('D', 'E', 'F') ";
  sql_stmt += " AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_DEF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K =(CASE WHEN " + krm023 + ".PAY_CODE IN ('E', 'F') ";
  sql_stmt += "AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_EF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K =(CASE WHEN " + krm023 + ".PAY_CODE = 'F' ";
  sql_stmt += "AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_F_1K + 1 ELSE 0 END) ";
  sql_stmt += "FROM " + krm023 + " AS A INNER JOIN " + krm023 + " ON ";
  sql_stmt += "A.idn = " + krm023 + ".idn AND A.ISSUE = " + krm023 + ".ISSUE ";
  sql_stmt += "AND A.MON_SINCE = (" + krm023 + ".MON_SINCE - 1) ";
  sql_stmt += "WHERE (:inq_mon - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["inq_mon"] = now;
  command->Execute();
};

void prepare_KRM001(TADOCommand *command, const AnsiString &table, int now)
{
  AnsiString sql_stmt;

  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "idn = (CASE WHEN idn = '' THEN NULL ELSE idn END), ";
  sql_stmt += "CARD_BRAND = (CASE WHEN CARD_BRAND = '' THEN NULL ELSE CARD_BRAND END), ";
  sql_stmt += "CARD_TYPE = (CASE WHEN CARD_TYPE = '' THEN NULL ELSE CARD_TYPE END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "START_DATE = (CASE WHEN START_DATE = '' THEN NULL ELSE START_DATE END), ";
  sql_stmt += "STOP_DATE = (CASE WHEN STOP_DATE = '' THEN NULL ELSE STOP_DATE END), ";
  sql_stmt += "STOP_CODE = (CASE WHEN STOP_CODE = '' THEN NULL ELSE STOP_CODE END), ";
  sql_stmt += "AB_CODE = (CASE WHEN AB_CODE = '' THEN NULL ELSE AB_CODE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Create START_MON_SINCE and END_MON_SINCE for KRM001

  sql_stmt = "ALTER TABLE " + table + " ADD START_MON_SINCE INT, END_MON_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET START_DATE = (CASE  ";
  sql_stmt += " WHEN (LEFT(LTRIM(RTRIM(START_DATE)),1) BETWEEN '1' AND '9') AND (LEN(LTRIM(RTRIM(START_DATE)))<7) ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(START_DATE)) ELSE LTRIM(RTRIM(START_DATE)) END), ";
  sql_stmt += "STOP_DATE = (CASE WHEN (LEFT(LTRIM(RTRIM(STOP_DATE)),1) BETWEEN '1' AND '9') AND ";
  sql_stmt += "(LEN(LTRIM(RTRIM(STOP_DATE)))<7) AND STOP_DATE != '9991231' ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(STOP_DATE)) ELSE LTRIM(RTRIM(STOP_DATE)) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET STOP_DATE = '9991231' ";
  sql_stmt += " WHERE ((STOP_DATE = '') OR (STOP_DATE IS NULL));";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET START_MON_SINCE = (CASE ";
  sql_stmt += "WHEN LEFT(START_DATE,1) NOT BETWEEN '0' AND '9' THEN NULL ";
  sql_stmt += "ELSE LEFT(START_DATE, 3) * 12 + RIGHT(LEFT(START_DATE, 5), 2) END), ";
  sql_stmt += "END_MON_SINCE = (CASE ";
  sql_stmt += "WHEN LEFT(STOP_DATE, 1) NOT BETWEEN '0' AND '9' THEN NULL ";
  sql_stmt += "ELSE LEFT(STOP_DATE, 3) * 12 + RIGHT(LEFT(STOP_DATE, 5), 2) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
};

void prepare_BAM086(TADOCommand *command, const AnsiString &table, int now)
{
  AnsiString sql_stmt;
  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "idn = (CASE WHEN idn = '' THEN NULL ELSE idn END), ";
  sql_stmt += "DATA_YYY = (CASE WHEN DATA_YYY = '' THEN NULL ELSE DATA_YYY END), ";
  sql_stmt += "DATA_MM = (CASE WHEN DATA_MM = '' THEN NULL ELSE DATA_MM END), ";
  sql_stmt += "BANK_CODE = (CASE WHEN BANK_CODE = '' THEN NULL ELSE BANK_CODE END), ";
  sql_stmt += "BANK_NAME = (CASE WHEN BANK_NAME = '' THEN NULL ELSE BANK_NAME END), ";
  sql_stmt += "ACCOUNT_CODE = (CASE WHEN ACCOUNT_CODE = '' THEN NULL ELSE ACCOUNT_CODE END), ";
  sql_stmt += "ACCOUNT_CODE2 = (CASE WHEN ACCOUNT_CODE2 = '' THEN NULL ELSE ACCOUNT_CODE2 END), ";
  sql_stmt += "PURPOSE_CODE = (CASE WHEN PURPOSE_CODE = '' THEN NULL ELSE PURPOSE_CODE END), ";
  sql_stmt += "PAY_CODE_12 = (CASE WHEN PAY_CODE_12 = '' THEN NULL ELSE PAY_CODE_12 END), ";
  sql_stmt += "CO_LOAN = (CASE WHEN CO_LOAN = '' THEN NULL ELSE CO_LOAN END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Create Bank_Code2 for BAM009
  sql_stmt = "ALTER TABLE " + table + " ADD BANK_CODE2 CHAR(3);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET BANK_CODE2 = LEFT(BANK_CODE,3);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  build_bam_bucket(command, table, now);
};

void prepare_STM001(TADOCommand *command, const AnsiString &table,
                    const AnsiString &bank_atm_code)
{
  AnsiString sql_stmt;
  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "idn = (CASE WHEN idn = '' THEN NULL ELSE idn END), ";
  sql_stmt += "Query_Date = (CASE WHEN Query_Date = '' THEN NULL ELSE Query_Date END), ";
  sql_stmt += "Bank_Code = (CASE WHEN Bank_Code = '' THEN NULL ELSE Bank_Code END), ";
  sql_stmt += "Bank_Name = (CASE WHEN Bank_Name = '' THEN NULL ELSE Bank_Name END), ";
  sql_stmt += "Item_List = (CASE WHEN Item_List = '' THEN NULL ELSE Item_List END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + table + " ADD QUERY_MON_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET QUERY_DATE = (CASE ";
  sql_stmt += "WHEN LEFT(QUERY_DATE,1) NOT IN ('1', '0', '*') THEN '0' + ";
  sql_stmt += "LTRIM(RTRIM(QUERY_DATE)) ELSE QUERY_DATE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET QUERY_MON_SINCE = (CASE ";
  sql_stmt += "WHEN LEFT(QUERY_DATE,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(QUERY_DATE, 3) * 12 + RIGHT(LEFT(QUERY_DATE, 5), 2) ";
  sql_stmt += " END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM " + table + " WHERE ";
  sql_stmt += "DATEDIFF(day, CONVERT(DATETIME, CONVERT(CHAR(4),CONVERT(INT,LEFT(LTRIM(QUERY_DATE),3)) + 1911) + RIGHT(RTRIM(QUERY_DATE),4)), CONVERT(DATETIME, INQUIRY_DATE)) <= 30 AND "; //for production
  sql_stmt += "BANK_CODE = :bank_atm_code AND ((ITEM_LIST LIKE '%K%') OR (ITEM_LIST LIKE '%k%')) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  AnsiString branch_code = bank_atm_code + "CD01";
  command->Parameters->ParamValues["bank_atm_code"] = branch_code;
  command->Execute();
};

void prepare_JAS002(TADOCommand *command, const AnsiString &ori_table, const AnsiString &dest_table)
{
	AnsiString sql_stmt;

  sql_stmt = "UPDATE " + ori_table + " SET ";
  sql_stmt += "idn = (CASE WHEN idn = '' THEN NULL ELSE idn END), ";
  sql_stmt += "EVER_DELINQUENT = (CASE WHEN EVER_DELINQUENT = '' THEN NULL ELSE EVER_DELINQUENT END), ";
  sql_stmt += "DELINQUENT_DATE = (CASE WHEN DELINQUENT_DATE = '' THEN NULL ELSE DELINQUENT_DATE END), ";
  sql_stmt += "EVER_BAD_CHECK = (CASE WHEN EVER_BAD_CHECK = '' THEN NULL ELSE EVER_BAD_CHECK END), ";
  sql_stmt += "BAD_CHECK_DATE = (CASE WHEN BAD_CHECK_DATE = '' THEN NULL ELSE BAD_CHECK_DATE END), ";
  sql_stmt += "EVER_REJECT = (CASE WHEN EVER_REJECT = '' THEN NULL ELSE EVER_REJECT END), ";
  sql_stmt += "REJECT_DATE = (CASE WHEN REJECT_DATE = '' THEN NULL ELSE REJECT_DATE END), ";
  sql_stmt += "EVER_STOP_CARD = (CASE WHEN EVER_STOP_CARD = '' THEN NULL ELSE EVER_STOP_CARD END), ";
  sql_stmt += "STOP_CARD_DATE = (CASE WHEN STOP_CARD_DATE = '' THEN NULL ELSE STOP_CARD_DATE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();


	sql_stmt = "DROP TABLE " + dest_table + ";";
  sql_stmt = sql_stmt.UpperCase();
  try{
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//      if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
  sql_stmt = "CREATE TABLE " + dest_table;
  sql_stmt += " (idn Char(12), TYPE CHAR(1), E_DATE Char(7), ";
  sql_stmt += "E_MON_SINCE INT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT idn, 'D', DELINQUENT_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_DELINQUENT = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT idn, 'B', BAD_CHECK_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_BAD_CHECK = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT idn, 'R', REJECT_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_REJECT = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT idn, 'S', STOP_CARD_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_STOP_CARD = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + dest_table;
  sql_stmt += " SET E_DATE = (CASE WHEN (LEN(RTRIM(LTRIM(E_DATE))) = 6 OR LTRIM(RTRIM(E_DATE)) <> '*' OR E_DATE IS NOT NULL) ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(E_DATE)) ELSE LTRIM(RTRIM(E_DATE)) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + dest_table;
  sql_stmt += " SET E_MON_SINCE = CONVERT(INT, ISNULL(LEFT(E_DATE,3),0)) * 12 + ";
  sql_stmt += "CONVERT(INT, ISNULL(RIGHT(LEFT(E_DATE,5),2),0));";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
};

void get_idn_list(TADOCommand *command, AnsiString KRM001, AnsiString KRM023,
                  AnsiString BAM086, AnsiString STM001, AnsiString JAS002_T)
{
  AnsiString sql_stmt;
	try{
  	sql_stmt = "DROP TABLE IDN_LIST_TMP;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE IDN_LIST;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "CREATE TABLE IDN_LIST_TMP (IDN CHAR(11), HIT INT);";//KRM001 = 2, KRM023 = 1, BAM086 = 4, STM001 = 8, DAM103 or JAS002 = 16, DMAM203 = 32
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 2714)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
	};
	try{
  	sql_stmt = "CREATE TABLE IDN_LIST (IDN CHAR(11), HIT INT);";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 2714)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
	};
  sql_stmt = "DELETE FROM IDN_LIST_TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM IDN_LIST;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 1 FROM " + KRM023 + " ";
  command->CommandText = sql_stmt;
  sql_stmt = sql_stmt.UpperCase();
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 2 FROM " + KRM001 + " ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 4 FROM " + BAM086 + " ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 8 FROM " + STM001 + " ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 16 FROM " + JAS002_T + " ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST SELECT IDN, SUM(HIT) FROM IDN_LIST_TMP GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

	try{
  	sql_stmt = "DROP TABLE IDN_LIST_TMP;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
};

void create_common_working_tables(TADOCommand * command)
{
  AnsiString sql_stmt;
try{
  sql_stmt = "DROP TABLE TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
};
try{
  sql_stmt = "DROP TABLE TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
};
try{
  sql_stmt = "DROP TABLE TMP2;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
};
try{
  sql_stmt = "DROP TABLE OPEN_CARD;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
};
try{
  sql_stmt = "DROP TABLE OPEN_LINE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
};
try{
  sql_stmt = "DROP TABLE OPEN_ISSUER;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
};
try{
  sql_stmt = "DROP TABLE LATEST_STMT_MON;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
};
try{
  sql_stmt = "DROP TABLE LATEST_LINE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
};
  sql_stmt = "CREATE TABLE TMP(IDN CHAR(11), MON INT, V1 FLOAT, ";
  sql_stmt += "V2 FLOAT, V3 FLOAT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE TMP1(IDN CHAR(11), MON INT, V1 FLOAT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE TMP2(IDN CHAR(11), ISSUER CHAR(3));";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE OPEN_CARD (IDN CHAR(11), ISSUE CHAR(3), MON_SINCE INT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE OPEN_LINE (IDN CHAR(11), ISSUE CHAR(3), MON_SINCE INT, ";
  sql_stmt += "Cards INT, BUCKET INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE LATEST_STMT_MON (IDN CHAR(11), ISSUE CHAR(3), ";
  sql_stmt += "MON_SINCE INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE LATEST_LINE (IDN CHAR(11), ISSUE CHAR(3), ";
  sql_stmt += "MON_SINCE INT, MOB INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE OPEN_ISSUER (IDN CHAR(11), ISSUE CHAR(3), MON INT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
};

void prepare_2xx_infra(TADOCommand *command, int now)
{
  AnsiString sql_stmt;
  sql_stmt = "DECLARE @i INT SET @i = 0 WHILE @i<=13 BEGIN INSERT INTO ";
  sql_stmt += "OPEN_CARD SELECT IDN, ";
  sql_stmt += "(CASE WHEN CARD_BRAND = 'A' AND ISSUE = 'A82' THEN 'AEA' ELSE ISSUE END), ";
  sql_stmt += "(:now - @i) FROM " + KRM001;
  sql_stmt += " WHERE (END_MON_SINCE > (:now1 - @i)) AND (START_MON_SINCE <= ";
  sql_stmt += "(:now2 - @i)) AND ISSUE != '021' SET @i = @i + 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["now1"] = now;
  command->Parameters->ParamValues["now2"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO OPEN_LINE SELECT IDN, ISSUE, MON_SINCE, Count(*), NULL FROM ";
  sql_stmt += "OPEN_CARD GROUP BY IDN, ISSUE, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 0 WHILE @i<=13 BEGIN INSERT INTO ";
  sql_stmt += "OPEN_LINE SELECT IDN, (CASE ";
  sql_stmt += "WHEN CARD_BRAND = 'M' THEN 'CTM' ";
  sql_stmt += "WHEN CARD_BRAND = 'V' THEN 'CTV' ";
  sql_stmt +="WHEN CARD_BRAND = 'D' THEN 'CTD' ELSE ISSUE END), (:now - @i),1, NULL ";
  sql_stmt += "FROM " + KRM001 + " WHERE (END_MON_SINCE > (:now1 - @i)) AND ";
  sql_stmt += "(START_MON_SINCE <= (:now2 - @i)) AND ISSUE = '021' ";
  sql_stmt +="SET @i = @i + 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["now1"] = now;
  command->Parameters->ParamValues["now2"] = now;
  command->Execute();

  sql_stmt = "UPDATE OPEN_LINE SET BUCKET = 1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE OPEN_LINE SET BUCKET = 100 WHERE MON_SINCE = (:now - 13);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 13 WHILE @i > 0 BEGIN UPDATE OPEN_LINE ";
  sql_stmt += "SET BUCKET = A.BUCKET + 1 FROM OPEN_LINE AS A INNER JOIN ";
  sql_stmt += "OPEN_LINE ON A.IDN = OPEN_LINE.IDN AND ";
  sql_stmt +="A.ISSUE = OPEN_LINE.ISSUE AND A.MON_SINCE = (OPEN_LINE.MON_SINCE - 1) ";
  sql_stmt += "WHERE (:now - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO LATEST_STMT_MON SELECT IDN, ISSUE, MAX(MON_SINCE) ";
  sql_stmt += "FROM OPEN_LINE WHERE MON_SINCE <= :now GROUP BY IDN, ISSUE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO LATEST_LINE SELECT A.IDN, A.ISSUE, A.MON_SINCE, A.BUCKET ";
  sql_stmt += "FROM OPEN_LINE AS A INNER JOIN LATEST_STMT_MON AS B ";
  sql_stmt += "ON A.IDN = B.IDN AND A.ISSUE = B.ISSUE AND A.MON_SINCE = B.MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
};

void create_propensity_tables(TADOCommand *command)
{
  AnsiString sql_stmt;
/*try{
  sql_stmt = "DROP TABLE PM;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
} */
try{
  sql_stmt = "CREATE TABLE PM (IDN CHAR(11), HIT INT, EXCLUSION INT, ";
  sql_stmt += "FS029 INT, FS031 INT, FS042 INT, MS064 FLOAT, MS119_3M FLOAT, ";
  sql_stmt += "MS011_3M DECIMAL(13,5), MS011_6M DECIMAL(13,5), ";
  sql_stmt += "MS023_9M FLOAT, FS105_9M FLOAT, FS106_9M FLOAT, ";
  sql_stmt += "FS106_12M FLOAT, MS104_3M FLOAT, MS104_6M FLOAT, ";
  sql_stmt += "MS104_9M FLOAT, MS105_9M FLOAT, ";
  sql_stmt += "MS106_9M FLOAT, MS108_12M FLOAT, ";
  sql_stmt += "MS118_3M FLOAT, MS118_6M FLOAT, ";
  sql_stmt += "MS118_9M FLOAT, FS212_3M FLOAT, FS212_6M FLOAT, ";
  sql_stmt += "FS030 INT, FS042_Q FLOAT, FT212_43 FLOAT, FT212_43_R FLOAT, ";
  sql_stmt += "INT037_9 FLOAT, INT037_9_Q FLOAT, ";
  sql_stmt += "INT062_9 FLOAT, INT084_9 FLOAT, ";
  sql_stmt += "INT084_9_R FLOAT, MS064_R FLOAT, ";
  sql_stmt += "MS108_12M_R FLOAT, MS119_3M_R FLOAT, ";
  sql_stmt += "MT011_43 FLOAT, MT104_42 FLOAT, ";
  sql_stmt += "MT104_42_Q FLOAT, MT118_42 FLOAT, ";
  sql_stmt += "FS030_T FLOAT, FS042_Q_T FLOAT, ";
  sql_stmt += "FS106_12M_T FLOAT, FT212_43_R_T FLOAT, ";
  sql_stmt += "INT037_9_Q_T1 FLOAT, INT062_9_T1 FLOAT, ";
  sql_stmt += "INT084_9_R_T FLOAT, MS064_R_T FLOAT, ";
  sql_stmt += "MS108_12M_R_T FLOAT, MS119_3M_R_T FLOAT, ";
  sql_stmt += "MT011_43_Z INT, MT104_42_Q_T FLOAT, ";
  sql_stmt += "MT118_42_T FLOAT, SCORE DECIMAL(14,8), DECILE INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 2714)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
	};
};

void load_and_initialize_propensity(TADOCommand *command, const AnsiString &idno, unsigned int exclusion, unsigned int data_flag)
{
  AnsiString sql_stmt;

//load data
  sql_stmt = "INSERT INTO PM(IDN, HIT, EXCLUSION) VALUES (:idno, :hit, :exclusion);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Parameters->ParamValues["hit"] = data_flag;
  command->Parameters->ParamValues["exclusion"] = exclusion;
  command->Execute();
//KRM001 and KRM023 variables
	sql_stmt = "UPDATE PM SET ";
	sql_stmt += "FS212_3M=0, FS212_6M=0 ";
	sql_stmt += "FROM " + KRM001 + " AS A, " + KRM023 + " AS B ";
	sql_stmt += "WHERE A.IDN = B.IDN AND A.IDN = PM.IDN AND ";
	sql_stmt += "((a.issue = b.issue) OR ";
	sql_stmt += " ((a.issue = '021' and a.card_brand = 'V') and (b.issue = 'CTV')) OR ";
	sql_stmt += " ((a.issue = '021' and a.card_brand = 'M') and (b.issue = 'CTM')) OR ";
  sql_stmt += " ((a.issue = '021' and a.card_brand = 'D') and (b.issue = 'CTD')) OR ";
  sql_stmt += " ((a.issue = 'A82' and a.card_brand = 'A') and (b.issue = 'AEA'))) AND ";
	sql_stmt += "A.IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
 	command->CommandText = sql_stmt;
	command->Parameters->ParamValues["idno"] = idno;
	command->Execute();

//KRM023 variables
  sql_stmt = "UPDATE PM SET FS105_9M=0, FS106_9M=0, FS106_12M=0, MS011_3M=0,";
  sql_stmt += "MS011_6M=0, MS023_9M=0, MS104_3M=0, MS104_6M=0, MS104_9M=0, ";
  sql_stmt += "MS105_9M=0, MS106_9M=0, MS108_12M=0, MS118_3M=0, MS118_6M=0, ";
  sql_stmt += "MS118_9M=0, MS119_3M=0 WHERE (HIT & 0x1) = 0x1 AND IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();
//BAM009 variables
  sql_stmt = "UPDATE PM SET FS042=0, MS064=0 WHERE (HIT & 0x4) = 0x4 AND IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();
//STM001 variables
  sql_stmt = "UPDATE PM SET FS029=0, FS030=0, FS031=0 WHERE (HIT & 0x8) = 0x8 AND IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();
};

double propensity(TADOCommand *command, TADOQuery *query, const AnsiString &idno, int now, unsigned int exclusion, unsigned int data_flag)
{
  AnsiString sql_stmt;

  create_propensity_tables(command);
  load_and_initialize_propensity(command, idno, exclusion, data_flag);
  double score = -100000;

	bool run_model = true;
  if(((exclusion & 0x1) == 0x1) ||  ((exclusion & 0x2) == 0x2))
    run_model = false;

  if(run_model){
//generating s variables

/*----FS029----*/
  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  sql_stmt = sql_stmt.UpperCase();
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, NULL, COUNT(*) ";
  sql_stmt += "FROM " + STM001 + " WHERE ((ITEM_LIST LIKE '%K%') ";
  sql_stmt += "AND (:now - QUERY_MON_SINCE <= 12)) ";
  sql_stmt += "AND IDN = :idno GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS029 = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS031----*/
/*---Init temp table----*/
  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, NULL, COUNT(*) ";
  sql_stmt += "FROM " + STM001 + " WHERE ITEM_LIST IS NOT NULL AND ";
  sql_stmt += "ITEM_LIST <> '' AND (:now - QUERY_MON_SINCE <= 12) ";
  sql_stmt += "AND IDN = :idno GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS031 = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS030----*/

  sql_stmt = "UPDATE PM SET FS030 = FS031 - FS029;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();


/*----FS042----*/
  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, NULL, COUNT(*) FROM " + BAM086;
  sql_stmt += " WHERE ACCOUNT_CODE in ('H','S') AND (ACCOUNT_CODE2 ='' or ";
  sql_stmt += "ACCOUNT_CODE2 IS NULL OR ACCOUNT_CODE2 = 'N') AND IDN = :idno GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS042 = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS064----*/
  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, NULL, SUM(CONVERT(FLOAT,LOAN_AMT) + CONVERT(FLOAT,PASS_DUE_AMT)) ";
  sql_stmt += "FROM " + BAM086 + " WHERE (ACCOUNT_CODE2 = '' or ACCOUNT_CODE2 IS NULL OR ACCOUNT_CODE2 = 'N') ";
  sql_stmt += "AND ACCOUNT_CODE IN ('W','C', 'D', 'E') AND IDN = :idno GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS064 = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS011----*/
  sql_stmt = "DELETE FROM TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE ";
  sql_stmt += "WHEN PAY_CODE IN ('A', 'B') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idno GROUP BY IDN, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=6 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT IDN, @i, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= @i GROUP BY IDN SET @i = @i + 3 END";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS011_3M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=3;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS011_6M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=6;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS023----*/
  sql_stmt = "DELETE FROM TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CONVERT(FLOAT,LIMIT))";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idno GROUP BY IDN, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 9, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 9 AND (V1 > 0) GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS023_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS105----*/
  sql_stmt = "DELETE FROM TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN CASH = 'Y' ";
  sql_stmt += "THEN 1 ELSE 0 END) FROM " + KRM023 + " WHERE IDN =:idno GROUP BY IDN, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 9, AVG(V1), NULL, NULL FROM TMP1 WHERE ";
  sql_stmt += "(:now - MON)<= 9 GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS105_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS106----*/
  sql_stmt = "DELETE FROM TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('C', 'D', 'E', 'F') THEN 1 ELSE 0 END) FROM " + KRM023;
  sql_stmt += " WHERE IDN = :idno GROUP BY IDN, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 9 WHILE @i<=12 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT IDN, @i, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= @i AND IDN = :idno GROUP BY IDN SET @i = @i + 3 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS106_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS106_12M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=12;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS104----*/
  sql_stmt = "DELETE FROM TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=9 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT IDN, @i, SUM(PAYMENT_AMT), NULL, NULL FROM " + KRM023 + " WHERE ";
  sql_stmt += "(:now - MON_SINCE)<= @i AND PAY_CODE IN ('A', 'B', 'C') AND ";
  sql_stmt += "LIMIT <> 0 AND IDN = :idno GROUP BY IDN SET @i = @i + 3 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS104_3M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=3;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS104_6M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=6;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS104_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS105----*/
/*---Init temp table----*/
  sql_stmt = "DELETE FROM TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('C', 'D') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idno GROUP BY IDN, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 9, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 9 GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS105_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS106----*/
  sql_stmt = "DELETE FROM TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idno GROUP BY IDN, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 9, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 9 GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS106_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS108----*/
  sql_stmt = "DELETE FROM TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('A', 'B', 'C') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idno GROUP BY IDN, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 12, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 12 GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS108_12M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=12;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS118----*/
  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=9 BEGIN INSERT INTO TMP1 ";
  sql_stmt += "SELECT IDN, @i, MIN(PAYMENT_AMT/CONVERT(FLOAT,LIMIT)) ";
  sql_stmt += "FROM " + KRM023 + " WHERE (:now - MON_SINCE)<= @i AND PAY_CODE IN ";
  sql_stmt +="('C', 'D', 'E', 'F') AND LIMIT <> 0 AND IDN = :idno GROUP BY IDN ";
  sql_stmt +="SET @i = @i + 3 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS118_3M = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN AND MON=3;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS118_6M = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN AND MON=6;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS118_9M = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN AND MON=9;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS119----*/
/*---Init temp table----*/
  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, 3, MIN(PAYMENT_AMT/CONVERT(FLOAT,LIMIT)) ";
  sql_stmt += "FROM " + KRM023 + " WHERE (:now - MON_SINCE)<= 3 AND ";
  sql_stmt +="PAY_CODE IN ('A', 'B', 'C', 'D') AND LIMIT <> 0 AND IDN = :idno GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS119_3M = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN AND MON=3;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS212----*/
  sql_stmt = "DELETE FROM TMP;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 1 WHILE @i <= 12 BEGIN INSERT INTO TMP1 ";
  sql_stmt += "SELECT IDN, @i, COUNT(*) FROM " + KRM023 + " AS A WHERE ";
  sql_stmt += "PAY_CODE IN ('C', 'D', 'E', 'F') AND PAYMENT_AMT > 1 AND ";
  sql_stmt += "(:now - MON_SINCE) = @i AND ISSUE IN ";
  sql_stmt += "(SELECT ISSUE FROM LATEST_LINE WHERE (MON_SINCE - MOB + 1) >= ";
  sql_stmt += "(:now1 - 6) AND IDN = A.IDN) AND IDN = :idno ";
  sql_stmt += "GROUP BY IDN SET @i = @i + 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["now1"] = now;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=6 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT IDN, @i, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (MON)<= @i GROUP BY IDN SET @i = @i + 3 END";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS212_3M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND A.MON = 3;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS212_6M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND A.MON = 6;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//generating interaction and t variables
  sql_stmt = "UPDATE PM SET MT011_43 = 2 * MS011_3M - MS011_6M, ";
  sql_stmt += "MT118_42 = MS118_3M - MS118_9M + MS118_6M, ";
  sql_stmt += "MT104_42 = MS104_3M - MS104_9M + MS104_6M, ";
  sql_stmt += "FT212_43 = 2 * FS212_3M - FS212_6M, ";
  sql_stmt += "INT084_9 = FS105_9M / (CASE WHEN FS106_9M = 0 THEN NULL ELSE FS106_9M END), ";
  sql_stmt += "INT062_9 = MS023_9M - MS105_9M, ";
  sql_stmt += "INT037_9 = MS106_9M / (CASE WHEN MS023_9M = 0 THEN NULL ELSE MS023_9M END) ";
  sql_stmt += "WHERE IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

//generating squared, square_rooted, and zero indicator variables
  sql_stmt = "UPDATE PM SET MT104_42_Q = POWER(MT104_42, 2), ";
  sql_stmt += "MS119_3M_R = POWER((CASE WHEN MS119_3M<0 THEN NULL ELSE MS119_3M END), 0.5), ";
  sql_stmt += "MS108_12M_R = POWER(CONVERT(FLOAT,(CASE WHEN MS108_12M<0 THEN NULL ELSE MS108_12M END)), 0.5), ";
  sql_stmt += "MS064_R = POWER((CASE WHEN MS064<0 THEN NULL ELSE MS064 END), 0.5), ";
  sql_stmt += "INT084_9_R = POWER((CASE WHEN INT084_9<0 THEN NULL ELSE INT084_9 END), 0.5), ";
  sql_stmt += "INT037_9_Q = POWER(INT037_9, 2),";
  sql_stmt += "FT212_43_R = POWER((CASE WHEN FT212_43<0 THEN NULL ELSE FT212_43 END), 0.5), ";
  sql_stmt += "FS042_Q = POWER(FS042, 2) WHERE IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

//variable transformation
  sql_stmt = "UPDATE PM SET ";
  sql_stmt += "MT118_42_T = (CASE WHEN MT118_42>1.375 THEN -0.93 ELSE MT118_42 END), ";
  sql_stmt += "MT104_42_Q_T = (CASE WHEN MT104_42_Q>350000 THEN 304792.43 ELSE MT104_42_Q END), ";
  sql_stmt += "MT011_43_Z = (CASE WHEN MT011_43 = 0 THEN 1 ELSE 0 END), ";
  sql_stmt += "MS119_3M_R_T = (CASE WHEN MS119_3M_R>0.52 THEN 0.45 ELSE MS119_3M_R END), ";
  sql_stmt += "MS108_12M_R_T = (CASE WHEN MS108_12M_R>7.5 THEN 10.35 ELSE MS108_12M_R END), ";
  sql_stmt += "MS064_R_T = (CASE WHEN MS064_R IS NULL THEN -7.60 WHEN MS064_R>15 THEN 5.44 ELSE MS064_R END), ";
  sql_stmt += "INT084_9_R_T = (CASE WHEN INT084_9_R IS NULL THEN -0.43 WHEN INT084_9_R>0.6 THEN 0.29 ELSE INT084_9_R END), ";
  sql_stmt += "INT062_9_T1 = (CASE WHEN INT062_9>480 THEN 484.44 ELSE INT062_9 END), ";
  sql_stmt += "INT037_9_Q_T1 = (CASE WHEN INT037_9_Q IS NULL THEN -0.1 WHEN INT037_9_Q>0.96 THEN 0.30 ELSE INT037_9_Q END), ";
  sql_stmt += "FT212_43_R_T = (CASE WHEN FT212_43_R IS NULL THEN 0.10 WHEN FT212_43_R>1 THEN -1.51 ELSE FT212_43_R END), ";
  sql_stmt += "FS106_12M_T = (CASE WHEN FS106_12M>1.25 THEN 0.85 ELSE FS106_12M END), ";
  sql_stmt += "FS042_Q_T = (CASE WHEN FS042_Q IS NULL THEN -0.35 WHEN (FS042_Q>2 AND FS042_Q<6) THEN 0.64 WHEN FS042_Q>16 THEN -2.55 ELSE FS042_Q END), ";
  sql_stmt += "FS030_T = (CASE WHEN FS030 IS NULL THEN 0.12 WHEN FS030>4 THEN -1.19 ELSE FS030 END) ";
  sql_stmt += "WHERE IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

//score calculation
  sql_stmt = "UPDATE PM SET SCORE = 6983.56585+FS030_T*1573.47038+";
  sql_stmt +="FS042_Q_T*1185.2782+FS106_12M_T*2228.0058+FT212_43_R_T*4435.87825+";
  sql_stmt += "INT037_9_Q_T1*21844+INT062_9_T1*-19.98018+INT084_9_R_T*8933.21573+";
  sql_stmt +="MS064_R_T*489.60121+MS108_12M_R_T*504.85556+MS119_3M_R_T*11820+";
  sql_stmt +="MT011_43_Z*4135.46056+MT104_42_Q_T*0.0372+MT118_42_T*5906.03891 ";
  sql_stmt += "WHERE IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

//decile cut
  sql_stmt = "UPDATE PM SET DECILE = (CASE WHEN SCORE <=-1766.96 THEN 1 ";
  sql_stmt += "WHEN SCORE <=1215.25 THEN 2 WHEN SCORE <=3306.38 THEN 3 ";
  sql_stmt += "WHEN SCORE <=5422.75 THEN 4 WHEN SCORE <=7761.67 THEN 5 ";
  sql_stmt += "WHEN SCORE <=10909.35 THEN 6 WHEN SCORE <=14857.95 THEN 7 ";
  sql_stmt += "WHEN SCORE <=21364.09 THEN 8 WHEN SCORE <=29957.94 THEN 9 ";
  sql_stmt += "WHEN SCORE >29957.94 THEN 10 ELSE 0 END) WHERE EXCLUSION = 0 ";
  sql_stmt += "AND IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();

  sql_stmt = "SELECT SCORE FROM PM WHERE IDN = :idno;";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idno"] = idno;
  query->Open();
  score = query->FieldValues["SCORE"];
  query->Close();
  };
  return score;
};

int propensity_cut(TADOQuery *query, const AnsiString &IDN)
{
  AnsiString sql_stmt = "SELECT DECILE, EXCLUSION FROM PM WHERE IDN = :IDN;";
    sql_stmt = sql_stmt.UpperCase();
  int decile;
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["IDN"]= IDN;
  query->Open();
  if(query->RecordCount == 0)
    decile = -1; //No data at all
  else if(query->FieldValues["EXCLUSION"] == 1)
    decile = -2; //No KRM023 data
  else if(query->FieldValues["EXCLUSION"] == 2)
    decile = -4; //KRM023 max bucket > 3
  else
    decile = query->FieldValues["DECILE"];
  query->Close();
  return decile;
};

unsigned int get_data_availability(TADOQuery *query, const AnsiString &idno)
{
	AnsiString sql_stmt;
	sql_stmt = "SELECT HIT FROM IDN_LIST WHERE IDN = :idno;";
	sql_stmt = sql_stmt.UpperCase();
	query->Close();
	query->SQL->Clear();
	query->SQL->Add(sql_stmt);
	query->Parameters->ParamValues["idno"] = idno;
	query->Open();
  unsigned char flag;
  if(query->FieldByName("HIT")->IsNull)
    flag = 0;
  else
	  flag = query->FieldValues["HIT"];
	query->Close();
	return flag;
};

void build_bam_bucket(TADOCommand *command, const AnsiString &bam086, int now)
{
  AnsiString sql_stmt;
  sql_stmt = "ALTER TABLE " + bam086;
  sql_stmt += " add ";
  sql_stmt += "cycles int, pc_c01 char, pc_c02 char, pc_c03 char, pc_c04 char, pc_c05 char, pc_c06 char, ";
  sql_stmt += "            pc_c07 char, pc_c08 char, pc_c09 char, pc_c10 char, pc_c11 char, pc_c12 char,  ";
  sql_stmt += "            pc_01 float, pc_02 float, pc_03 float, pc_04 float, pc_05 float, pc_06 float, ";
  sql_stmt += "            pc_07 float, pc_08 float, pc_09 float, pc_10 float, pc_11 float, pc_12 float ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Execute();

  sql_stmt = "update " + bam086;
  sql_stmt += " set ";
  sql_stmt += "cycles = len(isnull(pay_code_12, '')) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Execute();

  sql_stmt = "update " + bam086;
  sql_stmt += " set ";
  sql_stmt += "pc_c01 = (case when cycles < 12 then null else right(left(pay_code_12,12),1) end), ";
  sql_stmt += "pc_c02 = (case when cycles < 11 then null else right(left(pay_code_12,11),1) end), ";
  sql_stmt += "pc_c03 = (case when cycles < 10 then null else right(left(pay_code_12,10),1) end), ";
  sql_stmt += "pc_c04 = (case when cycles < 9 then null else right(left(pay_code_12,9),1) end), ";
  sql_stmt += "pc_c05 = (case when cycles < 8 then null else right(left(pay_code_12,8),1) end), ";
  sql_stmt += "pc_c06 = (case when cycles < 7 then null else right(left(pay_code_12,7),1) end), ";
  sql_stmt += "pc_c07 = (case when cycles < 6 then null else right(left(pay_code_12,6),1) end), ";
  sql_stmt += "pc_c08 = (case when cycles < 5 then null else right(left(pay_code_12,5),1) end), ";
  sql_stmt += "pc_c09 = (case when cycles < 4 then null else right(left(pay_code_12,4),1) end), ";
  sql_stmt += "pc_c10 = (case when cycles < 3 then null else right(left(pay_code_12,3),1) end), ";
  sql_stmt += "pc_c11 = (case when cycles < 2 then null else right(left(pay_code_12,2),1) end), ";
  sql_stmt += "pc_c12 = (case when cycles < 1 then null else left(pay_code_12,1) end) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Execute();

  sql_stmt = "update " + bam086;
  sql_stmt += " set ";
  sql_stmt += "pc_01 = (case  ";
  sql_stmt += "           when pc_c01 = 'X' then 0 ";
  sql_stmt += "           when pc_c01 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c01 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c01 is null then null ";
  sql_stmt += "           else convert(float, pc_c01) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_02 = (case  ";
  sql_stmt += "           when pc_c02 = 'X' then 0 ";
  sql_stmt += "           when pc_c02 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c02 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c02 is null then null ";
  sql_stmt += "           else convert(float, pc_c02) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_03 = (case  ";
  sql_stmt += "           when pc_c03 = 'X' then 0 ";
  sql_stmt += "           when pc_c03 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c03 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c03 is null then null ";
  sql_stmt += "           else convert(float, pc_c03) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_04 = (case  ";
  sql_stmt += "           when pc_c04 = 'X' then 0 ";
  sql_stmt += "           when pc_c04 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c04 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c04 is null then null ";
  sql_stmt += "           else convert(float, pc_c04) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_05 = (case  ";
  sql_stmt += "           when pc_c05 = 'X' then 0 ";
  sql_stmt += "           when pc_c05 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c05 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c05 is null then null ";
  sql_stmt += "           else convert(float, pc_c05) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_06 = (case  ";
  sql_stmt += "           when pc_c06 = 'X' then 0 ";
  sql_stmt += "           when pc_c06 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c06 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c06 is null then null ";
  sql_stmt += "           else convert(float, pc_c06) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_07 = (case  ";
  sql_stmt += "           when pc_c07 = 'X' then 0 ";
  sql_stmt += "           when pc_c07 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c07 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c07 is null then null ";
  sql_stmt += "           else convert(float, pc_c07) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_08 = (case  ";
  sql_stmt += "           when pc_c08 = 'X' then 0 ";
  sql_stmt += "           when pc_c08 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c08 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c08 is null then null ";
  sql_stmt += "           else convert(float, pc_c08) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_09 = (case  ";
  sql_stmt += "           when pc_c09 = 'X' then 0 ";
  sql_stmt += "           when pc_c09 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c09 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c09 is null then null ";
  sql_stmt += "           else convert(float, pc_c09) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_10 = (case  ";
  sql_stmt += "           when pc_c10 = 'X' then 0 ";
  sql_stmt += "           when pc_c10 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c10 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c10 is null then null ";
  sql_stmt += "           else convert(float, pc_c10) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_11 = (case  ";
  sql_stmt += "           when pc_c11 = 'X' then 0 ";
  sql_stmt += "           when pc_c11 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c11 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c11 is null then null ";
  sql_stmt += "           else convert(float, pc_c11) ";
  sql_stmt += "         end), ";
  sql_stmt += "pc_12 = (case  ";
  sql_stmt += "           when pc_c12 = 'X' then 0 ";
  sql_stmt += "           when pc_c12 = 'A' then 0.25 ";
  sql_stmt += "           when pc_c12 = 'B' then 0.5 ";
  sql_stmt += "           when pc_c12 is null then null ";
  sql_stmt += "           else convert(float, pc_c12) ";
  sql_stmt += "         end) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Execute();

    try{
      sql_stmt = "DROP TABLE y_BAM086_tmp;";
      sql_stmt = sql_stmt.UpperCase();
      command->CommandText = sql_stmt;
    	command->Execute();
    }
    catch( Exception &E){
      if (AnsiString(E.ClassName()) == "EOleException")
        if(command->Connection->Errors->Item[0]->NativeError == 3701)
          command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
    }

  try{
    sql_stmt = "create table y_BAM086_tmp ";
    sql_stmt += "(IDN CHAR(12),  ";
    sql_stmt += " DATA_YYY Char(5), DATA_MM Char(2), BANK_CODE Char(7), BANK_NAME Char(40), ";
    sql_stmt += " ACCOUNT_CODE Char, ACCOUNT_CODE2 Char, PURPOSE_CODE Char, CONTRACT_AMT int,  ";
    sql_stmt += " LOAN_AMT int, PASS_DUE_AMT int, CO_LOAN Char, Bank_Code2 Char(3), Bucket float, Cycle int) ";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
	  command->Execute();
  }
  catch(Exception &E){
    if (static_cast<AnsiString>(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 2714)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
  }

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_01, 1 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_02, 2 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_03, 3 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_04, 4 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_05, 5 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_06, 6 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_07, 7 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_08, 8 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_09, 9 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_10, 10 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_11, 11 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_12, 12 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();
};

//Input: data availability code :
//0x01: krm023;
//0x02: krm001;
//0x04: bam086;
//0x10: jas002
//output
//0x01: No krm023;
//0x02: No krm001;
//0x04: krm001 first card open less than 6 months
//0x08: krm023 max bucket > 3;
//0x10: bam086 pass due amount > 0;
//0x20: jas002 major derag;
//0x40: bam086 cash card most current month delinquent
//0x80: bam086 cash card utilization >= 95%;
//0x100: bam086 pay_code_12 max > 1;
//0x200: credit card utilization > 0.75
//0x400:sum of credit card revolving balance and cash card balance > NT$500,000
//0x800:total lines no larger than 2 and has delinquent line(s)
//0x1000:Data insufficient (小) with more than two recent inquiries
//0x2000:Data insufficient (小)
unsigned int in_daco41(TADOQuery *query,
                const AnsiString &krm023, const AnsiString &krm001,
                const AnsiString &jas002, const AnsiString &bam085,
                const AnsiString &stm001,
                const AnsiString &idno,
                unsigned int flag, int now,
                unsigned int &unsecured_balance_opt, unsigned int &unsecured_balance_per,
                unsigned int &max_line, bool &p1)
{
  AnsiString sql_stmt;
  int ever_delinquent_line = 0;
  int exclusion = 0;
  int count = 0;
  double credit_line = 0;
  double credit_card_rev_balance_per = 0;
  double credit_card_rev_balance_opt = 0;
  double spread_payment_balance_per = 0;
  double spread_payment_balance_opt = 0;
  double unsecured_loan_balance = 0;
  double cash_card_balance = 0;
  if ((flag & 0x1)==0)
    exclusion |= 0x1;
  else{
    sql_stmt = "SELECT MAX(Bucket_ef_1k) As ef FROM " + krm023 + " ";
    sql_stmt +="WHERE IDN = :idno;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idno"] = idno;
    query->Open();
    int max_bucket = query->FieldValues["ef"];
    query->Close();
    if (max_bucket > 3)
      exclusion |= 0x8;

    sql_stmt = "SELECT MIN(MON_SINCE) AS MIN_MON_SINCE, MAX(MON_SINCE) AS MAX_MON_SINCE FROM " + krm023 + " ";
    sql_stmt +="WHERE IDN = :idno;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idno"] = idno;
    query->Open();
    int min_stmt = query->FieldValues["MIN_MON_SINCE"];
    int max_stmt = query->FieldValues["MAX_MON_SINCE"];
    query->Close();
    if(now - min_stmt < 6)
      exclusion |= 0x4;
    if(now - max_stmt > 6)
      exclusion |= 0x4;


    sql_stmt = "SELECT SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END) AS BAL, ";
    sql_stmt += "SUM(CASE WHEN ((PAY_CODE IN ('C', 'D', 'E', 'F')) OR (CASH = 'Y')) THEN SPREAD_PAYMENT ELSE 0 END) AS SPREAD_PAYMENT ";
    sql_stmt += "FROM " + krm023;
    sql_stmt += " WHERE :now - MON_SINCE = 1 AND IDN = :idno";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idno"] = idno;
    query->Parameters->ParamValues["now"] = now;
    query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      credit_card_rev_balance_per = query->FieldValues["BAL"];
      credit_card_rev_balance_per *= 1000;
    };
    if(!query->FieldValues["SPREAD_PAYMENT"].IsNull()){
      spread_payment_balance_per = query->FieldValues["SPREAD_PAYMENT"];
      spread_payment_balance_per *= 1000;
    };

    try{
      sql_stmt = "DROP TABLE KRM023_RANGE_TMP";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->ExecSQL();
    }
    catch(Exception &E){
      if (AnsiString(E.ClassName()) == "EOleException")
        if(query->Connection->Errors->Item[0]->NativeError == 3701)
          query->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
    };

    sql_stmt = "CREATE TABLE KRM023_RANGE_TMP ";
    sql_stmt += "(IDN CHAR(11), ISSUE CHAR(3), MON INT)";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->ExecSQL();

    sql_stmt = "INSERT INTO KRM023_RANGE_TMP(IDN, ISSUE, MON) ";
    sql_stmt += "SELECT IDN, ISSUE, MAX(MON_SINCE) FROM " + krm023 + " ";
    sql_stmt += "WHERE :now - MON_SINCE <= 3 ";
    sql_stmt += "GROUP BY IDN, ISSUE";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["now"] = now;
    query->ExecSQL();

    sql_stmt = "SELECT SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END) AS BAL, ";
    sql_stmt += "SUM(CASE WHEN ((PAY_CODE IN ('C', 'D', 'E', 'F')) OR (CASH = 'Y')) THEN SPREAD_PAYMENT ELSE 0 END) AS SPREAD_PAYMENT, ";
    sql_stmt += "SUM(CONVERT(FLOAT, LIMIT)) AS LINE ";
    sql_stmt += "FROM " + krm023 + " AS A ";
    sql_stmt += "INNER JOIN KRM023_RANGE_TMP AS B ON A.IDN = B.IDN AND ";
    sql_stmt += "A.ISSUE = B.ISSUE AND A.MON_SINCE = B.MON AND A.IDN = :idn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      credit_card_rev_balance_opt = query->FieldValues["BAL"];
      credit_card_rev_balance_opt *= 1000;
    };
    if(!query->FieldValues["LINE"].IsNull()){
      credit_line = query->FieldValues["LINE"];
      credit_line *= 1000;
    };
    if(!query->FieldValues["SPREAD_PAYMENT"].IsNull()){
      spread_payment_balance_opt = query->FieldValues["SPREAD_PAYMENT"];
      spread_payment_balance_opt *= 1000;
    };
    if((credit_line > 0) && (static_cast<double>(credit_card_rev_balance_opt / credit_line) > 0.75))
      exclusion |= 0x200;

    sql_stmt = "SELECT COUNT(DISTINCT ISSUE) AS LINES FROM " + krm023 + " ";
    sql_stmt += "WHERE BUCKET_EF_1K > 0 AND IDN = :idno";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idno"] = idno;
    query->Open();
    ever_delinquent_line = query->FieldValues["LINES"];

    sql_stmt = "SELECT COUNT(*) AS CNT FROM " + krm023 + " ";
    sql_stmt += "WHERE IDN = :idno GROUP BY IDN HAVING ";
    sql_stmt += "COUNT(*) = SUM(CASE WHEN PAY_CODE = 'X' THEN 1 ELSE 0 END)";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idno"] = idno;
    query->Open();
    if(query->FieldValues["CNT"].IsNull())
      p1 = false;
    else
      p1 = true;
  };

  if ((flag & 0x2)==0)
    exclusion |= 0x2;
  else{
    try{
      sql_stmt = "CREATE TABLE IND_TMP (IDN CHAR(11), Mon int);";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->ExecSQL();
    }
    catch(Exception &E){
      if (static_cast<AnsiString>(E.ClassName()) == "EOleException")
        if(query->Connection->Errors->Item[0]->NativeError == 2714)
          query->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
    }
    sql_stmt = "DELETE FROM IND_TMP;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->ExecSQL();

    sql_stmt = "INSERT INTO IND_TMP SELECT IDN, MIN(Start_Mon_Since) FROM ";
    sql_stmt += krm001 + " WHERE IDN = :idno GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idno"] = idno;
    query->ExecSQL();

    sql_stmt = "SELECT Mon FROM IND_TMP;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Open();

    int mons_before = now - query->FieldByName("Mon")->AsInteger;
    query->Close();
    if ((mons_before >= 1) && (mons_before < 7))
      exclusion |= 0x4;
    try{
      sql_stmt = "DROP TABLE IND_TMP;";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->ExecSQL();
    }
    catch( Exception &E){
      if (AnsiString(E.ClassName()) == "EOleException")
        if(query->Connection->Errors->Item[0]->NativeError == 3701)
          query->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
    };
    max_line = 0;
    sql_stmt = "SELECT MAX(CONVERT(INT, LIMIT)) AS MAX_LINE FROM " + krm001 + " ";
    sql_stmt += "WHERE STOP_DATE ='9991231' AND M_S = 'Y' AND IDN = :idno";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idno"] = idno;
  	query->Open();
    if(!query->FieldValues["MAX_LINE"].IsNull())
      max_line = query->FieldValues["MAX_LINE"] * 1000;
    if(max_line == 0){
      sql_stmt = "SELECT MAX(CONVERT(INT, LIMIT)) AS MAX_LINE FROM " + krm001 + " ";
      sql_stmt += "WHERE STOP_DATE <> '9991231' AND M_S = 'Y' AND IDN = :idno";
      sql_stmt = sql_stmt.UpperCase();
    	query->Close();
    	query->SQL->Clear();
    	query->SQL->Add(sql_stmt);
    	query->Parameters->ParamValues["idno"] = idno;
    	query->Open();
      if(!query->FieldValues["MAX_LINE"].IsNull())
        max_line = query->FieldValues["MAX_LINE"] * 1000;
    };
    if(ever_delinquent_line > 0){
      sql_stmt = "SELECT COUNT(*) AS LINES FROM OPEN_LINE WHERE IDN = :idno ";
      sql_stmt += "AND MON_SINCE = :now";
      sql_stmt = sql_stmt.UpperCase();
    	query->Close();
    	query->SQL->Clear();
    	query->SQL->Add(sql_stmt);
    	query->Parameters->ParamValues["idno"] = idno;
    	query->Parameters->ParamValues["now"] = now;
    	query->Open();
        if(query->FieldValues["LINES"] <= 2)
          exclusion |= 0x800;
    };
  };
	if(flag & 0x4){
/*----FS044----*/
		sql_stmt ="SELECT IDN, SUM(CASE WHEN PASS_DUE_AMT > 0 THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt +="FROM " + bam085 + " ";
		sql_stmt +="WHERE IDN = :idno GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idno"] = idno;
  	query->Open();
  	count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 0)
      exclusion |= 0x10;
    sql_stmt = "SELECT IDN, SUM(CASE WHEN ACCOUNT_CODE = 'Y' AND ";
    sql_stmt+="                           ISNULL(LEFT(PAY_CODE_12,1), '0') NOT IN ('0', 'X') ";
    sql_stmt+="                      THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt+="FROM " + bam085 + " ";
    sql_stmt+="WHERE IDN = :idno GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idno"] = idno;
  	query->Open();
  	count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 0)
      exclusion |= 0x40;
    sql_stmt = "SELECT IDN, SUM(CASE WHEN ACCOUNT_CODE = 'Y' AND ";
    sql_stmt+="                           ISNULL(CONTRACT_AMT,0) * 0.95 <= (ISNULL(LOAN_AMT,0) + ISNULL(PASS_DUE_AMT,0)) AND ";
    sql_stmt+="                           ISNULL(CONTRACT_AMT,0) > 0 ";
    sql_stmt+="                      THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt+="FROM " + bam085 + " ";
    sql_stmt+="WHERE IDN = :idno GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idno"] = idno;
  	query->Open();
  	count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 0)
      exclusion |= 0x80;

    sql_stmt = "SELECT IDN, MAX(Bucket) as cnt FROM y_BAM086_tmp ";
    sql_stmt += "WHERE IDN = :idno GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idno"] = idno;
  	query->Open();
    if(!query->FieldValues["CNT"].IsNull())
  	  count = query->FieldValues["CNT"];
    else
      count = 0;
  	query->Close();
    if(count > 1)
      exclusion |= 0x100;

    sql_stmt = "SELECT SUM(ISNULL(LOAN_AMT, 0) + ISNULL(PASS_DUE_AMT, 0)) ";
    sql_stmt += "AS BAL FROM " + bam085 + " ";
    sql_stmt += "WHERE IDN = :idno AND ACCOUNT_CODE = 'Y'";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idno"] = idno;
  	query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      cash_card_balance = query->FieldValues["BAL"];
      cash_card_balance *= 1000;
	  };
    sql_stmt = "SELECT SUM(ISNULL(LOAN_AMT, 0) + ISNULL(PASS_DUE_AMT, 0)) ";
    sql_stmt += "AS BAL FROM " + bam085 + " ";
    sql_stmt += "WHERE IDN = :idno AND ACCOUNT_CODE NOT IN ('Y', 'K') AND ";
    sql_stmt += "((ACCOUNT_CODE2 = 'N') OR (ACCOUNT_CODE2 IS NULL))";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
 	  query->SQL->Clear();
   	query->SQL->Add(sql_stmt);
 	  query->Parameters->ParamValues["idno"] = idno;
   	query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      unsecured_loan_balance = query->FieldValues["BAL"];
      unsecured_loan_balance *= 1000;
	  };
  };

  if((credit_card_rev_balance_opt + cash_card_balance) >= 500000)
    exclusion |= 0x400;

	if(flag & 0x10){
//		sql_stmt ="SELECT IDN, SUM(CASE WHEN :now - E_MON_SINCE <= 36 THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt ="SELECT IDN, COUNT(*) AS CNT ";
		sql_stmt +="FROM " + jas002 + " ";
		sql_stmt +="WHERE IDN = :idno GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
//	  query->Parameters->ParamValues["now"] = now;
	  query->Parameters->ParamValues["idno"] = idno;
	  query->Open();
	  int count;
    if(query->FieldByName("CNT")->IsNull)
      count = 0;
    else
      count = query->FieldValues["CNT"];
	  query->Close();
	  if(count > 0)
      exclusion |= 0x20;
	};
  if((!(flag & 0x1)) && (!(flag & 0x2)) && (!(flag & 0x4))){
    sql_stmt = "SELECT COUNT(*) AS CNT FROM "+ stm001 + " WHERE IDN = :idn";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
 	  query->SQL->Clear();
   	query->SQL->Add(sql_stmt);
 	  query->Parameters->ParamValues["idn"] = idno;
   	query->Open();
    if(!query->FieldValues["CNT"].IsNull())
      count = query->FieldValues["CNT"];
    else
      count = 0;
    if(count > 2)
      exclusion |= 0x1000;
    else
      exclusion |= 0x2000;
  };
  unsecured_balance_opt = credit_card_rev_balance_opt + cash_card_balance + unsecured_loan_balance + spread_payment_balance_opt;
  unsecured_balance_per = credit_card_rev_balance_per + cash_card_balance + unsecured_loan_balance + spread_payment_balance_per;

//  exclusion &= 0x07;
  return exclusion;
};

double daco41(TADOQuery *query, TADOCommand *command,
              const AnsiString &krm023, const AnsiString &krm001,
              const AnsiString &jas002, const AnsiString &bam085,
              const AnsiString &stm001, const AnsiString &idno,
              int gender, int now, unsigned int exclusion, unsigned int data_flag)
{
	AnsiString sql_stmt;
	double risk_score  = 1000;

	try{
  	sql_stmt = "CREATE TABLE DACO_V4_1_CAL (IDN CHAR(11), HIT INT, EXCLUSION INT, ";
  	sql_stmt += "gender int, fs002_6m_1k float, fs016_9m float, fs101_9m float, fs040 float, fs068 float, fs069 float,";
  	sql_stmt += "fs031 float, ms203_3m_1k float, ms203_6m_1k float, ms203_9m_1k float, app_last_month_bucket float,";
  	sql_stmt += "fs008_12mplus float, ms011_3m float, ms011_6m float, ms011_9m float, ms011_12m float,";
  	sql_stmt += "ms009_3m float, ms009_6m float, fs005_3m_1k float, ms056_6m_1k float, ";
    sql_stmt += "ms110_3m float, ms110_6m float, ms106_3m float, ms106_6m float, ";
  	sql_stmt += "mt110_43 float, fs002_6m_1k_q float, ms056_6m_1k_tran float, fs069_tran float, int015_9m float,";
  	sql_stmt += "fs031_tran float, mt203_42_1k float, app_last_month_bucket_t1 float, fs008_12mplus_r float,";
  	sql_stmt += "mt011_31 float, mt009_43 float, sex_tran int, ";
  	sql_stmt += "mt110_43_tran float, fs002_6m_1k_q_tran float, int015_9m_tran float, mt203_42_1k_r float,";
  	sql_stmt += "mt011_31_z float, mt009_43_q float,";
  	sql_stmt += "mt203_42_1k_r_tran float, mt009_43_q_tran2 float,";
  	sql_stmt += "score decimal(9,8))";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();
  }
  catch(Exception &E){
  if (static_cast<AnsiString>(E.ClassName()) == "EOleException")
    if(query->Connection->Errors->Item[0]->NativeError == 2714)
      query->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
  };
  sql_stmt = "INSERT INTO DACO_V4_1_CAL(IDN, HIT, EXCLUSION, GENDER) ";
	sql_stmt += "VALUES (:idno, :hit, :exclusion, :gender);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["idno"] = idno;
	command->Parameters->ParamValues["hit"] = data_flag;
	command->Parameters->ParamValues["gender"] = gender;
	command->Parameters->ParamValues["exclusion"] = exclusion;
	command->Execute();

	bool run_model = true;
  if(((exclusion & 0x1) == 0x1) ||  ((exclusion & 0x2) == 0x2))
    run_model = false;

	if (run_model){
//Load person and initialize variables
    if (data_flag & 0x01){ //KRM023
			sql_stmt = "UPDATE DACO_V4_1_CAL SET ";
			sql_stmt += "fs002_6m_1k = 0, fs005_3m_1k = 0, fs016_9m = 0, ";
      sql_stmt += "fs101_9m = 0, ms009_3m = 0, ms009_6m = 0, app_last_month_bucket = 0, ";
      sql_stmt += "ms011_3m = 0, ms011_6m = 0, ms011_9m = 0, ms011_12m = 0, ";
      sql_stmt += "ms056_6m_1k = 0, ms110_3m = 0, ms110_6m = 0, ms106_3m = 0, ms106_6m = 0";
  	};
    if (data_flag & 0x02){ //KRM001
			sql_stmt += ",  fs008_12mplus = 0";
  	};
    if (data_flag & 0x04){ //BAM085
			sql_stmt += ", fs068 = 0, fs069 = 0, fs040 = 0";
  	};
    if (data_flag & 0x08){ //STM001
			sql_stmt += ", fs031 = 0";
  	};
  	sql_stmt += " WHERE IDN = :idno;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
		command->Parameters->ParamValues["idno"] = idno;
		command->Execute();
  	if (data_flag & 0x03){ // KRM001 + KRM023
			sql_stmt = "UPDATE DACO_V4_1_CAL SET ";
			sql_stmt += "ms203_3m_1k = 0, ms203_6m_1k = 0, ms203_9m_1k = 0 ";
			sql_stmt += "FROM " + krm001 + " AS A, " + krm023 + " AS B ";
			sql_stmt += "WHERE A.IDN = B.IDN AND A.IDN = DACO_V4_1_CAL.IDN AND ";
			sql_stmt += "((a.issue = b.issue) OR ";
			sql_stmt += " ((a.issue = '021' and a.card_brand = 'V') and (b.issue = 'CTV')) OR ";
			sql_stmt += " ((a.issue = '021' and a.card_brand = 'M') and (b.issue = 'CTM')) OR ";
      sql_stmt += " ((a.issue = '021' and a.card_brand = 'D') and (b.issue = 'CTD')) OR ";
      sql_stmt += " ((a.issue = 'A82' and a.card_brand = 'A') and (b.issue = 'AEA'))) AND ";
	  	sql_stmt += "A.IDN = :idno;";
  	  sql_stmt = sql_stmt.UpperCase();
  		command->CommandText = sql_stmt;
			command->Parameters->ParamValues["idno"] = idno;
			command->Execute();
  	};

//Variable generation
//fs002
 		sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();
		sql_stmt = "delete from tmp;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, mon_since, 1 from " + krm023;
    sql_stmt += " where bucket_ef_1k >= 1 ";
    sql_stmt += "group by IDN, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "insert into tmp (IDN, mon, v1) ";
    sql_stmt += "select IDN, 6, sum(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= 6 and ";
    sql_stmt += "(:now1 - mon) >= 0 group by IDN;";
		sql_stmt = sql_stmt.UpperCase();
 		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
    command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt = " update DACO_V4_1_CAL set fs002_6m_1k = v1 ";
    sql_stmt += "from tmp as a where a.IDN = DACO_V4_1_CAL.IDN AND ";
    sql_stmt += "mon = 6;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = " update DACO_V4_1_CAL set fs002_6m_1k = (case when fs002_6m_1k > 6 then 6 else fs002_6m_1k end)";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

//fs040
		sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "insert into tmp1 (IDN, v1) ";
		sql_stmt += "select IDN, count(*) from " + bam085;
		sql_stmt += " where (account_code2=''or account_code2 is null) AND ";
		sql_stmt += "account_code <> 'K' group by IDN;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "update DACO_V4_1_CAL set fs040 = v1 ";
		sql_stmt += "from tmp1 as a ";
		sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

//fs068
		sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();
		sql_stmt = "delete from tmp2;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "insert into tmp2(IDN, issuer) ";
		sql_stmt += "select IDN, bank_code2 from " + bam085;
		sql_stmt += " where purpose_code = '1' and ";
		sql_stmt += "(account_code2 is not null or account_code2 <> '') ";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "insert into tmp1(IDN, v1) ";
		sql_stmt += "select IDN, count(*) from " + bam085;
		sql_stmt += " where not exists (select * from tmp2 ";
		sql_stmt += "where " + bam085 + ".IDN = tmp2.IDN and ";
		sql_stmt += bam085 + ".bank_code2 = tmp2.issuer) and ";
		sql_stmt += "purpose_code = '1' and ";
		sql_stmt += "((account_code2 is null) or (account_code2 = '')) and ";
		sql_stmt += "account_code != 'K' group by IDN;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "update DACO_V4_1_CAL set fs068 = v1 ";
		sql_stmt += "from tmp1 as a ";
		sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

//fs069
		sql_stmt = "update DACO_V4_1_CAL ";
		sql_stmt += "set fs069 = fs040 - fs068";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

//fs016
    sql_stmt = "delete from tmp1;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

    sql_stmt = "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, 9, count(distinct issue) from " + krm023;
    sql_stmt += " where (:now - mon_since) <= 9 and ";
    sql_stmt += "(:now1 - mon_since) >= 0 and ";
    sql_stmt += "cash = 'Y' group by IDN;";
		sql_stmt = sql_stmt.UpperCase();
 		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
    command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set fs016_9m = v1 ";
    sql_stmt += "from tmp1 as a where a.IDN = DACO_V4_1_CAL.IDN and ";
    sql_stmt += "mon = 9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

//fs101
    sql_stmt = "delete from tmp1;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

    sql_stmt = "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, 9, count(distinct issue) from " + krm023;
    sql_stmt += " where (:now - mon_since) <= 9 and ";
    sql_stmt += "(:now1 - mon_since) >= 0 group by IDN;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
    command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set fs101_9m = v1 ";
    sql_stmt += "from tmp1 as a where a.IDN = DACO_V4_1_CAL.IDN and ";
    sql_stmt += " mon = 9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

//fs031
    sql_stmt = "delete from tmp ";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

    sql_stmt = "insert into tmp (IDN, v1) ";
    sql_stmt += "select IDN, count(*) from " + stm001;
    sql_stmt += " where item_list is not null and ";
    sql_stmt += "item_list <> '' and (:now - query_mon_since) <= 12 ";
    sql_stmt += "group by IDN";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set fs031 = v1 from tmp as a where a.IDN = DACO_V4_1_CAL.IDN";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

//fs005
    sql_stmt = "delete from tmp";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();
    sql_stmt = "delete from tmp1";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp (IDN, mon, v1) ";
    sql_stmt += "select IDN, 3, count(*) from " + krm023;
    sql_stmt += " where bucket_f_1k >= 2 and ";
    sql_stmt += " (:now - mon_since) <= 3 and  (:now1 - mon_since) >= 0 ";
    sql_stmt += "group by IDN";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set fs005_3m_1k = v1 from tmp as a ";
    sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN and mon = 3";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set fs005_3m_1k = (case when fs005_3m_1k > 3 then 3 else fs005_3m_1k end)";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

//ms056
    sql_stmt = "delete from tmp";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();
    sql_stmt = "delete from tmp1";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, mon_since, sum(payment_amt) from " + krm023;
    sql_stmt += " where pay_code in ('D', 'E', 'F') and bucket_def_1k >= 1 ";
    sql_stmt += "group by IDN, mon_since";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp(IDN, mon, v1) ";
    sql_stmt += "select IDN, 6, max(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= 6 and (:now1 - mon) >= 0 ";
    sql_stmt += "group by IDN";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
    command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set ms056_6m_1k = v1 from tmp as a ";
    sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN and mon=6";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

//App_last_bucket
    sql_stmt = "delete from tmp";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "delete from tmp1";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp1 (IDN, MON)";
    sql_stmt += "select IDN, max(MON_SINCE) FROM " + krm023;
    sql_stmt += " group by IDN";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->CommandText = sql_stmt;
 		command->Execute();


    sql_stmt = "insert into tmp (IDN, v1) ";
    sql_stmt += "select A.IDN, max(bucket_ef_1k) from " + krm023;
    sql_stmt += " as A INNER JOIN TMP1 AS B ON A.IDN = B.IDN AND ";
    sql_stmt += " A.MON_SINCE = B.MON ";
    sql_stmt += "group by A.IDN";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set app_last_month_bucket = v1 from tmp a ";
    sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

//fs008_12mplus
    sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();
    sql_stmt += "delete from tmp;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt += "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, 13, count(distinct issue) from " + krm001;
    sql_stmt += " as a where :now - end_mon_since > 0 and ";
    sql_stmt += "stop_code = '3' and issue not in ('021', '081', '974') and ";
    sql_stmt += "issue not in (select distinct issue ";
    sql_stmt += "from open_issuer as b ";
    sql_stmt += "where a.IDN = b.IDN and a.end_mon_since = b.mon) ";
    sql_stmt += "group by IDN;";
		sql_stmt = sql_stmt.UpperCase();
 		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp (IDN, mon, v1, v2, v3) ";
    sql_stmt += "select distinct IDN, 13, 0, 0, 0 from " + krm001;
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update tmp set v1 = a.v1 from tmp1 as a ";
    sql_stmt += "where a.IDN = tmp.IDN and a.mon = tmp.mon;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, 13, count(*) from " + krm001;
    sql_stmt += " where stop_code = '3' and ";
    sql_stmt += "issue in ('021', '081', '974') and ";
    sql_stmt += ":now - end_mon_since > 0 group by IDN;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update tmp set v2 = a.v1 ";
    sql_stmt += "from tmp1 as a ";
    sql_stmt += "where a.IDN = tmp.IDN and a.mon = tmp.mon;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set fs008_12mplus = v1 + v2 ";
    sql_stmt += "from tmp as a ";
    sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN and mon = 13;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();


//ms009
    sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();
    sql_stmt = "delete from tmp;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, mon_since, sum(payment_amt) from " + krm023;
    sql_stmt += " group by IDN, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "declare @i int set @i = 3 ";
    sql_stmt += "while @i <= 6 begin ";
    sql_stmt += "insert into tmp (IDN, mon, v1) ";
    sql_stmt += "select IDN, @i, avg(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= @i and ";
    sql_stmt += "(:now1 - mon) > 0 group by IDN ";
    sql_stmt += " set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set ms009_3m = v1 from tmp as a ";
    sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN and mon=3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set ms009_6m = v1 from tmp as a ";
    sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN and mon=6;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

//ms011
    sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();
    sql_stmt = "delete from tmp;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, mon_since, sum(case when pay_code in ('A', 'B') then payment_amt else 0 end) ";
    sql_stmt += "from " + krm023 + " group by IDN, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "declare @i int set @i = 3 ";
    sql_stmt += "while @i <= 12 begin ";
    sql_stmt += "insert into tmp (IDN, mon, v1) select IDN, @i, avg(v1) ";
    sql_stmt += "from tmp1 where (:now - mon) <= @i and (:now1 - mon) > 0 ";
    sql_stmt += " group by IDN set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms011_3m = v1 ";
    sql_stmt += "from tmp as a where a.IDN = DACO_V4_1_CAL.IDN ";
    sql_stmt += "and mon = 3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms011_6m = v1 ";
    sql_stmt += "from tmp as a where a.IDN = DACO_V4_1_CAL.IDN ";
    sql_stmt += "and mon = 6;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms011_9m = v1 ";
    sql_stmt += "from tmp as a where a.IDN = DACO_V4_1_CAL.IDN ";
    sql_stmt += "and mon = 9;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms011_12m = v1 ";
    sql_stmt += "from tmp as a where a.IDN = DACO_V4_1_CAL.IDN ";
    sql_stmt += "and mon = 12;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

//ms106
    sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();
    sql_stmt = "delete from tmp;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, mon_since, sum(case when pay_code in ('C', 'D', 'E', 'F') then payment_amt ";
    sql_stmt += "else 0 end) from " + krm023;
    sql_stmt += " group by IDN, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "declare @i int set @i = 3 ";
    sql_stmt += "while @i <= 6 begin ";
    sql_stmt += "insert into tmp (IDN, mon, v1) ";
    sql_stmt += "select IDN, @i, avg(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= @i and (:now1 - mon) >= 0 ";
    sql_stmt += "group by IDN set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms106_3m = v1 ";
    sql_stmt += "from tmp as a where a.IDN = DACO_V4_1_CAL.IDN and mon=3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms106_6m = v1 ";
    sql_stmt += "from tmp as a where a.IDN = DACO_V4_1_CAL.IDN and mon=6;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

//ms110
    sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();
    sql_stmt = "delete from tmp;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, mon_since, sum(convert(decimal(16, 8), (case when limit='' then NULL else limit end))) ";
    sql_stmt += "from " + krm023;
    sql_stmt += " group by IDN, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "declare @i int set @i = 3 ";
    sql_stmt += "while @i <= 6 begin ";
    sql_stmt += "insert into tmp (IDN, mon, v1) select IDN, @i, max(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= @i and (:now1 - mon) >= 0 ";
    sql_stmt += "group by IDN set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt += "update DACO_V4_1_CAL ";
    sql_stmt += "set ms110_3m = (case when v1=0 then null else ms106_3m / v1 end) ";
    sql_stmt += "from tmp as a where a.IDN = DACO_V4_1_CAL.IDN ";
    sql_stmt += "and mon=3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt += "update DACO_V4_1_CAL ";
    sql_stmt += "set ms110_6m = (case when v1=0 then null else ms106_6m / v1 end) ";
    sql_stmt += "from \tmp as a where a.IDN = DACO_V4_1_CAL.IDN ";
    sql_stmt += "and mon=6;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

//ms203_1k
    sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

 		sql_stmt = "declare @i int set @i = 3 ";
    sql_stmt += "while @i <= 9 begin insert into tmp1 (IDN, mon, v1) ";
    sql_stmt += "select IDN, @i, sum(payment_amt) from " + krm023 + " as a ";
    sql_stmt += "where issue in (select issue from latest_line ";
    sql_stmt += "where mob <= 12 and IDN = a.IDN) and bucket_def_1k <> 0 and ";
    sql_stmt += ":now - mon_since <= @i and :now1 - mon_since > 0 ";
    sql_stmt += "group by IDN set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt ="  update DACO_V4_1_CAL ";
    sql_stmt += "set ms203_3m_1k = v1 from tmp1 as a ";
    sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN and a.mon = 3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt ="  update DACO_V4_1_CAL ";
    sql_stmt += "set ms203_6m_1k = v1 from tmp1 as a ";
    sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN and a.mon = 6;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt ="  update DACO_V4_1_CAL ";
    sql_stmt += "set ms203_9m_1k = v1 from tmp1 as a ";
    sql_stmt += "where a.IDN = DACO_V4_1_CAL.IDN and a.mon = 9;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

//Trend variables and first round transformation
    sql_stmt = "update DACO_V4_1_CAL set ";
    sql_stmt += "mt110_43 =  ms110_3m -  (ms110_6m +  ms110_3m), ";
    sql_stmt += "mt203_42_1k = ms203_3m_1k - ms203_9m_1k + ms203_6m_1k, ";
    sql_stmt += "mt009_43 = 2 * ms009_3m - ms009_6m, ";
    sql_stmt += "mt011_31 = ms011_6m - ms011_3m - ms011_12m + ms011_9m, ";
    sql_stmt += "sex_tran = gender, fs002_6m_1k_q = power(fs002_6m_1k, 2), ";
    sql_stmt += "int015_9m = (case when fs101_9m = 0 then null else fs016_9m / fs101_9m end), ";
    sql_stmt += "fs008_12mplus_r = power((case when fs008_12mplus < 0 then null else fs008_12mplus end), 0.5), ";
    sql_stmt += "ms056_6m_1k_tran = ms056_6m_1k, ";//(case when ms056_6m_1k >120 then 120 else ms056_6m_1k end), ";
    sql_stmt += "fs069_tran = (case when fs069 is null then -0.5 else fs069 end), ";//when fs069 > 6 then 6 rnd), "";
    sql_stmt += "fs031_tran = (case when fs031 is null then 0 else fs031 end), "; //when fs031 > 8 then 8 else fs031 end), ";
    sql_stmt += "app_last_month_bucket_t1 = (case when app_last_month_bucket is null then 0 else app_last_month_bucket end) ";
    sql_stmt += "WHERE IDN = :idno";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["idno"] = idno;
 		command->Execute();

//Second round transformation
    sql_stmt = "update DACO_V4_1_CAL set ";
    sql_stmt += "mt110_43_tran = (case when mt110_43 is null then 0 ";
    sql_stmt += "when mt110_43 < 0 then 0 "; //when mt110_43 > 1 then 1 ";
    sql_stmt += "else mt110_43 end), ";
    sql_stmt += "mt203_42_1k_r = power((case when mt203_42_1k < 0 then null else mt203_42_1k end), 0.5), ";
    sql_stmt += "fs002_6m_1k_q_tran = fs002_6m_1k_q, ";//(case when fs002_6m_1k_q > 16 then 16 else fs002_6m_1k_q end), ";
    sql_stmt += "mt011_31_z = (case when ABS(mt011_31) < 0.00000001 then 1 else 0 end), ";
    sql_stmt += "mt009_43_q = power(mt009_43, 2), ";
		sql_stmt += "int015_9m_tran = (case when int015_9m is null then 0.4 else int015_9m end) ";
//    sql_stmt += "when int015_9m > 0.4 then 0.4 else int015_9m end) ";
    sql_stmt += "WHERE IDN = :idno";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["idno"] = idno;
 		command->Execute();

//Third round transformation
  	sql_stmt = "update DACO_V4_1_CAL set ";
    sql_stmt += "mt203_42_1k_r_tran = (case when mt203_42_1k_r is null then 3.3 else mt203_42_1k_r end), ";
//    sql_stmt += "when mt203_42_1k_r > 6 then 6 else mt203_42_1k_r end),";
  	sql_stmt += "mt009_43_q_tran2=(case when mt009_43_q > 75000 then 1 else 0 end) ";
  	sql_stmt += "WHERE IDN = :idno";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["idno"] = idno;
 		command->Execute();

//Score calculation
    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set score = -0.02916 ";
    sql_stmt += "+ ms056_6m_1k_tran	*	0.00100 ";
    sql_stmt += "+ mt110_43_tran		*	0.14941 ";
    sql_stmt += "+ fs002_6m_1k_q_tran	*	0.01346 ";
    sql_stmt += "+ int015_9m_tran		*	0.26009 ";
    sql_stmt += "+ fs031_tran		*	0.01318 ";
    sql_stmt += "+ mt203_42_1k_r_tran	*	0.02712 ";
    sql_stmt += "+ app_last_month_bucket_t1 *	0.08179 ";
    sql_stmt += "+ fs008_12mplus_r	*	0.20956 ";
    sql_stmt += "+ fs069_tran		*	0.01431 ";
    sql_stmt += "+ mt011_31_z		*	0.06573 ";
    sql_stmt += "+ fs005_3m_1k	*	0.12226 ";
    sql_stmt += "+ sex_tran		*	0.02438 ";
    sql_stmt += "+ mt009_43_q_tran2	*	0.09215 ";
    sql_stmt += "WHERE IDN = :idno";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["idno"] = idno;
 		command->Execute();

	  sql_stmt = "SELECT SCORE FROM DACO_V4_1_CAL WHERE IDN = :idno;";
	  sql_stmt = sql_stmt.UpperCase();
	  query->Close();
	  query->SQL->Clear();
	  query->SQL->Add(sql_stmt);
	  query->Parameters->ParamValues["idno"] = idno;
	  query->Open();
	  risk_score = query->FieldValues["SCORE"];
	  query->Close();
	};
	return risk_score;
};

void clean_up(TADOCommand *command)
{
  AnsiString sql_stmt;
	try{
  	sql_stmt = "DROP TABLE " + BAM086 + ";";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE " + KRM023 + ";";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
	    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE " + KRM001 + ";";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
	 	if (AnsiString(E.ClassName()) == "EOleException")
  	  if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE " + JAS002 + ";";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE " + JAS002_T + ";";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE " + STM001 + ";";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
	  command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE TMP;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE TMP1;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE TMP2;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE PM;";
    sql_stmt = sql_stmt.UpperCase();
   	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE IDN_LIST;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE IDN_LIST_TMP;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
    sql_stmt = "DROP TABLE OPEN_CARD;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
	    if(command->Connection->Errors->Item[0]->NativeError == 3701)
  	    command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE OPEN_LINE;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
	    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE LATEST_STMT_MON;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE LATEST_LINE;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE OPEN_ISSUER;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE DACO_V4_1_CAL;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
	    if(command->Connection->Errors->Item[0]->NativeError == 3701)
  	    command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
  try{
    sql_stmt = "DROP TABLE y_BAM086_tmp;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
  	command->Execute();
  }
  catch( Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
  try{
    sql_stmt = "DROP TABLE KRM023_RANGE_TMP;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
  	command->Execute();
  }
  catch( Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
  try{
    sql_stmt = "DROP TABLE KRM023_TMP;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
  	command->Execute();
  }
  catch( Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
  try{
    sql_stmt = "DROP TABLE KRM034_BASE;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
  	command->Execute();
  }
  catch( Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
  try{
    sql_stmt = "DROP TABLE KRM037_BASE;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
  	command->Execute();
  }
  catch( Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
	try{
  	sql_stmt = "DROP TABLE KRM023_TMP1;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
/*	try{
  	sql_stmt = "DROP TABLE GG_CAL;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};
	try{
  	sql_stmt = "DROP TABLE LG_CAL;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
  	command->Execute();
	}
	catch(Exception &E){
  	if (AnsiString(E.ClassName()) == "EOleException")
    	if(command->Connection->Errors->Item[0]->NativeError == 3701)
      	command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
	};*/

};

int credit_card_line(TADOQuery *query, double drisk_score, double propensity_score,
                     unsigned int highest_line,
                     int &risk_cut, int &propensity_cut,
                     double &pb, double &dollar_bad, bool p1)
{
  int risk_twentile = 0;
  int propensity_twentile = 0;
  int line;

  if(drisk_score>=0.350166) risk_twentile = 20;
  else if(drisk_score>= 0.250566) risk_twentile = 19;
  else if(drisk_score>=0.18494) risk_twentile = 18;
  else if(drisk_score>=0.139139) risk_twentile = 17;
  else if(drisk_score>=0.10405) risk_twentile = 16;
  else if(drisk_score>=0.07854) risk_twentile = 15;
  else if(drisk_score>=0.06112) risk_twentile = 14;
  else if(drisk_score>=0.042595) risk_twentile = 13;
  else if(drisk_score>=0.029585) risk_twentile = 12;
  else if(drisk_score>=0.02271) risk_twentile = 11;
  else if(drisk_score>=0.01151) risk_twentile = 10;
  else if(drisk_score>=0.0084) risk_twentile = 9;
  else if(drisk_score>=-0.00054) risk_twentile = 8;
  else if(drisk_score>=-0.00478) risk_twentile = 7;
  else if(drisk_score>=-0.011935) risk_twentile = 6;
  else if(drisk_score>=-0.01485) risk_twentile = 5;
  else if(drisk_score>=-0.023135) risk_twentile = 4;
  else if(drisk_score>=-0.02916) risk_twentile = 3;
  else if(drisk_score>=-0.036315) risk_twentile = 2;
  else if(drisk_score<-0.036315) risk_twentile = 1;
  risk_cut = risk_twentile;

  if(p1) propensity_twentile = 1;
  else if(propensity_score >=37972.67018) propensity_twentile = 20;
  else if(propensity_score >=28629.00579) propensity_twentile = 19;
  else if(propensity_score >=22895.03027) propensity_twentile = 18;
  else if(propensity_score >=18042.06276) propensity_twentile = 17;
  else if(propensity_score >=13531.1677) propensity_twentile = 16;
  else if(propensity_score >=9853.66702) propensity_twentile = 15;
  else if(propensity_score >=7135.616463) propensity_twentile = 14;
  else if(propensity_score >=5389.343123) propensity_twentile = 13;
  else if(propensity_score >=3839.897738) propensity_twentile = 12;
  else if(propensity_score >=2591.455401) propensity_twentile = 11;
  else if(propensity_score >=1595.228502) propensity_twentile = 10;
  else if(propensity_score >=705.4672657) propensity_twentile = 9;
  else if(propensity_score >=-252.778643) propensity_twentile = 8;
  else if(propensity_score >=-1160.122547) propensity_twentile = 7;
  else if(propensity_score >=-2178.666869) propensity_twentile = 6;
  else if(propensity_score >=-3102.846491) propensity_twentile = 5;
  else if(propensity_score >=-3987.731732) propensity_twentile = 4;
  else if(propensity_score >=-5019.771815) propensity_twentile = 3;
  else if(propensity_score >=-7270.083616) propensity_twentile = 2;
  else if(propensity_score >=-10607.55531) propensity_twentile = 1;
  propensity_cut = propensity_twentile;

  double pb_table[20][20]={
 0.0006, 0.0006, 0.0006, 0.0006, 0.0006, 0.0006, 0.0006, 0.0007, 0.0008, 0.0009, 0.0011, 0.0015, 0.0021, 0.0029, 0.0042, 0.0060, 0.0085, 0.0120, 0.0167, 0.0231,
 0.0006, 0.0006, 0.0006, 0.0006, 0.0007, 0.0007, 0.0007, 0.0008, 0.0009, 0.0010, 0.0013, 0.0017, 0.0023, 0.0032, 0.0045, 0.0064, 0.0090, 0.0126, 0.0175, 0.0239,
 0.0006, 0.0006, 0.0007, 0.0007, 0.0007, 0.0008, 0.0008, 0.0009, 0.0010, 0.0012, 0.0014, 0.0018, 0.0025, 0.0034, 0.0048, 0.0067, 0.0094, 0.0131, 0.0180, 0.0246,
 0.0006, 0.0007, 0.0007, 0.0008, 0.0008, 0.0008, 0.0009, 0.0010, 0.0011, 0.0013, 0.0015, 0.0020, 0.0026, 0.0036, 0.0050, 0.0070, 0.0097, 0.0135, 0.0185, 0.0252,
 0.0006, 0.0007, 0.0008, 0.0009, 0.0009, 0.0009, 0.0010, 0.0011, 0.0012, 0.0014, 0.0017, 0.0021, 0.0028, 0.0038, 0.0052, 0.0072, 0.0100, 0.0139, 0.0190, 0.0258,
 0.0007, 0.0008, 0.0009, 0.0010, 0.0010, 0.0011, 0.0011, 0.0012, 0.0013, 0.0015, 0.0018, 0.0023, 0.0029, 0.0040, 0.0054, 0.0075, 0.0103, 0.0142, 0.0194, 0.0263,
 0.0007, 0.0009, 0.0010, 0.0011, 0.0012, 0.0012, 0.0013, 0.0014, 0.0015, 0.0017, 0.0020, 0.0025, 0.0032, 0.0042, 0.0057, 0.0078, 0.0107, 0.0146, 0.0199, 0.0269,
 0.0008, 0.0010, 0.0011, 0.0013, 0.0014, 0.0014, 0.0015, 0.0016, 0.0017, 0.0019, 0.0022, 0.0027, 0.0034, 0.0045, 0.0060, 0.0082, 0.0111, 0.0151, 0.0205, 0.0275,
 0.0008, 0.0011, 0.0013, 0.0015, 0.0016, 0.0017, 0.0018, 0.0019, 0.0020, 0.0023, 0.0026, 0.0031, 0.0038, 0.0049, 0.0065, 0.0086, 0.0116, 0.0157, 0.0211, 0.0282,
 0.0009, 0.0013, 0.0015, 0.0017, 0.0019, 0.0020, 0.0021, 0.0023, 0.0024, 0.0027, 0.0030, 0.0036, 0.0043, 0.0054, 0.0070, 0.0092, 0.0123, 0.0164, 0.0219, 0.0291,
 0.0010, 0.0014, 0.0018, 0.0020, 0.0023, 0.0024, 0.0026, 0.0028, 0.0030, 0.0032, 0.0036, 0.0041, 0.0049, 0.0061, 0.0077, 0.0100, 0.0131, 0.0173, 0.0228, 0.0301,
 0.0012, 0.0017, 0.0021, 0.0024, 0.0027, 0.0029, 0.0032, 0.0034, 0.0036, 0.0039, 0.0043, 0.0049, 0.0057, 0.0069, 0.0086, 0.0109, 0.0141, 0.0183, 0.0240, 0.0314,
 0.0014, 0.0020, 0.0025, 0.0029, 0.0033, 0.0036, 0.0039, 0.0041, 0.0044, 0.0048, 0.0052, 0.0059, 0.0067, 0.0080, 0.0097, 0.0121, 0.0153, 0.0197, 0.0254, 0.0329,
 0.0017, 0.0024, 0.0030, 0.0036, 0.0040, 0.0044, 0.0048, 0.0051, 0.0054, 0.0059, 0.0064, 0.0071, 0.0080, 0.0093, 0.0111, 0.0136, 0.0169, 0.0213, 0.0271, 0.0348,
 0.0022, 0.0031, 0.0038, 0.0045, 0.0050, 0.0055, 0.0060, 0.0064, 0.0068, 0.0073, 0.0079, 0.0087, 0.0097, 0.0111, 0.0129, 0.0155, 0.0189, 0.0234, 0.0294, 0.0371,
 0.0030, 0.0041, 0.0050, 0.0058, 0.0065, 0.0071, 0.0077, 0.0082, 0.0087, 0.0093, 0.0100, 0.0108, 0.0120, 0.0134, 0.0154, 0.0180, 0.0215, 0.0262, 0.0322, 0.0401,
 0.0047, 0.0059, 0.0070, 0.0080, 0.0088, 0.0095, 0.0102, 0.0109, 0.0115, 0.0122, 0.0130, 0.0139, 0.0152, 0.0167, 0.0188, 0.0215, 0.0252, 0.0299, 0.0361, 0.0441,
 0.0076, 0.0091, 0.0104, 0.0115, 0.0125, 0.0134, 0.0142, 0.0150, 0.0158, 0.0166, 0.0175, 0.0186, 0.0199, 0.0216, 0.0238, 0.0267, 0.0304, 0.0353, 0.0417, 0.0499,
 0.0128, 0.0146, 0.0161, 0.0174, 0.0186, 0.0197, 0.0207, 0.0216, 0.0225, 0.0235, 0.0245, 0.0258, 0.0272, 0.0291, 0.0314, 0.0344, 0.0383, 0.0434, 0.0499, 0.0582,
 0.0221, 0.0240, 0.0258, 0.0274, 0.0288, 0.0300, 0.0312, 0.0323, 0.0334, 0.0346, 0.0358, 0.0372, 0.0388, 0.0408, 0.0433, 0.0465, 0.0505, 0.0558, 0.0625, 0.0710};

 pb = pb_table[risk_twentile -1][propensity_twentile -1];


  double regular_line[20][20]={
 21, 21, 22, 24, 26, 29, 30, 29, 24, 19, 15, 12, 12, 12, 12, 10, 11, 12, 12, 13, 
 17, 18, 18, 19, 20, 21, 21, 20, 17, 15, 13, 11, 11, 11, 12, 10, 11, 11, 12, 13, 
 16, 16, 16, 16, 17, 17, 17, 16, 15, 13, 12, 11, 11, 11, 12, 10, 10, 11, 12, 12, 
 16, 15, 14, 14, 15, 15, 14, 14, 13, 12, 11, 10, 11, 11, 9, 9, 10, 11, 11, 12, 
 15, 14, 13, 12, 13, 13, 13, 13, 12, 11, 11, 10, 10, 11, 9, 9, 10, 11, 11, 12, 
 15, 12, 11, 11, 11, 11, 11, 11, 11, 10, 10, 10, 10, 11, 9, 9, 10, 11, 11, 12, 
 15, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 9, 9, 10, 10, 11, 11, 
 15, 11, 9, 8, 9, 8, 8, 9, 9, 9, 9, 10, 10, 11, 9, 9, 10, 10, 11, 11,
 15, 10, 8, 8, 8, 7, 7, 8, 8, 8, 9, 9, 10, 11, 9, 9, 10, 10, 11, 11, 
 16, 9, 7, 7, 7, 6, 7, 7, 7, 8, 8, 9, 10, 8, 8, 9, 10, 10, 10, 11, 
 16, 9, 7, 6, 6, 6, 6, 6, 7, 7, 8, 8, 9, 8, 8, 9, 9, 10, 10, 11,
 16, 8, 6, 6, 5, 5, 5, 6, 6, 7, 7, 8, 6, 7, 8, 9, 9, 10, 10, 10, 
 16, 8, 6, 5, 5, 5, 5, 5, 6, 6, 5, 5, 6, 7, 8, 8, 9, 9, 10, 10, 
 14, 7, 6, 5, 5, 5, 5, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9, 9, 10, 10,
 12, 7, 6, 5, 1, 1, 1, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 9, 10,
 10, 7, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 6, 7, 8, 8, 9, 9, 10,
// 8, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 6, 7, 8, 8, 9, 10, 10,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 10, 11,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 5, 6,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 5, 6, 8, 9, 11, 12,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 4, 5, 6, 6,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 5, 7, 8, 10};
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  line = regular_line[risk_twentile -1][propensity_twentile -1] * 10000;
  unsigned int line_cap = highest_line * 1.5;
  if(highest_line == 0)
    line_cap = line;
  else if(line_cap > highest_line + 50000)
   line_cap = highest_line + 50000;

  if(line > line_cap)
    line = line_cap;
  line += 5000;
  line /= 10000;
  line *= 10000;
  
  unsigned int line_index = line / 10000;
  
  if(line > 0)
    dollar_bad = get_dollar_bad(query, risk_twentile, propensity_twentile, line_index);

  return line;
};


void prescreen_out(unsigned int filter_flag, AnsiString idno,
                   AnsiString input_time)
{
  if (filter_flag & 0x20)
    throw cc_error(101, idno, input_time);
  if (filter_flag & 0x8)
    throw cc_error(102, idno, input_time);
  if (filter_flag & 0x40)
    throw cc_error(103, idno, input_time);
  if (filter_flag & 0x80)
    throw cc_error(104, idno, input_time);
  if (filter_flag & 0x10)
    throw cc_error(105, idno, input_time);
  if (filter_flag & 0x100)
    throw cc_error(107, idno, input_time);
   if (filter_flag & 0x400)
    throw cc_error(108, idno, input_time);

  if (filter_flag & 0x800)
    throw cc_error(204, idno, input_time);
  if (filter_flag & 0x200)
    throw cc_error(203, idno, input_time);

/*  if (filter_flag & 0x1000)
    throw cc_error(109, idno, input_time);
  if (filter_flag & 0x2000)
    throw cc_error(201, idno, input_time);*/


  if ((filter_flag & 0x1) || (filter_flag & 0x2) || (filter_flag & 0x4) || (filter_flag & 0x1000) || (filter_flag & 0x2000))
    throw cc_error(200, idno, input_time);
};

void store_result(TADOCommand *command,
                  const AnsiString &idno,
                  const AnsiString &input_time, int result_code,
                  const AnsiString &result_message, int optimal_line,
                  unsigned int unsecured_balance,
                  double pb, int risk_cut, int propensity_cut,
                  double dollar_bad, const AnsiString &note,
                  const AnsiString &version, bool normal)
{
  AnsiString sql_stmt;
  if(normal){
    int result_output;
    switch(result_code){
      case 0:
        result_output = 1;
        break;
      case 106:
        result_output = 2;
        break;
    };
    sql_stmt = "INSERT INTO Result (IDN, Input_Time, ";
    sql_stmt += "Suggested_Line, Suggestion_Code, Result_Code, Suggestion, ";
    sql_stmt += "PB, RISK_GROUP, PROPENSITY_GROUP, UNSECURED_BALANCE, ";
    sql_stmt += "DOLLAR_BAD, SPECIAL_NOTE, VERSION) ";
    sql_stmt += "VALUES(:idno, :input_time, :line, :code, :result_code, ";
    sql_stmt += ":message, :pb, :risk_cut, :propensity_cut, :unsecured_balance, ";
    sql_stmt += ":dollar_bad, :note, :version);";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idno"] = idno;
    command->Parameters->ParamValues["input_time"] = input_time;
    command->Parameters->ParamValues["line"] = optimal_line;
    command->Parameters->ParamValues["code"] = result_output;
    command->Parameters->ParamValues["result_code"] = result_code;
    command->Parameters->ParamValues["message"] = result_message;
    command->Parameters->ParamValues["pb"] = pb;
    command->Parameters->ParamValues["unsecured_balance"] = unsecured_balance;
    command->Parameters->ParamValues["risk_cut"] = risk_cut;
    command->Parameters->ParamValues["propensity_cut"] = propensity_cut;
    command->Parameters->ParamValues["note"] = note;
    command->Parameters->ParamValues["version"] = version;
    command->Parameters->ParamValues["dollar_bad"] = dollar_bad;
    command->Execute();
  }
  else{
    int result_output;
    sql_stmt = "INSERT INTO Result (IDN, Input_Time, ";
    sql_stmt += "Suggested_Line, Suggestion_Code, Result_Code, Suggestion, ";
    sql_stmt += "RISK_GROUP, PROPENSITY_GROUP, UNSECURED_BALANCE, SPECIAL_NOTE, VERSION) ";
    sql_stmt += "VALUES(:idno, :input_time, :line, :code, :result_code, ";
    sql_stmt += ":message, :risk_cut, :propensity_cut, :unsecured_balance, :note, :version);";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idno"] = idno;
    command->Parameters->ParamValues["input_time"] = input_time;
    command->Parameters->ParamValues["line"] = 0;
 //   command->Parameters->ParamValues["pb"] = 0;
    command->Parameters->ParamValues["risk_cut"] = 0;
    command->Parameters->ParamValues["propensity_cut"] = 0;
    command->Parameters->ParamValues["unsecured_balance"] = unsecured_balance;
    command->Parameters->ParamValues["note"] = note;
    command->Parameters->ParamValues["version"] = version;
    switch (result_code){
      case 0:
      case 1:
        result_output = -1;
        break;
      case 101:
      case 102:
      case 103:
      case 104:
      case 105:
      case 107:
      case 108:
      case 109:
        result_output = 2;
        break;
      case 200:  
      case 201:
      case 202:
      case 203:
      case 204:
        result_output = 3;
        break;
    };
    command->Parameters->ParamValues["code"] = result_output;
    command->Parameters->ParamValues["result_code"] = result_code;
    command->Parameters->ParamValues["message"] = result_message;
    command->Execute();
  };
};

void vam102_message(TADOQuery *query, const AnsiString &idn, const AnsiString &jcic_date, const AnsiString &input_time, AnsiString &note)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT NOTE FROM VAM102 WHERE IDN = :idn AND ";
  sql_stmt += " INQUIRY_DATE = :inq_date AND INPUT_TIME = :input_time";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Parameters->ParamValues["input_time"] = input_time;
  query->Parameters->ParamValues["inq_date"] = jcic_date;
  query->Open();
  while(!query->Eof){
    if(!query->FieldValues["NOTE"].IsNull()){
      note += query->FieldValues["NOTE"];
      note += " ";
    };
    query->Next();
  };
  query->Close();
};

double get_dollar_bad(TADOQuery *query, unsigned int risk_twentile,
                      unsigned int propensity_twentile, unsigned int line_index)
{
  AnsiString sql_stmt;
  double return_value = 0;
  sql_stmt = "SELECT P4 FROM PARAMETERS WHERE P1 = :p1 AND P2 = :p2 AND P3 = :p3";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["P1"] = risk_twentile;
  query->Parameters->ParamValues["P2"] = propensity_twentile;
  query->Parameters->ParamValues["P3"] = line_index;
  query->Open();
  if(query->RecordCount > 0)
   return_value = query->FieldValues["P4"];
  return return_value;
};

void data_insufficiency_model(TADOQuery *query, TADOCommand *command,
                              const AnsiString &krm023, const AnsiString &stm001,
                              const AnsiString &bam086, const AnsiString idn,
                              int now, unsigned int data_flag,
                              double &pb, int &line, double &dollar_bad)
{
  if(data_flag & 01)
    gg(query, command, krm023, stm001, idn, now, pb, line, dollar_bad);
  else
    lg(query, command, bam086, stm001, idn, now, pb, line, dollar_bad);
};

void gg(TADOQuery *query, TADOCommand *command,
        const AnsiString &krm023, const AnsiString &stm001,
        const AnsiString &idn, int now,
        double &pb, int &line, double &dollar_bad)
{
  AnsiString sql_stmt;
/*  try{
    sql_stmt = "DROP TABLE GG_CAL";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };*/
	try{
    sql_stmt = "CREATE TABLE GG_CAL(IDN CHAR(11), FS056_3M_1K float, ";
    sql_stmt += "AB_6M float, MS114_6M float, FS031 float, ";
    sql_stmt += "FS031_R float, FS031_R_TRAN float, FS056_3M_1K_T2 float, ";
    sql_stmt += "SCORE float, CUT int, LINE int, PB float, DOLLAR_BAD float)";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
  if (static_cast<AnsiString>(E.ClassName()) == "EOleException")
    if(query->Connection->Errors->Item[0]->NativeError == 2714)
      query->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
  };

  sql_stmt = "INSERT INTO GG_CAL(IDN, FS056_3M_1K, AB_6M, MS114_6M, FS031) VALUES(:idn, 0, 0, 0, 0)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

/*----FS056----*/
  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, MON, V1) SELECT IDN, MON_SINCE, 1 FROM ";
  sql_stmt += krm023 + " WHERE PAY_CODE IN ('D', 'E', 'F') AND ";
  sql_stmt += "BUCKET_DEF_1K >=1 AND IDN = :idn GROUP BY IDN, MON_SINCE";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, MON, V1) SELECT IDN, 3, SUM(V1) FROM TMP1 ";
  sql_stmt += "WHERE (:NOW - Mon)<= 3 GROUP BY IDN ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["NOW"] = now;
  command->Execute();

  sql_stmt = "UPDATE GG_CAL SET FS056_3M_1k = V1 FROM TMP AS A ";
  sql_stmt += "WHERE A.IDN = GG_CAL.IDN AND Mon=3";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----AB_6M----*/
  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, V1) SELECT IDN, COUNT(*) FROM " + krm023;
  sql_stmt += " WHERE :now - MON_SINCE <= 6 AND PAY_CODE IN ('A', 'B') AND ";
  sql_stmt += "IDN = :idn ";
  sql_stmt += "GROUP BY IDN ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Parameters->ParamValues["NOW"] = now;
  command->Execute();

  sql_stmt = "UPDATE GG_CAL SET AB_6M = (CASE WHEN V1 > 0 THEN 1 ELSE 0 END) ";
  sql_stmt += "FROM TMP1 AS A WHERE A.IDN = GG_CAL.IDN ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS114----*/
  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, MON, V1) ";
  sql_stmt += "SELECT IDN, 6, MAX(Payment_amt/CONVERT(float,Limit)) ";
  sql_stmt += "FROM " + krm023 + " WHERE (:NOW - Mon_Since)<= 6 AND ";
  sql_stmt += "PAY_CODE IN ('C', 'D', 'E', 'F') AND LIMIT <> 0 AND ";
  sql_stmt += "IDN = :idn GROUP BY IDN ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Parameters->ParamValues["NOW"] = now;
  command->Execute();

  sql_stmt = "UPDATE GG_CAL SET MS114_6M = V1 FROM TMP1 AS A ";
  sql_stmt += "WHERE A.IDN = GG_CAL.IDN AND Mon=6";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS031----*/
  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, V1) SELECT IDN, COUNT(*) FROM " + stm001;
  sql_stmt += " WHERE Item_List IS NOT NULL AND Item_List <> '' AND ";
  sql_stmt += ":Now - Query_Mon_Since <= 12 AND IDN = :idn GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Parameters->ParamValues["NOW"] = now;
  command->Execute();

  sql_stmt = "UPDATE GG_CAL SET FS031 = V1 FROM TMP1 AS A WHERE A.IDN = GG_CAL.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//transformation
  sql_stmt = "UPDATE GG_CAL SET FS056_3M_1K_T2 = (CASE WHEN FS056_3M_1K = 0 THEN 0 ";
  sql_stmt += "ELSE 1 END), FS031_R = (CASE WHEN FS031 < 0 THEN NULL ELSE ";
  sql_stmt += "POWER(FS031, 0.5) END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE GG_CAL SET FS031_R_TRAN = (CASE WHEN FS031_R IS NULL THEN 0 ";
  sql_stmt += "ELSE FS031_R END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//scoring
  sql_stmt = "UPDATE GG_CAL SET SCORE = 0.06740 + 0.31288 * FS056_3M_1K_T2 - ";
  sql_stmt += "0.11824 * AB_6M + 0.14593 * MS114_6M + 0.03335 * FS031_R_TRAN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Cut
  sql_stmt = "UPDATE GG_CAL SET CUT = (CASE WHEN SCORE <= 0.00683 THEN 1 ";
  sql_stmt += "WHEN SCORE <= 0.09813 THEN 2 WHEN SCORE <= 0.12844 THEN 3 ";
  sql_stmt += "WHEN SCORE > 0.12844 THEN 4 ELSE 0 END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Assignment
  sql_stmt = "UPDATE GG_CAL SET PB = (CASE WHEN CUT = 1 THEN 0.002 ";
  sql_stmt += "WHEN CUT = 2 THEN 0.0125 WHEN CUT = 3 THEN 0.05 ";
  sql_stmt += "WHEN CUT = 4 THEN 0.075 END), LINE = (CASE WHEN CUT = 1 THEN 50000 ";
  sql_stmt += "WHEN CUT = 2 THEN 50000 WHEN CUT = 3 THEN 0 WHEN CUT = 4 THEN 0 ";
  sql_stmt += "END), DOLLAR_BAD = (CASE WHEN CUT = 1 THEN 0.04 ";
  sql_stmt += "WHEN CUT = 2 THEN 0.08 WHEN CUT = 3 THEN NULL ";
  sql_stmt += "WHEN CUT = 4 THEN NULL END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "SELECT PB, LINE, DOLLAR_BAD FROM GG_CAL WHERE IDN = :idn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Open();
  if(query->FieldByName("PB")->IsNull)
    pb = 0;
  else
    pb = query->FieldValues["PB"];
  if(query->FieldByName("LINE")->IsNull)
    line =  0;
  else
    line = query->FieldValues["LINE"];
  if(query->FieldByName("DOLLAR_BAD")->IsNull)
    dollar_bad = 0;
  else
    dollar_bad = query->FieldValues["DOLLAR_BAD"];
};

void lg(TADOQuery *query, TADOCommand *command,
        const AnsiString &bam086, const AnsiString &stm001,
        const AnsiString &idn, int now,
        double &pb, int &line, double &dollar_bad)
{
 AnsiString sql_stmt;
/* try{
   sql_stmt = "DROP TABLE LG_CAL";
   sql_stmt = sql_stmt.UpperCase();
   command->CommandText = sql_stmt;
   command->Execute();
 }
 catch(Exception &E){
   if (AnsiString(E.ClassName()) == "EOleException")
     if(command->Connection->Errors->Item[0]->NativeError == 3701)
       command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
 };*/

	try{
    sql_stmt = "CREATE TABLE LG_CAL(IDN CHAR(11), FS535_3M float, FS031 float, ";
    sql_stmt += "FS535_3M_TRAN float, FS031_R float, FS031_R_TRAN float, ";
    sql_stmt += "SCORE float, CUT int, LINE int, PB float, DOLLAR_BAD float)";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
  if (static_cast<AnsiString>(E.ClassName()) == "EOleException")
    if(query->Connection->Errors->Item[0]->NativeError == 2714)
      query->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
  };

  sql_stmt = "INSERT INTO LG_CAL(IDN, FS031) VALUES(:idn, 0)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

/*----FS535----*/
  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 1 ";
  sql_stmt += "WHILE @i <= 11 BEGIN INSERT INTO TMP1(IDN, MON, V1) ";
  sql_stmt += "SELECT IDN, @i, COUNT(*) FROM " + bam086;
  sql_stmt += " WHERE account_code != 'K' AND IDN = :idn AND ";
  sql_stmt += "((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N')) AND ";
  sql_stmt += "LEN(LTRIM(RTRIM(PAY_CODE_12))) + :now - 1 - cast(isnull(data_yyy, 0) as int) * 12 - cast(isnull(data_mm, 0) as int) = @i ";
  sql_stmt += "GROUP BY IDN SET @i = @i + 1 END";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Parameters->ParamValues["NOW"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, MON, V1) SELECT IDN, 3, SUM(V1) ";
  sql_stmt += "FROM TMP1 WHERE MON <= 3 GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE LG_CAL SET FS535_3M = V1 FROM TMP AS A WHERE A.IDN = ";
  sql_stmt += "LG_CAL.IDN AND A.MON = 3";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS031----*/
  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, V1) SELECT IDN, COUNT(*) FROM " + stm001;
  sql_stmt += " WHERE Item_List IS NOT NULL AND Item_List <> '' AND ";
  sql_stmt += ":Now - Query_Mon_Since <= 12 AND IDN = :idn GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Parameters->ParamValues["NOW"] = now;
  command->Execute();

  sql_stmt = "UPDATE LG_CAL SET FS031 = V1 FROM TMP1 AS A WHERE A.IDN = LG_CAL.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Transformation
  sql_stmt = "UPDATE LG_CAL SET FS535_3M_TRAN = (CASE WHEN FS535_3M IS NULL THEN 0 ";
  sql_stmt += "ELSE FS535_3M END), FS031_R = (CASE WHEN FS031 < 0 THEN NULL ELSE ";
  sql_stmt += "POWER(FS031, 0.5) END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE LG_CAL SET FS031_R_TRAN = (CASE WHEN FS031_R IS NULL THEN 0 ";
  sql_stmt += "ELSE FS031_R END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Scoring
  sql_stmt = "UPDATE LG_CAL SET SCORE = 0.00961 + 0.15406 * FS535_3M_TRAN + ";
  sql_stmt += "0.07326 * FS031_R_TRAN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Cut
  sql_stmt = "UPDATE LG_CAL SET CUT = (CASE WHEN SCORE <= 0.00961 THEN 1 ";
  sql_stmt += "WHEN SCORE <= 0.11322 THEN 3 WHEN SCORE > 0.11322 THEN 4 END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Assignment
  sql_stmt = "UPDATE LG_CAL SET PB = (CASE WHEN CUT = 1 THEN 0.01 ";
  sql_stmt += "WHEN CUT = 3 THEN NULL WHEN CUT = 4 THEN 0.062 END), ";
  sql_stmt += "LINE = (CASE WHEN CUT = 1 THEN 50000 ";
  sql_stmt += "WHEN CUT = 3 THEN 0 WHEN CUT = 4 THEN 0 END), ";
  sql_stmt += "DOLLAR_BAD = (CASE WHEN CUT = 1 THEN 0.1 ";
  sql_stmt += "WHEN CUT = 3 THEN NULL WHEN CUT = 4 THEN NULL END)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "SELECT PB, LINE, DOLLAR_BAD FROM LG_CAL WHERE IDN = :idn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Open();
  if(query->FieldByName("PB")->IsNull)
    pb = 0;
  else
    pb = query->FieldValues["PB"];
  if(query->FieldByName("LINE")->IsNull)
    line =  0;
  else
    line = query->FieldValues["LINE"];
  if(query->FieldByName("DOLLAR_BAD")->IsNull)
    dollar_bad = 0;
  else
    dollar_bad = query->FieldValues["DOLLAR_BAD"];
};


