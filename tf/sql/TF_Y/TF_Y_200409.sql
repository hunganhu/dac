USE TF_Y_JCIC
GO

DROP TABLE JAS002_200409_T
GO
DROP TABLE JAS002_200409_T_DEDUP
GO
DROP TABLE TMP
GO
ALTER TABLE KRM023_200409_DEDUP DROP COLUMN BUCKET_DEF_1K
GO
ALTER TABLE KRM023_200409_DEDUP DROP COLUMN BUCKET_EF_1K
GO
ALTER TABLE KRM023_200409_DEDUP DROP COLUMN BUCKET_F_1K
GO
ALTER TABLE KRM023_200409_DEDUP DROP COLUMN PAYMENT_AMT
GO
ALTER TABLE KRM023_200409_DEDUP DROP COLUMN MON_SINCE
GO
ALTER TABLE KRM023_200409_DEDUP ADD BUCKET_DEF_1K INT
GO
ALTER TABLE KRM023_200409_DEDUP ADD BUCKET_EF_1K INT
GO
ALTER TABLE KRM023_200409_DEDUP ADD BUCKET_F_1K INT
GO
ALTER TABLE KRM023_200409_DEDUP ADD PAYMENT_AMT FLOAT
GO
ALTER TABLE KRM023_200409_DEDUP ADD MON_SINCE INT
GO

UPDATE KRM023_200409_DEDUP
SET
PAYMENT = (CASE WHEN RIGHT(LEFT(PAYMENT,2),1) IN ('H', 'M', 'L') THEN '0' + (LEFT(PAYMENT,2)) ELSE PAYMENT END)
GO

UPDATE KRM023_200409_DEDUP
SET
PAYMENT_AMT = (CASE RIGHT(PAYMENT,1) WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ELSE 0 END) * POWER(10, ISNULL(LEFT(PAYMENT,2),0)-1) / 1000.0
GO                 

UPDATE KRM023_200409_DEDUP
SET
MON_SINCE = (CASE WHEN LEFT(YRMON,1) = '*' THEN NULL  
             ELSE LEFT(YRMON, 3) * 12 + RIGHT(LEFT(YRMON, 5), 2) END)
GO  

CREATE TABLE JAS002_200409_T (IDN Char(6), Reason char, Date_Happen char(7), Mon_Since int)
GO
CREATE TABLE JAS002_200409_T_DEDUP (IDN Char(6), Reason char, Date_Happen char(7), Mon_Since int, Records int) 
GO
CREATE TABLE TMP (IDN Char(6), Mon int, V1 int)

INSERT INTO JAS002_200409_T
SELECT IDN, 'D', Delinquent_date, NULL
FROM JAS002_200409_DEDUP
WHERE (delinquent_date is not null) and 
      (delinquent_date != '')
GO
--716
INSERT INTO JAS002_200409_T
SELECT IDN, 'B', Bad_check_date, NULL
FROM JAS002_200409_DEDUP
WHERE (Bad_check_date is not null) and
      (bad_check_date != '')
GO
--12
INSERT INTO JAS002_200409_T
SELECT IDN, 'R', Reject_date, NULL
FROM JAS002_200409_DEDUP
WHERE (Reject_date is not null) and
      (Reject_date != '')
GO
--85
INSERT INTO JAS002_200409_T
SELECT IDN, 'S', Stop_Card_date, NULL
FROM JAS002_200409_DEDUP
WHERE (Stop_Card_date is not null) and
      (Stop_Card_date != '')
GO
--714

INSERT INTO JAS002_200409_T_DEDUP
SELECT IDN, Reason, Date_Happen, Mon_Since, COUNT(*) AS Records
FROM JAS002_200409_T
GROUP BY IDN, Reason, Date_Happen, Mon_Since
GO
--1527
UPDATE JAS002_200409_T_DEDUP
SET
Date_Happen =
(CASE
 WHEN LEFT(LTRIM(Date_Happen),1) NOT IN ('1', '0', '*') THEN '0' + (LTRIM(Date_Happen))
 ELSE Date_Happen 
 END)
GO
/*1527*/
UPDATE JAS002_200409_T_DEDUP
SET
Mon_Since =
(CASE
   WHEN LEFT(LTRIM(Date_Happen),1) = '*' THEN NULL
   ELSE LEFT(LTRIM(Date_Happen), 3) * 12 + Right(LEFT(RTRIM(Date_Happen), 5), 2)
 END)
GO
/*1527*/
UPDATE KRM023_200409_DEDUP
SET 
BUCKET_DEF_1k = 0,
BUCKET_EF_1k = 0,
BUCKET_F_1k = 0;
GO
/*199970*/ 
UPDATE KRM023_200409_DEDUP
SET BUCKET_DEF_1k = (CASE WHEN pay_code in ('D', 'E', 'F') AND Payment_Amt > 1 THEN 1 
                       ELSE 0
                       END), 
    BUCKET_EF_1k = (CASE WHEN pay_code in ('E', 'F') AND Payment_Amt > 1 THEN 1
                      ELSE 0 END), 
    BUCKET_F_1k = (CASE WHEN pay_code = 'F' AND Payment_Amt > 1 THEN 1
                     ELSE 0 END);
GO

DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 9

SET @i=12 
  WHILE @i > 0
  BEGIN
    UPDATE KRM023_200409_DEDUP
    SET  
      bucket_def_1k =(CASE WHEN KRM023_200409_DEDUP.PAY_code IN ('D', 'E', 'F') AND
    	                        KRM023_200409_DEDUP.Payment_Amt > 1 
        	                THEN a.bucket_def_1k + 1 
            		   ELSE 0 END), 
      bucket_ef_1k =(CASE WHEN  KRM023_200409_DEDUP.pay_code in ('E', 'F') AND
                                KRM023_200409_DEDUP.Payment_Amt > 1 
                                THEN a.bucket_ef_1k + 1 
                          ELSE 0 END), 
      bucket_f_1k =(CASE WHEN KRM023_200409_DEDUP.pay_code = 'F' AND
                              KRM023_200409_DEDUP.Payment_Amt > 1 
                         THEN a.bucket_f_1k + 1 
                         ELSE 0 END) 
      FROM KRM023_200409_DEDUP AS A INNER JOIN  KRM023_200409_DEDUP ON 
      A.IDN =  KRM023_200409_DEDUP.IDN AND 
      A.Issue_Name =  KRM023_200409_DEDUP.Issue_Name AND 
      A.Mon_Since = ( KRM023_200409_DEDUP.Mon_Since - 1)
      WHERE (@NOW - A.Mon_Since) = @i
      SET @i = @i - 1 
    END              

DELETE FROM TMP

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 3, COUNT(*)
FROM JAS002_200409_T_DEDUP
WHERE Reason = 'B' AND 
      @NOW - Mon_Since BETWEEN 0 AND 3
GROUP BY IDN

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 6, COUNT(*)
FROM JAS002_200409_T_DEDUP
WHERE Reason = 'B' AND 
      @NOW - Mon_Since BETWEEN 0 AND 6
GROUP BY IDN

SET @i = 12
WHILE @i <= 36
BEGIN
  INSERT INTO TMP(IDN, MON, V1)
  SELECT IDN, @i, COUNT(*)
  FROM JAS002_200409_T_DEDUP
  WHERE Reason = 'B' AND 
        @NOW - Mon_Since BETWEEN 0 AND @i
  GROUP BY IDN
  SET @i = @i + 12
END

UPDATE IDN_LIST
SET
JAS002_B_3M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 3

UPDATE IDN_LIST
SET
JAS002_B_6M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 6

UPDATE IDN_LIST
SET
JAS002_B_12M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 12

UPDATE IDN_LIST
SET
JAS002_B_24M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 24

UPDATE IDN_LIST
SET
JAS002_B_36M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 36


DELETE FROM TMP

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 3, COUNT(*)
FROM JAS002_200409_T_DEDUP
WHERE Reason = 'D' AND 
      @NOW - Mon_Since BETWEEN 0 AND 3
GROUP BY IDN

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 6, COUNT(*)
FROM JAS002_200409_T_DEDUP
WHERE Reason = 'D' AND 
      @NOW - Mon_Since BETWEEN 0 AND 6
GROUP BY IDN

SET @i = 12
WHILE @i <= 36
BEGIN
  INSERT INTO TMP(IDN, MON, V1)
  SELECT IDN, @i, COUNT(*)
  FROM JAS002_200409_T_DEDUP
  WHERE Reason = 'D' AND 
        @NOW - Mon_Since BETWEEN 0 AND @i
  GROUP BY IDN
  SET @i = @i + 12
END

UPDATE IDN_LIST
SET
JAS002_D_3M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 3

UPDATE IDN_LIST
SET
JAS002_D_6M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 6

UPDATE IDN_LIST
SET
JAS002_D_12M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 12

UPDATE IDN_LIST
SET
JAS002_D_24M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 24

UPDATE IDN_LIST
SET
JAS002_D_36M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 36


DELETE FROM TMP

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 3, COUNT(*)
FROM JAS002_200409_T_DEDUP
WHERE Reason = 'R' AND 
      @NOW - Mon_Since BETWEEN 0 AND 3
GROUP BY IDN

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 6, COUNT(*)
FROM JAS002_200409_T_DEDUP
WHERE Reason = 'R' AND 
      @NOW - Mon_Since BETWEEN 0 AND 6
GROUP BY IDN

SET @i = 12
WHILE @i <= 36
BEGIN
  INSERT INTO TMP(IDN, MON, V1)
  SELECT IDN, @i, COUNT(*)
  FROM JAS002_200409_T_DEDUP
  WHERE Reason = 'R' AND 
        @NOW - Mon_Since BETWEEN 0 AND @i
  GROUP BY IDN
  SET @i = @i + 12
END

UPDATE IDN_LIST
SET
JAS002_R_3M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 3

UPDATE IDN_LIST
SET
JAS002_R_6M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 6

UPDATE IDN_LIST
SET
JAS002_R_12M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 12

UPDATE IDN_LIST
SET
JAS002_R_24M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 24

UPDATE IDN_LIST
SET
JAS002_R_36M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 36


DELETE FROM TMP

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 3, COUNT(*)
FROM JAS002_200409_T_DEDUP
WHERE Reason = 'S' AND 
      @NOW - Mon_Since BETWEEN 0 AND 3
GROUP BY IDN

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 6, COUNT(*)
FROM JAS002_200409_T_DEDUP
WHERE Reason = 'S' AND 
      @NOW - Mon_Since BETWEEN 0 AND 6
GROUP BY IDN

SET @i = 12
WHILE @i <= 36
BEGIN
  INSERT INTO TMP(IDN, MON, V1)
  SELECT IDN, @i, COUNT(*)
  FROM JAS002_200409_T_DEDUP
  WHERE Reason = 'S' AND 
        @NOW - Mon_Since BETWEEN 0 AND @i
  GROUP BY IDN
  SET @i = @i + 12
END

UPDATE IDN_LIST
SET
JAS002_S_3M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 3

UPDATE IDN_LIST
SET
JAS002_S_6M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 6

UPDATE IDN_LIST
SET
JAS002_S_12M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 12

UPDATE IDN_LIST
SET
JAS002_S_24M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 24

UPDATE IDN_LIST
SET
JAS002_S_36M = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN AND
   Mon = 36


DELETE FROM TMP

INSERT INTO TMP(IDN, V1)
SELECT IDN, COUNT(*)
FROM BAM086_200409_DEDUP
WHERE (CAST(PASS_DUE_AMT AS INT) > 0) OR
      LEFT(PAY_CODE_12,1) NOT IN ('0', 'X')
GROUP BY IDN

UPDATE IDN_LIST
SET
DELINQUENT_LOANS = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN


DELETE FROM TMP

INSERT INTO TMP(IDN, V1)
SELECT IDN, MAX(BUCKET_EF_1K)
FROM KRM023_200409_DEDUP
GROUP BY IDN

UPDATE IDN_LIST
SET
MAX_BUCKET = V1
FROM TMP AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN

ALTER TABLE BAM086_200409_DEDUP ADD BANK_CODE2 CHAR(3)
GO

UPDATE BAM086_200409_DEDUP
SET
BANK_CODE2 = LEFT(LTRIM(BANK_CODE), 3)
GO

alter table BAM086_200409_DEDUP
add
cycles int, pc_c01 char, pc_c02 char, pc_c03 char, pc_c04 char, pc_c05 char, pc_c06 char,
            pc_c07 char, pc_c08 char, pc_c09 char, pc_c10 char, pc_c11 char, pc_c12 char, 
            pc_01 float, pc_02 float, pc_03 float, pc_04 float, pc_05 float, pc_06 float,
            pc_07 float, pc_08 float, pc_09 float, pc_10 float, pc_11 float, pc_12 float
go

update BAM086_200409_DEDUP
set
cycles = len(isnull(pay_code_12, ''))
go

update BAM086_200409_DEDUP
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
update BAM086_200409_DEDUP
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
drop table y_BAM086_tmp_200409
go
create table y_BAM086_tmp_200409 
(IDN Char(6), 
 DATA_YYY Char(5), DATA_MM Char(2), BANK_CODE Char(7), BANK_NAME Char(40),
 ACCOUNT_CODE Char, ACCOUNT_CODE2 Char, PURPOSE_CODE Char, CONTRACT_AMT int, 
 LOAN_AMT int, PASS_DUE_AMT int, CO_LOAN Char, Bank_Code2 Char(3), Bucket float, Cycle int)
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_01, 1
from BAM086_200409_DEDUP
go
--653
insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_02, 2
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_03, 3
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_04, 4
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_05, 5
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_06, 6
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_07, 7
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_08, 8
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_09, 9
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_10, 10
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_11, 11
from BAM086_200409_DEDUP
go

insert into y_BAM086_tmp_200409(IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, Bucket, Cycle)
select
IDN, DATA_YYY, DATA_MM, BANK_CODE, BANK_NAME, ACCOUNT_CODE, ACCOUNT_CODE2, PURPOSE_CODE, CONTRACT_AMT, LOAN_AMT, PASS_DUE_AMT, CO_LOAN, Bank_Code2, pc_12, 12
from BAM086_200409_DEDUP
go

ALTER TABLE IDN_LIST ADD BAM086_Max_Bucket_12M FLOAT, BAM086_Max_Bucket_3M FLOAT, BAM086_Max_Bucket_6M FLOAT, BAM086_Max_Bucket_9M FLOAT, BAM086_Bucket FLOAT
GO

DELETE FROM TMP1

DECLARE @i INT
SET @i = 10
WHILE @i > 0
BEGIN
  INSERT INTO TMP1(IDN, MON, V1)
  SELECT IDN, @i, MAX(bucket)
  FROM y_BAM086_tmp_200409
  WHERE cycle >= @i
  GROUP BY IDN
  SET @i = @i - 3
END

UPDATE IDN_LIST
SET
BAM086_Max_bucket_3M = V1
FROM TMP1 AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN
WHERE A.MON = 10
GO

UPDATE IDN_LIST
SET
BAM086_Max_bucket_9M = V1
FROM TMP1 AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN
WHERE A.MON = 7
GO

UPDATE IDN_LIST
SET
BAM086_Max_bucket_6M = V1
FROM TMP1 AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN
WHERE A.MON = 4
GO

UPDATE IDN_LIST
SET
BAM086_Max_bucket_12M = V1
FROM TMP1 AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN
WHERE A.MON = 1
GO

DELETE FROM TMP1

INSERT INTO TMP1(IDN, V1)
SELECT IDN, MAX(pc_12)
FROM BAM086_200409_DEDUP
GROUP BY IDN

UPDATE IDN_LIST
SET
BAM086_Bucket = V1
FROM TMP1 AS A INNER JOIN IDN_LIST AS B
ON A.IDN = B.IDN
GO