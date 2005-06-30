USE TF_KHJ
GO

/*-----TF_KHJ-----*/
DROP TABLE Open_Card_200412, Open_Line_200412, Latest_Stmt_Mon_200412, Latest_Line_200412
DROP TABLE MS2xx_200412, FS2xx_200412, TMP2xx, TMP2xx_1, Ind_200412 
GO

/*----Infrastructure for new variables----*/
/*---open_card and open_line contain months which should have transaction records based on
     krm001, open_line contains one record for each credit line each month. One-card-one-line
     issuers, i.e. 021(Citi) is processed seperately.  The other two one-card-one-line issuers,
     i.e. 081(HSBC) ,974(AIG), for their KRM023 data are consolidated, are treated as
     multi-card-one-line issers---*/ 
CREATE TABLE Open_Card_200412
(IDN Char(6), Issue char(3), Mon int)

CREATE TABLE Open_Line_200412
(IDN Char(6), Issue char(3), Mon int, Cards int)
GO

DECLARE @i INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12 /*for TF_KHJ*/
SET @i = 0

/*From the persepective of statement, end_mon_since should use "larger than or equal to"*/
WHILE @i<=13
BEGIN
	
	INSERT INTO Open_Card_200412
	SELECT IDN, (CASE WHEN CARD_BRAND = 'A' AND ISSUE = 'A82' THEN 'AEA' ELSE Issue END), (@Now - @i)
	FROM KRM001_200412
	WHERE (End_Mon_Since >= (@Now - @i)) AND
        (Start_Mon_Since < (@Now - @i)) AND
        ISSUE != '021'
	SET @i = @i + 1
END
GO

INSERT INTO Open_Line_200412
SELECT IDN, Issue, Mon, Count(*)
FROM Open_Card_200412
GROUP BY IDN, Issue, Mon
GO

DECLARE @i INT
DECLARE @NOW INT

SET @NOW = 93 * 12 + 12 /*for TF_KHJ*/
SET @i = 0

WHILE @i<=13
BEGIN
	
	INSERT INTO Open_Line_200412
	SELECT IDN, (CASE 
                 WHEN Card_Brand = 'M' THEN 'CTM'
                 WHEN Card_Brand = 'V' THEN 'CTV'
                 WHEN Card_Brand = 'D' THEN 'CTD'
               END), (@Now - @i),1
	FROM KRM001_200412
	WHERE (End_Mon_Since >= (@Now - @i)) AND
        (Start_Mon_Since < (@Now - @i)) AND
        ISSUE = '021'
	SET @i = @i + 1
END
GO

CREATE INDEX idx ON Open_Line_200412(IDN)
GO

ALTER TABLE Open_Line_200412 ADD Bucket int
GO

UPDATE Open_Line_200412
SET Bucket = 1
Go

UPDATE Open_Line_200412
SET Bucket = 100 /*arbitrary number, just to make sure it will out-of-range*/
WHERE MON = ((93 * 12 + 12) - 13)
GO

DECLARE @i INT, @NOW INT
SET @i = 13
SET @NOW = 93 * 12 + 12

WHILE @i > 0
BEGIN
UPDATE Open_Line_200412
SET
Bucket = A.Bucket + 1
FROM Open_Line_200412 AS A INNER JOIN Open_Line_200412 ON
      A.IDN = Open_Line_200412.IDN AND
      A.Issue = Open_Line_200412.Issue AND
      A.Mon = (Open_Line_200412.Mon - 1)
WHERE (@NOW - A.Mon) = @i

SET @i = @i - 1
END
Go

CREATE TABLE Latest_Stmt_Mon_200412 (IDN Char(6), Issue Char(3), Mon Int)
CREATE TABLE Latest_Line_200412 (IDN Char(6), Issue Char(3), Mon Int, MOB Int)
Go

DECLARE @NOW INT
SET @NOW = 93 * 12 + 12 /*for TF_KHJ*/

INSERT INTO Latest_Stmt_Mon_200412
SELECT IDN, Issue, Max(Mon)
FROM Open_Line_200412
WHERE Mon < @NOW
GROUP BY IDN, Issue
GO

CREATE INDEX idx ON Latest_Stmt_Mon_200412(IDN, Issue, Mon)
GO

INSERT INTO Latest_Line_200412
SELECT A.IDN, A.Issue, A.Mon, A.Bucket
FROM Open_Line_200412 AS A INNER JOIN Latest_Stmt_Mon_200412 AS B
ON A.IDN = B.IDN AND
   A.Issue = B.Issue AND
   A.Mon = B.Mon
GO

CREATE INDEX idx ON Latest_Line_200412(IDN, Issue)
GO


/*----Base Table----*/
CREATE TABLE MS2xx_200412
(IDN Char(6), MS201_3M_1k int, MS201_6M_1k int, MS201_9M_1k int, MS201_12M_1k int,
               MS202_3M_1k int, MS202_6M_1k int, MS202_9M_1k int, MS202_12M_1k int,
               MS203_3M_1k int, MS203_6M_1k int, MS203_9M_1k int, MS203_12M_1k int,
               MS204_3M_1k int, MS204_6M_1k int, MS204_9M_1k int, MS204_12M_1k int,
               MS205_3M_1k int, MS205_6M_1k int, MS205_9M_1k int, MS205_12M_1k int,
               MS206_3M_1k int, MS206_6M_1k int, MS206_9M_1k int, MS206_12M_1k int,
               MS207_3M_1k float, MS207_6M_1k float, MS207_9M_1k float, MS207_12M_1k float,
               MS208_3M_1k float, MS208_6M_1k float, MS208_9M_1k float, MS208_12M_1k float,
               MS209_3M_1k float, MS209_6M_1k float, MS209_9M_1k float, MS209_12M_1k float,
               MS210_3M_1k float, MS210_6M_1k float, MS210_9M_1k float, MS210_12M_1k float,
               MS211_3M_1k float, MS211_6M_1k float, MS211_9M_1k float, MS211_12M_1k float,
               MS212_3M_1k float, MS212_6M_1k float, MS212_9M_1k float, MS212_12M_1k float)
GO

CREATE TABLE FS2xx_200412
(IDN Char(6), FS201_3M_1k int, FS201_6M_1k int, FS201_9M_1k int, FS201_12M_1k int,
               FS202_3M_1k int, FS202_6M_1k int, FS202_9M_1k int, FS202_12M_1k int,
               FS203_3M_1k int, FS203_6M_1k int, FS203_9M_1k int, FS203_12M_1k int,
               FS204_3M_1k int, FS204_6M_1k int, FS204_9M_1k int, FS204_12M_1k int,
               FS205_3M_1k int, FS205_6M_1k int, FS205_9M_1k int, FS205_12M_1k int,
               FS206_3M_1k int, FS206_6M_1k int, FS206_9M_1k int, FS206_12M_1k int,
               FS207_3M_1k float, FS207_6M_1k float, FS207_9M_1k float, FS207_12M_1k float,
               FS208_3M_1k float, FS208_6M_1k float, FS208_9M_1k float, FS208_12M_1k float,
               FS209_3M_1k float, FS209_6M_1k float, FS209_9M_1k float, FS209_12M_1k float,
               FS210_3M_1k float, FS210_6M_1k float, FS210_9M_1k float, FS210_12M_1k float,
               FS211_3M_1k float, FS211_6M_1k float, FS211_9M_1k float, FS211_12M_1k float,
               FS212_3M_1k float, FS212_6M_1k float, FS212_9M_1k float, FS212_12M_1k float,
               FS213_1k float, FS214_1k float)

GO

INSERT INTO MS2xx_200412(IDN)
SELECT IDN
FROM BASE_200412
GO

INSERT INTO FS2xx_200412(IDN)
SELECT IDN
FROM BASE_200412
GO

CREATE INDEX IDX ON MS2xx_200412(IDN)
CREATE INDEX IDX ON FS2xx_200412(IDN)
GO

UPDATE MS2xx_200412
SET
MS201_3M_1k = 0, MS201_6M_1k = 0, MS201_9M_1k = 0, MS201_12M_1k = 0,
MS202_3M_1k = 0, MS202_6M_1k = 0, MS202_9M_1k = 0, MS202_12M_1k = 0,
MS203_3M_1k = 0, MS203_6M_1k = 0, MS203_9M_1k = 0, MS203_12M_1k = 0,
MS204_3M_1k = 0, MS204_6M_1k = 0, MS204_9M_1k = 0, MS204_12M_1k = 0,
MS205_3M_1k = 0, MS205_6M_1k = 0, MS205_9M_1k = 0, MS205_12M_1k = 0,
MS206_3M_1k = 0, MS206_6M_1k = 0, MS206_9M_1k = 0, MS206_12M_1k = 0,
MS207_3M_1k = 0, MS207_6M_1k = 0, MS207_9M_1k = 0, MS207_12M_1k = 0,
MS208_3M_1k = 0, MS208_6M_1k = 0, MS208_9M_1k = 0, MS208_12M_1k = 0,
MS209_3M_1k = 0, MS209_6M_1k = 0, MS209_9M_1k = 0, MS209_12M_1k = 0,
MS210_3M_1k = 0, MS210_6M_1k = 0, MS210_9M_1k = 0, MS210_12M_1k = 0,
MS211_3M_1k = 0, MS211_6M_1k = 0, MS211_9M_1k = 0, MS211_12M_1k = 0,
MS212_3M_1k = 0, MS212_6M_1k = 0, MS212_9M_1k = 0, MS212_12M_1k = 0
FROM KRM023_200412 AS A, KRM001_200412 AS B
WHERE A.IDN = B.IDN AND 
      A.IDN = MS2xx_200412.IDN AND
      ((A.ISSUE = B.ISSUE) OR ((B.ISSUE = '021') AND (A.ISSUE IN ('CTV', 'CTM', 'CTD'))))
GO 

UPDATE FS2xx_200412
SET
FS201_3M_1k = 0, FS201_6M_1k = 0, FS201_9M_1k = 0, FS201_12M_1k = 0,
FS202_3M_1k = 0, FS202_6M_1k = 0, FS202_9M_1k = 0, FS202_12M_1k = 0,
FS203_3M_1k = 0, FS203_6M_1k = 0, FS203_9M_1k = 0, FS203_12M_1k = 0,
FS204_3M_1k = 0, FS204_6M_1k = 0, FS204_9M_1k = 0, FS204_12M_1k = 0,
FS205_3M_1k = 0, FS205_6M_1k = 0, FS205_9M_1k = 0, FS205_12M_1k = 0,
FS206_3M_1k = 0, FS206_6M_1k = 0, FS206_9M_1k = 0, FS206_12M_1k = 0,
FS207_3M_1k = 0, FS207_6M_1k = 0, FS207_9M_1k = 0, FS207_12M_1k = 0,
FS208_3M_1k = 0, FS208_6M_1k = 0, FS208_9M_1k = 0, FS208_12M_1k = 0,
FS209_3M_1k = 0, FS209_6M_1k = 0, FS209_9M_1k = 0, FS209_12M_1k = 0,
FS210_3M_1k = 0, FS210_6M_1k = 0, FS210_9M_1k = 0, FS210_12M_1k = 0,
FS211_3M_1k = 0, FS211_6M_1k = 0, FS211_9M_1k = 0, FS211_12M_1k = 0,
FS212_3M_1k = 0, FS212_6M_1k = 0, FS212_9M_1k = 0, FS212_12M_1k = 0
FROM KRM023_200412 AS A, KRM001_200412 AS B
WHERE A.IDN = B.IDN AND 
      A.IDN = FS2xx_200412.IDN AND
      ((A.ISSUE = B.ISSUE) OR ((B.ISSUE = '021') AND (A.ISSUE IN ('CTV', 'CTM', 'CTD'))))
GO 

UPDATE FS2xx_200412
SET
FS213_1k = 0, FS214_1k = 0
FROM KRM001_200412 AS A
WHERE A.IDN = FS2xx_200412.IDN
      
GO 

/*----Temp----*/
CREATE TABLE TMP2xx (IDN Char(6), Mon int, v1 float)
CREATE TABLE TMP2xx_1 (IDN Char(6), Mon int, v1 float)
GO

/*----MS201_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Bucket_ef_1k != 0 AND
  (@NOW - Mon_Since) <= @I AND
  (@NOW - Mon_Since) > 0
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE MS2xx_200412
SET 
MS201_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS201_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS201_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS201_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----MS202_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Bucket_ef_1k <> 0 AND
  (@NOW - Mon_Since) <= @I AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE MS2xx_200412
SET 
MS202_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS202_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS202_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS202_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----MS203_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Bucket_def_1k != 0 AND
  (@NOW - Mon_Since) <= @I AND
  (@NOW - Mon_Since) > 0
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE MS2xx_200412
SET 
MS203_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS203_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS203_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS203_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----MS204_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Bucket_def_1k <> 0 AND
  (@NOW - Mon_Since) <= @I AND
  (@NOW - Mon_Since) > 0
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE MS2xx_200412
SET 
MS204_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS204_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS204_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS204_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO

/*----MS205_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Pay_Code in ('C', 'D', 'E','F') AND
  Payment_Amt > 1 AND
  (@NOW - Mon_Since) <= @I AND
  (@NOW - Mon_Since) > 0
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE MS2xx_200412
SET 
MS205_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS205_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS205_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS205_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----MS206_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Pay_Code in ('C', 'D', 'E','F') AND
  Payment_Amt > 1 AND
  (@NOW - Mon_Since) <= @I AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE MS2xx_200412
SET 
MS206_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS206_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS206_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS206_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO

/*----MS207_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Bucket_ef_1k != 0 AND
  (@NOW - Mon_Since) = @I
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE MS2xx_200412
SET 
MS207_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS207_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS207_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS207_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----MS208_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Bucket_ef_1k <> 0 AND
  (@NOW - Mon_Since) = @I
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE MS2xx_200412
SET 
MS208_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS208_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS208_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS208_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----MS209_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Bucket_def_1k != 0 AND
  (@NOW - Mon_Since) = @I
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE MS2xx_200412
SET 
MS209_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS209_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS209_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS209_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----MS210_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Bucket_def_1k <> 0 AND
  @NOW - Mon_Since = @I 
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE MS2xx_200412
SET 
MS210_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS210_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS210_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS210_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO

/*----MS211_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Pay_Code in ('C', 'D', 'E','F') AND
  Payment_Amt > 1 AND
  @NOW - Mon_Since = @I
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE MS2xx_200412
SET 
MS211_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS211_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS211_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS211_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----MS212_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, SUM(Payment_Amt)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Pay_Code in ('C', 'D', 'E','F') AND
  Payment_Amt > 1 AND
  @NOW - Mon_Since = @I 
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE MS2xx_200412
SET 
MS212_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE MS2xx_200412
SET 
MS212_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE MS2xx_200412
SET 
MS212_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE MS2xx_200412
SET 
MS212_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = MS2xx_200412.IDN AND
      A.Mon = 12
GO

/*----FS201_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Bucket_ef_1k != 0 AND
  (@NOW - Mon_Since) <= @I AND
  (@NOW - Mon_Since) > 0
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE FS2xx_200412
SET 
FS201_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS201_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS201_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS201_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----FS202_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Bucket_ef_1k <> 0 AND
  @NOW - Mon_Since <= @I 
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE FS2xx_200412
SET 
FS202_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS202_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS202_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS202_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----FS203_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Bucket_def_1k != 0 AND
  @NOW - Mon_Since <= @I AND
  (@NOW - Mon_Since) > 0
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE FS2xx_200412
SET 
FS203_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS203_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS203_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS203_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----FS204_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Bucket_def_1k <> 0 AND
  @NOW - Mon_Since <= @I AND
  (@NOW - Mon_Since) > 0
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE FS2xx_200412
SET 
FS204_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS204_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS204_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS204_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO

/*----FS205_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Pay_Code in ('C', 'D', 'E','F') AND
  Payment_Amt > 1 AND
  @NOW - Mon_Since <= @I AND
  (@NOW - Mon_Since) > 0
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE FS2xx_200412
SET 
FS205_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS205_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS205_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS205_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----FS206_1K----*/
DELETE FROM TMP2xx

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Pay_Code in ('C', 'D', 'E','F') AND
  Payment_Amt > 1 AND
  @NOW - Mon_Since <= @I AND
  (@NOW - Mon_Since) > 0 
  GROUP BY IDN

	SET @I = @I + 3
END
GO

UPDATE FS2xx_200412
SET 
FS206_3M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS206_6M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS206_9M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS206_12M_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO

/*----FS207_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Bucket_ef_1k != 0 AND
  @NOW - Mon_Since = @I
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE FS2xx_200412
SET 
FS207_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS207_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS207_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS207_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----FS208_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Bucket_ef_1k <> 0 AND
  @NOW - Mon_Since = @I 
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE FS2xx_200412
SET 
FS208_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS208_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS208_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS208_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----FS209_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Bucket_def_1k != 0 AND
  @NOW - Mon_Since = @I
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE FS2xx_200412
SET 
FS209_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS209_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS209_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS209_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----FS210_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Bucket_def_1k <> 0 AND
  @NOW - Mon_Since = @I 
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE FS2xx_200412
SET 
FS210_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS210_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS210_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS210_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO

/*----FS211_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE MOB <= 12 AND IDN = A.IDN) AND
  Pay_Code in ('C', 'D', 'E','F') AND
  Payment_Amt > 1 AND
  @NOW - Mon_Since = @I
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE FS2xx_200412
SET 
FS211_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS211_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS211_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS211_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO


/*----FS212_1K----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1
GO

DECLARE @I INT
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 1

WHILE @I <= 12
BEGIN
	INSERT INTO TMP2xx
	SELECT IDN, @I, COUNT(*)
  FROM KRM023_200412 AS A
  WHERE Issue IN
  (SELECT Issue FROM Latest_Line_200412
   WHERE  (Mon-MOB + 1) >=  (@Now - 6) AND IDN = A.IDN) AND
  Pay_Code in ('C', 'D', 'E','F') AND
  Payment_Amt > 1 AND
  @NOW - Mon_Since = @I 
  GROUP BY IDN

	SET @I = @I + 1
END
GO

DECLARE @I INT, @NOW INT
SET @NOW = 93 * 12 + 12
SET @I = 3
WHILE @I <= 12
BEGIN
  INSERT INTO TMP2xx_1
  SELECT IDN, @I, AVG(V1)
  FROM TMP2xx
  WHERE Mon <= @I
  GROUP BY IDN
 
  SET @I = @I + 3
END
GO  

UPDATE FS2xx_200412
SET 
FS212_3M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 3
GO

UPDATE FS2xx_200412
SET 
FS212_6M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 6
GO

UPDATE FS2xx_200412
SET 
FS212_9M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 9
GO

UPDATE FS2xx_200412
SET 
FS212_12M_1k = V1
FROM TMP2xx_1 AS A
WHERE A.IDN = FS2xx_200412.IDN AND
      A.Mon = 12
GO

/*----FS213_1k----*/
DELETE FROM TMP2xx
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12 

INSERT INTO TMP2xx
SELECT IDN, NULL, COUNT(DISTINCT ISSUE)
FROM Latest_Line_200412
WHERE (Mon-MOB + 1) >=  (@Now - 6) AND
      (@NOW - Mon) > 0
GROUP BY IDN
GO

UPDATE FS2xx_200412
SET
FS213_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN
GO

/*----FS214_1k----*/
DELETE FROM TMP2xx
GO

INSERT INTO TMP2xx
SELECT IDN, NULL, COUNT(DISTINCT ISSUE)
FROM Latest_Line_200412
WHERE MOB <= 12
GROUP BY IDN
GO

UPDATE FS2xx_200412
SET
FS214_1k = V1
FROM TMP2xx AS A
WHERE A.IDN = FS2xx_200412.IDN
GO

/*---Indicator variables----*/
CREATE TABLE Ind_200412 (IDN Char(6), Ind001 Int, Ind002 Int)
GO

INSERT INTO Ind_200412(IDN)
SELECT IDN
FROM BASE_200412
GO

CREATE INDEX idx ON Ind_200412(IDN)
GO

UPDATE Ind_200412
SET
Ind001 = 0
FROM KRM001_200412 AS A
WHERE A.IDN = Ind_200412.IDN

UPDATE Ind_200412
SET
Ind002 = 0
FROM KRM023_200412 AS A
WHERE A.IDN = Ind_200412.IDN

/*----Ind001----*/

DELETE FROM TMP2xx

DECLARE @NOW INT
SET @NOW = 93 * 12 + 12 

INSERT INTO TMP2xx
SELECT IDN, MIN(Start_Mon_Since), NULL
FROM KRM001_200412
GROUP BY IDN

UPDATE Ind_200412
SET
Ind001 = 1
FROM TMP2xx AS A
WHERE A.IDN = Ind_200412.IDN AND
      (@NOW - A.Mon) BETWEEN 1 AND (6 + 1)
GO

/*----Ind002----*/
DELETE FROM TMP2xx
DELETE FROM TMP2xx_1


DECLARE @NOW INT
SET @NOW = 93 * 12 + 12 

INSERT INTO TMP2xx
SELECT IDN, NULL, MAX(Payment_Amt)
FROM KRM023_200412
WHERE @Now - Mon_Since = 1
GROUP BY IDN, Mon_Since


INSERT INTO TMP2xx_1
SELECT IDN, NULL, MAX(V1)
FROM TMP2xx
GROUP BY IDN
GO


UPDATE Ind_200412
SET
Ind002 = 1
FROM TMP2xx_1 AS A
WHERE A.IDN = Ind_200412.IDN AND
      A.V1 > 800
GO
