USE TF_KHJ
GO

/*----Prepare Open_Issuer table for RS variables 010, 018, 022, 024----*/
/*----This table is extended to the open date of the first card of each person----*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 93 * 12 + 12
DELETE FROM Open_Issuer
SET @j = @Now - (SELECT MIN(StaRT_Mon_Since) FROM KRM001_200412)
SET @i = 0
WHILE @i<= @j
BEGIN
	
	INSERT INTO Open_Issuer
	SELECT IDN, ISSUE, (@Now - @i)
	FROM KRM001_200412
	WHERE (End_Mon_Since > (@Now - @i)) AND
        (StaRT_Mon_Since < (@Now - @i)) AND
		    ISSUE NOT IN ('021', '081', '974')

	SET @i = @i + 1
END
GO

/*----RT004----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 93 * 12 + 12
DELETE FROM RT_TMP1
DELETE FROM RT_TMP
/*---StaRT making RT004---*/
INSERT INTO RT_TMP1
SELECT IDN, MAX(End_Mon_Since)
FROM KRM001_200412 AS A
WHERE Stop_Code = '1' AND
      Issue NOT IN
        (SELECT Issuer 
         FROM Open_Issuer AS B
         WHERE A.IDN = B.IDN AND
               A.End_Mon_Since = B.Mon)
GROUP BY A.IDN

INSERT INTO RT_TMP
SELECT IDN, 0, 0
FROM KRM001_200412 AS A
GROUP BY A.IDN

UPDATE RT_TMP
SET
V1 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

DELETE FROM RT_TMP1

INSERT INTO RT_TMP1
SELECT A.IDN, MAX(End_Mon_Since)
FROM KRM001_200412 AS A INNER JOIN RT_TMP AS B ON A.IDN = B.IDN
WHERE Stop_Code = '1' AND
      End_Mon_Since < B.V1 AND
      Issue NOT IN
        (SELECT Issuer 
         FROM Open_Issuer AS B
         WHERE A.IDN = B.IDN AND
               A.End_Mon_Since = B.Mon) 
GROUP BY A.IDN

UPDATE RT_TMP
SET
V2 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN


UPDATE RT_200412
SET
RT004 = 
(CASE
   WHEN V1=0 THEN 0
   WHEN V2=0 THEN 0 
   ELSE  V1 - V2
END)
FROM RT_TMP AS A
WHERE A.IDN = RT_200412.IDN
GO


/*----RT005----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 93 * 12 + 12
DELETE FROM RT_TMP1
DELETE FROM RT_TMP
/*---StaRT making RT005---*/
INSERT INTO RT_TMP1
SELECT IDN, MAX(End_Mon_Since)
FROM KRM001_200412 AS A
WHERE Stop_Code = '3' AND
      Issue NOT IN
        (SELECT Issuer 
         FROM Open_Issuer AS B
         WHERE A.IDN = B.IDN AND
               A.End_Mon_Since = B.Mon)
GROUP BY A.IDN

INSERT INTO RT_TMP
SELECT A.IDN, 0, 0
FROM KRM001_200412 AS A
GROUP BY A.IDN

UPDATE RT_TMP
SET
V1 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN

DELETE FROM RT_TMP1

INSERT INTO RT_TMP1
SELECT A.IDN, MAX(End_Mon_Since)
FROM KRM001_200412 AS A INNER JOIN RT_TMP AS B ON A.IDN = B.IDN
WHERE Stop_Code = '3' AND
      End_Mon_Since < B.V1 AND
      Issue NOT IN
        (SELECT Issuer 
         FROM Open_Issuer AS B
         WHERE A.IDN = B.IDN AND
               A.End_Mon_Since = B.Mon) 
GROUP BY A.IDN

UPDATE RT_TMP
SET
V2 = A.V1
FROM RT_TMP1 AS A
WHERE A.IDN = RT_TMP.IDN


UPDATE RT_200412
SET
RT005 = 
(CASE
   WHEN V1=0 THEN 0
   WHEN V2=0 THEN 0 
   ELSE  V1 - V2
END)
FROM RT_TMP AS A
WHERE A.IDN = RT_200412.IDN
GO