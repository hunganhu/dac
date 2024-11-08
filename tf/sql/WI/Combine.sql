USE TF_KHJ
GO

DROP TABLE WI_MASTER
GO
SELECT * INTO WI_MASTER
FROM WI_MASTER_200402
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200403
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200404
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200405
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200406
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200407
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200408
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200409
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200410
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200411
GO
INSERT INTO WI_MASTER
SELECT * FROM WI_MASTER_200412
GO

DROP TABLE WI_VAR_GEN_BED
GO

CREATE TABLE WI_VAR_GEN_BED
(IDN CHAR(10), VAR CHAR(5), MON INT, VAL FLOAT)
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 1, WI001_1
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 2, WI001_2
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 3, WI001_3
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 4, WI001_4
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 5, WI001_5
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 6, WI001_6
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 7, WI001_7
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 8, WI001_8
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 9, WI001_9
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 10, WI001_10
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 11, WI001_11
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI001', 12, WI001_12
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 1, WI002_1
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 2, WI002_2
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 3, WI002_3
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 4, WI002_4
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 5, WI002_5
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 6, WI002_6
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 7, WI002_7
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 8, WI002_8
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 9, WI002_9
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 10, WI002_10
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 11, WI002_11
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI002', 12, WI002_12
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 1, WI003_1
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 2, WI003_2
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 3, WI003_3
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 4, WI003_4
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 5, WI003_5
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 6, WI003_6
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 7, WI003_7
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 8, WI003_8
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 9, WI003_9
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 10, WI003_10
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 11, WI003_11
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI003', 12, WI003_12
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 1, WI004_1
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 2, WI004_2
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 3, WI004_3
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 4, WI004_4
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 5, WI004_5
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 6, WI004_6
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 7, WI004_7
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 8, WI004_8
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 9, WI004_9
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 10, WI004_10
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 11, WI004_11
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI004', 12, WI004_12
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 1, WI005_1
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 2, WI005_2
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 3, WI005_3
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 4, WI005_4
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 5, WI005_5
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 6, WI005_6
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 7, WI005_7
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 8, WI005_8
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 9, WI005_9
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 10, WI005_10
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 11, WI005_11
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI005', 12, WI005_12
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 1, WI006_1
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 2, WI006_2
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 3, WI006_3
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 4, WI006_4
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 5, WI006_5
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 6, WI006_6
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 7, WI006_7
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 8, WI006_8
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 9, WI006_9
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 10, WI006_10
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 11, WI006_11
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI006', 12, WI006_12
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 1, WI007_1
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 2, WI007_2
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 3, WI007_3
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 4, WI007_4
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 5, WI007_5
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 6, WI007_6
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 7, WI007_7
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 8, WI007_8
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 9, WI007_9
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 10, WI007_10
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 11, WI007_11
FROM WI_MASTER
GO

INSERT INTO WI_VAR_GEN_BED
SELECT IDN, 'WI007', 12, WI007_12
FROM WI_MASTER
GO

ALTER TABLE WI_MASTER
ADD WI001_3M FLOAT, WI001_6M FLOAT, WI001_9M FLOAT, WI001_12M FLOAT, --CREDIT LINES, AVERAGE IN THE WINDOW
    WI002_3M FLOAT, WI002_6M FLOAT, WI002_9M FLOAT, WI002_12M FLOAT, --CREDIT LINES WITH LIMIT > NT$100K, AVERAGE IN THE WINDOW
    WI003_3M FLOAT, WI003_6M FLOAT, WI003_9M FLOAT, WI003_12M FLOAT, --CREDIT LINES WITH LIMIT > NT$200K, AVERAGE IN THE WINDOW
    WI004_3M FLOAT, WI004_6M FLOAT, WI004_9M FLOAT, WI004_12M FLOAT, --CREDIT LIMIT AMOUNT, AVERAGE IN THE WINDOW
    WI005_3M FLOAT, WI005_6M FLOAT, WI005_9M FLOAT, WI005_12M FLOAT, --PAY CODE IN 'A' 'B' AMOUNT, TOTAL IN THE WINDOW
    WI006_3M FLOAT, WI006_6M FLOAT, WI006_9M FLOAT, WI006_12M FLOAT, --AVERAGE REVOLVING BALANCE DECLINE AMOUNT, PAY CODE 'C' 'D'
    WI007_3M FLOAT, WI007_6M FLOAT, WI007_9M FLOAT, WI007_12M FLOAT, --AVERAGE REVOLVING BALANCE DECLINE AMOUNT, PAY CODE 'C' 'D' 'E' 'F'
    WI008_3M FLOAT, WI008_6M FLOAT, WI008_9M FLOAT, WI008_12M FLOAT, --PAY CODE IN 'A' 'B' AMOUNT TOTAL IN THE WINDOW, FOR PURE TRANSACTOR IN THE MONTH
    WI009_3M FLOAT, WI009_6M FLOAT, WI009_9M FLOAT, WI009_12M FLOAT, --AVERAGE REVOLVING BALANCE DECLINE AMOUNT, PAY CODE 'C' 'D', FOR PURE REVOLVER IN THE MONTH, PEOPLE EVER HAVE PAY CODE 'E' 'F' ARE EXCLUDED FROM THE VARIABLE 
    WI010_3M FLOAT, WI010_6M FLOAT, WI010_9M FLOAT, WI010_12M FLOAT  --AVERAGE REVOLVING BALANCE DECLINE AMOUNT, PAY CODE 'C' 'D' 'E' 'F', FOR PURE REVOLVER IN THE MONTH
GO

----WI001_XM----
DELETE FROM WI_TMP2

INSERT INTO WI_TMP2
SELECT IDN, 3, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI001' AND
      MON BETWEEN 1 AND 3
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 6, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI001' AND
      MON BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI001' AND
      MON BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI001' AND
      MON BETWEEN 1 AND 12
GROUP BY IDN
   
UPDATE WI_MASTER
SET
WI001_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI001_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI001_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI001_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO

----WI002_XM----
DELETE FROM WI_TMP2

INSERT INTO WI_TMP2
SELECT IDN, 3, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI002' AND
      MON BETWEEN 1 AND 3
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 6, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI002' AND
      MON BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI002' AND
      MON BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI002' AND
      MON BETWEEN 1 AND 12
GROUP BY IDN
   
UPDATE WI_MASTER
SET
WI002_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI002_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI002_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI002_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO

----WI003_XM----
DELETE FROM WI_TMP2

INSERT INTO WI_TMP2
SELECT IDN, 3, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI003' AND
      MON BETWEEN 1 AND 3
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 6, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI003' AND
      MON BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI003' AND
      MON BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI003' AND
      MON BETWEEN 1 AND 12
GROUP BY IDN
   
UPDATE WI_MASTER
SET
WI003_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI003_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI003_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI003_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO

----WI004_XM----
DELETE FROM WI_TMP2

INSERT INTO WI_TMP2
SELECT IDN, 3, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI004' AND
      MON BETWEEN 1 AND 3
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 6, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI004' AND
      MON BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI004' AND
      MON BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI004' AND
      MON BETWEEN 1 AND 12
GROUP BY IDN
   
UPDATE WI_MASTER
SET
WI004_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI004_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI004_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI004_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO

----WI005_XM----
DELETE FROM WI_TMP2

INSERT INTO WI_TMP2
SELECT IDN, 3, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI005' AND
      MON BETWEEN 1 AND 3
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 6, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI005' AND
      MON BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI005' AND
      MON BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI005' AND
      MON BETWEEN 1 AND 12
GROUP BY IDN
   
UPDATE WI_MASTER
SET
WI005_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI005_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI005_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI005_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO

----WI006_XM----
DELETE FROM WI_TMP2

INSERT INTO WI_TMP2
SELECT IDN, 3, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI006' AND
      MON BETWEEN 1 AND 3
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 6, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI006' AND
      MON BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI006' AND
      MON BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI006' AND
      MON BETWEEN 1 AND 12
GROUP BY IDN
   
UPDATE WI_MASTER
SET
WI006_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI006_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI006_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI006_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO

----WI007_XM----
DELETE FROM WI_TMP2

INSERT INTO WI_TMP2
SELECT IDN, 3, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI007' AND
      MON BETWEEN 1 AND 3
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 6, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI007' AND
      MON BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI007' AND
      MON BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, AVG(VAL)
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI007' AND
      MON BETWEEN 1 AND 12
GROUP BY IDN
   
UPDATE WI_MASTER
SET
WI007_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI007_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI007_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI007_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO

----WI008_XM----
DELETE FROM WI_TMP1
DELETE FROM WI_TMP2

DECLARE @NOW INT
SET @NOW = 93 * 12 + 2
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200402
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 3
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200403
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 4
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200404
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 5
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200405
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 6
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200406
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 7
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200407
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 8
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200408
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 9
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200409
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 10
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200410
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 11
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200411
GROUP BY IDN, MON_SINCE
GO
DECLARE @NOW INT
SET @NOW = 93 * 12 + 12
INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, @NOW - MON_SINCE, SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END)
FROM KRM023_200412
GROUP BY IDN, MON_SINCE
GO

UPDATE WI_TMP1
SET
V1 = (CASE WHEN V2 > 0 THEN NULL ELSE VAL END)
FROM WI_VAR_GEN_BED AS A INNER JOIN WI_TMP1 AS B
ON A.IDN = B.IDN AND A.MON = B.MON_SINCE
WHERE VAR = 'WI005'

INSERT INTO WI_TMP2
SELECT IDN, 3, SUM(V1)
FROM WI_TMP1
WHERE MON_SINCE BETWEEN 1 AND 3
GROUP BY IDN
GO
INSERT INTO WI_TMP2
SELECT IDN, 6, SUM(V1)
FROM WI_TMP1
WHERE MON_SINCE BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, SUM(V1)
FROM WI_TMP1
WHERE MON_SINCE BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, SUM(V1)
FROM WI_TMP1
WHERE MON_SINCE BETWEEN 1 AND 12
GROUP BY IDN
   
UPDATE WI_MASTER
SET
WI008_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI008_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI008_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI008_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO

----WI009_XM----
DELETE FROM WI_TMP1
DELETE FROM WI_TMP2

INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, MON, VAL
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI005'

UPDATE WI_TMP1
SET
V1 = (CASE WHEN V2>0 THEN NULL ELSE VAL END)
FROM WI_VAR_GEN_BED AS A INNER JOIN WI_TMP1 AS B
ON A.IDN = B.IDN AND
   A.MON = B.MON_SINCE
WHERE VAR = 'WI006'

INSERT INTO WI_TMP2
SELECT IDN, 3, AVG((CASE WHEN V1 = 0 THEN NULL ELSE V1 END))
FROM WI_TMP1
WHERE V1 IS NOT NULL AND 
      MON_SINCE BETWEEN 1 AND 3
GROUP BY IDN
      
INSERT INTO WI_TMP2
SELECT IDN, 6, AVG((CASE WHEN V1 = 0 THEN NULL ELSE V1 END))
FROM WI_TMP1
WHERE V1 IS NOT NULL AND 
      MON_SINCE BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, AVG((CASE WHEN V1 = 0 THEN NULL ELSE V1 END))
FROM WI_TMP1
WHERE V1 IS NOT NULL AND 
      MON_SINCE BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, AVG((CASE WHEN V1 = 0 THEN NULL ELSE V1 END))
FROM WI_TMP1
WHERE V1 IS NOT NULL AND 
      MON_SINCE BETWEEN 1 AND 12
GROUP BY IDN

UPDATE WI_TMP2
SET
V1 = 0
WHERE V1 IS NULL
   
UPDATE WI_MASTER
SET
WI009_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI009_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI009_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI009_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO

----WI010_XM----
DELETE FROM WI_TMP1
DELETE FROM WI_TMP2

INSERT INTO WI_TMP1(IDN, MON_SINCE, V2)
SELECT IDN, MON, VAL
FROM WI_VAR_GEN_BED
WHERE VAR = 'WI005'

UPDATE WI_TMP1
SET
V1 = (CASE WHEN V2>0 THEN NULL ELSE VAL END)
FROM WI_VAR_GEN_BED AS A INNER JOIN WI_TMP1 AS B
ON A.IDN = B.IDN AND
   A.MON = B.MON_SINCE
WHERE VAR = 'WI007'

INSERT INTO WI_TMP2
SELECT IDN, 3, AVG((CASE WHEN V1 = 0 THEN NULL ELSE V1 END))
FROM WI_TMP1
WHERE V1 IS NOT NULL AND 
      MON_SINCE BETWEEN 1 AND 3
GROUP BY IDN
      
INSERT INTO WI_TMP2
SELECT IDN, 6, AVG((CASE WHEN V1 = 0 THEN NULL ELSE V1 END))
FROM WI_TMP1
WHERE V1 IS NOT NULL AND 
      MON_SINCE BETWEEN 1 AND 6
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 9, AVG((CASE WHEN V1 = 0 THEN NULL ELSE V1 END))
FROM WI_TMP1
WHERE V1 IS NOT NULL AND 
      MON_SINCE BETWEEN 1 AND 9
GROUP BY IDN

INSERT INTO WI_TMP2
SELECT IDN, 12, AVG((CASE WHEN V1 = 0 THEN NULL ELSE V1 END))
FROM WI_TMP1
WHERE V1 IS NOT NULL AND 
      MON_SINCE BETWEEN 1 AND 12
GROUP BY IDN

UPDATE WI_TMP2
SET
V1 = 0
WHERE V1 IS NULL
   
UPDATE WI_MASTER
SET
WI010_3M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 3

UPDATE WI_MASTER
SET
WI010_6M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 6

UPDATE WI_MASTER
SET
WI010_9M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 9

UPDATE WI_MASTER
SET
WI010_12M = V1
FROM WI_TMP2 AS A INNER JOIN WI_MASTER AS B
ON A.IDN = B.IDN
WHERE A.MON_SINCE = 12
GO
