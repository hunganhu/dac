use TF_GX
go

/*----Declare global variable----*/

/*----RS012----*/
/*---Init---*/
DECLARE @Now INT 
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS012---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@NOW - QUERY_MON_SINCE), 0
FROM dbo.STM001_200312 AS A
WHERE ITEM_LIST LIKE '%K%' AND
      (@NOW - QUERY_MON_SINCE) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS012 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS013----*/
/*---Init---*/
DECLARE @Now INT 
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS013---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@NOW - QUERY_MON_SINCE), 0
FROM dbo.STM001_200312 AS A
WHERE (ITEM_LIST LIKE '%B%' OR ITEM_LIST LIKE '%D%' OR  ITEM_LIST LIKE '%J%') AND
      (@NOW - QUERY_MON_SINCE) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS013 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO


/*----RS014----*/
/*---Init---*/
DECLARE @Now INT 
SET @NOW = 92 * 12 + 12
DELETE FROM RS_TMP
/*---Start making RS014---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@NOW - QUERY_MON_SINCE), 0
FROM dbo.STM001_200312 AS A
WHERE ITEM_LIST <> '' AND
      (@NOW - QUERY_MON_SINCE) > 0
GROUP BY A.IDN

UPDATE RS_200312
SET
RS014 = V1
FROM RS_TMP AS A
WHERE A.IDN = RS_200312.IDN
GO




