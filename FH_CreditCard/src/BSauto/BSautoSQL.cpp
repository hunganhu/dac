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
"     	[# of payment terms] [int],"
"     	[Initial Loan Amount] [int],"
"     	[Setup Fee] [int],"
"     	[Maintenance Fee] [int] NOT NULL,"
"     	[Maintenance Fee Term] [int],"
"     	[Interest Rate] [float] NOT NULL,"
"     	[Birthday] [float] NOT NULL,"
"     	[Resident Phone] [char] (14) NOT NULL,"
"     	[Residence City / Province] [char] (14) NOT NULL,"
"     	[Residence ZIP] [char] (5) NOT NULL,"
"     	[GENDer] [char] (1) NOT NULL,"
"     	[Annual Income] [int],"
"     	[Home Ownership] [char] (1),"
"     	[Educational Level] [char] (1),"
"     	[Marital status] [char] (1),"
"     	[Occupation Status] [char] (1),"
"     	[# months in current residence] [int],"
"     	[# months with current occupation] [int]"
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
"     	[Account Type] [char] (1),"
"     	[Total # open cards with Fuhwa] [int],"
"     	[Account Status] [char] (1),"
"     	[Inactive date for status 6] [int],"
"     	[APR] [float],"
"     	[Credit Limit] [int],"
"     	[Cash Credit Limit] [int],"
"     	[Purchase Average Daily Balance] [float],"
"     	[Cash Average Daily Balance] [float],"
"     	[Purchase balance] [int],"
"     	[Cash balance] [int],"
"     	[Purchase amount] [int],"
"     	[Purchase count] [int],"
"     	[Purchase Credited Amount] [int],"
"     	[Cash amount] [int],"
"     	[Cash Advance count] [int],"
"     	[Cash Credited Amount] [int],"
"     	[other x-sell purchase amount] [int],"
"     	[other x-sell purchase count] [int],"
"     	[other x-sell purchase Credited amount] [int],"
"     	[Offshore purchase amount] [int],"
"     	[Offshore purchase count] [int],"
"     	[Offshore purchase Credited amount] [int],"
"     	[Offshore Cash amount] [int],"
"     	[Offshore Cash count] [int],"
"     	[Offshore Cash Credited amount] [int],"
"     	[Interest charge] [int],"
"     	[Interest charge credited] [int],"
"     	[Cash Advance Fee Assessed] [int],"
"     	[Cash Advance Fee Credited] [int],"
"     	[Annual Fee Assessed] [int],"
"     	[Annual Fee Credited] [int],"
"     	[Late Fee Assessed] [int],"
"     	[Late Fee Credited] [int],"
"     	[Other Fee Assessed] [int],"
"     	[Other Fee Credited] [int],"
"     	[Other Fee count] [int],"
"     	[Fraud Credit Amount] [int],"
"     	[Fraud Cash Credit Amount] [int],"
"     	[Bonus Points claimed] [int],"
"     	[Unclaimed Bonus Points balance] [int],"
"     	[Rebate (cash)] [int],"
"     	[Payment due date] [int],"
"     	[Payment amount] [int],"
"     	[Payment count] [int],"
"     	[Last payment date] [int],"
"     	[Minimum Payment] [int],"
"     	[# cycles Past Due] [int],"
"     	[Total bal on bill] [int]"
"     )"
"   END"
" else"
"   delete from statement_tmp",

/* Bulk_Insert_Data_STMT */
" bulk insert statement_tmp"
" from '%s\\%s'"
" with (firstrow = 2, formatfile='%s\\statement.fmt', keepnulls); ",
/* Bulk_Insert_Data_ACCT*/
" bulk insert account_tmp"
" from '%s\\%s'"
" with (firstrow = 2, formatfile='%s\\account.fmt', keepnulls)",

/* Check_Statement_Loaded */
" select count(*) as load_count from statement_tmp",

/* Check_Account_Loaded */
" select count(*) as load_count from account_tmp",

/* Check_Cycledate_Loaded */
" select count(*) as load_count from account_tmp where [cycle date] = :v0",

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
"   select [customer id] into #tmp_stmt from statement_tmp;"
"   create index i_custid on #tmp_stmt([customer id]);"
"   delete from statement"
"   where [statement month] = @target_month"
"     and [customer id] in (select [customer id] from #tmp_stmt);"
" insert into statement select * from statement_tmp; "
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

/*SYSTEM_Rename_Control_File*/
"ren %s\\FHBSAUTO.CTL FHBSAUTO_%s.CTL ",

/*SYSTEM_Exec_Advscore*/
"advscore -m %s  -u %s -p %s -s %s -d %s ",

/*SYSTEM_Exec_Bcp_Statement*/
/* the option -q is required because the field name of table has blank.
   So we have to set quoted_identifier on*/
"bcp %s..statement_tmp in %s\\%s -F 2 -f %s\\statement.fmt -q -U %s -P %s -S %s ",

/*SYSTEM_Exec_Bcp_Account*/
/* the option -q is required because the field name of table has blank.
   So we have to set quoted_identifier on*/
"bcp %s..account_tmp in %s\\%s -F 2 -f %s\\account.fmt -q -U %s -P %s -S %s ",

/*SYSTEM_Exec_Bcp_PD*/
"bcp \"select * from %s..credit_card_monthly_pd_riskgroup where cycle_date='%s'\" "
" queryout \"%s\\PD_%s_%s.csv\" -c -t , -U %s -P %s -S %s",

/*SYSTEM_Exec_Bcp_Profile*/
"bcp \"select * from %s..credit_card_monthly_profile_riskgroup where cycle_date='%s'\" "
"queryout \"%s\\PROFILE_%s_%s.csv\" -c -t , -U %s -P %s -S %s"

};
