USE TF_KHJ
GO

DROP TABLE BAM085_RS_200412
GO

CREATE TABLE BAM085_RS_200412
(IDN CHAR(6),
 RS101 Float, RS102 Float, RS103 Float, RS104 Float, 
 RS105 Float, RS106 Float, RS107 Float, RS108 Float, 
 RS109 Float, RS110 Float, RS111 Float, RS112 Float, 
 RS113 Float, RS114 Float, RS115 Float, RS116 Float, 
 RS117 Float, RS118 Float, RS119 Float, RS120 Float)
GO

DROP TABLE RS_TMP
GO
DROP TABLE RS_TMP1
GO
DROP TABLE RS_TMP2

CREATE TABLE RS_TMP
(IDN Char(6),
 Mon INT,
 V1 int,
 V2 int,
 V3 int)
GO

CREATE TABLE RS_TMP1
(IDN Char(6),
 Mon INT,
 V1 int)
GO

CREATE TABLE RS_TMP2
(IDN Char(6),
 Issuer char(3),
 Mon int,
 Line int)
GO

INSERT INTO BAM085_RS_200412(IDN)
SELECT DISTINCT IDN
FROM BAM085_200412
GO

UPDATE BAM085_RS_200412
SET
RS101=0,
RS102=0,
RS103=0,
RS104=0,
RS105=0,
RS106=0,
RS107=0,
RS108=0,
RS109=0,
RS110=0,
RS111=0,
RS112=0,
RS113=0,
RS114=0,
RS115=0,
RS116=0,
RS117=0,
RS118=0,
RS119=0,
RS120=0
GO


----RS101----
--最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS101 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS102----
--有擔保貸款 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code2 in ('S', 'W', 'M')
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS102 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS103----
--有擔保貸款 - 短期 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code in ('W','C','D','E') AND
      account_code2 in ('S', 'W', 'M')
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS103 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS104----
--有擔保貸款 - 中期 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code in ('H','S') AND
      account_code2 in ('S', 'W', 'M')
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS104 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS105----
--有擔保貸款 - 長期 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code in ('I','T') AND
      account_code2 in ('S', 'W', 'M')
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS105 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS106----
--無擔保貸款 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      (((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N')) AND
       Account_Code != 'K')
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS106 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS107----
--無擔保貸款 - 短期 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code in ('W','C', 'D', 'E') AND
      ((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N'))
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS107 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS108----
--無擔保貸款 - 中期 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code in ('H','S') AND
      ((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N'))
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS108 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS109----
--無擔保貸款 - 長期 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code in ('I','T') AND
      ((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N'))
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS109 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS110----
--財政部優惠貸款筆數 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code2 in ('V','W')
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS110 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS111----
--不動產貸款筆數 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      purpose_code = '1'
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS111 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS112----
--動產貸款筆數 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      purpose_code = '2'
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS112 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS113----
--週轉金貸款筆數 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      purpose_code = '4'
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS113 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS114----
--存保單質押筆數 (無擔保) 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code='K' AND
      ((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N')) AND
      purpose_code = '4'
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS114 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS115----
--現金卡筆數 (無擔保) 最近遲繳距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      account_code = 'Y'
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS115 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS116----
--Co-Loan 筆數 最近遲繳距今距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      Co_Loan = 'Y'
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS116 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS117----
--在同一金融機構沒有房貸的購屋用無擔保貸款 最近遲繳距今距今月數
DELETE FROM RS_TMP1
DELETE FROM RS_TMP2

INSERT INTO RS_TMP2
SELECT IDN, BANK_CODE2, NULL, NULL
FROM BAM085_200412
WHERE PURPOSE_CODE = '1' AND
      account_code2 in ('S', 'W', 'M')

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      NOT EXISTS
(SELECT * FROM RS_TMP2
 WHERE y_BAM085_tmp_200412.IDN = RS_TMP2.IDN AND
       y_BAM085_tmp_200412.BANK_CODE2 = RS_TMP2.ISSUER) AND
      PURPOSE_CODE = '1' AND
      ((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N')) AND
      ACCOUNT_CODE != 'K'
GROUP BY IDN  
 
UPDATE BAM085_RS_200412
SET
RS117 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS118----
--除在同一金融機構沒有房貸的購屋用無擔保貸款外的無擔保貸款 最近遲繳距今距今月數
DELETE FROM RS_TMP1
DELETE FROM RS_TMP
DELETE FROM RS_TMP2

INSERT INTO RS_TMP(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE PURPOSE_CODE != '1' AND
      ((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N'))
GROUP BY IDN
      
INSERT INTO RS_TMP2
SELECT IDN, BANK_CODE2, NULL, NULL
FROM BAM085_200412
WHERE PURPOSE_CODE = '1' AND
      account_code2 in ('S', 'W', 'M')

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      EXISTS
(SELECT * FROM RS_TMP2
 WHERE y_BAM085_tmp_200412.IDN = RS_TMP2.IDN AND
       y_BAM085_tmp_200412.BANK_CODE2 = RS_TMP2.ISSUER) AND
      PURPOSE_CODE = '1' AND
      ((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N')) AND
      ACCOUNT_CODE != 'K'
GROUP BY IDN  

INSERT INTO RS_TMP(IDN)
SELECT IDN FROM RS_TMP1
WHERE IDN NOT IN (SELECT IDN FROM RS_TMP)

UPDATE RS_TMP
SET
V2 = A.V1
FROM RS_TMP1 AS A INNER JOIN RS_TMP AS B
ON A.IDN = B.IDN

UPDATE RS_TMP
SET
V3 = (CASE 
        WHEN V1 >= V2 THEN V1
        WHEN V2 > V1 THEN V2
        WHEN (V1 IS NULL) AND (V2 IS NULL) THEN NULL
      END)

UPDATE BAM085_RS_200412
SET
RS118 = 13 - V3
FROM RS_TMP AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS119----
--有擔保購屋貸款 最近遲繳距今距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      PURPOSE_CODE = '1' AND
      account_code2 in ('S', 'W', 'M')
GROUP BY IDN

UPDATE BAM085_RS_200412
SET
RS119 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

----RS120----
--無擔保非購屋貸款 最近遲繳距今距今月數
DELETE FROM RS_TMP1

INSERT INTO RS_TMP1(IDN, V1)
SELECT IDN, MAX(cycle)
FROM y_BAM085_tmp_200412
WHERE Bucket > 0 AND
      PURPOSE_CODE != '1' AND
      ((account_code2 is null) OR (account_code2 = '') OR (account_code2 = 'N')) AND
      ACCOUNT_CODE != 'K'
GROUP BY IDN

UPDATE BAM085_RS_200412
SET
RS120 = 13 - V1
FROM RS_TMP1 AS A
WHERE A.IDN = BAM085_RS_200412.IDN
GO

