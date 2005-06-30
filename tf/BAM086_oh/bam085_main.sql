/*
drop table #bam085_vars
drop table #bam085_dedup
drop table #bam085_bucket
-- drop table master_bam085_200304_oh
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
        FS315 decimal(16, 8),
        FS316 decimal(16, 8),
        FS317 decimal(16, 8),
        FS318 decimal(16, 8),
        FS319 decimal(16, 8),
        FS320 decimal(16, 8),
        FS321 decimal(16, 8),
        FS322 decimal(16, 8),
        FS323 decimal(16, 8),
        FS324 decimal(16, 8),
        FS325 decimal(16, 8),
        FS326 decimal(16, 8),
        FS327 decimal(16, 8),
        FS328 decimal(16, 8),
        FS329 decimal(16, 8),
        FS330 decimal(16, 8),
        FS331 decimal(16, 8),
        FS332 decimal(16, 8),
        FS333 decimal(16, 8),
        FS334 decimal(16, 8),
        RS101 decimal(16, 8),
        RS102 decimal(16, 8),
        RS103 decimal(16, 8),
        RS104 decimal(16, 8),
        RS105 decimal(16, 8),
        RS106 decimal(16, 8),
        RS107 decimal(16, 8),
        RS108 decimal(16, 8),
        RS109 decimal(16, 8),
        RS110 decimal(16, 8),
        RS111 decimal(16, 8),
        RS112 decimal(16, 8),
        RS113 decimal(16, 8),
        RS114 decimal(16, 8),
        RS115 decimal(16, 8),
        RS116 decimal(16, 8),
        RS117 decimal(16, 8),
        RS118 decimal(16, 8),
        RS119 decimal(16, 8),
        RS120 decimal(16, 8),
        FS354_3M decimal(16, 8), FS354_6M decimal(16, 8), FS354_9M decimal(16, 8), FS354_12M decimal(16, 8),
        FS355_3M decimal(16, 8), FS355_6M decimal(16, 8), FS355_9M decimal(16, 8), FS355_12M decimal(16, 8),
        FS356_3M decimal(16, 8), FS356_6M decimal(16, 8), FS356_9M decimal(16, 8), FS356_12M decimal(16, 8),
        FS357_3M decimal(16, 8), FS357_6M decimal(16, 8), FS357_9M decimal(16, 8), FS357_12M decimal(16, 8),
        FS358_3M decimal(16, 8), FS358_6M decimal(16, 8), FS358_9M decimal(16, 8), FS358_12M decimal(16, 8),
        FS359_3M decimal(16, 8), FS359_6M decimal(16, 8), FS359_9M decimal(16, 8), FS359_12M decimal(16, 8),
        FS360_3M decimal(16, 8), FS360_6M decimal(16, 8), FS360_9M decimal(16, 8), FS360_12M decimal(16, 8),
        FS361_3M decimal(16, 8), FS361_6M decimal(16, 8), FS361_9M decimal(16, 8), FS361_12M decimal(16, 8),
        FS362_3M decimal(16, 8), FS362_6M decimal(16, 8), FS362_9M decimal(16, 8), FS362_12M decimal(16, 8),
        FS363_3M decimal(16, 8), FS363_6M decimal(16, 8), FS363_9M decimal(16, 8), FS363_12M decimal(16, 8),
        FS364_3M decimal(16, 8), FS364_6M decimal(16, 8), FS364_9M decimal(16, 8), FS364_12M decimal(16, 8),
        FS365_3M decimal(16, 8), FS365_6M decimal(16, 8), FS365_9M decimal(16, 8), FS365_12M decimal(16, 8),
        FS366_3M decimal(16, 8), FS366_6M decimal(16, 8), FS366_9M decimal(16, 8), FS366_12M decimal(16, 8),
        FS367_3M decimal(16, 8), FS367_6M decimal(16, 8), FS367_9M decimal(16, 8), FS367_12M decimal(16, 8),
        FS368_3M decimal(16, 8), FS368_6M decimal(16, 8), FS368_9M decimal(16, 8), FS368_12M decimal(16, 8),
        FS369_3M decimal(16, 8), FS369_6M decimal(16, 8), FS369_9M decimal(16, 8), FS369_12M decimal(16, 8),
        FS370_3M decimal(16, 8), FS370_6M decimal(16, 8), FS370_9M decimal(16, 8), FS370_12M decimal(16, 8),
        FS371_3M decimal(16, 8), FS371_6M decimal(16, 8), FS371_9M decimal(16, 8), FS371_12M decimal(16, 8),
        FS372_3M decimal(16, 8), FS372_6M decimal(16, 8), FS372_9M decimal(16, 8), FS372_12M decimal(16, 8),
        FS373_3M decimal(16, 8), FS373_6M decimal(16, 8), FS373_9M decimal(16, 8), FS373_12M decimal(16, 8),
        FS374_3M decimal(16, 8), FS374_6M decimal(16, 8), FS374_9M decimal(16, 8), FS374_12M decimal(16, 8),
        FS375_3M decimal(16, 8), FS375_6M decimal(16, 8), FS375_9M decimal(16, 8), FS375_12M decimal(16, 8),
        FS376_3M decimal(16, 8), FS376_6M decimal(16, 8), FS376_9M decimal(16, 8), FS376_12M decimal(16, 8),
        FS377_3M decimal(16, 8), FS377_6M decimal(16, 8), FS377_9M decimal(16, 8), FS377_12M decimal(16, 8),
        FS378_3M decimal(16, 8), FS378_6M decimal(16, 8), FS378_9M decimal(16, 8), FS378_12M decimal(16, 8),
        FS379_3M decimal(16, 8), FS379_6M decimal(16, 8), FS379_9M decimal(16, 8), FS379_12M decimal(16, 8),
        FS380_3M decimal(16, 8), FS380_6M decimal(16, 8), FS380_9M decimal(16, 8), FS380_12M decimal(16, 8),
        FS381_3M decimal(16, 8), FS381_6M decimal(16, 8), FS381_9M decimal(16, 8), FS381_12M decimal(16, 8),
        FS382_3M decimal(16, 8), FS382_6M decimal(16, 8), FS382_9M decimal(16, 8), FS382_12M decimal(16, 8),
        FS383_3M decimal(16, 8), FS383_6M decimal(16, 8), FS383_9M decimal(16, 8), FS383_12M decimal(16, 8),
        FS384_3M decimal(16, 8), FS384_6M decimal(16, 8), FS384_9M decimal(16, 8), FS384_12M decimal(16, 8),
        FS385_3M decimal(16, 8), FS385_6M decimal(16, 8), FS385_9M decimal(16, 8), FS385_12M decimal(16, 8),
        FS386_3M decimal(16, 8), FS386_6M decimal(16, 8), FS386_9M decimal(16, 8), FS386_12M decimal(16, 8),
        FS387_3M decimal(16, 8), FS387_6M decimal(16, 8), FS387_9M decimal(16, 8), FS387_12M decimal(16, 8),
        FS388_3M decimal(16, 8), FS388_6M decimal(16, 8), FS388_9M decimal(16, 8), FS388_12M decimal(16, 8),
        FS389_3M decimal(16, 8), FS389_6M decimal(16, 8), FS389_9M decimal(16, 8), FS389_12M decimal(16, 8),
        FS390_3M decimal(16, 8), FS390_6M decimal(16, 8), FS390_9M decimal(16, 8), FS390_12M decimal(16, 8),
        FS391_3M decimal(16, 8), FS391_6M decimal(16, 8), FS391_9M decimal(16, 8), FS391_12M decimal(16, 8),
        FS392_3M decimal(16, 8), FS392_6M decimal(16, 8), FS392_9M decimal(16, 8), FS392_12M decimal(16, 8),
        FS393_3M decimal(16, 8), FS393_6M decimal(16, 8), FS393_9M decimal(16, 8), FS393_12M decimal(16, 8),
        FS394_3M decimal(16, 8), FS394_6M decimal(16, 8), FS394_9M decimal(16, 8), FS394_12M decimal(16, 8),
        FS395_3M decimal(16, 8), FS395_6M decimal(16, 8), FS395_9M decimal(16, 8), FS395_12M decimal(16, 8),
        FS396_3M decimal(16, 8), FS396_6M decimal(16, 8), FS396_9M decimal(16, 8), FS396_12M decimal(16, 8),
        FS397_3M decimal(16, 8), FS397_6M decimal(16, 8), FS397_9M decimal(16, 8), FS397_12M decimal(16, 8),
        FS398_3M decimal(16, 8), FS398_6M decimal(16, 8), FS398_9M decimal(16, 8), FS398_12M decimal(16, 8),
        FS399_3M decimal(16, 8), FS399_6M decimal(16, 8), FS399_9M decimal(16, 8), FS399_12M decimal(16, 8),
        FS400_3M decimal(16, 8), FS400_6M decimal(16, 8), FS400_9M decimal(16, 8), FS400_12M decimal(16, 8),
        FS401_3M decimal(16, 8), FS401_6M decimal(16, 8), FS401_9M decimal(16, 8), FS401_12M decimal(16, 8),
        FS402_3M decimal(16, 8), FS402_6M decimal(16, 8), FS402_9M decimal(16, 8), FS402_12M decimal(16, 8),
        FS403_3M decimal(16, 8), FS403_6M decimal(16, 8), FS403_9M decimal(16, 8), FS403_12M decimal(16, 8),
        FS404_3M decimal(16, 8), FS404_6M decimal(16, 8), FS404_9M decimal(16, 8), FS404_12M decimal(16, 8),
        FS405_3M decimal(16, 8), FS405_6M decimal(16, 8), FS405_9M decimal(16, 8), FS405_12M decimal(16, 8),
        FS406_3M decimal(16, 8), FS406_6M decimal(16, 8), FS406_9M decimal(16, 8), FS406_12M decimal(16, 8),
        FS407_3M decimal(16, 8), FS407_6M decimal(16, 8), FS407_9M decimal(16, 8), FS407_12M decimal(16, 8),
        FS408_3M decimal(16, 8), FS408_6M decimal(16, 8), FS408_9M decimal(16, 8), FS408_12M decimal(16, 8),
        FS409_3M decimal(16, 8), FS409_6M decimal(16, 8), FS409_9M decimal(16, 8), FS409_12M decimal(16, 8),
        FS410_3M decimal(16, 8), FS410_6M decimal(16, 8), FS410_9M decimal(16, 8), FS410_12M decimal(16, 8),
        FS411_3M decimal(16, 8), FS411_6M decimal(16, 8), FS411_9M decimal(16, 8), FS411_12M decimal(16, 8),
        FS412_3M decimal(16, 8), FS412_6M decimal(16, 8), FS412_9M decimal(16, 8), FS412_12M decimal(16, 8),
        FS413_3M decimal(16, 8), FS413_6M decimal(16, 8), FS413_9M decimal(16, 8), FS413_12M decimal(16, 8),
        FS414_3M decimal(16, 8), FS414_6M decimal(16, 8), FS414_9M decimal(16, 8), FS414_12M decimal(16, 8),
        FS415_3M decimal(16, 8), FS415_6M decimal(16, 8), FS415_9M decimal(16, 8), FS415_12M decimal(16, 8),
        FS416_3M decimal(16, 8), FS416_6M decimal(16, 8), FS416_9M decimal(16, 8), FS416_12M decimal(16, 8),
        FS417_3M decimal(16, 8), FS417_6M decimal(16, 8), FS417_9M decimal(16, 8), FS417_12M decimal(16, 8),
        FS418_3M decimal(16, 8), FS418_6M decimal(16, 8), FS418_9M decimal(16, 8), FS418_12M decimal(16, 8),
        FS419_3M decimal(16, 8), FS419_6M decimal(16, 8), FS419_9M decimal(16, 8), FS419_12M decimal(16, 8),
        FS420_3M decimal(16, 8), FS420_6M decimal(16, 8), FS420_9M decimal(16, 8), FS420_12M decimal(16, 8),
        FS421_3M decimal(16, 8), FS421_6M decimal(16, 8), FS421_9M decimal(16, 8), FS421_12M decimal(16, 8),
        FS422_3M decimal(16, 8), FS422_6M decimal(16, 8), FS422_9M decimal(16, 8), FS422_12M decimal(16, 8),
        FS423_3M decimal(16, 8), FS423_6M decimal(16, 8), FS423_9M decimal(16, 8), FS423_12M decimal(16, 8),
        FS424_3M decimal(16, 8), FS424_6M decimal(16, 8), FS424_9M decimal(16, 8), FS424_12M decimal(16, 8),
        FS425_3M decimal(16, 8), FS425_6M decimal(16, 8), FS425_9M decimal(16, 8), FS425_12M decimal(16, 8),
        FS426_3M decimal(16, 8), FS426_6M decimal(16, 8), FS426_9M decimal(16, 8), FS426_12M decimal(16, 8),
        FS427_3M decimal(16, 8), FS427_6M decimal(16, 8), FS427_9M decimal(16, 8), FS427_12M decimal(16, 8),
        FS428_3M decimal(16, 8), FS428_6M decimal(16, 8), FS428_9M decimal(16, 8), FS428_12M decimal(16, 8),
        FS429_3M decimal(16, 8), FS429_6M decimal(16, 8), FS429_9M decimal(16, 8), FS429_12M decimal(16, 8),
        FS430_3M decimal(16, 8), FS430_6M decimal(16, 8), FS430_9M decimal(16, 8), FS430_12M decimal(16, 8),
        FS431_3M decimal(16, 8), FS431_6M decimal(16, 8), FS431_9M decimal(16, 8), FS431_12M decimal(16, 8),
        FS432_3M decimal(16, 8), FS432_6M decimal(16, 8), FS432_9M decimal(16, 8), FS432_12M decimal(16, 8),
        FS433_3M decimal(16, 8), FS433_6M decimal(16, 8), FS433_9M decimal(16, 8), FS433_12M decimal(16, 8),
        FS434_3M decimal(16, 8), FS434_6M decimal(16, 8), FS434_9M decimal(16, 8), FS434_12M decimal(16, 8),
        FS435_3M decimal(16, 8), FS435_6M decimal(16, 8), FS435_9M decimal(16, 8), FS435_12M decimal(16, 8),
        FS436_3M decimal(16, 8), FS436_6M decimal(16, 8), FS436_9M decimal(16, 8), FS436_12M decimal(16, 8),
        FS437_3M decimal(16, 8), FS437_6M decimal(16, 8), FS437_9M decimal(16, 8), FS437_12M decimal(16, 8),
        FS438_3M decimal(16, 8), FS438_6M decimal(16, 8), FS438_9M decimal(16, 8), FS438_12M decimal(16, 8),
        FS439_3M decimal(16, 8), FS439_6M decimal(16, 8), FS439_9M decimal(16, 8), FS439_12M decimal(16, 8),
        FS440_3M decimal(16, 8), FS440_6M decimal(16, 8), FS440_9M decimal(16, 8), FS440_12M decimal(16, 8),
        FS441_3M decimal(16, 8), FS441_6M decimal(16, 8), FS441_9M decimal(16, 8), FS441_12M decimal(16, 8),
        FS442_3M decimal(16, 8), FS442_6M decimal(16, 8), FS442_9M decimal(16, 8), FS442_12M decimal(16, 8),
        FS443_3M decimal(16, 8), FS443_6M decimal(16, 8), FS443_9M decimal(16, 8), FS443_12M decimal(16, 8),
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
exec generate_bam_vars_315_443 @now

select * into master_bam085_200303_oh
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
exec generate_bam_vars_315_443 @now

select * into master_bam085_200308_oh
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
exec generate_bam_vars_315_443 @now

select * into master_bam085_200312_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_200304_oh
go

alter view bam085_curr as
select * from BAM085_200304
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200304'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_315_443 @now

select * into master_bam085_200304_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_200305_oh
go

alter view bam085_curr as
select * from BAM085_200305
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200305'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_315_443 @now

select * into master_bam085_200305_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_200306_oh
go

alter view bam085_curr as
select * from BAM085_200306
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200306'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_315_443 @now

select * into master_bam085_200306_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_200307_oh
go

alter view bam085_curr as
select * from BAM085_200307
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200307'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec prepare_jcic_bam085_all
exec generate_bam_vars_315_443 @now

select * into master_bam085_200307_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_200309_oh
go

alter view bam085_curr as
select * from BAM085_200309
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200309'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_315_443 @now

select * into master_bam085_200309_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_200310_oh
go

alter view bam085_curr as
select * from BAM085_200310
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200310'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_315_443 @now

select * into master_bam085_200310_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_200311_oh
go

alter view bam085_curr as
select * from BAM085_200311
go

declare @inq_mon char(6)
declare @now int
set @inq_mon = '200311'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_315_443 @now

select * into master_bam085_200311_oh
from #bam085_vars
