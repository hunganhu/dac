USE TF_KHJ
go

/*----TF_KHJ----*/

/*----FS002----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS002---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE Bucket_ef_1K >=1
GROUP BY IDN, Mon_Since

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN 
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS002_3M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS002_6M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS002_9M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS002_12M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO

/*----FS003----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS002---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, 1
FROM KRM023_200408 
WHERE Bucket_f_1K >=1
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0 
  GROUP BY IDN 
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS003_3M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS003_6M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS003_9M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS003_12M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS004----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS004---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE Bucket_ef_1K >=2
GROUP BY IDN, Mon_Since

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS004_3M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS004_6M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS004_9M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS004_12M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS005----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS005---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE Bucket_f_1K >=2
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS005_3M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS005_6M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS005_9M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS005_12M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS006----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS006---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE Bucket_ef_1K >=3
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS006_3M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS006_6M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS006_9M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS006_12M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS007----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS007---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE Bucket_f_1K >=3
GROUP BY IDN, Mon_Since

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS007_3M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS007_6M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS007_9M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS007_12M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS056----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS056---*/
INSERT INTO FS_TMP1 
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE Pay_Code IN ('D', 'E', 'F') AND
      Bucket_def_1K >=1
GROUP BY IDN, Mon_Since  

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0 
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS056_3M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS056_6M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS056_9M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS056_12M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS057----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS057---*/
INSERT INTO FS_TMP1 
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE Pay_Code IN ('D', 'E', 'F') AND
      Bucket_def_1K >=2
GROUP BY IDN, Mon_Since  

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS057_3M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS057_6M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS057_9M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS057_12M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS058----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS058---*/
INSERT INTO FS_TMP1 
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE Pay_Code IN ('D', 'E', 'F') AND
      Bucket_def_1K >=3
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS058_3M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS058_6M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS058_9M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS058_12M_1k = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS059----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS059---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200408
  WHERE Bucket_ef_1K >=1 AND
  (@NOW - Mon_Since)<= @i AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS059_3M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS059_6M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS059_9M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS059_12M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS060----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS060---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200408
  WHERE Bucket_f_1K >=1 AND
  (@NOW - Mon_Since)<= @i AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS060_3M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS060_6M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS060_9M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS060_12M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS062----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS062---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200408
  WHERE Bucket_ef_1K >=2 AND
  (@NOW - Mon_Since)<= @i AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS062_3M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS062_6M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS062_9M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS062_12M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS063----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS063---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200408
  WHERE Bucket_f_1K >=2 AND
  (@NOW - Mon_Since)<= @i AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS063_3M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS063_6M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS063_9M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS063_12M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS065----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS065---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200408
  WHERE Bucket_ef_1K >=3 AND
  (@NOW - Mon_Since)<= @i AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS065_3M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS065_6M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS065_9M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS065_12M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS066----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS066---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200408
  WHERE Bucket_f_1K >=3 AND
  (@NOW - Mon_Since)<= @i AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS066_3M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS066_6M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS066_9M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS066_12M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS061----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS061---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200408
  WHERE Bucket_def_1K >=1 AND
  (@NOW - Mon_Since)<= @i AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS061_3M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS061_6M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS061_9M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS061_12M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS064----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS064---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200408
  WHERE Bucket_def_1K >=2 AND
  (@NOW - Mon_Since)<= @i AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS064_3M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS064_6M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS064_9M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS064_12M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS067----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS058---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM023_200408
  WHERE Bucket_def_1K >=3 AND
  (@NOW - Mon_Since)<= @i AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS067_3M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS067_6M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS067_9M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS067_12M_1k = V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS014----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS014---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT IDN, @i, COUNT(DISTINCT ISSUE), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('A', 'B')
  GROUP BY IDN 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS014_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS014_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS014_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS014_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS015----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS015---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT IDN, @i, COUNT(DISTINCT ISSUE), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code = 'X'
         
  GROUP BY IDN 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS015_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS015_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS015_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS015_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS016----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS016---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT IDN, @i, COUNT(DISTINCT ISSUE), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 AND 
        CASH = 'Y'         
  GROUP BY IDN 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS016_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS016_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS016_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS016_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS017----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS017---*/
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT IDN, @i, COUNT(DISTINCT ISSUE), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('C', 'D', 'E', 'F')         
  GROUP BY IDN 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS017_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS017_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS017_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS017_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS018----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS018---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE CASH = 'Y'
GROUP BY IDN, Mon_Since

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0 
  GROUP BY IDN 
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS018_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS018_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS018_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS018_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO


/*----FS021----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM FS_TMP
DELETE FROM FS_TMP1
SET @i = 3
/*---Start making FS021---*/
INSERT INTO FS_TMP1
SELECT IDN, Mon_Since, 1
FROM KRM023_200408
WHERE Pay_Code IN ('C', 'D', 'E', 'F')
GROUP BY IDN, Mon_Since

WHILE @i<=12
BEGIN

  INSERT INTO FS_TMP
  SELECT IDN, @i, SUM(V1), NULL, NULL
  FROM FS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0 
  GROUP BY IDN 
 
SET @i = @i + 3
END

UPDATE FS_200408
SET
FS021_3M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=3

UPDATE FS_200408
SET
FS021_6M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=6

UPDATE FS_200408
SET
FS021_9M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=9

UPDATE FS_200408
SET
FS021_12M = V1
FROM FS_TMP AS A
WHERE A.IDN = FS_200408.IDN AND Mon=12
GO