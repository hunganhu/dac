USE TF_KHJ
GO

DROP TABLE WI_MASTER_200404
GO

CREATE TABLE WI_MASTER_200404
(IDN CHAR(6),
 MS047 FLOAT,
 MS039 FLOAT,
 MS040 FLOAT,
 MS041 FLOAT,
 MS048 FLOAT,
 MS050 FLOAT,
 WI001_1 FLOAT, WI001_2 FLOAT, WI001_3 FLOAT, WI001_4 FLOAT, WI001_5 FLOAT, WI001_6 FLOAT, WI001_7 FLOAT, WI001_8 FLOAT, WI001_9 FLOAT, WI001_10 FLOAT, WI001_11 FLOAT, WI001_12 FLOAT,
 WI002_1 FLOAT, WI002_2 FLOAT, WI002_3 FLOAT, WI002_4 FLOAT, WI002_5 FLOAT, WI002_6 FLOAT, WI002_7 FLOAT, WI002_8 FLOAT, WI002_9 FLOAT, WI002_10 FLOAT, WI002_11 FLOAT, WI002_12 FLOAT,
 WI003_1 FLOAT, WI003_2 FLOAT, WI003_3 FLOAT, WI003_4 FLOAT, WI003_5 FLOAT, WI003_6 FLOAT, WI003_7 FLOAT, WI003_8 FLOAT, WI003_9 FLOAT, WI003_10 FLOAT, WI003_11 FLOAT, WI003_12 FLOAT,
 WI004_1 FLOAT, WI004_2 FLOAT, WI004_3 FLOAT, WI004_4 FLOAT, WI004_5 FLOAT, WI004_6 FLOAT, WI004_7 FLOAT, WI004_8 FLOAT, WI004_9 FLOAT, WI004_10 FLOAT, WI004_11 FLOAT, WI004_12 FLOAT,
 WI005_1 FLOAT, WI005_2 FLOAT, WI005_3 FLOAT, WI005_4 FLOAT, WI005_5 FLOAT, WI005_6 FLOAT, WI005_7 FLOAT, WI005_8 FLOAT, WI005_9 FLOAT, WI005_10 FLOAT, WI005_11 FLOAT, WI005_12 FLOAT,
 WI006_1 FLOAT, WI006_2 FLOAT, WI006_3 FLOAT, WI006_4 FLOAT, WI006_5 FLOAT, WI006_6 FLOAT, WI006_7 FLOAT, WI006_8 FLOAT, WI006_9 FLOAT, WI006_10 FLOAT, WI006_11 FLOAT, WI006_12 FLOAT,
 WI007_1 FLOAT, WI007_2 FLOAT, WI007_3 FLOAT, WI007_4 FLOAT, WI007_5 FLOAT, WI007_6 FLOAT, WI007_7 FLOAT, WI007_8 FLOAT, WI007_9 FLOAT, WI007_10 FLOAT, WI007_11 FLOAT, WI007_12 FLOAT)
GO

INSERT INTO WI_MASTER_200404
(IDN,
 WI001_1, WI001_2, WI001_3, WI001_4, WI001_5, WI001_6, WI001_7, WI001_8, WI001_9, WI001_10, WI001_11, WI001_12,
 WI002_1, WI002_2, WI002_3, WI002_4, WI002_5, WI002_6, WI002_7, WI002_8, WI002_9, WI002_10, WI002_11, WI002_12,
 WI003_1, WI003_2, WI003_3, WI003_4, WI003_5, WI003_6, WI003_7, WI003_8, WI003_9, WI003_10, WI003_11, WI003_12,
 WI004_1, WI004_2, WI004_3, WI004_4, WI004_5, WI004_6, WI004_7, WI004_8, WI004_9, WI004_10, WI004_11, WI004_12,
 WI005_1, WI005_2, WI005_3, WI005_4, WI005_5, WI005_6, WI005_7, WI005_8, WI005_9, WI005_10, WI005_11, WI005_12,
 WI007_1, WI007_2, WI007_3, WI007_4, WI007_5, WI007_6, WI007_7, WI007_8, WI007_9, WI007_10, WI007_11, WI007_12)
SELECT DISTINCT IDN,
       0,0,0,0,0,0,0,0,0,0,0,0, 
       0,0,0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,0,0,       
       0,0,0,0,0,0,0,0,0,0,0,0
FROM KRM023_200404
GO
--171
UPDATE WI_MASTER_200404
SET
MS047 = A.MS047,
MS039 = A.MS039,
MS040 = A.MS040,
MS041 = A.MS041,
MS048 = A.MS048,
MS050 = A.MS050
FROM MASTER_200404_1 AS A
WHERE WI_MASTER_200404.IDN = A.IDN

DROP TABLE WI_TMP1
GO

CREATE TABLE WI_TMP1
(IDN CHAR(6), MON_SINCE INT, V1 FLOAT, V2 FLOAT)
GO

DROP TABLE WI_TMP2
GO

CREATE TABLE WI_TMP2
(IDN CHAR(6), MON_SINCE INT, V1 FLOAT)
GO

DROP TABLE WI_TMP3
GO

CREATE TABLE WI_TMP3
(IDN CHAR(6))
GO

----WI001----
DELETE FROM WI_TMP1
GO

INSERT INTO WI_TMP1(IDN, MON_SINCE, V1)
SELECT IDN, MON_SINCE, COUNT(*)
FROM KRM023_200404
GROUP BY IDN, MON_SINCE
GO

DECLARE @NOW INT
SET @NOW = 93 * 12 + 4

UPDATE WI_MASTER_200404
SET
WI001_1 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 1

UPDATE WI_MASTER_200404
SET
WI001_2 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 2

UPDATE WI_MASTER_200404
SET
WI001_3 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 3

UPDATE WI_MASTER_200404
SET
WI001_4 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 4

UPDATE WI_MASTER_200404
SET
WI001_5 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 5

UPDATE WI_MASTER_200404
SET
WI001_6 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 6

UPDATE WI_MASTER_200404
SET
WI001_7 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 7

UPDATE WI_MASTER_200404
SET
WI001_8 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 8

UPDATE WI_MASTER_200404
SET
WI001_9 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 9

UPDATE WI_MASTER_200404
SET
WI001_10 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 10

UPDATE WI_MASTER_200404
SET
WI001_11 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 11

UPDATE WI_MASTER_200404
SET
WI001_12 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 12
GO

----WI002----
DELETE FROM WI_TMP1
GO

INSERT INTO WI_TMP1(IDN, MON_SINCE, V1)
SELECT IDN, MON_SINCE, SUM(CASE WHEN CONVERT(INT, LIMIT) > 100 THEN 1 ELSE 0 END)
FROM KRM023_200404
GROUP BY IDN, MON_SINCE
GO

DECLARE @NOW INT
SET @NOW = 93 * 12 + 4

UPDATE WI_MASTER_200404
SET
WI002_1 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 1

UPDATE WI_MASTER_200404
SET
WI002_2 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 2

UPDATE WI_MASTER_200404
SET
WI002_3 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 3

UPDATE WI_MASTER_200404
SET
WI002_4 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 4

UPDATE WI_MASTER_200404
SET
WI002_5 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 5

UPDATE WI_MASTER_200404
SET
WI002_6 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 6

UPDATE WI_MASTER_200404
SET
WI002_7 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 7

UPDATE WI_MASTER_200404
SET
WI002_8 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 8

UPDATE WI_MASTER_200404
SET
WI002_9 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 9

UPDATE WI_MASTER_200404
SET
WI002_10 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 10

UPDATE WI_MASTER_200404
SET
WI002_11 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 11

UPDATE WI_MASTER_200404
SET
WI002_12 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 12
GO

----WI003----
DELETE FROM WI_TMP1
GO

INSERT INTO WI_TMP1(IDN, MON_SINCE, V1)
SELECT IDN, MON_SINCE, SUM(CASE WHEN CONVERT(INT, LIMIT) > 200 THEN 1 ELSE 0 END)
FROM KRM023_200404
GROUP BY IDN, MON_SINCE
GO

DECLARE @NOW INT
SET @NOW = 93 * 12 + 4

UPDATE WI_MASTER_200404
SET
WI003_1 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 1

UPDATE WI_MASTER_200404
SET
WI003_2 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 2

UPDATE WI_MASTER_200404
SET
WI003_3 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 3

UPDATE WI_MASTER_200404
SET
WI003_4 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 4

UPDATE WI_MASTER_200404
SET
WI003_5 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 5

UPDATE WI_MASTER_200404
SET
WI003_6 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 6

UPDATE WI_MASTER_200404
SET
WI003_7 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 7

UPDATE WI_MASTER_200404
SET
WI003_8 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 8

UPDATE WI_MASTER_200404
SET
WI003_9 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 9

UPDATE WI_MASTER_200404
SET
WI003_10 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 10

UPDATE WI_MASTER_200404
SET
WI003_11 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 11

UPDATE WI_MASTER_200404
SET
WI003_12 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 12
GO

----WI004----
DELETE FROM WI_TMP1
GO

INSERT INTO WI_TMP1(IDN, MON_SINCE, V1)
SELECT IDN, MON_SINCE, SUM(CONVERT(INT, LIMIT))
FROM KRM023_200404
GROUP BY IDN, MON_SINCE
GO

DECLARE @NOW INT
SET @NOW = 93 * 12 + 4

UPDATE WI_MASTER_200404
SET
WI004_1 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 1

UPDATE WI_MASTER_200404
SET
WI004_2 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 2

UPDATE WI_MASTER_200404
SET
WI004_3 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 3

UPDATE WI_MASTER_200404
SET
WI004_4 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 4

UPDATE WI_MASTER_200404
SET
WI004_5 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 5

UPDATE WI_MASTER_200404
SET
WI004_6 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 6

UPDATE WI_MASTER_200404
SET
WI004_7 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 7

UPDATE WI_MASTER_200404
SET
WI004_8 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 8

UPDATE WI_MASTER_200404
SET
WI004_9 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 9

UPDATE WI_MASTER_200404
SET
WI004_10 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 10

UPDATE WI_MASTER_200404
SET
WI004_11 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 11

UPDATE WI_MASTER_200404
SET
WI004_12 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 12
GO

----WI005----
DELETE FROM WI_TMP1
GO

INSERT INTO WI_TMP1(IDN, MON_SINCE, V1)
SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('A', 'B') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200404
GROUP BY IDN, MON_SINCE
GO

DECLARE @NOW INT
SET @NOW = 93 * 12 + 4

UPDATE WI_MASTER_200404
SET
WI005_1 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 1

UPDATE WI_MASTER_200404
SET
WI005_2 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 2

UPDATE WI_MASTER_200404
SET
WI005_3 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 3

UPDATE WI_MASTER_200404
SET
WI005_4 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 4

UPDATE WI_MASTER_200404
SET
WI005_5 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 5

UPDATE WI_MASTER_200404
SET
WI005_6 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 6

UPDATE WI_MASTER_200404
SET
WI005_7 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 7

UPDATE WI_MASTER_200404
SET
WI005_8 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 8

UPDATE WI_MASTER_200404
SET
WI005_9 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 9

UPDATE WI_MASTER_200404
SET
WI005_10 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 10

UPDATE WI_MASTER_200404
SET
WI005_11 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 11

UPDATE WI_MASTER_200404
SET
WI005_12 = A.V1
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 12
GO

----WI006----
DELETE FROM WI_TMP1
GO
DELETE FROM WI_TMP2
GO
DELETE FROM WI_TMP3

INSERT INTO WI_TMP3
SELECT IDN
FROM KRM023_200404
GROUP BY IDN
HAVING MAX(BUCKET_EF_1K) = 0
GO

INSERT INTO WI_TMP1(IDN, MON_SINCE, V1, V2)
SELECT IDN, MON_SINCE, 0, 0
FROM KRM023_200404
WHERE IDN IN
      (SELECT IDN FROM WI_TMP3)
GROUP BY IDN, MON_SINCE
GO

INSERT INTO WI_TMP2(IDN, MON_SINCE, V1)
SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200404
WHERE IDN IN
      (SELECT IDN FROM WI_TMP3)
GROUP BY IDN, MON_SINCE
GO

UPDATE WI_TMP1
SET
V1 = A.V1
FROM WI_TMP2 AS A
WHERE A.IDN = WI_TMP1.IDN AND
      A.MON_SINCE = WI_TMP1.MON_SINCE
GO

UPDATE WI_TMP1
SET
V2 = (CASE 
        WHEN (WI_TMP1.V1 - A.V1) >= 0 THEN 0
        ELSE (WI_TMP1.V1 - A.V1) * -1
      END) 
FROM WI_TMP1 AS A INNER JOIN WI_TMP1
ON A.IDN = WI_TMP1.IDN AND
   A.MON_SINCE = WI_TMP1.MON_SINCE - 1
GO

DECLARE @NOW INT
SET @NOW = 93 * 12 + 4

UPDATE WI_MASTER_200404
SET
WI006_1 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 1

UPDATE WI_MASTER_200404
SET
WI006_2 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 2

UPDATE WI_MASTER_200404
SET
WI006_3 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 3

UPDATE WI_MASTER_200404
SET
WI006_4 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 4

UPDATE WI_MASTER_200404
SET
WI006_5 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 5

UPDATE WI_MASTER_200404
SET
WI006_6 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 6

UPDATE WI_MASTER_200404
SET
WI006_7 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 7

UPDATE WI_MASTER_200404
SET
WI006_8 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 8

UPDATE WI_MASTER_200404
SET
WI006_9 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 9

UPDATE WI_MASTER_200404
SET
WI006_10 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 10

UPDATE WI_MASTER_200404
SET
WI006_11 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 11

UPDATE WI_MASTER_200404
SET
WI006_12 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 12
GO

----WI007----
DELETE FROM WI_TMP1
GO

INSERT INTO WI_TMP1(IDN, MON_SINCE, V1, V2)
SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END), 0
FROM KRM023_200404
GROUP BY IDN, MON_SINCE
GO

UPDATE WI_TMP1
SET
V2 = (CASE 
        WHEN (WI_TMP1.V1 - A.V1) >= 0 THEN 0
        ELSE (WI_TMP1.V1 - A.V1) * -1
      END) 
FROM WI_TMP1 AS A INNER JOIN WI_TMP1
ON A.IDN = WI_TMP1.IDN AND
   A.MON_SINCE = WI_TMP1.MON_SINCE - 1
GO

DECLARE @NOW INT
SET @NOW = 93 * 12 + 4

UPDATE WI_MASTER_200404
SET
WI007_1 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 1

UPDATE WI_MASTER_200404
SET
WI007_2 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 2

UPDATE WI_MASTER_200404
SET
WI007_3 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 3

UPDATE WI_MASTER_200404
SET
WI007_4 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 4

UPDATE WI_MASTER_200404
SET
WI007_5 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 5

UPDATE WI_MASTER_200404
SET
WI007_6 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 6

UPDATE WI_MASTER_200404
SET
WI007_7 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 7

UPDATE WI_MASTER_200404
SET
WI007_8 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 8

UPDATE WI_MASTER_200404
SET
WI007_9 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 9

UPDATE WI_MASTER_200404
SET
WI007_10 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 10

UPDATE WI_MASTER_200404
SET
WI007_11 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 11

UPDATE WI_MASTER_200404
SET
WI007_12 = A.V2
FROM WI_TMP1 AS A
WHERE A.IDN = WI_MASTER_200404.IDN AND
      @NOW - A.MON_SINCE = 12
GO