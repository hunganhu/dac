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
        FS444_3M decimal(16,8), FS444_6M decimal(16,8), FS444_9M decimal(16,8), FS444_12M decimal(16,8),
        FS445_3M decimal(16,8), FS445_6M decimal(16,8), FS445_9M decimal(16,8), FS445_12M decimal(16,8),
        FS446_3M decimal(16,8), FS446_6M decimal(16,8), FS446_9M decimal(16,8), FS446_12M decimal(16,8),
        FS447_3M decimal(16,8), FS447_6M decimal(16,8), FS447_9M decimal(16,8), FS447_12M decimal(16,8),
        FS448_3M decimal(16,8), FS448_6M decimal(16,8), FS448_9M decimal(16,8), FS448_12M decimal(16,8),
        FS449_3M decimal(16,8), FS449_6M decimal(16,8), FS449_9M decimal(16,8), FS449_12M decimal(16,8),
        FS450_3M decimal(16,8), FS450_6M decimal(16,8), FS450_9M decimal(16,8), FS450_12M decimal(16,8),
        FS451_3M decimal(16,8), FS451_6M decimal(16,8), FS451_9M decimal(16,8), FS451_12M decimal(16,8),
        FS452_3M decimal(16,8), FS452_6M decimal(16,8), FS452_9M decimal(16,8), FS452_12M decimal(16,8),
        FS453_3M decimal(16,8), FS453_6M decimal(16,8), FS453_9M decimal(16,8), FS453_12M decimal(16,8),
        FS454_3M decimal(16,8), FS454_6M decimal(16,8), FS454_9M decimal(16,8), FS454_12M decimal(16,8),
        FS455_3M decimal(16,8), FS455_6M decimal(16,8), FS455_9M decimal(16,8), FS455_12M decimal(16,8),
        FS456_3M decimal(16,8), FS456_6M decimal(16,8), FS456_9M decimal(16,8), FS456_12M decimal(16,8),
        FS457_3M decimal(16,8), FS457_6M decimal(16,8), FS457_9M decimal(16,8), FS457_12M decimal(16,8),
        FS458_3M decimal(16,8), FS458_6M decimal(16,8), FS458_9M decimal(16,8), FS458_12M decimal(16,8),
        FS459_3M decimal(16,8), FS459_6M decimal(16,8), FS459_9M decimal(16,8), FS459_12M decimal(16,8),
        FS460_3M decimal(16,8), FS460_6M decimal(16,8), FS460_9M decimal(16,8), FS460_12M decimal(16,8),
        FS461_3M decimal(16,8), FS461_6M decimal(16,8), FS461_9M decimal(16,8), FS461_12M decimal(16,8),
        FS462_3M decimal(16,8), FS462_6M decimal(16,8), FS462_9M decimal(16,8), FS462_12M decimal(16,8),
        FS463_3M decimal(16,8), FS463_6M decimal(16,8), FS463_9M decimal(16,8), FS463_12M decimal(16,8),
        FS464_3M decimal(16,8), FS464_6M decimal(16,8), FS464_9M decimal(16,8), FS464_12M decimal(16,8),
        FS465_3M decimal(16,8), FS465_6M decimal(16,8), FS465_9M decimal(16,8), FS465_12M decimal(16,8),
        FS466_3M decimal(16,8), FS466_6M decimal(16,8), FS466_9M decimal(16,8), FS466_12M decimal(16,8),
        FS467_3M decimal(16,8), FS467_6M decimal(16,8), FS467_9M decimal(16,8), FS467_12M decimal(16,8),
        FS468_3M decimal(16,8), FS468_6M decimal(16,8), FS468_9M decimal(16,8), FS468_12M decimal(16,8),
        FS469_3M decimal(16,8), FS469_6M decimal(16,8), FS469_9M decimal(16,8), FS469_12M decimal(16,8),
        FS470_3M decimal(16,8), FS470_6M decimal(16,8), FS470_9M decimal(16,8), FS470_12M decimal(16,8),
        FS471_3M decimal(16,8), FS471_6M decimal(16,8), FS471_9M decimal(16,8), FS471_12M decimal(16,8),
        FS472_3M decimal(16,8), FS472_6M decimal(16,8), FS472_9M decimal(16,8), FS472_12M decimal(16,8),
        FS473_3M decimal(16,8), FS473_6M decimal(16,8), FS473_9M decimal(16,8), FS473_12M decimal(16,8),
        FS474_3M decimal(16,8), FS474_6M decimal(16,8), FS474_9M decimal(16,8), FS474_12M decimal(16,8),
        FS475_3M decimal(16,8), FS475_6M decimal(16,8), FS475_9M decimal(16,8), FS475_12M decimal(16,8),
        FS476_3M decimal(16,8), FS476_6M decimal(16,8), FS476_9M decimal(16,8), FS476_12M decimal(16,8),
        FS477_3M decimal(16,8), FS477_6M decimal(16,8), FS477_9M decimal(16,8), FS477_12M decimal(16,8),
        FS478_3M decimal(16,8), FS478_6M decimal(16,8), FS478_9M decimal(16,8), FS478_12M decimal(16,8),
        FS479_3M decimal(16,8), FS479_6M decimal(16,8), FS479_9M decimal(16,8), FS479_12M decimal(16,8),
        FS480_3M decimal(16,8), FS480_6M decimal(16,8), FS480_9M decimal(16,8), FS480_12M decimal(16,8),
        FS481_3M decimal(16,8), FS481_6M decimal(16,8), FS481_9M decimal(16,8), FS481_12M decimal(16,8),
        FS482_3M decimal(16,8), FS482_6M decimal(16,8), FS482_9M decimal(16,8), FS482_12M decimal(16,8),
        FS483_3M decimal(16,8), FS483_6M decimal(16,8), FS483_9M decimal(16,8), FS483_12M decimal(16,8),
        FS484_3M decimal(16,8), FS484_6M decimal(16,8), FS484_9M decimal(16,8), FS484_12M decimal(16,8),
        FS485_3M decimal(16,8), FS485_6M decimal(16,8), FS485_9M decimal(16,8), FS485_12M decimal(16,8),
        FS486_3M decimal(16,8), FS486_6M decimal(16,8), FS486_9M decimal(16,8), FS486_12M decimal(16,8),
        FS487_3M decimal(16,8), FS487_6M decimal(16,8), FS487_9M decimal(16,8), FS487_12M decimal(16,8),
        FS488_3M decimal(16,8), FS488_6M decimal(16,8), FS488_9M decimal(16,8), FS488_12M decimal(16,8),
        FS489_3M decimal(16,8), FS489_6M decimal(16,8), FS489_9M decimal(16,8), FS489_12M decimal(16,8),
        FS490_3M decimal(16,8), FS490_6M decimal(16,8), FS490_9M decimal(16,8), FS490_12M decimal(16,8),
        FS491_3M decimal(16,8), FS491_6M decimal(16,8), FS491_9M decimal(16,8), FS491_12M decimal(16,8),
        FS492_3M decimal(16,8), FS492_6M decimal(16,8), FS492_9M decimal(16,8), FS492_12M decimal(16,8),
        FS493_3M decimal(16,8), FS493_6M decimal(16,8), FS493_9M decimal(16,8), FS493_12M decimal(16,8),
        FS494_3M decimal(16,8), FS494_6M decimal(16,8), FS494_9M decimal(16,8), FS494_12M decimal(16,8),
        FS495_3M decimal(16,8), FS495_6M decimal(16,8), FS495_9M decimal(16,8), FS495_12M decimal(16,8),
        FS496_3M decimal(16,8), FS496_6M decimal(16,8), FS496_9M decimal(16,8), FS496_12M decimal(16,8),
        FS497_3M decimal(16,8), FS497_6M decimal(16,8), FS497_9M decimal(16,8), FS497_12M decimal(16,8),
        FS498_3M decimal(16,8), FS498_6M decimal(16,8), FS498_9M decimal(16,8), FS498_12M decimal(16,8),
        FS499_3M decimal(16,8), FS499_6M decimal(16,8), FS499_9M decimal(16,8), FS499_12M decimal(16,8),
        FS500_3M decimal(16,8), FS500_6M decimal(16,8), FS500_9M decimal(16,8), FS500_12M decimal(16,8),
        FS501_3M decimal(16,8), FS501_6M decimal(16,8), FS501_9M decimal(16,8), FS501_12M decimal(16,8),
        FS502_3M decimal(16,8), FS502_6M decimal(16,8), FS502_9M decimal(16,8), FS502_12M decimal(16,8),
        FS503_3M decimal(16,8), FS503_6M decimal(16,8), FS503_9M decimal(16,8), FS503_12M decimal(16,8),
        FS504_3M decimal(16,8), FS504_6M decimal(16,8), FS504_9M decimal(16,8), FS504_12M decimal(16,8),
        FS505_3M decimal(16,8), FS505_6M decimal(16,8), FS505_9M decimal(16,8), FS505_12M decimal(16,8),
        FS506_3M decimal(16,8), FS506_6M decimal(16,8), FS506_9M decimal(16,8), FS506_12M decimal(16,8),
        FS507_3M decimal(16,8), FS507_6M decimal(16,8), FS507_9M decimal(16,8), FS507_12M decimal(16,8),
        FS508_3M decimal(16,8), FS508_6M decimal(16,8), FS508_9M decimal(16,8), FS508_12M decimal(16,8),
        FS509_3M decimal(16,8), FS509_6M decimal(16,8), FS509_9M decimal(16,8), FS509_12M decimal(16,8),
        FS510_3M decimal(16,8), FS510_6M decimal(16,8), FS510_9M decimal(16,8), FS510_12M decimal(16,8),
        FS511_3M decimal(16,8), FS511_6M decimal(16,8), FS511_9M decimal(16,8), FS511_12M decimal(16,8),
        FS512_3M decimal(16,8), FS512_6M decimal(16,8), FS512_9M decimal(16,8), FS512_12M decimal(16,8),
        FS513_3M decimal(16,8), FS513_6M decimal(16,8), FS513_9M decimal(16,8), FS513_12M decimal(16,8),
        FS514_3M decimal(16,8), FS514_6M decimal(16,8), FS514_9M decimal(16,8), FS514_12M decimal(16,8),
        FS515_3M decimal(16,8), FS515_6M decimal(16,8), FS515_9M decimal(16,8), FS515_12M decimal(16,8),
        FS516_3M decimal(16,8), FS516_6M decimal(16,8), FS516_9M decimal(16,8), FS516_12M decimal(16,8),
        FS517_3M decimal(16,8), FS517_6M decimal(16,8), FS517_9M decimal(16,8), FS517_12M decimal(16,8),
        FS518_3M decimal(16,8), FS518_6M decimal(16,8), FS518_9M decimal(16,8), FS518_12M decimal(16,8),
        FS519_3M decimal(16,8), FS519_6M decimal(16,8), FS519_9M decimal(16,8), FS519_12M decimal(16,8),
        FS520_3M decimal(16,8), FS520_6M decimal(16,8), FS520_9M decimal(16,8), FS520_12M decimal(16,8),
        FS521_3M decimal(16,8), FS521_6M decimal(16,8), FS521_9M decimal(16,8), FS521_12M decimal(16,8),
        FS522_3M decimal(16,8), FS522_6M decimal(16,8), FS522_9M decimal(16,8), FS522_12M decimal(16,8),
        FS523_3M decimal(16,8), FS523_6M decimal(16,8), FS523_9M decimal(16,8), FS523_12M decimal(16,8),
        FS524_3M decimal(16,8), FS524_6M decimal(16,8), FS524_9M decimal(16,8), FS524_12M decimal(16,8),
        FS525_3M decimal(16,8), FS525_6M decimal(16,8), FS525_9M decimal(16,8), FS525_12M decimal(16,8),
        FS526_3M decimal(16,8), FS526_6M decimal(16,8), FS526_9M decimal(16,8), FS526_12M decimal(16,8),
        FS527_3M decimal(16,8), FS527_6M decimal(16,8), FS527_9M decimal(16,8), FS527_12M decimal(16,8),
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
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200303_oh
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
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200308_oh
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
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200312_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_rest_200304_oh
go

alter view bam085_curr as
select * from BAM085_200304
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200304'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200304_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_rest_200305_oh
go

alter view bam085_curr as
select * from BAM085_200305
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200305'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200305_oh
from #bam085_vars
 
delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_rest_200306_oh
go

alter view bam085_curr as
select * from BAM085_200306
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200306'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200306_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_rest_200307_oh
go

alter view bam085_curr as
select * from BAM085_200307
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200307'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)


exec prepare_jcic_bam085_all
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200307_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_rest_200309_oh
go

alter view bam085_curr as
select * from BAM085_200309
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200309'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200309_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_rest_200310_oh
go

alter view bam085_curr as
select * from BAM085_200310
go
declare @inq_mon char(6)
declare @now int
set @inq_mon = '200310'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200310_oh
from #bam085_vars

delete from #bam085_vars
delete from #bam085_dedup
delete from #bam085_bucket
-- drop table master_bam085_rest_200311_oh
go

alter view bam085_curr as
select * from BAM085_200311
go

declare @inq_mon char(6)
declare @now int
set @inq_mon = '200311'
set @now = (left(@inq_mon, 4) - 1911) * 12 + right(@inq_mon,2)

exec prepare_jcic_bam085_all
exec generate_bam_vars_rest @now

select * into master_bam085_rest_200311_oh
from #bam085_vars
