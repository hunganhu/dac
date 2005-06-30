/*
drop table #bam085_vars
drop table #bam085_dedup
drop table #bam085_bucket
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
        FS443_3M decimal(16, 8), FS443_6M decimal(16, 8), FS443_9M decimal(16, 8), FS443_12M decimal(16, 8)
 )
create index i_bamvar on #bam085_vars(idn);



delete from #bam085_dedup
delete from #bam085_bucket
delete from #bam085_vars


 insert into #bam085_dedup (idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, cnt)
    select idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, count(*)
    from bam085_200304
    where idn = 'A13949'
    group by idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan;
 update #bam085_dedup
    set bank_code2 = left(bank_code,3),
        mon_since = data_yyy * 12 + data_mm,
        cycle = len(pay_code_12),
        account_code = (case when account_code is null then '' else account_code end),
        account_code2 = (case when account_code2 is null then '' else account_code2 end),
        purpose_code = (case when purpose_code is null then '' else purpose_code end),
        co_loan = (case when co_loan is null then '' else co_loan end)

 insert into #bam085_vars (idn)
    select distinct idn
    from #bam085_dedup

 declare record_cur CURSOR local scroll static for
    select idn, mon_since, bank_code, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, rtrim(pay_code_12), co_loan, cycle
      from #bam085_dedup

 declare @idn char(14)
 declare @data_yyy char(5)
 declare @data_mm  char(2)
 declare @bank_code char(7)
 declare @account_code char(1)
 declare @account_code2 char(1)
 declare @purpose_code char(1)
 declare @contract_amt int
 declare @loan_amt int
 declare @pass_due_amt int
 declare @co_loan char(1)
 declare @pay_code_12 char(12)
 declare @paycode char(1)
 declare @bucket  float
 declare @month_since int
 declare @cycle int
 declare @i int

 open record_cur
    select idn, mon_since, bank_code, account_code, account_code2, purpose_code, contract_amt,
           loan_amt, pass_due_amt, rtrim(pay_code_12), co_loan, cycle
    from #bam085_dedup
 fetch next from record_cur into @idn, @month_since, @bank_code, @account_code, @account_code2, @purpose_code, 
                                 @contract_amt, @loan_amt, @pass_due_amt, @pay_code_12, @co_loan, @cycle

 while (@@fetch_status = 0)
    begin
       set @i = 0

       while  @i < @cycle
          begin
             set @paycode = substring(@pay_code_12, (@i + 1), 1)
             set @bucket = (case when @paycode = 'A' then 0.25
                                 when @paycode = 'B' then 0.5
                                 when @paycode = 'X' then 0
                                 else convert(float, @paycode) end)
             insert into #bam085_bucket (idn, bank_code, mon_since, account_code, account_code2, purpose_code, contract_amt,
                                         loan_amt, pass_due_amt, co_loan, bucket, cured, decreased, month_dec)
                                 values (@idn, @bank_code, @month_since - @i, @account_code, @account_code2, @purpose_code, @contract_amt,
                                         @loan_amt, @pass_due_amt, @co_loan, @bucket, 0, 0, 0)
             set @i = @i + 1
          end
       fetch next from record_cur into @idn, @month_since, @bank_code, @account_code, @account_code2, @purpose_code, 
                                       @contract_amt, @loan_amt, @pass_due_amt, @pay_code_12, @co_loan, @cycle
    end
 close record_cur
 deallocate record_cur

 declare cycle_cur CURSOR local scroll static for
    select distinct mon_since
      from #bam085_bucket
      order by mon_since asc;
 open cycle_cur
 fetch next from cycle_cur into @month_since    /* Skip the oldest month*/
 fetch next from cycle_cur into @month_since

 while (@@fetch_status = 0)
  begin
    update #bam085_bucket
      set cured = (case when #bam085_bucket.bucket = 0 and b.bucket > 0 then 1 else 0 end),
          decreased = (case when #bam085_bucket.bucket < b.bucket then 1 else 0 end),
          month_dec = (case when #bam085_bucket.bucket < b.bucket then b.bucket-#bam085_bucket.bucket else 0 end)
    from #bam085_bucket join #bam085_bucket as b
      on #bam085_bucket.idn = b.idn
         and #bam085_bucket.bank_code = b.bank_code
         and #bam085_bucket.account_code = b.account_code
         and #bam085_bucket.account_code2 = b.account_code2
         and #bam085_bucket.purpose_code = b.purpose_code       
         and #bam085_bucket.contract_amt = b.contract_amt       
         and #bam085_bucket.loan_amt = b.loan_amt       
         and #bam085_bucket.pass_due_amt = b.pass_due_amt       
         and #bam085_bucket.co_loan = b.co_loan       
    where #bam085_bucket.mon_since = @month_since
      and #bam085_bucket.mon_since = b.mon_since + 1

    fetch next from cycle_cur into @month_since
  end
 close cycle_cur
 deallocate cycle_cur


 create table #tmp (
    idn varchar(14),
    mon int,
    v1 decimal (16, 8),
    v2 decimal (16, 8),
    v3 decimal (16, 8));
 create table #tmp1 (
    idn varchar(14),
    mon int,
    v1 decimal (16, 8));
 create index i_tmp on #tmp(idn, mon);



update #bam085_vars
    set FS315 = 0,
        FS316 = 0,
        FS317 = 0,
        FS318 = 0,
        FS319 = 0,
        FS320 = 0,
        FS321 = 0,
        FS322 = 0,
        FS323 = 0,
        FS324 = 0,
        FS325 = 0,
        FS326 = 0,
        FS327 = 0,
        FS328 = 0,
        FS329 = 0,
        FS330 = 0,
        FS331 = 0,
        FS332 = 0,
        FS333 = 0,
        FS334 = 0,
        RS101 = 0,
        RS102 = 0,
        RS103 = 0,
        RS104 = 0,
        RS105 = 0,
        RS106 = 0,
        RS107 = 0,
        RS108 = 0,
        RS109 = 0,
        RS110 = 0,
        RS111 = 0,
        RS112 = 0,
        RS113 = 0,
        RS114 = 0,
        RS115 = 0,
        RS116 = 0,
        RS117 = 0,
        RS118 = 0,
        RS119 = 0,
        RS120 = 0,
        FS354_3M = 0, FS354_6M = 0, FS354_9M = 0, FS354_12M = 0,
        FS355_3M = 0, FS355_6M = 0, FS355_9M = 0, FS355_12M = 0,
        FS356_3M = 0, FS356_6M = 0, FS356_9M = 0, FS356_12M = 0,
        FS357_3M = 0, FS357_6M = 0, FS357_9M = 0, FS357_12M = 0,
        FS358_3M = 0, FS358_6M = 0, FS358_9M = 0, FS358_12M = 0,
        FS359_3M = 0, FS359_6M = 0, FS359_9M = 0, FS359_12M = 0,
        FS360_3M = 0, FS360_6M = 0, FS360_9M = 0, FS360_12M = 0,
        FS361_3M = 0, FS361_6M = 0, FS361_9M = 0, FS361_12M = 0,
        FS362_3M = 0, FS362_6M = 0, FS362_9M = 0, FS362_12M = 0,
        FS363_3M = 0, FS363_6M = 0, FS363_9M = 0, FS363_12M = 0,
        FS364_3M = 0, FS364_6M = 0, FS364_9M = 0, FS364_12M = 0,
        FS365_3M = 0, FS365_6M = 0, FS365_9M = 0, FS365_12M = 0,
        FS366_3M = 0, FS366_6M = 0, FS366_9M = 0, FS366_12M = 0,
        FS367_3M = 0, FS367_6M = 0, FS367_9M = 0, FS367_12M = 0,
        FS368_3M = 0, FS368_6M = 0, FS368_9M = 0, FS368_12M = 0,
        FS369_3M = 0, FS369_6M = 0, FS369_9M = 0, FS369_12M = 0,
        FS370_3M = 0, FS370_6M = 0, FS370_9M = 0, FS370_12M = 0,
        FS371_3M = 0, FS371_6M = 0, FS371_9M = 0, FS371_12M = 0,
        FS372_3M = 0, FS372_6M = 0, FS372_9M = 0, FS372_12M = 0,
        FS373_3M = 0, FS373_6M = 0, FS373_9M = 0, FS373_12M = 0,
        FS374_3M = 0, FS374_6M = 0, FS374_9M = 0, FS374_12M = 0,
        FS375_3M = 0, FS375_6M = 0, FS375_9M = 0, FS375_12M = 0,
        FS376_3M = 0, FS376_6M = 0, FS376_9M = 0, FS376_12M = 0,
        FS377_3M = 0, FS377_6M = 0, FS377_9M = 0, FS377_12M = 0,
        FS378_3M = 0, FS378_6M = 0, FS378_9M = 0, FS378_12M = 0,
        FS379_3M = 0, FS379_6M = 0, FS379_9M = 0, FS379_12M = 0,
        FS380_3M = 0, FS380_6M = 0, FS380_9M = 0, FS380_12M = 0,
        FS381_3M = 0, FS381_6M = 0, FS381_9M = 0, FS381_12M = 0,
        FS382_3M = 0, FS382_6M = 0, FS382_9M = 0, FS382_12M = 0,
        FS383_3M = 0, FS383_6M = 0, FS383_9M = 0, FS383_12M = 0,
        FS384_3M = 0, FS384_6M = 0, FS384_9M = 0, FS384_12M = 0,
        FS385_3M = 0, FS385_6M = 0, FS385_9M = 0, FS385_12M = 0,
        FS386_3M = 0, FS386_6M = 0, FS386_9M = 0, FS386_12M = 0,
        FS387_3M = 0, FS387_6M = 0, FS387_9M = 0, FS387_12M = 0,
        FS388_3M = 0, FS388_6M = 0, FS388_9M = 0, FS388_12M = 0,
        FS389_3M = 0, FS389_6M = 0, FS389_9M = 0, FS389_12M = 0,
        FS390_3M = 0, FS390_6M = 0, FS390_9M = 0, FS390_12M = 0,
        FS391_3M = 0, FS391_6M = 0, FS391_9M = 0, FS391_12M = 0,
        FS392_3M = 0, FS392_6M = 0, FS392_9M = 0, FS392_12M = 0,
        FS393_3M = 0, FS393_6M = 0, FS393_9M = 0, FS393_12M = 0,
        FS394_3M = 0, FS394_6M = 0, FS394_9M = 0, FS394_12M = 0,
        FS395_3M = 0, FS395_6M = 0, FS395_9M = 0, FS395_12M = 0,
        FS396_3M = 0, FS396_6M = 0, FS396_9M = 0, FS396_12M = 0,
        FS397_3M = 0, FS397_6M = 0, FS397_9M = 0, FS397_12M = 0,
        FS398_3M = 0, FS398_6M = 0, FS398_9M = 0, FS398_12M = 0,
        FS399_3M = 0, FS399_6M = 0, FS399_9M = 0, FS399_12M = 0,
        FS400_3M = 0, FS400_6M = 0, FS400_9M = 0, FS400_12M = 0,
        FS401_3M = 0, FS401_6M = 0, FS401_9M = 0, FS401_12M = 0,
        FS402_3M = 0, FS402_6M = 0, FS402_9M = 0, FS402_12M = 0,
        FS403_3M = 0, FS403_6M = 0, FS403_9M = 0, FS403_12M = 0,
        FS404_3M = 0, FS404_6M = 0, FS404_9M = 0, FS404_12M = 0,
        FS405_3M = 0, FS405_6M = 0, FS405_9M = 0, FS405_12M = 0,
        FS406_3M = 0, FS406_6M = 0, FS406_9M = 0, FS406_12M = 0,
        FS407_3M = 0, FS407_6M = 0, FS407_9M = 0, FS407_12M = 0,
        FS408_3M = 0, FS408_6M = 0, FS408_9M = 0, FS408_12M = 0,
        FS409_3M = 0, FS409_6M = 0, FS409_9M = 0, FS409_12M = 0,
        FS410_3M = 0, FS410_6M = 0, FS410_9M = 0, FS410_12M = 0,
        FS411_3M = 0, FS411_6M = 0, FS411_9M = 0, FS411_12M = 0,
        FS412_3M = 0, FS412_6M = 0, FS412_9M = 0, FS412_12M = 0,
        FS413_3M = 0, FS413_6M = 0, FS413_9M = 0, FS413_12M = 0,
        FS414_3M = 0, FS414_6M = 0, FS414_9M = 0, FS414_12M = 0,
        FS415_3M = 0, FS415_6M = 0, FS415_9M = 0, FS415_12M = 0,
        FS416_3M = 0, FS416_6M = 0, FS416_9M = 0, FS416_12M = 0,
        FS417_3M = 0, FS417_6M = 0, FS417_9M = 0, FS417_12M = 0,
        FS418_3M = 0, FS418_6M = 0, FS418_9M = 0, FS418_12M = 0,
        FS419_3M = 0, FS419_6M = 0, FS419_9M = 0, FS419_12M = 0,
        FS420_3M = 0, FS420_6M = 0, FS420_9M = 0, FS420_12M = 0,
        FS421_3M = 0, FS421_6M = 0, FS421_9M = 0, FS421_12M = 0,
        FS422_3M = 0, FS422_6M = 0, FS422_9M = 0, FS422_12M = 0,
        FS423_3M = 0, FS423_6M = 0, FS423_9M = 0, FS423_12M = 0,
        FS424_3M = 0, FS424_6M = 0, FS424_9M = 0, FS424_12M = 0,
        FS425_3M = 0, FS425_6M = 0, FS425_9M = 0, FS425_12M = 0,
        FS426_3M = 0, FS426_6M = 0, FS426_9M = 0, FS426_12M = 0,
        FS427_3M = 0, FS427_6M = 0, FS427_9M = 0, FS427_12M = 0,
        FS428_3M = 0, FS428_6M = 0, FS428_9M = 0, FS428_12M = 0,
        FS429_3M = 0, FS429_6M = 0, FS429_9M = 0, FS429_12M = 0,
        FS430_3M = 0, FS430_6M = 0, FS430_9M = 0, FS430_12M = 0,
        FS431_3M = 0, FS431_6M = 0, FS431_9M = 0, FS431_12M = 0,
        FS432_3M = 0, FS432_6M = 0, FS432_9M = 0, FS432_12M = 0,
        FS433_3M = 0, FS433_6M = 0, FS433_9M = 0, FS433_12M = 0,
        FS434_3M = 0, FS434_6M = 0, FS434_9M = 0, FS434_12M = 0,
        FS435_3M = 0, FS435_6M = 0, FS435_9M = 0, FS435_12M = 0,
        FS436_3M = 0, FS436_6M = 0, FS436_9M = 0, FS436_12M = 0,
        FS437_3M = 0, FS437_6M = 0, FS437_9M = 0, FS437_12M = 0,
        FS438_3M = 0, FS438_6M = 0, FS438_9M = 0, FS438_12M = 0,
        FS439_3M = 0, FS439_6M = 0, FS439_9M = 0, FS439_12M = 0,
        FS440_3M = 0, FS440_6M = 0, FS440_9M = 0, FS440_12M = 0,
        FS441_3M = 0, FS441_6M = 0, FS441_9M = 0, FS441_12M = 0,
        FS442_3M = 0, FS442_6M = 0, FS442_9M = 0, FS442_12M = 0,
        FS443_3M = 0, FS443_6M = 0, FS443_9M = 0, FS443_12M = 0
    from #bam085_dedup as a
    where a.idn = #bam085_vars.idn;



/*****************************************************************************************
  * start making FS366	週轉金貸款曾經cured最多筆數	4	purpose_code = 4
  *****************************************************************************************/
declare @i int
declare @now int
set @now = 92 * 12 + 4

 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where purpose_code = '4'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS366_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS366_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS366_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS366_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12


/*
select * from #bam085_dedup
select * from #bam085_bucket order by bank_code, account_code, account_code2,purpose_code, contract_amt, mon_since

select * from #tmp
select * from #bam085_vars


*/