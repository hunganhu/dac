USE TF_Y_JCIC
GO

SELECT COUNT(*), COUNT(DISTINCT IDN) FROM KRM023
--424643	8747

SELECT COUNT(*), COUNT(DISTINCT IDN) FROM BAM086
--68878	9639
SELECT COUNT(*), COUNT(DISTINCT IDN) FROM JAS002
--9919	9919
--Number of records adds up

SELECT COUNT(*), COUNT(DISTINCT IDN), SUM(CNT) FROM KRM023_DEDUP
--424643	8747	424643
SELECT COUNT(*), COUNT(DISTINCT IDN), SUM(CNT) FROM BAM086_DEDUP
--68878	9639	68878
SELECT COUNT(*), COUNT(DISTINCT IDN), SUM(CNT) FROM JAS002_DEDUP
--9919	9919	9919
--No duplicated record

CREATE INDEX idx ON BAM086_DEDUP(IDN)
CREATE INDEX idx ON KRM023_DEDUP(IDN)
CREATE INDEX idx ON JAS002_DEDUP(IDN)

SELECT CO_LOAN, COUNT(*) FROM BAM086_DEDUP
GROUP BY CO_LOAN
ORDER BY CO_LOAN
/*
 	68589
*	289
*/
SELECT PAY_CODE_12, COUNT(*) FROM BAM086_DEDUP
GROUP BY PAY_CODE_12
ORDER BY PAY_CODE_12

SELECT ACCOUNT_CODE2, COUNT(*) FROM BAM086_DEDUP
GROUP BY ACCOUNT_CODE2
ORDER BY ACCOUNT_CODE2
/*
 	66336
S	2542
*/
SELECT ACCOUNT_CODE, COUNT(*) FROM BAM086_DEDUP
GROUP BY ACCOUNT_CODE
ORDER BY ACCOUNT_CODE

SELECT BANK_NAME, COUNT(*) FROM BAM086_DEDUP
GROUP BY BANK_NAME
ORDER BY BANK_NAME

SELECT BANK_CODE, COUNT(*) FROM BAM086_DEDUP
GROUP BY BANK_CODE
ORDER BY BANK_CODE

SELECT LEFT(BANK_CODE,3), COUNT(*) FROM BAM086_DEDUP
GROUP BY LEFT(BANK_CODE,3)
ORDER BY LEFT(BANK_CODE,3)

SELECT LEFT(BANK_CODE,3), BANK_NAME, COUNT(*)
FROM BAM086_DEDUP
WHERE LEFT(BANK_CODE,3) > 800
GROUP BY LEFT(BANK_CODE,3), BANK_NAME
ORDER BY LEFT(BANK_CODE,3), BANK_NAME

SELECT * FROM JAS002_DEDUP

update dbo.JAS002_DEDUP
set STOP_CARD_DATE = ''
where STOP_CARD_DATE = '"'

SELECT * FROM JAS002_DEDUP
WHERE (EVER_DELINQUENT = 'Y' AND DELINQUENT_DATE = '') OR
      (EVER_BAD_CHECK = 'Y' AND BAD_CHECK_DATE = '') OR
      (EVER_REJECT = 'Y' AND REJECT_DATE = '') OR
      (EVER_STOP_CARD = 'Y' AND STOP_CARD_DATE = '')

SELECT PAY_CODE, COUNT(*)
FROM KRM023_DEDUP
GROUP BY PAY_CODE
ORDER BY PAY_CODE

SELECT CASH, COUNT(*)
FROM KRM023_DEDUP
GROUP BY CASH
ORDER BY CASH

SELECT PAYMENT, COUNT(*)
FROM KRM023_DEDUP
GROUP BY PAYMENT
ORDER BY PAYMENT

SELECT LIMIT, COUNT(*)
FROM KRM023_DEDUP
GROUP BY LIMIT
ORDER BY LIMIT

SELECT COUNT(*), COUNT(DISTINCT IDN) FROM KRM023_DEDUP WHERE LIMIT = 0
--5145	1624
SELECT PAY_CODE, COUNT(*), COUNT(DISTINCT IDN) FROM KRM023_DEDUP WHERE LIMIT = 0
GROUP BY PAY_CODE
ORDER BY PAY_CODE
/*
A	1340	255
C	95	11
D	4	3
E	94	33
F	311	94
X	3301	1563
*/
SELECT PAY_CODE, PAYMENT, COUNT(*), COUNT(DISTINCT IDN)
FROM KRM023_DEDUP
WHERE PAY_CODE IN ('D', 'E', 'F') AND LIMIT = 0
GROUP BY PAY_CODE, PAYMENT
ORDER BY PAY_CODE, PAYMENT
--We can calculate the buckets anyway

SELECT KR_CODE, COUNT(*)
FROM KRM023_DEDUP
GROUP BY KR_CODE
ORDER BY KR_CODE

SELECT ISSUE_NAME, COUNT(*)
FROM KRM023_DEDUP
GROUP BY ISSUE_NAME
ORDER BY ISSUE_NAME

SELECT ISSUE, COUNT(*)
FROM KRM023_DEDUP
GROUP BY ISSUE
ORDER BY ISSUE

SELECT YRMON, COUNT(*)
FROM KRM023_DEDUP
GROUP BY YRMON
ORDER BY YRMON

SELECT INQUIRY_DATE, COUNT(*)
FROM JAS002_DEDUP
GROUP BY INQUIRY_DATE
ORDER BY INQUIRY_DATE

SELECT INQUIRY_DATE, COUNT(*)
FROM BAM086_DEDUP
GROUP BY INQUIRY_DATE
ORDER BY INQUIRY_DATE

SELECT INQUIRY_DATE, COUNT(*)
FROM KRM023_DEDUP
GROUP BY INQUIRY_DATE
ORDER BY INQUIRY_DATE

SELECT COUNT(*)
FROM KRM023_DEDUP AS A INNER JOIN JAS002_DEDUP AS B
ON A.IDN = B.IDN
WHERE A.INQUIRY_DATE != B.INQUIRY_DATE
--0
SELECT COUNT(*)
FROM JAS002_DEDUP AS A INNER JOIN BAM086_DEDUP AS B
ON A.IDN = B.IDN
WHERE A.INQUIRY_DATE != B.INQUIRY_DATE
--0
SELECT COUNT(*)
FROM KRM023_DEDUP AS A INNER JOIN BAM086_DEDUP AS B
ON A.IDN = B.IDN
WHERE A.INQUIRY_DATE != B.INQUIRY_DATE
--0
SELECT INQUIRY_DATE, TRANCH, COUNT(*), COUNT(DISTINCT A.IDN)
FROM JAS002_DEDUP AS A INNER JOIN TF_GX.dbo.data_availability AS B
ON A.IDN = B.IDN
GROUP BY INQUIRY_DATE, TRANCH
ORDER BY INQUIRY_DATE, TRANCH




ALTER TABLE IDN_LIST ADD MAX_BUCKET INT, DELINQUENT_LOANS INT, 
                         JAS002_B_3M INT, JAS002_B_6M INT, JAS002_B_12M INT, JAS002_B_24M INT, JAS002_B_36M INT,
                         JAS002_D_3M INT, JAS002_D_6M INT, JAS002_D_12M INT, JAS002_D_24M INT, JAS002_D_36M INT,
                         JAS002_R_3M INT, JAS002_R_6M INT, JAS002_R_12M INT, JAS002_R_24M INT, JAS002_R_36M INT,
                         JAS002_S_3M INT, JAS002_S_6M INT, JAS002_S_12M INT, JAS002_S_24M INT, JAS002_S_36M INT
GO


SELECT * INTO BAM086_200502_DEDUP
FROM BAM086_DEDUP
WHERE INQUIRY_DATE = '0940201'
--68878
SELECT * INTO KRM023_200502_DEDUP
FROM KRM023_DEDUP
WHERE INQUIRY_DATE = '0940201'
--424643
SELECT * INTO JAS002_200502_DEDUP
FROM JAS002_DEDUP
WHERE INQUIRY_DATE = '0940201'
--9919