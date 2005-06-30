/*
drop table #bam085_vars
drop table #bam085_dedup
drop table #bam085_bucket
-- drop table master_bam085_fs2_200304_oh
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
/*
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
*/
 create table #bam085_vars (
        idn   char(14),
        FS528_3M decimal(16, 8), FS528_6M decimal(16, 8), FS528_9M decimal(16, 8), FS528_12M decimal(16, 8),
        FS529_3M decimal(16, 8), FS529_6M decimal(16, 8), FS529_9M decimal(16, 8), FS529_12M decimal(16, 8),
        FS530_3M decimal(16, 8), FS530_6M decimal(16, 8), FS530_9M decimal(16, 8), FS530_12M decimal(16, 8),
        FS531_3M decimal(16, 8), FS531_6M decimal(16, 8), FS531_9M decimal(16, 8), FS531_12M decimal(16, 8),
        FS532_3M decimal(16, 8), FS532_6M decimal(16, 8), FS532_9M decimal(16, 8), FS532_12M decimal(16, 8),
        FS533_3M decimal(16, 8), FS533_6M decimal(16, 8), FS533_9M decimal(16, 8), FS533_12M decimal(16, 8),
        FS534_3M decimal(16, 8), FS534_6M decimal(16, 8), FS534_9M decimal(16, 8), FS534_12M decimal(16, 8),
        FS535_3M decimal(16, 8), FS535_6M decimal(16, 8), FS535_9M decimal(16, 8), FS535_12M decimal(16, 8),
        FS536_3M decimal(16, 8), FS536_6M decimal(16, 8), FS536_9M decimal(16, 8), FS536_12M decimal(16, 8),
        FS537_3M decimal(16, 8), FS537_6M decimal(16, 8), FS537_9M decimal(16, 8), FS537_12M decimal(16, 8),
        FS538_3M decimal(16, 8), FS538_6M decimal(16, 8), FS538_9M decimal(16, 8), FS538_12M decimal(16, 8),
        FS539_3M decimal(16, 8), FS539_6M decimal(16, 8), FS539_9M decimal(16, 8), FS539_12M decimal(16, 8),
        FS540_3M decimal(16, 8), FS540_6M decimal(16, 8), FS540_9M decimal(16, 8), FS540_12M decimal(16, 8),
        FS541_3M decimal(16, 8), FS541_6M decimal(16, 8), FS541_9M decimal(16, 8), FS541_12M decimal(16, 8),
        FS542_3M decimal(16, 8), FS542_6M decimal(16, 8), FS542_9M decimal(16, 8), FS542_12M decimal(16, 8),
        FS543_3M decimal(16, 8), FS543_6M decimal(16, 8), FS543_9M decimal(16, 8), FS543_12M decimal(16, 8),
        FS544_3M decimal(16, 8), FS544_6M decimal(16, 8), FS544_9M decimal(16, 8), FS544_12M decimal(16, 8),
        FS545_3M decimal(16, 8), FS545_6M decimal(16, 8), FS545_9M decimal(16, 8), FS545_12M decimal(16, 8),
        FS546_3M decimal(16, 8), FS546_6M decimal(16, 8), FS546_9M decimal(16, 8), FS546_12M decimal(16, 8),
        FS547_3M decimal(16, 8), FS547_6M decimal(16, 8), FS547_9M decimal(16, 8), FS547_12M decimal(16, 8),
        FS548_3M decimal(16, 8), FS548_6M decimal(16, 8), FS548_9M decimal(16, 8), FS548_12M decimal(16, 8),
        FS549_3M decimal(16, 8), FS549_6M decimal(16, 8), FS549_9M decimal(16, 8), FS549_12M decimal(16, 8),
        FS550_3M decimal(16, 8), FS550_6M decimal(16, 8), FS550_9M decimal(16, 8), FS550_12M decimal(16, 8),
        FS551_3M decimal(16, 8), FS551_6M decimal(16, 8), FS551_9M decimal(16, 8), FS551_12M decimal(16, 8),
        FS552_3M decimal(16, 8), FS552_6M decimal(16, 8), FS552_9M decimal(16, 8), FS552_12M decimal(16, 8),
        FS553_3M decimal(16, 8), FS553_6M decimal(16, 8), FS553_9M decimal(16, 8), FS553_12M decimal(16, 8),
        FS554_3M decimal(16, 8), FS554_6M decimal(16, 8), FS554_9M decimal(16, 8), FS554_12M decimal(16, 8)
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


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200303_oh
from #bam085_vars
go
delete from #bam085_vars
delete from #bam085_dedup

go

alter view bam085_curr as
select * from BAM085_200308
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200308'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200308_oh
from #bam085_vars
go
delete from #bam085_vars
delete from #bam085_dedup

go

alter view bam085_curr as
select * from BAM085_200312
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200312'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200312_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup

-- drop table master_bam085_fs2_200304_oh
go

alter view bam085_curr as
select * from BAM085_200304
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200304'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200304_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup

-- drop table master_bam085_fs2_200305_oh
go

alter view bam085_curr as
select * from BAM085_200305
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200305'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200305_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup

-- drop table master_bam085_fs2_200306_oh
go

alter view bam085_curr as
select * from BAM085_200306
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200306'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200306_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup

-- drop table master_bam085_fs2_200307_oh
go

alter view bam085_curr as
select * from BAM085_200307
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200307'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200307_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup

-- drop table master_bam085_fs2_200309_oh
go

alter view bam085_curr as
select * from BAM085_200309
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200309'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200309_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup

-- drop table master_bam085_fs2_200310_oh
go

alter view bam085_curr as
select * from BAM085_200310
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200310'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200310_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup

-- drop table master_bam085_fs2_200311_oh
go

alter view bam085_curr as
select * from BAM085_200311
go

declare @inq_mon char(6)
declare @now int
set @inq_mon = '200311'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec generate_bam_fs528_554 @now

select * into master_bam085_fs2_200311_oh
from #bam085_vars
