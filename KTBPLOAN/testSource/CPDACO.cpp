//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include "CPDACO.h"
#include "npv.h"
/*
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
} */
//---------------------------------------------------------------------------
int TNB_Ploan_AM(char *idno, char *jcic_inquiry_date, char *app_input_time,
                 char *bank_code, char *ole_db_str, int gender, char *error)
{

  AnsiString idn = static_cast<AnsiString>(idno);
  AnsiString jcic_date = static_cast<AnsiString>(jcic_inquiry_date);
  AnsiString input_time = static_cast<AnsiString>(app_input_time);
  AnsiString error_message = "";
  AnsiString connection_string = static_cast<AnsiString>(ole_db_str);
  int status = 0;
  double pb = 0;
  AnsiString special_note = "";
  AnsiString version = "1.33";

  AnsiString bank = static_cast<AnsiString>(bank_code);
  if(bank!= "054")
    throw cc_error(1, idn, input_time);

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
  unsigned int unsecured_balance_dac = 0;
  unsigned int max_line = 0;

  try{

    int now = yrmon_to_mon(jcic_date, timelock, "20070101", no_archive, use_krm037);

    if(now > 0){
      connect(connection, connection_string);
      initialize_ado_objects(connection, query, query2, command, command_timeout);

      copy_table(command, "KRM021", KRM001, idn, jcic_date, input_time);
      copy_table(command, "KRM023", KRM023, idn, jcic_date, input_time);
      copy_table(command, "KRM037", KRM037, idn, jcic_date, input_time);
      copy_table(command, "BAM086", BAM086, idn, jcic_date, input_time);
      copy_table(command, "STM007", STM001, idn, jcic_date, input_time);
      copy_table(command, "JAS002", JAS002, idn, jcic_date, input_time);

      vam102_message(query, idn, jcic_date, input_time, special_note);


//      prepare_KRM023(command, KRM023, now);
      merge_prepare_KRM023_KRM037(command, KRM023, KRM037, now);
      prepare_KRM001(command, KRM001, now);
      prepare_BAM086(command, BAM086, now);
      prepare_STM001(command, STM001, "054");
      prepare_JAS002(command, JAS002, JAS002_T);

      get_idn_list(command, KRM001, KRM023, BAM086, STM001, JAS002_T);
      create_common_working_tables(command);
      prepare_2xx_infra(command, now);
      unsigned int data_flag = get_data_availability(query, idn);
      unsigned int filter_flag = in_pdaco(query, KRM023, KRM001, JAS002_T, BAM086, idn, data_flag, now, unsecured_balance_opt, unsecured_balance_per, max_line, unsecured_balance_dac);
      unsigned int insufficiency_flag =
                     data_insufficiency(query, data_flag, filter_flag,
                                        KRM023, KRM001, STM001,now);      
      filter_flag |= check_credit_card_block(query, idn);
      if (filter_flag != 0)
        prescreen_out(filter_flag, insufficiency_flag, idn, input_time);
      else{
        int result_code = 0;
        AnsiString result_message;

        double ms093, ms094, ms105, ms047, ms080, wi008_3m, wi008_9m;
        double wi002_12m, wi004_12m, wi006_9m, pdaco_score;
        double apr;
        unsigned int loan_amount, terms, application_fee;
        bool ms093_isnull, ms094_isnull, ms105_isnull, ms047_isnull;
        bool ms080_isnull, wi006_9m_isnull, wi008_3m_isnull, wi008_9m_isnull;
        bool wi002_12m_isnull, wi004_12m_isnull;

        get_product_parameters(query, idn, input_time,
                               apr, loan_amount, terms, application_fee);

/*        cpdaco_v1_00_preparation(query, command, KRM023, KRM001, JAS002_T,
                                 BAM086, STM001, idn, gender, now, filter_flag,
                                 data_flag, ms093, ms094, ms105, ms047,
                                 ms080, wi006_9m, wi008_3m, wi008_9m,
                                 wi002_12m, wi004_12m, pdaco_score,
                                 ms093_isnull, ms094_isnull, ms105_isnull,
                                 ms047_isnull, ms080_isnull, wi006_9m_isnull,
                                 wi008_3m_isnull, wi008_9m_isnull,
                                 wi002_12m_isnull, wi004_12m_isnull);*/
        pdaco_score = pdaco_1_00(query, command,
                                 KRM023, KRM001, JAS002_T, BAM086, STM001,
                                 idn, gender, now, filter_flag, data_flag);
        pb = get_pdaco_1_00_pb(query, idn);
//        pb = get_pb(query, idn);
        if(pb >= 0.05)
          throw cc_error(109, idn, input_time);
        double cap_amount = 0;
        double trial_amount = 0;
        if(pb <= 0.01)
          cap_amount = 1000000;
        else if (pb <= 0.013)
          cap_amount = 950000;
        else if (pb <= 0.016)
          cap_amount = 900000;
        else if (pb <= 0.019)
          cap_amount = 850000;
        else if (pb <= 0.021)
          cap_amount = 800000;
        else if (pb <= 0.024)
          cap_amount = 750000;
        else if (pb <= 0.027)
          cap_amount = 700000;
        else if (pb <= 0.03)
          cap_amount = 650000;
        else if (pb <= 0.033)
          cap_amount = 600000;
        else if (pb <= 0.036)
          cap_amount = 550000;
        else if (pb <= 0.039)
          cap_amount = 500000;
        else if (pb <= 0.041)
          cap_amount = 450000;
        else if (pb <= 0.044)
          cap_amount = 400000;
        else if (pb <= 0.047)
          cap_amount = 350000;
        else if (pb < 0.05)
          cap_amount = 300000;

        if(((loan_amount - cap_amount) > 0) && ((loan_amount - cap_amount) <= 50000))
          trial_amount = loan_amount;
        else{
//          if(cap_amount > loan_amount)
//            trial_amount = loan_amount;
//          else
            trial_amount = cap_amount;
        };
        unsigned int product_type;
        unsigned int monthly_income = get_income_product(query, idn, input_time, product_type);
        int fsc_lendable = monthly_income * 22 - unsecured_balance_opt;
        unsigned int loan_amount_dac = trial_amount;
        double portion_unsecured = 1;

        if(fsc_lendable < 0)
          fsc_lendable = 0;
        else if(product_type == 1)
          portion_unsecured = 0.2;
        else if(product_type ==2)
          portion_unsecured = 0.3;

        if(fsc_lendable > 500000)
          fsc_lendable = (fsc_lendable - 500000) + 500000 / portion_unsecured;
        else
          fsc_lendable = fsc_lendable / portion_unsecured;
        if(fsc_lendable < 0)
          fsc_lendable = 0;
        unsigned int fsc_lendable_rounding = (fsc_lendable / 10000) * 10000;  

//        if(fsc_lendable < trial_amount)
//          trial_amount = fsc_lendable;
        if(pb < 0.01){
          if(loan_amount_dac > fsc_lendable)
            loan_amount_dac = fsc_lendable_rounding;
        }
        else if((pb >= 0.01) && (pb < 0.03)){
          unsigned int app_amount_multiple = ((loan_amount * 1.5) / 10000) * 10000;
          unsigned int lower = app_amount_multiple > fsc_lendable_rounding ? fsc_lendable_rounding : app_amount_multiple;
          if(loan_amount_dac > lower)
            loan_amount_dac = lower;
        }
        else if(pb >= 0.03){
          unsigned int app_amount_multiple = loan_amount;
          unsigned int lower = app_amount_multiple > fsc_lendable_rounding ? fsc_lendable_rounding : app_amount_multiple;
          if(loan_amount_dac > lower)
            loan_amount_dac = lower;
        };


        ploan  application(loan_amount_dac, apr, terms, application_fee,
                           ms093, ms094, ms105, ms047, ms080, wi006_9m,
                           wi008_3m, wi008_9m, wi002_12m, wi004_12m,
                           pdaco_score, ms093_isnull, ms094_isnull,
                           ms105_isnull, ms047_isnull, ms080_isnull,
                           wi006_9m_isnull, wi008_3m_isnull,
                           wi008_9m_isnull, wi002_12m_isnull, wi004_12m_isnull);
//        double pb;

        double min_rate;
        double npv = application.npv(pb, min_rate);

        store_result(command, idn, input_time, 0, "", unsecured_balance_opt,
                     loan_amount_dac, fsc_lendable, unsecured_balance_dac,
                     monthly_income, npv, pb, min_rate, special_note, version, true);
      };
    }
    else{
      throw cc_error(0, idn, input_time);
    };
  }
  catch(cc_error &Err){
    status = -1;
    error_message += Err.ShowMessage();
    if(Err.error_code() > 100)
      store_result(command, Err.idn(), Err.app_date(),
                   Err.error_code(), Err.ShowMessage(), unsecured_balance_opt,
                   0, 0, unsecured_balance_dac, 0, 0, pb, 0, special_note, version,
                   false);
  }
  catch(Exception &E){
  	error_message += E.Message;
  	status = -1;
	};
  strcpy(error,error_message.c_str());

//  clean_up(command);
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
  }
  return (year - 1911) * 12 + month;
};


void copy_table(TADOCommand *command,
                const AnsiString &source_table, const AnsiString &destination_table,
                const AnsiString &idn, const AnsiString &jcic_date,
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
  sql_stmt +="AND IDN = :idn AND Input_Time = :input_time ";

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
    sql_stmt += "AND ISSUE != 'TOT' ";
    sql_stmt += "GROUP BY IDN, INQUIRY_DATE, INPUT_TIME, BILL_DATE, ISSUE, ";
    sql_stmt += "ISSUE_NAME, CARD_TYPE, PERM_LIMIT, TEMP_LIMIT, CASH_LIMIT, ";
    sql_stmt += "PAYABLE, CASH_LENT, LAST_PAYA, REVOL_BAL, PAY_STAT, PAY_CODE, ";
    sql_stmt += "REVOL_RATE, PRE_OWED, DEBT_CODE, CLOSE_CODE, CLEAR_DATE";
  }
  else if(source_table == "KRM034"){
    sql_stmt += "AND ISSUE != 'TOT' ";
    sql_stmt += "GROUP BY IDN, INQUIRY_DATE, INPUT_TIME, BILL_DATE, ISSUE, ";
    sql_stmt += "ISSUE_NAME, CARD_TYPE, PERM_LIMIT, CASH_YN, ";
    sql_stmt += "LAST_PAYA, PAY_STAT, PAY_CODE, ";
    sql_stmt += "DEBT_CODE, CLOSE_CODE, CLEAR_DATE";
  };
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["jcic_date"] = jcic_date;
  command->Parameters->ParamValues["idn"] = idn;
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
  sql_stmt += "WHEN LEN(BILL_DATE) = 7 AND CONVERT(INT, RIGHT(BILL_DATE,2)) > 15 ";
  sql_stmt += "THEN CONVERT(INT, LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 7 THEN CONVERT(INT,LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) - 1 ";
  sql_stmt += "ELSE NULL END), ISSUE, ISSUE_NAME, PERM_LIMIT, LAST_PAYA, REVOL_BAL / 1000.0, ";
  sql_stmt += "(CASE WHEN CASH_LENT > 0 THEN 'Y' ELSE 'N' END), ";
  sql_stmt += "(CASE WHEN PAY_STAT = 'X' AND PAY_CODE = 'X' THEN 'X' ";
  sql_stmt += "WHEN PAY_STAT = '1' AND PAY_CODE = 'N' THEN 'A' ";
  sql_stmt += "WHEN PAY_STAT = '1' AND PAY_CODE = '0' THEN 'B' ";
  sql_stmt += "WHEN PAY_STAT = '2' AND PAY_CODE = 'N' THEN 'C' ";
  sql_stmt += "WHEN REVOL_BAL > 0 THEN 'C' ";
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

  sql_stmt = "ALTER TABLE " + krm023 + " ";
  sql_stmt += "ADD PAYMENT_AMT FLOAT, BUCKET_DEF_1K INT, BUCKET_EF_1K INT, BUCKET_F_1K INT, MON_SINCE INT, SPREAD_PAYMENT FLOAT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + krm023 + "(IDN, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT_AMT, ";
  sql_stmt += "CASH, PAY_CODE, SPREAD_PAYMENT) SELECT  IDN, MON_SINCE, ";
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
  sql_stmt += "idn = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
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
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
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
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
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
  sql_stmt += "LEFT(LTRIM(BANK_CODE), 3) = :bank_atm_code AND ((ITEM_LIST LIKE '%K%') OR (ITEM_LIST LIKE '%k%')) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["bank_atm_code"] = bank_atm_code;
  command->Execute();
};

void prepare_JAS002(TADOCommand *command, const AnsiString &ori_table, const AnsiString &dest_table)
{
	AnsiString sql_stmt;

  sql_stmt = "UPDATE " + ori_table + " SET ";
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
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
  sql_stmt += " (IDN Char(11), TYPE CHAR(1), E_DATE Char(7), ";
  sql_stmt += "E_MON_SINCE INT);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT IDN, 'D', DELINQUENT_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_DELINQUENT = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT IDN, 'B', BAD_CHECK_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_BAD_CHECK = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT IDN, 'R', REJECT_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_REJECT = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT IDN, 'S', STOP_CARD_DATE, NULL FROM " + ori_table;
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
  	sql_stmt = "CREATE TABLE IDN_LIST_TMP (IDN CHAR(11), HIT INT);";//KRM001 = 2, KRM023 = 1, BAM086 = 4, STM001 = 8, JAS002 = 16
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

//  sql_stmt = "INSERT INTO idn_LIST_TMP SELECT DISTINCT MSN, 16 FROM " + DAM103 + " ";
  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 16 FROM " + JAS002_T + " ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*  sql_stmt = "INSERT INTO idn_LIST_TMP SELECT DISTINCT MSN, 32 FROM " + DAM203 + " ";
  command->CommandText = sql_stmt;
  command->Execute();*/

  sql_stmt = "INSERT INTO IDN_LIST SELECT IDN, SUM(HIT) FROM IDN_LIST_TMP GROUP BY IDN";
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
  sql_stmt = "DROP TABLE TMP3;";
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

  sql_stmt = "CREATE TABLE TMP3(IDN CHAR(11), MON INT, V1 FLOAT);";
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

  sql_stmt = "INSERT INTO OPEN_LINE SELECT IDN, ISSUE, MON_SINCE, Count(*), NULL FROM ";
  sql_stmt += "OPEN_CARD GROUP BY IDN, ISSUE, MON_SINCE;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 0 WHILE @i<=13 BEGIN INSERT INTO ";
  sql_stmt += "OPEN_LINE SELECT IDN, (CASE WHEN (:now3 - @i) > 1140 THEN ISSUE ";
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

unsigned int get_data_availability(TADOQuery *query, const AnsiString &idn)
{
	AnsiString sql_stmt;
	sql_stmt = "SELECT HIT FROM IDN_LIST WHERE IDN = :idn;";
	sql_stmt = sql_stmt.UpperCase();
	query->Close();
	query->SQL->Clear();
	query->SQL->Add(sql_stmt);
	query->Parameters->ParamValues["idn"] = idn;
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
    sql_stmt += "(IDN CHAR(14),  ";
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
//0x04: krm023 max bucket > 3;
//0x08: krm001 first card open less than 6 months
//0x10: bam086 pass due amount > 0;
//0x20: bam086 jas002 major derag in past 30 months;
//0x40: bam086 cash card most current month delinquent
//0x80: bam086 cash card utilization >= 100%;
//0x100: bam086 pay_code_12 max > 3;
//0x200:krm023 has data on the inquiry month only
//0x400:sum of credit card revolving balance and cash card balance > NT$500,000
unsigned int in_pdaco(TADOQuery *query,
                const AnsiString &krm023, const AnsiString &krm001,
                const AnsiString &jas002, const AnsiString &bam085,
                const AnsiString &idn,
                unsigned int flag, int now,
                unsigned int &unsecured_balance_opt, unsigned int &unsecured_balance_per,
                unsigned int &max_line, unsigned int &unsecured_balance_dac)
{
  AnsiString sql_stmt;
  int exclusion = 0;
  int count = 0;
  double credit_line = 0;
  double credit_card_rev_balance_per = 0;
  double credit_card_rev_balance_opt = 0;
  double spread_payment_balance_opt = 0;
  double spread_payment_balance_per = 0;
  double unsecured_loan_balance = 0;
  double unsecured_loan_balance_specific = 0;
  double cash_card_balance = 0;
  if ((flag & 0x1)==0)
    exclusion |= 0x1;
  else{
    sql_stmt = "SELECT MAX(Bucket_ef_1k) As ef FROM " + krm023 + " ";
    sql_stmt +="WHERE idn = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idn;
    query->Open();
    int max_bucket = query->FieldValues["ef"];
    query->Close();
    if (max_bucket > 3)
      exclusion |= 0x4;

    sql_stmt = "SELECT MIN(MON_SINCE) AS MONTH FROM " + krm023;
    sql_stmt += " WHERE IDN = :idn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idn;
    query->Open();
    int month = query->FieldValues["MONTH"];
    if(month >= now)
      exclusion |= 0x200;
    if(now - month < 6)
      exclusion |= 0x8;

    sql_stmt = "SELECT SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END) AS BAL, ";
    sql_stmt += "SUM(CASE WHEN ((PAY_CODE IN ('C', 'D', 'E', 'F')) OR (CASH = 'Y')) THEN SPREAD_PAYMENT ELSE 0 END) AS SPREAD_PAYMENT ";
    sql_stmt += "FROM " + krm023;
    sql_stmt += " WHERE :now - MON_SINCE = 1 AND IDN = :idn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idn;
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
    query->Parameters->ParamValues["idn"] = idn;
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
  }

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

    sql_stmt = "INSERT INTO IND_TMP SELECT idn, MIN(Start_Mon_Since) FROM ";
    sql_stmt += krm001 + " WHERE IDN = :idn GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idn;
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
    }
  };
	if(flag & 0x4){
/*----FS044----*/
		sql_stmt ="SELECT IDN, SUM(CASE WHEN PASS_DUE_AMT > 0 THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt +="FROM " + bam085 + " ";
		sql_stmt +="WHERE IDN = :idn GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idn"] = idn;
  	query->Open();
  	count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 0)
      exclusion |= 0x10;
    sql_stmt = "SELECT IDN, SUM(CASE WHEN ACCOUNT_CODE = 'Y' AND ";
    sql_stmt+="                           ISNULL(LEFT(PAY_CODE_12,1), '0') NOT IN ('0', 'X') ";
    sql_stmt+="                      THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt+="FROM " + bam085 + " ";
    sql_stmt+="WHERE IDN = :idn GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idn"] = idn;
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
    sql_stmt+="WHERE IDN = :idn GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idn"] = idn;
  	query->Open();
  	count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 0)
      exclusion |= 0x80;

    sql_stmt = "SELECT IDN, MAX(Bucket) as cnt FROM y_BAM086_tmp ";
    sql_stmt += "WHERE IDN = :idn GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idn"] = idn;
  	query->Open();
    if(query->FieldValues["CNT"].IsNull())
      count = 0;
    else
  	  count = query->FieldValues["CNT"];
  	query->Close();
    if(count > 3)
      exclusion |= 0x100;

    sql_stmt = "SELECT SUM(ISNULL(LOAN_AMT, 0) + ISNULL(PASS_DUE_AMT, 0)) ";
    sql_stmt += "AS BAL FROM " + bam085 + " ";
    sql_stmt += "WHERE IDN = :idn AND ACCOUNT_CODE = 'Y';";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idn"] = idn;
  	query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      cash_card_balance = query->FieldValues["BAL"];
      cash_card_balance *= 1000;
    };

    sql_stmt = "SELECT SUM(ISNULL(LOAN_AMT, 0) + ISNULL(PASS_DUE_AMT, 0)) AS BAL, ";
    sql_stmt += "SUM(CASE WHEN LEFT(BANK_CODE, 3) IN ('812', '816', '810', '822', '809', '814') THEN ISNULL(LOAN_AMT, 0) + ISNULL(PASS_DUE_AMT, 0) ELSE 0 END) AS BAL_SPECIFIC ";
    sql_stmt += "FROM " + bam085 + " ";
    sql_stmt += "WHERE IDN = :idn AND ACCOUNT_CODE NOT IN ('Y', 'K') AND ";
    sql_stmt += "((ACCOUNT_CODE2 = 'N') OR (ACCOUNT_CODE2 IS NULL))";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
  	query->Parameters->ParamValues["idn"] = idn;
  	query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      unsecured_loan_balance = query->FieldValues["BAL"];
      unsecured_loan_balance *= 1000;
      unsecured_loan_balance_specific = query->FieldValues["BAL_SPECIFIC"];
      unsecured_loan_balance_specific *= 1000;
    };
  };
  unsecured_balance_dac = credit_card_rev_balance_opt + cash_card_balance + unsecured_loan_balance_specific + spread_payment_balance_opt;
  if(unsecured_balance_dac >= 500000)
    exclusion |= 0x400;

  if(flag & 0x10){
		sql_stmt ="SELECT IDN, SUM(CASE WHEN :now - E_MON_SINCE <= 36 THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt +="FROM " + jas002 + " ";
		sql_stmt +="WHERE IDN = :idn GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
	  query->Parameters->ParamValues["now"] = now;
	  query->Parameters->ParamValues["idn"] = idn;
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
  unsecured_balance_opt = credit_card_rev_balance_opt + cash_card_balance + unsecured_loan_balance + spread_payment_balance_opt;
  unsecured_balance_per = credit_card_rev_balance_per + cash_card_balance + unsecured_loan_balance + spread_payment_balance_per;
  return exclusion;
};

double pdaco_1_00(TADOQuery *query, TADOCommand *command,
                  const AnsiString &krm023, const AnsiString &krm001,
                  const AnsiString &jas002, const AnsiString &bam085,
                  const AnsiString &stm001, const AnsiString &idn,
                  int gender, int now, unsigned int exclusion, unsigned int data_flag)
{
  AnsiString sql_stmt;
  bool with_gender = true;
	double risk_score  = 1000;

	try{
	  sql_stmt = "CREATE TABLE PDACO_V1_00_CAL (IDN CHAR(11), ";
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
		sql_stmt += "EXCLUSION int, HIT int, SCORE decimal(13,12), CUT INT, PB FLOAT);";
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

  sql_stmt = "INSERT INTO PDACO_V1_00_CAL(IDN, HIT, EXCLUSION, SEX) ";
	sql_stmt += "VALUES (:idn, :hit, :exclusion, :gender);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["idn"] = idn;
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
      sql_stmt += "WHERE IDN = :idn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["idn"] = idn;
			command->Execute();
		};
		if((data_flag & 0x1) && (data_flag & 0x2)){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS212_3M_1K = 0, FS212_6M_1K = 0, ";
			sql_stmt += "FS205_3M_1K = 0 ";
      sql_stmt += "FROM " + krm001 + " AS A, " + krm023 + " As B ";
      sql_stmt += "WHERE PDACO_V1_00_CAL.IDN = :idn AND ";
      sql_stmt += "PDACO_V1_00_CAL.IDN = A.IDN AND ";
      sql_stmt += "A.IDN = B.IDN AND ";
      sql_stmt += "(CASE WHEN A.ISSUE = '021' AND CARD_BRAND = 'V' THEN 'CTV' ";
      sql_stmt += "WHEN A.ISSUE = '021' AND CARD_BRAND = 'M' THEN 'CTM' ";
      sql_stmt += "WHEN A.ISSUE = '021' AND CARD_BRAND = 'D' THEN 'CTD' ";
      sql_stmt += "WHEN A.ISSUE = 'A82' AND CARD_BRAND = 'A' THEN 'AEA' ";
      sql_stmt += "ELSE A.ISSUE END) = B.ISSUE;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["idn"] = idn;
			command->Execute();
		};
		if(data_flag & 0x4){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS051 = 0 ";
      sql_stmt += "WHERE IDN = :idn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["idn"] = idn;
			command->Execute();
		};
		if(data_flag & 0x8){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS031 = 0 ";
      sql_stmt += "WHERE idn = :idn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["idn"] = idn;
			command->Execute();
		};

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
	  sql_stmt += "(IDN CHAR(11), Mon INT, V1 FLOAT, V2 FLOAT, V3 FLOAT);";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

	  sql_stmt = "CREATE TABLE TMP1 ";
	  sql_stmt += "(IDN CHAR(11), Mon INT, V1 FLOAT);";
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
    sql_stmt +="SELECT IDN, 9, COUNT(DISTINCT ISSUE), NULL, NULL ";
		sql_stmt +=("FROM " + krm023 + " ");
    sql_stmt +="WHERE (:now - Mon_Since)<= 9 AND ";
//    sql_stmt +="(:now1 - Mon_Since) > 0 AND ";
    sql_stmt +="Pay_Code IN ('A', 'B') GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
		command->Execute();

    sql_stmt ="UPDATE PDACO_V1_00_CAL ";
    sql_stmt +="SET FS014_9M = V1 FROM TMP AS A ";
    sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=9;";
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
		sql_stmt +="SELECT IDN, 3, COUNT(DISTINCT ISSUE), NULL, NULL ";
		sql_stmt +=("FROM " + krm023 + " ");
		sql_stmt +="WHERE (:now - Mon_Since)<= 3 AND ";
//		sql_stmt +="(:now1 - Mon_Since) > 0 AND ";
		sql_stmt +="CASH = 'Y' GROUP BY IDN ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS016_3M = V1 FROM TMP AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3;";
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
		sql_stmt +="SELECT IDN, @i, COUNT(DISTINCT ISSUE) ";
		sql_stmt +=("FROM " + krm023 + " ");
		sql_stmt +="WHERE (:now - Mon_Since)<= @i ";
//		sql_stmt +="AND (:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY IDN SET @i = @i + 6 END;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS101_3M = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS101_9M = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=9;";
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
		sql_stmt +="SELECT IDN, @i, COUNT(DISTINCT ISSUE) ";
		sql_stmt +="FROM " + krm023 + " ";
		sql_stmt +="WHERE Bucket_ef_1K >=1 AND ";
		sql_stmt +="(:now - Mon_Since)<= @i ";
//		sql_stmt +="AND (:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY IDN SET @i = @i + 3 END;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS059_3M_1k = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS059_6M_1k = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=6;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS059_9M_1k = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=9;";
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
		sql_stmt +="SELECT IDN, @I, COUNT(*), NULL, NULL ";
		sql_stmt +="FROM " + krm023 + "  AS A ";
		sql_stmt +="WHERE Issue IN ";
		sql_stmt +="(SELECT Issue FROM Latest_Line ";
		sql_stmt +="WHERE  (MON_SINCE - MOB + 1) >=  (:now - 6) AND IDN = A.IDN) AND ";
		sql_stmt +="Pay_Code in ('C', 'D', 'E','F') AND ";
		sql_stmt +="Payment_Amt > 1 AND ";
		sql_stmt +="(:now1 - Mon_Since) = @I  ";
		sql_stmt +="GROUP BY IDN ";
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
		sql_stmt +="SELECT IDN, @I, AVG(V1) ";
		sql_stmt +="FROM TMP ";
		sql_stmt +="WHERE Mon <= @I ";
		sql_stmt +="GROUP BY IDN ";
		sql_stmt +="SET @I = @I + 3 ";
		sql_stmt +="END ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET  ";
		sql_stmt +="FS212_3M_1k = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND A.Mon = 3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET  ";
		sql_stmt +="FS212_6M_1k = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND A.Mon = 6;";
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

		sql_stmt = "CREATE TABLE FS_ISSUE(IDN Char(11), MON Int, ISSUE Char(3));";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

/*---Start making FS102---*/
		sql_stmt ="INSERT INTO FS_Issue ";
		sql_stmt +="SELECT IDN, Mon_Since, Issue  ";
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
		sql_stmt +="SELECT DISTINCT IDN, @i, 0, NULL, NULL ";
		sql_stmt +=("FROM " + krm023 + "  ");
		sql_stmt +="WHERE (:now - Mon_Since)<= @i ";
//		sql_stmt +="AND (:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY IDN SET @i = @i + 3 END;";
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
		sql_stmt +="SELECT IDN, @i, COUNT(DISTINCT ISSUE) ";
		sql_stmt +="FROM FS_Issue ";
		sql_stmt +="WHERE (:now - Mon)<= @i ";
//		sql_stmt +="AND (:now1 - Mon) > 0 ";
		sql_stmt +="GROUP BY IDN SET @i = @i + 3 END;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

		sql_stmt ="UPDATE TMP ";
		sql_stmt +="SET V1 = A.V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = TMP.IDN AND TMP.MON = A.MON;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS102_3M = V1 FROM TMP AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS102_6M = V1 FROM TMP AS A ";
    sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=6;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET ";
		sql_stmt +="FS102_9M = V1 ";
		sql_stmt +="FROM TMP AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=9;";
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
		  sql_stmt +="SELECT IDN, NULL, COUNT(*) ";
  		sql_stmt +="FROM " + stm001 + " ";
	  	sql_stmt +="WHERE Item_List IS NOT NULL AND  ";
		  sql_stmt +="Item_List <> '' AND  ";
  		sql_stmt +="(:now - QUERY_MON_SINCE) <= 12 ";
	  	sql_stmt +="GROUP BY IDN ";
      sql_stmt = sql_stmt.UpperCase();
		  command->CommandText = sql_stmt;
  	  command->Parameters->ParamValues["now"] = now;
 	  	command->Execute();

  		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
	  	sql_stmt +="SET FS031 = V1 ";
		  sql_stmt +="FROM TMP1 AS A ";
  		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN ";
      sql_stmt = sql_stmt.UpperCase();
	  	command->CommandText = sql_stmt;
 		  command->Execute();
    };
/*----App_Last_Month_Bucket----*/
    sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt = "INSERT INTO TMP(IDN, MON) SELECT IDN, MAX(MON_SINCE)";
    sql_stmt += "FROM " + krm023 + " ";
    sql_stmt += "GROUP BY IDN";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "INSERT INTO TMP1 SELECT A.IDN, NULL, MAX(BUCKET_EF_1K) ";
    sql_stmt += "FROM " + krm023 + " AS A INNER JOIN TMP AS B ";
    sql_stmt += "ON A.IDN = B.IDN AND A.MON_SINCE = B.MON ";
    sql_stmt += "GROUP BY A.IDN;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt = "UPDATE PDACO_V1_00_CAL SET APP_LAST_MONTH_BUCKET = A.V1 ";
		sql_stmt += "FROM TMP1 AS A WHERE PDACO_V1_00_CAL.IDN = A.IDN;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

/*----FS205_1K----*/
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt +="INSERT INTO TMP1 ";
		sql_stmt +="SELECT IDN, 3, COUNT(*) ";
		sql_stmt +="FROM " + krm023 + "  AS A ";
		sql_stmt +="WHERE Issue IN ";
		sql_stmt +="(SELECT Issue FROM Latest_Line ";
		sql_stmt +="WHERE MOB <= 12 AND IDN = A.IDN) AND ";
		sql_stmt +="Pay_Code in ('C', 'D', 'E','F') AND ";
		sql_stmt +="Payment_Amt > 1 AND ";
		sql_stmt +="(:now - Mon_Since) <= 3 ";
//		sql_stmt +="(:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET  ";
		sql_stmt +="FS205_3M_1k = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND A.Mon = 3;";
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
	  	sql_stmt +="SELECT IDN, NULL, COUNT(*) ";
		  sql_stmt +="FROM " + bam085 + " ";
  		sql_stmt +="WHERE purpose_code = '4' ";
	  	sql_stmt +="GROUP BY IDN;";
      sql_stmt = sql_stmt.UpperCase();
	    command->CommandText = sql_stmt;
  	  command->Execute();

  		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
	  	sql_stmt +="SET FS051 = V1 ";
		  sql_stmt +="FROM TMP1 AS A ";
  		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN;";
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
    sql_stmt+="SELECT IDN, 6, MIN(Payment_amt/CONVERT(decimal(13,5),Limit)) ";
    sql_stmt+=("FROM " + krm023 + " ");
    sql_stmt+="WHERE (:now - Mon_Since)<= 6 AND ";
    sql_stmt+="      (:now1 - Mon_Since) > 0 AND ";
    sql_stmt+="       Pay_Code IN ('C', 'D') AND ";
    sql_stmt+="       Limit <> 0 GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

    sql_stmt ="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET ";
    sql_stmt+="MS117_6M = V1 FROM TMP1 AS A ";
    sql_stmt+="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=6 ";
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
    sql_stmt +="SELECT IDN, Mon_Since, 1 ";
    sql_stmt +=("FROM " + krm023 + " ");
    sql_stmt +="WHERE Bucket_f_1K >=2 ";
    sql_stmt +="GROUP BY IDN, Mon_Since  ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt ="INSERT INTO TMP ";
    sql_stmt +="SELECT IDN, 3, SUM(V1), NULL, NULL ";
    sql_stmt +="FROM TMP1 ";
    sql_stmt +="WHERE (:now - Mon)<= 3 ";
//    sql_stmt +="      AND (:now1 - Mon) > 0  ";
    sql_stmt +="GROUP BY IDN ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

    sql_stmt ="UPDATE PDACO_V1_00_CAL ";
    sql_stmt +="SET ";
    sql_stmt +="FS005_3M_1k = V1 ";
    sql_stmt +="FROM TMP AS A ";
    sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3";
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
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
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
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
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
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
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
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();

    sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
    sql_stmt += "CUT = (CASE ";
    sql_stmt += "WHEN SCORE<= -0.03231 THEN 1 ";
    sql_stmt +="WHEN SCORE<= -0.02275 THEN 2 ";
    sql_stmt +="WHEN SCORE<= -0.01479 THEN 3 ";
    sql_stmt +="WHEN SCORE<= -0.00919 THEN 4 ";
    sql_stmt +="WHEN SCORE<= -0.00438 THEN 5 ";
    sql_stmt +="WHEN SCORE<= 0.00101 THEN 6 ";
    sql_stmt +="WHEN SCORE<= 0.00624 THEN 7 ";
    sql_stmt +="WHEN SCORE<= 0.01245 THEN 8 ";
    sql_stmt +="WHEN SCORE<= 0.01836 THEN 9 ";
    sql_stmt +="WHEN SCORE<= 0.02482 THEN 10 ";
    sql_stmt +="WHEN SCORE<= 0.03219 THEN 11 ";
    sql_stmt +="WHEN SCORE<= 0.03963 THEN 12 ";
    sql_stmt +="WHEN SCORE<= 0.04759 THEN 13 ";
    sql_stmt +="WHEN SCORE<= 0.05585 THEN 14 ";
    sql_stmt +="WHEN SCORE<= 0.06657 THEN 15 ";
    sql_stmt +="WHEN SCORE<= 0.07865 THEN 16 ";
    sql_stmt +="WHEN SCORE<= 0.09435 THEN 17 ";
    sql_stmt +="WHEN SCORE<= 0.11509 THEN 18 ";
    sql_stmt +="WHEN SCORE<= 0.15002 THEN 19 ";
    sql_stmt +="WHEN SCORE > 0.15002 THEN 20 ";
    sql_stmt +="ELSE 0 END) ";
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();

    sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
    sql_stmt += "PB = (CASE ";
    sql_stmt += "WHEN CUT = 1 THEN 0.007 ";
    sql_stmt += "WHEN CUT = 2 THEN 0.008 ";
    sql_stmt += "WHEN CUT = 3 THEN 0.009 ";
    sql_stmt += "WHEN CUT = 4 THEN 0.010 ";
    sql_stmt += "WHEN CUT = 5 THEN 0.012 ";
    sql_stmt += "WHEN CUT = 6 THEN 0.013 ";
    sql_stmt += "WHEN CUT = 7 THEN 0.015 ";
    sql_stmt += "WHEN CUT = 8 THEN 0.017 ";
    sql_stmt += "WHEN CUT = 9 THEN 0.020 ";
    sql_stmt += "WHEN CUT = 10 THEN 0.023 ";
    sql_stmt += "WHEN CUT = 11 THEN 0.026 ";
    sql_stmt += "WHEN CUT = 12 THEN 0.03 ";
    sql_stmt += "WHEN CUT = 13 THEN 0.034 ";
    sql_stmt += "WHEN CUT = 14 THEN 0.038 ";
    sql_stmt += "WHEN CUT = 15 THEN 0.043 ";
    sql_stmt += "WHEN CUT = 16 THEN 0.050 ";
    sql_stmt += "WHEN CUT = 17 THEN 0.060 ";
    sql_stmt += "WHEN CUT = 18 THEN 0.080 ";
    sql_stmt += "WHEN CUT = 19 THEN 0.119 ";
    sql_stmt += "WHEN CUT = 20 THEN 0.18 END) ";
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();

	  sql_stmt = "SELECT SCORE FROM PDACO_V1_00_CAL WHERE IDN = :idn;";
	  sql_stmt = sql_stmt.UpperCase();
	  query->Close();
	  query->SQL->Clear();
	  query->SQL->Add(sql_stmt);
	  query->Parameters->ParamValues["idn"] = idn;
	  query->Open();
	  risk_score = query->FieldValues["SCORE"];
	  query->Close();
	}
	else{
  	sql_stmt = "UPDATE PDACO_V1_00_CAL SET EXCLUSION = :exclusion ";
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["exclusion"] = exclusion;
	  command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();
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
    sql_stmt = "DROP TABLE Inbound_Report";
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
    sql_stmt = "DROP TABLE Outbound_Report";
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
    sql_stmt = "DROP TABLE Report";
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
    sql_stmt = "DROP TABLE First_Application";
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
    sql_stmt = "DROP TABLE CI";
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
    sql_stmt = "DROP TABLE TMP3";
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
    sql_stmt = "DROP TABLE FS_ISSUE";
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
};

void prescreen_out(unsigned int filter_flag, unsigned int insufficiency_flag, AnsiString idn,
                   AnsiString input_time)
{
  if (filter_flag & 0x20)
    throw cc_error(101, idn, input_time);
  if (filter_flag & 0x4)
    throw cc_error(102, idn, input_time);
  if (filter_flag & 0x40)
    throw cc_error(103, idn, input_time);
  if (filter_flag & 0x80)
    throw cc_error(104, idn, input_time);
  if (filter_flag & 0x10)
    throw cc_error(105, idn, input_time);
  if (filter_flag & 0x100)
    throw cc_error(107, idn, input_time);
  if (filter_flag & 0x400)
    throw cc_error(108, idn, input_time);
  if (filter_flag & 0x800)
    throw cc_error(110, idn, input_time);
  if (insufficiency_flag & 0x01)
    throw cc_error(111, idn, input_time);
  if (insufficiency_flag & 0x02)
    throw cc_error(112, idn, input_time);
  if (insufficiency_flag & 0x04)
    throw cc_error(113, idn, input_time);
  if (insufficiency_flag & 0x08)
    throw cc_error(201, idn, input_time);
};

void get_product_parameters(TADOQuery *query,
                            const AnsiString &idn, const AnsiString &input_time,
                            double &apr, unsigned int &loan_amount,
                            unsigned int &terms, unsigned int &application_fee)
{
	AnsiString sql_stmt;
	sql_stmt = "SELECT * FROM APPLICANT WHERE IDN = :idn AND INPUT_TIME = :input_time";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Parameters->ParamValues["input_time"] = input_time;
  query->Open();
  if(query->RecordCount ==0)
  	throw cc_error(312, idn, input_time);
  else if(query->RecordCount > 1)
  	throw cc_error(313, idn, input_time);
  else{
    if((query->FieldValues["APPLICATION_RATE"].IsNull()) || (query->FieldValues["APPLICATION_RATE"]<=0))
    	throw cc_error(310, idn, input_time);
    else
    	apr = query->FieldValues["APPLICATION_RATE"];
    if((query->FieldValues["APPLICATION_AMOUNT"].IsNull()) || (query->FieldValues["APPLICATION_AMOUNT"]<150000))
    	throw cc_error(202, idn, input_time);
    else
    	loan_amount = query->FieldValues["APPLICATION_AMOUNT"];
    if((query->FieldValues["APPLICATION_TERMS"].IsNull()) || (query->FieldValues["APPLICATION_TERMS"]<12))
    	throw cc_error(304, idn, input_time);
    else
    	terms = query->FieldValues["APPLICATION_TERMS"];
    if((query->FieldValues["APPLICATION_FEE"].IsNull()) || (query->FieldValues["APPLICATION_FEE"]<0))
    	throw cc_error(306, idn, input_time);
    else
    	application_fee = query->FieldValues["APPLICATION_FEE"];
  }

};

void cpdaco_v1_00_preparation(TADOQuery *query, TADOCommand *command,
                    const AnsiString &krm023, const AnsiString &krm001,
                    const AnsiString &jas002, const AnsiString &bam085,
                    const AnsiString &stm001,
                    const AnsiString &idn, int gender,
                    int now, unsigned int exclusion, unsigned int data_flag,
                    double &ms093, double &ms094, double &ms105, double &ms047,
                    double &ms080, double &wi006_9m, double &wi008_3m,
                    double &wi008_9m, double &wi002_12m, double &wi004_12m,
                    double &pdaco_score,
                    bool &ms093_isnull, bool &ms094_isnull, bool &ms105_isnull,
                    bool &ms047_isnull, bool &ms080_isnull,
                    bool &wi006_9m_isnull, bool &wi008_3m_isnull,
                    bool &wi008_9m_isnull, bool &wi002_12m_isnull,
                    bool &wi004_12m_isnull)
{
  pdaco_score = pdaco_1_00(query, command,
                           krm023, krm001, jas002, bam085, stm001,
                           idn, gender, now, exclusion, data_flag);
  ci_variables(query, command, now, krm023, bam085, idn, data_flag,
               ms093, ms094, ms105, ms047, ms080, wi006_9m, wi008_3m, wi008_9m,
               wi002_12m, wi004_12m, ms093_isnull, ms094_isnull, ms105_isnull,
               ms047_isnull, ms080_isnull, wi006_9m_isnull, wi008_3m_isnull,
               wi008_9m_isnull, wi002_12m_isnull, wi004_12m_isnull);

};

void ci_variables(TADOQuery *query, TADOCommand *command, int now,
                  const AnsiString &krm023, const AnsiString &bam085,
                  const AnsiString &idn, unsigned int data_flag,
                  double &ms093, double &ms094, double &ms105, double &ms047,
                  double &ms080, double &wi006_9m, double &wi008_3m,
                  double &wi008_9m, double &wi002_12m, double &wi004_12m,
                  bool &ms093_isnull, bool &ms094_isnull, bool &ms105_isnull, bool &ms047_isnull,
                  bool &ms080_isnull, bool &wi006_9m_isnull, bool &wi008_3m_isnull,
                  bool &wi008_9m_isnull, bool &wi002_12m_isnull, bool &wi004_12m_isnull)
{
  AnsiString sql_stmt;

  try{
    sql_stmt = "CREATE TABLE CI (IDN CHAR(11), MS047 float, MS080 float, ";
    sql_stmt += "MS093 float, MS094 float, MS105 float, WI002_12M float, ";
    sql_stmt += "WI004_12M float, WI006_9M float, WI008_3M float, ";
    sql_stmt += "WI008_9M float)";
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

	sql_stmt = "INSERT INTO CI(IDN) VALUES (:idn)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  if(data_flag & 0x4){
    sql_stmt = "UPDATE CI SET MS047 = 0, MS080 = 0, MS093 = 0, MS105 = 0 ";
    sql_stmt += "WHERE IDN = :idn";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
    command->Execute();
  };
  if(data_flag & 0x1){
    sql_stmt = "UPDATE CI SET MS094 = 0, WI002_12M = 0, WI004_12M = 0, ";
    sql_stmt += "WI006_9M = 0, WI008_3M = 0, WI008_9M = 0";
    sql_stmt += "WHERE IDN = :idn";
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
    command->Execute();
  };

  /*----MS047----*/
  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, V1) ";
  sql_stmt += "SELECT IDN, SUM(CONVERT(INT, CONTRACT_AMT)) FROM " + bam085;
  sql_stmt += " WHERE purpose_code = '1' GROUP BY IDN;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET MS047 = V1 FROM TMP AS A ";
  sql_stmt += "WHERE A.IDN = CI.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//MS080
  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, V1) ";
  sql_stmt += "SELECT IDN, SUM(CAST(CONTRACT_AMT AS FLOAT) - CAST(LOAN_AMT AS FLOAT) - CAST(PASS_DUE_AMT AS FLOAT)) ";
  sql_stmt += "FROM " + bam085 + " WHERE ACCOUNT_CODE != 'Y' GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET MS080 = V1 FROM TMP AS A WHERE A.IDN = CI.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//MS093
  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, V1) ";
  sql_stmt += "SELECT idn, SUM(CASE ";
  sql_stmt += "WHEN account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10* 150 / 1000 ";
  sql_stmt += "WHEN account_code = 'C' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 100 / 1000 ";
  sql_stmt += "WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 150 / 1000 ";
  sql_stmt += "WHEN account_code = 'E' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 100 / 1000 ";
  sql_stmt += "WHEN account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 238 / 1000 ";
  sql_stmt += "WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 177 / 1000 ";
  sql_stmt += "WHEN account_code = 'I' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 127 / 1000 ";
  sql_stmt += "WHEN account_code = 'I' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 72 / 1000 ";
  sql_stmt += "WHEN account_code = 'O' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 177 / 1000 ";
  sql_stmt += "WHEN account_code = 'O' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 238 / 1000 ";
  sql_stmt += "WHEN account_code = 'K' THEN (contract_amt) / 10 * 17 / 1000 ";
  sql_stmt += "WHEN account_code = 'Z' THEN (contract_amt) / 10 * 213 / 1000 ";
  sql_stmt += "END) FROM " + bam085 + " GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET MS093 = V1 FROM TMP AS A WHERE A.IDN = CI.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//MS094
  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, V1) ";
  sql_stmt += "SELECT idn, SUM(PAYMENT_AMT) FROM " + krm023;
  sql_stmt += " WHERE :now - MON_SINCE <= 3 AND PAY_CODE IN ('C', 'D', 'E', 'F') ";
  sql_stmt += "GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, V1) ";
  sql_stmt += "SELECT idn, AVG(V1) FROM TMP ";
  sql_stmt += "GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*Anne's code
  sql_stmt = "INSERT INTO TMP1(idn, V1) ";
  sql_stmt += "SELECT MSN, SUM(PAYMENT_AMT) / 3.0 FROM " + krm023;
  sql_stmt += " WHERE :now - MON_SINCE <=3 AND PAY_CODE IN ('C', 'D', 'E', 'F') ";
  sql_stmt += "GROUP BY msn";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();*/

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET MS094 = V1 FROM TMP1 AS A WHERE A.IDN = CI.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//MS105
  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, V1) ";
  sql_stmt += "SELECT idn, sum( convert(float, loan_amt))+sum(convert(float, pass_due_amt)) ";
  sql_stmt += "FROM " + bam085 + " where account_code='Y' GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET MS105 = V1 FROM TMP AS A WHERE A.IDN = CI.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//WI002_12M
  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, MON, V1) ";
  sql_stmt += "SELECT IDN, MON_SINCE, COUNT(*) FROM " + krm023;
  sql_stmt += " WHERE CONVERT(INT, LIMIT) > 100 GROUP BY IDN, MON_SINCE";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, V1) ";
  sql_stmt += "SELECT IDN, AVG(CONVERT(FLOAT,V1)) FROM TMP1 WHERE :now - MON BETWEEN 1 AND 12 ";
  sql_stmt += "GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET WI002_12M = V1 FROM TMP AS A WHERE A.IDN = CI.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();


//WI004_12M
  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, MON, V1) ";
  sql_stmt += "SELECT idn, MON_SINCE, SUM(CONVERT(INT, LIMIT)) ";
  sql_stmt += "FROM " + krm023 + " GROUP BY IDN, MON_SINCE";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, V1) ";
  sql_stmt += "SELECT IDN, AVG(V1) FROM TMP1 WHERE :now - MON BETWEEN 1 AND 12 ";
  sql_stmt += "GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET WI004_12M = V1 FROM TMP AS A WHERE A.IDN = CI.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();


//WI006_9M
  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP3";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP3(IDN) ";
  sql_stmt += "SELECT IDN FROM " + krm023 + " GROUP BY IDN ";
  sql_stmt += "HAVING MAX(BUCKET_EF_1K) = 0";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, MON, V1, V2) ";
  sql_stmt += "SELECT IDN, MON_SINCE, 0, 0 FROM " + krm023 + " WHERE IDN IN ";
  sql_stmt += "(SELECT IDN FROM TMP3) GROUP BY IDN, MON_SINCE";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, MON, V1) ";
  sql_stmt += "SELECT IDN, MON_SINCE, SUM(PAYMENT_AMT) FROM " + krm023;
  sql_stmt += " WHERE PAY_CODE IN ('C', 'D') AND ";
  sql_stmt += "IDN IN (SELECT IDN FROM TMP3) GROUP BY IDN, MON_SINCE";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE TMP SET V1 = A.V1 FROM TMP1 AS A ";
  sql_stmt += "WHERE A.IDN = TMP.IDN AND A.MON = TMP.MON";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE TMP SET V2 = (CASE WHEN (TMP.V1 - A.V1) >= 0 THEN 0 ";
  sql_stmt += "ELSE (TMP.V1 - A.V1) * -1 END) FROM TMP AS A ";
  sql_stmt +="INNER JOIN TMP ON A.IDN = TMP.IDN AND A.MON = TMP.MON - 1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP3";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP3 (idn, V1) ";
  sql_stmt += "SELECT IDN, AVG(V2) FROM TMP WHERE :now - MON BETWEEN 1 AND 9 ";
  sql_stmt += "GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET WI006_9M = A.V1 FROM TMP3 AS A WHERE A.IDN = CI.IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();


//WI008_3M
  sql_stmt = "DELETE FROM TMP";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP(IDN, MON, V2) ";
  sql_stmt += "SELECT IDN, :NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + krm023 + " GROUP BY IDN, MON_SINCE";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, MON, V1)";
  sql_stmt += "SELECT IDN, MON_SINCE, SUM(PAYMENT_AMT) FROM " + krm023;
  sql_stmt += " WHERE PAY_CODE IN ('A', 'B') GROUP BY IDN, MON_SINCE";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE TMP ";
  sql_stmt += "SET V1 = (CASE WHEN V2 > 0 THEN NULL ELSE A.V1 END) ";
  sql_stmt += "FROM TMP1 AS A INNER JOIN TMP AS B ON A.IDN = B.IDN ";
  sql_stmt += "AND A.MON = B.MON";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1(IDN, MON, V1) ";
  sql_stmt += "SELECT IDN, 3, SUM(V1) FROM TMP WHERE MON BETWEEN 1 AND 3 ";
  sql_stmt += "GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET WI008_3M = A.V1 FROM TMP1 AS A WHERE A.IDN = CI.IDN ";
  sql_stmt += "AND A.MON = 3";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//WI008_9M
  sql_stmt = "INSERT INTO TMP1(IDN, MON, V1) ";
  sql_stmt += "SELECT IDN, 9, SUM(V1) FROM TMP WHERE MON BETWEEN 1 AND 9 ";
  sql_stmt += "GROUP BY IDN";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE CI ";
  sql_stmt += "SET WI008_9M = A.V1 FROM TMP1 AS A WHERE A.IDN = CI.IDN ";
  sql_stmt += "AND A.MON = 9";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "SELECT * FROM CI WHERE IDN = :idn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Open();

  ms047 = query->FieldValues["MS047"].IsNull() ? 0 : query->FieldValues["MS047"];
  if(query->FieldValues["MS047"].IsNull())
    ms047_isnull = true;
  ms080 = query->FieldValues["MS080"].IsNull() ? 0 : query->FieldValues["MS080"];
  if(query->FieldValues["MS080"].IsNull())
    ms080_isnull = true;
  ms093 = query->FieldValues["MS093"].IsNull() ? 0 : query->FieldValues["MS093"];
  if(query->FieldValues["MS093"].IsNull())
    ms093_isnull = true;
  ms094 = query->FieldValues["MS094"].IsNull() ? 0 : query->FieldValues["MS094"];
  if(query->FieldValues["MS094"].IsNull())
    ms094_isnull = true;
  ms105 = query->FieldValues["MS105"].IsNull() ? 0 : query->FieldValues["MS105"];
  if(query->FieldValues["MS105"].IsNull())
    ms105_isnull = true;
  wi002_12m = query->FieldValues["WI002_12M"].IsNull() ? 0 : query->FieldValues["WI002_12M"];
  if(query->FieldValues["wi002_12m"].IsNull())
    wi002_12m_isnull = true;
  wi004_12m = query->FieldValues["WI004_12M"].IsNull() ? 0 : query->FieldValues["WI004_12M"];
  if(query->FieldValues["wi004_12m"].IsNull())
    wi004_12m_isnull = true;
  wi006_9m = query->FieldValues["WI006_9M"].IsNull() ? 0 : query->FieldValues["WI006_9M"];
  if(query->FieldValues["wi006_9m"].IsNull())
    wi006_9m_isnull = true;
  wi008_3m = query->FieldValues["WI008_3M"].IsNull() ? 0 : query->FieldValues["WI008_9M"];
  if(query->FieldValues["wi008_3m"].IsNull())
    wi008_3m_isnull = true;
  wi008_9m = query->FieldValues["WI008_9M"].IsNull() ? 0 : query->FieldValues["WI008_9M"];
  if(query->FieldValues["wi008_9m"].IsNull())
    wi008_9m_isnull = true;
};

void store_result(TADOCommand *command,
                  const AnsiString &idno,
                  const AnsiString &input_time, int result_code,
                  const AnsiString &result_message,
                  unsigned int unsecured_balance, int approved_amount_dac,
                  int approved_amount_fsc, unsigned int unsecured_balance_dac,
                  unsigned int monthly_income, int npv, double pb,
                  double min_rate, const AnsiString &note,
                  const AnsiString &version, bool normal)
{
  AnsiString sql_stmt;
  AnsiString result_string;
  if(normal){
    int result_output;
/*    if(npv > -1000 && npv < 0){
      result_output = 4;
      result_string = "模組建議人工試算。";
    }
    else*/ if(npv <=0){
      result_output = 2;
      result_string = "模組建議婉拒：經濟價值過低。";
    }
    else if((monthly_income >= INCOME_CAP) && (approved_amount_dac >= APPROVED_AMT_CAP)){
      result_output = 1;
      result_string = "請核實收入，無誤後，模組建議核准。";
    }
    else{
      result_output = 1;
      result_string = "模組建議核准。";
    };

    sql_stmt = "INSERT INTO Result (IDN, Input_Time, ";
    sql_stmt += "APPROVED_AMOUNT, PB, NPV, Suggestion, Suggestion_code, ";
    sql_stmt += "Min_Rate, Unsecured_Balance, APPROVED_AMOUNT_FSC, ";
    sql_stmt += "SPECIAL_NOTE, UNSECURED_BALANCE_DAC, VERSION) ";
    sql_stmt += "VALUES(:idno, :input_time, :approved_amount, :pb, :npv, ";
    sql_stmt += ":message, :code, :min_rate, :unsecured_balance, ";
    sql_stmt += ":approved_amount_fsc, :note, :unsecured_balance_dac, :version);";
    sql_stmt = sql_stmt.UpperCase();
    int output_npv;
    if(npv < 0)
      output_npv = 0;
    else
      output_npv = npv;
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idno"] = idno;
    command->Parameters->ParamValues["input_time"] = input_time;
    command->Parameters->ParamValues["npv"] = output_npv;
    command->Parameters->ParamValues["code"] = result_output;
    command->Parameters->ParamValues["message"] = result_string;
    command->Parameters->ParamValues["approved_amount"] = approved_amount_dac;
    command->Parameters->ParamValues["approved_amount_fsc"] = approved_amount_fsc;
    command->Parameters->ParamValues["pb"] = pb;
    command->Parameters->ParamValues["min_rate"] = min_rate;
    command->Parameters->ParamValues["unsecured_balance"] = unsecured_balance;
    command->Parameters->ParamValues["unsecured_balance_dac"] = unsecured_balance_dac;
    command->Parameters->ParamValues["note"] = note;
    command->Parameters->ParamValues["version"] = version;
    command->Execute();
  }
  else{
    int result_output;
    sql_stmt = "INSERT INTO Result (IDN, Input_Time, ";
    sql_stmt += "APPROVED_AMOUNT, PB, NPV, Suggestion, Suggestion_code, ";
    sql_stmt += "Unsecured_Balance, APPROVED_AMOUNT_FSC, SPECIAL_NOTE, ";
    sql_stmt += "UNSECURED_BALANCE_DAC, VERSION) ";
    sql_stmt += "VALUES(:idno, :input_time, :approved_amount, :pb, :npv, ";
    sql_stmt += ":message, :code, :unsecured_balance, :approved_amount_fsc, ";
    sql_stmt += ":note, :unsecured_balance_dac, :version);";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idno"] = idno;
    command->Parameters->ParamValues["input_time"] = input_time;
    command->Parameters->ParamValues["npv"] = 0;
    command->Parameters->ParamValues["pb"] = pb;
    command->Parameters->ParamValues["approved_amount"] = 0;
    command->Parameters->ParamValues["approved_amount_fsc"] = 0;
    command->Parameters->ParamValues["unsecured_balance"] = unsecured_balance;
    command->Parameters->ParamValues["unsecured_balance_dac"] = unsecured_balance_dac;
    command->Parameters->ParamValues["note"] = note;
    command->Parameters->ParamValues["version"] = version;
    switch (result_code){
      case 0:
      case 1:
      case 301:
      case 302:
      case 303:
      case 304:
      case 305:
      case 306:
      case 307:
      case 308:
      case 309:
      case 310:
      case 311:
      case 312:
      case 313:
      case 314:
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
      case 110:
      case 111:
      case 112:
      case 113:      
        result_output = 2;
        break;
      case 201:
        result_output = 3;
        break;
    };
    command->Parameters->ParamValues["code"] = result_output;
    command->Parameters->ParamValues["message"] = result_message;
    command->Execute();
  };
};


//The following is testing code
double get_pdaco_score(TADOQuery *query, const AnsiString &idn)
{
  AnsiString sql_stmt;
  double return_value;
  sql_stmt = "SELECT PDACO_SCORE FROM input_data_step1 WHERE IDN = :idn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Open();
  return_value = query->FieldValues["PDACO_SCORE"];
  query->Close();
  return return_value;
};

void store_ci_score(TADOCommand *command, const AnsiString &idn,
                    const AnsiString &input_time, double ci_score)
{
	AnsiString sql_stmt;
  try{
	  sql_stmt = "CREATE TABLE CI_SCORE (IDN CHAR(11), INPUT_TIME CHAR(12), ";
	  sql_stmt += "SCORE float)";
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
	sql_stmt = "INSERT INTO CI_SCORE(IDN, INPUT_TIME, SCORE) VALUES(";
	sql_stmt += ":idn, :input_time, :score)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
	command->Parameters->ParamValues["idn"] = idn;
  command->Parameters->ParamValues["input_time"] = input_time;
  command->Parameters->ParamValues["score"] = ci_score;
  command->Execute();
};

double get_pdaco_1_00_pb(TADOQuery *query, const AnsiString &idn)
{
  AnsiString sql_stmt = "SELECT PB FROM PDACO_V1_00_CAL WHERE IDN = :idn;";
  double pb;
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Open();
  pb = query->FieldValues["PB"];
  query->Close();
  return pb;
};

unsigned int check_credit_card_block(TADOQuery *query, const AnsiString &idn)
{
  AnsiString sql_stmt;
  unsigned int return_value = 0x0;
  sql_stmt = "SELECT COUNT(*) AS CNT FROM CREDIT_BLOCK_LIST WHERE IDN = :idn ";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Open();
  if(query->FieldValues["CNT"] > 0)
    return_value = 0x800;
  return return_value;
};

double get_pb(TADOQuery *query, const AnsiString &idn)
{
  AnsiString sql_stmt = "SELECT PB FROM RESULT_MIN_RATE WHERE IDN = :idn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Open();
  double return_value = query->FieldValues["PB"];
  query->Close();
  return return_value;
};

unsigned int get_income_product(TADOQuery *query,
                        const AnsiString &idn, const AnsiString &input_time,
                        unsigned int &product_type)
{
  AnsiString sql_stmt;
  unsigned int monthly_income;
  sql_stmt = "SELECT MONTHLY_INCOME, PRODUCT_TYPE FROM APPLICANT WHERE IDN = :idn AND ";
  sql_stmt += "INPUT_TIME = :input_time";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Parameters->ParamValues["input_time"] = input_time;
  query->Open();
  monthly_income = query->FieldValues["MONTHLY_INCOME"];
  product_type = query->FieldValues["PRODUCT_TYPE"];
  query->Close();
  return monthly_income;
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

unsigned int data_insufficiency(TADOQuery *query,
                                unsigned int data_flag,
                                unsigned int filter_flag,
                                const AnsiString &krm023,
                                const AnsiString &krm001,
                                const AnsiString &stm001, int now)
{
  unsigned int return_value = 0;
  unsigned int delinquent_lines = 0;
  unsigned int inquiries = 0;
  unsigned int stop_cards = 0;

  if((filter_flag & 0x1) || (filter_flag & 0x2) || (filter_flag & 0x200) || (filter_flag & 0x8)){
    AnsiString sql_stmt;
    return_value |= 0x8;
    if(data_flag & 0x1){
      sql_stmt += "SELECT IDN, COUNT(DISTINCT ISSUE) AS V1 ";
      sql_stmt += "FROM " + krm023 + " ";
      sql_stmt += "WHERE Bucket_ef_1K >=1 AND ";
      sql_stmt += "(:now - Mon_Since)<= 12 AND ";
      sql_stmt += "(:now1 - Mon_Since) > 0 ";
      sql_stmt += "GROUP BY IDN";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["now"] = now;
      query->Parameters->ParamValues["now1"] = now;
      query->Open();
      if(!query->FieldValues["V1"].IsNull())
        delinquent_lines = query->FieldValues["V1"];
    };
    if(data_flag & 0x2){
      sql_stmt = "SELECT IDN, COUNT(DISTINCT ISSUE) AS V1 ";
      sql_stmt += "FROM " + krm001 + " ";
      sql_stmt += "WHERE STOP_CODE = '3' ";
      sql_stmt += "GROUP BY IDN";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Open();
      if(!query->FieldValues["V1"].IsNull())
        stop_cards = query->FieldValues["V1"];
    };
    if(data_flag & 0x8){
		  sql_stmt ="SELECT IDN, COUNT(*) AS V1 ";
  		sql_stmt +="FROM " + stm001 + " ";
	  	sql_stmt +="WHERE Item_List IS NOT NULL AND  ";
		  sql_stmt +="Item_List <> '' AND  ";
  		sql_stmt +="(:now - QUERY_MON_SINCE) <= 12 ";
	  	sql_stmt +="GROUP BY IDN";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->Parameters->ParamValues["now"] = now;
      query->Open();
      if(!query->FieldValues["V1"].IsNull())
        inquiries = query->FieldValues["V1"];
    };
    if(data_flag & 0x01){
      if(inquiries > 5)
        return_value |= 0x01;
      if(delinquent_lines > 0)
        return_value |= 0x02;
      if(stop_cards > 0)
        return_value |= 0x04;
    }
    else if((data_flag & 0x02) || (data_flag & 0x04)){
      if(inquiries > 3)
        return_value |= 0x01;
      if(stop_cards > 0)
        return_value |= 0x04;
    }
    else
      return_value |= 0x8;
  }
  else
    return_value = 0;
  return return_value;
};
