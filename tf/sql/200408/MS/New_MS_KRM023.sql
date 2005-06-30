USE TF_KHJ
GO

DROP TABLE MS1xx_200408
GO

CREATE TABLE MS1xx_200408
(IDN Char(6), 
MS101_3M float,	MS101_6M float,	MS101_9M float,	MS101_12M float,
MS102_3M float,	MS102_6M float,	MS102_9M float,	MS102_12M float,
MS103_3M float,	MS103_6M float,	MS103_9M float,	MS103_12M float,
MS104_3M float,	MS104_6M float,	MS104_9M float,	MS104_12M float,
MS105_3M float,	MS105_6M float,	MS105_9M float,	MS105_12M float,
MS106_3M float,	MS106_6M float,	MS106_9M float,	MS106_12M float,
MS107_3M float,	MS107_6M float,	MS107_9M float,	MS107_12M float,
MS108_3M float,	MS108_6M float,	MS108_9M float,	MS108_12M float,
MS109_3M float,	MS109_6M float,	MS109_9M float,	MS109_12M float,
MS110_3M float,	MS110_6M float,	MS110_9M float,	MS110_12M float,
MS111_3M float,	MS111_6M float,	MS111_9M float,	MS111_12M float,
MS112_3M float,	MS112_6M float,	MS112_9M float,	MS112_12M float,
MS113_3M float,	MS113_6M float,	MS113_9M float,	MS113_12M float,
MS114_3M float,	MS114_6M float,	MS114_9M float,	MS114_12M float,
MS115_3M float,	MS115_6M float,	MS115_9M float,	MS115_12M float,
MS116_3M float,	MS116_6M float,	MS116_9M float,	MS116_12M float,
MS117_3M float,	MS117_6M float,	MS117_9M float,	MS117_12M float,
MS118_3M float,	MS118_6M float,	MS118_9M float,	MS118_12M float,
MS119_3M float,	MS119_6M float,	MS119_9M float,	MS119_12M float,
MS120_3M float,	MS120_6M float,	MS120_9M float,	MS120_12M float,
MS121_3M float,	MS121_6M float,	MS121_9M float,	MS121_12M float,
MS122_3M float,	MS122_6M float,	MS122_9M float,	MS122_12M float,
MS123_3M float,	MS123_6M float,	MS123_9M float,	MS123_12M float,
MS124_3M float,	MS124_6M float,	MS124_9M float,	MS124_12M float,
MS125_3M float,	MS125_6M float,	MS125_9M float,	MS125_12M float)
GO

INSERT INTO MS1xx_200408
SELECT DISTINCT IDN,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0
FROM BASE_200408 AS A
WHERE AVAIL_FLAG & 0x01 = 0x01
GO
/*6150*/

/*----TF_KHJ----*/

/*----MS101----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS101---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, SUM(Payment_Amt), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i  AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('C', 'D') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS101_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS101_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS101_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS101_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO

/*----MS102----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS102---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, SUM(Payment_Amt), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i  AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('C', 'D', 'E', 'F') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS102_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS102_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS102_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS102_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS103----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS103---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, SUM(Payment_Amt), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i  AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('A', 'B', 'C', 'D') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS103_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS103_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS103_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS103_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS104----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS104---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, SUM(Payment_Amt), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i  AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('A', 'B', 'C') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS104_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS104_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS104_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS104_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS105----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS105---*/
INSERT INTO MS_TMP1
SELECT IDN, Mon_Since, SUM(CASE
                             WHEN Pay_Code IN ('C', 'D') THEN Payment_amt
                             ELSE 0
                           END)
FROM KRM023_200408
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, AVG(V1), NULL, NULL
  FROM MS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE MS1xx_200408
SET
MS105_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS105_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS105_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS105_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS106----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS106---*/
INSERT INTO MS_TMP1
SELECT IDN, Mon_Since, SUM(CASE
                             WHEN Pay_Code IN ('C', 'D', 'E', 'F') THEN Payment_amt
                             ELSE 0
                           END)
FROM KRM023_200408
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, AVG(V1), NULL, NULL
  FROM MS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE MS1xx_200408
SET
MS106_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS106_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS106_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS106_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS107----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS107---*/
INSERT INTO MS_TMP1
SELECT IDN, Mon_Since, SUM(CASE
                             WHEN Pay_Code IN ('A', 'B', 'C', 'D') THEN Payment_amt
                             ELSE 0
                           END)
FROM KRM023_200408
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, AVG(V1), NULL, NULL
  FROM MS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE MS1xx_200408
SET
MS107_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS107_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS107_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS107_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS108----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS108---*/
INSERT INTO MS_TMP1
SELECT IDN, Mon_Since, SUM(CASE
                             WHEN Pay_Code IN ('A', 'B', 'C') THEN Payment_amt
                             ELSE 0
                           END)
FROM KRM023_200408
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, AVG(V1), NULL, NULL
  FROM MS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END


UPDATE MS1xx_200408
SET
MS108_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS108_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS108_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS108_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS109----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS109---*/
INSERT INTO MS_TMP1
SELECT IDN, Mon_Since, SUM(CONVERT(float, Limit))
FROM KRM023_200408
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MAX(V1), NULL, NULL
  FROM MS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS109_3M = (CASE WHEN V1=0 THEN NULL ELSE MS105_3M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS109_6M = (CASE WHEN V1=0 THEN NULL ELSE MS105_6M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS109_9M = (CASE WHEN V1=0 THEN NULL ELSE MS105_9M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS109_12M = (CASE WHEN V1=0 THEN NULL ELSE MS105_12M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS110----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS110---*/
INSERT INTO MS_TMP1
SELECT IDN, Mon_Since, SUM(CONVERT(float, Limit))
FROM KRM023_200408
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MAX(V1), NULL, NULL
  FROM MS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS110_3M = (CASE WHEN V1=0 THEN NULL ELSE MS106_3M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS110_6M = (CASE WHEN V1=0 THEN NULL ELSE MS106_6M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS110_9M = (CASE WHEN V1=0 THEN NULL ELSE MS106_9M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS110_12M = (CASE WHEN V1=0 THEN NULL ELSE MS106_12M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS111----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS111---*/
INSERT INTO MS_TMP1
SELECT IDN, Mon_Since, SUM(CONVERT(float, Limit))
FROM KRM023_200408
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MAX(V1), NULL, NULL
  FROM MS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS111_3M = (CASE WHEN V1=0 THEN NULL ELSE MS107_3M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS111_6M = (CASE WHEN V1=0 THEN NULL ELSE MS107_6M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS111_9M = (CASE WHEN V1=0 THEN NULL ELSE MS107_9M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS111_12M = (CASE WHEN V1=0 THEN NULL ELSE MS107_12M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS112----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS112---*/
INSERT INTO MS_TMP1
SELECT IDN, Mon_Since, SUM(CONVERT(float, Limit))
FROM KRM023_200408
GROUP BY IDN, Mon_Since 

WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MAX(V1), NULL, NULL
  FROM MS_TMP1
  WHERE (@NOW - Mon)<= @i AND
        (@NOW - Mon) > 0 
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS112_3M = (CASE WHEN V1=0 THEN NULL ELSE MS108_3M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS112_6M = (CASE WHEN V1=0 THEN NULL ELSE MS108_6M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS112_9M = (CASE WHEN V1=0 THEN NULL ELSE MS108_9M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS112_12M = (CASE WHEN V1=0 THEN NULL ELSE MS108_12M/V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS113----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS113---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MAX(Payment_amt/CONVERT(float,Limit)), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i  AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('C', 'D') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS113_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS113_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS113_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS113_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS114----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS114---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MAX(Payment_amt/CONVERT(float,Limit)), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i  AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('C', 'D', 'E', 'F') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS114_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS114_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS114_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS114_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS115----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS115---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MAX(Payment_amt/CONVERT(float,Limit)), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i  AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('C', 'D', 'A', 'B') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS115_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS115_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS115_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS115_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS116----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS116---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MAX(Payment_amt/CONVERT(float,Limit)), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('A', 'B', 'C') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS116_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS116_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS116_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS116_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS117----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS117---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MIN(Payment_amt/CONVERT(float,Limit)), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('C', 'D') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS117_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS117_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS117_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS117_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS118----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS118---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MIN(Payment_amt/CONVERT(float,Limit)), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('C', 'D', 'E', 'F') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS118_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS118_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS118_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS118_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS119----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS119---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MIN(Payment_amt/CONVERT(float,Limit)), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('A', 'B', 'C', 'D') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS119_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS119_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS119_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS119_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS120----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS120---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP
  SELECT IDN, @i, MIN(Payment_amt/CONVERT(float,Limit)), NULL, NULL
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0 AND
        Pay_Code IN ('A', 'B', 'C') AND
        Limit <> 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS120_3M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS120_6M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS120_9M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS120_12M = V1
FROM MS_TMP AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO


/*----MS121----*/
UPDATE MS1xx_200408
SET
MS121_3M = MS113_3M - MS117_3M,
MS121_6M = MS113_6M - MS117_6M,
MS121_9M = MS113_9M - MS117_9M,
MS121_12M = MS113_12M - MS117_12M
GO


/*----MS122----*/
UPDATE MS1xx_200408
SET
MS122_3M = MS114_3M - MS118_3M,
MS122_6M = MS114_6M - MS118_6M,
MS122_9M = MS114_9M - MS118_9M,
MS122_12M = MS114_12M - MS118_12M
GO


/*----MS123----*/
UPDATE MS1xx_200408
SET
MS123_3M = MS115_3M - MS119_3M,
MS123_6M = MS115_6M - MS119_6M,
MS123_9M = MS115_9M - MS119_9M,
MS123_12M = MS115_12M - MS119_12M
GO


/*----MS124----*/
UPDATE MS1xx_200408
SET
MS124_3M = MS116_3M - MS120_3M,
MS124_6M = MS116_6M - MS120_6M,
MS124_9M = MS116_9M - MS120_9M,
MS124_12M = MS116_12M - MS120_12M
GO


/*----MS125----*/
/*---Init temp table----*/
DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 93 * 12 + 8
DELETE FROM MS_TMP
DELETE FROM MS_TMP1
SET @i = 3
/*---Start making MS125---*/
WHILE @i<=12
BEGIN

  INSERT INTO MS_TMP1
  SELECT IDN, @i, SUM(Payment_amt)
  FROM KRM023_200408
  WHERE (@NOW - Mon_Since)<= @i AND
        (@NOW - Mon_Since) > 0
  GROUP BY IDN  
 
SET @i = @i + 3
END

UPDATE MS1xx_200408
SET
MS125_3M = V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=3

UPDATE MS1xx_200408
SET
MS125_6M = V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=6

UPDATE MS1xx_200408
SET
MS125_9M = V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=9

UPDATE MS1xx_200408
SET
MS125_12M = V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS1xx_200408.IDN AND Mon=12
GO