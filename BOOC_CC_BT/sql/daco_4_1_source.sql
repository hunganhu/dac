use oliver
go

 CREATE PROCEDURE prepare_jcic_data
    (@case_sn varchar(20),
     @idn varchar(14),
     @dac_sn int,
     @now int)
 AS

 insert into #krm021_dedup (case_sn, idn, dac_sn, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, cnt)
    select case_sn, idn, dac_sn, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, count(*)
    from krm021
    where case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn
    group by case_sn, idn, dac_sn, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit;

 insert into #krm023_dedup (case_sn, idn, dac_sn, issue, issue_name, limit, yrmon, kr_code, payment, cash, pay_code, cnt)
    select case_sn, idn, dac_sn, issue, issue_name, limit, yrmon, kr_code, payment, cash, pay_code, count(*)
    from krm023
    where case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn
    group by case_sn, idn, dac_sn, issue, issue_name, limit, yrmon, kr_code, payment, cash, pay_code;

 insert into #stm001_dedup (case_sn, idn, dac_sn, bank_code, bank_name, query_date, item_list, cnt)
    select case_sn, idn, dac_sn, bank_code, bank_name, query_date, item_list, count(*)
    from stm001
    where case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn
    group by case_sn, idn, dac_sn, bank_code, bank_name, query_date, item_list;

 insert into #bam085_dedup (case_sn, idn, dac_sn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, cnt)
    select case_sn, idn, dac_sn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, count(*)
    from bam085
    where case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn
    group by case_sn, idn, dac_sn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan;
/*
 insert into #jas002_t (case_sn, idn, dac_sn, reason, date_happen)
    select case_sn, idn, dac_sn, 'D', data_yyymmdd
    from bas001
    where data_yyymmdd is not null
      and case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn;
 insert into #jas002_t (case_sn, idn, dac_sn, reason, date_happen)
    select case_sn, idn, dac_sn, 'B', ck_date
    from dam103
    where ck_date is not null
      and case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn;
 insert into #jas002_t (case_sn, idn, dac_sn, reason, date_happen)
    select case_sn, idn, dac_sn, 'R', beg_date
    from dam203
    where beg_date is not null
      and case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn;
 insert into #jas002_t (case_sn, idn, dac_sn, reason, date_happen)
    select case_sn, idn, dac_sn, 'S', stop_date
    from krm021
    where stop_code = '3'
      and case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn;
 insert into #jas002_t_dedup (case_sn, idn, dac_sn, reason, date_happen, mon_since, cnt)
    select case_sn, idn, dac_sn, reason, date_happen, mon_since, count(*)
    from #jas002_t
    group by case_sn, idn, dac_sn, reason, date_happen, mon_since;
*/

 update #krm021_dedup set card_brand = (case when card_brand = '' then NULL else card_brand end),
                   card_type  = (case when card_type  = '' then NULL else card_type  end),
                   issue      = (case when issue      = '' then NULL else issue      end),
                   issue_name = (case when issue_name = '' then NULL else issue_name end),
                   start_date = (case when start_date = '' then NULL else start_date end),
                   stop_date  = (case when stop_date  = '' then NULL else stop_date  end),
                   stop_code  = (case when stop_code  = '' then NULL else stop_code  end),
                   ab_code    = (case when ab_code    = '' then NULL else ab_code    end),
                   m_s        = (case when m_s        = '' then NULL else m_s        end),
                   limit      = (case when limit      = '' then NULL else limit      end)
 update #krm023_dedup set issue      = (case when issue      = '' then NULL else issue      end),
                   issue_name = (case when issue_name = '' then NULL else issue_name end),
                   limit      = (case when limit      = '' then NULL else limit      end),
                   yrmon      = (case when yrmon      = '' then NULL else yrmon      end),
                   kr_code    = (case when kr_code    = '' then NULL else kr_code    end),
                   payment    = (case when payment    = '' then NULL else payment    end),
                   cash       = (case when cash       = '' then NULL else cash       end),
                   pay_code   = (case when pay_code   = '' then NULL else pay_code   end)
 update #stm001_dedup set bank_code  = (case when bank_code  = '' then NULL else bank_code  end),
                   bank_name  = (case when bank_name  = '' then NULL else bank_name  end),
                   query_date = (case when query_date = '' then NULL else query_date end),
                   item_list  = (case when item_list  = '' then NULL else item_list  end)
 update #bam085_dedup set data_yyy      = (case when data_yyy      = '' then NULL else data_yyy      end),
                   data_mm       = (case when data_mm       = '' then NULL else data_mm       end),
                   bank_code     = (case when bank_code     = '' then NULL else bank_code     end),
                   bank_name     = (case when bank_name     = '' then NULL else bank_name     end),
                   account_code  = (case when account_code  = '' then NULL else account_code  end),
                   account_code2 = (case when account_code2 = '' then NULL else account_code2 end),
                   purpose_code  = (case when purpose_code  = '' then NULL else purpose_code  end),
                   pay_code_12   = (case when pay_code_12   = '' then NULL else pay_code_12   end),
                   co_loan       = (case when co_loan       = '' then NULL else co_loan       end)

 update #krm021_dedup
    set start_date = (case when left(start_date,1) not in ('1', '0', '*') then '0' + (start_date)
 		           else start_date
 		      end),
        stop_date =  (case when left(stop_date,1) not in ('1', '0', '*') then '0' + (stop_date)
  		           else stop_date
 	 	      end);
 update #krm021_dedup
    set start_mon_since = (case	when left(start_date,1) = '*' then null
 			        else left(start_date, 3) * 12 + right(left(start_date, 5), 2)
 		           end),
        end_mon_since = (case when left(stop_date, 1) = '*' then null
 			      else left(stop_date, 3) * 12 + right(left(stop_date, 5), 2)
 		         end);
 update #krm021_dedup
    set end_mon_since = 12000   /*999 * 12 + 12*/
    where end_mon_since is null;
/*
 update #jas002_t_dedup
    set date_happen = (case when left(ltrim(date_happen),1) not in ('1', '0', '*') then '0' + (ltrim(date_happen))
  		            else date_happen
  		       end);
 update #jas002_t_dedup
    set mon_since = (case when left(ltrim(date_happen),1) = '*' then null
    		          else left(ltrim(date_happen), 3) * 12 + right(left(rtrim(date_happen), 5), 2)
  		     end);
*/
 update #krm023_dedup
    set yrmon = (case when left(yrmon,1) not in ('1', '0', '*') then '0' + (yrmon)
 	              else yrmon
 	         end);
 update #krm023_dedup
    set mon_since = (case when left(yrmon,1) = '*' then null
 		          else left(yrmon, 3) * 12 + right(left(yrmon, 5), 2)
 		     end);
 update #stm001_dedup
    set query_date = (case when left(query_date,1) not in ('1', '0', '*') then '0' + (query_date)
 		           else query_date
 		      end);
 update #stm001_dedup
    set query_mon_since = (case	when left(query_date,1) = '*' then null
 			        else left(query_date, 3) * 12 + right(left(query_date, 5), 2)
 		           end);
 update #krm023_dedup
    set payment = (case when right(left(payment,2),1) in ('H', 'M', 'L') then '0' + (left(payment,2))
                        else payment end);
 update #krm023_dedup
    set payment_amt = (case right(payment,1) when 'L' then 2
                                             when 'M' then 5
                                             when 'H' then 8
                                             else 0 end)
                      * power(10, isnull(left(payment,2),0)-1) / 1000.0;
 update #krm023_dedup
    set bucket_def_1k = (case when pay_code in ('D', 'E', 'F') and payment_amt > 1 then 1
                              else 0 end),
        bucket_ef_1k =  (case when pay_code in ('E', 'F') and payment_amt > 1 then 1
                              else 0 end),
        bucket_f_1k =   (case when pay_code = 'F' and payment_amt > 1 then 1
                              else 0 end);
 declare @i int
 set @i=12
 while @i > 0
    begin
       update #krm023_dedup
          set bucket_def_1k = (case when #krm023_dedup.pay_code in ('D', 'E', 'F') and
                                         #krm023_dedup.payment_amt > 1 then a.bucket_def_1k + 1
                                    else 0 end),
              bucket_ef_1k =  (case when #krm023_dedup.pay_code in ('E', 'F') and
                                         #krm023_dedup.payment_amt > 1 then a.bucket_ef_1k + 1
                                    else 0 end),
              bucket_f_1k =   (case when #krm023_dedup.pay_code = 'F' and
                                         #krm023_dedup.payment_amt > 1 then a.bucket_f_1k + 1
                                    else 0 end)
          from #krm023_dedup as a inner join #krm023_dedup
            on a.idn =  #krm023_dedup.idn and
               a.issue =  #krm023_dedup.issue and
               a.mon_since = ( #krm023_dedup.mon_since - 1)
          where (@now - a.mon_since) = @i
       set @i = @i - 1
    end;
 update #bam085_dedup
    set bank_code2 = left(bank_code,3);
 /* risk filter process */
 create table #base_tmp (
    idn varchar (14),
    avail_flag int
 );
 insert into #base_tmp(idn, avail_flag)
    values (@idn, 0);
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 1
    from #krm021_dedup;
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 2
    from #krm023_dedup;
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 4
    from #bam085_dedup;
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 8
    from #stm001_dedup;
/*
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 16
    from bas001
    where case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn;
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 32
    from dam103
    where case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn;
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 64
    from dam203
    where case_sn = @case_sn and idn = @idn and dac_sn = @dac_sn;
*/
 insert into #base (idn, avail_flag)
    select idn, sum(avail_flag)
    from #base_tmp
    group by idn;
 /* Update filter variable*/
 create table #tmp (
    idn varchar(14),
    mon int,
    v1 decimal (16, 8),
    v2 decimal (16, 8),
    v3 decimal (16, 8));
/*
--drop table #major_defect
 select idn, count(*) as cnt
    into #major_defect
 from #jas002_t_dedup
 where @now - mon_since <= 36
 group by idn;
 update #base
    set jas002_defect = 1
    from #major_defect as a
    where a.idn = #base.idn
      and a.cnt > 0;
*/
 update #base
    set krm023_hit = 1
    where avail_flag & 0x02 = 0x02;
 update #base
    set krm021_hit = 1
    where avail_flag & 0x01 = 0x01;
 insert into #tmp(idn, v1)
    select idn, max(bucket_ef_1k)
    from #krm023_dedup
    group by idn;
 update #base
    set max_bucket = v1
    from #tmp as a
    where a.idn = #base.idn
 /*----FS044----*/
 delete from #tmp;
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where pass_due_amt > 0
    group by idn;
 update #base
    set fs044 = v1
    from #tmp as a
    where a.idn = #base.idn;
 /*----cash card max bucket----*/
 delete from #tmp;
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code = 'Y'
      and ISNULL(left(pay_code_12, 1),'0') not in ('0', 'X')
    group by idn;
 update #base
    set cash_max_bucket = v1
    from #tmp as a
    where a.idn = #base.idn;
 /*----cash card utilization----*/
 delete from #tmp;
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code = 'Y'
      and ISNULL(loan_amt,0) / contract_amt >= 1
      and contract_amt > 0
    group by idn;
 update #base
    set cash_utilization = v1
    from #tmp as a
    where a.idn = #base.idn;
 /*----Ind001----*/
 delete from #tmp;
 insert into #tmp (idn, mon)
    select idn, min(start_mon_since)
    from #krm021_dedup
    group by idn;
 update #base
    set ind001 = 1
    from #tmp as a
    where a.idn = #base.idn
      and (@now - a.mon) between 1 and 7;
 drop table #base_tmp;
 drop table #tmp;

 insert into #daco_v41_cal(case_sn, idn, dac_sn, now)
    values (@case_sn, @idn, @dac_sn, @now)
 update #daco_v41_cal
    set inquiry_date = a.inquiry_date,
        sex = a.sex,
        guarantor = a.guarantor
    from app_info a
    where a.case_sn = #daco_v41_cal.case_sn
      and a.idn = #daco_v41_cal.idn
      and a.dac_sn = #daco_v41_cal.dac_sn;
/*
 update #daco_v41_cal
    set jas002_defect = a.jas002_defect,
        avail_flag = a.avail_flag,
        krm021_hit = a.krm021_hit,
        krm023_hit = a.krm023_hit,
        max_bucket = a.max_bucket,
        fs044 = a.fs044,
        cash_max_bucket = a.cash_max_bucket,
        cash_utilization = a.cash_utilization,
        ind001 = a.ind001
    from #base a
    where a.idn = #daco_v41_cal.idn;
*/
/* -- end of procedure prepare_jcic_data --*/
go

/* batch process - prepare jcic data */
if exists (select * from dbo.sysobjects where id = object_id(N'[prepare_jcic_data_all]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure prepare_jcic_data_all;
go

CREATE PROCEDURE prepare_jcic_data_all
 (@now int)
 AS
 insert into #krm021_dedup (case_sn, idn, dac_sn, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, cnt)
    select case_sn, idn, dac_sn, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, count(*)
    from krm021
    group by case_sn, idn, dac_sn, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit;

 insert into #krm023_dedup (case_sn, idn, dac_sn, issue, issue_name, limit, yrmon, kr_code, payment, cash, pay_code, cnt)
    select case_sn, idn, dac_sn, issue, issue_name, limit, yrmon, kr_code, payment, cash, pay_code, count(*)
    from krm023
    group by case_sn, idn, dac_sn, issue, issue_name, limit, yrmon, kr_code, payment, cash, pay_code;

 insert into #stm001_dedup (case_sn, idn, dac_sn, bank_code, bank_name, query_date, item_list, cnt)
    select case_sn, idn, dac_sn, bank_code, bank_name, query_date, item_list, count(*)
    from stm001
    group by case_sn, idn, dac_sn, bank_code, bank_name, query_date, item_list;

 insert into #bam085_dedup (case_sn, idn, dac_sn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, cnt)
    select case_sn, idn, dac_sn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, count(*)
    from bam085
    group by case_sn, idn, dac_sn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan;


create index i_krm001 on krm001_dedup(idn);
create index i_krm023 on krm023_dedup(idn);
create index i_bam085 on bam085_dedup(idn);
create index i_jas002t on jas002_t_dedup(idn);
create index i_stm001 on stm001_dedup(idn);

 update #krm023_dedup
    set yrmon = (case when left(yrmon,1) not in ('1', '0', '*') then '0' + (yrmon)
 	              else yrmon
 	         end);
 update #krm023_dedup
    set mon_since = (case when left(yrmon,1) = '*' then null
 		          else left(yrmon, 3) * 12 + right(left(yrmon, 5), 2)
 		     end);
 update #stm001_dedup
    set query_date = (case when left(query_date,1) not in ('1', '0', '*') then '0' + (query_date)
 		           else query_date
 		      end);
 update #stm001_dedup
    set query_mon_since = (case	when left(query_date,1) = '*' then null
 			        else left(query_date, 3) * 12 + right(left(query_date, 5), 2)
 		           end);
 update #krm023_dedup
    set payment = (case when right(left(payment,2),1) in ('H', 'M', 'L') then '0' + (left(payment,2))
                        else payment end);
 update #krm023_dedup
    set payment_amt = (case right(payment,1) when 'L' then 2
                                             when 'M' then 5
                                             when 'H' then 8
                                             else 0 end)
                      * power(10, isnull(left(payment,2),0)-1) / 1000.0;
 update #krm023_dedup
    set bucket_def_1k = (case when pay_code in ('D', 'E', 'F') and payment_amt > 1 then 1
                              else 0 end),
        bucket_ef_1k =  (case when pay_code in ('E', 'F') and payment_amt > 1 then 1
                              else 0 end),
        bucket_f_1k =   (case when pay_code = 'F' and payment_amt > 1 then 1
                              else 0 end);
 declare @i int
 set @i=12
 while @i > 0
    begin
       update #krm023_dedup
          set bucket_def_1k = (case when #krm023_dedup.pay_code in ('D', 'E', 'F') and
                                         #krm023_dedup.payment_amt > 1 then a.bucket_def_1k + 1
                                    else 0 end),
              bucket_ef_1k =  (case when #krm023_dedup.pay_code in ('E', 'F') and
                                         #krm023_dedup.payment_amt > 1 then a.bucket_ef_1k + 1
                                    else 0 end),
              bucket_f_1k =   (case when #krm023_dedup.pay_code = 'F' and
                                         #krm023_dedup.payment_amt > 1 then a.bucket_f_1k + 1
                                    else 0 end)
          from #krm023_dedup as a inner join #krm023_dedup
            on a.idn =  #krm023_dedup.idn and
               a.issue =  #krm023_dedup.issue and
               a.mon_since = ( #krm023_dedup.mon_since - 1)
          where (@now - a.mon_since) = @i
       set @i = @i - 1
    end;
 update #bam085_dedup
    set bank_code2 = left(bank_code,3);

 /* risk filter process */
 create table #base_tmp (
    idn varchar (14),
    avail_flag int
 );
 insert into #base_tmp(idn, avail_flag)
    select idn, 0
    from app_info;
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 1
    from #krm021_dedup;
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 2
    from #krm023_dedup;
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 4
    from #bam085_dedup;
 insert into #base_tmp(idn, avail_flag)
    select distinct idn, 8
    from #stm001_dedup;
 insert into #base (idn, avail_flag)
    select idn, sum(avail_flag)
    from #base_tmp
    group by idn;
 /* Update filter variable*/
 create table #tmp (
    idn varchar(14),
    mon int,
    v1 decimal (16, 8),
    v2 decimal (16, 8),
    v3 decimal (16, 8));
 update #base
    set krm023_hit = 1
    where avail_flag & 0x02 = 0x02;
 update #base
    set krm021_hit = 1
    where avail_flag & 0x01 = 0x01;
 insert into #tmp(idn, v1)
    select idn, max(bucket_ef_1k)
    from #krm023_dedup
    group by idn;
 update #base
    set max_bucket = v1
    from #tmp as a
    where a.idn = #base.idn
 /*----FS044----*/
 delete from #tmp;
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where pass_due_amt > 0
    group by idn;
 update #base
    set fs044 = v1
    from #tmp as a
    where a.idn = #base.idn;
 /*----cash card max bucket----*/
 delete from #tmp;
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code = 'Y'
      and ISNULL(left(pay_code_12, 1),'0') not in ('0', 'X')
    group by idn;
 update #base
    set cash_max_bucket = v1
    from #tmp as a
    where a.idn = #base.idn;
 /*----cash card utilization----*/
 delete from #tmp;
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code = 'Y'
      and (ISNULL(loan_amt,0) / contract_amt) >= 1
      and contract_amt > 0
    group by idn;
 update #base
    set cash_utilization = v1
    from #tmp as a
    where a.idn = #base.idn;
 /*----Ind001----*/
 delete from #tmp;
 insert into #tmp (idn, mon)
    select idn, min(start_mon_since)
    from #krm021_dedup
    group by idn;
 update #base
    set ind001 = 1
    from #tmp as a
    where a.idn = #base.idn
      and (@now - a.mon) between 1 and 7;
 drop table #base_tmp;
 drop table #tmp;

 insert into #daco_v41_cal(case_sn, idn, dac_sn, inquiry_date, sex)
    select case_sn, idn, dac_sn, inquiry_date, sex
    from app_info;
/*
 update #daco_v41_cal
    set jas002_defect = a.jas002_defect,
        avail_flag = a.avail_flag,
        krm021_hit = a.krm021_hit,
        krm023_hit = a.krm023_hit,
        max_bucket = a.max_bucket,
        fs044 = a.fs044,
        cash_max_bucket = a.cash_max_bucket,
        cash_utilization = a.cash_utilization,
        ind001 = a.ind001
    from #base a
    where a.idn = #daco_v41_cal.idn;
*/    
/* -- end of procedure prepare_jcic_data_all --*/
go

/*
@curr_inqmon int: input parameter, 1: inquiry date > 15, 0 inquiry date <= 15.

Variable		Definition						Calibrated Definition
----------------------- ------------------------------------------------------- ----------------------------------------------------------------------
MS056_6M_1k_tran	Max延遲繳款金額 (M1+) - Pay code in ('D','E','F')	if query date is before 14, then use N and N-1 month as the last month
MT110_43_tran		平均循環百分比 (Total OS / Total Line) paycode CDEF	if query date is before 14, then use N and N-1 month as the last month
FS002_6M_1k_q_tran	有延遲繳款月數 (M1+) - Pay code in ('E','F')		if query date is before 14, then use N and N-1 month as the last month (max 6 months b4 trans)
INT015_9M_tran		cash advnace lines/all open lines (FS016/FS101)		if query date is before 14, then use N and N-1 month as the last month
FS031_tran		JCIC查詢次數within 4 month from the inquiry date	no "15 day rule"
MT203_42_1K_r_tran	balance (paycode DEF) on accounts opened less than a year	keep "15 day" rule
app_last_month_bucket_t1 max bucket for last 2 months,				no "15 day" rule
FS008_12MPLUS_r		強制停額度次數 (Stop code = 3)				no impact
FS069_tran									no impact
MT011_31_z		平均刷卡並付清金額					keep "15 day" rule
FS005_3M_1K		有延遲繳款月數 (M2+) - Pay code in ('F')		if query date is before 14, then use N and N-1 month as the last month (max 3 months before trans)
sex_tran		male							no impact
MT009_43_q_tran2	平均餘額 (Total OS)					keep "15 day" rule
*/

 CREATE PROCEDURE generate_daco41_score
   (@now int)
 AS
 declare @i int
 declare @months int
 /*****************************************************************************************
  *   create temportary intermediate tables
  *****************************************************************************************/
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
 create table #tmp2(
    idn varchar(14),
    issuer char(3));
 create table #open_issuer (
    idn varchar(14),
    issuer char(3),
    mon int);
 create table #lines (
    idn varchar(14),
    issuer char(3),
    mon int,
    line decimal (16, 8));
 create table #open_card (
    idn varchar(14),
    issue char(3),
    mon int);
 create table #open_line (
    idn varchar(14),
    issue char(3),
    mon int,
    cards int,
    bucket int);
 create table #latest_stmt_mon (
    idn varchar(14),
    issue char(3),
    mon int);
 create table #latest_line (
    idn varchar(14),
    issue char(3),
    mon int,
    mob int);
 /*****************************************************************************************
  *   Initialize #daco_v41_cal table
  *****************************************************************************************/
 /*Init stm001 variables*/
 /* No initialization for fs031*/

 /*Init bam085 variables*/
 update #daco_v41_cal
    set fs068 = 0,
        fs040 = 0
    from #bam085_dedup as a
    where a.idn = #daco_v41_cal.idn;

 /*Init krm021 variables*/
 update #daco_v41_cal
    set fs008_12mplus = 0
    from #krm021_dedup as a
    where a.idn = #daco_v41_cal.idn;
 /*Init krm023 variables*/
 update #daco_v41_cal
    set fs002_6m_1k = 0,
        fs005_3m_1k = 0,
        fs016_9m = 0,
        fs101_9m = 0,
        ms009_3m = 0,
        ms009_6m = 0,
	app_last_month_bucket = 0,
        ms011_3m = 0,
        ms011_6m = 0,
        ms011_9m = 0,
        ms011_12m = 0,
        ms056_6m_1k = 0,
        ms110_3m = 0,
        ms110_6m = 0,
        ms106_3m = 0,
        ms106_6m = 0
    from #krm023_dedup as a
    where a.idn = #daco_v41_cal.idn;
 /*Init krm021 and krm023 variables*/
 update #daco_v41_cal
    set ms203_3m_1k = 0,
        ms203_6m_1k = 0,
        ms203_9m_1k = 0
    from #krm021_dedup as a, #krm023_dedup as b
    where a.idn = b.idn
      and a.idn = #daco_v41_cal.idn
      and ((a.issue = b.issue)
          or ((a.issue = '021' and a.card_brand = 'V') and (b.issue = 'CTV'))
          or ((a.issue = '021' and a.card_brand = 'M') and (b.issue = 'CTM'))
          or ((a.issue = '021' and a.card_brand = 'D') and (b.issue = 'CTD'))
          or ((a.issue = 'A82' and a.card_brand = 'A') and (b.issue = 'AEA'))
          )

 /*****************************************************************************************
  *   start making fs002  有延遲繳款月數 (M1+) - Pay code in ('E','F')
  *****************************************************************************************/
 delete from #tmp1;
 delete from #tmp;
 insert into #tmp1 (idn, mon, v1)
    select idn, mon_since, 1
    from #krm023_dedup
    where bucket_ef_1k >= 1
    group by idn, mon_since;
 insert into #tmp (idn, mon, v1)
    select idn, 6, sum(v1)
    from #tmp1
    where (@now - mon) <= 6
      and (@now - mon) >= 0
--      and (@now - mon) >= @curr_inqmon
--      and (@now - mon) > 0     ****old
    group by idn;
 update #daco_v41_cal
    set fs002_6m_1k = v1
    from #tmp as a
    where a.idn = #daco_v41_cal.idn
      and mon = 6;
 update #daco_v41_cal
    set fs002_6m_1k = (case when fs002_6m_1k > 6 then 6
                            else fs002_6m_1k end)

 /*****************************************************************************************
  *   start making fs069   FS040 - FS068
  *  FS068	不動產貸款筆數	purpose_code = 1 and unsecured loan for real estate (without mortgage from the same bank)
  *  FS040	無擔保貸款筆數	account_code2 is NULL
  *****************************************************************************************/
 /* ---start making fs040--- */
  delete from #tmp1;
  insert into #tmp1 (idn, v1)
     select idn, count(*)
     from #bam085_dedup
     where (account_code2=''
        or account_code2 is null)
        and account_code <> 'K'
     group by idn;
  update #daco_v41_cal
     set fs040 = v1
     from #tmp1 as a
     where a.idn = #daco_v41_cal.idn;
 /* ---start making fs068--- */
  delete from #tmp1;
  delete from #tmp2;
  insert into #tmp2(idn, issuer)
     select idn, bank_code2
     from #bam085_dedup
     where purpose_code = '1' 
       and (account_code2 is not null or account_code2 <> '')

   insert into #tmp1(idn, v1) 
      select idn, count(*)
      from #bam085_dedup
      where not exists (select * from #tmp2
                        where #bam085_dedup.idn = #tmp2.idn
                          and #bam085_dedup.bank_code2 = #tmp2.issuer)
        and purpose_code = '1'
        and ((account_code2 is null) or (account_code2 = ''))
        and account_code != 'K'
      group by idn  
  
  update #daco_v41_cal
     set fs068 = v1
     from #tmp1 as a
     where a.idn = #daco_v41_cal.idn;
  update #daco_v41_cal
     set fs069 = fs040 - fs068

 /*****************************************************************************************
  *   start making int015_9m
  *****************************************************************************************/
 /* ---start making fs016--- */
 delete from #tmp1;
 insert into #tmp1 (idn, mon, v1)
    select idn, 9, count(distinct issue)
    from #krm023_dedup
    where (@now - mon_since) <= 9
      and (@now - mon_since) >= 0
--      and (@now - mon_since) >= @curr_inqmon
--      and (@now - mon_since) > 0    ****old
      and cash = 'Y'
    group by idn;
 update #daco_v41_cal
    set fs016_9m = v1
    from #tmp1 as a
    where a.idn = #daco_v41_cal.idn
      and mon = 9;

 /* ---start making fs101--- */
 delete from #tmp1;
 insert into #tmp1 (idn, mon, v1)
    select idn, 9, count(distinct issue)
    from #krm023_dedup
    where (@now - mon_since) <= 9
      and (@now - mon_since) >= 0
--      and (@now - mon_since) >= @curr_inqmon
--      and (@now - mon_since) > 0     ****old
    group by idn;
 update #daco_v41_cal
    set fs101_9m = v1
    from #tmp1 as a
    where a.idn = #daco_v41_cal.idn
      and mon = 9;

 /*****************************************************************************************
  * start making fs031_tran	JCIC查詢次數 (All) (stm001)
  * JCIC 查詢次數 within 4 month from the inquiry date (Credit Card ), no "15 day rule"
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #stm001_dedup
    where item_list is not null
      and item_list <> ''
--    and @now - query_mon_since <= 12  ****old
      and (@now - query_mon_since) <= 3
    group by idn

 update #daco_v41_cal
    set fs031 = v1
    from #tmp as a
    where a.idn = #daco_v41_cal.idn
 /*****************************************************************************************
  * start making fs005_3m_1k	有延遲繳款月數 (M2+) - Pay code in ('F')
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (idn, mon, v1)
    select idn, 3, count(*)
    from #krm023_dedup
    where bucket_f_1k >= 2
      and (@now - mon_since) <= 3
      and (@now - mon_since) >= 0
--      and (@now - mon_since) >= @curr_inqmon
    group by idn

 update #daco_v41_cal
    set fs005_3m_1k = v1
    from #tmp as a
    where a.idn = #daco_v41_cal.idn
      and mon = 3
 update #daco_v41_cal
    set fs005_3m_1k = (case when fs005_3m_1k > 3 then 3
                            else fs005_3m_1k end)

 /*****************************************************************************************
  * start making ms056_6m_1k	Max延遲繳款金額 (M1+) - Pay code in ('D','E','F')
  * Sum up all payment of all delinquent lines by month 2. Pick the max in 3, 6, 9, 14 months
  *****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 /*---start making ms056---*/
 insert into #tmp1 (idn, mon, v1)
    select idn, mon_since, sum(payment_amt)
    from #krm023_dedup
    where pay_code in ('D', 'E', 'F')
      and bucket_def_1k >= 1
    group by idn, mon_since
 insert into #tmp(idn, mon, v1)
    select idn, 6, max(v1)
    from #tmp1
    where (@now - mon) <= 6
      and (@now - mon) >= 0
    group by idn

 update #daco_v41_cal
    set ms056_6m_1k = v1
    from #tmp as a
    where a.idn = #daco_v41_cal.idn and mon=6
 /*****************************************************************************************
  * start making sex_tran	gender
  *****************************************************************************************/
/*
 update #daco_v41_cal
    set sex = substring(idn, 2, 1)
*/
 /*****************************************************************************************
  * start making app_last_month_bucket_t1	last month max among all lines
  * revised: max bucket for last 2 months, no "15 day" rule
  *****************************************************************************************/
 delete from #tmp

 insert into #tmp (idn, v1)
    select idn, max(bucket_ef_1k)
    from #krm023_dedup
    where (@now - mon_since) <= 1  /*max bucket for last 2 months, no "15 day" rule*/
      and (@now - mon_since) >= 0
--      and (@now - mon_since) >= @curr_inqmon
--      and (@now - mon_since) = 1  /**** old */
    group by idn

 update #daco_v41_cal
    set app_last_month_bucket = v1
    from #tmp a
    where a.idn = #daco_v41_cal.idn

 /*****************************************************************************************
  *   start making fs008_12mplus 
  *****************************************************************************************/
 /* Initial global variables */

 set @months = (select @now - min(start_mon_since) from #krm021_dedup);
 delete from #open_issuer;
 set @i = 0;
 while @i <= (@months)
    begin
       insert into #open_issuer (idn, issuer, mon)
          select idn, issue, (@now - @i)
          from #krm021_dedup
          where (end_mon_since > (@now - @i))
            and (start_mon_since <= (@now - @i))
       set @i = @i + 1
    end;
    
 /*********************************************************************************************
  * Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC
  * TMP to store lines closed, OO and MO,
  * OO stands for one-card-one-line, MO stands for Multi-card-one-line
  * Lines to store maximum newly closed lines for each customer on each issuer during
  * specific time frame(month)
  * TMP1 to store line closed for each customer during specific time frame--(3, 6, 9, 12 months)
  ***********************************************************************************************/
  delete from #tmp1;
  delete from #tmp;
  insert into #tmp1 (idn, mon, v1)
     select idn, 13, count(distinct issue)
     from #krm021_dedup as a
     where @now - end_mon_since > 0
       and stop_code = '3'
       and issue not in ('021', '081', '974')
       and issue not in (select distinct issuer
   	  	         from #open_issuer as b
   		         where a.idn = b.idn
   		           and a.end_mon_since = b.mon)
     group by idn;
  insert into #tmp (idn, mon, v1, v2, v3)
     select distinct idn, 13, 0, 0, 0
     from #krm021_dedup;
  update #tmp
     set v1 = a.v1
     from #tmp1 as a
     where a.idn = #tmp.idn
       and a.mon = #tmp.mon;

  delete from #tmp1;
  insert into #tmp1 (idn, mon, v1)
     select idn, 13, count(*)
     from #krm021_dedup
     where stop_code = '3'
       and issue in ('021', '081', '974')
       and @now - end_mon_since > 0
     group by idn;
  update #tmp
     set v2 = a.v1
     from #tmp1 as a
     where a.idn = #tmp.idn
       and a.mon = #tmp.mon;
  update #daco_v41_cal
     set fs008_12mplus = v1 + v2
     from #tmp as a
     where a.idn = #daco_v41_cal.idn
       and mon = 13;

 /*****************************************************************************************
  *   start making ms009
  *****************************************************************************************/
  delete from #tmp1;
  delete from #tmp;
  insert into #tmp1 (idn, mon, v1)
     select idn, mon_since, sum(payment_amt)
     from #krm023_dedup
     group by idn, mon_since;
  set @i = 3
  while @i <= 6
     begin
        insert into #tmp (idn, mon, v1)
           select idn, @i, avg(v1)
           from #tmp1
           where (@now - mon) <= @i
             and (@now - mon) > 0      -- keep 15-day rule
           group by idn
        set @i = @i + 3
     end;
  update #daco_v41_cal
     set ms009_3m = v1
     from #tmp as a
     where a.idn = #daco_v41_cal.idn
       and mon=3;
  update #daco_v41_cal
     set ms009_6m = v1
     from #tmp as a
     where a.idn = #daco_v41_cal.idn
       and mon=6;
 /*****************************************************************************************
  *   start making ms011
  *****************************************************************************************/
  delete from #tmp1;
  delete from #tmp;
  insert into #tmp1 (idn, mon, v1)
     select idn, mon_since, sum(case when pay_code in ('A', 'B') then payment_amt else 0 end)
     from #krm023_dedup
     group by idn, mon_since;
  set @i = 3
  while @i <= 12
     begin
        insert into #tmp (idn, mon, v1)
           select idn, @i, avg(v1)
           from #tmp1
           where (@now - mon) <= @i
             and (@now - mon) > 0      -- keep 15-day rule
           group by idn
        set @i = @i + 3
     end;
  update #daco_v41_cal
     set ms011_3m = v1
     from #tmp as a
     where a.idn = #daco_v41_cal.idn
       and mon = 3;
  update #daco_v41_cal
     set ms011_6m = v1
     from #tmp as a
     where a.idn = #daco_v41_cal.idn
       and mon = 6;
  update #daco_v41_cal
     set ms011_9m = v1
     from #tmp as a
     where a.idn = #daco_v41_cal.idn
       and mon = 9;
  update #daco_v41_cal
     set ms011_12m = v1
     from #tmp as a
     where a.idn = #daco_v41_cal.idn
       and mon = 12;
 /*****************************************************************************************
  *   start making ms106 平均循環金額	CDEF	(Per month, FOR K25 AVAILIBLE MONTHS ONLY)
  *****************************************************************************************/
 delete from #tmp1;
 delete from #tmp;
 insert into #tmp1 (idn, mon, v1)
    select idn, mon_since, sum(case when pay_code in ('C', 'D', 'E', 'F') then payment_amt
    				    else 0 end)
    from #krm023_dedup
    group by idn, mon_since;
 set @i = 3
 while @i <= 6
    begin
       insert into #tmp (idn, mon, v1)
          select idn, @i, avg(v1)
          from #tmp1
          where (@now - mon) <= @i
            and (@now - mon) >= 0
--            and (@now - mon) >= @curr_inqmon
--            and (@now - mon) > 0      ****old
          group by idn
       set @i = @i + 3
    end;
 update #daco_v41_cal
    set ms106_3m = v1
    from #tmp as a
    where a.idn = #daco_v41_cal.idn
      and mon=3;
 update #daco_v41_cal
    set ms106_6m = v1
    from #tmp as a
    where a.idn = #daco_v41_cal.idn
      and mon=6;
 /*****************************************************************************************
  *   start making ms110 平均循環百分比 (Total OS / Total Line)	CDEF	Payment_amt
  *****************************************************************************************/
 delete from #tmp1;
 delete from #tmp;
 insert into #tmp1 (idn, mon, v1)
    select idn, mon_since, sum(convert(decimal(16, 8), (case when limit='' then NULL else limit end)))
    from #krm023_dedup
    group by idn, mon_since;
 set @i = 3
 while @i <= 6
    begin
       insert into #tmp (idn, mon, v1)
          select idn, @i, max(v1)
          from #tmp1
          where (@now - mon) <= @i
            and (@now - mon) >= 0
--            and (@now - mon) >= @curr_inqmon
--          and (@now - mon) > 0      /****old*/
          group by idn
       set @i = @i + 3
    end;
 update #daco_v41_cal
    set ms110_3m = (case when v1=0 then null else ms106_3m / v1 end)
    from #tmp as a
    where a.idn = #daco_v41_cal.idn
      and mon=3;
 update #daco_v41_cal
    set ms110_6m = (case when v1=0 then null else ms106_6m / v1 end)
    from #tmp as a
    where a.idn = #daco_v41_cal.idn
      and mon=6;
 /*****************************************************************************************
  *   start making Prepare intermediate tables for KRM023 and krm021 variables
  *****************************************************************************************/
  set @i = 0
  while @i <= 13
     begin
        insert into #open_card (idn, issue, mon)
           select idn,
                  (case when card_brand = 'A' and issue = 'A82' then 'AEA'
                   else issue end),  -- for American Express
                  (@now - @i)
           from #krm021_dedup
           where (end_mon_since > (@now - @i))
             and (start_mon_since <= (@now - @i)) -- add .eq.
             and issue != '021'
        set @i = @i + 1
     end;
  insert into #open_line (idn, issue, mon, cards)
     select idn, issue, mon, count(*)
     from #open_card
     group by idn, issue, mon;
  set @i = 0
  while @i <= 13
     begin
        insert into #open_line (idn, issue, mon, cards)
           select idn,
                  (case when card_brand = 'M' then 'CTM'
                        when card_brand = 'V' then 'CTV'
                        when card_brand = 'D' then 'CTD' end),
                  (@now - @i),
                  1
           from #krm021_dedup
           where (end_mon_since > (@now - @i))
             and (start_mon_since <= (@now - @i))  -- add .eq.
             and issue = '021'
        set @i = @i + 1
     end;
  update #open_line
     set bucket = 1;
 /*arbitrary number, just to make sure it will out-of-range*/
  update #open_line
     set bucket = 100
     where mon = (@now - 13);
  set @i = 13;
  while @i > 0
     begin
        update #open_line
           set bucket = a.bucket + 1
        from #open_line as a inner join #open_line
             on  a.idn   = #open_line.idn
             and a.issue = #open_line.issue
             and a.mon   = (#open_line.mon - 1)
        where (@now - a.mon) = @i
        set @i = @i - 1
    end;
  insert into #latest_stmt_mon (idn, issue, mon)
     select idn, issue, max(mon)
     from #open_line
     where mon <= @now  --- add .eq.
--     where mon < @now  ****old
     group by idn, issue;
  insert into #latest_line (idn, issue, mon, mob)
     select a.idn, a.issue, a.mon, a.bucket
     from #open_line as a inner join #latest_stmt_mon as b
         on a.idn = b.idn
         and a.issue = b.issue
         and a.mon = b.mon;
 /*****************************************************************************************
  *   start making ms203_1k
  * MS203_1K	amount owed on accounts opened less than a year	paycode DEF
  *****************************************************************************************/
  delete from #tmp1;
  set @i = 3
  while @i <= 9
     begin
        insert into #tmp1 (idn, mon, v1)
           select idn, @i, sum(payment_amt)
           from #krm023_dedup as a
           where issue in (select issue
      		           from #latest_line
    		           where mob <= 12 and idn = a.idn)
             and bucket_def_1k <> 0
             and @now - mon_since <= @i
             and @now - mon_since > 0      -- keep 15-day rule
           group by idn
        set @i = @i + 3
     end;
  update #daco_v41_cal
     set ms203_3m_1k = v1
     from #tmp1 as a
     where a.idn = #daco_v41_cal.idn
       and a.mon = 3;
  update #daco_v41_cal
     set ms203_6m_1k = v1
     from #tmp1 as a
     where a.idn = #daco_v41_cal.idn
       and a.mon = 6;
  update #daco_v41_cal
     set ms203_9m_1k = v1
     from #tmp1 as a
     where a.idn = #daco_v41_cal.idn
       and a.mon = 9;
 /*****************************************************************************************
  * start making transformed variables:
  *   if MS056_6M_1k>120 then MS056_6M_1k_tran=120;
  * 	 else MS056_6M_1k_tran=MS056_6M_1k;
  *   if MT110_43<0 then MT110_43_tran=0;
  *	 else if MT110_43>1 then MT110_43_tran=1;
  *	 else MT110_43_tran=MT110_43;
  *   FS002_6M_1k_q=FS002_6M_1k**2;
  *   if FS002_6M_1k_q>16 then FS002_6M_1k_q_tran=16;
  *	 else FS002_6M_1k_q_tran=FS002_6M_1k_q;
  *   INT015_9M	=	FS016_9M/FS101_9M;
  *   if INT015_9M=. then INT015_9M_tran=0.4;
  *	 else if INT015_9M>0.4 then INT015_9M_tran=0.4;
  *	 else INT015_9M_tran=INT015_9M;
  *   if FS031=. then FS031_tran=0;
  *	 else if FS031>8 then FS031_tran=8;
  *	 else FS031_tran=FS031;
  *   MT203_42_1K_r=MT203_42_1K**0.5;
  *   if MT203_42_1K_r=. then MT203_42_1K_r_tran=3.3;
  *	 else if MT203_42_1K_r>6 then MT203_42_1K_r_tran=6;
  *	 else MT203_42_1K_r_tran=MT203_42_1K_r;
  *   if App_Last_Month_Bucket=. then app_last_month_bucket_t1=0;
  *	 else app_last_month_bucket_t1=App_Last_Month_Bucket;
  *   FS008_12MPLUS_r=FS008_12MPLUS**0.5;
  *   if FS069=. then FS069_tran=-0.5;
  *	 else if FS069>6 then FS069_tran=6;
  *	 else FS069_tran=FS069;
  *   MT011_31_z=(MT011_31=0);
  *   sex_tran=(sex="M");
  *   MT009_43_q=MT009_43**2;
  *   MT009_43_q_tran2=(MT009_43_q>75000);
  *
  *****************************************************************************************/
  update #daco_v41_cal
     set mt110_43 =  ms110_3m -  (ms110_6m +  ms110_3m),
         mt203_42_1k = ms203_3m_1k - ms203_9m_1k + ms203_6m_1k,
         mt009_43 = 2 * ms009_3m - ms009_6m,
         mt011_31 = ms011_6m - ms011_3m - ms011_12m + ms011_9m
  update #daco_v41_cal
     set fs002_6m_1k_q = power(fs002_6m_1k, 2),
         int015_9m = (case when fs101_9m = 0 then null else fs016_9m / fs101_9m end),
         mt203_42_1k_r = power((case when mt203_42_1k < 0 then null else mt203_42_1k end), 0.5),
         mt011_31_z = (case when mt011_31 = 0 then 1 else 0 end),
         fs008_12mplus_r = power((case when fs008_12mplus < 0 then null else fs008_12mplus end), 0.5),
         mt009_43_q = power(mt009_43, 2)
  update #daco_v41_cal
     set ms056_6m_1k_tran = (case when ms056_6m_1k >120 then 120
    			          else ms056_6m_1k end),
         mt110_43_tran = (case when mt110_43 is null then 0
         		       when mt110_43 < 0 then 0
                               when mt110_43 > 1 then 1
    			       else mt110_43 end),
         fs002_6m_1k_q_tran = (case when fs002_6m_1k_q > 16 then 16
    			       else fs002_6m_1k_q end),
         fs031_tran = (case when fs031 is null then 0
    			    when fs031 > 8 then 8
    			    else fs031 end),
         mt203_42_1k_r_tran = (case when mt203_42_1k_r is null then 3.3
    			         when mt203_42_1k_r > 6 then 6
    			         else mt203_42_1k_r end),
         app_last_month_bucket_t1 = (case when app_last_month_bucket is null then 0
   				            else app_last_month_bucket end),
         int015_9m_tran = (case when int015_9m is null then 0.4
         		     when int015_9m > 0.4 then 0.4
                             else int015_9m end),
         fs069_tran = (case when fs069 is null then -0.5
         		    when fs069 > 6 then 6
                            else fs069 end),
         sex_tran = (case when sex = 1 then 1 else 0 end),
         mt009_43_q_tran2=(case when mt009_43_q > 75000 then 1 else 0 end);
 /* ---start making DACO score v4.1--- */
   update #daco_v41_cal
    set rscore41 = -0.02916
               + ms056_6m_1k_tran	*	0.00100
               + mt110_43_tran		*	0.14941
               + fs002_6m_1k_q_tran	*	0.01346
               + int015_9m_tran		*	0.26009
               + fs031_tran		*	0.01318
               + mt203_42_1k_r_tran	*	0.02712
               + app_last_month_bucket_t1 *	0.08179
               + fs008_12mplus_r	*	0.20956
               + fs069_tran		*	0.01431
               + mt011_31_z		*	0.06573
               + fs005_3m_1k		*	0.12226
               + sex_tran		*	0.02438
               + mt009_43_q_tran2	*	0.09215;
          /* -- end of procedure generate_daco41_score --*/
go

 CREATE PROCEDURE fwpl_rscore
  AS

 declare @case_sn varchar(20)
 declare @idn     varchar(14)
 declare @dac_sn  int
 declare @inquiry_date char(8)
 declare @year int
 declare @month int
 declare @day int
 declare @curr_inqmon int
 declare @now int


 /* create working tables */
  create table #bam085_dedup (
  	case_sn varchar(20),
  	idn varchar (14),
  	dac_sn decimal(3,0),
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
  	cnt int
  );
  create table #krm021_dedup (
  	case_sn varchar(20),
  	idn varchar (14),
  	dac_sn decimal(3,0),
  	card_brand char (1),
  	card_type char (1),
  	issue char (3),
  	issue_name char (40),
  	start_date char (7),
  	stop_date char (7),
  	stop_code char (1),
  	ab_code char (1),
  	m_s char (1),
  	limit char (6),
  	start_mon_since int,
  	end_mon_since int,
  	cnt int
  );
  create table #krm023_dedup (
  	case_sn varchar(20),
  	idn varchar (14),
  	dac_sn decimal(3,0),
  	issue char (3),
  	issue_name char (40),
  	limit char (6),
  	yrmon char (5),
  	kr_code char (7),
  	payment char (3),
  	cash char (1),
  	pay_code char (1),
  	mon_since int,
  	payment_amt float,
  	bucket_def_1k int default 0,
  	bucket_ef_1k int default 0,
  	bucket_f_1k int default 0,
  	cnt int
  );
  create table #stm001_dedup (
  	case_sn varchar(20),
  	idn varchar (14),
  	dac_sn decimal(3,0),
  	bank_code char (7),
  	bank_name char (40),
  	query_date char (7),
  	item_list char (10),
  	query_mon_since int,
  	cnt int
  );
  /*
  create table #jas002_t (
  	case_sn varchar(20),
  	idn char (11),
  	dac_sn decimal(3,0),
  	reason char (1),
  	date_happen char (7),
  	mon_since int
  );
  create table #jas002_t_dedup (
  	case_sn varchar(20),
  	idn varchar(14),
  	dac_sn decimal(3,0),
  	reason char (1),
  	date_happen char (7),
  	mon_since int,
  	cnt int
  );
  */
  create table #base (
   	idn varchar(14),
  	avail_flag int default 0,
  	jas002_defect int default 0,
  	krm021_hit int default 0,
  	krm023_hit int default 0,
  	max_bucket int default 0,
 	fs044 int default 0,
  	cash_max_bucket int default 0,
  	cash_utilization int default 0,
        ind001 int default 0
  );
   create table #daco_v41_cal (
    case_sn		varchar(20),
    idn			varchar(14),
    dac_sn		decimal(3,0),
    inquiry_date	char(8),
    guarantor		char (1),
    sex			int,
    now			int,
/*DACO 4.1 variables*/
    ms056_6m_1k		decimal (16, 8),
    ms056_6m_1k_tran	decimal (16, 8),
    ms110_3m		decimal (16, 8),
    ms110_6m		decimal (16, 8),
    ms106_3m		decimal (16, 8),
    ms106_6m		decimal (16, 8),
    mt110_43		decimal (16, 8),
    mt110_43_tran	decimal (16, 8),
    fs002_6m_1k		decimal (16, 8),
    fs002_6m_1k_q	decimal (16, 8),
    fs002_6m_1k_q_tran	decimal (16, 8),
    fs016_9m		decimal (16, 8),
    fs101_9m		decimal (16, 8),
    fs040		decimal (16, 8),
    fs068		decimal (16, 8),
    fs069		decimal (16, 8),
    fs069_tran		decimal (16, 8),
    int015_9m		decimal (16, 8),
    int015_9m_tran	decimal (16, 8),
    fs031		decimal (16, 8),
    fs031_tran		decimal (16, 8),
    ms203_3m_1k		decimal (16, 8),
    ms203_6m_1k		decimal (16, 8),
    ms203_9m_1k		decimal (16, 8),
    mt203_42_1k		decimal (16, 8),
    mt203_42_1k_r	decimal (16, 8),
    mt203_42_1k_r_tran	decimal (16, 8),
    app_last_month_bucket	decimal (16, 8),
    app_last_month_bucket_t1	decimal (16, 8),
    fs008_12mplus	decimal (16, 8),
    fs008_12mplus_r	decimal (16, 8),
    fs008_12mplus_r_tran decimal (16, 8),
    ms011_3m		decimal (16, 8),
    ms011_6m		decimal (16, 8),
    ms011_9m		decimal (16, 8),
    ms011_12m		decimal (16, 8),
    mt011_31		decimal (16, 8),
    mt011_31_z		decimal (16, 8),
    ms009_3m		decimal (16, 8),
    ms009_6m		decimal (16, 8),
    mt009_43		decimal (16, 8),
    mt009_43_q		decimal (16, 8),
    mt009_43_q_tran2	decimal (16, 8),
    fs005_3m_1k		decimal (16, 8),
    sex_tran		int,
    rscore41		decimal (16, 8),
    );

   create table #ploan_v41_cal (
    case_sn		varchar(20),
    idn			varchar(14),
    dac_sn		decimal(3,0),
    inquiry_date	char(8),
    guarantor		char(1),
    sex			int,
    now			int,
    avail_flag		int,
    jas002_defect	int,
    krm021_hit		int,
    krm023_hit		int,
    max_bucket		int,
    fs044		int,
    cash_max_bucket	int,
    cash_utilization	int,
    ind001		int,
/* DACO4.1 variables*/
    fs031		decimal (16, 8),
    app_last_month_bucket	decimal (16, 8),
    rscore41		decimal (16, 8),
/*Fuhwa Ploan variables*/
    fs102_3m		decimal (16, 8),
    fs102_6m		decimal (16, 8),
    fs102_9m		decimal (16, 8),
    ft102_42		decimal (16, 8),
    ft102_42_r		decimal (16, 8),
    ft102_42_r_tran	decimal (16, 8),
    fs031_q		decimal (16, 8),
    fs031_q_tran	decimal (16, 8),
    gender_z		int,
    fs212_3m_1k		decimal (16, 8),
    fs212_6m_1k		decimal (16, 8),
    ft212_43_1k		decimal (16, 8),
    ft212_43_1k_q	decimal (16, 8),
    ft212_43_1k_q_tran	decimal (16, 8),
    ms120_9m		decimal (16, 8),
    ms120_9m_r		decimal (16, 8),
    ms120_9m_r_tran	decimal (16, 8),
    app_last_month_bucket_tran	decimal (16, 8),
    ms026_3m		decimal (16, 8),
    ms026_3m_r		decimal (16, 8),
    ms026_3m_r_tran	decimal (16, 8),
    fs059_3m_1k 	decimal (16, 8),
    fs059_6m_1k		decimal (16, 8),
    ft059_43_1k		decimal (16, 8),
    ft059_43_1k_q	decimal (16, 8),
    ft059_43_1k_q_tran	decimal (16, 8),
    fs063_12m_1k	decimal (16, 8),
    fs063_12m_1k_q	decimal (16, 8),
    ms024_3m		decimal (16, 8),
    ms024_3m_tran	decimal (16, 8),
    fs018_3m		decimal (16, 8),
    fs018_6m		decimal (16, 8),
    fs018_9m		decimal (16, 8),
    ft018_42		decimal (16, 8),
    ft018_42_tran	decimal (16, 8),
    ft018_42_tran_q	decimal (16, 8),
    rs017		decimal (16, 8),
    rs017_r		decimal (16, 8),
    rs017_r_tran	decimal (16, 8),
    fs005_12m_1k	decimal (16, 8),
    fs005_12m_1k_r	decimal (16, 8),
    ms119_12m		decimal (16, 8),
    ms119_12m_r		decimal (16, 8),
    ms119_12m_r_tran	decimal (16, 8),
    f_score1		decimal (16, 8),
    riskscore		decimal (16, 8),
    twentile		int,
    pd			decimal (16, 8)
    );

 declare cursor1 cursor for
 select case_sn, idn, dac_sn, inquiry_date
 from app_info

 open cursor1

 fetch next from cursor1
 into @case_sn, @idn, @dac_sn, @inquiry_date

 while @@FETCH_STATUS = 0
 begin

    set @year = convert (int, substring(@inquiry_date, 1, 4))
    set @month = convert (int, substring(@inquiry_date, 5, 2))
    set @day = convert (int, substring(@inquiry_date, 7,2))
    set @curr_inqmon = 1

    if (@day <= 15)
      begin
        set @month = @month - 1
        set @curr_inqmon = 0
      end
    if (@month = 0)
      begin
        set @month = 12;
        set @year = @year - 1
      end

    set @now = (@year - 1911) * 12 + @month;

    exec prepare_jcic_data @case_sn, @idn, @dac_sn, @now
    exec generate_daco41_score @now

    insert into #ploan_v41_cal (case_sn, idn, dac_sn, inquiry_date, guarantor, sex, now, fs031, app_last_month_bucket, rscore41)
       select case_sn, idn, dac_sn, inquiry_date, guarantor, sex, now, fs031, app_last_month_bucket, rscore41
       from #daco_v41_cal
       
    update #ploan_v41_cal
       set jas002_defect = a.jas002_defect,
           avail_flag = a.avail_flag,
           krm021_hit = a.krm021_hit,
           krm023_hit = a.krm023_hit,
           max_bucket = a.max_bucket,
           fs044 = a.fs044,
           cash_max_bucket = a.cash_max_bucket,
           cash_utilization = a.cash_utilization,
           ind001 = a.ind001
       from #base a
       where a.idn = #ploan_v41_cal.idn;

    exec generate_ploan_score41 @now, @curr_inqmon

    insert into daco_v41_cal
      select * from #daco_v41_cal
    insert into ploan_v41_cal
      select * from #ploan_v41_cal

    delete from #bam085_dedup;
    delete from #krm021_dedup;
    delete from #krm023_dedup;
    delete from #stm001_dedup;
--    delete from #jas002_t;
--    delete from #jas002_t_dedup;
    delete from #base;
    delete from #daco_v41_cal;
    delete from #ploan_v41_cal;

    fetch next from cursor1
    into @case_sn, @idn, @dac_sn, @inquiry_date
 end

 close cursor1
 deallocate cursor1
 go

 -- exec fwpl_rscore
/*
    drop table #bam085_dedup;
    drop table #krm021_dedup;
    drop table #krm023_dedup;
    drop table #stm001_dedup;
--    drop table #jas002_t;
--    drop table #jas002_t_dedup;
    drop table #base;
    drop table #daco_v41_cal;
    drop table #ploan_v41_cal;
*/