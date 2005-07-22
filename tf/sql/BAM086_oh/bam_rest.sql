alter procedure generate_bam_vars_rest
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
    set FS444_3M = 0, FS444_6M = 0, FS444_9M = 0, FS444_12M = 0,
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
        MS360_3M = 0, MS360_6M = 0, MS360_9M = 0, MS360_12M = 0,
        MS361_3M = 0, MS361_6M = 0, MS361_9M = 0, MS361_12M = 0,
        MS362_3M = 0, MS362_6M = 0, MS362_9M = 0, MS362_12M = 0,
        MS363_3M = 0, MS363_6M = 0, MS363_9M = 0, MS363_12M = 0,
        MS364_3M = 0, MS364_6M = 0, MS364_9M = 0, MS364_12M = 0,
        MS365_3M = 0, MS365_6M = 0, MS365_9M = 0, MS365_12M = 0,
        MS366_3M = 0, MS366_6M = 0, MS366_9M = 0, MS366_12M = 0,
        MS367_3M = 0, MS367_6M = 0, MS367_9M = 0, MS367_12M = 0,
        MS368_3M = 0, MS368_6M = 0, MS368_9M = 0, MS368_12M = 0,
        MS369_3M = 0, MS369_6M = 0, MS369_9M = 0, MS369_12M = 0,
        MS370_3M = 0, MS370_6M = 0, MS370_9M = 0, MS370_12M = 0,
        MS371_3M = 0, MS371_6M = 0, MS371_9M = 0, MS371_12M = 0,
        MS372_3M = 0, MS372_6M = 0, MS372_9M = 0, MS372_12M = 0,
        MS373_3M = 0, MS373_6M = 0, MS373_9M = 0, MS373_12M = 0,
        MS374_3M = 0, MS374_6M = 0, MS374_9M = 0, MS374_12M = 0,
        MS375_3M = 0, MS375_6M = 0, MS375_9M = 0, MS375_12M = 0,
        MS376_3M = 0, MS376_6M = 0, MS376_9M = 0, MS376_12M = 0,
        MS377_3M = 0, MS377_6M = 0, MS377_9M = 0, MS377_12M = 0,
        MS378_3M = 0, MS378_6M = 0, MS378_9M = 0, MS378_12M = 0,
        MS379_3M = 0, MS379_6M = 0, MS379_9M = 0, MS379_12M = 0,
        MS380_3M = 0, MS380_6M = 0, MS380_9M = 0, MS380_12M = 0,
        MS381_3M = 0, MS381_6M = 0, MS381_9M = 0, MS381_12M = 0,
        MS382_3M = 0, MS382_6M = 0, MS382_9M = 0, MS382_12M = 0,
        MS383_3M = 0, MS383_6M = 0, MS383_9M = 0, MS383_12M = 0,
        MS384_3M = 0, MS384_6M = 0, MS384_9M = 0, MS384_12M = 0,
        MS385_3M = 0, MS385_6M = 0, MS385_9M = 0, MS385_12M = 0,
        MS386_3M = 0, MS386_6M = 0, MS386_9M = 0, MS386_12M = 0,
        MS387_3M = 0, MS387_6M = 0, MS387_9M = 0, MS387_12M = 0,
        MS388_3M = 0, MS388_6M = 0, MS388_9M = 0, MS388_12M = 0,
        MS389_3M = 0, MS389_6M = 0, MS389_9M = 0, MS389_12M = 0,
        MS390_3M = 0, MS390_6M = 0, MS390_9M = 0, MS390_12M = 0,
        MS391_3M = 0, MS391_6M = 0, MS391_9M = 0, MS391_12M = 0,
        MS392_3M = 0, MS392_6M = 0, MS392_9M = 0, MS392_12M = 0,
        MS393_3M = 0, MS393_6M = 0, MS393_9M = 0, MS393_12M = 0,
        MS394_3M = 0, MS394_6M = 0, MS394_9M = 0, MS394_12M = 0,
        MS395_3M = 0, MS395_6M = 0, MS395_9M = 0, MS395_12M = 0,
        MS396_3M = 0, MS396_6M = 0, MS396_9M = 0, MS396_12M = 0,
        MS397_3M = 0, MS397_6M = 0, MS397_9M = 0, MS397_12M = 0,
        MS398_3M = 0, MS398_6M = 0, MS398_9M = 0, MS398_12M = 0,
        MS399_3M = 0, MS399_6M = 0, MS399_9M = 0, MS399_12M = 0,
        MS400_3M = 0, MS400_6M = 0, MS400_9M = 0, MS400_12M = 0,
        MS401_3M = 0, MS401_6M = 0, MS401_9M = 0, MS401_12M = 0,
        MS402_3M = 0, MS402_6M = 0, MS402_9M = 0, MS402_12M = 0,
        MS403_3M = 0, MS403_6M = 0, MS403_9M = 0, MS403_12M = 0,
        MS404_3M = 0, MS404_6M = 0, MS404_9M = 0, MS404_12M = 0
    from #bam085_dedup as a
    where a.idn = #bam085_vars.idn;
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
 /* MS360 繳款紀錄在X月內的授信平均餘額 4 Pass_due_amt = 0 only;    ????                                     */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
    from #bam085_dedup
    where pass_due_amt = 0
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
    set MS360_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS360_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS360_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS360_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS361 繳款紀錄在X月內的訂約平均金額 4                                                                */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS361_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS361_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS361_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS361_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS362 繳款紀錄在X月內的訂約平均金額 4 max of all records                                            */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS362_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS362_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS362_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS362_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS363 繳款紀錄在X月內的還清訂約平均金額 4 loan_amt = 0 and pass_due_amt = 0  account_code <>'Y'      */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
    from #bam085_dedup
    where loan_amt = 0
      and pass_due_amt = 0
      and account_code <>'Y'
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
    set MS363_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS363_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS363_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS363_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS364 繳款紀錄在X月內的有擔保貸款訂約平均金額 4 account_code2 in ('S','W')                             */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS364_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS364_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS364_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS364_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS365 繳款紀錄在X月內的有擔保貸款訂約平均金額 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS365_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS365_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS365_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS365_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS366 繳款紀錄在X月內的有擔保貸款訂約平均金額 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS366_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS366_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS366_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS366_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS367 繳款紀錄在X月內的有擔保貸款訂約平均金額 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS367_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS367_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS367_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS367_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS368 繳款紀錄在X月內的無擔保貸款訂約平均金額 4 account_code2 is NULL                                  */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS368_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS368_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS368_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS368_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS369 繳款紀錄在X月內的無擔保貸款訂約平均金額 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS369_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS369_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS369_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS369_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS370 繳款紀錄在X月內的無擔保貸款訂約平均金額 - 中期 4 account_code2 is NULL account_code in ('H','S') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS370_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS370_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS370_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS370_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS371 繳款紀錄在X月內的無擔保貸款訂約平均金額 - 長期 4 account_code2 is NULL account_code in ('I','T') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS371_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS371_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS371_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS371_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS372 繳款紀錄在X月內的逾期訂約平均金額 4 pass_due_amt > 0              ????                               */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS372_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS372_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS372_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS372_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS373 繳款紀錄在X月內的催收訂約平均金額 4 account_code  = 'A'                                          */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS373_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS373_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS373_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS373_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS374 繳款紀錄在X月內的呆帳訂約平均金額 4 account_code  = 'B'                                          */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS374_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS374_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS374_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS374_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS375 繳款紀錄在X月內的財政部優惠貸款訂約平均金額 4 account_code2 in ('V','W')                         */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS375_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS375_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS375_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS375_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS376 繳款紀錄在X月內的不動產貸款訂約平均金額 4 purpose_code = 1                                       */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS376_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS376_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS376_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS376_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS377 繳款紀錄在X月內的動產貸款訂約平均金額 4 purpose_code = 2                                         */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS377_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS377_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS377_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS377_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS378 繳款紀錄在X月內的企業投資貸款訂約平均金額 4 purpose_code = 3                                     */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS378_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS378_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS378_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS378_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS379 繳款紀錄在X月內的週轉金貸款訂約平均金額 4 purpose_code = 4                                       */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS379_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS379_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS379_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS379_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS380 繳款紀錄在X月內的助學貸款訂約平均金額 (無擔保) 4 account_code2 is NULL account_code in ('Z') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
    set MS380_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS380_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS380_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS380_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS381 繳款紀錄在X月內的存保單質押訂約平均金額 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS381_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS381_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS381_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS381_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS382 繳款紀錄在X月內的現金卡訂約平均金額 (無擔保) 4 account_code2 is NULL account_code in ('Y')       */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS382_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS382_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS382_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS382_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS383 繳款紀錄在X月內的Co-Loan 訂約平均金額 4 Co-Loan = 'Y'                                            */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(contract_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS383_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS383_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS383_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS383_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12


 /*****************************************************************************************/
 /* MS384 繳款紀錄在X月內的Max 貸放平均餘額 4 max of all records                                           */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS384_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS384_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS384_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS384_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS385 繳款紀錄在X月內的有擔保貸款平均餘額 4 account_code2 in ('S','W')                                 */  
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS385_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS385_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS385_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS385_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS386 繳款紀錄在X月內的有擔保貸款平均餘額 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')*/ 
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS386_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS386_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS386_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS386_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS387 繳款紀錄在X月內的有擔保貸款平均餘額 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS387_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS387_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS387_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS387_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS388 繳款紀錄在X月內的有擔保貸款平均餘額 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS388_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS388_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS388_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS388_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS389 繳款紀錄在X月內的無擔保貸款平均餘額 4 account_code2 is NULL                                      */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS389_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS389_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS389_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS389_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS390 繳款紀錄在X月內的無擔保貸款平均餘額 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E')*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS390_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS390_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS390_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS390_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS391 繳款紀錄在X月內的無擔保貸款平均餘額 - 中期 4 account_code2 is NULL account_code in ('H','S')     */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS391_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS391_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS391_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS391_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS392 繳款紀錄在X月內的無擔保貸款平均餘額 - 長期 4 account_code2 is NULL account_code in ('I','T')     */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS392_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS392_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS392_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS392_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS393 繳款紀錄在X月內的逾期平均餘額 4 pass_due_amt > 0                                                 */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS393_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS393_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS393_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS393_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS394 繳款紀錄在X月內的催收平均餘額 4 account_code  = 'A'                                              */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS394_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS394_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS394_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS394_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS395 繳款紀錄在X月內的呆帳平均餘額 4 account_code  = 'B'                                              */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS395_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS395_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS395_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS395_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS396 繳款紀錄在X月內的財政部優惠貸款平均餘額 4 account_code2 in ('V','W')                             */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS396_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS396_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS396_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS396_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS397 繳款紀錄在X月內的不動產貸款平均餘額 4 purpose_code = 1                                           */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS397_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS397_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS397_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS397_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS398 繳款紀錄在X月內的動產貸款平均餘額 4 purpose_code = 2                                             */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS398_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS398_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS398_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS398_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS399 繳款紀錄在X月內的企業投資貸款平均餘額 4 purpose_code = 3                                         */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS399_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS399_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS399_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS399_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS400 繳款紀錄在X月內的週轉金貸款平均餘額 4 purpose_code = 4                                           */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS400_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS400_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS400_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS400_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS401 繳款紀錄在X月內的助學貸款平均餘額 (無擔保) 4 account_code2 is NULL account_code in ('Z') purpose_code = 4 */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
    from #bam085_dedup
    where purpose_code = '4'
      and (account_code2 is NULL or account_code2 = '')
      and account_code ='Z'
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
    set MS401_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS401_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS401_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS401_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS402 繳款紀錄在X月內的存保單質押平均餘額 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4*/
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS402_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS402_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS402_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS402_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS403 繳款紀錄在X月內的現金卡平均餘額 (無擔保) 4 account_code2 is NULL account_code in ('Y')           */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS403_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS403_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS403_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS403_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS404 繳款紀錄在X月內的Co-Loan 平均餘額 4 Co-Loan = 'Y'                                                */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, (mon_since - cycle + 1), sum(loan_amt + pass_due_amt)
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
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS404_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS404_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS404_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS404_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12


