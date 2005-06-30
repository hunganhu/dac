USE TF_KHJ
GO

/*----RS002----*/
/*---Init---*/
DECLARE @Now INT 
SET @NOW = 93 * 12 + 4
DELETE FROM RS_TMP
/*---Start making RS002---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@NOW - MON_SINCE), 0
FROM KRM023_200404 AS A
WHERE bucket_def_1K > 0 AND
      (@NOW - MON_SINCE) > 0
GROUP BY A.IDN

UPDATE RS_200404
SET
RS002_1K = V1
FROM RS_TMP AS A
WHERE RS_200404.IDN = A.IDN
GO


/*----RS003----*/
/*---Init---*/
DECLARE @Now INT 
SET @NOW = 93 * 12 + 4
DELETE FROM RS_TMP
/*---Start making RS003---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@NOW - MON_SINCE), 0
FROM KRM023_200404 AS A
WHERE bucket_ef_1K > 0 AND
      (@NOW - MON_SINCE) > 0
GROUP BY A.IDN

UPDATE RS_200404
SET
RS003_1K = V1
FROM RS_TMP AS A
WHERE RS_200404.IDN = A.IDN
GO


/*----RS004----*/
/*---Init---*/
DECLARE @Now INT 
SET @NOW = 93 * 12 + 4
DELETE FROM RS_TMP
/*---Start making RS004---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@NOW - MON_SINCE), 0
FROM KRM023_200404 AS A
WHERE bucket_f_1K > 0 AND
      (@NOW - MON_SINCE) > 0
GROUP BY A.IDN

UPDATE RS_200404
SET
RS004_1K = V1
FROM RS_TMP AS A
WHERE RS_200404.IDN = A.IDN
GO


/*----RS008----*/
/*---Init---*/
DECLARE @Now INT 
SET @NOW = 93 * 12 + 4
DELETE FROM RS_TMP
/*---Start making RS008---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@NOW - MON_SINCE), 0
FROM KRM023_200404 AS A
WHERE CASH = 'Y' AND
      (@NOW - MON_SINCE) > 0
GROUP BY A.IDN

UPDATE RS_200404
SET
RS008 = V1
FROM RS_TMP AS A
WHERE RS_200404.IDN = A.IDN
GO


/*----RS009----*/
/*---Init---*/
DECLARE @Now INT 
SET @NOW = 93 * 12 + 4
DELETE FROM RS_TMP
/*---Start making RS009---*/
INSERT INTO RS_TMP
SELECT IDN, MIN(@NOW - MON_SINCE), 0
FROM KRM023_200404 AS A
WHERE Pay_Code = 'C' AND
      (@NOW - MON_SINCE) > 0
GROUP BY A.IDN

UPDATE RS_200404
SET
RS009 = V1
FROM RS_TMP AS A
WHERE RS_200404.IDN = A.IDN
GO
