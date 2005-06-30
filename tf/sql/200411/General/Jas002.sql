USE TF_KHJ

DROP TABLE TMP
GO

DROP TABLE JAS002_VAR_200411
GO

CREATE TABLE TMP (IDN CHAR(6), MON INT, V1 INT)
GO

CREATE TABLE JAS002_VAR_200411
(
IDN CHAR(6), 
JAS002_B_3M INT, JAS002_B_6M INT, JAS002_B_12M INT, JAS002_B_24M INT, JAS002_B_36M INT,
JAS002_D_3M INT, JAS002_D_6M INT, JAS002_D_12M INT, JAS002_D_24M INT, JAS002_D_36M INT,
JAS002_R_3M INT, JAS002_R_6M INT, JAS002_R_12M INT, JAS002_R_24M INT, JAS002_R_36M INT,
JAS002_S_3M INT, JAS002_S_6M INT, JAS002_S_12M INT, JAS002_S_24M INT, JAS002_S_36M INT
)
GO

INSERT INTO JAS002_VAR_200411(IDN)
SELECT IDN FROM MASTER_200411_1
GO

UPDATE JAS002_VAR_200411
SET
JAS002_B_3M = 0, JAS002_B_6M = 0, JAS002_B_12M = 0, JAS002_B_24M = 0, JAS002_B_36M = 0,
JAS002_D_3M = 0, JAS002_D_6M = 0, JAS002_D_12M = 0, JAS002_D_24M = 0, JAS002_D_36M = 0,
JAS002_R_3M = 0, JAS002_R_6M = 0, JAS002_R_12M = 0, JAS002_R_24M = 0, JAS002_R_36M = 0,
JAS002_S_3M = 0, JAS002_S_6M = 0, JAS002_S_12M = 0, JAS002_S_24M = 0, JAS002_S_36M = 0
GO

DECLARE @NOW INT
DECLARE @i INT
SET @NOW = 92 * 12 + 3

DELETE FROM TMP

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 3, COUNT(*)
FROM JAS002_200411_T_DEDUP
WHERE Reason = 'B' AND 
      @NOW - Mon_Since BETWEEN 0 AND 3
GROUP BY IDN

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 6, COUNT(*)
FROM JAS002_200411_T_DEDUP
WHERE Reason = 'B' AND 
      @NOW - Mon_Since BETWEEN 0 AND 6
GROUP BY IDN

SET @i = 12
WHILE @i <= 36
BEGIN
  INSERT INTO TMP(IDN, MON, V1)
  SELECT IDN, @i, COUNT(*)
  FROM JAS002_200411_T_DEDUP
  WHERE Reason = 'B' AND 
        @NOW - Mon_Since BETWEEN 0 AND @i
  GROUP BY IDN
  SET @i = @i + 12
END

UPDATE JAS002_VAR_200411
SET
JAS002_B_3M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 3

UPDATE JAS002_VAR_200411
SET
JAS002_B_6M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 6

UPDATE JAS002_VAR_200411
SET
JAS002_B_12M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 12

UPDATE JAS002_VAR_200411
SET
JAS002_B_24M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 24

UPDATE JAS002_VAR_200411
SET
JAS002_B_36M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 36


DELETE FROM TMP

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 3, COUNT(*)
FROM JAS002_200411_T_DEDUP
WHERE Reason = 'D' AND 
      @NOW - Mon_Since BETWEEN 0 AND 3
GROUP BY IDN

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 6, COUNT(*)
FROM JAS002_200411_T_DEDUP
WHERE Reason = 'D' AND 
      @NOW - Mon_Since BETWEEN 0 AND 6
GROUP BY IDN

SET @i = 12
WHILE @i <= 36
BEGIN
  INSERT INTO TMP(IDN, MON, V1)
  SELECT IDN, @i, COUNT(*)
  FROM JAS002_200411_T_DEDUP
  WHERE Reason = 'D' AND 
        @NOW - Mon_Since BETWEEN 0 AND @i
  GROUP BY IDN
  SET @i = @i + 12
END

UPDATE JAS002_VAR_200411
SET
JAS002_D_3M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 3

UPDATE JAS002_VAR_200411
SET
JAS002_D_6M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 6

UPDATE JAS002_VAR_200411
SET
JAS002_D_12M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 12

UPDATE JAS002_VAR_200411
SET
JAS002_D_24M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 24

UPDATE JAS002_VAR_200411
SET
JAS002_D_36M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 36


DELETE FROM TMP

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 3, COUNT(*)
FROM JAS002_200411_T_DEDUP
WHERE Reason = 'R' AND 
      @NOW - Mon_Since BETWEEN 0 AND 3
GROUP BY IDN

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 6, COUNT(*)
FROM JAS002_200411_T_DEDUP
WHERE Reason = 'R' AND 
      @NOW - Mon_Since BETWEEN 0 AND 6
GROUP BY IDN

SET @i = 12
WHILE @i <= 36
BEGIN
  INSERT INTO TMP(IDN, MON, V1)
  SELECT IDN, @i, COUNT(*)
  FROM JAS002_200411_T_DEDUP
  WHERE Reason = 'R' AND 
        @NOW - Mon_Since BETWEEN 0 AND @i
  GROUP BY IDN
  SET @i = @i + 12
END

UPDATE JAS002_VAR_200411
SET
JAS002_R_3M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 3

UPDATE JAS002_VAR_200411
SET
JAS002_R_6M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 6

UPDATE JAS002_VAR_200411
SET
JAS002_R_12M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 12

UPDATE JAS002_VAR_200411
SET
JAS002_R_24M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 24

UPDATE JAS002_VAR_200411
SET
JAS002_R_36M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 36


DELETE FROM TMP

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 3, COUNT(*)
FROM JAS002_200411_T_DEDUP
WHERE Reason = 'S' AND 
      @NOW - Mon_Since BETWEEN 0 AND 3
GROUP BY IDN

INSERT INTO TMP(IDN, MON, V1)
SELECT IDN, 6, COUNT(*)
FROM JAS002_200411_T_DEDUP
WHERE Reason = 'S' AND 
      @NOW - Mon_Since BETWEEN 0 AND 6
GROUP BY IDN

SET @i = 12
WHILE @i <= 36
BEGIN
  INSERT INTO TMP(IDN, MON, V1)
  SELECT IDN, @i, COUNT(*)
  FROM JAS002_200411_T_DEDUP
  WHERE Reason = 'S' AND 
        @NOW - Mon_Since BETWEEN 0 AND @i
  GROUP BY IDN
  SET @i = @i + 12
END

UPDATE JAS002_VAR_200411
SET
JAS002_S_3M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 3

UPDATE JAS002_VAR_200411
SET
JAS002_S_6M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 6

UPDATE JAS002_VAR_200411
SET
JAS002_S_12M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 12

UPDATE JAS002_VAR_200411
SET
JAS002_S_24M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 24

UPDATE JAS002_VAR_200411
SET
JAS002_S_36M = V1
FROM TMP AS A INNER JOIN JAS002_VAR_200411 AS B
ON A.IDN = B.IDN AND
   Mon = 36
