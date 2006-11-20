//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#include <time.h>
#include "AM.h"
#include "npv.h"
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
int BOOC_CC_BT_AM(char *case_sn, char *idn, /*char *jcic_data_date,*/ int product_type,
                  char *bank_code, char *ole_db_str, char *error)
{

  AnsiString app_no = static_cast<AnsiString>(case_sn);
  app_no = app_no.Trim();
  AnsiString idno = static_cast<AnsiString>(idn);
  idno = idno.Trim();
  AnsiString jcic_date = "";//static_cast<AnsiString>(jcic_data_date);
  AnsiString error_message = "";
  AnsiString connection_string = static_cast<AnsiString>(ole_db_str);
  int status = 0;

  double cash_card_util_cap = 0.95;
  double credit_card_util_cap = 0.75;
  int cc_balance_cap = 500000;

//general product features
  double regular_apr = 0;
  int gender = 0;
//bt specific product featres
  int bt_counts = 0;
  int total_bt_amount = 0;
  int bt_fee_type = 0;
  double monthly_bt_fee = 0;
  int months_charging_bt_fee = 0;
  int bt_period = 0;
  AnsiString card_type;
  AnsiString app_date="";
  AnsiString bank = static_cast<AnsiString>(bank_code);
  if(bank!= "010")
    throw cc_error(1, app_no, idno, jcic_date, 0, 0, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);

  CoInitialize(NULL);
  TADOConnection *connection = new TADOConnection(NULL);
  TADOQuery *query = new TADOQuery(NULL);
  TADOQuery *query2 = new TADOQuery(NULL);
  TADOCommand *command = new TADOCommand(NULL);
  int command_timeout = 300;
  bool timelock = true;
  bool archive = false;
  bool use_krm034 = true;
  unsigned int unsecured_amount_opt = 0;
  unsigned int unsecured_amount_per = 0;
  unsigned int highest_line = 0;
  bool p1 = false;

  try{

    int now = yrmon_to_mon(jcic_date, timelock, "20161031", archive, use_krm034);

    if(now > 0){
      connect(connection, connection_string);
      initialize_ado_objects(connection, query, query2, command, command_timeout);

      get_prescreen_parameters(query, app_no, idno, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);

      get_product_parameters(query, app_no, idno, product_type,
                             app_date, regular_apr,
                             bt_counts, total_bt_amount, bt_fee_type,
                             monthly_bt_fee, months_charging_bt_fee, bt_period,
                             gender, card_type, cash_card_util_cap,
                             credit_card_util_cap, cc_balance_cap);

      vector<double>interest_scheme;
      if(product_type == 2){ //BT
        rates_to_vector(query, app_no, idno, interest_scheme, regular_apr);
      };

      check_error(regular_apr, bt_counts, total_bt_amount, bt_fee_type,
                  monthly_bt_fee, months_charging_bt_fee, bt_period,
                  gender, product_type, card_type, interest_scheme,
                  app_no, idno, app_date,cash_card_util_cap,
                  credit_card_util_cap, cc_balance_cap);

	    copy_table(command, "KRM021", KRM001, app_no, idno);
 		  copy_table(command, "KRM023", KRM023, app_no, idno);
// 		  copy_table(command, "KRM037", KRM037, app_no, idno);
 		  copy_table(command, "KRM040", KRM040, app_no, idno);
   		copy_table(command, "BAM087", BAM086, app_no, idno);
     	copy_table(command, "STM007", STM001, app_no, idno);
     	copy_table(command, "JAS002", JAS002, app_no, idno);

//    	prepare_KRM023(command, KRM023, now);
//      merge_prepare_KRM023_KRM034(command, KRM023, KRM034, now);
//      merge_prepare_KRM023_KRM037(command, KRM023, KRM037, now);
      merge_prepare_KRM023_KRM040(command, KRM023, KRM040, now);
 	    prepare_KRM001(command, KRM001, now);
	    prepare_BAM086(command, BAM086, now);
		  prepare_STM001(command, STM001, "010", jcic_date);
      prepare_JAS002(command, JAS002, JAS002_T);

   		get_idn_list(command, KRM001, KRM023, BAM086, STM001, JAS002_T);
 		  create_common_working_tables(command);
   		prepare_2xx_infra(command, now);

   		unsigned int data_flag = get_data_availability(query, app_no);

    	unsigned int filter_flag = in_daco41(query, KRM023, KRM001, JAS002_T, BAM086, app_no, data_flag, now,
                                           unsecured_amount_opt, unsecured_amount_per, highest_line, p1,
                                           cash_card_util_cap, credit_card_util_cap, cc_balance_cap);

      if (filter_flag != 0)
        prescreen_out(filter_flag, app_no, idn, app_date, product_type,
                      total_bt_amount,cash_card_util_cap,
                      credit_card_util_cap, cc_balance_cap);
      else{
        double drisk_score;
        double propensity_score;
        int risk_cut;
        int propensity_cut;
        double pb = 0;
        double ev = 0;
        int optimal_line = 0;
        double dollar_bad = 0;
        int result_code = 0;
        AnsiString result_message;

        if(product_type == 1){
      	  drisk_score = daco41(query, command, KRM023, KRM001, JAS002_T, BAM086, STM001, app_no,
                               gender, now, filter_flag, data_flag);
          propensity_score = propensity(command, query, app_no, now, filter_flag, data_flag);
          optimal_line = credit_card_line(query, drisk_score, propensity_score, highest_line, card_type, risk_cut, propensity_cut, pb, ev, dollar_bad, p1);
          if(optimal_line == 0){
            result_message = "建議婉拒：其他原因。";
            result_code = 106;
          }
          else{
            result_message = "建議核准。";
            result_code = 0;
          };
        }
        else{
          double prisk_score = pdaco_1_00(query, command, KRM023, KRM001, JAS002_T, BAM086, STM001, app_no,
                                          gender, now, filter_flag, data_flag);
          bool tf_a2_qualified = check_tf_a2_krm023(query, KRM023, now);
          double tf_a2_risk_score;
          if(tf_a2_qualified)
            tf_a2_risk_score = tf_a2(query, command, KRM023, KRM001, BAM086, STM001,
                                     app_no, gender, now, filter_flag, data_flag);

//         get_score(query, prisk_score, tf_a2_risk_score, tf_a2_qualified, app_no);


          pb = bt_pb(prisk_score, tf_a2_risk_score, tf_a2_qualified);
          bt application(bt_counts, total_bt_amount, bt_fee_type,
                         monthly_bt_fee, months_charging_bt_fee, bt_period,
                         pb, interest_scheme, regular_apr);
          ev = application.npv();
          optimal_line = total_bt_amount;
          if(ev < 0){
            result_message = "建議婉拒：其他原因。";
            result_code = 106;
          }
          else{
            result_message = "建議核准。";
            result_code = 0;
          };
        };
        store_result(command, app_no, idno, app_date, product_type, result_code,
                     result_message, optimal_line,
                     risk_cut, propensity_cut, ev, pb, dollar_bad, true); //normal output
      };
    }
    else{
      throw cc_error(0, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
    };
  }
  catch(cc_error &Err){
    status = -1;
    error_message += Err.ShowMessage();
    if(Err.error_code() > 100)
      store_result(command, Err.case_sn(), Err.idn(), Err.app_date(),
                   Err.product_type(), Err.error_code(), Err.ShowMessage(),
                   Err.line(), 0, 0, 0, 0, 0, false);
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
                const AnsiString &case_sn, const AnsiString &idno)
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
  sql_stmt+= " FROM " + source_table + " WHERE CASE_SN = :case_sn AND idn = :idno ";
  if(source_table == "KRM021"){
    sql_stmt += "GROUP BY IDN, CARD_BRAND, CARD_TYPE, ISSUE, ISSUE_NAME, ";
    sql_stmt += "START_DATE, STOP_DATE, STOP_CODE, AB_CODE, M_S, LIMIT, RISK, ";
    sql_stmt += "CLEAR_DATE, PIDN, CNAME, CASE_SN";
  }
  else if(source_table == "KRM023"){
    sql_stmt += "GROUP BY IDN, YRMON, ISSUE, ISSUE_NAME, KR_CODE, LIMIT, ";
    sql_stmt += "PAYMENT, CASH, PAY_CODE, CASE_SN";
  }
  else if(source_table == "BAM087"){
    sql_stmt += "GROUP BY IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ";
    sql_stmt += "ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT1, ";
    sql_stmt += "CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, PAY_CODE_12, CO_LOAN, ";
    sql_stmt += "CASE_SN, ACT_Y_MARK, CONTRACT_AMT_Y";
  }
  else if(source_table == "STM007"){
    sql_stmt += "GROUP BY IDN, QUERY_DATE, BANK_CODE, BANK_NAME, ITEM_LIST, ";
    sql_stmt += "INQ_PURPOSE1, INQ_PURPOSE, CASE_SN";
  }
  else if(source_table == "JAS002"){
    sql_stmt += "GROUP BY IDN, EVER_DELINQUENT, DELINQUENT_DATE, ";
    sql_stmt += "EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, ";
    sql_stmt += "EVER_STOP_CARD, STOP_CARD_DATE, CASE_SN";
  }
  else if(source_table == "KRM037"){
    sql_stmt += "AND ISSUE != 'TOT' ";
    sql_stmt += "GROUP BY CASE_SN, IDN, BILL_DATE, ISSUE, ";
    sql_stmt += "ISSUE_NAME, CARD_TYPE, PERM_LIMIT, TEMP_LIMIT, CASH_LIMIT, ";
    sql_stmt += "PAYABLE, CASH_LENT, LAST_PAYA, REVOL_BAL, PAY_STAT, PAY_CODE, ";
    sql_stmt += "REVOL_RATE, PRE_OWED, DEBT_CODE, CLOSE_CODE, CLEAR_DATE";
  }
  else if(source_table == "KRM040"){
    sql_stmt += "AND ISSUE != 'TOT' ";
    sql_stmt += "GROUP BY CASE_SN, IDN, BILL_DATE, ISSUE, ISSUE_NAME, ";
    sql_stmt += "BILL_MARK, CARD_TYPE, PERM_LIMIT, TEMP_LIMIT, CASH_LIMIT, ";
    sql_stmt += "PAYABLE, CASH_LENT, LAST_PAYA, REVOL_BAL, PAY_STAT, PAY_CODE, ";
    sql_stmt += "REVOL_RATE, PRE_OWED, DEBT_CODE, CLOSE_CODE, CLEAR_DATE";
  }
  else if(source_table == "KRM034"){
    sql_stmt += "AND ISSUE != 'TOT' ";
    sql_stmt += "GROUP BY CASE_SN, IDN, BILL_DATE, ISSUE, ";
    sql_stmt += "ISSUE_NAME, CARD_TYPE, PERM_LIMIT, CASH_YN, ";
    sql_stmt += "LAST_PAYA, PAY_STAT, PAY_CODE, ";
    sql_stmt += "DEBT_CODE, CLOSE_CODE, CLEAR_DATE";
  };
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Parameters->ParamValues["idno"] = idno;
  command->Execute();
};

void prepare_KRM023(TADOCommand *command, const AnsiString &table, int now)
{
  AnsiString sql_stmt;

  sql_stmt = "UPDATE " + table + " ";
  sql_stmt += "SET ";
  sql_stmt += "CASE_SN = (CASE WHEN CASE_SN = '' THEN NULL ELSE case_sn END), ";
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
  sql_stmt += "A.case_sn = " + table + ".case_sn AND A.ISSUE = " + table + ".ISSUE ";
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
  sql_stmt += "(CASE_SN CHAR(12), IDN CHAR(11), ";
  sql_stmt += "MON_SINCE INT, ISSUE CHAR(3), ISSUE_NAME CHAR(40), LIMIT CHAR(5),";
  sql_stmt += "PAYMENT CHAR(3), PAYMENT_AMT FLOAT, CASH CHAR, PAY_CODE CHAR, SPREAD_PAYMENT FLOAT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO KRM023_TMP (CASE_SN, IDN, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, PAYMENT_AMT, ";
  sql_stmt += "CASH, PAY_CODE, SPREAD_PAYMENT) SELECT CASE_SN, IDN, ";
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
  sql_stmt += "WHEN ISSUE = 'A82' AND CARD_TYPE = 'E' THEN 'AEE' ELSE ISSUE END), ";
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
  sql_stmt += "(CASE_SN CHAR(12), IDN CHAR(11), ";
  sql_stmt += "MON_SINCE INT, ISSUE CHAR(3), ISSUE_NAME CHAR(40), LIMIT CHAR(5),";
  sql_stmt += "PAYMENT_AMT FLOAT, CASH CHAR, PAY_CODE CHAR, SPREAD_PAYMENT FLOAT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO KRM023_TMP1 (CASE_SN, IDN, MON_SINCE, ISSUE, ISSUE_NAME, ";
  sql_stmt += "LIMIT, PAYMENT_AMT, CASH, PAY_CODE, SPREAD_PAYMENT) ";
  sql_stmt += "SELECT CASE_SN, IDN, MON_SINCE, ISSUE, ISSUE_NAME, SUM(CONVERT(INT, LIMIT)), ";
  sql_stmt += "SUM(PAYMENT_AMT), MAX(CASH), ";
  sql_stmt += "MAX(CASE WHEN PAY_CODE = 'X' THEN '0' ELSE PAY_CODE END), ";
  sql_stmt += "SUM(SPREAD_PAYMENT) FROM KRM023_TMP ";
  sql_stmt += "GROUP BY CASE_SN, IDN, MON_SINCE, ISSUE, ISSUE_NAME";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + krm023 + " ";
  sql_stmt += "ADD PAYMENT_AMT FLOAT, BUCKET_DEF_1K INT, BUCKET_EF_1K INT, BUCKET_F_1K INT, MON_SINCE INT, SPREAD_PAYMENT FLOAT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + krm023 + "(CASE_SN, IDN, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT_AMT, ";
  sql_stmt += "CASH, PAY_CODE, SPREAD_PAYMENT) SELECT CASE_SN, IDN, MON_SINCE, ";
  sql_stmt += "ISSUE, ISSUE_NAME, LIMIT, PAYMENT_AMT, CASH, ";
  sql_stmt += "(CASE WHEN PAY_CODE = '0' THEN 'X' ELSE PAY_CODE END), SPREAD_PAYMENT ";
  sql_stmt += "FROM KRM023_TMP1 WHERE MON_SINCE > 1140"; //1140 = 94 * 12 + 12
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET ";
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
  sql_stmt += "A.CASE_SN = " + krm023 + ".CASE_SN AND ";
  sql_stmt += "A.idn = " + krm023 + ".idn AND A.ISSUE = " + krm023 + ".ISSUE ";
  sql_stmt += "AND A.MON_SINCE = (" + krm023 + ".MON_SINCE - 1) ";
  sql_stmt += "WHERE (:inq_mon - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["inq_mon"] = now;
  command->Execute();
};

void merge_prepare_KRM023_KRM040(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm040, int now)
{
  AnsiString sql_stmt;
  sql_stmt = "UPDATE " + krm040 + " SET ";
  sql_stmt += "BILL_DATE = (CASE WHEN BILL_DATE = '' THEN NULL ELSE LTRIM(RTRIM(BILL_DATE)) END), ";
  sql_stmt += "BILL_MARK = (CASE WHEN BILL_MARK = '' THEN NULL ELSE LTRIM(RTRIM(BILL_MARK)) END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "CARD_TYPE = (CASE WHEN CARD_TYPE = '' THEN NULL ELSE CARD_TYPE END), ";
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
//    if(E.Message.SubString(0,16) == "μLak ‥o°￡ ﹐eRAai");
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
//    if(E.Message.SubString(0,16) == "μLak ‥o°￡ ﹐eRAai");
  };

  sql_stmt = "CREATE TABLE KRM023_TMP ";
  sql_stmt += "(CASE_SN CHAR(12), IDN CHAR(11), ";
  sql_stmt += "MON_SINCE INT, ISSUE CHAR(3), ISSUE_NAME CHAR(40), LIMIT CHAR(5),";
  sql_stmt += "PAYMENT FLOAT, PAYMENT_AMT FLOAT, CASH CHAR, PAY_CODE CHAR, SPREAD_PAYMENT FLOAT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO KRM023_TMP (CASE_SN, IDN, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, PAYMENT_AMT, ";
  sql_stmt += "CASH, PAY_CODE, SPREAD_PAYMENT) SELECT CASE_SN, IDN, ";
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
  sql_stmt += "WHEN ISSUE = 'A82' AND CARD_TYPE = 'E' THEN 'AEE' ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME, PERM_LIMIT, LAST_PAYA / 1000.0, REVOL_BAL / 1000.0, ";
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
  sql_stmt += "FROM " + krm040;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE KRM023_TMP SET PAYMENT_AMT = ";
  sql_stmt += "(CASE WHEN PAYMENT < 0 THEN 0 ELSE PAYMENT END)";
  sql_stmt += "WHERE ((PAYMENT_AMT IS NULL) OR (PAYMENT_AMT = 0))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE KRM023_TMP1 ";
  sql_stmt += "(CASE_SN CHAR(12), IDN CHAR(11), ";
  sql_stmt += "MON_SINCE INT, ISSUE CHAR(3), ISSUE_NAME CHAR(40), LIMIT CHAR(5),";
  sql_stmt += "PAYMENT_AMT FLOAT, CASH CHAR, PAY_CODE CHAR, SPREAD_PAYMENT FLOAT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO KRM023_TMP1 (CASE_SN, IDN, MON_SINCE, ISSUE, ISSUE_NAME, ";
  sql_stmt += "LIMIT, PAYMENT_AMT, CASH, PAY_CODE, SPREAD_PAYMENT) ";
  sql_stmt += "SELECT CASE_SN, IDN, MON_SINCE, ISSUE, ISSUE_NAME, SUM(CONVERT(INT, LIMIT)), ";
  sql_stmt += "SUM(PAYMENT_AMT), MAX(CASH), ";
  sql_stmt += "MAX(CASE WHEN PAY_CODE = 'X' THEN '0' ELSE PAY_CODE END), ";
  sql_stmt += "SUM(SPREAD_PAYMENT) FROM KRM023_TMP ";
  sql_stmt += "GROUP BY CASE_SN, IDN, MON_SINCE, ISSUE, ISSUE_NAME";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + krm023 + " ";
  sql_stmt += "ADD PAYMENT_AMT FLOAT, BUCKET_DEF_1K INT, BUCKET_EF_1K INT, BUCKET_F_1K INT, MON_SINCE INT, SPREAD_PAYMENT FLOAT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + krm023 + "(CASE_SN, IDN, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT_AMT, ";
  sql_stmt += "CASH, PAY_CODE, SPREAD_PAYMENT) SELECT CASE_SN, IDN, MON_SINCE, ";
  sql_stmt += "ISSUE, ISSUE_NAME, LIMIT, PAYMENT_AMT, CASH, ";
  sql_stmt += "(CASE WHEN PAY_CODE = '0' THEN 'X' ELSE PAY_CODE END), SPREAD_PAYMENT ";
  sql_stmt += "FROM KRM023_TMP1 WHERE MON_SINCE > 1140"; //1140 = 94 * 12 + 12
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET ";
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
  sql_stmt += "A.CASE_SN = " + krm023 + ".CASE_SN AND ";
  sql_stmt += "A.idn = " + krm023 + ".idn AND A.ISSUE = " + krm023 + ".ISSUE ";
  sql_stmt += "AND A.MON_SINCE = (" + krm023 + ".MON_SINCE - 1) ";
  sql_stmt += "WHERE (:inq_mon - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["inq_mon"] = now;
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
  sql_stmt += "(CASE_SN CHAR(12), IDN CHAR(11), ";
  sql_stmt += "MON_SINCE INT, ISSUE CHAR(3), ISSUE_NAME CHAR(40), LIMIT CHAR(5),";
  sql_stmt += "PAYMENT CHAR(3), PAYMENT_AMT FLOAT, CASH CHAR, PAY_CODE CHAR)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO KRM023_TMP (CASE_SN, IDN, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, ";
  sql_stmt += "CASH, PAY_CODE) SELECT CASE_SN, IDN, ";
  sql_stmt += "(CASE WHEN LEFT(BILL_DATE,1) = '*' THEN NULL ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 4 AND LEFT(BILL_DATE,1) BETWEEN '1' AND '9' ";
  sql_stmt += "THEN CONVERT(INT, LEFT(BILL_DATE, 2)) * 12 + CONVERT(INT, RIGHT(BILL_DATE, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 5 AND LEFT(BILL_DATE,1) = '0' THEN ";
  sql_stmt += "CONVERT(INT, LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, RIGHT(BILL_DATE, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 7 AND CONVERT(INT, RIGHT(BILL_DATE,2)) > 15 ";
  sql_stmt += "THEN CONVERT(INT, LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 7 THEN CONVERT(INT,LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) - 1 ";
  sql_stmt += "ELSE NULL END), ISSUE, ISSUE_NAME, PERM_LIMIT, LAST_PAYA, CASH_YN, ";
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

  sql_stmt = "INSERT INTO " + krm023 + "(CASE_SN, IDN, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, CASH, ";
  sql_stmt += "PAY_CODE) SELECT CASE_SN, IDN, MON_SINCE, ";
  sql_stmt += "ISSUE, ISSUE_NAME, LIMIT, PAYMENT, CASH, PAY_CODE ";
  sql_stmt += "FROM KRM023_TMP WHERE MON_SINCE > 1140"; //1140 = 94 * 12 + 12
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET ";
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
  sql_stmt += "A.CASE_SN = " + krm023 + ".CASE_SN AND ";
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
  sql_stmt += "case_sn = (CASE WHEN case_sn = '' THEN NULL ELSE case_sn END), ";
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
  sql_stmt += "case_sn = (CASE WHEN case_sn = '' THEN NULL ELSE case_sn END), ";
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

  sql_stmt = " UPDATE " + table + " SET CONTRACT_AMT = CONTRACT_AMT1  where CONTRACT_AMT1 > CONTRACT_AMT";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET CONTRACT_AMT = CONTRACT_AMT_Y where ACCOUNT_CODE= 'Y' and CONTRACT_AMT_Y != 0";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  build_bam_bucket(command, table, now);
};

void prepare_STM001(TADOCommand *command, const AnsiString &table,
                    const AnsiString &bank_atm_code,
                    const AnsiString &jcic_inquiry_date)
{
  AnsiString sql_stmt;
  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "case_sn = (CASE WHEN case_sn = '' THEN NULL ELSE case_sn END), ";
  sql_stmt += "Query_Date = (CASE WHEN Query_Date = '' THEN NULL ELSE Query_Date END), ";
  sql_stmt += "Bank_Code = (CASE WHEN Bank_Code = '' THEN NULL ELSE Bank_Code END), ";
  sql_stmt += "Bank_Name = (CASE WHEN Bank_Name = '' THEN NULL ELSE Bank_Name END), ";
  sql_stmt += "Item_List = (CASE WHEN Item_List = '' THEN NULL ELSE Item_List END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + table + " ADD QUERY_MON_SINCE INT";
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
  AnsiString unit_code = bank_atm_code + "CD01";


  if(jcic_inquiry_date.IsEmpty())
    sql_stmt += "DATEDIFF(day, CONVERT(DATETIME, CONVERT(CHAR(4),CONVERT(INT,LEFT(LTRIM(QUERY_DATE),3)) + 1911) + RIGHT(RTRIM(QUERY_DATE),4)),GETDATE()) <= 30 AND "; //for production
  else
    sql_stmt += "DATEDIFF(day, CONVERT(DATETIME, CONVERT(CHAR(4),CONVERT(INT,LEFT(LTRIM(QUERY_DATE),3)) + 1911) + RIGHT(RTRIM(QUERY_DATE),4)), CONVERT(DATETIME, '" + jcic_inquiry_date + "')) <= 30 AND ";
  sql_stmt += "BANK_CODE = '" + unit_code + "'";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
//  command->Parameters->ParamValues["bank_atm_code"] = unit_code;
  command->Execute();
};

void prepare_JAS002(TADOCommand *command, const AnsiString &ori_table, const AnsiString &dest_table)
{
	AnsiString sql_stmt;

  sql_stmt = "UPDATE " + ori_table + " SET ";
  sql_stmt += "case_sn = (CASE WHEN case_sn = '' THEN NULL ELSE case_sn END), ";
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
  sql_stmt += " (case_sn Char(12), TYPE CHAR(1), E_DATE Char(7), ";
  sql_stmt += "E_MON_SINCE INT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT case_sn, 'D', DELINQUENT_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_DELINQUENT = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT case_sn, 'B', BAD_CHECK_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_BAD_CHECK = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT case_sn, 'R', REJECT_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_REJECT = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT case_sn, 'S', STOP_CARD_DATE, NULL FROM " + ori_table;
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
  	sql_stmt = "CREATE TABLE IDN_LIST_TMP (case_sn CHAR(12), HIT INT);";//KRM001 = 2, KRM023 = 1, BAM086 = 4, STM001 = 8, DAM103 or JAS002 = 16, DMAM203 = 32
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
  	sql_stmt = "CREATE TABLE IDN_LIST (case_sn CHAR(12), HIT INT);";
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

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT case_sn, 1 FROM " + KRM023 + " ";
  command->CommandText = sql_stmt;
  sql_stmt = sql_stmt.UpperCase();
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT case_sn, 2 FROM " + KRM001 + " ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT case_sn, 4 FROM " + BAM086 + " ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT case_sn, 8 FROM " + STM001 + " ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT case_sn, 16 FROM " + JAS002_T + " ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST SELECT case_sn, SUM(HIT) FROM IDN_LIST_TMP GROUP BY case_sn;";
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
  sql_stmt = "CREATE TABLE TMP(case_sn CHAR(12), MON INT, V1 FLOAT, ";
  sql_stmt += "V2 FLOAT, V3 FLOAT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE TMP1(case_sn CHAR(12), MON INT, V1 FLOAT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE TMP2(case_sn CHAR(12), ISSUER CHAR(3));";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE OPEN_CARD (case_sn CHAR(12), ISSUE CHAR(3), MON_SINCE INT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE OPEN_LINE (case_sn CHAR(12), ISSUE CHAR(3), MON_SINCE INT, ";
  sql_stmt += "Cards INT, BUCKET INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE LATEST_STMT_MON (case_sn CHAR(12), ISSUE CHAR(3), ";
  sql_stmt += "MON_SINCE INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE LATEST_LINE (case_sn CHAR(12), ISSUE CHAR(3), ";
  sql_stmt += "MON_SINCE INT, MOB INT)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "CREATE TABLE OPEN_ISSUER (case_sn CHAR(12), ISSUE CHAR(3), MON INT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
};

void prepare_2xx_infra(TADOCommand *command, int now)
{
  AnsiString sql_stmt;
  sql_stmt = "DECLARE @i INT SET @i = 0 WHILE @i<=13 BEGIN INSERT INTO ";
  sql_stmt += "OPEN_CARD SELECT case_sn, ";
  sql_stmt += "(CASE WHEN CARD_BRAND = 'A' AND ISSUE = 'A82' AND (:now0 - @i) <= 1140 THEN 'AEA' ELSE ISSUE END), ";
  sql_stmt += "(:now - @i) FROM " + KRM001;
  sql_stmt += " WHERE (END_MON_SINCE > (:now1 - @i)) AND (START_MON_SINCE <= ";
  sql_stmt += "(:now2 - @i)) AND ISSUE != '021' SET @i = @i + 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now0"] = now;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["now1"] = now;
  command->Parameters->ParamValues["now2"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO OPEN_LINE SELECT case_sn, ISSUE, MON_SINCE, Count(*), NULL FROM ";
  sql_stmt += "OPEN_CARD GROUP BY case_sn, ISSUE, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 0 WHILE @i<=13 BEGIN INSERT INTO ";
  sql_stmt += "OPEN_LINE SELECT case_sn, (CASE WHEN (:now3 - @i) > 1140 THEN ISSUE ";
  sql_stmt += "WHEN CARD_BRAND = 'M' THEN 'CTM' ";
  sql_stmt += "WHEN CARD_BRAND = 'V' THEN 'CTV' ";
  sql_stmt +="WHEN CARD_BRAND = 'D' THEN 'CTD' END), (:now - @i),1, NULL ";
  sql_stmt += "FROM " + KRM001 + " WHERE (END_MON_SINCE > (:now1 - @i)) AND ";
  sql_stmt += "(START_MON_SINCE <= (:now2 - @i)) AND ISSUE = '021' ";
  sql_stmt +="SET @i = @i + 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["now1"] = now;
  command->Parameters->ParamValues["now2"] = now;
  command->Parameters->ParamValues["now3"] = now;
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
  sql_stmt += "OPEN_LINE ON A.case_sn = OPEN_LINE.case_sn AND ";
  sql_stmt +="A.ISSUE = OPEN_LINE.ISSUE AND A.MON_SINCE = (OPEN_LINE.MON_SINCE - 1) ";
  sql_stmt += "WHERE (:now - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO LATEST_STMT_MON SELECT case_sn, ISSUE, MAX(MON_SINCE) ";
  sql_stmt += "FROM OPEN_LINE WHERE MON_SINCE <= :now GROUP BY case_sn, ISSUE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO LATEST_LINE SELECT A.case_sn, A.ISSUE, A.MON_SINCE, A.BUCKET ";
  sql_stmt += "FROM OPEN_LINE AS A INNER JOIN LATEST_STMT_MON AS B ";
  sql_stmt += "ON A.case_sn = B.case_sn AND A.ISSUE = B.ISSUE AND A.MON_SINCE = B.MON_SINCE;";
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
  sql_stmt = "CREATE TABLE PM (case_sn CHAR(12), HIT INT, EXCLUSION INT, ";
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

void load_and_initialize_propensity(TADOCommand *command, const AnsiString &case_sn, unsigned int exclusion, unsigned int data_flag)
{
  AnsiString sql_stmt;

//load data
  sql_stmt = "INSERT INTO PM(case_sn, HIT, EXCLUSION) VALUES (:case_sn, :hit, :exclusion);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Parameters->ParamValues["hit"] = data_flag;
  command->Parameters->ParamValues["exclusion"] = exclusion;
  command->Execute();
//KRM001 and KRM023 variables
	sql_stmt = "UPDATE PM SET ";
	sql_stmt += "FS212_3M=0, FS212_6M=0 ";
	sql_stmt += "FROM " + KRM001 + " AS A, " + KRM023 + " AS B ";
	sql_stmt += "WHERE A.case_sn = B.case_sn AND A.case_sn = PM.case_sn AND ";
	sql_stmt += "((a.issue = b.issue) OR ";
	sql_stmt += " ((a.issue = '021' and a.card_brand = 'V') and (b.issue = 'CTV')) OR ";
	sql_stmt += " ((a.issue = '021' and a.card_brand = 'M') and (b.issue = 'CTM')) OR ";
  sql_stmt += " ((a.issue = '021' and a.card_brand = 'D') and (b.issue = 'CTD')) OR ";
  sql_stmt += " ((a.issue = 'A82' and a.card_brand = 'A') and (b.issue = 'AEA'))) AND ";
	sql_stmt += "A.case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
 	command->CommandText = sql_stmt;
	command->Parameters->ParamValues["case_sn"] = case_sn;
	command->Execute();

//KRM023 variables
  sql_stmt = "UPDATE PM SET FS105_9M=0, FS106_9M=0, FS106_12M=0, MS011_3M=0,";
  sql_stmt += "MS011_6M=0, MS023_9M=0, MS104_3M=0, MS104_6M=0, MS104_9M=0, ";
  sql_stmt += "MS105_9M=0, MS106_9M=0, MS108_12M=0, MS118_3M=0, MS118_6M=0, ";
  sql_stmt += "MS118_9M=0, MS119_3M=0 WHERE (HIT & 0x1) = 0x1 AND case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();
//BAM009 variables
  sql_stmt = "UPDATE PM SET FS042=0, MS064=0 WHERE (HIT & 0x4) = 0x4 AND case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();
//STM001 variables
  sql_stmt = "UPDATE PM SET FS029=0, FS030=0, FS031=0 WHERE (HIT & 0x8) = 0x8 AND case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();
};

double propensity(TADOCommand *command, TADOQuery *query, const AnsiString &case_sn, int now, unsigned int exclusion, unsigned int data_flag)
{
  AnsiString sql_stmt;

  create_propensity_tables(command);
  load_and_initialize_propensity(command, case_sn, exclusion, data_flag);
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

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, NULL, COUNT(*) ";
  sql_stmt += "FROM " + STM001 + " WHERE ((ITEM_LIST LIKE '%K%') ";
  sql_stmt += "AND (:now - QUERY_MON_SINCE <= 12)) ";
  sql_stmt += "AND case_sn = :case_sn GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS029 = V1 FROM TMP1 AS A WHERE A.case_sn = PM.case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS031----*/
/*---Init temp table----*/
  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, NULL, COUNT(*) ";
  sql_stmt += "FROM " + STM001 + " WHERE ITEM_LIST IS NOT NULL AND ";
  sql_stmt += "ITEM_LIST <> '' AND (:now - QUERY_MON_SINCE <= 12) ";
  sql_stmt += "AND case_sn = :case_sn GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS031 = V1 FROM TMP1 AS A WHERE A.case_sn = PM.case_sn;";
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

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, NULL, COUNT(*) FROM " + BAM086;
  sql_stmt += " WHERE ACCOUNT_CODE in ('H','S') AND (ACCOUNT_CODE2 ='' or ";
  sql_stmt += "ACCOUNT_CODE2 IS NULL OR ACCOUNT_CODE2 = 'N') AND case_sn = :case_sn GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS042 = V1 FROM TMP1 AS A WHERE A.case_sn = PM.case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS064----*/
  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, NULL, SUM(CONVERT(FLOAT,LOAN_AMT) + CONVERT(FLOAT,PASS_DUE_AMT)) ";
  sql_stmt += "FROM " + BAM086 + " WHERE (ACCOUNT_CODE2 = '' or ACCOUNT_CODE2 IS NULL OR ACCOUNT_CODE2 = 'N') ";
  sql_stmt += "AND ACCOUNT_CODE IN ('W','C', 'D', 'E') AND case_sn = :case_sn GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS064 = V1 FROM TMP1 AS A WHERE A.case_sn = PM.case_sn;";
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

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, MON_SINCE, SUM(CASE ";
  sql_stmt += "WHEN PAY_CODE IN ('A', 'B') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE case_sn = :case_sn GROUP BY case_sn, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=6 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT case_sn, @i, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= @i GROUP BY case_sn SET @i = @i + 3 END";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS011_3M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=3;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS011_6M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=6;";
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

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, MON_SINCE, SUM(CONVERT(FLOAT,LIMIT))";
  sql_stmt += "FROM " + KRM023 + " WHERE case_sn = :case_sn GROUP BY case_sn, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT case_sn, 9, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 9 AND (V1 > 0) GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS023_9M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=9;";
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

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, MON_SINCE, SUM(CASE WHEN CASH = 'Y' ";
  sql_stmt += "THEN 1 ELSE 0 END) FROM " + KRM023 + " WHERE case_sn =:case_sn GROUP BY case_sn, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT case_sn, 9, AVG(V1), NULL, NULL FROM TMP1 WHERE ";
  sql_stmt += "(:now - MON)<= 9 GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS105_9M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=9;";
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

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('C', 'D', 'E', 'F') THEN 1 ELSE 0 END) FROM " + KRM023;
  sql_stmt += " WHERE case_sn = :case_sn GROUP BY case_sn, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 9 WHILE @i<=12 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT case_sn, @i, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= @i AND case_sn = :case_sn GROUP BY case_sn SET @i = @i + 3 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS106_9M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=9;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS106_12M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=12;";
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
  sql_stmt += "SELECT case_sn, @i, SUM(PAYMENT_AMT), NULL, NULL FROM " + KRM023 + " WHERE ";
  sql_stmt += "(:now - MON_SINCE)<= @i AND PAY_CODE IN ('A', 'B', 'C') AND ";
  sql_stmt += "LIMIT <> 0 AND case_sn = :case_sn GROUP BY case_sn SET @i = @i + 3 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS104_3M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=3;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS104_6M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=6;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS104_9M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=9;";
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

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('C', 'D') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE case_sn = :case_sn GROUP BY case_sn, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT case_sn, 9, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 9 GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS105_9M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=9";
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

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE case_sn = :case_sn GROUP BY case_sn, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT case_sn, 9, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 9 GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS106_9M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=9;";
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

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('A', 'B', 'C') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE case_sn = :case_sn GROUP BY case_sn, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT case_sn, 12, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 12 GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS108_12M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND MON=12;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS118----*/
  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=9 BEGIN INSERT INTO TMP1 ";
  sql_stmt += "SELECT case_sn, @i, MIN(PAYMENT_AMT/CONVERT(FLOAT,LIMIT)) ";
  sql_stmt += "FROM " + KRM023 + " WHERE (:now - MON_SINCE)<= @i AND PAY_CODE IN ";
  sql_stmt +="('C', 'D', 'E', 'F') AND LIMIT <> 0 AND case_sn = :case_sn GROUP BY case_sn ";
  sql_stmt +="SET @i = @i + 3 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS118_3M = V1 FROM TMP1 AS A WHERE A.case_sn = PM.case_sn AND MON=3;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS118_6M = V1 FROM TMP1 AS A WHERE A.case_sn = PM.case_sn AND MON=6;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS118_9M = V1 FROM TMP1 AS A WHERE A.case_sn = PM.case_sn AND MON=9;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS119----*/
/*---Init temp table----*/
  sql_stmt = "DELETE FROM TMP1;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT case_sn, 3, MIN(PAYMENT_AMT/CONVERT(FLOAT,LIMIT)) ";
  sql_stmt += "FROM " + KRM023 + " WHERE (:now - MON_SINCE)<= 3 AND ";
  sql_stmt +="PAY_CODE IN ('A', 'B', 'C', 'D') AND LIMIT <> 0 AND case_sn = :case_sn GROUP BY case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS119_3M = V1 FROM TMP1 AS A WHERE A.case_sn = PM.case_sn AND MON=3;";
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
  sql_stmt += "SELECT case_sn, @i, COUNT(*) FROM " + KRM023 + " AS A WHERE ";
  sql_stmt += "PAY_CODE IN ('C', 'D', 'E', 'F') AND PAYMENT_AMT > 1 AND ";
  sql_stmt += "(:now - MON_SINCE) = @i AND ISSUE IN ";
  sql_stmt += "(SELECT ISSUE FROM LATEST_LINE WHERE (MON_SINCE - MOB + 1) >= ";
  sql_stmt += "(:now1 - 6) AND case_sn = A.case_sn) AND case_sn = :case_sn ";
  sql_stmt += "GROUP BY case_sn SET @i = @i + 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["now1"] = now;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=6 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT case_sn, @i, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (MON)<= @i GROUP BY case_sn SET @i = @i + 3 END";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS212_3M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND A.MON = 3;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS212_6M = V1 FROM TMP AS A WHERE A.case_sn = PM.case_sn AND A.MON = 6;";
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
  sql_stmt += "WHERE case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

//generating squared, square_rooted, and zero indicator variables
  sql_stmt = "UPDATE PM SET MT104_42_Q = POWER(MT104_42, 2), ";
  sql_stmt += "MS119_3M_R = POWER((CASE WHEN MS119_3M<0 THEN NULL ELSE MS119_3M END), 0.5), ";
  sql_stmt += "MS108_12M_R = POWER(CONVERT(FLOAT,(CASE WHEN MS108_12M<0 THEN NULL ELSE MS108_12M END)), 0.5), ";
  sql_stmt += "MS064_R = POWER((CASE WHEN MS064<0 THEN NULL ELSE MS064 END), 0.5), ";
  sql_stmt += "INT084_9_R = POWER((CASE WHEN INT084_9<0 THEN NULL ELSE INT084_9 END), 0.5), ";
  sql_stmt += "INT037_9_Q = POWER(INT037_9, 2),";
  sql_stmt += "FT212_43_R = POWER((CASE WHEN FT212_43<0 THEN NULL ELSE FT212_43 END), 0.5), ";
  sql_stmt += "FS042_Q = POWER(FS042, 2) WHERE case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
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
  sql_stmt += "WHERE case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

//score calculation
  sql_stmt = "UPDATE PM SET SCORE = 6983.56585+FS030_T*1573.47038+";
  sql_stmt +="FS042_Q_T*1185.2782+FS106_12M_T*2228.0058+FT212_43_R_T*4435.87825+";
  sql_stmt += "INT037_9_Q_T1*21844+INT062_9_T1*-19.98018+INT084_9_R_T*8933.21573+";
  sql_stmt +="MS064_R_T*489.60121+MS108_12M_R_T*504.85556+MS119_3M_R_T*11820+";
  sql_stmt +="MT011_43_Z*4135.46056+MT104_42_Q_T*0.0372+MT118_42_T*5906.03891 ";
  sql_stmt += "WHERE case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

//decile cut
  sql_stmt = "UPDATE PM SET DECILE = (CASE WHEN SCORE <=-1766.96 THEN 1 ";
  sql_stmt += "WHEN SCORE <=1215.25 THEN 2 WHEN SCORE <=3306.38 THEN 3 ";
  sql_stmt += "WHEN SCORE <=5422.75 THEN 4 WHEN SCORE <=7761.67 THEN 5 ";
  sql_stmt += "WHEN SCORE <=10909.35 THEN 6 WHEN SCORE <=14857.95 THEN 7 ";
  sql_stmt += "WHEN SCORE <=21364.09 THEN 8 WHEN SCORE <=29957.94 THEN 9 ";
  sql_stmt += "WHEN SCORE >29957.94 THEN 10 ELSE 0 END) WHERE EXCLUSION = 0 ";
  sql_stmt += "AND case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["case_sn"] = case_sn;
  command->Execute();

  sql_stmt = "SELECT SCORE FROM PM WHERE case_sn = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["case_sn"] = case_sn;
  query->Open();
  score = query->FieldValues["SCORE"];
  query->Close();
  };
  return score;
};

int propensity_cut(TADOQuery *query, const AnsiString &case_sn)
{
  AnsiString sql_stmt = "SELECT DECILE, EXCLUSION FROM PM WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
  int decile;
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["case_sn"]= case_sn;
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

unsigned int get_data_availability(TADOQuery *query, const AnsiString &case_sn)
{
	AnsiString sql_stmt;
	sql_stmt = "SELECT HIT FROM IDN_LIST WHERE case_sn = :case_sn;";
	sql_stmt = sql_stmt.UpperCase();
	query->Close();
	query->SQL->Clear();
	query->SQL->Add(sql_stmt);
	query->Parameters->ParamValues["case_sn"] = case_sn;
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
    sql_stmt += "(case_sn CHAR(12),  ";
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

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_01, 1 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_02, 2 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_03, 3 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_04, 4 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_05, 5 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_06, 6 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_07, 7 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_08, 8 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_09, 9 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_10, 10 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_11, 11 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "insert into y_BAM086_tmp(case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle) ";
  sql_stmt += "select ";
  sql_stmt += "case_sn, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, (CASE WHEN CONTRACT_AMT > CONTRACT_AMT1 THEN CONTRACT_AMT ELSE CONTRACT_AMT1 END), LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_12, 12 - :now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1 ";
  sql_stmt += "from " + bam086;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["now"] = now;
  command->Execute();
};

void get_prescreen_parameters(TADOQuery *query,
                              const AnsiString &case_sn, const AnsiString &idno,
                              double &cash_card_util_cap, double &credit_card_util_cap, int &cc_balance_cap)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT CASH_CARD_UTIL_CAP, CREDIT_CARD_UTIL_CAP, CC_BALANCE_CAP ";
  sql_stmt += "FROM APP_INFO WHERE CASE_SN = :case_sn AND IDN = :idn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["case_sn"] = case_sn;
  query->Parameters->ParamValues["idn"] = idno;
  query->Open();
  if(!query->FieldValues["CASH_CARD_UTIL_CAP"].IsNull())
    cash_card_util_cap = query->FieldValues["CASH_CARD_UTIL_CAP"];
  if(!query->FieldValues["CREDIT_CARD_UTIL_CAP"].IsNull())
    credit_card_util_cap = query->FieldValues["CREDIT_CARD_UTIL_CAP"];
  if(!query->FieldValues["CC_BALANCE_CAP"].IsNull())
    cc_balance_cap = query->FieldValues["CC_BALANCE_CAP"];
  query->Close();
};
/*
JCIC data availability flag
0x00 : No JCIC data
0x01 : KRM023 / KRM037 hit
0x02 : KRM001 hit
0x04 : BAM086 hit
0x08 : STM007 hit (represented with STM001)
0x10 : JAS002 hit

0x0000 : Pass prescreen
0x0001 : No KRM023 / KRM037
0x0002 : No KRM001
0x0004 : KRM023 max bucket > 3 (ef, 1K)
0x0008 : Earliest KRM023 month less than 6 months ago
               first KRM001 card open less than 7 months
0x0010 : BAM086 Pass_Due_Amt > 0
0x0020 : JAS002 major derag
0x0040 : BAM086 cash card delinquent in latest month
0x0080 : BAM086 cash card utilization >= 95%
0x0100 : BAM086 max bucket > 3
0x0200 : KRM023 data only on inquiry month or later
0x0400 : Unsecured revolving balance(*) >= NT$500K
0x1000 : Unsecured balance(**) >= NT$1M
*/

unsigned int in_daco41(TADOQuery *query,
                const AnsiString &krm023, const AnsiString &krm001,
                const AnsiString &jas002, const AnsiString &bam085,
                const AnsiString &case_sn,
                unsigned int flag, int now,
                unsigned int &unsecured_amount_opt, unsigned int &unsecured_amount_per,
                unsigned int &max_line, bool &p1,
                double cash_card_util_cap, double credit_card_util_cap, int cc_balance_cap)
{
  AnsiString sql_stmt;
  int ever_delinquent_line = 0;
  int exclusion = 0;
  int count = 0;
  double credit_line = 0;
  double credit_card_rev_balance_per = 0;
  double credit_card_rev_balance_opt = 0;
  double unsecured_loan_balance = 0;
  double cash_card_balance = 0;
  if ((flag & 0x1)==0)
    exclusion |= 0x1;
  else{
    sql_stmt = "SELECT MAX(Bucket_ef_1k) As ef FROM " + krm023 + " ";
    sql_stmt +="WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["case_sn"] = case_sn;
    query->Open();
    int max_bucket = query->FieldValues["ef"];
    query->Close();
    if (max_bucket > 3)
      exclusion |= 0x4;

    sql_stmt = "SELECT MIN(MON_SINCE) AS MON_SINCE FROM " + krm023 + " ";
    sql_stmt += "WHERE CASE_SN = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["case_sn"] = case_sn;
    query->Open();
    int min_stmt = query->FieldValues["MON_SINCE"];
    query->Close();
    if(now - min_stmt < 6)
      exclusion |= 0x8;

    sql_stmt = "SELECT SUM(PAYMENT_AMT) AS BAL FROM " + krm023;
    sql_stmt += " WHERE :now - MON_SINCE = 1 AND ";
    sql_stmt += "PAY_CODE IN ('C', 'D', 'E', 'F') AND ";
    sql_stmt += "CASE_SN = :case_sn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["case_sn"] = case_sn;
    query->Parameters->ParamValues["now"] = now;
    query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      credit_card_rev_balance_per = query->FieldValues["BAL"];
      credit_card_rev_balance_per *= 1000;
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
    sql_stmt += "(CASE_SN CHAR(12), ISSUE CHAR(3), MON INT)";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->ExecSQL();

    sql_stmt = "INSERT INTO KRM023_RANGE_TMP(CASE_SN, ISSUE, MON) ";
    sql_stmt += "SELECT CASE_SN, ISSUE, MAX(MON_SINCE) FROM " + krm023 + " ";
    sql_stmt += "WHERE :now - MON_SINCE <= 3 ";
    sql_stmt += "GROUP BY CASE_SN, ISSUE";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["now"] = now;
    query->ExecSQL();

    sql_stmt = "SELECT SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END) AS BAL, ";
    sql_stmt += "SUM(CONVERT(FLOAT, LIMIT)) AS LINE ";
    sql_stmt += "FROM " + krm023 + " AS A ";
    sql_stmt += "INNER JOIN KRM023_RANGE_TMP AS B ON A.CASE_SN = B.CASE_SN AND ";
    sql_stmt += "A.ISSUE = B.ISSUE AND A.MON_SINCE = B.MON ";
    sql_stmt += "WHERE A.CASE_SN = :case_sn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["case_sn"] = case_sn;
    query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      credit_card_rev_balance_opt = query->FieldValues["BAL"];
      credit_card_rev_balance_opt *= 1000;
    };
    if(!query->FieldValues["LINE"].IsNull()){
      credit_line = query->FieldValues["LINE"];
      credit_line *= 1000;
    };
    if((credit_line > 0) && (static_cast<double>(credit_card_rev_balance_opt / credit_line) > credit_card_util_cap))
      exclusion |= 0x200;

    sql_stmt = "SELECT COUNT(DISTINCT ISSUE) AS LINES FROM " + krm023 + " ";
    sql_stmt += "WHERE BUCKET_EF_1K > 0 AND CASE_SN = :case_sn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["case_sn"] = case_sn;
    query->Open();
    ever_delinquent_line = query->FieldValues["LINES"];
//Push propensity decile to 1 for dormat users
    sql_stmt = "SELECT COUNT(*) AS CNT FROM " + krm023 + " ";
    sql_stmt += "WHERE CASE_SN = :case_sn GROUP BY CASE_SN HAVING ";
    sql_stmt += "COUNT(*) = SUM(CASE WHEN PAY_CODE = 'X' THEN 1 ELSE 0 END)";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["case_sn"] = case_sn;
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
      sql_stmt = "CREATE TABLE IND_TMP (case_sn CHAR(12), Mon int);";
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

    sql_stmt = "INSERT INTO IND_TMP SELECT case_sn, MIN(Start_Mon_Since) FROM ";
    sql_stmt += krm001 + " WHERE case_sn = :case_sn GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["case_sn"] = case_sn;
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
      exclusion |= 0x8;
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
    sql_stmt += "WHERE STOP_DATE ='9991231' AND M_S = 'Y' AND CASE_SN = :case_sn";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["case_sn"] = case_sn;
  	query->Open();
    if(!query->FieldValues["MAX_LINE"].IsNull())
      max_line = query->FieldValues["MAX_LINE"] * 1000;
    if(max_line == 0){
      sql_stmt = "SELECT MAX(CONVERT(INT, LIMIT)) AS MAX_LINE FROM " + krm001 + " ";
      sql_stmt += "WHERE STOP_DATE <> '9991231' AND M_S = 'Y' AND CASE_SN = :case_sn";
      sql_stmt = sql_stmt.UpperCase();
    	query->Close();
    	query->SQL->Clear();
    	query->SQL->Add(sql_stmt);
    	query->Parameters->ParamValues["case_sn"] = case_sn;
    	query->Open();
      if(!query->FieldValues["MAX_LINE"].IsNull())
        max_line = query->FieldValues["MAX_LINE"] * 1000;
    };
    if(ever_delinquent_line > 0){
      sql_stmt = "SELECT COUNT(*) AS LINES FROM OPEN_LINE WHERE CASE_SN = :case_sn ";
      sql_stmt += "AND MON_SINCE = :now";
      sql_stmt = sql_stmt.UpperCase();
    	query->Close();
    	query->SQL->Clear();
    	query->SQL->Add(sql_stmt);
    	query->Parameters->ParamValues["case_sn"] = case_sn;
    	query->Parameters->ParamValues["now"] = now;
    	query->Open();
        if(query->FieldValues["LINES"] <= 2)
          exclusion |= 0x800;
    };
  };
	if(flag & 0x4){
/*----FS044----*/
		sql_stmt ="SELECT case_sn, SUM(CASE WHEN PASS_DUE_AMT > 0 THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt +="FROM " + bam085 + " ";
		sql_stmt +="WHERE case_sn = :case_sn GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["case_sn"] = case_sn;
  	query->Open();
  	count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 0)
      exclusion |= 0x10;
    sql_stmt = "SELECT case_sn, SUM(CASE WHEN ACCOUNT_CODE = 'Y' AND ";
    sql_stmt+="                           ISNULL(LEFT(PAY_CODE_12,1), '0') NOT IN ('0', 'X') ";
    sql_stmt+="                      THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt+="FROM " + bam085 + " ";
    sql_stmt+="WHERE case_sn = :case_sn GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["case_sn"] = case_sn;
  	query->Open();
  	count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 0)
      exclusion |= 0x40;

    sql_stmt = "SELECT case_sn, SUM(CASE WHEN ACCOUNT_CODE = 'Y' AND ";
    sql_stmt+= " (CONTRACT_AMT * " + static_cast<AnsiString>(cash_card_util_cap) + ") <= (ISNULL(LOAN_AMT,0) + ISNULL(PASS_DUE_AMT,0)) AND ";
    sql_stmt+= " ISNULL(CONTRACT_AMT,0) > 0 ";
    sql_stmt+= " THEN 1 ELSE 0 END) AS CNT ";
    sql_stmt+= "FROM " + bam085 + " ";
    sql_stmt+= "WHERE case_sn = :case_sn GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["case_sn"] = case_sn;
  	query->Open();
  	count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 0)
      exclusion |= 0x80;

    sql_stmt = "SELECT case_sn, MAX(Bucket) as cnt FROM y_BAM086_tmp ";
    sql_stmt += "WHERE case_sn = :case_sn GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["case_sn"] = case_sn;
  	query->Open();
    if(query->FieldValues["CNT"].IsNull())
      count = 0;
    else
  	  count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 1)
      exclusion |= 0x100;

    sql_stmt = "SELECT SUM(ISNULL(LOAN_AMT, 0) + ISNULL(PASS_DUE_AMT, 0)) ";
    sql_stmt += "AS BAL FROM " + bam085 + " ";
    sql_stmt += "WHERE CASE_SN = :case_sn AND ACCOUNT_CODE = 'Y';";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["case_sn"] = case_sn;
  	query->Open();
    if(!query->FieldValues["BAL"].IsNull())
      cash_card_balance = query->FieldValues["BAL"];
    cash_card_balance *= 1000;

    sql_stmt = "SELECT SUM(ISNULL(LOAN_AMT, 0) + ISNULL(PASS_DUE_AMT, 0)) ";
    sql_stmt += "AS BAL FROM " + bam085 + " ";
    sql_stmt += "WHERE CASE_SN = :case_sn AND ACCOUNT_CODE NOT IN ('K', 'Y') AND ";
    sql_stmt += "(ACCOUNT_CODE2 = 'N' OR (ACCOUNT_CODE2 IS NULL));";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["case_sn"] = case_sn;
  	query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      unsecured_loan_balance = query->FieldValues["BAL"];
      unsecured_loan_balance *= 1000;
    };
    query->Close();
	};

  if((credit_card_rev_balance_opt + cash_card_balance) >= cc_balance_cap)
    exclusion |= 0x400;

  unsecured_amount_opt = (credit_card_rev_balance_opt + cash_card_balance + unsecured_loan_balance);
  unsecured_amount_per = (credit_card_rev_balance_per + cash_card_balance + unsecured_loan_balance);

  if(flag & 0x10){
//		sql_stmt ="SELECT case_sn, SUM(CASE WHEN :now - E_MON_SINCE <= 36 THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt ="SELECT case_sn, COUNT(*) AS CNT ";
		sql_stmt +="FROM " + jas002 + " ";
		sql_stmt +="WHERE case_sn = :case_sn GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
//	  query->Parameters->ParamValues["now"] = now;
	  query->Parameters->ParamValues["case_sn"] = case_sn;
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
  return exclusion;
};

double daco41(TADOQuery *query, TADOCommand *command,
              const AnsiString &krm023, const AnsiString &krm001,
              const AnsiString &jas002, const AnsiString &bam085,
              const AnsiString &stm001, const AnsiString &case_sn,
              int gender, int now, unsigned int exclusion, unsigned int data_flag)
{
	AnsiString sql_stmt;
	double risk_score  = 1000;

	try{
  	sql_stmt = "CREATE TABLE DACO_V4_1_CAL (case_sn CHAR(12), HIT INT, EXCLUSION INT, ";
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
  sql_stmt = "INSERT INTO DACO_V4_1_CAL(case_sn, HIT, EXCLUSION, GENDER) ";
	sql_stmt += "VALUES (:case_sn, :hit, :exclusion, :gender);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["case_sn"] = case_sn;
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
  	sql_stmt += " WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
		command->Parameters->ParamValues["case_sn"] = case_sn;
		command->Execute();
  	if (data_flag & 0x03){ // KRM001 + KRM023
			sql_stmt = "UPDATE DACO_V4_1_CAL SET ";
			sql_stmt += "ms203_3m_1k = 0, ms203_6m_1k = 0, ms203_9m_1k = 0 ";
			sql_stmt += "FROM " + krm001 + " AS A, " + krm023 + " AS B ";
			sql_stmt += "WHERE A.case_sn = B.case_sn AND A.case_sn = DACO_V4_1_CAL.case_sn AND ";
			sql_stmt += "((a.issue = b.issue) OR ";
			sql_stmt += " ((a.issue = '021' and a.card_brand = 'V') and (b.issue = 'CTV')) OR ";
			sql_stmt += " ((a.issue = '021' and a.card_brand = 'M') and (b.issue = 'CTM')) OR ";
      sql_stmt += " ((a.issue = '021' and a.card_brand = 'D') and (b.issue = 'CTD')) OR ";
      sql_stmt += " ((a.issue = 'A82' and a.card_brand = 'A') and (b.issue = 'AEA'))) AND ";
	  	sql_stmt += "A.case_sn = :case_sn;";
  	  sql_stmt = sql_stmt.UpperCase();
  		command->CommandText = sql_stmt;
			command->Parameters->ParamValues["case_sn"] = case_sn;
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

		sql_stmt = "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, mon_since, 1 from " + krm023;
    sql_stmt += " where bucket_ef_1k >= 1 ";
    sql_stmt += "group by case_sn, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "insert into tmp (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 6, sum(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= 6 and ";
    sql_stmt += "(:now1 - mon) >= 0 group by case_sn;";
		sql_stmt = sql_stmt.UpperCase();
 		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
    command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt = " update DACO_V4_1_CAL set fs002_6m_1k = v1 ";
    sql_stmt += "from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn AND ";
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

		sql_stmt = "insert into tmp1 (case_sn, v1) ";
		sql_stmt += "select case_sn, count(*) from " + bam085;
		sql_stmt += " where (account_code2=''or account_code2 is null) AND ";
		sql_stmt += "account_code <> 'K' group by case_sn;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "update DACO_V4_1_CAL set fs040 = v1 ";
		sql_stmt += "from tmp1 as a ";
		sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn;";
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

		sql_stmt = "insert into tmp2(case_sn, issuer) ";
		sql_stmt += "select case_sn, bank_code2 from " + bam085;
		sql_stmt += " where purpose_code = '1' and ";
		sql_stmt += "(account_code2 is not null or account_code2 <> '') ";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "insert into tmp1(case_sn, v1) ";
		sql_stmt += "select case_sn, count(*) from " + bam085;
		sql_stmt += " where not exists (select * from tmp2 ";
		sql_stmt += "where " + bam085 + ".case_sn = tmp2.case_sn and ";
		sql_stmt += bam085 + ".bank_code2 = tmp2.issuer) and ";
		sql_stmt += "purpose_code = '1' and ";
		sql_stmt += "((account_code2 is null) or (account_code2 = '')) and ";
		sql_stmt += "account_code != 'K' group by case_sn;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

		sql_stmt = "update DACO_V4_1_CAL set fs068 = v1 ";
		sql_stmt += "from tmp1 as a ";
		sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn;";
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

    sql_stmt = "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 9, count(distinct issue) from " + krm023;
    sql_stmt += " where (:now - mon_since) <= 9 and ";
    sql_stmt += "(:now1 - mon_since) >= 0 and ";
    sql_stmt += "cash = 'Y' group by case_sn;";
		sql_stmt = sql_stmt.UpperCase();
 		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
    command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set fs016_9m = v1 ";
    sql_stmt += "from tmp1 as a where a.case_sn = DACO_V4_1_CAL.case_sn and ";
    sql_stmt += "mon = 9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

//fs101
    sql_stmt = "delete from tmp1;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

    sql_stmt = "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 9, count(distinct issue) from " + krm023;
    sql_stmt += " where (:now - mon_since) <= 9 and ";
    sql_stmt += "(:now1 - mon_since) >= 0 group by case_sn;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
    command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set fs101_9m = v1 ";
    sql_stmt += "from tmp1 as a where a.case_sn = DACO_V4_1_CAL.case_sn and ";
    sql_stmt += " mon = 9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

//fs031
    sql_stmt = "delete from tmp ";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();

    sql_stmt = "insert into tmp (case_sn, v1) ";
    sql_stmt += "select case_sn, count(*) from " + stm001;
    sql_stmt += " where item_list is not null and ";
    sql_stmt += "item_list <> '' and (:now - query_mon_since) <= 3 ";
    sql_stmt += "group by case_sn";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set fs031 = v1 from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn";
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

    sql_stmt = "insert into tmp (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 3, count(*) from " + krm023;
    sql_stmt += " where bucket_f_1k >= 2 and ";
    sql_stmt += " (:now - mon_since) <= 3 and  (:now1 - mon_since) >= 0 ";
    sql_stmt += "group by case_sn";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set fs005_3m_1k = v1 from tmp as a ";
    sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn and mon = 3";
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

    sql_stmt = "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, mon_since, sum(payment_amt) from " + krm023;
    sql_stmt += " where pay_code in ('D', 'E', 'F') and bucket_def_1k >= 1 ";
    sql_stmt += "group by case_sn, mon_since";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp(case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 6, max(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= 6 and (:now1 - mon) >= 0 ";
    sql_stmt += "group by case_sn";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
    command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set ms056_6m_1k = v1 from tmp as a ";
    sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn and mon=6";
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

    sql_stmt = "insert into tmp1(case_sn, mon) ";
    sql_stmt += "select case_sn, max(mon_since) from " + krm023 + " group by case_sn";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp (case_sn, v1) ";
    sql_stmt += "select a.case_sn, max(bucket_ef_1k) from " + krm023 + " as a ";
    sql_stmt += "inner join tmp1 as b on a.case_sn = b.case_sn and ";
//    sql_stmt += " where (:now - mon_since) <= 1 and (:now1 - mon_since) >= 0 ";
    sql_stmt += "a.mon_since = b.mon group by a.case_sn";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
//		command->Parameters->ParamValues["now"] = now;
//    command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set app_last_month_bucket = v1 from tmp a ";
    sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn";
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

    sql_stmt += "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 13, count(distinct issue) from " + krm001;
    sql_stmt += " as a where :now - end_mon_since > 0 and ";
    sql_stmt += "stop_code = '3' and issue not in ('021', '081', '974') and ";
    sql_stmt += "issue not in (select distinct issue ";
    sql_stmt += "from open_issuer as b ";
    sql_stmt += "where a.case_sn = b.case_sn and a.end_mon_since = b.mon) ";
    sql_stmt += "group by case_sn;";
		sql_stmt = sql_stmt.UpperCase();
 		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp (case_sn, mon, v1, v2, v3) ";
    sql_stmt += "select distinct case_sn, 13, 0, 0, 0 from " + krm001;
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update tmp set v1 = a.v1 from tmp1 as a ";
    sql_stmt += "where a.case_sn = tmp.case_sn and a.mon = tmp.mon;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "delete from tmp1;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 13, count(*) from " + krm001;
    sql_stmt += " where stop_code = '3' and ";
    sql_stmt += "issue in ('021', '081', '974') and ";
    sql_stmt += ":now - end_mon_since > 0 group by case_sn;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update tmp set v2 = a.v1 ";
    sql_stmt += "from tmp1 as a ";
    sql_stmt += "where a.case_sn = tmp.case_sn and a.mon = tmp.mon;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set fs008_12mplus = v1 + v2 ";
    sql_stmt += "from tmp as a ";
    sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn and mon = 13;";
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

    sql_stmt = "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, mon_since, sum(payment_amt) from " + krm023;
    sql_stmt += " group by case_sn, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "declare @i int set @i = 3 ";
    sql_stmt += "while @i <= 6 begin ";
    sql_stmt += "insert into tmp (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, @i, avg(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= @i and ";
    sql_stmt += "(:now1 - mon) > 0 group by case_sn ";
    sql_stmt += " set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set ms009_3m = v1 from tmp as a ";
    sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn and mon=3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL ";
    sql_stmt += "set ms009_6m = v1 from tmp as a ";
    sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn and mon=6;";
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

    sql_stmt = "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, mon_since, sum(case when pay_code in ('A', 'B') then payment_amt else 0 end) ";
    sql_stmt += "from " + krm023 + " group by case_sn, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "declare @i int set @i = 3 ";
    sql_stmt += "while @i <= 12 begin ";
    sql_stmt += "insert into tmp (case_sn, mon, v1) select case_sn, @i, avg(v1) ";
    sql_stmt += "from tmp1 where (:now - mon) <= @i and (:now1 - mon) > 0 ";
    sql_stmt += " group by case_sn set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms011_3m = v1 ";
    sql_stmt += "from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn ";
    sql_stmt += "and mon = 3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms011_6m = v1 ";
    sql_stmt += "from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn ";
    sql_stmt += "and mon = 6;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms011_9m = v1 ";
    sql_stmt += "from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn ";
    sql_stmt += "and mon = 9;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms011_12m = v1 ";
    sql_stmt += "from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn ";
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

    sql_stmt = "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, mon_since, sum(case when pay_code in ('C', 'D', 'E', 'F') then payment_amt ";
    sql_stmt += "else 0 end) from " + krm023;
    sql_stmt += " group by case_sn, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "declare @i int set @i = 3 ";
    sql_stmt += "while @i <= 6 begin ";
    sql_stmt += "insert into tmp (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, @i, avg(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= @i and (:now1 - mon) >= 0 ";
    sql_stmt += "group by case_sn set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms106_3m = v1 ";
    sql_stmt += "from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn and mon=3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "update DACO_V4_1_CAL set ms106_6m = v1 ";
    sql_stmt += "from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn and mon=6;";
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

    sql_stmt = "insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, mon_since, sum(convert(decimal(16, 8), (case when limit='' then NULL else limit end))) ";
    sql_stmt += "from " + krm023;
    sql_stmt += " group by case_sn, mon_since;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "declare @i int set @i = 3 ";
    sql_stmt += "while @i <= 6 begin ";
    sql_stmt += "insert into tmp (case_sn, mon, v1) select case_sn, @i, max(v1) from tmp1 ";
    sql_stmt += "where (:now - mon) <= @i and (:now1 - mon) >= 0 ";
    sql_stmt += "group by case_sn set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt += "update DACO_V4_1_CAL ";
    sql_stmt += "set ms110_3m = (case when v1=0 then null else ms106_3m / v1 end) ";
    sql_stmt += "from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn ";
    sql_stmt += "and mon=3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt += "update DACO_V4_1_CAL ";
    sql_stmt += "set ms110_6m = (case when v1=0 then null else ms106_6m / v1 end) ";
    sql_stmt += "from tmp as a where a.case_sn = DACO_V4_1_CAL.case_sn ";
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
    sql_stmt += "while @i <= 9 begin insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, @i, sum(payment_amt) from " + krm023 + " as a ";
    sql_stmt += "where issue in (select issue from latest_line ";
    sql_stmt += "where mob <= 12 and case_sn = a.case_sn) and bucket_def_1k <> 0 and ";
    sql_stmt += ":now - mon_since <= @i and :now1 - mon_since > 0 ";
    sql_stmt += "group by case_sn set @i = @i + 3 end;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["now"] = now;
		command->Parameters->ParamValues["now1"] = now;
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt ="  update DACO_V4_1_CAL ";
    sql_stmt += "set ms203_3m_1k = v1 from tmp1 as a ";
    sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn and a.mon = 3;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt ="  update DACO_V4_1_CAL ";
    sql_stmt += "set ms203_6m_1k = v1 from tmp1 as a ";
    sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn and a.mon = 6;";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt ="  update DACO_V4_1_CAL ";
    sql_stmt += "set ms203_9m_1k = v1 from tmp1 as a ";
    sql_stmt += "where a.case_sn = DACO_V4_1_CAL.case_sn and a.mon = 9;";
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
    sql_stmt += "fs031_tran = (case when fs031 is null then 0 when fs031 > 8 then 8 else fs031 end), ";
    sql_stmt += "app_last_month_bucket_t1 = (case when app_last_month_bucket is null then 0 else app_last_month_bucket end) ";
    sql_stmt += "WHERE case_sn = :case_sn";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["case_sn"] = case_sn;
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
    sql_stmt += "WHERE case_sn = :case_sn";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["case_sn"] = case_sn;
 		command->Execute();

//Third round transformation
  	sql_stmt = "update DACO_V4_1_CAL set ";
    sql_stmt += "mt203_42_1k_r_tran = (case when mt203_42_1k_r is null then 3.3 else mt203_42_1k_r end), ";
//    sql_stmt += "when mt203_42_1k_r > 6 then 6 else mt203_42_1k_r end),";
  	sql_stmt += "mt009_43_q_tran2=(case when mt009_43_q > 75000 then 1 else 0 end) ";
  	sql_stmt += "WHERE case_sn = :case_sn";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["case_sn"] = case_sn;
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
    sql_stmt += "WHERE case_sn = :case_sn";
		sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Parameters->ParamValues["case_sn"] = case_sn;
 		command->Execute();

	  sql_stmt = "SELECT SCORE FROM DACO_V4_1_CAL WHERE case_sn = :case_sn;";
	  sql_stmt = sql_stmt.UpperCase();
	  query->Close();
	  query->SQL->Clear();
	  query->SQL->Add(sql_stmt);
	  query->Parameters->ParamValues["case_sn"] = case_sn;
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
  	sql_stmt = "DROP TABLE " + KRM040 + ";";
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
  	sql_stmt = "DROP TABLE TF_A2_CAL;";
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
  	sql_stmt = "DROP TABLE PDACO_V1_00_CAL;";
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
      sql_stmt = "DROP TABLE KRM023_TMP1;";
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
};

double pdaco_1_00(TADOQuery *query, TADOCommand *command,
                  const AnsiString &krm023, const AnsiString &krm001,
                  const AnsiString &jas002, const AnsiString &bam085,
                  const AnsiString &stm001, const AnsiString &case_sn,
                  int gender, int now, unsigned int exclusion, unsigned int data_flag)
{
  AnsiString sql_stmt;
  bool with_gender = true;
	double risk_score  = 1000;

	try{
		sql_stmt = "CREATE TABLE PDACO_V1_00_CAL (case_sn CHAR(12), ";
		sql_stmt += "FS014_9M int, FS101_9M int, ";
    sql_stmt += "FS016_3M int, FS101_3M int, FS005_3M_1k int, ";
		sql_stmt += "FS059_3M_1K int, FS059_6M_1K int, FS059_9M_1K int, SEX float, ";
		sql_stmt += "FS212_3M_1K float, FS212_6M_1K float, FS031 int, MS117_6M float, ";
		sql_stmt += "FS102_3M int, FS102_6M int, FS102_9M int, FS051 int, ";
		sql_stmt += "APP_LAST_MONTH_BUCKET int, FS205_3M_1K int, ";
		sql_stmt += "INT015_3M float, INT028_9M float, ";
    sql_stmt += "FT059_1K_42 int, FT212_43_1K float, FT102_42 int, ";
		sql_stmt += "FT059_1K_43 int, ";
		sql_stmt += "FT059_1K_42_Q int, FT212_43_1K_Q float, FT102_42_R float, ";
		sql_stmt += "FS205_3M_1K_Q int, FT059_1K_43_R float, ";
		sql_stmt += "INT015_3M_T float, INT028_9M_T float, ";
    sql_stmt += "FT059_1k_42_Q_T int, FT212_43_1K_Q_T float, ";
		sql_stmt += "FT102_42_R_T float, FS031_T int, SEX_T float, APP_LAST_MONTH_BUCKET_T int, ";
		sql_stmt += "FS205_3M_1K_Q_T int, FT059_1K_43_R_T float, ";
		sql_stmt += "FS051_T int, ";
		sql_stmt += "EXCLUSION int, HIT int, SCORE decimal(13,12), CUT int, PB1 float, PB2 float);";
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

  sql_stmt = "INSERT INTO PDACO_V1_00_CAL(case_sn, HIT, EXCLUSION, SEX) ";
	sql_stmt += "VALUES (:case_sn, :hit, :exclusion, :gender);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["case_sn"] = case_sn;
	command->Parameters->ParamValues["hit"] = data_flag;
	command->Parameters->ParamValues["gender"] = gender;
	command->Parameters->ParamValues["exclusion"] = exclusion;
	command->Execute();

	bool run_model = true;
  if(((exclusion & 0x1) == 0x1) ||  ((exclusion & 0x2) == 0x2))
    run_model = false;

//	int exclusion = 0; in_pdaco_1_00(query, krm023, krm001, jas002, bam085, data_flag, now); since this function is used as the module exclusion and is called before
	if(run_model){
//Init variables
		if(data_flag & 0x1){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS016_3M = 0, FS101_3M = 0, FS005_3M_1k = 0, FS014_9M = 0, ";
			sql_stmt += "FS059_3M_1K = 0, FS059_6M_1K = 0, FS059_9M_1K = 0, ";
			sql_stmt += "FS102_3M = 0, FS102_6M = 0, FS102_9M = 0, MS117_6M = 0, FS101_9M = 0 ";
      sql_stmt += "WHERE case_sn = :case_sn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["case_sn"] = case_sn;
			command->Execute();
		};
		if((data_flag & 0x1) && (data_flag & 0x2)){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS212_3M_1K = 0, FS212_6M_1K = 0, ";
			sql_stmt += "FS205_3M_1K = 0 ";
      sql_stmt += "FROM " + krm001 + " AS A, " + krm023 + " As B ";
      sql_stmt += "WHERE PDACO_V1_00_CAL.case_sn = :case_sn AND ";
      sql_stmt += "PDACO_V1_00_CAL.case_sn = A.case_sn AND ";
      sql_stmt += "A.case_sn = B.case_sn AND ";
      sql_stmt += "(CASE WHEN A.ISSUE = '021' AND CARD_BRAND = 'V' THEN 'CTV' ";
      sql_stmt += "WHEN A.ISSUE = '021' AND CARD_BRAND = 'M' THEN 'CTM' ";
      sql_stmt += "WHEN A.ISSUE = '021' AND CARD_BRAND = 'D' THEN 'CTD' ";
      sql_stmt += "WHEN A.ISSUE = 'A82' AND CARD_BRAND = 'A' THEN 'AEA' ";
      sql_stmt += "ELSE A.ISSUE END) = B.ISSUE;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["case_sn"] = case_sn;
			command->Execute();
		};
		if(data_flag & 0x4){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS051 = 0 ";
      sql_stmt += "WHERE case_sn = :case_sn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["case_sn"] = case_sn;
			command->Execute();
		};
		if(data_flag & 0x8){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS031 = 0 ";
      sql_stmt += "WHERE case_sn = :case_sn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["case_sn"] = case_sn;
			command->Execute();
		};
//		prepare_2xx_variables(command, query, krm023, krm001, now);


//create temp tables
		try{
  		sql_stmt = "DROP TABLE TMP;";
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
  		sql_stmt = "DROP TABLE TMP1;";
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
		sql_stmt = "CREATE TABLE TMP ";
	  sql_stmt += "(case_sn CHAR(12), Mon INT, V1 FLOAT, V2 FLOAT, V3 FLOAT);";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

	  sql_stmt = "CREATE TABLE TMP1 ";
	  sql_stmt += "(case_sn CHAR(12), Mon INT, V1 FLOAT);";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

/*----FS014----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS014---*/
    sql_stmt ="INSERT INTO TMP ";
    sql_stmt +="SELECT case_sn, 9, COUNT(DISTINCT ISSUE), NULL, NULL ";
		sql_stmt +=("FROM " + krm023 + " ");
    sql_stmt +="WHERE (:now - Mon_Since)<= 9 AND ";
//    sql_stmt +="(:now1 - Mon_Since) > 0 AND ";
    sql_stmt +="Pay_Code IN ('A', 'B') GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
		command->Execute();

    sql_stmt ="UPDATE PDACO_V1_00_CAL ";
    sql_stmt +="SET FS014_9M = V1 FROM TMP AS A ";
    sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS016----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS016---*/
		sql_stmt ="INSERT INTO TMP ";
		sql_stmt +="SELECT case_sn, 3, COUNT(DISTINCT ISSUE), NULL, NULL ";
		sql_stmt +=("FROM " + krm023 + " ");
		sql_stmt +="WHERE (:now - Mon_Since)<= 3 AND ";
//		sql_stmt +="(:now1 - Mon_Since) > 0 AND ";
		sql_stmt +="CASH = 'Y' GROUP BY case_sn ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS016_3M = V1 FROM TMP AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS101----*/
/*---Init temp table----*/
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS101---*/
		sql_stmt ="DECLARE @i INT ";
		sql_stmt +="SET @i = 3 ";
		sql_stmt +="WHILE @i<=9 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP1 ";
		sql_stmt +="SELECT case_sn, @i, COUNT(DISTINCT ISSUE) ";
		sql_stmt +=("FROM " + krm023 + " ");
		sql_stmt +="WHERE (:now - Mon_Since)<= @i ";
//		sql_stmt +="AND (:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY case_sn SET @i = @i + 6 END;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS101_3M = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS101_9M = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS059----*/
/*---Init temp table----*/
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS059---*/
		sql_stmt ="DECLARE @i INT ";
		sql_stmt +="SET @i = 3 ";
		sql_stmt +="WHILE @i<=9 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP1 ";
		sql_stmt +="SELECT case_sn, @i, COUNT(DISTINCT ISSUE) ";
		sql_stmt +="FROM " + krm023 + " ";
		sql_stmt +="WHERE Bucket_ef_1K >=1 AND ";
		sql_stmt +="(:now - Mon_Since)<= @i ";
//		sql_stmt +="AND (:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY case_sn SET @i = @i + 3 END;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS059_3M_1k = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS059_6M_1k = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=6;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS059_9M_1k = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS212_1K----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS212_1K---*/
		sql_stmt ="DECLARE @I INT ";
		sql_stmt +="SET @I = 1 ";
		sql_stmt +="WHILE @I <= 12 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP ";
		sql_stmt +="SELECT case_sn, @I, COUNT(*), NULL, NULL ";
		sql_stmt +="FROM " + krm023 + "  AS A ";
		sql_stmt +="WHERE Issue IN ";
		sql_stmt +="(SELECT Issue FROM Latest_Line ";
		sql_stmt +="WHERE  (MON_SINCE - MOB + 1) >=  (:now - 6) AND case_sn = A.case_sn) AND ";
		sql_stmt +="Pay_Code in ('C', 'D', 'E','F') AND ";
		sql_stmt +="Payment_Amt > 1 AND ";
		sql_stmt +="(:now1 - Mon_Since) = @I  ";
		sql_stmt +="GROUP BY case_sn ";
		sql_stmt +="SET @I = @I + 1 ";
		sql_stmt +="END ";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

		sql_stmt ="DECLARE @I INT ";
		sql_stmt +="SET @I = 3 ";
		sql_stmt +="WHILE @I <= 6 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP1 ";
		sql_stmt +="SELECT case_sn, @I, AVG(V1) ";
		sql_stmt +="FROM TMP ";
		sql_stmt +="WHERE Mon <= @I ";
		sql_stmt +="GROUP BY case_sn ";
		sql_stmt +="SET @I = @I + 3 ";
		sql_stmt +="END ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET  ";
		sql_stmt +="FS212_3M_1k = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND A.Mon = 3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET  ";
		sql_stmt +="FS212_6M_1k = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND A.Mon = 6;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS102----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

		try{
  		sql_stmt = "DROP TABLE FS_ISSUE;";
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

		sql_stmt = "CREATE TABLE FS_ISSUE(case_sn Char(14), MON Int, ISSUE Char(3));";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

/*---Start making FS102---*/
		sql_stmt ="INSERT INTO FS_Issue ";
		sql_stmt +="SELECT case_sn, Mon_Since, Issue  ";
		sql_stmt +="FROM " + krm023 + "  ";
		sql_stmt +="WHERE Limit > 100 ";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="DECLARE @i INT ";
		sql_stmt +="SET @i = 3 ";
		sql_stmt +="WHILE @i<=9 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP ";
		sql_stmt +="SELECT DISTINCT case_sn, @i, 0, NULL, NULL ";
		sql_stmt +=("FROM " + krm023 + "  ");
		sql_stmt +="WHERE (:now - Mon_Since)<= @i ";
//		sql_stmt +="AND (:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY case_sn SET @i = @i + 3 END;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

		sql_stmt ="DECLARE @i INT ";
		sql_stmt +="SET @i = 3 ";
		sql_stmt +="WHILE @i<=9 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP1  ";
		sql_stmt +="SELECT case_sn, @i, COUNT(DISTINCT ISSUE) ";
		sql_stmt +="FROM FS_Issue ";
		sql_stmt +="WHERE (:now - Mon)<= @i ";
//		sql_stmt +="AND (:now1 - Mon) > 0 ";
		sql_stmt +="GROUP BY case_sn SET @i = @i + 3 END;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

		sql_stmt ="UPDATE TMP ";
		sql_stmt +="SET V1 = A.V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.case_sn = TMP.case_sn AND TMP.MON = A.MON;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS102_3M = V1 FROM TMP AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS102_6M = V1 FROM TMP AS A ";
    sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=6;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET ";
		sql_stmt +="FS102_9M = V1 ";
		sql_stmt +="FROM TMP AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    if(data_flag & 0x8){
/*----FS031----*/
/*---Init temp table----*/
      sql_stmt ="DELETE FROM TMP1;";
      sql_stmt = sql_stmt.UpperCase();
	    command->CommandText = sql_stmt;
	    command->Execute();
/*---Start making FS031---*/
		  sql_stmt ="INSERT INTO TMP1 ";
		  sql_stmt +="SELECT case_sn, NULL, COUNT(*) ";
  		sql_stmt +="FROM " + stm001 + " ";
	  	sql_stmt +="WHERE Item_List IS NOT NULL AND  ";
		  sql_stmt +="Item_List <> '' AND  ";
  		sql_stmt +="(:now - QUERY_MON_SINCE) <= 12 ";
	  	sql_stmt +="GROUP BY case_sn ";
      sql_stmt = sql_stmt.UpperCase();
		  command->CommandText = sql_stmt;
  	  command->Parameters->ParamValues["now"] = now;
 	  	command->Execute();

  		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
	  	sql_stmt +="SET FS031 = V1 ";
		  sql_stmt +="FROM TMP1 AS A ";
  		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn ";
      sql_stmt = sql_stmt.UpperCase();
	  	command->CommandText = sql_stmt;
 		  command->Execute();
    };
/*----App_Last_Month_Bucket----*/
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = "INSERT INTO TMP1 SELECT case_sn, NULL, MAX(BUCKET_EF_1K) ";
    sql_stmt += "FROM " + krm023 + " ";
    sql_stmt += "WHERE :now - Mon_Since <= 1 GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
 		command->Execute();

		sql_stmt = "UPDATE PDACO_V1_00_CAL SET APP_LAST_MONTH_BUCKET = A.V1 ";
		sql_stmt += "FROM TMP1 AS A WHERE PDACO_V1_00_CAL.case_sn = A.case_sn;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

/*----FS205_1K----*/
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt +="INSERT INTO TMP1 ";
		sql_stmt +="SELECT case_sn, 3, COUNT(*) ";
		sql_stmt +="FROM " + krm023 + "  AS A ";
		sql_stmt +="WHERE Issue IN ";
		sql_stmt +="(SELECT Issue FROM Latest_Line ";
		sql_stmt +="WHERE MOB <= 12 AND case_sn = A.case_sn) AND ";
		sql_stmt +="Pay_Code in ('C', 'D', 'E','F') AND ";
		sql_stmt +="Payment_Amt > 1 AND ";
		sql_stmt +="(:now - Mon_Since) <= 3 ";
//		sql_stmt +="(:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET  ";
		sql_stmt +="FS205_3M_1k = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND A.Mon = 3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    if(data_flag & 0x4){
/*----FS051----*/
/*---Init temp table----*/
  		sql_stmt ="DELETE FROM TMP1;";
      sql_stmt = sql_stmt.UpperCase();
	    command->CommandText = sql_stmt;
	    command->Execute();
/*---Start making FS051---*/
  		sql_stmt ="INSERT INTO TMP1 ";
	  	sql_stmt +="SELECT case_sn, NULL, COUNT(*) ";
		  sql_stmt +="FROM " + bam085 + " ";
  		sql_stmt +="WHERE purpose_code = '4' ";
	  	sql_stmt +="GROUP BY case_sn;";
      sql_stmt = sql_stmt.UpperCase();
	    command->CommandText = sql_stmt;
  	  command->Execute();

  		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
	  	sql_stmt +="SET FS051 = V1 ";
		  sql_stmt +="FROM TMP1 AS A ";
  		sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn;";
      sql_stmt = sql_stmt.UpperCase();
	    command->CommandText = sql_stmt;
	    command->Execute();
    };
/*----MS117----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making MS117---*/
    sql_stmt ="INSERT INTO TMP1 ";
    sql_stmt+="SELECT case_sn, 6, MIN(Payment_amt/CONVERT(decimal(13,5),Limit)) ";
    sql_stmt+=("FROM " + krm023 + " ");
    sql_stmt+="WHERE (:now - Mon_Since)<= 6 AND ";
    sql_stmt+="      (:now1 - Mon_Since) > 0 AND ";
    sql_stmt+="       Pay_Code IN ('C', 'D') AND ";
    sql_stmt+="       Limit <> 0 GROUP BY case_sn;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

    sql_stmt ="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET ";
    sql_stmt+="MS117_6M = V1 FROM TMP1 AS A ";
    sql_stmt+="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=6 ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

/*----FS005----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS005---*/
    sql_stmt ="INSERT INTO TMP1 ";
    sql_stmt +="SELECT case_sn, Mon_Since, 1 ";
    sql_stmt +=("FROM " + krm023 + " ");
    sql_stmt +="WHERE Bucket_f_1K >=2 ";
    sql_stmt +="GROUP BY case_sn, Mon_Since  ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt ="INSERT INTO TMP ";
    sql_stmt +="SELECT case_sn, 3, SUM(V1), NULL, NULL ";
    sql_stmt +="FROM TMP1 ";
    sql_stmt +="WHERE (:now - Mon)<= 3 ";
//    sql_stmt +="      AND (:now1 - Mon) > 0  ";
    sql_stmt +="GROUP BY case_sn ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

    sql_stmt ="UPDATE PDACO_V1_00_CAL ";
    sql_stmt +="SET ";
    sql_stmt +="FS005_3M_1k = V1 ";
    sql_stmt +="FROM TMP AS A ";
    sql_stmt +="WHERE A.case_sn = PDACO_V1_00_CAL.case_sn AND Mon=3";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

//Trend and interaction variables
		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt+="SET  ";
		sql_stmt+="INT015_3M = FS016_3M / (CASE WHEN FS101_3M = 0 THEN NULL ELSE CONVERT(FLOAT,FS101_3M) END), ";
    sql_stmt+="INT028_9M = FS014_9M / (CASE WHEN FS101_9M = 0 THEN NULL ELSE CONVERT(FLOAT,FS101_9M) END), ";
		sql_stmt+="FT059_1K_42 = FS059_3M_1K - FS059_9M_1K + FS059_6M_1K, ";
		sql_stmt+="FT212_43_1K = 2 * FS212_3M_1K - FS212_6M_1K, ";
		sql_stmt+="FT102_42 = FS102_3M - FS102_9M + FS102_6M, ";
		sql_stmt+="FT059_1K_43 = 2 * FS059_3M_1K - FS059_6M_1K ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
    command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();

//Variable transformation phase 1
    sql_stmt="UPDATE PDACO_V1_00_CAL ";
		sql_stmt+="SET  ";
		sql_stmt+="INT015_3M_T =  ";
		sql_stmt+="(CASE WHEN INT015_3M>1 THEN 1 ";
		sql_stmt+="	    WHEN INT015_3M IS NULL THEN 1 ";
		sql_stmt+=" ELSE INT015_3M ";
		sql_stmt+=" END),	 ";
    sql_stmt+="INT028_9M_T = ";
		sql_stmt+="(CASE WHEN INT028_9M IS NULL THEN 0.3 ";
		sql_stmt+="	    WHEN INT028_9M > 0.3 THEN 0.3 ";
		sql_stmt+=" ELSE INT028_9M ";
		sql_stmt+=" END),	 ";
		sql_stmt+="FT059_1K_42_Q = POWER(FT059_1K_42,2), ";
		sql_stmt+="FT212_43_1K_Q = POWER(FT212_43_1K,2), ";
		sql_stmt+="FT102_42_R = POWER((CASE WHEN FT102_42 < 0 THEN NULL ELSE CONVERT(FLOAT,FT102_42) END),0.5), ";
		sql_stmt+="FS031_T = ";
		sql_stmt+="(CASE WHEN FS031 IS NULL THEN -1 ";
		sql_stmt+="	    WHEN FS031>9 THEN 9 ";
		sql_stmt+="	    ELSE FS031 ";
		sql_stmt+=" END), ";
    if(with_gender)
		  sql_stmt+="SEX_T = (CASE WHEN SEX = 1 THEN 1 ELSE 0 END),  ";
    else
		  sql_stmt+="SEX_T = 0.5,  ";
		sql_stmt+="APP_LAST_MONTH_BUCKET_T = ";
		sql_stmt+="(CASE WHEN APP_LAST_MONTH_BUCKET IS NULL THEN 1 ";
		sql_stmt+="	    WHEN APP_LAST_MONTH_BUCKET>2 THEN 2 ";
		sql_stmt+=" ELSE APP_LAST_MONTH_BUCKET ";
		sql_stmt+=" END), ";
		sql_stmt+="FS205_3M_1K_Q = POWER(FS205_3M_1K,2), ";
		sql_stmt+="FT059_1K_43_R= POWER((CASE WHEN FT059_1K_43 < 0 THEN NULL ELSE CONVERT(FLOAT,FT059_1K_43) END),0.5), ";
		sql_stmt+="FS051_T = ";
		sql_stmt+="(CASE WHEN FS051 IS NULL THEN 2 ";
		sql_stmt+="	    WHEN FS051>5 THEN 5 ";
		sql_stmt+="	    ELSE FS051 ";
		sql_stmt+=" END) ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();
//Variable transformation phase 2
    sql_stmt="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET  ";
    sql_stmt+="FT059_1K_42_Q_T =  ";
    sql_stmt+="(CASE WHEN FT059_1K_42_Q>16 THEN 16 ";
    sql_stmt+=" ELSE FT059_1K_42_Q ";
    sql_stmt+=" END), ";
    sql_stmt+="FT212_43_1K_Q_T = ";
    sql_stmt+="(CASE WHEN FT212_43_1K_Q IS NULL THEN 0 ";
    sql_stmt+="	    ELSE FT212_43_1K_Q ";
    sql_stmt+=" END), ";
    sql_stmt+=" FT102_42_R_T = ";
    sql_stmt+="(CASE WHEN FT102_42_R IS NULL THEN -0.2 ";
    sql_stmt+="	    WHEN FT102_42_R>1.7 THEN 1.7 ";
    sql_stmt+=" ELSE FT102_42_R ";
    sql_stmt+=" END), ";
    sql_stmt+="FS205_3M_1K_Q_T = ";
    sql_stmt+="(CASE WHEN FS205_3M_1K_Q IS NULL THEN 0 ";
    sql_stmt+="	    WHEN FS205_3M_1K_Q>60 THEN 60 ";
    sql_stmt+=" ELSE FS205_3M_1K_Q ";
    sql_stmt+=" END), ";
    sql_stmt+="FT059_1K_43_R_T = ";
    sql_stmt+="(CASE WHEN FT059_1K_43_R IS NULL THEN 1 ";
    sql_stmt+=" ELSE FT059_1K_43_R ";
    sql_stmt+=" END) ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();
//Model calculation
    sql_stmt="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET  ";
    sql_stmt+="SCORE=0.01474 + ";
    sql_stmt+="INT015_3M_T * 0.04190 + ";
    sql_stmt+="FT059_1K_42_Q_T * 0.00964 + ";
    sql_stmt+="FT212_43_1K_Q_T * 0.01186 + ";
    sql_stmt+="FT102_42_R_T * -0.02683 + ";
    sql_stmt+="FS031_T * 0.00573 + ";
    sql_stmt+="MS117_6M * 0.04292 + ";
    sql_stmt+="FS005_3M_1K * 0.12726 + ";
    sql_stmt+="SEX_T * 0.02654 + ";
    sql_stmt+="App_Last_Month_Bucket_T * 0.05866 + ";
    sql_stmt+="FS205_3M_1K_Q_T * 0.00058973 + ";
    sql_stmt+="INT028_9M_T * -0.08045 + ";
    sql_stmt+="FT059_1K_43_R_T * 0.01873 + ";
    sql_stmt+="FS051_T * 0.00345 ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();
//Cut
    sql_stmt="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET  ";
    sql_stmt+="CUT =  ";
    sql_stmt+="(CASE  ";
    sql_stmt+="WHEN SCORE <=	-0.03231	THEN 1 ";
    sql_stmt+="WHEN SCORE <=	-0.02275	THEN 2 ";
    sql_stmt+="WHEN SCORE <=	-0.01479	THEN 3 ";
    sql_stmt+="WHEN SCORE <=	-0.00919	THEN 4 ";
    sql_stmt+="WHEN SCORE <=	-0.00438	THEN 5 ";
    sql_stmt+="WHEN SCORE <=	 0.00101	THEN 6 ";
    sql_stmt+="WHEN SCORE <=	 0.00624	THEN 7 ";
    sql_stmt+="WHEN SCORE <=	 0.01245	THEN 8 ";
    sql_stmt+="WHEN SCORE <=	 0.01836	THEN 9 ";
    sql_stmt+="WHEN SCORE <=	 0.02482	THEN 10 ";
    sql_stmt+="WHEN SCORE <=	 0.03219	THEN 11 ";
    sql_stmt+="WHEN SCORE <=	 0.03963	THEN 12 ";
    sql_stmt+="WHEN SCORE <=	 0.04759	THEN 13 ";
    sql_stmt+="WHEN SCORE <=	 0.05585	THEN 14 ";
    sql_stmt+="WHEN SCORE <=	 0.06657	THEN 15 ";
    sql_stmt+="WHEN SCORE <=	 0.07865	THEN 16 ";
    sql_stmt+="WHEN SCORE <=	 0.09435	THEN 17 ";
    sql_stmt+="WHEN SCORE <=	 0.11509	THEN 18 ";
    sql_stmt+="WHEN SCORE <=	 0.15002	THEN 19 ";
    sql_stmt+="WHEN SCORE >	 0.15002	THEN 20 ";
    sql_stmt+="ELSE 0	END) ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();

//commented out on Apr. 6 2005 for the following code should have no effect
//when confirmed via testing, this comment and the following commented-out codes
//will be deleted
//PB assignment for the first PB fit - M4
/*    sql_stmt="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET  ";
    sql_stmt+="PB1 =  ";
    sql_stmt+="(CASE  ";
    sql_stmt += "WHEN CUT = 0 THEN  NULL ";
    sql_stmt += "WHEN CUT = 1 THEN  0.004660116 ";
    sql_stmt += "WHEN CUT = 2 THEN  0.004997554 ";
    sql_stmt += "WHEN CUT = 3 THEN  0.005376044 ";
    sql_stmt += "WHEN CUT = 4 THEN  0.005807849 ";
    sql_stmt += "WHEN CUT = 5 THEN  0.006308895 ";
    sql_stmt += "WHEN CUT = 6 THEN  0.006899864 ";
    sql_stmt += "WHEN CUT = 7 THEN  0.007607615 ";
    sql_stmt += "WHEN CUT = 8 THEN  0.008467033 ";
    sql_stmt += "WHEN CUT = 9 THEN  0.009523419 ";
    sql_stmt += "WHEN CUT = 10 THEN  0.010835609 ";
    sql_stmt += "WHEN CUT = 11 THEN  0.012480012 ";
    sql_stmt += "WHEN CUT = 12 THEN  0.01455586 ";
    sql_stmt += "WHEN CUT = 13 THEN  0.017192025 ";
    sql_stmt += "WHEN CUT = 14 THEN  0.020555877 ";
    sql_stmt += "WHEN CUT = 15 THEN  0.024864775 ";
    sql_stmt += "WHEN CUT = 16 THEN  0.030401007 ";
    sql_stmt += "WHEN CUT = 17 THEN  0.037531178 ";
    sql_stmt += "WHEN CUT = 18 THEN  0.0467314 ";
    sql_stmt += "WHEN CUT = 19 THEN  0.05862 ";
    sql_stmt += "WHEN CUT = 20 THEN  0.074 END) ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();
//PB assignment for the second PB fit
    sql_stmt="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET  ";
    sql_stmt+="PB2 =  ";
    sql_stmt+="(CASE  ";
    sql_stmt += "WHEN CUT = 0 THEN NULL ";
    sql_stmt += "WHEN CUT = 1 THEN 0.001402016 ";
    sql_stmt += "WHEN CUT = 2 THEN 0.002803476 ";
    sql_stmt += "WHEN CUT = 3 THEN 0.004205993 ";
    sql_stmt += "WHEN CUT = 4 THEN 0.005610332 ";
    sql_stmt += "WHEN CUT = 5 THEN 0.007017814 ";
    sql_stmt += "WHEN CUT = 6 THEN 0.008430714 ";
    sql_stmt += "WHEN CUT = 7 THEN 0.009852955 ";
    sql_stmt += "WHEN CUT = 8 THEN 0.011291301 ";
    sql_stmt += "WHEN CUT = 9 THEN 0.012757416 ";
    sql_stmt += "WHEN CUT = 10 THEN 0.014271407 ";
    sql_stmt += "WHEN CUT = 11 THEN 0.015867943 ";
    sql_stmt += "WHEN CUT = 12 THEN 0.017606797 ";
    sql_stmt += "WHEN CUT = 13 THEN 0.01959103 ";
    sql_stmt += "WHEN CUT = 14 THEN 0.021998328 ";
    sql_stmt += "WHEN CUT = 15 THEN 0.025135048 ";
    sql_stmt += "WHEN CUT = 16 THEN 0.029529392 ";
    sql_stmt += "WHEN CUT = 17 THEN 0.036092056 ";
    sql_stmt += "WHEN CUT = 18 THEN 0.0463932 ";
    sql_stmt += "WHEN CUT = 19 THEN 0.06314 ";
    sql_stmt += "WHEN CUT = 20 THEN  0.091 END) ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();        */

	  sql_stmt = "SELECT SCORE FROM PDACO_V1_00_CAL WHERE case_sn = :case_sn;";
	  sql_stmt = sql_stmt.UpperCase();
	  query->Close();
	  query->SQL->Clear();
	  query->SQL->Add(sql_stmt);
	  query->Parameters->ParamValues["case_sn"] = case_sn;
	  query->Open();
	  risk_score = query->FieldValues["SCORE"];
	  query->Close();
	}
	else{
  	sql_stmt = "UPDATE PDACO_V1_00_CAL SET EXCLUSION = :exclusion ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["exclusion"] = exclusion;
	  command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();
  };
  return risk_score;
};

double tf_a2(TADOQuery *query, TADOCommand *command,
             const AnsiString &krm023, const AnsiString &krm001,
             const AnsiString &bam085, const AnsiString &stm001,
             const AnsiString &case_sn, int gender, int now,
             unsigned int exclusion, unsigned int data_flag)
{
	AnsiString sql_stmt;
  bool with_gender = true;
	double risk_score  = 1000;

  try{
		sql_stmt = "CREATE TABLE TF_A2_CAL(case_sn CHAR(12), ";
		sql_stmt += "FS031 int, FS059_3M_1K int, ";
    sql_stmt += "FS310 int, Gender int, Loan_Del_Number_6M_Tran3 int, ";
		sql_stmt += "APP_LAST_MONTH_BUCKET int, FS203_12M_1k int, ";
    sql_stmt += "FS014_12M int, MS056_6M_1K float, MS024_3M float, ";
		sql_stmt += "FS031_Tran3 int, FS059_3M_1K_Tran3 int, ";
    sql_stmt += "FS310_Tran3 int, ";
		sql_stmt += "APP_LAST_MONTH_BUCKET_Tran3 int, FS203_12M_1k_Tran3 int, ";
    sql_stmt += "FS014_12M_Tran3 int, MS056_6M_1K_R float, MS024_3M_R float, ";
    sql_stmt += "MS056_6M_1K_R_Tran3 float, MS024_3M_R_Tran3 float, ";
		sql_stmt += "EXCLUSION int, HIT int, SCORE decimal(11,10));";
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

  sql_stmt = "INSERT INTO TF_A2_CAL(case_sn, HIT, EXCLUSION, Gender, Loan_Del_Number_6M_Tran3) ";
	sql_stmt += "VALUES (:case_sn, :hit, :exclusion, :gender, :loan_del);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["case_sn"] = case_sn;
	command->Parameters->ParamValues["hit"] = data_flag;
	command->Parameters->ParamValues["gender"] = gender;
	command->Parameters->ParamValues["exclusion"] = exclusion;
	command->Parameters->ParamValues["loan_del"] = 0; //TF variable
	command->Execute();

	bool run_model = true;
  if(((exclusion & 0x1) == 0x1) ||  ((exclusion & 0x2) == 0x2))
    run_model = false;

	if(run_model){
//Init variables
		if(data_flag & 0x1){ //KRM023
			sql_stmt = "UPDATE TF_A2_CAL SET ";
			sql_stmt += "FS014_12M = 0, MS056_6M_1K = 0, MS024_3M = 0, FS059_3M_1K = 0 ";
      sql_stmt += "WHERE case_sn = :case_sn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["case_sn"] = case_sn;
			command->Execute();
		};
		if((data_flag & 0x1) && (data_flag & 0x2)){ //KRM001 and KRM023
			sql_stmt = "UPDATE TF_A2_CAL SET ";
			sql_stmt += "FS203_12M_1K = 0 ";
      sql_stmt += "FROM " + krm001 + " AS A, " + krm023 + " As B ";
      sql_stmt += "WHERE TF_A2_CAL.case_sn = :case_sn AND ";
      sql_stmt += "TF_A2_CAL.case_sn = A.case_sn AND ";
      sql_stmt += "A.case_sn = B.case_sn AND ";
      sql_stmt += "(CASE WHEN A.ISSUE = '021' AND CARD_BRAND = 'V' THEN 'CTV' ";
      sql_stmt += "WHEN A.ISSUE = '021' AND CARD_BRAND = 'M' THEN 'CTM' ";
      sql_stmt += "WHEN A.ISSUE = '021' AND CARD_BRAND = 'D' THEN 'CTD' ";
      sql_stmt += "WHEN A.ISSUE = 'A82' AND CARD_BRAND = 'A' THEN 'AEA' ";
      sql_stmt += "ELSE A.ISSUE END) = B.ISSUE;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["case_sn"] = case_sn;
			command->Execute();
		};
		if(data_flag & 0x4){  //BAM085
			sql_stmt = "UPDATE TF_A2_CAL SET ";
			sql_stmt += "FS310 = 0 ";
      sql_stmt += "WHERE case_sn = :case_sn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["case_sn"] = case_sn;
			command->Execute();
		};
//		if(data_flag & 0x8){ //STM001  -- to accomodate Oliver's code
			sql_stmt = "UPDATE TF_A2_CAL SET ";
			sql_stmt += "FS031 = 0 ";
      sql_stmt += "WHERE case_sn = :case_sn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["case_sn"] = case_sn;
			command->Execute();
//		};
//		prepare_2xx_variables(command, query, krm023, krm001, now);


//create temp tables
		try{
  		sql_stmt = "DROP TABLE TMP;";
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
  		sql_stmt = "DROP TABLE TMP1;";
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
		sql_stmt = "CREATE TABLE TMP ";
	  sql_stmt += "(case_sn CHAR(12), Mon INT, V1 FLOAT, V2 FLOAT, V3 FLOAT);";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

	  sql_stmt = "CREATE TABLE TMP1 ";
	  sql_stmt += "(case_sn CHAR(12), Mon INT, V1 FLOAT);";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

//Variables
//BAM085 variables
  if(data_flag & 0x4){  //BAM085
//FS310現金卡utilization >= 98% 筆數 (Loan_amt + Pass_due_amt) / contract_amt >= 0.98, providing contract_amt <> 0 20050903 0.98 to 0.95
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = " insert into tmp(case_sn, v1) ";
    sql_stmt += "select case_sn, count(*) ";
    sql_stmt += "from " + bam085;
    sql_stmt += " where account_code = 'Y' ";
    sql_stmt += "and ((convert(float, isnull(loan_amt, 0)) + convert(float, isnull(pass_due_amt, 0))) / ";
    sql_stmt += " (case when isnull(CONTRACT_AMT, 0) = 0 then null else convert(float, CONTRACT_AMT) end)) >= 0.98 ";
    sql_stmt += "group by case_sn ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = " update tf_a2_cal ";
    sql_stmt += "set fs310 = v1 ";
    sql_stmt += "from tmp as a ";
    sql_stmt += "where a.case_sn = tf_a2_cal.case_sn";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
  };

//KRM023 variables
  if(data_flag & 0x1){  //KRM023
//fs059_1k 有延遲繳款額度數 (M1+) - Pay code in ('E','F')no 15-day rule
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = "insert into tmp (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 3, count(distinct issue) ";
    sql_stmt += "from " + krm023;
    sql_stmt += " where bucket_ef_1k >= 1 ";
    sql_stmt += "and mon_since >= :now - 3 ";
    sql_stmt += "and mon_since <= :now1 ";
    sql_stmt += "group by case_sn; ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

    sql_stmt = "update tf_a2_cal ";
    sql_stmt += "set fs059_3m_1k = v1 ";
    sql_stmt += "from tmp as a ";
    sql_stmt += "where a.case_sn = tf_a2_cal.case_sn and mon = 3;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();


//app_last_month_bucket_tran last month max among all lines
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = "insert into tmp (case_sn, v1) ";
    sql_stmt += "select case_sn, max(bucket_ef_1k) ";
    sql_stmt += "from " + krm023;
    sql_stmt += " where mon_since >= :now - 1 ";//max bucket for last 2 months, no 15-day rule
    sql_stmt += "and mon_since <= :now1 ";
    sql_stmt += "group by case_sn; ";
    sql_stmt = sql_stmt.UpperCase();
   command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

    sql_stmt = "update tf_a2_cal ";
    sql_stmt += "set app_last_month_bucket = v1 ";
    sql_stmt += "from tmp a ";
    sql_stmt += "where a.case_sn = tf_a2_cal.case_sn";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();


//MS024持有之信用卡平均額度For line > 0 onlyno 15-day rule
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = "insert into tmp (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 3, avg(convert(decimal (16, 8),(case when limit='' then NULL else limit end))) ";
    sql_stmt += "from " + krm023;
    sql_stmt += " where mon_since >= :now - 3 ";
    sql_stmt += "and mon_since <= :now1 /* no 15-day rule */ ";
    sql_stmt += "and convert(decimal (16, 8), (case when limit='' then NULL else limit end)) > 0 ";
    sql_stmt += "group by case_sn; ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

    sql_stmt = " update tf_a2_cal ";
    sql_stmt += "set ms024_3m = v1 ";
    sql_stmt += "from tmp as a ";
    sql_stmt += "where a.case_sn = tf_a2_cal.case_sn ";
    sql_stmt += "and mon=3;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();


//MS056 Max延遲繳款金額 (M1+) - Pay code in ('D','E','F') Sum up all payment of all delinquent lines by month 2. Pick the max in 3, 6, 9, 14 months
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
		sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = " insert into tmp (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, mon_since, sum(payment_amt) ";
    sql_stmt += "from " + krm023;
    sql_stmt += " where pay_code in ('D', 'E', 'F') ";
    sql_stmt += "and bucket_def_1k >= 1 ";
    sql_stmt += "group by case_sn, mon_since; ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = " insert into tmp1 (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 6, max(v1) ";
    sql_stmt += "from tmp ";
    sql_stmt += "where mon >= (:now - 6) ";
    sql_stmt += "and mon <= :now1 "; //No 15-day rule
    sql_stmt += "group by case_sn; ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

    sql_stmt = " update tf_a2_cal ";
    sql_stmt += "set ms056_6m_1k = v1 ";
    sql_stmt += "from tmp1 as a ";
    sql_stmt += "where a.case_sn = tf_a2_cal.case_sn and mon=6;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();


//FS014_12M 刷卡並付清額度數(lines) (Paycode A,B)
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = "insert into tmp (case_sn, mon, v1) ";
    sql_stmt += "select case_sn, 12, count(distinct issue) ";
    sql_stmt += "from " + krm023;
    sql_stmt += " where mon_since >= (:now - 12) ";
    sql_stmt += "and mon_since <= :now1 "; //No 15-day rule
    sql_stmt += "and pay_code in ('A', 'B') ";
    sql_stmt += "group by case_sn; ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

    sql_stmt = "update tf_a2_cal ";
    sql_stmt += "set fs014_12m = v1 ";
    sql_stmt += "from tmp as a ";
    sql_stmt += "where a.case_sn = tf_a2_cal.case_sn and mon=12;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
  };

//KRM001 and KRM023 variables
		if((data_flag & 0x1) && (data_flag & 0x2)){ //KRM001 and KRM023
//FS203_1Kof delinqunt lines opened less than a year paycode DEF
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = "insert into tmp (case_sn, mon, v1) ";
    sql_stmt += "select a.case_sn, 12, count(*) ";
    sql_stmt += "from " + krm023 + " as a ";
    sql_stmt += "where issue in (select issue from latest_line ";
    sql_stmt += " where mob <= 12 and case_sn = a.case_sn) ";
    sql_stmt += "and bucket_def_1k != 0 ";
    sql_stmt += "and mon_since >= (:now - 12) ";
    sql_stmt += "and mon_since <= :now1 "; //no 15-day rule
    sql_stmt += "group by a.case_sn ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

    sql_stmt = "update tf_a2_cal ";
    sql_stmt += "set fs203_12m_1k = v1 ";
    sql_stmt += "from tmp as a ";
    sql_stmt += "where a.case_sn = tf_a2_cal.case_sn and ";
    sql_stmt += "a.mon = 12;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
  };


  if(data_flag & 0x8){ //STM001
//STM001 variables
//fs031 JCIC查詢次數 (All) (stm001)within 3 month
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = "insert into tmp (case_sn, v1) ";
    sql_stmt += "select case_sn, count(*) ";
    sql_stmt += "from " + stm001;
    sql_stmt += " where item_list is not null ";
    sql_stmt += "and item_list <> '' ";
    sql_stmt += "and :now - query_mon_since <= 3 ";
    sql_stmt += "group by case_sn ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Execute();

    sql_stmt = "update tf_a2_cal ";
    sql_stmt += "set fs031 = v1 ";
    sql_stmt += "from tmp as a ";
    sql_stmt += "where a.case_sn = tf_A2_cal.case_sn";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
  };

//Variable transformation phase 1
    sql_stmt = "update tf_a2_cal ";
    sql_stmt += "set fs031_tran3 =(case when fs031 is null then -1 ";
	sql_stmt += "		 when fs031 > 12 then 12 ";// release
    sql_stmt += "else fs031 end), ";
//    sql_stmt += "loan_del_number_6m_tran3 =(case when loan_del_number_6m is null then 0 ";
//    sql_stmt += "when loan_del_number_6m > 25 then 25 ";
//    sql_stmt += "else loan_del_number_6m end), ";
    sql_stmt += "fs310_tran3 =(case when fs310 is null then -1 ";
 sql_stmt += "	 when fs310 > 3 then 3 "; // release
    sql_stmt += "else fs310 end), ";
    sql_stmt += "fs059_3m_1k_tran3 =(case when fs059_3m_1k is null then 0 ";
    sql_stmt += "when fs059_3m_1k > 3 then 3 ";
    sql_stmt += "else fs059_3m_1k end), ";
    sql_stmt += "app_last_month_bucket_tran3 =(case when app_last_month_bucket > 0 then 1 ";
    sql_stmt += "else 0 end), ";
    sql_stmt += "fs203_12m_1k_tran3 =(case when fs203_12m_1k is null then 0 ";
	sql_stmt += "when fs203_12m_1k > 3 then 3 "; // release
    sql_stmt += "else fs203_12m_1k end), ";
    sql_stmt += "fs014_12m_tran3 =(case when fs014_12m is null then 4 ";
    sql_stmt += "when fs014_12m > 4 then 4 else fs014_12m end), ";
    sql_stmt += "ms056_6m_1k_r =power ((case when ms056_6m_1k < 0 then null else ms056_6m_1k end), 0.5), ";
    sql_stmt += "ms024_3m_r =power ((case when ms024_3m < 0 then null else ms024_3m end), 0.5) ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();


//Variable transformation phase 2
    sql_stmt = "update tf_a2_cal set ";
    sql_stmt += "ms056_6m_1k_r_tran3 =(case when ms056_6m_1k_r is null then 2 ";
    sql_stmt += "when ms056_6m_1k_r < 2 then 2 ";
    sql_stmt += "when ms056_6m_1k_r > 20 then 20 ";
    sql_stmt += "else ms056_6m_1k_r end), ";
    sql_stmt += "ms024_3m_r_tran3 =(case when ms024_3m_r is null then 6.4 ";
    sql_stmt += "when ms024_3m_r < 6.4 then 6.4 ";
    sql_stmt += "when ms024_3m_r > 13.5 then 13.5 ";
    sql_stmt += "else ms024_3m_r end) ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();


//Model calculation
    sql_stmt = "UPDATE TF_A2_CAL SET ";
    sql_stmt+="score  = 0.14624 + ";
    sql_stmt+="fs031_tran3 * 0.01263 + ";
    sql_stmt+="loan_del_number_6m_tran3 * 0.00492 + ";
    sql_stmt+="gender* 0.05417 + ";
    sql_stmt+="fs310_tran3 * 0.02756 + ";
    sql_stmt+="fs059_3m_1k_tran3 * 0.03977 + ";
    sql_stmt+="app_last_month_bucket_tran3 * 0.10649 + ";
    sql_stmt+="fs203_12m_1k_tran3 * 0.03699 + ";
    sql_stmt+="fs014_12m_tran3 * -0.02399 + ";
    sql_stmt+="ms056_6m_1k_r_tran3 * 0.00371 + ";
    sql_stmt+="ms024_3m_r_tran3 * -0.01112 ";
    sql_stmt += "WHERE case_sn = :case_sn;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["case_sn"] = case_sn;
	  command->Execute();

	  sql_stmt = "SELECT SCORE FROM TF_A2_CAL WHERE case_sn = :case_sn;";
	  sql_stmt = sql_stmt.UpperCase();
	  query->Close();
	  query->SQL->Clear();
	  query->SQL->Add(sql_stmt);
	  query->Parameters->ParamValues["case_sn"] = case_sn;
	  query->Open();
	  risk_score = query->FieldValues["SCORE"];
	  query->Close();
	};
  return risk_score;
};


int credit_card_line(TADOQuery *query,
                     double drisk_score, double propensity_score,
                     unsigned int highest_line,
                     const AnsiString &card_type,
                     int &risk_cut, int &propensity_cut,
                     double &pb, double &ev, double &dollar_bad, bool p1)
{
  int risk_twentile = 0;
  int propensity_twentile = 0;
  unsigned int line_index;
  unsigned int line;

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

 if((card_type == "G") || (card_type == "C")){
  double regular_line[20][20]={
 24, 23, 24, 27, 30, 30, 30, 30, 29, 21, 15, 13, 12, 12, 12, 11, 11, 12, 13, 14,
 18, 19, 20, 21, 23, 24, 24, 22, 19, 16, 13, 12, 11, 11, 12, 10, 11, 12, 13, 13,
 18, 17, 17, 18, 18, 19, 19, 18, 16, 14, 12, 11, 11, 11, 12, 10, 11, 12, 12, 13,
 17, 16, 15, 16, 16, 16, 16, 15, 14, 13, 11, 11, 11, 11, 9, 10, 11, 11, 12, 13,
 16, 14, 13, 14, 14, 14, 14, 13, 13, 12, 11, 11, 11, 11, 9, 10, 11, 11, 12, 13,
 16, 13, 12, 12, 12, 12, 12, 12, 12, 11, 10, 10, 11, 11, 9, 10, 11, 11, 12, 12,
 16, 12, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 9, 10, 11, 11, 12, 12,
 16, 11, 10, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 11, 9, 10, 11, 11, 12, 12,
 16, 10, 9, 8, 8, 8, 8, 8, 8, 9, 9, 10, 10, 11, 9, 10, 10, 11, 11, 12,
 17, 10, 8, 7, 7, 7, 7, 7, 7, 8, 8, 9, 10, 8, 9, 10, 10, 11, 11, 12,
 18, 10, 8, 7, 6, 6, 6, 7, 7, 7, 8, 9, 10, 8, 9, 10, 10, 11, 11, 11,
 20, 9, 7, 6, 6, 6, 6, 6, 6, 7, 8, 8, 7, 8, 9, 9, 10, 10, 11, 11,
 20, 8, 6, 6, 5, 5, 5, 6, 6, 7, 5, 6, 7, 7, 8, 9, 10, 10, 11, 11,
 17, 8, 6, 5, 5, 5, 5, 3, 3, 4, 5, 6, 6, 7, 8, 9, 9, 10, 10, 11,
 13, 8, 6, 6, 1, 1, 1, 2, 3, 4, 5, 5, 6, 7, 8, 8, 9, 10, 10, 11,
 11, 8, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 10, 10, 11,
// 10, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 8, 9, 10, 11, 11,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 5, 6, 8, 9, 11, 12,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 4, 5, 6, 6,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 5, 7, 8, 10};
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  line = regular_line[risk_twentile -1][propensity_twentile -1] * 10000;
//The line granted can not exceed 1.5 times of the maximum line the applicant currently held
  unsigned int line_cap = highest_line * 1.5;
  if(highest_line == 0)
    line_cap = line;
  else if(line_cap > highest_line + 50000)
   line_cap = highest_line + 50000;
//Rounding the line granted to NT$0000
  if(line > line_cap)
    line = line_cap;
  line += 5000;
  line /= 10000;
  line *= 10000;

 double regular_ev[20][20][31]= {
0, 2.63156449, 3.785625083, 3.135391587, 1.750321086, 0.365250585, -1.019819916, -2.404890417, -3.789960918, -5.175031419, -6.56010192, -7.945172421, -9.330242922, -10.71531342, -12.10038392, -13.48545442, -14.87052493, -16.25559543, -17.64066593, -19.02573643, -20.41080693, -21.79587743, -23.18094793, -24.56601843, -25.95108893, -27.33615943, -28.72122994, -30.10630044, -31.49137094, -32.87644144, -34.26151194,
0, 2.662277226, 2.923458532, 1.478448016, -0.327448768, -2.133345552, -3.939242337, -5.745139121, -7.551035905, -9.35693269, -11.16282947, -12.96872626, -14.77462304, -16.58051983, -18.38641661, -20.1923134, -21.99821018, -23.80410696, -25.61000375, -27.41590053, -29.22179732, -31.0276941, -32.83359089, -34.63948767, -36.44538445, -38.25128124, -40.05717802, -41.86307481, -43.66897159, -45.47486838, -47.28076516,
0, 2.924945067, 2.963377979, 1.354717707, -0.535541057, -2.425799821, -4.316058585, -6.206317349, -8.096576113, -9.986834877, -11.87709364, -13.76735241, -15.65761117, -17.54786993, -19.4381287, -21.32838746, -23.21864623, -25.10890499, -26.99916375, -28.88942252, -30.77968128, -32.66994005, -34.56019881, -36.45045757, -38.34071634, -40.2309751, -42.12123387, -44.01149263, -45.90175139, -47.79201016, -49.68226892,
0, 3.325677828, 3.341651948, 1.668040083, -0.302879826, -2.273799735, -4.244719645, -6.215639554, -8.186559463, -10.15747937, -12.12839928, -14.09931919, -16.0702391, -18.04115901, -20.01207892, -21.98299883, -23.95391874, -25.92483865, -27.89575855, -29.86667846, -31.83759837, -33.80851828, -35.77943819, -37.7503581, -39.72127801, -41.69219792, -43.66311783, -45.63403774, -47.60495765, -49.57587755, -51.54679746,
0, 3.847793034, 3.96998081, 2.287847737, 0.243369005, -1.801109727, -3.84558846, -5.890067192, -7.934545924, -9.979024656, -12.02350339, -14.06798212, -16.11246085, -18.15693959, -20.20141832, -22.24589705, -24.29037578, -26.33485451, -28.37933325, -30.42381198, -32.46829071, -34.51276944, -36.55724818, -38.60172691, -40.64620564, -42.69068437, -44.73516311, -46.77964184, -48.82412057, -50.8685993, -52.91307803,
0, 4.496364843, 4.839942972, 3.202431922, 1.098421699, -1.005588524, -3.109598748, -5.213608971, -7.317619194, -9.421629417, -11.52563964, -13.62964986, -15.73366009, -17.83767031, -19.94168053, -22.04569076, -24.14970098, -26.2537112, -28.35772143, -30.46173165, -32.56574187, -34.66975209, -36.77376232, -38.87777254, -40.98178276, -43.08579299, -45.18980321, -47.29381343, -49.39782366, -51.50183388, -53.6058441,
0, 5.285962344, 5.971665612, 4.439848341, 2.299219221, 0.158590102, -1.982039018, -4.122668137, -6.263297257, -8.403926376, -10.5445555, -12.68518462, -14.82581374, -16.96644285, -19.10707197, -21.24770109, -23.38833021, -25.52895933, -27.66958845, -29.81021757, -31.95084669, -34.09147581, -36.23210493, -38.37273405, -40.51336317, -42.65399229, -44.79462141, -46.93525053, -49.07587965, -51.21650877, -53.35713789,
0, 6.236481167, 7.400296274, 6.048456362, 3.904161059, 1.759865755, -0.384429548, -2.528724851, -4.673020155, -6.817315458, -8.961610762, -11.10590607, -13.25020137, -15.39449667, -17.53879198, -19.68308728, -21.82738258, -23.97167789, -26.11597319, -28.26026849, -30.4045638, -32.5488591, -34.6931544, -36.83744971, -38.98174501, -41.12604031, -43.27033562, -45.41463092, -47.55892622, -49.70322153, -51.84751683,
0, 7.371190062, 9.171921337, 8.090889363, 5.985260864, 3.879632366, 1.774003867, -0.331624632, -2.437253131, -4.542881629, -6.648510128, -8.754138627, -10.85976713, -12.96539562, -15.07102412, -17.17665262, -19.28228112, -21.38790962, -23.49353812, -25.59916662, -27.70479512, -29.81042361, -31.91605211, -34.02168061, -36.12730911, -38.23293761, -40.33856611, -42.44419461, -44.54982311, -46.6554516, -48.7610801,
0, 8.714947543, 11.34323637, 10.70659222, 8.750284318, 6.729953033, 4.709621748, 2.689290463, 0.668959178, -1.351372107, -3.371703392, -5.392034677, -7.412365961, -9.432697246, -11.45302853, -13.47335982, -15.4936911, -17.51402239, -19.53435367, -21.55468496, -23.57501624, -25.59534753, -27.61567881, -29.6360101, -31.65634138, -33.67667266, -35.69700395, -37.71733523, -39.73766652, -41.7579978, -43.77832909,
0, 10.29043362, 13.98198192, 14.08493802, 12.48055203, 10.58118615, 8.681820266, 6.782454387, 4.883088507, 2.983722628, 1.084356748, -0.815009132, -2.714375011, -4.613740891, -6.51310677, -8.41247265, -10.31183853, -12.21120441, -14.11057029, -16.00993617, -17.90930205, -19.80866793, -21.70803381, -23.60739969, -25.50676557, -27.40613145, -29.30549733, -31.2048632, -33.10422908, -35.00359496, -36.90296084,
0, 12.10728414, 17.15923442, 18.30303784, 17.20022849, 15.409547, 13.61886551, 11.82818403, 10.03750254, 8.246821048, 6.456139559, 4.66545807, 2.874776581, 1.084095092, -0.706586396, -2.497267885, -4.287949374, -6.078630863, -7.869312352, -9.659993841, -11.45067533, -13.24135682, -15.03203831, -16.8227198, -18.61340129, -20.40408277, -22.19476426, -23.98544575, -25.77612724, -27.56680873, -29.35749022,
0, 14.12975763, 20.90036115, 23.48678686, 23.0318242, 21.2092915, 19.3867588, 17.5642261, 15.74169341, 13.91916071, 12.09662801, 10.27409531, 8.451562611, 6.629029912, 4.806497213, 2.983964514, 1.161431816, -0.661100883, -2.483633582, -4.306166281, -6.128698979, -7.951231678, -9.773764377, -11.59629708, -13.41882977, -15.24136247, -17.06389517, -18.88642787, -20.70896057, -22.53149327, -24.35402597,
0, 16.18790815, 25.00530539, 29.45145956, 30.65784882, 29.37164665, 27.08674565, 24.80184465, 22.51694365, 20.23204265, 17.94714165, 15.66224065, 13.37733965, 11.09243865, 8.807537648, 6.522636648, 4.237735647, 1.952834646, -0.332066354, -2.616967355, -4.901868356, -7.186769356, -9.471670357, -11.75657136, -14.04147236, -16.32637336, -18.61127436, -20.89617536, -23.18107636, -25.46597736, -27.75087836,
0, 17.7765964, 28.59830686, 34.99582957, 38.18327627, 38.63568205, 36.48584379, 32.87772416, 29.10270104, 25.32767792, 21.55265479, 17.77763167, 14.00260855, 10.22758543, 6.452562311, 2.67753919, -1.09748393, -4.872507051, -8.647530172, -12.42255329, -16.19757641, -19.97259953, -23.74762266, -27.52264578, -31.2976689, -35.07269202, -38.84771514, -42.62273826, -46.39776138, -50.1727845, -53.94780762,
0, 17.71778546, 29.35508185, 36.73071381, 40.94358995, 42.60494962, 42.01818653, 39.31095399, 34.52482131, 27.89077461, 20.44426761, 12.99776061, 5.551253611, -1.895253388, -9.341760386, -16.78826738, -24.23477438, -31.68128138, -39.12778838, -46.57429538, -54.02080238, -61.46730938, -68.91381638, -76.36032337, -83.80683037, -91.25333737, -98.69984437, -106.1463514, -113.5928584, -121.0393654, -128.4858724,
0, 13.73650622, 22.57480159, 27.6129779, 29.64350522, 29.22120228, 26.72058715, 22.38342139, 16.35727844, 8.725986844, -0.467177613, -11.20264901, -23.4731415, -37.27665178, -52.27165191, -67.69178805, -83.11192418, -98.53206032, -113.9521965, -129.3723326, -144.7924687, -160.2126049, -175.632741, -191.0528771, -206.4730133, -221.8931494, -237.3132855, -252.7334217, -268.1535578, -283.573694, -298.9938301,
0, 1.918135252, 0.158967411, -4.703356597, -12.20151192, -21.95921584, -33.67711134, -47.12021678, -62.10684514, -78.4988968, -96.19343027, -115.1154164, -135.211582, -156.4452496, -178.7920813, -202.2366349, -226.7696422, -252.3859203, -279.0828272, -306.8591757, -335.7145203, -365.6487339, -396.6053487, -428.044941, -459.4845332, -490.9241255, -522.3637178, -553.8033101, -585.2429024, -616.6824946, -648.1220869,
0, -24.20441742, -51.03963264, -80.23201339, -111.5411346, -144.7565706, -179.6949136, -216.1970072, -254.1253877, -293.3619216, -333.8056322, -375.3707042, -417.984659, -461.5866911, -506.1261567, -551.5612065, -597.8575545, -644.9873736, -692.9283106, -741.6626132, -791.1763597, -841.4587852, -892.5016968, -944.2989693, -996.8461159, -1050.139926, -1104.178163, -1158.95932, -1214.482418, -1270.746848, -1327.752257,
0, -75.18823188, -152.2105572, -230.9424434, -311.2691493, -393.0851082, -476.2933391, -560.8048868, -646.5382887, -733.4190682, -821.3792536, -910.3569216, -1000.295765, -1091.144683, -1182.857393, -1275.392068, -1368.710989, -1462.780219, -1557.5693, -1653.050964, -1749.200866, -1845.997326, -1943.4211, -2041.455152, -2140.084456, -2239.295799, -2339.077607, -2439.419778, -2540.313531, -2641.751269, -2743.726444,
0, 3.339978523, 4.920958857, 4.41032017, 2.912626962, 1.414933754, -0.082759453, -1.580452661, -3.078145868, -4.575839076, -6.073532284, -7.571225491, -9.068918699, -10.56661191, -12.06430511, -13.56199832, -15.05969153, -16.55738474, -18.05507794, -19.55277115, -21.05046436, -22.54815757, -24.04585078, -25.54354398, -27.04123719, -28.5389304, -30.03662361, -31.53431681, -33.03201002, -34.52970323, -36.02739644,
0, 3.731321018, 4.590262932, 3.396723089, 1.645212793, -0.106297504, -1.8578078, -3.609318097, -5.360828393, -7.11233869, -8.863848986, -10.61535928, -12.36686958, -14.11837988, -15.86989017, -17.62140047, -19.37291077, -21.12442106, -22.87593136, -24.62744166, -26.37895195, -28.13046225, -29.88197254, -31.63348284, -33.38499314, -35.13650343, -36.88801373, -38.63952403, -40.39103432, -42.14254462, -43.89405492,
0, 4.300181266, 4.978397941, 3.519039994, 1.572930813, -0.373178368, -2.319287548, -4.265396729, -6.21150591, -8.157615091, -10.10372427, -12.04983345, -13.99594263, -15.94205181, -17.88816099, -19.83427018, -21.78037936, -23.72648854, -25.67259772, -27.6187069, -29.56481608, -31.51092526, -33.45703444, -35.40314362, -37.3492528, -39.29536198, -41.24147116, -43.18758034, -45.13368953, -47.07979871, -49.02590789,
0, 4.970639692, 5.648340873, 4.022482283, 1.869291742, -0.2838988, -2.437089341, -4.590279883, -6.743470425, -8.896660966, -11.04985151, -13.20304205, -15.35623259, -17.50942313, -19.66261367, -21.81580422, -23.96899476, -26.1221853, -28.27537584, -30.42856638, -32.58175692, -34.73494747, -36.88813801, -39.04132855, -41.19451909, -43.34770963, -45.50090017, -47.65409072, -49.80728126, -51.9604718, -54.11366234,
0, 5.725284721, 6.50625732, 4.755623339, 2.375435001, -0.004753337, -2.384941675, -4.765130013, -7.145318351, -9.525506689, -11.90569503, -14.28588337, -16.6660717, -19.04626004, -21.42644838, -23.80663672, -26.18682506, -28.56701339, -30.94720173, -33.32739007, -35.70757841, -38.08776675, -40.46795508, -42.84814342, -45.22833176, -47.6085201, -49.98870844, -52.36889677, -54.74908511, -57.12927345, -59.50946179,
0, 6.563435774, 7.528839334, 5.677711924, 3.046758385, 0.415804846, -2.215148693, -4.846102233, -7.477055772, -10.10800931, -12.73896285, -15.36991639, -18.00086993, -20.63182347, -23.26277701, -25.89373055, -28.52468409, -31.15563762, -33.78659116, -36.4175447, -39.04849824, -41.67945178, -44.31040532, -46.94135886, -49.5723124, -52.20326594, -54.83421948, -57.46517302, -60.09612655, -62.72708009, -65.35803363,
0, 7.492135946, 8.71923965, 6.787697296, 3.880886914, 0.974076533, -1.932733848, -4.83954423, -7.746354611, -10.65316499, -13.55997537, -16.46678575, -19.37359614, -22.28040652, -25.1872169, -28.09402728, -31.00083766, -33.90764804, -36.81445842, -39.72126881, -42.62807919, -45.53488957, -48.44169995, -51.34851033, -54.25532071, -57.16213109, -60.06894147, -62.97575186, -65.88256224, -68.78937262, -71.696183,
0, 8.523264313, 10.09560002, 8.106163849, 4.898648589, 1.691133328, -1.516381932, -4.723897192, -7.931412453, -11.13892771, -14.34644297, -17.55395823, -20.76147349, -23.96898876, -27.17650402, -30.38401928, -33.59153454, -36.7990498, -40.00656506, -43.21408032, -46.42159558, -49.62911084, -52.8366261, -56.04414136, -59.25165662, -62.45917188, -65.66668714, -68.8742024, -72.08171766, -75.28923292, -78.49674818,
0, 9.672514708, 11.68887874, 9.742532817, 6.280739122, 2.747571926, -0.785595271, -4.318762467, -7.851929663, -11.38509686, -14.91826406, -18.45143125, -21.98459845, -25.51776564, -29.05093284, -32.58410004, -36.11726723, -39.65043443, -43.18360163, -46.71676882, -50.24993602, -53.78310322, -57.31627041, -60.84943761, -64.3826048, -67.915772, -71.4489392, -74.98210639, -78.51527359, -82.04844079, -85.58160798,
0, 10.95889552, 13.54575971, 11.80924204, 8.189233213, 4.300539643, 0.411846074, -3.476847495, -7.365541065, -11.25423463, -15.1429282, -19.03162177, -22.92031534, -26.80900891, -30.69770248, -34.58639605, -38.47508962, -42.36378319, -46.25247676, -50.14117033, -54.0298639, -57.91855747, -61.80725104, -65.69594461, -69.58463817, -73.47333174, -77.36202531, -81.25071888, -85.13941245, -89.02810602, -92.91679959,
0, 12.40316467, 15.73503105, 14.3656426, 10.6394681, 6.34539749, 2.051326881, -2.242743729, -6.536814339, -10.83088495, -15.12495556, -19.41902617, -23.71309678, -28.00716739, -32.301238, -36.59530861, -40.88937922, -45.18344983, -49.47752044, -53.77159105, -58.06566166, -62.35973226, -66.65380287, -70.94787348, -75.24194409, -79.5360147, -83.83008531, -88.12415592, -92.41822653, -96.71229714, -101.0063678,
0, 14.0204796, 18.3496698, 17.60180168, 13.84876252, 9.04267985, 4.236597184, -0.569485482, -5.375568147, -10.18165081, -14.98773348, -19.79381614, -24.59989881, -29.40598148, -34.21206414, -39.01814681, -43.82422947, -48.63031214, -53.43639481, -58.24247747, -63.04856014, -67.8546428, -72.66072547, -77.46680813, -82.2728908, -87.07897347, -91.88505613, -96.6911388, -101.4972215, -106.3033041, -111.1093868,
0, 15.79219326, 21.47024766, 21.72939295, 18.06841768, 12.54020668, 6.978541901, 1.416877126, -4.14478765, -9.706452425, -15.2681172, -20.82978198, -26.39144675, -31.95311153, -37.5147763, -43.07644108, -48.63810585, -54.19977063, -59.7614354, -65.32310018, -70.88476495, -76.44642973, -82.0080945, -87.56975928, -93.13142406, -98.69308883, -104.2547536, -109.8164184, -115.3780832, -120.9397479, -126.5014127,
0, 17.57737128, 24.99069975, 26.71173139, 24.43886903, 18.97039141, 12.11104836, 5.251705311, -1.607637739, -8.466980788, -15.32632384, -22.18566689, -29.04500994, -35.90435299, -42.76369604, -49.62303909, -56.48238214, -63.34172519, -70.20106824, -77.06041128, -83.91975433, -90.77909738, -97.63844043, -104.4977835, -111.3571265, -118.2164696, -125.0758126, -131.9351557, -138.7944987, -145.6538418, -152.5131848,
0, 18.89764193, 28.13960473, 31.53176713, 30.89860275, 26.95755894, 19.91293873, 10.97017231, 1.665394245, -7.639383821, -16.94416189, -26.24893995, -35.55371802, -44.85849609, -54.16327415, -63.46805222, -72.77283029, -82.07760835, -91.38238642, -100.6871645, -109.9919426, -119.2967206, -128.6014987, -137.9062768, -147.2110548, -156.5158329, -165.820611, -175.125389, -184.4301671, -193.7349452, -203.0397232,
0, 18.56579856, 28.61635344, 32.91158745, 33.11297616, 30.14210867, 24.45596865, 16.24736733, 5.579785726, -7.395579718, -21.45547284, -35.51536596, -49.57525909, -63.63515221, -77.69504533, -91.75493846, -105.8148316, -119.8747247, -133.9346178, -147.994511, -162.0544041, -176.1142972, -190.1741903, -204.2340834, -218.2939766, -232.3538697, -246.4137628, -260.4736559, -274.5335491, -288.5934422, -302.6533353,
0, 14.24180396, 21.56911539, 23.66106221, 21.72528998, 16.60436781, 8.865104234, -1.127818214, -13.15246155, -27.08305452, -42.85474404, -60.43817233, -79.82234051, -101.0041748, -123.67123, -146.9249187, -170.1786075, -193.4322962, -216.685985, -239.9396738, -263.1933625, -286.4470513, -309.70074, -332.9544288, -356.2081175, -379.4618063, -402.7154951, -425.9691838, -449.2228726, -472.4765613, -495.7302501,
0, 1.924783247, -1.369421092, -9.000248138, -20.25121119, -34.54682124, -51.43039549, -70.54438937, -91.61308785, -114.4274939, -138.8322534, -164.7144577, -191.994168, -220.6165053, -250.545155, -281.7571363, -314.2386882, -347.9821277, -382.9835408, -419.2411657, -456.7543376, -495.522864, -535.5399446, -576.1779185, -616.8158925, -657.4538665, -698.0918404, -738.7298144, -779.3677884, -820.0057623, -860.6437363,
0, -24.90944123, -53.56157199, -85.53494695, -120.4598093, -158.0130247, -197.9133803, -239.9172305, -283.8144768, -329.4248624, -376.5945697, -425.1931032, -475.1104434, -526.2544582, -578.5485561, -631.9295675, -686.3458412, -741.7555407, -798.1251295, -855.42803, -913.6434452, -972.75533, -1032.7515, -1093.622869, -1155.362793, -1217.966534, -1281.430795, -1345.753362, -1410.932798, -1476.968215, -1543.859093,
0, -76.83585575, -156.280833, -238.1431402, -322.2462169, -408.4277606, -496.5388006, -586.4428167, -678.0149013, -771.140964, -865.716977, -961.6482598, -1058.848802, -1157.240623, -1256.753169, -1357.322737, -1458.891944, -1561.409216, -1664.828316, -1769.107893, -1874.21107, -1980.105047, -2086.760739, -2194.152434, -2302.257476, -2411.05597, -2520.530508, -2630.665921, -2741.449037, -2852.868473, -2964.914437,
0, 4.479143877, 6.503533241, 6.134246158, 4.750252954, 3.366259751, 1.982266548, 0.598273345, -0.785719859, -2.169713062, -3.553706265, -4.937699469, -6.321692672, -7.705685875, -9.089679079, -10.47367228, -11.85766549, -13.24165869, -14.62565189, -16.0096451, -17.3936383, -18.7776315, -20.16162471, -21.54561791, -22.92961111, -24.31360432, -25.69759752, -27.08159072, -28.46558392, -29.84957713, -31.23357033,
0, 5.215136216, 6.758670945, 5.813538589, 4.141716493, 2.469894397, 0.798072301, -0.873749795, -2.545571892, -4.217393988, -5.889216084, -7.56103818, -9.232860276, -10.90468237, -12.57650447, -14.24832656, -15.92014866, -17.59197076, -19.26379285, -20.93561495, -22.60743704, -24.27925914, -25.95108124, -27.62290333, -29.29472543, -30.96654752, -32.63836962, -34.31019172, -35.98201381, -37.65383591, -39.325658,
0, 6.107122637, 7.576948453, 6.339741489, 4.390614717, 2.441487945, 0.492361174, -1.456765598, -3.40589237, -5.355019142, -7.304145914, -9.253272686, -11.20239946, -13.15152623, -15.100653, -17.04977977, -18.99890655, -20.94803332, -22.89716009, -24.84628686, -26.79541363, -28.7445404, -30.69366718, -32.64279395, -34.59192072, -36.54104749, -38.49017426, -40.43930104, -42.38842781, -44.33755458, -46.28668135,
0, 7.080471652, 8.627411767, 7.181077705, 4.934847911, 2.688618117, 0.442388323, -1.803841471, -4.050071265, -6.296301059, -8.542530853, -10.78876065, -13.03499044, -15.28122024, -17.52745003, -19.77367982, -22.01990962, -24.26613941, -26.51236921, -28.758599, -31.00482879, -33.25105859, -35.49728838, -37.74351818, -39.98974797, -42.23597776, -44.48220756, -46.72843735, -48.97466715, -51.22089694, -53.46712673,
0, 8.111439342, 9.815177359, 8.184375685, 5.601595168, 3.018814651, 0.436034134, -2.146746383, -4.7295269, -7.312307417, -9.895087934, -12.47786845, -15.06064897, -17.64342949, -20.22621, -22.80899052, -25.39177104, -27.97455155, -30.55733207, -33.14011259, -35.7228931, -38.30567362, -40.88845414, -43.47123466, -46.05401517, -48.63679569, -51.21957621, -53.80235672, -56.38513724, -58.96791776, -61.55069827,
0, 9.191118856, 11.10230454, 9.284761165, 6.310822033, 3.336882902, 0.36294377, -2.610995362, -5.584934493, -8.558873625, -11.53281276, -14.50675189, -17.48069102, -20.45463015, -23.42856928, -26.40250841, -29.37644755, -32.35038668, -35.32432581, -38.29826494, -41.27220407, -44.2461432, -47.22008234, -50.19402147, -53.1679606, -56.14189973, -59.11583886, -62.08977799, -65.06371712, -68.03765626, -71.01159539,
0, 10.31776873, 12.47450628, 10.45326101, 7.021619167, 3.589977324, 0.158335481, -3.273306362, -6.704948206, -10.13659005, -13.56823189, -16.99987374, -20.43151558, -23.86315742, -27.29479926, -30.72644111, -34.15808295, -37.58972479, -41.02136664, -44.45300848, -47.88465032, -51.31629217, -54.74793401, -58.17957585, -61.6112177, -65.04285954, -68.47450138, -71.90614323, -75.33778507, -78.76942691, -82.20106876,
0, 11.49453168, 13.9324006, 11.81441037, 7.980290852, 4.014527955, 0.048765058, -3.91699784, -7.882760737, -11.84852363, -15.81428653, -19.78004943, -23.74581233, -27.71157522, -31.67733812, -35.64310102, -39.60886392, -43.57462681, -47.54038971, -51.50615261, -55.4719155, -59.4376784, -63.4034413, -67.3692042, -71.33496709, -75.30072999, -79.26649289, -83.23225579, -87.19801868, -91.16378158, -95.12954448,
0, 12.72904655, 15.49124012, 13.30464975, 9.036872667, 4.450726821, -0.135419025, -4.721564871, -9.307710716, -13.89385656, -18.48000241, -23.06614825, -27.6522941, -32.23843995, -36.82458579, -41.41073164, -45.99687748, -50.58302333, -55.16916918, -59.75531502, -64.34146087, -68.92760671, -73.51375256, -78.0998984, -82.68604425, -87.2721901, -91.85833594, -96.44448179, -101.0306276, -105.6167735, -110.2029193,
0, 14.03405941, 17.18684439, 14.97548988, 10.24078916, 4.933621921, -0.37354532, -5.680712561, -10.9878798, -16.29504704, -21.60221429, -26.90938153, -32.21654877, -37.52371601, -42.83088325, -48.13805049, -53.44521773, -58.75238497, -64.05955222, -69.36671946, -74.6738867, -79.98105394, -85.28822118, -90.59538842, -95.90255566, -101.2097229, -106.5168901, -111.8240574, -117.1312246, -122.4383919, -127.7455591,
0, 15.42794485, 19.08760601, 16.95067024, 11.73217877, 5.574168017, -0.583842736, -6.741853489, -12.89986424, -19.05787499, -25.21588575, -31.3738965, -37.53190725, -43.68991801, -49.84792876, -56.00593951, -62.16395027, -68.32196102, -74.47997177, -80.63798253, -86.79599328, -92.95400403, -99.11201478, -105.2700255, -111.4280363, -117.586047, -123.7440578, -129.9020685, -136.0600793, -142.2180901, -148.3761008,
0, 16.93087648, 21.30623886, 19.44951649, 13.75743584, 6.55297069, -0.651494463, -7.855959616, -15.06042477, -22.26488992, -29.46935508, -36.67382023, -43.87828538, -51.08275054, -58.28721569, -65.49168084, -72.69614599, -79.90061115, -87.1050763, -94.30954145, -101.5140066, -108.7184718, -115.9229369, -123.1274021, -130.3318672, -137.5363324, -144.7407975, -151.9452627, -159.1497278, -166.354193, -173.5586581,
0, 18.54121377, 23.97673994, 22.76386991, 16.94141588, 8.668630381, 0.076334473, -8.515961435, -17.10825734, -25.70055325, -34.29284916, -42.88514507, -51.47744098, -60.06973688, -68.66203279, -77.2543287, -85.84662461, -94.43892052, -103.0312164, -111.6235123, -120.2158082, -128.8081041, -137.4004001, -145.992696, -154.5849919, -163.1772878, -171.7695837, -180.3618796, -188.9541755, -197.5464714, -206.1387673,
0, 20.14887735, 27.09007696, 27.0060946, 22.2076031, 13.44523621, 2.816528225, -7.812179759, -18.44088774, -29.06959573, -39.69830371, -50.32701169, -60.95571968, -71.58442766, -82.21313564, -92.84184363, -103.4705516, -114.0992596, -124.7279676, -135.3566756, -145.9853835, -156.6140915, -167.2427995, -177.8715075, -188.5002155, -199.1289235, -209.7576314, -220.3863394, -231.0150474, -241.6437554, -252.2724634,
0, 21.3080135, 29.99472679, 31.36403422, 27.91264344, 20.60721828, 9.721387296, -3.569947331, -17.49785432, -31.42576132, -45.35366831, -59.2815753, -73.20948229, -87.13738928, -101.0652963, -114.9932033, -128.9211103, -142.8490173, -156.7769242, -170.7048312, -184.6327382, -198.5606452, -212.4885522, -226.4164592, -240.3443662, -254.2722732, -268.2001802, -282.1280872, -296.0559942, -309.9839012, -323.9118081,
0, 20.82972201, 30.44133721, 32.71902638, 29.96472526, 23.43453186, 13.74106999, 1.138894312, -14.28904134, -32.52454864, -52.17383939, -71.83408118, -91.49432298, -111.1545648, -130.8148066, -150.4750484, -170.1352902, -189.795532, -209.4557738, -229.1160156, -248.7762574, -268.4364992, -288.096741, -307.7569828, -327.4172246, -347.0774663, -366.7377081, -386.3979499, -406.0581917, -425.7184335, -445.3786753,
0, 16.30942156, 23.44190827, 23.78168079, 19.02457225, 10.33992874, -1.495361702, -15.98801638, -32.84199885, -51.89198396, -73.0539928, -96.29056102, -121.5878238, -148.9419264, -178.1145478, -208.0775787, -238.0406095, -268.0036403, -297.9666712, -327.929702, -357.8927328, -387.8557637, -417.8187945, -447.7818254, -477.7448562, -507.707887, -537.6709179, -567.6339487, -597.5969795, -627.5600104, -657.5230412,
0, 3.650664364, 0.339486836, -8.673524461, -22.37242366, -39.94717053, -60.75986846, -84.31509116, -110.2339983, -138.2319461, -168.0993047, -199.6852014, -232.8839146, -267.6236518, -303.8574505, -341.5559507, -380.7017945, -421.2854198, -463.3020209, -506.7494634, -551.6269511, -597.9342546, -645.6713304, -694.1954619, -742.7917651, -791.3880684, -839.9843716, -888.5806748, -937.176978, -985.7732812, -1034.369584,
0, -23.73811569, -52.48012361, -85.6229375, -122.6396878, -163.0719492, -206.5225597, -252.6490029, -301.157325, -351.7965588, -404.353627, -458.6487, -514.5309798, -571.874887, -630.5766244, -690.5510943, -751.7291449, -814.0551245, -877.484719, -941.9830538, -1007.523037, -1074.083926, -1141.650092, -1210.209977, -1279.755207, -1350.279857, -1421.779853, -1494.25248, -1567.695998, -1642.109342, -1717.491896,
0, -76.45437094, -156.3970073, -239.5536341, -325.6723964, -414.5223731, -505.8921653, -599.5885575, -695.4352473, -793.2716441, -892.9517312, -994.3429913, -1097.325392, -1201.79043, -1307.640229, -1414.786694, -1523.150714, -1632.661418, -1743.255474, -1854.876435, -1967.474129, -2081.004089, -2195.427026, -2310.708333, -2426.817631, -2543.728343, -2661.417312, -2779.86443, -2899.05232, -3018.966021, -3139.592722,
0, 6.478792444, 9.422071963, 9.385761137, 8.167462624, 6.949164112, 5.730865599, 4.512567086, 3.294268574, 2.075970061, 0.857671549, -0.360626964, -1.578925476, -2.797223989, -4.015522502, -5.233821014, -6.452119527, -7.670418039, -8.888716552, -10.10701506, -11.32531358, -12.54361209, -13.7619106, -14.98020911, -16.19850763, -17.41680614, -18.63510465, -19.85340317, -21.07170168, -22.29000019, -23.5082987,
0, 7.519124682, 10.11091827, 9.520440453, 7.940103844, 6.359767236, 4.779430627, 3.199094019, 1.61875741, 0.038420802, -1.541915807, -3.122252415, -4.702589024, -6.282925632, -7.863262241, -9.443598849, -11.02393546, -12.60427207, -14.18460867, -15.76494528, -17.34528189, -18.9256185, -20.50595511, -22.08629172, -23.66662833, -25.24696493, -26.82730154, -28.40763815, -29.98797476, -31.56831137, -33.14864798,
0, 8.729013947, 11.34715362, 10.45719798, 8.537351266, 6.617504555, 4.697657844, 2.777811133, 0.857964422, -1.061882289, -2.981729, -4.90157571, -6.821422421, -8.741269132, -10.66111584, -12.58096255, -14.50080926, -16.42065598, -18.34050269, -20.2603494, -22.18019611, -24.10004282, -26.01988953, -27.93973624, -29.85958295, -31.77942966, -33.69927637, -35.61912308, -37.5389698, -39.45881651, -41.37866322,
0, 10.01832226, 12.8088715, 11.69961625, 9.420501213, 7.141386177, 4.86227114, 2.583156103, 0.304041066, -1.975073971, -4.254189008, -6.533304045, -8.812419082, -11.09153412, -13.37064916, -15.64976419, -17.92887923, -20.20799427, -22.4871093, -24.76622434, -27.04533938, -29.32445441, -31.60356945, -33.88268449, -36.16179952, -38.44091456, -40.7200296, -42.99914463, -45.27825967, -47.55737471, -49.83648975,
0, 11.35088218, 14.38202323, 13.06798836, 10.37715203, 7.686315701, 4.995479371, 2.304643041, -0.386193289, -3.077029619, -5.767865949, -8.458702279, -11.14953861, -13.84037494, -16.53121127, -19.2220476, -21.91288393, -24.60372026, -27.29455659, -29.98539292, -32.67622925, -35.36706558, -38.05790191, -40.74873824, -43.43957457, -46.1304109, -48.82124723, -51.51208356, -54.20291989, -56.89375622, -59.58459255,
0, 12.70660995, 16.00727912, 14.56959593, 11.49200527, 8.310062923, 5.128120575, 1.946178227, -1.235764121, -4.417706469, -7.599648817, -10.78159116, -13.96353351, -17.14547586, -20.32741821, -23.50936056, -26.6913029, -29.87324525, -33.0551876, -36.23712995, -39.4190723, -42.60101464, -45.78295699, -48.96489934, -52.14684169, -55.32878404, -58.51072638, -61.69266873, -64.87461108, -68.05655343, -71.23849577,
0, 14.07333635, 17.64953438, 16.08540057, 12.56480112, 8.789468954, 5.014136785, 1.238804617, -2.536527552, -6.31185972, -10.08719189, -13.86252406, -17.63785623, -21.41318839, -25.18852056, -28.96385273, -32.7391849, -36.51451707, -40.28984924, -44.06518141, -47.84051357, -51.61584574, -55.39117791, -59.16651008, -62.94184225, -66.71717442, -70.49250658, -74.26783875, -78.04317092, -81.81850309, -85.59383526,
0, 15.44447157, 19.28991823, 17.57443547, 13.53035245, 9.039074382, 4.547796311, 0.05651824, -4.434759832, -8.926037903, -13.41731597, -17.90859404, -22.39987212, -26.89115019, -31.38242826, -35.87370633, -40.3649844, -44.85626247, -49.34754054, -53.83881861, -58.33009668, -62.82137476, -67.31265283, -71.8039309, -76.29520897, -80.78648704, -85.27776511, -89.76904318, -94.26032125, -98.75159932, -103.2428774,
0, 16.81895892, 20.92667709, 19.03215304, 14.37334003, 9.02374062, 3.674141211, -1.675458199, -7.025057609, -12.37465702, -17.72425643, -23.07385584, -28.42345525, -33.77305466, -39.12265407, -44.47225348, -49.82185289, -55.1714523, -60.52105171, -65.87065112, -71.22025053, -76.56984994, -81.91944935, -87.26904876, -92.61864817, -97.96824758, -103.317847, -108.6674464, -114.0170458, -119.3666452, -124.7162446,
0, 18.20285141, 22.58371246, 20.49914084, 15.13179204, 8.757474494, 2.383156948, -3.991160598, -10.36547814, -16.73979569, -23.11411324, -29.48843078, -35.86274833, -42.23706588, -48.61138342, -54.98570097, -61.36001852, -67.73433606, -74.10865361, -80.48297115, -86.8572887, -93.23160625, -99.60592379, -105.9802413, -112.3545589, -118.7288764, -125.103194, -131.4775115, -137.8518291, -144.2261466, -150.6004642,
0, 19.61185425, 24.32791084, 22.09510737, 15.93545838, 8.331466128, 0.727473882, -6.876518365, -14.48051061, -22.08450286, -29.68849511, -37.29248735, -44.8964796, -52.50047185, -60.10446409, -67.70845634, -75.31244859, -82.91644083, -90.52043308, -98.12442533, -105.7284176, -113.3324098, -120.9364021, -128.5403943, -136.1443866, -143.7483788, -151.3523711, -158.9563633, -166.5603555, -174.1643478, -181.76834,
0, 21.07108924, 26.29056082, 24.06419723, 17.05311823, 7.939553926, -1.174010384, -10.28757469, -19.401139, -28.51470331, -37.62826762, -46.74183193, -55.85539624, -64.96896055, -74.08252486, -83.19608917, -92.30965348, -101.4232178, -110.5367821, -119.6503464, -128.7639107, -137.877475, -146.9910393, -156.1046036, -165.218168, -174.3317323, -183.4452966, -192.5588609, -201.6724252, -210.7859895, -219.8995538,
0, 22.59658242, 28.65924937, 26.77468201, 19.62559614, 9.322301349, -1.73545905, -12.79321945, -23.85097985, -34.90874025, -45.96650065, -57.02426105, -68.08202145, -79.13978184, -90.19754224, -101.2553026, -112.313063, -123.3708234, -134.4285838, -145.4863442, -156.5441046, -167.601865, -178.6596254, -189.7173858, -200.7751462, -211.8329066, -222.890667, -233.9484274, -245.0061878, -256.0639482, -267.1217086,
0, 24.1119976, 31.52834326, 30.49424729, 24.03961844, 12.9437027, -0.615330647, -14.36791203, -28.12049341, -41.87307479, -55.62565617, -69.37823756, -83.13081894, -96.88340032, -110.6359817, -124.3885631, -138.1411445, -151.8937258, -165.6463072, -179.3988886, -193.15147, -206.9040514, -220.6566328, -234.4092141, -248.1617955, -261.9143769, -275.6669583, -289.4195397, -303.172121, -316.9247024, -330.6772838,
0, 25.21102267, 34.3865722, 34.64721043, 29.27731674, 19.5275178, 5.750063375, -11.04207279, -28.86295636, -46.68383992, -64.50472349, -82.32560706, -100.1464906, -117.9673742, -135.7882578, -153.6091413, -171.4300249, -189.2509085, -207.071792, -224.8926756, -242.7135592, -260.5344427, -278.3553263, -296.1762099, -313.9970934, -331.817977, -349.6388606, -367.4597442, -385.2806277, -403.1015113, -420.9223949,
0, 24.71160835, 35.06791604, 36.33417779, 31.57424358, 22.42864316, 9.682591162, -6.343097696, -25.54495508, -47.90013285, -72.08619117, -96.52772865, -120.9692661, -145.4108036, -169.8523411, -194.2938786, -218.7354161, -243.1769536, -267.618491, -292.0600285, -316.501566, -340.9431035, -365.384641, -389.8261785, -414.2677159, -438.7092534, -463.1507909, -487.5923284, -512.0338659, -536.4754034, -560.9169408,
0, 20.14436522, 28.46243791, 28.21700395, 21.6979788, 10.46222148, -4.471228244, -22.4654243, -43.1443661, -66.30163514, -91.83417871, -119.6967752, -149.872909, -182.3580078, -217.0577457, -252.8180131, -288.5782806, -324.338548, -360.0988154, -395.8590828, -431.6193502, -467.3796176, -503.1398851, -538.9001525, -574.6604199, -610.4206873, -646.1809547, -681.9412221, -717.7014896, -753.461757, -789.2220244,
0, 7.304272645, 5.592705237, -3.399474112, -18.2852375, -37.96861978, -61.59465696, -88.50583946, -118.2045356, -150.3208583, -184.5854645, -220.8067969, -258.8522933, -298.6331124, -340.0919388, -383.1934577, -427.9171065, -474.2517338, -522.1918235, -571.7349626, -622.8802619, -675.6274668, -729.9765284, -785.3242239, -840.8696274, -896.4150309, -951.9604344, -1007.505838, -1063.051241, -1118.596645, -1174.142048,
0, -20.48198822, -47.45821901, -80.096163, -117.6723671, -159.5607919, -205.2220983, -254.193832, -306.0814563, -360.5501825, -417.317552, -476.146723, -536.8404165, -599.2354795, -663.198021, -728.6190823, -795.4107997, -863.5030245, -932.84036, -1003.379584, -1075.087421, -1147.938628, -1221.914377, -1297.00088, -1373.188255, -1450.469592, -1528.840191, -1608.296959, -1688.837942, -1770.461957, -1853.168332,
0, -73.84069675, -152.2079797, -234.7235054, -321.0409683, -410.8438791, -503.8434623, -599.7766673, -698.4042906, -799.5092034, -902.8946805, -1008.382827, -1115.813102, -1225.040922, -1335.936365, -1448.382947, -1562.276477, -1677.523992, -1794.042762, -1911.759362, -2030.608809, -2150.533764, -2271.483792, -2393.414671, -2516.287766, -2640.069442, -2764.730532, -2890.245845, -3016.593716, -3143.7556, -3271.715696,
0, 9.783197503, 14.32648246, 14.84496938, 13.78379471, 12.72262005, 11.66144538, 10.60027072, 9.539096056, 8.477921392, 7.416746727, 6.355572063, 5.294397399, 4.233222735, 3.172048071, 2.110873406, 1.049698742, -0.011475922, -1.072650586, -2.13382525, -3.194999915, -4.256174579, -5.317349243, -6.378523907, -7.439698572, -8.500873236, -9.5620479, -10.62322256, -11.68439723, -12.74557189, -13.80674656,
0, 11.08916235, 15.32078863, 15.2608221, 13.76654839, 12.27227469, 10.77800098, 9.283727274, 7.789453568, 6.295179861, 4.800906154, 3.306632448, 1.812358741, 0.318085034, -1.176188673, -2.670462379, -4.164736086, -5.659009793, -7.153283499, -8.647557206, -10.14183091, -11.63610462, -13.13037833, -14.62465203, -16.11892574, -17.61319945, -19.10747315, -20.60174686, -22.09602057, -23.59029427, -25.08456798,
0, 12.60639228, 16.94423757, 16.62725155, 14.79073724, 12.90880817, 11.02687909, 9.144950011, 7.263020933, 5.381091855, 3.499162777, 1.617233699, -0.264695379, -2.146624457, -4.028553535, -5.910482613, -7.792411691, -9.674340769, -11.55626985, -13.43819893, -15.320128, -17.20205708, -19.08398616, -20.96591524, -22.84784431, -24.72977339, -26.61170247, -28.49363155, -30.37556063, -32.2574897, -34.13941878,
0, 14.21675249, 18.82638201, 18.39739992, 16.25794115, 13.97517591, 11.69241067, 9.409645424, 7.126880182, 4.84411494, 2.561349698, 0.278584456, -2.004180786, -4.286946028, -6.56971127, -8.852476512, -11.13524175, -13.418007, -15.70077224, -17.98353748, -20.26630272, -22.54906797, -24.83183321, -27.11459845, -29.39736369, -31.68012893, -33.96289418, -36.24565942, -38.52842466, -40.8111899, -43.09395514,
0, 15.86696787, 20.81879526, 20.32757126, 17.86232142, 15.11919399, 12.37606655, 9.632939121, 6.889811688, 4.146684254, 1.403556821, -1.339570613, -4.082698046, -6.82582548, -9.568952913, -12.31208035, -15.05520778, -17.79833521, -20.54146265, -23.28459008, -26.02771751, -28.77084495, -31.51397238, -34.25709982, -37.00022725, -39.74335468, -42.48648212, -45.22960955, -47.97273698, -50.71586442, -53.45899185,
0, 17.52339731, 22.83431312, 22.28119434, 19.42942821, 16.12729019, 12.82515217, 9.52301415, 6.220876131, 2.918738112, -0.383399907, -3.685537926, -6.987675944, -10.28981396, -13.59195198, -16.89409, -20.19622802, -23.49836604, -26.80050406, -30.10264208, -33.4047801, -36.70691812, -40.00905613, -43.31119415, -46.61333217, -49.91547019, -53.21760821, -56.51974623, -59.82188425, -63.12402227, -66.42616029,
0, 19.16205253, 24.81361243, 24.16372364, 20.832786, 16.83880802, 12.84483005, 8.850852067, 4.856874089, 0.86289611, -3.131081868, -7.125059847, -11.11903783, -15.1130158, -19.10699378, -23.10097176, -27.09494974, -31.08892772, -35.0829057, -39.07688367, -43.07086165, -47.06483963, -51.05881761, -55.05279559, -59.04677357, -63.04075154, -67.03472952, -71.0287075, -75.02268548, -79.01666346, -83.01064144,
0, 20.76573894, 26.71632228, 25.91001585, 21.98064545, 17.13107308, 12.2815007, 7.431928329, 2.582355956, -2.267216418, -7.116788791, -11.96636116, -16.81593354, -21.66550591, -26.51507828, -31.36465066, -36.21422303, -41.0637954, -45.91336778, -50.76294015, -55.61251253, -60.4620849, -65.31165727, -70.16122965, -75.01080202, -79.86037439, -84.70994677, -89.55951914, -94.40909151, -99.25866389, -104.1082363,
0, 22.32415296, 28.52241885, 27.48775015, 22.82018752, 16.92126088, 11.02233425, 5.123407614, -0.775519021, -6.674445656, -12.57337229, -18.47229893, -24.37122556, -30.2701522, -36.16907883, -42.06800547, -47.9669321, -53.86585874, -59.76478537, -65.66371201, -71.56263864, -77.46156528, -83.36049191, -89.25941855, -95.15834518, -101.0572718, -106.9561985, -112.8551251, -118.7540517, -124.6529784, -130.551905,
0, 23.83634467, 30.24305456, 28.91692362, 23.35975173, 16.18386692, 9.007982111, 1.8320973, -5.343787511, -12.51967232, -19.69555713, -26.87144195, -34.04732676, -41.22321157, -48.39909638, -55.57498119, -62.750866, -69.92675081, -77.10263563, -84.27852044, -91.45440525, -98.63029006, -105.8061749, -112.9820597, -120.1579445, -127.3338293, -134.5097141, -141.6855989, -148.8614837, -156.0373686, -163.2132534,
0, 25.31528428, 31.94304414, 30.31071531, 23.71470265, 14.98616653, 6.257630423, -2.470905688, -11.1994418, -19.92797791, -28.65651402, -37.38505013, -46.11358624, -54.84212235, -63.57065847, -72.29919458, -81.02773069, -89.7562668, -98.48480291, -107.213339, -115.9418751, -124.6704112, -133.3989474, -142.1274835, -150.8560196, -159.5845557, -168.3130918, -177.0416279, -185.770164, -194.4987001, -203.2272362,
0, 26.79136023, 33.77211812, 31.93448846, 24.49491769, 14.17882173, 3.537714169, -7.103393391, -17.74450095, -28.38560851, -39.02671607, -49.66782363, -60.3089312, -70.95003876, -81.59114632, -92.23225388, -102.8733614, -113.514469, -124.1555766, -134.7966841, -145.4377917, -156.0788992, -166.7200068, -177.3611144, -188.0022219, -198.6433295, -209.284437, -219.9255446, -230.5666522, -241.2077597, -251.8488673,
0, 28.2995076, 35.97330665, 34.23092253, 26.58399162, 14.90310883, 1.825702905, -11.25170302, -24.32910894, -37.40651487, -50.48392079, -63.56132672, -76.63873264, -89.71613857, -102.7935445, -115.8709504, -128.9483563, -142.0257623, -155.1031682, -168.1805741, -181.25798, -194.335386, -207.4127919, -220.4901978, -233.5676037, -246.6450097, -259.7224156, -272.7998215, -285.8772274, -298.9546334, -312.0320393,
0, 29.80080153, 38.75337275, 37.6335444, 30.3717679, 17.99522384, 2.383020038, -13.97931435, -30.34164873, -46.70398312, -63.0663175, -79.42865189, -95.79098627, -112.1533207, -128.515655, -144.8779894, -161.2403238, -177.6026582, -193.9649926, -210.327327, -226.6896614, -243.0519957, -259.4143301, -275.7766645, -292.1389989, -308.5013333, -324.8636677, -341.2260021, -357.5883364, -373.9506708, -390.3130052,
0, 30.9367213, 41.76583591, 41.84416649, 35.42196781, 24.10383586, 8.34547391, -11.18878744, -32.31628347, -53.44377949, -74.57127552, -95.69877154, -116.8262676, -137.9537636, -159.0812596, -180.2087556, -201.3362517, -222.4637477, -243.5912437, -264.7187397, -285.8462358, -306.9737318, -328.1012278, -349.2287239, -370.3562199, -391.4837159, -412.6112119, -433.738708, -454.866204, -475.9937, -497.121196,
0, 30.54265327, 42.96617975, 44.25580563, 38.40903117, 27.53158859, 12.61655183, -5.934592366, -27.99238591, -53.52754858, -81.44378739, -110.0041578, -138.5645282, -167.1248986, -195.685269, -224.2456394, -252.8060098, -281.3663802, -309.9267506, -338.487121, -367.0474914, -395.6078618, -424.1682322, -452.7286026, -481.288973, -509.8493434, -538.4097138, -566.9700842, -595.5304546, -624.090825, -652.6511954,
0, 26.08519492, 37.1409959, 37.54106854, 30.31427068, 17.49139075, 0.38259496, -20.20533549, -43.80333166, -70.157743, -99.14372443, -130.7071288, -164.8282938, -201.5017333, -240.72644, -281.3841722, -322.1993398, -363.0145074, -403.8296751, -444.6448427, -485.4600103, -526.2751779, -567.0903456, -607.9055132, -648.7206808, -689.5358484, -730.3510161, -771.1661837, -811.9813513, -852.7965189, -893.6116866,
0, 13.23473966, 14.95477404, 7.503543341, -7.262728272, -27.89112119, -53.25927003, -82.51277338, -115.0118043, -150.2860038, -187.9967835, -227.9062052, -269.8516499, -313.7255341, -359.4593757, -407.0115563, -456.3581787, -507.4864611, -560.3901618, -615.0665797, -671.5147249, -729.7343125, -789.725286, -850.997914, -912.6665064, -974.3350989, -1036.003691, -1097.672284, -1159.340876, -1221.009469, -1282.678061,
0, -14.78335982, -37.8794912, -68.16038892, -104.6514501, -146.5142548, -193.0308461, -243.5894212, -297.6713423, -354.8393861, -414.7271471, -477.0295163, -541.4941605, -607.9139264, -676.1201015, -745.9764623, -817.3740468, -890.2265878, -964.4665496, -1040.04171, -1116.912239, -1195.048213, -1274.427535, -1355.034191, -1436.85683, -1519.887602, -1604.121229, -1689.554283, -1776.18462, -1864.010964, -1953.032596,
0, -68.63373461, -143.0607251, -222.768502, -307.2896367, -396.1984673, -489.1080115, -585.6670583, -685.5574307, -788.4914136, -894.2093378, -1002.477315, -1113.085116, -1225.844187, -1340.585795, -1457.159301, -1575.430544, -1695.280351, -1816.603141, -1939.305637, -2063.305676, -2188.531104, -2314.918762, -2442.413553, -2570.967582, -2700.539373, -2831.093149, -2962.59818, -3095.028186, -3228.360801, -3362.577081,
0, 14.92295037, 22.03221278, 23.61721617, 22.88384194, 21.94774827, 21.0116546, 20.07556093, 19.13946727, 18.2033736, 17.26727993, 16.33118626, 15.39509259, 14.45899892, 13.52290525, 12.58681159, 11.65071792, 10.71462425, 9.77853058, 8.842436912, 7.906343243, 6.970249575, 6.034155906, 5.098062238, 4.161968569, 3.2258749, 2.289781232, 1.353687563, 0.417593895, -0.518499774, -1.454593442,
0, 16.46027769, 23.22488212, 24.21420062, 23.02220344, 21.58245506, 20.14270668, 18.7029583, 17.26320991, 15.82346153, 14.38371315, 12.94396476, 11.50421638, 10.064468, 8.624719615, 7.184971232, 5.745222849, 4.305474466, 2.865726083, 1.4259777, -0.013770683, -1.453519066, -2.893267448, -4.333015831, -5.772764214, -7.212512597, -8.65226098, -10.09200936, -11.53175775, -12.97150613, -14.41125451,
0, 18.27225364, 25.19740258, 25.99333204, 24.49048836, 22.62254692, 20.75460549, 18.88666405, 17.01872262, 15.15078119, 13.28283975, 11.41489832, 9.546956884, 7.67901545, 5.811074016, 3.943132582, 2.075191148, 0.207249714, -1.66069172, -3.528633154, -5.396574589, -7.264516023, -9.132457457, -11.00039889, -12.86834032, -14.73628176, -16.60422319, -18.47216463, -20.34010606, -22.2080475, -24.07598893,
0, 20.20386026, 27.49522054, 28.26270336, 26.49462705, 24.19795343, 21.90127981, 19.60460619, 17.30793256, 15.01125894, 12.71458532, 10.4179117, 8.121238075, 5.824564453, 3.527890831, 1.231217209, -1.065456414, -3.362130036, -5.658803658, -7.95547728, -10.2521509, -12.54882452, -14.84549815, -17.14217177, -19.43884539, -21.73551901, -24.03219264, -26.32886626, -28.62553988, -30.9222135, -33.21888712,
0, 22.18132865, 29.92445204, 30.72562445, 28.67271943, 25.8862277, 23.09973597, 20.31324424, 17.52675251, 14.74026078, 11.95376905, 9.16727732, 6.380785591, 3.594293861, 0.807802132, -1.978689598, -4.765181327, -7.551673056, -10.33816479, -13.12465652, -15.91114824, -18.69763997, -21.4841317, -24.27062343, -27.05711516, -29.84360689, -32.63009862, -35.41659035, -38.20308208, -40.98957381, -43.77606554,
0, 24.15568584, 32.36495969, 33.19605877, 30.78666771, 27.39756964, 24.00847156, 20.61937349, 17.23027542, 13.84117735, 10.45207928, 7.062981208, 3.673883136, 0.284785065, -3.104313006, -6.493411077, -9.882509149, -13.27160722, -16.66070529, -20.04980336, -23.43890143, -26.82799951, -30.21709758, -33.60619565, -36.99529372, -40.38439179, -43.77348986, -47.16258793, -50.551686, -53.94078408, -57.32988215,
0, 26.09005083, 34.73033362, 35.5389135, 32.65728978, 28.50703775, 24.35678572, 20.20653369, 16.05628166, 11.90602964, 7.755777608, 3.605525579, -0.544726449, -4.694978477, -8.845230506, -12.99548253, -17.14573456, -21.29598659, -25.44623862, -29.59649065, -33.74674268, -37.8969947, -42.04724673, -46.19749876, -50.34775079, -54.49800282, -58.64825485, -62.79850687, -66.9487589, -71.09901093, -75.24926296,
0, 27.95593434, 36.9569617, 37.6540745, 34.14678768, 29.03501947, 23.92325127, 18.81148307, 13.69971487, 8.587946665, 3.476178462, -1.63558974, -6.747357943, -11.85912614, -16.97089435, -22.08266255, -27.19443075, -32.30619895, -37.41796716, -42.52973536, -47.64150356, -52.75327176, -57.86503997, -62.97680817, -68.08857637, -73.20034457, -78.31211278, -83.42388098, -88.53564918, -93.64741738, -98.75918558,
0, 29.73335703, 39.0055051, 39.47953219, 35.16224329, 28.84820976, 22.53417623, 16.2201427, 9.906109178, 3.59207565, -2.721957877, -9.035991404, -15.35002493, -21.66405846, -27.97809199, -34.29212551, -40.60615904, -46.92019257, -53.23422609, -59.54825962, -65.86229315, -72.17632668, -78.4903602, -84.80439373, -91.11842726, -97.43246079, -103.7464943, -110.0605278, -116.3745614, -122.6885949, -129.0026284,
0, 31.41417131, 40.87386122, 41.01323732, 35.80489828, 28.12889826, 20.32801821, 12.52713816, 4.726258105, -3.074621948, -10.875502, -18.67638205, -26.47726211, -34.27814216, -42.07902221, -49.87990226, -57.68078232, -65.48166237, -73.28254242, -81.08342247, -88.88430253, -96.68518258, -104.4860626, -112.2869427, -120.0878227, -127.8887028, -135.6895828, -143.4904629, -151.2913429, -159.092223, -166.8931031,
0, 33.00870179, 42.62480469, 42.36017644, 36.30619536, 27.18444069, 17.55437174, 7.924302797, -1.70576615, -11.3358351, -20.96590404, -30.59597299, -40.22604194, -49.85611088, -59.48617983, -69.11624877, -78.74631772, -88.37638667, -98.00645561, -107.6365246, -117.2665935, -126.8966625, -136.5267314, -146.1568003, -155.7868693, -165.4169382, -175.0470072, -184.6770761, -194.3071451, -203.937214, -213.567283,
0, 34.55314866, 44.42737752, 43.80481628, 36.97448777, 26.24183216, 14.34659397, 2.451355778, -9.443882412, -21.3391206, -33.23435879, -45.12959698, -57.02483517, -68.92007336, -80.81531155, -92.71054974, -104.6057879, -116.5010261, -128.3962643, -140.2915025, -152.1867407, -164.0819789, -175.9772171, -187.8724553, -199.7676935, -211.6629316, -223.5581698, -235.453408, -247.3486462, -259.2438844, -271.1391226,
0, 36.1028543, 46.58322156, 45.86425509, 38.57035973, 26.11470864, 11.3453235, -3.424061652, -18.1934468, -32.96283195, -47.7322171, -62.50160224, -77.27098739, -92.04037254, -106.8097577, -121.5791428, -136.348528, -151.1179131, -165.8872983, -180.6566834, -195.4260686, -210.1954537, -224.9648389, -239.734224, -254.5036092, -269.2729943, -284.0423795, -298.8117646, -313.5811498, -328.3505349, -343.1199201,
0, 37.66015592, 49.42173205, 49.15096756, 41.94778954, 29.1987835, 12.19790417, -6.388510052, -24.97492428, -43.5613385, -62.14775273, -80.73416695, -99.32058118, -117.9069954, -136.4934096, -155.0798239, -173.6662381, -192.2526523, -210.8390665, -229.4254808, -248.011895, -266.5983092, -285.1847234, -303.7711377, -322.3575519, -340.9439661, -359.5303803, -378.1167946, -396.7032088, -415.289623, -433.8760372,
0, 38.92314387, 52.78741348, 53.68991723, 47.09523661, 35.07751771, 18.24302107, -3.291918622, -27.22530785, -51.21207241, -75.19883698, -99.18560154, -123.1723661, -147.1591307, -171.1458952, -195.1326598, -219.1194244, -243.1061889, -267.0929535, -291.079718, -315.0664826, -339.0532472, -363.0400117, -387.0267763, -411.0135409, -435.0003054, -458.98707, -482.9738346, -506.9605991, -530.9473637, -554.9341282,
0, 38.75779399, 54.80088615, 57.24886549, 51.25786034, 39.51611562, 23.28314688, 3.068665494, -20.96025797, -48.76394721, -79.69147302, -111.858079, -144.0246849, -176.1912909, -208.3578968, -240.5245028, -272.6911087, -304.8577147, -337.0243206, -369.1909266, -401.3575325, -433.5241385, -465.6907444, -497.8573504, -530.0239563, -562.1905623, -594.3571682, -626.5237742, -658.6903801, -690.8569861, -723.0235921,
0, 34.57116654, 50.17352811, 52.58438598, 45.75778283, 32.31326222, 13.92060699, -8.404078747, -34.07664959, -62.7843636, -94.37442434, -128.7807191, -165.979063, -205.9625244, -248.7297417, -293.4536094, -338.742318, -384.0310267, -429.3197353, -474.6084439, -519.8971526, -565.1858612, -610.4745698, -655.7632785, -701.0519871, -746.3406957, -791.6294044, -836.918113, -882.2068217, -927.4955303, -972.7842389,
0, 21.8922669, 29.1750076, 24.97262854, 11.73836711, -8.624818414, -34.66053157, -65.27067424, -99.64101054, -137.1790231, -177.4626191, -220.1983342, -265.1877747, -312.3011292, -361.4566711, -412.6052657, -465.7189847, -520.7830222, -577.7901945, -636.7373988, -697.6234905, -760.4481326, -825.2112542, -891.6465431, -958.7847635, -1025.922984, -1093.061204, -1160.199425, -1227.337645, -1294.475866, -1361.614086,
0, -6.180167086, -22.93476204, -48.752582, -82.33546793, -122.5732432, -168.5209819, -219.3784502, -274.4715736, -333.235783, -395.2011058, -459.9788674, -527.2498794, -596.7539922, -668.2809005, -741.6620904, -816.7638264, -893.4810805, -971.7323106, -1051.455002, -1132.601892, -1215.137793, -1299.03696, -1384.280917, -1470.856691, -1558.755399, -1647.971133, -1738.500089, -1830.339917, -1923.489226, -2017.947244,
0, -60.36353462, -128.0971087, -202.5136434, -282.9887163, -368.9557907, -459.9017334, -555.3626097, -654.9197408, -758.1960139, -864.8524309, -974.584886, -1087.121161, -1202.218125, -1319.659133, -1439.251609, -1560.824802, -1684.227716, -1809.32719, -1936.006131, -2064.161887, -2193.704748, -2324.55658, -2456.649562, -2589.925045, -2724.332507, -2859.828603, -2996.376308, -3133.944134, -3272.505439, -3412.03779,
0, 22.54837005, 33.63231317, 37.11673117, 37.00740211, 36.12857792, 35.24975374, 34.37092955, 33.49210536, 32.61328117, 31.73445698, 30.8556328, 29.97680861, 29.09798442, 28.21916023, 27.34033604, 26.46151186, 25.58268767, 24.70386348, 23.82503929, 22.9462151, 22.06739092, 21.18856673, 20.30974254, 19.43091835, 18.55209416, 17.67326998, 16.79444579, 15.9156216, 15.03679741, 14.15797322,
0, 24.28506135, 34.92085828, 37.66869486, 36.99004173, 35.53148119, 34.07292064, 32.6143601, 31.15579956, 29.69723902, 28.23867848, 26.78011794, 25.3215574, 23.86299685, 22.40443631, 20.94587577, 19.48731523, 18.02875469, 16.57019415, 15.11163361, 13.65307306, 12.19451252, 10.73595198, 9.277391439, 7.818830898, 6.360270356, 4.901709815, 3.443149273, 1.984588732, 0.52602819, -0.932532351,
0, 26.37672955, 37.19331366, 39.76048131, 38.77049444, 36.84423415, 34.91797386, 32.99171357, 31.06545328, 29.13919299, 27.2129327, 25.28667241, 23.36041212, 21.43415183, 19.50789154, 17.58163125, 15.65537096, 13.72911067, 11.80285038, 9.876590094, 7.950329804, 6.024069514, 4.097809224, 2.171548934, 0.245288644, -1.680971646, -3.607231935, -5.533492225, -7.459752515, -9.386012805, -11.3122731,
0, 28.62505112, 39.88619934, 42.50217987, 41.28992521, 38.91378046, 36.53763571, 34.16149096, 31.78534622, 29.40920147, 27.03305672, 24.65691197, 22.28076722, 19.90462248, 17.52847773, 15.15233298, 12.77618823, 10.40004349, 8.023898738, 5.64775399, 3.271609242, 0.895464494, -1.480680254, -3.856825001, -6.232969749, -8.609114497, -10.98525924, -13.36140399, -15.73754874, -18.11369349, -20.48983824,
0, 30.93330057, 42.75182325, 45.50745537, 44.09284038, 41.23335307, 38.34987219, 35.46639131, 32.58291042, 29.69942954, 26.81594866, 23.93246778, 21.0489869, 18.16550601, 15.28202513, 12.39854425, 9.515063367, 6.631582485, 3.748101603, 0.864620721, -2.018860161, -4.902341043, -7.785821926, -10.66930281, -13.55278369, -16.43626457, -19.31974545, -22.20322634, -25.08670722, -27.9701881, -30.85366898,
0, 33.23585778, 45.63299859, 48.53154518, 46.94591469, 43.58639777, 40.07348001, 36.56056225, 33.04764449, 29.53472673, 26.02180896, 22.5088912, 18.99597344, 15.48305568, 11.97013792, 8.457220158, 4.944302397, 1.431384636, -2.081533126, -5.594450887, -9.107368648, -12.62028641, -16.13320417, -19.64612193, -23.15903969, -26.67195745, -30.18487522, -33.69779298, -37.21071074, -40.7236285, -44.23654626,
0, 35.48213197, 48.41386364, 51.39382752, 49.54341982, 45.54259673, 41.22052422, 36.89845171, 32.5763792, 28.2543067, 23.93223419, 19.61016168, 15.28808917, 10.96601667, 6.643944159, 2.321871651, -2.000200856, -6.322273364, -10.64434587, -14.96641838, -19.28849089, -23.61056339, -27.9326359, -32.25470841, -36.57678092, -40.89885342, -45.22092593, -49.54299844, -53.86507095, -58.18714346, -62.50921596,
0, 37.63179929, 51.00610199, 53.95628099, 51.70136518, 46.86891464, 41.50504232, 36.14117, 30.77729768, 25.41342536, 20.04955303, 14.68568071, 9.321808392, 3.95793607, -1.405936251, -6.769808572, -12.13368089, -17.49755321, -22.86142554, -28.22529786, -33.58917018, -38.9530425, -44.31691482, -49.68078714, -55.04465946, -60.40853179, -65.77240411, -71.13627643, -76.50014875, -81.86402107, -87.22789339,
0, 39.65486215, 53.35022879, 56.1254815, 53.29433433, 47.39905757, 40.70978782, 34.02051806, 27.33124831, 20.64197855, 13.9527088, 7.263439041, 0.574169285, -6.11510047, -12.80437023, -19.49363998, -26.18290974, -32.87217949, -39.56144925, -46.250719, -52.93998876, -59.62925851, -66.31852827, -73.00779802, -79.69706778, -86.38633753, -93.07560729, -99.76487705, -106.4541468, -113.1434166, -119.8326863,
0, 41.53582426, 55.43066894, 57.87644115, 54.29046152, 47.07258641, 38.7203458, 30.36810519, 22.01586458, 13.66362397, 5.311383356, -3.040857255, -11.39309787, -19.74533848, -28.09757909, -36.4498197, -44.80206031, -53.15430092, -61.50654153, -69.85878214, -78.21102275, -86.56326336, -94.91550397, -103.2677446, -111.6199852, -119.9722258, -128.3244664, -136.676707, -145.0289476, -153.3811882, -161.7334289,
0, 43.28246353, 57.3087138, 59.30565868, 54.82803082, 46.01983575, 35.59948347, 25.17913118, 14.7587789, 4.338426614, -6.08192567, -16.50227795, -26.92263024, -37.34298252, -47.76333481, -58.18368709, -68.60403938, -79.02439166, -89.44474395, -99.86509623, -110.2854485, -120.7058008, -131.1261531, -141.5465054, -151.9668577, -162.3872099, -172.8075622, -183.2279145, -193.6482668, -204.0686191, -214.4889714,
0, 44.9372911, 59.17444157, 60.71704718, 55.33771255, 44.69346425, 31.69669303, 18.69992181, 5.703150587, -7.293620633, -20.29039185, -33.28716307, -46.2839343, -59.28070552, -72.27747674, -85.27424796, -98.27101918, -111.2677904, -124.2645616, -137.2613328, -150.2581041, -163.2548753, -176.2516465, -189.2484177, -202.2451889, -215.2419602, -228.2387314, -241.2355026, -254.2322738, -267.229045, -280.2258163,
0, 46.57733199, 61.39208321, 62.7050326, 56.64894414, 44.66151005, 29.0965536, 12.8327641, -3.431025389, -19.69481488, -35.95860437, -52.22239387, -68.48618336, -84.74997285, -101.0137623, -117.2775518, -133.5413413, -149.8051308, -166.0689203, -182.3327098, -198.5964993, -214.8602888, -231.1240783, -247.3878678, -263.6516573, -279.9154468, -296.1792362, -312.4430257, -328.7068152, -344.9706047, -361.2343942,
0, 48.2494165, 64.42339378, 66.0750238, 59.83875633, 47.64047503, 29.85201594, 9.287610035, -11.27679587, -31.84120178, -52.40560769, -72.97001359, -93.5344195, -114.0988254, -134.6632313, -155.2276372, -175.7920431, -196.356449, -216.9208549, -237.4852609, -258.0496668, -278.6140727, -299.1784786, -319.7428845, -340.3072904, -360.8716963, -381.4361022, -402.0005081, -422.564914, -443.1293199, -463.6937258,
0, 49.71658171, 68.32798838, 71.21815777, 65.38723232, 53.55287478, 36.55206751, 14.56465818, -10.94462224, -37.49260203, -64.04058181, -90.5885616, -117.1365414, -143.6845212, -170.232501, -196.7804807, -223.3284605, -249.8764403, -276.4244201, -302.9723999, -329.5203797, -356.0683595, -382.6163392, -409.164319, -435.7122988, -462.2602786, -488.8082584, -515.3562382, -541.904218, -568.4521977, -595.0001775,
0, 49.89106388, 71.43791422, 76.35961416, 71.2468623, 59.53009965, 42.82400461, 21.79238777, -3.339126428, -32.51271266, -65.71891527, -101.0397237, -136.4591714, -171.878619, -207.2980667, -242.7175143, -278.136962, -313.5564096, -348.9758573, -384.3953049, -419.8147526, -455.2342002, -490.6536479, -526.0730955, -561.4925432, -596.9119909, -632.3314385, -667.7508862, -703.1703338, -738.5897815, -774.0092291,
0, 46.13147409, 68.43252513, 74.42870716, 69.22465435, 56.17311756, 37.39416543, 14.16891535, -12.77002575, -43.03141274, -76.42368713, -112.863132, -152.3183852, -194.7800049, -240.2459443, -288.4050368, -337.7552467, -387.1054567, -436.4556666, -485.8058765, -535.1560864, -584.5062963, -633.8565062, -683.2067161, -732.556926, -781.9071359, -831.2573458, -880.6075557, -929.9577657, -979.3079756, -1028.658185,
0, 33.81173008, 49.16244877, 50.17032124, 40.04372654, 21.25165686, -4.331525786, -35.3041546, -70.63550834, -109.5817915, -151.6175671, -196.3805399, -243.6277601, -293.2014851, -345.0031038, -398.9736864, -455.0798821, -513.3040396, -573.6375719, -636.0767331, -700.6201107, -767.2672701, -836.018112, -906.8726223, -978.8927552, -1051.026677, -1123.160599, -1195.294521, -1267.428442, -1339.562364, -1411.696286,
0, 5.873599788, -1.65903528, -20.59178865, -49.21086839, -86.0589011, -129.9025678, -179.7035232, -234.5923507, -293.8453195, -356.8637188, -423.1555562, -492.3194215, -564.0303219, -638.027311, -714.1027416, -792.0929824, -871.8704492, -953.3368118, -1036.417245, -1121.055603, -1207.210406, -1294.851538, -1383.95755, -1474.513499, -1566.509229, -1659.938033, -1754.795624, -1851.079366, -1948.787712, -2047.919802,
0, -48.47431871, -106.2983583, -172.557785, -246.4246344, -327.1504364, -414.0597835, -506.5443175, -604.0571164, -706.1074611, -812.2559613, -922.110024, -1035.319644, -1151.573503, -1270.595355, -1392.140686, -1515.993631, -1641.964138, -1769.885349, -1899.611208, -2031.014263, -2163.983655, -2298.423289, -2434.250165, -2571.392859, -2709.790159, -2849.389817, -2990.147435, -3132.025463, -3274.992296, -3419.021473,
0, 33.44336129, 50.55211993, 57.13273429, 58.16428107, 57.38015976, 56.4309106, 55.48166143, 54.53241227, 53.5831631, 52.63391394, 51.68466477, 50.73541561, 49.78616644, 48.83691728, 47.88766811, 46.93841895, 45.98916978, 45.03992061, 44.09067145, 43.14142228, 42.19217312, 41.24292395, 40.29367479, 39.34442562, 38.39517646, 37.44592729, 36.49667813, 35.54742896, 34.5981798, 33.64893063,
0, 35.34781033, 51.82923416, 57.44770069, 57.85929877, 56.50801378, 54.89094861, 53.27388343, 51.65681826, 50.03975309, 48.42268791, 46.80562274, 45.18855756, 43.57149239, 41.95442722, 40.33736204, 38.72029687, 37.10323169, 35.48616652, 33.86910135, 32.25203617, 30.634971, 29.01790582, 27.40084065, 25.78377548, 24.1667103, 22.54964513, 20.93257995, 19.31551478, 17.69844961, 16.08138443,
0, 37.70105006, 54.33813422, 59.73360041, 59.94902652, 58.22821934, 56.0981803, 53.96814127, 51.83810223, 49.70806319, 47.57802416, 45.44798512, 43.31794608, 41.18790705, 39.05786801, 36.92782897, 34.79778993, 32.6677509, 30.53771186, 28.40767282, 26.27763379, 24.14759475, 22.01755571, 19.88751667, 17.75747764, 15.6274386, 13.49739956, 11.36736053, 9.237321488, 7.107282451, 4.977243414,
0, 40.25672464, 57.38736546, 62.87534871, 63.07751297, 61.06493242, 58.4635218, 55.86211119, 53.26070057, 50.65928996, 48.05787934, 45.45646872, 42.85505811, 40.25364749, 37.65223688, 35.05082626, 32.44941565, 29.84800503, 27.24659442, 24.6451838, 22.04377319, 19.44236257, 16.84095195, 14.23954134, 11.63813072, 9.036720108, 6.435309493, 3.833898878, 1.232488262, -1.368922353, -3.970332969,
0, 42.89333079, 60.66885431, 66.38492883, 66.6324736, 64.31474659, 61.19308635, 58.07142611, 54.94976587, 51.82810563, 48.70644538, 45.58478514, 42.4631249, 39.34146466, 36.21980442, 33.09814418, 29.97648394, 26.85482369, 23.73316345, 20.61150321, 17.48984297, 14.36818273, 11.24652249, 8.124862246, 5.003202004, 1.881541763, -1.240118479, -4.36177872, -7.483438962, -10.6050992, -13.72675944,
0, 45.52761636, 63.98510003, 69.95213647, 70.21450942, 67.50118416, 63.73245161, 59.96371905, 56.1949865, 52.42625395, 48.6575214, 44.88878885, 41.1200563, 37.35132375, 33.5825912, 29.81385865, 26.0451261, 22.27639355, 18.50766099, 14.73892844, 10.97019589, 7.201463342, 3.432730791, -0.33600176, -4.104734311, -7.873466862, -11.64219941, -15.41093196, -19.17966451, -22.94839707, -26.71712962,
0, 48.09465377, 67.18886047, 73.34666388, 73.52160862, 70.25452161, 65.64215356, 61.02978551, 56.41741746, 51.80504941, 47.19268137, 42.58031332, 37.96794527, 33.35557722, 28.74320917, 24.13084113, 19.51847308, 14.90610503, 10.29373698, 5.681368934, 1.069000886, -3.543367162, -8.15573521, -12.76810326, -17.38047131, -21.99283935, -26.6052074, -31.21757545, -35.8299435, -40.44231155, -45.05467959,
0, 50.54184952, 70.16590317, 76.3899546, 76.31636264, 72.27734029, 66.56052164, 60.84370299, 55.12688434, 49.41006569, 43.69324704, 37.97642839, 32.25960974, 26.54279109, 20.82597244, 15.1091538, 9.392335146, 3.675516496, -2.041302154, -7.758120803, -13.47493945, -19.1917581, -24.90857675, -30.6253954, -36.34221405, -42.0590327, -47.77585135, -53.49267, -59.20948865, -64.9263073, -70.64312595,
0, 52.82887535, 72.83592239, 78.95573144, 78.42842177, 73.34746476, 66.20368714, 59.05990953, 51.91613192, 44.7723543, 37.62857669, 30.48479907, 23.34102146, 16.19724384, 9.053466229, 1.909688614, -5.234089, -12.37786661, -19.52164423, -26.66542184, -33.80919946, -40.95297707, -48.09675469, -55.2405323, -62.38430992, -69.52808753, -76.67186515, -83.81564276, -90.95942037, -98.10319799, -105.2469756,
0, 54.93267081, 75.16979236, 80.99587656, 79.79009251, 73.41153736, 64.50876666, 55.55125406, 46.59374145, 37.63622884, 28.67871624, 19.72120363, 10.76369103, 1.806178423, -7.151334182, -16.10884679, -25.06635939, -34.023872, -42.9813846, -51.93889721, -60.89640981, -69.85392242, -78.81143502, -87.76894763, -96.72646024, -105.6839728, -114.6414854, -123.5989981, -132.5565107, -141.5140233, -150.4715359,
0, 56.85835683, 77.22809838, 82.59979144, 80.51731083, 72.83065153, 62.0266449, 50.79111996, 39.55559503, 28.3200701, 17.08454516, 5.84902023, -5.386504703, -16.62202964, -27.85755457, -39.0930795, -50.32860443, -61.56412937, -72.7996543, -84.03517923, -95.27070417, -106.5062291, -117.741754, -128.977279, -140.2128039, -151.4483288, -162.6838538, -173.9193787, -185.1549036, -196.3904286, -207.6259535,
0, 58.65480628, 79.22400602, 84.09432842, 81.04495086, 72.00752759, 59.01095707, 44.92033583, 30.82971458, 16.73909333, 2.648472079, -11.44214917, -25.53277042, -39.62339167, -53.71401292, -67.80463417, -81.89525541, -95.98587666, -110.0764979, -124.1671192, -138.2577404, -152.3483617, -166.4389829, -180.5296042, -194.6202254, -208.7108467, -222.8014679, -236.8920892, -250.9827104, -265.0733317, -279.1639529,
0, 60.42113959, 81.58822447, 86.15784064, 82.27273519, 72.11981118, 56.70401566, 38.98952832, 21.27504099, 3.560553649, -14.15393369, -31.86842102, -49.58290836, -67.2973957, -85.01188304, -102.7263704, -120.4408577, -138.155345, -155.8698324, -173.5843197, -191.2988071, -209.0132944, -226.7277817, -244.4422691, -262.1567564, -279.8712437, -297.5857311, -315.3002184, -333.0147058, -350.7291931, -368.4436804,
0, 62.250402, 84.92230151, 89.80230867, 85.51409658, 74.79289639, 58.25525695, 37.32146916, 14.86216414, -7.597140878, -30.0564459, -52.51575092, -74.97505593, -97.43436095, -119.893666, -142.352971, -164.812276, -187.271581, -209.730886, -232.1901911, -254.6494961, -277.1088011, -299.5681061, -322.0274111, -344.4867162, -366.9460212, -389.4053262, -411.8646312, -434.3239362, -456.7832413, -479.2425463,
0, 63.97729145, 89.51943213, 95.81829568, 91.79535831, 81.05653616, 64.79802575, 43.31528091, 16.78676469, -12.1966691, -41.18010289, -70.16353667, -99.14697046, -128.1304042, -157.113838, -186.0972718, -215.0807056, -244.0641394, -273.0475732, -302.031007, -331.0144408, -359.9978745, -388.9813083, -417.9647421, -446.9481759, -475.9316097, -504.9150435, -533.8984773, -562.8819111, -591.8653448, -620.8487786,
0, 64.57783727, 93.96803998, 102.9601596, 99.89645176, 89.1560043, 72.83576683, 51.82541735, 26.44480734, -3.215396789, -37.13730222, -74.41353746, -112.9141871, -151.4148368, -189.9154864, -228.4161361, -266.9167857, -305.4174354, -343.918085, -382.4187347, -420.9193843, -459.420034, -497.9206837, -536.4213333, -574.921983, -613.4226326, -651.9232823, -690.4239319, -728.9245816, -767.4252312, -805.9258809,
0, 61.38232262, 92.97672371, 104.4321668, 102.2602997, 90.7202764, 72.4978291, 49.21460579, 21.79992078, -9.246696986, -43.67742841, -81.38136918, -122.3152318, -166.4650279, -213.8273201, -264.4015557, -317.0871736, -370.2782234, -423.4692732, -476.6603229, -529.8513727, -583.0424224, -636.2334722, -689.424522, -742.6155717, -795.8066215, -848.9976712, -902.188721, -955.3797708, -1008.570821, -1061.76187,
0, 49.60418111, 75.97926179, 84.48437848, 79.26845486, 63.5042062, 39.58519241, 9.290318111, -26.08015695, -65.59335951, -108.5938741, -154.6320277, -203.4075657, -254.7261827, -308.4666442, -364.5564969, -422.9546181, -483.6390884, -546.5991013, -611.829833, -679.329393, -749.0971607, -821.1329777, -895.4368164, -971.5841696, -1048.441028, -1125.297886, -1202.154744, -1279.011602, -1355.86846, -1432.725318,
0, 21.99519442, 27.04866591, 17.79749193, -3.516821629, -34.99785443, -75.04903656, -122.3332493, -175.7368685, -234.337879, -297.3777087, -364.2364513, -434.411167, -507.4969678, -583.1706156, -661.1763757, -741.3138893, -823.4278417, -907.3992237, -993.1379953, -1080.576978, -1169.666819, -1260.371873, -1352.666886, -1446.534348, -1541.962404, -1638.943254, -1737.471925, -1837.545361, -1939.161765, -2042.320136,
0, -32.33972276, -76.51306949, -131.3123998, -195.6479267, -268.5379436, -349.0997132, -436.5409849, -530.1521058, -629.298695, -733.4148492, -841.9968505, -954.5973474, -1070.819982, -1190.314435, -1312.77187, -1437.920741, -1565.522955, -1695.370349, -1827.281482, -1961.098703, -2096.685483, -2233.923999, -2372.712936, -2512.965512, -2654.607689, -2797.576571, -2941.818964, -3087.290095, -3233.952462, -3381.774824,
0, 48.51998336, 74.55708116, 86.1286091, 89.58972584, 89.37385652, 88.14871582, 86.90582536, 85.6629349, 84.42004443, 83.17715397, 81.93426351, 80.69137304, 79.44848258, 78.20559212, 76.96270165, 75.71981119, 74.47692073, 73.23403026, 71.9911398, 70.74824933, 69.50535887, 68.26246841, 67.01957794, 65.77668748, 64.53379702, 63.29090655, 62.04801609, 60.80512563, 59.56223516, 58.3193447,
0, 50.5609014, 75.71140087, 85.96961994, 88.59348323, 87.78044301, 85.84669843, 83.82910909, 81.81151974, 79.79393039, 77.77634105, 75.7587517, 73.74116236, 71.72357301, 69.70598367, 67.68839432, 65.67080497, 63.65321563, 61.63562628, 59.61803694, 57.60044759, 55.58285824, 53.5652689, 51.54767955, 49.53009021, 47.51250086, 45.49491151, 43.47732217, 41.45973282, 39.44214348, 37.42455413,
0, 53.15505798, 78.38126084, 88.28484466, 90.71544388, 89.65792553, 87.24992523, 84.6612499, 82.07257457, 79.48389924, 76.8952239, 74.30654857, 71.71787324, 69.12919791, 66.54052258, 63.95184725, 61.36317192, 58.77449659, 56.18582126, 53.59714593, 51.0084706, 48.41979527, 45.83111994, 43.24244461, 40.65376928, 38.06509395, 35.47641862, 32.88774329, 30.29906795, 27.71039262, 25.12171729,
0, 56.00468313, 81.73228454, 91.70453851, 94.22258991, 93.08283219, 90.30188512, 87.21271438, 84.12354364, 81.0343729, 77.94520216, 74.85603142, 71.76686068, 68.67768994, 65.5885192, 62.49934846, 59.41017772, 56.32100698, 53.23183624, 50.1426655, 47.05349476, 43.96432402, 40.87515328, 37.78598254, 34.6968118, 31.60764105, 28.51847031, 25.42929957, 22.34012883, 19.25095809, 16.16178735,
0, 58.96216904, 85.39085322, 95.62836344, 98.34349114, 97.1639968, 94.005702, 90.38046054, 86.75521908, 83.12997762, 79.50473616, 75.87949471, 72.25425325, 68.62901179, 65.00377033, 61.37852888, 57.75328742, 54.12804596, 50.5028045, 46.87756305, 43.25232159, 39.62708013, 36.00183867, 32.37659721, 28.75135576, 25.1261143, 21.50087284, 17.87563138, 14.25038993, 10.62514847, 6.99990701,
0, 61.92589991, 89.11659962, 99.67511414, 102.5808075, 101.3093941, 97.67874409, 93.39028244, 89.10182079, 84.81335914, 80.52489749, 76.23643584, 71.94797419, 67.65951254, 63.37105089, 59.08258924, 54.79412759, 50.50566594, 46.21720429, 41.92874264, 37.64028098, 33.35181933, 29.06335768, 24.77489603, 20.48643438, 16.19797273, 11.90951108, 7.621049431, 3.332587781, -0.95587387, -5.24433552,
0, 64.81612863, 92.7293881, 103.5613782, 106.5603409, 105.0644674, 100.7861924, 95.62524287, 90.46429333, 85.3033438, 80.14239426, 74.98144472, 69.82049519, 64.65954565, 59.49859611, 54.33764657, 49.17669704, 44.0157475, 38.85479796, 33.69384842, 28.53289889, 23.37194935, 18.21099981, 13.05005028, 7.889100738, 2.728151201, -2.432798337, -7.593747874, -12.75469741, -17.91564695, -23.07659649,
0, 67.56762439, 96.08809023, 107.065901, 109.9867107, 108.065295, 102.8924351, 96.57395027, 90.25546543, 83.9369806, 77.61849576, 71.30001092, 64.98152608, 58.66304124, 52.3445564, 46.02607156, 39.70758672, 33.38910189, 27.07061705, 20.75213221, 14.43364737, 8.115162531, 1.796677692, -4.521807147, -10.84029199, -17.15877682, -23.47726166, -29.7957465, -36.11423134, -42.43271618, -48.75120102,
0, 70.12939127, 99.09096879, 110.0284983, 112.642254, 110.040571, 103.6636828, 95.83001362, 87.99634447, 80.16267532, 72.32900616, 64.49533701, 56.66166786, 48.8279987, 40.99432955, 33.1606604, 25.32699124, 17.49332209, 9.659652938, 1.825983785, -6.007685369, -13.84135452, -21.67502367, -29.50869283, -37.34236198, -45.17603113, -53.00970029, -60.84336944, -68.67703859, -76.51070775, -84.3443769,
0, 72.47040992, 101.6951321, 112.3783421, 114.4234094, 110.8591518, 102.9194096, 93.13820579, 83.35700198, 73.57579817, 63.79459437, 54.01339056, 44.23218675, 34.45098294, 24.66977913, 14.88857532, 5.107371506, -4.673832304, -14.45503611, -24.23623992, -34.01744373, -43.79864754, -53.57985135, -63.36105516, -73.14225897, -82.92346278, -92.70466659, -102.4858704, -112.2670742, -122.048278, -131.8294818,
0, 74.59257643, 103.9608322, 114.2004779, 115.4268244, 110.6404447, 100.7512094, 88.50256309, 76.25391681, 64.00527053, 51.75662425, 39.50797796, 27.25933168, 15.0106854, 2.762039117, -9.486607166, -21.73525345, -33.98389973, -46.23254601, -58.4811923, -70.72983858, -82.97848486, -95.22713114, -107.4757774, -119.7244237, -131.97307, -144.2217163, -156.4703626, -168.7190088, -180.9676551, -193.2163014,
0, 76.55025115, 106.1255626, 115.8508188, 116.0986436, 109.9433221, 97.9908426, 82.90021263, 67.54166221, 52.18311179, 36.82456138, 21.46601096, 6.107460541, -9.251089877, -24.60964029, -39.96819071, -55.32674113, -70.68529155, -86.04384197, -101.4023924, -116.7609428, -132.1194932, -147.4780436, -162.8365941, -178.1951445, -193.5536949, -208.9122453, -224.2707957, -239.6293461, -254.9878966, -270.346447,
0, 78.46345062, 108.6886767, 118.1013942, 117.4202249, 109.9855912, 96.53110999, 78.66436125, 59.35227075, 40.04018025, 20.72808975, 1.415999255, -17.89609124, -37.20818174, -56.52027224, -75.83236274, -95.14445323, -114.4565437, -133.7686342, -153.0807247, -172.3928152, -191.7049057, -211.0169962, -230.3290867, -249.6411772, -268.9532677, -288.2653582, -307.5774487, -326.8895392, -346.2016297, -365.5137202,
0, 80.47073669, 112.3948013, 122.1830339, 120.9582749, 112.6382308, 98.28448552, 78.08772707, 54.29734556, 29.82662936, 5.355913166, -19.11480303, -43.58551923, -68.05623542, -92.52695162, -116.9976678, -141.468384, -165.9391002, -190.4098164, -214.8805326, -239.3512488, -263.821965, -288.2926812, -312.7633974, -337.2341136, -361.7048298, -386.175546, -410.6462622, -435.1169784, -459.5876946, -484.0584108,
0, 82.48253832, 117.781223, 129.3051277, 128.3208116, 119.6430155, 105.0212806, 84.95980882, 59.56085242, 29.77886858, -1.722995192, -33.22485896, -64.72672273, -96.22858649, -127.7304503, -159.232314, -190.7341778, -222.2360416, -253.7379053, -285.2397691, -316.7416329, -348.2434966, -379.7453604, -411.2472242, -442.7490879, -474.2509517, -505.7528155, -537.2546792, -568.756543, -600.2584068, -631.7602705,
0, 83.55690905, 123.7328316, 138.803167, 139.1993711, 130.5019638, 115.463379, 95.30638467, 70.50370467, 41.2061277, 7.450020515, -30.75893509, -71.79063097, -113.4188493, -155.0470677, -196.6752861, -238.3035044, -279.9317228, -321.5599412, -363.1881595, -404.8163779, -446.4445962, -488.0728146, -529.701033, -571.3292513, -612.9574697, -654.5856881, -696.2139064, -737.8421248, -779.4703432, -821.0985615,
0, 81.02597594, 125.0669425, 144.2666266, 146.816166, 138.0790868, 121.4484823, 98.98578286, 71.88448493, 40.7964719, 6.053129659, -32.19165883, -73.87434185, -118.9724882, -167.4797402, -219.394793, -274.7174862, -331.7500597, -388.7886973, -445.8273349, -502.8659725, -559.9046101, -616.9432478, -673.9818854, -731.020523, -788.0591606, -845.0977982, -902.1364358, -959.1750734, -1016.213711, -1073.252349,
0, 69.95110075, 110.8419562, 129.5414495, 131.3442354, 120.2823813, 99.38590555, 70.89869492, 36.45572383, -2.773034332, -45.96712748, -92.56418749, -142.1886115, -194.596879, -249.6364385, -307.2154915, -367.2813677, -429.8055296, -494.7735626, -562.178797, -632.0184746, -704.2916127, -778.9979257, -856.1373519, -935.7098828, -1016.876411, -1098.420384, -1179.964357, -1261.50833, -1343.052303, -1424.596276,
0, 42.8630023, 64.41248013, 68.07436372, 56.74819906, 32.8765487, -1.493162148, -44.653715, -95.18948187, -151.9335585, -213.9301174, -280.4014854, -350.7194822, -424.3805907, -500.9845563, -580.2160461, -661.8290246, -745.6335286, -831.4845538, -919.272786, -1008.916935, -1100.357454, -1193.551445, -1288.468569, -1385.08781, -1483.394949, -1583.380614, -1685.038811, -1788.365844, -1893.359518, -2000.018595,
0, -11.27571326, -37.47946559, -77.03043779, -128.5066566, -190.6313245, -262.2601226, -342.3694313, -430.0454181, -524.4739416, -624.9312235, -730.7752443, -841.4378169, -956.4172972, -1075.271891, -1197.613521, -1323.102209, -1451.440957, -1582.371066, -1715.667893, -1851.136986, -1988.610592, -2127.944498, -2269.015182, -2411.717252, -2555.961152, -2701.671102, -2848.783273, -2997.244152, -3147.009101, -3298.041085,
0, 68.79514633, 107.7112338, 127.0999713, 134.8939302, 136.6484175, 135.6105505, 133.7054035, 131.8002565, 129.8951094, 127.9899624, 126.0848153, 124.1796683, 122.2745213, 120.3693742, 118.4642272, 116.5590801, 114.6539331, 112.7487861, 110.843639, 108.938492, 107.0333449, 105.1281979, 103.2230509, 101.3179038, 99.41275678, 97.50760974, 95.6024627, 93.69731566, 91.79216862, 89.88702158,
0, 70.94267638, 108.6354403, 126.2089131, 132.7306108, 133.7672515, 132.0582296, 129.2999476, 126.4872729, 123.6745982, 120.8619235, 118.0492489, 115.2365742, 112.4238995, 109.6112248, 106.7985501, 103.9858755, 101.1732008, 98.3605261, 95.54785142, 92.73517674, 89.92250206, 87.10982738, 84.2971527, 81.48447802, 78.67180334, 75.85912867, 73.04645399, 70.23377931, 67.42110463, 64.60842995,
0, 73.7559433, 111.3866566, 128.3578751, 134.535894, 135.4153379, 133.3955172, 130.0770044, 126.6149266, 123.1528488, 119.6907709, 116.2286931, 112.7666153, 109.3045375, 105.8424596, 102.3803818, 98.91830396, 95.45622613, 91.9941483, 88.53207047, 85.06999264, 81.6079148, 78.14583697, 74.68375914, 71.22168131, 67.75960348, 64.29752565, 60.83544782, 57.37336999, 53.91129216, 50.44921433,
0, 76.88361204, 114.9764998, 131.8960073, 138.1475741, 139.1251488, 136.9671683, 133.2201195, 129.213223, 125.2063266, 121.1994301, 117.1925336, 113.1856372, 109.1787407, 105.1718443, 101.1649478, 97.15805135, 93.15115489, 89.14425843, 85.13736197, 81.13046551, 77.12356905, 73.11667259, 69.10977613, 65.10287967, 61.09598322, 57.08908676, 53.0821903, 49.07529384, 45.06839738, 41.06150092,
0, 80.15107187, 118.9619685, 136.1017805, 142.6148925, 143.7823004, 141.5313386, 137.3655764, 132.79614, 128.2267035, 123.6572671, 119.0878307, 114.5183943, 109.9489579, 105.3795214, 100.810085, 96.2406486, 91.67121218, 87.10177576, 82.53233934, 77.96290292, 73.3934665, 68.82403008, 64.25459365, 59.68515723, 55.11572081, 50.54628439, 45.97684797, 41.40741155, 36.83797513, 32.26853871,
0, 83.43782121, 123.0579434, 140.51867, 147.3319384, 148.6607964, 146.2565716, 141.577275, 136.3220522, 131.0668294, 125.8116065, 120.5563837, 115.3011609, 110.0459381, 104.7907153, 99.53549245, 94.28026963, 89.02504681, 83.76982399, 78.51460117, 73.25937835, 68.00415553, 62.74893271, 57.4937099, 52.23848708, 46.98326426, 41.72804144, 36.47281862, 31.2175958, 25.96237298, 20.70715016,
0, 86.64892398, 127.0502683, 144.8076228, 151.8458542, 153.2092497, 150.4998195, 145.1191578, 138.9601614, 132.8011651, 126.6421687, 120.4831724, 114.324176, 108.1651797, 102.0061833, 95.84718698, 89.68819063, 83.52919428, 77.37019792, 71.21120157, 65.05220522, 58.89320887, 52.73421251, 46.57521616, 40.41621981, 34.25722345, 28.0982271, 21.93923075, 15.7802344, 9.621238045, 3.462241692,
0, 89.70618555, 130.7701256, 148.7031166, 155.7992548, 156.9868432, 153.7405811, 147.3871979, 140.0189379, 132.6506779, 125.2824179, 117.9141579, 110.545898, 103.177638, 95.80937796, 88.44111797, 81.07285797, 73.70459798, 66.33633798, 58.96807799, 51.59981799, 44.231558, 36.863298, 29.49503801, 22.12677801, 14.75851802, 7.390258024, 0.021998029, -7.346261966, -14.71452196, -22.08278196,
0, 92.547545, 134.0936731, 152.0103492, 158.925292, 159.6591799, 155.5802334, 147.9092353, 138.9425503, 129.9758653, 121.0091803, 112.0424953, 103.0758102, 94.10912524, 85.14244023, 76.17575521, 67.2090702, 58.24238519, 49.27570018, 40.30901517, 31.34233016, 22.37564515, 13.40896014, 4.442275125, -4.524409887, -13.4910949, -22.45777991, -31.42446492, -40.39114993, -49.35783494, -58.32451995,
0, 95.13337494, 136.9635532, 154.6363522, 161.085596, 161.0447542, 155.8003021, 146.4072478, 135.367705, 124.3281621, 113.2886192, 102.2490764, 91.20953348, 80.16999061, 69.13044774, 58.09090487, 47.051362, 36.01181913, 24.97227626, 13.93273339, 2.893190521, -8.146352349, -19.18589522, -30.22543809, -41.26498096, -52.30452383, -63.3440667, -74.38360957, -85.42315244, -96.46269531, -107.5022382,
0, 97.46114653, 139.4388436, 156.6646792, 162.3633448, 161.2281314, 154.5013333, 142.9452321, 129.2606673, 115.5761024, 101.8915376, 88.2069727, 74.52240784, 60.83784299, 47.15327813, 33.46871327, 19.78414841, 6.099583554, -7.584981304, -21.26954616, -34.95411102, -48.63867588, -62.32324074, -76.00780559, -89.69237045, -103.3769353, -117.0615002, -130.746065, -144.4306299, -158.1151947, -171.7997596,
0, 99.58853856, 141.7802285, 158.4867757, 163.2288614, 160.7602612, 152.3460792, 138.2017798, 121.1840731, 104.1499181, 87.11576316, 70.08160818, 53.0474532, 36.01329822, 18.97914324, 1.944988257, -15.08916672, -32.1233217, -49.15747668, -66.19163167, -83.22578665, -100.2599416, -117.2940966, -134.3282516, -151.3624066, -168.3965615, -185.4307165, -202.4648715, -219.4990265, -236.5331815, -253.5673364,
0, 101.6530088, 144.5538249, 160.9782852, 164.7652078, 160.9216016, 150.8629284, 134.8622975, 114.7336566, 93.43458941, 72.13552217, 50.83645493, 29.53738769, 8.238320446, -13.06074679, -34.35981404, -55.65888128, -76.95794852, -98.25701576, -119.556083, -140.8551502, -162.1542175, -183.4532847, -204.752352, -226.0514192, -247.3504864, -268.6495537, -289.9486209, -311.2476882, -332.5467554, -353.8458226,
0, 103.8346734, 148.6442718, 165.5978087, 168.8129676, 163.8532911, 152.5243885, 135.2668147, 112.1491878, 85.70150587, 58.85140506, 32.00130425, 5.151203443, -21.69889737, -48.54899818, -75.39909899, -102.2491998, -129.0993006, -155.9494014, -182.7995022, -209.649603, -236.4997038, -263.3498047, -290.1999055, -317.0500063, -343.9001071, -370.7502079, -397.6003087, -424.4504095, -451.3005103, -478.1506111,
0, 106.1201309, 154.8343446, 173.9789061, 177.5775505, 172.0380792, 159.9352592, 142.1370346, 118.8709257, 90.17761846, 57.29318043, 22.92933802, -11.4345044, -45.79834682, -80.16218923, -114.5260316, -148.8898741, -183.2537165, -217.6175589, -251.9814013, -286.3452437, -320.7090861, -355.0729286, -389.436771, -423.8006134, -458.1644558, -492.5282982, -526.8921406, -561.2559831, -595.6198255, -629.9836679,
0, 107.6666932, 162.3385079, 186.0679451, 191.7013243, 186.3097755, 173.523809, 155.0514395, 131.6106043, 103.4610361, 70.67905752, 33.28122955, -8.730210003, -53.42527678, -98.49723016, -143.5691835, -188.6411369, -233.7130903, -278.7850437, -323.8569971, -368.9289505, -414.0009038, -459.0728572, -504.1448106, -549.216764, -594.2887174, -639.3606707, -684.4326241, -729.5045775, -774.5765309, -819.6484843,
0, 105.8475121, 166.1774911, 195.9519289, 205.3081818, 200.9284848, 187.0804671, 166.3913373, 140.4100575, 110.0040404, 75.62966946, 37.50928075, -4.258839893, -49.63639209, -98.61077532, -151.178719, -207.3396252, -266.9043605, -328.0764737, -389.2485869, -450.4207, -511.5928132, -572.7649264, -633.9370396, -695.1091528, -756.2812659, -817.4533791, -878.6254923, -939.7976055, -1000.969719, -1062.141832,
0, 95.59844301, 155.1234526, 187.2237741, 198.5525724, 194.1682368, 177.8676813, 152.4611275, 119.9965046, 81.94080075, 39.32493387, -7.142003594, -56.98394663, -109.8899573, -165.6632605, -224.1837985, -285.3813908, -349.2170439, -415.6703781, -484.7315109, -556.39608, -630.6623784, -707.529837, -786.9983034, -869.067748, -953.7381675, -1039.965549, -1126.449407, -1212.933264, -1299.417121, -1385.900978,
0, 69.20666782, 111.7689167, 132.0752981, 133.8276578, 120.1340545, 93.59117688, 56.3571984, 10.21594957, -43.36677121, -103.193984, -168.2946997, -237.8868202, -311.3451548, -388.1739925, -467.983711, -550.4709481, -635.4019017, -722.598363, -811.9261236, -903.2854326, -996.6032124, -1091.82677, -1188.918774, -1287.85329, -1388.612691, -1491.185287, -1595.563545, -1701.742758, -1809.720098, -1919.493933,
0, 15.44655027, 12.1535681, -7.83202722, -42.67425607, -90.72932573, -150.5282707, -220.7609036, -300.2609981, -387.9926277, -483.0375899, -584.5838452, -691.914907, -804.4001191, -921.4857623, -1042.686932, -1167.580138, -1295.796565, -1427.015961, -1560.961094, -1697.392743, -1836.105177, -1976.922084, -2119.692923, -2264.289647, -2410.603787, -2558.543844, -2708.032982, -2859.006981, -3011.412434, -3165.205155,
0, 95.35463914, 152.2942178, 183.4701483, 198.4900176, 204.1907617, 204.9542336, 203.0931459, 200.1053961, 196.9567973, 193.8081985, 190.6595997, 187.511001, 184.3624022, 181.2138034, 178.0652046, 174.9166058, 171.7680071, 168.6194083, 165.4708095, 162.3222107, 159.1736119, 156.0250132, 152.8764144, 149.7278156, 146.5792168, 143.430618, 140.2820193, 137.1334205, 133.9848217, 130.8362229,
0, 97.58121926, 152.8946225, 181.5986075, 194.6713133, 199.2106166, 199.256911, 196.6478766, 192.6876825, 188.4653005, 184.2429185, 180.0205365, 175.7981545, 171.5757726, 167.3533906, 163.1310086, 158.9086266, 154.6862446, 150.4638626, 146.2414807, 142.0190987, 137.7967167, 133.5743347, 129.3519527, 125.1295707, 120.9071887, 116.6848068, 112.4624248, 108.2400428, 104.0176608, 99.79527883,
0, 100.5917651, 155.6528916, 183.3807931, 195.7689736, 200.0246773, 199.9034202, 196.9402904, 192.349044, 187.3710623, 182.3930806, 177.4150989, 172.4371172, 167.4591355, 162.4811538, 157.5031721, 152.5251904, 147.5472087, 142.569227, 137.5912453, 132.6132636, 127.6352819, 122.6573002, 117.6793185, 112.7013368, 107.7233551, 102.7453734, 97.76739166, 92.78940996, 87.81142825, 82.83344655,
0, 103.980389, 159.419529, 186.8639917, 199.1515074, 203.5292548, 203.5034452, 200.3872605, 195.3319259, 189.7417599, 184.151594, 178.5614281, 172.9712621, 167.3810962, 161.7909302, 156.2007643, 150.6105983, 145.0204324, 139.4302664, 133.8401005, 128.2499345, 122.6597686, 117.0696026, 111.4794367, 105.8892707, 100.2991048, 94.70893886, 89.11877291, 83.52860696, 77.93844102, 72.34827507,
0, 107.5449649, 163.6796098, 191.1987583, 203.6747737, 208.3531579, 208.5173072, 205.3073379, 199.8166457, 193.618826, 187.4210064, 181.2231867, 175.025367, 168.8275474, 162.6297277, 156.4319081, 150.2340884, 144.0362687, 137.8384491, 131.6406294, 125.4428097, 119.2449901, 113.0471704, 106.8493508, 100.6515311, 94.45371144, 88.25589177, 82.05807211, 75.86025245, 69.66243279, 63.46461312,
0, 111.1457617, 168.1019818, 195.850172, 208.617257, 213.6193249, 213.9384806, 210.575151, 204.5590659, 197.6383364, 190.717607, 183.7968775, 176.8761481, 169.9554186, 163.0346891, 156.1139597, 149.1932302, 142.2725007, 135.3517713, 128.4310418, 121.5103124, 114.5895829, 107.6688534, 100.748124, 93.82739452, 86.90666506, 79.9859356, 73.06520614, 66.14447668, 59.22374721, 52.30301775,
0, 114.672376, 172.4376609, 200.4218325, 213.4432386, 218.6693359, 218.9995008, 215.3190669, 208.5817536, 200.7152585, 192.8487635, 184.9822685, 177.1157734, 169.2492784, 161.3827833, 153.5162883, 145.6497932, 137.7832982, 129.9168032, 122.0503081, 114.1838131, 106.317318, 98.450823, 90.58432796, 82.71783292, 74.85133788, 66.98484284, 59.1183478, 51.25185276, 43.38535772, 35.51886268,
0, 118.0333506, 176.4894623, 204.6028859, 217.7307545, 222.9791971, 223.0821724, 218.8293759, 211.0795528, 201.9450213, 192.8104898, 183.6759583, 174.5414269, 165.4068954, 156.2723639, 147.1378324, 138.0033009, 128.8687694, 119.7342379, 110.5997064, 101.4651749, 92.33064343, 83.19611193, 74.06158044, 64.92704895, 55.79251746, 46.65798597, 37.52345447, 28.38892298, 19.25439149, 10.11986,
0, 121.1550995, 180.1105863, 208.1632836, 221.1628047, 226.1485698, 225.7078516, 220.5518506, 211.413331, 200.5935953, 189.7738597, 178.954124, 168.1343884, 157.3146528, 146.4949171, 135.6751815, 124.8554458, 114.0357102, 103.2159746, 92.39623891, 81.57650328, 70.75676764, 59.937032, 49.11729636, 38.29756072, 27.47782508, 16.65808944, 5.838353798, -4.981381842, -15.80111748, -26.62085312,
0, 123.9884921, 183.2277657, 210.9878909, 223.5678657, 227.9474792, 226.5927778, 220.1546201, 209.1805196, 196.1624386, 183.1443576, 170.1262767, 157.1081957, 144.0901147, 131.0720338, 118.0539528, 105.0358719, 92.01779089, 78.99970993, 65.98162896, 52.963548, 39.94546703, 26.92738607, 13.90930511, 0.891224144, -12.12685682, -25.14493778, -38.16301875, -51.18109971, -64.19918067, -77.21726164,
0, 126.5247501, 185.8962289, 213.1599706, 225.0222097, 228.4355519, 225.7875383, 217.6930571, 204.3886049, 188.5510186, 172.7134324, 156.8758461, 141.0382599, 125.2006736, 109.3630874, 93.52550115, 77.6879149, 61.85032865, 46.0127424, 30.17515615, 14.3375699, -1.500016353, -17.3376026, -33.17518885, -49.0127751, -64.85036135, -80.6879476, -96.52553385, -112.3631201, -128.2007064, -144.0382926,
0, 128.8213661, 188.3950257, 215.1098243, 226.034937, 228.1782945, 223.9285389, 213.904746, 198.2111343, 179.2385937, 159.8179885, 140.3973834, 120.9767782, 101.556173, 82.13556788, 62.71496272, 43.29435756, 23.8737524, 4.453147243, -14.96745792, -34.38806307, -53.80866823, -73.22927339, -92.64987855, -112.0704837, -131.4910889, -150.911694, -170.3322992, -189.7529043, -209.1735095, -228.5941147,
0, 131.027332, 191.3489062, 217.8217213, 227.8138416, 228.5550038, 222.584703, 210.6019628, 192.7394517, 170.3477595, 146.3601999, 122.3726404, 98.38508081, 74.39752125, 50.40996169, 26.42240213, 2.434842567, -21.552717, -45.54027656, -69.52783612, -93.51539568, -117.5029552, -141.4905148, -165.4780744, -189.4656339, -213.4531935, -237.440753, -261.4283126, -285.4158722, -309.4034317, -333.3909913,
0, 133.3565884, 195.7706738, 222.9835189, 232.4941842, 232.0135842, 224.4959151, 210.8740617, 191.370526, 166.0196566, 136.9236665, 107.0049895, 77.08631247, 47.16763544, 17.24895842, -12.66971861, -42.58839563, -72.50707266, -102.4257497, -132.3444267, -162.2631037, -192.1817808, -222.1004578, -252.0191348, -281.9378118, -311.8564889, -341.7751659, -371.6938429, -401.6125199, -431.531197, -461.449874,
0, 135.8676685, 202.6796359, 232.6455221, 242.8783871, 241.7896006, 233.1242408, 218.3474755, 197.9403384, 172.0260107, 140.6262136, 104.6055695, 66.70580351, 28.80603754, -9.093728435, -46.99349441, -84.89326038, -122.7930264, -160.6927923, -198.5925583, -236.4923243, -274.3920902, -312.2918562, -350.1916222, -388.0913882, -425.9911541, -463.8909201, -501.7906861, -539.6904521, -577.590218, -615.489984,
0, 137.8309855, 211.6442474, 247.3763321, 260.5590012, 260.0562203, 250.6421301, 234.7121719, 213.3663564, 187.0555646, 155.9388151, 120.061722, 79.43399662, 34.05694477, -14.40465742, -63.5761861, -112.7477148, -161.9192435, -211.0907721, -260.2623008, -309.4338295, -358.6053582, -407.7768868, -456.9484155, -506.1199442, -555.2914729, -604.4630015, -653.6345302, -702.8060589, -751.9775876, -801.1491163,
0, 136.7055152, 217.9927478, 261.8709768, 280.6575124, 282.5699438, 272.9377746, 255.1069417, 231.1003651, 202.0881672, 168.7125088, 131.3041205, 90.02053795, 44.92978042, -3.942280688, -56.58733583, -113.0032746, -173.1897167, -237.1466223, -303.0379534, -368.9784791, -434.9190049, -500.8595306, -566.8000563, -632.7405821, -698.6811078, -764.6216335, -830.5621593, -896.502685, -962.4432107, -1028.383736,
0, 127.3479759, 210.3492895, 259.6780629, 283.5500391, 288.2187428, 278.3867771, 257.5439373, 228.2426438, 192.3200799, 151.0753852, 105.4092825, 55.9326183, 3.049464398, -52.98034131, -111.995035, -173.8976108, -238.6323876, -306.1690185, -376.4919617, -449.5938406, -525.4714711, -604.1236355, -685.5499371, -769.7502722, -856.7246209, -946.4729807, -1038.058179, -1130.09321, -1222.128241, -1314.163272,
0, 101.7958769, 170.5542487, 211.8062816, 230.2072883, 229.6574945, 213.4087524, 184.1585169, 144.132311, 95.15581667, 38.71764511, -23.97624034, -91.95555105, -164.4453244, -240.8314428, -320.6313286, -403.4691838, -489.0552023, -577.168235, -667.6414417, -760.3505084, -855.2040561, -952.1359071, -1051.098913, -1152.060085, -1254.996804, -1359.893904, -1466.741473, -1575.533215, -1686.265262, -1798.935324,
0, 48.58665502, 73.8040296, 78.26885964, 64.3225635, 34.05639871, -10.66532065, -68.18382505, -137.0261937, -215.8872525, -303.6129609, -399.1851888, -501.7077896, -610.3938794, -724.5542378, -843.5867508, -966.966819, -1094.23866, -1225.007438, -1358.932153, -1495.719239, -1635.1168, -1776.909448, -1920.913675, -2066.973735, -2214.957961, -2364.755519, -2516.27351, -2669.434436, -2824.173958, -2980.438938,
0, 129.3116857, 210.6950349, 258.9281924, 285.2904299, 297.9956475, 302.6408808, 302.6739822, 299.8876305, 295.2893522, 290.0157582, 284.7421642, 279.4685702, 274.1949762, 268.9213822, 263.6477882, 258.3741942, 253.1006002, 247.8270062, 242.5534122, 237.2798182, 232.0062242, 226.7326302, 221.4590362, 216.1854422, 210.9118482, 205.6382542, 200.3646603, 195.0910663, 189.8174723, 184.5438783,
0, 131.5916089, 210.8972854, 255.8638817, 279.363069, 290.118879, 293.5935993, 292.8253345, 289.2071236, 283.5027447, 276.9478691, 270.3929934, 263.8381178, 257.2832421, 250.7283665, 244.1734908, 237.6186152, 231.0637395, 224.5088639, 217.9539882, 211.3991126, 204.844237, 198.2893613, 191.7344857, 185.17961, 178.6247344, 172.0698587, 165.5149831, 158.9601074, 152.4052318, 145.8503561,
0, 134.7775268, 213.599964, 257.0988008, 279.3692114, 289.4470387, 292.634246, 291.6611358, 287.6692771, 281.2635605, 273.8110238, 266.3584871, 258.9059505, 251.4534138, 244.0008771, 236.5483404, 229.0958037, 221.643267, 214.1907303, 206.7381936, 199.2856569, 191.8331202, 184.3805836, 176.9280469, 169.4755102, 162.0229735, 154.5704368, 147.1179001, 139.6653634, 132.2128267, 124.76029,
0, 138.4090499, 217.489277, 260.3646098, 282.1776461, 292.1784255, 295.500275, 294.6195302, 290.4950906, 283.5959116, 275.4326496, 267.2693877, 259.1061258, 250.9428639, 242.779602, 234.6163401, 226.4530782, 218.2898163, 210.1265544, 201.9632925, 193.8000306, 185.6367687, 177.4735067, 169.3102448, 161.1469829, 152.983721, 144.8204591, 136.6571972, 128.4939353, 120.3306734, 112.1674115,
0, 142.2563603, 221.9770561, 264.6794679, 286.4411002, 296.6631236, 300.3076932, 299.6334954, 295.4582781, 288.1184119, 279.2754709, 270.4325298, 261.5895888, 252.7466477, 243.9037066, 235.0607656, 226.2178245, 217.3748835, 208.5319424, 199.6890014, 190.8460603, 182.0031193, 173.1601782, 164.3172372, 155.4742961, 146.6313551, 137.788414, 128.945473, 120.1025319, 111.2595909, 102.4166498,
0, 146.1603041, 226.6853597, 269.4280516, 291.3180913, 301.8620646, 305.8515226, 305.3549741, 301.0773096, 293.2164072, 283.5904839, 273.9645605, 264.3386372, 254.7127139, 245.0867905, 235.4608672, 225.8349439, 216.2090205, 206.5830972, 196.9571739, 187.3312505, 177.7053272, 168.0794039, 158.4534805, 148.8275572, 139.2016339, 129.5757105, 119.9497872, 110.3238639, 100.6979405, 91.0720172,
0, 149.9948074, 231.3298159, 274.1555131, 296.1876954, 307.003306, 311.2260329, 310.7564947, 306.2074781, 297.712206, 287.1650463, 276.5321495, 265.8992527, 255.266356, 244.6334592, 234.0005625, 223.3676657, 212.734769, 202.1018722, 191.4689755, 180.8360787, 170.2031819, 159.5702852, 148.9373884, 138.3044917, 127.6715949, 117.0386982, 106.4058014, 95.77290464, 85.14000788, 74.50711113,
0, 153.65488, 235.6842507, 278.5048661, 300.562026, 311.4757348, 315.7068432, 315.0074624, 309.9145071, 300.5385034, 288.7947993, 276.8194277, 264.8440561, 252.8686845, 240.8933128, 228.9179412, 216.9425696, 204.967198, 192.9918263, 181.0164547, 169.0410831, 157.0657114, 145.0903398, 133.1149682, 121.1395966, 109.1642249, 97.18885331, 85.21348168, 73.23811005, 61.26273842, 49.2873668,
0, 157.0549197, 239.5778438, 282.2099338, 304.0734689, 314.8116341, 318.7314651, 317.455457, 311.4585536, 300.8339679, 287.4871625, 273.727541, 259.9679196, 246.2082981, 232.4486766, 218.6890552, 204.9294337, 191.1698122, 177.4101908, 163.6505693, 149.8909479, 136.1313264, 122.3717049, 108.6120835, 94.85246199, 81.09284053, 67.33321906, 53.5735976, 39.81397613, 26.05435467, 12.2947332,
0, 160.1352479, 242.919214, 285.1321291, 306.5184459, 316.7352448, 319.9536163, 317.6886891, 310.3673287, 298.0702933, 282.6335061, 266.5410565, 250.4486068, 234.3561572, 218.2637076, 202.171258, 186.0788084, 169.9863588, 153.8939092, 137.8014596, 121.70901, 105.6165604, 89.52411075, 73.43166114, 57.33921153, 41.24676192, 25.1543123, 9.061862695, -7.030586916, -23.12303653, -39.21548614,
0, 162.8785979, 245.7551705, 287.3524537, 307.97076, 317.2918089, 319.3882291, 315.7004112, 306.6243923, 292.2339401, 274.1656965, 255.0734239, 235.9811513, 216.8888788, 197.7966062, 178.7043336, 159.6120611, 140.5197885, 121.4275159, 102.3352434, 83.2429708, 64.15069823, 45.05842566, 25.96615309, 6.873880526, -12.21839204, -31.31066461, -50.40293718, -69.49520974, -88.58748231, -107.6797549,
0, 165.3378086, 248.3743179, 289.3371017, 308.9887526, 317.0853752, 317.6787705, 312.1905456, 301.0125203, 284.2195407, 262.9654526, 240.0539625, 217.1424724, 194.2309823, 171.3194922, 148.4080021, 125.496512, 102.5850219, 79.67353174, 56.76204164, 33.85055153, 10.93906142, -11.97242869, -34.8839188, -57.7954089, -80.70689901, -103.6183891, -126.5298792, -149.4413693, -172.3528594, -195.2643496,
0, 167.6655212, 251.4447962, 292.173801, 310.921924, 317.6338389, 316.4912259, 308.9998037, 295.5958732, 276.3711483, 251.4161976, 223.6359201, 195.8556427, 168.0753653, 140.2950878, 112.5148104, 84.73453297, 56.95425554, 29.17397811, 1.393700678, -26.38657675, -54.16685418, -81.94713162, -109.727409, -137.5076865, -165.2879639, -193.0682413, -220.8485188, -248.6287962, -276.4090736, -304.1893511,
0, 170.098229, 256.0853327, 297.7681587, 316.2132207, 321.7318466, 318.8947464, 309.4818214, 294.0746908, 272.8216339, 245.7487104, 213.7137038, 179.6253368, 145.5369699, 111.4486029, 77.36023589, 43.2718689, 9.18350191, -24.90486508, -58.99323206, -93.08159905, -127.169966, -161.258333, -195.3467, -229.435067, -263.523434, -297.611801, -331.700168, -365.788535, -399.8769019, -433.9652689,
0, 172.7567218, 263.5305166, 308.5663615, 328.2484201, 333.3673198, 329.2257531, 318.1971724, 301.2233917, 278.6227669, 250.4814682, 216.816364, 177.6294424, 135.3901219, 92.85926394, 50.32840599, 7.797548036, -34.73330991, -77.26416786, -119.7950258, -162.3258838, -204.8567417, -247.3875997, -289.9184576, -332.4493156, -374.9801735, -417.5110315, -460.0418894, -502.5727474, -545.1036053, -587.6344633,
0, 175.0320292, 273.7152746, 325.7570514, 349.5437093, 356.0106563, 351.3637941, 338.9331308, 320.3880224, 296.4977663, 267.5810242, 233.7526713, 195.047043, 151.4720211, 103.0288263, 50.41143185, -3.946124481, -58.30368081, -112.6612371, -167.0187935, -221.3763498, -275.7339061, -330.0914625, -384.4490188, -438.8065752, -493.1641315, -547.5216878, -601.8792442, -656.2368005, -710.5943568, -764.9519132,
0, 174.5138119, 282.3799097, 344.7498063, 376.292099, 386.9588597, 383.3374767, 369.6677389, 348.592382, 321.6988065, 289.9001864, 253.6956853, 213.3419538, 168.9614581, 120.6074514, 68.30050452, 12.04741937, -48.14998382, -112.2913449, -180.3766187, -251.2972138, -323.0811752, -394.8651365, -466.6490978, -538.4330592, -610.2170205, -682.0009819, -753.7849432, -825.5689045, -897.3528659, -969.1368272,
0, 166.0436095, 278.1812414, 349.3051795, 389.3702406, 405.9836962, 404.8938709, 390.3916422, 365.6375517, 332.9258318, 293.8953419, 249.6962026, 201.1198053, 148.6988537, 92.78316638, 33.59612424, -28.72411256, -94.09517652, -162.4700175, -233.8230781, -308.1412848, -385.4184195, -465.6517789, -548.8403142, -634.9836742, -724.0817627, -816.1345595, -911.1420619, -1008.708058, -1107.355197, -1206.002335,
0, 141.4263971, 242.2862542, 309.4267204, 348.6051844, 364.6412473, 361.5506447, 342.6628675, 310.7241069, 267.9870187, 216.2886916, 157.1180874, 91.67412101, 20.91544445, -54.3970899, -133.6644027, -216.418794, -302.2984575, -391.0262479, -482.3921159, -576.238687, -672.4495176, -770.9396156, -871.6478644, -974.5310305, -1079.559079, -1186.711559, -1295.974856, -1407.340127, -1520.801794, -1636.356449,
0, 88.91877419, 148.9307934, 183.3332214, 195.070773, 186.7686326, 160.7627583, 119.1277379, 63.70236109, -3.886938835, -82.20462932, -169.9864697, -266.1216192, -369.6363389, -479.6791698, -595.5074788, -716.4752675, -842.0221473, -971.6633886, -1104.980958, -1241.615464, -1381.258932, -1523.648344, -1668.559871, -1815.803737, -1965.219664, -2116.672837, -2270.05034, -2425.258024, -2582.217756, -2740.865017,
0, 171.7659843, 285.3022991, 357.2418662, 400.44182, 424.5085481, 436.309163, 440.4676903, 439.8436704, 435.9898754, 429.5838969, 421.4397278, 412.7474043, 404.0550809, 395.3627575, 386.670434, 377.9781106, 369.2857872, 360.5934638, 351.9011403, 343.2088169, 334.5164935, 325.82417, 317.1318466, 308.4395232, 299.7471997, 291.0548763, 282.3625529, 273.6702295, 264.977906, 256.2855826,
0, 174.0735091, 285.0508435, 352.8224187, 392.0285809, 413.0238242, 422.7625995, 425.6024469, 424.0191376, 419.2269028, 411.6940621, 402.153554, 391.9230615, 381.692569, 371.4620765, 361.231584, 351.0010915, 340.770599, 330.5401065, 320.309614, 310.0791215, 299.848629, 289.6181365, 279.387644, 269.1571515, 258.926659, 248.6961664, 238.4656739, 228.2351814, 218.0046889, 207.7741964,
0, 177.4114027, 287.6477782, 353.3647314, 390.6082952, 410.2499504, 419.2455173, 421.7177157, 419.8621554, 414.6764686, 406.5122175, 396.0463038, 384.7319281, 373.4175525, 362.1031768, 350.7888011, 339.4744254, 328.1600497, 316.845674, 305.5312983, 294.2169226, 282.9025469, 271.5881713, 260.2737956, 248.9594199, 237.6450442, 226.3306685, 215.0162928, 203.7019171, 192.3875414, 181.0731658,
0, 181.2656643, 291.6153707, 356.2783528, 392.5302756, 411.6443843, 420.5353742, 423.1092687, 421.3340873, 416.0481006, 407.5253637, 396.3862268, 384.2229827, 372.0597386, 359.8964946, 347.7332505, 335.5700064, 323.4067624, 311.2435183, 299.0802742, 286.9170301, 274.7537861, 262.590542, 250.4272979, 238.2640539, 226.1008098, 213.9375657, 201.7743217, 189.6110776, 177.4478335, 165.2845894, 
0, 185.3788783, 296.2919727, 360.4467247, 396.2379832, 415.264678, 424.3732385, 427.2665665, 425.7197472, 420.4466952, 411.6659626, 399.9332079, 386.9828019, 374.0323958, 361.0819898, 348.1315838, 335.1811777, 322.2307717, 309.2803656, 296.3299596, 283.3795535, 270.4291475, 257.4787415, 244.5283354, 231.5779294, 218.6275233, 205.6771173, 192.7267113, 179.7763052, 166.8258992, 153.8754931, 
0, 189.5723958, 301.2525419, 365.1729913, 400.7674436, 419.9036193, 429.340683, 432.5900608, 431.2597095, 425.9633614, 416.8780953, 404.4831322, 390.6580661, 376.8329999, 363.0079338, 349.1828677, 335.3578015, 321.5327354, 307.7076693, 293.8826032, 280.057537, 266.2324709, 252.4074048, 238.5823386, 224.7572725, 210.9322064, 197.1071403, 183.2820741, 169.457008, 155.6319419, 141.8068757, 
0, 193.7043331, 306.1769531, 369.9431432, 405.4113073, 424.6724968, 434.3849764, 437.8795189, 436.6176127, 431.1307452, 421.5641789, 408.3061353, 393.3849117, 378.4636882, 363.5424647, 348.6212411, 333.7000176, 318.7787941, 303.8575705, 288.936347, 274.0151235, 259.0938999, 244.1726764, 229.2514529, 214.3302293, 199.4090058, 184.4877823, 169.5665587, 154.6453352, 139.7241117, 124.8028881, 
0, 197.6559336, 310.8094689, 374.3533159, 409.6150183, 428.8712974, 438.6703031, 442.1731111, 440.7123934, 434.7515211, 424.4110049, 409.9690672, 393.6064054, 377.2437436, 360.8810818, 344.5184201, 328.1557583, 311.7930965, 295.4304347, 279.067773, 262.7051112, 246.3424494, 229.9797876, 213.6171258, 197.2544641, 180.8918023, 164.5291405, 148.1664787, 131.8038169, 115.4411552, 99.07849339, 
0, 201.3291063, 314.9540491, 378.0998865, 412.9596964, 431.9645146, 441.5483967, 444.7155523, 442.6864633, 435.8684379, 424.3628012, 408.3081664, 390.0409884, 371.7738104, 353.5066324, 335.2394544, 316.9722764, 298.7050984, 280.4379204, 262.1707424, 243.9035644, 225.6363865, 207.3692085, 189.1020305, 170.8348525, 152.5676745, 134.3004965, 116.0333185, 97.76614048, 79.49896249, 61.23178449, 
0, 204.6525692, 318.4984815, 381.0180534, 415.2090972, 433.6327141, 442.6139292, 445.0185884, 441.973426, 433.8414515, 420.7097544, 402.5914106, 381.9095215, 361.1569919, 340.4044622, 319.6519325, 298.8994028, 278.1468732, 257.3943435, 236.6418138, 215.8892842, 195.1367545, 174.3842248, 153.6316951, 132.8791655, 112.1266358, 91.37410612, 70.62157645, 49.86904677, 29.1165171, 8.36398743, 
0, 207.5982398, 321.4752428, 383.1809631, 416.4344236, 433.9143243, 441.8601589, 443.0314539, 438.4863843, 428.5576384, 413.3234013, 392.7953099, 369.2341731, 345.2865349, 321.3388967, 297.3912586, 273.4436204, 249.4959822, 225.548344, 201.6007059, 177.6530677, 153.7054295, 129.7577913, 105.8101532, 81.86251499, 57.91487682, 33.96723865, 10.01960047, -13.9280377, -37.87567588, -61.82331405,
0, 210.2095156, 324.1706509, 385.0802444, 417.2445272, 433.469699, 439.9702367, 439.4598476, 432.9690453, 420.8207156, 403.0914908, 379.7932271, 352.7445312, 324.729488, 296.7144449, 268.6994017, 240.6843585, 212.6693154, 184.6542722, 156.6392291, 128.6241859, 100.6091428, 72.59409963, 44.57905648, 16.56401333, -11.45102982, -39.46607298, -67.48111613, -95.49615928, -123.5112024, -151.5262456, 
0, 212.6338573, 327.2750501, 387.8889996, 419.1328062, 433.9857425, 438.764026, 436.2499482, 427.5196552, 412.9222565, 392.5460838, 366.4065474, 335.2989161, 302.1041599, 268.9094037, 235.7146475, 202.5198912, 169.325135, 136.1303788, 102.9356226, 69.7408664, 36.54611019, 3.351353983, -29.84340223, -63.03815844, -96.23291465, -129.4276709, -162.6224271, -195.8171833, -229.0119395, -262.2066957, 
0, 215.1166129, 331.9809933, 393.7017102, 424.8450076, 438.633253, 441.7040232, 437.1123039, 426.1166904, 409.1653821, 386.3892012, 357.8168826, 323.4525213, 284.8793548, 244.9932657, 205.1071767, 165.2210877, 125.3349986, 85.44890959, 45.56282055, 5.676731515, -34.20935752, -74.09544656, -113.9815356, -153.8676246, -193.7537137, -233.6398027, -273.5258917, -313.4119808, -353.2980698, -393.1841589,
0, 217.8258508, 339.7033183, 405.3226633, 438.3210695, 452.1395296, 454.0113619, 447.5779299, 434.5274444, 415.5653233, 390.9491417, 360.7573877, 325.0086308, 283.705894, 237.2693468, 188.4758481, 139.6823494, 90.88885073, 42.09535205, -6.698146634, -55.49164531, -104.285144, -153.0786427, -201.8721414, -250.66564, -299.4591387, -348.2526374, -397.0461361, -445.8396348, -494.6331334, -543.4266321, 
0, 220.2718137, 350.7388441, 424.5447926, 462.9604031, 479.2038311, 481.189195, 473.4536087, 458.4681174, 437.4982804, 411.1454642, 379.669998, 343.1719618, 301.6842812, 255.2157208, 203.7680064, 147.3413528, 87.29531636, 26.11936041, -35.05659554, -96.23255149, -157.4085074, -218.5844634, -279.7604193, -340.9363753, -402.1123312, -463.2882872, -524.4642431, -585.6401991, -646.816155, -707.992111,
0, 220.2129773, 361.3326809, 447.5904365, 496.0885833, 518.6716395, 523.3716562, 515.5082222, 498.5118315, 474.5287304, 444.8558575, 410.2460468, 371.1162076, 327.6846906, 280.0584282, 228.2856548, 172.3860015, 112.3664712, 48.22916206, -20.0254311, -92.39722577, -168.8862139, -247.7889835, -327.046938, -406.3048925, -485.562847, -564.8208016, -644.0787561, -723.3367106, -802.5946651, -881.8526196, 
0, 212.5508355, 360.3825135, 458.7200506, 519.3872945, 551.4799893, 561.9240265, 555.9351007, 537.3942096, 509.1517951, 473.2718049, 431.2255622, 384.0440621, 332.4361498, 276.8789896, 217.686284, 155.0588546, 89.121438, 19.94888463, -52.41564381, -127.9483879, -206.6369665, -288.4753376, -373.4607792, -461.5921809, -552.8691435, -647.2915451, -744.8593561, -845.5725712, -949.4311899, -1055.589551, 
0, 188.9020382, 328.53636, 427.2180525, 491.9415074, 528.5652573, 541.9745976, 536.2242029, 514.6627768, 480.0416088, 434.6087624, 380.1904716, 318.261188, 250.0035938, 176.3597755, 98.07464237, 15.73256784, -70.21186314, -159.4082103, -251.5889295, -346.5518895, -444.1460952, -544.2601236, -646.812841, -751.746021, -859.0185368, -968.6018434, -1080.476509, -1194.629586, -1311.052652, -1429.740377, 
0, 137.2126091, 239.001946, 309.4576175, 352.2274217, 370.5592877, 367.3398003, 345.1295408, 306.1954608, 252.540497, 185.9306211, 107.9195105, 19.87101095, -77.02044154, -181.7112881, -293.292057, -410.9721812, -534.0662942, -661.9818854, -794.2082031, -930.3062987, -1069.900116, -1212.668534, -1358.338275, -1506.677608, -1657.490761, -1810.612985, -1965.906206, -2123.255196, -2282.564228, -2443.754151, 
0, 223.7659494, 378.4039012, 482.0749715, 549.0504945, 590.2714451, 613.8752084, 625.6881306, 629.6820013, 628.3922212, 623.2948439, 615.1388128, 604.2282205, 591.1241969, 577.1676224, 563.2110479, 549.2544733, 535.2978988, 521.3413243, 507.3847498, 493.4281753, 479.4716007, 465.5150262, 451.5584517, 437.6018772, 423.6453026, 409.6887281, 395.7321536, 381.7755791, 367.8190046, 353.86243, 
0, 226.0724981, 377.655983, 476.1872762, 537.8687009, 574.6028997, 594.8347603, 604.3005938, 606.6847194, 604.1836401, 597.9780161, 588.6126898, 576.2850791, 561.4865041, 545.6770142, 529.8675242, 514.0580343, 498.2485443, 482.4390543, 466.6295644, 450.8200744, 435.0105845, 419.2010945, 403.3916045, 387.5821146, 371.7726246, 355.9631347, 340.1536447, 324.3441548, 308.5346648, 292.7251748, 
0, 229.5351523, 380.1051484, 475.9289813, 534.7584007, 569.2039508, 587.8879569, 596.4311993, 598.2805302, 595.3853223, 588.7314432, 578.7425109, 565.5596814, 549.6105433, 532.478183, 515.3458226, 498.2134623, 481.081102, 463.9487417, 446.8163814, 429.6840211, 412.5516608, 395.4193005, 378.2869402, 361.1545799, 344.0222196, 326.8898593, 309.7574989, 292.6251386, 275.4927783, 258.360418, 
0, 233.5877885, 384.1129816, 478.3877796, 535.5405843, 568.7650602, 586.794533, 595.1211547, 596.9799931, 594.119061, 587.3773411, 577.094156, 563.3749276, 546.5772783, 528.409583, 510.2418877, 492.0741924, 473.9064971, 455.7388018, 437.5711065, 419.4034112, 401.2357159, 383.0680207, 364.9003254, 346.7326301, 328.5649348, 310.3972395, 292.2295442, 274.0618489, 255.8941536, 237.7264583,
0, 237.945684, 388.9450106, 482.3120333, 538.4465159, 571.043228, 588.9020001, 597.3780698, 599.5123646, 596.8814979, 590.2074055, 579.7667304, 565.6402818, 548.1057759, 528.9991507, 509.8925255, 490.7859004, 471.6792752, 452.57265, 433.4660248, 414.3593997, 395.2527745, 376.1461493, 357.0395242, 337.932899, 318.8262738, 299.7196487, 280.6130235, 261.5063983, 242.3997732, 223.293148, 
0, 242.4107132, 394.1291491, 486.9229601, 542.3884896, 574.6599316, 592.5702976, 601.3325918, 603.8076508, 601.4229664, 594.8044438, 584.1803769, 569.6138316, 551.2907551, 531.1772602, 511.0637652, 490.9502703, 470.8367753, 450.7232804, 430.6097854, 410.4962905, 390.3827955, 370.2693006, 350.1558056, 330.0423107, 309.9288157, 289.8153208, 269.7018258, 249.5883309, 229.4748359, 209.361341, 
0, 246.8250969, 399.3093231, 491.6469442, 546.5720762, 578.6082849, 596.5954533, 605.6013088, 608.3188328, 606.044253, 599.3233462, 588.3462743, 573.1629434, 553.8537239, 532.5178917, 511.1820594, 489.8462272, 468.510395, 447.1745628, 425.8387305, 404.5028983, 383.1670661, 361.8312339, 340.4954016, 319.1595694, 297.8237372, 276.487905, 255.1520727, 233.8162405, 212.4804083, 191.144576, 
0, 251.0561226, 404.1997314, 496.0324884, 550.3753921, 582.0986443, 600.0279519, 609.0849924, 611.8064793, 609.372632, 602.2618437, 590.6338495, 574.5286393, 553.952539, 531.0919411, 508.182074, 485.2722068, 462.3623397, 439.4524725, 416.5426053, 393.6327382, 370.722871, 347.8130038, 324.9031367, 301.9932695, 279.0834023, 256.1735352, 233.263668, 210.3538009, 187.4439337, 164.5340665, 
0, 254.9928004, 408.577916, 499.7368322, 553.3270018, 584.5272588, 602.1332593, 610.9232018, 613.2899961, 610.3121103, 602.4115496, 589.7238357, 572.281491, 550.0894397, 525.3235369, 500.3584733, 475.3934096, 450.428346, 425.4632823, 400.4982187, 375.533155, 350.5680914, 325.6030277, 300.6379641, 275.6729004, 250.7078368, 225.7427731, 200.7777095, 175.8126458, 150.8475822, 125.8825185,
0, 258.5513059, 412.3079942, 502.5650419, 555.1551899, 585.531014, 602.4497113, 610.5551605, 612.1127304, 608.1145962, 598.9370995, 584.6971286, 565.4217833, 541.1150379, 513.8887523, 486.2584979, 458.6282434, 430.997989, 403.3677346, 375.7374802, 348.1072258, 320.4769713, 292.8467169, 265.2164625, 237.5862081, 209.9559537, 182.3256992, 154.6954448, 127.0651904, 99.43493597, 71.80468155,
0, 261.6906317, 415.4017484, 504.5739317, 555.9230022, 585.1430682, 600.9585894, 607.9023251, 608.1379719, 602.5918198, 591.607087, 575.287968, 553.6581586, 526.7211046, 496.4033386, 465.3583685, 434.3133984, 403.2684283, 372.2234582, 341.1784881, 310.1335181, 279.088548, 248.0435779, 216.9986078, 185.9536377, 154.9086676, 123.8636975, 92.81872746, 61.77375737, 30.72878728, -0.316182804,
0, 264.4402813, 418.1300969, 506.2638191, 556.2798269, 584.085346, 598.4058939, 603.7149894, 602.1210705, 594.5166423, 581.2296983, 562.3596145, 537.9292346, 507.9420079, 473.8682553, 438.4862738, 403.1042922, 367.7223107, 332.3403291, 296.9583476, 261.576366, 226.1943845, 190.812403, 155.4304214, 120.0484399, 84.66645832, 49.28447678, 13.90249524, -21.47948631, -56.86146785, -92.24344939,
0, 266.9339589, 421.1812701, 508.8633485, 557.8458194, 584.2235416, 596.806954, 600.115321, 596.2879173, 586.2397306, 570.3138149, 548.6179345, 521.178509, 488.0000262, 449.530514, 408.6392963, 367.7480786, 326.8568608, 285.9656431, 245.0744253, 204.1832076, 163.2919899, 122.4007721, 81.50955439, 40.61833665, -0.272881095, -41.16409883, -82.05531657, -122.9465343, -163.8377521, -204.7289698,
0, 269.4114514, 425.7827778, 514.6092734, 563.6392002, 589.1087095, 600.0639725, 601.2582925, 595.0173777, 582.3827653, 563.771265, 539.3268531, 509.0902693, 473.070202, 431.2678621, 384.9303234, 336.9486813, 288.9670393, 240.9853972, 193.0037552, 145.0221131, 97.04047107, 49.05882901, 1.077186961, -46.90445509, -94.88609714, -142.8677392, -190.8493812, -238.8310233, -286.8126654, -334.7943074,
0, 272.069217, 433.4817732, 526.6058724, 578.1027802, 604.1749302, 614.278372, 613.6758963, 605.1356715, 590.0176896, 568.9363101, 542.1412578, 509.7211067, 471.7020509, 428.0901085, 378.8862351, 324.2027687, 266.8356339, 209.4684991, 152.1013643, 94.73422953, 37.36709472, -20.00004008, -77.36717489, -134.7343097, -192.1014445, -249.4685793, -306.8357141, -364.2028489, -421.5699837, -478.9371185,
0, 274.5253774, 444.9106562, 547.2151998, 605.4667114, 635.2708045, 646.4730486, 645.0823488, 634.6284201, 617.0942947, 593.5370206, 564.4857266, 530.1857926, 490.7407261, 446.1892434, 396.5437229, 341.8073654, 281.9807844, 217.0640548, 148.1752083, 77.85931724, 7.543426167, -62.77246491, -133.088356, -203.4042471, -273.7201381, -344.0360292, -414.3519203, -484.6678114, -554.9837025, -625.2995935,
0, 274.7323482, 456.8862755, 573.5518808, 644.2410421, 682.7842649, 698.8136169, 698.9075285, 687.4622929, 667.3452664, 640.3759926, 607.6736851, 569.9026511, 527.4412565, 480.4948633, 429.1687536, 373.5133283, 313.5507789, 249.2899084, 180.7337734, 107.8832644, 30.73858566, -50.70022962, -136.4331782, -224.9039126, -313.965366, -403.0268195, -492.0882729, -581.1497264, -670.2111798, -759.2726333,
0, 267.7282244, 458.7608953, 590.674222, 677.2283939, 729.1047099, 754.5191295, 759.729594, 749.4525393, 727.2022443, 695.5650318, 656.4188497, 611.107405, 560.5767948, 505.4814674, 446.2653496, 383.2230849, 316.5455356, 246.3530001, 172.7189872, 95.686852, 15.28114276, -68.48488601, -155.6044676, -246.0743711, -339.8931742, -437.060311, -537.575583, -641.4389314, -748.6503422, -859.2098131,
0, 245.0113436, 430.8885208, 567.5303049, 663.2777935, 725.1314803, 758.94216, 769.5783062, 761.0723493, 736.7480825, 699.3312371, 651.045091, 593.6928115, 528.7280773, 457.3153821, 380.3812873, 298.6577684, 212.7186803, 123.0102645, 29.87651942, -66.41983724, -165.6801637, -267.755879, -372.5371344, -479.9437361, -589.9179375, -702.4187736, -817.4176566, -934.8949927, -1054.837617, -1177.236878,
0, 194.2106678, 345.4552783, 458.7225434, 538.4595032, 588.6234722, 612.7296819, 613.8949181, 594.8774361, 558.1134184, 505.7502278, 439.6766902, 361.5506309, 272.8238735, 174.764896, 68.47933108, -45.07152043, -165.053998, -290.7470635, -421.5288145, -556.8643985, -696.2952047, -839.4292206, -985.9324475, -1135.521276, -1287.955735, -1443.033519, -1600.58474, -1760.467299, -1922.562846, -2086.773244,
0, 286.2738265, 492.0996608, 636.8246584, 735.9261724, 801.5639196, 843.0902535, 867.5184128, 879.9486038, 883.9517598, 881.9108004, 875.3191889, 865.0365568, 851.5011152, 834.898513, 815.2866889, 793.8362658, 772.0469139, 750.257562, 728.46821, 706.6788581, 684.8895061, 663.1001542, 641.3108023, 619.5214503, 597.7320984, 575.9427465, 554.1533945, 532.3640426, 510.5746907, 488.7853387,
0, 288.544989, 490.8162629, 629.3930766, 721.7832872, 781.2871804, 817.7686141, 838.3365463, 847.940181, 849.881169, 846.2465325, 838.2662684, 826.5999385, 811.557003, 793.2562711, 771.7307286, 748.1511595, 724.1274995, 700.1038395, 676.0801795, 652.0565195, 628.0328595, 604.0091995, 579.9855395, 555.9618795, 531.9382195, 507.9145595, 483.8908995, 459.8672395, 435.8435795, 411.8199195,
0, 292.0987767, 493.0762595, 628.2549423, 716.7915387, 772.858133, 806.6681157, 825.3563422, 833.7231714, 834.8522931, 830.6125326, 822.0541517, 809.7105486, 793.816733, 774.4565403, 751.6513402, 726.5646213, 700.9174302, 675.2702392, 649.6230481, 623.9758571, 598.328666, 572.681475, 547.0342839, 521.3870929, 495.7399018, 470.0927108, 444.4455197, 418.7983287, 393.1511376, 367.5039466,
0, 296.3187924, 497.0859305, 630.1819947, 716.2448581, 770.1896495, 802.4974221, 820.2921048, 828.2252305, 829.187838, 824.8688421, 816.1790497, 803.5598672, 787.1955956, 767.1480262, 743.4318289, 717.1938711, 690.2675409, 663.3412106, 636.4148804, 609.4885501, 582.5622199, 555.6358896, 528.7095593, 501.7832291, 474.8568988, 447.9305686, 421.0042383, 394.0779081, 367.1515778, 340.2252476,
0, 300.8934195, 502.0393075, 633.7888235, 718.1622383, 770.7347755, 802.2002025, 819.646361, 827.5758241, 828.7037996, 824.5642581, 815.9540313, 803.243287, 786.5793046, 766.0088147, 741.5422401, 714.3000276, 686.2296042, 658.1591809, 630.0887575, 602.0183342, 573.9479108, 545.8774875, 517.8070641, 489.7366408, 461.6662174, 433.5957941, 405.5253707, 377.4549474, 349.384524, 321.3141007,
0, 305.6051338, 507.4176059, 638.2149597, 721.3320132, 772.9423784, 803.9107364, 821.2698932, 829.3738157, 830.7749196, 826.8708376, 818.3627182, 805.5637634, 788.5931794, 767.4869332, 742.252607, 713.9740176, 684.7147834, 655.4555491, 626.1963149, 596.9370806, 567.6778464, 538.4186121, 509.1593779, 479.9001437, 450.6409094, 421.3816752, 392.1224409, 362.8632067, 333.6039724, 304.3447382,
0, 310.2802147, 512.8287578, 642.8269777, 724.8723397, 775.6868309, 806.2720791, 823.5910315, 831.8501582, 833.4501991, 829.6674545, 821.1215207, 808.0791806, 790.6378564, 768.8256052, 742.6480437, 713.141068, 682.487037, 651.8330061, 621.1789751, 590.5249442, 559.8709132, 529.2168823, 498.5628514, 467.9088204, 437.2547895, 406.6007585, 375.9467276, 345.2926966, 314.6386657, 283.9846348,
0, 314.7718512, 517.9565057, 647.1250363, 728.0932566, 778.0883127, 808.2201861, 825.370499, 833.6004717, 835.1694812, 831.2454086, 822.3778609, 808.7954297, 790.5784202, 767.7489825, 740.3113459, 709.2387543, 676.8350947, 644.4314352, 612.0277757, 579.6241161, 547.2204566, 514.8167971, 482.4131375, 450.009478, 417.6058185, 385.2021589, 352.7984994, 320.3948398, 287.9911803, 255.5875208,
0, 318.9558303, 522.550911, 650.7253995, 730.4688412, 779.4736018, 808.9338186, 825.6420569, 833.5187028, 834.6925631, 830.235223, 820.6364646, 806.0935295, 786.6732798, 762.3934496, 733.2572861, 700.1505041, 665.5011137, 630.8517232, 596.2023328, 561.5529424, 526.9035519, 492.2541615, 457.6047711, 422.9553807, 388.3059902, 353.6565998, 319.0072094, 284.3578189, 249.7084285, 215.0590381,
0, 322.7350395, 526.4498207, 653.3985677, 731.6872565, 779.4330972, 807.8951527, 823.7752071, 830.8619862, 831.1672858, 825.6795146, 814.8386751, 798.8169532, 777.670939, 751.4151595, 720.05219, 684.3283432, 646.7970934, 609.2658436, 571.7345939, 534.2033441, 496.6720943, 459.1408445, 421.6095947, 384.0783449, 346.5470951, 309.0158453, 271.4845955, 233.9533457, 196.4220959, 158.8908461,
0, 326.0538536, 529.6383798, 655.1751217, 731.7933192, 777.9891918, 805.0757041, 819.6740142, 825.4608321, 824.3517301, 817.2691002, 804.6149619, 786.5432296, 763.1033941, 734.3079095, 700.1589534, 661.1464191, 619.9469397, 578.7474603, 537.5479808, 496.3485014, 455.149022, 413.9495426, 372.7500632, 331.5505838, 290.3511044, 249.1516249, 207.9521455, 166.7526661, 125.5531867, 84.35370729,
0, 328.9242269, 532.3595515, 656.5438547, 731.4572352, 775.9158732, 801.2915256, 814.1588517, 818.1238346, 815.0422139, 805.7970317, 790.7690435, 770.1028029, 743.8446165, 712.0061996, 674.5896499, 631.5951854, 585.7848912, 539.9477206, 494.11055, 448.2733794, 402.4362088, 356.5990381, 310.7618675, 264.9246969, 219.0875263, 173.2503557, 127.4131851, 81.57601447, 35.73884386, -10.09832676,
0, 331.458749, 535.275307, 658.7517366, 732.3907813, 775.2415467, 798.7676397, 809.5755961, 811.282809, 805.7519524, 793.8728468, 776.03264, 752.3808231, 722.966731, 687.8035033, 646.8937152, 600.2376825, 549.6209851, 497.9167363, 446.2124876, 394.5082389, 342.8039902, 291.0997415, 239.3954928, 187.6912441, 135.9869954, 84.28274674, 32.57849804, -19.12575066, -70.82999936, -122.5342481,
0, 333.8782331, 539.6071494, 664.1178891, 737.8278659, 779.866769, 801.8568784, 810.582304, 809.8216234, 801.5654638, 786.7944893, 765.9536535, 739.2250635, 706.6739973, 668.3200809, 624.1679441, 574.2183549, 518.4713867, 459.5388588, 400.3194593, 341.1000599, 281.8806604, 222.6612609, 163.4418615, 104.222462, 45.00306252, -14.21633694, -73.43573641, -132.6551359, -191.8745353, -251.0939348,
0, 336.3870186, 546.977736, 675.9686229, 752.6352164, 795.8697721, 817.5083248, 824.7194222, 821.6784608, 810.70919, 793.0371815, 769.2685336, 739.680407, 704.3877996, 663.4328787, 616.8287976, 564.5788522, 506.6836476, 443.1432532, 375.4087644, 306.302774, 237.1967837, 168.0907933, 98.98480296, 29.8788126, -39.22717775, -108.3331681, -177.4391585, -246.5451488, -315.6511392, -384.7571295,
0, 338.6910427, 558.3060012, 697.1734409, 781.801314, 830.1576106, 854.151842, 861.4820606, 856.9850418, 843.6033662, 823.0622293, 796.3306747, 763.9263766, 726.1099002, 683.0033237, 634.6590308, 581.0971577, 522.3244252, 458.3426939, 389.1523619, 314.7534881, 235.1460771, 152.719488, 70.07798921, -12.56350956, -95.20500834, -177.8465071, -260.4880059, -343.1295047, -425.7710035, -508.4125022,
0, 338.9449708, 571.0098056, 725.7845981, 825.0553324, 884.6501263, 915.9029031, 926.8015893, 922.8784925, 907.8920413, 884.3415087, 853.8496356, 817.4421576, 775.7480691, 729.1399685, 677.8299678, 621.9333669, 561.5095296, 496.5871072, 427.1788878, 353.2900549, 274.9224699, 192.0767038, 104.7528988, 12.95108139, -83.32874534, -183.6224457, -285.6910151, -387.7595844, -489.8281538, -591.8967232,
0, 332.3910064, 575.0894874, 747.9384893, 866.6287985, 943.4749289, 988.0645011, 1007.798477, 1008.337779, 993.9700309, 967.908549, 932.5341982, 889.5894058, 840.3323874, 785.6585412, 726.1949821, 662.373298, 594.48483, 522.7220802, 447.2092454, 368.0243391, 285.2149085, 198.808955, 108.8223325, 15.26361752, -81.86279127, -182.5547762, -286.8113926, -394.6322564, -506.0172288, -620.9662664,
0, 310.4981389, 550.8820781, 732.7012546, 865.717508, 958.1512118, 1016.897899, 1047.718373, 1055.405043, 1043.927006, 1016.556162, 975.9764791, 924.3782923, 863.539398, 794.894494, 719.5943966, 638.5563087, 552.5062899, 462.0149569, 367.5273337, 269.3876664, 167.8599295, 63.14465998, -44.60731759, -155.2837842, -268.8022102, -385.1026592, -504.1421927, -625.8905032, -750.3265429, -877.435953,
0, 260.6281307, 469.7498418, 633.3343546, 756.704013, 844.5964781, 901.221725, 930.3142083, 935.1805433, 918.7430285, 883.579316, 831.9585188, 765.8740257, 687.0732772, 597.0847417, 497.2423141, 388.707346, 272.4885023, 149.4596248, 20.37577482, -114.1123909, -253.445761, -397.1452937, -544.8017965, -696.0668455, -850.6447362, -1008.285366, -1168.777956, -1331.945533, -1497.640082, -1665.738314,
0, 360.131707, 628.2237378, 824.4803784, 965.3565608, 1064.076077, 1131.105912, 1174.587741, 1200.727657, 1214.145293, 1218.183515, 1215.179978, 1206.701888, 1193.745444, 1176.901512, 1156.489291, 1132.659869, 1105.471875, 1074.941804, 1042.281604, 1009.161061, 976.0405169, 942.9199732, 909.7994295, 876.6788857, 843.558342, 810.4377983, 777.3172545, 744.1967108, 711.0761671, 677.9556233,
0, 362.3245881, 626.3599488, 815.4491695, 948.1296732, 1038.904662, 1098.930188, 1136.622177, 1158.18777, 1168.085466, 1169.418574, 1164.266639, 1153.959541, 1139.299145, 1120.733461, 1098.488462, 1072.662891, 1043.291616, 1010.383409, 975.1393719, 939.3273832, 903.5153944, 867.7034057, 831.8914169, 796.0794281, 760.2674394, 724.4554506, 688.6434619, 652.8314731, 617.0194844, 581.2074956,
0, 365.9271285, 628.381653, 813.3660555, 941.1218363, 1027.153651, 1083.121897, 1117.613279, 1136.798804, 1144.988569, 1145.092623, 1138.997081, 1127.864573, 1112.367979, 1092.866296, 1069.531332, 1042.433748, 1011.596796, 977.0258677, 939.903543, 902.0964807, 864.2894184, 826.4823561, 788.6752938, 750.8682315, 713.0611692, 675.2541068, 637.4470445, 599.6399822, 561.8329199, 524.0258576,
0, 370.2746632, 632.3465633, 814.6970061, 939.1097288, 1021.961659, 1075.326533, 1107.90837, 1125.821325, 1133.231911, 1132.879056, 1126.486895, 1115.084607, 1099.246967, 1079.268609, 1055.284293, 1027.346645, 995.4719813, 959.663833, 921.0781644, 881.6815167, 842.2848689, 802.8882212, 763.4915735, 724.0949258, 684.6982781, 645.3016304, 605.9049827, 566.5083349, 527.1116872, 487.7150395,
0, 375.0291519, 637.3789887, 817.92546, 939.9024603, 1020.478306, 1072.079994, 1103.492373, 1120.758066, 1127.902652, 1127.508281, 1121.157493, 1109.767595, 1093.834488, 1073.603307, 1049.181675, 1020.609705, 987.8991923, 951.0525416, 911.1909001, 870.3822091, 829.573518, 788.7648269, 747.9561358, 707.1474447, 666.3387536, 625.5300625, 584.7213714, 543.9126803, 503.1039893, 462.2952982,
0, 379.9537817, 642.9139981, 822.1100318, 942.1650537, 1020.979923, 1071.289438, 1101.931265, 1118.865234, 1125.978094, 1125.706132, 1119.505495, 1108.197329, 1092.212274, 1071.756126, 1046.915873, 1017.722576, 984.1849363, 946.3046327, 905.1595333, 862.9208858, 820.6822384, 778.4435909, 736.2049434, 693.966296, 651.7276485, 609.4890011, 567.2503536, 525.0117061, 482.7730587, 440.5344112,
0, 384.8588721, 648.5236092, 826.5575233, 944.928198, 1022.221995, 1071.444368, 1101.462991, 1118.146089, 1125.243761, 1125.05564, 1118.923809, 1107.585611, 1091.416784, 1070.591253, 1045.179962, 1015.207247, 980.6796247, 941.5982927, 898.9883612, 855.1266287, 811.2648961, 767.4031635, 723.541431, 679.6796984, 635.8179658, 591.9562333, 548.0945007, 504.2327681, 460.3710356, 416.509303,
0, 389.5834104, 653.8608396, 830.7191654, 947.433078, 1023.233849, 1071.365672, 1100.707202, 1117.028395, 1123.94548, 1123.629915, 1117.320432, 1105.680881, 1089.040433, 1067.54693, 1041.258678, 1010.194947, 974.3606832, 933.756755, 889.3426075, 843.5030335, 797.6634595, 751.8238855, 705.9843116, 660.1447376, 614.3051636, 568.4655896, 522.6260157, 476.7864417, 430.9468677, 385.1072937,
0, 393.9897265, 658.6474257, 834.1685177, 949.0970074, 1023.270712, 1070.144103, 1098.591232, 1114.279774, 1120.696359, 1119.89309, 1113.015515, 1100.663245, 1083.126033, 1060.530381, 1032.924644, 1000.324258, 962.7330401, 920.1516327, 873.4507029, 825.1253177, 776.7999324, 728.4745472, 680.1491619, 631.8237767, 583.4983914, 535.1730062, 486.8476209, 438.5222357, 390.1968505, 341.8714652,
0, 397.9668888, 662.6928197, 836.6372131, 949.5623851, 1021.871285, 1067.202589, 1094.414682, 1109.07351, 1114.543907, 1112.769626, 1104.814142, 1091.222162, 1072.250795, 1048.009464, 1018.538855, 983.8515755, 943.9506456, 898.8365584, 849.2409853, 797.7704837, 746.2999821, 694.8294805, 643.3589789, 591.8884773, 540.4179757, 488.9474741, 437.4769725, 386.0064709, 334.5359693, 283.0654678,
0, 401.4434496, 665.950594, 838.1197894, 948.8431836, 1019.035837, 1062.49678, 1088.065776, 1101.216863, 1105.208836, 1101.893828, 1092.267885, 1076.831963, 1055.818018, 1029.322797, 997.381538, 960.0049378, 917.1955126, 868.9536683, 815.7566622, 760.3206866, 704.884711, 649.4487353, 594.0127597, 538.5767841, 483.1408085, 427.7048329, 372.2688573, 316.8328817, 261.3969061, 205.9609305,
0, 404.4111555, 668.6250948, 839.0729758, 947.6047889, 1015.568236, 1056.902278, 1080.441259, 1091.595998, 1093.551588, 1088.09768, 1076.185732, 1058.288903, 1034.623948, 1005.280434, 970.2907619, 929.6647509, 883.4047285, 831.5110802, 773.9838423, 713.6041732, 653.1888669, 592.7735606, 532.3582544, 471.9429481, 411.5276418, 351.1123356, 290.6970293, 230.281723, 169.8664167, 109.4511105,
0, 406.9558664, 671.3307688, 840.723003, 947.6033984, 1013.619919, 1052.836242, 1074.124365, 1082.896099, 1082.326822, 1074.197068, 1059.453112, 1038.566523, 1011.754569, 979.108034, 940.6604073, 896.4221773, 846.3959723, 790.5822713, 728.981128, 663.3944501, 596.7159576, 530.0374651, 463.3589726, 396.6804801, 330.0019876, 263.3234951, 196.6450026, 129.9665101, 63.28801761, -3.390474885,
0, 409.2682608, 675.2447152, 845.4056376, 952.2114044, 1017.375019, 1055.068233, 1074.287914, 1080.568466, 1077.193466, 1066.031167, 1048.093657, 1023.898212, 993.6913102, 957.580726, 915.6087755, 867.7899294, 814.1283084, 754.6248245, 689.2796178, 618.0927004, 543.7118083, 469.0577967, 394.403785, 319.7497733, 245.0957617, 170.44175, 95.78773836, 21.1337267, -53.52028496, -128.1742966,
0, 411.5408497, 682.0054734, 856.5771217, 966.6133991, 1033.456603, 1071.321019, 1089.446819, 1093.678999, 1087.602245, 1073.338632, 1052.094172, 1024.522426, 990.9582415, 951.5620304, 906.4046091, 855.5142265, 798.9007897, 736.5672351, 668.514256, 594.7419707, 515.2503918, 431.5373886, 346.4626079, 261.3878271, 176.3130464, 91.23826563, 6.163484879, -78.91129587, -163.9860766, -249.0608574,
0, 413.5415871, 692.7508008, 877.5239011, 996.4542992, 1069.716818, 1111.304073, 1130.738913, 1134.367679, 1126.322055, 1109.222321, 1084.6818, 1053.660965, 1016.709976, 974.1301955, 926.0782821, 872.6307731, 813.8223683, 749.6674232, 680.1712341, 605.3354877, 525.1606264, 439.6467391, 348.7938379, 252.6019235, 153.557405, 54.32978867, -44.89782763, -144.1254439, -243.3530602, -342.5806765,
0, 413.6186152, 705.4857023, 907.2324178, 1042.682219, 1129.583345, 1181.003179, 1206.440773, 1212.706176, 1204.608387, 1185.487602, 1157.622008, 1122.534543, 1081.220826, 1034.315744, 982.2129914, 925.1491053, 863.2611704, 796.62541, 725.2822159, 649.2518155, 568.5436791, 483.161894, 393.1080557, 298.3827136, 198.986028, 94.91803685, -13.82125343, -127.2318421, -244.7871195, -364.1514364,
0, 407.2677728, 711.0079483, 933.1489099, 1091.230484, 1199.190952, 1268.030195, 1306.365604, 1320.89554, 1316.783479, 1297.974452, 1267.453979, 1227.458453, 1179.644779, 1125.226048, 1065.07908, 999.8288617, 929.9141676, 855.6379646, 777.2056695, 694.7537868, 608.3710284, 518.1136333, 424.016276, 326.0996744, 224.3757731, 118.8511822, 9.529390641, -103.5878597, -220.4997546, -341.2059423,
0, 386.2994096, 690.9371515, 926.9547302, 1105.438086, 1235.779189, 1325.907038, 1382.491114, 1411.120077, 1416.458268, 1402.382373, 1372.100376, 1328.254772, 1273.011804, 1208.138351, 1135.067908, 1054.956995, 968.733182, 877.1357785, 780.7501671, 680.0366179, 575.3543491, 466.9815063, 355.131653, 239.967298, 121.6109157, 0.153860745, -124.3364777, -251.8109806, -382.2343065, -515.5813818,
0, 337.8275572, 615.9580624, 841.0591077, 1019.103621, 1155.43404, 1254.82134, 1321.519254, 1359.314028, 1371.570009, 1361.271373, 1331.06026, 1283.271589, 1219.964778, 1142.952625, 1053.827547, 953.9853933, 844.6470146, 726.8777708, 601.6051462, 469.6346234, 331.6639652, 188.2960385, 40.05030534, -112.6268983, -269.3531909, -429.8019776, -593.6951931, -760.7968704, -930.9074534, -1103.858782,
0, 446.0261844, 788.2707951, 1047.497554, 1240.909919, 1382.620768, 1484.080645, 1554.466208, 1601.030546, 1629.417067, 1643.938668, 1647.823949, 1643.432254, 1632.439359, 1615.995671, 1594.858847, 1569.502788, 1540.205038, 1507.114652, 1470.302727, 1429.797848, 1385.608858, 1338.030336, 1288.903529, 1239.776723, 1190.649916, 1141.52311, 1092.396303, 1043.269497, 994.14269, 945.0158835,
0, 448.0873499, 785.7672788, 1036.812253, 1220.517543, 1352.373463, 1444.665009, 1507.003335, 1546.793479, 1569.642699, 1579.713743, 1580.027316, 1572.717986, 1559.247673, 1540.58086, 1517.325558, 1489.844032, 1458.337177, 1422.906399, 1383.59673, 1340.424827, 1293.39536, 1242.73616, 1190.375625, 1138.01509, 1085.654555, 1033.29402, 980.9334847, 928.5729496, 876.2124145, 823.8518794,
0, 451.6856761, 787.4862859, 1033.716816, 1211.391922, 1337.096696, 1423.750021, 1481.268901, 1517.141902, 1536.919616, 1544.629816, 1543.124711, 1534.367461, 1519.664836, 1499.85261, 1475.440009, 1446.71921, 1413.845556, 1376.893812, 1335.895423, 1290.861311, 1241.794321, 1188.848948, 1134.041411, 1079.233875, 1024.426339, 969.618803, 914.8112669, 860.0037308, 805.1961946, 750.3886585,
0, 456.1102377, 791.3443947, 1034.384527, 1207.808884, 1329.16979, 1411.924184, 1466.230442, 1499.624529, 1517.587927, 1524.019178, 1521.620268, 1512.208462, 1496.963554, 1476.619872, 1451.61171, 1422.180192, 1388.44891, 1350.474951, 1308.281226, 1261.875277, 1211.258954, 1156.506365, 1099.722564, 1042.938763, 986.1549626, 929.3711618, 872.587361, 815.8035602, 759.0197594, 702.2359586,
0, 460.9970479, 796.3982096, 1037.171229, 1207.372024, 1325.442801, 1405.316916, 1457.35093, 1489.103123, 1505.976545, 1511.743514, 1508.967313, 1499.335663, 1483.919393, 1463.368603, 1438.057434, 1408.187475, 1373.858664, 1335.115473, 1291.975012, 1244.44265, 1192.519633, 1136.206141, 1077.683652, 1019.147399, 960.6111466, 902.0748941, 843.5386415, 785.0023889, 726.4661363, 667.9298838,
0, 466.0901241, 802.0373299, 1041.0558, 1208.624468, 1324.020771, 1401.608652, 1451.909865, 1482.485528, 1498.652869, 1504.060099, 1501.140394, 1491.464079, 1476.006236, 1455.345141, 1429.805213, 1399.556409, 1364.68041, 1325.212322, 1281.165135, 1232.542751, 1179.346029, 1121.575087, 1061.407911, 1001.131607, 940.8553027, 880.5789986, 820.3026944, 760.0263903, 699.7500861, 639.4737819,
0, 471.1838206, 807.7980299, 1045.28553, 1210.509691, 1323.540501, 1399.135353, 1447.954112, 1477.544792, 1493.13291, 1498.24452, 1495.18986, 1485.43175, 1469.860039, 1448.990842, 1423.106768, 1392.351987, 1356.793779, 1316.460132, 1271.36103, 1221.499349, 1166.875697, 1107.490151, 1045.510245, 983.316923, 921.1236009, 858.9302789, 796.7369568, 734.5436348, 672.3503127, 610.1569907,
0, 476.1028508, 813.3024509, 1049.262323, 1212.199359, 1322.939716, 1396.602196, 1443.961596, 1472.539539, 1487.465392, 1492.145334, 1488.773263, 1478.713147, 1462.778528, 1441.431326, 1414.918685, 1383.363533, 1346.821657, 1305.315558, 1258.852992, 1207.436109, 1151.065343, 1089.740748, 1025.609055, 961.147178, 896.6853005, 832.2234231, 767.7615457, 703.2996683, 638.8377909, 574.3759134,
0, 480.6959313, 818.2434015, 1052.515595, 1213.051772, 1321.399519, 1393.009657, 1438.751653, 1466.110048, 1480.115527, 1484.057773, 1480.021245, 1469.279382, 1452.577593, 1430.330619, 1402.755541, 1369.957778, 1331.983935, 1288.852273, 1240.568889, 1187.135415, 1128.552168, 1064.819187, 998.0416319, 930.7943711, 863.5471103, 796.2998495, 729.0525888, 661.805328, 594.5580672, 527.3108065,
0, 484.8379779, 822.4003795, 1054.734407, 1212.657846, 1318.400328, 1387.715572, 1431.550224, 1457.344687, 1470.031883, 1472.7911, 1467.606052, 1455.668914, 1437.665401, 1413.969937, 1384.774571, 1350.17055, 1310.19727, 1264.869777, 1214.192955, 1158.168074, 1096.795375, 1030.074883, 960.0215749, 889.3093453, 818.5971157, 747.8848862, 677.1726566, 606.4604271, 535.7481975, 465.0359679,
0, 488.4409759, 825.6917101, 1055.86865, 1210.987652, 1313.905503, 1380.646886, 1422.222087, 1446.02707, 1456.90503, 1457.937128, 1451.019731, 1437.276199, 1417.342898, 1391.561757, 1360.1052, 1323.053694, 1280.441332, 1232.280884, 1178.576416, 1119.328966, 1054.538725, 984.2057174, 910.1391494, 835.1104046, 760.0816598, 685.052915, 610.0241702, 534.9954254, 459.9666806, 384.9379358,
0, 491.4747868, 828.27501, 1056.324587, 1208.674536, 1308.718724, 1372.713578, 1411.726892, 1433.123448, 1441.679368, 1440.402909, 1431.126665, 1414.924899, 1392.399726, 1363.871237, 1329.499155, 1289.357279, 1243.476597, 1191.868637, 1134.537059, 1071.4828, 1002.706036, 928.2067906, 849.3261481, 768.9253496, 688.5245512, 608.1237527, 527.7229542, 447.3221558, 366.9213573, 286.5205589,
0, 493.9947702, 830.7003866, 1057.271598, 1207.467472, 1305.058857, 1366.479602, 1402.862471, 1421.579958, 1427.391621, 1423.282959, 1411.066262, 1391.80096, 1366.079872, 1334.218116, 1296.373266, 1252.618512, 1202.984897, 1147.484165, 1086.120152, 1018.893886, 945.8055761, 866.8552515, 782.3603166, 695.2517778, 608.143239, 521.0347002, 433.9261614, 346.8176226, 259.7090839, 172.6005451,
0, 496.1531488, 834.0680907, 1060.998325, 1210.790003, 1307.300939, 1367.054612, 1401.2742, 1417.419632, 1420.331002, 1413.064402, 1397.490965, 1374.716745, 1345.36959, 1309.789485, 1268.150587, 1220.536382, 1166.983795, 1107.507611, 1042.113046, 970.8016651, 893.5738429, 810.4296456, 721.3690807, 627.5645552, 531.9742528, 436.3839503, 340.7936479, 245.2033454, 149.613043, 54.02274052,
0, 498.1135552, 839.9762446, 1070.993853, 1224.019279, 1322.479123, 1382.814261, 1416.362806, 1430.794342, 1431.187031, 1420.823919, 1401.771442, 1375.29154, 1342.128801, 1302.705545, 1257.250594, 1205.881451, 1148.654732, 1085.595699, 1016.714613, 942.0151744, 861.4985317, 775.1649759, 683.0145626, 585.0472992, 481.3151465, 374.7265294, 268.1379123, 161.5492952, 54.96067808, -51.62793903,
0, 499.678107, 849.702922, 1090.847254, 1253.322192, 1359.243849, 1424.591538, 1460.747228, 1475.693114, 1474.932546, 1462.189369, 1439.931648, 1409.757923, 1372.677252, 1329.308429, 1280.018687, 1225.017963, 1164.42121, 1098.288293, 1026.648598, 949.5155672, 866.8948462, 778.788567, 685.1974342, 586.1216433, 481.5612373, 371.516223, 255.986601, 136.1507912, 14.75109717, -106.6485968,
0, 499.3655305, 861.7860046, 1120.420107, 1300.813247, 1422.47209, 1500.151605, 1544.899709, 1564.898727, 1566.1377, 1552.944806, 1528.405025, 1494.684405, 1453.279011, 1405.203703, 1351.133385, 1291.507129, 1226.603683, 1156.595232, 1081.584909, 1001.63236, 916.7707282, 827.0175946, 732.3817967, 632.867501, 528.4765276, 419.2096002, 305.0669754, 186.0487314, 62.15488809, -66.61455086,
0, 493.7338107, 870.696987, 1154.062625, 1362.667956, 1511.741692, 1613.52415, 1677.793666, 1712.311404, 1723.195408, 1715.233523, 1692.143765, 1656.789705, 1611.357546, 1557.500748, 1496.45733, 1429.144272, 1356.232904, 1278.208562, 1195.417361, 1108.102502, 1016.432123, 920.5204276, 820.4434847, 716.2508817, 607.9741777, 495.6329276, 379.2388883, 258.7988914, 134.3167547, 5.794518555,
0, 473.7220856, 855.0896169, 1157.985871, 1394.339745, 1574.370894, 1706.808016, 1799.082797, 1857.501807, 1887.398483, 1893.267165, 1878.88098, 1847.39523, 1801.437809, 1743.188026, 1674.445108, 1596.687527, 1511.124204, 1418.738546, 1320.326154, 1216.526997, 1107.852739, 994.7098428, 877.4190113, 756.2314624, 631.3424814, 502.9026399, 371.0270273, 235.8027968, 97.29529188, -44.44701832,
0, 425.7420801, 783.0502501, 1079.250865, 1320.940071, 1514.048304, 1663.89984, 1775.267716, 1852.424338, 1899.188042, 1918.965902, 1914.793022, 1889.368573, 1845.088789, 1784.07715, 1708.211943, 1619.151404, 1518.356621, 1407.112354, 1286.545946, 1157.644467, 1021.270233, 878.1748278, 729.0117509, 574.347808, 414.6733473, 250.4114415, 81.92610819, -90.4703466, -266.5107818, -445.9673988,
0, 544.4498375, 973.3200663, 1307.67626, 1565.254538, 1760.876891, 1906.829722, 2013.207401, 2088.222639, 2138.485465, 2169.25258, 2184.648853, 2187.862711, 2181.317146, 2166.818079, 2145.681777, 2118.843002, 2086.945591, 2050.417093, 2009.529117, 1964.444988, 1915.256309, 1862.009985, 1804.727247, 1743.416153, 1678.079054, 1608.766186, 1537.490348, 1466.21451, 1394.938673, 1323.662835,
0, 546.313886, 970.0963972, 1295.26706, 1541.62753, 1725.437161, 1859.928335, 1955.765571, 2021.452296, 2063.689094, 2087.687116, 2097.440258, 2095.959578, 2085.473344, 2067.595947, 2043.468855, 2013.876603, 1979.340737, 1940.194463, 1896.640658, 1848.795704, 1796.721527, 1740.447998, 1679.987741, 1615.345181, 1546.521494, 1473.516877, 1398.395993, 1323.249296, 1248.102599, 1172.955903,
0, 549.8431644, 971.4270531, 1291.074888, 1530.289681, 1706.484285, 1833.625934, 1922.802087, 1982.714142, 2020.105302, 2040.12862, 2046.660968, 2042.56841, 2029.928176, 2010.212171, 1984.436653, 1953.282458, 1917.189839, 1876.431725, 1831.168894, 1781.490278, 1727.44131, 1669.042926, 1606.303521, 1539.225869, 1467.810655, 1392.057987, 1314.02939, 1235.893587, 1157.757784, 1079.621981,
0, 554.2823903, 975.0950863, 1290.9951, 1525.036967, 1695.662252, 1817.475167, 1901.914716, 1957.833242, 1991.990744, 2009.473766, 2014.04716, 2008.446507, 1994.618438, 1973.915629, 1947.252709, 1915.228816, 1878.222096, 1836.460875, 1790.075837, 1739.137009, 1683.678923, 1623.716873, 1559.256726, 1490.300321, 1416.848086, 1338.900086, 1258.509718, 1177.924864, 1097.34001, 1016.755156,
0, 559.2420792, 980.0914936, 1293.260064, 1523.273294, 1689.528105, 1807.204599, 1888.046754, 1941.026048, 1972.901035, 1988.685049, 1992.033286, 1985.559683, 1971.093148, 1949.881901, 1922.753869, 1890.240323, 1852.669186, 1810.233739, 1763.041725, 1711.14923, 1654.583034, 1593.354582, 1527.468086, 1456.924809, 1381.72503, 1301.868788, 1219.396492, 1136.636373, 1053.876253, 971.116133,
0, 564.4471893, 985.7611353, 1296.770383, 1523.421921, 1686.017856, 1800.269879, 1878.191966, 1928.850241, 1958.987471, 1973.538253, 1976.04958, 1969.020172, 1954.170686, 1932.655721, 1905.227366, 1872.35895, 1834.33658, 1791.32511, 1743.414192, 1690.649245, 1633.051316, 1570.629102, 1503.385689, 1431.32196, 1354.438103, 1272.734143, 1188.232522, 1103.343289, 1018.454057, 933.5648248,
0, 569.6761078, 991.6047687, 1300.714276, 1524.339672, 1683.650221, 1794.853937, 1870.20627, 1918.847818, 1947.493715, 1960.995284, 1962.792544, 1955.274277, 1940.060578, 1918.221107, 1890.440693, 1857.142423, 1818.576995, 1774.885773, 1726.143853, 1672.388312, 1613.635852, 1549.893159, 1481.162523, 1407.444576, 1328.739444, 1245.047144, 1158.366248, 1071.190341, 984.0144331, 896.8385255,
0, 574.7392215, 997.2136022, 1304.444122, 1525.128253, 1681.270687, 1789.545542, 1862.425175, 1909.107396, 1936.269787, 1948.67688, 1949.663171, 1941.512354, 1925.75073, 1903.370473, 1874.99635, 1841.007549, 1801.624503, 1756.968975, 1707.104214, 1652.060689, 1591.851763, 1526.482627, 1455.955001, 1380.269342, 1299.425739, 1213.424201, 1124.230039, 1034.422219, 944.6143993, 854.8065796,
0, 579.4715378, 1002.251136, 1307.444117, 1525.085011, 1677.983612, 1783.252065, 1853.557473, 1898.139906, 1923.631423, 1934.707668, 1934.599912, 1925.491904, 1908.822755, 1885.514099, 1856.137132, 1821.032695, 1780.395349, 1734.330478, 1682.891668, 1626.104145, 1563.978694, 1496.519391, 1423.727543, 1345.603488, 1262.147288, 1173.35895, 1081.150513, 988.1880455, 895.2255783, 802.2631111,
0, 583.7336453, 1006.465895, 1309.357869, 1523.74426, 1673.2043, 1775.258844, 1842.748803, 1884.944016, 1908.425864, 1917.781643, 1916.143827, 1905.602885, 1887.518262, 1862.74841, 1831.817493, 1795.033355, 1752.56877, 1704.515633, 1650.919768, 1591.802298, 1527.172047, 1457.03227, 1381.383984, 1300.227444, 1213.562695, 1121.389742, 1025.519144, 928.7050037, 831.890863, 735.0767224,
0, 587.4205991, 1009.738529, 1310.083816, 1521.020288, 1666.843546, 1765.447386, 1829.825816, 1869.269769, 1890.310114, 1897.451807, 1893.737826, 1881.175942, 1861.056337, 1834.183592, 1801.042545, 1761.914026, 1716.953355, 1666.241879, 1609.81951, 1547.70433, 1479.90376, 1406.420488, 1327.255338, 1242.408511, 1151.880044, 1055.669941, 955.3715786, 853.8257734, 752.2799681, 650.7341629,
0, 590.4795318, 1012.174436, 1309.962883, 1517.489087, 1659.672195, 1754.72789, 1815.782003, 1852.145237, 1870.308856, 1874.712795, 1868.330102, 1853.104906, 1830.27503, 1800.605141, 1764.551591, 1722.376121, 1674.222064, 1620.163749, 1560.237293, 1494.458912, 1422.83521, 1345.368563, 1262.059698, 1172.908795, 1077.915887, 977.0809776, 871.5170076, 764.1449779, 656.7729482, 549.4009185,
0, 592.933189, 1014.247471, 1310.077517, 1514.84305, 1653.914428, 1745.747475, 1803.576943, 1836.745901, 1851.737493, 1852.965706, 1843.373118, 1824.875856, 1798.689206, 1765.561353, 1725.937573, 1680.072765, 1628.106413, 1570.110915, 1506.121565, 1436.154312, 1360.215725, 1278.308211, 1190.432532, 1096.588882, 996.777301, 890.9977935, 779.3244654, 664.7412796, 550.1580939, 435.5749081,
0, 594.8898682, 1016.956217, 1312.613367, 1516.470098, 1654.029914, 1743.848034, 1799.238282, 1829.610082, 1841.504086, 1839.384664, 1826.238223, 1804.0183, 1773.971258, 1736.870261, 1693.17993, 1643.169532, 1586.988741, 1524.716816, 1456.393401, 1382.037021, 1301.655645, 1215.252373, 1122.828264, 1024.383625, 919.9185259, 809.43298, 692.9269881, 571.3551128, 447.7275771, 324.1000413,
0, 596.4709459, 1021.809993, 1320.999742, 1527.807595, 1667.30667, 1757.89633, 1812.907534, 1841.866288, 1851.477844, 1846.384818, 1829.744066, 1803.659762, 1769.503764, 1728.148717, 1680.134597, 1625.785277, 1565.288254, 1498.747749, 1426.219034, 1347.729849, 1263.293236, 1172.914865, 1076.596973, 974.3403435, 866.1452119, 752.0116356, 631.9396252, 505.929182, 373.9803061, 238.8926209,
0, 597.5404606, 1030.33502, 1339.354282, 1555.904641, 1703.68573, 1800.460209, 1859.398811, 1890.15511, 1899.716894, 1893.074308, 1873.738877, 1844.142294, 1805.939137, 1760.233667, 1707.747289, 1648.940247, 1584.09849, 1513.394468, 1436.928733, 1354.757709, 1266.911681, 1173.406096, 1074.248363, 969.4417949, 858.9877446, 742.8867099, 621.1388505, 493.7442091, 360.7027949, 222.0146093,
0, 598.1726005, 1046.511076, 1377.906891, 1618.430302, 1788.602137, 1904.456579, 1978.424431, 2020.062863, 2036.654584, 2033.696521, 2015.295542, 1984.486466, 1943.485517, 1893.890549, 1836.837708, 1773.122738, 1703.293854, 1627.72197, 1546.653097, 1460.246843, 1368.604256, 1271.787576, 1169.833975, 1062.76489, 950.5921945, 833.3221716, 710.9579695, 583.5010685, 450.9521136, 313.3113611,
0, 592.5245808, 1056.439565, 1415.139993, 1688.021611, 1891.092474, 2037.50255, 2138.000682, 2201.327432, 2234.551471, 2243.356457, 2232.28463, 2204.942704, 2164.17506, 2112.208684, 2050.773808, 1981.203738, 1904.516965, 1821.484257, 1732.683093, 1638.541505, 1539.37309, 1435.404751, 1326.798442, 1213.668074, 1096.09248, 974.12527, 847.8021954, 717.146596, 582.173349, 442.891689,
0, 572.402358, 1041.589825, 1422.079082, 1726.479121, 1965.714332, 2149.224859, 2285.146153, 2380.469587, 2441.185805, 2472.41239, 2478.507311, 2463.169486, 2429.52771, 2380.219089, 2317.45804, 2243.096813, 2158.678428, 2065.482832, 1964.567025, 1856.799813, 1742.891815, 1623.421264, 1498.856118, 1369.572927, 1235.872853, 1097.99523, 956.1289737, 810.422129, 660.9898334, 507.9209052,
0, 524.7002895, 971.7997049, 1349.219386, 1664.12889, 1923.009352, 2131.712033, 2295.512557, 2419.161081, 2506.928685, 2562.65019, 2589.763666, 2591.346824, 2570.150511, 2528.629501, 2468.970766, 2393.119394, 2302.802336, 2199.55012, 2084.716683, 1959.497471, 1824.945911, 1681.988406, 1531.43794, 1374.006428, 1210.315879, 1040.908501, 866.2558011, 686.7667921, 502.7953648, 314.6469004,
0, 655.6583449, 1183.952939, 1606.036112, 1939.982287, 2201.148837, 2402.505556, 2554.934457, 2667.501574, 2747.702407, 2801.682618, 2834.435555, 2849.978137, 2851.506596, 2841.533549, 2822.007813, 2794.418348, 2759.883683, 2719.228107, 2673.045908, 2621.754857, 2565.640103, 2504.889628, 2439.622298, 2369.909557, 2295.79172, 2217.289779, 2134.413568, 2047.167075, 1955.551614, 1859.567503,
0, 657.2470105, 1179.902898, 1591.804632, 1913.038044, 2160.424493, 2347.958782, 2487.201661, 2587.630142, 2656.948724, 2701.364534, 2725.829254, 2734.250589, 2729.675935, 2714.450766, 2690.354155, 2658.713719, 2620.502177, 2576.417561, 2526.949043, 2472.43018, 2413.081283, 2349.042504, 2280.399082, 2207.200104, 2129.471982, 2047.227756, 1960.473174, 1869.210411, 1773.44013, 1673.162474,
0, 660.6297336, 1180.734066, 1586.402287, 1899.377502, 2137.66277, 2316.060292, 2446.64896, 2539.205347, 2601.572947, 2639.984265, 2659.340103, 2663.450158, 2655.238822, 2636.919839, 2610.143257, 2576.117879, 2535.712207, 2489.53666, 2438.009618, 2381.40966, 2319.916132, 2253.640004, 2182.646757, 2106.97287, 2026.637263, 1941.648901, 1852.011558, 1757.72659, 1658.794383, 1555.215016,
0, 665.0086095, 1184.103384, 1585.46192, 1892.340567, 2123.802607, 2295.359468, 2419.532691, 2506.343949, 2563.739998, 2597.958995, 2613.844258, 2615.111107, 2604.572056, 2584.325255, 2555.910702, 2520.438393, 2478.692236, 2431.213211, 2378.364952, 2320.384583, 2257.421375, 2189.565471, 2116.868679, 2039.359035, 1957.050625, 1869.949891, 1778.059436, 1681.380156, 1579.912292, 1473.655892,
0, 669.9690548, 1188.938296, 1587.096817, 1889.138226, 2115.116588, 2281.193099, 2400.28415, 2482.620743, 2536.22855, 2567.337263, 2580.727225, 2580.020755, 2567.924973, 2546.432349, 2516.984711, 2480.605849, 2438.007421, 2389.672358, 2335.919504, 2276.952828, 2212.898107, 2143.829629, 2069.789066, 1990.798381, 1906.868292, 1818.003543, 1724.205986, 1625.476229, 1521.814432, 1413.220621,
0, 675.2170597, 1194.53965, 1590.130917, 1888.076346, 2109.374787, 2270.795314, 2385.614097, 2464.24444, 2514.772099, 2543.407063, 2554.862025, 2552.666905, 2539.427951, 2517.039125, 2486.852731, 2449.81552, 2406.57584, 2357.566735, 2303.069327, 2243.260234, 2178.246281, 2108.089251, 2032.823026, 1952.465028, 1867.023526, 1776.502076, 1680.902017, 1580.223773, 1474.467451, 1363.633068,
0, 680.5150372, 1200.372935, 1593.693935, 1887.925931, 2104.978832, 2262.197029, 2373.188329, 2448.526237, 2496.341017, 2522.813581, 2532.58501, 2529.093228, 2514.847247, 2491.64828, 2460.766032, 2423.077498, 2379.174739, 2329.447259, 2274.143869, 2213.418222, 2147.361548, 2076.025574, 1999.438059, 1917.612944, 1830.556679, 1738.271959, 1640.759766, 1538.020402, 1430.053938, 1316.860386,
0, 685.6590053, 1205.998437, 1597.088696, 1887.718547, 2100.681591, 2253.870068, 2361.199301, 2433.383978, 2478.585504, 2502.9478, 2511.037142, 2506.200035, 2490.851574, 2466.705307, 2434.954324, 2396.412051, 2351.620138, 2300.929773, 2244.561843, 2182.650522, 2115.274075, 2042.476037, 1964.279297, 1880.695094, 1791.728501, 1697.381581, 1597.655065, 1492.549167, 1382.06394, 1266.199389,
0, 690.4702084, 1211.050144, 1599.75347, 1886.689095, 2095.508383, 2244.625827, 2348.24219, 2417.196334, 2459.669264, 2481.761465, 2487.962304, 2481.528125, 2464.783711, 2439.359976, 2406.379057, 2366.596499, 2320.508805, 2268.433427, 2210.567093, 2147.027421, 2077.881874, 2003.167301, 1922.902699, 1837.097199, 1745.754829, 1648.877186, 1546.46482, 1438.517888, 1325.036425, 1206.020436,
0, 694.7948489, 1215.245049, 1601.284384, 1884.311152, 2088.803161, 2233.66943, 2333.37415, 2398.864707, 2438.332156, 2457.82928, 2461.768454, 2453.31881, 2434.719723, 2407.525379, 2372.79313, 2331.226517, 2283.282141, 2229.248121, 2169.300535, 2103.543111, 2032.034413, 1954.805896, 1871.873486, 1783.244691, 1688.922753, 1588.90893, 1483.203643, 1371.807011, 1254.719059, 1131.93979,
0, 698.5109729, 1218.424773, 1601.523531, 1880.433424, 2080.409595, 2220.819171, 2316.36509, 2378.08872, 2414.184803, 2430.658245, 2431.848573, 2420.844534, 2399.808267, 2370.225718, 2333.097537, 2289.082461, 2238.603262, 2181.923606, 2119.202659, 2050.532982, 1975.966128, 1895.529392, 1809.236386, 1717.093427, 1619.103199, 1515.266722, 1405.584334, 1290.056128, 1168.682122, 1041.462322,
0, 701.5427801, 1220.639329, 1600.735604, 1875.553285, 2071.034734, 2206.948309, 2298.204145, 2355.924548, 2388.307772, 2401.318977, 2399.237741, 2385.087444, 2360.968242, 2328.312105, 2288.075511, 2240.882872, 2187.131503, 2127.06702, 2060.836363, 1988.524208, 1910.177279, 1825.820087, 1735.464747, 1639.116831, 1536.778688, 1428.451206, 1314.134678, 1193.829184, 1067.534741, 935.2513525,
0, 703.8796163, 1222.278972, 1599.893339, 1871.26397, 2062.847668, 2194.71604, 2281.940858, 2335.714969, 2364.251832, 2373.50106, 2367.712174, 2349.874168, 2322.054376, 2285.656482, 2241.614301, 2190.535136, 2132.804067, 2068.65842, 1998.239823, 1921.629747, 1838.873118, 1749.993523, 1655.002649, 1553.905902, 1446.705582, 1333.402572, 1213.99717, 1088.48946, 956.8794625, 819.1671796,
0, 705.5828814, 1224.223589, 1601.034721, 1870.839868, 2060.285986, 2189.573233, 2273.861673, 2324.406684, 2349.468006, 2355.031299, 2345.375497, 2323.514421, 2291.536714, 2250.864378, 2202.4468, 2146.904271, 2084.632428, 2015.87692, 1940.785716, 1859.44494, 1771.902783, 1678.185021, 1578.304727, 1472.268132, 1360.077992, 1241.735421, 1117.240819, 986.5943121, 849.7959333, 706.8456891,
0, 707.627833, 1231.013717, 1613.441182, 1888.578601, 2082.40334, 2214.785798, 2300.785033, 2351.70113, 2375.92367, 2379.610357, 2367.225142, 2341.961012, 2306.068819, 2261.110259, 2208.150198, 2147.900985, 2080.829192, 2007.233343, 1927.299508, 1841.140308, 1748.821685, 1650.380812, 1545.837755, 1435.202809, 1318.480954, 1195.674434, 1066.784177, 931.8105278, 790.7536005, 643.6134256,
0, 709.7157881, 1243.72422, 1640.631577, 1931.037301, 2139.026917, 2283.419609, 2378.805034, 2436.399943, 2464.751494, 2470.310864, 2457.89775, 2431.073631, 2392.439263, 2343.869685, 2286.698089, 2221.858194, 2149.993248, 2071.538444, 1986.782411, 1895.912401, 1799.046952, 1696.259076, 1587.592374, 1473.071986, 1352.711839, 1226.5193, 1094.498059, 956.6498529, 812.975437, 663.4751115,
0, 709.4681452, 1257.795132, 1676.662759, 1991.891005, 2224.410128, 2391.091124, 2505.452206, 2578.257116, 2618.019453, 2631.425663, 2623.687923, 2598.8369, 2559.963172, 2509.415047, 2448.959558, 2379.912534, 2303.242864, 2219.655379, 2129.656134, 2033.603334, 1931.746622, 1824.25706, 1711.249702, 1592.800355, 1468.957847, 1339.752838, 1205.204043, 1065.322524, 920.1145848, 769.58367,
0, 703.0228836, 1266.035774, 1712.262578, 2061.31724, 2329.704688, 2531.260976, 2677.538877, 2778.144701, 2841.031576, 2872.754009, 2878.68806, 2863.221115, 2829.914827, 2781.644465, 2720.7176, 2648.974734, 2567.87424, 2478.563685, 2381.939431, 2278.696148, 2169.367714, 2054.360784, 1933.982162, 1808.460959, 1677.966395, 1542.621974, 1402.516691, 1257.713781, 1108.257504, 954.1783276,
0, 682.483604, 1250.914558, 1720.218308, 2103.498677, 2412.235472, 2656.463419, 2844.933924, 2985.261056, 3084.053057, 3147.030599, 3179.132905, 3184.612813, 3167.121742, 3129.785478, 3075.271624, 3005.849483, 2923.443113, 2829.67819, 2725.92332, 2613.326344, 2492.84615, 2365.280481, 2231.290147, 2091.420058, 1946.117419, 1795.747421, 1640.606715, 1480.934953, 1316.924606, 1148.729308,
0, 634.8070421, 1182.546727, 1651.650103, 2049.790259, 2383.942638, 2660.441182, 2885.030567, 3062.91474, 3198.801992, 3296.946773, 3361.188443, 3394.987151, 3401.457023, 3383.396833, 3343.318303, 3283.472208, 3205.872414, 3112.317989, 3004.413529, 2883.587806, 2751.110863, 2608.109668, 2455.582417, 2294.411602, 2125.375921, 1949.161121, 1766.369858, 1577.530641, 1383.105946, 1183.499548,
0, 779.6225497, 1420.161263, 1942.680877, 2365.425211, 2704.126796, 2972.290434, 3181.452187, 3341.415224, 3460.463948, 3545.557747, 3602.505695, 3636.123481, 3650.373796, 3648.491362, 3633.093775, 3606.27923, 3569.712232, 3524.698275, 3472.248492, 3413.135189, 3347.939168, 3277.089669, 3200.897742, 3119.5838, 3033.300064, 2942.148576, 2846.195389, 2745.481517, 2640.031184, 2529.857835,
0, 780.8441985, 1415.149823, 1926.490538, 2335.04698, 2658.014704, 2909.973312, 3103.219438, 3248.066136, 3353.111106, 3425.47612, 3471.019867, 3494.52637, 3499.87099, 3490.165966, 3467.887323, 3434.984879, 3392.976996, 3343.031623, 3286.035085, 3222.649969, 3153.36339, 3078.526808, 2998.388496, 2913.119663, 2822.835157, 2727.60959, 2627.489653, 2522.503288, 2412.666343, 2297.987232,
0, 783.9897294, 1415.342171, 1919.726799, 2318.918392, 2631.297021, 2872.296206, 3054.80325, 3189.515582, 3285.256717, 3349.255261, 3387.390213, 3404.405621, 3404.097457, 3389.475442, 3362.902336, 3326.213066, 3280.81591, 3227.777774, 3167.895472, 3101.754744, 3029.778644, 2952.266736, 2869.426462, 2781.397872, 2688.272804, 2590.109488, 2486.943412, 2378.795217, 2265.67625, 2147.592331,
0, 788.2201502, 1418.276116, 1917.775347, 2309.948369, 2614.241877, 2846.845913, 3021.160756, 3148.208829, 3236.996584, 3294.831059, 3327.595473, 3339.987944, 3335.727156, 3317.728493, 3288.253948, 3249.038817, 3201.397984, 3146.314346, 3084.511731, 3016.514424, 2942.695235, 2863.31384, 2778.54694, 2688.511635, 2593.2832, 2492.908355, 2387.414927, 2276.818701, 2161.128121, 2040.347389,
0, 793.0961159, 1422.817477, 1918.634789, 2305.162231, 2602.847265, 2828.579078, 2996.223169, 3117.08911, 3200.338048, 3253.336052, 3281.958957, 3290.853964, 3283.662819, 3263.211046, 3231.667302, 3190.6766, 3141.470794, 3084.959418, 3021.803655, 2952.475941, 2877.307429, 2796.525309, 2710.281711, 2618.675741, 2521.769954, 2419.602423, 2312.19535, 2199.561044, 2081.705927, 1958.633111,
0, 798.3047143, 1428.223776, 1921.054057, 2302.751191, 2594.721031, 2814.512647, 2976.42264, 3092.019823, 3170.598657, 3219.569178, 3244.79054, 3250.854701, 3241.326242, 3218.943758, 3185.787791, 3143.419767, 3092.995997, 3035.360342, 2971.118794, 2900.698824, 2824.396042, 2742.410372, 2654.873675, 2561.870479, 2463.453228, 2359.65326, 2250.488487, 2135.968632, 2016.09865, 1890.880878,
0, 803.5923675, 1433.925476, 1924.104997, 2301.401467, 2588.149061, 2802.527052, 2959.236356, 3070.08256, 3144.47593, 3189.857972, 3212.063331, 3215.624989, 3204.029992, 3179.932257, 3145.32833, 3101.701374, 3050.138088, 2991.422735, 2926.111957, 2854.593623, 2777.132505, 2693.905242, 2605.026664, 2510.569257, 2410.577263, 2305.076656, 2194.082007, 2077.601064, 1955.637687, 1828.193658,
0, 808.7406859, 1439.45199, 1927.040986, 2300.07455, 2581.791548, 2790.976953, 2942.713333, 3049.023815, 3119.4194, 3161.362318, 3180.656021, 3181.771386, 3168.117724, 3142.266311, 3106.133318, 3061.128242, 3008.273243, 2948.298115, 2881.715038, 2808.876699, 2730.020874, 2645.304123, 2554.826812, 2458.651371, 2356.815306, 2249.340271, 2136.238194, 2017.515295, 1893.17461, 1763.217503,
0, 813.5571219, 1444.407724, 1929.25397, 2297.941875, 2574.592982, 2778.575248, 2925.332082, 3027.086708, 3093.437449, 3131.857935, 3148.114896, 3146.614834, 3130.689676, 3102.830365, 3064.876335, 3018.167836, 2963.667214, 2902.054474, 2833.801702, 2759.230296, 2678.554358, 2591.91309, 2499.394547, 2401.052733, 2296.919605, 2187.013319, 2071.343706, 1949.915797, 1822.731999, 1689.793371,
0, 817.8735108, 1448.477924, 1930.291327, 2294.413231, 2565.820802, 2764.439765, 2906.052851, 3003.066447, 3065.154001, 3099.792799, 3112.708982, 3108.243977, 3089.654034, 3059.353167, 3019.10852, 2970.196044, 2913.523302, 2849.725296, 2779.238347, 2702.356316, 2619.272758, 2530.112021, 2434.951788, 2333.839067, 2226.80129, 2113.853809, 1995.004806, 1870.258396, 1739.616512, 1603.079987,
0, 821.5510051, 1451.464674, 1929.935677, 2289.264336, 2555.239273, 2748.308946, 2884.569284, 2976.591823, 3034.114133, 3064.61192, 3073.769992, 3065.867255, 3044.089078, 3010.778668, 2967.637606, 2915.884307, 2856.377943, 2789.714281, 2716.298868, 2636.402191, 2550.200602, 2457.806201, 2359.288242, 2254.688147, 2144.029793, 2027.326372, 1904.58482, 1775.808575, 1640.999229, 1500.157457,
0, 824.4911159, 1453.361305, 1928.368335, 2282.897585, 2543.46502, 2730.983648, 2861.82516, 2948.70494, 3001.415529, 3027.430662, 3032.399403, 3020.547471, 2995.000681, 2958.043452, 2911.323576, 2856.012842, 2792.931712, 2722.644976, 2645.534236, 2561.852054, 2471.761804, 2375.366501, 2272.72926, 2163.887517, 2048.862665, 1927.666423, 1800.304898, 1666.781094, 1527.09639, 1381.251366,
0, 826.6625805, 1454.515422, 1926.522559, 2276.903291, 2532.731486, 2715.277242, 2841.126465, 2923.109905, 2971.069496, 2992.486467, 2992.992687, 2976.783915, 2946.951199, 2905.744388, 2854.779791, 2795.202223, 2727.810106, 2653.150959, 2571.59334, 2483.380325, 2388.668634, 2287.556798, 2180.105033, 2066.348984, 1946.308999, 1819.996221, 1687.416466, 1548.572602, 1403.465953, 1252.097082,
0, 830.0223732, 1462.547482, 1939.514356, 2294.474173, 2554.096932, 2739.439843, 2867.009724, 2949.648087, 2997.2643, 3017.439215, 3015.918944, 2997.015974, 2963.932596, 2919.019636, 2863.981636, 2800.038094, 2728.048879, 2648.610746, 2562.130715, 2468.881152, 2369.040511, 2262.723015, 2149.99989, 2030.914268, 1905.491416, 1773.74559, 1635.684487, 1491.312062, 1340.630228, 1183.639851,
0, 832.5244039, 1471.452119, 1956.564095, 2319.946195, 2587.324857, 2779.198851, 2911.793739, 2997.862843, 3047.355874, 3067.973966, 3065.627696, 3044.812651, 3008.915313, 2960.460389, 2901.30925, 2832.817819, 2755.961059, 2671.430189, 2579.707789, 2481.125165, 2375.905617, 2264.19662, 2146.093409, 2021.655958, 1890.920992, 1753.910299, 1610.636347, 1461.105985, 1305.322799, 1143.288563,
0, 833.8151763, 1482.130382, 1980.885432, 2359.473505, 2641.809574, 2847.245697, 2991.351946, 3086.579805, 3142.823187, 3167.890597, 3167.900486, 3147.610506, 3110.690173, 3059.945254, 2997.501259, 2924.952428, 2843.481817, 2753.957306, 2657.007702, 2553.08251, 2442.498393, 2325.474902, 2202.161622, 2072.658534, 1937.031059, 1795.321009, 1647.554417, 1493.747017, 1333.908006, 1168.042563,
0, 832.5433817, 1493.58028, 2013.216171, 2416.599466, 2724.651073, 2954.699913, 3121.034687, 3235.381335, 3307.314697, 3344.61213, 3353.556071, 3339.191832, 3305.5463, 3255.812618, 3192.505366, 3117.590292, 3032.592163, 2938.68391, 2836.759848, 2727.495422, 2611.395614, 2488.833854, 2360.083061, 2225.340171, 2084.745344, 1938.396844, 1786.362443, 1628.68805, 1465.404156, 1296.530571,
0, 825.0702705, 1499.334996, 2045.506297, 2483.09838, 2828.828736, 3096.975385, 3299.694276, 3447.300592, 3548.517469, 3610.695324, 3640.004756, 3641.605722, 3619.795473, 3578.137526, 3519.573739, 3446.521373, 3360.956863, 3264.487847, 3158.41486, 3043.783958, 2921.431411, 2792.021496, 2656.078287, 2514.012275, 2366.142524, 2212.715019, 2053.91776, 1889.893093, 1720.747732, 1546.56082,
0, 803.8170356, 1482.941719, 2052.482873, 2525.84456, 2914.896837, 3230.131633, 3480.804854, 3675.065725, 3820.074365, 3922.108474, 3986.660009, 4018.522632, 4021.870675, 4000.330327, 3957.043686, 3894.726285, 3815.718657, 3722.032458, 3615.391642, 3497.269131, 3368.919408, 3231.407411, 3085.634083, 2932.358914, 2772.219773, 2605.750305, 2433.395152, 2255.523223, 2072.439233, 1884.393701,
0, 755.8882491, 1415.080535, 1986.417164, 2477.988802, 2897.192693, 3250.78524, 3544.931114, 3785.249112, 3976.854933, 4124.401042, 4232.113802, 4303.82802, 4343.019066, 4352.832707, 4336.112792, 4295.426918, 4233.090203, 4151.187285, 4051.592658, 3935.989443, 3805.886711, 3662.635442, 3507.443209, 3341.387682, 3165.429025, 2980.42126, 2787.122686, 2586.2054, 2378.263997, 2163.823505};
  line_index = line / 10000;
  ev = regular_ev[propensity_twentile -1][risk_twentile -1][line_index];
  }
  else{
    double platinum_line[20][20]={
26, 25, 26, 27, 29, 30, 30, 29, 25, 21, 17, 14, 13, 12, 12, 12, 12, 10, 10, 11,
22, 22, 23, 23, 23, 24, 23, 22, 20, 17, 14, 13, 12, 11, 11, 11, 11, 9, 10, 10,
21, 21, 20, 20, 20, 20, 20, 19, 17, 15, 13, 12, 11, 11, 11, 11, 11, 9, 9, 10,
20, 19, 18, 18, 18, 18, 17, 17, 16, 14, 13, 12, 11, 11, 11, 11, 11, 9, 9, 10,
20, 18, 17, 16, 16, 16, 15, 15, 14, 13, 12, 11, 11, 11, 11, 11, 8, 9, 9, 9,
19, 16, 15, 14, 14, 14, 14, 13, 13, 12, 11, 11, 11, 11, 11, 11, 8, 8, 9, 9,
29, 23, 20, 19, 18, 18, 17, 17, 16, 15, 14, 13, 12, 12, 12, 12, 8, 8, 9, 9,
28, 21, 18, 16, 15, 15, 15, 15, 14, 14, 13, 12, 12, 12, 12, 12, 8, 8, 8, 9,
30, 30, 28, 25, 23, 22, 21, 21, 20, 19, 17, 16, 15, 14, 14, 14, 8, 8, 8, 9,
30, 30, 24, 21, 19, 18, 18, 17, 17, 16, 15, 15, 14, 14, 14, 13, 7, 8, 8, 8,
30, 28, 21, 18, 16, 15, 15, 15, 14, 14, 13, 13, 13, 13, 13, 13, 7, 8, 8, 8,
30, 30, 24, 20, 18, 17, 16, 15, 15, 14, 14, 14, 14, 14, 14, 7, 7, 7, 8, 8,
30, 28, 21, 17, 15, 14, 13, 13, 13, 13, 13, 12, 13, 13, 13, 6, 7, 7, 7, 8,
30, 24, 17, 15, 13, 12, 11, 11, 11, 11, 11, 11, 12, 12, 5, 6, 6, 7, 7, 8,
30, 19, 15, 12, 11, 10, 10, 10, 10, 10, 10, 11, 11, 5, 5, 6, 6, 7, 7, 8,
23, 15, 12, 11, 10, 9, 9, 9, 9, 9, 10, 4, 4, 5, 5, 6, 6, 7, 7, 8,
//15, 12, 10, 9, 8, 8, 0, 0, 1, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//5, 4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 4, 4, 4, 5,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    line = platinum_line[risk_twentile -1][propensity_twentile -1] * 10000;
//The line granted can not exceed 1.5 times of the maximum line the applicant currently held    
    unsigned int line_cap = highest_line * 1.5;
    if(highest_line == 0)
      line_cap = line;
    else if(line_cap > highest_line + 50000)
      line_cap = highest_line + 50000;
//Rounding the line granted to NT$0000
    if(line > line_cap)
      line = line_cap;
    line += 5000;
    line /= 10000;
    line *= 10000;

    double platinum_ev[20][20][31]= {
0, 0.726188831, 0.42360312, -1.135298616, -3.205861679, -5.276424741, -7.346987803, -9.417550866, -11.48811393, -13.55867699, -15.62924005, -17.69980312, -19.77036618, -21.84092924, -23.9114923, -25.98205537, -28.05261843, -30.12318149, -32.19374455, -34.26430761, -36.33487068, -38.40543374, -40.4759968, -42.54655986, -44.61712293, -46.68768599, -48.75824905, -50.82881211, -52.89937518, -54.96993824, -57.0405013,
0, 0.718581748, -0.234701031, -2.375991985, -4.768565259, -7.161138533, -9.553711806, -11.94628508, -14.33885835, -16.73143163, -19.1240049, -21.51657817, -23.90915145, -26.30172472, -28.69429799, -31.08687127, -33.47944454, -35.87201782, -38.26459109, -40.65716436, -43.04973764, -45.44231091, -47.83488418, -50.22745746, -52.62003073, -55.012604, -57.40517728, -59.79775055, -62.19032383, -64.5828971, -66.97547037,
0, 0.868099256, -0.273657587, -2.562272681, -5.046962628, -7.531652575, -10.01634252, -12.50103247, -14.98572242, -17.47041236, -19.95510231, -22.43979226, -24.92448221, -27.40917215, -29.8938621, -32.37855205, -34.86324199, -37.34793194, -39.83262189, -42.31731184, -44.80200178, -47.28669173, -49.77138168, -52.25607163, -54.74076157, -57.22545152, -59.71014147, -62.19483141, -64.67952136, -67.16421131, -69.64890126,
0, 1.1058164, -0.092888306, -2.468038671, -5.05020233, -7.632365989, -10.21452965, -12.79669331, -15.37885696, -17.96102062, -20.54318428, -23.12534794, -25.7075116, -28.28967526, -30.87183892, -33.45400257, -36.03616623, -38.61832989, -41.20049355, -43.78265721, -46.36482087, -48.94698453, -51.52914818, -54.11131184, -56.6934755, -59.27563916, -61.85780282, -64.43996648, -67.02213013, -69.60429379, -72.18645745,
0, 1.416871378, 0.239632617, -2.193940804, -4.879812706, -7.565684608, -10.25155651, -12.93742841, -15.62330031, -18.30917222, -20.99504412, -23.68091602, -26.36678792, -29.05265982, -31.73853173, -34.42440363, -37.11027553, -39.79614743, -42.48201933, -45.16789124, -47.85376314, -50.53963504, -53.22550694, -55.91137884, -58.59725075, -61.28312265, -63.96899455, -66.65486645, -69.34073835, -72.02661026, -74.71248216,
0, 1.80172021, 0.711887134, -1.757363023, -4.551432947, -7.345502871, -10.1395728, -12.93364272, -15.72771264, -18.52178257, -21.31585249, -24.10992242, -26.90399234, -29.69806227, -32.49213219, -35.28620211, -38.08027204, -40.87434196, -43.66841189, -46.46248181, -49.25655174, -52.05062166, -54.84469158, -57.63876151, -60.43283143, -63.22690136, -66.02097128, -68.81504121, -71.60911113, -74.40318106, -77.19725098,
0, 2.267498905, 1.331872977, -1.147795359, -4.051374679, -6.954953999, -9.858533319, -12.76211264, -15.66569196, -18.56927128, -21.4728506, -24.37642992, -27.28000924, -30.18358856, -33.08716788, -35.9907472, -38.89432652, -41.79790584, -44.70148516, -47.60506448, -50.5086438, -53.41222312, -56.31580244, -59.21938176, -62.12296108, -65.0265404, -67.93011972, -70.83369904, -73.73727836, -76.64085768, -79.544437,
0, 2.824927371, 2.117796578, -0.340965356, -3.351507854, -6.362050351, -9.372592849, -12.38313535, -15.39367784, -18.40422034, -21.41476284, -24.42530534, -27.43584784, -30.44639033, -33.45693283, -36.46747533, -39.47801783, -42.48856032, -45.49910282, -48.50964532, -51.52018782, -54.53073031, -57.54127281, -60.55181531, -63.56235781, -66.57290031, -69.5834428, -72.5939853, -75.6045278, -78.6150703, -81.62561279,
0, 3.486435409, 3.094203427, 0.695421972, -2.416778123, -5.528978217, -8.641178311, -11.75337841, -14.8655785, -17.97777859, -21.08997869, -24.20217878, -27.31437888, -30.42657897, -33.53877907, -36.65097916, -39.76317925, -42.87537935, -45.98757944, -49.09977954, -52.21197963, -55.32417973, -58.43637982, -61.54857992, -64.66078001, -67.7729801, -70.8851802, -73.99738029, -77.10958039, -80.22178048, -83.33398058,
0, 4.263607483, 4.289121557, 2.041288037, -1.125417152, -4.336701327, -7.547985502, -10.75926968, -13.97055385, -17.18183803, -20.3931222, -23.60440638, -26.81569055, -30.02697473, -33.2382589, -36.44954308, -39.66082725, -42.87211143, -46.0833956, -49.29467978, -52.50596395, -55.71724813, -58.9285323, -62.13981648, -65.35110065, -68.56238483, -71.773669, -74.98495318, -78.19623735, -81.40752153, -84.6188057,
0, 5.161492702, 5.728232567, 3.796263426, 0.675485182, -2.650685275, -5.976855731, -9.303026188, -12.62919665, -15.9553671, -19.28153756, -22.60770802, -25.93387847, -29.26004893, -32.58621939, -35.91238984, -39.2385603, -42.56473076, -45.89090121, -49.21707167, -52.54324213, -55.86941258, -59.19558304, -62.5217535, -65.84792395, -69.17409441, -72.50026487, -75.82643532, -79.15260578, -82.47877624, -85.80494669,
0, 6.164461059, 7.416347688, 5.947021872, 2.913396953, -0.599188019, -4.111772992, -7.624357964, -11.13694294, -14.64952791, -18.16211288, -21.67469785, -25.18728283, -28.6998678, -32.21245277, -35.72503774, -39.23762272, -42.75020769, -46.26279266, -49.77537763, -53.28796261, -56.80054758, -60.31313255, -63.82571752, -67.3383025, -70.85088747, -74.36347244, -77.87605741, -81.38864239, -84.90122736, -88.41381233,
0, 7.200551709, 9.276981295, 8.440000222, 5.485323941, 1.578419212, -2.328485518, -6.235390247, -10.14229498, -14.04919971, -17.95610443, -21.86300916, -25.76991389, -29.67681862, -33.58372335, -37.49062808, -41.39753281, -45.30443754, -49.21134227, -53.118247, -57.02515173, -60.93205646, -64.83896119, -68.74586592, -72.65277065, -76.55967537, -80.4665801, -84.37348483, -88.28038956, -92.18729429, -96.09419902,
0, 8.054910392, 10.97779102, 10.85700888, 8.48040349, 4.368225955, -0.439337859, -5.246901672, -10.05446549, -14.8620293, -19.66959311, -24.47715693, -29.28472074, -34.09228455, -38.89984837, -43.70741218, -48.514976, -53.32253981, -58.13010362, -62.93766744, -67.74523125, -72.55279506, -77.36035888, -82.16792269, -86.9754865, -91.78305032, -96.59061413, -101.3981779, -106.2057418, -111.0133056, -115.8208694,
0, 8.185657916, 11.52868252, 11.79117939, 9.818525147, 5.941483063, 0.252518566, -6.45183598, -13.27240413, -20.09297228, -26.91354043, -33.73410859, -40.55467674, -47.37524489, -54.19581304, -61.01638119, -67.83694934, -74.65751749, -81.47808565, -88.2986538, -95.11922195, -101.9397901, -108.7603583, -115.5809264, -122.4014946, -129.2220627, -136.0426309, -142.863199, -149.6837672, -156.5043353, -163.3249035,
0, 6.41310288, 8.592408221, 7.804349569, 4.814088486, 0.047226213, -6.284986555, -14.09366582, -23.34986717, -33.89275678, -45.0013562, -56.10995563, -67.21855506, -78.32715448, -89.43575391, -100.5443533, -111.6529528, -122.7615522, -133.8701516, -144.978751, -156.0873505, -167.1959499, -178.3045493, -189.4131488, -200.5217482, -211.6303476, -222.738947, -233.8475465, -244.9561459, -256.0647453, -267.1733447,
0, 0.499736052, -2.411114241, -7.96795806, -15.61900462, -24.97793793, -35.78398395, -47.86880597, -61.12964976, -75.50814614, -90.97416252, -107.5140753, -125.1228114, -143.7989759, -163.3047656, -183.1065743, -202.9083829, -222.7101915, -242.5120001, -262.3138087, -282.1156174, -301.917426, -321.7192346, -341.5210432, -361.3228519, -381.1246605, -400.9264691, -420.7282777, -440.5300863, -460.331895, -480.1337036,
0, -13.42680113, -29.41407997, -47.56206247, -67.54535393, -89.10195161, -112.0234155, -136.1461298, -161.3435872, -187.5196299, -214.6025806, -242.5401972, -271.2953858, -300.8426083, -331.1649183, -362.2515636, -394.0960908, -426.6948911, -460.0461249, -494.1489655, -529.0031028, -564.6084481, -600.9256836, -637.5792125, -674.2327414, -710.8862704, -747.5397993, -784.1933282, -820.8468572, -857.5003861, -894.1539151,
0, -41.8181942, -85.46819257, -130.7594671, -177.5246116, -225.6171084, -274.909252, -325.2902231, -376.6643066, -428.9492473, -482.0747358, -535.9810196, -590.6176324, -645.9422358, -701.9195668, -758.5204862, -815.7211209, -873.502095, -931.8478444, -990.746008, -1050.186892, -1110.163002, -1170.668632, -1231.699518, -1293.252535, -1355.325449, -1417.916701, -1481.025243, -1544.650392, -1608.791725, -1673.448996,
0, -95.24315428, -191.7633736, -289.473947, -388.2949811, -488.1529704, -588.9803881, -690.7152951, -793.3009697, -896.6855545, -1000.821721, -1105.666353, -1211.180244, -1317.327813, -1424.076834, -1531.398183, -1639.265595, -1747.655443, -1856.546517, -1965.919831, -2075.758434, -2186.047231, -2296.772821, -2407.923343, -2519.488335, -2631.458596, -2743.826067, -2856.583714, -2969.725425, -3083.245907, -3197.140603,
0, 1.205345416, 1.18591399, -0.28989559, -2.452983602, -4.616071615, -6.779159627, -8.94224764, -11.10533565, -13.26842366, -15.43151168, -17.59459969, -19.7576877, -21.92077571, -24.08386373, -26.24695174, -28.41003975, -30.57312776, -32.73621578, -34.89930379, -37.0623918, -39.22547981, -41.38856783, -43.55165584, -45.71474385, -47.87783186, -50.04091988, -52.20400789, -54.3670959, -56.53018391, -58.69327193,
0, 1.414525013, 0.82903466, -1.185582873, -3.588710919, -5.991838965, -8.394967012, -10.79809506, -13.2012231, -15.60435115, -18.0074792, -20.41060724, -22.81373529, -25.21686334, -27.61999138, -30.02311943, -32.42624748, -34.82937552, -37.23250357, -39.63563162, -42.03875966, -44.44188771, -46.84501575, -49.2481438, -51.65127185, -54.05439989, -56.45752794, -58.86065599, -61.26378403, -63.66691208, -66.07004013,
0, 1.742292691, 0.962639437, -1.305390501, -3.912341561, -6.519292621, -9.126243681, -11.73319474, -14.3401458, -16.94709686, -19.55404792, -22.16099898, -24.76795004, -27.3749011, -29.98185216, -32.58880322, -35.19575428, -37.80270534, -40.4096564, -43.01660746, -45.62355852, -48.23050958, -50.83746064, -53.4444117, -56.05136276, -58.65831382, -61.26526488, -63.87221594, -66.479167, -69.08611806, -71.69306912,
0, 2.128206017, 1.267272135, -1.197612607, -4.029674511, -6.861736415, -9.693798319, -12.52586022, -15.35792213, -18.18998403, -21.02204593, -23.85410784, -26.68616974, -29.51823165, -32.35029355, -35.18235545, -38.01441736, -40.84647926, -43.67854116, -46.51060307, -49.34266497, -52.17472688, -55.00678878, -57.83885068, -60.67091259, -63.50297449, -66.3350364, -69.1670983, -71.9991602, -74.83122211, -77.66328401,
0, 2.553354211, 1.664022113, -0.988048075, -4.078472096, -7.168896116, -10.25932014, -13.34974416, -16.44016818, -19.5305922, -22.62101622, -25.71144024, -28.80186426, -31.89228828, -34.9827123, -38.07313632, -41.16356034, -44.25398436, -47.34440838, -50.4348324, -53.52525642, -56.61568044, -59.70610446, -62.79652848, -65.88695251, -68.97737653, -72.06780055, -75.15822457, -78.24864859, -81.33907261, -84.42949663,
0, 3.010911572, 2.123956591, -0.724127133, -4.115197762, -7.50626839, -10.89733902, -14.28840965, -17.67948028, -21.0705509, -24.46162153, -27.85269216, -31.24376279, -34.63483342, -38.02590405, -41.41697468, -44.8080453, -48.19911593, -51.59018656, -54.98125719, -58.37232782, -61.76339845, -65.15446908, -68.54553971, -71.93661033, -75.32768096, -78.71875159, -82.10982222, -85.50089285, -88.89196348, -92.28303411,
0, 3.499673094, 2.637052668, -0.424908589, -4.165941434, -7.906974279, -11.64800712, -15.38903997, -19.13007281, -22.87110566, -26.6121385, -30.35317135, -34.09420419, -37.83523704, -41.57626988, -45.31730273, -49.05833557, -52.79936842, -56.54040126, -60.28143411, -64.02246695, -67.7634998, -71.50453264, -75.24556549, -78.98659833, -82.72763118, -86.46866403, -90.20969687, -93.95072972, -97.69176256, -101.4327954,
0, 4.021801539, 3.203725224, -0.094385316, -4.240634909, -8.386884503, -12.5331341, -16.67938369, -20.82563328, -24.97188288, -29.11813247, -33.26438206, -37.41063165, -41.55688125, -45.70313084, -49.84938043, -53.99563003, -58.14187962, -62.28812921, -66.43437881, -70.5806284, -74.72687799, -78.87312759, -83.01937718, -87.16562677, -91.31187637, -95.45812596, -99.60437555, -103.7506251, -107.8968747, -112.0431243,
0, 4.581573417, 3.832228306, 0.323678885, -4.240065103, -8.853505908, -13.46694671, -18.08038752, -22.69382832, -27.30726913, -31.92070994, -36.53415074, -41.14759155, -45.76103235, -50.37447316, -54.98791396, -59.60135477, -64.21479557, -68.82823638, -73.44167719, -78.05511799, -82.6685588, -87.2819996, -91.89544041, -96.50888121, -101.122322, -105.7357628, -110.3492036, -114.9626444, -119.5760852, -124.189526,
0, 5.183698157, 4.538015618, 0.881995711, -4.085484531, -9.24004788, -14.39461123, -19.54917458, -24.70373792, -29.85830127, -35.01286462, -40.16742797, -45.32199132, -50.47655467, -55.63111801, -60.78568136, -65.94024471, -71.09480806, -76.24937141, -81.40393476, -86.5584981, -91.71306145, -96.8676248, -102.0221881, -107.1767515, -112.3313148, -117.4858782, -122.6404415, -127.7950049, -132.9495682, -138.1041316,
0, 5.829148896, 5.34202292, 1.581449547, -3.820135734, -9.617142684, -15.41414964, -21.21115659, -27.00816354, -32.80517049, -38.60217744, -44.39918439, -50.19619134, -55.99319829, -61.79020524, -67.58721219, -73.38421914, -79.18122609, -84.97823304, -90.77524, -96.57224695, -102.3692539, -108.1662608, -113.9632678, -119.7602747, -125.5572817, -131.3542887, -137.1512956, -142.9483026, -148.7453095, -154.5423165,
0, 6.503456975, 6.258944065, 2.479314695, -3.392792046, -9.998124835, -16.60345762, -23.20879041, -29.8141232, -36.41945599, -43.02478878, -49.63012157, -56.23545435, -62.84078714, -69.44611993, -76.05145272, -82.65678551, -89.2621183, -95.86745108, -102.4727839, -109.0781167, -115.6834494, -122.2887822, -128.894115, -135.4994478, -142.1047806, -148.7101134, -155.3154462, -161.920779, -168.5261118, -175.1314445,
0, 7.143927956, 7.245459209, 3.573845936, -2.827316974, -10.52862169, -18.25322001, -25.97781833, -33.70241665, -41.42701497, -49.15161329, -56.87621161, -64.60080993, -72.32540825, -80.05000657, -87.77460489, -95.49920321, -103.2238015, -110.9483999, -118.6729982, -126.3975965, -134.1221948, -141.8467931, -149.5713915, -157.2959898, -165.0205881, -172.7451864, -180.4697847, -188.1943831, -195.9189814, -203.6435797,
0, 7.553313025, 8.02948162, 4.542147934, -1.726103271, -10.21943668, -19.68121893, -29.14300118, -38.60478343, -48.06656568, -57.52834794, -66.99013019, -76.45191244, -85.91369469, -95.37547694, -104.8372592, -114.2990414, -123.7608237, -133.222606, -142.6843882, -152.1461705, -161.6079527, -171.069735, -180.5315172, -189.9932995, -199.4550817, -208.916864, -218.3786462, -227.8404285, -237.3022107, -246.763993,
0, 7.20581266, 7.688450589, 4.097917722, -2.295419069, -10.99200673, -21.84959065, -34.0288391, -46.46015346, -58.89146782, -71.32278218, -83.75409654, -96.18541091, -108.6167253, -121.0480396, -133.479354, -145.9106684, -158.3419827, -170.7732971, -183.2046114, -195.6359258, -208.0672402, -220.4985545, -232.9298689, -245.3611832, -257.7924976, -270.223812, -282.6551263, -295.0864407, -307.5177551, -319.9490694,
0, 4.912825211, 3.896552083, -1.127106486, -9.001277882, -19.08426675, -31.05787889, -44.78786133, -60.23001273, -77.27905618, -95.08324788, -112.8874396, -130.6916313, -148.4958229, -166.3000146, -184.1042063, -201.908398, -219.7125897, -237.5167814, -255.3209731, -273.1251648, -290.9293565, -308.7335482, -326.5377399, -344.3419316, -362.1461232, -379.9503149, -397.7545066, -415.5586983, -433.36289, -451.1670817,
0, -1.610960538, -8.036428775, -18.10724091, -30.98252269, -46.07559975, -62.99180769, -81.47722088, -101.3773056, -122.6044883, -145.1136124, -168.8842361, -193.9087011, -220.1848671, -247.4951809, -275.2139632, -302.9327454, -330.6515277, -358.37031, -386.0890922, -413.8078745, -441.5266567, -469.245439, -496.9642212, -524.6830035, -552.4017857, -580.120568, -607.8393503, -635.5581325, -663.2769148, -690.995697,
0, -16.3004277, -36.23479611, -59.18872138, -84.66331937, -112.2578815, -141.6544231, -172.6039887, -204.9146, -238.4407339, -273.0742187, -308.7364369, -345.3717277, -382.9418792, -421.4216061, -460.7949068, -501.0521996, -542.1881351, -584.1999878, -627.0865312, -670.8473022, -715.4821668, -760.98639, -806.9229365, -852.859483, -898.7960296, -944.7325761, -990.6691226, -1036.605669, -1082.542216, -1128.478762,
0, -45.64945476, -93.9049153, -144.4729325, -197.0960475, -251.5492645, -307.636777, -365.1889367, -424.0594533, -484.1228156, -545.2719215, -607.4159081, -670.4781696, -734.3945551, -799.1117346, -864.5857236, -930.7805576, -997.6671065, -1065.222019, -1133.426789, -1202.266936, -1271.731288, -1341.811361, -1412.500827, -1483.795063, -1555.690767, -1628.185645, -1701.278154, -1774.967295, -1849.252449, -1924.133253,
0, -100.2416409, -202.2999929, -306.0415131, -411.3432637, -518.0922348, -626.1846993, -735.5255992, -846.0279623, -957.6123484, -1070.206324, -1183.743963, -1298.165377, -1413.41627, -1529.447515, -1646.214756, -1763.678038, -1881.801451, -2000.552799, -2119.903291, -2239.82725, -2360.301836, -2481.306798, -2602.824233, -2724.838367, -2847.335345, -2970.303049, -3093.730914, -3217.609769, -3341.93169, -3466.689857,
0, 1.986228366, 2.26323014, 0.873533509, -1.222695149, -3.318923808, -5.415152466, -7.511381124, -9.607609783, -11.70383844, -13.8000671, -15.89629576, -17.99252442, -20.08875307, -22.18498173, -24.28121039, -26.37743905, -28.47366771, -30.56989637, -32.66612502, -34.76235368, -36.85858234, -38.954811, -41.05103966, -43.14726832, -45.24349697, -47.33972563, -49.43595429, -51.53218295, -53.62841161, -55.72464027,
0, 2.406758025, 2.257008319, 0.37441896, -2.014158847, -4.402736655, -6.791314462, -9.179892269, -11.56847008, -13.95704788, -16.34562569, -18.7342035, -21.12278131, -23.51135911, -25.89993692, -28.28851473, -30.67709253, -33.06567034, -35.45424815, -37.84282596, -40.23140376, -42.61998157, -45.00855938, -47.39713719, -49.78571499, -52.1742928, -54.56287061, -56.95144841, -59.34002622, -61.72860403, -64.11718184,
0, 2.933123319, 2.63733271, 0.456656104, -2.219724761, -4.896105627, -7.572486492, -10.24886736, -12.92524822, -15.60162909, -18.27800995, -20.95439082, -23.63077168, -26.30715255, -28.98353341, -31.65991428, -34.33629514, -37.01267601, -39.68905687, -42.36543774, -45.04181861, -47.71819947, -50.39458034, -53.0709612, -55.74734207, -58.42372293, -61.1001038, -63.77648466, -66.45286553, -69.12924639, -71.80562726,
0, 3.50138133, 3.149807427, 0.714039855, -2.278688932, -5.271417718, -8.264146505, -11.25687529, -14.24960408, -17.24233286, -20.23506165, -23.22779044, -26.22051923, -29.21324801, -32.2059768, -35.19870559, -38.19143437, -41.18416316, -44.17689195, -47.16962073, -50.16234952, -53.15507831, -56.14780709, -59.14053588, -62.13326467, -65.12599345, -68.11872224, -71.11145103, -74.10417981, -77.0969086, -80.08963739,
0, 4.084443903, 3.707254909, 1.008251705, -2.353607007, -5.71546572, -9.077324432, -12.43918314, -15.80104186, -19.16290057, -22.52475928, -25.88661799, -29.24847671, -32.61033542, -35.97219413, -39.33405284, -42.69591156, -46.05777027, -49.41962898, -52.78148769, -56.14334641, -59.50520512, -62.86706383, -66.22892254, -69.59078126, -72.95263997, -76.31449868, -79.67635739, -83.03821611, -86.40007482, -89.76193353,
0, 4.666322225, 4.263683262, 1.264759076, -2.539354533, -6.343468142, -10.14758175, -13.95169536, -17.75580897, -21.55992258, -25.36403619, -29.1681498, -32.97226341, -36.77637702, -40.58049063, -44.38460423, -48.18871784, -51.99283145, -55.79694506, -59.60105867, -63.40517228, -67.20928589, -71.0133995, -74.81751311, -78.62162672, -82.42574033, -86.22985394, -90.03396755, -93.83808116, -97.64219476, -101.4463084,
0, 5.23647172, 4.790480523, 1.435392182, -2.901744388, -7.238880959, -11.57601753, -15.9131541, -20.25029067, -24.58742724, -28.92456381, -33.26170038, -37.59883695, -41.93597352, -46.27311009, -50.61024666, -54.94738323, -59.2845198, -63.62165637, -67.95879294, -72.29592951, -76.63306608, -80.97020265, -85.30733922, -89.64447579, -93.98161237, -98.31874894, -102.6558855, -106.9930221, -111.3301586, -115.6672952,
0, 5.787899232, 5.26971419, 1.579319647, -3.306002454, -8.282986823, -13.25997119, -18.23695556, -23.21393993, -28.1909243, -33.16790867, -38.14489303, -43.1218774, -48.09886177, -53.07584614, -58.05283051, -63.02981488, -68.00679925, -72.98378361, -77.96076798, -82.93775235, -87.91473672, -92.89172109, -97.86870546, -102.8456898, -107.8226742, -112.7996586, -117.7766429, -122.7536273, -127.7306117, -132.707596,
0, 6.316313141, 5.692783599, 1.623454063, -3.8949897, -9.635111169, -15.37523264, -21.11535411, -26.85547558, -32.59559704, -38.33571851, -44.07583998, -49.81596145, -55.55608292, -61.29620439, -67.03632586, -72.77644732, -78.51656879, -84.25669026, -89.99681173, -95.7369332, -101.4770547, -107.2171761, -112.9572976, -118.6974191, -124.4375405, -130.177662, -135.9177835, -141.6579049, -147.3980264, -153.1381479,
0, 6.819192693, 6.061838115, 1.56947468, -4.679875217, -11.32782913, -17.97578304, -24.62373694, -31.27169085, -37.91964476, -44.56759867, -51.21555258, -57.86350649, -64.5114604, -71.15941431, -77.80736821, -84.45532212, -91.10327603, -97.75122994, -104.3991839, -111.0471378, -117.6950917, -124.3430456, -130.9909995, -137.6389534, -144.2869073, -150.9348612, -157.5828151, -164.230769, -170.8787229, -177.5266768,
0, 7.29305516, 6.39194536, 1.454709924, -5.62818892, -13.36526778, -21.10234664, -28.83942549, -36.57650435, -44.31358321, -52.05066207, -59.78774093, -67.52481978, -75.26189864, -82.9989775, -90.73605636, -98.47313522, -106.2102141, -113.9472929, -121.6843718, -129.4214506, -137.1585295, -144.8956084, -152.6326872, -160.3697661, -168.1068449, -175.8439238, -183.5810027, -191.3180815, -199.0551604, -206.7922392,
0, 7.724189883, 6.70607236, 1.348602409, -6.679397867, -15.76045978, -24.8415217, -33.92258361, -43.00364553, -52.08470744, -61.16576936, -70.24683127, -79.32789319, -88.4089551, -97.49001702, -106.5710789, -115.6521408, -124.7332028, -133.8142647, -142.8953266, -151.9763885, -161.0574504, -170.1385123, -179.2195742, -188.3006362, -197.3816981, -206.46276, -215.5438219, -224.6248838, -233.7059457, -242.7870077,
0, 8.059062068, 6.992735167, 1.297181393, -7.607990088, -18.21930842, -29.05309939, -39.88689036, -50.72068133, -61.55447229, -72.38826326, -83.22205423, -94.05584519, -104.8896362, -115.7234271, -126.5572181, -137.3910091, -148.2248, -159.058591, -169.892382, -180.7261729, -191.5599639, -202.3937549, -213.2275458, -224.0613368, -234.8951278, -245.7289187, -256.5627097, -267.3965007, -278.2302916, -289.0640826,
0, 8.119271163, 7.042138423, 1.073425784, -8.177963124, -20.18936783, -33.5002914, -46.81121498, -60.12213855, -73.43306213, -86.7439857, -100.0549093, -113.3658329, -126.6767564, -139.98768, -153.2986036, -166.6095272, -179.9204507, -193.2313743, -206.5422979, -219.8532215, -233.164145, -246.4750686, -259.7859922, -273.0969158, -286.4078393, -299.7187629, -313.0296865, -326.3406101, -339.6515336, -352.9624572,
0, 7.399064497, 6.010015223, -0.474087184, -10.31480448, -22.83905614, -37.85631668, -54.54851081, -71.68394536, -88.81937991, -105.9548145, -123.090249, -140.2256836, -157.3611181, -174.4965527, -191.6319872, -208.7674218, -225.9028563, -243.0382909, -260.1737254, -277.30916, -294.4445945, -311.5800291, -328.7154636, -345.8508982, -362.9863327, -380.1217673, -397.2572018, -414.3926364, -431.5280709, -448.6635055,
0, 4.706102283, 1.60112352, -6.610411241, -18.32568194, -32.67007725, -49.21703194, -67.78930157, -88.32912602, -110.8238409, -134.3029623, -157.7897088, -181.2764553, -204.7632018, -228.2499483, -251.7366948, -275.2234413, -298.7101878, -322.1969343, -345.6836808, -369.1704273, -392.6571738, -416.1439203, -439.6306668, -463.1174134, -486.6041599, -510.0909064, -533.5776529, -557.0643994, -580.5511459, -604.0378924,
0, -2.292394455, -10.97461771, -24.38655534, -41.34741185, -61.0429806, -82.93232045, -106.6719625, -132.0557788, -158.9686548, -187.3521165, -217.180078, -248.4428851, -281.1378539, -315.0990433, -349.6105886, -384.122134, -418.6336793, -453.1452247, -487.65677, -522.1683154, -556.6798607, -591.1914061, -625.7029514, -660.2144968, -694.7260421, -729.2375875, -763.7491328, -798.2606782, -832.7722235, -867.2837689,
0, -17.60035202, -40.04818073, -66.46615221, -96.14687122, -128.526313, -163.1603151, -199.7039159, -237.893332, -277.530368, -318.4690603, -360.6043574, -403.8626461, -448.193938, -493.5655333, -539.9569889, -587.3562188, -635.7565648, -685.1546807, -735.5490804, -786.9392095, -839.3249084, -892.7061466, -946.6354057, -1000.614917, -1054.594429, -1108.573941, -1162.553452, -1216.532964, -1270.512476, -1324.491987,
0, -47.76104028, -99.00625204, -153.315711, -210.3225619, -269.7076068, -331.1943048, -394.5441645, -459.552508, -526.0445888, -593.872043, -662.9096564, -733.0524297, -804.2129228, -876.3188626, -949.3109962, -1023.141174, -1097.770645, -1173.168555, -1249.310621, -1326.177979, -1403.756185, -1482.034358, -1561.00445, -1640.660629, -1720.998772, -1802.016041, -1883.710544, -1966.081072, -2049.126882, -2132.847545,
0, -103.3761457, -209.1811436, -317.2240182, -427.3294049, -539.336515, -653.0981532, -768.479785, -885.3586524, -1003.622937, -1123.17097, -1243.910478, -1365.757886, -1488.637643, -1612.481598, -1737.228411, -1862.822999, -1989.216014, -2116.363358, -2244.225729, -2372.76819, -2501.959783, -2631.773147, -2762.184188, -2893.171753, -3024.717337, -3156.804814, -3289.420185, -3422.551349, -3556.187891, -3690.320888,
0, 3.366004411, 4.270258503, 3.099846872, 1.106426352, -0.886994167, -2.880414687, -4.873835206, -6.867255726, -8.860676246, -10.85409677, -12.84751729, -14.8409378, -16.83435832, -18.82777884, -20.82119936, -22.81461988, -24.8080404, -26.80146092, -28.79488144, -30.78830196, -32.78172248, -34.775143, -36.76856352, -38.76198404, -40.75540456, -42.74882508, -44.7422456, -46.73566612, -48.72908664, -50.72250716,
0, 3.973598543, 4.516334537, 2.84327955, 0.480993541, -1.881292469, -4.243578479, -6.605864489, -8.968150499, -11.33043651, -13.69272252, -16.05500853, -18.41729454, -20.77958055, -23.14186656, -25.50415257, -27.86643858, -30.22872459, -32.5910106, -34.95329661, -37.31558262, -39.67786863, -42.04015464, -44.40244065, -46.76472666, -49.12701267, -51.48929868, -53.85158468, -56.21387069, -58.5761567, -60.93844271,
0, 4.701205829, 5.147457143, 3.151048806, 0.43753528, -2.275978245, -4.98949177, -7.703005296, -10.41651882, -13.13003235, -15.84354587, -18.5570594, -21.27057292, -23.98408645, -26.69759997, -29.4111135, -32.12462702, -34.83814055, -37.55165407, -40.2651676, -42.97868113, -45.69219465, -48.40570818, -51.1192217, -53.83273523, -56.54624875, -59.25976228, -61.9732758, -64.68678933, -67.40030285, -70.11381638,
0, 5.469368857, 5.906095237, 3.627417894, 0.534176227, -2.55906544, -5.652307107, -8.745548773, -11.83879044, -14.93203211, -18.02527377, -21.11851544, -24.21175711, -27.30499877, -30.39824044, -33.49148211, -36.58472378, -39.67796544, -42.77120711, -45.86444878, -48.95769044, -52.05093211, -55.14417378, -58.23741544, -61.33065711, -64.42389878, -67.51714044, -70.61038211, -73.70362378, -76.79686545, -79.89010711,
0, 6.238383009, 6.683794586, 4.103693763, 0.564936724, -2.973820315, -6.512577353, -10.05133439, -13.59009143, -17.12884847, -20.66760551, -24.20636255, -27.74511958, -31.28387662, -34.82263366, -38.3613907, -41.90014774, -45.43890478, -48.97766182, -52.51641885, -56.05517589, -59.59393293, -63.13268997, -66.67144701, -70.21020405, -73.74896109, -77.28771812, -80.82647516, -84.3652322, -87.90398924, -91.44274628,
0, 6.980901922, 7.412519995, 4.544859948, 0.53534501, -3.546829204, -7.629003418, -11.71117763, -15.79335185, -19.87552606, -23.95770027, -28.03987449, -32.1220487, -36.20422291, -40.28639713, -44.36857134, -48.45074556, -52.53291977, -56.61509398, -60.6972682, -64.77944241, -68.86161663, -72.94379084, -77.02596505, -81.10813927, -85.19031348, -89.27248769, -93.35466191, -97.43683612, -101.5190103, -105.6011845,
0, 7.675790751, 8.042505238, 4.830046401, 0.255315667, -4.496783492, -9.248882651, -14.00098181, -18.75308097, -23.50518013, -28.25727929, -33.00937845, -37.76147761, -42.51357676, -47.26567592, -52.01777508, -56.76987424, -61.5219734, -66.27407256, -71.02617172, -75.77827088, -80.53037004, -85.28246919, -90.03456835, -94.78666751, -99.53876667, -104.2908658, -109.042965, -113.7950641, -118.5471633, -123.2992625,
0, 8.306121678, 8.535931981, 4.893710119, -0.369898104, -5.944884785, -11.51987147, -17.09485815, -22.66984483, -28.24483151, -33.81981819, -39.39480487, -44.96979155, -50.54477823, -56.11976491, -61.69475159, -67.26973827, -72.84472495, -78.41971163, -83.99469832, -89.569685, -95.14467168, -100.7196584, -106.294645, -111.8696317, -117.4446184, -123.0196051, -128.5945918, -134.1695784, -139.7445651, -145.3195518,
0, 8.85851874, 8.866298816, 4.694764035, -1.401523781, -7.978800827, -14.55607787, -21.13335492, -27.71063197, -34.28790901, -40.86518606, -47.4424631, -54.01974015, -60.5970172, -67.17429424, -73.75157129, -80.32884834, -86.90612538, -93.48340243, -100.0606795, -106.6379565, -113.2152336, -119.7925106, -126.3697877, -132.9470647, -139.5243418, -146.1016188, -152.6788958, -159.2561729, -165.8334499, -172.410727,
0, 9.322871645, 9.021598256, 4.218494081, -2.870382467, -10.66040357, -18.45042467, -26.24044578, -34.03046688, -41.82048798, -49.61050908, -57.40053019, -65.19055129, -72.98057239, -80.77059349, -88.5606146, -96.3506357, -104.1406568, -111.9306779, -119.720699, -127.5107201, -135.3007412, -143.0907623, -150.8807834, -158.6708045, -166.4608256, -174.2508467, -182.0408678, -189.8308889, -197.62091, -205.4109311,
0, 9.690991404, 9.010154058, 3.490879163, -4.762627912, -14.02182003, -23.28101214, -32.54020426, -41.79939637, -51.05858849, -60.31778061, -69.57697272, -78.83616484, -88.09535695, -97.35454907, -106.6137412, -115.8729333, -125.1321254, -134.3913175, -143.6505096, -152.9097018, -162.1688939, -171.428086, -180.6872781, -189.9464702, -199.2056623, -208.4648545, -217.7240466, -226.9832387, -236.2424308, -245.5016229,
0, 9.949833213, 8.862307593, 2.590304502, -7.01325894, -18.08076665, -29.14827436, -40.21578207, -51.28328978, -62.35079749, -73.4183052, -84.48581291, -95.55332062, -106.6208283, -117.688336, -128.7558437, -139.8233515, -150.8908592, -161.9583669, -173.0258746, -184.0933823, -195.16089, -206.2283977, -217.2959054, -228.3634131, -239.4309208, -250.4984286, -261.5659363, -272.633444, -283.7009517, -294.7684594,
0, 10.05544177, 8.598446004, 1.607852433, -9.048384678, -21.90087308, -35.27869005, -48.65650702, -62.034324, -75.41214097, -88.78995794, -102.1677749, -115.5455919, -128.9234089, -142.3012258, -155.6790428, -169.0568598, -182.4346767, -195.8124937, -209.1903107, -222.5681277, -235.9459446, -249.3237616, -262.7015786, -276.0793956, -289.4572125, -302.8350295, -316.2128465, -329.5906634, -342.9684804, -356.3462974,
0, 9.85024027, 8.075427157, 0.416636868, -11.01642856, -25.68118618, -42.06099354, -58.5755669, -75.09014026, -91.60471363, -108.119287, -124.6338604, -141.1484337, -157.6630071, -174.1775804, -190.6921538, -207.2067272, -223.7213005, -240.2358739, -256.7504473, -273.2650206, -289.779594, -306.2941673, -322.8087407, -339.3233141, -355.8378874, -372.3524608, -388.8670341, -405.3816075, -421.8961809, -438.4107542,
0, 8.853381261, 6.541386541, -1.977992007, -14.4178664, -29.90743495, -48.20142889, -68.59452242, -89.70392481, -110.8133272, -131.9227296, -153.032132, -174.1415344, -195.2509368, -216.3603392, -237.4697416, -258.5791439, -279.6885463, -300.7979487, -321.9073511, -343.0167535, -364.1261559, -385.2355583, -406.3449607, -427.4543631, -448.5637655, -469.6731679, -490.7825703, -511.8919726, -533.001375, -554.1107774,
0, 5.874707133, 1.753929393, -8.696177068, -23.34227719, -41.04208279, -61.24885499, -83.73921298, -108.4411126, -135.3386448, -163.5110049, -191.8612532, -220.2115016, -248.5617499, -276.9119982, -305.2622466, -333.6124949, -361.9627432, -390.3129916, -418.6632399, -447.0134882, -475.3637365, -503.7139849, -532.0642332, -560.4144815, -588.7647299, -617.1149782, -645.4652265, -673.8154749, -702.1657232, -730.5159715,
0, -1.466239415, -11.16703489, -26.83053385, -46.86228586, -70.17826201, -96.0689128, -124.0907376, -153.9819301, -185.5987627, -218.8694806, -253.7625963, -290.2666102, -328.3783386, -368.032103, -408.424306, -448.8165089, -489.2087119, -529.6009148, -569.9931178, -610.3853207, -650.7775237, -691.1697266, -731.5619295, -771.9541325, -812.3463354, -852.7385384, -893.1307413, -933.5229443, -973.9151472, -1014.30735,
0, -17.25150419, -40.7806681, -69.37926583, -102.0815206, -138.1243203, -176.9123601, -217.9878212, -261.0042065, -305.7039665, -351.8995596, -399.4576061, -448.285804, -498.3222935, -549.5271665, -601.8758338, -655.3539774, -709.953833, -765.6715596, -822.5054772, -880.4549664, -939.5198502, -999.7000944, -1060.57568, -1121.588927, -1182.602175, -1243.615423, -1304.628671, -1365.641919, -1426.655167, -1487.668414,
0, -48.08377294, -100.6894368, -157.2373077, -217.2236511, -280.2125603, -345.8284978, -413.7494603, -483.7007348, -555.4492107, -628.7982141, -703.5828339, -779.6657064, -856.9332293, -935.2921754, -1014.666677, -1094.995557, -1176.229969, -1258.331343, -1341.269582, -1425.021518, -1509.569573, -1594.900638, -1681.005112, -1767.876116, -1855.508836, -1943.900001, -2033.047457, -2122.949844, -2213.606337, -2305.01647,
0, -104.5872696, -212.3263661, -322.953852, -436.2285974, -551.9302332, -669.8576873, -789.8277995, -911.6740145, -1035.245147, -1160.404219, -1287.027363, -1415.002796, -1544.229846, -1674.618049, -1806.086298, -1938.562046, -2071.980563, -2206.284245, -2341.421966, -2477.348481, -2614.023863, -2751.412996, -2889.485088, -3028.213237, -3167.574025, -3307.547141, -3448.115047, -3589.262656, -3730.977056, -3873.247245,
0, 5.650619807, 7.652729019, 6.852402464, 4.952169765, 3.051937065, 1.151704365, -0.748528334, -2.648761034, -4.548993734, -6.449226433, -8.349459133, -10.24969183, -12.14992453, -14.05015723, -15.95038993, -17.85062263, -19.75085533, -21.65108803, -23.55132073, -25.45155343, -27.35178613, -29.25201883, -31.15225153, -33.05248423, -34.95271693, -36.85294963, -38.75318233, -40.65341503, -42.55364773, -44.45388043,
0, 6.420328442, 8.065807533, 6.723083732, 4.381662927, 2.040242122, -0.301178682, -2.642599487, -4.984020292, -7.325441096, -9.666861901, -12.00828271, -14.34970351, -16.69112432, -19.03254512, -21.37396592, -23.71538673, -26.05680753, -28.39822834, -30.73964914, -33.08106995, -35.42249075, -37.76391156, -40.10533236, -42.44675317, -44.78817397, -47.12959478, -49.47101558, -51.81243639, -54.15385719, -56.49527799,
0, 7.346097509, 8.93486966, 7.28251876, 4.572132065, 1.830123411, -0.911885242, -3.653893895, -6.395902548, -9.137911202, -11.87991985, -14.62192851, -17.36393716, -20.10594581, -22.84795447, -25.58996312, -28.33197177, -31.07398043, -33.81598908, -36.55799773, -39.30000639, -42.04201504, -44.78402369, -47.52603235, -50.268041, -53.01004965, -55.75205831, -58.49406696, -61.23607561, -63.97808427, -66.72009292,
0, 8.323967138, 9.958568819, 8.084824182, 5.020087864, 1.855568354, -1.308951156, -4.473470666, -7.637990176, -10.80250969, -13.9670292, -17.13154871, -20.29606822, -23.46058773, -26.62510724, -29.78962675, -32.95414626, -36.11866577, -39.28318528, -42.44770479, -45.6122243, -48.77674381, -51.94126332, -55.10578283, -58.27030234, -61.43482185, -64.59934136, -67.76386087, -70.92838038, -74.09289989, -77.2574194,
0, 9.298174839, 10.99622174, 8.90431404, 5.437907404, 1.778016749, -1.881873906, -5.541764561, -9.201655217, -12.86154587, -16.52143653, -20.18132718, -23.84121784, -27.50110849, -31.16099915, -34.8208898, -38.48078046, -42.14067111, -45.80056177, -49.46045242, -53.12034308, -56.78023373, -60.44012439, -64.10001504, -67.7599057, -71.41979636, -75.07968701, -78.73957767, -82.39946832, -86.05935898, -89.71924963,
0, 10.22823198, 11.9530075, 9.594701079, 5.635864269, 1.363436996, -2.908990277, -7.18141755, -11.45384482, -15.7262721, -19.99869937, -24.27112664, -28.54355392, -32.81598119, -37.08840846, -41.36083574, -45.63326301, -49.90569028, -54.17811756, -58.45054483, -62.7229721, -66.99539937, -71.26782665, -75.54025392, -79.81268119, -84.08510847, -88.35753574, -92.62996301, -96.90239029, -101.1748176, -105.4472448,
0, 11.0813478, 12.75544403, 10.04189025, 5.461542879, 0.419525646, -4.622491586, -9.664508818, -14.70652605, -19.74854328, -24.79056052, -29.83257775, -34.87459498, -39.91661221, -44.95862944, -50.00064668, -55.04266391, -60.08468114, -65.12669837, -70.16871561, -75.21073284, -80.25275007, -85.2947673, -90.33678454, -95.37880177, -100.420819, -105.4628362, -110.5048535, -115.5468707, -120.5888879, -125.6309052,
0, 11.82997513, 13.34427193, 10.15379863, 4.788760033, -1.217008089, -7.222776211, -13.22854433, -19.23431245, -25.24008058, -31.2458487, -37.25161682, -43.25738494, -49.26315306, -55.26892118, -61.27468931, -67.28045743, -73.28622555, -79.29199367, -85.29776179, -91.30352991, -97.30929804, -103.3150662, -109.3208343, -115.3266024, -121.3323705, -127.3381386, -133.3439068, -139.3496749, -145.355443, -151.3612111,
0, 12.45120586, 13.67408121, 9.860720124, 3.517803073, -3.682503212, -10.8828095, -18.08311578, -25.28342207, -32.48372835, -39.68403464, -46.88434092, -54.08464721, -61.28495349, -68.48525978, -75.68556606, -82.88587235, -90.08617863, -97.28648492, -104.4867912, -111.6870975, -118.8874038, -126.0877101, -133.2880163, -140.4883226, -147.6886289, -154.8889352, -162.0892415, -169.2895478, -176.4898541, -183.6901603,
0, 12.92707039, 13.71802082, 9.124653966, 1.585246006, -7.081259251, -15.74776451, -24.41426976, -33.08077502, -41.74728028, -50.41378553, -59.08029079, -67.74679605, -76.4133013, -85.07980656, -93.74631182, -102.4128171, -111.0793223, -119.7458276, -128.4123328, -137.0788381, -145.7453434, -154.4118486, -163.0783539, -171.7448591, -180.4113644, -189.0778696, -197.7443749, -206.4108802, -215.0773854, -223.7438907,
0, 13.24458122, 13.47718364, 7.958350172, -1.016657979, -11.47652572, -21.93639347, -32.39626121, -42.85612895, -53.3159967, -63.77586444, -74.23573219, -84.69559993, -95.15546767, -105.6153354, -116.0752032, -126.5350709, -136.9949386, -147.4548064, -157.9146741, -168.3745419, -178.8344096, -189.2942774, -199.7541451, -210.2140129, -220.6738806, -231.1337483, -241.5936161, -252.0534838, -262.5133516, -272.9732193,
0, 13.39154035, 12.98911144, 6.446507443, -3.996684841, -16.44277938, -29.11517698, -41.78757458, -54.45997218, -67.13236978, -79.80476738, -92.47716498, -105.1495626, -117.8219602, -130.4943578, -143.1667554, -155.839153, -168.5115506, -181.1839482, -193.8563458, -206.5287434, -219.201141, -231.8735386, -244.5459362, -257.2183338, -269.8907314, -282.563129, -295.2355266, -307.9079242, -320.5803218, -333.2527194,
0, 13.33440101, 12.3072806, 4.723744648, -6.971081588, -21.47471734, -36.95074135, -52.42676536, -67.90278937, -83.37881339, -98.8548374, -114.3308614, -129.8068854, -145.2829094, -160.7589334, -176.2349575, -191.7109815, -207.1870055, -222.6630295, -238.1390535, -253.6150775, -269.0911015, -284.5671255, -300.0431495, -315.5191736, -330.9951976, -346.4712216, -361.9472456, -377.4232696, -392.8992936, -408.3753176,
0, 12.93893191, 11.36140224, 2.770538604, -10.09691605, -26.52574382, -45.20753644, -64.4116392, -83.61574195, -102.8198447, -122.0239475, -141.2280502, -160.432153, -179.6362557, -198.8403585, -218.0444612, -237.248564, -256.4526668, -275.6567695, -294.8608723, -314.064975, -333.2690778, -352.4731805, -371.6772833, -390.881386, -410.0854888, -429.2895915, -448.4936943, -467.6977971, -486.9018998, -506.1060026,
0, 11.75490592, 9.509040758, -0.222512223, -14.48033347, -32.14715522, -52.90567478, -76.29332391, -100.79033, -125.2873361, -149.7843422, -174.2813484, -198.7783545, -223.2753606, -247.7723667, -272.2693728, -296.7663789, -321.263385, -345.7603912, -370.2573973, -394.7544034, -419.2514095, -443.7484156, -468.2454217, -492.7424278, -517.239434, -541.7364401, -566.2334462, -590.7304523, -615.2274584, -639.7244645,
0, 8.601691097, 4.58717428, -7.179791459, -23.91577758, -44.15457902, -67.20472608, -92.78666664, -120.81026, -151.2551187, -183.3579022, -215.9091896, -248.460477, -281.0117644, -313.5630518, -346.1143391, -378.6656265, -411.2169139, -443.7682013, -476.3194887, -508.8707761, -541.4220635, -573.9733509, -606.5246383, -639.0759256, -671.627213, -704.1785004, -736.7297878, -769.2810752, -801.8323626, -834.38365,
0, 1.051681094, -8.361488623, -25.19415961, -47.33736157, -73.37707221, -102.4010329, -133.8474882, -167.389829, -202.8514404, -240.1453936, -279.2339866, -320.1035285, -362.7501994, -407.1732978, -452.594201, -498.1247251, -543.6552493, -589.1857735, -634.7162976, -680.2468218, -725.777346, -771.3078701, -816.8383943, -862.3689185, -907.8994426, -953.4299668, -998.960491, -1044.491015, -1090.021539, -1135.552063,
0, -15.06646156, -38.15052339, -67.6204662, -102.1838154, -140.8289144, -182.7742688, -227.4249656, -274.335498, -323.1783589, -373.7177918, -425.7881221, -479.2761194, -534.1068743, -590.2327031, -647.6246273, -706.2660075, -766.1479437, -827.2660901, -889.6185657, -953.2046813, -1018.024239, -1084.077198, -1151.022564, -1218.243636, -1285.464709, -1352.685782, -1419.906854, -1487.127927, -1554.349, -1621.570072,
0, -46.42848959, -98.64509776, -155.8644032, -217.4077974, -282.6914951, -351.2155885, -422.5540839, -496.3458579, -572.2864753, -650.1208107, -729.6364193, -810.6576031, -893.0401231, -976.6665059, -1061.4419, -1147.290437, -1234.152048, -1321.97971, -1410.737061, -1500.396366, -1590.936792, -1682.34295, -1774.603692, -1867.711114, -1961.659748, -2056.445922, -2152.067248, -2248.522232, -2345.809987, -2443.930011,
0, -103.6878155, -211.4094304, -322.808016, -437.5581054, -555.3633135, -675.9541876, -799.0861822, -924.537753, -1052.108566, -1181.617816, -1312.902649, -1445.816682, -1580.228628, -1716.021, -1853.088907, -1991.338936, -2130.688107, -2271.062904, -2412.398381, -2554.637326, -2697.7295, -2841.630926, -2986.30324, -3131.713093, -3277.831606, -3424.633868, -3572.098481, -3720.207148, -3868.944293, -4018.296728,
0, 9.203396683, 12.96618284, 12.88247015, 11.18448904, 9.345355958, 7.506222874, 5.66708979, 3.827956706, 1.988823622, 0.149690538, -1.689442546, -3.52857563, -5.367708714, -7.206841798, -9.045974882, -10.88510797, -12.72424105, -14.56337413, -16.40250722, -18.2416403, -20.08077339, -21.91990647, -23.75903955, -25.59817264, -27.43730572, -29.27643881, -31.11557189, -32.95470497, -34.79383806, -36.63297114,
0, 10.11155641, 13.47209049, 12.81133763, 10.63175151, 8.279658151, 5.92756479, 3.575471428, 1.223378066, -1.128715296, -3.480808657, -5.832902019, -8.184995381, -10.53708874, -12.8891821, -15.24127547, -17.59336883, -19.94546219, -22.29755555, -24.64964891, -27.00174227, -29.35383564, -31.705929, -34.05802236, -36.41011572, -38.76220908, -41.11430244, -43.46639581, -45.81848917, -48.17058253, -50.52267589,
0, 11.22902836, 14.55715544, 13.61755438, 11.07733535, 8.282901579, 5.488467807, 2.694034034, -0.100399738, -2.89483351, -5.689267282, -8.483701054, -11.27813483, -14.0725686, -16.86700237, -19.66143614, -22.45586991, -25.25030369, -28.04473746, -30.83917123, -33.633605, -36.42803878, -39.22247255, -42.01690632, -44.81134009, -47.60577386, -50.40020764, -53.19464141, -55.98907518, -58.78350895, -61.57794273,
0, 12.42089227, 15.85091063, 14.73840355, 11.86040624, 8.614350713, 5.368295184, 2.122239656, -1.123815872, -4.3698714, -7.615926928, -10.86198246, -14.10803798, -17.35409351, -20.60014904, -23.84620457, -27.0922601, -30.33831563, -33.58437115, -36.83042668, -40.07648221, -43.32253774, -46.56859327, -49.8146488, -53.06070432, -56.30675985, -59.55281538, -62.79887091, -66.04492644, -69.29098196, -72.53703749,
0, 13.61280285, 17.172395, 15.89836033, 12.63705362, 8.86495616, 5.092858696, 1.320761232, -2.451336233, -6.223433697, -9.995531161, -13.76762863, -17.53972609, -21.31182355, -25.08392102, -28.85601848, -32.62811595, -36.40021341, -40.17231088, -43.94440834, -47.7165058, -51.48860327, -55.26070073, -59.0327982, -62.80489566, -66.57699313, -70.34909059, -74.12118805, -77.89328552, -81.66538298, -85.43748045,
0, 14.74977539, 18.39618738, 16.90523332, 13.15795102, 8.728506308, 4.299061594, -0.13038312, -4.559827833, -8.989272547, -13.41871726, -17.84816197, -22.27760669, -26.7070514, -31.13649612, -35.56594083, -39.99538554, -44.42483026, -48.85427497, -53.28371968, -57.7131644, -62.14260911, -66.57205383, -71.00149854, -75.43094325, -79.86038797, -84.28983268, -88.71927739, -93.14872211, -97.57816682, -102.0076115,
0, 15.78652556, 19.42291253, 17.60614531, 13.21991785, 7.9503543, 2.680790755, -2.588772791, -7.858336336, -13.12789988, -18.39746343, -23.66702697, -28.93659052, -34.20615406, -39.47571761, -44.74528116, -50.0148447, -55.28440825, -60.55397179, -65.82353534, -71.09309888, -76.36266243, -81.63222597, -86.90178952, -92.17135307, -97.44091661, -102.7104802, -107.9800437, -113.2496072, -118.5191708, -123.7887343,
0, 16.68433456, 20.17051392, 17.87474177, 12.65147866, 6.311027571, -0.029423518, -6.369874607, -12.7103257, -19.05077679, -25.39122787, -31.73167896, -38.07213005, -44.41258114, -50.75303223, -57.09348332, -63.43393441, -69.7743855, -76.11483659, -82.45528768, -88.79573877, -95.13618985, -101.4766409, -107.817092, -114.1575431, -120.4979942, -126.8384453, -133.1788964, -139.5193475, -145.8597986, -152.2002497,
0, 17.41040418, 20.57382675, 17.61118569, 11.31238835, 3.623273159, -4.065842032, -11.75495722, -19.44407241, -27.1331876, -34.8223028, -42.51141799, -50.20053318, -57.88964837, -65.57876356, -73.26787875, -80.95699394, -88.64610913, -96.33522432, -104.0243395, -111.7134547, -119.4025699, -127.0916851, -134.7808003, -142.4699155, -150.1590307, -157.8481459, -165.537261, -173.2263762, -180.9154914, -188.6046066,
0, 17.93871972, 20.59070377, 16.75302536, 9.191774239, -0.087690146, -9.454107532, -18.82052492, -28.18694231, -37.55335969, -46.91977708, -56.28619446, -65.65261185, -75.01902924, -84.38544662, -93.75186401, -103.1182814, -112.4846988, -121.8511162, -131.2175336, -140.5839509, -149.9503683, -159.3167857, -168.6832031, -178.0496205, -187.4160379, -196.7824553, -206.1488726, -215.51529, -224.8817074, -234.2481248,
0, 18.2515141, 20.21494243, 15.29849404, 6.350964039, -4.732633004, -16.17016535, -27.60769769, -39.04523003, -50.48276238, -61.92029472, -73.35782706, -84.79535941, -96.23289175, -107.6704241, -119.1079564, -130.5454888, -141.9830211, -153.4205535, -164.8580858, -176.2956182, -187.7331505, -199.1706828, -210.6082152, -222.0457475, -233.4832799, -244.9208122, -256.3583446, -267.7958769, -279.2334092, -290.6709416,
0, 18.33777783, 19.49181457, 13.33701752, 2.859800104, -10.3345775, -24.33845237, -38.34232724, -52.34620211, -66.35007698, -80.35395184, -94.35782671, -108.3617016, -122.3655765, -136.3694513, -150.3733262, -164.3772011, -178.3810759, -192.3849508, -206.3888257, -220.3927005, -234.3965754, -248.4004503, -262.4043251, -276.4082, -290.4120749, -304.4159497, -318.4198246, -332.4236995, -346.4275744, -360.4314492,
0, 18.17538231, 18.51002497, 11.04665812, -0.994780874, -16.63031048, -33.87687523, -51.12343998, -68.37000473, -85.61656948, -102.8631342, -120.109699, -137.3562637, -154.6028285, -171.8493932, -189.095958, -206.3425227, -223.5890875, -240.8356522, -258.082217, -275.3287817, -292.5753465, -309.8219112, -327.068476, -344.3150407, -361.5616055, -378.8081702, -396.054735, -413.3012997, -430.5478645, -447.7944292,
0, 17.65609299, 17.27920508, 8.524291419, -5.057608191, -22.50102498, -42.90498828, -64.41294726, -85.92090625, -107.4288652, -128.9368242, -150.4447832, -171.9527422, -193.4607012, -214.9686602, -236.4766191, -257.9845781, -279.4925371, -301.0004961, -322.5084551, -344.0164141, -365.524373, -387.032332, -408.540291, -430.04825, -451.556209, -473.064168, -494.572127, -516.0800859, -537.5880449, -559.0960039,
0, 16.366417, 15.28453357, 5.177472773, -10.14981954, -29.25313149, -51.71032854, -77.44040776, -104.8405088, -132.2777748, -159.7150407, -187.1523066, -214.5895725, -242.0268384, -269.4641043, -296.9013702, -324.3386362, -351.7759021, -379.213168, -406.6504339, -434.0876998, -461.5249657, -488.9622317, -516.3994976, -543.8367635, -571.2740294, -598.7112953, -626.1485612, -653.5858272, -681.0230931, -708.460359,
0, 13.14438232, 10.47269335, -1.665167583, -19.67902817, -41.69708427, -66.84234493, -94.75990207, -125.3334257, -158.5352896, -193.9122568, -230.1519859, -266.3917149, -302.631444, -338.8711731, -375.1109022, -411.3506313, -447.5903603, -483.8300894, -520.0698185, -556.3095476, -592.5492766, -628.7890057, -665.0287348, -701.2684639, -737.508193, -773.747922, -809.9876511, -846.2273802, -882.4671093, -918.7068383,
0, 5.518327532, -2.171177197, -19.04582398, -42.35244345, -70.2670677, -101.6270398, -135.7248544, -172.1537755, -210.6960433, -251.2452669, -293.7554124, -338.2096402, -384.6031238, -432.934915, -482.6291068, -532.716593, -582.8040792, -632.8915654, -682.9790516, -733.0665378, -783.154024, -833.2415102, -883.3289964, -933.4164826, -983.5039688, -1033.591455, -1083.678941, -1133.766427, -1183.853914, -1233.9414,
0, -10.78409646, -31.74069306, -60.69428977, -95.93671636, -136.1429577, -180.2989361, -227.6400545, -277.5993666, -329.7643072, -383.8409764, -439.6250371, -496.9783494, -555.8105266, -616.0646631, -677.7065467, -740.7167313, -805.0849076, -870.8060737, -937.878069, -1006.300097, -1076.071922, -1147.193495, -1219.479386, -1292.254723, -1365.030059, -1437.805396, -1510.580732, -1583.356069, -1656.131405, -1728.906742,
0, -42.52933525, -92.42156288, -148.624444, -210.2340805, -276.4774651, -346.696653, -420.3344478, -496.9214963, -576.0646937, -657.4368016, -740.767189, -825.8336073, -912.4549164, -1000.484682, -1089.80557, -1180.324461, -1271.968225, -1364.680084, -1458.416511, -1553.144595, -1648.839838, -1745.484312, -1843.065153, -1941.573328, -2041.002652, -2141.34901, -2242.609755, -2344.783249, -2447.868526, -2551.865047,
0, -100.4100459, -205.9517933, -316.1468346, -430.5603892, -548.7979818, -670.5023221, -795.3503757, -923.0506199, -1053.340474, -1185.983896, -1320.76914, -1457.506661, -1596.027171, -1736.17982, -1877.83052, -2020.860378, -2165.164256, -2310.649436, -2457.234387, -2604.847633, -2753.426709, -2902.917212, -3053.271917, -3204.449989, -3356.416249, -3509.140517, -3662.597011, -3816.763809, -3971.622358, -4127.157038,
0, 14.46632767, 20.9500635, 22.14230874, 20.83225569, 18.98640912, 17.14056255, 15.29471599, 13.44886942, 11.60302285, 9.757176283, 7.911329715, 6.065483148, 4.219636581, 2.373790013, 0.527943446, -1.317903122, -3.163749689, -5.009596257, -6.855442824, -8.701289391, -10.54713596, -12.39298253, -14.23882909, -16.08467566, -17.93052223, -19.7763688, -21.62221536, -23.46806193, -25.3139085, -27.15975507,
0, 15.4889815, 21.47408571, 21.9668641, 20.07380419, 17.63770078, 15.20159737, 12.76549397, 10.32939056, 7.89328715, 5.457183742, 3.021080334, 0.584976926, -1.851126482, -4.287229889, -6.723333297, -9.159436705, -11.59554011, -14.03164352, -16.46774693, -18.90385034, -21.33995374, -23.77605715, -26.21216056, -28.64826397, -31.08436738, -33.52047078, -35.95657419, -38.3926776, -40.82878101, -43.26488441,
0, 16.78798763, 22.74156629, 22.95114004, 20.6838946, 17.7647292, 14.84556381, 11.92639842, 9.007233032, 6.088067641, 3.16890225, 0.249736859, -2.669428531, -5.588593922, -8.507759313, -11.4269247, -14.34609009, -17.26525549, -20.18442088, -23.10358627, -26.02275166, -28.94191705, -31.86108244, -34.78024783, -37.69941322, -40.61857861, -43.537744, -46.45690939, -49.37607479, -52.29524018, -55.21440557,
0, 18.19273874, 24.29512611, 24.37795006, 21.79520344, 18.40204914, 15.00889485, 11.61574056, 8.22258627, 4.829431978, 1.436277686, -1.956876606, -5.350030898, -8.743185189, -12.13633948, -15.52949377, -18.92264806, -22.31580236, -25.70895665, -29.10211094, -32.49526523, -35.88841952, -39.28157382, -42.67472811, -46.0678824, -49.46103669, -52.85419098, -56.24734527, -59.64049957, -63.03365386, -66.42680815,
0, 19.60843437, 25.90739667, 25.89593833, 22.98076916, 19.05954657, 15.12161744, 11.18368831, 7.245759183, 3.307830053, -0.630099077, -4.568028207, -8.505957337, -12.44388647, -16.3818156, -20.31974473, -24.25767386, -28.19560299, -32.13353212, -36.07146125, -40.00939038, -43.94731951, -47.88524864, -51.82317777, -55.7611069, -59.69903603, -63.63696516, -67.57489429, -71.51282342, -75.45075255, -79.38868168,
0, 20.96456653, 27.41929553, 27.2602268, 23.97886024, 19.46235026, 14.83902847, 10.21570668, 5.592384888, 0.969063096, -3.654258696, -8.277580488, -12.90090228, -17.52422407, -22.14754586, -26.77086766, -31.39418945, -36.01751124, -40.64083303, -45.26415482, -49.88747662, -54.51079841, -59.1341202, -63.75744199, -68.38076378, -73.00408558, -77.62740737, -82.25072916, -86.87405095, -91.49737274, -96.12069454,
0, 22.20270358, 28.70373776, 28.27543296, 24.48377777, 19.19480497, 13.68214868, 8.16949239, 2.656836102, -2.855820186, -8.368476475, -13.88113276, -19.39378905, -24.90644534, -30.41910163, -35.93175792, -41.4444142, -46.95707049, -52.46972678, -57.98238307, -63.49503936, -69.00769565, -74.52035193, -80.03300822, -85.54566451, -91.0583208, -96.57097709, -102.0836334, -107.5962897, -113.108946, -118.6216022,
0, 23.27251141, 29.65471548, 28.7786828, 24.27838284, 17.98336537, 11.31832302, 4.653280675, -2.011761671, -8.676804018, -15.34184636, -22.00688871, -28.67193106, -35.3369734, -42.00201575, -48.6670581, -55.33210044, -61.99714279, -68.66218514, -75.32722748, -81.99226983, -88.65731218, -95.32235452, -101.9873969, -108.6524392, -115.3174816, -121.9825239, -128.6475663, -135.3126086, -141.9776509, -148.6426933,
0, 24.13100735, 30.18661865, 28.63863905, 23.18696655, 15.6017643, 7.463710867, -0.674342566, -8.812395999, -16.95044943, -25.08850286, -33.2265563, -41.36460973, -49.50266316, -57.6407166, -65.77877003, -73.91682346, -82.05487689, -90.19293033, -98.33098376, -106.4690372, -114.6070906, -122.7451441, -130.8831975, -139.0212509, -147.1593044, -155.2973578, -163.4354112, -171.5734647, -179.7115181, -187.8495715,
0, 24.74397907, 30.24175775, 27.76763576, 21.09365161, 11.8908067, 1.89811187, -8.094582964, -18.0872778, -28.07997263, -38.07266746, -48.0653623, -58.05805713, -68.05075196, -78.0434468, -88.03614163, -98.02883646, -108.0215313, -118.0142261, -128.006921, -137.9996158, -147.9923106, -157.9850055, -167.9777003, -177.9703951, -187.96309, -197.9557848, -207.9484796, -217.9411745, -227.9338693, -237.9265641,
0, 25.08890805, 29.80694098, 26.14906156, 17.98298719, 6.801572355, -5.50237509, -17.80632253, -30.11026998, -42.41421742, -54.71816487, -67.02211231, -79.32605976, -91.6300072, -103.9339546, -116.2379021, -128.5418495, -140.845797, -153.1497444, -165.4536919, -177.7576393, -190.0615868, -202.3655342, -214.6694817, -226.9734291, -239.2773765, -251.581324, -263.8852714, -276.1892189, -288.4931663, -300.7971138,
0, 25.1562852, 28.93629979, 23.87720095, 13.99840862, 0.453697605, -14.72905693, -29.91181147, -45.09456601, -60.27732055, -75.46007509, -90.64282962, -105.8255842, -121.0083387, -136.1910932, -151.3738478, -166.5566023, -181.7393569, -196.9221114, -212.1048659, -227.2876205, -242.470375, -257.6531295, -272.8358841, -288.0186386, -303.2013932, -318.3841477, -333.5669022, -348.7496568, -363.9324113, -379.1151659,
0, 24.93625649, 27.75647352, 21.17549032, 9.463503027, -6.378434367, -24.71137294, -43.53090354, -62.35043415, -81.16996476, -99.98949537, -118.809026, -137.6285566, -156.4480872, -175.2676178, -194.0871484, -212.906679, -231.7262096, -250.5457402, -269.3652708, -288.1848014, -307.004332, -325.8238627, -344.6433933, -363.4629239, -382.2824545, -401.1019851, -419.9215157, -438.7410463, -457.5605769, -476.3801075,
0, 24.34880552, 26.36373067, 18.26685769, 4.677702659, -13.06275651, -34.69561325, -58.26134031, -81.82706736, -105.3927944, -128.9585215, -152.5242485, -176.0899756, -199.6557026, -223.2214297, -246.7871568, -270.3528838, -293.9186109, -317.4843379, -341.050065, -364.615792, -388.1815191, -411.7472461, -435.3129732, -458.8787003, -482.4444273, -506.0101544, -529.5758814, -553.1416085, -576.7073355, -600.2730626,
0, 23.0229522, 24.38761148, 14.80569478, -0.848652848, -20.68314382, -44.11499381, -71.01897626, -100.3752103, -130.4546827, -160.5341551, -190.6136275, -220.6930999, -250.7725723, -280.8520447, -310.9315171, -341.0109895, -371.0904619, -401.1699343, -431.2494067, -461.3288792, -491.4083516, -521.487824, -551.5672964, -581.6467688, -611.7262412, -641.8057136, -671.885186, -701.9646584, -732.0441308, -762.1236032,
0, 19.82627277, 19.91668876, 8.431141049, -10.04131788, -33.11209846, -59.65691261, -89.21355416, -121.6249226, -156.8507544, -194.8844659, -234.3905611, -273.9653381, -313.540115, -353.1148919, -392.6896688, -432.2644457, -471.8392227, -511.4139996, -550.9887765, -590.5635534, -630.1383303, -669.7131072, -709.2878842, -748.8626611, -788.437438, -828.0122149, -867.5869918, -907.1617688, -946.7365457, -986.3113226,
0, 12.25074996, 7.908560577, -7.786583056, -31.28036362, -60.23828663, -93.18416465, -129.2259656, -167.8535808, -208.7945895, -251.9156108, -297.1583534, -344.50098, -393.9369164, -445.4647374, -498.86778, -553.1001885, -607.332597, -661.5650055, -715.7974139, -770.0298224, -824.2622309, -878.4946394, -932.7270479, -986.9594564, -1041.191865, -1095.424273, -1149.656682, -1203.88909, -1258.121499, -1312.353907,
0, -4.086535415, -21.02733034, -47.95493823, -82.63538952, -123.3495441, -168.792298, -217.9876817, -270.2180119, -324.9653895, -381.8639568, -440.6614504, -501.1887062, -563.3358894, -627.0343377, -692.2430188, -758.9387109, -827.1091234, -896.7482772, -967.8535641, -1040.424001, -1114.459284, -1189.959346, -1266.924175, -1344.700619, -1422.556292, -1500.411966, -1578.26764, -1656.123314, -1733.978988, -1811.834661,
0, -36.06387596, -81.46240961, -134.7987575, -194.8796196, -260.690238, -331.3718623, -406.2015006, -484.5737849, -565.9847872, -650.0176295, -736.3297415, -824.6416244, -914.726988, -1006.404138, -1099.52849, -1193.98611, -1289.688161, -1386.566178, -1484.568059, -1583.654711, -1683.797253, -1784.974714, -1887.17216, -1990.379185, -2094.588719, -2199.79609, -2305.998316, -2413.193559, -2521.380743, -2630.559269,
0, -94.42797557, -195.3660895, -302.1776983, -414.2862967, -531.1707308, -652.3607195, -777.4326677, -906.005758, -1037.738306, -1172.324369, -1309.490585, -1448.993251, -1590.615597, -1734.165274, -1879.47203, -2026.385561, -2174.773533, -2324.519766, -2475.522561, -2627.693174, -2780.954416, -2935.239377, -3090.490262, -3246.657342, -3403.697994, -3561.575839, -3720.259963, -3879.724219, -4039.946602, -4200.908686,
0, 21.96705423, 32.56045091, 35.82318212, 35.22214046, 33.35686316, 31.37660861, 29.39635405, 27.41609949, 25.43584494, 23.45559038, 21.47533582, 19.49508127, 17.51482671, 15.53457216, 13.5543176, 11.57406304, 9.593808488, 7.613553932, 5.633299375, 3.653044819, 1.672790263, -0.307464293, -2.287718849, -4.267973405, -6.248227962, -8.228482518, -10.20873707, -12.18899163, -14.16924619, -16.14950074,
0, 23.07855113, 33.02058318, 35.39882205, 34.15155866, 31.67681342, 29.01700749, 26.35720156, 23.69739563, 21.03758971, 18.37778378, 15.71797785, 13.05817192, 10.39836599, 7.738560065, 5.078754137, 2.418948209, -0.240857719, -2.900663648, -5.560469576, -8.220275504, -10.88008143, -13.53988736, -16.19969329, -18.85949922, -21.51930514, -24.17911107, -26.838917, -29.49872293, -32.15852886, -34.81833478,
0, 24.54472252, 34.42276842, 36.47336264, 34.91713499, 32.01272306, 28.82336616, 25.63400926, 22.44465236, 19.25529546, 16.06593856, 12.87658166, 9.687224757, 6.497867856, 3.308510955, 0.119154054, -3.070202847, -6.259559748, -9.448916649, -12.63827355, -15.82763045, -19.01698735, -22.20634425, -25.39570115, -28.58505805, -31.77441496, -34.96377186, -38.15312876, -41.34248566, -44.53184256, -47.72119946,
0, 26.15574047, 36.20896754, 38.15550215, 36.42156187, 33.1455119, 29.4594641, 25.7734163, 22.0873685, 18.40132071, 14.71527291, 11.02922511, 7.343177314, 3.657129516, -0.028918281, -3.714966078, -7.401013876, -11.08706167, -14.77310947, -18.45915727, -22.14520507, -25.83125286, -29.51730066, -33.20334846, -36.88939626, -40.57544405, -44.26149185, -47.94753965, -51.63358744, -55.31963524, -59.00568304,
0, 27.79493153, 38.10052803, 40.00922342, 38.11022383, 34.42504532, 30.18009432, 25.93514332, 21.69019231, 17.44524131, 13.20029031, 8.955339307, 4.710388304, 0.465437301, -3.779513701, -8.024464704, -12.26941571, -16.51436671, -20.75931771, -25.00426871, -29.24921972, -33.49417072, -37.73912172, -41.98407272, -46.22902373, -50.47397473, -54.71892573, -58.96387673, -63.20882774, -67.45377874, -71.69872974,
0, 29.37547269, 39.90222125, 41.73196797, 39.5896016, 35.37527956, 30.42608559, 25.47689162, 20.52769765, 15.57850368, 10.62930971, 5.680115743, 0.730921774, -4.218272196, -9.167466166, -14.11666014, -19.06585411, -24.01504808, -28.96424205, -33.91343602, -38.86262999, -43.81182396, -48.76101792, -53.71021189, -58.65940586, -63.60859983, -68.5577938, -73.50698777, -78.45618174, -83.40537571, -88.35456968,
0, 30.82526775, 41.45774839, 43.08271142, 40.54185622, 35.60434091, 29.73011682, 23.85589272, 17.98166863, 12.10744454, 6.233220448, 0.358996356, -5.515227736, -11.38945183, -17.26367592, -23.13790001, -29.0121241, -34.8863482, -40.76057229, -46.63479638, -52.50902047, -58.38324457, -64.25746866, -70.13169275, -76.00591684, -81.88014093, -87.75436503, -93.62858912, -99.50281321, -105.3770373, -111.2512614,
0, 32.08199992, 42.6361776, 43.86003731, 40.69889662, 34.77665739, 27.68618158, 20.59570577, 13.50522995, 6.41475414, -0.675721673, -7.766197487, -14.8566733, -21.94714911, -29.03762493, -36.12810074, -43.21857655, -50.30905237, -57.39952818, -64.49000399, -71.58047981, -78.67095562, -85.76143143, -92.85190725, -99.94238306, -107.0328589, -114.1233347, -121.2138105, -128.3042863, -135.3947621, -142.4852379,
0, 33.09223134, 43.33087734, 43.8999469, 39.84067907, 32.61074181, 23.9444785, 15.2782152, 6.611951891, -2.054311414, -10.72057472, -19.38683803, -28.05310133, -36.71936464, -45.38562794, -54.05189125, -62.71815455, -71.38441786, -80.05068116, -88.71694447, -97.38320777, -106.0494711, -114.7157344, -123.3819977, -132.048261, -140.7145243, -149.3807876, -158.0470509, -166.7133142, -175.3795775, -184.0458408,
0, 33.81335709, 43.46847145, 43.0892919, 37.81387685, 28.93670128, 18.30193392, 7.629050205, -3.043833511, -13.71671723, -24.38960094, -35.06248466, -45.73536838, -56.40825209, -67.08113581, -77.75401953, -88.42690324, -99.09978696, -109.7726707, -120.4455544, -131.1184381, -141.7913218, -152.4642055, -163.1370893, -173.809973, -184.4828567, -195.1557404, -205.8286241, -216.5015078, -227.1743916, -237.8472753,
0, 34.21799161, 43.02924248, 41.39746813, 34.57541028, 23.85120612, 10.95641712, -2.238834735, -15.43408659, -28.62933845, -41.82459031, -55.01984216, -68.21509402, -81.41034588, -94.60559774, -107.8008496, -120.9961015, -134.1913533, -147.3866052, -160.581857, -173.7771089, -186.9723607, -200.1676126, -213.3628645, -226.5581163, -239.7533682, -252.94862, -266.1438719, -279.3391237, -292.5343756, -305.7296275,
0, 34.29812762, 42.07754183, 38.92594911, 30.25993053, 17.42448201, 1.832309876, -14.52164133, -30.87559253, -47.22954374, -63.58349495, -79.93744615, -96.29139736, -112.6453486, -128.9992998, -145.353251, -161.7072022, -178.0611534, -194.4151046, -210.7690558, -227.123007, -243.4769582, -259.8309094, -276.1848606, -292.5388118, -308.892763, -325.2467142, -341.6006654, -357.9546166, -374.3085679, -390.6625191,
0, 34.05644303, 40.78059122, 35.94805026, 25.228552, 10.14482012, -8.603408506, -28.95219847, -49.30098844, -69.64977841, -89.99856837, -110.3473583, -130.6961483, -151.0449383, -171.3937282, -191.7425182, -212.0913082, -232.4400981, -252.7888881, -273.1376781, -293.486468, -313.835258, -334.184048, -354.5328379, -374.8816279, -395.2304179, -415.5792078, -435.9279978, -456.2767878, -476.6255777, -496.9743677,
0, 33.44231159, 39.32641785, 32.82216703, 19.934156, 2.566905094, -18.8502875, -43.3284842, -68.86888676, -94.40928931, -119.9496919, -145.4900944, -171.030497, -196.5708995, -222.1113021, -247.6517047, -273.1921072, -298.7325098, -324.2729123, -349.8133149, -375.3537174, -400.89412, -426.4345225, -451.9749251, -477.5153277, -503.0557302, -528.5961328, -554.1365353, -579.6769379, -605.2173404, -630.757743,
0, 32.13191668, 37.501735, 29.47260822, 14.25649124, -5.635844382, -29.37149627, -56.74474221, -87.63131976, -120.2272374, -152.8231551, -185.4190728, -218.0149904, -250.6109081, -283.2068258, -315.8027434, -348.3986611, -380.9945788, -413.5904964, -446.1864141, -478.7823318, -511.3782494, -543.9741671, -576.5700848, -609.1660025, -641.7619201, -674.3578378, -706.9537555, -739.5496731, -772.1455908, -804.7415085,
0, 29.03456854, 33.56825915, 23.89892216, 5.835205765, -17.57376511, -44.86790063, -75.42772489, -109.0305278, -145.6131789, -185.1632198, -227.0488546, -269.7870396, -312.5252247, -355.2634097, -398.0015948, -440.7397799, -483.4779649, -526.21615, -568.954335, -611.6925201, -654.4307052, -697.1688902, -739.9070753, -782.6452603, -825.3834454, -868.1216304, -910.8598155, -953.5980006, -996.3361856, -1039.074371,
0, 21.61988413, 22.49866877, 9.354852607, -13.27755422, -42.43293583, -76.24123762, -113.5733186, -153.7821044, -196.519777, -241.6137817, -288.9868968, -338.6089815, -390.4702958, -444.5684463, -500.9030482, -558.7077893, -616.8644586, -675.021128, -733.1777973, -791.3344667, -849.491136, -907.6478054, -965.8044747, -1023.961144, -1082.117813, -1140.274483, -1198.431152, -1256.587821, -1314.744491, -1372.90116,
0, 5.390597999, -5.39304706, -28.61940991, -61.39962704, -101.5245614, -147.3275956, -197.5700922, -251.3466904, -308.0078377, -367.0971735, -428.3015938, -491.4120337, -556.2931998, -622.8606775, -691.0640182, -760.8745884, -832.2771258, -905.2641059, -979.83217, -1055.980002, -1133.707169, -1213.013561, -1293.899159, -1376.068383, -1458.731629, -1541.394875, -1624.058121, -1706.721367, -1789.384613, -1872.047859,
0, -26.66643568, -65.12926208, -113.5523025, -170.3748138, -234.2763331, -304.1451811, -379.0503324, -458.2163791, -541.0013293, -626.8769949, -715.4117401, -806.2553716, -899.1259693, -993.7984647, -1090.094792, -1187.875443, -1287.032273, -1387.482416, -1489.16318, -1592.027787, -1696.041874, -1801.180621, -1907.426445, -2014.767152, -2123.19449, -2232.703026, -2343.289297, -2454.951176, -2567.687412, -2681.497308,
0, -85.37265005, -178.984961, -279.9960621, -387.6471433, -501.2546496, -620.2039382, -743.9433708, -871.9788217, -1003.868576, -1139.218601, -1277.67816, -1418.935767, -1562.715439, -1708.773254, -1856.894171, -2006.889114, -2158.5923, -2311.858788, -2466.562241, -2622.592889, -2779.855667, -2938.268535, -3097.76095, -3258.272485, -3419.751589, -3582.154468, -3745.444083, -3909.589257, -4074.563881, -4240.3462,
0, 32.31162548, 48.96859155, 55.55329316, 56.49077815, 54.86800616, 52.54248632, 50.20460745, 47.86672859, 45.52884972, 43.19097085, 40.85309198, 38.51521312, 36.17733425, 33.83945538, 31.50157652, 29.16369765, 26.82581878, 24.48793992, 22.15006105, 19.81218218, 17.47430332, 15.13642445, 12.79854558, 10.46066671, 8.122787848, 5.784908981, 3.447030114, 1.109151247, -1.22872762, -3.566606487,
0, 33.48446866, 49.27587089, 54.6978934, 54.804173, 52.51736326, 49.45021574, 46.32468775, 43.19915976, 40.07363177, 36.94810379, 33.8225758, 30.69704781, 27.57151982, 24.44599184, 21.32046385, 18.19493586, 15.06940787, 11.94387989, 8.818351897, 5.69282391, 2.567295922, -0.558232066, -3.683760053, -6.809288041, -9.934816029, -13.06034402, -16.185872, -19.31139999, -22.43692798, -25.56245597,
0, 35.09952462, 50.75240332, 55.73626084, 55.51673695, 52.86846376, 49.27986043, 45.56545447, 41.85104851, 38.13664256, 34.4222366, 30.70783064, 26.99342468, 23.27901873, 19.56461277, 15.85020681, 12.13580086, 8.421394898, 4.706988941, 0.992582983, -2.721822974, -6.436228931, -10.15063489, -13.86504085, -17.5794468, -21.29385276, -25.00825872, -28.72266467, -32.43707063, -36.15147659, -39.86588255,
0, 36.90515806, 52.72861818, 57.58175167, 57.24458154, 54.3605072, 50.3336854, 46.09224984, 41.85081428, 37.60937872, 33.36794316, 29.1265076, 24.88507204, 20.64363648, 16.40220092, 12.16076536, 7.919329795, 3.677894235, -0.563541325, -4.804976885, -9.046412446, -13.28784801, -17.52928357, -21.77071913, -26.01215469, -30.25359025, -34.49502581, -38.73646137, -42.97789693, -47.21933249, -51.46076805,
0, 38.76175537, 54.87069672, 59.70584882, 59.30322411, 56.18880419, 51.6965608, 46.87918607, 42.06181134, 37.24443661, 32.42706188, 27.60968715, 22.79231241, 17.97493768, 13.15756295, 8.340188218, 3.522813486, -1.294561246, -6.111935978, -10.92931071, -15.74668544, -20.56406017, -25.38143491, -30.19880964, -35.01618437, -39.8335591, -44.65093383, -49.46830857, -54.2856833, -59.10305803, -63.92043276,
0, 40.56556762, 56.94528387, 61.74413814, 61.2143922, 57.77615912, 52.69520783, 47.15622737, 41.61724691, 36.07826645, 30.539286, 25.00030554, 19.46132508, 13.92234462, 8.383364165, 2.844383707, -2.694596751, -8.233577209, -13.77255767, -19.31153812, -24.85051858, -30.38949904, -35.9284795, -41.46745995, -47.00644041, -52.54542087, -58.08440133, -63.62338179, -69.16236224, -74.7013427, -80.24032316,
0, 42.23042072, 58.76565893, 63.40734135, 62.59494726, 58.65283354, 52.77335517, 46.27927733, 39.78519948, 33.29112164, 26.79704379, 20.30296595, 13.8088881, 7.314810257, 0.820732413, -5.673345432, -12.16742328, -18.66150112, -25.15557897, -31.64965681, -38.14373466, -44.6378125, -51.13189034, -57.62596819, -64.12004603, -70.61412388, -77.10820172, -83.60227957, -90.09635741, -96.59043526, -103.0845131,
0, 43.68169753, 60.17516043, 64.45380821, 63.12242843, 58.41944455, 51.4525052, 43.68787606, 35.92324691, 28.15861777, 20.39398863, 12.62935949, 4.86473035, -2.899898791, -10.66452793, -18.42915707, -26.19378621, -33.95841535, -41.7230445, -49.48767364, -57.25230278, -65.01693192, -72.78156106, -80.5461902, -88.31081934, -96.07544848, -103.8400776, -111.6047068, -119.3693359, -127.133965, -134.8985942,
0, 44.85521942, 61.04560837, 64.68599206, 62.53059634, 56.74373099, 48.32821943, 38.89836257, 29.46850572, 20.03864886, 10.608792, 1.178935142, -8.250921716, -17.68077857, -27.11063543, -36.54049229, -45.97034915, -55.400206, -64.83006286, -74.25991972, -83.68977658, -93.11963343, -102.5494903, -111.9793471, -121.409204, -130.8390609, -140.2689177, -149.6987746, -159.1286314, -168.5584883, -177.9883452,
0, 45.69967098, 61.28770387, 63.96542526, 60.62847457, 53.38579432, 43.09649802, 31.52500621, 19.9535144, 8.382022588, -3.189469222, -14.76096103, -26.33245284, -37.90394465, -49.47543646, -61.04692827, -72.61842008, -84.18991189, -95.7614037, -107.3328955, -118.9043873, -130.4758791, -142.0473709, -153.6188628, -165.1903546, -176.7618464, -188.3333382, -199.90483, -211.4763218, -223.0478136, -234.6193054,
0, 46.18236707, 60.87538929, 62.24930814, 57.34733654, 48.25875006, 35.61708471, 21.33257974, 7.048074769, -7.236430202, -21.52093517, -35.80544014, -50.08994511, -64.37445008, -78.65895505, -92.94346003, -107.227965, -121.51247, -135.7969749, -150.0814799, -164.3659849, -178.6504898, -192.9349948, -207.2194998, -221.5040048, -235.7885097, -250.0730147, -264.3575197, -278.6420246, -292.9265296, -307.2110346,
0, 46.29616101, 59.88403467, 59.65046831, 52.81783965, 41.52674979, 26.19914366, 8.686467579, -9.012759435, -26.71198645, -44.41121346, -62.11044047, -79.80966749, -97.5088945, -115.2081215, -132.9073485, -150.6065755, -168.3058026, -186.0050296, -203.7042566, -221.4034836, -239.1027106, -256.8019376, -274.5011646, -292.2003916, -309.8996187, -327.5988457, -345.2980727, -362.9972997, -380.6965267, -398.3957537,
0, 46.05539878, 58.52289475, 56.49880185, 47.44641358, 33.69164029, 15.74529372, -5.273280627, -27.29823527, -49.32318991, -71.34814455, -93.37309919, -115.3980538, -137.4230085, -159.4479631, -181.4729178, -203.4978724, -225.522827, -247.5477817, -269.5727363, -291.597691, -313.6226456, -335.6476002, -357.6725549, -379.6975095, -401.7224642, -423.7474188, -445.7723735, -467.7973281, -489.8222827, -511.8472374,
0, 45.43833227, 57.07399203, 53.29664311, 41.85103333, 25.46503132, 4.877809829, -19.77785655, -46.9357349, -74.56732502, -102.1989151, -129.8305053, -157.4620954, -185.0936855, -212.7252756, -240.3568658, -267.9884559, -295.620046, -323.2516361, -350.8832262, -378.5148164, -406.1464065, -433.7779966, -461.4095867, -489.0411769, -516.672767, -544.3043571, -571.9359472, -599.5675374, -627.1991275, -654.8307176,
0, 44.1711191, 55.48848565, 50.25165671, 36.30545033, 17.00233331, -6.439502409, -33.66898479, -64.61491648, -98.6127163, -133.8080552, -169.0033941, -204.198733, -239.394072, -274.5894109, -309.7847498, -344.9800887, -380.1754276, -415.3707665, -450.5661054, -485.7614443, -520.9567833, -556.1521222, -591.3474611, -626.5428, -661.7381389, -696.9334778, -732.1288167, -767.3241557, -802.5194946, -837.7148335,
0, 41.21747656, 52.22910755, 45.75991237, 29.07323293, 6.05474218, -21.37906145, -52.37677401, -86.60924163, -123.9714713, -164.438141, -208.0052947, -253.5379006, -299.4858605, -345.4338204, -391.3817803, -437.3297402, -483.2777001, -529.2256601, -575.17362, -621.1215799, -667.0695398, -713.0174997, -758.9654596, -804.9134195, -850.8613794, -896.8093393, -942.7572992, -988.7052591, -1034.653219, -1080.601179,
0, 34.04564489, 42.33891363, 33.33538949, 12.73849831, -15.71717818, -49.66905536, -87.68177511, -128.9243151, -172.9427741, -219.5064105, -268.5080768, -319.9035226, -373.6771291, -429.8244694, -488.3446351, -549.2375145, -611.3209703, -673.4086485, -735.4963267, -797.584005, -859.6716832, -921.7593614, -983.8470396, -1045.934718, -1108.022396, -1170.110074, -1232.197752, -1294.285431, -1356.373109, -1418.460787,
0, 18.04985195, 15.86527974, -1.770966371, -31.17225923, -69.5311015, -114.7377147, -165.229698, -219.8686313, -277.8398711, -338.572143, -401.6738744, -466.883536, -534.0315662, -603.0117448, -673.760153, -746.240117, -820.4317663, -896.3250642, -973.9153665, -1053.200753, -1134.180539, -1216.854526, -1301.222671, -1387.284968, -1474.457152, -1561.892149, -1649.327146, -1736.762143, -1824.197139, -1911.632136,
0, -13.93955619, -42.71955471, -83.95454682, -135.6256011, -196.03198, -263.7480773, -337.58517, -416.5575671, -499.8527623, -586.8052204, -676.8734534, -769.6200643, -864.6944581, -961.8179404, -1060.770948, -1161.382167, -1263.519329, -1367.081467, -1471.992468, -1578.195729, -1685.649789, -1794.324778, -1904.199571, -2015.259535, -2127.494769, -2240.898748, -2355.467298, -2471.197845, -2588.088862, -2706.139487,
0, -72.84565161, -156.0855814, -248.6190773, -349.4560301, -457.7074158, -572.5764538, -693.3504058, -819.3929773, -950.1372878, -1085.079376, -1223.772209, -1365.820163, -1510.873946, -1658.625941, -1808.805931, -1961.177194, -2115.532932, -2271.693014, -2429.501024, -2588.821566, -2749.537835, -2911.549416, -3074.770302, -3239.12711, -3404.557482, -3571.008654, -3738.436181, -3906.802802, -4076.077439, -4246.234301,
0, 46.16394927, 71.52334081, 83.28598956, 86.97528359, 86.45934026, 83.9991022, 80.93498375, 77.87086529, 74.80674683, 71.74262838, 68.67850992, 65.61439147, 62.55027301, 59.48615455, 56.4220361, 53.35791764, 50.29379918, 47.22968073, 44.16556227, 41.10144381, 38.03732536, 34.9732069, 31.90908845, 28.84496999, 25.78085153, 22.71673308, 19.65261462, 16.58849616, 13.52437771, 10.46025925,
0, 47.36946956, 71.58728213, 81.79618778, 84.30982796, 83.00426529, 79.78691897, 75.83898092, 71.85316971, 67.8673585, 63.88154729, 59.89573609, 55.90992488, 51.92411367, 47.93830246, 43.95249125, 39.96668004, 35.98086883, 31.99505763, 28.00924642, 24.02343521, 20.037624, 16.05181279, 12.06600158, 8.080190374, 4.094379165, 0.108567957, -3.877243252, -7.86305446, -11.84886567, -15.83467688,
0, 49.11208995, 73.07046156, 82.64378656, 84.70189022, 83.070637, 79.42064823, 74.86639029, 70.21216907, 65.55794785, 60.90372663, 56.24950541, 51.59528419, 46.94106297, 42.28684175, 37.63262053, 32.97839931, 28.32417809, 23.66995686, 19.01573564, 14.36151442, 9.707293203, 5.053071982, 0.398850762, -4.255370459, -8.90959168, -13.5638129, -18.21803412, -22.87225534, -27.52647656, -32.18069778,
0, 51.09664682, 75.18361219, 84.52771662, 86.44381635, 84.68767616, 80.7482731, 75.70241411, 70.4756252, 65.24883628, 60.02204736, 54.79525844, 49.56846953, 44.34168061, 39.11489169, 33.88810278, 28.66131386, 23.43452494, 18.20773602, 12.98094711, 7.754158191, 2.527369274, -2.699419643, -7.926208561, -13.15299748, -18.37978639, -23.60657531, -28.83336423, -34.06015315, -39.28694206, -44.51373098,
0, 53.15982278, 77.53480111, 86.82037073, 88.70665135, 86.87075952, 82.65468325, 77.10534626, 71.27493405, 65.44452185, 59.61410964, 53.78369743, 47.95328522, 42.12287301, 36.2924608, 30.4620486, 24.63163639, 18.80122418, 12.97081197, 7.140399763, 1.309987554, -4.520424654, -10.35083686, -16.18124907, -22.01166128, -27.84207349, -33.6724857, -39.5028979, -45.33331011, -51.16372232, -56.99413453,
0, 55.1804819, 79.85103872, 89.09217968, 90.91957782, 88.92822332, 84.3375522, 78.16276618, 71.58696716, 65.01116815, 58.43536913, 51.85957012, 45.2837711, 38.70797209, 32.13217307, 25.55637406, 18.98057504, 12.40477603, 5.828977014, -0.746822, -7.322621015, -13.89842003, -20.47421904, -27.05001806, -33.62581707, -40.20161609, -46.7774151, -53.35321412, -59.92901313, -66.50481215, -73.08061116,
0, 57.05804055, 81.91424888, 91.00362221, 92.62935295, 90.30373931, 85.14224862, 78.1208001, 70.55740318, 62.99400625, 55.43060933, 47.86721241, 40.30381548, 32.74041856, 25.17702164, 17.61362471, 10.05022779, 2.486830866, -5.076566057, -12.63996298, -20.2033599, -27.76675683, -35.33015375, -42.89355067, -50.4569476, -58.02034452, -65.58374144, -73.14713837, -80.71053529, -88.27393221, -95.83732914,
0, 58.70529456, 83.54136961, 92.27154366, 93.45610083, 90.52658052, 84.50980433, 76.3295638, 67.44267237, 58.55578093, 49.66888949, 40.78199806, 31.89510662, 23.00821519, 14.12132375, 5.234432316, -3.65245912, -12.53935056, -21.42624199, -30.31313343, -39.20002486, -48.0869163, -56.97380773, -65.86069917, -74.7475906, -83.63448204, -92.52137348, -101.4082649, -110.2951563, -119.1820478, -128.0689392,
0, 60.04699887, 84.58210538, 92.66420724, 93.08588716, 89.20374637, 81.97046617, 72.23606821, 61.59949348, 50.96291875, 40.32634402, 29.68976929, 19.05319456, 8.416619834, -2.219954896, -12.85652963, -23.49310435, -34.12967908, -44.76625381, -55.40282854, -66.03940327, -76.675978, -87.31255273, -97.94912746, -108.5857022, -119.2222769, -129.8588516, -140.4954264, -151.1320011, -161.7685758, -172.4051506,
0, 61.02263414, 84.9306645, 92.0180984, 91.29065761, 86.04422041, 77.17454952, 65.41623906, 52.5114913, 39.60674354, 26.70199578, 13.79724802, 0.892500263, -12.0122475, -24.91699526, -37.82174301, -50.72649077, -63.63123853, -76.53598629, -89.44073405, -102.3454818, -115.2502296, -128.1549773, -141.0597251, -153.9644728, -166.8692206, -179.7739684, -192.6787161, -205.5834639, -218.4882116, -231.3929594,
0, 61.59334432, 84.55399169, 90.28010924, 87.97994746, 80.92140856, 69.96949245, 55.65496997, 39.85841466, 24.06185935, 8.265304041, -7.531251269, -23.32780658, -39.12436189, -54.9209172, -70.71747251, -86.51402782, -102.3105831, -118.1071384, -133.9036937, -149.7002491, -165.4968044, -181.2933597, -197.089915, -212.8864703, -228.6830256, -244.4795809, -260.2761362, -276.0726915, -291.8692468, -307.6658022,
0, 61.75130174, 83.53762152, 87.57883335, 83.28928321, 73.9789882, 60.52883051, 43.0888364, 23.64811101, 4.195932796, -15.25624542, -34.70842363, -54.16060185, -73.61278006, -93.06495828, -112.5171365, -131.9693147, -151.4214929, -170.8736711, -190.3258494, -209.7780276, -229.2302058, -248.682384, -268.1345622, -287.5867404, -307.0389186, -326.4910969, -345.9432751, -365.3954533, -384.8476315, -404.2998097,
0, 61.52008714, 82.13147676, 84.30758442, 77.68427234, 65.74788199, 49.4839886, 29.08275595, 5.807221915, -18.28327098, -42.37376388, -66.46425678, -90.55474968, -114.6452426, -138.7357355, -162.8262284, -186.9167213, -211.0072142, -235.0977071, -259.1882, -283.2786929, -307.3691858, -331.4596787, -355.5501716, -379.6406645, -403.7311574, -427.8216503, -451.9121431, -476.002636, -500.0931289, -524.1836218,
0, 60.90404688, 80.70939033, 81.11877355, 71.96223037, 57.11361245, 37.83015163, 14.41860293, -13.07325551, -42.88380546, -72.97455659, -103.0653077, -133.1560588, -163.24681, -193.3375611, -223.4283122, -253.5190633, -283.6098145, -313.7005656, -343.7913167, -373.8820678, -403.972819, -434.0635701, -464.1543212, -494.2450724, -524.3358235, -554.4265746, -584.5173257, -614.6080769, -644.698828, -674.7895791,
0, 59.67959575, 79.38789518, 78.50707437, 66.80175166, 48.73362959, 26.09549855, -0.508298602, -30.91934137, -65.10930041, -102.2175114, -140.3558202, -178.4941291, -216.6324379, -254.7707467, -292.9090555, -331.0473644, -369.1856732, -407.323982, -445.4622908, -483.6005997, -521.7389085, -559.8772173, -598.0155261, -636.153835, -674.2921438, -712.4304526, -750.5687614, -788.7070703, -826.8453791, -864.9836879,
0, 56.87683783, 76.85375138, 75.28572672, 61.11755671, 39.2728146, 12.27939664, -18.67341978, -53.08574052, -90.77671558, -131.6932022, -175.8236691, -223.166558, -272.3780358, -321.8519369, -371.325838, -420.799739, -470.2736401, -519.7475412, -569.2214422, -618.6953433, -668.1692444, -717.6431454, -767.1170465, -816.5909476, -866.0648486, -915.5387497, -965.0126508, -1014.486552, -1063.960453, -1113.434354,
0, 49.98989913, 68.28629187, 65.30709741, 48.11087873, 21.35042425, -12.00274867, -50.1193351, -91.92080465, -136.8031846, -184.4486986, -234.7025359, -287.4962607, -342.8032048, -400.6145945, -460.9281526, -523.7434627, -588.9288327, -655.2334075, -721.5379823, -787.8425571, -854.1471319, -920.4517067, -986.7562815, -1053.060856, -1119.365431, -1185.670006, -1251.974581, -1318.279155, -1384.58373, -1450.888305,
0, 34.32298119, 43.52830958, 33.63803963, 9.284718654, -26.00953601, -69.60096239, -119.5328251, -174.3791667, -233.1185347, -295.033109, -359.629154, -426.5751893, -495.6547112, -566.7307067, -639.7195806, -714.5724666, -791.2622141, -869.7746326, -950.1028411, -1032.243802, -1116.196327, -1201.96002, -1289.534775, -1378.92057, -1470.117404, -1562.398339, -1654.857856, -1747.317373, -1839.77689, -1932.236407,
0, 2.534257543, -13.48378782, -44.99842169, -89.43207748, -144.6206379, -208.7560486, -280.3355902, -358.1171953, -441.0802399, -528.3912759, -619.3742112, -713.484477, -810.2867577, -909.4358938, -1010.660594, -1113.74963, -1218.540207, -1324.908239, -1432.760278, -1542.026869, -1652.657128, -1764.614365, -1877.872588, -1992.413736, -2108.225538, -2225.299854, -2343.631439, -2463.217015, -2584.054612, -2706.143095,
0, -56.43298096, -125.9141535, -207.0182635, -298.4669326, -399.1156007, -507.9414383, -624.0321721, -746.5757678, -874.8509178, -1008.218285, -1146.112452, -1288.034535, -1433.545413, -1582.259536, -1733.839268, -1887.989736, -2044.454138, -2203.009483, -2363.462738, -2525.647336, -2689.420025, -2854.658044, -3021.256569, -3189.126445, -3358.192144, -3528.389957, -3699.666382, -3871.976706, -4045.283738, -4219.556716,
0, 64.21548987, 101.6828701, 121.2112213, 129.490232, 131.2804078, 129.6327992, 126.1577299, 121.8981746, 117.5266214, 113.1550682, 108.783515, 104.4119618, 100.0404086, 95.66885537, 91.29730216, 86.92574895, 82.55419574, 78.18264253, 73.81108932, 69.43953611, 65.06798291, 60.6964297, 56.32487649, 51.95332328, 47.58177007, 43.21021686, 38.83866365, 34.46711044, 30.09555723, 25.72400402,
0, 65.42434415, 101.4179347, 118.8835931, 125.4653127, 126.10529, 123.61682, 119.279465, 114.001308, 108.5405916, 103.0798752, 97.61915879, 92.15844238, 86.69772598, 81.23700958, 75.77629318, 70.31557678, 64.85486038, 59.39414398, 53.93342757, 48.47271117, 43.01199477, 37.55127837, 32.09056197, 26.62984557, 21.16912917, 15.70841276, 10.24769636, 4.786979962, -0.67373644, -6.134452841,
0, 67.27086073, 102.8390898, 119.3754055, 125.2307922, 125.4235981, 122.56876, 117.735148, 111.767891, 105.5313531, 99.29481525, 93.05827739, 86.82173954, 80.58520168, 74.34866383, 68.11212597, 61.87558812, 55.63905026, 49.40251241, 43.16597455, 36.9294367, 30.69289884, 24.45636099, 18.21982313, 11.98328528, 5.746747422, -0.489790433, -6.726328288, -12.96286614, -19.199404, -25.43594185,
0, 69.41544434, 105.0319923, 121.1560627, 126.7264682, 126.7893632, 123.7860955, 118.6310234, 112.1257363, 105.2480503, 98.37036422, 91.49267818, 84.61499214, 77.73730609, 70.85962005, 63.98193401, 57.10424797, 50.22656192, 43.34887588, 36.47118984, 29.5935038, 22.71581775, 15.83813171, 8.960445669, 2.082759627, -4.794926416, -11.67261246, -18.5502985, -25.42798454, -32.30567059, -39.18335663,
0, 71.67058363, 107.5445589, 123.4937852, 128.9685943, 129.0139729, 125.9161248, 120.4689039, 113.4336334, 105.9059954, 98.37835743, 90.85071946, 83.3230815, 75.79544354, 68.26780558, 60.74016761, 53.21252965, 45.68489169, 38.15725372, 30.62961576, 23.1019778, 15.57433984, 8.046701873, 0.51906391, -7.008574052, -14.53621202, -22.06384998, -29.59148794, -37.1191259, -44.64676387, -52.17440183,
0, 73.89742204, 110.0630851, 125.8914229, 131.2885328, 131.2789214, 128.0086345, 122.1742614, 114.4927953, 106.1814322, 97.87006917, 89.55870612, 81.24734308, 72.93598003, 64.62461699, 56.31325394, 48.0018909, 39.69052785, 31.37916481, 23.06780176, 14.75643872, 6.445075674, -1.866287371, -10.17765042, -18.48901346, -26.80037651, -35.11173955, -43.4231026, -51.73446564, -60.04582868, -68.35719173,
0, 75.98070074, 112.3374339, 127.957808, 133.1597243, 132.9338272, 129.2989303, 122.8714845, 114.315557, 104.9733892, 95.63122129, 86.28905342, 76.94688554, 67.60471767, 58.26254979, 48.92038192, 39.57821404, 30.23604617, 20.89387829, 11.55171042, 2.209542543, -7.132625332, -16.47479321, -25.81696108, -35.15912896, -44.50129683, -53.84346471, -63.18563258, -72.52780046, -81.86996833, -91.21213621,
0, 77.82036317, 114.1575554, 129.3673686, 134.1428556, 133.4319592, 129.1383136, 121.8117779, 112.0502896, 101.3246395, 90.59898946, 79.87333939, 69.14768932, 58.42203925, 47.69638918, 36.97073911, 26.24508904, 15.51943896, 4.793788893, -5.931861179, -16.65751125, -27.38316132, -38.10881139, -48.83446146, -59.56011154, -70.28576161, -81.01141168, -91.73706175, -102.4627118, -113.1883619, -123.914012,
0, 79.32973009, 115.3503507, 129.8535979, 133.8754349, 132.3173555, 126.9808577, 118.3611449, 106.9684353, 94.40510793, 81.84178056, 69.27845318, 56.71512581, 44.15179844, 31.58847107, 19.0251437, 6.461816326, -6.101511045, -18.66483842, -31.22816579, -43.79149316, -56.35482053, -68.9181479, -81.48147527, -94.04480265, -106.60813, -119.1714574, -131.7347848, -144.2981121, -156.8614395, -169.4247669,
0, 80.43841498, 115.7924496, 129.2277966, 132.0933222, 129.2489996, 122.4119178, 112.0352648, 98.50026987, 83.54206713, 68.58386438, 53.62566164, 38.6674589, 23.70925616, 8.75105342, -6.207149321, -21.16535206, -36.1235548, -51.08175754, -66.03996029, -80.99816303, -95.95636577, -110.9145685, -125.8727712, -140.830974, -155.7891767, -170.7473795, -185.7055822, -200.663785, -215.6219877, -230.5801904,
0, 81.1000858, 115.4418679, 127.4272809, 128.6887467, 124.0673109, 115.2253999, 102.5911556, 86.32924881, 68.30354034, 50.27783188, 32.25212341, 14.22641495, -3.79929352, -21.82500199, -39.85071045, -57.87641892, -75.90212738, -93.92783585, -111.9535443, -129.9792528, -148.0049612, -166.0306697, -184.0563782, -202.0820866, -220.1077951, -238.1335036, -256.159212, -274.1849205, -292.210629, -310.2363374,
0, 81.30376356, 114.3908844, 124.5986344, 123.8121481, 116.9109936, 105.5583638, 90.18531653, 70.8644182, 49.26042703, 27.344452, 5.428476969, -16.48749806, -38.4034731, -60.31944813, -82.23542316, -104.1513982, -126.0673732, -147.9833483, -169.8993233, -191.8152983, -213.7312734, -235.6472484, -257.5632234, -279.4791985, -301.3951735, -323.3111485, -345.2271236, -367.1430986, -389.0590736, -410.9750487,
0, 81.07812388, 112.9243753, 121.2019973, 118.004239, 108.36511, 94.05284129, 75.55415746, 52.96143466, 27.21507803, 0.357531297, -26.50001544, -53.35756217, -80.2151089, -107.0726556, -133.9302024, -160.7877491, -187.6452958, -214.5028426, -241.3603893, -268.217936, -295.0754828, -321.9330295, -348.7905762, -375.648123, -402.5056697, -429.3632164, -456.2207632, -483.0783099, -509.9358566, -536.7934034,
0, 80.44820141, 111.4996863, 118.0408198, 112.2560623, 99.51446408, 81.873009, 59.98125757, 33.99413621, 3.935561592, -28.73071155, -61.96981528, -95.20891901, -128.4480227, -161.6871265, -194.9262302, -228.1653339, -261.4044377, -294.6435414, -327.8826451, -361.1217489, -394.3608526, -427.5999563, -460.83906, -494.0781638, -527.3172675, -560.5563712, -593.795475, -627.0345787, -660.2736824, -693.5127862,
0, 79.23769764, 110.3924444, 115.8915498, 107.650705, 91.52668183, 70.12714918, 44.47229214, 14.89705073, -18.51280491, -55.74215043, -96.18895607, -137.9441797, -179.6994034, -221.454627, -263.2098507, -304.9650743, -346.720298, -388.4755216, -430.2307453, -471.9859689, -513.7411926, -555.4964162, -597.2516398, -639.0068635, -680.7620871, -722.5173108, -764.2725344, -806.0277581, -847.7829817, -889.5382054,
0, 76.55272251, 108.5303989, 113.9924461, 103.7535095, 83.98549977, 58.01261437, 27.50278041, -6.778073099, -44.51600125, -85.6002806, -129.9991504, -177.7058487, -228.7194663, -281.8808325, -335.536515, -389.1921975, -442.84788, -496.5035625, -550.159245, -603.8149275, -657.47061, -711.1262925, -764.781975, -818.4376575, -872.09334, -925.7490225, -979.404705, -1033.060387, -1086.71607, -1140.371752,
0, 69.94446334, 101.301605, 106.6111466, 94.44953607, 70.53861719, 38.58928221, 0.931281035, -41.02684271, -86.47034191, -134.9520849, -186.2417865, -240.229756, -296.8686319, -356.1404003, -418.0392753, -482.5637874, -549.7136718, -619.4889008, -690.6110553, -761.7674637, -832.923872, -904.0802804, -975.2366888, -1046.393097, -1117.549506, -1188.705914, -1259.862322, -1331.018731, -1402.175139, -1473.331547,
0, 54.65936521, 78.44049228, 78.77568996, 61.38551649, 30.62425289, -10.23371478, -58.75846892, -113.172779, -172.1974285, -234.9278365, -300.7368368, -369.1991015, -440.0332786, -513.0584451, -588.1619661, -665.276297, -744.3626676, -825.3999475, -908.3773152, -993.2896343, -1080.134688, -1168.911628, -1259.620178, -1352.260266, -1446.831879, -1543.335014, -1641.117125, -1739.212452, -1837.307778, -1935.403105,
0, 23.17914159, 23.35452373, 4.377454763, -30.51060712, -78.59396485, -137.608379, -205.6756324, -281.2461107, -363.0486057, -450.0466088, -541.4004172, -636.4344263, -734.6090357, -835.49664, -938.7612242, -1044.141123, -1151.434547, -1260.487511, -1371.183846, -1483.436993, -1597.183325, -1712.376764, -1828.984481, -1946.983505, -2066.358087, -2187.097668, -2309.195345, -2432.646734, -2557.449143, -2683.600977,
0, -35.72046669, -87.7127827, -154.154935, -233.4166122, -324.0416878, -424.7320542, -534.3327232, -651.8181113, -776.2794358, -906.9131474, -1043.01033, -1183.947003, -1329.175266, -1478.21522, -1630.647618, -1786.107185, -1944.276563, -2104.880831, -2267.682554, -2432.477323, -2599.089746, -2767.36985, -2937.189859, -3108.441328, -3281.03258, -3454.88644, -3629.938225, -3806.133973, -3983.428885, -4161.785962,
0, 87.15015842, 140.9281866, 171.6239926, 187.0912461, 193.0492222, 193.3950935, 190.529573, 185.7008892, 179.6105748, 173.0500434, 166.4895119, 159.9289805, 153.3684491, 146.8079176, 140.2473862, 133.6868548, 127.1263233, 120.5657919, 114.0052605, 107.444729, 100.8841976, 94.32366615, 87.76313471, 81.20260327, 74.64207184, 68.0815404, 61.52100897, 54.96047753, 48.3999461, 41.83941466,
0, 88.33192132, 140.257552, 168.2731467, 181.3411505, 185.5360391, 184.6611613, 180.8319257, 175.0182947, 167.7520811, 159.8936732, 152.0352652, 144.1768573, 136.3184493, 128.4600413, 120.6016334, 112.7432254, 104.8848175, 97.02640953, 89.16800158, 81.30959363, 73.45118567, 65.59277772, 57.73436977, 49.87596181, 42.01755386, 34.1591459, 26.30073795, 18.44233, 10.58392204, 2.72551409,
0, 90.25618806, 141.5512798, 168.2508217, 180.1691745, 183.5979259, 182.2287918, 177.9628607, 171.5949984, 163.5463856, 154.76888, 145.9913744, 137.2138688, 128.4363632, 119.6588576, 110.881352, 102.1038464, 93.32634075, 84.54883514, 75.77132954, 66.99382393, 58.21631833, 49.43881272, 40.66130711, 31.88380151, 23.1062959, 14.3287903, 5.55128469, -3.226220916, -12.00372652, -20.78123213,
0, 92.53875799, 143.7673153, 169.7866846, 181.1405397, 184.2696116, 182.7481962, 178.3005469, 171.594338, 162.9560995, 153.4376899, 143.9192802, 134.4008706, 124.882461, 115.3640513, 105.8456417, 96.32723203, 86.80882239, 77.29041274, 67.7720031, 58.25359346, 48.73518382, 39.21677417, 29.69836453, 20.17995489, 10.66154525, 1.143135605, -8.375274037, -17.89368368, -27.41209332, -36.93050296,
0, 94.96767358, 146.3922474, 172.0412763, 183.1094023, 186.1425877, 184.595943, 180.0421739, 173.050672, 163.8556496, 153.6140477, 143.3724458, 133.130844, 122.8892421, 112.6476402, 102.4060384, 92.16443651, 81.92283464, 71.68123278, 61.43963091, 51.19802905, 40.95642719, 30.71482532, 20.47322346, 10.23162159, -0.009980269, -10.25158213, -20.493184, -30.73478586, -40.97638773, -51.21798959,
0, 97.38613154, 149.070947, 174.4481946, 185.3059017, 188.2634305, 186.657101, 181.9271998, 174.5645367, 164.7068929, 153.6202786, 142.5336643, 131.44705, 120.3604357, 109.2738214, 98.18720707, 87.10059276, 76.01397845, 64.92736415, 53.84074984, 42.75413554, 31.66752123, 20.58090693, 9.49429262, -1.592321686, -12.67893599, -23.7655503, -34.8521646, -45.93877891, -57.02539322, -68.11200752,
0, 99.66398678, 151.5206864, 176.5636503, 187.1282504, 189.8828385, 188.0468525, 182.9436786, 174.9999955, 164.3085652, 152.1884214, 140.0085795, 127.8287377, 115.6488958, 103.4690539, 91.28921209, 79.10937024, 66.92952839, 54.74968653, 42.56984468, 30.39000282, 18.21016097, 6.030319115, -6.14952274, -18.32936459, -30.50920645, -42.6890483, -54.86889016, -67.04873201, -79.22857387, -91.40841572,
0, 101.6880864, 153.5038845, 178.0189293, 188.0765406, 190.375053, 188.0205224, 182.2329303, 173.3861262, 161.5570687, 148.0793997, 134.4404223, 120.801445, 107.1624676, 93.52349029, 79.88451295, 66.2455356, 52.60655826, 38.96758091, 25.32860357, 11.68962622, -1.949351123, -15.58832847, -29.22730581, -42.86628316, -56.5052605, -70.14423785, -83.78321519, -97.42219254, -111.0611699, -124.7001472,
0, 103.3599293, 154.8238063, 178.5120038, 187.7392171, 189.2199214, 185.9530722, 179.0684005, 168.8966059, 155.5025919, 140.2131173, 124.6362021, 109.0592869, 93.48237165, 77.90545644, 62.32854123, 46.75162602, 31.17471081, 15.5977956, 0.020880389, -15.55603482, -31.13295003, -46.70986524, -62.28678045, -77.86369566, -93.44061087, -109.0175261, -124.5944413, -140.1713565, -155.7482717, -171.3251869,
0, 104.5984952, 155.3378908, 177.8279772, 185.8166643, 186.0281024, 181.3665895, 172.8870982, 160.8868516, 145.4220808, 127.7711252, 109.6636369, 91.55614865, 73.44866037, 55.3411721, 37.23368382, 19.12619554, 1.018707266, -17.08878101, -35.19626929, -53.30375756, -71.41124584, -89.51873412, -107.6262224, -125.7337107, -143.8411989, -161.9486872, -180.0561755, -198.1636638, -216.2711521, -234.3786403,
0, 105.3483839, 154.9919759, 175.8931179, 182.1867604, 180.6142099, 174.0112064, 163.3806825, 148.998396, 130.9157106, 110.2722075, 88.91568094, 67.55915439, 46.20262784, 24.84610129, 3.489574746, -17.8669518, -39.22347835, -60.5800049, -81.93653144, -103.293058, -124.6495845, -146.0061111, -167.3626376, -188.7191642, -210.0756907, -231.4322173, -252.7887438, -274.1452704, -295.5017969, -316.8583235,
0, 105.5923248, 153.8788911, 172.8698476, 177.0220271, 173.1285296, 164.010593, 150.6580712, 133.3437855, 112.1198505, 87.78968002, 62.3054734, 36.82126678, 11.33706016, -14.14714646, -39.63135308, -65.1155597, -90.59976633, -116.0839729, -141.5681796, -167.0523862, -192.5365928, -218.0207994, -243.505006, -268.9892127, -294.4734193, -319.9576259, -345.4418325, -370.9260392, -396.4102458, -421.8944524,
0, 105.3585726, 152.3078004, 169.2814579, 170.9499973, 164.2378181, 152.0565917, 135.4547387, 114.7360518, 89.96439075, 61.20281779, 30.47399168, -0.254834441, -30.98366056, -61.71248668, -92.44131279, -123.1701389, -153.898965, -184.6277911, -215.3566173, -246.0854434, -276.8142695, -307.5430956, -338.2719217, -369.0007478, -399.729574, -430.4584001, -461.1872262, -491.9160523, -522.6448784, -553.3737045,
0, 104.6849783, 150.8040301, 166.074356, 165.1644064, 155.2538793, 139.5253247, 119.2180741, 94.73710451, 66.18566162, 33.58189061, -2.476940516, -39.9655106, -77.45408068, -114.9426508, -152.4312208, -189.9197909, -227.408361, -264.8969311, -302.3855012, -339.8740713, -377.3626413, -414.8512114, -452.3397815, -489.8283516, -527.3169217, -564.8054917, -602.2940618, -639.7826319, -677.271202, -714.7597721,
0, 103.4357706, 149.7874629, 164.2921743, 161.1432402, 147.8540913, 128.1169511, 103.5844363, 74.91232476, 42.32228515, 5.874317097, -34.41983395, -78.55878384, -124.8230955, -171.2904024, -217.7577092, -264.2250161, -310.692323, -357.1596299, -403.6269368, -450.0942437, -496.5615506, -543.0288575, -589.4961644, -635.9634712, -682.4307781, -728.898085, -775.3653919, -821.8326988, -868.3000057, -914.7673126,
0, 100.7982788, 148.4354897, 163.5966065, 159.0839025, 142.5116176, 118.2008734, 88.47032859, 54.48234292, 16.77257231, -24.43710172, -69.06669237, -117.0922922, -168.5084131, -223.3142052, -281.0264602, -339.9503765, -398.8742928, -457.7982091, -516.7221254, -575.6460417, -634.569958, -693.4938743, -752.4177906, -811.3417069, -870.2656232, -929.1895395, -988.1134558, -1047.037372, -1105.961288, -1164.885205,
0, 94.41197048, 142.4176529, 158.7446821, 153.6066891, 133.9332244, 104.3110358, 67.69225547, 25.91699971, -19.90943666, -69.15121764, -121.4607795, -176.6594184, -234.6618621, -295.4310308, -358.9526012, -425.2218372, -494.2374713, -565.9992528, -640.5071502, -716.98926, -794.072525, -871.15579, -948.2390549, -1025.32232, -1102.405585, -1179.48885, -1256.572115, -1333.65538, -1410.738645, -1487.82191,
0, 79.51005181, 121.4857881, 134.9040065, 126.6961689, 102.1591597, 65.29549294, 19.09287329, -34.24803844, -93.12978598, -156.4112443, -223.2915932, -293.219793, -365.8249262, -440.8634169, -518.1797267, -597.6776557, -679.2998521, -763.0135551, -848.8009695, -936.6529923, -1026.565294, -1118.535991, -1212.564355, -1308.650141, -1406.793281, -1506.993762, -1609.251581, -1713.29086, -1818.08293, -1922.874999,
0, 48.4096947, 68.57322116, 65.25819643, 42.47344565, 3.574819984, -48.64155508, -111.8573806, -184.160505, -263.9824215, -350.0439794, -441.308426, -536.9409662, -636.274098, -738.7780458, -844.0356752, -951.7213305, -1061.583089, -1173.427975, -1287.109735, -1402.51879, -1519.57406, -1638.216366, -1758.403153, -1880.104319, -2003.298949, -2127.972805, -2254.116406, -2381.723596, -2510.79049, -2641.314716,
0, -10.31149823, -40.75054653, -89.02135511, -153.073542, -231.0792115, -321.411854, -422.6269502, -533.4441649, -652.7310253, -779.487981, -912.8347512, -1051.997866, -1196.299318, -1345.146244, -1498.021554, -1654.475443, -1814.117718, -1976.610866, -2141.663812, -2309.026315, -2478.483936, -2649.853536, -2822.979266, -2997.728984, -3173.991093, -3351.67172, -3530.692243, -3710.987106, -3892.501908, -4075.191722,
0, 115.6084808, 190.6720928, 236.7722109, 262.8611585, 275.627789, 279.8536197, 278.7583238, 274.3329772, 267.6587633, 259.2074837, 249.5459154, 239.5026716, 229.4594277, 219.4161838, 209.3729399, 199.329696, 189.2864521, 179.2432083, 169.1999644, 159.1567205, 149.1134766, 139.0702327, 129.0269888, 118.983745, 108.9405011, 98.8972572, 88.85401331, 78.81076943, 68.76752555, 58.72428166,
0, 116.7302288, 189.527079, 232.2400931, 255.0632309, 265.2062467, 267.5114707, 265.0130247, 259.4347688, 251.6221629, 241.9012943, 230.7825024, 219.1832797, 207.584057, 195.9848343, 184.3856116, 172.7863889, 161.1871662, 149.5879435, 137.9887208, 126.3894981, 114.7902754, 103.1910527, 91.59183004, 79.99260734, 68.39338464, 56.79416195, 45.19493925, 33.59571655, 21.99649385, 10.39727116,
0, 118.7025432, 190.6316342, 231.5621957, 252.6669562, 261.5156438, 262.9515424, 259.8452701, 253.7256127, 245.287214, 234.7748641, 222.6598843, 209.9541269, 197.2483695, 184.5426122, 171.8368548, 159.1310974, 146.4253401, 133.7195827, 121.0138253, 108.308068, 95.60231059, 82.89655322, 70.19079585, 57.48503849, 44.77928112, 32.07352375, 19.36776639, 6.662009021, -6.043748345, -18.74950571,
0, 121.0970342, 192.815835, 232.7233229, 252.8484099, 261.0405694, 262.1144503, 258.7897866, 252.4368948, 243.6394566, 232.5882941, 219.7153526, 206.1293331, 192.5433137, 178.9572942, 165.3712748, 151.7852553, 138.1992359, 124.6132165, 111.027197, 97.44117757, 83.85515813, 70.26913869, 56.68311925, 43.0970998, 29.51108036, 15.92506092, 2.339041475, -11.24697797, -24.83299741, -38.41901685,
0, 123.6772382, 195.5045266, 234.7743427, 254.2948775, 262.1423763, 263.0839548, 259.697915, 253.2202349, 244.1480078, 232.6334098, 219.0633417, 204.6454323, 190.2275229, 175.8096136, 161.3917042, 146.9737948, 132.5558855, 118.1379761, 103.7200667, 89.30215735, 74.88424798, 60.46633861, 46.04842924, 31.63051987, 17.2126105, 2.794701132, -11.62320824, -26.04111761, -40.45902698, -54.87693635,
0, 126.2682659, 198.3007604, 237.0785474, 256.1331616, 263.7280049, 264.5694316, 261.1024204, 254.4465793, 245.0292453, 232.9737169, 218.6136721, 203.2578576, 187.9020431, 172.5462286, 157.1904141, 141.8345996, 126.4787851, 111.1229706, 95.76715615, 80.41134166, 65.05552718, 49.69971269, 34.3438982, 18.98808371, 3.632269219, -11.72354527, -27.07935976, -42.43517425, -57.79098874, -73.14680323,
0, 128.7249214, 200.8888289, 239.1388259, 257.685126, 264.9466351, 265.5594513, 261.8427659, 254.814202, 244.8438303, 232.0327959, 216.6514143, 200.1119734, 183.5725325, 167.0330916, 150.4936507, 133.9542098, 117.4147689, 100.875328, 84.33588713, 67.79644624, 51.25700534, 34.71756445, 18.17812356, 1.638682661, -14.90075823, -31.44019913, -47.97964002, -64.51908092, -81.05852181, -97.59796271,
0, 130.9207388, 203.003111, 240.5426407, 258.3895318, 265.0919703, 265.2093825, 260.9428009, 253.2201547, 242.3640857, 228.4585045, 211.6971352, 193.5983805, 175.4996259, 157.4008713, 139.3021166, 121.203362, 103.1046074, 85.00585272, 66.90709808, 48.80834345, 30.70958881, 12.61083418, -5.487920455, -23.58667509, -41.68542972, -59.78418436, -77.88293899, -95.98169363, -114.0804483, -132.1792029,
0, 132.7450214, 204.4223117, 240.9513913, 257.7851285, 263.5791575, 262.8134616, 257.5798487, 248.7281307, 236.5419134, 221.0917486, 202.4741516, 182.3159778, 162.1578041, 141.9996303, 121.8414566, 101.6832828, 81.52510905, 61.3669353, 41.20876154, 21.05058779, 0.892414034, -19.26575972, -39.42393347, -59.58210723, -79.74028098, -99.89845474, -120.0566285, -140.2148022, -160.372976, -180.5311498,
0, 134.1053426, 204.982712, 240.1219474, 255.5362476, 259.9718104, 257.8327018, 251.1143882, 240.6014059, 226.5465156, 209.0103572, 188.0020353, 165.2087365, 142.3662513, 119.5237661, 96.68128086, 73.83879563, 50.9963104, 28.15382517, 5.311339945, -17.53114528, -40.37363051, -63.21611574, -86.05860097, -108.9010862, -131.7435714, -154.5860566, -177.4285419, -200.2710271, -223.1135123, -245.9559976,
0, 134.935581, 204.6137892, 237.9655366, 251.5061955, 254.0638467, 249.9830013, 241.1850964, 228.406931, 211.8801534, 191.6592032, 167.7521833, 141.7332773, 115.4452534, 89.15722948, 62.86920556, 36.58118164, 10.29315772, -15.99486619, -42.28289011, -68.57091403, -94.85893795, -121.1469619, -147.4349858, -173.7230097, -200.0110336, -226.2990575, -252.5870815, -278.8751054, -305.1631293, -331.4511532,
0, 135.2088124, 203.4006215, 234.6530523, 245.8903455, 246.0293588, 239.3971846, 227.8833563, 212.2054125, 192.5881762, 169.0850057, 141.7041564, 111.7119102, 81.04680389, 50.38169759, 19.71659129, -10.94851502, -41.61362132, -72.27872763, -102.9438339, -133.6089402, -164.2740465, -194.9391528, -225.6042591, -256.2693655, -286.9344718, -317.5995781, -348.2646844, -378.9297907, -409.594897, -440.2600033,
0, 134.9466968, 201.6618588, 230.758269, 239.4044757, 236.6378462, 226.8563137, 211.9971666, 192.8097168, 169.5370467, 142.2406608, 110.9312894, 76.16267193, 39.94080552, 3.718939113, -32.5029273, -68.72479371, -104.9466601, -141.1685265, -177.3903929, -213.6122593, -249.8341258, -286.0559922, -322.2778586, -358.499725, -394.7215914, -430.9434578, -467.1653242, -503.3871906, -539.609057, -575.8309234,
0, 134.1903415, 199.9684131, 227.3503781, 233.4415223, 227.4484779, 213.9929383, 195.2020666, 171.9522129, 144.5554549, 113.1028832, 77.61450555, 38.09317334, -4.358717394, -47.72478592, -91.09085444, -134.456923, -177.8229915, -221.18906, -264.5551285, -307.921197, -351.2872656, -394.6533341, -438.0194026, -481.3854711, -524.7515396, -568.1176082, -611.4836767, -654.8497452, -698.2158137, -741.5818823,
0, 132.834053, 198.8599885, 225.7138282, 229.8539604, 220.6392444, 203.1061838, 179.7902319, 151.8668768, 119.8272955, 83.85083562, 43.99214774, 0.264161735, -47.3310176, -98.5008294, -151.3117683, -204.1227072, -256.9336461, -309.7445849, -362.5555238, -415.3664627, -468.1774016, -520.9883405, -573.7992794, -626.6102182, -679.4211571, -732.232096, -785.0430349, -837.8539738, -890.6649127, -943.4758515,
0, 130.1453842, 197.760392, 225.9227266, 229.4430423, 217.5250389, 195.6792555, 167.0648497, 133.4023912, 95.57307892, 53.99563988, 8.851339266, -39.79013808, -91.90586623, -147.4897424, -206.5405651, -269.0581846, -334.0959525, -399.9204745, -465.7449966, -531.5695186, -597.3940407, -663.2185627, -729.0430848, -794.8676068, -860.6921289, -926.5166509, -992.341173, -1058.165695, -1123.990217, -1189.814739,
0, 123.8786156, 192.6851961, 223.2918264, 227.6067329, 213.8771622, 187.6957694, 152.7665429, 111.4781093, 65.32488323, 15.20991309, -38.34260871, -95.04239139, -154.7373991, -217.3531756, -282.8561357, -351.2326103, -422.4777234, -496.5900141, -573.5691378, -653.4150371, -736.1277064, -820.5210731, -905.1617522, -989.8024314, -1074.443111, -1159.08379, -1243.724469, -1328.365148, -1413.005827, -1497.646506,
0, 109.3060133, 173.5485776, 203.3287542, 206.8793968, 190.5338018, 159.1144139, 116.2528809, 64.6515114, 6.295046249, -57.3794039, -125.3473679, -196.890972, -271.5166527, -348.8921642, -428.7990777, -511.097563, -595.7007668, -682.5565712, -771.6349233, -862.9192794, -956.4010186, -1052.075934, -1149.94213, -1249.998834, -1352.245768, -1456.682847, -1563.31005, -1672.127373, -1783.134817, -1895.743553,
0, 78.61097582, 122.9172216, 138.7084682, 130.8548564, 103.4356373, 59.85240422, 2.928395831, -65.00471154, -142.0312162, -226.585809, -317.3968973, -413.4378318, -513.88512, -618.0827944, -725.5121805, -835.7663835, -948.5288794, -1063.555659, -1180.660435, -1299.702463, -1420.576607, -1543.205284, -1667.532001, -1793.516216, -1921.129293, -2050.351355, -2181.168873, -2313.572831, -2447.557365, -2583.118763,
0, 20.15224739, 15.63954424, -10.6905479, -56.29809545, -118.9215905, -196.5511262, -287.4037897, -389.9011202, -502.6484883, -624.4162615, -754.1226264, -890.8179475, -1033.670548, -1181.953807, -1335.034466, -1492.362058, -1653.459369, -1817.913838, -1985.369832, -2155.521715, -2328.107643, -2502.90402, -2679.720566, -2858.395929, -3038.793793, -3220.799445, -3404.316746, -3589.265463, -3775.578942, -3963.202071,
0, 150.1528777, 252.1724651, 318.7042068, 359.6851227, 382.7333289, 393.5148238, 396.0864471, 393.2137254, 386.6620385, 377.459151, 366.1265466, 352.8759624, 338.0981091, 322.7266311, 307.3551531, 291.9836751, 276.6121971, 261.2407191, 245.8692411, 230.4977632, 215.1262852, 199.7548072, 184.3833292, 169.0118512, 153.6403732, 138.2688952, 122.8974172, 107.5259392, 92.15446126, 76.78298327,
0, 151.1771404, 250.4881567, 312.859342, 349.5722489, 368.9145951, 376.7664909, 377.1220851, 372.5467409, 364.5698663, 354.0135459, 341.2571492, 326.4381368, 309.8986111, 292.655193, 275.411775, 258.168357, 240.9249389, 223.6815209, 206.4381028, 189.1946848, 171.9512668, 154.7078487, 137.4644307, 120.2210126, 102.9775946, 85.73417657, 68.49075853, 51.24734049, 34.00392245, 16.76050442,
0, 153.1625274, 251.342227, 311.4025964, 345.7040416, 363.0271871, 369.3756914, 368.6632604, 363.2899105, 354.6129749, 343.3189056, 329.7026645, 313.8625376, 296.0962513, 277.5060965, 258.9159418, 240.325787, 221.7356323, 203.1454775, 184.5553228, 165.965168, 147.3750133, 128.7848585, 110.1947038, 91.60454903, 73.01439428, 54.42423953, 35.83408478, 17.24393003, -1.346224715, -19.93637946,
0, 155.6372988, 253.4387547, 312.0734952, 344.8604788, 360.986352, 366.532062, 365.3217518, 359.6079741, 350.6078566, 338.9055767, 324.7374019, 308.1767367, 289.4725908, 269.8144925, 250.1563942, 230.4982959, 210.8401976, 191.1820993, 171.524001, 151.8659027, 132.2078044, 112.5497061, 92.89160778, 73.23350948, 53.57541117, 33.91731287, 14.25921457, -5.398883737, -25.05698204, -44.71508035,
0, 158.3405647, 256.1410529, 313.8126134, 345.5594441, 360.9171071, 366.0128565, 364.5755027, 358.7224201, 349.5513923, 337.5650977, 322.9561608, 305.7808482, 286.2325317, 265.5895607, 244.9465897, 224.3036186, 203.6606476, 183.0176766, 162.3747056, 141.7317346, 121.0887636, 100.4457926, 79.80282158, 59.15985057, 38.51687956, 17.87390855, -2.769062456, -23.41203346, -44.05500447, -64.69797548,
0, 161.079276, 259.0102204, 315.912404, 346.8231937, 361.5841197, 366.3455394, 364.7372275, 358.7511623, 349.3812953, 337.0634395, 321.9565542, 304.1045517, 283.6369055, 261.9226048, 240.2083041, 218.4940034, 196.7797027, 175.065402, 153.3511014, 131.6368007, 109.9225, 88.20819926, 66.49389857, 44.77959787, 23.06529717, 1.350996478, -20.36330422, -42.07760491, -63.79190561, -85.50620631,
0, 163.6930601, 261.6973011, 317.8219261, 347.8965242, 362.0337418, 366.3887213, 364.490082, 358.2129262, 348.459807, 335.6107448, 319.7981476, 301.056765, 279.4425578, 256.417236, 233.3919141, 210.3665923, 187.3412704, 164.3159486, 141.2906267, 118.2653049, 95.23998304, 72.21466119, 49.18933933, 26.16401748, 3.138695632, -19.88662622, -42.91194807, -65.93726992, -88.96259177, -111.9879136,
0, 166.0419536, 263.9081808, 319.0841358, 348.1561958, 361.4783673, 365.1959218, 362.7357854, 355.8642519, 345.403099, 331.6854284, 314.8224622, 294.842045, 271.7485817, 247.0644404, 222.3459932, 197.6275461, 172.9090989, 148.1906517, 123.4722046, 98.75375743, 74.03531027, 49.31686312, 24.59841596, -0.120031198, -24.83847836, -49.55692551, -74.27537267, -98.99381983, -123.712267, -148.4307141,
0, 168.0027351, 265.3960832, 319.3225512, 347.0901914, 359.2679086, 361.9800501, 358.5536385, 350.6548385, 339.034579, 323.986699, 305.6055614, 283.9138109, 258.9148766, 232.1237894, 205.194028, 178.2642665, 151.3345051, 124.4047437, 97.47498225, 70.54522082, 43.6154594, 16.68569797, -10.24406345, -37.17382488, -64.1035863, -91.03334773, -117.9631092, -144.8928706, -171.822632, -198.7523934,
0, 169.4708813, 265.9740804, 318.2629706, 344.3246898, 354.918747, 356.1427109, 351.2300781, 341.7582078, 328.4179435, 311.4713292, 290.9997533, 267.022085, 239.5410917, 210.0272453, 180.2321184, 150.4369915, 120.6418647, 90.84673781, 61.05161094, 31.25648408, 1.461357211, -28.33376965, -58.12889652, -87.92402338, -117.7191502, -147.5142771, -177.309404, -207.1045308, -236.8996577, -266.6947846,
0, 170.3680489, 265.5507963, 315.7955833, 339.7044292, 348.2050203, 347.3721305, 340.3618851, 328.6808493, 312.9739915, 293.480292, 270.2723007, 243.3665168, 212.7653394, 179.8101983, 146.3487108, 112.8872232, 79.42573556, 45.96424797, 12.50276038, -20.95872722, -54.42021481, -87.88170241, -121.34319, -154.8046776, -188.2661652, -221.7276528, -255.1891404, -288.650628, -322.1121156, -355.5736032,
0, 170.6545325, 264.1943719, 312.0880102, 333.4405787, 339.3281556, 335.8262513, 326.0497423, 311.4667178, 292.6986222, 269.9738346, 243.36155, 212.8776743, 178.5246091, 141.3261274, 103.2167361, 65.10734474, 26.99795341, -11.11143793, -49.22082926, -87.3302206, -125.4396119, -163.5490033, -201.6583946, -239.7677859, -277.8771773, -315.9865686, -354.0959599, -392.2053513, -430.3147426, -468.4241339,
0, 170.3394014, 262.2159611, 307.7436638, 326.325129, 329.1670824, 322.4041645, 309.1831042, 290.993462, 268.4723366, 241.858594, 211.2272695, 176.596759, 137.9701873, 95.65951267, 51.66262322, 7.665733765, -36.33115569, -80.32804514, -124.3249346, -168.3218241, -212.3187135, -256.315603, -300.3124924, -344.3093819, -388.3062713, -432.3031608, -476.3000502, -520.2969397, -564.2938291, -608.2907186,
0, 169.4569595, 260.2049537, 303.9220044, 319.9290119, 319.5310829, 309.0269963, 291.7264406, 269.2487884, 242.3192539, 211.2280587, 176.0754522, 136.8898011, 93.67715524, 46.4383576, -3.958263878, -55.4996618, -107.0410597, -158.5824577, -210.1238556, -261.6652535, -313.2066514, -364.7480494, -416.2894473, -467.8308452, -519.3722431, -570.9136411, -622.455039, -673.9964369, -725.5378348, -777.0792328,
0, 167.9184187, 258.7874601, 302.1077766, 316.4434017, 313.0759644, 298.589639, 276.6488661, 249.1810997, 217.133292, 180.9332166, 140.7547551, 96.65958075, 48.66593227, -3.221999841, -59.00355029, -118.600499, -180.0660652, -241.5316315, -302.9971978, -364.4627641, -425.9283303, -487.3938966, -548.8594629, -610.3250292, -671.7905954, -733.2561617, -794.721728, -856.1872942, -917.6528605, -979.1184268,
0, 165.0636461, 257.6153466, 302.7629575, 317.2367828, 311.9028905, 293.6166745, 266.562089, 233.1968474, 194.9017194, 152.4127197, 106.0982763, 56.12924548, 2.577698645, -54.52940628, -115.1834504, -179.3822049, -247.1252427, -318.4125116, -392.4655348, -467.5121981, -542.5588615, -617.6055248, -692.6521881, -767.6988514, -842.7455147, -917.792178, -992.8388414, -1067.885505, -1142.932168, -1217.978831,
0, 158.7792127, 253.0967865, 301.815348, 318.5208669, 312.8434174, 291.4897337, 259.0403261, 218.5562994, 172.0341718, 120.7408774, 65.45571589, 6.641239299, -55.43909911, -120.6423073, -188.8950783, -260.1623183, -334.4285755, -411.6877204, -491.9376248, -575.1776687, -661.4077097, -750.6277248, -842.8377117, -936.9539977, -1031.481597, -1126.009196, -1220.536795, -1315.064394, -1409.591993, -1504.119592,
0, 144.429798, 235.457075, 285.3200097, 303.5997154, 297.7334983, 273.4420672, 235.0827784, 185.9396496, 128.4596463, 64.44360481, -4.800874146, -78.33817778, -155.509546, -235.8592915, -319.0783116, -404.9614481, -493.3758052, -584.23762, -677.4957089, -773.1198841, -871.0930529, -971.4059861, -1074.053972, -1179.034762, -1286.347364, -1395.991385, -1507.966687, -1622.273229, -1738.911, -1857.88,
0, 114.1137623, 187.0528616, 225.7097374, 235.8922314, 222.4747331, 189.5311058, 140.45123, 78.04285574, 4.620314646, -77.91848776, -168.0255002, -264.4451912, -366.1653756, -472.3752255, -582.4295831, -695.8187798, -812.1432447, -931.0922633, -1052.426312, -1175.962463, -1301.562403, -1429.122677, -1558.566797, -1689.838926, -1822.898854, -1957.718059, -2094.276635, -2232.560938, -2372.561802, -2514.273203,
0, 55.96646234, 82.01599118, 81.62218798, 57.88149241, 13.54935199, -48.92660484, -127.3761841, -219.8788341, -324.7382597, -440.4592688, -565.7266865, -699.3861819, -840.4268615, -987.9654937, -1141.232235, -1299.55774, -1462.361535, -1629.14157, -1799.464816, -1972.958861, -2149.304378, -2328.228411, -2509.4984, -2692.916869, -2878.316728, -3065.557115, -3254.519736, -3445.105651, -3637.232451, -3830.831806,
0, 191.2337712, 326.4522274, 419.1266434, 480.0337803, 517.6403551, 538.4583393, 547.3709967, 547.9295615, 542.620447, 533.1028616, 520.4166924, 505.160494, 487.6393896, 467.9826449, 446.2306023, 423.1983574, 399.930119, 376.6618806, 353.3936422, 330.1254038, 306.8571653, 283.5889269, 260.3206885, 237.0524501, 213.7842117, 190.5159733, 167.2477349, 143.9794964, 120.711258, 97.44301961,
0, 192.116348, 324.1609751, 411.855642, 467.3909342, 500.0277378, 516.6341808, 522.1601758, 520.0518152, 512.6080129, 501.2819464, 486.930053, 470.0115804, 450.7420035, 429.2040481, 405.420682, 380.2071131, 354.6843272, 329.1615412, 303.6387552, 278.1159693, 252.5931833, 227.0703973, 201.5476114, 176.0248254, 150.5020394, 124.9792534, 99.45646747, 73.93368151, 48.41089554, 22.88810957,
0, 194.0726627, 324.6982729, 409.5086874, 461.8425777, 491.5678564, 505.7960286, 509.4950389, 506.00734, 497.4800813, 485.2145115, 469.9419196, 452.0337068, 431.6535084, 408.8596985, 383.6671567, 356.8860108, 329.7146125, 302.5432143, 275.371816, 248.2004178, 221.0290195, 193.8576212, 166.686223, 139.5148247, 112.3434265, 85.17202823, 58.00062997, 30.82923172, 3.657833466, -23.51356479,
0, 196.5887191, 326.64556, 409.5835553, 459.7726965, 487.5982725, 500.3581595, 503.0126942, 498.8007029, 489.7351938, 476.9921451, 461.2057445, 442.68334, 421.5532589, 397.858526, 371.6093558, 343.6043033, 315.1199424, 286.6355815, 258.1512206, 229.6668597, 201.1824988, 172.6981379, 144.213777, 115.7294161, 87.24505523, 58.76069433, 30.27633343, 1.791972526, -26.69238838, -55.17674928,
0, 199.379369, 329.3053029, 410.9110795, 459.529243, 486.0047013, 497.783442, 499.8006744, 495.1915053, 485.846513, 472.8337894, 456.7081621, 437.7273761, 415.9939811, 391.5405151, 364.3742354, 335.2753005, 305.5996888, 275.9240771, 246.2484654, 216.5728537, 186.897242, 157.2216303, 127.5460186, 97.87040688, 68.19479518, 38.51918347, 8.843571771, -20.83203993, -50.50765163, -80.18326334,
0, 202.2333734, 332.1967189, 412.7127864, 460.0294295, 485.408209, 496.4141185, 497.9440464, 493.0296263, 483.4480853, 470.1725844, 453.6913278, 434.2223958, 411.8487906, 386.5955557, 358.4679707, 328.2206888, 297.2905914, 266.360494, 235.4303967, 204.5002993, 173.5702019, 142.6401045, 111.7100071, 80.77990975, 49.84981236, 18.91971498, -12.0103824, -42.94047978, -73.87057716, -104.8006745,
0, 204.9750868, 334.9374435, 414.383571, 460.4404942, 484.7516036, 494.9772474, 495.965672, 490.64581, 480.6892781, 466.9847542, 449.9636586, 429.8117499, 406.5968626, 380.3385327, 351.0406709, 319.4245637, 287.0097709, 254.5949781, 222.1801852, 189.7653924, 157.3505996, 124.9358068, 92.52101393, 60.10622109, 27.69142826, -4.723364567, -37.1381574, -69.55295023, -101.9677431, -134.3825359,
0, 207.4508976, 337.2044928, 415.4212331, 460.0764699, 483.1653319, 492.4236134, 492.6429347, 486.651092, 476.0212721, 461.5666617, 443.6699477, 422.4902969, 398.0836249, 370.4653549, 339.6384316, 306.2805173, 271.9957906, 237.711064, 203.4263373, 169.1416106, 134.8568839, 100.5721573, 66.2874306, 32.00270393, -2.28202274, -36.56674941, -70.85147608, -105.1362028, -139.4209294, -173.7056561,
0, 209.5247914, 338.7247847, 415.4100027, 458.3730572, 479.9330056, 487.8842393, 486.9563689, 479.8791291, 468.1347914, 452.4694924, 433.2242637, 410.5364428, 384.4525856, 354.98504, 322.1360671, 286.5223832, 249.834593, 213.1468027, 176.4590125, 139.7712223, 103.0834321, 66.39564188, 29.70785166, -6.979938552, -43.66772877, -80.35551898, -117.0433092, -153.7310994, -190.4188896, -227.1066798,
0, 211.079593, 339.2863003, 414.0414115, 454.9149003, 474.52164, 480.7012292, 478.1200683, 469.4162177, 455.9904755, 438.5310627, 417.34435, 392.5502289, 364.1881026, 332.2680863, 296.791973, 258.2794147, 218.5083755, 178.7373364, 138.9662973, 99.19525815, 59.42421903, 19.6531799, -20.11785922, -59.88889835, -99.65993748, -139.4309766, -179.2020157, -218.9730549, -258.744094, -298.5151331,
0, 212.023528, 338.7726101, 411.1776234, 449.5210323, 466.6816117, 470.5364911, 465.6959319, 454.7199989, 438.94249, 419.0056599, 395.1894316, 367.6009744, 336.2747485, 301.2194255, 262.4365217, 220.267146, 176.5750173, 132.8828886, 89.19075992, 45.49863123, 1.806502545, -41.88562614, -85.57775483, -129.2698835, -172.9620122, -216.6541409, -260.3462696, -304.0383983, -347.7305269, -391.4226556,
0, 212.301537, 337.2261213, 406.9688321, 442.4045803, 456.6349268, 457.5779379, 449.8114981, 435.8464256, 416.9748574, 393.811649, 366.6219175, 335.5063145, 300.4970828, 261.6023797, 218.8236659, 172.1610932, 123.53788, 74.89595293, 26.25402589, -22.38790116, -71.0298282, -119.6717552, -168.3136823, -216.9556093, -265.5975364, -314.2394634, -362.8813905, -411.5233175, -460.1652445, -508.8071716,
0, 211.9049513, 334.9336317, 402.0221019, 434.4091862, 445.3586296, 442.8524531, 431.4907363, 413.7922421, 391.0539362, 363.8953936, 332.5861822, 297.2303917, 257.8623735, 214.4912766, 167.1188941, 115.745446, 61.61421759, 6.725735663, -48.16274627, -103.0512282, -157.9397101, -212.8281921, -267.716674, -322.6051559, -377.4936378, -432.3821198, -487.2706017, -542.1590836, -597.0475656, -651.9360475,
0, 210.8519109, 332.4744433, 397.5453872, 427.2441525, 434.8905941, 428.5772156, 413.0277403, 390.8731626, 363.4995408, 331.5897038, 295.4534692, 255.2176369, 210.9276595, 162.5972024, 110.2294892, 53.82505462, -6.61605005, -69.27523532, -132.134167, -194.9930988, -257.8520305, -320.7109622, -383.569894, -446.4288257, -509.2877574, -572.1466891, -635.0056209, -697.8645526, -760.7234843, -823.5824161,
0, 209.0562344, 330.5214767, 395.1693878, 423.384298, 428.3207387, 418.2200494, 398.0736345, 370.7887904, 337.9835257, 300.5111966, 258.7938246, 213.0243945, 163.2829786, 109.5989375, 51.98142994, -9.567249368, -75.04667902, -144.4568106, -216.7872578, -290.0726709, -363.358084, -436.6434971, -509.9289102, -583.2143233, -656.4997363, -729.7851494, -803.0705625, -876.3559756, -949.6413887, -1022.926802,
0, 205.9161144, 328.9204718, 395.7009561, 424.7148495, 428.4731246, 415.268282, 390.4604161, 357.4173363, 318.1879386, 273.9734633, 225.448648, 172.9739454, 116.7307848, 56.80416839, -6.769429435, -73.97598805, -144.8108278, -219.2726531, -297.3611868, -379.076388, -464.4182532, -551.7230904, -639.1775676, -726.6320448, -814.0865219, -901.5409991, -988.9954762, -1076.449953, -1163.904431, -1251.358908,
0, 199.4561827, 324.4925529, 395.7124642, 428.2853288, 433.2321428, 418.4446294, 389.4847201, 350.2045179, 303.2209875, 250.2743535, 192.4945177, 130.5956919, 65.01584022, -3.985600173, -76.26093731, -151.7298886, -230.3511197, -312.1046808, -396.9816677, -484.978471, -576.0937949, -670.3272417, -767.6787123, -868.1481883, -971.7356677, -1078.117976, -1185.736112, -1293.354248, -1400.972384, -1508.59052,
0, 185.1805225, 307.908833, 382.0014911, 418.3838701, 425.630471, 410.4170798, 377.8969291, 332.0116749, 275.7467575, 211.3395834, 140.4479307, 64.28503805, -16.27300878, -100.6027509, -188.2675331, -278.966844, -372.4978252, -468.7264388, -567.5662076, -668.9628114, -772.8831434, -879.3077066, -988.2254619, -1099.630438, -1213.519571, -1329.891387, -1448.745229, -1570.080829, -1693.898089, -1820.196981,
0, 155.164387, 261.5087987, 327.0750189, 358.660141, 361.9879476, 341.859745, 302.2867358, 246.605838, 177.5807024, 97.48952894, 8.201142348, -88.75934304, -192.1540617, -300.9840445, -414.447987, -531.907248, -652.8562778, -776.8977602, -903.7218287, -1033.088787, -1164.814832, -1298.760332, -1434.820268, -1572.916503, -1712.991572, -1855.003742, -1998.923116, -2144.72859, -2292.40551, -2441.943876,
0, 97.35445125, 158.8453807, 188.629111, 190.411549, 167.4914917, 122.8003119, 58.93828182, -21.79222807, -117.3564439, -225.9595665, -346.0216299, -476.1546529, -615.1419063, -761.9191293, -915.5575412, -1075.248502, -1240.289686, -1410.072643, -1584.071631, -1761.833603, -1942.969253, -2127.145028, -2314.076011, -2503.519596, -2695.269888, -2889.152741, -3085.021392, -3282.752616, -3482.243351, -3683.407752,
0, 239.1550889, 414.2236628, 539.2738098, 625.7629719, 682.898773, 717.9692946, 736.643521, 743.2427081, 740.9834696, 732.1934178, 718.5002432, 700.9951805, 680.3718737, 657.0417395, 631.2270331, 603.0329562, 572.5003317, 539.6406443, 505.2977651, 470.6343519, 435.9709387, 401.3075255, 366.6441122, 331.980699, 297.3172858, 262.6538726, 227.9904593, 193.3270461, 158.6636329, 124.0002197,
0, 239.8431081, 411.2481869, 530.4688478, 610.4125552, 661.1777761, 690.5323845, 704.3363231, 706.9115777, 701.3625649, 689.8500962, 673.8221415, 654.2046872, 631.5560653, 606.1882212, 578.2584986, 547.8356519, 514.9439585, 479.5895265, 442.6086743, 405.2323616, 367.856049, 330.4797364, 293.1034238, 255.7271112, 218.3507986, 180.974486, 143.5981733, 106.2218607, 68.84554811, 31.4692355,
0, 241.7193137, 411.3914532, 527.1217455, 603.004191, 649.8345614, 675.7319013, 686.675067, 686.9608827, 679.5904356, 666.5899704, 649.272775, 628.4483787, 604.5853026, 577.9335145, 548.6126426, 516.671887, 482.1274389, 444.9830516, 406.0621469, 366.6644646, 327.2667824, 287.8691001, 248.4714178, 209.0737355, 169.6760532, 130.278371, 90.88068868, 51.4830064, 12.08532412, -27.31235816,
0, 244.2286608, 413.1165741, 526.495053, 599.5319453, 643.6303793, 667.1972036, 676.29296, 675.1748876, 666.7440966, 652.9076812, 634.8661441, 613.3360948, 588.7177434, 561.2162424, 530.9254242, 497.8819312, 462.0971242, 423.5734627, 383.1158351, 342.0935291, 301.0712231, 260.0489171, 219.0266111, 178.0043051, 136.9819991, 95.95969309, 54.93738709, 13.91508109, -27.10722491, -68.12953091,
0, 247.0619535, 415.6661709, 527.3113374, 598.1750339, 640.2114264, 662.0733368, 669.8775878, 667.8316105, 658.7383847, 644.3958415, 625.905891, 603.9072472, 578.7452023, 550.5904419, 519.5179019, 485.5555143, 448.7114916, 408.9875078, 367.1642091, 324.6814872, 282.1987652, 239.7160433, 197.2333213, 154.7505994, 112.2678774, 69.78515546, 27.3024335, -15.18028845, -57.66301041, -100.1457324,
0, 249.9897566, 418.5177983, 528.7214869, 597.7457463, 638.0548061, 658.5158083, 665.2823509, 662.504219, 652.8877238, 638.1292348, 619.2426544, 596.7997418, 571.100347, 542.2877654, 510.4225646, 475.5263736, 437.60525, 396.6603631, 353.4422242, 309.4626562, 265.4830882, 221.5035202, 177.5239523, 133.5443843, 89.56481629, 45.5852483, 1.605680311, -42.37388768, -86.35345566, -130.3330237,
0, 252.8210761, 421.2556128, 530.0660695, 597.3334738, 635.9993659, 655.1193419, 660.8678957, 657.3310264, 647.1198993, 631.8357308, 612.4130028, 589.3651219, 562.9537109, 533.2998738, 500.4530063, 464.4300551, 425.2355576, 382.8703468, 338.0481995, 292.3544328, 246.6606661, 200.9668994, 155.2731327, 109.5793659, 63.88559922, 18.1918325, -27.50193422, -73.19570093, -118.8894677, -164.5832344,
0, 255.3885297, 423.5274734, 530.7971712, 596.188779, 633.0928825, 650.7313114, 655.2862047, 650.7751379, 639.7160638, 623.620951, 603.3523195, 579.3724026, 551.9104005, 521.0692528, 486.8895592, 449.384736, 408.5582281, 364.4106395, 317.6100247, 269.8168964, 222.0237682, 174.2306399, 126.4375117, 78.64438343, 30.85125518, -16.94187307, -64.73500132, -112.5281296, -160.3212578, -208.1143861,
0, 257.5431059, 425.0333211, 530.4584057, 593.6933297, 628.551012, 644.3996967, 647.4183178, 641.5532339, 629.232157, 611.8839521, 590.3062551, 564.9165701, 535.9166505, 503.3945776, 467.3840212, 427.8957297, 384.9323604, 338.4943607, 289.1879695, 238.7504824, 188.3129954, 137.8755083, 87.43802128, 37.00053423, -13.43695281, -63.87443986, -114.3119269, -164.7494139, -215.186901, -265.624388,
0, 259.1545727, 425.5344965, 528.7039375, 589.3861663, 621.7875988, 635.4047403, 636.4056142, 628.6657475, 614.5280471, 595.3459338, 571.8598967, 544.4492105, 513.2928989, 478.467152, 440.0002993, 397.9011191, 352.1717146, 302.8124291, 250.3327194, 196.5475116, 142.7623038, 88.97709596, 35.19188815, -18.59331966, -72.37852748, -126.1637353, -179.9489431, -233.7341509, -287.5193587, -341.3045665,
0, 260.1168552, 424.8860197, 525.3609841, 583.0517122, 612.521199, 623.3776017, 621.7759806, 611.5278655, 594.9023304, 573.1890006, 547.0814156, 516.928387, 482.8914236, 445.037891, 403.3923239, 357.9621707, 308.7491836, 255.7536449, 199.3078654, 141.3031088, 83.29835215, 25.2935955, -32.71116115, -90.71591779, -148.7206744, -206.7254311, -264.7301877, -322.7349444, -380.739701, -438.7444577,
0, 260.3576627, 423.0970874, 520.5479307, 574.8870945, 600.9788185, 608.5286941, 603.6880061, 590.224266, 570.3552291, 545.3270168, 515.802121, 482.1099984, 444.4015745, 402.7392139, 357.1454772, 307.6272012, 254.1860066, 196.8221618, 135.5356922, 72.26309672, 8.965687386, -54.33172195, -117.6291313, -180.9265406, -244.22395, -307.5213593, -370.8187686, -434.116178, -497.4135873, -560.7109966,
0, 259.8466554, 420.4152878, 514.8482226, 565.7546473, 588.2078722, 592.0003259, 583.309513, 565.9036353, 541.9936836, 512.8194307, 479.0397594, 440.9830208, 398.8004962, 352.5553978, 302.2710442, 247.9547382, 189.6083101, 127.2320931, 60.82612485, -8.354940293, -78.29622753, -148.2375148, -218.178802, -288.1200893, -358.0613765, -428.0026637, -497.943951, -567.8852382, -637.8265255, -707.7678127,
0, 258.578017, 417.383057, 509.4724827, 557.4482033, 576.4288156, 576.2819997, 563.2722061, 541.2529965, 512.5107013, 478.3461698, 439.4641109, 396.2247002, 348.7995591, 297.2637294, 241.6466756, 181.9584736, 118.2019926, 50.37786767, -21.51380372, -97.47301308, -175.6561623, -254.0294826, -332.402803, -410.7761233, -489.1494436, -567.5227639, -645.8960842, -724.2694045, -802.6427248, -881.0160451,
0, 256.4544296, 414.6785737, 506.1331803, 552.652307, 569.0964307, 565.362848, 547.8854202, 520.7339713, 486.4044922, 446.3745663, 401.4839302, 352.1876541, 298.7188893, 241.1893295, 179.6482836, 114.115421, 44.59763998, -28.90301474, -106.3860606, -187.8514151, -273.2990696, -361.6860692, -451.0212639, -540.3564587, -629.6916534, -719.0268481, -808.3620429, -897.6972376, -987.0324324, -1076.367627,
0, 252.9144315, 412.2942699, 505.9189726, 553.6979107, 569.6786578, 563.6041633, 542.1051131, 509.6004162, 468.9669315, 422.0291403, 369.910387, 313.2794406, 252.5193741, 187.8400231, 119.3504686, 47.10400263, -28.87521368, -108.5771847, -191.9982226, -279.137153, -369.9936677, -464.5677051, -562.8592566, -664.8683216, -768.8636328, -872.9864337, -977.1092345, -1081.232035, -1185.354836, -1289.477637,
0, 246.1145908, 407.4545337, 506.0443586, 558.4721545, 577.0958831, 571.0142629, 546.8414093, 509.3191076, 461.7958261, 406.5973178, 345.3098662, 278.9938822, 208.3426072, 133.7981074, 55.63451011, -25.98348617, -110.9604304, -199.2432487, -290.8038168, -385.6280503, -483.7093553, -585.0448858, -689.6335307, -797.4749077, -908.5689049, -1022.915496, -1140.514677, -1261.366447, -1385.104131, -1510.101436,
0, 231.7328645, 391.3802634, 494.2107603, 552.4372011, 575.7647028, 571.8523985, 546.7005103, 504.9730546, 450.2653211, 385.3242041, 312.2284974, 232.5353842, 147.3985556, 57.66267673, -36.06172995, -133.3394895, -233.8650878, -337.4275711, -443.8839091, -553.1390549, -665.1312391, -779.8213029, -897.1851026, -1017.208213, -1139.882316, -1265.20281, -1393.167266, -1523.77447, -1657.023856, -1792.915179,
0, 202.079507, 347.298626, 444.7380559, 502.1168513, 525.9745679, 521.8321067, 494.333376, 447.3697158, 384.1888717, 307.4901491, 219.507243, 122.080102, 16.71706546, -95.35160397, -213.1277747, -335.8061694, -462.7409295, -593.4172791, -727.4276208, -864.4514722, -1004.238712, -1146.595671, -1291.373644, -1438.45948, -1587.7679, -1739.235294, -1892.81474, -2048.472036, -2206.182575, -2365.928899,
0, 144.9260552, 248.2855147, 314.7209712, 348.3913279, 353.0164835, 331.9184309, 288.059012, 224.0745597, 142.3076432, 44.83612109, -66.50030399, -190.0758476, -324.456552, -468.3801804, -620.7379443, -780.5579252, -946.9900543, -1119.292529, -1296.819551, -1479.010267, -1665.378831, -1855.505471, -2049.02849, -2245.637108, -2445.065075, -2647.084986, -2851.503227, -3058.155494, -3266.902839, -3477.628174,
0, 294.0375407, 515.812774, 679.783319, 797.9278099, 880.0729349, 934.1916196, 966.6734995, 982.5688451, 985.8071169, 979.3913518, 965.5695995, 945.9846529, 921.8033417, 893.8266854, 862.5822344, 828.3999636, 791.4731232, 751.9055, 709.7465977, 665.0163202, 617.7208243, 568.0652237, 517.3315647, 466.5979056, 415.8642466, 365.1305875, 314.3969285, 263.6632694, 212.9296104, 162.1959514,
0, 294.4679367, 512.0600397, 669.3288936, 779.7096457, 853.9880825, 900.7181515, 926.5920249, 936.7657735, 935.1436879, 924.6242565, 907.3107744, 884.6895235, 857.7784277, 827.2490486, 793.5247409, 756.8577458, 717.3879451, 675.1859468, 630.2831073, 582.6910242, 532.4129489, 479.6075186, 425.6174618, 371.6274049, 317.637348, 263.6472911, 209.6572343, 155.6671774, 101.6771205, 47.68706366,
0, 296.2027393, 511.7156029, 664.8615121, 770.27281, 839.4975671, 881.5311023, 903.2785488, 909.954137, 905.4227014, 892.4887486, 873.1382502, 848.73814, 820.1983108, 788.1007054, 752.7998948, 714.4993197, 673.3071434, 629.2754247, 582.4260583, 532.7666488, 480.2991781, 425.1312186, 368.6666497, 312.2020808, 255.737512, 199.2729431, 142.8083743, 86.34380539, 29.87923653, -26.58533233,
0, 298.6470673, 513.1290541, 663.4170544, 765.2321281, 830.7958235, 869.4781031, 888.3521656, 892.6652451, 886.234055, 871.7731199, 851.1638123, 825.6714792, 796.1176026, 763.0134892, 726.6615338, 687.2296303, 644.8038345, 599.4238949, 551.1057648, 499.8546965, 445.6719771, 388.6092072, 330.1321205, 271.6550337, 213.177947, 154.7008603, 96.22377355, 37.74668682, -20.73039991, -79.20748664,
0, 301.4678949, 515.4843452, 663.6119516, 762.6006638, 825.2916472, 861.3865897, 878.0966856, 880.6847067, 872.9127541, 857.4074642, 835.9536313, 809.7264018, 779.4713867, 745.6412444, 708.4964874, 668.1774796, 624.7538086, 578.2564382, 528.6972827, 476.0800807, 420.4056999, 361.6742646, 301.4047222, 241.1255963, 180.8464703, 120.5673444, 60.28821841, 0.009092459, -60.27003349, -120.5491594,
0, 304.4175928, 518.2176777, 664.5270334, 761.0864442, 821.3186399, 855.2251241, 870.1321125, 871.3043529, 862.4441905, 846.0918453, 823.9415194, 797.086619, 766.2060848, 731.702567, 693.8019548, 652.6225888, 608.2213429, 560.6226608, 509.8355878, 455.8628399, 398.7050165, 338.3621989, 276.3509017, 214.2636197, 152.1763376, 90.08905551, 28.00177343, -34.08550864, -96.17279072, -158.2600728,
0, 307.2897527, 520.8797253, 665.4485367, 759.7009434, 817.6115236, 849.4557355, 862.6559613, 862.4678233, 852.5298107, 835.2970856, 812.3782509, 784.7918551, 753.1574937, 717.8345344, 679.0197548, 636.813539, 591.2627347, 542.3868289, 490.1927735, 434.6825719, 375.856646, 313.7150505, 249.7670217, 185.6703932, 121.5737648, 57.47713631, -6.619492163, -70.71612063, -134.8127491, -198.9093776,
0, 309.9031226, 523.0889855, 665.7824087, 757.6305947, 813.134638, 842.8216411, 854.1940133, 852.4890021, 841.2781463, 822.9331416, 798.9815832, 770.3731899, 737.6744259, 701.2069223, 661.142764, 617.5675431, 570.5201054, 520.0161178, 466.0609804, 408.6561904, 347.8020501, 283.4985968, 217.2405829, 150.752664, 84.26474521, 17.77682638, -48.71109245, -115.1990113, -181.6869301, -248.1748489,
0, 312.0955442, 524.5179214, 665.0305221, 754.2013044, 807.0336863, 834.2859552, 843.5264365, 839.9665587, 827.1091482, 807.244787, 781.8248671, 751.7360743, 717.4974306, 679.3977178, 637.5880876, 592.1419419, 543.0917317, 490.4501722, 434.2215084, 374.4068769, 311.0064984, 244.0203991, 174.9143872, 105.4813226, 36.04825806, -33.38480652, -102.8178711, -172.2509357, -241.6840003, -311.1170649,
0, 313.7234621, 524.9000789, 662.806992, 748.902388, 798.664587, 823.0627818, 829.7190369, 823.8140485, 808.7825715, 786.8383189, 759.3625359, 727.1853252, 690.7841446, 650.4197019, 606.2257205, 558.2657345, 506.567175, 451.1405177, 391.9891645, 329.1139995, 262.5151892, 192.1927528, 119.549702, 46.44784938, -26.65400329, -99.75585596, -172.8577086, -245.9595613, -319.061414, -392.1632666,
0, 314.665953, 524.0593095, 658.898011, 741.4744503, 787.7036313, 808.7434177, 812.2601864, 803.4036892, 785.5462065, 760.8331256, 730.5848729, 695.5836352, 656.2725094, 612.8895774, 565.5548976, 514.3244976, 459.2221168, 400.2566465, 337.430918, 270.7456536, 200.2009876, 125.7969353, 48.79323394, -28.88042365, -106.5540812, -184.2277388, -261.9013964, -339.575054, -417.2487116, -494.9223692,
0, 314.8323009, 521.9658989, 653.377263, 732.0795607, 774.3617616, 791.54309, 791.3299651, 778.8504278, 757.4300556, 729.1634728, 695.326702, 656.6677441, 613.6058469, 566.3638864, 515.0530781, 459.7248061, 400.4006483, 337.0886311, 269.7913026, 198.5093154, 123.2427931, 43.99175043, -38.31002488, -121.670273, -205.0305212, -288.3907694, -371.7510175, -455.1112657, -538.4715139, -621.831762,
0, 314.1678467, 518.8164716, 646.7793618, 721.5626945, 759.7179288, 772.6878262, 768.2240434, 751.4599923, 725.709715, 693.051993, 654.7487084, 611.5375345, 563.8312469, 511.8492592, 455.7012899, 395.4383002, 331.0819048, 262.6402817, 190.1161009, 113.5100785, 32.82236046, -51.94703311, -140.577096, -231.026991, -321.4768861, -411.9267811, -502.3766762, -592.8265712, -683.2764663, -773.7263614,
0, 312.6370031, 515.093966, 640.2731348, 711.7413233, 746.1105432, 754.8859648, 745.8822009, 724.2935412, 693.4901072, 655.5998306, 611.9255682, 563.2374556, 509.9726598, 452.3679089, 390.5444672, 324.5604655, 254.4416145, 180.1982081, 101.8338796, 19.34971739, -67.25401707, -157.9772777, -252.8200594, -350.9660242, -450.3554117, -549.7447992, -649.1341867, -748.5235742, -847.9129617, -947.3023492,
0, 310.1194256, 511.4425811, 635.585164, 705.4223926, 737.2660283, 742.5637266, 729.2101269, 702.5455017, 666.1057356, 622.1767882, 572.1975035, 517.0465605, 457.242409, 393.0791086, 324.7159853, 252.2348558, 175.6751414, 95.05443238, 10.37987214, -78.34596164, -171.1222697, -267.9488495, -368.8256622, -473.7527026, -582.6937909, -693.6238133, -804.5538356, -915.483858, -1026.41388, -1137.343903,
0, 306.0745869, 507.9467435, 634.0062612, 705.2888887, 737.1939035, 740.8475777, 724.1750893, 692.7343773, 650.3573836, 599.6369917, 542.2916966, 479.4345487, 411.7681485, 339.7233561, 263.5558732, 183.4118779, 99.37141133, 11.47615616, -80.25342602, -175.8079751, -275.1835607, -378.378698, -485.3928965, -596.22602, -710.8780386, -829.3489476, -951.6387464, -1076.926911, -1203.303995, -1329.68108,
0, 298.775036, 502.196288, 633.3518913, 710.028918, 745.8097074, 750.9680311, 733.1964669, 698.1923066, 650.1256716, 592.0102388, 525.9940561, 453.5853271, 375.825749, 293.4219611, 206.8438941, 116.3972705, 22.27617999, -75.39948546, -176.5579159, -281.1575798, -389.1754008, -500.5991363, -615.4226333, -733.6429905, -855.2589408, -980.2699801, -1108.67593, -1240.476735, -1375.672383, -1514.262871,
0, 284.6355499, 487.9646099, 626.1224277, 712.2252212, 756.8766252, 768.5994786, 754.2023601, 719.0893089, 667.520271, 602.8289865, 527.604281, 443.8400366, 353.0584894, 256.4109322, 154.7593861, 48.74233891, -61.17276843, -174.6480008, -291.4423594, -411.3865066, -534.3633682, -660.2934258, -789.1237131, -920.8197028, -1055.35942, -1192.72925, -1332.921005, -1475.92993, -1621.753365, -1770.38989,
0, 255.3570662, 446.4083701, 582.8207302, 672.9001459, 723.762483, 741.4854676, 731.2437245, 697.4284693, 643.7533336, 573.3476918, 488.8387422, 392.423498, 285.9317411, 170.880908, 48.52378579, -80.11017715, -214.179217, -352.9985965, -496.0143444, -642.7808551, -792.9418606, -946.2143444, -1102.375004, -1261.248917, -1422.700109, -1586.623738, -1752.939667, -1921.587213, -2092.520878, -2265.706913,
0, 198.2194537, 348.7889048, 456.8096375, 526.874498, 563.1128025, 569.2318028, 548.5549286, 504.0570225, 438.3967628, 353.946468, 252.8194584, 136.8951455, 7.842009188, -132.861389, -283.9072121, -444.1408421, -612.5449357, -788.2249244, -970.395848, -1158.370417, -1351.548207, -1549.405897, -1751.48846, -1957.401233, -2166.802791, -2379.398552, -2594.935052, -2813.194837, -3033.991896, -3257.167617,
0, 355.7783545, 631.0795209, 840.5712958, 996.603201, 1109.495675, 1187.803644, 1238.555741, 1267.470413, 1279.150171, 1277.255217, 1264.657665, 1243.577591, 1215.702106, 1182.28866, 1144.253757, 1102.248267, 1056.720488, 1007.968119, 956.1802792, 901.4706899, 843.9031342, 783.5102726, 720.3068861, 654.2985872, 585.4870116, 513.9071389, 440.9604656, 368.0137923, 295.067119, 222.1204457,
0, 355.8767832, 626.4359174, 828.3323835, 975.3539221, 1078.821795, 1147.949607, 1190.162981, 1211.382788, 1216.274592, 1208.466877, 1190.740561, 1165.192224, 1133.373387, 1096.408142, 1055.091282, 1009.969073, 961.4046638, 909.630064, 854.7865335, 796.9551098, 736.1789107, 672.4787353, 605.8633732, 536.3359062, 463.8971522, 388.54725, 311.7238063, 234.88239, 158.0409737, 81.19955736,
0, 357.3976519, 625.4893616, 822.6037853, 963.7133035, 1060.942351, 1124.016206, 1160.654978, 1176.917386, 1177.498669, 1165.986858, 1145.081389, 1116.777879, 1082.522689, 1043.340694, 999.9395069, 952.7931889, 902.2082846, 848.374832, 791.4047797, 731.3600506, 668.2722793, 602.1560426, 533.0171872, 460.8576436, 385.6778895, 307.4779996, 227.6933978, 147.8341493, 67.97490074, -11.8843478,
0, 359.7073868, 626.4808572, 820.2044382, 956.9308239, 1049.500734, 1108.082969, 1140.642417, 1153.342898, 1150.891346, 1136.829482, 1113.778745, 1083.643896, 1047.780362, 1007.13, 962.3296575, 913.796512, 861.7938619, 806.48069, 747.9479902, 686.244521, 621.3943278, 553.4080597, 482.2898008, 408.0408321, 330.6614524, 250.1517055, 167.9413571, 85.59558959, 3.24982205, -79.09594549,
0, 362.4493325, 628.5369222, 819.6471729, 952.8570028, 1041.672339, 1096.662967, 1126.006131, 1135.9486, 1131.196303, 1115.239986, 1090.624733, 1059.170609, 1022.151069, 980.4352377, 934.5995947, 885.0140594, 831.9069632, 775.4128853, 715.6068473, 652.5278997, 586.1946911, 516.6151888, 443.7923271, 367.7269846, 288.419356, 205.869468, 121.4980418, 36.92620616, -47.6456295, -132.2174651,
0, 365.3576631, 631.0527553, 819.9435389, 950.0967014, 1035.648443, 1087.538692, 1114.132687, 1121.743101, 1115.064869, 1097.533915, 1071.619982, 1039.062908, 1001.060774, 958.4175186, 911.6568163, 861.1082406, 806.9710071, 749.3599013, 688.3373483, 623.9349722, 556.1674283, 485.0407722, 410.5571516, 332.7171831, 251.5209967, 166.9686095, 80.46823215, -6.302035372, -93.07230289, -179.8425704,
0, 368.2104942, 633.5455833, 820.3255682, 947.5836267, 1030.059755, 1079.039467, 1103.056138, 1108.474296, 1099.969637, 1080.920099, 1053.720941, 1020.035455, 980.991698, 937.3344676, 889.5406039, 837.9047008, 782.6013155, 723.7288795, 661.3396818, 595.4595385, 526.1000789, 453.265959, 376.9587728, 297.1789601, 213.9266095, 127.2017322, 38.39591887, -50.75456745, -139.9050538, -229.0555401,
0, 370.8126192, 635.6044069, 820.152785, 944.4395821, 1023.786494, 1069.803581, 1091.175248, 1094.30586, 1083.844966, 1063.110071, 1034.42296, 999.3737513, 959.0251968, 914.0681393, 864.9375988, 811.8976017, 755.1016339, 694.6344774, 630.5401715, 562.8399331, 491.5430656, 416.6531841, 338.1714857, 256.0982888, 170.4336549, 81.17759142, -10.30147179, -102.2078201, -194.1141683, -286.0205166,
0, 372.9886264, 636.8738663, 818.884454, 939.9332004, 1015.902389, 1058.707073, 1077.167259, 1077.716737, 1064.973357, 1042.192735, 1011.62478, 974.7900188, 932.6904935, 885.9679302, 835.0200496, 780.0841757, 721.2957806, 658.7282408, 592.4188692, 522.3852304, 448.6348336, 371.1705163, 289.9931898, 205.1030883, 116.5002555, 24.18469628, -70.51225135, -165.7342243, -260.9561973, -356.1781703,
0, 374.5814817, 637.058936, 816.0925335, 933.5003769, 1005.700505, 1044.892552, 1060.017545, 1057.530003, 1042.012489, 1016.659103, 983.6509616, 944.4436425, 899.9837158, 850.8689166, 797.4642152, 739.9839616, 678.5484565, 613.2216855, 544.0355563, 471.0047756, 394.1354887, 313.4299614, 228.8889019, 140.5124872, 48.30074939, -47.74630806, -146.368007, -245.6466876, -344.9253683, -444.2040489,
0, 375.4551315, 635.9505507, 811.5173619, 924.8295054, 992.8038224, 1027.899623, 1039.164536, 1033.067765, 1014.156704, 985.5681184, 949.4208116, 907.1127783, 859.5423626, 807.2697872, 750.6326421, 689.8264658, 624.9594074, 556.0881085, 483.2403583, 406.4287479, 325.658445, 240.93132, 152.247947, 59.60846686, -36.98709527, -137.5387366, -240.9370019, -345.2038517, -449.4707015, -553.7375513,
0, 375.4998214, 633.4762113, 805.1774935, 914.0298732, 977.3843602, 1007.924394, 1014.789389, 1004.462316, 981.4629918, 948.8829083, 908.7921773, 862.5444787, 811.001701, 754.6962746, 693.9459548, 628.932989, 559.7571795, 486.470281, 409.0974413, 327.6499516, 242.1324097, 152.5464701, 58.89263969, -38.82895658, -140.6182959, -246.4753757, -355.6252637, -466.0340898, -576.4429158, -686.8517419,
0, 374.6350787, 629.7732392, 797.5376964, 901.8946368, 960.5092451, 986.2321691, 988.2786631, 973.1542351, 945.3730478, 908.0085262, 863.1095639, 812.0103461, 755.5570671, 694.270682, 628.4612337, 558.3062054, 483.9027188, 405.301185, 322.5261779, 235.5888034, 144.4936352, 49.24234989, -50.16452363, -153.7268485, -261.4445979, -373.3177683, -489.2947605, -607.297864, -725.3009675, -843.3040711,
0, 372.7917166, 625.2477525, 789.6860408, 890.2044069, 944.5607438, 965.6749063, 962.8172086, 942.5393579, 909.3955617, 866.4943247, 815.9150594, 759.0179994, 696.6709767, 629.4123396, 557.5655999, 481.3182462, 400.774496, 315.9895366, 226.9909714, 133.7916985, 36.39726828, -65.19016181, -170.9698542, -280.9415953, -395.1053357, -513.4610669, -636.0087881, -762.0838449, -889.5902421, -1017.096639,
0, 369.8170311, 620.4762437, 783.2986758, 881.796308, 933.427358, 951.0031604, 943.8059587, 918.4686378, 879.6600123, 830.6127008, 773.5247959, 709.8614194, 640.5777848, 566.2815078, 487.3485693, 404.0044805, 316.3797934, 224.5470771, 128.544816, 28.39232556, -75.90131261, -184.3321513, -296.8986324, -413.6002104, -534.4367215, -659.4081259, -788.514416, -921.7555909, -1059.131651, -1198.693239,
0, 365.2107403, 615.710183, 780.0258434, 879.956121, 932.0028873, 948.5341456, 938.7205759, 909.2838856, 865.0896191, 809.6123743, 745.2972002, 673.8372649, 596.384639, 513.7082103, 426.3102882, 334.5113363, 238.5104589, 138.4277314, 34.33316856, -73.73394387, -185.7525167, -301.7117964, -421.606622, -545.4346885, -673.1950531, -804.8873692, -940.5115257, -1080.067493, -1223.555264, -1370.974839,
0, 358.3256382, 612.3229334, 784.8939579, 894.1912307, 954.5032442, 976.9944778, 970.3204019, 941.1356006, 894.5109684, 834.2739726, 763.2841941, 683.6547532, 596.9287896, 504.2188775, 406.3161062, 303.774546, 196.9759143, 86.17847656, -28.44647272, -146.7877985, -268.7751428, -394.3651678, -523.5319911, -656.2606871, -792.542987, -932.3745167, -1075.753084, -1222.677658, -1373.14779, -1527.163301,
0, 343.9021061, 598.2543443, 779.3465006, 900.6836824, 973.4121758, 1006.688201, 1007.996094, 983.4218417, 937.8873041, 875.3499691, 798.9725626, 711.2664118, 614.2120372, 509.3600728, 397.9152645, 280.8059833, 158.741397, 32.25818517, -98.24155744, -232.4597967, -370.1781759, -511.2387061, -655.528618, -802.968598, -953.5037537, -1107.096758, -1263.722718, -1423.365381, -1586.014388, -1751.66331,
0, 314.241929, 556.6161271, 737.230448, 864.8644336, 947.1246542, 990.5842074, 1000.907746, 982.9633083, 940.9221332, 878.347555, 798.2739936, 703.2769768, 595.5350601, 476.8844408, 348.8669999, 212.7724447, 69.6751674, -79.53361352, -234.1179177, -393.4728019, -557.1034684, -724.6072976, -895.6584533, -1069.994748, -1247.406484, -1427.727014, -1610.8248, -1796.596757, -1984.962717, -2175.860837,
0, 256.9583599, 459.8838311, 614.2918419, 725.1744999, 797.0445686, 833.9762368, 839.6428793, 817.3519956, 770.077504, 700.4895618, 610.9820701, 503.6980151, 380.5527909, 243.2556376, 93.32932589, -67.8717935, -239.1452581, -419.4253738, -607.7694032, -803.344968, -1005.418574, -1213.345176, -1426.558697, -1644.563437, -1866.926296, -2093.269739, -2323.26546, -2556.628665, -2793.112943, -3032.50566,
0, 424.006587, 759.330779, 1020.70078, 1220.70167, 1370.02667, 1477.706546, 1551.318362, 1597.174726, 1620.494696, 1625.557449, 1615.839816, 1594.138746, 1562.67975, 1523.212345, 1477.093476, 1425.359895, 1368.790424, 1307.959011, 1243.279454, 1175.042641, 1103.447122, 1028.623786, 950.6553985, 869.5917057, 785.4607788, 698.2772345, 608.0479198, 514.7756111, 418.461224, 319.1049791,
0, 423.6867122, 753.6586658, 1006.513359, 1196.2364, 1334.540371, 1431.169747, 1494.174187, 1530.152461, 1544.469153, 1541.446238, 1524.531533, 1496.445943, 1459.311347, 1414.760879, 1364.033295, 1308.053003, 1247.497301, 1182.852224, 1114.458377, 1042.548013, 967.274535, 888.7355278, 806.9903436, 722.0731606, 634.0023698, 542.7870502, 448.4312052, 350.9363489, 250.3029419, 146.531085,
0, 424.9092307, 751.9717108, 999.3532109, 1182.193288, 1313.027332, 1402.162002, 1458.007736, 1487.371887, 1495.715819, 1487.379177, 1465.774352, 1433.554005, 1392.754369, 1344.916857, 1291.190385, 1232.416631, 1169.200328, 1101.966506, 1031.006479, 956.5142198, 878.6146059, 797.3849044, 712.8707099, 625.0974212, 534.0782131, 439.8193278, 342.3233934, 241.5913528, 137.6234759, 30.41981764,
0, 427.0029058, 752.4071365, 995.8325342, 1173.472315, 1298.600096, 1382.016365, 1432.439772, 1456.848332, 1460.775308, 1448.564283, 1423.587621, 1388.432271, 1345.056563, 1294.921424, 1239.099142, 1178.362599, 1113.257624, 1044.160898, 971.3256161, 894.9168855, 815.0386419, 731.7536525, 645.0979899, 555.091176, 461.7430185, 365.0580023, 265.0379413, 161.6834577, 54.99472049, -55.02823796,
0, 429.5876201, 754.0352371, 994.3632841, 1167.765729, 1288.206061, 1366.937198, 1412.952723, 1433.376403, 1433.796451, 1418.550554, 1390.967227, 1353.568664, 1308.239811, 1256.36801, 1198.957193, 1136.720219, 1070.15261, 999.5906255, 925.2562753, 847.2915806, 765.7841205, 680.7856196, 592.3250942, 500.4178372, 405.0713113, 306.2888203, 204.0716528, 98.42023303, -10.66532895, -123.1850132,
0, 432.3793365, 756.2105592, 993.8886178, 1163.576634, 1279.895946, 1354.522729, 1396.701077, 1413.681695, 1411.094631, 1393.263853, 1363.470772, 1324.173244, 1277.185981, 1223.827732, 1165.040097, 1101.482291, 1033.605754, 961.7120103, 885.9968037, 806.5831125, 723.545317, 636.9264372, 546.7500645, 453.0283288, 355.7669897, 254.9685212, 150.6338567, 42.76329164, -68.64310045, -183.5853068,
0, 435.1406306, 758.4169371, 993.5865797, 1159.761315, 1272.196843, 1342.969929, 1381.555584, 1395.316058, 1389.91239, 1369.648303, 1337.755345, 1296.627315, 1248.011219, 1193.161229, 1132.961429, 1068.022466, 998.7565848, 925.4349954, 848.2309436, 767.251416, 682.559936, 594.1925196, 502.1684958, 406.4975715, 307.1842385, 204.2303719, 97.63665681, -12.5966973, -126.4696404, -243.9821643,
0, 437.662264, 760.2137866, 992.7701101, 1155.376628, 1263.904348, 1330.811499, 1365.787214, 1376.292045, 1368.008718, 1345.215195, 1311.09094, 1267.966099, 1217.522284, 1160.952624, 1099.087859, 1032.494371, 961.5493077, 886.4971972, 807.491843, 724.6266678, 637.9561673, 547.5106557, 453.3060693, 355.3502337, 253.6466822, 148.1968502, 39.00124672, -73.93997778, -190.6267887, -311.0591806,
0, 439.7554951, 761.2177066, 990.8552545, 1149.632738, 1254.018421, 1316.834172, 1347.967871, 1354.966535, 1343.526534, 1317.89559, 1281.199613, 1235.70597, 1183.033391, 1124.317467, 1060.339535, 991.6256737, 918.5215958, 841.2483394, 759.9428845, 674.6871293, 585.5280461, 492.4912931, 395.5900976, 294.8308178, 190.2162588, 81.7475316, -30.57498077, -146.7511685, -266.7810072, -390.6644931,
0, 441.249737, 761.1046426, 987.3703028, 1141.909108, 1241.764768, 1300.103486, 1326.996567, 1330.06659, 1315.016925, 1286.062076, 1246.274347, 1197.860381, 1142.379435, 1080.913678, 1014.199337, 942.7262805, 866.8124374, 786.6584217, 702.3868355, 614.0699047, 521.748406, 425.4442417, 325.1685005, 220.9264103, 112.7202294, 0.550832377, -115.5814867, -235.6766456, -359.7346268, -487.7554274,
0, 441.9937224, 759.6316138, 982.0062496, 1131.834983, 1226.702778, 1280.094812, 1302.248628, 1300.852246, 1281.611602, 1248.707864, 1205.162612, 1153.126517, 1094.105058, 1029.13289, 958.9067569, 883.8853313, 804.3629871, 720.5233125, 632.4771337, 540.2889006, 443.9945084, 343.6129628, 239.1537419, 130.6212438, 18.01733706, -98.65726733, -219.4023343, -344.2177995, -473.103649, -606.059881,
0, 441.8584917, 756.6810833, 974.7188583, 1119.451962, 1208.943861, 1256.959111, 1273.879521, 1267.449469, 1243.377334, 1205.816613, 1157.747142, 1101.274076, 1037.85974, 968.5012014, 893.8644493, 814.384257, 730.3372716, 641.8945133, 549.1582966, 452.1875793, 351.0148953, 245.6573123, 136.1232647, 22.41663277, -95.46094803, -217.5088591, -343.7268967, -474.1150049, -608.6731716, -747.4013951,
0, 440.737189, 752.3259064, 965.8871246, 1105.471651, 1189.501152, 1231.951334, 1243.316554, 1231.390305, 1201.891555, 1158.962975, 1105.563472, 1043.77426, 975.0348071, 900.3224885, 820.2875121, 735.3527377, 645.7862945, 551.7534301, 453.3527588, 350.6410085, 243.6494611, 132.3945398, 16.88438217, -102.8772476, -226.8887481, -355.1495045, -487.6593094, -624.4181039, -765.4258745, -910.682619,
0, 438.5249388, 746.8834874, 956.4871984, 1091.58347, 1170.72637, 1207.981114, 1213.903536, 1196.33067, 1161.013708, 1112.120418, 1052.630191, 984.6415257, 909.608729, 828.5219286, 742.0421767, 650.6013759, 554.4750059, 453.8341101, 348.7817189, 239.3777981, 125.6559043, 7.63398393, -114.6788597, -241.2782879, -372.1623819, -507.3303667, -646.7819625, -790.517082, -938.5357026, -1090.83782,
0, 435.660633, 743.0350214, 952.2611518, 1086.782053, 1164.685563, 1199.807289, 1202.632735, 1181.030062, 1140.840873, 1086.352728, 1020.673842, 946.0274667, 863.9808529, 775.6213984, 681.6905611, 582.6843426, 478.9276152, 370.6282377, 257.9157692, 140.8686279, 19.53272738, -106.0660482, -235.9137278, -370.0031421, -508.3308241, -650.8952115, -797.6956581, -948.7319231, -1104.003928, -1263.511651,
0, 431.5278018, 740.7119301, 954.4336393, 1093.999085, 1176.178832, 1214.076532, 1217.850851, 1195.311926, 1152.411077, 1093.640208, 1022.355238, 941.0359965, 851.4933699, 755.0329329, 652.5829773, 544.7936451, 432.1128235, 314.843535, 193.1867496, 67.27284901, -62.81562886, -197.0278471, -335.3334469, -477.7149229, -624.1625776, -774.6712822, -929.2384663, -1087.862922, -1250.544122, -1417.281858,
0, 423.7685554, 735.3357682, 956.7614287, 1106.02379, 1197.696428, 1243.526488, 1252.926596, 1233.391462, 1190.84904, 1129.955051, 1054.338693, 966.8064884, 869.5103824, 764.0854842, 651.7621652, 533.4566266, 409.8434985, 281.413549, 148.5191411, 11.4096889, -129.7409817, -274.8137619, -423.7285416, -576.4325358, -732.8917466, -893.0848248, -1056.99874, -1224.62579, -1395.961585, -1571.003727,
0, 408.4996934, 719.5115105, 949.2060794, 1111.240436, 1217.106836, 1276.439215, 1297.281681, 1286.323033, 1249.100972, 1190.179351, 1113.301479, 1021.52226, 917.3216429, 802.7016574, 679.2690463, 548.3053381, 410.825985, 267.6300302, 119.3416035, -33.55560173, -190.6898509, -351.7779932, -516.6064029, -685.0155516, -856.8876139, -1032.136593, -1210.700525, -1392.535377, -1577.610333, -1765.904178,
0, 378.2275241, 677.04107, 906.8331094, 1076.727734, 1194.718244, 1267.791743, 1302.041765, 1302.769928, 1274.577502, 1221.447744, 1146.819798, 1053.654882, 944.4954511, 821.5179659, 686.5798572, 541.2612243, 386.9017722, 224.6334476, 55.40920052, -119.9717371, -300.8416979, -486.6452208, -676.9221879, -871.2931827, -1069.44682, -1271.128821, -1476.132628, -1684.291376, -1895.471046, -2109.564664,
0, 320.606053, 580.5877785, 785.8156496, 941.6323793, 1052.894914, 1124.01353, 1158.9882, 1161.442391, 1134.654451, 1081.586721, 1004.912515, 907.0411111, 790.1408503, 656.1604963, 506.8489428, 343.7733868, 168.3360667, -18.21033937, -214.7485638, -420.2837638, -633.9316176, -854.9074274, -1082.516156, -1316.143332, -1555.246753, -1799.348939, -2048.030265, -2300.922735, -2557.70433, -2818.093906,
0, 498.0334893, 899.2239788, 1218.238681, 1467.780628, 1658.804258, 1800.712861, 1901.538907, 1968.108268, 2006.18931, 2020.627799, 2015.46855, 1994.064689, 1959.175408, 1913.053019, 1857.520133, 1794.037707, 1723.764724, 1647.610189, 1566.278145, 1480.306344, 1390.099194, 1295.95557, 1198.092056, 1096.662121, 991.7717575, 883.4920161, 771.8688884, 656.9309332, 538.6950159, 417.1705001,
0, 497.1965451, 892.3594138, 1201.902767, 1439.878226, 1618.258921, 1747.196518, 1835.253068, 1889.60947, 1916.252237, 1920.140202, 1905.352728, 1875.22089, 1832.443073, 1779.186306, 1717.174626, 1647.765675, 1572.016676, 1490.740865, 1404.555383, 1313.921582, 1219.178639, 1120.571272, 1018.272351, 912.4010878, 803.03745, 690.2333904, 574.0214184, 454.4209901, 331.4431405, 205.0937282,
0, 498.0240096, 889.7678558, 1193.104995, 1423.197036, 1592.841883, 1712.785774, 1792.002039, 1837.939216, 1856.74105, 1853.440763, 1832.131858, 1796.117576, 1748.041025, 1689.997851, 1623.633225, 1550.224791, 1470.753124, 1385.961102, 1296.403539, 1202.488283, 1104.509903, 1002.676982, 897.1339616, 787.9783565, 675.2741135, 559.0617767, 439.3660572, 316.201327, 189.5754865, 59.49258812,
0, 499.8078766, 889.4872544, 1188.260986, 1412.304309, 1575.157806, 1688.094477, 1760.444835, 1799.883705, 1812.682212, 1803.928189, 1777.718076, 1737.323137, 1685.332663, 1623.776612, 1554.229994, 1477.901081, 1395.705416, 1308.327384, 1216.27098, 1119.90126, 1019.477809, 915.1814366, 807.1351791, 695.4205712, 580.0900274, 461.1760782, 338.6980993, 212.6670816, 83.08890425, -50.03350619,
0, 502.1446929, 890.5328068, 1185.684567, 1404.738635, 1561.932421, 1669.025737, 1735.67302, 1769.749038, 1777.632628, 1764.452727, 1734.300634, 1690.412156, 1635.323016, 1571.000603, 1498.954937, 1420.331426, 1335.987802, 1246.557366, 1152.500496, 1054.146142, 951.7248874, 845.39492, 735.262168, 621.3956374, 503.8388858, 382.6184222, 257.7497059, 129.2413079, -2.902300043, -138.6789522,
0, 504.7321798, 892.2187313, 1184.251425, 1398.902967, 1551.078088, 1652.99607, 1714.611525, 1743.979729, 1747.572341, 1730.548915, 1696.989133, 1650.090341, 1592.334527, 1525.628555, 1451.421089, 1370.799341, 1284.568438, 1193.315956, 1097.463841, 997.3097389, 893.0594801, 784.8522703, 672.7799236, 556.9012956, 437.2529037, 313.8565665, 186.724758, 55.86424691, -78.72151679, -217.0309046,
0, 507.3173026, 893.9955228, 1183.085883, 1393.576648, 1541.01893, 1638.070817, 1694.968443, 1719.931948, 1719.513625, 1698.894939, 1662.138509, 1612.400613, 1552.109256, 1483.112338, 1406.80004, 1324.205077, 1236.084109, 1142.983212, 1045.289972, 943.2744581, 837.1210297, 726.9526794, 612.849362, 494.8615489, 373.0200456, 247.3429377, 117.840371, -15.48226001, -152.6222667, -293.5784101,
0, 509.6767177, 895.3929531, 1181.454232, 1387.751449, 1530.465647, 1622.676657, 1674.886355, 1695.466353, 1691.039084, 1666.800444, 1626.79133, 1574.12478, 1511.174671, 1439.731362, 1361.129068, 1276.349212, 1186.103515, 1090.900127, 991.0956755, 886.9357329, 778.5858623, 666.1550732, 549.7132514, 429.303865, 304.9530306, 176.6758245, 44.48055293, -91.62845359, -231.6490818, -375.5803805,
0, 511.608295, 895.9994527, 1178.728906, 1380.578204, 1518.342991, 1605.509027, 1652.829436, 1668.815156, 1660.149424, 1632.036072, 1588.490428, 1532.580707, 1466.626902, 1392.36344, 1311.071103, 1223.683096, 1130.869494, 1033.10378, 930.7146633, 823.9259298, 712.8866582, 597.6937728, 478.4085874, 355.0687021, 227.6963704, 96.30423333, -39.10086371, -178.5154175, -321.9377527, -469.3671312,
0, 512.9278756, 895.4617262, 1174.393547, 1371.377686, 1503.805553, 1585.551274, 1627.604243, 1638.602934, 1625.28289, 1592.85049, 1545.292801, 1485.632725, 1416.137564, 1338.488177, 1253.915013, 1163.306507, 1067.294588, 966.3214019, 860.690757, 750.6072661, 636.2056993, 517.5726406, 394.7621786, 267.8070451, 136.7263413, 1.530756299, -137.7740133, -281.1851017, -428.7011631, -580.3216166,
0, 513.4690307, 893.5024658, 1168.087543, 1359.714727, 1486.340437, 1562.202775, 1598.509211, 1604.013211, 1585.494968, 1548.159758, 1495.965869, 1431.892672, 1358.158112, 1276.393748, 1187.784377, 1093.178372, 993.1739679, 888.1859918, 778.4968247, 664.2948028, 545.7027195, 422.7986295, 295.6307532, 164.2279323, 28.60679298, -111.2234799, -255.2580573, -403.4945442, -555.9318329, -712.5694524,
0, 513.0837412, 889.958143, 1159.698817, 1345.551744, 1465.979785, 1535.544508, 1565.645391, 1565.136062, 1540.834988, 1497.946075, 1440.402722, 1371.147265, 1292.356183, 1205.620101, 1112.086363, 1012.570878, 907.6449252, 797.7017656, 683.0071041, 563.7367982, 440.0046045, 311.8822511, 179.413685, 42.62497129, -98.46899294, -243.8600486, -393.5439429, -547.5185843, -705.7830132, -868.3368248,
0, 511.6476525, 884.8656208, 1149.56943, 1329.585712, 1443.765796, 1506.919883, 1530.596612, 1523.729936, 1493.172716, 1444.134076, 1380.535441, 1305.298287, 1220.574886, 1127.93179, 1028.494423, 923.059911, 812.1841897, 696.2484908, 575.5094397, 450.136287, 320.2381574, 185.8836561, 47.11471083, -96.04385879, -243.5778843, -395.4796067, -551.7449771, -712.3719979, -877.3597476, -1046.707834,
0, 510.3727095, 883.2296008, 1147.772495, 1327.362819, 1440.570687, 1502.058071, 1523.31854, 1513.293541, 1478.88289, 1425.365075, 1356.741069, 1276.013625, 1185.412472, 1086.574473, 980.6864869, 868.5976402, 750.9066538, 628.0290432, 500.2482142, 367.7538117, 230.6700927, 89.07658954, -56.97710341, -207.4609538, -362.3573679, -521.6564209, -685.3527521, -853.4436081, -1025.927656, -1202.804295,
0, 507.5910904, 880.3823027, 1146.072806, 1327.003736, 1441.088216, 1502.599378, 1522.834957, 1510.674993, 1473.047392, 1415.314394, 1341.591488, 1255.008917, 1157.924661, 1052.096656, 938.8209598, 819.0416894, 693.4377056, 562.4902983, 426.535484, 285.8039488, 140.4511743, -9.420152655, -163.7437219, -322.4777733, -485.5967354, -653.085438, -824.9351989, -1001.14125, -1181.701095, -1366.613497,
0, 502.6452574, 876.1283191, 1145.473801, 1331.147946, 1449.802564, 1514.912344, 1537.318608, 1525.691225, 1486.919253, 1426.439694, 1348.512775, 1256.451215, 1152.810068, 1039.54297, 918.1298947, 789.6809007, 655.01974, 514.7507106, 369.3116471, 219.0155354, 64.08286555, -95.33349075, -259.1293779, -427.2351812, -599.6053478, -776.2107636, -957.0333034, -1142.062011, -1331.290482, -1524.715112,
0, 493.8897764, 868.3609141, 1144.375624, 1339.444549, 1468.134758, 1542.512242, 1572.526116, 1566.341059, 1530.623958, 1470.790128, 1391.213981, 1295.408528, 1186.17766, 1065.744737, 935.8606428, 797.8941117, 652.9068276, 501.7154927, 344.9428118, 183.0590927, 16.41594909, -154.7266012, -330.1785272, -509.8027039, -693.5015395, -881.2066131, -1072.870735, -1268.46194, -1467.958998, -1671.348114,
0, 477.5941235, 850.1835116, 1133.582439, 1341.378496, 1485.211943, 1575.024454, 1619.280123, 1625.161332, 1598.741934, 1545.139966, 1468.651969, 1372.870782, 1260.788546, 1134.886513, 997.2130731, 849.4513477, 692.9775131, 528.9109556, 358.1572273, 181.4446884, -0.644371539, -187.6474227, -379.1995557, -575.0149496, -774.8714416, -978.5977544, -1186.062986, -1397.168021, -1611.838556, -1830.019484,
0, 446.4830074, 806.1436935, 1089.502216, 1305.891719, 1463.577231, 1569.864202, 1631.197447, 1653.25121, 1641.011024, 1598.848004, 1530.586159, 1439.563276, 1328.685912, 1200.478954, 1057.13022, 900.5305148, 732.3095336, 553.8679769, 366.4062201, 170.9498482, -31.62765187, -240.5847729, -455.2941951, -675.2270911, -899.9393177, -1129.059302, -1362.277443, -1599.336873, -1840.02542, -2084.168651,
0, 388.310678, 709.2927042, 969.1015162, 1173.370581, 1327.250543, 1435.445834, 1502.248865, 1531.571963, 1526.977149, 1491.703898, 1428.694993, 1340.620582, 1229.900538, 1098.725234, 949.0748206, 782.7370943, 601.3240551, 406.2872205, 198.9317849, -19.57030663, -248.1682994, -485.921517, -731.9891353, -985.6207812, -1246.147901, -1512.975844, -1785.576613, -2063.482232, -2346.27869, -2633.600413};
    line_index = line / 10000;

    ev = platinum_ev[propensity_twentile -1][risk_twentile -1][line_index];
  };
  if(line > 0)
    dollar_bad = get_dollar_bad(query, risk_twentile, propensity_twentile, line_index);

  return line;
};

double bt_pb(double prisk_score, double trisk_score, bool tf_a2)
{
  int prisk_twentile = 0;
  int trisk_twentile = 0;
  double pb = 0;

  if (prisk_score <= -0.002357619) prisk_twentile= 1;
  else if (prisk_score <= 0.01459757) prisk_twentile= 2;
  else if (prisk_score <= 0.025993133) prisk_twentile= 3;
  else if (prisk_score <= 0.034782458) prisk_twentile= 4;
  else if (prisk_score <= 0.0425548) prisk_twentile= 5;
  else if (prisk_score <= 0.051861903) prisk_twentile= 6;
  else if (prisk_score <= 0.060643863) prisk_twentile= 7;
  else if (prisk_score <= 0.067962661) prisk_twentile= 8;
  else if (prisk_score <= 0.077163933) prisk_twentile= 9;
  else if (prisk_score <= 0.084988284) prisk_twentile= 10;
  else if (prisk_score <= 0.093842519) prisk_twentile= 11;
  else if (prisk_score <= 0.101905955) prisk_twentile= 12;
  else if (prisk_score <= 0.11045779) prisk_twentile= 13;
  else if (prisk_score <= 0.118648427) prisk_twentile= 14;
  else if (prisk_score <= 0.128604911) prisk_twentile= 15;
  else if (prisk_score <= 0.142346132) prisk_twentile= 16;
  else if (prisk_score <= 0.162390943) prisk_twentile= 17;
  else if (prisk_score <= 0.186247548) prisk_twentile= 18;
  else if (prisk_score <= 0.23986008) prisk_twentile= 19;
  else prisk_twentile= 20;


  if(tf_a2){
  if (trisk_score <= -0.024931217) trisk_twentile = 1;
  else if (trisk_score <= -0.003154125) trisk_twentile = 2;
  else if (trisk_score <= 0.01387) trisk_twentile = 3;
  else if (trisk_score <= 0.03372) trisk_twentile = 4;
  else if (trisk_score <= 0.04762) trisk_twentile = 5;
  else if (trisk_score <= 0.060710855) trisk_twentile = 6;
  else if (trisk_score <= 0.073058004) trisk_twentile = 7;
  else if (trisk_score <= 0.08450187) trisk_twentile = 8;
  else if (trisk_score <= 0.098319911) trisk_twentile = 9;
  else if (trisk_score <= 0.111909847) trisk_twentile = 10;
  else if (trisk_score <= 0.125123332) trisk_twentile = 11;
  else if (trisk_score <= 0.13809) trisk_twentile = 12;
  else if (trisk_score <= 0.152115053) trisk_twentile = 13;
  else if (trisk_score <= 0.168621909) trisk_twentile = 14;
  else if (trisk_score <= 0.183484923) trisk_twentile = 15;
  else if (trisk_score <= 0.20256807) trisk_twentile = 16;
  else if (trisk_score <= 0.227865903) trisk_twentile = 17;
  else if (trisk_score <= 0.261612) trisk_twentile = 18;
  else if (trisk_score <= 0.324427323) trisk_twentile = 19;
  else trisk_twentile = 20;

  double ptpb_table[20][20] = {
 0.0128707189632691, 0.0135852068898074, 0.0142996948163457, 0.0150141827428867, 0.0157286706695419, 0.0164431585983897, 0.0171576465522654, 0.0178721347036902, 0.0185866240401919, 0.0193011191305628, 0.0200156378445797, 0.0207302412145378, 0.0214451156840815, 0.0221607800625237, 0.0228785681627096, 0.0236016838755820, 0.0243373827480989, 0.0251012697610633, 0.0259254143499647, 0.0268731091840793,
 0.0163056024645830, 0.0179039573937489, 0.0195023123229148, 0.0211006672520890, 0.0226990221816058, 0.0242973771177002, 0.0258957321288785, 0.0274940877327042, 0.0290924468917602, 0.0306908233124240, 0.0322892706040260, 0.0338879718634513, 0.0354874864216336, 0.0370893707065112, 0.0386976261566200, 0.0403218644447884, 0.0419838522118903, 0.0437304044003345, 0.0456577293165899, 0.0479557049684850,
 0.0196596940991381, 0.0219803322974142, 0.0243009704956903, 0.0266216086939802, 0.0289422468928411, 0.0312628851026648, 0.0335835234376281, 0.0359041627603372, 0.0382248080084301, 0.0405454820258694, 0.0428662741615390, 0.0451874895769140, 0.0475100604902175, 0.0498365809480131, 0.0521737200145274, 0.0545374971444746, 0.0569641900726443, 0.0595318237030520, 0.0624007452131462, 0.0658874179493090,
 0.0229744456453170, 0.0259386869359511, 0.0289029282265853, 0.0318671695172386, 0.0348314108086915, 0.0377956521154921, 0.0407598935974884, 0.0437241364623287, 0.0466883876227062, 0.0496526790601689, 0.0526171358631539, 0.0555821852577269, 0.0585491323494005, 0.0615216088033656, 0.0645089513095436, 0.0675335871045441, 0.0706463050170954, 0.0739563399128670, 0.0776881778403854, 0.0822848674847793,
 0.0262638120473431, 0.0298208861420293, 0.0333779602367157, 0.0369350343314266, 0.0404921084271656, 0.0440491825426373, 0.0476062568833607, 0.0511633330020263, 0.0547204197863828, 0.0582775583555639, 0.0618349095375626, 0.0653930226230427, 0.0689535756048310, 0.0725212377668122, 0.0761080134247737, 0.0797427375976296, 0.0834907102089687, 0.0874923760880897, 0.0920363601586400, 0.0976922964524446,
 0.0295347488446380, 0.0336477965339139, 0.0377608442231901, 0.0418738919124962, 0.0459869396030589, 0.0500999873177394, 0.0542130353077275, 0.0583260854707566, 0.0624391486696334, 0.0665522751610884, 0.0706656615127153, 0.0747799790799265, 0.0788972787433204, 0.0830232674067417, 0.0871726170150822, 0.0913805703773028, 0.0957269385293081, 0.1003833763014130, 0.1057026475689190, 0.1123809718608360,
 0.0327913801804608, 0.0374317905413824, 0.0420722009023042, 0.0467126112632614, 0.0513530216257037, 0.0559934320166489, 0.0606338427329584, 0.0652742560174134, 0.0699146847079066, 0.0745551881988946, 0.0791959987980724, 0.0838379099273013, 0.0884833453601132, 0.0931390496345983, 0.0978223623612927, 0.1025749342265600, 0.1074910875755740, 0.1127736876615070, 0.1188396381748460, 0.1265117458397710,
 0.0360364140674322, 0.0411809922022964, 0.0463255703371610, 0.0514701484720664, 0.0566147266086854, 0.0617593047781930, 0.0669038833231272, 0.0720484648313558, 0.0771930641160772, 0.0823377497104823, 0.0874827896662827, 0.0926290994850727, 0.0977794758736572, 0.1029417011160070, 0.1081357827738460, 0.1134097800935940, 0.1188725282687960, 0.1247581062720080, 0.1315475643570450, 0.1401903097123160,
 0.0392717536676501, 0.0449011110029504, 0.0505304683382510, 0.0561598256735979, 0.0617891830108870, 0.0674185403854533, 0.0730478981855424, 0.0786772593443390, 0.0843066406516533, 0.0899361197854065, 0.0955660005646080, 0.1011973206495820, 0.1068332499202870, 0.1124826090704200, 0.1181680753109070, 0.1239441205076610, 0.1299341018550030, 0.1364033316479030, 0.1438970468318610, 0.1534913339649940,
 0.0424988032949238, 0.0485963598847715, 0.0546939164746195, 0.0607914730645193, 0.0668890296565907, 0.0729865862903424, 0.0790841433998759, 0.0851817042648022, 0.0912792876578374, 0.0973769804305172, 0.1034751222813110, 0.1095748734098490, 0.1156797780249060, 0.1217996984280640, 0.1279599896598690, 0.1342215557626280, 0.1407223200243830, 0.1477589225027940, 0.1559409801588740, 0.1664716390583460,
 0.0457186379565076, 0.0522699638695227, 0.0588212897825383, 0.0653726156956112, 0.0719239416110882, 0.0784752675727074, 0.0850265940610312, 0.0915779247066206, 0.0981292802907578, 0.1046807569563580, 0.1112327307386440, 0.1177864859281950, 0.1243459457743350, 0.1309220272257130, 0.1375427965047430, 0.1442756695653090, 0.1512733155599110, 0.1588640873500600, 0.1677227701101830, 0.1791811192686120,
 0.0489321045674018, 0.0559244637022055, 0.0629168228370095, 0.0699091819718767, 0.0769015411093916, 0.0838939002977202, 0.0908862600660399, 0.0978786244119698, 0.1048710162167300, 0.1118635413339610, 0.1188566137639990, 0.1258516474135500, 0.1328529613503090, 0.1398725736006860, 0.1469413803316590, 0.1541335933344750, 0.1616172677644910, 0.1697538403646230, 0.1792860711849370, 0.1916798438326760,
 0.0521398859461351, 0.0595619078384053, 0.0669839297306760, 0.0744059516230166, 0.0818279735182836, 0.0892499954697026, 0.0966720180619253, 0.1040940457109140, 0.1115161036887420, 0.1189383088948980, 0.1263611184792640, 0.1337860935559410, 0.1412180024288290, 0.1486701120298290, 0.1561765265914620, 0.1638191577155740, 0.1717834846562450, 0.1804683933243200, 0.1906935545985870, 0.2040765512729490,
 0.0553425431691703, 0.0631839795075110, 0.0710254158458521, 0.0788668521842723, 0.0867082885259999, 0.0945497249311510, 0.1023911620597290, 0.1102326048946760, 0.1180740819420400, 0.1259157250166020, 0.1337580494432410, 0.1416028145262300, 0.1494553926205480, 0.1573307279959800, 0.1652672288158540, 0.1733571263760160, 0.1818092308120290, 0.1910725286189120, 0.2020694995664090, 0.2166203496997050,
 0.0585405446662893, 0.0667920839988678, 0.0750436233314469, 0.0832951626641208, 0.0915467020007563, 0.0997982414132583, 0.1080497816901620, 0.1163013287790270, 0.1245529166754720, 0.1328047024614130, 0.1410572998340090, 0.1493128026809350, 0.1575776015814000, 0.1658694643197520, 0.1742340347367520, 0.1827808727443540, 0.1917579216583950, 0.2016981146410800, 0.2136960249254810, 0.2299107179483640,
 0.0617342868368101, 0.0703874105104302, 0.0790405341840511, 0.0876936578577991, 0.0963467815368450, 0.1049999053171140, 0.1136530302484680, 0.1223061642360160, 0.1309593523973320, 0.1396128029307680, 0.1482673282813350, 0.1569256975039350, 0.1655963536226130, 0.1743027500231270, 0.1831050870089970, 0.1921477548984430, 0.2017572896812210, 0.2126350873267680, 0.2262208231664540, 0.2453526136161700,
 0.0649241090455560, 0.0739709771366680, 0.0830178452277813, 0.0920647133190930, 0.1011115814186430, 0.1101584496751430, 0.1192053197120680, 0.1282522037271700, 0.1372991712056510, 0.1463465422521530, 0.1553955641128920, 0.1644504819614910, 0.1735242234449040, 0.1826526583240320, 0.1919277596665210, 0.2015699071818660, 0.2120770264815440, 0.2245177330699800, 0.2410836690318360, 0.2660920308064720,
 0.0681103047784811, 0.0775436643354433, 0.0869770238924079, 0.0964103834497290, 0.1058437430218080, 0.1152771028743190, 0.1247104659011000, 0.1341438538023090, 0.1435773899829670, 0.1530116420810960, 0.1624488187983590, 0.1718964284149910, 0.1813773095878900, 0.1909547653565610, 0.2007909525209070, 0.2112740692237390, 0.2232804682337700, 0.2386894270102020, 0.2613523025169050, 0.2988503010619850,
 0.0712931301045915, 0.0811062403137745, 0.0909193505229621, 0.1007324607328490, 0.1105455709716370, 0.1203586817586270, 0.1301717987417640, 0.1399849642179960, 0.1497984184445600, 0.1596132654154230, 0.1694337968160150, 0.1792745894853830, 0.1891799487682730, 0.1992725899954060, 0.2098666511772090, 0.2217137019944460, 0.2365094159286060, 0.2578880582843830, 0.2932937817724020, 0.3573728720901310,
 0.0744728102049444, 0.0846593806148330, 0.0948459510247312, 0.1050325214360500, 0.1152190919059830, 0.1254056634866670, 0.1355922476113950, 0.1457789298393570, 0.1559661958488260, 0.1661562760446990, 0.1763578363415910, 0.1866002965508740, 0.1969730365744840, 0.2077235161038400, 0.2194849639138410, 0.2337718297750910, 0.2539998388941890, 0.2874874711129650, 0.3492208954316920, 0.4686769367726970};

    pb = ptpb_table[prisk_twentile - 1][trisk_twentile - 1];
  }
  else{
    if(prisk_twentile ==1) pb = 0.0103691282350033;
    else if(prisk_twentile ==2) pb = 0.0198997098956793;
    else if(prisk_twentile ==3) pb = 0.0294349762695844;
    else if(prisk_twentile ==4) pb = 0.0389867660844634;
    else if(prisk_twentile ==5) pb = 0.0485777679965445;
    else if(prisk_twentile ==6) pb = 0.0582442140163027;
    else if(prisk_twentile ==7) pb = 0.0680383012118025;
    else if(prisk_twentile ==8) pb = 0.0780304258781489;
    else if(prisk_twentile ==9) pb = 0.0883112767625602;
    else if(prisk_twentile ==10)pb = 0.0989938167140541;
    else if(prisk_twentile ==11) pb = 0.1102151728183160;
    else if(prisk_twentile ==12) pb = 0.1221384495012220;
    else if(prisk_twentile ==13) pb = 0.1349544754944860;
    else if(prisk_twentile ==14) pb = 0.1488834931193570;
    else if(prisk_twentile ==15) pb = 0.1641767966192060;
    else if(prisk_twentile ==16) pb = 0.1811183250097720;
    else if(prisk_twentile ==17) pb = 0.2000262139669880;
    else if(prisk_twentile ==18) pb = 0.2212543105424470;
    else if(prisk_twentile ==19) pb = 0.2451936539242920;
    else if(prisk_twentile ==20) pb = 0.2722739250048700;
  };
  return pb;
};

void get_product_parameters(TADOQuery *query, const AnsiString &case_sn,
                            const AnsiString &idno, int product_type,
                            AnsiString &app_date, double &regular_apr,
                            int &bt_counts, int &total_bt_amount,
                            int &bt_fee_type, double &monthly_bt_fee,
                            int &months_charging_bt_fee, int &bt_period,
                            int &gender, AnsiString &card_type,
                            double cash_card_util_cap,
                            double credit_card_util_cap,
                            int cc_balance_cap)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM App_info WHERE Case_SN = :case_sn AND IDN = :idno ";
  sql_stmt += " AND Product_Code = :product_type;";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["case_sn"] = case_sn;
  query->Parameters->ParamValues["idno"] = idno;
  query->Parameters->ParamValues["product_type"] = product_type;
  query->Open();
  if(query->RecordCount == 0)
    throw cc_error(312, case_sn, idno, "", product_type, 0, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  else if (query->RecordCount > 1)
    throw cc_error(313, case_sn, idno, "", product_type, 0, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  else{
    regular_apr = query->FieldValues["CC_Interest_Rate"];
    gender = query->FieldValues["Gender"];
    card_type = query->FieldValues["Card_Type"];
    app_date = query->FieldValues["Application_Date"];

    if(product_type == 2){
      bt_counts = query->FieldValues["No_of_BT"];
      total_bt_amount = query->FieldValues["BT_Amount"];
      bt_period = query->FieldValues["Teaser_Period"];
      bt_fee_type = query->FieldValues["BT_Fee_Code"];
      months_charging_bt_fee = query->FieldValues["BT_Fee_Month"];
      if(bt_fee_type == 1){
        if(query->FieldValues["BT_Fee"].IsNull())
          throw cc_error(307, case_sn, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
        else
          monthly_bt_fee = query->FieldValues["BT_Fee"];
      }
      else{
        if(query->FieldValues["BT_Fee"].IsNull())
          throw cc_error(307, case_sn, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
        else
          monthly_bt_fee = query->FieldValues["BT_Fee_Percentage"];
      };
    };
  };
};

void rates_to_vector(TADOQuery *query, const AnsiString &case_no,
                     const AnsiString &idno,
                     vector<double>&interest_scheme, double regular_apr)
{
  AnsiString sql_stmt;

  sql_stmt = "SELECT * FROM App_i WHERE Case_SN =:case AND IDN = :idno ORDER BY SEQ;";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["case"] = case_no;
  query->Parameters->ParamValues["idno"] = idno;
  query->Open();
  query->First();
  if(query->RecordCount!=0){
    int i = 1;
    interest_scheme.clear();
    interest_scheme.push_back(query->FieldByName("APR")->AsFloat/12.0);
    while (!query->Eof){
      int section = query->FieldByName("Periods")->AsInteger;
      while(i<=section){
        interest_scheme.push_back(query->FieldByName("APR")->AsFloat/12.0);
        ++i;
      }
      i = 1;
      query->Next();
    }
  };
};

void check_error(double regular_apr, int bt_counts, int total_bt_amount,
                 int bt_fee_type, double monthly_bt_fee,
                 int months_charging_bt_fee, int bt_period,
                 int gender, int product_type, const AnsiString &card_type,
                 vector<double> &interest_scheme,
                 AnsiString app_no, AnsiString idno, AnsiString app_date,
                 double cash_card_util_cap, double credit_card_util_cap, int cc_balance_cap)
{
  if((product_type != 1) && (product_type != 2))
    throw cc_error(301, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if((regular_apr > 0.2) || (regular_apr < 0))
    throw cc_error(310, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if((card_type != "P") && (card_type != "G") && (card_type != "C"))
    throw cc_error(314, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);


  if(product_type == 2){
    if(total_bt_amount <= 0)
      throw cc_error(302, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
    if(bt_counts <= 0)
      throw cc_error(303, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
    if(bt_period < 0)
      throw cc_error(304, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
    if((bt_fee_type != 1) && (bt_fee_type != 2))
      throw cc_error(306, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
    if((bt_fee_type == 2) && ((monthly_bt_fee > 1) || (monthly_bt_fee < 0)))
      throw cc_error(308, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
    if((bt_fee_type == 1) && (monthly_bt_fee < 0))
      throw cc_error(308, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);

    int interest_terms = static_cast<int>(interest_scheme.size());
    if(bt_period != (interest_terms-1))
      throw cc_error(305, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
    double monthly_regular_rate = regular_apr / 12.0;
    for(int i=0; i < interest_terms; ++i){
      double rate = interest_scheme[i];
      if((rate<0) || (rate > monthly_regular_rate))
        throw cc_error(309, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
    };
    if((months_charging_bt_fee <= 0) || (months_charging_bt_fee > bt_period))
      throw cc_error(311, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
    if(total_bt_amount > UPPER)
      throw cc_error(202, app_no, idno, app_date, product_type, total_bt_amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  };
};

void prescreen_out(unsigned int filter_flag, AnsiString app_no, AnsiString idno,
                   AnsiString app_date, int product_type, int amount,
                   double cash_card_util_cap, double credit_card_util_cap,
                   int cc_balance_cap)
{
  if (filter_flag & 0x20)
    throw cc_error(101, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if (filter_flag & 0x4)
    throw cc_error(102, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if (filter_flag & 0x40)
    throw cc_error(103, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if (filter_flag & 0x80)
    throw cc_error(104, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if (filter_flag & 0x10)
    throw cc_error(105, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if (filter_flag & 0x100)
    throw cc_error(105, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if (filter_flag & 0x400)
    throw cc_error(108, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if ((filter_flag & 0x1) || (filter_flag & 0x2))
    throw cc_error(201, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if (filter_flag & 0x8)
    throw cc_error(201, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if (filter_flag & 0x200)
    throw cc_error(203, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
  if (filter_flag & 0x800)
    throw cc_error(204, app_no, idno, app_date, product_type, amount, cash_card_util_cap, credit_card_util_cap, cc_balance_cap);
};

bool check_tf_a2_krm023(TADOQuery *query, const AnsiString &KRM023, const int now)
{
  bool return_value = false;
  AnsiString sql_stmt;
  sql_stmt = "SELECT MIN(Mon_Since) AS Month FROM " + KRM023 + ";";
  sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Open();
  int month = query->FieldValues["Month"];
  if(month < now)
    return_value = true;
  return return_value;
};

void store_result(TADOCommand *command,
                  const AnsiString &case_sn, const AnsiString &idno,
                  const AnsiString &app_date, int product_type, int result_code,
                  const AnsiString &result_message, int optimal_line,
                  int risk_cut, int propensity_cut,
                  double ev, double pb, double dollar_bad, bool normal)
{
  AnsiString sql_stmt;
  if(normal){
    sql_stmt = "INSERT INTO Result (CASE_SN, IDN, Application_Date, ";
    sql_stmt += "Suggested_Line, Product_Code, Result_Code, Result_Message, EV, ";
    sql_stmt += "PB, RISK_GROUP, PROPENSITY_GROUP, DOLLAR_BAD) ";
    sql_stmt += "VALUES(:case_sn, :idn, :app_date, :line, :product, :code, ";
    sql_stmt += ":message, :ev, :pb, :risk_cut, :pro_cut, :dollar_bad);";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["case_sn"] = case_sn.Trim();
    command->Parameters->ParamValues["idn"] = idno.Trim();
    command->Parameters->ParamValues["app_date"] = app_date;
    command->Parameters->ParamValues["line"] = optimal_line;
    command->Parameters->ParamValues["product"] = product_type;
    command->Parameters->ParamValues["code"] = result_code;
    command->Parameters->ParamValues["message"] = result_message;
    command->Parameters->ParamValues["ev"] = ev;
    command->Parameters->ParamValues["pb"] = pb;
    command->Parameters->ParamValues["risk_cut"] = risk_cut;
    command->Parameters->ParamValues["pro_cut"] = propensity_cut;
    command->Parameters->ParamValues["dollar_bad"] = dollar_bad;
    command->Execute();
  }
  else{
    sql_stmt = "INSERT INTO Result (CASE_SN, IDN, Application_Date, ";
    sql_stmt += "Suggested_Line, Product_Code, Result_Code, Result_Message) ";
    sql_stmt += "VALUES(:case_sn, :idn, :app_date, :line, :product, :code, ";
    sql_stmt += ":message);";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["case_sn"] = case_sn.Trim();
    command->Parameters->ParamValues["idn"] = idno.Trim();
    command->Parameters->ParamValues["app_date"] = app_date.Trim();
    command->Parameters->ParamValues["line"] = optimal_line;
    command->Parameters->ParamValues["product"] = product_type;
    command->Parameters->ParamValues["code"] = result_code;
    command->Parameters->ParamValues["message"] = result_message;
    command->Execute();
  };
};

//This function is for test only
void get_score(TADOQuery *query, double &pscore, double &tfscore, bool &tf, const AnsiString &case_sn)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT PDACO_SCORE, A2_SCORE FROM BOOC_BT_TEST_APP_INFO WHERE Account_ID = :case_sn;";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["case_sn"] = case_sn;
  query->Open();
  pscore = query->FieldValues["PDACO_SCORE"];
  if(!query->FieldValues["A2_SCORE"].IsNull()){
    tfscore = query->FieldValues["A2_SCORE"];
    tf = true;
  }
  else tf= false;
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










