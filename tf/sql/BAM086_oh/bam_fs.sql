

alter procedure prepare_jcic_bam085_all
 as
 insert into #bam085_dedup (idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, cnt)
    select idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, count(*)
    from bam085_curr
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

/* -- end of procedure prepare_jcic_data_all --*/
go


alter procedure generate_bam_vars_315_443
 (@now int)
 as
 declare @i int

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

 /*****************************************************************************************
  *   Initialize #bam085_vars table
  *****************************************************************************************/
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
        FS443_3M = 0, FS443_6M = 0, FS443_9M = 0, FS443_12M = 0,
        FS444_3M = 0, FS444_6M = 0, FS444_9M = 0, FS444_12M = 0,
        FS445_3M = 0, FS445_6M = 0, FS445_9M = 0, FS445_12M = 0,
        FS446_3M = 0, FS446_6M = 0, FS446_9M = 0, FS446_12M = 0,
        FS447_3M = 0, FS447_6M = 0, FS447_9M = 0, FS447_12M = 0,
        FS448_3M = 0, FS448_6M = 0, FS448_9M = 0, FS448_12M = 0,
        FS449_3M = 0, FS449_6M = 0, FS449_9M = 0, FS449_12M = 0,
        FS450_3M = 0, FS450_6M = 0, FS450_9M = 0, FS450_12M = 0,
        FS451_3M = 0, FS451_6M = 0, FS451_9M = 0, FS451_12M = 0,
        FS452_3M = 0, FS452_6M = 0, FS452_9M = 0, FS452_12M = 0,
        FS453_3M = 0, FS453_6M = 0, FS453_9M = 0, FS453_12M = 0,
        FS454_3M = 0, FS454_6M = 0, FS454_9M = 0, FS454_12M = 0,
        FS455_3M = 0, FS455_6M = 0, FS455_9M = 0, FS455_12M = 0,
        FS456_3M = 0, FS456_6M = 0, FS456_9M = 0, FS456_12M = 0,
        FS457_3M = 0, FS457_6M = 0, FS457_9M = 0, FS457_12M = 0,
        FS458_3M = 0, FS458_6M = 0, FS458_9M = 0, FS458_12M = 0,
        FS459_3M = 0, FS459_6M = 0, FS459_9M = 0, FS459_12M = 0,
        FS460_3M = 0, FS460_6M = 0, FS460_9M = 0, FS460_12M = 0,
        FS461_3M = 0, FS461_6M = 0, FS461_9M = 0, FS461_12M = 0,
        FS462_3M = 0, FS462_6M = 0, FS462_9M = 0, FS462_12M = 0,
        FS463_3M = 0, FS463_6M = 0, FS463_9M = 0, FS463_12M = 0,
        FS464_3M = 0, FS464_6M = 0, FS464_9M = 0, FS464_12M = 0,
        FS465_3M = 0, FS465_6M = 0, FS465_9M = 0, FS465_12M = 0,
        FS466_3M = 0, FS466_6M = 0, FS466_9M = 0, FS466_12M = 0,
        FS467_3M = 0, FS467_6M = 0, FS467_9M = 0, FS467_12M = 0,
        FS468_3M = 0, FS468_6M = 0, FS468_9M = 0, FS468_12M = 0,
        FS469_3M = 0, FS469_6M = 0, FS469_9M = 0, FS469_12M = 0,
        FS470_3M = 0, FS470_6M = 0, FS470_9M = 0, FS470_12M = 0,
        FS471_3M = 0, FS471_6M = 0, FS471_9M = 0, FS471_12M = 0,
        FS472_3M = 0, FS472_6M = 0, FS472_9M = 0, FS472_12M = 0,
        FS473_3M = 0, FS473_6M = 0, FS473_9M = 0, FS473_12M = 0,
        FS474_3M = 0, FS474_6M = 0, FS474_9M = 0, FS474_12M = 0,
        FS475_3M = 0, FS475_6M = 0, FS475_9M = 0, FS475_12M = 0,
        FS476_3M = 0, FS476_6M = 0, FS476_9M = 0, FS476_12M = 0,
        FS477_3M = 0, FS477_6M = 0, FS477_9M = 0, FS477_12M = 0,
        FS478_3M = 0, FS478_6M = 0, FS478_9M = 0, FS478_12M = 0,
        FS479_3M = 0, FS479_6M = 0, FS479_9M = 0, FS479_12M = 0,
        FS480_3M = 0, FS480_6M = 0, FS480_9M = 0, FS480_12M = 0,
        FS481_3M = 0, FS481_6M = 0, FS481_9M = 0, FS481_12M = 0,
        FS482_3M = 0, FS482_6M = 0, FS482_9M = 0, FS482_12M = 0,
        FS483_3M = 0, FS483_6M = 0, FS483_9M = 0, FS483_12M = 0,
        FS484_3M = 0, FS484_6M = 0, FS484_9M = 0, FS484_12M = 0,
        FS485_3M = 0, FS485_6M = 0, FS485_9M = 0, FS485_12M = 0,
        FS486_3M = 0, FS486_6M = 0, FS486_9M = 0, FS486_12M = 0,
        FS487_3M = 0, FS487_6M = 0, FS487_9M = 0, FS487_12M = 0,
        FS488_3M = 0, FS488_6M = 0, FS488_9M = 0, FS488_12M = 0,
        FS489_3M = 0, FS489_6M = 0, FS489_9M = 0, FS489_12M = 0,
        FS490_3M = 0, FS490_6M = 0, FS490_9M = 0, FS490_12M = 0,
        FS491_3M = 0, FS491_6M = 0, FS491_9M = 0, FS491_12M = 0,
        FS492_3M = 0, FS492_6M = 0, FS492_9M = 0, FS492_12M = 0,
        FS493_3M = 0, FS493_6M = 0, FS493_9M = 0, FS493_12M = 0,
        FS494_3M = 0, FS494_6M = 0, FS494_9M = 0, FS494_12M = 0,
        FS495_3M = 0, FS495_6M = 0, FS495_9M = 0, FS495_12M = 0,
        FS496_3M = 0, FS496_6M = 0, FS496_9M = 0, FS496_12M = 0,
        FS497_3M = 0, FS497_6M = 0, FS497_9M = 0, FS497_12M = 0,
        FS498_3M = 0, FS498_6M = 0, FS498_9M = 0, FS498_12M = 0,
        FS499_3M = 0, FS499_6M = 0, FS499_9M = 0, FS499_12M = 0,
        FS500_3M = 0, FS500_6M = 0, FS500_9M = 0, FS500_12M = 0,
        FS501_3M = 0, FS501_6M = 0, FS501_9M = 0, FS501_12M = 0,
        FS502_3M = 0, FS502_6M = 0, FS502_9M = 0, FS502_12M = 0,
        FS503_3M = 0, FS503_6M = 0, FS503_9M = 0, FS503_12M = 0,
        FS504_3M = 0, FS504_6M = 0, FS504_9M = 0, FS504_12M = 0,
        FS505_3M = 0, FS505_6M = 0, FS505_9M = 0, FS505_12M = 0,
        FS506_3M = 0, FS506_6M = 0, FS506_9M = 0, FS506_12M = 0,
        FS507_3M = 0, FS507_6M = 0, FS507_9M = 0, FS507_12M = 0,
        FS508_3M = 0, FS508_6M = 0, FS508_9M = 0, FS508_12M = 0,
        FS509_3M = 0, FS509_6M = 0, FS509_9M = 0, FS509_12M = 0,
        FS510_3M = 0, FS510_6M = 0, FS510_9M = 0, FS510_12M = 0,
        FS511_3M = 0, FS511_6M = 0, FS511_9M = 0, FS511_12M = 0,
        FS512_3M = 0, FS512_6M = 0, FS512_9M = 0, FS512_12M = 0,
        FS513_3M = 0, FS513_6M = 0, FS513_9M = 0, FS513_12M = 0,
        FS514_3M = 0, FS514_6M = 0, FS514_9M = 0, FS514_12M = 0,
        FS515_3M = 0, FS515_6M = 0, FS515_9M = 0, FS515_12M = 0,
        FS516_3M = 0, FS516_6M = 0, FS516_9M = 0, FS516_12M = 0,
        FS517_3M = 0, FS517_6M = 0, FS517_9M = 0, FS517_12M = 0,
        FS518_3M = 0, FS518_6M = 0, FS518_9M = 0, FS518_12M = 0,
        FS519_3M = 0, FS519_6M = 0, FS519_9M = 0, FS519_12M = 0,
        FS520_3M = 0, FS520_6M = 0, FS520_9M = 0, FS520_12M = 0,
        FS521_3M = 0, FS521_6M = 0, FS521_9M = 0, FS521_12M = 0,
        FS522_3M = 0, FS522_6M = 0, FS522_9M = 0, FS522_12M = 0,
        FS523_3M = 0, FS523_6M = 0, FS523_9M = 0, FS523_12M = 0,
        FS524_3M = 0, FS524_6M = 0, FS524_9M = 0, FS524_12M = 0,
        FS525_3M = 0, FS525_6M = 0, FS525_9M = 0, FS525_12M = 0,
        FS526_3M = 0, FS526_6M = 0, FS526_9M = 0, FS526_12M = 0,
        FS527_3M = 0, FS527_6M = 0, FS527_9M = 0, FS527_12M = 0,
        FS528_3M = 0, FS528_6M = 0, FS528_9M = 0, FS528_12M = 0,
        FS529_3M = 0, FS529_6M = 0, FS529_9M = 0, FS529_12M = 0,
        FS530_3M = 0, FS530_6M = 0, FS530_9M = 0, FS530_12M = 0,
        FS531_3M = 0, FS531_6M = 0, FS531_9M = 0, FS531_12M = 0,
        FS532_3M = 0, FS532_6M = 0, FS532_9M = 0, FS532_12M = 0,
        FS533_3M = 0, FS533_6M = 0, FS533_9M = 0, FS533_12M = 0,
        FS534_3M = 0, FS534_6M = 0, FS534_9M = 0, FS534_12M = 0,
        FS535_3M = 0, FS535_6M = 0, FS535_9M = 0, FS535_12M = 0,
        FS536_3M = 0, FS536_6M = 0, FS536_9M = 0, FS536_12M = 0,
        FS537_3M = 0, FS537_6M = 0, FS537_9M = 0, FS537_12M = 0,
        FS538_3M = 0, FS538_6M = 0, FS538_9M = 0, FS538_12M = 0,
        FS539_3M = 0, FS539_6M = 0, FS539_9M = 0, FS539_12M = 0,
        FS540_3M = 0, FS540_6M = 0, FS540_9M = 0, FS540_12M = 0,
        FS541_3M = 0, FS541_6M = 0, FS541_9M = 0, FS541_12M = 0,
        FS542_3M = 0, FS542_6M = 0, FS542_9M = 0, FS542_12M = 0,
        FS543_3M = 0, FS543_6M = 0, FS543_9M = 0, FS543_12M = 0,
        FS544_3M = 0, FS544_6M = 0, FS544_9M = 0, FS544_12M = 0,
        FS545_3M = 0, FS545_6M = 0, FS545_9M = 0, FS545_12M = 0,
        FS546_3M = 0, FS546_6M = 0, FS546_9M = 0, FS546_12M = 0,
        FS547_3M = 0, FS547_6M = 0, FS547_9M = 0, FS547_12M = 0,
        FS548_3M = 0, FS548_6M = 0, FS548_9M = 0, FS548_12M = 0,
        FS549_3M = 0, FS549_6M = 0, FS549_9M = 0, FS549_12M = 0,
        FS550_3M = 0, FS550_6M = 0, FS550_9M = 0, FS550_12M = 0,
        FS551_3M = 0, FS551_6M = 0, FS551_9M = 0, FS551_12M = 0,
        FS552_3M = 0, FS552_6M = 0, FS552_9M = 0, FS552_12M = 0,
        FS553_3M = 0, FS553_6M = 0, FS553_9M = 0, FS553_12M = 0,
        FS554_3M = 0, FS554_6M = 0, FS554_9M = 0, FS554_12M = 0
    from #bam085_dedup as a
    where a.idn = #bam085_vars.idn;

 /*****************************************************************************************
  * FS334 曾經最高遲繳月數 1
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    group by idn

 update #bam085_vars
    set FS334 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS315 有擔保貸款 曾經最高遲繳月數 1 account_code2 in ('S','W')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code2 in ('S','W')
       or account_code = 'K'
    group by idn

 update #bam085_vars
    set FS315 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * start making FS316 有擔保貸款 - 短期 曾經最高遲繳月數 1 account_code2 in ('S','W')  account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
    group by idn

 update #bam085_vars
    set FS316 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS317 有擔保貸款 - 中期 曾經最高遲繳月數 1 account_code2 in ('S','W')  account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code in ('H','S')
      and account_code2 in ('S','W')
    group by idn

 update #bam085_vars
    set FS317 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS318 有擔保貸款 - 長期 曾經最高遲繳月數 1 account_code2 in ('S','W')  account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code in ('I','T')
      and account_code2 in ('S','W')
    group by idn

 update #bam085_vars
    set FS318 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS319 無擔保貸款 曾經最高遲繳月數 1 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
    group by idn

 update #bam085_vars
    set FS319 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS320 無擔保貸款 - 短期 曾經最高遲繳月數 1 account_code2 is NULL account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
    group by idn

 update #bam085_vars
    set FS320 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS321 無擔保貸款 - 中期 曾經最高遲繳月數 1 account_code2 is NULL account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
    group by idn

 update #bam085_vars
    set FS321 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS322 無擔保貸款 - 長期 曾經最高遲繳月數 1 account_code2 is NULL account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
    group by idn

 update #bam085_vars
    set FS322 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS323 財政部優惠貸款筆數 曾經最高遲繳月數 1 account_code2 in ('V','W')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code2 in ('V','W')
    group by idn

 update #bam085_vars
    set FS323 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS324 不動產貸款筆數 曾經最高遲繳月數 1 purpose_code = 1
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where purpose_code = '1'
    group by idn

 update #bam085_vars
    set FS324 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS325 動產貸款筆數 曾經最高遲繳月數 1 purpose_code = 2
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where purpose_code = '2'
    group by idn

 update #bam085_vars
    set FS325 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS326 週轉金貸款筆數 曾經最高遲繳月數 1 purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where purpose_code = '4'
    group by idn

 update #bam085_vars
    set FS326 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS327 存保單質押筆數 (無擔保) 曾經最高遲繳月數 1 account_code2 is NULL account_code in ('K') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code='K'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
    group by idn

 update #bam085_vars
    set FS327 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS328 現金卡筆數 (無擔保) 曾經最高遲繳月數 1 account_code2 is NULL account_code in ('Y') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where account_code = 'Y'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
    group by idn

 update #bam085_vars
    set FS328 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn

 /*****************************************************************************************
  * FS329 Co-Loan 曾經最高遲繳月數 1 Co-Loan = 'Y'
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where co_loan = 'Y'
    group by idn

 update #bam085_vars
    set FS329 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn

 /*****************************************************************************************
  * FS330 在同一金融機構沒有房貸的購屋用無擔保貸款  曾經最高遲繳月數 1
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn

 update #bam085_vars
    set FS330 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS331 除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款  曾經最高遲繳月數 1
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, v1)    /*購屋用貸款外的無擔保貸款*/
    select idn, max(bucket)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn
 insert into #tmp (idn, v1)       /*有房貸的購屋用無擔保貸款*/
    select idn, max(bucket)
    from #bam085_bucket b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn

 insert into #tmp1 (idn, v1)
    select idn, max(v1)
    from #tmp
    group by idn

 update #bam085_vars
    set FS331 = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS332 有擔保購屋貸款  曾經最高遲繳月數 1 purpose_code = 1 account_code2 is not NULL
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
    group by idn

 update #bam085_vars
    set FS332 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * FS333 無擔保非購屋貸款  曾經最高遲繳月數 1 purpose_code != 1 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, max(bucket)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and purpose_code <> '1'
    group by idn

 update #bam085_vars
    set FS333 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn

 /*****************************************************************************************
  * RS101 最近遲繳距今月數 1 # records
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where bucket <> 0
    group by idn

 update #bam085_vars
    set RS101 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS102 有擔保貸款 最近遲繳距今月數 1 account_code2 in ('S','W')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where (account_code2 in ('S','W')
       or account_code = 'K')
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS102 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS103 有擔保貸款 - 短期 最近遲繳距今月數 1 account_code2 in ('S','W')  account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS103 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS104 有擔保貸款 - 中期 最近遲繳距今月數 1 account_code2 in ('S','W')  account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where account_code in ('H','S')
      and account_code2 in ('S','W')
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS104 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS105 有擔保貸款 - 長期 最近遲繳距今月數 1 account_code2 in ('S','W')  account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where account_code in ('I','T')
      and account_code2 in ('S','W')
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS105 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS106 無擔保貸款 最近遲繳距今月數 1 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS106 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS107 無擔保貸款 - 短期 最近遲繳距今月數 1 account_code2 is NULL account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS107 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS108 無擔保貸款 - 中期 最近遲繳距今月數 1 account_code2 is NULL account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS108 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS109 無擔保貸款 - 長期 最近遲繳距今月數 1 account_code2 is NULL account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS109 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS110 財政部優惠貸款筆數 最近遲繳距今月數 1 account_code2 in ('V','W')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where account_code2 in ('V','W')
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS110 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS111 不動產貸款筆數 最近遲繳距今月數 1 purpose_code = 1
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where purpose_code = '1'
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS111 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS112 動產貸款筆數 最近遲繳距今月數 1 purpose_code = 2
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where purpose_code = '2'
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS112 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS113 週轉金貸款筆數 最近遲繳距今月數 1 purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where purpose_code = '4'
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS113 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS114 存保單質押筆數 (無擔保) 最近遲繳距今月數 1 account_code2 is NULL account_code in ('K') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where account_code='K'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS114 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS115 現金卡筆數 (無擔保) 最近遲繳距今月數 1 account_code2 is NULL account_code in ('Y') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where account_code = 'Y'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS115 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn

 /*****************************************************************************************
  * RS116 Co-Loan 筆數 最近遲繳距今距今月數 1 Co-Loan = 'Y'
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from  #bam085_bucket
    where co_loan = 'Y'
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS116 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn

 /*****************************************************************************************
  * RS117 在同一金融機構沒有房貸的購屋用無擔保貸款 最近遲繳距今距今月數 1
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket b
    where b.bucket <> 0
      and (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn

 update #bam085_vars
    set RS117 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS118 除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款 最近遲繳距今距今月數 1
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)    /*購屋用貸款外的無擔保貸款*/
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where bucket <> 0
      and (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn
 insert into #tmp (idn, v1)       /*有房貸的購屋用無擔保貸款*/
    select idn, @now - max(mon_since)
    from #bam085_bucket b
    where b.bucket <> 0
      and (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn

 update #bam085_vars
    set RS118 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS119 有擔保購屋貸款 最近遲繳距今距今月數 1 purpose_code = 1 account_code2 is not NULL
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS119 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn
 /*****************************************************************************************
  * RS120 無擔保非購屋貸款 最近遲繳距今距今月數 1 purpose_code != 1 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, @now - max(mon_since)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and purpose_code <> '1'
      and bucket <> 0
    group by idn

 update #bam085_vars
    set RS120 = v1
    from #tmp as a
    where a.idn = #bam085_vars.idn


 /*First accumulate cured flag by month, then find max, avg within a season               */
 /*****************************************************************************************
  * FS354 總放款曾經cured最多最多筆數 4 # records
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
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
    set FS354_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS354_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS354_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS354_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS355 有擔保貸款曾經cured最多筆數 4 account_code2 in ('S','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code2 in ('S','W')
       or account_code = 'K'
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
    set FS355_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS355_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS355_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS355_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS356 有擔保貸款曾經cured最多筆數 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
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
    set FS356_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS356_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS356_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS356_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS357 有擔保貸款曾經cured最多筆數 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('H','S')
      and account_code2 in ('S','W')
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
    set FS357_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS357_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS357_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS357_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS358 有擔保貸款曾經cured最多筆數 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('I','T')
      and account_code2 in ('S','W')
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
    set FS358_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS358_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS358_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS358_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS359 無擔保貸款曾經cured最多筆數 4 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
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
    set FS359_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS359_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS359_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS359_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS360 無擔保貸款曾經cured最多筆數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
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
    set FS360_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS360_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS360_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS360_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS361 無擔保貸款曾經cured最多筆數 - 中期 4 account_code2 is NULL account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
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
    set FS361_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS361_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS361_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS361_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS362 無擔保貸款曾經cured最多筆數 - 長期 4 account_code2 is NULL account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
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
    set FS362_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS362_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS362_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS362_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS363 財政部優惠貸款曾經cured最多筆數 4 account_code2 in ('V','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code2 in ('V','W')
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
    set FS363_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS363_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS363_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS363_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS364 不動產貸款曾經cured最多筆數 4 purpose_code = 1
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where purpose_code = '1'
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
    set FS364_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS364_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS364_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS364_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS365 動產貸款曾經cured最多筆數 4 purpose_code = 2
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where purpose_code = '2'
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
    set FS365_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS365_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS365_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS365_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS366 週轉金貸款曾經cured最多筆數 4 purpose_code = 4
  *****************************************************************************************/
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

 /*****************************************************************************************
  * FS367 存保單質押曾經cured最多筆數 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2 =''  or account_code2 is null)
      and account_code = 'K'
      and purpose_code = '4'
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
    set FS367_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS367_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS367_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS367_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS368 現金卡曾經cured最多筆數 (無擔保) 4 account_code2 is NULL account_code in ('Y') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code = 'Y'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
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
    set FS368_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS368_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS368_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS368_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS369 Co-Loan 曾經cured最多筆數 4 Co-Loan = 'Y'
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where co_loan = 'Y'
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
    set FS369_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS369_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS369_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS369_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS370 在同一金融機構沒有房貸的購屋用無擔保貸款曾經cured最多筆數 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
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
    set FS370_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS370_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS370_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS370_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS371 除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款曾經cured最多筆數 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)  /*購屋用貸款外的無擔保貸款*/
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, mon_since
 insert into #tmp (idn, mon, v1)  /*有房貸的購屋用無擔保貸款*/
    select idn, mon_since, sum(cured)
    from #bam085_bucket b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
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
    set FS371_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS371_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS371_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS371_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS372 有擔保購屋貸款曾經cured最多筆數 4 purpose_code = 1 account_code2 is not NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
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
    set FS372_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS372_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS372_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS372_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS373 無擔保非購屋貸款曾經cured最多筆數 4 purpose_code != 1 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and purpose_code <> '1'
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
    set FS373_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS373_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS373_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS373_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12


 /*****************************************************************************************
  * FS374 總放款曾經cured平均筆數 4 # records
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS374_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS374_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS374_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS374_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS375 有擔保貸款曾經cured平均筆數 4 account_code2 in ('S','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code2 in ('S','W')
       or account_code = 'K'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS375_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS375_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS375_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS375_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS376 有擔保貸款曾經cured平均筆數 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS376_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS376_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS376_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS376_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS377 有擔保貸款曾經cured平均筆數 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('H','S')
      and account_code2 in ('S','W')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS377_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS377_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS377_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS377_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS378 有擔保貸款曾經cured平均筆數 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('I','T')
      and account_code2 in ('S','W')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS378_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS378_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS378_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS378_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS379 無擔保貸款曾經cured平均筆數 4 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS379_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS379_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS379_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS379_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS380 無擔保貸款曾經cured平均筆數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS380_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS380_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS380_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS380_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS381 無擔保貸款曾經cured平均筆數 - 中期 4 account_code2 is NULL account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS381_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS381_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS381_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS381_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS382 無擔保貸款曾經cured平均筆數 - 長期 4 account_code2 is NULL account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS382_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS382_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS382_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS382_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS383 財政部優惠貸款曾經cured平均筆數 4 account_code2 in ('V','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code2 in ('V','W')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS383_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS383_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS383_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS383_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS384 不動產貸款曾經cured平均筆數 4 purpose_code = 1
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where purpose_code = '1'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS384_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS384_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS384_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS384_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS385 動產貸款曾經cured平均筆數 4 purpose_code = 2
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where purpose_code = '2'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS385_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS385_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS385_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS385_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS386 週轉金貸款曾經cured平均筆數 4 purpose_code = 4
  *****************************************************************************************/
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
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS386_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS386_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS386_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS386_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS387 存保單質押曾經cured平均筆數 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2 =''  or account_code2 is null)
      and account_code = 'K'
      and purpose_code = '4'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS387_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS387_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS387_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS387_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS388 現金卡曾經cured平均筆數 (無擔保) 4 account_code2 is NULL account_code in ('Y') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where account_code = 'Y'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS388_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS388_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS388_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS388_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS389 Co-Loan 曾經cured平均筆數 4 Co-Loan = 'Y'
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where co_loan = 'Y'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS389_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS389_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS389_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS389_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS390 在同一金融機構沒有房貸的購屋用無擔保貸款曾經cured平均筆數 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS390_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS390_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS390_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS390_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS391 除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款曾經cured平均筆數 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)    /*購屋用貸款外的無擔保貸款*/
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, mon_since
 insert into #tmp (idn, mon, v1)      /*有房貸的購屋用無擔保貸款*/
    select idn, mon_since, sum(cured)
    from #bam085_bucket b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS391_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS391_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS391_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS391_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS392 有擔保購屋貸款曾經cured平均筆數 4 purpose_code = 1 account_code2 is not NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS392_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS392_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS392_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS392_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS393 無擔保非購屋貸款曾經cured平均筆數 4 purpose_code != 1 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(cured)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and purpose_code <> '1'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS393_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS393_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS393_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS393_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12




 /*First accumulate decreased flag by month, then find max, avg within a season               */
 /*****************************************************************************************
  * FS394 總放款曾經Bucket decrease最多筆數 4 # records
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
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
    set FS394_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS394_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS394_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS394_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS395 有擔保貸款曾經Bucket decrease最多筆數 4 account_code2 in ('S','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code2 in ('S','W')
       or account_code = 'K'
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
    set FS395_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS395_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS395_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS395_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS396 有擔保貸款曾經Bucket decrease最多筆數 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
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
    set FS396_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS396_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS396_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS396_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS397 有擔保貸款曾經Bucket decrease最多筆數 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('H','S')
      and account_code2 in ('S','W')
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
    set FS397_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS397_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS397_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS397_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS398 有擔保貸款曾經Bucket decrease最多筆數 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('I','T')
      and account_code2 in ('S','W')
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
    set FS398_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS398_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS398_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS398_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS399 無擔保貸款曾經Bucket decrease最多筆數 4 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
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
    set FS399_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS399_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS399_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS399_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS400 無擔保貸款曾經Bucket decrease最多筆數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
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
    set FS400_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS400_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS400_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS400_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS401 無擔保貸款曾經Bucket decrease最多筆數 - 中期 4 account_code2 is NULL account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
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
    set FS401_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS401_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS401_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS401_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS402 無擔保貸款曾經Bucket decrease最多筆數 - 長期 4 account_code2 is NULL account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
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
    set FS402_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS402_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS402_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS402_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS403 財政部優惠貸款曾經Bucket decrease最多筆數 4 account_code2 in ('V','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code2 in ('V','W')
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
    set FS403_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS403_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS403_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS403_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS404 不動產貸款曾經Bucket decrease最多筆數 4 purpose_code = 1
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where purpose_code = '1'
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
    set FS404_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS404_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS404_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS404_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS405 動產貸款曾經Bucket decrease最多筆數 4 purpose_code = 2
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where purpose_code = '2'
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
    set FS405_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS405_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS405_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS405_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS406 週轉金貸款曾經Bucket decrease最多筆數 4 purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
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
    set FS406_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS406_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS406_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS406_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS407 存保單質押曾經Bucket decrease最多筆數 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2 =''  or account_code2 is null)
      and account_code = 'K'
      and purpose_code = '4'
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
    set FS407_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS407_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS407_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS407_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS408 現金卡曾經Bucket decrease最多筆數 (無擔保) 4 account_code2 is NULL account_code in ('Y') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code = 'Y'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
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
    set FS408_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS408_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS408_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS408_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS409 Co-Loan 曾經Bucket decrease最多筆數 4 Co-Loan = 'Y'
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where co_loan = 'Y'
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
    set FS409_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS409_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS409_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS409_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS410 在同一金融機構沒有房貸的購屋用無擔保貸款曾經Bucket decrease最多筆數 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
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
    set FS410_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS410_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS410_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS410_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS411 除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款曾經Bucket decrease最多筆數 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)   /*購屋用貸款外的無擔保貸款*/
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, mon_since
 insert into #tmp (idn, mon, v1)      /*有房貸的購屋用無擔保貸款*/
    select idn, mon_since, sum(decreased)
    from #bam085_bucket b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
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
    set FS411_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS411_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS411_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS411_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS412 有擔保購屋貸款曾經Bucket decrease最多筆數 4 purpose_code = 1 account_code2 is not NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
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
    set FS412_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS412_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS412_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS412_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS413 無擔保非購屋貸款曾經Bucket decrease最多筆數 4 purpose_code != 1 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and purpose_code <> '1'
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
    set FS413_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS413_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS413_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS413_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12


 /*****************************************************************************************
  * FS414 總放款曾經Bucket decrease平均筆數 4 # records
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS414_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS414_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS414_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS414_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS415 有擔保貸款曾經Bucket decrease平均筆數 4 account_code2 in ('S','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code2 in ('S','W')
       or account_code = 'K'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS415_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS415_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS415_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS415_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS416 有擔保貸款曾經Bucket decrease平均筆數 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS416_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS416_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS416_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS416_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS417 有擔保貸款曾經Bucket decrease平均筆數 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('H','S')
      and account_code2 in ('S','W')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS417_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS417_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS417_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS417_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS418 有擔保貸款曾經Bucket decrease平均筆數 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('I','T')
      and account_code2 in ('S','W')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS418_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS418_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS418_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS418_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS419 無擔保貸款曾經Bucket decrease平均筆數 4 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS419_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS419_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS419_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS419_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS420 無擔保貸款曾經Bucket decrease平均筆數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS420_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS420_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS420_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS420_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS421 無擔保貸款曾經Bucket decrease平均筆數 - 中期 4 account_code2 is NULL account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS421_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS421_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS421_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS421_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS422 無擔保貸款曾經Bucket decrease平均筆數 - 長期 4 account_code2 is NULL account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS422_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS422_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS422_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS422_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS423 財政部優惠貸款曾經Bucket decrease平均筆數 4 account_code2 in ('V','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code2 in ('V','W')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS423_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS423_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS423_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS423_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS424 不動產貸款曾經Bucket decrease平均筆數 4 purpose_code = 1
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where purpose_code = '1'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS424_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS424_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS424_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS424_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS425 動產貸款曾經Bucket decrease平均筆數 4 purpose_code = 2
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where purpose_code = '2'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS425_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS425_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS425_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS425_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS426 週轉金貸款曾經Bucket decrease平均筆數 4 purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where purpose_code = '4'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS426_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS426_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS426_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS426_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS427 存保單質押曾經Bucket decrease平均筆數 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2 =''  or account_code2 is null)
      and account_code = 'K'
      and purpose_code = '4'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS427_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS427_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS427_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS427_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS428 現金卡曾經Bucket decrease平均筆數 (無擔保) 4 account_code2 is NULL account_code in ('Y') purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where account_code = 'Y'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS428_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS428_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS428_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS428_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS429 Co-Loan 曾經Bucket decrease平均筆數 4 Co-Loan = 'Y'
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where co_loan = 'Y'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS429_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS429_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS429_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS429_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS430 在同一金融機構沒有房貸的購屋用無擔保貸款曾經Bucket decrease平均筆數 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS430_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS430_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS430_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS430_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS431 除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款曾經Bucket decrease平均筆數 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)    /*購屋用貸款外的無擔保貸款*/
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, mon_since
 insert into #tmp (idn, mon, v1)       /*有房貸的購屋用無擔保貸款*/
    select idn, mon_since, sum(decreased)
    from #bam085_bucket b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS431_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS431_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS431_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS431_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS432 有擔保購屋貸款曾經Bucket decrease平均筆數 4 purpose_code = 1 account_code2 is not NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS432_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS432_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS432_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS432_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS433 無擔保非購屋貸款曾經Bucket decrease平均筆數 4 purpose_code != 1 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, sum(decreased)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and purpose_code <> '1'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS433_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS433_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS433_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS433_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*First accumulate month_decreased flag by month, then find max, avg within a season               */
 /*****************************************************************************************
  * FS434 總放款曾經Bucket decrease最多月數 4 # records
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket

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
    set FS434_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS434_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS434_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS434_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS435 有擔保貸款曾經Bucket decrease最多月數 4 account_code2 in ('S','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code2 in ('S','W')
       or account_code = 'K'

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
    set FS435_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS435_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS435_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS435_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS436 有擔保貸款曾經Bucket decrease最多月數 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')

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
    set FS436_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS436_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS436_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS436_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS437 有擔保貸款曾經Bucket decrease最多月數 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('H','S')
      and account_code2 in ('S','W')

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
    set FS437_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS437_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS437_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS437_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS438 有擔保貸款曾經Bucket decrease最多月數 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('I','T')
      and account_code2 in ('S','W')

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
    set FS438_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS438_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS438_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS438_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS439 無擔保貸款曾經Bucket decrease最多月數 4 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'

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
    set FS439_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS439_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS439_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS439_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS440 無擔保貸款曾經Bucket decrease最多月數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)

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
    set FS440_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS440_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS440_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS440_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS441 無擔保貸款曾經Bucket decrease最多月數 - 中期 4 account_code2 is NULL account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)

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
    set FS441_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS441_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS441_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS441_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS442 無擔保貸款曾經Bucket decrease最多月數 - 長期 4 account_code2 is NULL account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)

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
    set FS442_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS442_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS442_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS442_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS443 財政部優惠貸款曾經Bucket decrease最多月數 4 account_code2 in ('V','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code2 in ('V','W')

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
    set FS443_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS443_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS443_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS443_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /* Variables FS444 - FS527 are untested*/
 /*****************************************************************************************
  * start making FS444	不動產貸款曾經Bucket decrease最多月數	4	purpose_code = 1
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, max(month_dec)
    from #bam085_bucket
    where purpose_code = '1'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS444_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS444_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS444_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS444_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS445	動產貸款曾經Bucket decrease最多月數	4	purpose_code = 2
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, max(month_dec)
    from #bam085_bucket
    where purpose_code = '2'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS445_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS445_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS445_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS445_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS446	週轉金貸款曾經Bucket decrease最多月數	4	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, max(month_dec)
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
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS446_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS446_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS446_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS446_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * start making FS447	存保單質押曾經Bucket decrease最多月數 (無擔保)	4	account_code2 is NULL	account_code in ('K')	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, max(month_dec)
    from #bam085_bucket
    where (account_code2 =''  or account_code2 is null)
      and account_code = 'K'
      and purpose_code = '4'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS447_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS447_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS447_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS447_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS448	現金卡曾經Bucket decrease最多月數 (無擔保)	4	account_code2 is NULL	account_code in ('Y')	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, max(month_dec)
    from #bam085_bucket
    where account_code = 'Y'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS448_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS448_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS448_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS448_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS449	Co-Loan 曾經Bucket decrease最多月數	4	Co-Loan = 'Y'
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, max(month_dec)
    from #bam085_bucket
    where co_loan = 'Y'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS449_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS449_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS449_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS449_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * start making FS450	在同一金融機構沒有房貸的購屋用無擔保貸款曾經Bucket decrease最多月數	4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, max(month_dec)
    from #bam085_bucket b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS450_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS450_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS450_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS450_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS451	除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款曾經Bucket decrease最多月數	4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)    /*購屋用貸款外的無擔保貸款*/
    select idn, mon_since, max(month_dec)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, mon_since
 insert into #tmp (idn, mon, v1)       /*有房貸的購屋用無擔保貸款*/
    select idn, mon_since, max(month_dec)
    from #bam085_bucket b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS451_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS451_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS451_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS451_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS452	有擔保購屋貸款曾經Bucket decrease最多月數	4	purpose_code = 1	account_code2 is not NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, max(month_dec)
    from #bam085_bucket
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS452_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS452_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS452_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS452_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS453	無擔保非購屋貸款曾經Bucket decrease最多月數	4	purpose_code != 1	account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, max(month_dec)
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and purpose_code <> '1'
    group by idn, mon_since

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS453_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS453_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS453_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS453_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS454 總放款曾經Bucket decrease平均月數 4 # records
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where month_dec <> 0
 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS454_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS454_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS454_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS454_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS455 有擔保貸款曾經Bucket decrease平均月數 4 account_code2 in ('S','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code2 in ('S','W')
       or account_code = 'K'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS455_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS455_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS455_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS455_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS456 有擔保貸款曾經Bucket decrease平均月數 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS456_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS456_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS456_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS456_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS457 有擔保貸款曾經Bucket decrease平均月數 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('H','S')
      and account_code2 in ('S','W')
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS457_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS457_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS457_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS457_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS458 有擔保貸款曾經Bucket decrease平均月數 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('I','T')
      and account_code2 in ('S','W')
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS458_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS458_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS458_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS458_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS459 無擔保貸款曾經Bucket decrease平均月數 4 account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS459_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS459_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS459_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS459_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * FS460 無擔保貸款曾經Bucket decrease平均月數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS460_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS460_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS460_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS460_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS461 無擔保貸款曾經Bucket decrease平均月數 - 中期 4 account_code2 is NULL account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS461_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS461_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS461_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS461_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS462 無擔保貸款曾經Bucket decrease平均月數 - 長期 4 account_code2 is NULL account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS462_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS462_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS462_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS462_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * FS463 財政部優惠貸款曾經Bucket decrease平均月數 4 account_code2 in ('V','W')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code2 in ('V','W')
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS463_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS463_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS463_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS463_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /* Variables FS464 - FS527 are untested*/
 /*****************************************************************************************
  * start making FS464	不動產貸款曾經Bucket decrease平均月數	4	purpose_code = 1
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where purpose_code = '1'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS464_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS464_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS464_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS464_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS465	動產貸款曾經Bucket decrease平均月數	4	purpose_code = 2
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where purpose_code = '2'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS465_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS465_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS465_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS465_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS466	週轉金貸款曾經Bucket decrease平均月數	4	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where purpose_code = '4'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS466_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS466_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS466_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS466_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * start making FS467	存保單質押曾經Bucket decrease平均月數 (無擔保)	4	account_code2 is NULL	account_code in ('K')	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where (account_code2 =''  or account_code2 is null)
      and account_code = 'K'
      and purpose_code = '4'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS467_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS467_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS467_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS467_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS468	現金卡曾經Bucket decrease平均月數 (無擔保)	4	account_code2 is NULL	account_code in ('Y')	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where account_code = 'Y'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS468_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS468_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS468_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS468_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS469	Co-Loan 曾經Bucket decrease平均月數	4	Co-Loan = 'Y'
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where co_loan = 'Y'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS469_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS469_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS469_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS469_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************
  * start making FS470	在同一金融機構沒有房貸的購屋用無擔保貸款曾經Bucket decrease平均月數	4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS470_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS470_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS470_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS470_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS471	除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款曾經Bucket decrease平均月數	4
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)    /*購屋用貸款外的無擔保貸款*/
    select idn, mon_since, month_dec
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
      and month_dec <> 0
 insert into #tmp (idn, mon, v1)       /*有房貸的購屋用無擔保貸款*/
    select idn, mon_since, month_dec
    from #bam085_bucket b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_bucket c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS471_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS471_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS471_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS471_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS472	有擔保購屋貸款曾經Bucket decrease平均月數	4	purpose_code = 1	account_code2 is not NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS472_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS472_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS472_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS472_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************
  * start making FS473	無擔保非購屋貸款曾經Bucket decrease平均月數	4	purpose_code != 1	account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, mon_since, month_dec
    from #bam085_bucket
    where (account_code2 = '' or account_code2 is null)
      and purpose_code <> '1'
      and month_dec <> 0

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS473_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS473_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS473_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS473_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS474 繳款紀錄在X月內的總放款最多筆數 4 # records                                     */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS474_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS474_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS474_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS474_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS475 繳款紀錄在X月內的總放款最多銀行數 4 # distinct issuer (3 digit)                   */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(distinct bank_code2)
    from #bam085_dedup
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS475_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS475_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS475_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS475_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS476 繳款紀錄在X月內的還清最多筆數 4 loan_amt = 0 and pass_due_amt = 0 and account_code <> 'Y'*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where loan_amt = 0
      and pass_due_amt = 0
      and account_code <> 'Y'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS476_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS476_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS476_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS476_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS477 繳款紀錄在X月內的有擔保貸款最多筆數 4 account_code2 in ('S','W')                  */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code2 in ('S','W')
       or account_code = 'K'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS477_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS477_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS477_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS477_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS478 繳款紀錄在X月內的有擔保貸款最多筆數 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS478_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS478_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS478_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS478_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS479 繳款紀錄在X月內的有擔保貸款最多筆數 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('H','S')
      and account_code2 in ('S','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS479_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS479_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS479_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS479_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS480 繳款紀錄在X月內的有擔保貸款最多筆數 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('I','T')
      and account_code2 in ('S','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS480_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS480_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS480_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS480_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS481 繳款紀錄在X月內的無擔保貸款最多筆數 4 account_code2 is NULL and account_code <> 'K'*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS481_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS481_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS481_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS481_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS482 繳款紀錄在X月內的無擔保貸款最多筆數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS482_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS482_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS482_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS482_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS483 繳款紀錄在X月內的無擔保貸款最多筆數 - 中期 4 account_code2 is NULL account_code in ('H','S') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS483_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS483_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS483_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS483_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS484 繳款紀錄在X月內的無擔保貸款最多筆數 - 長期 4 account_code2 is NULL account_code in ('I','T')*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS484_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS484_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS484_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS484_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS485 繳款紀錄在X月內的逾期最多筆數 4 pass_due_amt > 0                                  */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where pass_due_amt > 0
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS485_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS485_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS485_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS485_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS486 繳款紀錄在X月內的催收最多筆數 4 account_code  = 'A'                               */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code  = 'A'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS486_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS486_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS486_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS486_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS487 繳款紀錄在X月內的呆帳最多筆數 4 account_code  = 'B'                               */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code  = 'B'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS487_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS487_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS487_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS487_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS488 繳款紀錄在X月內的財政部優惠貸款最多筆數 4 account_code2 in ('V','W')              */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code2 in ('V','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS488_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS488_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS488_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS488_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS489 繳款紀錄在X月內的不動產貸款最多筆數 4 purpose_code = 1                            */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '1'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS489_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS489_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS489_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS489_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS490 繳款紀錄在X月內的動產貸款最多筆數 4 purpose_code = 2                              */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '2'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS490_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS490_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS490_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS490_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS491 繳款紀錄在X月內的企業投資貸款最多筆數 4 purpose_code = 3                          */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '3'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS491_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS491_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS491_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS491_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS492 繳款紀錄在X月內的週轉金貸款最多筆數 4 purpose_code = 4                            */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '4'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS492_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS492_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS492_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS492_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS493 繳款紀錄在X月內的助學貸款最多筆數 (無擔保) 4 account_code2 is NULL account_code in ('Z') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '4'
      and (account_code2 is NULL or account_code2 = '')
      and account_code = 'Z'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS493_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS493_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS493_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS493_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS494 繳款紀錄在X月內的存保單質押最多筆數 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 =''  or account_code2 is null)
      and account_code = 'K'
      and purpose_code = '4'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS494_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS494_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS494_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS494_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS495 繳款紀錄在X月內的現金卡最多筆數 (無擔保) 4 account_code2 is NULL account_code in ('Y') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('Y')
      and (account_code2 =''  or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS495_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS495_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS495_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS495_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS496 繳款紀錄在X月內的Co-Loan 最多筆數 4 Co-Loan = 'Y'                                 */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where co_loan = 'Y'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS496_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS496_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS496_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS496_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12


 /*****************************************************************************************/
 /* FS497 繳款紀錄在X月內的在同一金融機構沒有房貸的購屋用無擔保貸款最多筆數 4             */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_dedup c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS497_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS497_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS497_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS497_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS498 繳款紀錄在X月內的除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款最多筆數 */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)  /*購屋用貸款外的無擔保貸款*/
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, (mon_since - cycle + 1)
 insert into #tmp (idn, mon, v1)  /*有房貸的購屋用無擔保貸款*/
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_dedup c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS498_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS498_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS498_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS498_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS499 繳款紀錄在X月內的有擔保購屋貸款最多筆數 4 purpose_code = 1 account_code2 is not NULL*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
     group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS499_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS499_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS499_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS499_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS500 繳款紀錄在X月內的無擔保非購屋貸款最多筆數 4 purpose_code != 1 account_code2 is NULL*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS500_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS500_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS500_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS500_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS501 繳款紀錄在X月內的總放款平均筆數 4 # records                                     */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS501_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS501_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS501_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS501_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS502 繳款紀錄在X月內的總放款平均銀行數 4 # distinct issuer (3 digit)                   */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(distinct bank_code2)
    from #bam085_dedup
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS502_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS502_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS502_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS502_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS503 繳款紀錄在X月內的還清平均筆數 4 loan_amt = 0 and pass_due_amt = 0 and account_code <> 'Y'*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where loan_amt = 0
      and pass_due_amt = 0
      and account_code <> 'Y'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS503_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS503_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS503_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS503_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS504 繳款紀錄在X月內的有擔保貸款平均筆數 4 account_code2 in ('S','W')                  */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code2 in ('S','W')
       or account_code = 'K'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS504_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS504_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS504_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS504_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS505 繳款紀錄在X月內的有擔保貸款平均筆數 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS505_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS505_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS505_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS505_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS506 繳款紀錄在X月內的有擔保貸款平均筆數 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('H','S')
      and account_code2 in ('S','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS506_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS506_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS506_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS506_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS507 繳款紀錄在X月內的有擔保貸款平均筆數 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('I','T')
      and account_code2 in ('S','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS507_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS507_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS507_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS507_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS508 繳款紀錄在X月內的無擔保貸款平均筆數 4 account_code2 is NULL and account_code <> 'K'*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS508_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS508_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS508_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS508_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS509 繳款紀錄在X月內的無擔保貸款平均筆數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS509_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS509_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS509_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS509_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS510 繳款紀錄在X月內的無擔保貸款平均筆數 - 中期 4 account_code2 is NULL account_code in ('H','S') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS510_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS510_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS510_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS510_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS511 繳款紀錄在X月內的無擔保貸款平均筆數 - 長期 4 account_code2 is NULL account_code in ('I','T')*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS511_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS511_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS511_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS511_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS512 繳款紀錄在X月內的逾期平均筆數 4 pass_due_amt > 0                                  */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where pass_due_amt > 0
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS512_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS512_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS512_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS512_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS513 繳款紀錄在X月內的催收平均筆數 4 account_code  = 'A'                               */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code  = 'A'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS513_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS513_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS513_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS513_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS514 繳款紀錄在X月內的呆帳平均筆數 4 account_code  = 'B'                               */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code  = 'B'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS514_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS514_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS514_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS514_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS515 繳款紀錄在X月內的財政部優惠貸款平均筆數 4 account_code2 in ('V','W')              */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code2 in ('V','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS515_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS515_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS515_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS515_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS516 繳款紀錄在X月內的不動產貸款平均筆數 4 purpose_code = 1                            */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '1'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS516_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS516_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS516_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS516_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS517 繳款紀錄在X月內的動產貸款平均筆數 4 purpose_code = 2                              */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '2'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS517_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS517_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS517_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS517_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS518 繳款紀錄在X月內的企業投資貸款平均筆數 4 purpose_code = 3                          */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '3'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS518_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS518_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS518_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS518_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS519 繳款紀錄在X月內的週轉金貸款平均筆數 4 purpose_code = 4                            */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '4'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS519_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS519_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS519_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS519_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS520 繳款紀錄在X月內的助學貸款平均筆數 (無擔保) 4 account_code2 is NULL account_code in ('Z') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '4'
      and (account_code2 is NULL or account_code2 = '')
      and account_code = 'Z'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS520_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS520_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS520_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS520_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS521 繳款紀錄在X月內的存保單質押平均筆數 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 =''  or account_code2 is null)
      and account_code = 'K'
      and purpose_code = '4'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS521_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS521_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS521_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS521_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS522 繳款紀錄在X月內的現金卡平均筆數 (無擔保) 4 account_code2 is NULL account_code in ('Y') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('Y')
      and (account_code2 =''  or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS522_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS522_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS522_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS522_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS523 繳款紀錄在X月內的Co-Loan 平均筆數 4 Co-Loan = 'Y'                                 */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where co_loan = 'Y'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS523_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS523_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS523_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS523_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS524 繳款紀錄在X月內的在同一金融機構沒有房貸的購屋用無擔保貸款平均筆數 4             */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_dedup c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS524_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS524_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS524_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS524_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS525 繳款紀錄在X月內的除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款平均筆數 */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)  /*購屋用貸款外的無擔保貸款*/
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, (mon_since - cycle + 1)
 insert into #tmp (idn, mon, v1)  /*有房貸的購屋用無擔保貸款*/
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_dedup c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS525_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS525_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS525_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS525_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS526 繳款紀錄在X月內的有擔保購屋貸款平均筆數 4 purpose_code = 1 account_code2 is not NULL*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
     group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS526_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS526_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS526_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS526_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS527 繳款紀錄在X月內的無擔保非購屋貸款平均筆數 4 purpose_code != 1 account_code2 is NULL*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, avg(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
           and v1 <> 0
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS527_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS527_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS527_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS527_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS528 繳款紀錄在X月內的總放款總筆數 4 # records                                       */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS528_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS528_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS528_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS528_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS529 繳款紀錄在X月內的總放款總銀行數 4 # distinct issuer (3 digit)                   */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v3)
    select idn, (mon_since - cycle + 1), bank_code2
    from #bam085_dedup

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, count(distinct v3)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS529_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS529_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS529_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS529_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS530 繳款紀錄在X月內的還清總筆數 4 loan_amt = 0 and pass_due_amt = 0  account_code <> 'Y'*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where loan_amt = 0
      and pass_due_amt = 0
      and account_code <> 'Y'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS530_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS530_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS530_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS530_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS531 繳款紀錄在X月內的有擔保貸款總筆數 4 account_code2 in ('S','W')                  */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code2 in ('S','W')
       or account_code = 'K'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS531_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS531_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS531_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS531_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS532 繳款紀錄在X月內的有擔保貸款總筆數 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS532_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS532_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS532_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS532_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS533 繳款紀錄在X月內的有擔保貸款總筆數 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('H','S')
      and account_code2 in ('S','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS533_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS533_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS533_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS533_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS534 繳款紀錄在X月內的有擔保貸款總筆數 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('I','T')
      and account_code2 in ('S','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS534_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS534_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS534_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS534_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS535 繳款紀錄在X月內的無擔保貸款總筆數 4 account_code2 is NULL                       */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2=''
           or account_code2 is null)
      and account_code <> 'K'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS535_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS535_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS535_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS535_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS536 繳款紀錄在X月內的無擔保貸款總筆數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS536_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS536_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS536_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS536_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS537 繳款紀錄在X月內的無擔保貸款總筆數 - 中期 4 account_code2 is NULL account_code in ('H','S') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS537_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS537_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS537_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS537_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS538 繳款紀錄在X月內的無擔保貸款總筆數 - 長期 4 account_code2 is NULL account_code in ('I','T')*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('I','T')
      and (account_code2 ='' or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS538_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS538_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS538_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS538_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS539 繳款紀錄在X月內的逾期總筆數 4 pass_due_amt > 0                                  */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where pass_due_amt > 0
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS539_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS539_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS539_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS539_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS540 繳款紀錄在X月內的催收總筆數 4 account_code  = 'A'                               */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code  = 'A'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS540_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS540_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS540_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS540_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS541 繳款紀錄在X月內的呆帳總筆數 4 account_code  = 'B'                               */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code  = 'B'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS541_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS541_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS541_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS541_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS542 繳款紀錄在X月內的財政部優惠貸款總筆數 4 account_code2 in ('V','W')              */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code2 in ('V','W')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS542_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS542_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS542_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS542_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS543 繳款紀錄在X月內的不動產貸款總筆數 4 purpose_code = 1                            */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '1'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS543_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS543_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS543_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS543_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS544 繳款紀錄在X月內的動產貸款總筆數 4 purpose_code = 2                              */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '2'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS544_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS544_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS544_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS544_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS545 繳款紀錄在X月內的企業投資貸款總筆數 4 purpose_code = 3                          */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '3'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS545_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS545_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS545_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS545_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* FS546 繳款紀錄在X月內的週轉金貸款總筆數 4 purpose_code = 4                            */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '4'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS546_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS546_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS546_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS546_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS547 繳款紀錄在X月內的助學貸款總筆數 (無擔保) 4 account_code2 is NULL account_code in ('Z') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where purpose_code = '4'
      and (account_code2 is NULL or account_code2 = '')
      and account_code = 'Z'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS547_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS547_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS547_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS547_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS548 繳款紀錄在X月內的存保單質押總筆數 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 =''  or account_code2 is null)
      and account_code = 'K'
      and purpose_code = '4'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS548_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS548_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS548_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS548_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS549 繳款紀錄在X月內的現金卡總筆數 (無擔保) 4 account_code2 is NULL account_code in ('Y') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('Y')
      and (account_code2 =''  or account_code2 is null)
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS549_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS549_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS549_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS549_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS550 繳款紀錄在X月內的Co-Loan 總筆數 4 Co-Loan = 'Y'                                 */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where co_loan = 'Y'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS550_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS550_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS550_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS550_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12


 /*****************************************************************************************/
 /* FS551 繳款紀錄在X月內的在同一金融機構沒有房貸的購屋用無擔保貸款總筆數 4               */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup b
    where (b.account_code2 = '' or b.account_code2 is null)
      and b.account_code <> 'K'
      and b.purpose_code = 1
      and b.idn not in (select c.idn from #bam085_dedup c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS551_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS551_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS551_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS551_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS552 繳款紀錄在X月內的除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款總筆數 */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)  /*購屋用貸款外的無擔保貸款*/
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, (mon_since - cycle + 1)
 insert into #tmp (idn, mon, v1)  /*有房貸的購屋用無擔保貸款*/
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup b
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code = 1
      and b.idn in (select c.idn from #bam085_dedup c
                        where b.idn = c.idn
                          and left(b.bank_code, 3) = left(c.bank_code, 3)
                          and c.account_code2 in ('S', 'W')
                          and c.purpose_code = '1')
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS552_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS552_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS552_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS552_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS553 繳款紀錄在X月內的有擔保購屋貸款總筆數 4 purpose_code = 1 account_code2 is not NULL*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 <> '' and account_code2 is not null)
      and purpose_code = '1'
     group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS553_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS553_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS553_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS553_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* FS554 繳款紀錄在X月內的無擔保非購屋貸款總筆數 4 purpose_code != 1 account_code2 is NULL*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where (account_code2 = '' or account_code2 is null)
      and account_code <> 'K'
      and purpose_code <> '1'
    group by idn, (mon_since - cycle + 1)

 set @i = 3
 while @i <= 12
   begin
      insert into #tmp1 (idn, mon, v1)
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set FS554_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set FS554_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set FS554_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set FS554_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
/* END OF procedure generate_bam_vars_315_443 */
go
