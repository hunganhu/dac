USE TF_KHJ
GO

/*----TF_KHJ----*/

/*----MS031----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS031---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT,CONVERT(INT, LOAN_AMT)) + CONVERT(INT, CONVERT(INT, PASS_DUE_AMT))), NULL, NULL
FROM BAM085_200405
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS031 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO



/*----MS032----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS032---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT,Contract_Amt)), NULL, NULL
FROM BAM085_200405
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS032 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS033----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS033---*/
INSERT INTO MS_TMP
SELECT IDN, @i, MAX(CONVERT(INT,Contract_Amt)), NULL, NULL
FROM BAM085_200405
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS033 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS034----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS034---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT,Contract_Amt)), NULL, NULL
FROM BAM085_200405
WHERE (CONVERT(INT, LOAN_AMT) = 0) AND
      (CONVERT(INT, PASS_DUE_AMT) = 0)
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS034 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS035----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS035---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT,Contract_Amt)), NULL, NULL
FROM BAM085_200405
WHERE account_code2 in ('S', 'W', 'M')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS035 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS036----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS036---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT,Contract_Amt)), NULL, NULL
FROM BAM085_200405
WHERE account_code in ('W','C','D','E') AND
      account_code2 in ('S', 'W', 'M')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS036 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS037----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS037---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT,Contract_Amt)), NULL, NULL
FROM BAM085_200405
WHERE account_code in ('H','S') AND
      account_code2 in ('S', 'W', 'M')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS037 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS038----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS038---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code in ('I','T') AND
      account_code2 in ('S', 'W', 'M')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS038 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS039----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS039---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE (((account_code2 = '') or (account_code2 is null) or (account_code = 'N')))
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS039 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS040----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS040---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code in ('W','C', 'D', 'E') AND
      (((account_code2 = '') or (account_code2 is null) or (account_code = 'N')))
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS040 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

/*----MS041----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS041---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code in ('H','S') AND
      (((account_code2 = '') or (account_code2 is null) or (account_code = 'N')))
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS041 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS042----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS042---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code in ('I','T') AND
      (((account_code2 = '') or (account_code2 is null) or (account_code = 'N')))
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS042 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS043----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS043---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE CONVERT(INT, PASS_DUE_AMT) > 0
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS043 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS044----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS044---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code = 'A'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS044 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS045----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS045---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code = 'B'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS045 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS046----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS046---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code2 in ('V','W')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS046 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS047----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS047---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE purpose_code = '1'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS047 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS048----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS048---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE purpose_code = '2'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS048 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS049----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS049---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE purpose_code = '3'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS049 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS050----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS050---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE purpose_code = '4'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS050 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS051----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS051---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code='Z' AND
      ((Account_code2 is null) OR (Account_code2 = '') OR (Account_code2 = 'N')) AND
      purpose_code = '4'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS051 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS052----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS052---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code='K' AND
      ((Account_code2 is null) OR (Account_code2 = '') OR (Account_code2 = 'N')) AND
      purpose_code = '4'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS052 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS053----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS053---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code IN ('C','E','H') AND
      ((Account_code2 is null) OR (Account_code2 = '') OR (Account_code2 = 'N')) AND
      purpose_code = '4'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS053 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS054----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS054---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, CONTRACT_AMT)), NULL, NULL
FROM BAM085_200405
WHERE Co_Loan = 'Y'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS054 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS055----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS055---*/
INSERT INTO MS_TMP
SELECT IDN, @i, MAX(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS055 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS059----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS059---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code2 in ('S', 'W', 'M')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS059 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS060----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS060---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE   account_code2 in ('S', 'W', 'M') AND
        account_code in ('W','C','D','E')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS060 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS061----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS061---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE   account_code2 in ('S', 'W', 'M') AND
        account_code in ('H','S')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS061 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS062----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS062---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE   account_code2 in ('S', 'W', 'M') AND
        account_code in ('I','T')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS062 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS063----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS063---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE ((account_code2 = '') or (account_code2 is null) or (account_code = 'N'))
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS063 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS064----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS064---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE   ((account_code2 = '') or (account_code2 is null) or (account_code = 'N')) AND
        account_code IN ('W','C', 'D', 'E')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS064 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS065----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS065---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE   ((account_code2 = '') or (account_code2 is null) or (account_code = 'N')) AND
        account_code in ('H','S')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS065 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS066----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS066---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE   ((account_code2 = '') or (account_code2 is null) or (account_code = 'N')) AND
        account_code in ('I','T')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS066 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS067----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS067---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE CONVERT(INT, PASS_DUE_AMT) > 0
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS067 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS068----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS068---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE   account_code = 'A'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS068 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS069----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS069---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE   account_code = 'B'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS069 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS070----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS070---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE account_code2 in ('V','W')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS070 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS071----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS071---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE purpose_code = '1'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS071 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS072----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS072---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE purpose_code = '2'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS072 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS073----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS073---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE purpose_code = '3'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS073 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS074----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS074---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE purpose_code = '4'
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS074 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS075----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS075---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE ((account_code2 = '') or (account_code2 is null) or (account_code = 'N')) AND
      (account_code = 'Z') AND
      (purpose_code ='4')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS075 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS076----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS076---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE ((account_code2 = '') or (account_code2 is null) or (account_code = 'N')) AND
      (account_code = 'K') AND
      (purpose_code ='4')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS076 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO



/*----MS077----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS077---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE ((account_code2 = '') or (account_code2 is null) or (account_code = 'N')) AND
      (account_code in ('C', 'E', 'H')) AND
      (purpose_code ='4')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS077 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


/*----MS078----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 5
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS078---*/
INSERT INTO MS_TMP
SELECT IDN, @i, SUM(CONVERT(INT, LOAN_AMT) + CONVERT(INT, PASS_DUE_AMT)), NULL, NULL
FROM BAM085_200405
WHERE (Co_Loan = 'Y')
GROUP BY IDN  
 
UPDATE MS_200405
SET
MS078 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS079
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (loan_amt + pass_due_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) AND BANK_CODE2 IN ('809', '103', '815') THEN (loan_amt + pass_due_amt) / 10.0 * 158 / 1000.0
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (loan_amt + pass_due_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (loan_amt + pass_due_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'K' THEN (loan_amt + pass_due_amt) / 10.0 * 12 / 1000.0
          WHEN account_code = 'C' and account_code2 in ('S', 'W', 'M') THEN (loan_amt + pass_due_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'E' and account_code2 in ('S', 'W', 'M') THEN (loan_amt + pass_due_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') AND BANK_CODE2 IN ('001','052','016','056','057') THEN (loan_amt + pass_due_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') THEN (loan_amt + pass_due_amt) / 10.0 * 193 / 1000.0
          WHEN account_code = 'I' and account_code2 in ('S', 'W', 'M') THEN (loan_amt + pass_due_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'Z' THEN (loan_amt + pass_due_amt) / 10.0 * 233 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS079 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS080
DELETE FROM MS_TMP

INSERT INTO MS_TMP(IDN, V1)
SELECT IDN, SUM(CAST(CONTRACT_AMT AS INT) - CAST(LOAN_AMT AS INT) + CAST(PASS_DUE_AMT AS INT))
FROM BAM085_200405
WHERE ACCOUNT_CODE != 'Y'
GROUP BY IDN

UPDATE MS_200405
SET
MS080 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS081
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (loan_amt + pass_due_amt) / 10.0 * 72 / 1000.0
--          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) AND BANK_CODE2 IN ('809', '103', '815') THEN (loan_amt + pass_due_amt) / 10.0 * 158 / 1000.0
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (loan_amt + pass_due_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (loan_amt + pass_due_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'K' THEN (loan_amt + pass_due_amt) / 10.0 * 12 / 1000.0
          WHEN account_code = 'C' and account_code2 in ('S', 'W', 'M') THEN (loan_amt + pass_due_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'E' and account_code2 in ('S', 'W', 'M') THEN (loan_amt + pass_due_amt) / 10.0 * 72 / 1000.0
--          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') AND BANK_CODE2 IN ('001','052','016','056','057') THEN (loan_amt + pass_due_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') THEN (loan_amt + pass_due_amt) / 10.0 * 193 / 1000.0
          WHEN account_code = 'I' and account_code2 in ('S', 'W', 'M') THEN (loan_amt + pass_due_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'Z' THEN (loan_amt + pass_due_amt) / 10.0 * 233 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS081 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS082
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) AND BANK_CODE2 IN ('809', '103', '815') THEN (contract_amt) / 10.0 * 158 / 1000.0
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'K' THEN (contract_amt) / 10.0 * 12 / 1000.0
          WHEN account_code = 'C' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'E' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') AND BANK_CODE2 IN ('001','052','016','056','057') THEN (contract_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 193 / 1000.0
          WHEN account_code = 'I' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'Z' THEN (contract_amt) / 10.0 * 233 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS082 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS083
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 342 / 1000.0
          WHEN account_code = 'K' THEN (contract_amt) / 10.0 * 12 / 1000.0
          WHEN account_code = 'C' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'E' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 193 / 1000.0
          WHEN account_code = 'I' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'Z' THEN (contract_amt) / 10.0 * 233 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS083 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS084
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 72 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS084 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS085
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 342 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS085 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS086
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 342 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS086 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS087
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'K' THEN (contract_amt) / 10.0 * 12 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS087 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS088
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'C' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS088 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS089
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'E' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS089 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS090
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 193 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS090 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS091
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'I' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS091 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS092
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'Z' THEN (contract_amt) / 10.0 * 233 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS092 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO

--MS093
DELETE FROM MS_TMP
DELETE FROM MS_TMP1

INSERT INTO MS_TMP
SELECT IDN, NULL,
       SUM(CASE 
          WHEN account_code = 'C' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 150 / 1000.0
          WHEN account_code = 'C' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 100 / 1000.0
          WHEN account_code = 'E' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 150 / 1000.0
          WHEN account_code = 'E' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 100 / 1000.0          
          WHEN account_code = 'H' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 238 / 1000.0
          WHEN account_code = 'H' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 177 / 1000.0
          WHEN account_code = 'I' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 127 / 1000.0
          WHEN account_code = 'I' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 72 / 1000.0
          WHEN account_code = 'O' and ((account_code2 is null) or (account_code2 = '') or (account_code2 = 'N')) THEN (contract_amt) / 10.0 * 177 / 1000.0
          WHEN account_code = 'O' and account_code2 in ('S', 'W', 'M') THEN (contract_amt) / 10.0 * 238 / 1000.0
          WHEN account_code = 'K' THEN (contract_amt) / 10.0 * 17 / 1000.0
          WHEN account_code = 'Z' THEN (contract_amt) / 10.0 * 213 / 1000.0
        END), NULL, NULL
FROM BAM085_200405
GROUP BY IDN

UPDATE MS_200405
SET
MS093 = V1
FROM MS_TMP AS A
WHERE A.IDN = MS_200405.IDN
GO


