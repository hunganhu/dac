USE TF_KHJ
GO

/*----TF_KHJ----*/

/*----RT001----*/
/*---Init temp table---*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM RT_TMP
DELETE FROM RT_TMP1
/*---Starting making RT001---*/
INSERT INTO RT_TMP
SELECT DISTINCT IDN, NULL, NULL
FROM STM001_200404

INSERT INTO RT_TMP1
SELECT IDN, MAX(QUERY_MON_SINCE) 
FROM dbo.STM001_200404 AS A 
WHERE ITEM_LIST LIKE '%K%' 
GROUP BY A.IDN

UPDATE RT_TMP
SET
V1 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

DELETE FROM RT_TMP1

INSERT INTO RT_TMP1
SELECT A.IDN, MAX(QUERY_MON_SINCE) 
FROM dbo.STM001_200404 AS A, RT_TMP AS B 
WHERE A.IDN = B.IDN AND
      ITEM_LIST LIKE '%K%' AND
      QUERY_MON_SINCE < B.V1  
GROUP BY A.IDN

UPDATE RT_TMP
SET
V2 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

UPDATE RT_200404
SET
RT001 = 
(CASE
   WHEN (V1=0 OR V1 IS NULL) THEN 0
   WHEN (V2=0 OR V2 IS NULL) THEN 0
   ELSE  V1 - V2
END)
FROM RT_TMP AS A
WHERE A.IDN = RT_200404.IDN
go


/*----RT002----*/
/*---Init temp table---*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM RT_TMP
DELETE FROM RT_TMP1
/*---Starting making RT002---*/
INSERT INTO RT_TMP
SELECT DISTINCT IDN, NULL, NULL
FROM STM001_200404

INSERT INTO RT_TMP1
SELECT IDN, MAX(QUERY_MON_SINCE) 
FROM dbo.STM001_200404 AS A 
WHERE (ITEM_LIST LIKE '%B%' OR ITEM_LIST LIKE '%D%' OR ITEM_LIST LIKE '%J%') 
GROUP BY A.IDN

UPDATE RT_TMP
SET
V1 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

DELETE FROM RT_TMP1

INSERT INTO RT_TMP1
SELECT A.IDN, MAX(QUERY_MON_SINCE) 
FROM dbo.STM001_200404 AS A, RT_TMP AS B 
WHERE A.IDN = B.IDN AND
      (ITEM_LIST LIKE '%B%' OR ITEM_LIST LIKE '%D%' OR ITEM_LIST LIKE '%J%') AND
      QUERY_MON_SINCE < B.V1  
GROUP BY A.IDN

UPDATE RT_TMP
SET
V2 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

UPDATE RT_200404
SET
RT002 = 
(CASE
   WHEN (V1=0 OR V1 IS NULL) THEN 0
   WHEN (V2=0 OR V2 IS NULL) THEN 0
   ELSE  V1 - V2
END)
FROM RT_TMP AS A
WHERE A.IDN = RT_200404.IDN
go


/*----RT003----*/
/*---Init temp table---*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM RT_TMP
DELETE FROM RT_TMP1
/*---Starting making RT003---*/
INSERT INTO RT_TMP
SELECT DISTINCT IDN, NULL, NULL
FROM STM001_200404

INSERT INTO RT_TMP1
SELECT IDN, MAX(QUERY_MON_SINCE) 
FROM dbo.STM001_200404 AS A 
WHERE ITEM_LIST <> '' 
GROUP BY A.IDN

UPDATE RT_TMP
SET
V1 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

DELETE FROM RT_TMP1

INSERT INTO RT_TMP1
SELECT A.IDN, MAX(QUERY_MON_SINCE) 
FROM dbo.STM001_200404 AS A, RT_TMP AS B 
WHERE A.IDN = B.IDN AND
      ITEM_LIST <> '' AND
      QUERY_MON_SINCE < B.V1  
GROUP BY A.IDN

UPDATE RT_TMP
SET
V2 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

UPDATE RT_200404
SET
RT003 = 
(CASE
   WHEN (V1=0 OR V1 IS NULL) THEN 0
   WHEN (V2=0 OR V2 IS NULL) THEN 0
   ELSE  V1 - V2
END)
FROM RT_TMP AS A
WHERE A.IDN = RT_200404.IDN
go


