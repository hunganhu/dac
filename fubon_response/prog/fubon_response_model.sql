declare @current char(12)
set @current = '20050604'

if not exists (select * from dbo.sysobjects where id = object_id(N'fubon_cc_stmts') and objectproperty(id, N'isusertable') = 1)
 create table fubon_cc_stmts (
    billing_close_date datetime,
    cardholder_status_code char (1),
    idn varchar(11),
    last_payment_amt int,
    monthly_limit_amt int,
    revolving_interest_amt int,
    this_term_expenditure_amt int,
    this_term_min_payment int,
    this_term_total_amt_receivable int,
    this_term_cash_advance_amt int,
    mon_since int,
    bucket_M int,
    cash_adv int,
    bal_dec int default 0,
    bal_pct_diff float,
    min_pay int default 0,
    exp_dec int default 0,
    exp_pct_diff float,
    last_balance int,
    last_min_payment int,
    last_expenditure int,
    last_cash_advance int,
    last_revolving_int_amt int
   );

insert into  fubon_cc_stmts(billing_close_date, idn, cardholder_status_code, last_payment_amt,
 monthly_limit_amt, revolving_interest_amt, this_term_expenditure_amt, this_term_min_payment,
 this_term_total_amt_receivable, this_term_cash_advance_amt)
select billing_close_date, primary_cardholder_id, cardholder_status_code, last_payment_amt,
 monthly_limit_amt, revolving_interest_amt, this_term_expenditure_amt, this_term_min_payment,
 this_term_total_amt_receivable, this_term_cash_advance_amt
from cc_credit_card_statements

update fubon_cc_stmts
   set mon_since = (cast (substring(convert(char(8),billing_close_date,112),1,4) as int) - 1911) * 12 +
                      cast (substring(convert(char(8),billing_close_date,112),5,2) as int);


create index i_idn on fubon_cc_stmts(idn);

declare @now int
declare @i int
set @now = 94 * 12 + 7

 set @i=12
 while @i > 0
    begin
       update fubon_cc_stmts
          set /*last_balance = b.this_term_total_amt_receivable,
              last_min_payment = b.this_term_min_payment,
              last_expenditure = b.this_term_expenditure_amt,
              last_cash_advance = b.this_term_cash_advance_amt,*/
	      last_revolving_int_amt = b.revolving_interest_amt
          from fubon_cc_stmts, fubon_cc_stmts as b
          where fubon_cc_stmts.mon_since = b.mon_since + 1
            and fubon_cc_stmts.idn = b.idn
            and (@now - fubon_cc_stmts.mon_since) = @i
       set @i = @i - 1
    end;

 update fubon_cc_stmts
    set bal_dec  = (case when this_term_total_amt_receivable < last_balance then  1 else 0 end),
        bal_pct_diff  = convert(float,(this_term_total_amt_receivable - last_balance))
                        / (case when last_balance=0 then null else last_balance end),
        min_pay  = (case when (last_payment_amt = last_min_payment) and (last_min_payment > 0) then 1 else 0 end),
        exp_dec  = (case when This_Term_Expenditure_Amt < last_expenditure then 1 else 0 end),
        exp_pct_diff  = convert(float,(This_Term_Expenditure_Amt - last_expenditure))
                        / (case when last_expenditure = 0 then null else last_expenditure end)

-- within 3 months
if exists (select * from dbo.sysobjects where id = object_id(N'fubon_cc_stmts_3') and objectproperty(id, N'isusertable') = 1)
   drop table fubon_cc_stmts_3;
   
select a.*
into fubon_cc_stmts_3
from fubon_cc_stmts a, response_model b
where a.idn = b.Primary_Cardholder_ID
  and a.mon_since > (b.now - 3);

create index i_idn_3 on fubon_cc_stmts_3(idn);
go

-- within 6 months
if exists (select * from dbo.sysobjects where id = object_id(N'fubon_cc_stmts_6') and objectproperty(id, N'isusertable') = 1)
   drop table fubon_cc_stmts_6;

select a.*
into fubon_cc_stmts_6
from fubon_cc_stmts a, response_model b
where a.idn = b.Primary_Cardholder_ID
  and a.mon_since > (b.now - 6);

create index i_idn_6 on fubon_cc_stmts_6(idn);
go

declare @now int
declare @i int
set @now = 94 * 12 + 7

 update fubon_cc_stmts_6
    set bucket_M = (case when min_pay = 1 then 1 else 0 end);

 set @i=7
 while @i > 0
    begin
       update fubon_cc_stmts_6
          set bucket_M = (case when fubon_cc_stmts_6.min_pay = 1 then b.bucket_M + 1 else 0 end)
          from fubon_cc_stmts_6, fubon_cc_stmts_6 as b
          where fubon_cc_stmts_6.mon_since = b.mon_since + 1
            and fubon_cc_stmts_6.idn = b.idn
            and (@now - fubon_cc_stmts_6.mon_since) = @i
       set @i = @i - 1
    end;
go

-- within 9 months
if exists (select * from dbo.sysobjects where id = object_id(N'fubon_cc_stmts_9') and objectproperty(id, N'isusertable') = 1)
   drop table fubon_cc_stmts_9
select a.*
into fubon_cc_stmts_9
from fubon_cc_stmts a, response_model b
where a.idn = b.Primary_Cardholder_ID
  and a.mon_since > (b.now - 9)

create index i_idn_9 on fubon_cc_stmts_9(idn)
go
--8843399

-- within 12 months
create view fubon_cc_stmts_12 as
select * from fubon_cc_stmts
/*
if exists (select * from dbo.sysobjects where id = object_id(N'fubon_cc_stmts_12') and objectproperty(id, N'isusertable') = 1)
   drop table fubon_cc_stmts_12
select a.*
into fubon_cc_stmts_12
from fubon_cc_stmts a, response_model b
where a.idn = b.Primary_Cardholder_ID
  and a.mon_since > (b.now - 12)

create index i_idn_12 on fubon_cc_stmts_12(idn)
go
*/
-------------------------------------------------------------------------------------------------

create table response_model (
	Primary_Cardholder_ID	char(11),
	start_date	datetime,
	end_date	datetime,
	max_cycle	char(6),
	now		int,
	cashADV_ind	int default 0,
	revINT_ind	int default 0,
	expAMT_ind	int default 0,
	Vintage_ind	int default 0,
	wm_flag		int default 0,
	ins_flag	int default 0,
	close_flag	int default 0,
	stmt_3		int default 0,
	stmt_6		int default 0,
	stmt_9		int default 0,
	stmt_12		int default 0,
	card		int,
	gender		int,
	age		int,
	edu		int,
	/* R1 variables */
	fs003_6		float default 0,
	fs072_9		float default 0,
	fs089_3		float default 0,
	fs096_12	float default 0,
	ms035_12	float default 0,
	ms062_12	float default 0,
	rs001		float default 0,
	/* T1 variables */
	fs191_12	float default 0,
	fs204		float default 0,
	/* T2 variables */
	ms023_12	float default 0,
	fs197_9		float default 0,
	/* R1 */
	age_t_r1	float,
	ms035_12_t_r1	float,
	rs001_r		float,
	rs001_r_t_r1	float,
	FS089_3_n	float,
	FS089_3_n_q	float,
	FS089_3_n_q_t_r1	float,
	MS062_12_r	float,
	MS062_12_r_t_r1 float,
	FS096_12_n	float,
	FS096_12_n_t_r1	float,
	FS072_9_n	float,
	FS072_9_n_t_r1	float,
	FS003_6_n	float,
	FS003_6_n_r	float,
	FS003_6_n_r_t_r1	float,
	rscore_r1	float,
	twentile_r1	float,
	/* T1 */
	rs001_t_t1	float,
	fs191_12_n	float,
	fs191_12_n_r	float,
	fs191_12_n_r_t_t1	float,
	FS204_t_t1	float,
	edu_t_t1	float,
	rscore_t1	float,
	twentile_t1	float,
	/* T2 */
	ms023_12_1k	float,
	ms023_12_1k_t2	float,
	FS197_9_n	float,
	FS197_9_n_t_t2	float,
	rscore_t2	float,
	twentile_t2	float,
	/* N1 */
	age_t_n1	float,
	edu_t_n1	float,
	fs204_t_n1	float,
	index7		float,
	rscore_n1	float,
	twentile_n1	float
);

insert response_model(Primary_Cardholder_ID, start_date, end_date)
select  Primary_Cardholder_ID,
        min(Card_Issue_Date) as start_date,
        max(case when Card_Cancel_Date is null then convert(datetime, '99991231', 112)
             else Card_Cancel_Date end) as end_date
from cc_acct_credit_card
group by Primary_Cardholder_ID;
go
--1472554
update response_model
set max_cycle = (case when max_date < '20050501' then '200505'
                      else left(convert(char(8),max_date,112), 6) end)
from (select idn,
             max(Billing_Close_Date) as max_date
      from  fubon_cc_stmts
      group by idn) as a
where response_model.Primary_Cardholder_ID = a.idn;
go
update response_model
set max_cycle =  '200505',
    now = 1133
where max_cycle is null;

--1085985
update response_model
set now = (substring(max_cycle, 1, 4) - 1911) * 12 + substring(max_cycle, 5, 2);
go
-- 1472554
/*
select *
into cc_stmts
from cc_credit_card_statements a,
     response_model b
where a.Primary_Cardholder_ID = b.Primary_Cardholder_ID
  and a.billing_close_date between (@current -10000) and @current
--  and datediff(day, b.start_date, a.billing_close_date) > -30
  and (datediff(day, b.end_date, a.billing_close_date) <= 30
       or This_Term_Total_Amt_Receivable > 0)
*/

update response_model
   set cashADV_ind = (case when t1.cash_advance_amt > 0 then 1 else 0 end),
       revINT_ind = (case when t1.Revolving_Interest_Amt > 0 then 1 else 0 end),
       expAMT_ind = (case when t1.Expenditure_Amt > 0 then 1 else 0 end)
   from (select idn,
                sum(This_Term_Cash_Advance_Amt) as cash_advance_amt,
                sum(Revolving_Interest_Amt) as Revolving_Interest_Amt,
        	sum(This_Term_Expenditure_Amt) as Expenditure_Amt,
 	 from fubon_cc_stmts
 	 group by idn) as t1
   where response_model.Primary_Cardholder_ID = t1.idn;
-- 1085985
update response_model
   set expAMT_ind = (case when t1.Expenditure_Amt > 0 then 1 else 0 end)
   from (select idn,
        	count(*) as Expenditure_Amt
 	 from fubon_cc_stmts
	 where This_Term_Expenditure_Amt > 0
 	 group by idn) as t1
   where response_model.Primary_Cardholder_ID = t1.idn;
--796195

update response_model
   set Vintage_ind = datediff(month,start_date, convert(datetime,@current,112))
/*
declare @current char(12)
set @current = '20050604'

update response_model
   set Vintage_ind = datediff(month,start_date, convert(datetime,@current,112))
--1472554
select top 100 start_date, 
       datediff(month,start_date, convert(datetime,@current,112)),
       (case when datediff(month,start_date, convert(datetime,@current,112)) >= 12 then 1 else 0 end)
from response_model

*/


declare @current char(12)
set @current = '20050604'

update response_model
   set gender = (case when a.gender_code = 'M' then 1 else 0 end),
       age = datediff(year, dob, convert(datetime, @current, 112)) + 1,
       edu = cast(a.education_code as int)
   from cc_party_bank_credit_card a
   where response_model.Primary_Cardholder_ID = a.party_ID;
-- 1455567


update response_model
   set	fs003_6	=0,
	fs072_9 =0,
	fs089_3= 0,
	fs096_12= 0,
	ms035_12= 0,
	ms062_12= 0,
	rs001= 0,
	/* T1 variables */
	fs191_12= 0,
	fs204=0,
	/* T2 variables */
	ms023_12= 0,
	fs197_9= 0,
        stmt_3 = 0,
        stmt_6 = 0,
        stmt_9 = 0,
        stmt_12 = 0

update response_model
   set	cashADV_ind= (case when cashADV_ind is null then 0 else cashADV_ind end),
	revINT_ind= (case when revINT_ind is null then 0 else revINT_ind end),
	expAMT_ind= (case when expAMT_ind is null then 0 else expAMT_ind end),
	Vintage_ind= (case when Vintage_ind is null then 0 else Vintage_ind end),
	wm_flag	= 0,
	ins_flag= 0,
	close_flag= 0

--1472554
/*
select count(*) from dbo.cc_party_bank_credit_card
insert into dbo.cc_party_bank_credit_card
select * from Fubon_Ploan_MKT_2005.dbo.cc_party_bank_credit_card
*/

update response_model
   set stmt_3 = a.v1
   from (select idn, count(*) as v1
         from fubon_cc_stmts_3
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
   
update response_model
   set stmt_6 = a.v1
   from (select idn, count(*) as v1
         from fubon_cc_stmts_6
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--1031041

update response_model
   set stmt_9 = a.v1
   from (select idn, count(*) as v1
         from fubon_cc_stmts_9
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--1064518
   
update response_model
   set stmt_12 = a.v1
   from (select idn, count(*) as v1
         from fubon_cc_stmts_12
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--1085985


update response_model
   set wm_flag = wm_flag + a.v1
   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1
         from bu_customer_investment_trust
         group by customer_id) as a
   where a.customer_id = response_model.Primary_Cardholder_ID;
-- 12939
update response_model
   set wm_flag = wm_flag + a.v1
   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1
         from bu_customer_futures
         group by customer_id) as a
   where a.customer_id = response_model.Primary_Cardholder_ID;
--13499
update response_model
   set wm_flag = wm_flag + a.v1
   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1
         from bu_customer_security_brokage
         group by customer_id) as a
   where a.customer_id = response_model.Primary_Cardholder_ID;
--164003
update response_model
   set wm_flag = wm_flag + a.v1
   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1
         from bu_customer_security_bond
         group by customer_id) as a
   where a.customer_id = response_model.Primary_Cardholder_ID;
--528
update response_model
   set wm_flag = wm_flag + a.v1
   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1
         from bu_customer_bank_bond
         group by customer_id) as a
   where a.customer_id = response_model.Primary_Cardholder_ID;
--364
update response_model
   set wm_flag = wm_flag + a.v1
   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1
         from bu_customer_bank_fund
         group by customer_id) as a
   where a.customer_id = response_model.Primary_Cardholder_ID;
--45736
update response_model
   set ins_flag = ins_flag + a.v1
   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1
         from bu_customer_property_insure
         group by customer_id) as a
   where a.customer_id = response_model.Primary_Cardholder_ID;
--666837
update response_model
   set ins_flag = ins_flag + a.v1
   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1
         from bu_customer_insurance_group
         group by customer_id) as a
   where a.customer_id = response_model.Primary_Cardholder_ID;
--106382
update response_model
   set ins_flag = ins_flag + a.v1
   from (select customer_id, (case when count(*)> 0 then 1 else 0 end) as v1
         from bu_customer_insurance_life
         group by customer_id) as a
   where a.customer_id = response_model.Primary_Cardholder_ID;
--115355
select distinct a.Primary_Cardholder_ID
into FB_close_acct
from cc_acct_credit_card as a left join
    (select distinct Primary_Cardholder_ID
     from cc_acct_credit_card
     where Acct_Status_Code = 0)  as b
on a.Primary_Cardholder_ID = b.Primary_Cardholder_ID
where b.Primary_Cardholder_ID is null
--533932

update response_model
   set close_flag = 1
   from FB_close_acct a
   where response_model.Primary_Cardholder_ID = a.Primary_Cardholder_ID;
-- 533932
update response_model
   set wm_flag = (case when wm_flag > 0 then 1 else 0 end),
       ins_flag = (case when ins_flag > 0 then 1 else 0 end),
       close_flag = (case when close_flag = 1 then 1 else 0 end);
--1472554
/*
 0: others
 1: N1
 2: T1
 3: T2
 4: R1
*/

update response_model
   set card = 0
   where cashADV_ind = 1;
-- 101861  
update response_model
   set card = (case when Vintage_ind < 12 then 40 else 4 end)
   where cashADV_ind = 0
     and revINT_ind = 1;
--177209
update response_model
   set card = (case when Vintage_ind < 12 then 3 else 2 end)
   where cashADV_ind = 0
     and revINT_ind = 0
     and expAMT_ind = 1;
-- 531642 
-- 532083
update response_model
   set card = (case when wm_flag = 1 and ins_flag =0 then 10 else 1 end)
   where cashADV_ind = 0
     and revINT_ind = 0
     and expAMT_ind = 0;
-- 661842  
-- 661401
select card, count(*)
from response_model
group by card
order by card
/*
0	149068
1	620380
2	491900
3	39742
4	171464
*/
/*
0	101861 C
1	619956 N1
2	492317 T1
3	39766  T2
4	171464 R1
10	41445  N2
40	5745   R2

C	101861
N1	619956
N2	41445
R1	171464
R2	5745
T1	492317
T2	39766
*/----------------------------------------------------------------------------------------R1
-- FS003  硈尿Xる度煤程肂Ω计
update response_model
   set fs003_6 = a.v1
   from (select idn, max(bucket_M) as v1
         from fubon_cc_stmts_6
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
-- 1031041

-- FS072  緇肂搭ぶ>=5%る计
update response_model
   set fs072_9 = a.v1
   from (select idn, count(*) as v1
         from fubon_cc_stmts_9
         where -bal_pct_diff >= 0.05
           and bal_dec = 1
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--819971
-- FS089  緇肂ㄏノ瞯>=80%る计
update response_model
   set fs089_3 = a.v1
   from (select idn, count(*) as v1
         from fubon_cc_stmts_3
         where convert(float, this_term_total_amt_receivable) /
              (case when Monthly_Limit_Amt = 0 then NULL else Monthly_Limit_Amt end) >= 0.8
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--102372
-- FS096  碻吏>=4Kる计
update response_model
   set fs096_12 = a.v1
   from (select idn, count(*) as v1
         from fubon_cc_stmts_12
         where revolving_interest_amt >= 4000
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--18608
-- MS035	average utilization ratio (balance /monthly limit)
update response_model
   set ms035_12 = a.v1
   from (select idn, avg(convert(float, this_term_total_amt_receivable) / (case when monthly_limit_amt = 0 then null else monthly_limit_amt end)) as v1
         from fubon_cc_stmts_12
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--1085985
-- MS062	revolving interest amount previous / current
update response_model
   set ms062_12 = a.v1
   from (select idn, max(convert(float, revolving_interest_amt)/(case when last_revolving_int_amt = 0 then NULL else last_revolving_int_amt end)) as v1
         from fubon_cc_stmts_12
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
-- 1085985   
-- RS001	程Ω秨禯さる计
declare @current char(12)
set @current = '20050604'

update response_model
   set rs001 = ((cast(substring(@current, 1, 4) as int)-1911) * 12 +
               (cast(substring(@current, 5, 2) as int))) - 
               ((cast(substring(t1.recent_open_date, 1, 4) as int)-1911) * 12 +
               (cast(substring(t1.recent_open_date, 5, 2) as int)))
   from (select primary_cardholder_id,
                left(convert(char(8), max(Card_Issue_Date), 112), 6) as recent_open_date
         from cc_acct_credit_card
         group by primary_cardholder_id) as t1
   where t1.primary_cardholder_id = response_model.primary_cardholder_id;
--1472554   

 update response_model
    set FS089_3_n=FS089_3 / (case when stmt_3 = 0 then null else stmt_3 end),
        rs001_r = power ((case when rs001 < 0 then null else rs001 end), 0.5),
        MS062_12_r = power ((case when MS062_12 < 0 then null else MS062_12 end), 0.5),
        FS096_12_n = FS096_12 / (case when stmt_12 = 0 then null else stmt_12 end),
        FS072_9_n = FS072_9 / (case when stmt_9 = 0 then null else stmt_9 end),
        FS003_6_n = FS003_6 / (case when stmt_6 = 0 then null else stmt_6 end),
        age_t_r1 = (case when age < 30 then 1
        		 when age < 40 then 2
        		 when age < 50 then 3
                         else  4 end);
go
--1472554
 update response_model
    set ms035_12_t_r1 = (case when ms035_12 > 0.84 then 0.84
	                      else ms035_12 end),
        rs001_r_t_r1 = (case when rs001_r < 3 then 3
        		     when rs001_r > 8.6 then 8.6
	                     else  rs001_r end),
        FS089_3_n_q = FS089_3_n * FS089_3_n,
        MS062_12_r_t_r1 = (case when MS062_12_r is null then 0
        		     when MS062_12_r > 1.2 then 1.2
	                     else MS062_12_r end),
	FS096_12_n_t_r1 = (case when FS096_12_n > 0 then 1
	                      else 0 end),
        FS072_9_n_t_r1 = (case when FS072_9_n is null then 0.44
        		     when FS072_9_n > 0.66 then 0.66
	                     else FS072_9_n end),
        FS003_6_n_r = power ((case when FS003_6 < 0 then null else FS003_6 end), 0.5);
go
--1472554
 update response_model
    set FS089_3_n_q_t_r1 = (case when FS089_3_n_q is null then 0
	                      else FS089_3_n_q end),
        FS003_6_n_r_t_r1 = (case when FS003_6_n_r is null then 0
	                      else FS003_6_n_r end);
go
--1472554
 update response_model
    set rscore_r1 =		0.02929	+
        ms035_12_t_r1	 *	0.12817	+
        age_t_r1	 *   	0.0201	+
        FS089_3_n_q_t_r1 *	0.05321	+
        rs001_r_t_r1	 *     -0.00822	+
        MS062_12_r_t_r1	 *	0.0335	+
        FS096_12_n_t_r1	 *	0.07503	+
        FS072_9_n_t_r1	 *     -0.04747	+
        FS003_6_n_r_t_r1 *	0.02337	
     where card = 4;
--171464
 update response_model
    set twentile_r1 = (case when rscore_r1 is null then 0
                            when rscore_r1 <= -0.00459 then 1
                            when rscore_r1 <= 0.00603 then 2
                            when rscore_r1 <= 0.01403 then 3
                            when rscore_r1 <= 0.02107 then 4
                            when rscore_r1 <= 0.02732 then 5
                            when rscore_r1 <= 0.03314 then 6
                            when rscore_r1 <= 0.03947 then 7
                            when rscore_r1 <= 0.04602 then 8
                            when rscore_r1 <= 0.05321 then 9
                            when rscore_r1 <= 0.06038 then 10
                            when rscore_r1 <= 0.06976 then 11
                            when rscore_r1 <= 0.08236 then 12
                            when rscore_r1 <= 0.10001 then 13
                            when rscore_r1 <= 0.12253 then 14
                            when rscore_r1 <= 0.1454  then 15
                            when rscore_r1 <= 0.17111 then 16
                            when rscore_r1 <= 0.19378 then 17
                            when rscore_r1 <= 0.21221 then 18
                            when rscore_r1 <= 0.23687 then 19
                            else 20 end)
     where card = 4;
--171464
----------------------------------------------------------------------------------------T1
-- FS191  禣蹿搭ぶ>=20%る计
update response_model
   set fs191_12 = a.v1
   from (select idn, count(*) as v1
         from fubon_cc_stmts_12
         where -exp_pct_diff >= 0.2
           and exp_dec = 1
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--768382
-- FS204  炊计
update response_model
   set fs204 = t1.cnt
   from (select a.primary_cardholder_id, count(*) as cnt
         from cc_acct_credit_card_100k a,
              rd_card_type_code b,
              rd_credit_card_product_id c
         where a.card_type_code = b.code
           and a.product_id = c.code
           and not (b.[description] like '%フ%' or c.[description] like '%フ%')
           and not (b.[description] like '%%' or c.[description] like '%%')
         group by primary_cardholder_id) as t1
    where t1.primary_cardholder_id = response_model.Primary_Cardholder_ID;
--63135
 update response_model
    set FS191_12_n=FS191_12 / (case when stmt_12 =0 then null else stmt_12 end),
        rs001_t_t1 = (case when rs001 < 12  then 12
    			   when rs001 > 40  then 40
	                   else rs001 end),
        FS204_t_t1 = (case when FS204 > 2 then 2
	                      else FS204 end),
        edu_t_t1 = (case when edu > 1 then 2
	                 else edu end);
 go
--1472554
 update response_model
    set FS191_12_n_r = power ((case when FS191_12_n < 0 then null else FS191_12_n end), 0.5);
 go
--1472554
 update response_model
    set fs191_12_n_r_t_t1 = (case when fs191_12_n_r > 0  then 1
	                          else 0 end);
 go
--1472554
 update response_model
    set rscore_t1 =		0.04151	+
        rs001_t_t1	  *    -0.00238	+
        fs191_12_n_r_t_t1 *    -0.08967	+
        FS204_t_t1	  *	0.02252	+
        AGE		  *	0.00173	+
        edu_t_t1	  *	0.0525
     where card = 2;
 go
 update response_model
    set twentile_t1 = (case when rscore_t1 is null then 0
                            when rscore_t1 <= 0.02284 then 1
                            when rscore_t1 <= 0.03128 then 2
                            when rscore_t1 <= 0.03733 then 3
                            when rscore_t1 <= 0.04231 then 4
                            when rscore_t1 <= 0.04771 then 5
                            when rscore_t1 <= 0.05226 then 6
                            when rscore_t1 <= 0.05639 then 7
                            when rscore_t1 <= 0.06053 then 8
                            when rscore_t1 <= 0.06549 then 9
                            when rscore_t1 <= 0.07069 then 10
                            when rscore_t1 <= 0.07587 then 11
                            when rscore_t1 <= 0.08061 then 12
                            when rscore_t1 <= 0.08537 then 13
                            when rscore_t1 <= 0.09082 then 14
                            when rscore_t1 <= 0.09704 then 15
                            when rscore_t1 <= 0.1027  then 16
                            when rscore_t1 <= 0.10983 then 17
                            when rscore_t1 <= 0.12002 then 18
                            when rscore_t1 <= 0.13365 then 19
                            else 20 end)
     where card = 2;
go
--492317
----------------------------------------------------------------------------------------T2
-- MS023	average open to buy
update response_model
   set ms023_12 = a.v1
   from (select idn, avg(cast((monthly_limit_amt - this_term_total_amt_receivable) as float)) as v1
         from fubon_cc_stmts_12
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--1085985
-- FS197  禣蹿搭ぶ>=50%る计
update response_model
   set fs197_9 = a.v1
   from (select idn, count(*) as v1
         from fubon_cc_stmts_9
         where -exp_pct_diff >= 0.5
           and exp_dec = 1
         group by idn) as a
   where a.idn = response_model.Primary_Cardholder_ID;
--726668

 update response_model
    set ms023_12_1k = ms023_12 / 1000.0,
        FS197_9_n = FS197_9 / (case when stmt_9 = 0 then null else stmt_9 end);
 go
--1472554
 update response_model
    set ms023_12_1k_t2 = (case when ms023_12_1k < 20 then 20
			       when ms023_12_1k > 200 then 200
	                       else ms023_12_1k end),
        FS197_9_n_t_t2 = (case when FS197_9_n < 0.12 then 0.12
			       when FS197_9_n > 0.33 then 0.33
	                       else FS197_9_n end);
 go
--1472554
 update response_model
    set rscore_t2 =		0.08526	+
        ms023_12_1k_t2	*   -0.00096186	+
        age		*	0.00542	+
        FS197_9_n_t_t2	*	-0.3683
     where card = 3;
go
 update response_model
    set twentile_t2 = (case when rscore_t2 is null then 0
                            when rscore_t2 <= -0.03093 then 1
                            when rscore_t2 <= -0.00919 then 2
                            when rscore_t2 <= 0.00893  then 3
                            when rscore_t2 <= 0.01866  then 4
                            when rscore_t2 <= 0.02823  then 5
                            when rscore_t2 <= 0.04155  then 6
                            when rscore_t2 <= 0.05283  then 7
                            when rscore_t2 <= 0.06393  then 8
                            when rscore_t2 <= 0.07324  then 9
                            when rscore_t2 <= 0.08468  then 10
                            when rscore_t2 <= 0.09477  then 11
                            when rscore_t2 <= 0.10396  then 12
                            when rscore_t2 <= 0.11232  then 13
                            when rscore_t2 <= 0.12383  then 14
                            when rscore_t2 <= 0.13208  then 15
                            when rscore_t2 <= 0.1437   then 16
                            when rscore_t2 <= 0.15823  then 17
                            when rscore_t2 <= 0.17981  then 18
                            when rscore_t2 <= 0.21463  then 19
                            else 20 end)
     where card = 3;
--39766
----------------------------------------------------------------------------------------N1
update response_model
    set age_t_n1 = (case when age < 24  then 24
    			 when age > 32  then 32
	                 else age end),
        FS204_t_n1 = (case when FS204 > 4 then 4
	                      else FS204 end),
	index7 = (1 - wm_flag) * (1 - ins_flag) * close_flag,
        edu_t_n1 = (case when edu = 1 then 1
                         when edu = 2 then 1
                         when edu = 3 then 2
	                 else 3 end);
--1472554
 update response_model
    set rscore_n1 =	  0.25173  +
        edu_t_n1	* 0.06283  +
        age_t_n1	* -0.01055 +
        fs204_t_n1	* 0.01605  +
        gender		* 0.02327  +
        index7		* -0.01884
     where card = 1;
go
 update response_model
    set twentile_n1 = (case when rscore_n1 is null then 0
                            when rscore_n1 <= 0.02095 then 1
                            when rscore_n1 <= 0.03964 then 2
                            when rscore_n1 <= 0.04205 then 3
                            when rscore_n1 <= 0.05477 then 4
                            when rscore_n1 <= 0.0581  then 5
                            when rscore_n1 <= 0.06181 then 6
                            when rscore_n1 <= 0.06639 then 7
                            when rscore_n1 <= 0.07694 then 8
                            when rscore_n1 <= 0.08378 then 9
                            when rscore_n1 <= 0.09525 then 10
                            when rscore_n1 <= 0.10247 then 11
                            when rscore_n1 <= 0.10625 then 12
                            when rscore_n1 <= 0.11588 then 13
                            when rscore_n1 <= 0.11867 then 14
                            when rscore_n1 <= 0.12509 then 15
                            when rscore_n1 <= 0.13457 then 16
                            when rscore_n1 <= 0.14069 then 17
                            when rscore_n1 <= 0.15567 then 18
                            when rscore_n1 <= 0.17368 then 19
                            else 20 end)
     where card = 1;
--619956