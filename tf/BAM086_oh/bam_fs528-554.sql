alter procedure generate_bam_fs528_554
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
    v3 varchar(20));
 create table #tmp1 (
    idn varchar(14),
    mon int,
    v1 decimal (16, 8));
 create index i_tmp on #tmp(idn, mon);

 /*****************************************************************************************/
 /*   Initialize #bam085_vars table                                                       */
 /*****************************************************************************************/
 update #bam085_vars
    set FS528_3M = 0, FS528_6M = 0, FS528_9M = 0, FS528_12M = 0,
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

 /*First accumulate cured flag by month, then find sum, max, avg within a season          */
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
