//---------------------------------------------------------------------------
#pragma hdrstop

#include "BSautoSQL.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

char *SQLCommands[] = {
/* Clean_Temp_Tables */
" IF NOT exists (select * from dbo.sysobjects where id = object_id(N'[account_tmp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)"
"   BEGIN"
"     CREATE TABLE [account_tmp] ("
"     	[Customer ID] [char] (11) NOT NULL,"
"     	[Account ID] [char] (16) NOT NULL,"
"     	[Account Type] [char] (1) NOT NULL,"
"     	[Account Open Date] [float] NOT NULL,"
"     	[Account Close Date] [float] NULL,"
"     	[Cycle Date] [float] NOT NULL,"
"     	[Secured Amount] [int] NOT NULL,"
"     	[Channel] [char] (3) NOT NULL,"
"     	[# of payment terms] [int] NULL,"
"     	[Initial Loan Amount] [int] NULL,"
"     	[Setup Fee] [int] NULL,"
"     	[Maintenance Fee] [int] NOT NULL,"
"     	[Maintenance Fee Term] [int] NULL,"
"     	[Interest Rate] [float] NOT NULL,"
"     	[Birthday] [float] NOT NULL,"
"     	[Resident Phone] [char] (14) NOT NULL,"
"     	[Residence City / Province] [char] (14) NOT NULL,"
"     	[Residence ZIP] [char] (5) NOT NULL,"
"     	[GENDer] [char] (1) NOT NULL,"
"     	[Annual Income] [int] NULL,"
"     	[Home Ownership] [char] (1) NULL,"
"     	[Educational Level] [char] (1) NULL,"
"     	[Marital status] [char] (1) NULL,"
"     	[Occupation Status] [char] (1) NULL,"
"     	[# months in current residence] [int] NULL,"
"     	[# months with current occupation] [int] NULL"
"     )"
"   END"
" ELSE"
"   delete from account_tmp"
" "
" if  NOT exists (select * from dbo.sysobjects where id = object_id(N'[bcauto_log]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)"
"   BEGIN"
"     CREATE TABLE [bcauto_log] ("
"     	[cycle_date] [char] (8),"
"     	[start_time] [char] (20),"
"     	[end_time] [char] (20),"
"     	[return_code] int,"
"     	[status] [varchar] (80)"
"     )"
"   END"
" "
" if  NOT exists (select * from dbo.sysobjects where id = object_id(N'[statement_tmp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)"
"   BEGIN"
"     CREATE TABLE [statement_tmp] ("
"     	[Customer ID] [char] (11) NOT NULL,"
"     	[Account ID] [char] (16) NOT NULL,"
"     	[Statement month] [char] (6) NOT NULL,"
"     	[Account Type] [char] (1) NULL,"
"     	[Total # open cards with Fuhwa] [int] NULL,"
"     	[Account Status] [char] (1) NULL,"
"     	[Inactive date for status 6] [int] NULL,"
"     	[APR] [float] NULL,"
"     	[Credit Limit] [int] NULL,"
"     	[Cash Credit Limit] [int] NULL,"
"     	[Purchase Average Daily Balance] [float] NULL,"
"     	[Cash Average Daily Balance] [float] NULL,"
"     	[Purchase balance] [int] NULL,"
"     	[Cash balance] [int] NULL,"
"     	[Purchase amount] [int] NULL,"
"     	[Purchase count] [int] NULL,"
"     	[Purchase Credited Amount] [int] NULL,"
"     	[Cash amount] [int] NULL,"
"     	[Cash Advance count] [int] NULL,"
"     	[Cash Credited Amount] [int] NULL,"
"     	[other x-sell purchase amount] [int] NULL,"
"     	[other x-sell purchase count] [int] NULL,"
"     	[other x-sell purchase Credited amount] [int] NULL,"
"     	[Offshore purchase amount] [int] NULL,"
"     	[Offshore purchase count] [int] NULL,"
"     	[Offshore purchase Credited amount] [int] NULL,"
"     	[Offshore Cash amount] [int] NULL,"
"     	[Offshore Cash count] [int] NULL,"
"     	[Offshore Cash Credited amount] [int] NULL,"
"     	[Interest charge] [int] NULL,"
"     	[Interest charge credited] [int] NULL,"
"     	[Cash Advance Fee Assessed] [int] NULL,"
"     	[Cash Advance Fee Credited] [int] NULL,"
"     	[Annual Fee Assessed] [int] NULL,"
"     	[Annual Fee Credited] [int] NULL,"
"     	[Late Fee Assessed] [int] NULL,"
"     	[Late Fee Credited] [int] NULL,"
"     	[Other Fee Assessed] [int] NULL,"
"     	[Other Fee Credited] [int] NULL,"
"     	[Other Fee count] [int] NULL,"
"     	[Fraud Credit Amount] [int] NULL,"
"     	[Fraud Cash Credit Amount] [int] NULL,"
"     	[Bonus Points claimed] [int] NULL,"
"     	[Unclaimed Bonus Points balance] [int] NULL,"
"     	[Rebate (cash)] [int] NULL,"
"     	[Payment due date] [int] NULL,"
"     	[Payment amount] [int] NULL,"
"     	[Payment count] [int] NULL,"
"     	[Last payment date] [int] NULL,"
"     	[Minimum Payment] [int] NULL,"
"     	[# cycles Past Due] [int] NULL,"
"     	[Total bal on bill] [int] NULL"
"     )"
"   END"
" else"
"   delete from statement_tmp",

/* Bulk_Insert_Data */
" bulk insert statement_tmp"
" from '%s\\%s'"
" with (firstrow = 2, formatfile='%s\\statement.fmt', keepnulls); "
" bulk insert account_tmp"
" from '%s\\%s'"
" with (firstrow = 2, formatfile='%s\\account.fmt', keepnulls)",

/* Check_Statement_Loaded */
" select count(*) as load_count from statement_tmp",

/* Check_Account_Loaded */
" select count(*) as load_count from account_tmp",

/* DROP_PROCEDURE_Load_to_Statement */
"if exists (select * from dbo.sysobjects where id = object_id(N'[Load_to_Statement]') and"
" OBJECTPROPERTY(id, N'IsProcedure') = 1)"
" drop procedure Load_to_Statement;",

/* CREATE_PROCEDURE_Load_to_Statement */
" CREATE PROCEDURE Load_to_Statement"
"  (@cycle_date char(8),"
"   @row_affected int output"
"  )"
" AS"
"   declare @target_month varchar(6)"
"   declare @yyyymm int"
"   declare @yyyy int"
"   declare @mm int"
"   declare @dd int"
"   set @yyyymm = cast (substring(@cycle_date, 1, 6) as int)"
"   set @yyyy = cast (substring(@cycle_date, 1, 4) as int)"
"   set @mm = @yyyymm % 100"
"   set @dd = cast (@cycle_date as int) % 100"
"   if @dd = 1"
"    begin"
"      if @mm <= 1"
"         begin"
"            set @mm = @mm - 1 + 12"
"            set @yyyy = @yyyy - 1"
"         end"
"      else"
"         set @mm = @mm - 1"
"    end"
"   set @target_month = cast ((@yyyy * 100 + @mm) as char(6))"
" "
"   delete from statement"
"   where [statement month] = @target_month"
"     and [customer id] in (select [customer id] from statement_tmp)"
"  insert into statement([Customer ID], [Account ID], [Statement month], [Account Type],"
"   [Total # open cards with Fuhwa], [Account Status], [Inactive date for status 6], [APR],"
"   [Credit Limit], [Cash Credit Limit], [Purchase Average Daily Balance], [Cash Average Daily Balance],"
"   [Purchase balance], [Cash balance], [Purchase amount], [Purchase count], [Purchase Credited Amount],"
"   [Cash amount], [Cash Advance count], [Cash Credited Amount], [other x-sell purchase amount],"
"   [other x-sell purchase count], [other x-sell purchase Credited amount],"
"   [Offshore purchase amount], [Offshore purchase count], [Offshore purchase Credited amount],"
"   [Offshore Cash amount], [Offshore Cash count], [Offshore Cash Credited amount],"
"   [Interest charge], [Interest charge credited], [Cash Advance Fee Assessed],"
"   [Cash Advance Fee Credited], [Annual Fee Assessed], [Annual Fee Credited],"
"   [Late Fee Assessed], [Late Fee Credited], [Other Fee Assessed], [Other Fee Credited],"
"   [Other Fee count], [Fraud Credit Amount], [Fraud Cash Credit Amount], [Bonus Points claimed],"
"   [Unclaimed Bonus Points balance], [Rebate (cash)], [Payment due date], [Payment amount],"
"   [Payment count], [Last payment date], [Minimum Payment], [# cycles Past Due], [Total bal on bill])"
" select [Customer ID], [Account ID], [Statement month], [Account Type],"
"   [Total # open cards with Fuhwa], [Account Status], [Inactive date for status 6], [APR],"
"   [Credit Limit], [Cash Credit Limit], [Purchase Average Daily Balance], [Cash Average Daily Balance],"
"   [Purchase balance], [Cash balance], [Purchase amount], [Purchase count], [Purchase Credited Amount],"
"   [Cash amount], [Cash Advance count], [Cash Credited Amount], [other x-sell purchase amount],"
"   [other x-sell purchase count], [other x-sell purchase Credited amount],"
"   [Offshore purchase amount], [Offshore purchase count], [Offshore purchase Credited amount],"
"   [Offshore Cash amount], [Offshore Cash count], [Offshore Cash Credited amount],"
"   [Interest charge], [Interest charge credited], [Cash Advance Fee Assessed],"
"   [Cash Advance Fee Credited], [Annual Fee Assessed], [Annual Fee Credited],"
"   [Late Fee Assessed], [Late Fee Credited], [Other Fee Assessed], [Other Fee Credited],"
"   [Other Fee count], [Fraud Credit Amount], [Fraud Cash Credit Amount], [Bonus Points claimed],"
"   [Unclaimed Bonus Points balance], [Rebate (cash)], [Payment due date], [Payment amount],"
"   [Payment count], [Last payment date], [Minimum Payment], [# cycles Past Due], [Total bal on bill]"
" from statement_tmp;"
" set @row_affected = @@ROWCOUNT",

/* EXEC_PROCEDURE_Load_to_Statement */
" declare @row_affected int"
" EXEC Load_to_Statement '%s', @row_affected OUTPUT"
" select @row_affected as row_affected into #statement_load_rows",

/* DROP_PROCEDURE_Load_to_Account */
"if exists (select * from dbo.sysobjects where id = object_id(N'[Load_to_Account]') and"
" OBJECTPROPERTY(id, N'IsProcedure') = 1)"
" drop procedure Load_to_Account;",

/* CREATE_PROCEDURE_Load_to_Account */
"CREATE PROCEDURE Load_to_Account"
"( @row_affected int output"
")"
"AS"
" delete from account;"
" insert into account([Customer ID], [Account ID], [Account Type], [Account Open Date], [Account Close Date],"
"  [Cycle Date], [Secured Amount], [Channel], [# of payment terms], [Initial Loan Amount], [Setup Fee],"
"  [Maintenance Fee], [Maintenance Fee Term], [Interest Rate], [Birthday], [Resident Phone],"
"  [Residence City / Province], [Residence ZIP], [GENDer], [Annual Income], [Home Ownership], [Educational Level],"
"  [Marital status], [Occupation Status], [# months in current residence], [# months with current occupation])"
"  select [Customer ID], [Account ID], [Account Type], [Account Open Date], [Account Close Date],"
"   [Cycle Date], [Secured Amount], [Channel], [# of payment terms], [Initial Loan Amount], [Setup Fee],"
"   [Maintenance Fee],[Maintenance Fee Term], [Interest Rate], [Birthday], [Resident Phone],"
"   [Residence City / Province],     [Residence ZIP], [GENDer], [Annual Income], [Home Ownership], [Educational Level],"
"   [Marital status], [Occupation Status], [# months in current residence], [# months with current occupation]"
"  from account_tmp"
" set @row_affected = @@ROWCOUNT",

/* EXEC_PROCEDURE_Load_to_Account */
" declare @row_affected int"
" EXEC Load_to_account @row_affected OUTPUT"
" select @row_affected as row_affected into #account_load_rows",

/* Check_Production_Statement_Loaded */
" select row_affected from #statement_load_rows",

/* Check_Production_Account_Loaded */
" select row_affected from #account_load_rows",

/* Write_Log */
" insert into bcauto_log(cycle_date, start_time, end_time, return_code, status) "
"   values (:v0, :v1, :v2, :v3, :v4)",

/*SYSTEM_Exec_Advscore*/
"advscore -m %s  -u %s -p %s -s %s -d %s ",

/*SYSTEM_Exec_Bcp_PD*/
"bcp \"select * from %s..credit_card_monthly_profile_riskgroup where cycle_date='%s'\" "
" queryout \"%s\\PD_%s_%s.csv\" -c -t , -U %s -P %s -S %s",

/*SYSTEM_Exec_Bcp_Profile*/
"bcp \"select * from %s..credit_card_monthly_profile_riskgroup where cycle_date='%s'\" "
"queryout \"%s\\PROFILE_%s_%s.csv\" -c -t , -U %s -P %s -S %s"

};
