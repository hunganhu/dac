USE TF_KHJ
GO
/*----200411----*/

/*----RT008----*/
/*---Init temp table---*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 11
DELETE FROM RT_TMP
DELETE FROM RT_TMP1
/*---Starting making RT008---*/
INSERT INTO RT_TMP
SELECT DISTINCT IDN, 0, 0
FROM KRM023_200411

INSERT INTO RT_TMP1
SELECT IDN, MAX(Mon_Since) 
FROM dbo.KRM023_200411 AS A 
WHERE Bucket_def_1K > 0
GROUP BY A.IDN

UPDATE RT_TMP
SET
V1 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

DELETE FROM RT_TMP1

INSERT INTO RT_TMP1
SELECT A.IDN, MAX(Mon_Since) 
FROM dbo.KRM023_200411 AS A, RT_TMP AS B 
WHERE A.IDN = B.IDN AND
      Bucket_def_1K > 0 AND
      Mon_Since < B.V1  
GROUP BY A.IDN

UPDATE RT_TMP
SET
V2 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

UPDATE RT_200411
SET
RT008_1K =
(CASE
   WHEN V1=0 THEN 0
   WHEN V2=0 THEN 0 
   ELSE  V1 - V2
END)
FROM RT_TMP AS A
WHERE A.IDN = RT_200411.IDN
go


/*----RT009----*/
/*---Init temp table---*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 11
DELETE FROM RT_TMP
DELETE FROM RT_TMP1
/*---Starting making RT009---*/
INSERT INTO RT_TMP
SELECT DISTINCT IDN, 0, 0
FROM KRM023_200411

INSERT INTO RT_TMP1
SELECT IDN, MAX(Mon_Since) 
FROM dbo.KRM023_200411 AS A 
WHERE Bucket_ef_1K > 0
GROUP BY A.IDN

UPDATE RT_TMP
SET
V1 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

DELETE FROM RT_TMP1

INSERT INTO RT_TMP1
SELECT A.IDN, MAX(Mon_Since) 
FROM dbo.KRM023_200411 AS A, RT_TMP AS B 
WHERE A.IDN = B.IDN AND
      Bucket_ef_1K > 0 AND
      Mon_Since < B.V1  
GROUP BY A.IDN

UPDATE RT_TMP
SET
V2 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

UPDATE RT_200411
SET
RT009_1K = 
(CASE
   WHEN V1=0 THEN 0
   WHEN V2=0 THEN 0 
   ELSE  V1 - V2
END)
FROM RT_TMP AS A
WHERE A.IDN = RT_200411.IDN
go


/*----RT010----*/
/*---Init temp table---*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 11
DELETE FROM RT_TMP
DELETE FROM RT_TMP1
/*---Starting making RT010---*/
INSERT INTO RT_TMP
SELECT DISTINCT IDN, 0, 0
FROM KRM023_200411

INSERT INTO RT_TMP1
SELECT IDN, MAX(Mon_Since) 
FROM dbo.KRM023_200411 AS A 
WHERE Bucket_f_1K > 0
GROUP BY A.IDN

UPDATE RT_TMP
SET
V1 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

DELETE FROM RT_TMP1

INSERT INTO RT_TMP1
SELECT A.IDN, MAX(Mon_Since) 
FROM dbo.KRM023_200411 AS A, RT_TMP AS B 
WHERE A.IDN = B.IDN AND
      Bucket_f_1K > 0 AND
      Mon_Since < B.V1  
GROUP BY A.IDN

UPDATE RT_TMP
SET
V2 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

UPDATE RT_200411
SET
RT010_1K = 
(CASE
   WHEN V1=0 THEN 0
   WHEN V2=0 THEN 0 
   ELSE  V1 - V2
END)
FROM RT_TMP AS A
WHERE A.IDN = RT_200411.IDN
go


/*----RT011----*/
/*---Init temp table---*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 11
DELETE FROM RT_TMP
DELETE FROM RT_TMP1
/*---Starting making RT011---*/
INSERT INTO RT_TMP
SELECT DISTINCT IDN, 0, 0
FROM KRM023_200411

INSERT INTO RT_TMP1
SELECT IDN, MAX(Mon_Since) 
FROM dbo.KRM023_200411 AS A 
WHERE CASH = 'Y'
GROUP BY A.IDN

UPDATE RT_TMP
SET
V1 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

DELETE FROM RT_TMP1

INSERT INTO RT_TMP1
SELECT A.IDN, MAX(Mon_Since) 
FROM dbo.KRM023_200411 AS A, RT_TMP AS B 
WHERE A.IDN = B.IDN AND
      CASH = 'Y' AND
      Mon_Since < B.V1  
GROUP BY A.IDN

UPDATE RT_TMP
SET
V2 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

UPDATE RT_200411
SET
RT011 = 
(CASE
   WHEN V1=0 THEN 0
   WHEN V2=0 THEN 0 
   ELSE  V1 - V2
END)
FROM RT_TMP AS A
WHERE A.IDN = RT_200411.IDN
go

