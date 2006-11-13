Dear Yangkai,
 
Below is the precise definition for BOOC's module changes.  Attached excel is the control log.
 
3----card debt >500k
create view last_month as
select idn, issue, max(YRMON) as last_month
from dbo.krm023
where YRMON in ('9409','9410','9411')
group by idn, issue
go
 
select a.*
into ccb
from dbo.KRM023 as a, last_month as b
where a.idn=b.idn and a.issue=b.issue and a.YRMON=b.last_month
 
credit card debt = sum(payment_amt) where pay_code in (C,D,E,F) from ccb
cash card debt = sum(loan_amount + pass_due_amt) where account_code='Y' from BAM087
if credit card debt + cash card debt >=500000, then decline
 
5------manual review : credit card revovling ration>75%
create view last_month as
select idn, issue, max(YRMON) as last_month
from dbo.krm023
where YRMON in ('9409','9410','9411')
group by idn, issue
go
 
select a.*
into ccb
from dbo.KRM023 as a, last_month as b
where a.idn=b.idn and a.issue=b.issue and a.YRMON=b.last_month
 
drop view temp1
go
create view temp1 as
select idn, sum(convert(float,limit)) as ttl_line
from ccb
group by idn
go
 

drop view temp2
go
create view temp2 as
select idn, sum(payment_amt) as ttl_rev_os
from ccb
where pay_code in ('C','D','E','F')
group by idn
go
credit card revovling ration>75% is TRUE if ttl_rev_os/(ttl_line*1000)>0.75
 
6------new manual review criteria: open_line<=2 and ever EF
number of line :
    source : from KRM021
    count open major lines (where stop_date ='' and M_S='Y'), if ISSUE in ('021','974','081') then line count=KRM021 record count, else then each distinct issuer         count only once
 
ever EF:
    source : from KRM023
    definition : ever E or F on KRM023
 
7--------max optimal line cap
highest current open line :
select idn, max(LIMIT_NUM) as max_open_line
from dbo.KRM021
where stop_date ='' and M_S='Y'
group by idn
 
highest closed line :
select idn, max(LIMIT_NUM) as max_closed_line
from dbo.KRM021
where stop_date <>'' and M_S='Y'
group by idn
 
line_cap=max_open_line, but if max_open_line=0 or NULL, then use max_closed_line
 
module output line = min(original_module_optimal_line, line_cap*1.5, line_cap+500000)
 
11----admend cash card prescreen criteria : cash card utilizaton >=95%
create view cc1 as
select idn
from dbo.BAM087
where account_code='Y'
 and contract_amt is not null
 and contract_amt <>0
 and (PASS_DUE_AMT+LOAN_AMT)/convert(float,CONTRACT_AMT)>=0.95
go
 

create view cc2 as
select idn
from dbo.BAM087
where account_code='Y'
 and contract_amt1 is not null
 and contract_amt1 <>0
 and (PASS_DUE_AMT+LOAN_AMT)/convert(float,CONTRACT_AMT1)>=0.95
go
 
if idn in cc1 or cc2
 
 