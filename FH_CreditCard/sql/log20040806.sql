/* 
drop procedure Assign_AccountAge
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

--print 'Target Month: ' + @target_month
--print 'Next Month: ' + @next_month
--print '6 Month Ago: ' + @six_month_ago
--print '12 Month Ago: ' + @twelve_month_ago
--print '24 Month Ago: ' + @twentyfour_month_ago

 update source
 set age = 0
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >= @six_month_ago;

 update source
 set age = 1
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@twelve_month_ago and
       a.[account_open_date] <@six_month_ago;

 update source
 set age = 2
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@twentyfour_month_ago and
       a.[account_open_date] <@twelve_month_ago;

 update source
 set age = 3
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] <@twentyfour_month_ago;
*/
/*
--328,014
select count(*) from jeff.sample_0405

--328,014
select count(*) from dbo.source where statement_month='200405'

select age, count(*) from jeff.sample_0405
group by age

select age, count(*) from dbo.source where statement_month='200405'
group by age

update source
set age=0
where age is null

-- same 327,519, diff 0, null 495
select count(*)
from jeff.sample_0405 a, source b
where a.masked_id = b.idn and
      b.statement_month = '200405' and
      a.age = b.age

 update source
 set age = 0
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=200405 and
       a.[account_open_date] = 20031201;

--328,014
select count(*) from account;
select count(*)
 from source
 where statement_month = '200405'
   and idn in (select masked_id from account)

-- diff 1362, same 195,148
select count(*)
from jeff.sample_0405 a, source b
where a.masked_id = b.idn and
      b.statement_month = '200405' and
      a.flag <> b.flag


select flag, count(*)
from jeff.sample_0305
group by flag
order by flag

select flag, count(*)
from source
where statement_month = '200305'
group by flag
order by flag

-- same 1612473, diff 0
select count(*)
from jeff.model_all_stmt a, source b 
where a.masked_id = b.idn and
      a.statement_month = b.statement_month and
      a.statement_month <= '200305' and
      a.pay_code <> b.paycode

*/
/*
 declare @target_month char(6)
 declare @yyyymm int
 declare @yyyy int
 declare @mm int
 declare @next_month varchar(10)
 declare @six_month_ago varchar(10)
 set @target_month = '200305'
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
 
 print '6 month ago ' + @six_month_ago
 print 'next month ' + @next_month

--0:28,78508
 select count(*)
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month='200305' and
       a.[account_open_date] > '20021130'

--118,002
 select count(*)
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month='200305' and
       a.[account_open_date] < '20021201'
-- 9
 select count(*)
 from source
 where statement_month='200305' 
   and idn not in (select masked_id from account)


 and
       paycode in ('E', 'F');

 update source
 set flag = 8
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@six_month_ago and
       paycode in ('E', 'F');

 update source
 set flag = 7
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@six_month_ago and
       paycode not in ('E', 'F');

 update source
 set flag = 9
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
      ( account_status=6 and (inactive_date_for_status_6 <@next_month or inactive_date_for_status_6 is null ));
 update source
 set flag = 10
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode NOT in ('E', 'F') and
       0 = (select SUM(interest_charge)
            from source c
            where b.idn = c.idn and
                  c.month_since between b.month_since - 11 and b.month_since) and
       12<= (select count(*)
             from source c
             where b.idn = c.idn and
                   c.month_since between b.month_since - 11 and b.month_since and
                   paycode='X');
 update source
 set flag = 11
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode NOT in ('E', 'F') and
       0 = (select SUM(interest_charge)
            from source c
            where b.idn = c.idn and
            c.month_since between b.month_since - 11 and b.month_since) and
       12> (select count(*)
            from source c
            where b.idn = c.idn and
                  c.month_since between b.month_since - 11 and b.month_since and
                  paycode='X');
 update source
 set flag = 12
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode NOT in ('E', 'F') and
       0 < (select SUM(interest_charge)
            from source c
            where b.idn = c.idn and
                  c.month_since between b.month_since - 11 and b.month_since);
 update source
 set flag = 0
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode in ('E', 'F');
 update source
 set flag = 1
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 1;
 update source
 set flag = 2
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 2;
 update source
 set flag = 3
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 3;
 update source
 set flag = 4
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 4;
 update source
 set flag = 5
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 5;
 update source
 set flag = 6
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due >=6;

*/
/*
--AMMS035_06_r_tran, AMMS035_06_r, AMMS035_06
 update vars
 set AMMS035_06=
 (select  AVG(credit_limit - total_bal_on_bill) /  AVG(credit_limit)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 5 and vars.month_since)
 where age = 1;

 update vars
 set AMMS035_06_r= SQRT(AMMS035_06)
 where age = 1;

 update vars
 set AMMS035_06_r_tran= (case when AMMS035_06_r is NULL then 0
                              when AMMS035_06_r > 1 then 1
                              else AMFS035_06_r
                         end)
 where age = 1;

--AMFS016_03_tran, AMFS016_03
 update vars
 set AMFS016_03=
 (select  count(*)
  from  source a
  where vars.idn=a.idn 
    and a.month_since between vars.month_since - 2 and vars.month_since
    and a.ANNUAL_FEE_ASSESSED > 0)
 where age = 1;

 update vars
 set AMFS016_03_tran= (case
                         when AMFS016_03 > 2 then 2
                         else AMFS016_03
                       end)
 where age = 1;

-- gender_tran
 update vars
 set gender_tran=
 (select  (case when a.gener = 1 then 1
                else 0
           end)
  from  account a
  where vars.idn=a.idn)
 where age = 1;

-- edu_tran
 update vars
 set edu_tran=
 (select  (case when a.Educational_Level not in ('1','2','3') then 1
                else 0
           end)
  from  account a
  where vars.idn=a.idn)
 where age = 1;

-- ownership1
 update vars
 set ownership1=
 (select  (case when a.Home_ownership = 1 then 1
                else 0
           end)
  from  account a
  where vars.idn=a.idn)
 where age = 1;

----------------------------------------------------------------------------
--AMMT061_43_tran, AMMT061_43, AMMS061_03, AMMS061_06
 update vars
 set AMMS061_03=
 (select  AVG((purchase_balance + cash_balance + interest_charge) / credit_limit)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 2 and vars.month_since)
 where age in (2, 3);

 update vars
 set AMMS061_03=
 (select  AVG((purchase_balance + cash_balance + interest_charge) / credit_limit)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 5 and vars.month_since)
 where age in (2, 3);

 update vars
 set AMMT061_43=AMMS061_03 - (AMMS061_06 - AMMS061_03)
 where age in (2, 3);

 update vars
 set AMMT061_43_tran= (case when AMMT061_43 < 0 then 0.02
                            else AMMT061_43
                         end)
 where  age in (2, 3);

--AMFS004_12_tran, AMFS004_12
 update vars
 set AMFS004_12=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 11 and vars.month_since and 
        a.cash_amount > 0)
 where age in (2, 3);

 update vars
 set AMFS004_12_tran= (case when AMFS004_12 > 4 then 4
                            else AMFS004_12
                         end)
 where  age in (2, 3);

--AMMS060_12_q, AMMS060_12
 update vars
 set AMMS060_12=
 (select  AVG(purchase_balance + cash_balance + interest_charge) / AVG(credit_limit) 
  from  source a
  where vars.idn=a.idn and 
        paycode in ('C', 'D', 'E', 'F') and
        a.month_since between vars.month_since - 11 and vars.month_since)
 where age in (2, 3);

 update vars
 set AMMS060_12_q= AMMS060_12 * AMMS060_12
 where age in (2, 3);

--AMMS066_09_tran, AMMS066_09
 update vars
 set AMMS066_09=
 (select  max(payment_amount / (purchase_balance + cash_balance))
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 8 and vars.month_since and 
        a.purchase_balance + a.cash_balance > 0)
 where age in (2, 3);

 update vars
 set AMMS066_09_tran= (case when AMMS066_09 > 2.4 then 2.4
                            else AMMS066_09
                         end)
 where  age in (2, 3);


-- AMFS014_03_q, AMFS014_03
 update vars
 set AMFS014_03=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 2 and vars.month_since and 
        a.paycode in ('E', 'F'))
 where age in (2, 3);

-- AMFS001_06_z,AMFS001_06

 update vars
 set AMFS001_06=
 (select  SUM(Cash_Advance_count)
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 5 and vars.month_since and 
           cash_advance_count > 0)
 where age in (2, 3) and exists (select *
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 5 and vars.month_since and 
           cash_advance_count > 0);
 

--AMFS016_09
 update vars
 set amfs016_09=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 8 and vars.month_since and 
        a.late_fee_assessed > 0)
 where age in (2, 3);

-- edu_tran2
 update vars
 set edu_tran=
 (select  (case when a.Educational_Level not in ('1','2','3','4') then 1
                else 0
           end)
  from  account a
  where vars.idn=a.idn)
 where age in (2, 3);

-- marriage_i2, index2, marriage_t
-- marriage_t
 update vars
 set marriage_t=
 (select  (case when a.marital_status = 1 then 1
                when a.marital_status = 2 then 2
                else 3
           end)
  from  account a
  where vars.idn=a.idn)
 where age in (2, 3);

 update vars
 set index2=
 (select  (case when var.age = 2 then 1
                else 0
           end)
  from  account a
  where vars.idn=a.idn)
 where age in (2, 3);

 update vars
 set marriage_i2= marriage_t * index2
 where age in (2, 3);
-- scores
 update vars
 set score = 0.0566
           + AMMT061_43_tran * 0.17151 *
           + AMFS014-12 * 0.0191
           + AMFS004_12_tran * 0.02875
           + AMMS060_12_q * 0.09258
           + AMMS066_09_tran * -0.04762
           + edu_tran2* 0.02766
           + AMFS014_03_q * 0.03393
           + AMFS001_06_z * -0.047
           + AMFS016_09 * 0.01189
           + marriage_i2 * 0.014
 where age in (2, 3);

 update vars
 set score = 0.38056
           + AMMS035_06r_tran * -0.42027
           + AMFS016_03_tran * 0.06839
           + gender_tran * 0.04519
           + edu_tran * 0.03736
           + ownership1 * -0.02698
 where age = 1;

*/

/*
select statement_month, count(*)
from dbo.source
group by statement_month
order by statement_month

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200405


insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200404

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200403

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200402

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200401

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200312

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200311

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200310

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200309

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200308

insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200307
insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
 select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
   Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
   Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
 from dbo.Stm_200306

-- 200312  514
select count(*)
from source a, dbo.Stm_200312 b
where a.statement_month = '200312'
  and a.idn = b.masked_id
  and a.paycode <> b.pay_code

select   count(*)
from source a, dbo.Stm_200312 b
where a.statement_month between '200306' and '200405'
  and a.idn = b.masked_id
  and a.statement_month = b.statement_month
  and a.paycode <> b.pay_code
group by b.statement_month
order by b.statement_month


select *
from stm_200311
where masked_id in 
(select b.masked_id
from source a, dbo.Stm_200312 b
where a.statement_month = '200312'
  and a.idn = b.masked_id
  and a.paycode <> b.pay_code
)

select b.masked_id
from
     source a, dbo.Stm_200312 b
where a.statement_month = '200312'
  and a.idn = b.masked_id
  and a.paycode <> b.pay_code

select *
from dbo.Stm_200310
where masked_id = 313551

select *
from dbo.Stm_200311
where masked_id = 313551

select *
from dbo.Stm_200312
where masked_id = 313551

select *
from dbo.Stm_200312
where masked_id = 313551

select account_open_Date
from dbo.Acct_200405
where masked_id = 313551

select statement_month, count(*)
from jeff.allstmt_0405
group by statement_month
order by statement_month


select paycode, count(*)
from source 
where statement_month = '200312'
group by paycode
order by paycode

--
A	57002
B	23
C	36275
D	2982
E	1189
F	8482
X	198053
--
select pay_code, count(*)
from stm_200312
group by pay_code
order by pay_code

--
A	56655
B	23
C	36131
D	2977
E	1189
F	8464
X	198567
--
select paycode, count(*)
from source
where statement_month = '200312'
group by paycode
order by paycode
--
A	56656
B	23
C	36135
D	2978
E	1189
F	8466
X	198573
--

select paycode, count(*)
from FW_SCORE.Oliver.source
where statement_month = '200312'
group by paycode
order by paycode

select a.*, b.*
from source a,  dbo.Stm_200312 b
where a.statement_month = '200312'
  and a.idn = b.masked_id
  and a.paycode <> b.pay_code
/*
313681, X, A
313301, X, A
*/ 
/*
select * 
from source
where idn=313681

select *
from dbo.Stm_200311
where masked_id = 313681

select *
 from source as a
 where statement_month = '200312' and
      not exists ( select idn
              from source as b
              where a.idn = b.idn and
                    month_since = a.month_since - 1 );

select count(*)
from  source
where statement_month = '200312'

select count(*)
from  FW_SCORE.Oliver.source
where statement_month = '200312'

select count(*)
from  dbo.Stm_200312

select a.*, b.*
from source a, jeff.sample_0405 b
where a.statement_month='200405'
  and a.idn = b.masked_id
  and a.flag <> b.flag

select a.*, b.*
from source a, dbo.Acct_200405 b
where idn=322141
  and a.idn = b.masked_id
order by statement_month

select a.*, b.*
from source a, jeff.sample_0405 b
where a.statement_month='200405'
  and a.idn = b.masked_id
  and a.age <> b.age

select idn, 
       (case when __cycles_past_due = 1 then 1
             when __cycles_past_due = 2 then 2
             when __cycles_past_due = 3 then 3
             when __cycles_past_due = 4 then 4
             when __cycles_past_due = 5 then 5
             when __cycles_past_due >= 6 then 6
        end) as flag
from source
where statement_month = '200405'
  and flag = 12
  and __cycles_Past_Due >0


alter table source 
add Purchase_balance float,
    Cash_balance float

-- 3440642
update source
set purchase_balance = b.purchase_balance,
    cash_balance = b.cash_balance
from source a, jeff.allstmt_0405 b
where a.statement_month = b.statement_month
  and a.idn = b.masked_id

-- d 0, s 80663
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.amfs001_06 = b.amfs001_06


-- d 0, s 80663
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.amfs014_12 = b.amfs014_12

-- d 0, s 80663
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.amfs014_03 = b.amfs014_03

-- d 0, s 80663
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.amfs004_12 = b.amfs004_12

-- d 0, s 80663
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.amfs016_09 = b.amfs016_09

-- d 0, s 80663
select idn, a.amms060_12, b.amms060_12
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.amms060_12 = b.amms060_12

select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and b.amms060_12 is null

-- diff: 295431     	.88019	.90269

select * 
from source
where idn = 295431
order by statement_month desc

select * 
from jeff.allstmt_0405
where masked_id = 295431
order by statement_month desc


/*
11	1	52530
11	2	27383
11	3	20104
12	1	32710
12	2	22147
12	3	11029
--
select flag, age, count(*)
from oliver.vars_adv
group by flag, age
order by flag, age

-- d 0, s 80663
-- s 21149, d 0
update Oliver.vars_adv
set amms061_03 = 0
--select count(*)
--from Oliver.vars_adv
where amms061_03 is null
  and age in ( 2, 3) 
  and flag in (11, 12)

select count(*)
from jeff.Table_0405_t4
where amms061_03 =0
  and age in ( 2, 3) 
  and flag in (11, 12)

select count(*)
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.flag in (11, 12)
  and a.amms061_03 = b.amms061_03

select b.idn, a.amms061_03, b.amms061_03
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.amms061_03 <> b.amms061_03
/*
13         	.20386	.61157
25         	.07151	.14301
--
select * 
from source
where idn = 13
order by statement_month desc

select * 
from jeff.allstmt_0405
where masked_id = 13
order by statement_month desc

-- d 0, s 80663
select count(*)
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.flag in (11, 12)
  and a.amms061_06 = b.amms061_06


-- d 0, s 80663,
select count(*)
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in ( 2, 3) 
  and a.flag in (11, 12)
  and a.amms066_09 = b.amms066_09 

-- d 0, s 85240
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and a.amfs016_03 = b.amfs016_03

-- d 189, s 85051
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and a.amms035_06 <> b.amms035_06

select *
from jeff.allstmt_0405
where masked_id = 313728
  and statement_month between 200312 and 200405




--  0,000005 < #0 < 0.00001
--284118.0	.98294937	.98294938

select idn, a.amms035_06, b.amms035_06
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and a.amms035_06 <> b.amms035_06
--  and abs(a.amms035_06 - b.amms035_06) > 0.000005


-- #0 > 0.0001,#1 > 0.00003, #4> 0.00002, #23 > 0.0000001 
select count(*)
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and abs(a.amms035_06_r - b.amms035_06_r) > 0.0000001

-- #0 > 0.0001, #23 > 0.00001, #1 > 0.0
select count(*)
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and abs(a.amms035_06_r_tran - b.amms035_06_r_tran) > 0.0000001

-- #0 > 0.000001
select count(*)
from jeff.table_0405_t a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and abs(a.amfs016_03_tran - b.amfs016_03_tran) > 0.000001

-- d 0, s 85240
select count(*)
from jeff.table_0405_t a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and a.gender_tran = b.gender_tran

-- d 0, s 85240
select count(*)
from jeff.table_0405_t a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and a.edu_tran = b.edu_tran

-- d 0, s 85240
select count(*)
from jeff.table_0405_t a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and a.ownership1 = b.ownership1

-- #0 > 0.00000001, #85240 < 0.00000001
select a.*, b.*
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age = 1 
  and abs(a.score5 - b.score) < 0.00000001

select *
from jeff.table_0405_t3
where masked_id in (196798.0, 10083.0,11937.0)

select *
from Oliver.vars
where idn in (196798.0, 10083.0,11937.0)

-- d 24199, s 56464
select count(*)
--select a.amms061_03, a.amms061_06, a.ammt061_43,  b.amms061_03, b.amms061_06, b.ammt061_43
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.ammt061_43 <> b.ammt061_43

select count(*)
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and abs( a.ammt061_43 - b.ammt061_43) > 0.00001

-- d 0, s 80653
select count(*)
select a.amms061_03, a.amms061_06, a.ammt061_43, a.ammt061_43_tran, b.amms061_03, b.amms061_06, b.ammt061_43, a.ammt061_43_tran
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.ammt061_43_tran <> b.ammt061_43_tran


-- d 0, s 80663
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.amfs004_12_tran = b.amfs004_12_tran

-- d 31765, s 48907, #0 > 0.000000005
select a.masked_id, a.amms060_12, b.amms060_12, a.amms060_12_q, b.amms060_12_q
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and abs(a.amms060_12_q - b.amms060_12_q) > 0.0000000005
--  and a.amms060_12_q = b.amms060_12_q

-- 295431.0	.88019	.90269
select a.masked_id, a.amms060_12, b.amms060_12
from jeff.table_0405_t3 a, jeff.table_0405 b
where a.masked_id = b.masked_id
  and a.age in (2, 3)
  and a.masked_id = 295431

-- d 0, s 80652
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.amms066_09_tran = b.amms066_09_tran

select a.masked_id, a.amms066_09, b.amms066_09, a.amms066_09_tran, b.amms066_09_tran
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.amms066_09_tran <> b.amms066_09_tran

select a.masked_id, a.amms066_09, b.amms066_09
from jeff.table_0405_t4 a, jeff.table_0405 b
where a.masked_id = b.masked_id
  and a.age in (2, 3)
  and a.amms066_09 <> b.amms066_09

-- d 0, s 80663
select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.edu_tran2 = b.edu_tran2

select count(*)
from jeff.table_0405_t4 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.educational_level = b.educational_level

select educational_level, count(*)
from jeff.Table_0405_t
group by educational_level
order by educational_level

select edu_tran2, count(*)
from jeff.Table_0405_t3
group by edu_tran2
order by edu_tran2

select edu_tran2, count(*)
from Oliver.vars
group by edu_tran2
order by edu_tran2

-- d 0, s 80663
select count(*)
from jeff.table_0405_t3 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.amfs014_03_q = b.amfs014_03_q

-- d 0, s 80663
select count(*)
from jeff.table_0405_t3 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.amfs001_06_z = b.amfs001_06_z

-- d 0, s 80663
select count(*)
from jeff.table_0405_t3 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.index2 = b.index2

-- d 0, s 80663
select count(*)
from jeff.table_0405_t3 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.marriage_i2 = b.marriage_i2
-- d 3, s 80663
select a.masked_id,a.index2, a.marriage_t, a.marriage_i2, b.index2, b.marriage_t, b.marriage_i2
from jeff.table_0405_t3 a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.marriage_i2 = b.marriage_i2

-- d0, sa 80663
select count(*)
from jeff.table_0405_t a, oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and a.marital_status = b.marital_status


select marriage, count(*)
from jeff.table_0405_t
group by marriage
order by marriage

select marital_status, count(*)
from jeff.table_0405_t
group by marital_status
order by marital_status

-- #0 > 1.0 e -9
select count(*)
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (2, 3)
  and b.flag in (11, 12)
  and abs(a.bscore_23 - b.score) > 0.000000001

--#0 > 1.0 e-8
select count(*)
from jeff.table_0405_t4 a, Oliver.vars_adv b
where a.masked_id = b.idn
  and a.age in (1)
  and b.flag in (11, 12)
  and abs(a.score5 - b.score) > 0.00000001

select top 10 *
from Oliver.source_adv
where statement_month = '200405'

select flag, count(*)
from Oliver.vars
group by flag
order by flag

select flag, count(*)
from Oliver.source_adv
where statement_month = '200405'
group by flag
order by flag


select flag, count(*)
from Oliver.vars_adv
group by flag
order by flag

select flag, age, count(*)
from Oliver.vars_adv
group by flag, age
order by flag, age

select flag, age, count(*)
from jeff.Table_0405_t4
group by flag, age
order by flag, age
/*
11	1	52530
11	2	27383
11	3	20104
12	1	32710
12	2	22147
12	3	11029

--
select statement_month, paycode, count(*)
from dbo.source_0405
where statement_month between '200306' and '200405'
group by statement_month, paycode
order by statement_month, paycode

select statement_month, paycode, count(*)
from oliver.source_adv
where statement_month between '200306' and '200405'
group by statement_month, paycode
order by statement_month, paycode

select statement_month, count(*)
from jeff.allstmt_0405
group by statement_month
order by statement_month

insert into jeff.allstmt_0405
select *
from dbo.Stm_200305


select a.idn,
       a.amms035_06_r_tran, a.amfs016_03_tran, a.gender_tran, a.edu_tran, a.ownership1, a.score,
       b.amms035_06_r_tran, b.amfs016_03_tran, b.gender_tran, b.edu_tran, b.ownership1, b.score5
from Oliver.vars_adv a, jeff.table_0405_t4 b 
where a.idn = b.masked_id
  and a.age =1
  and a.flag in (11, 12)
order by idn


select a.idn,
       a.ammt061_43_tran, a.amfs014_12, a.amfs004_12_tran, a.amms060_12_q, a.amms066_09_tran, a.edu_tran2, a.amfs014_03_q, a.amfs001_06_z, a.amfs016_09, a.marriage_i2, a.score,
       b.ammt061_43_tran, b.amfs014_12, b.amfs004_12_tran, b.amms060_12_q, b.amms066_09_tran, b.edu_tran2, b.amfs014_03_q, b.amfs001_06_z, b.amfs016_09, b.marriage_i2, b.bscore_23
from Oliver.vars_adv a, jeff.table_0405_t4 b 
where a.idn = b.masked_id
  and a.age in (2, 3)
  and a.flag in (11, 12)
order by idn

alter table vars_adv
-- drop column pd
 add pd decimal (7,4);

select pd, count(*)
from vars_adv
group by pd
order by pd

drop table dbo.Stm_200404

select top 1 *
into Stm_200407
from Stm_200403

delete from Stm_200404

select *
into fuhwa_new.oliver.stm_200407
from stm_200407

insert into Stm_200404 (customer_id, masked_id, Statement_month, Account_Type, Total___open_cards_with_Fuhwa, Account_Status, Inactive_date_for_status_6, APR, Credit_Limit, Cash_Credit_Limit, Purchase_Average_Daily_Balance, Cash_Average_Daily_Balance, Purchase_balance, Cash_balance, Purchase_amount, Purchase_count, Purchase_Credited_Amount, Cash_amount, Cash_Advance_count, Cash_Credited_Amount, other_x_sell_purchase_amount, other_x_sell_purchase_count, other_x_sell_purchase_Credited, Offshore_purchase_amount, Offshore_purchase_count, Offshore_purchase_Credited_amou, Offshore_Cash_amount, Offshore_Cash_count, Offshore_Cash_Credited_amount, Interest_charge, Interest_charge_credited, Cash_Advance_Fee_Assessed, Cash_Advance_Fee_Credited, Annual_Fee_Assessed, Annual_Fee_Credited, Late_Fee_Assessed, Late_Fee_Credited, Other_Fee_Assessed, Other_Fee_Credited, Other_Fee_count, Fraud_Credit_Amount, Fraud_Cash_Credit_Amount, Bonus_Points_claimed, Unclaimed_Bonus_Points_balance, Rebate__cash_, Payment_due_date, Payment_amount, Payment_count, Last_payment_date, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill)
select customer_id, masked_id, Statement_month, Account_Type, Total___open_cards_with_Fuhwa, Account_Status, Inactive_date_for_status_6, APR, Credit_Limit, Cash_Credit_Limit, Purchase_Average_Daily_Balance, Cash_Average_Daily_Balance, Purchase_balance, Cash_balance, Purchase_amount, Purchase_count, Purchase_Credited_Amount, Cash_amount, Cash_Advance_count, Cash_Credited_Amount, other_x_sell_purchase_amount, other_x_sell_purchase_count, other_x_sell_purchase_Credited, Offshore_purchase_amount, Offshore_purchase_count, Offshore_purchase_Credited_amou, Offshore_Cash_amount, Offshore_Cash_count, Offshore_Cash_Credited_amount, Interest_charge, Interest_charge_credited, Cash_Advance_Fee_Assessed, Cash_Advance_Fee_Credited, Annual_Fee_Assessed, Annual_Fee_Credited, Late_Fee_Assessed, Late_Fee_Credited, Other_Fee_Assessed, Other_Fee_Credited, Other_Fee_count, Fraud_Credit_Amount, Fraud_Cash_Credit_Amount, Bonus_Points_claimed, Unclaimed_Bonus_Points_balance, Rebate__cash_, Payment_due_date, Payment_amount, Payment_count, Last_payment_date, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill
from fw_score.Oliver.Stm_200404

insert into Stm_200405 (customer_id, masked_id, Statement_month, Account_Type, Total___open_cards_with_Fuhwa, Account_Status, Inactive_date_for_status_6, APR, Credit_Limit, Cash_Credit_Limit, Purchase_Average_Daily_Balance, Cash_Average_Daily_Balance, Purchase_balance, Cash_balance, Purchase_amount, Purchase_count, Purchase_Credited_Amount, Cash_amount, Cash_Advance_count, Cash_Credited_Amount, other_x_sell_purchase_amount, other_x_sell_purchase_count, other_x_sell_purchase_Credited, Offshore_purchase_amount, Offshore_purchase_count, Offshore_purchase_Credited_amou, Offshore_Cash_amount, Offshore_Cash_count, Offshore_Cash_Credited_amount, Interest_charge, Interest_charge_credited, Cash_Advance_Fee_Assessed, Cash_Advance_Fee_Credited, Annual_Fee_Assessed, Annual_Fee_Credited, Late_Fee_Assessed, Late_Fee_Credited, Other_Fee_Assessed, Other_Fee_Credited, Other_Fee_count, Fraud_Credit_Amount, Fraud_Cash_Credit_Amount, Bonus_Points_claimed, Unclaimed_Bonus_Points_balance, Rebate__cash_, Payment_due_date, Payment_amount, Payment_count, Last_payment_date, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill)
select customer_id, masked_id, Statement_month, Account_Type, Total___open_cards_with_Fuhwa, Account_Status, Inactive_date_for_status_6, APR, Credit_Limit, Cash_Credit_Limit, Purchase_Average_Daily_Balance, Cash_Average_Daily_Balance, Purchase_balance, Cash_balance, Purchase_amount, Purchase_count, Purchase_Credited_Amount, Cash_amount, Cash_Advance_count, Cash_Credited_Amount, other_x_sell_purchase_amount, other_x_sell_purchase_count, other_x_sell_purchase_Credited, Offshore_purchase_amount, Offshore_purchase_count, Offshore_purchase_Credited_amou, Offshore_Cash_amount, Offshore_Cash_count, Offshore_Cash_Credited_amount, Interest_charge, Interest_charge_credited, Cash_Advance_Fee_Assessed, Cash_Advance_Fee_Credited, Annual_Fee_Assessed, Annual_Fee_Credited, Late_Fee_Assessed, Late_Fee_Credited, Other_Fee_Assessed, Other_Fee_Credited, Other_Fee_count, Fraud_Credit_Amount, Fraud_Cash_Credit_Amount, Bonus_Points_claimed, Unclaimed_Bonus_Points_balance, Rebate__cash_, Payment_due_date, Payment_amount, Payment_count, Last_payment_date, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill
from fw_score.Oliver.Stm_200405
insert into Stm_200406 (customer_id, masked_id, Statement_month, Account_Type, Total___open_cards_with_Fuhwa, Account_Status, Inactive_date_for_status_6, APR, Credit_Limit, Cash_Credit_Limit, Purchase_Average_Daily_Balance, Cash_Average_Daily_Balance, Purchase_balance, Cash_balance, Purchase_amount, Purchase_count, Purchase_Credited_Amount, Cash_amount, Cash_Advance_count, Cash_Credited_Amount, other_x_sell_purchase_amount, other_x_sell_purchase_count, other_x_sell_purchase_Credited, Offshore_purchase_amount, Offshore_purchase_count, Offshore_purchase_Credited_amou, Offshore_Cash_amount, Offshore_Cash_count, Offshore_Cash_Credited_amount, Interest_charge, Interest_charge_credited, Cash_Advance_Fee_Assessed, Cash_Advance_Fee_Credited, Annual_Fee_Assessed, Annual_Fee_Credited, Late_Fee_Assessed, Late_Fee_Credited, Other_Fee_Assessed, Other_Fee_Credited, Other_Fee_count, Fraud_Credit_Amount, Fraud_Cash_Credit_Amount, Bonus_Points_claimed, Unclaimed_Bonus_Points_balance, Rebate__cash_, Payment_due_date, Payment_amount, Payment_count, Last_payment_date, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill)
select customer_id, masked_id, Statement_month, Account_Type, Total___open_cards_with_Fuhwa, Account_Status, Inactive_date_for_status_6, APR, Credit_Limit, Cash_Credit_Limit, Purchase_Average_Daily_Balance, Cash_Average_Daily_Balance, Purchase_balance, Cash_balance, Purchase_amount, Purchase_count, Purchase_Credited_Amount, Cash_amount, Cash_Advance_count, Cash_Credited_Amount, other_x_sell_purchase_amount, other_x_sell_purchase_count, other_x_sell_purchase_Credited, Offshore_purchase_amount, Offshore_purchase_count, Offshore_purchase_Credited_amou, Offshore_Cash_amount, Offshore_Cash_count, Offshore_Cash_Credited_amount, Interest_charge, Interest_charge_credited, Cash_Advance_Fee_Assessed, Cash_Advance_Fee_Credited, Annual_Fee_Assessed, Annual_Fee_Credited, Late_Fee_Assessed, Late_Fee_Credited, Other_Fee_Assessed, Other_Fee_Credited, Other_Fee_count, Fraud_Credit_Amount, Fraud_Cash_Credit_Amount, Bonus_Points_claimed, Unclaimed_Bonus_Points_balance, Rebate__cash_, Payment_due_date, Payment_amount, Payment_count, Last_payment_date, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill
from fw_score.Oliver.Stm_200406

insert into Acct_200405 (customer_id, account_id, account_type, account_open_date, account_close_date, cycle_date, secured_amount, CHANNEL, __of_payment_terms, initial_loan_amount, setup_fee, maintenance_fee,Maintenance_Fee_Term, interest_rate, BIRTHDAY, resident_phone, residence_city___province, residence_ZIP, GENDER, Annual_income, Home_ownership, Educational_Level, Marital_status, Occupation_status, __months_in_current_residence, __months_with_current_occupatio)
select [Customer ID], [Account ID], [Account Type], [Account Open Date], [Account Close Date], [Cycle Date], [Secured Amount], [Channel], [# of payment terms], [Initial Loan Amount], [Setup Fee], [Maintenance Fee], [Maintenance Fee Term], [Interest Rate], [Birthday], [Resident Phone], [Residence City / Province], [Residence ZIP], [Gender], [Annual Income], [Home Ownership], [Educational Level], [Marital status], [Occupation Status], [# months in current residence], [# months with current occupation]
from fw_score.dbo.account_200405

insert into Stm_200407 (customer_id, masked_id, Statement_month, Account_Type, Total___open_cards_with_Fuhwa, Account_Status, Inactive_date_for_status_6, APR, Credit_Limit, Cash_Credit_Limit, Purchase_Average_Daily_Balance, Cash_Average_Daily_Balance, Purchase_balance, Cash_balance, Purchase_amount, Purchase_count, Purchase_Credited_Amount, Cash_amount, Cash_Advance_count, Cash_Credited_Amount, other_x_sell_purchase_amount, other_x_sell_purchase_count, other_x_sell_purchase_Credited, Offshore_purchase_amount, Offshore_purchase_count, Offshore_purchase_Credited_amou, Offshore_Cash_amount, Offshore_Cash_count, Offshore_Cash_Credited_amount, Interest_charge, Interest_charge_credited, Cash_Advance_Fee_Assessed, Cash_Advance_Fee_Credited, Annual_Fee_Assessed, Annual_Fee_Credited, Late_Fee_Assessed, Late_Fee_Credited, Other_Fee_Assessed, Other_Fee_Credited, Other_Fee_count, Fraud_Credit_Amount, Fraud_Cash_Credit_Amount, Bonus_Points_claimed, Unclaimed_Bonus_Points_balance, Rebate__cash_, Payment_due_date, Payment_amount, Payment_count, Last_payment_date, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill)
select customer_id, masked_id, Statement_month, Account_Type, Total___open_cards_with_Fuhwa, Account_Status, Inactive_date_for_status_6, APR, Credit_Limit, Cash_Credit_Limit, Purchase_Average_Daily_Balance, Cash_Average_Daily_Balance, Purchase_balance, Cash_balance, Purchase_amount, Purchase_count, Purchase_Credited_Amount, Cash_amount, Cash_Advance_count, Cash_Credited_Amount, other_x_sell_purchase_amount, other_x_sell_purchase_count, other_x_sell_purchase_Credited, Offshore_purchase_amount, Offshore_purchase_count, Offshore_purchase_Credited_amou, Offshore_Cash_amount, Offshore_Cash_count, Offshore_Cash_Credited_amount, Interest_charge, Interest_charge_credited, Cash_Advance_Fee_Assessed, Cash_Advance_Fee_Credited, Annual_Fee_Assessed, Annual_Fee_Credited, Late_Fee_Assessed, Late_Fee_Credited, Other_Fee_Assessed, Other_Fee_Credited, Other_Fee_count, Fraud_Credit_Amount, Fraud_Cash_Credit_Amount, Bonus_Points_claimed, Unclaimed_Bonus_Points_balance, Rebate__cash_, Payment_due_date, Payment_amount, Payment_count, Last_payment_date, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill
from fuhwa_new.dbo.stm_200407

insert into Acct_200407 (customer_id, account_id, account_type, account_open_date, account_close_date, cycle_date, secured_amount, CHANNEL, __of_payment_terms, initial_loan_amount, setup_fee, maintenance_fee,Maintenance_Fee_Term, interest_rate, BIRTHDAY, resident_phone, residence_city___province, residence_ZIP, GENDER, Annual_income, Home_ownership, Educational_Level, Marital_status, Occupation_status, __months_in_current_residence, __months_with_current_occupatio)
select [Customer ID], [Account ID], [Account Type], [Account Open Date], [Account Close Date], [Cycle Date], [Secured Amount], [Channel], [# of payment terms], [Initial Loan Amount], [Setup Fee], [Maintenance Fee], [Maintenance Fee Term], [Interest Rate], [Birthday], [Resident Phone], [Residence City / Province], [Residence ZIP], [Gender], [Annual Income], [Home Ownership], [Educational Level], [Marital status], [Occupation Status], [# months in current residence], [# months with current occupation]
from fw_score.dbo.account

select * 
into fuhwa_new.oliver.Stm_200406
from oliver.Stm_200406

 set flag = (case when __cycles_past_due = 1 then 1
             when __cycles_past_due = 2 then 2
             when __cycles_past_due = 3 then 3
             when __cycles_past_due = 4 then 4
             when __cycles_past_due = 5 then 5
             when __cycles_past_due >= 6 then 6
             end)


 select idn, flag, __cycles_past_due
 from source a
 where a.statement_month = '200407'
   and a.flag = 12
   and a.idn  not in (select idn
                      from vars_adv
                      where flag = 12)
*/
/*
(B142907583, E141346505, T148966138, A228589092, B142900170, H241573090)
B142907583      	12	45.0
E141346505      	12	71.0
T148966138      	12	53.0
A228589092      	12	22.0
B142900170      	12	45.0
H241573090      	12	1.0
--


select a.*, b.*
from statement a, account b
where a.[customer id] in ('B142907583', 'E141346505', 'T148966138', 'A228589092', 'B142900170', 'H241573090')
  and a.[customer id] = b.[customer id]
  and a.[statement month] = '200407'

select __cycles_past_due, count(*)
from source 
where statement_month = '200407'
group by __cycles_past_due
order by __cycles_past_due


alter table oliver.vars_adv
add beg_revolver int 

update oliver.vars_adv
set beg_revolver = (case when b.interest_charge > 0 then 1 else 0 end)
--select count(*)
from  oliver.source_adv b
where oliver.vars_adv.idn = b.idn
  and b.statement_month = '200407'
*/
/*
0	1	47
0	2	27
0	3	19
1	1	1303
1	2	1179
1	3	412
2	1	354
2	2	348
2	3	52
3	1	287
3	2	330
3	3	48
4	1	292
4	2	338
4	3	53
5	1	297
5	2	285
5	3	41
6	1	935
6	2	2801
6	3	2459
7	0	17229
8	0	199
9	1	7584
9	2	21331
9	3	51078
10	1	5397
10	2	37862
10	3	12921   56180   age 2&3 90446
11	1	45531
11	2	30249
11	3	20278   96058   50527
12	1	31636
12	2	28733
12	3	11186   71555   39919

select flag, age, count(*)
from oliver.vars_adv
group by flag, age
order by flag, age

-- s 214849, d 8944, tt 223793
-- d 159, s 223634
select count(*)
from fuhwa_new.jeff.Final_0407 a, Oliver.vars_adv b
where a.flag in (10,11,12)
  and a.idn = b.idn
  and a.pd = b.pd

select a.idn, a.flag, a.age, a.rscore, a.risk, a.propensity, a.pd, b.flag, a.age, b.score, b.segment, b.propensity, b.pd
from fuhwa_new.jeff.Final_0407 a, Oliver.vars_adv b
where a.flag in (10,11,12)
  and a.idn = b.idn
  and a.pd <> b.pd

 update oliver.vars_adv
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


-- s 9403, d 158210, t167613
select count(*)
from jeff.Final_0407 a, Oliver.vars_adv b
where a.flag in (11,12)
  and a.idn = b.idn
  and a.rscore <> b.score

-- # 0 > 0.00000001
select count(*)
from jeff.Final_0407 a, Oliver.vars_adv b
where a.flag in (11,12)
  and a.idn = b.idn
  and abs(a.rscore - b.score) > 0.000000001

-- propensity, s 71555, d 0
select a.flag, a.age, a.beg_rev_adb, a.propensity, b.flag, b.age,b.beg_rev_adb, b.propensity
select count(*)
from fuhwa_new.jeff.Final_0407 a, Oliver.vars_adv b
where a.flag in (12)
  and a.idn = b.idn
  and a.propensity = b.propensity

-- s 163649, d 3964
select count(*)
from fuhwa_new.jeff.Final_0407 a, Oliver.vars_adv b
where a.flag in (11,12)
  and a.idn = b.idn
  and a.risk <> b.segment

select a.flag, a.age, a.rscore, a.risk, b.flag, b.age, b.score, b.segment
from fuhwa_new.jeff.Final_0407 a, Oliver.vars_adv b
where a.flag in (11,12)
  and a.idn = b.idn
  and a.risk <> b.segment

--109328
select count(*)
from oliver.vars_adv
where pd is null
*/
--167619
select count(*)
from jeff.table_0407

select flag, count(*)
from jeff.table_0407
group by flag

/*
200308	249331
200309	269157
200310	286442
200311	297980
200312	303995
200401	313785
200402	317601
200403	321109
200404	324660
200405	328014
200406	330705
200407	333121

*/
select a.statement_month, count(*)
from jeff.allstmt_0407 a, fw_score.oliver.source_adv b
where a.customer_id = b.idn
  and a.statement_month = b.statement_month
  and a.pay_code = b.paycode
group by a.statement_month
order by a.statement_month

select a.statement_month, count(*)
from jeff.allstmt_0407 a
group by a.statement_month
order by a.statement_month

select a.statement_month, count(*)
from fw_score.oliver.source a, fw_score.oliver.source_adv b
where a.idn = b.idn
  and a.statement_month = b.statement_month
  and a.paycode = b.paycode
group by a.statement_month
order by a.statement_month

select a.[statement month], count(*)
from fw_score.dbo.statement a
group by a.[statement month]
order by a.[statement month]

 
--flag, s 167613, d 0
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag = b.flag

-- AMFS001_06, d 0, s 90446
select b.idn, a.amms061_03, b.amms061_03
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMFS001_06 = b.AMFS001_06

--AMFS004_12, s 90446, d 0
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMFS004_12 = b.AMFS004_12

-- AMFS014_03, s 90446, d 
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMFS014_03 = b.AMFS014_03

-- AMFS014_12 d , s 90446
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMFS014_12 = b.AMFS014_12

-- AMFS016_03, s 77167, d 0
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (1)
  and a.AMFS016_03 = b.AMFS016_03

--AMFS016_09, s 90446, d 
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMFS016_09 = b.AMFS016_09

--AMMS002_06, the variable is not in model.
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.AMMS002_06 = b.AMMS002_06

--AMMS034_06, the variable is not in model.
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.AMMS034_06 <> b.AMMS034_06

--AMMS035_06 d 6 > 0.00000001, 77161,
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (1)
  and a.AMMS035_06 = b.AMMS035_06

select a.customer_id, a.AMMS035_06 , b.AMMS035_06
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (1)
  and a.AMMS035_06 <> b.AMMS035_06
  and abs(a.AMMS035_06 - b.AMMS035_06) > 0.00001


--AMMS060_12 s 90446,d 
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMS060_12 = b.AMMS060_12
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMS060_12 is null 
  and b.AMMS060_12 = 0
-- #0> 0.000002
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and abs(a.AMMS060_12 - b.AMMS060_12) > 0.0000001

--AMMS061_03 s 90446,
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMS061_03 = b.AMMS061_03

select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMS061_03 is null
  and b.AMMS061_03 = 0

--AMMS061_06 s 90446
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMS061_06 = b.AMMS061_06

select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMS061_06 is null
  and b.AMMS061_06 = 0

--AMMS066_09 s 90446
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMS066_09 = b.AMMS066_09

select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMS066_09 is null
  and b.AMMS066_09 = 0

--AMMT061_43 s 90446,
select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMT061_43 = b.AMMT061_43

select count(*)
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMT061_43 is null
  and b.AMMT061_43 = 0

select a.customer_id, a.amms061_03, a.amms061_06, a.ammt061_43,b.amms061_03, b.amms061_06, b.ammt061_43
from jeff.table_0407 a, fw_score.oliver.vars_adv b
where a.customer_id = b.idn
  and a.flag in ( 11,12) 
  and a.age in (2,3)
  and a.AMMT061_43 is null
  and b.AMMT061_43 <> 0
