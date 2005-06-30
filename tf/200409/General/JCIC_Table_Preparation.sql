USE TF_KHJ
GO
DROP TABLE JAS002_200409_T
GO
CREATE TABLE JAS002_200409_T (IDN Char(6), Reason char, Date_Happen char(7), Mon_Since int)
GO
INSERT INTO JAS002_200409_T
SELECT IDN, 'D', Delinquent_date, NULL
FROM JAS002_200409
WHERE delinquent_date is not null
GO
--2
INSERT INTO JAS002_200409_T
SELECT IDN, 'B', Bad_check_date, NULL
FROM JAS002_200409
WHERE Bad_check_date is not null
GO
--0
INSERT INTO JAS002_200409_T
SELECT IDN, 'R', Reject_date, NULL
FROM JAS002_200409
WHERE Reject_date is not null
GO
--2
INSERT INTO JAS002_200409_T
SELECT IDN, 'S', Stop_Card_date, NULL
FROM JAS002_200409
WHERE Stop_Card_date is not null
GO
--8
DROP TABLE JAS002_200409_T_DEDUP
GO
SELECT IDN, Reason, Date_Happen, Mon_Since, COUNT(*) AS Records
INTO JAS002_200409_T_DEDUP
FROM JAS002_200409_T
GROUP BY IDN, Reason, Date_Happen, Mon_Since
GO
--12
SELECT COUNT(*), COUNT(DISTINCT IDN), SUM(RECORDS)
FROM JAS002_200409_T_DEDUP
/*12	10	12*/

ALTER TABLE KRM001_200409 ADD Start_Mon_Since int, End_Mon_Since int
GO
UPDATE KRM001_200409
SET
Start_Date =
(CASE
 WHEN LEFT(Start_Date,1) NOT IN ('1', '0', '*') THEN '0' + (Start_Date)
 ELSE Start_Date 
 END),
Stop_Date =
(CASE
 WHEN LEFT(Stop_Date,1) NOT IN ('1', '0', '*') THEN '0' + (Stop_Date)
 ELSE Stop_Date 
 END)
/*1310*/
UPDATE KRM001_200409
SET
Start_Mon_Since =
(CASE
   WHEN LEFT(Start_Date,1) = '*' THEN NULL
   ELSE LEFT(Start_Date, 3) * 12 + Right(LEFT(Start_Date, 5), 2)
 END),
End_Mon_Since =
(CASE
   WHEN LEFT(Stop_Date, 1) = '*' THEN NULL
   ELSE LEFT(Stop_Date, 3) * 12 + Right(LEFT(Stop_Date, 5), 2)
 END)
GO
/*1310*/
UPDATE KRM001_200409
SET
End_Mon_Since = 999 * 12 + 12
WHERE End_Mon_Since IS NULL
GO
/*945*/
select end_mon_since from krm001_200409 where stop_date is null
select max(stop_date), max(end_mon_since) from krm001_200409 where stop_date is not null
select start_date, start_mon_since from krm001_200409 where start_date like '%*%'

UPDATE JAS002_200409_T_DEDUP
SET
Date_Happen =
(CASE
 WHEN LEFT(LTRIM(Date_Happen),1) NOT IN ('1', '0', '*') THEN '0' + (LTRIM(Date_Happen))
 ELSE Date_Happen 
 END)
GO
/*12*/
UPDATE JAS002_200409_T_DEDUP
SET
Mon_Since =
(CASE
   WHEN LEFT(LTRIM(Date_Happen),1) = '*' THEN NULL
   ELSE LEFT(LTRIM(Date_Happen), 3) * 12 + Right(LEFT(RTRIM(Date_Happen), 5), 2)
 END)
GO
/*12*/
SELECT * FROM JAS002_200409_T_DEDUP
GO

ALTER TABLE KRM023_200409 ADD Mon_Since int
GO
UPDATE KRM023_200409
SET
YRMON =
(CASE
 WHEN LEFT(YRMON,1) NOT IN ('1', '0', '*') THEN '0' + (YRMON)
 ELSE YRMON 
 END)
GO
/*6053*/
UPDATE KRM023_200409
SET
Mon_Since =
(CASE
   WHEN LEFT(YRMON,1) = '*' THEN NULL
   ELSE LEFT(YRMON, 3) * 12 + Right(LEFT(YRMON, 5), 2)
 END)
GO
/*6053*/
ALTER TABLE STM001_200409 ADD Query_Mon_Since int
GO
UPDATE STM001_200409
SET
Query_Date =
(CASE
 WHEN LEFT(Query_Date,1) NOT IN ('1', '0', '*') THEN '0' + (Query_Date)
 ELSE Query_Date 
 END)
GO
/*986*/
UPDATE STM001_200409
SET
Query_Mon_Since =
(CASE
   WHEN LEFT(Query_Date,1) = '*' THEN NULL
   ELSE LEFT(Query_Date, 3) * 12 + Right(LEFT(Query_Date, 5), 2)
 END)
GO
/*986*/
ALTER TABLE KRM023_200409 ADD Payment_Amt float
GO
UPDATE KRM023_200409
SET
Payment_Amt = 
(CASE RIGHT(RTRIM(Payment),1)
   WHEN 'L' THEN 2
   WHEN 'M' THEN 5
   WHEN 'H' THEN 8
   ELSE 0
 END) * POWER(10, ISNULL(LEFT(LTRIM(Payment),2),0)-1) / 1000.0
GO
/*6053*/
ALTER TABLE KRM023_200409 ADD Bucket_def_1K INT, Bucket_ef_1K INT, Bucket_f_1K INT;
GO
UPDATE KRM023_200409
SET 
BUCKET_DEF_1k = 0,
BUCKET_EF_1k = 0,
BUCKET_F_1k = 0;
GO
/*6053*/ 
UPDATE KRM023_200409
SET BUCKET_DEF_1k = (CASE WHEN pay_code in ('D', 'E', 'F') AND Payment_Amt > 1 THEN 1 
                       ELSE 0
                       END), 
    BUCKET_EF_1k = (CASE WHEN pay_code in ('E', 'F') AND Payment_Amt > 1 THEN 1
                      ELSE 0 END), 
    BUCKET_F_1k = (CASE WHEN pay_code = 'F' AND Payment_Amt > 1 THEN 1
                     ELSE 0 END);
GO
/*6053*/
DECLARE @i INT, @NOW INT
SET @NOW = 93 * 12 + 9
SET @i=12 
WHILE @i > 0
BEGIN
  UPDATE KRM023_200409
  SET  
  bucket_def_1k =(CASE WHEN KRM023_200409.PAY_code IN ('D', 'E', 'F') AND
                            KRM023_200409.Payment_Amt > 1 
                       THEN a.bucket_def_1k + 1 
                       ELSE 0 END), 
  bucket_ef_1k =(CASE WHEN  KRM023_200409.pay_code in ('E', 'F') AND
                            KRM023_200409.Payment_Amt > 1 
                      THEN a.bucket_ef_1k + 1 
                      ELSE 0 END), 
  bucket_f_1k =(CASE WHEN  KRM023_200409.pay_code = 'F' AND
                           KRM023_200409.Payment_Amt > 1 
                     THEN a.bucket_f_1k + 1 
                     ELSE 0 END) 
  FROM  KRM023_200409 AS A INNER JOIN  KRM023_200409 ON 
        A.IDN =  KRM023_200409.IDN AND 
        A.Issue =  KRM023_200409.Issue AND 
        A.Mon_Since = ( KRM023_200409.Mon_Since - 1) 
  WHERE (@NOW - A.Mon_Since) = @i 
SET @i = @i - 1 
END;
GO

ALTER TABLE BAM085_200409 ADD Bank_Code2 char(3)
GO
UPDATE BAM085_200409
SET
Bank_Code2 = LEFT(BANK_CODE,3)
GO
--653
DROP TABLE BASE_200409
GO
SELECT IDN, MIN_INQUIRY_DATE, ACTUAL_DATA_FLAG AS AVAIL_FLAG, INQUIRY_DATE_DIFF
INTO BASE_200409
FROM data_availability
WHERE JCIC_MONTH = '200409'
GO
--220
