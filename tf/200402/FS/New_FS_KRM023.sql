use TF_KHJ
go

drop table FS1xx_200402
GO

CREATE TABLE FS1xx_200402
(IDN Char(6),
FS101_3M Int,	FS101_6M Int,	FS101_9M Int,	FS101_12M Int,
FS102_3M Int,	FS102_6M Int,	FS102_9M Int,	FS102_12M Int,
FS103_3M float,	FS103_6M float,	FS103_9M float,	FS103_12M float,
FS104_3M float,	FS104_6M float,	FS104_9M float,	FS104_12M float,
FS105_3M float,	FS105_6M float,	FS105_9M float,	FS105_12M float,
FS106_3M float,	FS106_6M float,	FS106_9M float,	FS106_12M float,
FS107_3M float,	FS107_6M float,	FS107_9M float,	FS107_12M float,
FS108_3M Int,	FS108_6M Int,	FS108_9M Int,	FS108_12M Int)

INSERT INTO FS1xx_200402
SELECT
DISTINCT IDN,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0
FROM BASE_200402
WHERE AVAIL_FLAG & 0x01 = 0x01
GO

drop table fs_tmp, fs_tmp1, fs_issue
GO
CREATE TABLE FS_TMP
(IDN Char(6),
 MON Int,
 V1 float)
go

CREATE TABLE FS_TMP1
(IDN Char(6),
 MON Int,
 V1 float)
go

CREATE TABLE FS_Issue
(IDN Char(6),
 MON Int,
 Issue Char(3))
GO


/*----TF_KHJ----*/

/*----FS101----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 2
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS101---*/
WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200402
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE FS1xx_200402
SET
FS101_3M = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=3

UPDATE FS1xx_200402
SET
FS101_6M = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=6

UPDATE FS1xx_200402
SET
FS101_9M = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=9

UPDATE FS1xx_200402
SET
FS101_12M = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=12
GO

/*----FS102----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 2
DELETE FROM FS_Issue
DELETE FROM FS_TMP
DELETE FROM FS_TMP1

SET @i = 3
/*---Start making FS102---*/
INSERT INTO FS_Issue
SELECT IDN, Mon_Since, Issue     
FROM KRM023_200402
WHERE Limit > 100

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT DISTINCT IDN, @i, 0
  FROM KRM023_200402
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END

SET @i = 3
WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP1 
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM FS_Issue
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0   
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      FS_TMP.MON = A.MON


UPDATE FS1xx_200402
SET
FS102_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=3

UPDATE FS1xx_200402
SET
FS102_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=6

UPDATE FS1xx_200402
SET
FS102_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=9

UPDATE FS1xx_200402
SET
FS102_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=12
GO


/*----FS103----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 2
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS103---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, SUM(CASE
                             WHEN Pay_Code IN ('A', 'B') THEN 1
                             ELSE 0
                           END)
FROM KRM023_200402
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, AVG(V1)
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE FS1xx_200402
SET
FS103_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=3

UPDATE FS1xx_200402
SET
FS103_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=6

UPDATE FS1xx_200402
SET
FS103_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=9

UPDATE FS1xx_200402
SET
FS103_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=12
GO


/*----FS104----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 2
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS104---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, SUM(CASE
                             WHEN Pay_Code = 'X' THEN 1
                             ELSE 0
                           END)
FROM KRM023_200402
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, AVG(V1)
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE FS1xx_200402
SET
FS104_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=3

UPDATE FS1xx_200402
SET
FS104_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=6

UPDATE FS1xx_200402
SET
FS104_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=9

UPDATE FS1xx_200402
SET
FS104_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=12
GO


/*----FS105----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 2
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS105---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, SUM(CASE
                             WHEN Cash = 'Y' THEN 1
                             ELSE 0
                           END)
FROM KRM023_200402
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, AVG(V1)
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE FS1xx_200402
SET
FS105_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=3

UPDATE FS1xx_200402
SET
FS105_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=6

UPDATE FS1xx_200402
SET
FS105_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=9

UPDATE FS1xx_200402
SET
FS105_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=12
GO


/*----FS106----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 2
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS106---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, SUM(CASE
                             WHEN Pay_Code IN ('C', 'D', 'E', 'F') THEN 1
                             ELSE 0
                           END)
FROM KRM023_200402
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, AVG(V1)
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE FS1xx_200402
SET
FS106_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=3

UPDATE FS1xx_200402
SET
FS106_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=6

UPDATE FS1xx_200402
SET
FS106_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=9

UPDATE FS1xx_200402
SET
FS106_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=12
GO


/*----FS107----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 2
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS107---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, SUM(CASE
                             WHEN Pay_Code IN ('C', 'D') THEN 1
                             ELSE 0
                           END)
FROM KRM023_200402
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, AVG(V1)
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0 
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE FS1xx_200402
SET
FS107_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=3

UPDATE FS1xx_200402
SET
FS107_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=6

UPDATE FS1xx_200402
SET
FS107_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=9

UPDATE FS1xx_200402
SET
FS107_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=12
GO


/*----FS108----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 2
DELETE FROM FS_Issue
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS108---*/
INSERT INTO FS_Issue
SELECT IDN, Mon_Since, issue
FROM KRM023_200402
WHERE Pay_Code IN ('C', 'D')

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT DISTINCT IDN, @i, 0
  FROM KRM023_200402
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 
  GROUP BY IDN  
 
SET @i = @i + 3
END


SET @i = 3
WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM FS_Issue
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0    
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      FS_TMP.MON = A.MON


UPDATE FS1xx_200402
SET
FS108_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=3

UPDATE FS1xx_200402
SET
FS108_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=6

UPDATE FS1xx_200402
SET
FS108_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=9

UPDATE FS1xx_200402
SET
FS108_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS1xx_200402.IDN AND Mon=12
GO