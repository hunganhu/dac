create procedure generate_bam_ms
 (@now int)
 as

 declare @i int
 insert into #bam085_dedup (idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, cnt)
    select idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, count(*)
    from bam085_curr
    group by idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan;
 update #bam085_dedup
    set bank_code2 = left(bank_code,3),
        mon_since = data_yyy * 12 + data_mm,
        cycle = len(pay_code_12);

 insert into #bam085_vars (idn)
    select distinct idn
    from #bam085_dedup

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
    set MS315_3M = 0, MS315_6M = 0, MS315_9M = 0, MS315_12M = 0,
        MS316_3M = 0, MS316_6M = 0, MS316_9M = 0, MS316_12M = 0,
        MS317_3M = 0, MS317_6M = 0, MS317_9M = 0, MS317_12M = 0,
        MS318_3M = 0, MS318_6M = 0, MS318_9M = 0, MS318_12M = 0,
        MS319_3M = 0, MS319_6M = 0, MS319_9M = 0, MS319_12M = 0,
        MS320_3M = 0, MS320_6M = 0, MS320_9M = 0, MS320_12M = 0,
        MS321_3M = 0, MS321_6M = 0, MS321_9M = 0, MS321_12M = 0,
        MS322_3M = 0, MS322_6M = 0, MS322_9M = 0, MS322_12M = 0,
        MS323_3M = 0, MS323_6M = 0, MS323_9M = 0, MS323_12M = 0,
        MS324_3M = 0, MS324_6M = 0, MS324_9M = 0, MS324_12M = 0,
        MS325_3M = 0, MS325_6M = 0, MS325_9M = 0, MS325_12M = 0,
        MS326_3M = 0, MS326_6M = 0, MS326_9M = 0, MS326_12M = 0,
        MS327_3M = 0, MS327_6M = 0, MS327_9M = 0, MS327_12M = 0,
        MS328_3M = 0, MS328_6M = 0, MS328_9M = 0, MS328_12M = 0,
        MS329_3M = 0, MS329_6M = 0, MS329_9M = 0, MS329_12M = 0,
        MS330_3M = 0, MS330_6M = 0, MS330_9M = 0, MS330_12M = 0,
        MS331_3M = 0, MS331_6M = 0, MS331_9M = 0, MS331_12M = 0,
        MS332_3M = 0, MS332_6M = 0, MS332_9M = 0, MS332_12M = 0,
        MS333_3M = 0, MS333_6M = 0, MS333_9M = 0, MS333_12M = 0,
        MS334_3M = 0, MS334_6M = 0, MS334_9M = 0, MS334_12M = 0,
        MS335_3M = 0, MS335_6M = 0, MS335_9M = 0, MS335_12M = 0,
        MS336_3M = 0, MS336_6M = 0, MS336_9M = 0, MS336_12M = 0,
        MS337_3M = 0, MS337_6M = 0, MS337_9M = 0, MS337_12M = 0,
        MS338_3M = 0, MS338_6M = 0, MS338_9M = 0, MS338_12M = 0,
        MS339_3M = 0, MS339_6M = 0, MS339_9M = 0, MS339_12M = 0,
        MS340_3M = 0, MS340_6M = 0, MS340_9M = 0, MS340_12M = 0,
        MS341_3M = 0, MS341_6M = 0, MS341_9M = 0, MS341_12M = 0,
        MS342_3M = 0, MS342_6M = 0, MS342_9M = 0, MS342_12M = 0,
        MS343_3M = 0, MS343_6M = 0, MS343_9M = 0, MS343_12M = 0,
        MS344_3M = 0, MS344_6M = 0, MS344_9M = 0, MS344_12M = 0,
        MS345_3M = 0, MS345_6M = 0, MS345_9M = 0, MS345_12M = 0,
        MS346_3M = 0, MS346_6M = 0, MS346_9M = 0, MS346_12M = 0,
        MS347_3M = 0, MS347_6M = 0, MS347_9M = 0, MS347_12M = 0,
        MS348_3M = 0, MS348_6M = 0, MS348_9M = 0, MS348_12M = 0,
        MS349_3M = 0, MS349_6M = 0, MS349_9M = 0, MS349_12M = 0,
        MS350_3M = 0, MS350_6M = 0, MS350_9M = 0, MS350_12M = 0,
        MS351_3M = 0, MS351_6M = 0, MS351_9M = 0, MS351_12M = 0,
        MS352_3M = 0, MS352_6M = 0, MS352_9M = 0, MS352_12M = 0,
        MS353_3M = 0, MS353_6M = 0, MS353_9M = 0, MS353_12M = 0,
        MS354_3M = 0, MS354_6M = 0, MS354_9M = 0, MS354_12M = 0,
        MS355_3M = 0, MS355_6M = 0, MS355_9M = 0, MS355_12M = 0,
        MS356_3M = 0, MS356_6M = 0, MS356_9M = 0, MS356_12M = 0,
        MS357_3M = 0, MS357_6M = 0, MS357_9M = 0, MS357_12M = 0,
        MS358_3M = 0, MS358_6M = 0, MS358_9M = 0, MS358_12M = 0,
        MS359_3M = 0, MS359_6M = 0, MS359_9M = 0, MS359_12M = 0,
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

 /*First accumulate cured flag by month, then find max, avg within a season               */
 /*****************************************************************************************/
 /* MS315 繳款紀錄在X月內的授信總最高餘額 4 Pass_due_amt = 0 only;    ????                                     */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS315_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS315_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS315_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS315_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS316 繳款紀錄在X月內的訂約總最高金額 4                                                                */
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
         select idn, @i, sum(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS316_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS316_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS316_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS316_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS317 繳款紀錄在X月內的訂約最高金額 4 max of all records                                            */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS317_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS317_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS317_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS317_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS318 繳款紀錄在X月內的還清總訂約最高金額 4 loan_amt = 0 and pass_due_amt = 0  account_code <>'Y'      */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS318_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS318_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS318_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS318_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS319 繳款紀錄在X月內的有擔保貸款訂約最高金額 4 account_code2 in ('S','W')                             */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS319_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS319_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS319_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS319_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS320 繳款紀錄在X月內的有擔保貸款訂約最高金額 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')*/
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS320_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS320_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS320_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS320_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS321 繳款紀錄在X月內的有擔保貸款訂約最高金額 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')*/
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS321_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS321_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS321_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS321_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS322 繳款紀錄在X月內的有擔保貸款訂約最高金額 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')*/
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS322_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS322_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS322_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS322_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS323 繳款紀錄在X月內的無擔保貸款訂約最高金額 4 account_code2 is NULL                                  */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS323_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS323_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS323_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS323_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS324 繳款紀錄在X月內的無擔保貸款訂約最高金額 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E') */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS324_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS324_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS324_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS324_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS325 繳款紀錄在X月內的無擔保貸款訂約最高金額 - 中期 4 account_code2 is NULL account_code in ('H','S') */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS325_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS325_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS325_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS325_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS326 繳款紀錄在X月內的無擔保貸款訂約最高金額 - 長期 4 account_code2 is NULL account_code in ('I','T') */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS326_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS326_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS326_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS326_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS327 繳款紀錄在X月內的逾期訂約最高金額 4 pass_due_amt > 0              ????                               */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS327_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS327_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS327_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS327_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS328 繳款紀錄在X月內的催收訂約最高金額 4 account_code  = 'A'                                          */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS328_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS328_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS328_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS328_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS329 繳款紀錄在X月內的呆帳訂約最高金額 4 account_code  = 'B'                                          */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS329_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS329_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS329_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS329_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS330 繳款紀錄在X月內的財政部優惠貸款訂約最高金額 4 account_code2 in ('V','W')                         */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS330_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS330_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS330_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS330_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS331 繳款紀錄在X月內的不動產貸款訂約最高金額 4 purpose_code = 1                                       */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS331_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS331_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS331_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS331_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS332 繳款紀錄在X月內的動產貸款訂約最高金額 4 purpose_code = 2                                         */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS332_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS332_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS332_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS332_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS333 繳款紀錄在X月內的企業投資貸款訂約最高金額 4 purpose_code = 3                                     */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS333_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS333_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS333_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS333_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS334 繳款紀錄在X月內的週轉金貸款訂約最高金額 4 purpose_code = 4                                       */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS334_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS334_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS334_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS334_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS335 繳款紀錄在X月內的助學貸款訂約最高金額 (無擔保) 4 account_code2 is NULL account_code in ('Z') purpose_code = 4*/
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS335_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS335_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS335_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS335_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS336 繳款紀錄在X月內的存保單質押訂約最高金額 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4*/
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS336_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS336_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS336_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS336_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS337 繳款紀錄在X月內的現金卡訂約最高金額 (無擔保) 4 account_code2 is NULL account_code in ('Y')       */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS337_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS337_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS337_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS337_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS338 繳款紀錄在X月內的Co-Loan 訂約最高金額 4 Co-Loan = 'Y'                                            */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS338_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS338_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS338_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS338_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12


 /*****************************************************************************************/
 /* MS339 繳款紀錄在X月內的Max 貸放最高餘額 4 max of all records                                           */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS339_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS339_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS339_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS339_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS340 繳款紀錄在X月內的有擔保貸款最高餘額 4 account_code2 in ('S','W')                                 */  
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS340_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS340_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS340_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS340_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS341 繳款紀錄在X月內的有擔保貸款最高餘額 - 短期 4 account_code2 in ('S','W')  account_code in ('W','C','D','E')*/ 
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS341_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS341_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS341_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS341_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS342 繳款紀錄在X月內的有擔保貸款最高餘額 - 中期 4 account_code2 in ('S','W')  account_code in ('H','S')*/
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS342_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS342_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS342_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS342_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS343 繳款紀錄在X月內的有擔保貸款最高餘額 - 長期 4 account_code2 in ('S','W')  account_code in ('I','T')*/
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS343_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS343_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS343_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS343_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS344 繳款紀錄在X月內的無擔保貸款最高餘額 4 account_code2 is NULL                                      */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS344_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS344_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS344_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS344_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS345 繳款紀錄在X月內的無擔保貸款最高餘額 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E')*/
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS345_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS345_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS345_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS345_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS346 繳款紀錄在X月內的無擔保貸款最高餘額 - 中期 4 account_code2 is NULL account_code in ('H','S')     */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS346_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS346_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS346_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS346_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS347 繳款紀錄在X月內的無擔保貸款最高餘額 - 長期 4 account_code2 is NULL account_code in ('I','T')     */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS347_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS347_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS347_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS347_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS348 繳款紀錄在X月內的逾期最高餘額 4 pass_due_amt > 0                                                 */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS348_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS348_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS348_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS348_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS349 繳款紀錄在X月內的催收最高餘額 4 account_code  = 'A'                                              */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS349_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS349_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS349_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS349_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS350 繳款紀錄在X月內的呆帳最高餘額 4 account_code  = 'B'                                              */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS350_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS350_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS350_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS350_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS351 繳款紀錄在X月內的財政部優惠貸款最高餘額 4 account_code2 in ('V','W')                             */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS351_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS351_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS351_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS351_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS352 繳款紀錄在X月內的不動產貸款最高餘額 4 purpose_code = 1                                           */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS352_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS352_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS352_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS352_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS353 繳款紀錄在X月內的動產貸款最高餘額 4 purpose_code = 2                                             */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS353_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS353_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS353_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS353_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS354 繳款紀錄在X月內的企業投資貸款最高餘額 4 purpose_code = 3                                         */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS354_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS354_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS354_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS354_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12

 /*****************************************************************************************/
 /* MS355 繳款紀錄在X月內的週轉金貸款最高餘額 4 purpose_code = 4                                           */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS355_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS355_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS355_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS355_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS356 繳款紀錄在X月內的助學貸款最高餘額 (無擔保) 4 account_code2 is NULL account_code in ('Z') purpose_code = 4 */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS356_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS356_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS356_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS356_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS357 繳款紀錄在X月內的存保單質押最高餘額 (無擔保) 4 account_code2 is NULL account_code in ('K') purpose_code = 4*/
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS357_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS357_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS357_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS357_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS358 繳款紀錄在X月內的現金卡最高餘額 (無擔保) 4 account_code2 is NULL account_code in ('Y')           */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS358_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS358_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS358_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS358_12M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 12
 /*****************************************************************************************/
 /* MS359 繳款紀錄在X月內的Co-Loan 最高餘額 4 Co-Loan = 'Y'                                                */
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
         select idn, @i, max(v1)
         from #tmp
         where mon < @now
           and mon >= @now - @i + (@i / 12)
         group by idn
      set @i = @i + 3
   end

 update #bam085_vars
    set MS359_3M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 3
 update #bam085_vars
    set MS359_6M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 6
 update #bam085_vars
    set MS359_9M = v1
    from #tmp1 as a
    where a.idn = #bam085_vars.idn
      and a.mon = 9
 update #bam085_vars
    set MS359_12M = v1
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


