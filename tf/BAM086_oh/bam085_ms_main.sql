/*
drop table #bam085_vars
drop table #bam085_dedup
drop table #bam085_bucket
-- drop table master_bam085_ms_200304_oh
*/
  create table #bam085_dedup (
  	idn char (14),
  	data_yyy char (3),
  	data_mm char (2),
  	bank_code char (7),
  	bank_name char (40),
  	account_code char (1),
  	account_code2 char (1),
  	purpose_code char (1),
  	contract_amt decimal (10),
  	loan_amt decimal (10),
  	pass_due_amt decimal (10),
  	pay_code_12 char (12),
  	co_loan char (1),
  	bank_code2 char (3),
        mon_since int,
        cycle int,
  	cnt int
  );
create index i_bam085 on #bam085_dedup(idn, bank_code);

 create table #bam085_bucket (
    idn       char(14),
    bank_code char(7),
    mon_since int,
  	account_code char (1),
  	account_code2 char (1),
  	purpose_code char (1),
  	contract_amt decimal (10),
  	loan_amt decimal (10),
  	pass_due_amt decimal (10),
  	co_loan char (1),
    bucket    float,
    cured     int,
    decreased int,
    month_dec int
 )
create index i_bambucket on #bam085_bucket(idn, bank_code);
create index i_bucketac on #bam085_bucket(account_code);
create index i_bucketac2 on #bam085_bucket(account_code2);
create index i_bucketpc on #bam085_bucket(purpose_code);

 create table #bam085_vars (
        idn   char(14),
        MS315_3M decimal(16, 8), MS315_6M decimal(16, 8), MS315_9M decimal(16, 8), MS315_12M decimal(16, 8),
        MS316_3M decimal(16, 8), MS316_6M decimal(16, 8), MS316_9M decimal(16, 8), MS316_12M decimal(16, 8),
        MS317_3M decimal(16, 8), MS317_6M decimal(16, 8), MS317_9M decimal(16, 8), MS317_12M decimal(16, 8),
        MS318_3M decimal(16, 8), MS318_6M decimal(16, 8), MS318_9M decimal(16, 8), MS318_12M decimal(16, 8),
        MS319_3M decimal(16, 8), MS319_6M decimal(16, 8), MS319_9M decimal(16, 8), MS319_12M decimal(16, 8),
        MS320_3M decimal(16, 8), MS320_6M decimal(16, 8), MS320_9M decimal(16, 8), MS320_12M decimal(16, 8),
        MS321_3M decimal(16, 8), MS321_6M decimal(16, 8), MS321_9M decimal(16, 8), MS321_12M decimal(16, 8),
        MS322_3M decimal(16, 8), MS322_6M decimal(16, 8), MS322_9M decimal(16, 8), MS322_12M decimal(16, 8),
        MS323_3M decimal(16, 8), MS323_6M decimal(16, 8), MS323_9M decimal(16, 8), MS323_12M decimal(16, 8),
        MS324_3M decimal(16, 8), MS324_6M decimal(16, 8), MS324_9M decimal(16, 8), MS324_12M decimal(16, 8),
        MS325_3M decimal(16, 8), MS325_6M decimal(16, 8), MS325_9M decimal(16, 8), MS325_12M decimal(16, 8),
        MS326_3M decimal(16, 8), MS326_6M decimal(16, 8), MS326_9M decimal(16, 8), MS326_12M decimal(16, 8),
        MS327_3M decimal(16, 8), MS327_6M decimal(16, 8), MS327_9M decimal(16, 8), MS327_12M decimal(16, 8),
        MS328_3M decimal(16, 8), MS328_6M decimal(16, 8), MS328_9M decimal(16, 8), MS328_12M decimal(16, 8),
        MS329_3M decimal(16, 8), MS329_6M decimal(16, 8), MS329_9M decimal(16, 8), MS329_12M decimal(16, 8),
        MS330_3M decimal(16, 8), MS330_6M decimal(16, 8), MS330_9M decimal(16, 8), MS330_12M decimal(16, 8),
        MS331_3M decimal(16, 8), MS331_6M decimal(16, 8), MS331_9M decimal(16, 8), MS331_12M decimal(16, 8),
        MS332_3M decimal(16, 8), MS332_6M decimal(16, 8), MS332_9M decimal(16, 8), MS332_12M decimal(16, 8),
        MS333_3M decimal(16, 8), MS333_6M decimal(16, 8), MS333_9M decimal(16, 8), MS333_12M decimal(16, 8),
        MS334_3M decimal(16, 8), MS334_6M decimal(16, 8), MS334_9M decimal(16, 8), MS334_12M decimal(16, 8),
        MS335_3M decimal(16, 8), MS335_6M decimal(16, 8), MS335_9M decimal(16, 8), MS335_12M decimal(16, 8),
        MS336_3M decimal(16, 8), MS336_6M decimal(16, 8), MS336_9M decimal(16, 8), MS336_12M decimal(16, 8),
        MS337_3M decimal(16, 8), MS337_6M decimal(16, 8), MS337_9M decimal(16, 8), MS337_12M decimal(16, 8),
        MS338_3M decimal(16, 8), MS338_6M decimal(16, 8), MS338_9M decimal(16, 8), MS338_12M decimal(16, 8),
        MS339_3M decimal(16, 8), MS339_6M decimal(16, 8), MS339_9M decimal(16, 8), MS339_12M decimal(16, 8),
        MS340_3M decimal(16, 8), MS340_6M decimal(16, 8), MS340_9M decimal(16, 8), MS340_12M decimal(16, 8),
        MS341_3M decimal(16, 8), MS341_6M decimal(16, 8), MS341_9M decimal(16, 8), MS341_12M decimal(16, 8),
        MS342_3M decimal(16, 8), MS342_6M decimal(16, 8), MS342_9M decimal(16, 8), MS342_12M decimal(16, 8),
        MS343_3M decimal(16, 8), MS343_6M decimal(16, 8), MS343_9M decimal(16, 8), MS343_12M decimal(16, 8),
        MS344_3M decimal(16, 8), MS344_6M decimal(16, 8), MS344_9M decimal(16, 8), MS344_12M decimal(16, 8),
        MS345_3M decimal(16, 8), MS345_6M decimal(16, 8), MS345_9M decimal(16, 8), MS345_12M decimal(16, 8),
        MS346_3M decimal(16, 8), MS346_6M decimal(16, 8), MS346_9M decimal(16, 8), MS346_12M decimal(16, 8),
        MS347_3M decimal(16, 8), MS347_6M decimal(16, 8), MS347_9M decimal(16, 8), MS347_12M decimal(16, 8),
        MS348_3M decimal(16, 8), MS348_6M decimal(16, 8), MS348_9M decimal(16, 8), MS348_12M decimal(16, 8),
        MS349_3M decimal(16, 8), MS349_6M decimal(16, 8), MS349_9M decimal(16, 8), MS349_12M decimal(16, 8),
        MS350_3M decimal(16, 8), MS350_6M decimal(16, 8), MS350_9M decimal(16, 8), MS350_12M decimal(16, 8),
        MS351_3M decimal(16, 8), MS351_6M decimal(16, 8), MS351_9M decimal(16, 8), MS351_12M decimal(16, 8),
        MS352_3M decimal(16, 8), MS352_6M decimal(16, 8), MS352_9M decimal(16, 8), MS352_12M decimal(16, 8),
        MS353_3M decimal(16, 8), MS353_6M decimal(16, 8), MS353_9M decimal(16, 8), MS353_12M decimal(16, 8),
        MS354_3M decimal(16, 8), MS354_6M decimal(16, 8), MS354_9M decimal(16, 8), MS354_12M decimal(16, 8),
        MS355_3M decimal(16, 8), MS355_6M decimal(16, 8), MS355_9M decimal(16, 8), MS355_12M decimal(16, 8),
        MS356_3M decimal(16, 8), MS356_6M decimal(16, 8), MS356_9M decimal(16, 8), MS356_12M decimal(16, 8),
        MS357_3M decimal(16, 8), MS357_6M decimal(16, 8), MS357_9M decimal(16, 8), MS357_12M decimal(16, 8),
        MS358_3M decimal(16, 8), MS358_6M decimal(16, 8), MS358_9M decimal(16, 8), MS358_12M decimal(16, 8),
        MS359_3M decimal(16, 8), MS359_6M decimal(16, 8), MS359_9M decimal(16, 8), MS359_12M decimal(16, 8)
 )
create index i_bamvar on #bam085_vars(idn);

go
alter view bam085_curr as
select * from BAM085_200303
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200303'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200303_oh
from #bam085_vars
go
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
go

alter view bam085_curr as
select * from BAM085_200308
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200308'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200308_oh
from #bam085_vars
go
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
go

alter view bam085_curr as
select * from BAM085_200312
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200312'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200312_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_ms_200304_oh
go

alter view bam085_curr as
select * from BAM085_200304
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200304'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200304_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_ms_200305_oh
go

alter view bam085_curr as
select * from BAM085_200305
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200305'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200305_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_ms_200306_oh
go

alter view bam085_curr as
select * from BAM085_200306
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200306'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200306_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_ms_200307_oh
go

alter view bam085_curr as
select * from BAM085_200307
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200307'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200307_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_ms_200309_oh
go

alter view bam085_curr as
select * from BAM085_200309
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200309'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200309_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_ms_200310_oh
go

alter view bam085_curr as
select * from BAM085_200310
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200310'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200310_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_ms_200311_oh
go

alter view bam085_curr as
select * from BAM085_200311
go

declare @inq_mon char(6)
declare @now int
set @inq_mon = '200311'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

-- exec prepare_jcic_bam085_all
exec generate_bam_ms_315_359 @now

select * into master_bam085_ms_200311_oh
from #bam085_vars
