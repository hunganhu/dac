USE TF_KHJ
GO

DROP TABLE MS3xx_200409
GO

CREATE TABLE MS3xx_200409
(IDN Char(6), 
 MS301 int, MS302 int, MS303 int, MS304 int, MS305 int, MS306 int,
 MS307 float, MS308 float, MS309 float, MS310 float, MS311 float,
 MS312 float, MS313 float, MS314 float)
GO 

INSERT INTO MS3xx_200409(IDN)
SELECT DISTINCT IDN
FROM BAM085_200409
GO

DROP TABLE MS_TMP
GO

DROP TABLE MS_TMP1
GO

CREATE TABLE MS_TMP
(IDN Char(6),
 Mon INT,
 V1 float,
 V2 float,
 V3 float)
GO

CREATE TABLE MS_TMP1
(IDN Char(6),
 Mon INT,
 V1 float)
GO


/*----MS301----*/
/*現金卡總訂約金額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, SUM(CONVERT(INT,ISNULL(CONTRACT_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y'
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS301 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS302----*/
/*現金卡總餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, SUM(CONVERT(INT,ISNULL(LOAN_AMT,0)) + CONVERT(INT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y'
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS302 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS303----*/
/*逾期現金卡總餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, SUM(CONVERT(INT,ISNULL(LOAN_AMT,0)) + CONVERT(INT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      isnull(pc_12,0) > 0
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS303 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS304----*/
/*曾經逾期現金卡總餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, SUM(CONVERT(INT,ISNULL(LOAN_AMT,0)) + CONVERT(INT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      (isnull(pc_12,0) + isnull(pc_11,0) + isnull(pc_10,0) + isnull(pc_09,0) + 
       isnull(pc_08,0) + isnull(pc_07,0) + isnull(pc_06,0) + isnull(pc_05,0) + 
       isnull(pc_04,0) + isnull(pc_03,0) + isnull(pc_02,0) + isnull(pc_01,0)) > 0
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS304 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS305----*/
/*未逾期現金卡總餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, SUM(CONVERT(INT,ISNULL(LOAN_AMT,0)) + CONVERT(INT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      isnull(pc_12,0) = 0
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS305 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS306----*/
/*從未逾期現金卡總餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, SUM(CONVERT(INT,ISNULL(LOAN_AMT,0)) + CONVERT(INT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      (isnull(pc_12,0) + isnull(pc_11,0) + isnull(pc_10,0) + isnull(pc_09,0) + 
       isnull(pc_08,0) + isnull(pc_07,0) + isnull(pc_06,0) + isnull(pc_05,0) + 
       isnull(pc_04,0) + isnull(pc_03,0) + isnull(pc_02,0) + isnull(pc_01,0)) = 0
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS306 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS307----*/
/*現金卡平均訂約金額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, AVG(CONVERT(FLOAT,ISNULL(CONTRACT_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      isnull(CONTRACT_AMT,0) > 0
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS307 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS308----*/
/*現金卡平均餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, AVG(CONVERT(FLOAT,ISNULL(LOAN_AMT,0)) + CONVERT(FLOAT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      isnull(CONTRACT_AMT,0) > 0
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS308 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS309----*/
/*未逾期現金卡平均餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, AVG(CONVERT(FLOAT,ISNULL(LOAN_AMT,0)) + CONVERT(FLOAT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      isnull(pc_12,0) = 0 AND
      isnull(CONTRACT_AMT,0) > 0      
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS309 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS310----*/
/*從未逾期現金卡平均餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, AVG(CONVERT(FLOAT,ISNULL(LOAN_AMT,0)) + CONVERT(FLOAT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      (isnull(pc_12,0) + isnull(pc_11,0) + isnull(pc_10,0) + isnull(pc_09,0) + 
       isnull(pc_08,0) + isnull(pc_07,0) + isnull(pc_06,0) + isnull(pc_05,0) + 
       isnull(pc_04,0) + isnull(pc_03,0) + isnull(pc_02,0) + isnull(pc_01,0)) = 0 AND
      isnull(CONTRACT_AMT,0) > 0  
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS310 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS311----*/
/*萬泰現金卡總訂約金額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, SUM(CONVERT(FLOAT,ISNULL(CONTRACT_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      BANK_CODE2 = '809'
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS311 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS312----*/
/*萬泰現金卡總餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, SUM(CONVERT(FLOAT,ISNULL(LOAN_AMT,0)) + CONVERT(FLOAT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      BANK_CODE2 = '809'
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS312 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS313----*/
/*萬泰現金卡平均訂約金額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, AVG(CONVERT(FLOAT,ISNULL(CONTRACT_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      isnull(CONTRACT_AMT,0) > 0 AND
      BANK_CODE2 = '809'
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS313 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO

/*----MS314----*/
/*萬泰現金卡平均餘額*/
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL, AVG(CONVERT(FLOAT,ISNULL(LOAN_AMT,0)) + CONVERT(FLOAT,ISNULL(PASS_DUE_AMT,0))), NULL, NULL
FROM BAM085_200409
WHERE ACCOUNT_CODE = 'Y' AND
      isnull(CONTRACT_AMT,0) > 0 AND
      BANK_CODE2 = '809'
GROUP BY IDN  
 
UPDATE MS3xx_200409
SET
MS314 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS3xx_200409.IDN
GO