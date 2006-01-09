//---------------------------------------------------------------------------

#ifndef mainH
#define mainH

#define CONNECTION_STRING  "FILE NAME=./AdoConnect.udl"
#define NSTEPS (sizeof step / sizeof step[0])
#define DEBUG  if (Debug) fprintf
//---------------------------------------------------------------------------
char * CurrDateTime ();

enum SQLCodes { Create_Source_Table,
                Insert_Source_Table,
                Select_Close_Account,
                Select_Close_Account_Date,
                Select_Invalid_Statements,
                Delete_Redundant_Statements,
                Update_Month_Since,
                Create_Output_Table,
                Insert_ID_to_Output,
                Execute_Proc_Update_Max_Cycle,
                Drop_Proc_Update_Max_Cycle,
                Create_Proc_Update_Max_Cycle,
                Update_Now_on_Output,

                Create_Index_on_Stmt,
                Execute_Proc_Get_Prev_Stmt_Info,
                Drop_Proc_Get_Prev_Stmt_Info,
                Create_Proc_Get_Prev_Stmt_Info,
                Cal_Stmt_Flag,
                Insert_Stmt_3,
                Create_Index_Stmt_3,
                Insert_Stmt_6,
                Create_Index_Stmt_6,
                Execute_Proc_Cal_BucketM_on_Stmt_6,
                Drop_Proc_Cal_BucketM_on_Stmt_6,
                Create_Proc_Cal_BucketM_on_Stmt_6,
                Insert_Stmt_9,
                Create_Index_Stmt_9,

                Update_Indicators,
                Update_Vintage,
                Update_Demographics,
                Update_Stmt3_Count,
                Update_Stmt6_Count,
                Update_Stmt9_Count,
                Update_Stmt12_Count,
                Update_WM_Flag,
                Update_Insurance_Flag,
                Update_Close_Flag,
                Update_Card,

                Cal_FS003,
                Cal_FS072,
                Cal_FS089,
                Cal_FS096,
                Cal_MS035,
                Cal_MS062,
		Cal_RS001,
                Transform_R1_Vars,
                Cal_R1_Score_Twentile,

                Cal_FS191,
                Cal_FS204,
                Transform_T1_Vars,
                Cal_T1_Score_Twentile,

                Cal_MS023,
                Cal_FS197,
                Transform_T2_Vars,
                Cal_T2_Score_Twentile,

                Transform_N1_Vars,
                Cal_N1_Score_Twentile,
                Generate_Output_Table,
                Duplicate_Working_Table,
                Generate_Summary,

                End_of_SQL};

int step[] = {
	Create_Source_Table,
	Insert_Source_Table,
        Select_Close_Account,
        Select_Close_Account_Date,
        Select_Invalid_Statements,
        Delete_Redundant_Statements,
	Update_Month_Since,
	Create_Output_Table,
	Insert_ID_to_Output,

        Drop_Proc_Update_Max_Cycle,
        Create_Proc_Update_Max_Cycle,
        Execute_Proc_Update_Max_Cycle,
        Drop_Proc_Update_Max_Cycle,
	Update_Now_on_Output,

	Create_Index_on_Stmt,
        Drop_Proc_Get_Prev_Stmt_Info,
        Create_Proc_Get_Prev_Stmt_Info,
        Execute_Proc_Get_Prev_Stmt_Info,
        Drop_Proc_Update_Max_Cycle,
	Cal_Stmt_Flag,
	Insert_Stmt_3,
	Create_Index_Stmt_3,
	Insert_Stmt_6,
	Create_Index_Stmt_6,

        Drop_Proc_Cal_BucketM_on_Stmt_6,
        Create_Proc_Cal_BucketM_on_Stmt_6,
        Execute_Proc_Cal_BucketM_on_Stmt_6,
        Drop_Proc_Cal_BucketM_on_Stmt_6,
	Insert_Stmt_9,
	Create_Index_Stmt_9,

	Update_Indicators,
        Update_Vintage,
	Update_Demographics,
	Update_Stmt3_Count,
	Update_Stmt6_Count,
	Update_Stmt9_Count,
	Update_Stmt12_Count,
	Update_WM_Flag,
	Update_Insurance_Flag,
	Update_Close_Flag,
	Update_Card,

	Cal_FS003,
	Cal_FS072,
	Cal_FS089,
	Cal_FS096,
	Cal_MS035,
	Cal_MS062,
        Cal_RS001,
	Transform_R1_Vars,
	Cal_R1_Score_Twentile,

	Cal_FS191,
	Cal_FS204,
	Transform_T1_Vars,
	Cal_T1_Score_Twentile,

// Generate score for card T2, set to null for delivery
//	Cal_MS023,
//	Cal_FS197,
//	Transform_T2_Vars,
//	Cal_T2_Score_Twentile,

	Transform_N1_Vars,
	Cal_N1_Score_Twentile,

        Generate_Output_Table,
        Duplicate_Working_Table,

 	End_of_SQL
};

char *SQLNames[]= {"Create_Source_Table",
                "Insert_Source_Table",
                "Select_Close_Account",
                "Select_Close_Account_Date",
                "Select_Invalid_Statements",
                "Delete_Redundant_Statements",
                "Update_Month_Since",
                "Create_Output_Table",
                "Insert_ID_to_Output",
                "Execute_Proc_Update_Max_Cycle",
                "Drop_Proc_Update_Max_Cycle",
                "Create_Proc_Update_Max_Cycle",
                "Update_Now_on_Output",

                "Create_Index_on_Stmt",
                "Execute_Proc_Get_Prev_Stmt_Info",
                "Drop_Proc_Get_Prev_Stmt_Info",
                "Create_Proc_Get_Prev_Stmt_Info",
                "Cal_Stmt_Flag",
                "Insert_Stmt_3",
                "Create_Index_Stmt_3",
                "Insert_Stmt_6",
                "Create_Index_Stmt_6",
                "Execute_Proc_Cal_BucketM_on_Stmt_6",
                "Drop_Proc_Cal_BucketM_on_Stmt_6",
                "Create_Proc_Cal_BucketM_on_Stmt_6",
                "Insert_Stmt_9",
                "Create_Index_Stmt_9",

                "Update_Indicators",
                "Update_Vintage",
                "Update_Demographics",
                "Update_Stmt3_Count",
                "Update_Stmt6_Count",
                "Update_Stmt9_Count",
                "Update_Stmt12_Count",
                "Update_WM_Flag",
                "Update_Insurance_Flag",
                "Update_Close_Flag",
                "Update_Card",

                "Cal_FS003",
                "Cal_FS072",
                "Cal_FS089",
                "Cal_FS096",
                "Cal_MS035",
                "Cal_MS062",
		"Cal_RS001",
                "Transform_R1_Vars",
                "Cal_R1_Score_Twentile",

                "Cal_FS191",
                "Cal_FS204",
                "Transform_T1_Vars",
                "Cal_T1_Score_Twentile",

                "Cal_MS023",
                "Cal_FS197",
                "Transform_T2_Vars",
                "Cal_T2_Score_Twentile",

                "Transform_N1_Vars",
                "Cal_N1_Score_Twentile",
                "Generate_Output_Table",
                "Duplicate_Working_Table",
                "Generate_Summary"
 };


char *SQLCommands[] = {
/* Create_Source_Table */
" IF OBJECT_ID('tempdb..#fubon_cc_stmts') IS NOT NULL"
"    drop table #fubon_cc_stmts;"
" create table #fubon_cc_stmts ("
"    billing_close_date datetime,"
"    cardholder_status_code char (1),"
"    idn varchar(11),"
"    last_payment_amt int,"
"    monthly_limit_amt int,"
"    revolving_interest_amt int,"
"    this_term_expenditure_amt int,"
"    this_term_min_payment int,"
"    this_term_total_amt_receivable int,"
"    this_term_cash_advance_amt int,"
"    mon_since int,"
"    bucket_M int,"
"    cash_adv int,"
"    bal_dec int default 0,"
"    bal_pct_diff float,"
"    min_pay int default 0,"
"    exp_dec int default 0,"
"    exp_pct_diff float,"
"    last_balance int,"
"    last_min_payment int,"
"    last_expenditure int,"
"    last_cash_advance int,"
"    last_revolving_int_amt int"
"   );"
" IF OBJECT_ID('tempdb..#FB_close_acct') IS NOT NULL"
"    drop table #FB_close_acct;"
" create table #FB_close_acct ("
"    Primary_Cardholder_ID char(11)"
"   );"
" IF OBJECT_ID('tempdb..#FB_close_acct_date') IS NOT NULL"
"    drop table #FB_close_acct_date;"
" create table #FB_close_acct_date ("
"    Primary_Cardholder_ID char(11),"
"    card_cancel_date      datetime"
"   );"
" IF OBJECT_ID('tempdb..#FB_invalid_stmts') IS NOT NULL"
"    drop table #FB_invalid_stmts;"
" create table #FB_invalid_stmts ("
"    Primary_Cardholder_ID char(11),"
"    card_cancel_date      datetime,"
"    billing_close_date    datetime,"
"    balance               int "
"   );",

/*Insert_Source_Table*/
" insert into  #fubon_cc_stmts(billing_close_date, idn, last_payment_amt,"
"  monthly_limit_amt, revolving_interest_amt, this_term_expenditure_amt, this_term_min_payment,"
"  this_term_total_amt_receivable, this_term_cash_advance_amt)"
" select billing_close_date, primary_cardholder_id, last_payment_amt,"
"  monthly_limit_amt, revolving_interest_amt, this_term_expenditure_amt, this_term_min_payment,"
"  this_term_total_amt_receivable, this_term_cash_advance_amt"
" from cc_credit_card_statements",

/*Select_Close_Account*/
" insert into #FB_close_acct(Primary_Cardholder_ID)"
" select distinct a.Primary_Cardholder_ID"
" from cc_acct_credit_card as a left join"
"    (select distinct Primary_Cardholder_ID"
"     from cc_acct_credit_card"
"     where Acct_Status_Code = 0)  as b"
" on a.Primary_Cardholder_ID = b.Primary_Cardholder_ID"
" where b.Primary_Cardholder_ID is null",

/*Select_Close_Account_Date*/
" insert into #FB_close_acct_date"
" select a.Primary_Cardholder_ID, max(a.Card_Cancel_Date) as card_cancel_date"
" from cc_acct_credit_card  a,"
"      #FB_close_acct b"
" where a.Primary_Cardholder_ID = b.Primary_Cardholder_ID"
" group by a.Primary_Cardholder_ID;",

/*Select_Invalid_Statements*/
" insert into #FB_invalid_stmts(Primary_Cardholder_ID,card_cancel_date,billing_close_date,balance)"
" select a.Primary_Cardholder_ID,"
"        b.card_cancel_date,"
"        a.billing_close_date,"
"        This_Term_Total_Amt_Receivable"
" from #FB_close_acct_date b,"
"      cc_credit_card_statements a"
" where b.Primary_Cardholder_ID = a.Primary_Cardholder_ID"
" AND   DATEDIFF(day,card_cancel_date, billing_close_date) > 30"
" and   This_Term_Total_Amt_Receivable <= 0;",

/*Delete_Redundant_Statements*/
" delete #fubon_cc_stmts"
" from #FB_invalid_stmts as t1"
" where #fubon_cc_stmts.idn = t1.Primary_Cardholder_ID"
"   and #fubon_cc_stmts.billing_close_date = t1.billing_close_date;",

/*Update_Month_Since*/
"update #fubon_cc_stmts"
"   set mon_since = (cast (substring(convert(char(8),billing_close_date,112),1,4) as int) - 1911) * 12 +"
"                    cast (substring(convert(char(8),billing_close_date,112),5,2) as int);",

/*Create_Output_Table*/
" IF OBJECT_ID('tempdb..#response_model') IS NOT NULL"
"    drop table #response_model;"
" create table #response_model ("
"	Primary_Cardholder_ID	char(11),"
"	start_date	datetime,"
"	end_date	datetime,"
"	max_cycle	char(6),"
"	now		int,"
"	cashADV_ind	int default 0,"
"	revINT_ind	int default 0,"
"	expAMT_ind	int default 0,"
"	Vintage_ind	int default 0,"
"	wm_flag		int default 0,"
"	ins_flag	int default 0,"
"	close_flag	int default 0,"
"	stmt_3		int default 0,"
"	stmt_6		int default 0,"
"	stmt_9		int default 0,"
"	stmt_12		int default 0,"
"	card		varchar(2),"
"	gender		int,"
"	age		int,"
"	edu		int,"
"	fs003_6		float default 0,"
"	fs072_9		float default 0,"
"	fs089_3		float default 0,"
"	fs096_12	float default 0,"
"	ms035_12	float default 0,"
"	ms062_12	float default 0,"
"	rs001		float default 0,"
"	fs191_12	float default 0,"
"	fs204		float default 0,"
"	ms023_12	float default 0,"
"	fs197_9		float default 0,"
"	age_t_r1	float,"
"	ms035_12_t_r1	float,"
"	rs001_r		float,"
"	rs001_r_t_r1	float,"
"	FS089_3_n	float,"
"	FS089_3_n_q	float,"
"	FS089_3_n_q_t_r1	float,"
"	MS062_12_r	float,"
"	MS062_12_r_t_r1 float,"
"	FS096_12_n	float,"
"	FS096_12_n_t_r1	float,"
"	FS072_9_n	float,"
"	FS072_9_n_t_r1	float,"
"	FS003_6_n	float,"
"	FS003_6_n_r	float,"
"	FS003_6_n_r_t_r1	float,"
"	rscore_r1	float,"
"	twentile_r1	float,"
"	rs001_t_t1	float,"
"	fs191_12_n	float,"
"	fs191_12_n_r	float,"
"	fs191_12_n_r_t_t1	float,"
"	FS204_t_t1	float,"
"	edu_t_t1	float,"
"	rscore_t1	float,"
"	twentile_t1	float,"
"	ms023_12_1k	float,"
"	ms023_12_1k_t2	float,"
"	FS197_9_n	float,"
"	FS197_9_n_t_t2	float,"
"	rscore_t2	float,"
"	twentile_t2	float,"
"	age_t_n1	float,"
"	edu_t_n1	float,"
"	fs204_t_n1	float,"
"	index7		float,"
"	rscore_n1	float,"
"	twentile_n1	float"
");",

/*Insert_ID_to_Output*/
" insert #response_model(Primary_Cardholder_ID, start_date, end_date)"
" select  Primary_Cardholder_ID,"
"         min(Card_Issue_Date) as start_date,"
"         max(Card_Cancel_Date) as end_date"
" from cc_acct_credit_card"
" group by Primary_Cardholder_ID;",

/* Execute_Proc_Update_Max_Cycle */
"EXEC Update_Max_Cycle :v0",

/* Drop_Proc_Update_Max_Cycle */
"if exists (select * from dbo.sysobjects where id = object_id(N'[Update_Max_Cycle]')"
" and OBJECTPROPERTY(id, N'IsProcedure') = 1)"
"drop procedure [Update_Max_Cycle]",

/*Create_Proc_Update_Max_Cycle*/
" CREATE PROCEDURE Update_Max_Cycle"
" (@cycle_date varchar(8))"
" AS"
" declare @yyyymm int;"
" declare @yyyy int;"
" declare @mm int;"
" declare @dd int;"
" declare @prev_month varchar(10);"
" set @yyyymm = cast (substring(@cycle_date, 1, 6) as int)"
" set @yyyy = (@yyyymm - @yyyymm % 100 ) / 100"
" set @mm = @yyyymm % 100"
" if  @mm <= 1"
" begin"
"  set @mm = @mm - 1 + 12"
"  set @yyyy = @yyyy - 1"
" end"
" else"
"  set @mm = @mm - 1"
" set @prev_month = cast (((@yyyy * 100 + @mm) * 100 + 01) as char(8))"
" update #response_model"
" set max_cycle = (case when max_date < @prev_month then substring(@prev_month,1,6)"
"                      else left(convert(char(8),max_date,112), 6) end)"
" from (select idn,"
"             max(Billing_Close_Date) as max_date"
"      from  #fubon_cc_stmts"
"      group by idn) as a"
" where #response_model.Primary_Cardholder_ID = a.idn;",

/*Update_Now_on_Output*/
"update #response_model"
" set now = (substring(max_cycle, 1, 4) - 1911) * 12 + substring(max_cycle, 5, 2);",

/*Create_Index_on_Stmt*/
"create index i_idn on #fubon_cc_stmts(idn);",

/* Execute_Proc_Get_Prev_Stmt_Info */
"EXEC Get_Prev_Stmt_Info :v0",

/* Drop_Proc_Get_Prev_Stmt_Info */
"if exists (select * from dbo.sysobjects where id = object_id(N'[Get_Prev_Stmt_Info]')"
" and OBJECTPROPERTY(id, N'IsProcedure') = 1)"
"drop procedure [Get_Prev_Stmt_Info]",

/*Create_Proc_Get_Prev_Stmt_Info*/
" CREATE PROCEDURE Get_Prev_Stmt_Info"
" (@now int)"
" AS"
" declare @i int"
" set @i=12"
" while @i >= 0"
"    begin"
"       update #fubon_cc_stmts"
"          set last_balance = b.this_term_total_amt_receivable,"
"              last_min_payment = b.this_term_min_payment,"
"              last_expenditure = b.this_term_expenditure_amt,"
"              last_cash_advance = b.this_term_cash_advance_amt,"
"	       last_revolving_int_amt = b.revolving_interest_amt"
"          from #fubon_cc_stmts, #fubon_cc_stmts as b"
"          where #fubon_cc_stmts.mon_since = b.mon_since + 1"
"            and #fubon_cc_stmts.idn = b.idn"
"            and (@now - #fubon_cc_stmts.mon_since) = @i"
"       set @i = @i - 1"
"    end;",

/*Cal_Stmt_Flag*/
" update #fubon_cc_stmts"
"    set bal_dec  = (case when this_term_total_amt_receivable < last_balance then  1 else 0 end),"
"        bal_pct_diff  = convert(float,(this_term_total_amt_receivable - last_balance))"
"                        / (case when last_balance=0 then null else last_balance end),"
"        min_pay  = (case when (last_payment_amt = last_min_payment) and (last_min_payment > 0) then 1 else 0 end),"
"        exp_dec  = (case when This_Term_Expenditure_Amt < last_expenditure then 1 else 0 end),"
"        exp_pct_diff  = convert(float,(This_Term_Expenditure_Amt - last_expenditure))"
"                        / (case when last_expenditure = 0 then null else last_expenditure end)",

/*Insert_Stmt_3*/
" IF OBJECT_ID('tempdb..#fubon_cc_stmts_3') IS NOT NULL"
"    drop table #fubon_cc_stmts_3;"
" select a.*"
"   into #fubon_cc_stmts_3"
"   from #fubon_cc_stmts a, #response_model b"
"   where a.idn = b.Primary_Cardholder_ID"
"     and a.mon_since > (b.now - 3);",

/*Create_Index_Stmt_3*/
"create index i_idn_3 on #fubon_cc_stmts_3(idn);",

/*Insert_Stmt_6*/
" IF OBJECT_ID('tempdb..#fubon_cc_stmts_6') IS NOT NULL"
"    drop table #fubon_cc_stmts_6;"
" select a.*"
"   into #fubon_cc_stmts_6"
"   from #fubon_cc_stmts a, #response_model b"
"   where a.idn = b.Primary_Cardholder_ID"
"     and a.mon_since > (b.now - 6);",

/*Create_Index_Stmt_6*/
"create index i_idn_6 on #fubon_cc_stmts_6(idn);",

/* Execute_Proc_Cal_BucketM_on_Stmt_6 */
"EXEC Cal_BucketM_on_Stmt_6 :v0",

/* Drop_Proc_Cal_BucketM_on_Stmt_6 */
"if exists (select * from dbo.sysobjects where id = object_id(N'[Cal_BucketM_on_Stmt_6]')"
" and OBJECTPROPERTY(id, N'IsProcedure') = 1)"
" drop procedure [Cal_BucketM_on_Stmt_6]",

/*Create_Proc_Cal_BucketM_on_Stmt_6*/
" CREATE PROCEDURE Cal_BucketM_on_Stmt_6"
" (@now int)"
" AS"
" update #fubon_cc_stmts_6"
"    set bucket_M = (case when min_pay = 1 then 1 else 0 end);"
" declare @i int"
" set @i=6"
" while @i >= 0"
"    begin"
"       update #fubon_cc_stmts_6"
"          set bucket_M = (case when #fubon_cc_stmts_6.min_pay = 1 then b.bucket_M + 1 else 0 end)"
"          from #fubon_cc_stmts_6, #fubon_cc_stmts_6 as b"
"          where #fubon_cc_stmts_6.mon_since = b.mon_since + 1"
"            and #fubon_cc_stmts_6.idn = b.idn"
"            and (@now - #fubon_cc_stmts_6.mon_since) = @i"
"       set @i = @i - 1"
"    end;",

/*Insert_Stmt_9*/
" IF OBJECT_ID('tempdb..#fubon_cc_stmts_9') IS NOT NULL"
"    drop table #fubon_cc_stmts_9;"
" select a.*"
"   into #fubon_cc_stmts_9"
"   from #fubon_cc_stmts a, #response_model b"
"   where a.idn = b.Primary_Cardholder_ID"
"     and a.mon_since > (b.now - 9);",

/*Create_Index_Stmt_9*/
"create index i_idn_9 on #fubon_cc_stmts_9(idn);",

/*Update_Indicators*/
"update #response_model"
"   set cashADV_ind = (case when t1.cash_advance_amt > 0 then 1 else 0 end),"
"       revINT_ind = (case when t1.Revolving_Interest_Amt > 0 then 1 else 0 end)"
"   from (select idn,"
"                sum(This_Term_Cash_Advance_Amt) as cash_advance_amt,"
"                sum(Revolving_Interest_Amt) as Revolving_Interest_Amt"
" 	 from #fubon_cc_stmts"
" 	 group by idn) as t1"
"   where #response_model.Primary_Cardholder_ID = t1.idn; "
"update #response_model"
"   set expAMT_ind = (case when t1.Expenditure_Amt > 0 then 1 else 0 end)"
"   from (select idn,"
"        	count(*) as Expenditure_Amt"
" 	 from #fubon_cc_stmts"
"	 where This_Term_Expenditure_Amt > 0"
" 	 group by idn) as t1"
"   where #response_model.Primary_Cardholder_ID = t1.idn; ",

/*Update_Vintage*/
/*:v0 current date yyyymmdd*/
" update #response_model"
"   set Vintage_ind = datediff(month, start_date, convert(datetime, '%s', 112))",

/*Update_Demographics*/
/*v0 current date yyyymmdd*/
"update #response_model"
"   set gender = (case when a.gender_code = 'M' then 1 else 0 end),"
"       age = datediff(year, dob, convert(datetime, '%s', 112)) + 1,"
"       edu = cast(a.education_code as int)"
"   from cc_party_bank_credit_card a"
"   where #response_model.Primary_Cardholder_ID = a.party_ID;",

/*Update_Stmt3_Count*/
"update #response_model"
"   set stmt_3 = a.v1"
"   from (select idn, count(*) as v1"
"         from #fubon_cc_stmts_3"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

/*Update_Stmt6_Count*/
"update #response_model"
"   set stmt_6 = a.v1"
"   from (select idn, count(*) as v1"
"         from #fubon_cc_stmts_6"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

/*Update_Stmt9_Count*/
"update #response_model"
"   set stmt_9 = a.v1"
"   from (select idn, count(*) as v1"
"         from #fubon_cc_stmts_9"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

/*Update_Stmt12_Count*/
"update #response_model"
"   set stmt_12 = a.v1"
"   from (select idn, count(*) as v1"
"         from #fubon_cc_stmts"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

/*Update_WM_Flag*/
"update #response_model"
"   set wm_flag = wm_flag + a.v1"
"   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1"
"         from bu_customer_investment_trust"
"         group by customer_id) as a"
"   where a.customer_id = #response_model.Primary_Cardholder_ID;"
" update #response_model"
"   set wm_flag = wm_flag + a.v1"
"   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1"
"         from bu_customer_futures"
"         group by customer_id) as a"
"   where a.customer_id = #response_model.Primary_Cardholder_ID;"
" update #response_model"
"   set wm_flag = wm_flag + a.v1"
"   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1"
"         from bu_customer_security_brokage"
"         group by customer_id) as a"
"   where a.customer_id = #response_model.Primary_Cardholder_ID;"
" update #response_model"
"   set wm_flag = wm_flag + a.v1"
"   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1"
"         from bu_customer_security_bond"
"         group by customer_id) as a"
"   where a.customer_id = #response_model.Primary_Cardholder_ID;"
" update #response_model"
"   set wm_flag = wm_flag + a.v1"
"   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1"
"         from bu_customer_bank_bond"
"         group by customer_id) as a"
"   where a.customer_id = #response_model.Primary_Cardholder_ID;"
" update #response_model"
"   set wm_flag = wm_flag + a.v1"
"   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1"
"         from bu_customer_bank_fund"
"         group by customer_id) as a"
"   where a.customer_id = #response_model.Primary_Cardholder_ID;",

/*Update_Insurance_Flag*/
" update #response_model"
"   set ins_flag = ins_flag + a.v1"
"   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1"
"         from bu_customer_property_insure"
"         group by customer_id) as a"
"   where a.customer_id = #response_model.Primary_Cardholder_ID;"
" update #response_model"
"   set ins_flag = ins_flag + a.v1"
"   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1"
"         from bu_customer_insurance_group"
"         group by customer_id) as a"
"   where a.customer_id = #response_model.Primary_Cardholder_ID;"
" update #response_model"
"   set ins_flag = ins_flag + a.v1"
"   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1"
"         from bu_customer_insurance_life"
"         group by customer_id) as a"
"   where a.customer_id = #response_model.Primary_Cardholder_ID;",

/*Update_Close_Flag*/
"update #response_model"
"   set close_flag = 1"
"   from #FB_close_acct a"
"   where #response_model.Primary_Cardholder_ID = a.Primary_Cardholder_ID;"
"update #response_model"
"   set wm_flag = (case when wm_flag > 0 then 1 else 0 end),"
"       ins_flag = (case when ins_flag > 0 then 1 else 0 end),"
"       close_flag = (case when close_flag = 1 then 1 else 0 end);",

/*
" 0: C"
" 1: N1"
" 10: N2"
" 2: T1"
" 3: T2"
" 4: R1"
" 40: R2"
*/
/*Update_Card*/
"update #response_model"
"   set card = 'C'"
"   where cashADV_ind = 1;"
"update #response_model"
"   set card = (case when Vintage_ind < 12 then 'R2' else 'R1' end)"
"   where cashADV_ind = 0"
"     and revINT_ind = 1;"
"update #response_model"
"   set card = (case when Vintage_ind < 12 then 'T2' else 'T1' end)"
"   where cashADV_ind = 0"
"     and revINT_ind = 0"
"     and expAMT_ind = 1;"
"update #response_model"
"   set card = (case when wm_flag = 1 and ins_flag =0 then 'N2' else 'N1' end)"
"   where cashADV_ind = 0"
"     and revINT_ind = 0"
"     and expAMT_ind = 0;"
,

// FS003  連續X個月僅繳最低金額
/*Cal_FS003*/
" update #response_model"
"   set fs003_6 = a.v1"
"   from (select idn, max(bucket_M) as v1"
"         from #fubon_cc_stmts_6"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

// FS072  餘額減少>=5%
/*Cal_FS072*/
"update #response_model"
"   set fs072_9 = a.v1"
"   from (select idn, count(*) as v1"
"         from #fubon_cc_stmts_9"
"         where -bal_pct_diff >= 0.05"
"           and bal_dec = 1"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

// FS089  餘額使用率>=80%
/*Cal_FS089*/
"update #response_model"
"   set fs089_3 = a.v1"
"   from (select idn, count(*) as v1"
"         from #fubon_cc_stmts_3"
"         where convert(float, this_term_total_amt_receivable) /"
"              (case when Monthly_Limit_Amt = 0 then NULL else Monthly_Limit_Amt end) >= 0.8"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

// FS096  循環利息>=4K
/*Cal_FS096*/
"update #response_model"
"   set fs096_12 = a.v1"
"   from (select idn, count(*) as v1"
"         from #fubon_cc_stmts"
"         where revolving_interest_amt >= 4000"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

//-- MS035	average utilization ratio (balance /monthly limit)
/*Cal_MS035*/
"update #response_model"
"   set ms035_12 = a.v1"
"   from (select idn, avg(convert(float, this_term_total_amt_receivable) / (case when monthly_limit_amt = 0 then null else monthly_limit_amt end)) as v1"
"         from #fubon_cc_stmts"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

//-- MS062	revolving interest amount previous / current
/*Cal_MS062*/
"update #response_model"
"   set ms062_12 = a.v1"
"   from (select idn, max(convert(float, revolving_interest_amt)/(case when last_revolving_int_amt = 0 then NULL else last_revolving_int_amt end)) as v1"
"         from #fubon_cc_stmts"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

//-- RS001	最近一次開卡距
/*Cal_RS001*/
/*:v0 now 94*12+6*/
"update #response_model"
"   set rs001 = :v0 -"
"               ((cast(substring(t1.recent_open_date, 1, 4) as int)-1911) * 12 +"
"               (cast(substring(t1.recent_open_date, 5, 2) as int)))"
"   from (select primary_cardholder_id,"
"                left(convert(char(8), max(Card_Issue_Date), 112), 6) as recent_open_date"
"         from cc_acct_credit_card"
"         group by primary_cardholder_id) as t1"
"   where t1.primary_cardholder_id = #response_model.primary_cardholder_id;"
,

/*Transform_R1_Vars*/
" update #response_model"
"    set FS089_3_n=FS089_3 / (case when stmt_3 = 0 then null else stmt_3 end),"
"        rs001_r = power ((case when rs001 < 0 then null else rs001 end), 0.5),"
"        MS062_12_r = power ((case when MS062_12 < 0 then null else MS062_12 end), 0.5),"
"        FS096_12_n = FS096_12 / (case when stmt_12 = 0 then null else stmt_12 end),"
"        FS072_9_n = FS072_9 / (case when stmt_9 = 0 then null else stmt_9 end),"
"        FS003_6_n = FS003_6 / (case when stmt_6 = 0 then null else stmt_6 end),"
"        age_t_r1 = (case when age < 30 then 1"
"        		 when age < 40 then 2"
"        		 when age < 50 then 3"
"                         else  4 end);"
" update #response_model"
"    set ms035_12_t_r1 = (case when ms035_12 > 0.84 then 0.84"
"	                      else ms035_12 end),"
"        rs001_r_t_r1 = (case when rs001_r < 3 then 3"
"        		     when rs001_r > 8.6 then 8.6"
"	                     else  rs001_r end),"
"        FS089_3_n_q = FS089_3_n * FS089_3_n,"
"        MS062_12_r_t_r1 = (case when MS062_12_r is null then 0"
"        		     when MS062_12_r > 1.2 then 1.2"
"	                     else MS062_12_r end),"
"	FS096_12_n_t_r1 = (case when FS096_12_n > 0 then 1"
"	                      else 0 end),"
"        FS072_9_n_t_r1 = (case when FS072_9_n is null then 0.44"
"        		     when FS072_9_n > 0.66 then 0.66"
"	                     else FS072_9_n end),"
"        FS003_6_n_r = power ((case when FS003_6_n < 0 then null else FS003_6_n end), 0.5);"
" update #response_model"
"    set FS089_3_n_q_t_r1 = (case when FS089_3_n_q is null then 0"
"	                      else FS089_3_n_q end),"
"        FS003_6_n_r_t_r1 = (case when FS003_6_n_r is null then 0"
"	                      else FS003_6_n_r end);",

/*Cal_R1_Score_Twentile*/
" update #response_model"
"    set rscore_r1 =		0.02929	+"
"        ms035_12_t_r1	 *	0.12817	+"
"        age_t_r1	 *   	0.0201	+"
"        FS089_3_n_q_t_r1 *	0.05321	+"
"        rs001_r_t_r1	 *     -0.00822	+"
"        MS062_12_r_t_r1	 *	0.0335	+"
"        FS096_12_n_t_r1	 *	0.07503	+"
"        FS072_9_n_t_r1	 *     -0.04747	+"
"        FS003_6_n_r_t_r1 *	0.02337"
"     where card = 'R1';"
" update #response_model"
"    set twentile_r1 = (case when rscore_r1 is null then 0"
"                            when rscore_r1 <= -0.00459 then 1"
"                            when rscore_r1 <= 0.00603 then 2"
"                            when rscore_r1 <= 0.01403 then 3"
"                            when rscore_r1 <= 0.02107 then 4"
"                            when rscore_r1 <= 0.02732 then 5"
"                            when rscore_r1 <= 0.03314 then 6"
"                            when rscore_r1 <= 0.03947 then 7"
"                            when rscore_r1 <= 0.04602 then 8"
"                            when rscore_r1 <= 0.05321 then 9"
"                            when rscore_r1 <= 0.06038 then 10"
"                            when rscore_r1 <= 0.06976 then 11"
"                            when rscore_r1 <= 0.08236 then 12"
"                            when rscore_r1 <= 0.10001 then 13"
"                            when rscore_r1 <= 0.12253 then 14"
"                            when rscore_r1 <= 0.1454  then 15"
"                            when rscore_r1 <= 0.17111 then 16"
"                            when rscore_r1 <= 0.19378 then 17"
"                            when rscore_r1 <= 0.21221 then 18"
"                            when rscore_r1 <= 0.23687 then 19"
"                            else 20 end)"
"     where card = 'R1';",

//-- FS191  消費款減少>=20%
/*Cal_FS191*/
"update #response_model"
"   set fs191_12 = a.v1"
"   from (select idn, count(*) as v1"
"         from #fubon_cc_stmts"
"         where -exp_pct_diff >= 0.2"
"           and exp_dec = 1"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

//-- FS204  普卡卡
/*Cal_FS204*/
"update #response_model"
"   set fs204 = t1.cnt"
"   from (select a.primary_cardholder_id, count(*) as cnt"
"         from cc_acct_credit_card_100k a,"
"              rd_card_type_code b,"
"              rd_credit_card_product_id c"
"         where a.card_type_code = b.code"
"           and a.product_id = c.code"
"           and not (b.[description] like '%白%' or c.[description] like '%白金%')"
"           and not (b.[description] like '%金%' or c.[description] like '%金%')"
"         group by primary_cardholder_id) as t1"
"    where t1.primary_cardholder_id = #response_model.Primary_Cardholder_ID;",

/*Transform_T1_Vars*/
" update #response_model"
"    set FS191_12_n=FS191_12 / (case when stmt_12 =0 then null else stmt_12 end),"
"        rs001_t_t1 = (case when rs001 < 12  then 12"
"    			   when rs001 > 40  then 40"
"	                   else rs001 end),"
"        FS204_t_t1 = (case when FS204 > 2 then 2"
"	                      else FS204 end),"
"        edu_t_t1 = (case when edu in (1, 2) then 1"
"	                 else 2 end);"
" update #response_model"
"    set FS191_12_n_r = power ((case when FS191_12_n < 0 then null else FS191_12_n end), 0.5);"
" update #response_model"
"    set fs191_12_n_r_t_t1 = (case when fs191_12_n_r > 0  then 1"
"	                          else 0 end);",

/*Cal_T1_Score_Twentile*/
" update #response_model"
"    set rscore_t1 =		0.04151	+"
"        rs001_t_t1	  *    -0.00238	+"
"        fs191_12_n_r_t_t1 *    -0.08967	+"
"        FS204_t_t1	  *	0.02252	+"
"        AGE		  *	0.00173	+"
"        edu_t_t1	  *	0.0525"
"     where card = 'T1';"
" update #response_model"
"    set twentile_t1 = (case when rscore_t1 is null then 0"
"                            when rscore_t1 <= 0.02284 then 1"
"                            when rscore_t1 <= 0.03128 then 2"
"                            when rscore_t1 <= 0.03733 then 3"
"                            when rscore_t1 <= 0.04231 then 4"
"                            when rscore_t1 <= 0.04771 then 5"
"                            when rscore_t1 <= 0.05226 then 6"
"                            when rscore_t1 <= 0.05639 then 7"
"                            when rscore_t1 <= 0.06053 then 8"
"                            when rscore_t1 <= 0.06549 then 9"
"                            when rscore_t1 <= 0.07069 then 10"
"                            when rscore_t1 <= 0.07587 then 11"
"                            when rscore_t1 <= 0.08061 then 12"
"                            when rscore_t1 <= 0.08537 then 13"
"                            when rscore_t1 <= 0.09082 then 14"
"                            when rscore_t1 <= 0.09704 then 15"
"                            when rscore_t1 <= 0.1027  then 16"
"                            when rscore_t1 <= 0.10983 then 17"
"                            when rscore_t1 <= 0.12002 then 18"
"                            when rscore_t1 <= 0.13365 then 19"
"                            else 20 end)"
"     where card = 'T1';",

//-- MS023	average open to buy
/*Cal_MS023*/
"update #response_model"
"   set ms023_12 = a.v1"
"   from (select idn, avg(cast((monthly_limit_amt - this_term_total_amt_receivable) as float)) as v1"
"         from #fubon_cc_stmts"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

//-- FS197  消費款減少>=50%
/*Cal_FS197*/
"update #response_model"
"   set fs197_9 = a.v1"
"   from (select idn, count(*) as v1"
"         from #fubon_cc_stmts_9"
"         where -exp_pct_diff >= 0.5"
"           and exp_dec = 1"
"         group by idn) as a"
"   where a.idn = #response_model.Primary_Cardholder_ID;",

/*Transform_T2_Vars*/
" update #response_model"
"    set ms023_12_1k = ms023_12 / 1000.0,"
"        FS197_9_n = FS197_9 / (case when stmt_9 = 0 then null else stmt_9 end);"
" update #response_model"
"    set ms023_12_1k_t2 = (case when ms023_12_1k < 20 then 20"
"			       when ms023_12_1k > 200 then 200"
"	                       else ms023_12_1k end),"
"        FS197_9_n_t_t2 = (case when FS197_9_n < 0.12 then 0.12"
"			       when FS197_9_n > 0.33 then 0.33"
"	                       else FS197_9_n end);",

/*Cal_T2_Score_Twentile*/
" update #response_model"
"    set rscore_t2 =		0.08526	+"
"        ms023_12_1k_t2	*   -0.00096186	+"
"        age		*	0.00542	+"
"        FS197_9_n_t_t2	*	-0.3683"
"     where card = 'T2';"
" update #response_model"
"    set twentile_t2 = (case when rscore_t2 is null then 0"
"                            when rscore_t2 <= -0.03093 then 1"
"                            when rscore_t2 <= -0.00919 then 2"
"                            when rscore_t2 <= 0.00893  then 3"
"                            when rscore_t2 <= 0.01866  then 4"
"                            when rscore_t2 <= 0.02823  then 5"
"                            when rscore_t2 <= 0.04155  then 6"
"                            when rscore_t2 <= 0.05283  then 7"
"                            when rscore_t2 <= 0.06393  then 8"
"                            when rscore_t2 <= 0.07324  then 9"
"                            when rscore_t2 <= 0.08468  then 10"
"                            when rscore_t2 <= 0.09477  then 11"
"                            when rscore_t2 <= 0.10396  then 12"
"                            when rscore_t2 <= 0.11232  then 13"
"                            when rscore_t2 <= 0.12383  then 14"
"                            when rscore_t2 <= 0.13208  then 15"
"                            when rscore_t2 <= 0.1437   then 16"
"                            when rscore_t2 <= 0.15823  then 17"
"                            when rscore_t2 <= 0.17981  then 18"
"                            when rscore_t2 <= 0.21463  then 19"
"                            else 20 end)"
"     where card = 'T2';",

/*Transform_N1_Vars*/
"update #response_model"
"    set age_t_n1 = (case when age < 24  then 24"
"    			 when age > 32  then 32"
"	                 else age end),"
"        FS204_t_n1 = (case when FS204 > 4 then 4"
"	                      else FS204 end),"
"	 index7 = (1 - wm_flag) * (1 - ins_flag) * close_flag,"
"        edu_t_n1 = (case when edu = 1 then 1"
"                         when edu = 2 then 1"
"                         when edu = 3 then 2"
"	                 else 3 end);",

/*Cal_N1_Score_Twentile*/
" update #response_model"
"    set rscore_n1 =	  0.25173  +"
"        edu_t_n1	* 0.06283  +"
"        age_t_n1	* -0.01055 +"
"        fs204_t_n1	* 0.01605  +"
"        gender		* 0.02327  +"
"        index7		* -0.01884"
"     where card = 'N1';"
" update #response_model"
"    set twentile_n1 = (case when rscore_n1 is null then 0"
"                            when rscore_n1 <= 0.02095 then 1"
"                            when rscore_n1 <= 0.03964 then 2"
"                            when rscore_n1 <= 0.04205 then 3"
"                            when rscore_n1 <= 0.05477 then 4"
"                            when rscore_n1 <= 0.0581  then 5"
"                            when rscore_n1 <= 0.06181 then 6"
"                            when rscore_n1 <= 0.06639 then 7"
"                            when rscore_n1 <= 0.07694 then 8"
"                            when rscore_n1 <= 0.08378 then 9"
"                            when rscore_n1 <= 0.09525 then 10"
"                            when rscore_n1 <= 0.10247 then 11"
"                            when rscore_n1 <= 0.10625 then 12"
"                            when rscore_n1 <= 0.11588 then 13"
"                            when rscore_n1 <= 0.11867 then 14"
"                            when rscore_n1 <= 0.12509 then 15"
"                            when rscore_n1 <= 0.13457 then 16"
"                            when rscore_n1 <= 0.14069 then 17"
"                            when rscore_n1 <= 0.15567 then 18"
"                            when rscore_n1 <= 0.17368 then 19"
"                            else 20 end)"
"     where card = 'N1';",

/*Generate_Output_Table*/
" if  NOT exists (select * from dbo.sysobjects where id = object_id(N'[Fubon_response_score]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)"
"   begin"
"      create table Fubon_response_score ("
"	Primary_Cardholder_ID	char(11) not null,"
"	segment		varchar(20),"
"	twentile    	int,"
"	score		float"
"      );"
"     alter table Fubon_response_score add constraint p_id primary key (Primary_Cardholder_ID);"
"   end"
" delete from Fubon_response_score;"
" insert into Fubon_response_score(Primary_Cardholder_ID, segment, twentile, score)"
"    select Primary_Cardholder_ID, '循環信用', twentile_r1, rscore_r1"
"    from #response_model"
"    where card='R1';"
" insert into Fubon_response_score(Primary_Cardholder_ID, segment, twentile, score)"
"    select Primary_Cardholder_ID, '循環信用', NULL, NULL"
"    from #response_model"
"    where card='R2';"
" insert into Fubon_response_score(Primary_Cardholder_ID, segment, twentile, score)"
"    select Primary_Cardholder_ID, '循環信用', 21, NULL"
"    from #response_model"
"    where card='C';"
" insert into Fubon_response_score(Primary_Cardholder_ID, segment, twentile, score)"
"    select Primary_Cardholder_ID, '刷卡消費', twentile_t1, rscore_t1"
"    from #response_model"
"    where card='T1';"
" insert into Fubon_response_score(Primary_Cardholder_ID, segment, twentile, score)"
"    select Primary_Cardholder_ID, '刷卡消費', NULL, NULL"
"    from #response_model"
"    where card='T2';"
" insert into Fubon_response_score(Primary_Cardholder_ID, segment, twentile, score)"
"    select Primary_Cardholder_ID, '關戶/不動', twentile_n1, rscore_n1"
"    from #response_model"
"    where card='N1';"
" insert into Fubon_response_score(Primary_Cardholder_ID, segment, twentile, score)"
"    select Primary_Cardholder_ID, '關戶/不動', NULL, NULL"
"    from #response_model"
"    where card='N2';",

/*Duplicate_Working_Table*/
" if exists (select * from dbo.sysobjects where id = object_id(N'[Fubon_response_model]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)"
"    drop table Fubon_response_model;"
" select *"
" into Fubon_response_model"
" from #response_model;",

/*Generate_Summary*/
" select segment, twentile, count(*) as group_count from Fubon_response_score "
" group by segment, twentile order by segment, twentile; "
 };

#endif
