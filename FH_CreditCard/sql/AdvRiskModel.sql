create view statement as
select * from jeff.model_all_stmt

create view account as 
select * from dbo.Acct_200405

/*
200205	95675
200206	96693
200207	98395
200208	99609
200209	100518
200210	101334
200211	118010
200212	131705
200301	146922
200302	162386
200303	174738
200304	185644
200305	196519
200306	206650
200307	221833
200405	328014

source
200205	95675
200206	96693
200207	98395
200208	99609
200209	100518
200210	101334
200211	118010
200212	131705
200301	146922
200302	162386
200303	174738
200304	185644
200305	196519
200306	206650
200307	221833
200308	249341
200309	269168
200310	286453
200311	297991
200312	304006
200401	313796
200402	317612
200403	321120
200404	324658
200405	328014

*/
select statement_month, count(*)
from statement
group by statement_month
order by statement_month

select count(*)
from dbo.Stm_200205

insert into statement
select * from dbo.Stm_200205


/*  01- Create_Input_Table */
 CREATE TABLE [source] (
        [Statement_month] [varchar] (8) NULL,
        [Account_Status] [varchar] (3) NULL,
        [Inactive_date_for_status_6] [float] NULL,
        [Credit_Limit] [float] NULL,
        [Purchase_Average_Daily_Balance] [float] NULL,
        [Cash_Average_Daily_Balance] [float] NULL,
        [Purchase_amount] [float] NULL,
        [Purchase_count] [float] NULL,
        [Cash_amount] [float] NULL,
        [Cash_Advance_count] [float] NULL,
        [Interest_charge] [float] NULL,
        [Late_Fee_Assessed] [float] NULL,
        [Payment_Amount] [float] NULL,
        [Minimum_Payment] [float] NULL,
        [__cycles_Past_Due] [float] NULL,
        [Total_bal_on_bill] [float] NULL,
        [PayCode] [char](1) NULL,
        [flag] [int] NULL,
        [month_since] [int] NULL,
        [age] [int] NULL,
        [idn] [float] NULL      	-- In DAC env.
--        [idn] [char] (11) NULL	-- In Fuhwa env.
);

/* 02- Create_Proc_Load_Input_Table */
 CREATE PROCEDURE Load_Input_Table
 (@target_month varchar(8))
 AS
 declare month_cur CURSOR local scroll static for
    select top 13 statement_month
      from statement
      where statement_month <= @target_month
      group by statement_month
      order by statement_month desc;
 open month_cur 
 declare @month varchar(8) 
 fetch next from month_cur into @month 
 while (@@fetch_status = 0)
  begin
     insert into source_adv(Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
       Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
       Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
-- Below is the sql in DAC env.
     select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
       Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
       Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
-- Below is the sql in Fuhwa env.
--     select [Statement month], [Account Status], [Inactive date for status 6], [Credit Limit], [Purchase Average Daily Balance],
--       [Cash Average Daily Balance], [Purchase amount], [Purchase count], [Cash amount], [Cash Advance count], [Interest charge],
--       [Late Fee Assessed], [Payment Amount], [Minimum Payment], [# cycles Past Due], [Total bal on bill], [Customer id]
     from statement
     where statement_month = @month
     fetch next from month_cur into @month
  end
 close month_cur 
 deallocate month_cur 

/* 03- Execute_Proc_Load_Input_Table */
-- 09:55
EXEC Load_Input_Table '200305'

/* 04- Create_Index_u_statementMonth_idn_on_Input */
 create unique index u_statementMonth_idn on source_adv(statement_month, idn);
/* Create_Index_i_statementMonth_on_Input */
 create index i_statementMonth on source_adv(statement_month);
/* Create_Index_i_idn_on_Input */
 create index i_idn on source_adv(idn);
-- 01:02

/* 05- Calculate_Month_Since  */
-- 04:53, 1,708,148
 update source
 set month_since =
   (cast (substring(statement_month,1,4) as int) - 1911) * 12 +
    cast (substring(statement_month,5,2) as int)
 where statement_month between '200305' and '200405' 
 
 DROP PROCEDURE Assign_ALL_PayCode
/* 06-  Create_Proc_Assign_ALL_PayCode */
 CREATE PROCEDURE Assign_ALL_PayCode
 AS
 declare month_cur CURSOR local scroll static for
    select distinct statement_month
      from source
      where statement_month between '200305' and '200405' 
      order by statement_month asc;
 open month_cur
 declare @month varchar(8)
 fetch next from month_cur into @month
 fetch next from month_cur into @month
 while (@@fetch_status = 0)
  begin
 update source_advset paycode = 'A'
 from source_adv as a
 where statement_month = @month and
      exists ( select idn
              from source_adv as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1 and
                    minimum_payment = 0 and
                    (purchase_average_daily_balance + cash_average_daily_balance <> 0 or
                    purchase_amount + cash_amount <> 0) );
 update source_advset paycode = 'X'
 from source_adv as a
 where statement_month = @month and
      exists ( select idn
              from source_adv as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1 and
                    minimum_payment = 0 and
                    purchase_average_daily_balance + cash_average_daily_balance = 0 and
                    purchase_amount + cash_amount = 0 );
 update source_advset paycode = 'X'
 from source_adv as a
 where statement_month = @month and
      not exists ( select idn
              from source_adv as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1 );
 update source_advset paycode = 'F'
 from source_adv as a
 where statement_month = @month and
      exists ( select idn
              from source_adv as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and
                    payment_amount = 0);
 update source_advset paycode = 'E'
 from source_adv as a
 where statement_month = @month and
      exists ( select idn
              from source_adv as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and
                    payment_amount <> 0 and
                    payment_amount <  minimum_payment);
 update source_advset paycode = 'D'
 from source_adv as a
 where statement_month = @month and
      interest_charge > 0 and
      late_fee_assessed > 0 and
      exists ( select idn
              from source_adv as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and
                    payment_amount <> 0 and
                    payment_amount >= minimum_payment);
 update source_advset paycode = 'C'
 from source_adv as a
 where statement_month = @month and
      interest_charge > 0 and
      late_fee_assessed <= 0 and
      exists ( select idn
              from source_adv as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and
                    payment_amount <> 0 and
                    payment_amount >= minimum_payment);
 update source_advset paycode = 'B'
 from source_adv as a
 where statement_month = @month and
      interest_charge <= 0 and
      late_fee_assessed > 0 and
      exists ( select idn
              from source_adv as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and
                    payment_amount <> 0 and
                    payment_amount >= minimum_payment);
 update source_advset paycode = 'A'
 from source_adv as a
 where statement_month = @month and
      interest_charge <= 0 and
      late_fee_assessed <= 0 and
      exists ( select idn
              from source_adv as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and
                    payment_amount <> 0 and
                    payment_amount >= minimum_payment);
     fetch next from month_cur into @month
  end
 close month_cur
 deallocate month_cur

/* Execute_Proc_Assign_ALL_PayCode */
-- 18:04
EXEC Assign_ALL_PayCode


/* 07- CREATE PROCEDURE Assign_SegCode */
/* change 
    Fuhwa 	 <-> DAC
   [Customer IN] <-> [masked_id]
   [Account Open Date] <-> [account_open_date]
*/

 CREATE PROCEDURE Assign_SegCode
 (@target_month varchar(6))
 AS
 declare @yyyymm int
 declare @yyyy int
 declare @mm int
 declare @next_month varchar(10)
 declare @six_month_ago varchar(10)
 set @yyyymm = cast (@target_month as int)
 set @yyyy = (@yyyymm - @yyyymm % 100 ) / 100
 set @mm = @yyyymm % 100
 if  @mm + 1 > 12
 begin
  set @mm = @mm + 1 - 12
  set @yyyy = @yyyy + 1
 end
 else
  set @mm = @mm + 1
 set @next_month = cast ((@yyyy * 100 + @mm) as char(6)) + '01'
 set @yyyymm = cast (@target_month as int)
 set @yyyy = (@yyyymm - @yyyymm % 100 ) / 100
 set @mm = @yyyymm % 100
 if  @mm < 6
 begin
  set @mm = @mm - 5 + 12
  set @yyyy = @yyyy - 1
 end
 else
  set @mm = @mm - 5
 set @six_month_ago = cast ((@yyyy * 100 + @mm) as char(6)) + '01'
 
 update source
 set flag = 8
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@six_month_ago and
       paycode in ('E', 'F');
 update source
 set flag = 7
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@six_month_ago and
       paycode not in ('E', 'F');
 update source
 set flag = 9
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
      ( account_status=6 and (inactive_date_for_status_6 <@next_month or inactive_date_for_status_6 is null ));
 update source
 set flag = 10
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode NOT in ('E', 'F') and
       0 = (select SUM(interest_charge)
            from source_advc
            where b.idn = c.idn and
                  c.month_since between b.month_since - 11 and b.month_since) and
       12<= (select count(*)
             from source_advc
             where b.idn = c.idn and
                   c.month_since between b.month_since - 11 and b.month_since and
                   paycode='X');
 update source
 set flag = 11
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode NOT in ('E', 'F') and
       0 = (select SUM(interest_charge)
            from source_advc
            where b.idn = c.idn and
            c.month_since between b.month_since - 11 and b.month_since) and
       12> (select count(*)
            from source_advc
            where b.idn = c.idn and
                  c.month_since between b.month_since - 11 and b.month_since and
                  paycode='X');
 update source
 set flag = 12
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode NOT in ('E', 'F') and
       0 < (select SUM(interest_charge)
            from source_advc
            where b.idn = c.idn and
                  c.month_since between b.month_since - 11 and b.month_since);
 update source
 set flag = 0
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode in ('E', 'F');
 update source
 set flag = 1
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 1;
 update source
 set flag = 2
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 2;
 update source
 set flag = 3
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 3;
 update source
 set flag = 4
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 4;
 update source
 set flag = 5
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 5;
 update source
 set flag = 6
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due >=6;

 update source
 set flag = (case when __cycles_past_due = 1 then 1
             when __cycles_past_due = 2 then 2
             when __cycles_past_due = 3 then 3
             when __cycles_past_due = 4 then 4
             when __cycles_past_due = 5 then 5
             when __cycles_past_due >= 6 then 6
             end)
 where statement_month = @target_month
   and flag = 12
   and __cycles_past_due >0

/* Execute_Proc_Assign_SegCode */
-- 01:50
EXEC Assign_SegCode '200405'

 DROP PROCEDURE Assign_AccountAge
/* 08- CREATE PROCEDURE Assign_AccountAge */
 CREATE PROCEDURE Assign_AccountAge
 (@target_month varchar(6))
 AS
 declare @yyyymm int
 declare @yyyy int
 declare @mm int
 declare @next_month varchar(10)
 declare @six_month_ago varchar(10)
 declare @twelve_month_ago varchar(10)
 declare @twentyfour_month_ago varchar(10)
 set @yyyymm = cast (@target_month as int)
 set @yyyy = (@yyyymm - @yyyymm % 100 ) / 100
 
 set @mm = @yyyymm % 100
 if  @mm + 1 > 12
 begin
  set @mm = @mm + 1 - 12
  set @yyyy = @yyyy + 1
 end
 else
  set @mm = @mm + 1
 set @next_month = cast ((@yyyy * 100 + @mm) as char(6)) + '01'
 
 set @yyyymm = cast (@target_month as int)
 set @yyyy = (@yyyymm - @yyyymm % 100 ) / 100
 set @mm = @yyyymm % 100
 if  @mm < 6
 begin
  set @mm = @mm - 5 + 12
  set @yyyy = @yyyy - 1
 end
 else
  set @mm = @mm - 5
 set @six_month_ago = cast ((@yyyy * 100 + @mm) as char(6)) + '01'
 
 set @yyyymm = cast (@target_month as int)
 set @yyyy = (@yyyymm - @yyyymm % 100 ) / 100
 set @mm = @yyyymm % 100
 if  @mm + 1 > 12
  set @mm = @mm + 1 - 12
 else
  begin
    set @mm = @mm + 1
    set @yyyy = @yyyy - 1
  end
 set @twelve_month_ago = cast ((@yyyy * 100 + @mm) as char(6)) + '01'

 set @yyyymm = cast (@target_month as int)
 set @yyyy = (@yyyymm - @yyyymm % 100 ) / 100
 set @mm = @yyyymm % 100
 if  @mm + 1 > 12
  begin
    set @mm = @mm + 1 - 12
    set @yyyy = @yyyy - 1
  end
 else
  begin
    set @mm = @mm + 1
    set @yyyy = @yyyy - 2
  end
 set @twentyfour_month_ago = cast ((@yyyy * 100 + @mm) as char(6)) + '01'
/*
print 'Target Month: ' + @target_month
print 'Next Month: ' + @next_month
print '6 Month Ago: ' + @six_month_ago
print '12 Month Ago: ' + @twelve_month_ago
print '24 Month Ago: ' + @twentyfour_month_ago
*/
 update source
 set age = 0
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >= @six_month_ago;

 update source
 set age = 1
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@twelve_month_ago and
       a.[account_open_date] <@six_month_ago;

 update source
 set age = 2
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@twentyfour_month_ago and
       a.[account_open_date] <@twelve_month_ago;

 update source
 set age = 3
 from account a, source_advb
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] <@twentyfour_month_ago;

/* Execute_Proc_Assign_AccountAge */
-- 00:14, 118,002
 EXEC Assign_AccountAge '200405'

 CREATE TABLE [vars] (
        [Statement_month] [varchar] (8) NULL,
        [idn] [char] (11) NULL,
        [PayCode] [char] (1) NULL,
        [flag] [int] NULL,
        [age] [int] NULL,
        [month_since] [int] NULL,
        [beg_adb] [float] NULL,
        [beg_rev_adb] [float] NULL,
        [beg_revolver] [int] NULL,
        [propensity] [int] NULL,
        [segment] [int] NULL,
        [gender] [varchar] (1) NULL,
        [educational_level] [varchar] (1) NULL,
        [home_ownership] [varchar] (1) NULL,
        [marital_status] [varchar] (1) NULL,
        [gender_tran] [decimal](13, 5) NULL,
        [edu_tran] [decimal](13, 5) NULL,
        [edu_tran2] [decimal](13, 5) NULL,
        [ownership1] [decimal](13, 5) NULL,
        [index2] [decimal](13, 5) NULL,
        [marriage_t] [decimal](13, 5) NULL,
        [marriage_i2] [decimal](13, 5) NULL,
        [AMFS001_06] [decimal](13, 5) NULL,
        [AMFS001_06_z] [decimal](13, 5) NULL,
        [AMFS004_12] [decimal](13, 5) NULL,
        [AMFS004_12_tran] [decimal](13, 5) NULL,
        [AMFS014_03] [decimal](13, 5) NULL,
        [AMFS014_03_q] [decimal](13, 5) NULL ,
        [AMFS014_12] [decimal](13, 5) NULL ,
        [AMFS014_12_tran] [decimal](13, 5) NULL,
        [AMFS016_03] [decimal](13, 5) NULL,
        [AMFS016_09] [decimal](13, 5) NULL,
        [AMFS016_03_tran] [decimal](13, 5) NULL,
        [AMMS035_06] [decimal](13, 5) NULL,
        [AMMS035_06_r] [decimal](13, 5) NULL,
        [AMMS035_06_r_tran] [decimal](13, 5) NULL,
        [AMMS060_12] [decimal](13, 5) NULL,
        [AMMS060_12_q] [decimal](13, 5) NULL,
        [AMMS061_03] [decimal](13, 5) NULL,
        [AMMS061_06] [decimal](13, 5) NULL,
        [AMMT061_43] [decimal](13, 5) NULL,
        [AMMT061_43_tran] [decimal](13, 5) NULL,
        [AMMS066_09] [decimal](13, 5) NULL,
        [AMMS066_09_tran] [decimal](13, 5) NULL,
        [score] [float] NULL,
        [pd] [float] NULL 
 )

 insert into vars_adv (statement_month, idn, paycode, flag, age, month_since, beg_adb, beg_revolver)
 select statement_month, idn, paycode, flag, age, month_since,
  Purchase_Average_Daily_Balance+Cash_Average_Daily_Balance, (case where interest_charge > 0 then 1 else 0 end) 
 from source_adv
 where statement_month='200405';

 create unique index u_idn on vars (idn);

 update vars_adv
 set gender= a.gender,
     educational_level = a.educational_level,
     home_ownership = a.home_ownership,
     marital_status = a.marital_status,
     beg_rev_adb = beg_adb * beg_revolver
 from  account_adv a
 where vars_adv.idn=a.masked_id

--AMMS035_06_r_tran, AMMS035_06_r, AMMS035_06
 update vars
 set AMMS035_06=
 (select  AVG(credit_limit - total_bal_on_bill) /  AVG(credit_limit)
  from  source_adv a
  where vars_adv.idn=a.idn and 
        a.month_since between vars_adv.month_since - 5 and vars_adv.month_since)
 where flag in (11, 12) and age = 1;

-- 101375
 update vars
 set AMMS035_06_r= (case when AMMS035_06 >= 0 then SQRT(AMMS035_06)
                         else NULL
                    end)
 where flag in (11, 12) and age = 1;

--AMFS016_03_tran, AMFS016_03
 update vars
 set AMFS016_03=
 (select  count(*)
  from  source_adv a
  where vars_adv.idn=a.idn 
    and a.month_since between vars_adv.month_since - 2 and vars_adv.month_since
    and a.Late_Fee_Assessed > 0)
 where flag in (11, 12) and age = 1;

-- 101375
 update vars
 set AMFS016_03_tran = (case
                           when AMFS016_03 > 2 then 2
                           else AMFS016_03
                        end),
     AMMS035_06_r_tran = (case when AMMS035_06_r is NULL then 0
                            when AMMS035_06_r > 1 then 1
                            else AMMS035_06_r
                          end),
     gender_tran = (case when gender = 1 then 1
                     else 0
                   end),
     edu_tran = (case when educational_level not in ('1','2','3') then 1
                   else 0
                 end),
     ownership1 = (case when home_ownership = 1 then 1
                     else 0
                   end)
 where flag in (11, 12) and age = 1;

-- 101375
-- gender_tran
-- edu_tran
-- ownership1

----------------------------------------------------------------------------
-- AMMS061 = SUM(purchase_average_daily_balance + cash_average_daily_balance)/ sum(credit_limit) / 3
--AMMT061_43_tran, AMMT061_43, AMMS061_03, AMMS061_06
--03:46, 29279
--03:51, 21149
 update vars_adv
 set AMMS061_03 =
 (select  (SUM(purchase_average_daily_balance + cash_average_daily_balance) / SUM(credit_limit)) / 3
  from  source_adv a
  where vars_adv.idn=a.idn and 
        paycode in ('C', 'D', 'E', 'F') and
        a.month_since between vars_adv.month_since - 2 and vars_adv.month_since)
 where flag in (11, 12) and age in (2, 3)
   and exists (select  *  from  source_adv a
               where vars_adv.idn=a.idn and 
                     paycode in ('C', 'D', 'E', 'F') and
                     a.month_since between vars_adv.month_since - 2 and vars_adv.month_since);

--01:01, 34742
--0:12, 20680
 update vars_adv
 set AMMS061_06=
 (select  (SUM(purchase_average_daily_balance + cash_average_daily_balance) / SUM(credit_limit)) / 6
  from  source_adv a
  where vars_adv.idn=a.idn and 
        paycode in ('C', 'D', 'E', 'F') and
        a.month_since between vars_adv.month_since - 5 and vars_adv.month_since)
 where flag in (11, 12) and age in (2, 3)
   and exists (select  *  from  source_adv a
               where vars_adv.idn=a.idn and 
                     paycode in ('C', 'D', 'E', 'F') and
                     a.month_since between vars_adv.month_since - 5 and vars_adv.month_since);


--AMFS004_12_tran, AMFS004_12
--00:38, 196696
 update vars
 set AMFS004_12=
 (select  count(*)
  from  source_adv a
  where vars_adv.idn=a.idn and 
        a.month_since between vars_adv.month_since - 11 and vars_adv.month_since and 
        a.cash_amount > 0)
 where flag in (11, 12) and age in (2, 3);


--AMMS060_12_q, AMMS060_12
--01:21, 44042
 update vars
 set AMMS060_12=
 (select  AVG(purchase_average_daily_balance + cash_average_daily_balance) / AVG(credit_limit) 
  from  source_adv a
  where vars_adv.idn=a.idn and 
        paycode in ('C', 'D', 'E', 'F') and
        a.month_since between vars_adv.month_since - 11 and vars_adv.month_since)
 where flag in (11, 12) and age in (2, 3)
   and exists (select *
               from  source_adv a
               where vars_adv.idn=a.idn and 
                     paycode in ('C', 'D', 'E', 'F') and
                     a.month_since between vars_adv.month_since - 11 and vars_adv.month_since);


--AMMS066_09_tran, AMMS066_09
--01:11, 86283
 update vars
 set AMMS066_09=
 (select  max(payment_amount / total_bal_on_bill)
  from  source_adv a
  where vars_adv.idn=a.idn and 
        a.month_since between vars_adv.month_since - 9 and vars_adv.month_since - 1 and 
        a.total_bal_on_bill > 0)
 where flag in (11, 12) and age in (2, 3)
   and exists (select *
               from  source_adv a
               where vars_adv.idn=a.idn and 
                     a.month_since between vars_adv.month_since - 9 and vars_adv.month_since - 1 and 
                     a.total_bal_on_bill > 0);

 update vars
 set AMMS066_09_tran= (case when AMMS066_09 > 2.4 then 2.4
                            else AMMS066_09
                         end)
 where flag in (11, 12) and age in (2, 3);


-- AMFS014_03_q, AMFS014_03
--01:07, 196696
 update vars
 set AMFS014_12=
 (select  count(*)
  from  source_adv a
  where vars_adv.idn=a.idn and 
        a.month_since between vars_adv.month_since - 11 and vars_adv.month_since and 
        a.paycode in ('E', 'F'))
 where flag in (11, 12) and age in (2, 3);

--01:07, 196696
 update vars
 set AMFS014_03=
 (select  count(*)
  from  source_adv a
  where vars_adv.idn=a.idn and 
        a.month_since between vars_adv.month_since - 2 and vars_adv.month_since and 
        a.paycode in ('E', 'F'))
 where flag in (11, 12) and age in (2, 3);

-- AMFS001_06_z,AMFS001_06
--00:38, 7662
 update vars
 set AMFS001_06=
 (select  SUM(Cash_Advance_count)
  from  source
  where vars_adv.idn=idn and 
           month_since between vars_adv.month_since - 5 and vars_adv.month_since and 
           cash_advance_count > 0)
 where flag in (11, 12) and age in (2, 3) and exists (select *
  from  source
  where vars_adv.idn=idn and 
           month_since between vars_adv.month_since - 5 and vars_adv.month_since and 
           cash_advance_count > 0);

--AMFS016_09
--00:38, 196696
 update vars
 set amfs016_09=
 (select  count(*)
  from  source_adv a
  where vars_adv.idn=a.idn and 
        a.month_since between vars_adv.month_since - 8 and vars_adv.month_since and 
        a.late_fee_assessed > 0)
 where age in (2, 3);

-- 00:43, 196696 
 update vars
 set AMFS004_12_tran = (case when AMFS004_12 > 4 then 4
                          else AMFS004_12
                        end),
     AMMT061_43 = AMMS061_03 - (AMMS061_06 - AMMS061_03),
     AMMS060_12_q = AMMS060_12 * AMMS060_12,
     AMMS066_09_tran= (case when AMMS066_09 > 2.4 then 2.4
                            else AMMS066_09
                       end),
     AMFS014_03_q = AMFS014_03 * AMFS014_03,
     AMFS001_06_z = (case when AMFS001_06 = 0 then 1
                          else 0
                     end),
     edu_tran2 = (case when educational_level not in ('1','2','3','4') then 1
                   else 0
                 end),
     marriage_t = (case when marital_status = '1' then 1
                     when marital_status = '2' then 2
                     else 3
                   end),
     index2 = (case when age = 2 then 1
                 else 0
               end)
 where flag in (11, 12) and age in (2, 3);


-- 196696
-- edu_tran2
-- marriage_i2, index2, marriage_t

 update oliver.vars_adv
 set marriage_i2= marriage_t * index2,
     AMMT061_43_tran= (case when AMMT061_43 < 0 then 0.02
                            else AMMT061_43
                         end)
 where flag in (11, 12) and age in (2, 3);


-- scores
 update oliver.vars_adv
 set score = 0.0566
           + AMMT061_43_tran * 0.17151
           + AMFS014_12 * 0.0191
           + AMFS004_12_tran * 0.02875
           + AMMS060_12_q * 0.09258
           + AMMS066_09_tran * -0.04762
           + edu_tran2 * 0.02766
           + AMFS014_03_q * 0.03393
           + AMFS001_06_z * -0.047
           + AMFS016_09 * 0.01189
           + marriage_i2 * 0.014
 where flag in (11, 12) and age in (2, 3);

-- 101375 
update vars
 set score = 0.38056
           + AMMS035_06_r_tran * -0.42027
           + AMFS016_03_tran * 0.06839
           + gender_tran * 0.04519
           + edu_tran * 0.03736
           + ownership1 * -0.02698
 where flag in (11, 12) and age = 1;


-------------------------------------------------
alter table vars_adv
 add beg_rev_adb float,
     propensity int,
     segment int

update vars_adv
 set beg_rev_adb = b.Purchase_Average_Daily_Balance + b.Cash_Average_Daily_Balance
from  source_adv b
--select count(*)
--from vars_adv, source_adv b
where b.statement_month = '200405'
  and vars_adv.idn = b.idn

select count(*)
from vars_adv

-- assign propensity
 update vars_adv
 set propensity =
   case
     when beg_rev_adb  = 0     then 0
     when beg_rev_adb <= 3833  then 1 
     when beg_rev_adb <= 8000  then 2 
     when beg_rev_adb <= 12054 then 3 
     when beg_rev_adb <= 16031 then 4 
     when beg_rev_adb <= 20117 then 5 
     when beg_rev_adb <= 24243 then 6 
     when beg_rev_adb <= 27451 then 7 
     when beg_rev_adb <= 32446 then 8 
     when beg_rev_adb <= 40298 then 9 
     when beg_rev_adb <= 46801 then 10 
     when beg_rev_adb <= 64703 then 11
     else 12 
   end 
 where age = 1 and flag=12  

 update vars_adv
 set propensity =
   case
     when beg_rev_adb  = 0     then 0 
     when beg_rev_adb <= 3498  then 1 
     when beg_rev_adb <= 8695  then 2 
     when beg_rev_adb <= 14359 then 3 
     when beg_rev_adb <= 20032 then 4 
     when beg_rev_adb <= 24805 then 5 
     when beg_rev_adb <= 28767 then 6 
     when beg_rev_adb <= 36865 then 7 
     when beg_rev_adb <= 44588 then 8 
     when beg_rev_adb <= 61624 then 9 
     else 10 
   end 
 where age = 2 and flag=12  

 update vars_adv
 set propensity =
   case
     when beg_rev_adb  = 0     then 0 
     when beg_rev_adb <= 5052  then 1 
     when beg_rev_adb <= 12113 then 2 
     when beg_rev_adb <= 19294 then 3 
     when beg_rev_adb <= 25725 then 4 
     when beg_rev_adb <= 32025 then 5 
     when beg_rev_adb <= 39791 then 6 
     when beg_rev_adb <= 49948 then 7 
     when beg_rev_adb <= 63510 then 8 
     when beg_rev_adb <= 91082 then 9 
     else 10 
   end 
 where age = 3 and flag=12  

 update vars_adv
 set segment = (case
                  when score is null then 0				
                  when score <=	-0.02979 then 1
                  when score <=	-0.02413 then 2
                  when score <=	-0.01737 then 3
                  when score <=	-0.00374 then 4
                  when score <=	0.00622 then 5
                  when score <=	0.01586 then 6
                  when score <=	0.02082 then 7
                  when score <=	0.0305 then 8
                  when score <=	0.04235 then 9
                  when score <=	0.04968 then 10
                  when score <=	0.06221 then 11
                  when score <=	0.07735 then 12
                  when score <=	0.09516 then 13
                  when score <=	0.11905 then 14
                  when score <=	0.14482 then 15
                  when score <=	0.17466 then 16
                  when score <=	0.21126 then 17
                  when score <=	0.25251 then 18
                  when score <=	0.31023 then 19
                  else 20
                end)
 where flag in (11, 12) and age = 1;	

 update vars_adv
 set segment = (case
                  when score is null then 0
                  when score <= -0.03745 then 1
                  when score <= -0.02793 then 2
                  when score <= -0.01876 then 3
                  when score <= -0.01002 then 4
                  when score <= -0.00393 then 5
                  when score <= 0.00423 then 6
                  when score <= 0.01198 then 7
                  when score <= 0.02187 then 8
                  when score <= 0.03142 then 9
                  when score <= 0.04497 then 10
                  when score <= 0.06176 then 11
                  when score <= 0.0806 then 12
                  when score <= 0.10071 then 13
                  when score <= 0.12308 then 14
                  when score <= 0.14673 then 15
                  when score <= 0.1701 then 16
                  when score <= 0.19599 then 17
                  when score <= 0.22703 then 18
                  when score <= 0.28059 then 19
                  else 20
                end)
 where flag in (11, 12) and age in (2, 3);	


 update vars_adv
  set pd = (case
               when segment <= 10 then 0.0005
             else 0.0031
            end)
 where flag = 11 and age = 1  
 update vars_adv
  set pd = (case
               when segment <= 10 then 0.0004
             else 0.0094
            end)
 where flag = 11 and age in (2, 3)

 update vars_adv
  set pd = (case
      when segment = 1 then 0.0004
      when segment = 2 then 0.0012
      when segment = 3 then 0.002
      when segment = 4 then 0.0028
      when segment = 5 then 0.0038
      when segment = 6 then 0.0048
      when segment = 7 then 0.0057
      when segment = 8 then 0.0068
      when segment = 9 then 0.0079
      when segment = 10 then 0.009
      when segment = 11 then 0.011
      when segment = 12 then 0.013
      when segment = 13 then 0.016
      when segment = 14 then 0.021
      when segment = 15 then 0.028
      when segment = 16 then 0.039
      when segment = 17 then 0.058
      when segment = 18 then 0.092
      when segment = 19 then 0.15
      when segment = 20 then 0.3
      else 0
    end)
 where flag = 12 and age = 1 and propensity = 0  

 update vars_adv
  set pd = (case
              when segment = 1 then 0.0014
              when segment = 2 then 0.0024
              when segment = 3 then 0.0035
              when segment = 4 then 0.0047
              when segment = 5 then 0.0059
              when segment = 6 then 0.0072
              when segment = 7 then 0.0084
              when segment = 8 then 0.0098
              when segment = 9 then 0.011
              when segment = 10 then 0.013
              when segment = 11 then 0.014
              when segment = 12 then 0.017
              when segment = 13 then 0.02
              when segment = 14 then 0.026
              when segment = 15 then 0.033
              when segment = 16 then 0.045
              when segment = 17 then 0.064
              when segment = 18 then 0.099
              when segment = 19 then 0.16
              when segment = 20 then 0.3
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 1  

 update vars_adv
  set pd = (case
              when segment = 1 then 0.0024
              when segment = 2 then 0.0037
              when segment = 3 then 0.0051
              when segment = 4 then 0.0065
              when segment = 5 then 0.0081
              when segment = 6 then 0.0097
              when segment = 7 then 0.011
              when segment = 8 then 0.013
              when segment = 9 then 0.015
              when segment = 10 then 0.016
              when segment = 11 then 0.018
              when segment = 12 then 0.021
              when segment = 13 then 0.025
              when segment = 14 then 0.031
              when segment = 15 then 0.039
              when segment = 16 then 0.051
              when segment = 17 then 0.071
              when segment = 18 then 0.11
              when segment = 19 then 0.17
              when segment = 20 then 0.31
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 2  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0035
              when segment = 2 then 0.005
              when segment = 3 then 0.0066
              when segment = 4 then 0.0083
              when segment = 5 then 0.01
              when segment = 6 then 0.012
              when segment = 7 then 0.014
              when segment = 8 then 0.016
              when segment = 9 then 0.018
              when segment = 10 then 0.02
              when segment = 11 then 0.022
              when segment = 12 then 0.026
              when segment = 13 then 0.03
              when segment = 14 then 0.036
              when segment = 15 then 0.044
              when segment = 16 then 0.057
              when segment = 17 then 0.077
              when segment = 18 then 0.11
              when segment = 19 then 0.18
              when segment = 20 then 0.32
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 3  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0045
              when segment = 2 then 0.0063
              when segment = 3 then 0.0082
              when segment = 4 then 0.01
              when segment = 5 then 0.012
              when segment = 6 then 0.015
              when segment = 7 then 0.017
              when segment = 8 then 0.019
              when segment = 9 then 0.021
              when segment = 10 then 0.024
              when segment = 11 then 0.026
              when segment = 12 then 0.03
              when segment = 13 then 0.034
              when segment = 14 then 0.041
              when segment = 15 then 0.05
              when segment = 16 then 0.062
              when segment = 17 then 0.083
              when segment = 18 then 0.12
              when segment = 19 then 0.18
              when segment = 20 then 0.33
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 4  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0055
              when segment = 2 then 0.0076
              when segment = 3 then 0.0097
              when segment = 4 then 0.012
              when segment = 5 then 0.015
              when segment = 6 then 0.017
              when segment = 7 then 0.019
              when segment = 8 then 0.022
              when segment = 9 then 0.025
              when segment = 10 then 0.027
              when segment = 11 then 0.03
              when segment = 12 then 0.034
              when segment = 13 then 0.039
              when segment = 14 then 0.046
              when segment = 15 then 0.055
              when segment = 16 then 0.068
              when segment = 17 then 0.089
              when segment = 18 then 0.13
              when segment = 19 then 0.19
              when segment = 20 then 0.33
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 5  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0065
              when segment = 2 then 0.0089
              when segment = 3 then 0.011
              when segment = 4 then 0.014
              when segment = 5 then 0.017
              when segment = 6 then 0.019
              when segment = 7 then 0.022
              when segment = 8 then 0.025
              when segment = 9 then 0.028
              when segment = 10 then 0.031
              when segment = 11 then 0.034
              when segment = 12 then 0.038
              when segment = 13 then 0.044
              when segment = 14 then 0.051
              when segment = 15 then 0.061
              when segment = 16 then 0.074
              when segment = 17 then 0.095
              when segment = 18 then 0.13
              when segment = 19 then 0.2
              when segment = 20 then 0.34
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 6  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0076
              when segment = 2 then 0.01
              when segment = 3 then 0.013
              when segment = 4 then 0.016
              when segment = 5 then 0.019
              when segment = 6 then 0.022
              when segment = 7 then 0.025
              when segment = 8 then 0.028
              when segment = 9 then 0.031
              when segment = 10 then 0.035
              when segment = 11 then 0.038
              when segment = 12 then 0.043
              when segment = 13 then 0.048
              when segment = 14 then 0.056
              when segment = 15 then 0.066
              when segment = 16 then 0.08
              when segment = 17 then 0.1
              when segment = 18 then 0.14
              when segment = 19 then 0.2
              when segment = 20 then 0.35
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 7  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0086
              when segment = 2 then 0.011
              when segment = 3 then 0.014
              when segment = 4 then 0.018
              when segment = 5 then 0.021
              when segment = 6 then 0.024
              when segment = 7 then 0.028
              when segment = 8 then 0.031
              when segment = 9 then 0.035
              when segment = 10 then 0.038
              when segment = 11 then 0.042
              when segment = 12 then 0.047
              when segment = 13 then 0.053
              when segment = 14 then 0.061
              when segment = 15 then 0.071
              when segment = 16 then 0.086
              when segment = 17 then 0.11
              when segment = 18 then 0.15
              when segment = 19 then 0.21
              when segment = 20 then 0.35
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 8  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0096
              when segment = 2 then 0.013
              when segment = 3 then 0.016
              when segment = 4 then 0.019
              when segment = 5 then 0.023
              when segment = 6 then 0.027
              when segment = 7 then 0.03
              when segment = 8 then 0.034
              when segment = 9 then 0.038
              when segment = 10 then 0.042
              when segment = 11 then 0.046
              when segment = 12 then 0.051
              when segment = 13 then 0.058
              when segment = 14 then 0.066
              when segment = 15 then 0.077
              when segment = 16 then 0.091
              when segment = 17 then 0.11
              when segment = 18 then 0.15
              when segment = 19 then 0.22
              when segment = 20 then 0.36
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 9  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.011
              when segment = 2 then 0.014
              when segment = 3 then 0.018
              when segment = 4 then 0.021
              when segment = 5 then 0.025
              when segment = 6 then 0.029
              when segment = 7 then 0.033
              when segment = 8 then 0.037
              when segment = 9 then 0.042
              when segment = 10 then 0.046
              when segment = 11 then 0.05
              when segment = 12 then 0.056
              when segment = 13 then 0.062
              when segment = 14 then 0.071
              when segment = 15 then 0.082
              when segment = 16 then 0.097
              when segment = 17 then 0.12
              when segment = 18 then 0.16
              when segment = 19 then 0.23
              when segment = 20 then 0.37
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 10  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.012
              when segment = 2 then 0.015
              when segment = 3 then 0.019
              when segment = 4 then 0.023
              when segment = 5 then 0.027
              when segment = 6 then 0.032
              when segment = 7 then 0.036
              when segment = 8 then 0.04
              when segment = 9 then 0.045
              when segment = 10 then 0.049
              when segment = 11 then 0.054
              when segment = 12 then 0.06
              when segment = 13 then 0.067
              when segment = 14 then 0.076
              when segment = 15 then 0.088
              when segment = 16 then 0.1
              when segment = 17 then 0.13
              when segment = 18 then 0.17
              when segment = 19 then 0.23
              when segment = 20 then 0.38
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 11  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.013
              when segment = 2 then 0.017
              when segment = 3 then 0.021
              when segment = 4 then 0.025
              when segment = 5 then 0.03
              when segment = 6 then 0.034
              when segment = 7 then 0.039
              when segment = 8 then 0.044
              when segment = 9 then 0.048
              when segment = 10 then 0.053
              when segment = 11 then 0.058
              when segment = 12 then 0.064
              when segment = 13 then 0.072
              when segment = 14 then 0.081
              when segment = 15 then 0.093
              when segment = 16 then 0.11
              when segment = 17 then 0.13
              when segment = 18 then 0.17
              when segment = 19 then 0.24
              when segment = 20 then 0.38
              else 0
            end)
 where flag = 12 and age = 1 and propensity = 12 

 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0004
              when segment = 3 then 0.0009
              when segment = 4 then 0.0018
              when segment = 5 then 0.0026
              when segment = 6 then 0.0036
              when segment = 7 then 0.0048
              when segment = 8 then 0.006
              when segment = 9 then 0.0075
              when segment = 10 then 0.009
              when segment = 11 then 0.01
              when segment = 12 then 0.012
              when segment = 13 then 0.013
              when segment = 14 then 0.014
              when segment = 15 then 0.016
              when segment = 16 then 0.017
              when segment = 17 then 0.018
              when segment = 18 then 0.02
              when segment = 19 then 0.026
              when segment = 20 then 0.11
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 0  

 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0004
              when segment = 3 then 0.0012
              when segment = 4 then 0.0023
              when segment = 5 then 0.0033
              when segment = 6 then 0.0046
              when segment = 7 then 0.0061
              when segment = 8 then 0.0076
              when segment = 9 then 0.0095
              when segment = 10 then 0.011
              when segment = 11 then 0.013
              when segment = 12 then 0.015
              when segment = 13 then 0.017
              when segment = 14 then 0.018
              when segment = 15 then 0.02
              when segment = 16 then 0.021
              when segment = 17 then 0.023
              when segment = 18 then 0.025
              when segment = 19 then 0.033
              when segment = 20 then 0.13
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 1  

 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0004
              when segment = 3 then 0.0014
              when segment = 4 then 0.0028
              when segment = 5 then 0.004
              when segment = 6 then 0.0055
              when segment = 7 then 0.0073
              when segment = 8 then 0.0092
              when segment = 9 then 0.011
              when segment = 10 then 0.014
              when segment = 11 then 0.016
              when segment = 12 then 0.018
              when segment = 13 then 0.02
              when segment = 14 then 0.022
              when segment = 15 then 0.024
              when segment = 16 then 0.026
              when segment = 17 then 0.028
              when segment = 18 then 0.03
              when segment = 19 then 0.039
              when segment = 20 then 0.15
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 2  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0004
              when segment = 3 then 0.0017
              when segment = 4 then 0.0033
              when segment = 5 then 0.0046
              when segment = 6 then 0.0065
              when segment = 7 then 0.0086
              when segment = 8 then 0.011
              when segment = 9 then 0.013
              when segment = 10 then 0.016
              when segment = 11 then 0.019
              when segment = 12 then 0.021
              when segment = 13 then 0.024
              when segment = 14 then 0.026
              when segment = 15 then 0.028
              when segment = 16 then 0.031
              when segment = 17 then 0.033
              when segment = 18 then 0.036
              when segment = 19 then 0.046
              when segment = 20 then 0.17
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 3  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0004
              when segment = 3 then 0.0019
              when segment = 4 then 0.0038
              when segment = 5 then 0.0053
              when segment = 6 then 0.0075
              when segment = 7 then 0.0099
              when segment = 8 then 0.012
              when segment = 9 then 0.015
              when segment = 10 then 0.019
              when segment = 11 then 0.022
              when segment = 12 then 0.024
              when segment = 13 then 0.027
              when segment = 14 then 0.03
              when segment = 15 then 0.032
              when segment = 16 then 0.035
              when segment = 17 then 0.038
              when segment = 18 then 0.041
              when segment = 19 then 0.053
              when segment = 20 then 0.19
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 4  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0007
              when segment = 3 then 0.0022
              when segment = 4 then 0.0043
              when segment = 5 then 0.006
              when segment = 6 then 0.0084
              when segment = 7 then 0.011
              when segment = 8 then 0.014
              when segment = 9 then 0.017
              when segment = 10 then 0.021
              when segment = 11 then 0.024
              when segment = 12 then 0.028
              when segment = 13 then 0.031
              when segment = 14 then 0.034
              when segment = 15 then 0.037
              when segment = 16 then 0.04
              when segment = 17 then 0.043
              when segment = 18 then 0.046
              when segment = 19 then 0.06
              when segment = 20 then 0.22
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 5  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0007
              when segment = 3 then 0.0024
              when segment = 4 then 0.0047
              when segment = 5 then 0.0067
              when segment = 6 then 0.0094
              when segment = 7 then 0.012
              when segment = 8 then 0.016
              when segment = 9 then 0.019
              when segment = 10 then 0.023
              when segment = 11 then 0.027
              when segment = 12 then 0.031
              when segment = 13 then 0.034
              when segment = 14 then 0.037
              when segment = 15 then 0.041
              when segment = 16 then 0.044
              when segment = 17 then 0.047
              when segment = 18 then 0.051
              when segment = 19 then 0.067
              when segment = 20 then 0.25
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 6  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0008
              when segment = 3 then 0.0027
              when segment = 4 then 0.0052
              when segment = 5 then 0.0074
              when segment = 6 then 0.01
              when segment = 7 then 0.014
              when segment = 8 then 0.017
              when segment = 9 then 0.021
              when segment = 10 then 0.026
              when segment = 11 then 0.03
              when segment = 12 then 0.034
              when segment = 13 then 0.038
              when segment = 14 then 0.041
              when segment = 15 then 0.045
              when segment = 16 then 0.049
              when segment = 17 then 0.052
              when segment = 18 then 0.057
              when segment = 19 then 0.074
              when segment = 20 then 0.28
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 7  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0009
              when segment = 3 then 0.0029
              when segment = 4 then 0.0057
              when segment = 5 then 0.0081
              when segment = 6 then 0.011
              when segment = 7 then 0.015
              when segment = 8 then 0.019
              when segment = 9 then 0.023
              when segment = 10 then 0.028
              when segment = 11 then 0.033
              when segment = 12 then 0.037
              when segment = 13 then 0.041
              when segment = 14 then 0.045
              when segment = 15 then 0.049
              when segment = 16 then 0.053
              when segment = 17 then 0.057
              when segment = 18 then 0.062
              when segment = 19 then 0.082
              when segment = 20 then 0.32
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 8  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.001
              when segment = 3 then 0.0032
              when segment = 4 then 0.0062
              when segment = 5 then 0.0088
              when segment = 6 then 0.012
              when segment = 7 then 0.016
              when segment = 8 then 0.02
              when segment = 9 then 0.025
              when segment = 10 then 0.03
              when segment = 11 then 0.035
              when segment = 12 then 0.04
              when segment = 13 then 0.045
              when segment = 14 then 0.049
              when segment = 15 then 0.053
              when segment = 16 then 0.058
              when segment = 17 then 0.062
              when segment = 18 then 0.067
              when segment = 19 then 0.09
              when segment = 20 then 0.37
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 9  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0019
              when segment = 2 then 0.0025
              when segment = 3 then 0.0049
              when segment = 4 then 0.0082
              when segment = 5 then 0.011
              when segment = 6 then 0.015
              when segment = 7 then 0.019
              when segment = 8 then 0.024
              when segment = 9 then 0.029
              when segment = 10 then 0.034
              when segment = 11 then 0.04
              when segment = 12 then 0.045
              when segment = 13 then 0.05
              when segment = 14 then 0.054
              when segment = 15 then 0.059
              when segment = 16 then 0.064
              when segment = 17 then 0.068
              when segment = 18 then 0.074
              when segment = 19 then 0.1
              when segment = 20 then 0.42
              else 0
            end)
 where flag = 12 and age = 2 and propensity = 10  

 update vars_adv
  set pd = (case
              when segment = 1 then 0.0004
              when segment = 2 then 0.0011
              when segment = 3 then 0.0017
              when segment = 4 then 0.0023
              when segment = 5 then 0.0029
              when segment = 6 then 0.0035
              when segment = 7 then 0.0041
              when segment = 8 then 0.0047
              when segment = 9 then 0.0053
              when segment = 10 then 0.0058
              when segment = 11 then 0.0064
              when segment = 12 then 0.007
              when segment = 13 then 0.0075
              when segment = 14 then 0.0082
              when segment = 15 then 0.009
              when segment = 16 then 0.01
              when segment = 17 then 0.013
              when segment = 18 then 0.02
              when segment = 19 then 0.039
              when segment = 20 then 0.11
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 0  

 update vars_adv
  set pd = (case
              when segment = 1 then 0.0005
              when segment = 2 then 0.0015
              when segment = 3 then 0.0023
              when segment = 4 then 0.0031
              when segment = 5 then 0.004
              when segment = 6 then 0.0048
              when segment = 7 then 0.0056
              when segment = 8 then 0.0064
              when segment = 9 then 0.0071
              when segment = 10 then 0.0078
              when segment = 11 then 0.0086
              when segment = 12 then 0.0094
              when segment = 13 then 0.01
              when segment = 14 then 0.011
              when segment = 15 then 0.012
              when segment = 16 then 0.014
              when segment = 17 then 0.017
              when segment = 18 then 0.024
              when segment = 19 then 0.046
              when segment = 20 then 0.12
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 1  

 update vars_adv
  set pd = (case
              when segment = 1 then 0.0006
              when segment = 2 then 0.0018
              when segment = 3 then 0.0029
              when segment = 4 then 0.0039
              when segment = 5 then 0.005
              when segment = 6 then 0.006
              when segment = 7 then 0.007
              when segment = 8 then 0.008
              when segment = 9 then 0.0089
              when segment = 10 then 0.0099
              when segment = 11 then 0.011
              when segment = 12 then 0.012
              when segment = 13 then 0.013
              when segment = 14 then 0.014
              when segment = 15 then 0.015
              when segment = 16 then 0.017
              when segment = 17 then 0.021
              when segment = 18 then 0.029
              when segment = 19 then 0.053
              when segment = 20 then 0.14
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 2  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0007
              when segment = 2 then 0.0022
              when segment = 3 then 0.0035
              when segment = 4 then 0.0047
              when segment = 5 then 0.0061
              when segment = 6 then 0.0073
              when segment = 7 then 0.0085
              when segment = 8 then 0.0097
              when segment = 9 then 0.011
              when segment = 10 then 0.012
              when segment = 11 then 0.013
              when segment = 12 then 0.014
              when segment = 13 then 0.015
              when segment = 14 then 0.017
              when segment = 15 then 0.018
              when segment = 16 then 0.02
              when segment = 17 then 0.025
              when segment = 18 then 0.034
              when segment = 19 then 0.061
              when segment = 20 then 0.15
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 3  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0009
              when segment = 2 then 0.0026
              when segment = 3 then 0.0041
              when segment = 4 then 0.0056
              when segment = 5 then 0.0071
              when segment = 6 then 0.0085
              when segment = 7 then 0.01
              when segment = 8 then 0.011
              when segment = 9 then 0.013
              when segment = 10 then 0.014
              when segment = 11 then 0.015
              when segment = 12 then 0.017
              when segment = 13 then 0.018
              when segment = 14 then 0.02
              when segment = 15 then 0.021
              when segment = 16 then 0.024
              when segment = 17 then 0.029
              when segment = 18 then 0.039
              when segment = 19 then 0.069
              when segment = 20 then 0.17
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 4  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.001
              when segment = 2 then 0.003
              when segment = 3 then 0.0047
              when segment = 4 then 0.0064
              when segment = 5 then 0.0082
              when segment = 6 then 0.0098
              when segment = 7 then 0.011
              when segment = 8 then 0.013
              when segment = 9 then 0.015
              when segment = 10 then 0.016
              when segment = 11 then 0.018
              when segment = 12 then 0.019
              when segment = 13 then 0.021
              when segment = 14 then 0.022
              when segment = 15 then 0.024
              when segment = 16 then 0.027
              when segment = 17 then 0.033
              when segment = 18 then 0.044
              when segment = 19 then 0.077
              when segment = 20 then 0.19
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 5  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0018
              when segment = 2 then 0.0041
              when segment = 3 then 0.0061
              when segment = 4 then 0.008
              when segment = 5 then 0.0099
              when segment = 6 then 0.012
              when segment = 7 then 0.014
              when segment = 8 then 0.016
              when segment = 9 then 0.017
              when segment = 10 then 0.019
              when segment = 11 then 0.021
              when segment = 12 then 0.023
              when segment = 13 then 0.024
              when segment = 14 then 0.026
              when segment = 15 then 0.028
              when segment = 16 then 0.032
              when segment = 17 then 0.038
              when segment = 18 then 0.05
              when segment = 19 then 0.087
              when segment = 20 then 0.21
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 6  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0027
              when segment = 2 then 0.0052
              when segment = 3 then 0.0074
              when segment = 4 then 0.0095
              when segment = 5 then 0.012
              when segment = 6 then 0.014
              when segment = 7 then 0.016
              when segment = 8 then 0.018
              when segment = 9 then 0.02
              when segment = 10 then 0.022
              when segment = 11 then 0.024
              when segment = 12 then 0.026
              when segment = 13 then 0.028
              when segment = 14 then 0.03
              when segment = 15 then 0.032
              when segment = 16 then 0.036
              when segment = 17 then 0.042
              when segment = 18 then 0.056
              when segment = 19 then 0.097
              when segment = 20 then 0.23
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 7  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0035
              when segment = 2 then 0.0063
              when segment = 3 then 0.0087
              when segment = 4 then 0.011
              when segment = 5 then 0.014
              when segment = 6 then 0.016
              when segment = 7 then 0.018
              when segment = 8 then 0.02
              when segment = 9 then 0.023
              when segment = 10 then 0.025
              when segment = 11 then 0.027
              when segment = 12 then 0.029
              when segment = 13 then 0.031
              when segment = 14 then 0.034
              when segment = 15 then 0.036
              when segment = 16 then 0.04
              when segment = 17 then 0.048
              when segment = 18 then 0.063
              when segment = 19 then 0.11
              when segment = 20 then 0.26
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 8  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0043
              when segment = 2 then 0.0074
              when segment = 3 then 0.01
              when segment = 4 then 0.013
              when segment = 5 then 0.015
              when segment = 6 then 0.018
              when segment = 7 then 0.02
              when segment = 8 then 0.023
              when segment = 9 then 0.025
              when segment = 10 then 0.028
              when segment = 11 then 0.03
              when segment = 12 then 0.032
              when segment = 13 then 0.035
              when segment = 14 then 0.037
              when segment = 15 then 0.04
              when segment = 16 then 0.045
              when segment = 17 then 0.053
              when segment = 18 then 0.07
              when segment = 19 then 0.12
              when segment = 20 then 0.28
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 9  
 update vars_adv
  set pd = (case
              when segment = 1 then 0.0052
              when segment = 2 then 0.0085
              when segment = 3 then 0.011
              when segment = 4 then 0.014
              when segment = 5 then 0.017
              when segment = 6 then 0.02
              when segment = 7 then 0.023
              when segment = 8 then 0.025
              when segment = 9 then 0.028
              when segment = 10 then 0.03
              when segment = 11 then 0.033
              when segment = 12 then 0.036
              when segment = 13 then 0.038
              when segment = 14 then 0.041
              when segment = 15 then 0.044
              when segment = 16 then 0.049
              when segment = 17 then 0.058
              when segment = 18 then 0.077
              when segment = 19 then 0.13
              when segment = 20 then 0.31
              else 0
            end)
 where flag = 12 and age = 3 and propensity = 10  


 update vars_adv
 set pd = 0.0004
 where flag = 10;
