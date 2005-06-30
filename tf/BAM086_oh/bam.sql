use oliver
go

 create procedure prepare_jcic_data
    (@case_sn varchar(20),
     @idn varchar(14),
     @dac_sn int,
     @now int)
 as

 insert into #bam085_dedup (idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, cnt)
    select idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, count(*)
    from bam085
    where idn = @idn
    group by idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan;

 update #bam085_dedup
    set bank_code2 = left(bank_code,3);

/* -- end of procedure prepare_jcic_data --*/
go

/* batch process - prepare jcic data */
if exists (select * from dbo.sysobjects where id = object_id(n'[prepare_jcic_data_all]') and objectproperty(id, n'isprocedure') = 1)
drop procedure prepare_jcic_data_all;
go

create procedure prepare_jcic_data_all
 (@now int)
 as

 insert into #bam085_dedup (idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, cnt)
    select idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan, count(*)
    from bam085
    group by idn, data_yyy, data_mm, bank_code, bank_name, account_code, account_code2, purpose_code, contract_amt, loan_amt, pass_due_amt, pay_code_12, co_loan;


create index i_bam085 on bam085_dedup(idn);
 update #bam085_dedup
    set bank_code2 = left(bank_code,3);

/* -- end of procedure prepare_jcic_data_all --*/
go


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

 /*****************************************************************************************
  *   Initialize #daco_v41_cal table
  *****************************************************************************************/
 update #bam_var
    set fs033 = 0,
        fs034 = 0,
        fs035 = 0,
        fs036 = 0,
        fs037 = 0,
        fs038 = 0,
        fs038 = 0,
        fs040 = 0,
        fs041 = 0,
        fs042 = 0,
        fs043 = 0,
        fs044 = 0,
        fs045 = 0,
        fs046 = 0,
        fs047 = 0,
        fs048 = 0,
        fs049 = 0,
        fs050 = 0,
        fs051 = 0,
        fs052 = 0,
        fs053 = 0,
        fs054 = 0,
        fs055 = 0,
        ms031 = 0,
        ms032 = 0,
        ms033 = 0,
        ms034 = 0,
        ms035 = 0,
        ms036 = 0,
        ms037 = 0,
        ms038 = 0,
        ms039 = 0,
        ms040 = 0,
from #bam085_dedup as a
    where a.idn = #dbam_var.idn;

 /*****************************************************************************************
  * start making FS033	總放款筆數	1	# records	
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    group by idn  
  
 update #bam_var
    set fs033 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS034	總放款銀行數	1	# distinct issuer (3 digit)	
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1) 
    select idn, count(distinct bank_code2)
    from #bam085_dedup
    group by idn  
  
 update #bam_var
    set  fs034 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS035	還清筆數	1	loan_amt = 0 and pass_due_amt = 0	
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1) 
    select idn, count(*)
    from #bam085_dedup
    where loan_amt = 0
      and pass_due_amt = 0
    group by idn  
  
 update #bam_var
    set  fs035 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS036	有擔保貸款筆數	1	account_code2 in ('S','W') 	
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code2 in ('S','W')
    group by idn  
  
 update #bam_var
    set fs036 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS037	有擔保貸款筆數 - 短期	1	account_code2 in ('S','W') 	account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code in ('W','C','D','E')
      and account_code2 in ('S','W')
    group by idn  
     
 update #bam_var
    set fs037 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS038	有擔保貸款筆數 - 中期	1	account_code2 in ('S','W') 	account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code in ('H','S')
      and account_code2 in ('S','W')
    group by idn  
     
 update #bam_var
    set fs038 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS039	有擔保貸款筆數 - 長期	1	account_code2 in ('S','W') 	account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code in ('I','T')
      and account_code2 in ('S','W')
    group by idn  
     
 update #bam_var
    set fs039 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS040	無擔保貸款筆數	1	account_code2 is NULL
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code2=''
       or account_code2 is null
    group by idn  
     
 update #bam_var
    set fs040 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS041	無擔保貸款筆數 - 短期	1	account_code2 is NULL	account_code in ('W','C','D','E')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code in ('W','C', 'D', 'E')
      and (account_code2 ='' or account_code2 is null)
    group by idn  
     
 update #bam_var
    set fs041 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS042	無擔保貸款筆數 - 中期	1	account_code2 is NULL	account_code in ('H','S')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code in ('H','S')
      and (account_code2 ='' or account_code2 is null)
    group by idn  
     
 update #bam_var
    set fs042 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS043	無擔保貸款筆數 - 長期	1	account_code2 is NULL	account_code in ('I','T')
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code in ('I','T') and
          (account_code2 ='' or account_code2 is null)
    group by idn  
     
 update #bam_var
    set fs043 = v1
    from #tmp as a
    where a.idn = #bam_var.idn 
 /*****************************************************************************************
  * start making FS044	逾期筆數	1	pass_due_amt > 0
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where pass_due_amt > 0
    group by idn  
     
 update #bam_var
    set fs044 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS045	催收筆數	1	account_code  = 'A'
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code = 'A'
    group by idn  
     
 update #bam_var
    set fs045 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
  /*****************************************************************************************
  * start making FS046	呆帳筆數	1	account_code  = 'B'
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code = 'B'
    group by idn  
     
 update #bam_var
    set
    fs046 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS047	財政部優惠貸款筆數	1	account_code2 in ('V','W') 
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code2 in ('V','W')
    group by idn  
     
 update #bam_var
    set fs047 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS048	不動產貸款筆數	1	purpose_code = 1
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where purpose_code = '1'
    group by idn  
     
 update #bam_var
    set fs048 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS049	動產貸款筆數	1	purpose_code = 2
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where purpose_code = '2'
    group by idn  
     
 update #bam_var
    set fs049 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS050	企業投資貸款筆數	1	purpose_code = 3
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where purpose_code = '3'
    group by idn  
     
 update #bam_var
    set fs050 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS051	週轉金貸款筆數	1	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where purpose_code = '4'
    group by idn  
     
 update #bam_var
    set fs051 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS052	助學貸款筆數 (無擔保)	1	account_code2 is NULL	account_code in ('Z')	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code='Z'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
    group by idn  
     
 update #bam_var
    set fs052 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS053	存保單質押筆數 (無擔保)	1	account_code2 is NULL	account_code in ('K')	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code='K'
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
    group by idn  
     
 update #bam_var
    set fs053 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making FS054	類現金卡筆數 (無擔保)	1	account_code2 is NULL	account_code in ('C','E','H')	purpose_code = 4
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where account_code in ('C','E','H')
      and (account_code2 =''  or account_code2 is null)
      and purpose_code = '4'
    group by idn  
     
 update #bam_var
    set fs054 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 
 /*****************************************************************************************
  * start making FS055	Co-Loan 筆數	1	Co-Loan = 'Y'
  *****************************************************************************************/
 delete from #tmp
 insert into #tmp (idn, v1)
    select idn, count(*)
    from #bam085_dedup
    where co_loan = 'Y'
    group by idn  
     
 update #bam_var
    set fs055 = v1
    from #tmp as a
    where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making MS031	授信總餘額		Pass_due_amt = 0 only;
  *****************************************************************************************/
 /*----ms031----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms031---*/
 insert into #tmp
 select idn, @i, sum(convert(int,convert(int, loan_amt)) + convert(int, convert(int, pass_due_amt))), null, null
 from #bam085_dedup
 group by idn  
  
 update #bam_var
 set ms031 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms032----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms032---*/
 insert into #tmp
 select idn, @i, sum(convert(int,contract_amt)), null, null
 from #bam085_dedup
 group by idn  
  
 update #bam_var
 set
 ms032 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms033----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms033---*/
 insert into #tmp
 select idn, @i, max(convert(int,contract_amt)), null, null
 from #bam085_dedup
 group by idn  
  
 update #bam_var
 set
 ms033 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms034----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms034---*/
 insert into #tmp
 select idn, @i, sum(convert(int,contract_amt)), null, null
 from #bam085_dedup
 where (convert(int, loan_amt) = 0) and
       (convert(int, pass_due_amt) = 0)
 group by idn  
  
 update #bam_var
 set
 ms034 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms035----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms035---*/
 insert into #tmp
 select idn, @i, sum(convert(int,contract_amt)), null, null
 from #bam085_dedup
 where account_code2 in ('S','W')
 group by idn  
  
 update #bam_var
 set
 ms035 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms036----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms036---*/
 insert into #tmp
 select idn, @i, sum(convert(int,contract_amt)), null, null
 from #bam085_dedup
 where account_code in ('W','C','D','E') and
       account_code2 in ('S','W')
 group by idn  
  
 update #bam_var
 set
 ms036 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms037----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms037---*/
 insert into #tmp
 select idn, @i, sum(convert(int,contract_amt)), null, null
 from #bam085_dedup
 where account_code in ('H','S') and
       account_code2 in ('S','W')
 group by idn  
  
 update #bam_var
 set
 ms037 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms038----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms038---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code in ('I','T') and
       account_code2 in ('S','W')
 group by idn  
  
 update #bam_var
 set
 ms038 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms039----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms039---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where (account_code2='' or account_code2 is null)
 group by idn  
  
 update #bam_var
 set
 ms039 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms040----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms040---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code in ('W','C', 'D', 'E') and
       (account_code2 ='' or account_code2 is null)
 group by idn  
  
 update #bam_var
 set
 ms040 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms041----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms041---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code in ('H','S') and
       (account_code2 ='' or account_code2 is null)
 group by idn  
  
 update #bam_var
 set
 ms041 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms042----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms042---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code in ('I','T') and
       (account_code2 ='' or account_code2 is null)
 group by idn  
  
 update #bam_var
 set
 ms042 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms043----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms043---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where convert(int, pass_due_amt) > 0
 group by idn  
  
 update #bam_var
 set
 ms043 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms044----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms044---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code = 'A'
 group by idn  
  
 update #bam_var
 set
 ms044 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms045----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms045---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code = 'B'
 group by idn  
  
 update #bam_var
 set
 ms045 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms046----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms046---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code2 in ('V','W')
 group by idn  
  
 update #bam_var
 set
 ms046 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms047----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms047---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where purpose_code = '1'
 group by idn  
  
 update #bam_var
 set
 ms047 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms048----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms048---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where purpose_code = '2'
 group by idn  
  
 update #bam_var
 set
 ms048 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms049----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms049---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where purpose_code = '3'
 group by idn  
  
 update #bam_var
 set
 ms049 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms050----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms050---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where purpose_code = '4'
 group by idn  
  
 update #bam_var
 set
 ms050 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms051----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms051---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code='Z' and
       (account_code2 =''  or account_code2 is null) and
       purpose_code = '4'
 group by idn  
  
 update #bam_var
 set
 ms051 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms052----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms052---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code='K' and
       (account_code2 =''  or account_code2 is null) and
       purpose_code = '4'
 group by idn  
  
 update #bam_var
 set
 ms052 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms053----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms053---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where account_code in ('C','E','H') and
       (account_code2 =''  or account_code2 is null) and
       purpose_code = '4'
 group by idn  
  
 update #bam_var
 set
 ms053 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms054----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms054---*/
 insert into #tmp
 select idn, @i, sum(convert(int, contract_amt)), null, null
 from #bam085_dedup
 where co_loan = 'Y'
 group by idn  
  
 update #bam_var
 set
 ms054 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms055----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms055---*/
 insert into #tmp
 select idn, @i, max(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 group by idn  
  
 update #bam_var
 set
 ms055 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms059----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms059---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where account_code2 in ('S', 'W')
 group by idn  
  
 update #bam_var
 set
 ms059 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms060----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms060---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where   account_code2 in ('S', 'W') and
         account_code in ('W','C','D','E')
 group by idn  
  
 update #bam_var
 set
 ms060 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms061----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms061---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where   account_code2 in ('S', 'W') and
         account_code in ('H','S')
 group by idn  
  
 update #bam_var
 set
 ms061 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms062----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms062---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where   account_code2 in ('S', 'W') and
         account_code in ('I','T')
 group by idn  
  
 update #bam_var
 set
 ms062 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms063----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms063---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where   (account_code2 = '' or account_code2 is null)
 group by idn  
  
 update #bam_var
 set
 ms063 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms064----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms064---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where   (account_code2 = '' or account_code2 is null) and
         account_code in ('W','C', 'D', 'E')
 group by idn  
  
 update #bam_var
 set
 ms064 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms065----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms065---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where   (account_code2 = '' or account_code2 is null) and
         account_code in ('H','S')
 group by idn  
  
 update #bam_var
 set
 ms065 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms066----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms066---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where   (account_code2 = '' or account_code2 is null) and
         account_code in ('I','T')
 group by idn  
  
 update #bam_var
 set
 ms066 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms067----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms067---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where convert(int, pass_due_amt) > 0
 group by idn  
  
 update #bam_var
 set
 ms067 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms068----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms068---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where   account_code = 'A'
 group by idn  
  
 update #bam_var
 set
 ms068 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms069----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms069---*/
 insert into #tmp
 select idn, @i, sum(convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where   account_code = 'B'
 group by idn  
  
 update #bam_var
 set
 ms069 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms070----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms070---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where account_code2 in ('V','W')
 group by idn  
  
 update #bam_var
 set
 ms070 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms071----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms071---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where purpose_code = '1'
 group by idn  
  
 update #bam_var
 set
 ms071 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms072----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms072---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where purpose_code = '2'
 group by idn  
  
 update #bam_var
 set
 ms072 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms073----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms073---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where purpose_code = '3'
 group by idn  
  
 update #bam_var
 set
 ms073 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms074----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms074---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where purpose_code = '4'
 group by idn  
  
 update #bam_var
 set
 ms074 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms075----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms075---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where (account_code2 = '' or account_code2 is null) and
       (account_code = 'Z') and
       (purpose_code ='4')
 group by idn  
  
 update #bam_var
 set
 ms075 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms076----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms076---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where (account_code2 = '' or account_code2 is null) and
       (account_code = 'K') and
       (purpose_code ='4')
 group by idn  
  
 update #bam_var
 set
 ms076 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms077----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms077---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where (account_code2 = '' or account_code2 is null) and
       (account_code in ('C', 'E', 'H')) and
       (purpose_code ='4')
 group by idn  
  
 update #bam_var
 set
 ms077 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
 
 
 /*****************************************************************************************
  * start making 
  *****************************************************************************************/
 /*----ms078----*/
 /*---init temp table----*/
 declare @now int
 declare @i int
 set @now = 92 * 12 + 5
 delete from #tmp
 delete from #tmp1
 set @i = 3
 /*---start making ms078---*/
 insert into #tmp
 select idn, @i, sum(convert(int, loan_amt) + convert(int, pass_due_amt)), null, null
 from #bam085_dedup
 where (co_loan = 'Y')
 group by idn  
  
 update #bam_var
 set
 ms078 = v1
 from #tmp as a
 where a.idn = #bam_var.idn
 go
