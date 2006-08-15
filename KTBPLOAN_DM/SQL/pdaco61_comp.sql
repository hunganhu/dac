CREATE VIEW KRM023_DEDUP AS
SELECT * FROM COMBINED_KRM023_DEDUP
GO

CREATE VIEW STM007_DEDUP AS
SELECT * FROM STM001_DEDUP
GO


SELECT A.MSN, A.INPUT_TIME, B.INPUT_TIME FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.INPUT_TIME != B.INPUT_TIME

SELECT A.MSN, A.REQUEST_AMT, B.REQUEST_AMT FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.REQUEST_AMT != B.REQUEST_AMT

SELECT A.MSN, A.APR, B.APR FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.APR != B.APR

SELECT A.MSN, A.TOTAL_TERM, B.TOTAL_TERM FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.TOTAL_TERM != B.TOTAL_TERM

SELECT A.MSN, A.APPROVED_AMT, B.APPROVED_AMT FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.APPROVED_AMT != B.APPROVED_AMT

SELECT A.MSN, A.KRM001_HIT, B.KRM021_HIT FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.KRM001_HIT != B.KRM021_HIT

SELECT A.MSN, A.KRM023_HIT, B.KRM023_HIT FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.KRM023_HIT != B.KRM023_HIT

SELECT A.MSN, A.BAM086_HIT, B.BAM086_HIT FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.BAM086_HIT != B.BAM086_HIT

SELECT A.MSN, A.IND001, B.IND001 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.IND001 != B.IND001
-- 0

SELECT A.MSN, A.JAS002_DEFECT, B.JAS002_DEFECT FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.JAS002_DEFECT != B.JAS002_DEFECT
--0

SELECT A.MSN, A.FS031, B.FS031 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS031 != B.FS031
--0

SELECT A.MSN, A.FS031_1M, B.FS031_1M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS031_1M != B.FS031_1M
--0

SELECT A.MSN, A.RS017, B.RS017 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.RS017 != B.RS017

SELECT A.MSN, A.STOP_CODE_FLAG, B.STOP_CODE_FLAG FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.STOP_CODE_FLAG != B.STOP_CODE_FLAG

SELECT A.MSN, A.FS036, B.FS036 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS036 != B.FS036
-- 0

SELECT A.MSN, A.FS040, B.FS040 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS040 != B.FS040
-- 0

SELECT A.MSN, A.FS044, B.FS044 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS044 != B.FS044
-- 0

SELECT A.MSN, A.MS063, B.MS063 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.MS063 != B.MS063
-- 0

SELECT A.MSN, A.MS074, B.MS074 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.MS074 != B.MS074
-- 0

SELECT A.MSN, A.MS093, B.MS093 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.MS093 != B.MS093
-- 0

SELECT A.MSN, A.MS105, B.MS105 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.MS105 != B.MS105
-- 0

SELECT A.MSN, A.FS302, B.FS302 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.FS302 != B.FS302
-- 0

SELECT A.MSN, A.FS309, B.FS309 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.FS309 != B.FS309
-- 0

SELECT A.MSN, A.FS334, B.FS334 FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.FS334 != B.FS334
-- 0

SELECT A.MSN, A.FS334B_1M, B.FS334B_1M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS334B_1M != B.FS334B_1M
-- 0

SELECT A.MSN, A.FS546_9M, B.FS546_9M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS546_9M != B.FS546_9M
-- 0

SELECT A.MSN, A.FS314B, B.FS314B FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.FS314B != B.FS314B
-- 0

SELECT A.MSN, A.APP_MAX_BUCKET, B.APP_MAX_BUCKET FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.APP_MAX_BUCKET != B.APP_MAX_BUCKET
-- 0

 UPDATE PDACO_V6_1
 SET CDEF_FLAG_1M   = 0
 WHERE KRM023_HIT = 1;

SELECT A.MSN, A.CDEF_FLAG_1M, B.CDEF_FLAG_1M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.CDEF_FLAG_1M != B.CDEF_FLAG_1M
-- 59 WITH CASH=Y AND PRE-OWE >0
/*
2006010144592       	0	1.00000000
2006010071909       	0	1.00000000
2006070083670       	0	1.00000000
2006100067889       	0	1.00000000
2006070087569       	0	1.00000000
2006010394581       	0	1.00000000
2006010311228       	0	1.00000000
2006010706058       	0	1.00000000
2006070164976       	0	1.00000000
2006100060300       	0	1.00000000
2006010856348       	0	1.00000000
2006010212000       	0	1.00000000
2006010193464       	0	1.00000000

SELECT * FROM dbo.COMBINED_KRM023_DEDUP WHERE MSN='2006010144592' ORDER BY MON_SINCE

SELECT MSN, MON_SINCE
FROM	COMBINED_KRM023_DEDUP AS A
WHERE	INQUIRY_MON-1 - MON_SINCE <= 1
AND		(PAY_CODE IN ('C','D','E','F') OR
		 (CASH = 'Y' AND ISNULL(SPREAD_PAYMENT,0) > 0))

SELECT MSN, SUM(ISNULL(SPREAD_PAYMENT,0))
FROM	COMBINED_KRM023_DEDUP AS A
WHERE	INQUIRY_MON-1 - MON_SINCE <= 1
AND CASH='Y'
AND MSN IN 
(SELECT A.MSN
FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.CDEF_FLAG_1M != B.CDEF_FLAG_1M)
GROUP BY MSN


*/
SELECT A.MSN, A.FS002_3M_1K, B.FS002_3M_1K FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS002_3M_1K != B.FS002_3M_1K
-- 0


SELECT A.MSN, A.FS014_6M, B.FS014_6M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS014_6M != B.FS014_6M
-- 0

SELECT A.MSN, A.FS016C_9M, B.FS016C_9M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS016C_9M != B.FS016C_9M
-- 0

SELECT A.MSN, A.FS016F_12M, B.FS016F_12M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS016F_12M != B.FS016F_12M

-- 0

SELECT A.MSN, A.FS018_12M, B.FS018_12M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS018_12M != B.FS018_12M

SELECT A.MSN, A.FS021_9M, B.FS021_9M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS021_9M != B.FS021_9M
-- 0

SELECT A.MSN, A.FS059_12M_1K, B.FS059_12M_1K FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS059_12M_1K != B.FS059_12M_1K
-- 0

SELECT A.MSN, A.FS059_3M_1K, B.FS059_3M_1K FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS059_3M_1K != B.FS059_3M_1K
-- 0

SELECT A.MSN, A.FS061_6M_1K, B.FS061_6M_1K FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS061_6M_1K != B.FS061_6M_1K
-- 0

SELECT A.MSN, A.FS073B_12M, B.FS073B_12M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.FS073B_12M != B.FS073B_12M
-- 0

SELECT A.MSN, A.FS101_6M, B.FS101_6M FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.FS101_6M != B.FS101_6M
-- 0

SELECT A.MSN, A.FS205_3M_1K, B.FS205_3M_1K FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.FS205_3M_1K != B.FS205_3M_1K
--**** 197
/*
2006100018564       	10	.00000000
2006040092171       	13	8.00000000
2006010193442       	3	.00000000
001-01-0000760      	15	14.00000000
2006011010260       	10	5.00000000
2006010238244       	7	5.00000000
2006010097198       	5	.00000000
2006010986538       	8	4.00000000
2006010529817       	10	5.00000000
2006050063434       	8	4.00000000

SELECT * FROM COMBINED_KRM023_DEDUP WHERE MSN='2006010193442' ORDER BY MON_SINCE, ISSUE
SELECT * FROM KRM021_DEDUP WHERE MSN='2006010193442'
SELECT * FROM OPEN_CARD WHERE IDN='A121999209'
SELECT * FROM OPEN_LINE WHERE IDN='A121999209' order by ISSUE, MON
SELECT * FROM LATEST_STMT_MON WHERE IDN='A121999209'
SELECT * FROM LATEST_LINE WHERE IDN='A121999209'


*/
SELECT A.MSN, A.MS001_12M_1K, B.MS001_12M_1K, abs(A.MS001_12M_1K - B.MS001_12M_1K)
FROM KTB_BASE_20060710 A, PDACO_V6_1 B
WHERE A.MSN = B.MSN AND A.MS001_12M_1K != B.MS001_12M_1K
order by abs(A.MS001_12M_1K - B.MS001_12M_1K) desc
-- 81 2E-13

SELECT A.MSN, A.MS094B, B.MS094B, ABS(A.MS094B - B.MS094B)
FROM KTB_BASE_20060710 A, PDACO_V6_1 B
WHERE A.MSN = B.MSN AND A.MS094B != B.MS094B
ORDER BY ABS(A.MS094B - B.MS094B) DESC
-- 2118 3E-9
/*
2006010336298       	183.90700000000001	.00000000
2006050085905       	279.88799999999998	.00000000
2006010658899       	279.89766666666668	.00000000
2006010880375       	164.75033333333334	120.00000000
2006010340590       	24.244	.00000000
2006070143680       	153.42533333333333	.00000000
*/
SELECT A.MSN, A.MS118_9M, B.MS118_9M, abs(A.MS118_9M - B.MS118_9M)
FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND A.MS118_9M != B.MS118_9M
order by abs(A.MS118_9M - B.MS118_9M) desc
-- 1666 5E-9

SELECT A.MSN, A.MS601, B.MS601, ABS(A.MS601 - B.MS601)
FROM KTB_BASE_20060710 A, PDACO_V6_1 B
WHERE A.MSN = B.MSN AND
 A.MS601 != B.MS601
ORDER BY ABS(A.MS601 - B.MS601) DESC

-- 624 9E-13

SELECT A.MSN, A.MS605, B.MS605, ABS(A.MS605 - B.MS605) FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.MS605 != B.MS605
ORDER BY ABS(A.MS605 - B.MS605) DESC
-- 1075 5E-9

SELECT A.MSN, A.WI001_9M, B.WI001_9M, ABS(A.WI001_9M - B.WI001_9M) FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.WI001_9M != B.WI001_9M
ORDER BY ABS(A.WI001_9M - B.WI001_9M) DESC
--2219 9E-8

SELECT A.MSN, A.WI003_9M, B.WI003_9M, ABS(A.WI003_9M - B.WI003_9M) FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.WI003_9M != B.WI003_9M
ORDER BY ABS(A.WI003_9M - B.WI003_9M) DESC
-- 566 8E-7

SELECT A.MSN, A.WI004_9M, B.WI004_9M, ABS(A.WI004_9M - B.WI004_9M) FROM KTB_BASE_20060710 A, PDACO_V6_1 B 
WHERE A.MSN = B.MSN AND
 A.WI004_9M != B.WI004_9M
ORDER BY ABS(A.WI004_9M - B.WI004_9M) DESC
-- 2307 9E-8
