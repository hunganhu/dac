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
