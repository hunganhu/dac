SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER   view application as
select * from TF_KHJ.dbo.application where idn in
     (select idn from TF_KHJ.dbo.jas002_200402)
GO

ALTER  view bam085(app_sn, data_time, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan) as
select idn, TIME_STAMP, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan
from TF_KHJ.dbo.BAM085_200402
GO

ALTER  view jas002 (app_sn, data_time, EVER_DELINQUENT, DELINQUENT_DATE, EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, EVER_STOP_CARD, STOP_CARD_DATE) as
select idn, TIME_STAMP, EVER_DELINQUENT, DELINQUENT_DATE, EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, EVER_STOP_CARD, STOP_CARD_DATE
from TF_KHJ.dbo.JAS002_200402
GO

ALTER  view krm001(app_sn, data_time, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, limit_type, usage_type, secure) as
select idn, TIME_STAMP, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, limit_type, usage_type, secure
from TF_KHJ.dbo.KRM001_200402
GO

ALTER  view krm023(app_sn, data_time, yrmon, issue, issue_name, kr_code, limit, payment, cash, pay_code) as
select idn, TIME_STAMP, yrmon, issue, issue_name, kr_code, limit, payment, cash, pay_code
from TF_KHJ.dbo.KRM023_200402
GO

ALTER  view stm001(app_sn, data_time, query_date, bank_code, bank_name, item_list) as
select idn, TIME_STAMP, query_date, bank_code, bank_name, item_list
from TF_KHJ.dbo.STM001_200402
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

use oliver
delete from tf_application

CREATE TABLE tf_application (
	IDN char (6),
	martial_status char (4),
	edu char (14),
	is_secretive char (2),
	actual_amount int,
	interest_rate float,
	periods int,
	loan1_interest_payment1 int,
	loan1_interest_payment2 int,
	loan1_interest_payment3 int,
	loan1_interest_payment4 int,
	loan1_interest_payment5 int,
	loan1_interest_payment6 int,
	loan2_interest_payment1 int,
	loan2_interest_payment2 int,
	loan2_interest_payment3 int,
	loan2_interest_payment4 int,
	loan2_interest_payment5 int,
	loan2_interest_payment6 int,
	loan3_interest_payment1 int,
	loan3_interest_payment2 int,
	loan3_interest_payment3 int,
	loan3_interest_payment4 int,
	loan3_interest_payment5 int,
	loan3_interest_payment6 int,
	loan3_interest_payment7 int,
	GENDER char (3),
	inquiry_date char (8)
)
GO

--10305
insert into tf_application(idn, actual_amount, interest_rate, periods)
   select idn, actual_amount, interest_rate, periods
   from tf_test.dbo.model_master

-- 10305
update tf_application
   set  loan1_interest_payment1 = a.loan1_interest_payment1,
	loan1_interest_payment2 = a.loan1_interest_payment2,
	loan1_interest_payment3 = a.loan1_interest_payment3,
	loan1_interest_payment4 = a.loan1_interest_payment4,
	loan1_interest_payment5 = a.loan1_interest_payment5,
	loan1_interest_payment6 = a.loan1_interest_payment6,
	loan2_interest_payment1 = a.loan2_interest_payment1,
	loan2_interest_payment2 = a.loan2_interest_payment2,
	loan2_interest_payment3 = a.loan2_interest_payment3,
	loan2_interest_payment4 = a.loan2_interest_payment4,
	loan2_interest_payment5 = a.loan2_interest_payment5,
	loan2_interest_payment6 = a.loan2_interest_payment6,
	loan3_interest_payment1 = a.loan3_interest_payment1,
	loan3_interest_payment2 = a.loan3_interest_payment2,
	loan3_interest_payment3 = a.loan3_interest_payment3,
	loan3_interest_payment4 = a.loan3_interest_payment4,
	loan3_interest_payment5 = a.loan3_interest_payment5,
	loan3_interest_payment6 = a.loan3_interest_payment6,
        gender = a.gender,
        martial_status = a.martial_status,
        edu = a.edu,
        is_secretive = a.is_secretive
from tf_gx.dbo.application a
where tf_application.idn = a.idn


update tf_application
   set  loan1_interest_payment1 = a.loan1_interest_payment1,
	loan1_interest_payment2 = a.loan1_interest_payment2,
	loan1_interest_payment3 = a.loan1_interest_payment3,
	loan1_interest_payment4 = a.loan1_interest_payment4,
	loan1_interest_payment5 = a.loan1_interest_payment5,
	loan1_interest_payment6 = a.loan1_interest_payment6,
	loan2_interest_payment1 = a.loan2_interest_payment1,
	loan2_interest_payment2 = a.loan2_interest_payment2,
	loan2_interest_payment3 = a.loan2_interest_payment3,
	loan2_interest_payment4 = a.loan2_interest_payment4,
	loan2_interest_payment5 = a.loan2_interest_payment5,
	loan2_interest_payment6 = a.loan2_interest_payment6,
	loan3_interest_payment1 = a.loan3_interest_payment1,
	loan3_interest_payment2 = a.loan3_interest_payment2,
	loan3_interest_payment3 = a.loan3_interest_payment3,
	loan3_interest_payment4 = a.loan3_interest_payment4,
	loan3_interest_payment5 = a.loan3_interest_payment5,
	loan3_interest_payment6 = a.loan3_interest_payment6,
        gender = a.gender,
        martial_status = a.martial_status,
        edu = a.edu,
        is_secretive = a.is_secretive
from tf_khj.dbo.application a
where tf_application.idn = a.idn

-- 9975
update tf_application
  set inquiry_date = a.time_stamp
  from tf_test.dbo.jas002 a
  where tf_application.idn = a.idn

--A	10305
select left(idn, 1), count(*)
from tf_application
group by left(idn, 1)

--
  select count(*), count(distinct a.idn) 
  from tf_test.dbo.stm001 a, tf_application b 
  where b.idn = a.idn

select inquiry_date, count(*)
from tf_application
group by inquiry_date
order by inquiry_date

--330
update tf_application
set inquiry_date = '20040106'
where inquiry_date is null

select count(*)
from tf_test.dbo.bam085
where idn in
(select idn from tf_application where inquiry_date is null)



select *
into risk_model_jas002
from jas002
where idn in (select idn from model_master)

select *
into risk_model_bam085
from bam085
where idn in (select idn from model_master)

select *
into risk_model_krm001
from krm001
where idn in (select idn from model_master)

select *
into risk_model_krm023
from krm023
where idn in (select idn from model_master)

select *
into risk_model_stm001
from stm001
where idn in (select idn from model_master)
select * 
into risk_model_application
from oliver.oliver.tf_application
/*
(影響 9975 個資料列)
(影響 29810 個資料列)
(影響 59510 個資料列)
(影響 266406 個資料列)
(影響 40299 個資料列)
(影響 10305 個資料列)
*/

ALTER  view bam085(app_sn, data_time, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan) as
select idn, TIME_STAMP, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan
from tf_test.dbo.risk_model_bam085
GO
ALTER  view jas002 (app_sn, data_time, EVER_DELINQUENT, DELINQUENT_DATE, EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, EVER_STOP_CARD, STOP_CARD_DATE) as
select idn, TIME_STAMP, EVER_DELINQUENT, DELINQUENT_DATE, EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, EVER_STOP_CARD, STOP_CARD_DATE
from tf_test.dbo.risk_model_JAS002
GO

ALTER  view krm001(app_sn, data_time, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, limit_type, usage_type, secure) as
select idn, TIME_STAMP, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, limit_type, usage_type, secure
from tf_test.dbo.risk_model_KRM001
GO

ALTER  view krm023(app_sn, data_time, yrmon, issue, issue_name, kr_code, limit, payment, cash, pay_code) as
select idn, TIME_STAMP, yrmon, issue, issue_name, kr_code, limit, payment, cash, pay_code
from tf_test.dbo.risk_model_KRM023
GO

ALTER  view stm001(app_sn, data_time, query_date, bank_code, bank_name, item_list) as
select idn, TIME_STAMP, query_date, bank_code, bank_name, item_list
from tf_test.dbo.risk_model_STM001
GO


--871
select jas002_defect, app_max_bucket, fs044, fs334, fs302, return_msg
from daco_v41_cal
where return_msg is not null

--6538
select krm001_hit, krm023_hit, ind001, full_score, full_twentile
from daco_v41_cal
where krm001_hit = 1 and krm023_hit = 1 and ind001 is null

--856
select bam085_hit, ms080, br80_score, br80_twentile, brmp_score, brmp_twentile, krm001_hit, krm023_hit, ind001, full_score, full_twentile
from daco_v41_cal
where bam085_hit = 1 and (ms080<=0 or ms080 is null) and (krm001_hit is null or krm023_hit is null or ind001 = 1)

-- 1128
select bam085_hit, ms080, brmp_score, brmp_twentile, br80_score, br80_twentile, krm001_hit, krm023_hit, ind001, full_score, full_twentile
from daco_v41_cal
where bam085_hit = 1 and ms080>0 and (krm001_hit is null or krm023_hit is null or ind001 = 1)

--1783
select demo_score, demo_twentile,bam085_hit, ms080, brmp_score, brmp_twentile, br80_score, br80_twentile, krm001_hit, krm023_hit, ind001, full_score, full_twentile
from daco_v41_cal
where (krm001_hit is null or krm023_hit is null or ind001 =1) and bam085_hit is null



-------test env setup---------------------------------------------------------------------------------------------
insert into app_info (app_sn, data_time, product_type, gender, zip, secretive, edu, marriage_status, cof, roe, ts_tax_rate, tf_tax_rate, info_porcessing_cost, operation_cost, hr_cost, commission)
select idn,'0', 1, gender_tran,'100', secret_tran, education_tran2, marriage, 0.0372, 0.10, 0.0225, 0.0476, 15, 17, 120, 0
from model_master

insert into loan_condition (app_sn, tsn, loan_amount, apr, terms, application_fee, credit_checking_fee, risk_mgmt_fee, risk_mgmt_fee_terms, teaser_rate, teaser_period, grace_period, sales_channel)
select idn, 1, actual_amount, interest_rate, periods, 0, 0, 0, 0, 0.0, 0, 0, '1'
from model_master

insert into ts(app_sn, data_time, loan1_payment1, loan1_payment2, loan1_payment3, loan1_payment4, loan1_payment5, loan1_payment6, loan2_payment1, loan2_payment2, loan2_payment3, loan2_payment4, loan2_payment5, loan2_payment6, loan3_payment1, loan3_payment2, loan3_payment3, loan3_payment4, loan3_payment5, loan3_payment6)
select IDN, '0', loan1_interest_payment1, loan1_interest_payment2, loan1_interest_payment3, loan1_interest_payment4, loan1_interest_payment5, loan1_interest_payment6, loan2_interest_payment1, loan2_interest_payment2, loan2_interest_payment3, loan2_interest_payment4, loan2_interest_payment5, loan2_interest_payment6, loan3_interest_payment1, loan3_interest_payment2, loan3_interest_payment3, loan3_interest_payment4, loan3_interest_payment5, loan3_interest_payment6
from application


 update app_info
    set edu = (case when a.edu = '研究所(含)以上' then 1
			  when a.edu = '大學'           then 2
			  when a.edu = '專科'           then 3
			  when a.edu = '高中(職)'       then 4
			  when a.edu = '國中以下'       then 5
			  else 6 end)
   from risk_model_application a
   where a.idn = app_info.app_sn


 update app_info
    set data_time = isnull((((convert(int, application_yyy)+ 1911)*100 + application_mm) * 100 + application_dd), '0')
   from application a
   where a.idn = app_info.app_sn


 update ts
    set data_time = isnull((((convert(int, application_yyy)+ 1911)*100 + application_mm) * 100 + application_dd), '0')
   from application a
   where a.idn = ts.app_sn

create table risk_model_driving (
	app_sn char(12),
	app_date char(12),
	ts_date  char(8),
	jcic_date char(8)
)

insert into risk_model_driving (app_sn, app_date, ts_date)
select app_sn, data_time, data_time
from app_info

update risk_model_driving
  set jcic_date = a.data_time
  from jas002 a
  where risk_model_driving.app_sn = a.app_sn

update risk_model_driving
set jcic_date = '20040106'
where jcic_date is null

use oliver
go
create  view bam082(app_sn, data_time, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan) as
select app_sn, data_time, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan
from tf_test.dbo.risk_new_bam082
GO
alter   view jas002 (app_sn, data_time, EVER_DELINQUENT, DELINQUENT_DATE, EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, EVER_STOP_CARD, STOP_CARD_DATE) as
select app_sn, data_time, EVER_DELINQUENT, DELINQUENT_DATE, EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, EVER_STOP_CARD, STOP_CARD_DATE
from tf_test.dbo.risk_new_JAS002
GO

alter   view krm001(app_sn, data_time, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, limit_type, usage_type, secure) as
select app_sn, data_time, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, limit_type, usage_type, secure
from tf_test.dbo.risk_new_KRM001
GO

alter   view krm023(app_sn, data_time, yrmon, issue, issue_name, kr_code, limit, payment, cash, pay_code) as
select app_sn, data_time, yrmon, issue, issue_name, kr_code, limit, payment, cash, pay_code
from tf_test.dbo.risk_new_KRM023
GO

alter   view stm001(app_sn, data_time, query_date, bank_code, bank_name, item_list) as
select app_sn, data_time, query_date, bank_code, bank_name, item_list
from tf_test.dbo.risk_new_STM001
GO

  update tf_ploan_cal
     set apr = (case when a.apr is null then 0.0 else a.apr / 100.0 end),
         periods = (case when a.terms,
         principal= a.loan_amount
     from loan_condition a
     where tf_ploan_cal.app_sn = a.app_sn

----------------- integrating test env setup-----------------------------------

update loan_condition
set loan_amount = a.loan_amount,
    apr = a.apr,
    terms = a.terms
from dbo.risk_loan_condition a
where loan_condition.app_sn = a.app_sn


alter table loan_condition alter column terms int null
alter table loan_condition alter column apr int null
alter table loan_condition alter column loan_amount int null


create table EM_ts (
	app_sn		nvarchar(10) not null,
	data_time	char(8) not null,
	loan1_payment1	int,
	loan1_payment2	int,
	loan1_payment3	int,
	loan1_payment4	int,
	loan1_payment5	int,
	loan1_payment6	int,
	loan2_payment1	int,
	loan2_payment2	int,
	loan2_payment3	int,
	loan2_payment4	int,
	loan2_payment5	int,
	loan2_payment6	int,
	loan3_payment1	int,
	loan3_payment2	int,
	loan3_payment3	int,
	loan3_payment4	int,
	loan3_payment5	int,
	loan3_payment6	int
);
alter table EM_ts add constraint p_emts primary key (app_sn, data_time); 
go

create table EM_app_info (
	app_sn		nvarchar(10) not null,
	data_time	char(12) not null,
	product_type	int not null check (product_type in (1, 2)),
	gender		int not null check (gender in (0, 1)),
	zip		char(3) not null,
	secretive	int not null check (secretive in (0, 1)),
	edu		int not null check (edu in (1, 2, 3, 4, 5, 6)),
	marriage_status	int not null check (marriage_status in (1, 2, 3, 4)),
	alien		int not null check (alien in (0, 1)),
	age_over_limit	int not null check (age_over_limit in (0, 1)),
	ts_cashcard_restricted	int not null check (ts_cashcard_restricted in (0, 1)),
	cof		decimal(5,4) not null,
	roe		decimal(5,4) not null,
	ts_tax_rate	decimal(5,4) not null,
	tf_tax_rate	decimal(5,4) not null,
	info_processing_cost	int not null,
	operation_cost	int not null,
	hr_cost		int not null,
	risk_level	int not null check (risk_level in (1, 2)),
	sales_channel	char(3) not null,
	commission	int not null default 0
);
alter table EM_app_info add constraint p_emapp_info primary key(app_sn, data_time); 
go

create table EM_loan_condition (
	app_sn		nvarchar(10) not null,
	tsn		char(12) not null,
	loan_amount	int,
	apr		float,
	terms		int,
	application_fee	int not null,
	credit_checking_fee	int not null,
	risk_mgmt_fee	int not null,
	risk_mgmt_fee_terms	int not null,
	teaser_rate	decimal(5,4) not null,
	teaser_period	int not null,
	grace_period	int not null
);
alter table EM_loan_condition add constraint p_emloan_condition primary key (app_sn, tsn); 
go


insert into dbo.EM_app_info (app_sn, data_time, product_type, gender, zip, secretive, edu, marriage_status, alien, age_over_limit, ts_cashcard_restricted, cof, roe, ts_tax_rate, tf_tax_rate, info_processing_cost, operation_cost, hr_cost, risk_level, sales_channel, commission)
select app_sn, data_time, 1, gender, zip, secretive, edu, marriage_status, 0, 0, 0, cof, roe, .0255, tf_tax_rate, info_porcessing_cost, operation_cost, hr_cost,1,'001', commission
from dbo.risk_model_app_info

insert into EM_loan_condition (app_sn, tsn, loan_amount, apr, terms, application_fee, credit_checking_fee, risk_mgmt_fee, risk_mgmt_fee_terms, teaser_rate, teaser_period, grace_period)
select app_sn, tsn, loan_amount, apr, terms, application_fee, credit_checking_fee, risk_mgmt_fee, risk_mgmt_fee_terms, teaser_rate, teaser_period, grace_period
from dbo.risk_model_loan_condition

insert into dbo.EM_ts (app_sn, data_time, loan1_payment1, loan1_payment2, loan1_payment3, loan1_payment4, loan1_payment5, loan1_payment6, loan2_payment1, loan2_payment2, loan2_payment3, loan2_payment4, loan2_payment5, loan2_payment6, loan3_payment1, loan3_payment2, loan3_payment3, loan3_payment4, loan3_payment5, loan3_payment6)
select app_sn, data_time, loan1_payment1, loan1_payment2, loan1_payment3, loan1_payment4, loan1_payment5, loan1_payment6, loan2_payment1, loan2_payment2, loan2_payment3, loan2_payment4, loan2_payment5, loan2_payment6, loan3_payment1, loan3_payment2, loan3_payment3, loan3_payment4, loan3_payment5, loan3_payment6
from dbo.risk_model_ts


update EM_loan_condition
set apr = a.apr,
    loan_amount = a.principal,
    terms = a.periods,
    application_fee = a.SETUP_FEE,
    credit_checking_fee = a.CREDIT_FEE,
    risk_mgmt_fee = a.risk_fee,
    risk_mgmt_fee_terms = 0
from oliver.oliver.Gx_test_case a
where a.idn = EM_loan_condition.app_sn

update EM_app_info
set risk_level = a.risk_level,
    sales_channel = (case when a.channel = 1 then '001'
                          when a.channel = 2 then '002'
                          when a.channel = 3 then '003'
                          when a.channel = 4 then '004'
                          when a.channel = 5 then '005'
                          when a.channel = 6 then '006'
                          when a.channel = 7 then '007'
                          when a.channel = 8 then '008'
                          when a.channel = 9 then '009'
                      end)
from oliver.oliver.Gx_test_case a
where a.idn = EM_app_info.app_sn

