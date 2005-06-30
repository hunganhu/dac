USE TF_KHJ
GO

/*----TF_KHJ----*/

/*----FS029----*/
/*---Init temp table----*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 10
DELETE FROM FS_TMP
DELETE FROM FS_TMP1

/*---Start making FS029---*/
INSERT INTO FS_TMP
SELECT IDN, NULL, COUNT(*), NULL, NULL
FROM STM001_200410
WHERE Item_List like '%K%' AND 
      @Now - Query_Mon_Since <= 12
GROUP BY IDN  
 
UPDATE FS_200410
SET
FS029 = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200410.IDN
GO

/*----FS030----*/
/*---Init temp table----*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 10
DELETE FROM FS_TMP
DELETE FROM FS_TMP1

/*---Start making FS030---*/
INSERT INTO FS_TMP
SELECT IDN, NULL, COUNT(*), NULL, NULL
FROM STM001_200410
WHERE ((Item_List like '%B%') or
       (Item_List like '%D%') or
       (Item_List like '%J%')) AND 
      @Now - Query_Mon_Since <= 12
GROUP BY IDN  
 
UPDATE FS_200410
SET
FS030 = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200410.IDN
GO

/*----FS031----*/
/*---Init temp table----*/
DECLARE @NOW INT
SET @NOW = 93 * 12 + 10
DELETE FROM FS_TMP
DELETE FROM FS_TMP1

/*---Start making FS031---*/
INSERT INTO FS_TMP
SELECT IDN, NULL, COUNT(*), NULL, NULL
FROM STM001_200410
WHERE Item_List IS NOT NULL AND 
      Item_List <> '' AND 
      @Now - Query_Mon_Since <= 12
GROUP BY IDN   

UPDATE FS_200410
SET
FS031 = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200410.IDN
GO


UPDATE FS_200410
SET
FS030 = FS031 - FS029
GO

--FS027
DECLARE @NOW INT
SET @NOW = 93 * 12 + 10
DELETE FROM FS_TMP
DELETE FROM FS_TMP1

INSERT INTO FS_TMP
SELECT IDN, NULL, COUNT(*), NULL, NULL
FROM STM001_200410
WHERE bank_name like '%�H�Υd%' and
      @Now - Query_Mon_Since <= 12
GROUP BY IDN   

UPDATE FS_200410
SET
FS027 = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200410.IDN
GO


--FS028
UPDATE FS_200410
SET
FS028 = FS031 - FS027
GO