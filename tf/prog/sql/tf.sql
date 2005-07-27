/* Get date from input tables*/

alter PROCEDURE TF_prepare_jcic_data_all
 (@app_sn char(11), @data_time char(8))
 AS
 SET NOCOUNT ON
 insert into #krm001_dedup (app_sn, data_time, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, limit_type, usage_type, secure, cnt)
    select app_sn, data_time, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, limit_type, usage_type, secure, count(*)
    from krm001
    where app_sn = @app_sn and data_time = @data_time
    group by app_sn, data_time, card_brand, card_type, issue, issue_name, start_date, stop_date, stop_code, ab_code, m_s, limit, limit_type, usage_type, secure;
 insert into #krm023_dedup (app_sn, data_time, issue, issue_name, limit, yrmon, kr_code, payment, cash, pay_code, cnt)
    select app_sn, data_time, issue, issue_name, limit, yrmon, kr_code, payment,  cash, pay_code, count(*)
    from krm023
    where app_sn = @app_sn and data_time = @data_time
    group by app_sn, data_time, issue, issue_name, limit, yrmon, kr_code, payment, cash, pay_code;
 insert into #stm001_dedup (app_sn, data_time, bank_code, bank_name, query_date, item_list, cnt)
    select app_sn, data_time, bank_code, bank_name, query_date, item_list, count(*)
    from stm001
    where app_sn = @app_sn and data_time = @data_time
    group by app_sn, data_time, bank_code, bank_name, query_date, item_list;
 insert into #bam085_dedup (app_sn, data_time, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, cnt)
    select app_sn, data_time, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, count(*)
    from bam082
    where app_sn = @app_sn and data_time = @data_time
    group by app_sn, data_time, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan;
 insert into #jas002_t (app_sn, data_time, reason, date_happen)
    select app_sn, data_time, 'D', delinquent_date
    from jas002
    where app_sn = @app_sn and data_time = @data_time
      and delinquent_date is not null;
 insert into #jas002_t (app_sn, data_time, reason, date_happen)
    select app_sn, data_time, 'B', bad_check_date
    from jas002
    where app_sn = @app_sn and data_time = @data_time
      and bad_check_date is not null;
 insert into #jas002_t (app_sn, data_time, reason, date_happen)
    select app_sn, data_time, 'R', reject_date
    from jas002
    where app_sn = @app_sn and data_time = @data_time
      and reject_date is not null;
 insert into #jas002_t (app_sn, data_time, reason, date_happen)
    select app_sn, data_time, 'S', stop_card_date
    from jas002
    where app_sn = @app_sn and data_time = @data_time
      and stop_card_date is not null;
 insert into #jas002_t_dedup (app_sn, data_time, reason, date_happen, mon_since, cnt)
    select app_sn, data_time, reason, date_happen, mon_since, count(*)
    from #jas002_t
    group by app_sn, data_time, reason, date_happen, mon_since;
 update #krm001_dedup
    set start_date = (case
    		   when left(start_date,1) not in ('1', '0', '*') then '0' + (start_date)
    		   else start_date 
    		  end),
        stop_date =  (case
    		   when left(stop_date,1) not in ('1', '0', '*') then '0' + (stop_date)
     		   else stop_date 
    		  end);
 update #krm001_dedup
    set now = (case when substring(data_time, 7, 2) > '15' then
     				(convert (int, substring(data_time, 1, 4)) - 1911) * 12
               			+ convert (int, substring(data_time, 5, 2))
              	    when substring(data_time, 7, 2) <= '15' then
     				(convert (int, substring(data_time, 1, 4)) - 1911) * 12
               			+ convert (int, substring(data_time, 5, 2)) - 1
               	end)
 update #krm001_dedup
    set start_mon_since = (case
    			when left(start_date,1) = '*' then null
    			else left(start_date, 3) * 12 + right(left(start_date, 5), 2)
    		       end),
        end_mon_since = (case
    			when left(stop_date, 1) = '*' then null
    			else left(stop_date, 3) * 12 + right(left(stop_date, 5), 2)
    		     end);
 update #krm001_dedup
    set end_mon_since = 12000   /*999 * 12 + 12*/
    where end_mon_since is null;
 update #jas002_t_dedup
    set date_happen = (case when left(ltrim(date_happen),1) not in ('1', '0', '*') then '0' + (ltrim(date_happen))
     		            else date_happen 
     		       end);
 update #jas002_t_dedup
    set mon_since = (case when left(ltrim(date_happen),1) = '*' then null
       		          else left(ltrim(date_happen), 3) * 12 + right(left(rtrim(date_happen), 5), 2)
     		     end);
 update #jas002_t_dedup
    set now = (case when substring(data_time, 7, 2) > '15' then
     				(convert (int, substring(data_time, 1, 4)) - 1911) * 12
               			+ convert (int, substring(data_time, 5, 2))
              	    when substring(data_time, 7, 2) <= '15' then
     				(convert (int, substring(data_time, 1, 4)) - 1911) * 12
               			+ convert (int, substring(data_time, 5, 2)) - 1
               	end)
 update #krm023_dedup
    set yrmon = (case when left(yrmon,1) not in ('1', '0', '*') then '0' + (yrmon)
    	              else yrmon 
    	         end);
 update #krm023_dedup
    set mon_since = (case when left(yrmon,1) = '*' then null
   		          else left(yrmon, 3) * 12 + right(left(yrmon, 5), 2)
 		     end);
 update #krm023_dedup
    set now = (case when substring(data_time, 7, 2) > '15' then
     				(convert (int, substring(data_time, 1, 4)) - 1911) * 12
               			+ convert (int, substring(data_time, 5, 2))
              	    when substring(data_time, 7, 2) <= '15' then
     				(convert (int, substring(data_time, 1, 4)) - 1911) * 12
               			+ convert (int, substring(data_time, 5, 2)) - 1
               	end)
 update #stm001_dedup
    set query_date = (case when left(query_date,1) not in ('1', '0', '*') then '0' + (query_date)
 		           else query_date 
 		      end);
 update #stm001_dedup
    set query_mon_since = (case	when left(query_date,1) = '*' then null
 			        else left(query_date, 3) * 12 + right(left(query_date, 5), 2)
 		           end);
 update #stm001_dedup
    set now = (case when substring(data_time, 7, 2) > '15' then
     				(convert (int, substring(data_time, 1, 4)) - 1911) * 12
               			+ convert (int, substring(data_time, 5, 2))
              	    when substring(data_time, 7, 2) <= '15' then
     				(convert (int, substring(data_time, 1, 4)) - 1911) * 12
               			+ convert (int, substring(data_time, 5, 2)) - 1
               	end)
 update #krm023_dedup
    set payment = (case when right(left(payment,2),1) in ('H', 'M', 'L') then '0' + (left(payment,2))
                        else payment end);
 update #krm023_dedup
    set payment_amt = (case right(payment,1) when 'L' then 2 when 'M' then 5 when 'H' then 8
                          else 0 end) * power(10, isnull(left(payment,2),0)-1) / 1000.0;        
 update #krm023_dedup
    set bucket_def_1k = (case when pay_code in ('D', 'E', 'F') and payment_amt > 1 then 1 
                              else 0 end), 
        bucket_ef_1k = (case when pay_code in ('E', 'F') and payment_amt > 1 then 1
                             else 0 end), 
        bucket_f_1k = (case when pay_code = 'F' and payment_amt > 1 then 1
                            else 0 end);
 declare @i int
 set @i=12 
 while @i > 0
    begin
       update #krm023_dedup
          set  
            bucket_def_1k =(case when #krm023_dedup.pay_code in ('D', 'E', 'F') and
                                      #krm023_dedup.payment_amt > 1 
                                 then a.bucket_def_1k + 1 
                                 else 0 end), 
            bucket_ef_1k =(case when  #krm023_dedup.pay_code in ('E', 'F') and
                                      #krm023_dedup.payment_amt > 1 
                                then a.bucket_ef_1k + 1 
                                else 0 end), 
            bucket_f_1k =(case when  #krm023_dedup.pay_code = 'F' and
                                     #krm023_dedup.payment_amt > 1 
                               then a.bucket_f_1k + 1 
                               else 0 end) 
          from #krm023_dedup, #krm023_dedup as a, #tf_ploan_cal as b
          where a.app_sn = #krm023_dedup.app_sn 
            and a.issue = #krm023_dedup.issue 
            and a.mon_since = (#krm023_dedup.mon_since - 1)
            and a.app_sn = b.app_sn
            and (b.now - a.mon_since) = @i 
       set @i = @i - 1 
    end;
  update #bam085_dedup
     set bank_code2 = left(bank_code,3),
         mon_since = data_yyy * 12 + data_mm,
         cycle = len(ltrim(rtrim(pay_code_12))),
         account_code = (case when account_code is null then '' else account_code end),
         account_code2 = (case when account_code2 is null then '' else account_code2 end),
         purpose_code = (case when purpose_code is null then '' else purpose_code end),
         co_loan = (case when co_loan is null then '' else co_loan end)
 
 set @i = 1
 while @i <= 12
    begin
       insert into #bam085_bucket (app_sn, data_time, bank_code, mon_since, account_code, account_code2, purpose_code,
                                  contract_amt, loan_amt, pass_due_amt, co_loan, bucket)
          select app_sn, data_time, bank_code, (mon_since - @i + 1), account_code, account_code2, purpose_code, contract_amt,
            loan_amt, pass_due_amt,  co_loan,
                         (case when substring(rtrim(pay_code_12), @i, 1) = 'A' then 0.25
                               when substring(rtrim(pay_code_12), @i, 1) = 'B' then 0.5
                               when substring(rtrim(pay_code_12), @i, 1) = 'X' then 0
                               else convert(float, substring(rtrim(pay_code_12), @i, 1)) end)
          from #bam085_dedup
          where cycle >= @i
       set @i = @i + 1
    end
go

-- EXEC TF_prepare_jcic_data_all 1118

/* -- end of procedure prepare_jcic_data_all --*/

/* Pre-screen*/
/* JCIC tables Hits */
/*
alter PROCEDURE TF_loan_prescreen
 AS
 SET NOCOUNT ON
 create table #tmp (
  app_sn char(11),
  mon int,
  v1 decimal (16, 8),
  v2 decimal (16, 8),
  v3 decimal (16, 8));
*/
 if exists (select * from dbo.sysobjects where id = object_id(N'#base_tmp') and objectproperty(id, N'isusertable') = 1)
    drop table #base_tmp;
 create table #base_tmp (
 	app_sn char (11),
 	avail_flag int
 );
 insert into #base_tmp(app_sn, avail_flag)
    select distinct app_sn, 0 
    from #tf_ploan_cal;
 insert into #base_tmp(app_sn, avail_flag)  /* 0x01 krm001 hit*/
    select distinct app_sn, 1 
    from #krm001_dedup;
 insert into #base_tmp(app_sn, avail_flag)  /* 0x02 krm023 hit*/
    select distinct app_sn, 2
    from #krm023_dedup;
 insert into #base_tmp(app_sn, avail_flag)  /* 0x04 bam085 hit*/
    select distinct app_sn, 4
    from #bam085_dedup;
                                            /* 0x08 bam101 hit*/
 insert into #base_tmp(app_sn, avail_flag)  /* 0x10 stm001 hit*/
    select distinct app_sn, 16
    from #stm001_dedup;
 insert into #base_tmp(app_sn, avail_flag)  /* 0x20 jas002 hit*/
    select distinct app_sn, 32
    from jas002
 insert into #tmp (app_sn, v1)
    select app_sn, sum(avail_flag)
    from #base_tmp
    group by app_sn;
 update #tf_ploan_cal
    set avail_flag = a.v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn;
 /* Update filter variable*/
 update #tf_ploan_cal
    set bam085_hit = 1
    where avail_flag & 0x04 = 0x04;
 update #tf_ploan_cal
    set krm023_hit = 1
    where avail_flag & 0x02 = 0x02;
 update #tf_ploan_cal
    set krm001_hit = 1
    where avail_flag & 0x01 = 0x01;
/* jas002_defect  = 1 */
 delete from #tmp;
 insert into #tmp (app_sn, v1)
    select app_sn, count(*)
    from #jas002_t_dedup
    where now - mon_since <= 36
    group by app_sn;
 update #tf_ploan_cal
    set jas002_defect = 1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
      and a.v1 > 0;
/* app_max_bucket > 3 */
 delete from #tmp;
 insert into #tmp (app_sn, v1)
    select app_sn, max(bucket_ef_1k)
    from #krm023_dedup
    group by app_sn
 update #tf_ploan_cal
    set app_max_bucket = a.v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn;
/* fs044 > 0 */
 delete from #tmp;
 insert into #tmp (app_sn, v1)
    select app_sn, count(*)
    from #bam085_dedup
    where pass_due_amt > 0
    group by app_sn;
 update #tf_ploan_cal
    set fs044 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn;
/* FS334 > 3 曾經最高逾期月數*/
 delete from #tmp
 insert into #tmp(app_sn, v1)
    select app_sn, max(bucket)
    from #bam085_bucket
    group by app_sn  
 update #tf_ploan_cal
    set fs334 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
 /* FS302 >= 1 當月逾期現金卡筆數(cash card max bucket) */
 delete from #tmp;
 insert into #tmp (app_sn, v1)
    select app_sn, count(*)
    from #bam085_dedup
    where account_code = 'Y'
      and ISNULL(left(pay_code_12, 1),'0') not in ('0', 'X')
    group by app_sn;
 update #tf_ploan_cal
    set fs302 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn;
 /* IND001   Account payment history is less than 6 month */
 delete from #tmp;
 insert into #tmp (app_sn, mon)
    select app_sn, min(start_mon_since)
    from #krm001_dedup
    group by app_sn;
 update #tf_ploan_cal
    set ind001 = 1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
      and (#tf_ploan_cal.now - a.mon) between 1 and 7;
/* MS080 貸款已繳款總金額 */
 delete from #tmp
 insert into #tmp(app_sn, v1)
    select app_sn, sum(cast(ISNULL(contract_amt, 0) as int) - cast(ISNULL(loan_amt, 0) as int) + cast(ISNULL(pass_due_amt, 0) as int))
    from #bam085_dedup
    where account_code != 'Y'
    group by app_sn
 update #tf_ploan_cal
    set ms080 = a.v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn;
 update #tf_ploan_cal
    set ms080 = 0
    where ms080 is null;
go

/* Choose one from 4 score cards, full JCIC, BAM085_1(ms080 >0), BAM085_2 (ms080<=0),
    and demographic.*/
    
 /* Demographic model (No KRM001, KRM023 and BAM085)*/
/*
alter PROCEDURE TF_demographic_model
 AS
 SET NOCOUNT ON
 create table #tmp (
  app_sn char(11),
  mon int,
  v1 decimal (16, 8),
  v2 decimal (16, 8),
  v3 decimal (16, 8));
*/
 /* initialize variables */
    update #tf_ploan_cal
       set fs029 = 0
 /*****************************************************************************************/
 /* FS029  JCIC查詢次數 (Credit Card) in 3 month  Item_List in ('K')                     */
 /*****************************************************************************************/
 insert into #tmp (app_sn, v1)
    select app_sn, count(*)
    from #stm001_dedup
    where item_list is not null
      and item_list like '%K%'
      and now - query_mon_since <= 3
      and app_sn = b.app_sn
    group by app_sn
 update #tf_ploan_cal
    set fs029 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
 /* Transformation */
 update #tf_ploan_cal
    set fs029_tran0 =(case when fs029 < 1 then 1
			    when fs029 > 5 then 5
			    else fs029 end),
        single_0 = (case when marriage_status = 2 then 1
			    else 0 end),
	education_tran0 = (case when edu < 4 then 1 /* 專科以上 */
				else 0 end);
    
 update #tf_ploan_cal
    set demo_score =  0.05401	      +       
		      secretive	      * -0.08681 +
		      gender	      *  0.07774 +       
		      education_tran0 * -0.10880 +
     		      single_0        *  0.07549 +
          	      fs029_tran0     *  0.03419;
 update #tf_ploan_cal
    set demo_twentile = (case when demo_score <= 0.05714 then 1
                              when demo_score <= 0.08001 then 2
                              when demo_score <= 0.0882  then 4
                              when demo_score <= 0.13815 then 5
                              when demo_score <= 0.15462 then 6
                              when demo_score <= 0.16369 then 7
                              when demo_score <= 0.16594 then 11
                              when demo_score <= 0.19788 then 14
                              when demo_score <= 0.23207 then 15
                              when demo_score <= 0.2352  then 16
                              when demo_score <= 0.24143 then 17
                              when demo_score <= 0.26626 then 18
                              when demo_score <= 0.30045 then 19
                              else 20 end),
        score_card = 'C';
go
/*
alter PROCEDURE TF_BAM_no_payment
 AS
 SET NOCOUNT ON

 create table #tmp (
  app_sn char(11),
  mon int,
  v1 decimal (16, 8),
  v2 decimal (16, 8),
  v3 decimal (16, 8));
 create table #tmp1 (
  app_sn char(11),
  mon int,
  v1 decimal (16, 8));
*/
 /* initialize variables */
 update #tf_ploan_cal
    set fs031 = 0
 update  #tf_ploan_cal
    set fs536_3m = 0.0,
        fs314 = 0.0,
        ms082 = 0.0
    from #bam085_dedup a
    where a.app_sn = #tf_ploan_cal.app_sn
 /*****************************************************************************************/
 /* start making fs031 JCIC查詢次數 (All) (stm001)within 3 month                        */
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp (app_sn, v1)
    select app_sn, count(*)
    from #stm001_dedup
    where item_list is not null
      and item_list <> ''
      and now - query_mon_since <= 3
    group by app_sn
 update #tf_ploan_cal
    set fs031 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
 /*****************************************************************************************/
 /* FS536_3M 繳款紀錄在X月內的無擔保貸款總筆數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (app_sn, mon, v1)
    select app_sn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
    group by app_sn, (mon_since - cycle + 1)
 insert into #tmp1 (app_sn, mon, v1)
    select app_sn, 3, sum(v1)
    from #tmp a, #tf_ploan_cal b
    where a.mon < b.now
      and a.mon >= b.now - 3
      and a.app_sn = b.app_sn
    group by app_sn
 update #tf_ploan_cal
    set fs536_3m = a.v1
    from #tmp1 as a
    where a.app_sn = #tf_ploan_cal.app_sn
      and a.mon = 3
 /*****************************************************************************************/
 /* FS314 動用比例超過100%筆數                                                            */
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp(app_sn, v1)
    select app_sn, count(*)
    from #bam085_dedup
    where account_code = 'Y' and
          ((convert(float, isnull(loan_amt, 0)) + convert(float, isnull(pass_due_amt, 0))) / 
           (case when isnull(contract_amt, 0) = 0 then null else convert(float, contract_amt) end)) >= 1.00
    group by app_sn    
 update #tf_ploan_cal
    set fs314 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
 /*****************************************************************************************/
 /* MS082 貸款月付金    台新資融版, 以 contract_amount 為基準                             */
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp(app_sn, v1)
    select app_sn, 
       sum(case when account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) then convert(float, isnull(contract_amt, 0)) / 10.0 * 72 / 1000.0
          when account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) AND BANK_CODE2 IN ('809', '103', '815') then convert(float, isnull(contract_amt, 0)) / 10.0 * 158 / 1000.0
          when account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) then convert(float, isnull(contract_amt, 0)) / 10.0 * 342 / 1000.0
          when account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) then convert(float, isnull(contract_amt, 0)) / 10.0 * 342 / 1000.0
          when account_code = 'K' then convert(float, isnull(contract_amt, 0)) / 10.0 * 12 / 1000.0
          when account_code = 'C' and account_code2 in ('S', 'W', 'M') then convert(float, isnull(contract_amt, 0)) / 10.0 * 72 / 1000.0
          when account_code = 'E' and account_code2 in ('S', 'W', 'M') then convert(float, isnull(contract_amt, 0)) / 10.0 * 72 / 1000.0
          when account_code = 'H' and account_code2 in ('S', 'W', 'M') AND BANK_CODE2 IN ('001','052','016','056','057') then convert(float, isnull(contract_amt, 0)) / 10.0 * 342 / 1000.0
          when account_code = 'H' and account_code2 in ('S', 'W', 'M') then convert(float, isnull(contract_amt, 0)) / 10.0 * 193 / 1000.0
          when account_code = 'I' and account_code2 in ('S', 'W', 'M') then convert(float, isnull(contract_amt, 0)) / 10.0 * 72 / 1000.0
          when account_code = 'Z' then convert(float, isnull(contract_amt, 0)) / 10.0 * 233 / 1000.0
        end)
    from #bam085_dedup
    group by app_sn
 update #tf_ploan_cal
    set ms082 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn;
 update #tf_ploan_cal
    set amortization_rate = (case when apr = 0 then 1/periods
                                  else power ((1 + (apr / 12)), periods) * (apr / 12)
                                       / (power ((1 + (apr / 12)), periods) - 1) end);
 update #tf_ploan_cal
    set monthly_payment = principal * amortization_rate;
 update #tf_ploan_cal
    set mp = monthly_payment + ms082 * 1000.0;
 /* BAM085_2 score card  ms080 <= 0*/
 update #tf_ploan_cal
    set fs031_r =power ((case when fs031 < 0 then null else fs031 end), 0.5),
 	mp_r1 =power ((case when mp < 0 then null else mp end), 0.5);
 update #tf_ploan_cal
    set fs031_r_tran1 =(case when fs031_r is null then 0
			     when fs031_r > 3 then 3
			     else fs031_r end),
 	fs536_3M_tran1 =(case when fs536_3M > 0 then 1
			      else 0 end),
 	fs314_tran1 =(case when fs314 > 2 then 2
			   else fs314 end),
 	education_tran1 = (case when edu < 4 then 1 /* 專科以上 */
				else 0 end);  
 update #tf_ploan_cal
    set brmp_score = 0.03931 +
                     fs314_tran1      * 0.10852  +
                     fs031_r_tran1    * 0.0459   +
                     fs536_3m_tran1   * 0.10714  +
                     gender	      * 0.08149  +
                     education_tran1  * -0.11864 +
                     mp_r1            * 0.00066177;
 update #tf_ploan_cal
    set brmp_twentile = (case when brmp_score is null    then 0
                              when brmp_score <= 0.11128 then 1
                              when brmp_score <= 0.14783 then 2
                              when brmp_score <= 0.1667  then 3
                              when brmp_score <= 0.17928 then 4
                              when brmp_score <= 0.1983  then 5
                              when brmp_score <= 0.21391 then 6
                              when brmp_score <= 0.22595 then 7
                              when brmp_score <= 0.23512 then 8
                              when brmp_score <= 0.24838 then 9
                              when brmp_score <= 0.25812 then 10
                              when brmp_score <= 0.27329 then 11
                              when brmp_score <= 0.28318 then 12
                              when brmp_score <= 0.29733 then 13
                              when brmp_score <= 0.3292  then 14
                              when brmp_score <= 0.34902 then 15
                              when brmp_score <= 0.36492 then 16
                              when brmp_score <= 0.38854 then 17
                              when brmp_score <= 0.4233  then 18
                              when brmp_score <= 0.47779 then 19
                              else 20 end),
        score_card = 'B2';

 go   
/*
alter PROCEDURE TF_BAM_with_payment
 AS
 SET NOCOUNT ON
 
 create table #tmp (
  app_sn char(11),
  mon int,
  v1 decimal (16, 8),
  v2 decimal (16, 8),
  v3 decimal (16, 8));
 create table #tmp1 (
  app_sn char(11),
  mon int,
  v1 decimal (16, 8));
*/
 /* initialize variables */
 update #tf_ploan_cal
    set fs031 = 0
 update  #tf_ploan_cal
    set fs536_3m = 0.0,
        fs313 = 0.0,
        fs314 = 0.0,
        ms082 = 0.0
    from #bam085_dedup a
    where a.app_sn = #tf_ploan_cal.app_sn
 /*****************************************************************************************/
 /* start making fs031 JCIC查詢次數 (All) (stm001)within 3 month                        */
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp (app_sn, v1)
    select app_sn, count(*)
    from #stm001_dedup
    where item_list is not null
      and item_list <> ''
      and now - query_mon_since <= 3
    group by app_sn
 update #tf_ploan_cal
    set fs031 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
  /*****************************************************************************************/
 /* FS536_3M 繳款紀錄在X月內的無擔保貸款總筆數 - 短期 4 account_code2 is NULL account_code in ('W','C','D','E') */
 /*****************************************************************************************/
 delete from #tmp
 delete from #tmp1
 insert into #tmp (app_sn, mon, v1)
    select app_sn, (mon_since - cycle + 1), count(*)
    from #bam085_dedup
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
    group by app_sn, (mon_since - cycle + 1)
 insert into #tmp1 (app_sn, mon, v1)
    select app_sn, 3, sum(v1)
    from #tmp a, #tf_ploan_cal b
    where a.mon < b.now
      and a.mon >= b.now - 3
      and a.app_sn = b.app_sn
    group by app_sn
 update #tf_ploan_cal
    set fs536_3m = a.v1
    from #tmp1 as a
    where a.app_sn = #tf_ploan_cal.app_sn
      and a.mon = 3
 /*****************************************************************************************/
 /* FS313 動用比例超過96%筆數                                                             */
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp(app_sn, v1)
    select app_sn, count(*)
    from #bam085_dedup
    where account_code = 'Y' and
          ((convert(float, isnull(loan_amt, 0)) + convert(float, isnull(pass_due_amt, 0))) / 
           (case when isnull(contract_amt, 0) = 0 then null else convert(float, contract_amt) end)) >= 0.96
    group by app_sn  
 update #tf_ploan_cal
    set fs313 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
 update #tf_ploan_cal
    set fs031_r =power ((case when fs031 < 0 then null else fs031 end), 0.5);
 update #tf_ploan_cal
    set fs031_r_tran2 =(case when fs031_r is null then 0
       when fs031_r > 3 then 3
       else fs031_r end),
 fs536_3M_tran2 =(case when fs536_3M > 0 then 1
       else 0 end),
 fs313_tran2 =(case when fs313 > 0 then 1
       else 0 end);
 update #tf_ploan_cal
    set br80_score = 0.06407 +
                     fs031_r_tran2  * 0.08924 +
                     fs536_3M_tran2 * 0.15268 +
                     fs313_tran2    * 0.09274;
 update #tf_ploan_cal
    set br80_twentile = (case when br80_score is null    then 0
                              when br80_score <= 0.06407 then 1
                              when br80_score <= 0.15331 then 3
                              when br80_score <= 0.15681 then 4
                              when br80_score <= 0.19027 then 5
                              when br80_score <= 0.21864 then 6
                              when br80_score <= 0.24255 then 7
                              when br80_score <= 0.24605 then 8
                              when br80_score <= 0.28266 then 9
                              when br80_score <= 0.30599 then 10
                              when br80_score <= 0.31648 then 11
                              when br80_score <= 0.34295 then 12
                              when br80_score <= 0.37132 then 13
                              when br80_score <= 0.3754  then 14
                              when br80_score <= 0.39873 then 15
                              when br80_score <= 0.4163  then 16
                              when br80_score <= 0.43534 then 17
                              when br80_score <= 0.46916 then 18
                              when br80_score <= 0.50904 then 19
                              else 20 end),
        score_card = 'B1';

 go  
 
/*
alter PROCEDURE TF_ploan_model
 AS
 SET NOCOUNT ON
 declare @i int
 create table #tmp (
    app_sn char(11),
    mon int,
    v1 decimal (16, 8),
    v2 decimal (16, 8),
    v3 decimal (16, 8));
 create table #tmp1 (
    app_sn char(11),
    mon int,
    v1 decimal (16, 8));
*/    
 if exists (select * from dbo.sysobjects where id = object_id(N'#open_card') and objectproperty(id, N'isusertable') = 1)
   drop table #open_card;
 create table #open_card (
    app_sn varchar(14),
    issue char(3),
    mon int);
 if exists (select * from dbo.sysobjects where id = object_id(N'#open_line') and objectproperty(id, N'isusertable') = 1)
   drop table #open_line;
 create table #open_line (
    app_sn varchar(14),
    issue char(3),
    mon int,
    cards int,
    bucket int);
 if exists (select * from dbo.sysobjects where id = object_id(N'#latest_stmt_mon') and objectproperty(id, N'isusertable') = 1)
   drop table #latest_stmt_mon;
 create table #latest_stmt_mon (
    app_sn varchar(14),
    issue char(3),
    mon int);
 if exists (select * from dbo.sysobjects where id = object_id(N'#latest_line') and objectproperty(id, N'isusertable') = 1)
   drop table #latest_line;
 create table #latest_line (
    app_sn varchar(14),
    issue char(3),
    mon int,
    mob int);
 /* initialize variables */
 update #tf_ploan_cal
    set fs031 = 0
 update #tf_ploan_cal
    set fs310 = 0.0
    from #bam085_dedup a
    where a.app_sn = #tf_ploan_cal.app_sn
 update #tf_ploan_cal
    set fs059_3m_1k = 0.0,
        app_last_month_bucket = 0.0,
        ms024_3m = 0.0,
        ms056_6m_1k = 0.0,
        fs014_12m = 0.0,
        wi001_12m = 0.0
    from #krm023_dedup a
    where a.app_sn = #tf_ploan_cal.app_sn
 update #tf_ploan_cal
    set fs203_12m_1k = 0
    from #krm001_dedup as a, #krm023_dedup as b
    where app_sn = b.app_sn
      and a.app_sn = #tf_ploan_cal.app_sn
      and ((a.issue = b.issue) 
           or ((a.issue = '021' and a.card_brand = 'V') and (b.issue = 'CTV')) 
           or ((a.issue = '021' and a.card_brand = 'M') and (b.issue = 'CTM')) 
           or ((a.issue = '021' and a.card_brand = 'D') and (b.issue = 'CTD')) 
           or ((a.issue = 'A82' and a.card_brand = 'A') and (b.issue = 'AEA'))
          )

 /* Full JCIC (with KRM001, KRM023, and BAM085) */
 /*****************************************************************************************/
 /* start making fs031 JCIC查詢次數 (All) (stm001)within 3 month                        */
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp (app_sn, v1)
    select app_sn, count(*)
    from #stm001_dedup
    where item_list is not null
      and item_list <> ''
      and now - query_mon_since <= 3
    group by app_sn
 update #tf_ploan_cal
    set fs031 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
 /*****************************************************************************************/
 /* fs059_1k 有延遲繳款額度數 (M1+) - Pay code in ('E','F')  no "15 day" rule             */
 /*****************************************************************************************/
 delete from #tmp
 set @i = 3
 insert into #tmp (app_sn, mon, v1)
    select app_sn, 3, count(distinct issue)
    from #krm023_dedup
    where bucket_ef_1k >= 1
      and mon_since >= now - 3
      and mon_since <= now
    group by app_sn;
 update #tf_ploan_cal
    set fs059_3m_1k = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn and mon = 3;

 /*****************************************************************************************/
 /* app_last_month_bucket_tran last month max among all lines                            */
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp (app_sn, v1)
    select app_sn, max(bucket_ef_1k)
    from #krm023_dedup
    where mon_since >= now - 1  /*max bucket for last 2 months, no "15 day" rule*/
      and mon_since <= now
    group by app_sn;
 update #tf_ploan_cal
    set app_last_month_bucket = v1
    from #tmp a
    where a.app_sn = #tf_ploan_cal.app_sn
 
 /*****************************************************************************************/
 /* MS024  持有之信用卡平均額度  For line > 0 only        no "15 day" rule                */
 /*****************************************************************************************/
 delete from #tmp;
 insert into #tmp (app_sn, mon, v1)
    select app_sn, 3, avg(convert(decimal (16, 8),(case when limit='' then NULL else limit end)))
    from #krm023_dedup
    where mon_since >= now - 3
      and mon_since <= now       /* no "15 day" rule */   
      and convert(decimal (16, 8), (case when limit='' then NULL else limit end)) > 0
    group by app_sn;
 update #tf_ploan_cal
    set ms024_3m = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
      and mon=3;

 /*****************************************************************************************/
 /* FS203_1K # of delinqunt lines opened less than a year paycode DEF               */
 /*****************************************************************************************/
 /* ---Prepare intermediate tables for KRM023 and krm021 variables --- */
 set @i = 0
 while @i <= 13
    begin
       insert into #open_card (app_sn, issue, mon)
          select app_sn,
                 (case when card_brand = 'A' and issue = 'A82' then 'AEA'
                       else issue end),  /*for American Express*/
                 (now - @i)
          from #krm001_dedup
          where (end_mon_since > (now - @i))
            and (start_mon_since <= (now - @i))  /*add .eq.*/
            and issue != '021'
       set @i = @i + 1
    end;
 insert into #open_line (app_sn, issue, mon, cards)
    select app_sn, issue, mon, count(*)
    from #open_card
    group by app_sn, issue, mon;
 set @i = 0
 while @i <= 13
    begin
       insert into #open_line (app_sn, issue, mon, cards)
          select app_sn, (case when card_brand = 'M' then 'CTM'
                            when card_brand = 'V' then 'CTV'
                            when card_brand = 'D' then 'CTD' end), (now - @i), 1
          from #krm001_dedup
          where (end_mon_since > (now - @i))
            and (start_mon_since <= (now - @i))  /* add .eq.*/
            and issue = '021'
       set @i = @i + 1
    end;
 update #open_line
    set bucket = 1;
 update #open_line
    set bucket = 100
    from #tf_ploan_cal a
    where mon = (a.now - 13)
      and app_sn = #open_line.app_sn;
 set @i = 13;
 while @i > 0
    begin
       update #open_line
          set bucket = a.bucket + 1
          from #open_line, #open_line as a, #tf_ploan_cal as b
          where app_sn = #open_line.app_sn
            and a.issue = #open_line.issue
            and a.mon = (#open_line.mon - 1)
            and a.app_sn = b.app_sn
            and (now - a.mon) = @i
       set @i = @i - 1
    end;
 insert into #latest_stmt_mon (app_sn, issue, mon)
    select app_sn, issue, max(mon)
    from #open_line a, #tf_ploan_cal b
    where a.mon <= b.now  /* add .eq. */
      and a.app_sn = b.app_sn
    group by a.app_sn, a.issue;
 insert into #latest_line (app_sn, issue, mon, mob)
    select app_sn, a.issue, a.mon, a.bucket
    from #open_line as a inner join #latest_stmt_mon as b
      on a.app_sn = b.app_sn
     and a.issue = b.issue
     and a.mon = b.mon;
 /* Making FS203_12M_1k */
 delete from #tmp;
 set @i = 12;      
 insert into #tmp (app_sn, mon, v1)
    select app_sn, 12, count(*)
    from #krm023_dedup as a
    where issue in (select issue from #latest_line
           where mob <= 12 and app_sn = a.app_sn)
      and bucket_def_1k != 0
      and mon_since >= (now - 12)
      and mon_since <= now                  /*no "15 day" rule*/
    group by app_sn
 update #tf_ploan_cal
    set fs203_12m_1k = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn and
          a.mon = 12;
 /*****************************************************************************************/
 /* MS056 Max延遲繳款金額 (M1+) - Pay code in ('D','E','F') Sum up all payment of         */
 /* all delinquent lines by month 2. Pick the max in 3, 6, 9, 14 months                   */
 /*****************************************************************************************/
 delete from #tmp;
 delete from #tmp1;
 insert into #tmp (app_sn, mon, v1)
    select app_sn, mon_since, sum(payment_amt)
    from #krm023_dedup
    where pay_code in ('D', 'E', 'F')
      and bucket_def_1k >= 1
    group by app_sn, mon_since;
 insert into #tmp1 (app_sn, mon, v1)
    select app_sn, 6, max(v1)
    from #tmp a, #tf_ploan_cal b
    where a.mon >= (b.now - 6)
      and a.mon <= b.now  /*no "15 day" rule*/
      and a.app_sn = b.app_sn
    group by a.app_sn;  
 update #tf_ploan_cal
    set ms056_6m_1k = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn and mon=6;
 /*****************************************************************************************/
 /* FS014_12M 刷卡並付清額度數(lines) (Paycode A,B)                                       */
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp (app_sn, mon, v1)
    select app_sn, 12, count(distinct issue)
    from #krm023_dedup
    where mon_since >= (now - 12)
      and mon_since <= now    /*no "15 day" rule*/
      and pay_code in ('A', 'B')
    group by app_sn;
 update #tf_ploan_cal
    set fs014_12m = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn and mon=12;
 /*****************************************************************************************/
 /* FS310  現金卡utilization >= 98% 筆數 (Loan_amt + Pass_due_amt) / contract_amt >= 0.98, providing contract_amt <> 0*/
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp(app_sn, v1)
    select app_sn, count(*)
    from #bam085_dedup
    where account_code = 'Y'
      and ((convert(float, isnull(loan_amt, 0)) + convert(float, isnull(pass_due_amt, 0))) / 
           (case when isnull(contract_amt, 0) = 0 then null else convert(float, contract_amt) end)) >= 0.98
    group by app_sn  
  
 update #tf_ploan_cal
    set fs310 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn

 /*****************************************************************************************/
 /* MS082 貸款月付金    台新資融版, 以 contract_amount 為基準                             */
 /*****************************************************************************************/
 delete from #tmp
 insert into #tmp(app_sn, v1)
    select app_sn, 
       sum(case when account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) then convert(float, isnull(contract_amt, 0)) / 10.0 * 72 / 1000.0
          when account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) AND BANK_CODE2 IN ('809', '103', '815') then convert(float, isnull(contract_amt, 0)) / 10.0 * 158 / 1000.0
          when account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) then convert(float, isnull(contract_amt, 0)) / 10.0 * 342 / 1000.0
          when account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) then convert(float, isnull(contract_amt, 0)) / 10.0 * 342 / 1000.0
          when account_code = 'K' then convert(float, isnull(contract_amt, 0)) / 10.0 * 12 / 1000.0
          when account_code = 'C' and account_code2 in ('S', 'W', 'M') then convert(float, isnull(contract_amt, 0)) / 10.0 * 72 / 1000.0
          when account_code = 'E' and account_code2 in ('S', 'W', 'M') then convert(float, isnull(contract_amt, 0)) / 10.0 * 72 / 1000.0
          when account_code = 'H' and account_code2 in ('S', 'W', 'M') AND BANK_CODE2 IN ('001','052','016','056','057') then convert(float, isnull(contract_amt, 0)) / 10.0 * 342 / 1000.0
          when account_code = 'H' and account_code2 in ('S', 'W', 'M') then convert(float, isnull(contract_amt, 0)) / 10.0 * 193 / 1000.0
          when account_code = 'I' and account_code2 in ('S', 'W', 'M') then convert(float, isnull(contract_amt, 0)) / 10.0 * 72 / 1000.0
          when account_code = 'Z' then convert(float, isnull(contract_amt, 0)) / 10.0 * 233 / 1000.0
        end)
    from #bam085_dedup
    group by app_sn
 update #tf_ploan_cal
    set ms082 = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn;    
 /*****************************************************************************************/
 /* WI001_12M  WI001 每月持有之信用額度數 (All opened)                                 */
 /*****************************************************************************************/
 insert into #tmp (app_sn, v1)
    select app_sn, count(*)
    from #krm023_dedup
    where now - mon_since = 12
    group by app_sn
 update #tf_ploan_cal
    set wi001_12m = a.v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
 /*****************************************************************************************/
 /* LN001_12M  = (monthly_payment + ms082 * 1000) / wi001_12m  */
 /*****************************************************************************************/
 update #tf_ploan_cal
    set amortization_rate = (case when apr = 0 then (1 / periods)
                                  else power ((1 + (apr / 12 )), periods) * (apr / 12 )
                                       / (power ((1 + (apr / 12 )), periods) - 1) end);
 update #tf_ploan_cal
    set monthly_payment = principal * amortization_rate;
 update #tf_ploan_cal
    set LN001_12M = (monthly_payment + ms082 * 1000.0) / 
                    (case when WI001_12M = 0 then null else WI001_12M end);
 /*****************************************************************************************/
 /* loan_del_number_6m : it is calculated when initialized.                                                                   */
 /*****************************************************************************************/
 /*
 delete from #tmp
 insert into #tmp(app_sn, v1)
    select idn,
       isnull(loan1_interest_payment6,0) + isnull(loan2_interest_payment6,0) + isnull(loan3_interest_payment6,0)
     + isnull(loan1_interest_payment5,0) + isnull(loan2_interest_payment5,0) + isnull(loan3_interest_payment5,0)
     + isnull(loan1_interest_payment4,0) + isnull(loan2_interest_payment4,0) + isnull(loan3_interest_payment4,0)
     + isnull(loan1_interest_payment3,0) + isnull(loan2_interest_payment3,0) + isnull(loan3_interest_payment3,0)
     + isnull(loan1_interest_payment2,0) + isnull(loan2_interest_payment2,0) + isnull(loan3_interest_payment2,0)
     + isnull(loan1_interest_payment1,0) + isnull(loan2_interest_payment1,0) + isnull(loan3_interest_payment1,0)
    from ts
 update #tf_ploan_cal
    set loan_del_number_6m = v1
    from #tmp as a
    where a.app_sn = #tf_ploan_cal.app_sn
 */
 update #tf_ploan_cal
    set LN001_12m_r =power ((case when LN001_12m < 0 then null else LN001_12m end), 0.5),
        ms056_6m_1k_r =power ((case when ms056_6m_1k < 0 then null else ms056_6m_1k end), 0.5),
        ms024_3m_r =power ((case when ms024_3m < 0 then null else ms024_3m end), 0.5);       
 update #tf_ploan_cal
    set fs031_tran3 =(case when fs031 is null then -1
			   when fs031 > 12 then 12
      			   else fs031 end),
 	loan_del_number_6m_tran3 =(case when loan_del_number_6m is null then 0
			         	when loan_del_number_6m > 25 then 25
			         	else loan_del_number_6m end),
 	LN001_12m_r_tran3 =(case when LN001_12m_r is null then 150
		          	 when LN001_12m_r > 150 then 150
           		  	 else LN001_12m_r end),
 	fs310_tran3 =(case when fs310 is null then -1
                   	   when fs310 >= 3 then 4
     			   else fs310 end),
 	fs059_3m_1k_tran3 =(case when fs059_3m_1k is null then 0
           			 when fs059_3m_1k > 3 then 3
           			 else fs059_3m_1k end),
 	app_last_month_bucket_tran3 =(case when app_last_month_bucket > 0 then 1
                         		   else 0 end),
 	fs203_12m_1k_tran3 =(case when fs203_12m_1k is null then 0
            			  when fs203_12m_1k > 3 then 3
            			  else fs203_12m_1k end),
 	fs014_12m_tran3 =(case when fs014_12m is null then 4
		    	       when fs014_12m > 4 then 4
         		       else fs014_12m end),
 	ms056_6m_1k_r_tran3 =(case when ms056_6m_1k_r < 2 then 2
             			   when ms056_6m_1k_r > 20 then 20
             			   else ms056_6m_1k_r end),
 	ms024_3m_r_tran3 =(case when ms024_3m_r < 6.4 then 6.4
        	                when ms024_3m_r > 13.5 then 13.5
          			else ms024_3m_r end);  
 update #tf_ploan_cal
    set full_score = 	0.10695 +
			fs031_tran3		 * 0.01955	 +
			loan_del_number_6m_tran3 * 0.00542	 +
			gender    		 * 0.05442	 +
			LN001_12m_r_tran3	 * 0.00040341	 +
			fs310_tran3		 * 0.03331	 +
			fs059_3m_1k_tran3	 * 0.04101	 +
			app_last_month_bucket_tran3 * 0.1141	 +
			fs203_12m_1k_tran3	 * 0.04799	 +
			fs014_12m_tran3		 * -0.02282	 +
			ms056_6m_1k_r_tran3	 * 0.00595	 +
			ms024_3m_r_tran3	 * -0.01193;
 update #tf_ploan_cal
    set full_twentile = (case when full_score <= -0.00455 then 1
                              when full_score <= 0.03109  then 2
                              when full_score <= 0.05953  then 3
                              when full_score <= 0.08193  then 4
                              when full_score <= 0.09965  then 5
                              when full_score <= 0.11647  then 6
                              when full_score <= 0.13359  then 7
                              when full_score <= 0.15048  then 8
                              when full_score <= 0.16679  then 9
                              when full_score <= 0.18253  then 10
                              when full_score <= 0.19921  then 11
                              when full_score <= 0.21956  then 12
                              when full_score <= 0.24267  then 13
                              when full_score <= 0.26477  then 14
                              when full_score <= 0.29127  then 15
                              when full_score <= 0.3196   then 16
                              when full_score <= 0.3542   then 17
                              when full_score <= 0.40292  then 18
                              when full_score <= 0.4896   then 19
                              else 20 end),
        score_card = 'A2';
 drop table #open_card;
 drop table #open_line;
 drop table #latest_stmt_mon;
 drop table #latest_line;,
go



/* Assign PB*/

/* Calculate Economic values and lines*/






/******** main program */
/* create working tables */
 if exists (select * from dbo.sysobjects where id = object_id(N'#bam085_dedup') and objectproperty(id, N'isusertable') = 1)
    drop table #bam085_dedup;
 create table #bam085_dedup (
    app_sn char(11),
    data_time char(8),
    data_yyy char(3),
    data_mm char(2),
    bank_code char(7),
    bank_name char(40),
    account_code char(1),
    account_code2 char(1),
    purpose_code char(1),
    contract_amt decimal (10),
    loan_amt decimal (10),
    pass_due_amt decimal (10),
    pay_code_12 char(12),
    co_loan char(1),
    bank_code2 char(3),
    mon_since int,
    cycle int,
    cnt int
 );
 if exists (select * from dbo.sysobjects where id = object_id(N'#bam085_bucket') and objectproperty(id, N'isusertable') = 1)
    drop table #bam085_bucket;
  create table #bam085_bucket (
     app_sn char(11),
     data_time char(8),
     bank_code char(7),
     mon_since int,
     account_code char (1),
     account_code2 char (1),
     purpose_code char (1),
     contract_amt decimal (10),
     loan_amt decimal (10),
     pass_due_amt decimal (10),
     co_loan char (1),
     bucket    float
  )
 if exists (select * from dbo.sysobjects where id = object_id(N'#krm001_dedup') and objectproperty(id, N'isusertable') = 1)
    drop table #krm001_dedup;
 create table #krm001_dedup (
    app_sn char(11),
    data_time char(8),
    card_brand char(1),
    card_type char(1),
    issue char(3),
    issue_name char(40),
    start_date char(7),
    stop_date char(7),
    stop_code char(1),
    ab_code char(1),
    m_s char(1),
    limit char(6),
    limit_type char(1),
    usage_type char(1),
    secure char(1),
    start_mon_since int,
    end_mon_since int,
    cnt int
 );
 if exists (select * from dbo.sysobjects where id = object_id(N'#krm023_dedup') and objectproperty(id, N'isusertable') = 1)
    drop table #krm023_dedup;
 create table #krm023_dedup (
    app_sn char(11),
    data_time char(8),
    yrmon char(5),
    issue char(3),
    issue_name char(40),
    kr_code char(7),
    limit char(6),
    payment char(3),
    cash char(1),
    pay_code char(1),
    mon_since int,
    payment_amt float,
    bucket_def_1k int default 0,
    bucket_ef_1k int default 0,
    bucket_f_1k int default 0,
    cnt int
 );
 if exists (select * from dbo.sysobjects where id = object_id(N'#stm001_dedup') and objectproperty(id, N'isusertable') = 1)
    drop table #stm001_dedup;
 create table #stm001_dedup (
    app_sn char(11),
    data_time char(8),
    query_date char(7),
    bank_code char(7),
    bank_name char(40),
    item_list char(10),
    query_mon_since int,
    cnt int
 );
 if exists (select * from dbo.sysobjects where id = object_id(N'#jas002_t') and objectproperty(id, N'isusertable') = 1)
    drop table #jas002_t;
 create table #jas002_t (
    app_sn char(11),
    data_time char(8),
    reason char(1),
    date_happen char(7),
    mon_since int
 );
 if exists (select * from dbo.sysobjects where id = object_id(N'#jas002_t_dedup') and objectproperty(id, N'isusertable') = 1)
    drop table #jas002_t_dedup;
 create table #jas002_t_dedup (
    app_sn char(11),
    data_time char(8),
    reason char(1),
    date_happen char(7),
    mon_since int,
    cnt int 
 );
 if exists (select * from dbo.sysobjects where id = object_id(N'#tf_ploan_cal') and objectproperty(id, N'isusertable') = 1)
    drop table #tf_ploan_cal
  create table #tf_ploan_cal (
     app_sn char (11),
     app_date char(8),
     ts_date char(8),
     jcic_date char(8),
     now int,
     avail_flag int,
     ind001 int default 0,
     krm001_hit int default 0,
     krm023_hit int default 0,
     bam085_hit int default 0,
     jas002_defect int default 0,
     app_max_bucket int default 0,
     fs044 int default 0,
     fs334 int default 0,
     fs302 int default 0,
     ms080 int,
     apr decimal(16,8),
     periods int,
     principal int,
     /*demographic variables*/
     edu int,
     education_tran0 int,
     gender char(3),
     secretive int,
     marriage_status int,
     single_0 int,
     fs029 decimal(16,8),
     fs029_tran0 decimal(16,8),
     demo_score decimal(16,8),
     demo_twentile decimal(16,8),
     /*BAM085 variables*/
     fs031 decimal(16,8),
     fs031_r decimal(16,8),
     fs536_3m decimal(16,8),
     fs313 decimal(16,8),
     fs031_r_tran2 decimal(16,8),
     fs536_3m_tran2 decimal(16,8),
     fs313_tran2 decimal(16,8),
     fs314 decimal(16,8),
     ms082 decimal(16,8),
     amortization_rate decimal(16,8),
     monthly_payment decimal(16,8),
     mp decimal(16,8),
     fs314_tran1 decimal(16,8),
     fs031_r_tran1 decimal(16,8),
     fs536_3m_tran1 decimal(16,8),
     mp_r1 decimal(16,8),
     education_tran1 int,
     br80_score decimal(16,8),
     brmp_score decimal(16,8),
     br80_twentile int,
     brmp_twentile int,
     /*TF ploan model*/
     fs031_tran3 decimal(16,8),
     loan_del_number_6m decimal(16,8),
     loan_del_number_6m_tran3 decimal(16,8),
     WI001_12m decimal(16,8),
     LN001_12m decimal(16,8),
     LN001_12m_r decimal(16,8),
     LN001_12m_r_tran3 decimal(16,8),
     fs310 decimal(16,8),
     fs310_tran3 decimal(16,8),
     fs059_3m_1k decimal(16,8),
     fs059_3m_1k_tran3 decimal(16,8),
     app_last_month_bucket decimal(16,8),
     app_last_month_bucket_tran3 decimal(16,8),
     fs203_12m_1k decimal(16,8),
     fs203_12m_1k_tran3 decimal(16,8),
     fs014_12m decimal(16,8),
     fs014_12m_tran3 decimal(16,8),
     ms056_6m_1k decimal(16,8),
     ms056_6m_1k_r decimal(16,8),
     ms056_6m_1k_r_tran3 decimal(16,8),
     ms024_3m decimal(16,8),
     ms024_3m_r decimal(16,8),
     ms024_3m_r_tran3 decimal(16,8),
     full_score decimal(16,8),
     full_twentile int,
     score_card char(4),
     pb decimal(16,8),
     return_msg varchar(64)
  );
 if exists (select * from dbo.sysobjects where id = object_id(N'#tmp') and objectproperty(id, N'isusertable') = 1)
    drop table #tmp
  create table #tmp (
   app_sn char(11),
   mon int,
   v1 decimal (16, 8),
   v2 decimal (16, 8),
   v3 decimal (16, 8));
 if exists (select * from dbo.sysobjects where id = object_id(N'#tmp1') and objectproperty(id, N'isusertable') = 1)
    drop table #tmp1
  create table #tmp1 (
   app_sn char(11),
   mon int,
   v1 decimal (16, 8));

/* load application data */
/*
  insert into #tf_ploan_cal (app_sn, data_time, edu, gender, secretive, marriage_status, inquiry_date)
     select app_sn, data_time, edu, gender, secretive, marriage_status, :v2
     from app_info
     where app_sn = :v0 and data_time = :v1
*/
/* prepare jcic data */
   exec tf_prepare_jcic_data_all '', ''    /* @now 93/02 */

/* prescreen */
   exec tf_loan_prescreen 1118     /* @now 93/02 */

   select app_sn, jas002_defect, app_max_bucket, fs044, fs334, fs302, ind001, krm001_hit, krm023_hit, bam085_hit, ms080
/* select approciated model: demo, bam085_1,bam085_2, full jcic */
   
 declare record_cur CURSOR local scroll static for
    select app_sn, jas002_defect, app_max_bucket, fs044, fs334, fs302, ind001, krm001_hit, krm023_hit, bam085_hit, ms080
      from #tf_ploan_cal

 declare @app_sn char(11)
 declare @jas002_defect int
 declare @app_max_bucket int
 declare @fs044 int
 declare @fs334 int
 declare @fs302 int
 declare @ind001 int
 declare @krm001_hit int
 declare @krm023_hit int
 declare @bam085_hit int
 declare @ms080 int

 open record_cur
 fetch next from record_cur into @app_sn, @jas002_defect, @app_max_bucket, @fs044, @fs334, @fs302,
       @ind001, @krm001_hit, @krm023_hit, @bam085_hit, @ms080

 while (@@fetch_status = 0)
    begin
      if @jas002_defect > 0 
         update #tf_ploan_cal
           set return_msg = '103 - 有退票強停拒往授信異常等記錄'
           WHERE CURRENT OF record_cur
      else if @app_max_bucket > 3
               update #tf_ploan_cal
                 set return_msg = '104 - 信用卡有90 天以上遲繳記錄'
                 WHERE CURRENT OF record_cur
           else if @fs044 > 0
                   update #tf_ploan_cal
                     set return_msg = '105 - 貸款有遲繳記錄'
                     WHERE CURRENT OF record_cur
                else if @fs334 > 0
                        update #tf_ploan_cal
                          set return_msg = '106 - 貸款有90 天以上遲繳記錄'
                          WHERE CURRENT OF record_cur
                     else if @fs302 > 0
                          update #tf_ploan_cal
                            set return_msg = '107 - 現金卡使用超出額度'
                            WHERE CURRENT OF record_cur
      if @krm001_hit = 1 and @krm023_hit = 1 and @ind001 = 0 
         exec TF_ploan_model 1118     /* @now 93/02 */
      else if @bam085_hit = 1 and @ms080 <= 0
              exec TF_BAM_no_payment 1118     /* @now 93/02 */
           else if @bam085_hit = 1 and @ms080 > 0
                   exec TF_BAM_with_payment 1118     /* @now 93/02 */
                else
                   exec TF_demographic_model 1118     /* @now 93/02 */
                   
      fetch next from record_cur into @app_sn, @jas002_defect, @app_max_bucket, @fs044, @fs334, @fs302,
            @ind001, @krm001_hit, @krm023_hit, @bam085_hit, @ms080
    end
 close record_cur
 deallocate record_cur

 update #tf_ploan_cal
    set return_msg = (case when jas002_defect > 0 then '103 - 有退票強停拒往授信異常等記錄'
                           when app_max_bucket > 3 then '104 - 信用卡有90 天以上遲繳記錄'
                           when fs044 > 0 then '105 - 貸款有遲繳記錄'
                           when fs334 > 0 then '106 - 貸款有90 天以上遲繳記錄'
                           when fs302 > 0 then '107 - 現金卡使用超出額度'
                      end)                            
 update #tf_ploan_cal
    set score_card = 'NA'
    where return_msg is not null                            

 exec TF_ploan_model
 exec TF_BAM_no_payment
 exec TF_BAM_with_payment
 exec TF_demographic_model
/*
  insert into #tf_ploan_cal (app_sn, data_time, edu, gender, secretive, marriage_status, inquiry_date)
     select app_sn, data_time, edu, gender, secretive, marriage_status, :v2
     from app_info
     where app_sn = :v0 and data_time = :v1
*/
  update #tf_ploan_cal
     set now = (case when substring(inquiry_date, 7, 2) > '15' then
     				(convert (int, substring(inquiry_date, 1, 4)) - 1911) * 12
               			+ convert (int, substring(inquiry_date, 5, 2))
               	     when substring(inquiry_date, 7, 2) <= '15' then
     				(convert (int, substring(inquiry_date, 1, 4)) - 1911) * 12
               			+ convert (int, substring(inquiry_date, 5, 2)) - 1
               	end)
  update #tf_ploan_cal
     set loan_del_number_6m = 
     (select isnull(a.loan1_interest_payment6,0) + isnull(a.loan2_interest_payment6,0) + isnull(a.loan3_interest_payment6,0)
     + isnull(a.loan1_interest_payment5,0) + isnull(a.loan2_interest_payment5,0) + isnull(a.loan3_interest_payment5,0)
     + isnull(a.loan1_interest_payment4,0) + isnull(a.loan2_interest_payment4,0) + isnull(a.loan3_interest_payment4,0)
     + isnull(a.loan1_interest_payment3,0) + isnull(a.loan2_interest_payment3,0) + isnull(a.loan3_interest_payment3,0)
     + isnull(a.loan1_interest_payment2,0) + isnull(a.loan2_interest_payment2,0) + isnull(a.loan3_interest_payment2,0)
     + isnull(a.loan1_interest_payment1,0) + isnull(a.loan2_interest_payment1,0) + isnull(a.loan3_interest_payment1,0)
    from ts a
    where app_sn = :v0
      and data_time = :v1)
  update #tf_ploan_cal
     set apr = a.apr,
         periods = a.terms,
         principal= a.loan_amount
     from loan_condition a
     where #tf_ploan_cal.app_sn = a.app_sn


/*Write_Prescreen_Result*/
 insert into prescreen(app_sn, app_date, jcic_date, product_type, code, reason)
  values (:v0, :v1, :v2, :v3, :v4, :v5);



 if exists (select * from dbo.sysobjects where id = object_id(N'dac_audit') and objectproperty(id, N'isusertable') = 1)
    drop table dac_audit
  create table dac_audit (
     app_sn char (11),
     app_date char(8),
     ts_date char(8),
     jcic_date char(8),
     now int,
     avail_flag int,
     ind001 int default 0,
     krm001_hit int default 0,
     krm023_hit int default 0,
     bam085_hit int default 0,
     jas002_defect int default 0,
     app_max_bucket int default 0,
     fs044 int default 0,
     fs334 int default 0,
     fs302 int default 0,
     ms080 int,
     apr decimal(16,8),
     periods int,
     principal int,
     /*demographic variables*/
     edu int,
     education_tran0 int,
     gender char(3),
     secretive int,
     marriage_status int,
     single_0 int,
     fs029 decimal(16,8),
     fs029_tran0 decimal(16,8),
     demo_score decimal(16,8),
     demo_twentile decimal(16,8),
     /*BAM085 variables*/
     fs031 decimal(16,8),
     fs031_r decimal(16,8),
     fs536_3m decimal(16,8),
     fs313 decimal(16,8),
     fs031_r_tran2 decimal(16,8),
     fs536_3m_tran2 decimal(16,8),
     fs313_tran2 decimal(16,8),
     fs314 decimal(16,8),
     ms082 decimal(16,8),
     amortization_rate decimal(16,8),
     monthly_payment decimal(16,8),
     mp decimal(16,8),
     fs314_tran1 decimal(16,8),
     fs031_r_tran1 decimal(16,8),
     fs536_3m_tran1 decimal(16,8),
     mp_r1 decimal(16,8),
     education_tran1 int,
     br80_score decimal(16,8),
     brmp_score decimal(16,8),
     br80_twentile int,
     brmp_twentile int,
     /*TF ploan model*/
     fs031_tran3 decimal(16,8),
     loan_del_number_6m decimal(16,8),
     loan_del_number_6m_tran3 decimal(16,8),
     WI001_12m decimal(16,8),
     LN001_12m decimal(16,8),
     LN001_12m_r decimal(16,8),
     LN001_12m_r_tran3 decimal(16,8),
     fs310 decimal(16,8),
     fs310_tran3 decimal(16,8),
     fs059_3m_1k decimal(16,8),
     fs059_3m_1k_tran3 decimal(16,8),
     app_last_month_bucket decimal(16,8),
     app_last_month_bucket_tran3 decimal(16,8),
     fs203_12m_1k decimal(16,8),
     fs203_12m_1k_tran3 decimal(16,8),
     fs014_12m decimal(16,8),
     fs014_12m_tran3 decimal(16,8),
     ms056_6m_1k decimal(16,8),
     ms056_6m_1k_r decimal(16,8),
     ms056_6m_1k_r_tran3 decimal(16,8),
     ms024_3m decimal(16,8),
     ms024_3m_r decimal(16,8),
     ms024_3m_r_tran3 decimal(16,8),
     full_score decimal(16,8),
     full_twentile int,
     score_card char(4),
     pb decimal(16,8),
     return_msg varchar(64)
  );
 insert into dac_audit(app_sn, app_date, ts_date, jcic_date, now, avail_flag, ind001, krm001_hit,
       krm023_hit, bam085_hit, jas002_defect, app_max_bucket, fs044, fs334, fs302, ms080, apr, 
       periods, principal,edu, education_tran0, gender, secretive,
       marriage_status, single_0, fs029, fs029_tran0, demo_score, demo_twentile, fs031, fs031_r,
       fs536_3m, fs313, fs031_r_tran2, fs536_3m_tran2, fs313_tran2, fs314, ms082,
       amortization_rate, monthly_payment, mp, fs314_tran1, fs031_r_tran1, fs536_3m_tran1, mp_r1,
       education_tran1, br80_score, brmp_score, br80_twentile, brmp_twentile,
       fs031_tran3, a.loan_del_number_6m, a.loan_del_number_6m_tran3, WI001_12m, LN001_12m, 
       LN001_12m_r, LN001_12m_r_tran3, fs310, fs310_tran3, fs059_3m_1k, 
       fs059_3m_1k_tran3, app_last_month_bucket, app_last_month_bucket_tran3, fs203_12m_1k, 
       fs203_12m_1k_tran3, fs014_12m, fs014_12m_tran3, ms056_6m_1k, ms056_6m_1k_r, 
       ms056_6m_1k_r_tran3, ms024_3m, ms024_3m_r, ms024_3m_r_tran3, full_score, full_twentile, 
       score_card, pb, return_msg)
   select app_sn, app_date, ts_date, jcic_date, now, avail_flag, ind001, krm001_hit,
       krm023_hit, bam085_hit, jas002_defect, app_max_bucket, fs044, fs334, fs302, ms080, apr, 
       periods, principal,edu, education_tran0, gender, secretive,
       marriage_status, single_0, fs029, fs029_tran0, demo_score, demo_twentile, fs031, fs031_r,
       fs536_3m, fs313, fs031_r_tran2, fs536_3m_tran2, fs313_tran2, fs314, ms082,
       amortization_rate, monthly_payment, mp, fs314_tran1, fs031_r_tran1, fs536_3m_tran1, mp_r1,
       education_tran1, br80_score, brmp_score, br80_twentile, brmp_twentile,
       fs031_tran3, a.loan_del_number_6m, a.loan_del_number_6m_tran3, WI001_12m, LN001_12m, 
       LN001_12m_r, LN001_12m_r_tran3, fs310, fs310_tran3, fs059_3m_1k, 
       fs059_3m_1k_tran3, app_last_month_bucket, app_last_month_bucket_tran3, fs203_12m_1k, 
       fs203_12m_1k_tran3, fs014_12m, fs014_12m_tran3, ms056_6m_1k, ms056_6m_1k_r, 
       ms056_6m_1k_r_tran3, ms024_3m, ms024_3m_r, ms024_3m_r_tran3, full_score, full_twentile, 
       score_card, pb, return_msg
   from #tf_ploan_cal
