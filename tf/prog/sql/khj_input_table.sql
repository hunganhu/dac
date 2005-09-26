use tf_test

select IDN as app_sn
into khj_model_sample
from tf_khj.dbo.model_sample
WHERE model_sample_flag = 2

select idn as app_sn, time_stamp as data_time, EVER_DELINQUENT, DELINQUENT_DATE, EVER_BAD_CHECK, BAD_CHECK_DATE, EVER_REJECT, REJECT_DATE, EVER_STOP_CARD, STOP_CARD_DATE
into khj_risk_jas002
from tf_khj.dbo.jas002
where idn in (select app_sn from khj_model_sample)
  and time_stamp is not null


select idn as app_sn, time_stamp as data_time, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit
into khj_risk_krm001
from tf_khj.dbo.krm001
where idn in (select app_sn from khj_model_sample)
  and card_brand is not null

select idn as app_sn, time_stamp as data_time, yrmon, issue, issue_name, kr_code, limit, payment, cash, pay_code
into khj_risk_krm023
from tf_khj.dbo.krm023
where idn in (select app_sn from khj_model_sample)
  and yrmon is not null

select idn as app_sn, TIME_STAMP as data_time, query_date, bank_code, bank_name, item_list
into khj_risk_stm001
from tf_khj.dbo.stm001
where idn in (select app_sn from khj_model_sample)
  and query_date is not null


select idn as app_sn, TIME_STAMP as data_time, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan
into khj_risk_bam082
from tf_khj.dbo.bam085
where idn in (select app_sn from khj_model_sample)
  and data_yyy is not null


create index i_bam082 on dbo.khj_risk_bam082 (app_sn, data_time)
create index i_jas002 on dbo.khj_risk_jas002(app_sn, data_time)
create index i_krm001 on dbo.khj_risk_krm001(app_sn, data_time)
create index i_krm023 on dbo.khj_risk_krm023(app_sn, data_time)
create index i_stm001 on dbo.khj_risk_stm001(app_sn, data_time)



create table khj_risk_ts (
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
alter table khj_risk_ts add constraint p_ts primary key (app_sn, data_time); 
go

create table khj_risk_app_info (
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
alter table khj_risk_app_info add constraint p_app_info primary key(app_sn, data_time); 
go

create table khj_risk_loan_condition (
	app_sn		nvarchar(10) not null,
	tsn		char(12) not null,
	loan_amount	int ,
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
alter table khj_risk_loan_condition add constraint p_loan_condition primary key (app_sn, tsn); 
go

insert into khj_risk_app_info (app_sn, data_time, product_type, gender, zip, secretive, edu, marriage_status, alien, age_over_limit, ts_cashcard_restricted, cof, roe, ts_tax_rate, tf_tax_rate, info_processing_cost, operation_cost, hr_cost, risk_level, sales_channel, commission)
select idn,'0', 2, (case when new_gender = '¨k' then 1 else 0 end), '100', secret_tran, education, marriage, 0, 0, 0, 0.0372, 0.10, 0.0255, 0.0476, 15, 17, 120, 1, '001', 0
from tf_khj.dbo.application
where idn in (select app_sn from khj_model_sample)

insert into khj_risk_loan_condition (app_sn, tsn, loan_amount, apr, terms, application_fee, credit_checking_fee, risk_mgmt_fee, risk_mgmt_fee_terms, teaser_rate, teaser_period, grace_period)
select idn, '1', NULL, interest_rate/100.0, NULL, 0, 0, 0, 0, 0.0, 0, 0
from tf_khj.dbo.application
where idn in (select app_sn from khj_model_sample)

insert into khj_risk_ts(app_sn, data_time, loan1_payment1, loan1_payment2, loan1_payment3, loan1_payment4, loan1_payment5, loan1_payment6, loan2_payment1, loan2_payment2, loan2_payment3, loan2_payment4, loan2_payment5, loan2_payment6, loan3_payment1, loan3_payment2, loan3_payment3, loan3_payment4, loan3_payment5, loan3_payment6)
select IDN, '0', loan1_interest_payment1, loan1_interest_payment2, loan1_interest_payment3, loan1_interest_payment4, loan1_interest_payment5, loan1_interest_payment6, loan2_interest_payment1, loan2_interest_payment2, loan2_interest_payment3, loan2_interest_payment4, loan2_interest_payment5, loan2_interest_payment6, loan3_interest_payment1, loan3_interest_payment2, loan3_interest_payment3, loan3_interest_payment4, loan3_interest_payment5, loan3_interest_payment6
from tf_khj.dbo.application
where idn in (select app_sn from khj_model_sample)


select sum(max_v1 - min_v1), sum(max_p - min_p)
from (select idn, max(actual_amount) as max_v1, min(actual_amount) as min_v1,
 max(periods) as max_p, min(periods) as min_p
from tf_khj.dbo.stmt_loaded
group by idn) as a1

update khj_risk_loan_condition
 set loan_amount = a1.max_v1,
     terms = a1.max_p
from 
(select idn, max(actual_amount) as max_v1, max(periods) as max_p
from tf_khj.dbo.stmt_loaded
group by idn) as a1
where khj_risk_loan_condition.app_sn = a1.idn

/*
select max(MIN_INQUIRY_DATE)
from TF_khj.dbo.data_availability
*/

update khj_risk_app_info
 set data_time = isnull(a.min_inquiry_date, '20041230')
from tf_khj.dbo.data_availability a
where khj_risk_app_info.app_sn = a.idn

update khj_risk_ts
 set data_time = isnull(a.min_inquiry_date, '20041230')
from tf_khj.dbo.data_availability a
where khj_risk_ts.app_sn = a.idn

create table risk_model_driving (
	app_sn char(12),
	app_date char(12),
	ts_date  char(8),
	jcic_date char(8)
)

insert into khj_risk_driving (app_sn, app_date, ts_date)
select app_sn, data_time, data_time
from khj_risk_app_info


update khj_risk_driving
  set jcic_date = a.data_time
  from khj_risk_jas002 a
  where khj_risk_driving.app_sn = a.app_sn

update khj_risk_driving
set jcic_date = '20041230'
where jcic_date is null

/*
-- 15631
select count(*)
from dbo.khj_risk_jas002 a, khj_risk_driving b
where a.app_sn = b.app_sn

select count(distinct a.app_sn)
from dbo.khj_risk_bam082 a, khj_risk_driving b
where a.app_sn = b.app_sn

update khj_risk_app_info
set product_type = 2
*/

alter view bam082 as select * from khj_risk_bam082
go

alter view jas002 as select * from khj_risk_jas002
go
alter view krm001 as select * from khj_risk_krm001
go
alter view krm023 as select * from khj_risk_krm023
go
alter view stm001 as select * from khj_risk_stm001
go
alter view app_info as select * from khj_risk_app_info
go
alter view loan_condition as select * from khj_risk_loan_condition
go

alter view ts as select * from khj_risk_ts
go


create table EM_KHJ_ts (
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
alter table EM_KHJ_ts add constraint p_EM_KHJ_ts primary key (app_sn, data_time); 
go

create table EM_KHJ_app_info (
	app_sn		nvarchar(10) not null,
	data_time	char(12) not null,
	product_type	int not null check (product_type in (1, 2)),
	gender		int not null check (gender in (0, 1)),
	zip		char(3) not null,
	secretive	int not null check (secretive in (0, 1)),
	education_level	int not null check (education_level in (1, 2, 3, 4, 5, 6)),
	marriage_status	int not null check (marriage_status in (1, 2, 3, 4)),
	alien		int not null check (alien in (0, 1)),
	age_over_limit	int not null check (age_over_limit in (0, 1)),
	ts_cashcard_restricted	int not null check (ts_cashcard_restricted in (0, 1)),
	cof		decimal(5,4) not null check (cof > 0),
	roe		decimal(5,4) not null check (roe > 0),
	ts_tax_rate	decimal(5,4) not null check (ts_tax_rate > 0),
	tf_tax_rate	decimal(5,4) not null check (tf_tax_rate > 0),
	info_processing_cost	int not null check (info_processing_cost > 0),
	operation_cost	int not null check (operation_cost > 0),
	hr_cost		int not null check (hr_cost > 0),
	risk_level	int not null check (risk_level in (1, 2)),
	sales_channel	char(3) not null check (sales_channel in ('001', '002', '003',
	                                                          '004', '005', '006',
	                                                          '007', '008', '009')),
	commission	int not null check (commission >= 0),
	add_loan	nvarchar(10),
	promotion_code	nvarchar(10),
	sales_region	nvarchar(20),
	sales_unit	nvarchar(20),
	sales_userno	nvarchar(10)

);
alter table EM_KHJ_app_info add constraint p_EM_KHJ_app_info primary key(app_sn, data_time); 
go

create table EM_KHJ_loan_condition (
	app_sn		nvarchar(10) not null,
	tsn		char(12) not null,
	loan_amount	int not null check (loan_amount > 0),
	apr		float not null check (apr >= 0.15 and apr <=0.2),
	terms		int not null check (terms > 0),
	application_fee	int not null check (application_fee >= 0),
	credit_checking_fee	int not null check (credit_checking_fee >= 0),
	risk_mgmt_fee	int not null check (risk_mgmt_fee >= 0),
	risk_mgmt_fee_terms	int not null check (risk_mgmt_fee_terms >= 0),
	teaser_rate	decimal(5,4) not null check (teaser_rate >= 0 ),
	teaser_period	int not null check (teaser_period >= 0),
	grace_period	int not null check (grace_period >= 0)
);
alter table EM_KHJ_loan_condition add constraint p_EM_KHJ_loan_condition primary key (app_sn, tsn); 
go


insert into EM_KHJ_app_info (app_sn, data_time, product_type, gender, zip, secretive, education_level, marriage_status, alien, age_over_limit, ts_cashcard_restricted, cof, roe, ts_tax_rate, tf_tax_rate, info_processing_cost, operation_cost, hr_cost, risk_level, sales_channel, commission)
select app_sn, data_time, product_type, gender, zip, secretive, edu, marriage_status, 0, 0, 0, cof, roe, .0255, tf_tax_rate, info_processing_cost, operation_cost, 80, 1,'001', commission
from khj_risk_app_info

insert into EM_KHJ_loan_condition (app_sn, tsn, loan_amount, apr, terms, application_fee, credit_checking_fee, risk_mgmt_fee, risk_mgmt_fee_terms, teaser_rate, teaser_period, grace_period)
select app_sn, tsn, 1, 0.2, 100, application_fee, credit_checking_fee, risk_mgmt_fee, risk_mgmt_fee_terms, teaser_rate, teaser_period, grace_period
from khj_risk_loan_condition

insert into EM_KHJ_ts (app_sn, data_time, loan1_payment1, loan1_payment2, loan1_payment3, loan1_payment4, loan1_payment5, loan1_payment6, loan2_payment1, loan2_payment2, loan2_payment3, loan2_payment4, loan2_payment5, loan2_payment6, loan3_payment1, loan3_payment2, loan3_payment3, loan3_payment4, loan3_payment5, loan3_payment6)
select app_sn, data_time, loan1_payment1, loan1_payment2, loan1_payment3, loan1_payment4, loan1_payment5, loan1_payment6, loan2_payment1, loan2_payment2, loan2_payment3, loan2_payment4, loan2_payment5, loan2_payment6, loan3_payment1, loan3_payment2, loan3_payment3, loan3_payment4, loan3_payment5, loan3_payment6
from khj_risk_ts


update EM_KHJ_loan_condition
set apr = a.apr,
    loan_amount = a.principal,
    terms = a.periods,
    application_fee = a.SETUP_FEE,
    credit_checking_fee = a.CREDIT_FEE,
    risk_mgmt_fee = a.risk_fee,
    risk_mgmt_fee_terms = 0
from oliver.oliver.tf_khj_score_0920_15day_testcase a
where a.idn = EM_KHJ_loan_condition.app_sn

update EM_KHJ_app_info
set sales_channel = (case when a.channel = 1 then '001'
                          when a.channel = 2 then '002'
                          when a.channel = 3 then '003'
                          when a.channel = 4 then '004'
                          when a.channel = 5 then '005'
                          when a.channel = 6 then '006'
                          when a.channel = 7 then '007'
                          when a.channel = 8 then '008'
                          when a.channel = 9 then '009'
                      end)
from oliver.oliver.tf_khj_score_0920_15day_testcase a
where a.idn = EM_KHJ_app_info.app_sn


select * from khj_risk_driving where ts_date = 0

alter view bam082 as select * from khj_risk_bam082
go

alter view jas002 as select * from khj_risk_jas002
go
alter view krm001 as select * from khj_risk_krm001
go
alter view krm023 as select * from khj_risk_krm023
go
alter view stm001 as select * from khj_risk_stm001
go
alter view app_info as select * from EM_KHJ_app_info
go
alter view loan_condition as select * from EM_KHJ_loan_condition
go

alter view ts as select * from EM_KHJ_ts
go
alter view test_in as select * from khj_risk_driving
go

select *
into tf_gx_score_0913_15day_testcase
from oliver.oliver.tf_khj_score_0920_15day_testcase

create index i_idn on tf_gx_score_0913_15day_testcase(idn)

CREATE TABLE [test_out] (
	[case_sn] [char] (12),
	[idn] [char] (11),
	[dac_sn] [decimal](3, 0),
	[return_msg] [varchar] (256)
) 
GO


