drop table fw_ms
go

select idn into fw_ms
from idn_all
go

alter table fw_ms
add
ms093 float,
ms094 float,
ms105 float
go

--MS093	貸款月付金		DAC版, 以 contract_amount 為基準
DELETE FROM MS_TMP

INSERT INTO MS_TMP
SELECT IDN, 
       SUM(CASE 
          WHEN account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10* 150 / 1000
          WHEN account_code = 'C' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 100 / 1000
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 150 / 1000
          WHEN account_code = 'E' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 100 / 1000          
          WHEN account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 238 / 1000
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 177 / 1000
          WHEN account_code = 'I' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 127 / 1000
          WHEN account_code = 'I' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 72 / 1000
          WHEN account_code = 'O' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 177 / 1000
          WHEN account_code = 'O' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 238 / 1000
          WHEN account_code = 'K' THEN (contract_amt) / 10 * 17 / 1000
          WHEN account_code = 'Z' THEN (contract_amt) / 10 * 213 / 1000
        END)
FROM FW_PLOAN.dbo.BAM086_200305_DEDUP
GROUP BY IDN

UPDATE fw_ms
SET
MS093 = V1
FROM MS_TMP AS A
WHERE A.IDN = fw_ms.IDN
GO

update fw_ms
set ms093=0
where ms093 is null
go

select ms093, count(*)
from fw_ms
group by ms093
order by ms093
go

DELETE FROM MS_TMP

INSERT INTO MS_TMP
SELECT IDN, 
       SUM(CASE 
          WHEN account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10* 150 / 1000
          WHEN account_code = 'C' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 100 / 1000
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 150 / 1000
          WHEN account_code = 'E' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 100 / 1000          
          WHEN account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 238 / 1000
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 177 / 1000
          WHEN account_code = 'I' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 127 / 1000
          WHEN account_code = 'I' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 72 / 1000
          WHEN account_code = 'O' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10 * 177 / 1000
          WHEN account_code = 'O' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10 * 238 / 1000
          WHEN account_code = 'K' THEN (contract_amt) / 10 * 17 / 1000
          WHEN account_code = 'Z' THEN (contract_amt) / 10 * 213 / 1000
        END)
FROM FW_PLOAN.dbo.BAM086_200308_DEDUP
GROUP BY IDN

UPDATE fw_ms
SET
MS093 = V1
FROM MS_TMP AS A
WHERE A.IDN = fw_ms.IDN
GO

update fw_ms
set ms093=0
where ms093 is null
go

select ms093, count(*)
from fw_ms
group by ms093
order by ms093
go

--MS094 avg. credit card revolving balance in past 3 months

/* 456 */
update fw_ms
set ms094=null
go

update fw_ms
set ms094=a.amt
from (select idn, sum(Payment_Amt)/3 as amt
      from fw_ploan.dbo.KRM023_200305_WITH_THREE_BUCKET
      where 1108-mon_since<3 and
            pay_code in ('C','D','E','F')
      group by idn) as a
where a.idn=fw_ms.idn
go

update fw_ms
set ms094=0
where ms094 is null
go


select ms094, count(*)
from fw_ms
group by ms094
order by ms094
go

select *
from fw_ms
where ms094>2400
go

--D148147229	171.0	122.0	2473.3333333333335
H146548745	23.0	15.0	2433.3333333333335

select *
from fw_ploan.dbo.KRM023_200305_WITH_THREE_BUCKET
where idn='H146548745'
order by mon_since
go

/* 789 */
update fw_ms
set ms094=null
go

update fw_ms
set ms094=a.amt
from (select idn, sum(Payment_Amt)/3 as amt
      from fw_ploan.dbo.KRM023_200308_WITH_THREE_BUCKET
      where 1111-mon_since<3 and
            pay_code in ('C','D','E','F')
      group by idn) as a
where a.idn=fw_ms.idn
go

update fw_ms
set ms094=0
where ms094 is null
go


select ms094, count(*)
from fw_ms
group by ms094
order by ms094
go

select *
from fw_ms
where ms094>2500
go

--F126719923	86.0	62.0	2996.8333333333335

select *
from fw_ploan.dbo.KRM023_200308_WITH_THREE_BUCKET
where idn='F126719923'
order by mon_since
go

--MS105  avg. cash card balance

alter table fw_ms
add MS105 float
go

update fw_ms
set ms105=0
go

update fw_ms
set ms105=a.amt
from (select idn, ( sum( convert(float, loan_amt))+sum(convert(float, pass_due_amt)))  as amt
      from FW_PLOAN.dbo.BAM086_200305_DEDUP
      where account_code='Y'
      group by idn) as a
where a.idn=fw_ms.idn
go

select ms105, count(*)
from fw_ms
group by ms105
order by ms105
go

alter table fw_ms
add MS105 float
go

update fw_ms
set ms105=0
go

update fw_ms
set ms105=a.amt
from (select idn, ( sum( convert(float, loan_amt))+sum(convert(float, pass_due_amt)))  as amt
      from FW_PLOAN.dbo.BAM086_200308_DEDUP
      where account_code='Y'
      group by idn) as a
where a.idn=fw_ms.idn
go

select ms105, count(*)
from fw_ms
group by ms105
order by ms105
go

select a.idn, a.ms094, b.ms094
from fw_ms as a inner join fw_ms_200308 as b
      on a.idn=b.idn 
where a.ms094<>b.ms094
go

/* A126091638	160.0	17.0
A128577457	84.0	20.0 */

select *
from FW_PLOAN.dbo.BAM086_200308_DEDUP
where idn='A126091638'
go

select idn, ( sum( convert(float, loan_amt))+sum(convert(float, pass_due_amt)))  as amt
from FW_PLOAN.dbo.BAM086_200308_DEDUP
where account_code='Y' and
      idn='A126091638'
group by idn
go