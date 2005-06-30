USE TF_KHJ
GO

/*----TF_KHJ----*/
/*----Init global variables----*/

/*----MS018----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--MS_TMP to store line increased, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum new lines for each customer on each issuer during specific time frame--*/
/*--MS_TMP1 to store total line increased for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM MS_TMP 
DELETE FROM Lines
DELETE FROM MS_TMP1
DELETE FROM Open_Issuer
/*---Start making MS018---*/
SET @i = 0
WHILE @i<=12
BEGIN
	
	INSERT INTO Open_Issuer
	SELECT IDN, ISSUE, (@Now - @i)
	FROM KRM001_200404
	WHERE (End_Mon_Since > (@Now - @i)) AND
        (Start_Mon_Since < (@Now - @i)) AND
		    ISSUE NOT IN ('021', '081', '974')

	SET @i = @i + 1
END

INSERT INTO Lines
SELECT IDN, ISSUE, Start_Mon_Since, MAX(CONVERT(float,Limit))
FROM KRM001_200404 AS A
WHERE (@Now - Start_Mon_Since) <= 12 AND
      (@NOW - Start_Mon_Since) > 0 AND  
      ISSUE NOT IN ('021', '081', '974') AND
   	  ISSUE NOT IN (SELECT DISTINCT Issuer 
     	              FROM Open_Issuer AS B
       	            WHERE A.IDN = B.IDN AND B.MON = A.Start_Mon_Since)
GROUP BY IDN, ISSUE, Start_Mon_Since


SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, SUM(Line)
	FROM Lines
	WHERE (@NOW - MON) <= @i
	GROUP BY IDN

SET @i = @i + 3
END


SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP
	SELECT DISTINCT IDN, @i, 0, 0, 0
	FROM KRM001_200404

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V1 = ISNULL(A.V1,0)
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

DELETE FROM MS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, SUM(CONVERT(float,Limit))
	FROM KRM001_200404
	WHERE ISSUE IN ('021', '081', '974') AND
  	    (@Now - Start_Mon_Since) <= @i AND
        (@NOW - Start_Mon_Since) > 0   
GROUP BY IDN

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V2 = ISNULL(A.V1,0)
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

UPDATE MS_200404
SET
MS018_3M = ISNULL(V1, 0) + ISNULL(V2, 0)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 3

UPDATE MS_200404
SET
MS018_6M = ISNULL(V1, 0) + ISNULL(V2, 0)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 6

UPDATE MS_200404
SET
MS018_9M = ISNULL(V1, 0) + ISNULL(V2, 0)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 9

UPDATE MS_200404
SET
MS018_12M = ISNULL(V1, 0) + ISNULL(V2, 0)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 12
GO


/*----MS019----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--Open_Issuer is created at MS018*/
/*--MS_TMP to store number of lines increased, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for mult-card-one-line*/
/*--MS_TMP1 to store number of lines increased for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM MS_TMP 
DELETE FROM Lines
DELETE FROM MS_TMP1
/*---Start making MS019---*/

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, COUNT(DISTINCT ISSUE)
	FROM KRM001_200404 AS A
	WHERE (@Now - Start_Mon_Since) <= @i AND
              (@NOW - Start_Mon_Since) > 0 AND 
  	     CONVERT(float, Limit) > 0 AND
    	  ISSUE NOT IN ('021', '081', '974') AND
      	ISSUE NOT IN (SELECT DISTINCT Issuer 
        	            FROM Open_Issuer AS B
          	          WHERE A.IDN = B.IDN AND A.Start_Mon_Since = B.MON)  
	GROUP BY IDN

SET @i = @i + 3
END

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP
	SELECT DISTINCT IDN, @i, 0, 0, 0
	FROM KRM001_200404

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V1 = CONVERT(FLOAT,ISNULL(A.V1,0))
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

DELETE FROM MS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, COUNT(*)
	FROM KRM001_200404
	WHERE CONVERT(float, Limit) > 0 AND
  	    ISSUE IN ('021', '081', '974') AND
  		  (@Now - Start_Mon_Since) <= @i AND
        (@NOW - Start_Mon_Since) > 0   
	GROUP BY IDN

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V2 = CONVERT(FLOAT,ISNULL(A.V1,0))
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

UPDATE MS_200404
SET
MS019_3M = (CASE WHEN (ISNULL(V1, 0) + ISNULL(V2, 0)) = 0 THEN 0 ELSE (MS018_3M / (V1+V2)) END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 3

UPDATE MS_200404
SET
MS019_6M = (CASE WHEN (ISNULL(V1, 0) + ISNULL(V2, 0)) = 0 THEN 0 ELSE (MS018_6M / (V1+V2)) END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 6

UPDATE MS_200404
SET
MS019_9M = (CASE WHEN (ISNULL(V1, 0) + ISNULL(V2, 0)) = 0 THEN 0 ELSE (MS018_9M / (V1+V2)) END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 9

UPDATE MS_200404
SET
MS019_12M = (CASE WHEN (ISNULL(V1, 0) + ISNULL(V2, 0)) = 0 THEN 0 ELSE (MS018_12M / (V1+V2)) END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 12
GO
/*Divide is redefined to be zero on Dec 29, 2003 in accordance with definition of 0 -> The ones no new line during particular period*/

/*----MS020----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--Open_Issuer is created at MS018--*/
/*--MS_TMP to store largest line increased, V2 for OO and V1 for MO--*/
/*--MS_TMP1 to store largest line increased for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM MS_TMP 
DELETE FROM Lines
DELETE FROM MS_TMP1
/*---Start making MS020---*/
SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, MAX(CONVERT(float,Limit))
	FROM KRM001_200404 AS A
	WHERE CONVERT(float,Limit) > 0 AND
        (@Now - Start_Mon_Since) <= @i AND
        (@NOW - Start_Mon_Since) > 0 AND 
  	    ISSUE NOT IN ('021', '081', '974') AND
    	  ISSUE NOT IN (SELECT DISTINCT Issuer
      	              FROM Open_Issuer AS B
        	            WHERE A.IDN = B.IDN AND A.Start_Mon_Since = B.MON)  
	GROUP BY IDN

SET @i = @i + 3
END

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP
	SELECT DISTINCT IDN, @i, 0, 0, 0
	FROM KRM001_200404

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V1 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

DELETE FROM MS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, MAX(CONVERT(float,Limit))
	FROM KRM001_200404
	WHERE CONVERT(float,Limit) > 0 AND 
        ISSUE IN ('021', '081', '974') AND
  	    (@Now - Start_Mon_Since) <= @i AND
        (@NOW - Start_Mon_Since) > 0   
	GROUP BY IDN

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V2 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

UPDATE MS_200404
SET
MS020_3M = (CASE WHEN V1>V2 THEN V1 ELSE V2 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 3

UPDATE MS_200404
SET
MS020_6M = (CASE WHEN V1>V2 THEN V1 ELSE V2 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 6

UPDATE MS_200404
SET
MS020_9M = (CASE WHEN V1>V2 THEN V1 ELSE V2 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 9

UPDATE MS_200404
SET
MS020_12M = (CASE WHEN V1>V2 THEN V1 ELSE V2 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 12
GO


/*----MS021----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--Open_Issuer is created at MS018--*/
/*--MS_TMP to store smallest line increased,V1 for OO and  V2 for MO--*/
/*--MS_TMP1 to store smallest line increased for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM MS_TMP 
DELETE FROM Lines
DELETE FROM MS_TMP1
/*---Start making MS021---*/
SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, MIN(CONVERT(float,Limit))
	FROM KRM001_200404 AS A
	WHERE CONVERT(float,Limit) > 0 AND
        (@Now - Start_Mon_Since) <= @i AND
        (@NOW - Start_Mon_Since) > 0  AND
  	    ISSUE NOT IN ('021', '081', '974') AND
    	  ISSUE NOT IN (SELECT DISTINCT Issuer 
      	              FROM Open_Issuer AS B
        	            WHERE A.IDN = B.IDN AND A.Start_Mon_Since = B.MON)  
	GROUP BY IDN

SET @i = @i + 3
END

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP
	SELECT DISTINCT IDN, @i, 0, 0, 0
	FROM KRM001_200404

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V1 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

DELETE FROM MS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, MIN(CONVERT(float,Limit))
	FROM KRM001_200404
	WHERE CONVERT(float,Limit) > 0 AND 
        ISSUE IN ('021', '081', '974') AND
  	    (@Now - Start_Mon_Since) <= @i AND
        (@NOW - Start_Mon_Since) > 0   
	GROUP BY IDN

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V2 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON


UPDATE MS_200404
SET
MS021_3M = (CASE WHEN ((V1 = 0) OR (V2 = 0)) THEN ISNULL(V1, 0) + ISNULL(V2, 0) WHEN V1>V2 THEN V2 ELSE V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 3

UPDATE MS_200404
SET
MS021_6M = (CASE WHEN ((V1 = 0) OR (V2 = 0)) THEN ISNULL(V1, 0) + ISNULL(V2, 0) WHEN V1>V2 THEN V2 ELSE V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 6

UPDATE MS_200404
SET
MS021_9M = (CASE WHEN ((V1 = 0) OR (V2 = 0)) THEN ISNULL(V1, 0) + ISNULL(V2, 0) WHEN V1>V2 THEN V2 ELSE V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 9

UPDATE MS_200404
SET
MS021_12M = (CASE WHEN ((V1 = 0) OR (V2 = 0)) THEN ISNULL(V1, 0) + ISNULL(V2, 0) WHEN V1>V2 THEN V2 ELSE V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 12
GO


/*----MS027----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--MS_TMP to store lines closed, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for Multi-card-one-line*/
/*--Lines to store maximum newly closed lines for each customer on each issuer during specific time frame(month)--*/
/*--MS_TMP1 to store line closed for each customer during specific time frame--(3, 6, 9, 12 months)*/ 
DECLARE @i INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM MS_TMP 
DELETE FROM Lines
DELETE FROM MS_TMP1
/*---Start making MS027---*/
INSERT INTO Lines
SELECT IDN, ISSUE, End_Mon_Since, MAX(CONVERT(float,Limit))
FROM KRM001_200404 AS A
WHERE Stop_Code = '1' AND
      (@Now - End_Mon_Since) <= 12 AND
      (@NOW - End_Mon_Since) > 0 AND
      ISSUE NOT IN ('021', '081', '974')
GROUP BY IDN, ISSUE, End_Mon_Since

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, SUM(Line)
	FROM Lines
	WHERE (@NOW - Mon) <= @i
	GROUP BY IDN

SET @i = @i + 3
END

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP
	SELECT DISTINCT IDN, @i, 0, 0, 0
	FROM KRM001_200404

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V1 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

DELETE FROM MS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, SUM(CONVERT(float,Limit))
	FROM KRM001_200404
	WHERE Stop_Code = '1' AND
        ISSUE IN ('021', '081', '974') AND
  	     (@Now - End_Mon_Since) <= @i AND
         (@NOW - End_Mon_Since) > 0
GROUP BY IDN

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V2 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

UPDATE MS_200404
SET
MS027_3M = ISNULL(V1, 0) + ISNULL(V2, 0)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 3

UPDATE MS_200404
SET
MS027_6M = ISNULL(V1, 0) + ISNULL(V2, 0)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 6

UPDATE MS_200404
SET
MS027_9M = ISNULL(V1, 0) + ISNULL(V2, 0)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 9

UPDATE MS_200404
SET
MS027_12M = ISNULL(V1, 0) + ISNULL(V2, 0)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 12
GO


/*----MS028----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--Open_Issuer is created at MS018*/
/*--MS_TMP to store number of lines closed, OO and MO--*/
/*OO stands for one-card-one-line, MO stands for mult-card-one-line*/
/*--MS_TMP1 to store number of line closed for each customer during specific time frame(month)--*/ 
DECLARE @i INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM MS_TMP 
DELETE FROM Lines
DELETE FROM MS_TMP1
/*---Start making MS028---*/

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, COUNT(DISTINCT ISSUE)
	FROM KRM001_200404 AS A
	WHERE (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0 AND
  	    CONVERT(float,Limit) > 0 AND
        Stop_Code = '1' AND  
    	  ISSUE NOT IN ('021', '081', '974') AND
      	ISSUE NOT IN (SELECT DISTINCT Issuer
        	            FROM Open_Issuer AS B
          	          WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON)  
	GROUP BY IDN

SET @i = @i + 3
END

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP
	SELECT DISTINCT IDN, @i, 0, 0, 0
	FROM KRM001_200404

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V1 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

DELETE FROM MS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, COUNT(*)
	FROM KRM001_200404
	WHERE CONVERT(float,Limit) > 0 AND
        Stop_Code = '1' AND
  	    ISSUE IN ('021', '081', '974') AND
  		  (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0  
	GROUP BY IDN

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V2 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

UPDATE MS_200404
SET
MS028_3M = (CASE WHEN (ISNULL(V1, 0) + ISNULL(V2, 0)) = 0 THEN 0 ELSE (MS027_3M / (V1+V2)) END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 3

UPDATE MS_200404
SET
MS028_6M = (CASE WHEN (ISNULL(V1, 0) + ISNULL(V2, 0)) = 0 THEN 0 ELSE (MS027_6M / (V1+V2)) END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 6

UPDATE MS_200404
SET
MS028_9M = (CASE WHEN (ISNULL(V1, 0) + ISNULL(V2, 0)) = 0 THEN 0 ELSE (MS027_9M / (V1+V2)) END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 9

UPDATE MS_200404
SET
MS028_12M = (CASE WHEN (ISNULL(V1, 0) + ISNULL(V2, 0)) = 0 THEN 0 ELSE (MS027_12M / (V1+V2)) END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 12
GO


/*----MS029----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--Open_Issuer is created at MS018--*/
/*--MS_TMP to store maximum line closed, OO and MO--*/
/*--MS_TMP1 to store maximum line closed for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM MS_TMP 
DELETE FROM Lines
DELETE FROM MS_TMP1
/*---Start making MS029---*/
SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, MAX(Limit)
	FROM KRM001_200404 AS A
	WHERE Stop_Code = '1' AND
        CONVERT(float,Limit) > 0 AND
        (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0 AND
  	    ISSUE NOT IN ('021', '081', '974') AND
    	  ISSUE NOT IN (SELECT DISTINCT Issuer
      	              FROM Open_Issuer AS B
        	            WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON)  
	GROUP BY IDN

SET @i = @i + 3
END

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP
	SELECT DISTINCT IDN, @i, 0, 0, 0
	FROM KRM001_200404

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V1 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

DELETE FROM MS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, MAX(CONVERT(float,Limit))
	FROM KRM001_200404
	WHERE Stop_Code = '1' AND 
        CONVERT(float,Limit) > 0 AND 
        ISSUE IN ('021', '081', '974') AND
  	    (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0  
	GROUP BY IDN

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V2 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

UPDATE MS_200404
SET
MS029_3M = (CASE WHEN V1>V2 THEN V1 ELSE V2 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 3

UPDATE MS_200404
SET
MS029_6M = (CASE WHEN V1>V2 THEN V1 ELSE V2 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 6

UPDATE MS_200404
SET
MS029_9M = (CASE WHEN V1>V2 THEN V1 ELSE V2 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 9

UPDATE MS_200404
SET
MS029_12M = (CASE WHEN V1>V2 THEN V1 ELSE V2 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 12
GO


/*----MS030----*/
/*---Init temp table---*/
/*--Open_Issuer to store all open cards which are not issued by Citi, AIG, and HSBC--*/
/*--Open_Issuer is created at MS018--*/
/*--MS_TMP to store minimum line closed, OO and MO--*/
/*--MS_TMP1 to store minimum line closed for each customer during specific time frame--*/ 
DECLARE @i INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
DELETE FROM MS_TMP 
DELETE FROM Lines
DELETE FROM MS_TMP1
/*---Start making MS030---*/
SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, MIN(CONVERT(float,Limit))
	FROM KRM001_200404 AS A
	WHERE Stop_Code = '1' AND
        CONVERT(float,Limit) > 0 AND
        (@Now - End_Mon_Since) <= @i  AND
        (@NOW - End_Mon_Since) > 0 AND
  	    ISSUE NOT IN ('021', '081', '974') AND
    	  ISSUE NOT IN (SELECT DISTINCT Issuer 
      	              FROM Open_Issuer AS B
        	            WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON)  
	GROUP BY IDN

SET @i = @i + 3
END

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP
	SELECT DISTINCT IDN, @i, 0, 0, 0
	FROM KRM001_200404

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V1 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

DELETE FROM MS_TMP1

SET @i = 3
WHILE @i<=12
BEGIN

	INSERT INTO MS_TMP1
	SELECT IDN, @i, MIN(CONVERT(float,Limit))
	FROM KRM001_200404
	WHERE Stop_Code = '1' AND 
        CONVERT(float, Limit) > 0 AND 
        ISSUE IN ('021', '081', '974') AND
  	    (@Now - End_Mon_Since) <= @i AND
        (@NOW - End_Mon_Since) > 0
	GROUP BY IDN

SET @i = @i + 3
END

UPDATE MS_TMP
SET
V2 = A.V1
FROM MS_TMP1 AS A
WHERE A.IDN = MS_TMP.IDN AND
      A.MON = MS_TMP.MON

UPDATE MS_200404
SET
MS030_3M = (CASE WHEN ((V1 = 0) OR (V2 = 0)) THEN ISNULL(V1, 0) + ISNULL(V2, 0) WHEN V1>V2 THEN V2 ELSE V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 3

UPDATE MS_200404
SET
MS030_6M = (CASE WHEN ((V1 = 0) OR (V2 = 0)) THEN ISNULL(V1, 0) + ISNULL(V2, 0) WHEN V1>V2 THEN V2 ELSE V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 6

UPDATE MS_200404
SET
MS030_9M = (CASE WHEN ((V1 = 0) OR (V2 = 0)) THEN ISNULL(V1, 0) + ISNULL(V2, 0) WHEN V1>V2 THEN V2 ELSE V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 9

UPDATE MS_200404
SET
MS030_12M = (CASE WHEN ((V1 = 0) OR (V2 = 0)) THEN ISNULL(V1, 0) + ISNULL(V2, 0) WHEN V1>V2 THEN V2 ELSE V1 END)
FROM MS_TMP AS A
WHERE A.IDN = MS_200404.IDN  AND MON = 12
GO