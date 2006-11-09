/*Check for inconsistant data between app_info and ploan_result*/
SELECT A.CASE_SN, A.DAC_SN
FROM App_info AS A LEFT JOIN Ploan_result AS B
ON A.CASE_SN = B.CASE_SN AND
   A.DAC_SN = B.DAC_SN
WHERE B.CASE_SN IS NULL AND
      A.Application_date BETWEEN :beg_date AND :end_date

--if this query contains data, the data will be written into file "Ploan_result_missing.log"

SELECT A.CASE_SN, A.DAC_SN
FROM Ploan_result AS A LEFT JOIN App_info AS B
ON A.CASE_SN = B.CASE_SN AND
   A.DAC_SN = B.DAC_SN
WHERE B.CASE_SN IS NULL AND
      B.Application_date BETWEEN :beg_date AND :end_date

--if this query contains data, the data will be written into file "App_info_missing.log"


/*Case_SN dedup based on the primary key: Case_SN, Max Principal(requested amount), Max EV (NPV)*/
CREATE TABLE #TMP1 (CASE_SN Char(12), Principal int)
GO

INSERT INTO #TMP1
SELECT A.CASE_SN, MAX(Principal)
FROM App_info AS A INNER JOIN Ploan_result AS B
ON A.CASE_SN = B.CASE_SN
WHERE A.Application_date BETWEEN :beg_date AND :end_date
GROUP BY A.CASE_SN
GO

CREATE INDEX idx ON #TMP1(CASE_SN, Principal)
GO

CREATE TABLE #TMP2 (CASE_SN Char(12), DAC_SN decimal(3,0))
GO 

INSERT INTO #TMP2
SELECT A.CASE_SN, A.DAC_SN
FROM App_info AS A INNER JOIN #TMP1 AS B
ON A.CASE_SN = B.CASE_SN AND
   A.Principal = B.Principal
GO

CREATE INDEX idx ON #TMP2(CASE_SN, DAC_SN)
GO

CREATE TABLE #TMP3 (CASE_SN Char(12), EV decimal(16,2))
GO

INSERT INTO #TMP3
SELECT A.CASE_SN, MAX(EV) 
FROM #TMP2 AS A INNER JOIN Ploan_result AS B
ON A.CASE_SN = B.CASE_SN AND 
   A.DAC_SN
GROUP BY CASE_SN
GO

CREATE TABLE #TMP4 (CASE_SN Char(12), DAC_SN decimal(3,0))
GO

INERST INTO #TMP4
SELECT A.CASE_SN, A.DAC_SN
FROM Ploan_result AS A INNER JOIN #TMP3 AS B
ON A.CASE_SN = B.CASE_SN AND
   A.EV = B.EV
GO

CREATE TABLE #TMP5 (CASE_SN Char(12), DAC_SN decimal(3,0))
GO

INSERT INTO #TMP5
SELECT CASE_SN, MIN(DAC_SN)
FROM #TMP4
GROUP BY CASE_SN
GO

CRETATE TABLE #MODULE_MASTER
(CASE_SN Char(12), DAC_SN decimal(3,0), 
 Principal int, 
 EV decimal(16,2), PB decimal(10,6), Return_msg varchar(4))
GO

INSERT INTO #MODULE_MASTER(CASE_SN, DAC_SN, Principal)
SELECT A.CASE_SN, A.DAC_SN, Principal     
FROM App_info AS A INNER JOIN #TMP5 AS B
ON A.CASE_SN = B.CASE_SN AND
   A.DAC_SN = B.DAC_SN
GO

UPDATE #MODULE_MASTER
SET
EV = A.EV,
PB = A.PB,
Return_msg = LEFT(A.Return_msg,2)
FROM Ploan_result AS A
WHERE A.CASE_SN = #MODULE_MASTER.CASE_SN AND
      A.DAC_SN = #MODULE_MASTER.DAC_SN
GO

/*Check for inconsistant data between app_r and ploan_result*/
SELECT A.CASE_SN, A.DAC_SN
FROM App_r AS A LEFT JOIN Ploan_result AS B
ON A.CASE_SN = B.CASE_SN AND
   A.DAC_SN = B.DAC_SN
WHERE B.CASE_SN IS NULL AND
      A.Application_date BETWEEN :beg_date AND :end_date

--if this query contains data, the data will be written into file "no_module_output.log"

CREATE TABLE #RESULT_MASTER
(CASE_SN Char(12), DAC_SN decimal(3,0)
 Principal int, 
 EV decimal(16,2), PB decimal(10,6), Return_msg varchar(4),
 Decision int)
GO

INSERT INTO #RESULT_MASTER
SELECT A.CASE_SN, A.DAC_SN, A.Principal, A.EV, A.PB, A.Return_msg,
       B.Decisison
FROM #MODULE_MASTER AS A INNER JOIN App_r AS B
ON A.CASE_SN = B.CASE_SN AND
   A.DAC_SN = B.DAC_SN
GO

/*Report generation codes*/           
 
 


   