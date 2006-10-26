USE TF_GX
GO

/*----Prepare Open_Issuer table for RS_200312 variables 010, 018, 022, 024----*/
/*----This table is extended to the open date of the fiRS_200312t card of each peRS_200312on----*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM Open_Issuer
SET @j = @Now - (SELECT MIN(Start_Mon_Since) FROM KRM001_200312)
SET @i = 0
WHILE @i<= @j
BEGIN
	
	INSERT INTO Open_Issuer
	SELECT IDN, ISSUE, (@Now - @i)
	FROM KRM001_200312
	WHERE (End_Mon_Since > (@Now - @i)) AND
        (Start_Mon_Since < (@Now - @i)) AND
		    ISSUE NOT IN ('021', '081', '974')

	SET @i = @i + 1
END
GO


/*----RS005----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS005---*/
INSERT INTO RS_TMP
SELECT IDN, MAX(@Now - Start_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE CARD_TYPE='G' AND
      (@NOW - Start_Mon_Since) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS005 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS006----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS006---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@Now - Start_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE CARD_TYPE='G' AND
      (@NOW - Start_Mon_Since) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS006 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS015----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS015---*/
INSERT INTO RS_TMP
SELECT IDN, MAX(@Now - Start_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE CARD_TYPE='P' AND
      (@NOW - Start_Mon_Since) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS015 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS016----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS016---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@Now - Start_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE CARD_TYPE='P' AND
      (@NOW - Start_Mon_Since) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS016 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS017----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS017---*/
INSERT INTO RS_TMP
SELECT IDN, MAX(@Now - Start_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE (@NOW - Start_Mon_Since) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS017 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS018----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS018---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@Now - Start_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE ISSUE NOT IN (SELECT DISTINCT Issuer 
     	              FROM Open_Issuer AS B
       	            WHERE A.IDN = B.IDN AND A.Start_Mon_Since = B.MON) AND
      (@NOW - Start_Mon_Since) > 0  
GROUP BY A.IDN

UPDATE RS_200312
SET
RS018 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS019----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS019---*/
INSERT INTO RS_TMP
SELECT IDN, MAX(@Now - Start_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE CARD_BRAND='A' AND
      (@NOW - Start_Mon_Since) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS019 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS020----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS020---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@Now - Start_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE CARD_BRAND='A' AND
      (@NOW - Start_Mon_Since) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS020 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS021----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS021---*/
INSERT INTO RS_TMP
SELECT IDN, MAX(@Now - End_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE Stop_Code IN ('1','2') AND
      ISSUE NOT IN (SELECT DISTINCT Issuer 
     	              FROM Open_Issuer AS B
       	            WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON) AND
      (@NOW - End_Mon_Since) > 0 
GROUP BY A.IDN

UPDATE RS_200312
SET
RS021 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS022----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS022---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@Now - End_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE Stop_Code IN ('1','2') AND
      ISSUE NOT IN (SELECT DISTINCT Issuer 
     	              FROM Open_Issuer AS B
       	            WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON) AND
      (@NOW - End_Mon_Since) > 0 
GROUP BY A.IDN

UPDATE RS_200312
SET
RS022 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS023----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS023---*/
INSERT INTO RS_TMP
SELECT IDN, MAX(@Now - End_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE Stop_Code = '3' AND
      ISSUE NOT IN (SELECT DISTINCT Issuer 
     	              FROM Open_Issuer AS B
       	            WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON) AND
      (@NOW - End_Mon_Since) > 0 
GROUP BY A.IDN

UPDATE RS_200312
SET
RS023 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS024----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS024---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@Now - End_Mon_Since), 0
FROM KRM001_200312 AS A
WHERE Stop_Code = '3' AND 
      ISSUE NOT IN (SELECT DISTINCT Issuer 
     	              FROM Open_Issuer AS B
       	            WHERE A.IDN = B.IDN AND A.End_Mon_Since = B.MON) AND
      (@NOW - End_Mon_Since) > 0 
GROUP BY A.IDN

UPDATE RS_200312
SET
RS024 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS010----*/
/*---Init---*/
DECLARE @Now INT, @i INT, @j INT
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
DELETE FROM RS_TMP1

/*---Start making RS010---*/
INSERT INTO RS_TMP
SELECT DISTINCT IDN, 0, 0
FROM KRM001_200312

/*--Abolished--*/
/*--Contuine with RS010--*/
INSERT INTO RS_TMP1
SELECT IDN, MIN(@Now - Start_Mon_Since)
FROM KRM001_200312 AS A
WHERE ISSUE NOT IN ('021', '081', '974') AND
   	  ISSUE NOT IN (SELECT DISTINCT Issuer 
     	              FROM Open_Issuer AS B
       	            WHERE A.IDN = B.IDN AND A.Start_Mon_Since = B.MON) AND
      (@NOW - Start_Mon_Since) > 0  
GROUP BY IDN

UPDATE RS_TMP
SET V1 = A.V1
FROM RS_TMP1 AS A
WHERE RS_TMP.IDN = A.IDN

DELETE FROM RS_TMP1

INSERT INTO RS_TMP1
SELECT IDN, MIN(@Now - Start_Mon_Since)
FROM KRM001_200312
WHERE ISSUE IN ('021', '081', '974') AND
      (@NOW - Start_Mon_Since) > 0 
GROUP BY IDN
	
UPDATE RS_TMP
SET V2 = A.V1
FROM RS_TMP1 AS A
WHERE RS_TMP.IDN = A.IDN 

UPDATE RS_200312
SET
RS010 =
(CASE
   WHEN V1 = 0 THEN V2
   WHEN V2 = 0 THEN V1 
   WHEN V1>V2 THEN V2
   ELSE V1
END)
FROM RS_TMP AS A
WHERE RS_200312.IDN = A.IDN
GO
