USE TF_KHJ
GO

/*----TF_KHJ----*/
/*----Init global variables----*/
DECLARE @i INT
DECLARE @NOW INT
DECLARE @Months INT
SET @NOW = 93 * 12 + 5
DELETE FROM Open_Issuer
DELETE FROM Lines

SET @i = 3

select @months = (@now - min(start_mon_since)) from KRM001_200405

SET @i = 0
WHILE @i<= @Months
BEGIN
  INSERT INTO Open_Issuer
  SELECT IDN, ISSUE, (@Now - @i)
  FROM KRM001_200405
  WHERE (End_Mon_Since > (@Now - @i)) AND
  (Start_Mon_Since < (@Now - @i)) /*AND
  ISSUE NOT IN ('021', '081', '974')*/
SET @i = @i + 1
END

INSERT INTO Lines
SELECT IDN, ISSUE, Start_Mon_Since, MAX(CONVERT(float,Limit))
FROM KRM001_200405 AS A
WHERE (@Now - Start_Mon_Since) <= 12 AND
      (@NOW - Start_Mon_Since) > 0 AND
      ISSUE NOT IN ('021', '081', '974') AND
   	  ISSUE NOT IN (SELECT DISTINCT Issuer 
     	              FROM Open_Issuer AS B
       	            WHERE A.IDN = B.IDN AND B.MON = A.Start_Mon_Since)
GROUP BY IDN, ISSUE, Start_Mon_Since
GO

/*----FS008----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--FS_TMP to store lines closed, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum newly closed lines for each customer on each issuer during specific time frame(month)--*/
/*--FS_TMP1 to store line closed for each customer during specific time frame--(3, 6, 9, 12 months)*/ 
DECLARE @i INT
DECLARE @NOW INT
DECLARE @Months INT
SET @NOW = 93 * 12 + 5
DELETE FROM FS_TMP 
DELETE FROM FS_TMP1
/*---Start making FS008---*/
SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM001_200405 AS A
  WHERE (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0 AND        
        Stop_Code = '3' AND  
        ISSUE NOT IN ('021', '081', '974') AND
      	ISSUE NOT IN (SELECT DISTINCT Issuer
        	            FROM Open_Issuer AS B
          	          WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON)  
  GROUP BY IDN
SET @i = @i + 3
END

INSERT INTO FS_TMP1
SELECT IDN, 13, COUNT(DISTINCT ISSUE)
FROM KRM001_200405 AS A
WHERE @Now - End_Mon_Since > 0 AND      
      Stop_Code = '3' AND  
      ISSUE NOT IN ('021', '081', '974') AND
      ISSUE NOT IN (SELECT DISTINCT Issuer
       	            FROM Open_Issuer AS B
                    WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON)  
      GROUP BY IDN

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT DISTINCT IDN, @i, 0, 0, 0
  FROM KRM001_200405
SET @i = @i + 3
END

INSERT INTO FS_TMP
SELECT DISTINCT IDN, 13, 0, 0, 0
FROM KRM001_200405

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.MON = FS_TMP.MON

DELETE FROM FS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(*)
  FROM KRM001_200405
  WHERE Stop_Code = '3' AND
        ISSUE IN ('021', '081', '974') AND
        (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0  
	GROUP BY IDN
SET @i = @i + 3
END

INSERT INTO FS_TMP1
SELECT IDN, 13, COUNT(*)
FROM KRM001_200405
WHERE Stop_Code = '3' AND
      ISSUE IN ('021', '081', '974') AND
      @Now - End_Mon_Since > 0
GROUP BY IDN

UPDATE FS_TMP
SET
V2 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.MON = FS_TMP.MON

UPDATE FS_200405
SET
FS008_3M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 3

UPDATE FS_200405
SET
FS008_6M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 6

UPDATE FS_200405
SET
FS008_9M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 9

UPDATE FS_200405
SET
FS008_12M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 12

UPDATE FS_200405
SET
FS008_12MPlus = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 13
GO


/*----FS009----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--FS_TMP to store lines closed, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum newly closed lines for each customer on each issuer during specific time frame(month)--*/
/*--FS_TMP1 to store line closed for each customer during specific time frame--(3, 6, 9, 12 months)*/ 
DECLARE @i INT
DECLARE @NOW INT
DECLARE @Months INT
SET @NOW = 93 * 12 + 5
DELETE FROM FS_TMP 
DELETE FROM FS_TMP1
/*---Start making FS009---*/
SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM001_200405 AS A
  WHERE (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0 AND         
        Stop_Code = '1' AND  
        ISSUE NOT IN ('021', '081', '974') AND
      	ISSUE NOT IN (SELECT DISTINCT Issuer
        	            FROM Open_Issuer AS B
          	          WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON)  
  GROUP BY IDN
SET @i = @i + 3
END

INSERT INTO FS_TMP1
SELECT IDN, 13, COUNT(DISTINCT ISSUE)
FROM KRM001_200405 AS A
WHERE @NOW - End_Mon_Since > 0 AND      
      Stop_Code = '1' AND  
      ISSUE NOT IN ('021', '081', '974') AND
      ISSUE NOT IN (SELECT DISTINCT Issuer
       	            FROM Open_Issuer AS B
                    WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON)  
      GROUP BY IDN

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT DISTINCT IDN, @i, 0, 0, 0
  FROM KRM001_200405
SET @i = @i + 3
END

INSERT INTO FS_TMP
SELECT DISTINCT IDN, 13, 0, 0, 0
FROM KRM001_200405

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.MON = FS_TMP.MON

DELETE FROM FS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(*)
  FROM KRM001_200405
  WHERE Stop_Code = '1' AND
        ISSUE IN ('021', '081', '974') AND
        (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0  
	GROUP BY IDN
SET @i = @i + 3
END

INSERT INTO FS_TMP1
SELECT IDN, 13, COUNT(*)
FROM KRM001_200405
WHERE Stop_Code = '1' AND
      ISSUE IN ('021', '081', '974') AND
      @NOW - End_Mon_Since > 0 
GROUP BY IDN

UPDATE FS_TMP
SET
V2 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.MON = FS_TMP.MON

UPDATE FS_200405
SET
FS009_3M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 3

UPDATE FS_200405
SET
FS009_6M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 6

UPDATE FS_200405
SET
FS009_9M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 9

UPDATE FS_200405
SET
FS009_12M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 12

UPDATE FS_200405
SET
FS009_12MPlus = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 13
GO


/*----FS012----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--FS_TMP to store lines closed, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum newly closed lines for each customer on each issuer during specific time frame(month)--*/
/*--FS_TMP1 to store line closed for each customer during specific time frame--(3, 6, 9, 12 months)*/ 
DECLARE @i INT
DECLARE @NOW INT
DECLARE @Months INT
SET @NOW = 93 * 12 + 5
DELETE FROM FS_TMP 
DELETE FROM FS_TMP1
/*---Start making FS012---*/
SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUE)
  FROM KRM001_200405 AS A
  WHERE (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0 AND        
        Stop_Code = '2' AND  
        ISSUE NOT IN ('021', '081', '974') AND
      	ISSUE NOT IN (SELECT DISTINCT Issuer
        	            FROM Open_Issuer AS B
          	          WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON)  
  GROUP BY IDN
SET @i = @i + 3
END

INSERT INTO FS_TMP1
SELECT IDN, 13, COUNT(DISTINCT ISSUE)
FROM KRM001_200405 AS A
WHERE @NOW - End_Mon_Since > 0 AND      
      Stop_Code = '2' AND  
      ISSUE NOT IN ('021', '081', '974') AND
      ISSUE NOT IN (SELECT DISTINCT Issuer
       	            FROM Open_Issuer AS B
                    WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON)  
      GROUP BY IDN

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT DISTINCT IDN, @i, 0, 0, 0
  FROM KRM001_200405
SET @i = @i + 3
END

INSERT INTO FS_TMP
SELECT DISTINCT IDN, 13, 0, 0, 0
FROM KRM001_200405

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.MON = FS_TMP.MON

DELETE FROM FS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(*)
  FROM KRM001_200405
  WHERE Stop_Code = '2' AND
        ISSUE IN ('021', '081', '974') AND
        (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0
	GROUP BY IDN
SET @i = @i + 3
END

INSERT INTO FS_TMP1
SELECT IDN, 13, COUNT(*)
FROM KRM001_200405
WHERE Stop_Code = '2' AND
      ISSUE IN ('021', '081', '974') AND
      @NOW - End_Mon_Since > 0 
GROUP BY IDN

UPDATE FS_TMP
SET
V2 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.MON = FS_TMP.MON

UPDATE FS_200405
SET
FS012_3M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 3

UPDATE FS_200405
SET
FS012_6M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 6

UPDATE FS_200405
SET
FS012_9M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 9

UPDATE FS_200405
SET
FS012_12M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 12

UPDATE FS_200405
SET
FS012_12MPlus = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 13
GO


/*----FS020----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--FS_TMP to store line increased, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum new lines for each customer on each issuer during specific time frame--*/
/*--FS_TMP1 to store total line increased for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
DECLARE @Months INT
SET @NOW = 93 * 12 + 5
DELETE FROM FS_TMP 
DELETE FROM FS_TMP1

/*---Start making FS020---*/
SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO FS_TMP1
	SELECT IDN, @i, COUNT(*)
	FROM Lines
	WHERE (@NOW - MON) <= @i
	GROUP BY IDN

SET @i = @i + 3
END


SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO FS_TMP
	SELECT DISTINCT IDN, @i, 0, 0, 0
	FROM KRM001_200405

SET @i = @i + 3
END

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.MON = FS_TMP.MON

DELETE FROM FS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO FS_TMP1
	SELECT IDN, @i, COUNT(*)
	FROM KRM001_200405
	WHERE ISSUE IN ('021', '081', '974') AND
  	    (@Now - Start_Mon_Since) <= @i AND
  	    (@Now - Start_Mon_Since) > 0
GROUP BY IDN

SET @i = @i + 3
END

UPDATE FS_TMP
SET
V2 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.MON = FS_TMP.MON

UPDATE FS_200405
SET
FS020_3M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN  AND MON = 3

UPDATE FS_200405
SET
FS020_6M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN  AND MON = 6

UPDATE FS_200405
SET
FS020_9M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN  AND MON = 9

UPDATE FS_200405
SET
FS020_12M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN  AND MON = 12
GO


/*----FS023----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--FS_TMP to store line increased, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum new lines for each customer on each issuer during specific time frame--*/
/*--FS_TMP1 to store total line increased for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
DECLARE @Months INT
SET @NOW = 93 * 12 + 5
DELETE FROM FS_TMP 
DELETE FROM FS_TMP1
/*---Start making FS023---*/
SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUER)
  FROM Open_Issuer
  WHERE (@NOW - Mon) <= @i AND
        (@Now - Mon) >= 0 AND
        Issuer NOT IN ('021', '081', '974')
  GROUP BY IDN
SET @i = @i + 3
END


SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT DISTINCT IDN, @i, 0, 0, 0
  FROM KRM001_200405
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.Mon = FS_TMP.MON

DELETE FS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(*)
  FROM KRM001_200405
  WHERE (@NOW - End_Mon_Since) <= @i AND
        (((@NOW - End_Mon_Since) > 0) OR (End_Mon_Since = 12000)) AND  
        Issue IN ('021', '081', '974')
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V2 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.Mon = FS_TMP.MON

DELETE FS_TMP1

UPDATE FS_200405
SET
FS023_3M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 3

UPDATE FS_200405
SET
FS023_6M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 6

UPDATE FS_200405
SET
FS023_9M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 9

UPDATE FS_200405
SET
FS023_12M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 12
GO


/*----FS024----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--FS_TMP to store line increased, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum new lines for each customer on each issuer during specific time frame--*/
/*--FS_TMP1 to store total line increased for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
DECLARE @Months INT
SET @NOW = 93 * 12 + 5
DELETE FROM FS_TMP 
DELETE FROM FS_TMP1
DELETE FROM Open_Issuer
/*---Start making FS024---*/
SET @i = 0
WHILE @i<=12
BEGIN
  INSERT INTO Open_Issuer
  SELECT IDN, ISSUE, (@Now - @i)
  FROM KRM001_200405
  WHERE (End_Mon_Since > (@Now - @i)) AND
  (Start_Mon_Since < (@Now - @i)) AND
  ISSUE NOT IN ('021', '081', '974') AND
  M_S = 'N'
SET @i = @i + 1
END

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUER)
  FROM Open_Issuer
  WHERE (@NOW - Mon) <= @i AND
        (@NOW - Mon) >= 0
  GROUP BY IDN
SET @i = @i + 3
END


SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT DISTINCT IDN, @i, 0, 0, 0
  FROM KRM001_200405
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.Mon = FS_TMP.MON

DELETE FS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(*)
  FROM KRM001_200405
  WHERE (@NOW - End_Mon_Since) <= @i AND
        (((@NOW - End_Mon_Since) > 0) OR (End_Mon_Since = 12000)) AND 
        Issue IN ('021', '081', '974') AND
        M_S = 'N'
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V2 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.Mon = FS_TMP.MON

UPDATE FS_200405
SET
FS024_3M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 3

UPDATE FS_200405
SET
FS024_6M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 6

UPDATE FS_200405
SET
FS024_9M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 9

UPDATE FS_200405
SET
FS024_12M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 12
GO


/*----FS025----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--FS_TMP to store line increased, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum new lines for each customer on each issuer during specific time frame--*/
/*--FS_TMP1 to store total line increased for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
DECLARE @Months INT
SET @NOW = 93 * 12 + 5
DELETE FROM FS_TMP 
DELETE FROM FS_TMP1
DELETE FROM Open_Issuer
/*---Start making FS025---*/
SET @i = 0
WHILE @i<=12
BEGIN
  INSERT INTO Open_Issuer
  SELECT IDN, ISSUE, (@Now - @i)
  FROM KRM001_200405
  WHERE (End_Mon_Since > (@Now - @i)) AND
  (Start_Mon_Since < (@Now - @i)) AND
  ISSUE NOT IN ('021', '081', '974') AND
  CONVERT(Float,Limit) > 100.0
SET @i = @i + 1
END

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUER)
  FROM Open_Issuer
  WHERE (@NOW - Mon) <= @i AND
        (@NOW - Mon) >= 0
  GROUP BY IDN
SET @i = @i + 3
END


SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT DISTINCT IDN, @i, 0, 0, 0
  FROM KRM001_200405
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.Mon = FS_TMP.MON

DELETE FS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(*)
  FROM KRM001_200405
  WHERE (@NOW - End_Mon_Since) <= @i AND
        (((@NOW - End_Mon_Since) > 0) OR (End_Mon_Since = 12000)) AND 
        Issue IN ('021', '081', '974') AND
        CONVERT(Float,Limit) > 100.0
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V2 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.Mon = FS_TMP.MON

UPDATE FS_200405
SET
FS025_3M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 3

UPDATE FS_200405
SET
FS025_6M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 6

UPDATE FS_200405
SET
FS025_9M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 9

UPDATE FS_200405
SET
FS025_12M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 12
GO


/*----FS032----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--FS_TMP to store line increased, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum new lines for each customer on each issuer during specific time frame--*/
/*--FS_TMP1 to store total line increased for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
DECLARE @Months INT
SET @NOW = 93 * 12 + 5
DELETE FROM FS_TMP 
DELETE FROM FS_TMP1
DELETE FROM Open_Issuer
/*---Start making FS032---*/
SET @i = 0
WHILE @i<=12
BEGIN
  INSERT INTO Open_Issuer
  SELECT IDN, ISSUE, (@Now - @i)
  FROM KRM001_200405
  WHERE (End_Mon_Since > (@Now - @i)) AND
  (Start_Mon_Since < (@Now - @i)) AND
  ISSUE NOT IN ('021', '081', '974') AND
  Secure = 'Y'
SET @i = @i + 1
END

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(DISTINCT ISSUER)
  FROM Open_Issuer
  WHERE (@NOW - Mon) <= @i AND
        (@NOW - Mon) > 0  
  GROUP BY IDN
SET @i = @i + 3
END


SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP
  SELECT DISTINCT IDN, @i, 0, 0, 0
  FROM KRM001_200405
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V1 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.Mon = FS_TMP.MON

DELETE FS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN
  INSERT INTO FS_TMP1
  SELECT IDN, @i, COUNT(*)
  FROM KRM001_200405
  WHERE (@NOW - End_Mon_Since) <= @i AND
        (((@NOW - End_Mon_Since) > 0) OR (End_Mon_Since = 12000)) AND
        Issue IN ('021', '081', '974') AND
        Secure = 'Y'
  GROUP BY IDN
SET @i = @i + 3
END

UPDATE FS_TMP
SET
V2 = A.V1
FROM FS_TMP1 AS A
WHERE A.IDN = FS_TMP.IDN AND
      A.Mon = FS_TMP.MON

UPDATE FS_200405
SET
FS032_3M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 3

UPDATE FS_200405
SET
FS032_6M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 6

UPDATE FS_200405
SET
FS032_9M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 9

UPDATE FS_200405
SET
FS032_12M = ISNULL(V1,0) + ISNULL(V2,0)
FROM FS_TMP AS A
WHERE A.IDN = FS_200405.IDN AND
      MON = 12
GO

