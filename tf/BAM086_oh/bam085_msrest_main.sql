/*
drop table #bam085_vars
drop table #bam085_dedup
drop table #bam085_bucket
-- drop table master_bam085_rest_200304_oh
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
        MS360_3M decimal(16,8), MS360_6M decimal(16,8), MS360_9M decimal(16,8), MS360_12M decimal(16,8),
        MS361_3M decimal(16,8), MS361_6M decimal(16,8), MS361_9M decimal(16,8), MS361_12M decimal(16,8),
        MS362_3M decimal(16,8), MS362_6M decimal(16,8), MS362_9M decimal(16,8), MS362_12M decimal(16,8),
        MS363_3M decimal(16,8), MS363_6M decimal(16,8), MS363_9M decimal(16,8), MS363_12M decimal(16,8),
        MS364_3M decimal(16,8), MS364_6M decimal(16,8), MS364_9M decimal(16,8), MS364_12M decimal(16,8),
        MS365_3M decimal(16,8), MS365_6M decimal(16,8), MS365_9M decimal(16,8), MS365_12M decimal(16,8),
        MS366_3M decimal(16,8), MS366_6M decimal(16,8), MS366_9M decimal(16,8), MS366_12M decimal(16,8),
        MS367_3M decimal(16,8), MS367_6M decimal(16,8), MS367_9M decimal(16,8), MS367_12M decimal(16,8),
        MS368_3M decimal(16,8), MS368_6M decimal(16,8), MS368_9M decimal(16,8), MS368_12M decimal(16,8),
        MS369_3M decimal(16,8), MS369_6M decimal(16,8), MS369_9M decimal(16,8), MS369_12M decimal(16,8),
        MS370_3M decimal(16,8), MS370_6M decimal(16,8), MS370_9M decimal(16,8), MS370_12M decimal(16,8),
        MS371_3M decimal(16,8), MS371_6M decimal(16,8), MS371_9M decimal(16,8), MS371_12M decimal(16,8),
        MS372_3M decimal(16,8), MS372_6M decimal(16,8), MS372_9M decimal(16,8), MS372_12M decimal(16,8),
        MS373_3M decimal(16,8), MS373_6M decimal(16,8), MS373_9M decimal(16,8), MS373_12M decimal(16,8),
        MS374_3M decimal(16,8), MS374_6M decimal(16,8), MS374_9M decimal(16,8), MS374_12M decimal(16,8),
        MS375_3M decimal(16,8), MS375_6M decimal(16,8), MS375_9M decimal(16,8), MS375_12M decimal(16,8),
        MS376_3M decimal(16,8), MS376_6M decimal(16,8), MS376_9M decimal(16,8), MS376_12M decimal(16,8),
        MS377_3M decimal(16,8), MS377_6M decimal(16,8), MS377_9M decimal(16,8), MS377_12M decimal(16,8),
        MS378_3M decimal(16,8), MS378_6M decimal(16,8), MS378_9M decimal(16,8), MS378_12M decimal(16,8),
        MS379_3M decimal(16,8), MS379_6M decimal(16,8), MS379_9M decimal(16,8), MS379_12M decimal(16,8),
        MS380_3M decimal(16,8), MS380_6M decimal(16,8), MS380_9M decimal(16,8), MS380_12M decimal(16,8),
        MS381_3M decimal(16,8), MS381_6M decimal(16,8), MS381_9M decimal(16,8), MS381_12M decimal(16,8),
        MS382_3M decimal(16,8), MS382_6M decimal(16,8), MS382_9M decimal(16,8), MS382_12M decimal(16,8),
        MS383_3M decimal(16,8), MS383_6M decimal(16,8), MS383_9M decimal(16,8), MS383_12M decimal(16,8),
        MS384_3M decimal(16,8), MS384_6M decimal(16,8), MS384_9M decimal(16,8), MS384_12M decimal(16,8),
        MS385_3M decimal(16,8), MS385_6M decimal(16,8), MS385_9M decimal(16,8), MS385_12M decimal(16,8),
        MS386_3M decimal(16,8), MS386_6M decimal(16,8), MS386_9M decimal(16,8), MS386_12M decimal(16,8),
        MS387_3M decimal(16,8), MS387_6M decimal(16,8), MS387_9M decimal(16,8), MS387_12M decimal(16,8),
        MS388_3M decimal(16,8), MS388_6M decimal(16,8), MS388_9M decimal(16,8), MS388_12M decimal(16,8),
        MS389_3M decimal(16,8), MS389_6M decimal(16,8), MS389_9M decimal(16,8), MS389_12M decimal(16,8),
        MS390_3M decimal(16,8), MS390_6M decimal(16,8), MS390_9M decimal(16,8), MS390_12M decimal(16,8),
        MS391_3M decimal(16,8), MS391_6M decimal(16,8), MS391_9M decimal(16,8), MS391_12M decimal(16,8),
        MS392_3M decimal(16,8), MS392_6M decimal(16,8), MS392_9M decimal(16,8), MS392_12M decimal(16,8),
        MS393_3M decimal(16,8), MS393_6M decimal(16,8), MS393_9M decimal(16,8), MS393_12M decimal(16,8),
        MS394_3M decimal(16,8), MS394_6M decimal(16,8), MS394_9M decimal(16,8), MS394_12M decimal(16,8),
        MS395_3M decimal(16,8), MS395_6M decimal(16,8), MS395_9M decimal(16,8), MS395_12M decimal(16,8),
        MS396_3M decimal(16,8), MS396_6M decimal(16,8), MS396_9M decimal(16,8), MS396_12M decimal(16,8),
        MS397_3M decimal(16,8), MS397_6M decimal(16,8), MS397_9M decimal(16,8), MS397_12M decimal(16,8),
        MS398_3M decimal(16,8), MS398_6M decimal(16,8), MS398_9M decimal(16,8), MS398_12M decimal(16,8),
        MS399_3M decimal(16,8), MS399_6M decimal(16,8), MS399_9M decimal(16,8), MS399_12M decimal(16,8),
        MS400_3M decimal(16,8), MS400_6M decimal(16,8), MS400_9M decimal(16,8), MS400_12M decimal(16,8),
        MS401_3M decimal(16,8), MS401_6M decimal(16,8), MS401_9M decimal(16,8), MS401_12M decimal(16,8),
        MS402_3M decimal(16,8), MS402_6M decimal(16,8), MS402_9M decimal(16,8), MS402_12M decimal(16,8),
        MS403_3M decimal(16,8), MS403_6M decimal(16,8), MS403_9M decimal(16,8), MS403_12M decimal(16,8),
        MS404_3M decimal(16,8), MS404_6M decimal(16,8), MS404_9M decimal(16,8), MS404_12M decimal(16,8)
 )
create index i_bamvar on #bam085_vars(idn);
go

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
go

alter view bam085_curr as
select * from BAM085_200303
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200303'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200303_oh
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

exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200308_oh
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

exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200312_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_msrest_200304_oh
go

alter view bam085_curr as
select * from BAM085_200304
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200304'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200304_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_msrest_200305_oh
go

alter view bam085_curr as
select * from BAM085_200305
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200305'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200305_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_msrest_200306_oh
go

alter view bam085_curr as
select * from BAM085_200306
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200306'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200306_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_msrest_200307_oh
go

alter view bam085_curr as
select * from BAM085_200307
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200307'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200307_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_msrest_200309_oh
go

alter view bam085_curr as
select * from BAM085_200309
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200309'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200309_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_msrest_200310_oh
go

alter view bam085_curr as
select * from BAM085_200310
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200310'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200310_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_msrest_200311_oh
go

alter view bam085_curr as
select * from BAM085_200311
go

declare @inq_mon char(6)
declare @now int
set @inq_mon = '200311'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_msrest @now

select * into master_bam085_msrest_200311_oh
from #bam085_vars
