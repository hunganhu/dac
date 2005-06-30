use TF_KHJ
go

alter table BAM085_200409
add
cycles int, pc_c01 char, pc_c02 char, pc_c03 char, pc_c04 char, pc_c05 char, pc_c06 char,
            pc_c07 char, pc_c08 char, pc_c09 char, pc_c10 char, pc_c11 char, pc_c12 char, 
            pc_01 float, pc_02 float, pc_03 float, pc_04 float, pc_05 float, pc_06 float,
            pc_07 float, pc_08 float, pc_09 float, pc_10 float, pc_11 float, pc_12 float
go

update BAM085_200409
set
cycles = len(isnull(pay_code_12, ''))
go

update BAM085_200409
set
pc_c01 = (case when cycles < 12 then null else right(left(pay_code_12,12),1) end),
pc_c02 = (case when cycles < 11 then null else right(left(pay_code_12,11),1) end),
pc_c03 = (case when cycles < 10 then null else right(left(pay_code_12,10),1) end),
pc_c04 = (case when cycles < 9 then null else right(left(pay_code_12,9),1) end),
pc_c05 = (case when cycles < 8 then null else right(left(pay_code_12,8),1) end),
pc_c06 = (case when cycles < 7 then null else right(left(pay_code_12,7),1) end),
pc_c07 = (case when cycles < 6 then null else right(left(pay_code_12,6),1) end),
pc_c08 = (case when cycles < 5 then null else right(left(pay_code_12,5),1) end),
pc_c09 = (case when cycles < 4 then null else right(left(pay_code_12,4),1) end),
pc_c10 = (case when cycles < 3 then null else right(left(pay_code_12,3),1) end),
pc_c11 = (case when cycles < 2 then null else right(left(pay_code_12,2),1) end),
pc_c12 = (case when cycles < 1 then null else left(pay_code_12,1) end)
go
--653
update BAM085_200409
set
pc_01 = (case 
           when pc_c01 = 'X' then 0
           when pc_c01 = 'A' then 0.25
           when pc_c01 = 'B' then 0.5
           when pc_c01 is null then null
           else convert(float, pc_c01)
         end),
pc_02 = (case 
           when pc_c02 = 'X' then 0
           when pc_c02 = 'A' then 0.25
           when pc_c02 = 'B' then 0.5
           when pc_c02 is null then null
           else convert(float, pc_c02)
         end),
pc_03 = (case 
           when pc_c03 = 'X' then 0
           when pc_c03 = 'A' then 0.25
           when pc_c03 = 'B' then 0.5
           when pc_c03 is null then null
           else convert(float, pc_c03)
         end),
pc_04 = (case 
           when pc_c04 = 'X' then 0
           when pc_c04 = 'A' then 0.25
           when pc_c04 = 'B' then 0.5
           when pc_c04 is null then null
           else convert(float, pc_c04)
         end),
pc_05 = (case 
           when pc_c05 = 'X' then 0
           when pc_c05 = 'A' then 0.25
           when pc_c05 = 'B' then 0.5
           when pc_c05 is null then null
           else convert(float, pc_c05)
         end),
pc_06 = (case 
           when pc_c06 = 'X' then 0
           when pc_c06 = 'A' then 0.25
           when pc_c06 = 'B' then 0.5
           when pc_c06 is null then null
           else convert(float, pc_c06)
         end),
pc_07 = (case 
           when pc_c07 = 'X' then 0
           when pc_c07 = 'A' then 0.25
           when pc_c07 = 'B' then 0.5
           when pc_c07 is null then null
           else convert(float, pc_c07)
         end),
pc_08 = (case 
           when pc_c08 = 'X' then 0
           when pc_c08 = 'A' then 0.25
           when pc_c08 = 'B' then 0.5
           when pc_c08 is null then null
           else convert(float, pc_c08)
         end),
pc_09 = (case 
           when pc_c09 = 'X' then 0
           when pc_c09 = 'A' then 0.25
           when pc_c09 = 'B' then 0.5
           when pc_c09 is null then null
           else convert(float, pc_c09)
         end),
pc_10 = (case 
           when pc_c10 = 'X' then 0
           when pc_c10 = 'A' then 0.25
           when pc_c10 = 'B' then 0.5
           when pc_c10 is null then null
           else convert(float, pc_c10)
         end),
pc_11 = (case 
           when pc_c11 = 'X' then 0
           when pc_c11 = 'A' then 0.25
           when pc_c11 = 'B' then 0.5
           when pc_c11 is null then null
           else convert(float, pc_c11)
         end),
pc_12 = (case 
           when pc_c12 = 'X' then 0
           when pc_c12 = 'A' then 0.25
           when pc_c12 = 'B' then 0.5
           when pc_c12 is null then null           
           else convert(float, pc_c12)
         end)
go
--653
drop table y_BAM085_tmp_200409
go
create table y_BAM085_tmp_200409 
(IDN Char(6), 
 DATA_YYY Char(5), DATA_MM Char(2), BANK_CODE Char(7), BANK_NAME Char(40),
 ACCOUNT_CODE Char, ACCOUNT_CODE2 Char, PURPOSE_CODE Char, CONTRACT_AMT int, 
 LOAN_AMT int, PASS_DUE_AMT int, CO_LOAN Char, Bank_Code2 Char(3), Bucket float, Cycle int)
go

declare @now int
set @now = 93 * 12 + 9
insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_01, 1 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_02, 2 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_03, 3 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_04, 4 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_05, 5 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_06, 6 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_07, 7 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_08, 8 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_09, 9 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_10, 10 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_11, 11 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409

insert into y_BAM085_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_12, 12 - @now + cast(isnull(data_yyy,0) as int) * 12 + cast(isnull(data_mm,0) as int) + 1
from BAM085_200409
go
