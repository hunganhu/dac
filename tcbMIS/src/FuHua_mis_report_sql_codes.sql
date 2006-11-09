/*Check for inconsistant data between app_info and ploan_result*/
SELECT A.CASE_SN, A.DAC_SN, A.IDN
FROM Ploan_result AS A LEFT JOIN App_info AS B
ON A.CASE_SN = B.CASE_SN AND
   A.DAC_SN = B.DAC_SN AND
   A.IDN = B.IDN
WHERE B.CASE_SN IS 0 AND
      B.Application_date BETWEEN :beg_date AND :end_date      

--if this query contains data, the data will be written into file "App_info_missing.log"


/*Create base view*/
DROP VIEW BASE
GO

CREATE VIEW BASE
AS
SELECT A.CASE_SN, A.IDN, A.DAC_SN, A.Principal, B.EV, B.PB,
       (CASE 
         WHEN B.EV >= 1000 AND (A.Segment IN ('01', '02') AND A.Principal <= 1100000) THEN 1
         WHEN B.EV >= 1000 AND (A.Segment IN ('01', '02') AND A.Principal > 1100000) THEN 2
         WHEN B.EV >= 1000 AND (A.Segment IN ('03', '04') AND A.Principal <= 900000) THEN 1
         WHEN B.EV >= 1000 AND (A.Segment IN ('03', '04') AND A.Principal > 900000) THEN 2
         WHEN B.EV >= 1000 AND (A.Segment IN ('05') AND A.Principal <= 700000) THEN 1
         WHEN B.EV >= 1000 AND (A.Segment IN ('05') AND A.Principal > 700000) THEN 2
         WHEN B.EV >= 1000 AND (A.Segment NOT IN ('01', '02', '03', '04', '05') AND A.Principal <= 450000) THEN 1
         WHEN B.EV >= 1000 AND (A.Segment NOT IN ('01', '02', '03', '04', '05') AND A.Principal > 450000) THEN 2         
         WHEN B.EV < 1000 AND B.EV >= -1000 THEN 2   
         WHEN B.EV < -1000 THEN 3
         WHEN B.EV IS 0 AND B.Checksum1 IN (101, 102) THEN 2
         WHEN B.EV IS 0 AND B.Chencksum1 BETWEEN 103 AND 107 THEN 3
        END) AS MODULE_DECISION
FROM App_info AS A INNER JOIN Ploan_result AS B
ON A.CASE_SN = B.CASE_SN AND
   A.DAC_SN = B.DAC_SN AND
   A.IDN = B.IDN
WHERE A.Application_date BETWEEN :beg_date AND :end_date
GO   

CREATE TABLE #REPORT_BASE
(CASE_SN char(12), PB decimal(10,6), EV decimal(16,2), Principal float, Module_decision float, Region float, Principal_group float, Final_Decision float)
GO

INSERT INTO #REPORT_BASE(CASE_SN)
SELECT DISTINCT CASE_SN
FROM BASE
GO        

CREATE TABLE #TMP1 (CASE_SN char(12) DAC_SN decimal(3,0))
GO

INSERT INTO #TMP1
SELECT CASE_SN, MAX(DAC_SN)
FROM BASE
GROUP BY CASE_SN
GO

CREATE TABLE #TMP2 (CASE_SN char(12), PB decimal(10,6))
GO

INSERT INTO #TMP2
SELECT A.CASE_SN, A.PB
FROM BASE AS A INNER JOIN #TMP1 AS B
ON A.CASE_SN = B.CASE_SN AND
   A.DAC_SN = B.DAC_SN
GO

--PB is taken from the latest record
UPDATE #REPORT_BASE
SET
PB = A.PB
FROM #TMP2 AS A
WHERE A.CASE_SN = #REPORT_BASE.CASE_SN
GO
   
DROP VIEW Principal
GO

CREATE VIEW Principal
AS
SELECT CASE_SN, MAX(Principal) AS Max_principal
FROM BASE
GROUP BY CASE_SN
GO

--Largest amount is taken as the principal and the approved amount
UPDATE #REPORT_BASE
SET
Principal = A.Max_principal
FROM principal AS A
WHERE #REPORT_BASE.CASE_SN = A.CASE_SN
GO

DROP VIEW EV
GO

CREATE VIEW EV
AS
SELECT CASE_SN, MAX(EV) AS Max_EV
FROM BASE
GROUP BY CASE_SN
GO 

--Largest NPV value is taken
UPDATE #REPORT_BASE
SET
EV = A.Max_EV
FROM EV AS A
WHERE #REPORT_BASE.CASE_SN = A.CASE_SN
GO

DROP VIEW MODULE_DECISION
GO

CREATE VIEW MODULE_DECISION
AS
SELECT CASE_SN, MIN(MODULE_DECISION) AS DECISION
FROM BASE
GROUP BY CASE_SN
GO

--The order to take the module result: approved > manual review > decline
UPDATE #REPORT_BASE
SET
MODULE_DECISION = DECISISON
FROM MODULE_DECISION AS A
WHERE A.CASE_SN = #REPORT_BASE.CASE_SN
GO

--Assign region code, based on information from §Å«T©ú
UPDATE #REPORT_BASE
SET
Region = CONVERT(float,RIGHT(LEFT(CASE_SN,3),1))
GO


UPDATE #REPORT_BASE
SET
Principal_group = (CASE
                     WHEN Principal <= 1100000 THEN 1
                     WHEN Principal > 1100000 AND Principal <= 2000000 THEN 2
                     WHEN Principal > 2000000 THEN 3
                   END)
GO                     

UPDATE #REPORT_BASE
SET
Final_Decision = Decision
FROM App_r AS A
WHERE A.CASE_SN = #REPORT_BASE.CASE_SN
GO

--Base table for report part one
CREATE TABLE #ONE 
(Principal_group float, Region float,
 NS float, I float, II float, III float, IV float, V float, VI float, VII float, VIII float, IX float, X float, XI float,
 NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float)
GO
INSERT INTO #ONE
SELECT Principal_group, Region,
       SUM(CASE WHEN EV IS 0 THEN 1 ELSE 0 END) AS NS,
       SUM(CASE WHEN PB <= 0.005 THEN 1 ELSE 0 END) AS I,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 THEN 1 ELSE 0 END) AS II,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 THEN 1 ELSE 0 END) AS III,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 THEN 1 ELSE 0 END) AS IV,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 THEN 1 ELSE 0 END) AS V,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 THEN 1 ELSE 0 END) AS VI,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 THEN 1 ELSE 0 END) AS VII,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 THEN 1 ELSE 0 END) AS VIII,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 THEN 1 ELSE 0 END) AS IX,
       SUM(CASE WHEN PB > 0.045 THEN 1 ELSE 0 END) AS X,
       COUNT(*) AS XI,
       SUM(CASE WHEN EV IS 0 THEN Principal ELSE 0 END) AS NSD,
       SUM(CASE WHEN PB <= 0.005 THEN Principal ELSE 0 END) AS ID,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 THEN Principal ELSE 0 END) AS IID,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 THEN Principal ELSE 0 END) AS IIID,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 THEN Principal ELSE 0 END) AS IVD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 THEN Principal ELSE 0 END) AS VD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 THEN Principal ELSE 0 END) AS VID,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 THEN Principal ELSE 0 END) AS VIID,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 THEN Principal ELSE 0 END) AS VIIID,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 THEN Principal ELSE 0 END) AS IXD,
       SUM(CASE WHEN PB > 0.045 THEN Principal ELSE 0 END) AS XD,
       SUM(Principal) AS XID
FROM #REPORT_BASE
GROUP BY Principal_group, Region       
GO

--Table for data in portion two of the report
CREATE TABLE #TWO
(Principal_group float, Region float,
 NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float,
 NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float,
 NSMR float, IMR float, IIMR float, IIIMR float, IVMR float, VMR float, VIMR float, VIIMR float, VIIIMR float, IXMR float, XMR float, XIMR float,
 NSMRD float, IMRD float, IIMRD float, IIIMRD float, IVMRD float, VMRD float, VIMRD float, VIIMRD float, VIIIMRD float, IXMRD float, XMRD float, XIMRD float,
 NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float,
 NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float, 
 NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float)
GO

INSERT INTO #TWO
SELECT Principal_group, Region,
       0,
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IA,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IIA,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IIIA,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IVA,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN 1 ELSE 0 END) AS VA,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN 1 ELSE 0 END) AS VIA,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN 1 ELSE 0 END) AS VIIA,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN 1 ELSE 0 END) AS VIIIA,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IXA,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN 1 ELSE 0 END) AS XA,
       SUM(CASE WHEN Module_decision = 1 THEN 1 ELSE 0 END) AS XIA,
       0,       
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IAD,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IIAD,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IIIAD,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IVAD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN Principal ELSE 0 END) AS VAD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN Principal ELSE 0 END) AS VIAD,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN Principal ELSE 0 END) AS VIIAD,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN Principal ELSE 0 END) AS VIIIAD,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IXAD,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN Principal ELSE 0 END) AS XAD,
       SUM(CASE WHEN Module_decision = 1 THEN Principal ELSE 0 END) AS XIAD,
       SUM(CASE WHEN EV IS 0 AND Module_decision = 2 THEN 1 ELSE 0 END) AS NSMR,
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IMR,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IIMR,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IIIMR,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IVMR,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 2 THEN 1 ELSE 0 END) AS VMR,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 2 THEN 1 ELSE 0 END) AS VIMR,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 2 THEN 1 ELSE 0 END) AS VIIMR,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 2 THEN 1 ELSE 0 END) AS VIIIMR,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IXMR,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 2 THEN 1 ELSE 0 END) AS XMR,
       SUM(CASE WHEN Module_decision = 2 THEN 1 ELSE 0 END) AS XIMR,
       SUM(CASE WHEN EV IS 0 AND Module_decision = 2 THEN Principal ELSE 0 END) AS NSMRD,         
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IMRD,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IIMRD,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IIIMRD,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IVMRD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 2 THEN Principal ELSE 0 END) AS VMRD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 2 THEN Principal ELSE 0 END) AS VIMRD,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 2 THEN Principal ELSE 0 END) AS VIIMRD,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 2 THEN Principal ELSE 0 END) AS VIIIMRD,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IXMRD,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 2 THEN Principal ELSE 0 END) AS XMRD,
       SUM(CASE WHEN Module_decision = 2 THEN Principal ELSE 0 END) AS XIMRD,       
       0, 
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN PB ELSE 0 END) AS IPB,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN PB ELSE 0 END) AS IIPB,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN PB ELSE 0 END) AS IIIPB,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN PB ELSE 0 END) AS IVPB,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN PB ELSE 0 END) AS VPB,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN PB ELSE 0 END) AS VIPB,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN PB ELSE 0 END) AS VIIPB,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN PB ELSE 0 END) AS VIIIPB,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN PB ELSE 0 END) AS IXPB,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN PB ELSE 0 END) AS XPB,
       SUM(CASE WHEN Module_decision = 1 THEN PB ELSE 0 END) AS XIPB,       
       0,
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IPBD,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IIPBD,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IIIPBD,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IVPBD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS VPBD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS VIPBD,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS VIIPBD,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS VIIIPBD,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IXPBD,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS XPBD,
       SUM(CASE WHEN Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS XIPBD,       
       0, 
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN EV ELSE 0 END) AS INPV,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN EV ELSE 0 END) AS IINPV,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN EV ELSE 0 END) AS IIINPV,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN EV ELSE 0 END) AS IVNPV,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN EV ELSE 0 END) AS VNPV,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN EV ELSE 0 END) AS VINPV,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN EV ELSE 0 END) AS VIINPV,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN EV ELSE 0 END) AS VIIINPV,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN EV ELSE 0 END) AS IXNPV,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN EV ELSE 0 END) AS XNPV,
       SUM(CASE WHEN Module_decision = 1 THEN EV ELSE 0 END) AS XINPV
FROM #REPORT_BASE
GROUP BY Principal_group, Region       
GO

--Data for part three of the report
CREATE TABLE #THREE
(Principal_group float, Region float,
 NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float,
 NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float,
 NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float,
 NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float, 
 NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float)
GO

INSERT INTO #THREE
SELECT Principal_group, Region,
       0,
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IA,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IIA,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IIIA,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IVA,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN 1 ELSE 0 END) AS VA,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN 1 ELSE 0 END) AS VIA,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN 1 ELSE 0 END) AS VIIA,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN 1 ELSE 0 END) AS VIIIA,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN 1 ELSE 0 END) AS IXA,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN 1 ELSE 0 END) AS XA,
       SUM(CASE WHEN Module_decision = 1 THEN 1 ELSE 0 END) AS XIA,
       0,       
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IAD,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IIAD,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IIIAD,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IVAD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN Principal ELSE 0 END) AS VAD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN Principal ELSE 0 END) AS VIAD,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN Principal ELSE 0 END) AS VIIAD,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN Principal ELSE 0 END) AS VIIIAD,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN Principal ELSE 0 END) AS IXAD,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN Principal ELSE 0 END) AS XAD,
       SUM(CASE WHEN Module_decision = 1 THEN Principal ELSE 0 END) AS XIAD,
       0, 
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN PB ELSE 0 END) AS IPB,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN PB ELSE 0 END) AS IIPB,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN PB ELSE 0 END) AS IIIPB,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN PB ELSE 0 END) AS IVPB,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN PB ELSE 0 END) AS VPB,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN PB ELSE 0 END) AS VIPB,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN PB ELSE 0 END) AS VIIPB,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN PB ELSE 0 END) AS VIIIPB,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN PB ELSE 0 END) AS IXPB,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN PB ELSE 0 END) AS XPB,
       SUM(CASE WHEN Module_decision = 1 THEN PB ELSE 0 END) AS XIPB,       
       0,
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IPBD,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IIPBD,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IIIPBD,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IVPBD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS VPBD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS VIPBD,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS VIIPBD,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS VIIIPBD,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS IXPBD,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS XPBD,
       SUM(CASE WHEN Module_decision = 1 THEN (PB * Principal) ELSE 0 END) AS XIPBD,       
       0, 
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 1 THEN EV ELSE 0 END) AS INPV,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 1 THEN EV ELSE 0 END) AS IINPV,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 1 THEN EV ELSE 0 END) AS IIINPV,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 1 THEN EV ELSE 0 END) AS IVNPV,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 1 THEN EV ELSE 0 END) AS VNPV,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 1 THEN EV ELSE 0 END) AS VINPV,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 1 THEN EV ELSE 0 END) AS VIINPV,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 1 THEN EV ELSE 0 END) AS VIIINPV,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 1 THEN EV ELSE 0 END) AS IXNPV,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 1 THEN EV ELSE 0 END) AS XNPV,
       SUM(CASE WHEN Module_decision = 1 THEN EV ELSE 0 END) AS XINPV
FROM #REPORT_BASE
WHERE Final_decision = 1
GROUP BY Principal_group, Region       
GO  

--Data for part four of the report
CREATE TABLE #FOUR
(Principal_group float, Region float,
 NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float,
 NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float,
 NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float,
 NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float, 
 NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float,
 NSAA float, IAA float, IIAA float, IIIAA float, IVAA float, VAA float, VIAA float, VIIAA float, VIIIAA float, IXAA float, XAA float, XIAA float,
 NSAAD float, IAAD float, IIAAD float, IIIAAD float, IVAAD float, VAAD float, VIAAD float, VIIAAD float, VIIIAAD float, IXAAD float, XAAD float, XIAAD float,
 NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XPB float, XIAPB float,
 NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float, 
 NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float)
GO

INSERT INTO #FOUR
SELECT Principal_group, Region,
       SUM(CASE WHEN EV IS 0 AND Module_decision = 3 THEN 1 ELSE 0 END) AS NSA,       
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 3 THEN 1 ELSE 0 END) AS IA,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 3 THEN 1 ELSE 0 END) AS IIA,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 3 THEN 1 ELSE 0 END) AS IIIA,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 3 THEN 1 ELSE 0 END) AS IVA,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 3 THEN 1 ELSE 0 END) AS VA,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 3 THEN 1 ELSE 0 END) AS VIA,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 3 THEN 1 ELSE 0 END) AS VIIA,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 3 THEN 1 ELSE 0 END) AS VIIIA,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 3 THEN 1 ELSE 0 END) AS IXA,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 3 THEN 1 ELSE 0 END) AS XA,
       SUM(CASE WHEN Module_decision = 3 THEN 1 ELSE 0 END) AS XIA,
       SUM(CASE WHEN EV IS 0 AND Module_decision = 3 THEN Principal ELSE 0 END) AS NSAD,              
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 3 THEN Principal ELSE 0 END) AS IAD,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 3 THEN Principal ELSE 0 END) AS IIAD,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 3 THEN Principal ELSE 0 END) AS IIIAD,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 3 THEN Principal ELSE 0 END) AS IVAD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 3 THEN Principal ELSE 0 END) AS VAD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 3 THEN Principal ELSE 0 END) AS VIAD,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 3 THEN Principal ELSE 0 END) AS VIIAD,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 3 THEN Principal ELSE 0 END) AS VIIIAD,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 3 THEN Principal ELSE 0 END) AS IXAD,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 3 THEN Principal ELSE 0 END) AS XAD,
       SUM(CASE WHEN Module_decision = 3 THEN Principal ELSE 0 END) AS XIAD,
       0, 
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 3 THEN PB ELSE 0 END) AS IPB,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 3 THEN PB ELSE 0 END) AS IIPB,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 3 THEN PB ELSE 0 END) AS IIIPB,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 3 THEN PB ELSE 0 END) AS IVPB,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 3 THEN PB ELSE 0 END) AS VPB,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 3 THEN PB ELSE 0 END) AS VIPB,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 3 THEN PB ELSE 0 END) AS VIIPB,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 3 THEN PB ELSE 0 END) AS VIIIPB,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 3 THEN PB ELSE 0 END) AS IXPB,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 3 THEN PB ELSE 0 END) AS XPB,
       SUM(CASE WHEN Module_decision = 3 THEN PB ELSE 0 END) AS XIPB,       
       0,
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS IPBD,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS IIPBD,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS IIIPBD,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS IVPBD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS VPBD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS VIPBD,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS VIIPBD,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS VIIIPBD,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS IXPBD,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS XPBD,
       SUM(CASE WHEN Module_decision = 3 THEN (PB * Principal) ELSE 0 END) AS XIPBD,       
       0, 
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 3 THEN EV ELSE 0 END) AS INPV,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 3 THEN EV ELSE 0 END) AS IINPV,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 3 THEN EV ELSE 0 END) AS IIINPV,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 3 THEN EV ELSE 0 END) AS IVNPV,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 3 THEN EV ELSE 0 END) AS VNPV,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 3 THEN EV ELSE 0 END) AS VINPV,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 3 THEN EV ELSE 0 END) AS VIINPV,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 3 THEN EV ELSE 0 END) AS VIIINPV,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 3 THEN EV ELSE 0 END) AS IXNPV,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 3 THEN EV ELSE 0 END) AS XNPV,
       SUM(CASE WHEN Module_decision = 3 THEN EV ELSE 0 END) AS XINPV,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
FROM #REPORT_BASE
WHERE Final_decision = 1
GROUP BY Principal_group, Region       
GO

UPDATE #FOUR
SET
NSAA = (A.NSA + NSA),
IAA = (A.IA + IA),
IIAA = (A.IIA + IIA),
IIIAA = (A.IIIA + IIIA),
IVAA = (A.IVA + IVA),
VAA = (A.VA + VA),
VIAA = (A.VIA + VIA),
VIIAA = (A.VIIA + VIIA),
VIIIAA = (A.VIIIA + VIIIA),
IXAA = (A.IXA + IXA),
XAA = (A.XA + XA),
XIAA = (A.XIA + XIA),         
NSAAD = (A.NSAD + NSAD),
IAAD = (A.IAD + IAD),
IIAAD = (A.IIAD + IIAD),
IIIAAD = (A.IIIAD + IIIAD),
IVAAD = (A.IVAD + IVAD),
VAAD = (A.VAD + VAD),
VIAAD = (A.VIAD + VIAD),
VIIAAD = (A.VIIAD + VIIAD),
VIIIAAD = (A.VIIIAD + VIIIAD),
IXAAD = (A.IXAD + IXAD),
XAAD = (A.XAD + XAD),
XIAAD = (A.XIAD + XIAD),
IAPB = (A.IPB + IPB),
IIAPB = (A.IIPB + IIPB),
IIIAPB = (A.IIIPB + IIIPB),
IVAPB = (A.IVPB + IVPB),
VAPB = (A.VPB + VPB),
VIAPB = (A.VIPB + VIPB),
VIIAPB = (A.VIIPB + VIIPB),
VIIIAPB = (VIIIPB + VIIIPB),
IXAPB = (A.IXPB + IXPB),
XAPB = (A.XPB + XPB),
XIAPB = (A.XIPB + XIPB),
IAPBD = (A.IPBD + IPBD),
IIAPBD = (A.IIPBD + IIPBD),
IIIAPBD = (A.IIIPBD + IIIPBD),
IVAPBD = (A.IVPBD + IVPBD),
VAPBD = (A.VPBD + VPBD),
VIAPBD = (A.VIPBD + VIPBD),
VIIAPBD = (A.VIIPBD + VIIPBD),
VIIIAPBD = (VIIIPBD + VIIIPBD),
IXAPBD = (A.IXPBD + IXPBD),
XAPBD = (A.XPBD + XPBD),
XIAPBD = (A.XIPBD + XIPBD),
NSANPV = (A.NSNPV + NSNPV),
IANPV = (A.INPV + INPV),
IIANPV = (A.IINPV + IINPV),
IIIANPV = (A.IIINPV + IIINPV),
IVANPV = (A.IVNPV + IVNPV),
VANPV = (A.VNPV + VNPV),
VIANPV = (A.VINPV + VINPV),
VIIANPV = (A.VIINPV + VIINPV),
VIIIANPV = (A.VIIINPV + VIIINPV),
IXANPV = (A.IXNPV + IXNPV),
XANPV = (A.XNPV + XNPV),
XIANPV = (A.XINPV + XINPV)
FROM #THREE AS A
WHERE A.Principal_group = #FOUR.Principal_group AND
      A.Region = #FOUR.Region
GO  

CREATE TABLE #FOUR_AR(Principal float, Region float,
                      NS float, I float, II float, III float, IV float, V  float, VI float, VII float, VIII float, IX float, X float, XI float,
                      NSD float, ID float, IID float, IIID float, IVD float, VD  float, VID float, VIID float, VIIID float, IXD float, XD float, XID float)
GO

INSERT INTO #FOUR_AR
SELECT Principal_group, Region,
       SUM(CASE WHEN EV IS 0 THEN 1 ELSE 0 END) AS NS,
       SUM(CASE WHEN PB <= 0.005 THEN 1 ELSE 0 END) AS I,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 THEN 1 ELSE 0 END) AS II,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 THEN 1 ELSE 0 END) AS III,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 THEN 1 ELSE 0 END) AS IV,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 THEN 1 ELSE 0 END) AS V,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 THEN 1 ELSE 0 END) AS VI,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 THEN 1 ELSE 0 END) AS VII,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 THEN 1 ELSE 0 END) AS VIII,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 THEN 1 ELSE 0 END) AS IX,
       SUM(CASE WHEN PB > 0.045 THEN 1 ELSE 0 END) AS X,
       COUNT(*) AS XI,
       SUM(CASE WHEN EV IS 0 THEN Principal ELSE 0 END) AS NSD,
       SUM(CASE WHEN PB <= 0.005 THEN Principal ELSE 0 END) AS ID,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 THEN Principal ELSE 0 END) AS IID,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 THEN Principal ELSE 0 END) AS IIID,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 THEN Principal ELSE 0 END) AS IVD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 THEN Principal ELSE 0 END) AS VD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 THEN Principal ELSE 0 END) AS VID,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 THEN Principal ELSE 0 END) AS VIID,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 THEN Principal ELSE 0 END) AS VIIID,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 THEN Principal ELSE 0 END) AS IXD,
       SUM(CASE WHEN PB > 0.045 THEN Principal ELSE 0 END) AS XD,
       SUM(Principal) AS XID
FROM #REPORT_BASE
WHERE Module_decison = 3
GROUP BY Principal_group, Region       
GO

--Data for part five of the report
CREATE TABLE #FIVE
(Principal_group float, Region float,
 NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float,
 NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float,
 NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float,
 NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float, 
 NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float,
 NSAA float, IAA float, IIAA float, IIIAA float, IVAA float, VAA float, VIAA float, VIIAA float, VIIIAA float, IXAA float, XAA float, XIAA float,
 NSAAD float, IAAD float, IIAAD float, IIIAAD float, IVAAD float, VAAD float, VIAAD float, VIIAAD float, VIIIAAD float, IXAAD float, XAAD float, XIAAD float,
 NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XPB float, XIAPB float,
 NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float, 
 NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float)
GO

INSERT INTO #FIVE
SELECT Principal_group, Region,
       SUM(CASE WHEN EV IS 0 AND Module_decision = 2 THEN 1 ELSE 0 END) AS NSA,       
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IA,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IIA,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IIIA,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IVA,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 2 THEN 1 ELSE 0 END) AS VA,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 2 THEN 1 ELSE 0 END) AS VIA,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 2 THEN 1 ELSE 0 END) AS VIIA,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 2 THEN 1 ELSE 0 END) AS VIIIA,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 2 THEN 1 ELSE 0 END) AS IXA,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 2 THEN 1 ELSE 0 END) AS XA,
       SUM(CASE WHEN Module_decision = 2 THEN 1 ELSE 0 END) AS XIA,
       SUM(CASE WHEN EV IS 0 AND Module_decision = 2 THEN Principal ELSE 0 END) AS NSAD,              
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IAD,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IIAD,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IIIAD,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IVAD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 2 THEN Principal ELSE 0 END) AS VAD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 2 THEN Principal ELSE 0 END) AS VIAD,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 2 THEN Principal ELSE 0 END) AS VIIAD,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 2 THEN Principal ELSE 0 END) AS VIIIAD,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 2 THEN Principal ELSE 0 END) AS IXAD,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 2 THEN Principal ELSE 0 END) AS XAD,
       SUM(CASE WHEN Module_decision = 2 THEN Principal ELSE 0 END) AS XIAD,
       0, 
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 2 THEN PB ELSE 0 END) AS IPB,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 2 THEN PB ELSE 0 END) AS IIPB,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 2 THEN PB ELSE 0 END) AS IIIPB,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 2 THEN PB ELSE 0 END) AS IVPB,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 2 THEN PB ELSE 0 END) AS VPB,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 2 THEN PB ELSE 0 END) AS VIPB,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 2 THEN PB ELSE 0 END) AS VIIPB,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 2 THEN PB ELSE 0 END) AS VIIIPB,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 2 THEN PB ELSE 0 END) AS IXPB,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 2 THEN PB ELSE 0 END) AS XPB,
       SUM(CASE WHEN Module_decision = 2 THEN PB ELSE 0 END) AS XIPB,       
       0,
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS IPBD,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS IIPBD,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS IIIPBD,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS IVPBD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS VPBD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS VIPBD,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS VIIPBD,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS VIIIPBD,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS IXPBD,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS XPBD,
       SUM(CASE WHEN Module_decision = 2 THEN (PB * Principal) ELSE 0 END) AS XIPBD,       
       0, 
       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 2 THEN EV ELSE 0 END) AS INPV,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 2 THEN EV ELSE 0 END) AS IINPV,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 2 THEN EV ELSE 0 END) AS IIINPV,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 2 THEN EV ELSE 0 END) AS IVNPV,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 2 THEN EV ELSE 0 END) AS VNPV,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 2 THEN EV ELSE 0 END) AS VINPV,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 2 THEN EV ELSE 0 END) AS VIINPV,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 2 THEN EV ELSE 0 END) AS VIIINPV,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 2 THEN EV ELSE 0 END) AS IXNPV,
       SUM(CASE WHEN PB > 0.045 AND Module_decision = 2 THEN EV ELSE 0 END) AS XNPV,
       SUM(CASE WHEN Module_decision = 2 THEN EV ELSE 0 END) AS XINPV,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
FROM #REPORT_BASE
WHERE Final_decision = 1
GROUP BY Principal_group, Region       
GO

UPDATE #FIVE
SET
NSAA = (A.NSAA + NSA),
IAA = (A.IAA + IA),
IIAA = (A.IIAA + IIA),
IIIAA = (A.IIIAA + IIIA),
IVAA = (A.IVAA + IVA),
VAA = (A.VAA + VA),
VIAA = (A.VIAA + VIA),
VIIAA = (A.VIIAA + VIIA),
VIIIAA = (A.VIIIAA + VIIIA),
IXAA = (A.IXAA + IXA),
XAA = (A.XAA + XA),
XIAA = (A.XIAA + XIA),         
NSAAD = (A.NSAAD + NSAD),
IAAD = (A.IAAD + IAD),
IIAAD = (A.IIAAD + IIAD),
IIIAAD = (A.IIIAAD + IIIAD),
IVAAD = (A.IVAAD + IVAD),
VAAD = (A.VAAD + VAD),
VIAAD = (A.VIAAD + VIAD),
VIIAAD = (A.VIIAAD + VIIAD),
VIIIAAD = (A.VIIIAAD + VIIIAD),
IXAAD = (A.IXAAD + IXAD),
XAAD = (A.XAAD + XAD),
XIAAD = (A.XIAAD + XIAD),
IAPB = (A.IAPB + IPB),
IIAPB = (A.IIAPB + IIPB),
IIIAPB = (A.IIIAPB + IIIPB),
IVAPB = (A.IVAPB + IVPB),
VAPB = (A.VAPB + VPB),
VIAPB = (A.VIAPB + VIPB),
VIIAPB = (A.VIIAPB + VIIPB),
VIIIAPB = (VIIIAPB + VIIIPB),
IXAPB = (A.IXAPB + IXPB),
XAPB = (A.XAPB + XPB),
XIAPB = (A.XIAPB + XIPB),
IAPBD = (A.IAPBD + IPBD),
IIAPBD = (A.IIAPBD + IIPBD),
IIIAPBD = (A.IIIAPBD + IIIPBD),
IVAPBD = (A.IVAPBD + IVPBD),
VAPBD = (A.VAPBD + VPBD),
VIAPBD = (A.VIAPBD + VIPBD),
VIIAPBD = (A.VIIAPBD + VIIPBD),
VIIIAPBD = (VIIIAPBD + VIIIPBD),
IXAPBD = (A.IXAPBD + IXPBD),
XAPBD = (A.XAPBD + XPBD),
XIAPBD = (A.XIAPBD + XIPBD),
NSANPV = (A.NSANPV + NSNPV),
IANPV = (A.IANPV + INPV),
IIANPV = (A.IIANPV + IINPV),
IIIANPV = (A.IIIANPV + IIINPV),
IVANPV = (A.IVANPV + IVNPV),
VANPV = (A.VANPV + VNPV),
VIANPV = (A.VIANPV + VINPV),
VIIANPV = (A.VIIANPV + VIINPV),
VIIIANPV = (A.VIIIANPV + VIIINPV),
IXANPV = (A.IXANPV + IXNPV),
XANPV = (A.XANPV + XNPV),
XIANPV = (A.XIANPV + XINPV)
FROM #FOUR AS A
WHERE A.Principal_group = #FIVE.Principal_group AND
      A.Region = #FOUR.Region
GO  

CREATE TABLE #FIVE_AR(Principal float, Region float,
                      NS float, I float, II float, III float, IV float, V  float, VI float, VII float, VIII float, IX float, X float, XI float,
                      NSD float, ID float, IID float, IIID float, IVD float, VD  float, VID float, VIID float, VIIID float, IXD float, XD float, XID float)
GO

INSERT INTO #FIVE_AR
SELECT Principal_group, Region,
       SUM(CASE WHEN EV IS 0 THEN 1 ELSE 0 END) AS NS,
       SUM(CASE WHEN PB <= 0.005 THEN 1 ELSE 0 END) AS I,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 THEN 1 ELSE 0 END) AS II,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 THEN 1 ELSE 0 END) AS III,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 THEN 1 ELSE 0 END) AS IV,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 THEN 1 ELSE 0 END) AS V,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 THEN 1 ELSE 0 END) AS VI,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 THEN 1 ELSE 0 END) AS VII,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 THEN 1 ELSE 0 END) AS VIII,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 THEN 1 ELSE 0 END) AS IX,
       SUM(CASE WHEN PB > 0.045 THEN 1 ELSE 0 END) AS X,
       COUNT(*) AS XI,
       SUM(CASE WHEN EV IS 0 THEN Principal ELSE 0 END) AS NSD,
       SUM(CASE WHEN PB <= 0.005 THEN Principal ELSE 0 END) AS ID,
       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 THEN Principal ELSE 0 END) AS IID,
       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 THEN Principal ELSE 0 END) AS IIID,
       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 THEN Principal ELSE 0 END) AS IVD,
       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 THEN Principal ELSE 0 END) AS VD,
       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 THEN Principal ELSE 0 END) AS VID,
       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 THEN Principal ELSE 0 END) AS VIID,
       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 THEN Principal ELSE 0 END) AS VIIID,
       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 THEN Principal ELSE 0 END) AS IXD,
       SUM(CASE WHEN PB > 0.045 THEN Principal ELSE 0 END) AS XD,
       SUM(Principal) AS XID
FROM #REPORT_BASE
WHERE Module_decison = 2
GROUP BY Principal_group, Region       
GO

--Generate report tables
--First part
CREATE TABLE #PART_ONE
(NS float, I float, II float, III float, IV float, V float, VI float, VII float, VIII float, IX float, X float, XI float,
 NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float)
GO

--Second part
CREATE TABLE #PART_TWO
(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float,
 NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float,
 NSMR float, IMR float, IIMR float, IIIMR float, IVMR float, VMR float, VIMR float, VIIMR float, VIIIMR float, IXMR float, XMR float, XIMR float,
 NSMRD float, IMRD float, IIMRD float, IIIMRD float, IVMRD float, VMRD float, VIMRD float, VIIMRD float, VIIIMRD float, IXMRD float, XMRD float, XIMRD float,
 NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float,
 NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float,
 NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float,
 NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float, 
 NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float)
GO

--Third part
CREATE TABLE #PART_THREE
(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float,
 NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float,
 NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float,
 NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float,
 NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float,
 NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float, 
 NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float)
GO

--Fourth part
CREATE TABLE #PART_FOUR
(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float,
 NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float,
 NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float,
 NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float,
 NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float,
 NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float, 
 NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float,
 NSAAR float, IAAR float, IIAAR float, IIIAAR float, IVAAR float, VAAR float, VIAAR float, VIIAAR float, VIIIAAR float, IXAAR float, XAAR float, XIAAR float,
 NSAARD float, IAARD float, IIAARD float, IIIAARD float, IVAARD float, VAARD float, VIAARD float, VIIAARD float, VIIIAARD float, IXAARD float, XAARD float, XIAARD float,
 NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XPB float, XIAPB float,
 NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float, 
 NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float)
GO

CREATE TABLE #AR(NS float, I float, II float, III float, IV float, V  float, VI float, VII float, VIII float, IX float, X float, XI float,
                 NSD float, ID float, IID float, IIID float, IVD float, VD  float, VID float, VIID float, VIIID float, IXD float, XD float, XID float)
GO

--Fifth part
CREATE TABLE #PART_FIVE
(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float,
 NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float,
 NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float,
 NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float,
 NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float,
 NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float, 
 NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float,
 NSAAR float, IAAR float, IIAAR float, IIIAAR float, IVAAR float, VAAR float, VIAAR float, VIIAAR float, VIIIAAR float, IXAAR float, XAAR float, XIAAR float,
 NSAARD float, IAARD float, IIAARD float, IIIAARD float, IVAARD float, VAARD float, VIAARD float, VIIAARD float, VIIIAARD float, IXAARD float, XAARD float, XIAARD float,
 NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XPB float, XIAPB float,
 NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float, 
 NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float)
GO

--Generate report type 1-15
DELETE FROM #PART_ONE
GO

DELETE FROM #PART_TWO
GO

DELETE FROM #PART_THREE
GO

DELETE FROM #PART_FOUR
GO

DELETE FROM #PART_FIVE
GO

INSERT INTO #PART_ONE
SELECT 
NS, I, II, III, IV, V, VI, VII, VIII, IX, X, XI,
NSD, ID, IID, IIID, IVD, VD, VID, VIID, VIIID, IXD, XD, XID
FROM #ONE
WHERE Principal_group = :loan AND
      Region = :region
GO

INSERT INTO #PART_TWO
SELECT
0, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA,
0, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD,
NSMR, IMR, IIMR, IIIMR, IVMR, VMR, VIMR, VIIMR, VIIIMR, IXMR, XMR, XIMR,
NSMRD, IMRD, IIMRD, IIIMRD, IVMRD, VMRD, VIMRD, VIIMRD, VIIIMRD, IXMRD, XMRD, XIMRD,
0,
IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, 
IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END) AS IPB, 
IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END) AS IIPB, 
IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIA END) AS IIIPB, 
IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END) AS IVPB, 
VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END) AS VPB, 
VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END) AS VIPB, 
VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END) AS VIIPB, 
VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END) AS VIIIPB, 
IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END) AS IXPB, 
XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END) AS XPB, 
XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END) AS XIPB,
NULL, 
IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END) AS IPBD, 
IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END) AS IIPBD, 
IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END) AS IIIPBD, 
IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END) AS IVPBD, 
VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END) AS VPBD, 
VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END) AS VIPBD, 
VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END) AS VIIPBD, 
VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END) AS VIIIPBD, 
IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END) AS IXPBD, 
XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END) AS XPBD, 
XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END) AS XIPBD, 
NULL, INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV 
FROM #TWO INNER JOIN #ONE AS A 
WHERE #TWO.Principal_group = A.Principal_group AND
      #TWO.Region = A.Region AND
      #TWO.Principal_group = :loan AND
      #TWO.Region = :region
GO  

INSERT INTO #PART_THREE
SELECT
0, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA,
0, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD,
0,
IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, 
IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END) AS IPB, 
IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END) AS IIPB, 
IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIA END) AS IIIPB, 
IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END) AS IVPB, 
VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END) AS VPB, 
VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END) AS VIPB, 
VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END) AS VIIPB, 
VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END) AS VIIIPB, 
IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END) AS IXPB, 
XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END) AS XPB, 
XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END) AS XIPB,
NULL, 
IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END) AS IPBD, 
IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END) AS IIPBD, 
IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END) AS IIIPBD, 
IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END) AS IVPBD, 
VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END) AS VPBD, 
VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END) AS VIPBD, 
VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END) AS VIIPBD, 
VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END) AS VIIIPBD, 
IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END) AS IXPBD, 
XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END) AS XPBD, 
XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END) AS XIPBD, 
NULL, INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV 
FROM #THREE INNER JOIN #ONE AS A 
WHERE #THREE.Principal_group = A.Principal_group AND
      #THREE.Region = A.Region AND
      #THREE.Principal_group = :loan AND
      #THREE.Region = :region
GO   

INSERT INTO #PART_FOUR
SELECT
NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA,
NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD,
NSA / (CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END) AS NSAR,
IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
NSAD / (CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END) AS NSARD,
IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, 
IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END) AS IPB, 
IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END) AS IIPB, 
IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIA END) AS IIIPB, 
IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END) AS IVPB, 
VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END) AS VPB, 
VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END) AS VIPB, 
VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END) AS VIIPB, 
VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END) AS VIIIPB, 
IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END) AS IXPB, 
XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END) AS XPB, 
XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END) AS XIPB,
NULL, 
IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END) AS IPBD, 
IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END) AS IIPBD, 
IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END) AS IIIPBD, 
IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END) AS IVPBD, 
VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END) AS VPBD, 
VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END) AS VIPBD, 
VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END) AS VIIPBD, 
VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END) AS VIIIPBD, 
IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END) AS IXPBD, 
XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END) AS XPBD, 
XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END) AS XIPBD, 
NULL, INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV,
NSAA, IAA, IIAA, IIIAA, IVAA, VAA, VIAA, VIIAA, VIIIAA, IXAA, XAA, XIAA,
NSAAD, IAAD, IIAAD, IIIAAD, IVAAD, VAAD, VIAAD, VIIAAD, VIIIAAD, IXAAD, XAAD, XIAAD,
NULL,
IAPB = IAPB /(CASE WHEN IAA = 0 THEN NULL ELSE IAA END),
IIAPB = IIAPB /(CASE WHEN IIAA = 0 THEN NULL ELSE IIAA END),
IIIAPB = IIIAPB /(CASE WHEN IIIAA = 0 THEN NULL ELSE IIIAA END),
IVAPB = IVAPB /(CASE WHEN IVAA = 0 THEN NULL ELSE IVAA END),
VAPB = VAPB /(CASE WHEN VAA = 0 THEN NULL ELSE VAA END),
VIAPB = VIAPB /(CASE WHEN VIAA = 0 THEN NULL ELSE VIAA END),
VIIAPB = VIIAPB /(CASE WHEN VIIAA = 0 THEN NULL ELSE VIIAA END),
VIIIAPB = VIIIAPB /(CASE WHEN VIIIAA = 0 THEN NULL ELSE VIIIAA END),
IXAPB = IXAPB /(CASE WHEN IXAA = 0 THEN NULL ELSE IXAA END),
XAPB = XAPB /(CASE WHEN XAA = 0 THEN NULL ELSE XAA END),
XIAPB = XIAPB /(CASE WHEN XIAA = 0 THEN NULL ELSE XIAA END),
NULL,
IAPBD = IAPBD /(CASE WHEN IAAD = 0 THEN NULL ELSE IAAD END),
IIAPBD = IIAPBD /(CASE WHEN IIAAD = 0 THEN NULL ELSE IIAAD END),
IIIAPBD = IIIAPBD /(CASE WHEN IIIAAD = 0 THEN NULL ELSE IIIAAD END),
IVAPBD = IVAPBD /(CASE WHEN IVAAD = 0 THEN NULL ELSE IVAAD END),
VAPBD = VAPBD /(CASE WHEN VAAD = 0 THEN NULL ELSE VAAD END),
VIAPBD = VIAPBD /(CASE WHEN VIAAD = 0 THEN NULL ELSE VIAAD END),
VIIAPBD = VIIAPBD /(CASE WHEN VIIAAD = 0 THEN NULL ELSE VIIAAD END),
VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAAD = 0 THEN NULL ELSE VIIIAAD END),
IXAPBD = IXAPBD /(CASE WHEN IXAAD = 0 THEN NULL ELSE IXAAD END),
XAPBD = XAPBD /(CASE WHEN XAAD = 0 THEN NULL ELSE XAAD END),
XIAPBD = XIAPBD /(CASE WHEN XIAAD = 0 THEN NULL ELSE XIAAD END),
NULL, IANPV, IIANPV, IIIANPV, IVANPV, VANPV, VIANPV, VIIANPV, VIIIANPV, IXANPV, XANPV, XIANPV
FROM #FOUR INNER JOIN #FOUR_AR AS A 
WHERE #FOUR.Principal_group = A.Principal_group AND
      #FOUR.Region = A.Region AND
      #FOUR.Principal_group = :loan AND
      #FOUR.Region = :region
GO

UPDATE #PART_FOUR
SET
NSAAR = NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END),
IAAR = IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END),
IIAAR = IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END),
IIIAAR = IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END),
IVAAR = IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END),
VAAR = VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END),
VIAAR = VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END),
VIIAAR = VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END),
VIIIAAR = VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END),
IXAAR = IXAAR /(CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END),
XAAR = XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END),
XIAAR = XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END),
NSAARD = NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END),
IAARD = IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END),
IIAARD = IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END),
IIIAARD = IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END),
IVAARD = IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END),
VAARD = VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END),
VIAARD = VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END),
VIIAARD = VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END),
VIIIAARD = VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END),
IXAARD = IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END),
XAARD = XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END),
XIAARD = XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END)
FROM #ONE AS A
WHERE #ONE.Principal_group = :loan AND
      #ONE.Region = :region
GO

INSERT INTO #PART_FIVE
SELECT
NSA, IA, IIA, IIIA, IVA, VA, VIA, VIIA, VIIIA, IXA, XA, XIA,
NSAD, IAD, IIAD, IIIAD, IVAD, VAD, VIAD, VIIAD, VIIIAD, IXAD, XAD, XIAD,
NSA / (CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END) AS NSAR,
IA / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
IIA / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
IIIA / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
IVA / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
VA / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
VIA / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
VIIA / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
VIIIA / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
IXA / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
XA / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
XIA / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
NSAD / (CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END) AS NSARD,
IAD / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
IIAD / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
IIIAD / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
IVAD / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
VAD / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
VIAD / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
VIIAD / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
VIIIAD / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
IXAD / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
XAD / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
XIAD / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, 
IPB / (CASE WHEN IA = 0 THEN NULL ELSE IA END) AS IPB, 
IIPB / (CASE WHEN IIA = 0 THEN NULL ELSE IIA END) AS IIPB, 
IIIPB / (CASE WHEN IIIA = 0 THEN NULL ELSE IIA END) AS IIIPB, 
IVPB / (CASE WHEN IVA = 0 THEN NULL ELSE IVA END) AS IVPB, 
VPB / (CASE WHEN VA = 0 THEN NULL ELSE VA END) AS VPB, 
VIPB / (CASE WHEN VIA = 0 THEN NULL ELSE VIA END) AS VIPB, 
VIIPB / (CASE WHEN VIIA = 0 THEN NULL ELSE VIIA END) AS VIIPB, 
VIIIPB / (CASE WHEN VIIIA = 0 THEN NULL ELSE VIIIA END) AS VIIIPB, 
IXPB / (CASE WHEN IXA = 0 THEN NULL ELSE IXA END) AS IXPB, 
XPB / (CASE WHEN XA = 0 THEN NULL ELSE XA END) AS XPB, 
XIPB / (CASE WHEN XIA = 0 THEN NULL ELSE XIA END) AS XIPB,
NULL, 
IPBD / (CASE WHEN IAD = 0 THEN NULL ELSE IAD END) AS IPBD, 
IIPBD / (CASE WHEN IIAD = 0 THEN NULL ELSE IIAD END) AS IIPBD, 
IIIPBD / (CASE WHEN IIIAD = 0 THEN NULL ELSE IIIAD END) AS IIIPBD, 
IVPBD / (CASE WHEN IVAD = 0 THEN NULL ELSE IVAD END) AS IVPBD, 
VPBD / (CASE WHEN VAD = 0 THEN NULL ELSE VAD END) AS VPBD, 
VIPBD / (CASE WHEN VIAD = 0 THEN NULL ELSE VIAD END) AS VIPBD, 
VIIPBD / (CASE WHEN VIIAD = 0 THEN NULL ELSE VIIAD END) AS VIIPBD, 
VIIIPBD / (CASE WHEN VIIIAD = 0 THEN NULL ELSE VIIIAD END) AS VIIIPBD, 
IXPBD / (CASE WHEN IXAD = 0 THEN NULL ELSE IXAD END) AS IXPBD, 
XPBD / (CASE WHEN XAD = 0 THEN NULL ELSE XAD END) AS XPBD, 
XIPBD / (CASE WHEN XIAD = 0 THEN NULL ELSE XIAD END) AS XIPBD, 
NULL, INPV, IINPV, IIINPV, IVNPV, VNPV, VINPV, VIINPV, VIIINPV, IXNPV, XNPV, XINPV,
NSAA, IAA, IIAA, IIIAA, IVAA, VAA, VIAA, VIIAA, VIIIAA, IXAA, XAA, XIAA,
NSAAD, IAAD, IIAAD, IIIAAD, IVAAD, VAAD, VIAAD, VIIAAD, VIIIAAD, IXAAD, XAAD, XIAAD,
NULL,
IAPB = IAPB /(CASE WHEN IAA = 0 THEN NULL ELSE IAA END),
IIAPB = IIAPB /(CASE WHEN IIAA = 0 THEN NULL ELSE IIAA END),
IIIAPB = IIIAPB /(CASE WHEN IIIAA = 0 THEN NULL ELSE IIIAA END),
IVAPB = IVAPB /(CASE WHEN IVAA = 0 THEN NULL ELSE IVAA END),
VAPB = VAPB /(CASE WHEN VAA = 0 THEN NULL ELSE VAA END),
VIAPB = VIAPB /(CASE WHEN VIAA = 0 THEN NULL ELSE VIAA END),
VIIAPB = VIIAPB /(CASE WHEN VIIAA = 0 THEN NULL ELSE VIIAA END),
VIIIAPB = VIIIAPB /(CASE WHEN VIIIAA = 0 THEN NULL ELSE VIIIAA END),
IXAPB = IXAPB /(CASE WHEN IXAA = 0 THEN NULL ELSE IXAA END),
XAPB = XAPB /(CASE WHEN XAA = 0 THEN NULL ELSE XAA END),
XIAPB = XIAPB /(CASE WHEN XIAA = 0 THEN NULL ELSE XIAA END),
NULL,
IAPBD = IAPBD /(CASE WHEN IAAD = 0 THEN NULL ELSE IAAD END),
IIAPBD = IIAPBD /(CASE WHEN IIAAD = 0 THEN NULL ELSE IIAAD END),
IIIAPBD = IIIAPBD /(CASE WHEN IIIAAD = 0 THEN NULL ELSE IIIAAD END),
IVAPBD = IVAPBD /(CASE WHEN IVAAD = 0 THEN NULL ELSE IVAAD END),
VAPBD = VAPBD /(CASE WHEN VAAD = 0 THEN NULL ELSE VAAD END),
VIAPBD = VIAPBD /(CASE WHEN VIAAD = 0 THEN NULL ELSE VIAAD END),
VIIAPBD = VIIAPBD /(CASE WHEN VIIAAD = 0 THEN NULL ELSE VIIAAD END),
VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAAD = 0 THEN NULL ELSE VIIIAAD END),
IXAPBD = IXAPBD /(CASE WHEN IXAAD = 0 THEN NULL ELSE IXAAD END),
XAPBD = XAPBD /(CASE WHEN XAAD = 0 THEN NULL ELSE XAAD END),
XIAPBD = XIAPBD /(CASE WHEN XIAAD = 0 THEN NULL ELSE XIAAD END),
NULL, IANPV, IIANPV, IIIANPV, IVANPV, VANPV, VIANPV, VIIANPV, VIIIANPV, IXANPV, XANPV, XIANPV
FROM #FIVE INNER JOIN #FIVE_AR AS A 
WHERE #FIVE.Principal_group = A.Principal_group AND
      #FIVE.Region = A.Region AND
      #FIVE.Principal_group = :loan AND
      #FIVE.Region = :region
GO

UPDATE #PART_FIVE
SET
NSAAR = NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END),
IAAR = IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END),
IIAAR = IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END),
IIIAAR = IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END),
IVAAR = IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END),
VAAR = VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END),
VIAAR = VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END),
VIIAAR = VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END),
VIIIAAR = VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END),
IXAAR = IXAAR /(CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END),
XAAR = XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END),
XIAAR = XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END),
NSAARD = NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END),
IAARD = IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END),
IIAARD = IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END),
IIIAARD = IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END),
IVAARD = IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END),
VAARD = VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END),
VIAARD = VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END),
VIIAARD = VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END),
VIIIAARD = VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END),
IXAARD = IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END),
XAARD = XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END),
XIAARD = XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END)
FROM #ONE AS A
WHERE #ONE.Principal_group = :loan AND
      #ONE.Region = :region
GO

--Generate report type 16
DELETE FROM #PART_ONE
GO

DELETE FROM #PART_TWO
GO

DELETE FROM #PART_THREE
GO

DELETE FROM #PART_FOUR
GO

DELETE FROM #AR
GO


DELETE FROM #PART_FIVE
GO


INSERT INTO #PART_ONE
SELECT 
SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI),
SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID)
FROM #ONE
GO

INSERT INTO #PART_TWO
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
SUM(NSMR), SUM(IMR), SUM(IIMR), SUM(IIIMR), SUM(IVMR), SUM(VMR), SUM(VIMR), SUM(VIIMR), SUM(VIIIMR), SUM(IXMR), SUM(XMR), SUM(XIMR),
SUM(NSMRD), SUM(IMRD), SUM(IIMRD), SUM(IIIMRD), SUM(IVMRD), SUM(VMRD), SUM(VIMRD), SUM(VIIMRD), SUM(VIIIMRD), SUM(IXMRD), SUM(XMRD), SUM(XIMRD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
FROM #TWO, #PART_ONE AS A
GO

UPDATE #PART_TWO
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

INSERT INTO #PART_THREE
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
FROM #THREE, #PART_ONE AS A
GO

UPDATE #PART_THREE
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

INSERT INTO #AR
SELECT 
SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI),
SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID)
FROM #FOUR_AR
GO

INSERT INTO #PART_FOUR
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
SUM(NSAA), SUM(IAA), SUM(IIAA), SUM(IIIAA), SUM(IVAA), SUM(VAA), SUM(VIAA), SUM(VIIAA), SUM(VIIIAA), SUM(IXAA), SUM(XAA), SUM(XIAA),
SUM(NSAAD), SUM(IAAD), SUM(IIAAD), SUM(IIIAAD), SUM(IVAAD), SUM(VAAD), SUM(VIAAD), SUM(VIIAAD), SUM(VIIIAAD), SUM(IXAAD), SUM(XAAD), SUM(XIAAD),
NULL, SUM(IAPB), SUM(IIAPB), SUM(IIIAPB), SUM(IVAPB), SUM(VAPB), SUM(VIAPB), SUM(VIIAPB), SUM(VIIIAPB), SUM(IXAPB), SUM(XPB), SUM(XIAPB),
NULL, SUM(IAPBD), SUM(IIAPBD), SUM(IIIAPBD), SUM(IVAPBD), SUM(VAPBD), SUM(VIAPBD), SUM(VIIAPBD), SUM(VIIIAPBD), SUM(IXAPBD), SUM(XAPBD), SUM(XIAPBD),
NULL, SUM(IANPV), SUM(IIANPV), SUM(IIIANPV), SUM(IVANPV), SUM(VANPV), SUM(VIANPV), SUM(VIIANPV), SUM(VIIIANPV), SUM(IXANPV), SUM(XANPV), SUM(XIANPV)
FROM #FOUR, #AR AS A
GO

UPDATE #PART_FOUR
SET
IAPB = IAPB /(CASE WHEN IAAR = 0 THEN NULL ELSE IAAR END),
IIAPB = IIAPB /(CASE WHEN IIAAR = 0 THEN NULL ELSE IIAAR END),
IIIAPB = IIIAPB /(CASE WHEN IIIAAR = 0 THEN NULL ELSE IIIAAR END),
IVAPB = IVAPB /(CASE WHEN IVAAR = 0 THEN NULL ELSE IVAAR END),
VAPB = VAPB /(CASE WHEN VAAR = 0 THEN NULL ELSE VAAR END),
VIAPB = VIAPB /(CASE WHEN VIAAR = 0 THEN NULL ELSE VIAAR END),
VIIAPB = VIIAPB /(CASE WHEN VIIAAR = 0 THEN NULL ELSE VIIAAR END),
VIIIAPB = VIIIAPB /(CASE WHEN VIIIAAR = 0 THEN NULL ELSE VIIIAAR END),
IXAPB = IXAPB /(CASE WHEN IXAAR = 0 THEN NULL ELSE IXAAR END),
XAPB = XAPB /(CASE WHEN XAAR = 0 THEN NULL ELSE XAAR END),
XIAPB = XIAPB /(CASE WHEN XIAAR = 0 THEN NULL ELSE XIAAR END),
IAPBD = IAPBD /(CASE WHEN IAARD = 0 THEN NULL ELSE IAARD END),
IIAPBD = IIAPBD /(CASE WHEN IIAARD = 0 THEN NULL ELSE IIAARD END),
IIIAPBD = IIIAPBD /(CASE WHEN IIIAARD = 0 THEN NULL ELSE IIIAARD END),
IVAPBD = IVAPBD /(CASE WHEN IVAARD = 0 THEN NULL ELSE IVAARD END),
VAPBD = VAPBD /(CASE WHEN VAARD = 0 THEN NULL ELSE VAARD END),
VIAPBD = VIAPBD /(CASE WHEN VIAARD = 0 THEN NULL ELSE VIAARD END),
VIIAPBD = VIIAPBD /(CASE WHEN VIIAARD = 0 THEN NULL ELSE VIIAARD END),
VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAARD = 0 THEN NULL ELSE VIIIAARD END),
IXAPBD = IXAPBD /(CASE WHEN IXAARD = 0 THEN NULL ELSE IXAARD END),
XAPBD = XAPBD /(CASE WHEN XAARD = 0 THEN NULL ELSE XAARD END),
XIAPBD = XIAPBD /(CASE WHEN XIAARD = 0 THEN NULL ELSE XIAARD END)
GO

UPDATE #PART_FOUR
SET
NSAAR = NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END),
IAAR = IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END),
IIAAR = IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END),
IIIAAR = IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END),
IVAAR = IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END),
VAAR = VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END),
VIAAR = VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END),
VIIAAR = VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END),
VIIIAAR = VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END),
IXAAR = IXAAR /(CASE WHEN A,IX = 0 THEN NULL ELSE A,IX END),
XAAR = XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END),
XIAAR = XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END),
NSAARD = NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END),
IAARD = IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END),
IIAARD = IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END),
IIIAARD = IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END),
IVAARD = IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END),
VAARD = VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END),
VIAARD = VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END),
VIIAARD = VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END),
VIIIAARD = VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END),
IXAARD = IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END),
XAARD = XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END),
XIAARD = XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END)
FROM #PART_ONE AS A
GO

UPDATE #PART_FOUR
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

INSERT INTO #AR
SELECT 
SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI),
SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID)
FROM #FIVE_AR
GO

INSERT INTO #PART_FIVE
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
SUM(NSAA), SUM(IAA), SUM(IIAA), SUM(IIIAA), SUM(IVAA), SUM(VAA), SUM(VIAA), SUM(VIIAA), SUM(VIIIAA), SUM(IXAA), SUM(XAA), SUM(XIAA),
SUM(NSAAD), SUM(IAAD), SUM(IIAAD), SUM(IIIAAD), SUM(IVAAD), SUM(VAAD), SUM(VIAAD), SUM(VIIAAD), SUM(VIIIAAD), SUM(IXAAD), SUM(XAAD), SUM(XIAAD),
NULL, SUM(IAPB), SUM(IIAPB), SUM(IIIAPB), SUM(IVAPB), SUM(VAPB), SUM(VIAPB), SUM(VIIAPB), SUM(VIIIAPB), SUM(IXAPB), SUM(XPB), SUM(XIAPB),
NULL, SUM(IAPBD), SUM(IIAPBD), SUM(IIIAPBD), SUM(IVAPBD), SUM(VAPBD), SUM(VIAPBD), SUM(VIIAPBD), SUM(VIIIAPBD), SUM(IXAPBD), SUM(XAPBD), SUM(XIAPBD),
NULL, SUM(IANPV), SUM(IIANPV), SUM(IIIANPV), SUM(IVANPV), SUM(VANPV), SUM(VIANPV), SUM(VIIANPV), SUM(VIIIANPV), SUM(IXANPV), SUM(XANPV), SUM(XIANPV)
FROM #FIVE, #AR AS A
GO

UPDATE #PART_FIVE
SET
IAPB = IAPB /(CASE WHEN IAAR = 0 THEN NULL ELSE IAAR END),
IIAPB = IIAPB /(CASE WHEN IIAAR = 0 THEN NULL ELSE IIAAR END),
IIIAPB = IIIAPB /(CASE WHEN IIIAAR = 0 THEN NULL ELSE IIIAAR END),
IVAPB = IVAPB /(CASE WHEN IVAAR = 0 THEN NULL ELSE IVAAR END),
VAPB = VAPB /(CASE WHEN VAAR = 0 THEN NULL ELSE VAAR END),
VIAPB = VIAPB /(CASE WHEN VIAAR = 0 THEN NULL ELSE VIAAR END),
VIIAPB = VIIAPB /(CASE WHEN VIIAAR = 0 THEN NULL ELSE VIIAAR END),
VIIIAPB = VIIIAPB /(CASE WHEN VIIIAAR = 0 THEN NULL ELSE VIIIAAR END),
IXAPB = IXAPB /(CASE WHEN IXAAR = 0 THEN NULL ELSE IXAAR END),
XAPB = XAPB /(CASE WHEN XAAR = 0 THEN NULL ELSE XAAR END),
XIAPB = XIAPB /(CASE WHEN XIAAR = 0 THEN NULL ELSE XIAAR END),
IAPBD = IAPBD /(CASE WHEN IAARD = 0 THEN NULL ELSE IAARD END),
IIAPBD = IIAPBD /(CASE WHEN IIAARD = 0 THEN NULL ELSE IIAARD END),
IIIAPBD = IIIAPBD /(CASE WHEN IIIAARD = 0 THEN NULL ELSE IIIAARD END),
IVAPBD = IVAPBD /(CASE WHEN IVAARD = 0 THEN NULL ELSE IVAARD END),
VAPBD = VAPBD /(CASE WHEN VAARD = 0 THEN NULL ELSE VAARD END),
VIAPBD = VIAPBD /(CASE WHEN VIAARD = 0 THEN NULL ELSE VIAARD END),
VIIAPBD = VIIAPBD /(CASE WHEN VIIAARD = 0 THEN NULL ELSE VIIAARD END),
VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAARD = 0 THEN NULL ELSE VIIIAARD END),
IXAPBD = IXAPBD /(CASE WHEN IXAARD = 0 THEN NULL ELSE IXAARD END),
XAPBD = XAPBD /(CASE WHEN XAARD = 0 THEN NULL ELSE XAARD END),
XIAPBD = XIAPBD /(CASE WHEN XIAARD = 0 THEN NULL ELSE XIAARD END)
GO

UPDATE #PART_FIVE
SET
NSAAR = NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END),
IAAR = IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END),
IIAAR = IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END),
IIIAAR = IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END),
IVAAR = IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END),
VAAR = VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END),
VIAAR = VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END),
VIIAAR = VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END),
VIIIAAR = VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END),
IXAAR = IXAAR /(CASE WHEN A,IX = 0 THEN NULL ELSE A,IX END),
XAAR = XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END),
XIAAR = XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END),
NSAARD = NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END),
IAARD = IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END),
IIAARD = IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END),
IIIAARD = IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END),
IVAARD = IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END),
VAARD = VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END),
VIAARD = VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END),
VIIAARD = VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END),
VIIIAARD = VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END),
IXAARD = IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END),
XAARD = XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END),
XIAARD = XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END)
FROM #PART_ONE AS A
GO

UPDATE #PART_FIVE
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

--Generate report type 17-21
DELETE FROM #PART_ONE
GO

DELETE FROM #PART_TWO
GO

DELETE FROM #PART_THREE
GO

DELETE FROM #PART_FOUR
GO

DELETE FROM #AR
GO


DELETE FROM #PART_FIVE
GO


INSERT INTO #PART_ONE
SELECT 
SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI),
SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID)
FROM #ONE
WHERE Region = :region
GO

INSERT INTO #PART_TWO
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
SUM(NSMR), SUM(IMR), SUM(IIMR), SUM(IIIMR), SUM(IVMR), SUM(VMR), SUM(VIMR), SUM(VIIMR), SUM(VIIIMR), SUM(IXMR), SUM(XMR), SUM(XIMR),
SUM(NSMRD), SUM(IMRD), SUM(IIMRD), SUM(IIIMRD), SUM(IVMRD), SUM(VMRD), SUM(VIMRD), SUM(VIIMRD), SUM(VIIIMRD), SUM(IXMRD), SUM(XMRD), SUM(XIMRD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
FROM #TWO, #PART_ONE AS A
WHERE #TWO.Region = :region
GO

UPDATE #PART_TWO
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

INSERT INTO #PART_THREE
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
FROM #THREE, #PART_ONE AS A
WHERE #THREE.Region = :region
GO

UPDATE #PART_THREE
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

INSERT INTO #AR
SELECT 
SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI),
SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID)
FROM #FOUR_AR
WHERE Region = :region
GO

INSERT INTO #PART_FOUR
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
SUM(NSAA), SUM(IAA), SUM(IIAA), SUM(IIIAA), SUM(IVAA), SUM(VAA), SUM(VIAA), SUM(VIIAA), SUM(VIIIAA), SUM(IXAA), SUM(XAA), SUM(XIAA),
SUM(NSAAD), SUM(IAAD), SUM(IIAAD), SUM(IIIAAD), SUM(IVAAD), SUM(VAAD), SUM(VIAAD), SUM(VIIAAD), SUM(VIIIAAD), SUM(IXAAD), SUM(XAAD), SUM(XIAAD),
NULL, SUM(IAPB), SUM(IIAPB), SUM(IIIAPB), SUM(IVAPB), SUM(VAPB), SUM(VIAPB), SUM(VIIAPB), SUM(VIIIAPB), SUM(IXAPB), SUM(XPB), SUM(XIAPB),
NULL, SUM(IAPBD), SUM(IIAPBD), SUM(IIIAPBD), SUM(IVAPBD), SUM(VAPBD), SUM(VIAPBD), SUM(VIIAPBD), SUM(VIIIAPBD), SUM(IXAPBD), SUM(XAPBD), SUM(XIAPBD),
NULL, SUM(IANPV), SUM(IIANPV), SUM(IIIANPV), SUM(IVANPV), SUM(VANPV), SUM(VIANPV), SUM(VIIANPV), SUM(VIIIANPV), SUM(IXANPV), SUM(XANPV), SUM(XIANPV)
FROM #FOUR, #AR AS A
WHERE #FOUR.Region = :region
GO

UPDATE #PART_FOUR
SET
IAPB = IAPB /(CASE WHEN IAAR = 0 THEN NULL ELSE IAAR END),
IIAPB = IIAPB /(CASE WHEN IIAAR = 0 THEN NULL ELSE IIAAR END),
IIIAPB = IIIAPB /(CASE WHEN IIIAAR = 0 THEN NULL ELSE IIIAAR END),
IVAPB = IVAPB /(CASE WHEN IVAAR = 0 THEN NULL ELSE IVAAR END),
VAPB = VAPB /(CASE WHEN VAAR = 0 THEN NULL ELSE VAAR END),
VIAPB = VIAPB /(CASE WHEN VIAAR = 0 THEN NULL ELSE VIAAR END),
VIIAPB = VIIAPB /(CASE WHEN VIIAAR = 0 THEN NULL ELSE VIIAAR END),
VIIIAPB = VIIIAPB /(CASE WHEN VIIIAAR = 0 THEN NULL ELSE VIIIAAR END),
IXAPB = IXAPB /(CASE WHEN IXAAR = 0 THEN NULL ELSE IXAAR END),
XAPB = XAPB /(CASE WHEN XAAR = 0 THEN NULL ELSE XAAR END),
XIAPB = XIAPB /(CASE WHEN XIAAR = 0 THEN NULL ELSE XIAAR END),
IAPBD = IAPBD /(CASE WHEN IAARD = 0 THEN NULL ELSE IAARD END),
IIAPBD = IIAPBD /(CASE WHEN IIAARD = 0 THEN NULL ELSE IIAARD END),
IIIAPBD = IIIAPBD /(CASE WHEN IIIAARD = 0 THEN NULL ELSE IIIAARD END),
IVAPBD = IVAPBD /(CASE WHEN IVAARD = 0 THEN NULL ELSE IVAARD END),
VAPBD = VAPBD /(CASE WHEN VAARD = 0 THEN NULL ELSE VAARD END),
VIAPBD = VIAPBD /(CASE WHEN VIAARD = 0 THEN NULL ELSE VIAARD END),
VIIAPBD = VIIAPBD /(CASE WHEN VIIAARD = 0 THEN NULL ELSE VIIAARD END),
VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAARD = 0 THEN NULL ELSE VIIIAARD END),
IXAPBD = IXAPBD /(CASE WHEN IXAARD = 0 THEN NULL ELSE IXAARD END),
XAPBD = XAPBD /(CASE WHEN XAARD = 0 THEN NULL ELSE XAARD END),
XIAPBD = XIAPBD /(CASE WHEN XIAARD = 0 THEN NULL ELSE XIAARD END)
GO

UPDATE #PART_FOUR
SET
NSAAR = NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END),
IAAR = IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END),
IIAAR = IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END),
IIIAAR = IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END),
IVAAR = IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END),
VAAR = VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END),
VIAAR = VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END),
VIIAAR = VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END),
VIIIAAR = VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END),
IXAAR = IXAAR /(CASE WHEN A,IX = 0 THEN NULL ELSE A,IX END),
XAAR = XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END),
XIAAR = XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END),
NSAARD = NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END),
IAARD = IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END),
IIAARD = IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END),
IIIAARD = IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END),
IVAARD = IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END),
VAARD = VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END),
VIAARD = VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END),
VIIAARD = VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END),
VIIIAARD = VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END),
IXAARD = IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END),
XAARD = XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END),
XIAARD = XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END)
FROM #PART_ONE AS A
GO

UPDATE #PART_FOUR
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

DELETE FROM #AR
GO

INSERT INTO #AR
SELECT 
SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI),
SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID)
FROM #FIVE_AR
WHERE Region = :region
GO

INSERT INTO #PART_FIVE
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
SUM(NSAA), SUM(IAA), SUM(IIAA), SUM(IIIAA), SUM(IVAA), SUM(VAA), SUM(VIAA), SUM(VIIAA), SUM(VIIIAA), SUM(IXAA), SUM(XAA), SUM(XIAA),
SUM(NSAAD), SUM(IAAD), SUM(IIAAD), SUM(IIIAAD), SUM(IVAAD), SUM(VAAD), SUM(VIAAD), SUM(VIIAAD), SUM(VIIIAAD), SUM(IXAAD), SUM(XAAD), SUM(XIAAD),
NULL, SUM(IAPB), SUM(IIAPB), SUM(IIIAPB), SUM(IVAPB), SUM(VAPB), SUM(VIAPB), SUM(VIIAPB), SUM(VIIIAPB), SUM(IXAPB), SUM(XPB), SUM(XIAPB),
NULL, SUM(IAPBD), SUM(IIAPBD), SUM(IIIAPBD), SUM(IVAPBD), SUM(VAPBD), SUM(VIAPBD), SUM(VIIAPBD), SUM(VIIIAPBD), SUM(IXAPBD), SUM(XAPBD), SUM(XIAPBD),
NULL, SUM(IANPV), SUM(IIANPV), SUM(IIIANPV), SUM(IVANPV), SUM(VANPV), SUM(VIANPV), SUM(VIIANPV), SUM(VIIIANPV), SUM(IXANPV), SUM(XANPV), SUM(XIANPV)
FROM #FIVE, #AR AS A
WHERE #FIVE.Region = :region
GO

UPDATE #PART_FIVE
SET
IAPB = IAPB /(CASE WHEN IAAR = 0 THEN NULL ELSE IAAR END),
IIAPB = IIAPB /(CASE WHEN IIAAR = 0 THEN NULL ELSE IIAAR END),
IIIAPB = IIIAPB /(CASE WHEN IIIAAR = 0 THEN NULL ELSE IIIAAR END),
IVAPB = IVAPB /(CASE WHEN IVAAR = 0 THEN NULL ELSE IVAAR END),
VAPB = VAPB /(CASE WHEN VAAR = 0 THEN NULL ELSE VAAR END),
VIAPB = VIAPB /(CASE WHEN VIAAR = 0 THEN NULL ELSE VIAAR END),
VIIAPB = VIIAPB /(CASE WHEN VIIAAR = 0 THEN NULL ELSE VIIAAR END),
VIIIAPB = VIIIAPB /(CASE WHEN VIIIAAR = 0 THEN NULL ELSE VIIIAAR END),
IXAPB = IXAPB /(CASE WHEN IXAAR = 0 THEN NULL ELSE IXAAR END),
XAPB = XAPB /(CASE WHEN XAAR = 0 THEN NULL ELSE XAAR END),
XIAPB = XIAPB /(CASE WHEN XIAAR = 0 THEN NULL ELSE XIAAR END),
IAPBD = IAPBD /(CASE WHEN IAARD = 0 THEN NULL ELSE IAARD END),
IIAPBD = IIAPBD /(CASE WHEN IIAARD = 0 THEN NULL ELSE IIAARD END),
IIIAPBD = IIIAPBD /(CASE WHEN IIIAARD = 0 THEN NULL ELSE IIIAARD END),
IVAPBD = IVAPBD /(CASE WHEN IVAARD = 0 THEN NULL ELSE IVAARD END),
VAPBD = VAPBD /(CASE WHEN VAARD = 0 THEN NULL ELSE VAARD END),
VIAPBD = VIAPBD /(CASE WHEN VIAARD = 0 THEN NULL ELSE VIAARD END),
VIIAPBD = VIIAPBD /(CASE WHEN VIIAARD = 0 THEN NULL ELSE VIIAARD END),
VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAARD = 0 THEN NULL ELSE VIIIAARD END),
IXAPBD = IXAPBD /(CASE WHEN IXAARD = 0 THEN NULL ELSE IXAARD END),
XAPBD = XAPBD /(CASE WHEN XAARD = 0 THEN NULL ELSE XAARD END),
XIAPBD = XIAPBD /(CASE WHEN XIAARD = 0 THEN NULL ELSE XIAARD END)
GO

UPDATE #PART_FIVE
SET
NSAAR = NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END),
IAAR = IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END),
IIAAR = IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END),
IIIAAR = IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END),
IVAAR = IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END),
VAAR = VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END),
VIAAR = VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END),
VIIAAR = VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END),
VIIIAAR = VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END),
IXAAR = IXAAR /(CASE WHEN A,IX = 0 THEN NULL ELSE A,IX END),
XAAR = XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END),
XIAAR = XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END),
NSAARD = NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END),
IAARD = IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END),
IIAARD = IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END),
IIIAARD = IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END),
IVAARD = IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END),
VAARD = VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END),
VIAARD = VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END),
VIIAARD = VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END),
VIIIAARD = VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END),
IXAARD = IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END),
XAARD = XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END),
XIAARD = XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END)
FROM #PART_ONE AS A
GO

UPDATE #PART_FIVE
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

--Generate report type 22-24
DELETE FROM #PART_ONE
GO

DELETE FROM #PART_TWO
GO

DELETE FROM #PART_THREE
GO

DELETE FROM #PART_FOUR
GO

DELETE FROM #AR
GO


DELETE FROM #PART_FIVE
GO


INSERT INTO #PART_ONE
SELECT 
SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI),
SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID)
FROM #ONE
WHERE Principal_group = :loan
GO

INSERT INTO #PART_TWO
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
SUM(NSMR), SUM(IMR), SUM(IIMR), SUM(IIIMR), SUM(IVMR), SUM(VMR), SUM(VIMR), SUM(VIIMR), SUM(VIIIMR), SUM(IXMR), SUM(XMR), SUM(XIMR),
SUM(NSMRD), SUM(IMRD), SUM(IIMRD), SUM(IIIMRD), SUM(IVMRD), SUM(VMRD), SUM(VIMRD), SUM(VIIMRD), SUM(VIIIMRD), SUM(IXMRD), SUM(XMRD), SUM(XIMRD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
FROM #TWO, #PART_ONE AS A
WHERE #TWO.Principal_group = :loan
GO

UPDATE #PART_TWO
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

INSERT INTO #PART_THREE
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
FROM #THREE, #PART_ONE AS A
WHERE #THREE.Principal_group = :loan
GO

UPDATE #PART_THREE
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

INSERT INTO #AR
SELECT 
SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI),
SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID)
FROM #FOUR_AR
WHERE Principal_group = :loan
GO

INSERT INTO #PART_FOUR
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
SUM(NSAA), SUM(IAA), SUM(IIAA), SUM(IIIAA), SUM(IVAA), SUM(VAA), SUM(VIAA), SUM(VIIAA), SUM(VIIIAA), SUM(IXAA), SUM(XAA), SUM(XIAA),
SUM(NSAAD), SUM(IAAD), SUM(IIAAD), SUM(IIIAAD), SUM(IVAAD), SUM(VAAD), SUM(VIAAD), SUM(VIIAAD), SUM(VIIIAAD), SUM(IXAAD), SUM(XAAD), SUM(XIAAD),
NULL, SUM(IAPB), SUM(IIAPB), SUM(IIIAPB), SUM(IVAPB), SUM(VAPB), SUM(VIAPB), SUM(VIIAPB), SUM(VIIIAPB), SUM(IXAPB), SUM(XPB), SUM(XIAPB),
NULL, SUM(IAPBD), SUM(IIAPBD), SUM(IIIAPBD), SUM(IVAPBD), SUM(VAPBD), SUM(VIAPBD), SUM(VIIAPBD), SUM(VIIIAPBD), SUM(IXAPBD), SUM(XAPBD), SUM(XIAPBD),
NULL, SUM(IANPV), SUM(IIANPV), SUM(IIIANPV), SUM(IVANPV), SUM(VANPV), SUM(VIANPV), SUM(VIIANPV), SUM(VIIIANPV), SUM(IXANPV), SUM(XANPV), SUM(XIANPV)
FROM #FOUR, #AR AS A
WHERE #FOUR.Principal_group = :loan
GO

UPDATE #PART_FOUR
SET
IAPB = IAPB /(CASE WHEN IAAR = 0 THEN NULL ELSE IAAR END),
IIAPB = IIAPB /(CASE WHEN IIAAR = 0 THEN NULL ELSE IIAAR END),
IIIAPB = IIIAPB /(CASE WHEN IIIAAR = 0 THEN NULL ELSE IIIAAR END),
IVAPB = IVAPB /(CASE WHEN IVAAR = 0 THEN NULL ELSE IVAAR END),
VAPB = VAPB /(CASE WHEN VAAR = 0 THEN NULL ELSE VAAR END),
VIAPB = VIAPB /(CASE WHEN VIAAR = 0 THEN NULL ELSE VIAAR END),
VIIAPB = VIIAPB /(CASE WHEN VIIAAR = 0 THEN NULL ELSE VIIAAR END),
VIIIAPB = VIIIAPB /(CASE WHEN VIIIAAR = 0 THEN NULL ELSE VIIIAAR END),
IXAPB = IXAPB /(CASE WHEN IXAAR = 0 THEN NULL ELSE IXAAR END),
XAPB = XAPB /(CASE WHEN XAAR = 0 THEN NULL ELSE XAAR END),
XIAPB = XIAPB /(CASE WHEN XIAAR = 0 THEN NULL ELSE XIAAR END),
IAPBD = IAPBD /(CASE WHEN IAARD = 0 THEN NULL ELSE IAARD END),
IIAPBD = IIAPBD /(CASE WHEN IIAARD = 0 THEN NULL ELSE IIAARD END),
IIIAPBD = IIIAPBD /(CASE WHEN IIIAARD = 0 THEN NULL ELSE IIIAARD END),
IVAPBD = IVAPBD /(CASE WHEN IVAARD = 0 THEN NULL ELSE IVAARD END),
VAPBD = VAPBD /(CASE WHEN VAARD = 0 THEN NULL ELSE VAARD END),
VIAPBD = VIAPBD /(CASE WHEN VIAARD = 0 THEN NULL ELSE VIAARD END),
VIIAPBD = VIIAPBD /(CASE WHEN VIIAARD = 0 THEN NULL ELSE VIIAARD END),
VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAARD = 0 THEN NULL ELSE VIIIAARD END),
IXAPBD = IXAPBD /(CASE WHEN IXAARD = 0 THEN NULL ELSE IXAARD END),
XAPBD = XAPBD /(CASE WHEN XAARD = 0 THEN NULL ELSE XAARD END),
XIAPBD = XIAPBD /(CASE WHEN XIAARD = 0 THEN NULL ELSE XIAARD END)
GO

UPDATE #PART_FOUR
SET
NSAAR = NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END),
IAAR = IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END),
IIAAR = IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END),
IIIAAR = IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END),
IVAAR = IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END),
VAAR = VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END),
VIAAR = VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END),
VIIAAR = VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END),
VIIIAAR = VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END),
IXAAR = IXAAR /(CASE WHEN A,IX = 0 THEN NULL ELSE A,IX END),
XAAR = XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END),
XIAAR = XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END),
NSAARD = NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END),
IAARD = IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END),
IIAARD = IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END),
IIIAARD = IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END),
IVAARD = IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END),
VAARD = VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END),
VIAARD = VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END),
VIIAARD = VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END),
VIIIAARD = VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END),
IXAARD = IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END),
XAARD = XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END),
XIAARD = XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END)
FROM #PART_ONE AS A
GO

UPDATE #PART_FOUR
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO

DELETE FROM #AR
GO

INSERT INTO #AR
SELECT 
SUM(NS), SUM(I), SUM(II), SUM(III), SUM(IV), SUM(V), SUM(VI), SUM(VII), SUM(VIII), SUM(IX), SUM(X), SUM(XI),
SUM(NSD), SUM(ID), SUM(IID), SUM(IIID), SUM(IVD), SUM(VD), SUM(VID), SUM(VIID), SUM(VIIID), SUM(IXD), SUM(XD), SUM(XID)
FROM #FIVE_AR
WHERE Principal_group = :loan
GO

INSERT INTO #PART_FIVE
SELECT
0, SUM(IA), SUM(IIA), SUM(IIIA), SUM(IVA), SUM(VA), SUM(VIA), SUM(VIIA), SUM(VIIIA), SUM(IXA), SUM(XA), SUM(XIA),
0, SUM(IAD), SUM(IIAD), SUM(IIIAD), SUM(IVAD), SUM(VAD), SUM(VIAD), SUM(VIIAD), SUM(VIIIAD), SUM(IXAD), SUM(XAD), SUM(XIAD),
0,
SUM(IA) / (CASE WHEN A.I = 0 THEN NULL ELSE A.I END) AS IAR,
SUM(IIA) / (CASE WHEN A.II = 0 THEN NULL ELSE A.II END) AS IIAR,
SUM(IIIA) / (CASE WHEN A.III = 0 THEN NULL ELSE A.III END) AS IIIAR,
SUM(IVA) / (CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END) AS IVAR,
SUM(VA) / (CASE WHEN A.V = 0 THEN NULL ELSE A.V END) AS VAR,
SUM(VIA) / (CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END) AS VIAR,
SUM(VIIA) / (CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END) AS VIIAR,
SUM(VIIIA) / (CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END) AS VIIIAR,
SUM(IXA) / (CASE WHEN A.IX = 0 THEN NULL ELSE A.IX END) AS IXAR,
SUM(XA) / (CASE WHEN A.X = 0 THEN NULL ELSE A.X END) AS XAR,
SUM(XIA) / (CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END) AS XIAR,
0,
SUM(IAD) / (CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END) AS IARD,
SUM(IIAD) / (CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END) AS IIARD,
SUM(IIIAD) / (CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END) AS IIIARD,
SUM(IVAD) / (CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END) AS IVARD,
SUM(VAD) / (CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END) AS VARD,
SUM(VIAD) / (CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END) AS VIARD,
SUM(VIIAD) / (CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END) AS VIIARD,
SUM(VIIIAD) / (CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END) AS VIIIARD,
SUM(IXAD) / (CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END) AS IXARD,
SUM(XAD) / (CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END) AS XARD,
SUM(XIAD) / (CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END) AS XIARD,
NULL, SUM(IPB), SUM(IIPB), SUM(IIIPB), SUM(IVPB), SUM(VPB), SUM(VIPB), SUM(VIIPB), SUM(VIIIPB), SUM(IXPB), SUM(XPB), SUM(XIPB),
NULL, SUM(IPBD), SUM(IIPBD), SUM(IIIPBD), SUM(IVPBD), SUM(VPBD), SUM(VIPBD), SUM(VIIPBD), SUM(VIIIPBD), SUM(IXPBD), SUM(XPBD), SUM(XIPBD), 
NULL, SUM(INPV), SUM(IINPV), SUM(IIINPV), SUM(IVNPV), SUM(VNPV), SUM(VINPV), SUM(VIINPV), SUM(VIIINPV), SUM(IXNPV), SUM(XNPV), SUM(XINPV) 
SUM(NSAA), SUM(IAA), SUM(IIAA), SUM(IIIAA), SUM(IVAA), SUM(VAA), SUM(VIAA), SUM(VIIAA), SUM(VIIIAA), SUM(IXAA), SUM(XAA), SUM(XIAA),
SUM(NSAAD), SUM(IAAD), SUM(IIAAD), SUM(IIIAAD), SUM(IVAAD), SUM(VAAD), SUM(VIAAD), SUM(VIIAAD), SUM(VIIIAAD), SUM(IXAAD), SUM(XAAD), SUM(XIAAD),
NULL, SUM(IAPB), SUM(IIAPB), SUM(IIIAPB), SUM(IVAPB), SUM(VAPB), SUM(VIAPB), SUM(VIIAPB), SUM(VIIIAPB), SUM(IXAPB), SUM(XPB), SUM(XIAPB),
NULL, SUM(IAPBD), SUM(IIAPBD), SUM(IIIAPBD), SUM(IVAPBD), SUM(VAPBD), SUM(VIAPBD), SUM(VIIAPBD), SUM(VIIIAPBD), SUM(IXAPBD), SUM(XAPBD), SUM(XIAPBD),
NULL, SUM(IANPV), SUM(IIANPV), SUM(IIIANPV), SUM(IVANPV), SUM(VANPV), SUM(VIANPV), SUM(VIIANPV), SUM(VIIIANPV), SUM(IXANPV), SUM(XANPV), SUM(XIANPV)
FROM #FIVE, #AR AS A
WHERE #FIVE.Principal_group = :loan
GO

UPDATE #PART_FIVE
SET
IAPB = IAPB /(CASE WHEN IAAR = 0 THEN NULL ELSE IAAR END),
IIAPB = IIAPB /(CASE WHEN IIAAR = 0 THEN NULL ELSE IIAAR END),
IIIAPB = IIIAPB /(CASE WHEN IIIAAR = 0 THEN NULL ELSE IIIAAR END),
IVAPB = IVAPB /(CASE WHEN IVAAR = 0 THEN NULL ELSE IVAAR END),
VAPB = VAPB /(CASE WHEN VAAR = 0 THEN NULL ELSE VAAR END),
VIAPB = VIAPB /(CASE WHEN VIAAR = 0 THEN NULL ELSE VIAAR END),
VIIAPB = VIIAPB /(CASE WHEN VIIAAR = 0 THEN NULL ELSE VIIAAR END),
VIIIAPB = VIIIAPB /(CASE WHEN VIIIAAR = 0 THEN NULL ELSE VIIIAAR END),
IXAPB = IXAPB /(CASE WHEN IXAAR = 0 THEN NULL ELSE IXAAR END),
XAPB = XAPB /(CASE WHEN XAAR = 0 THEN NULL ELSE XAAR END),
XIAPB = XIAPB /(CASE WHEN XIAAR = 0 THEN NULL ELSE XIAAR END),
IAPBD = IAPBD /(CASE WHEN IAARD = 0 THEN NULL ELSE IAARD END),
IIAPBD = IIAPBD /(CASE WHEN IIAARD = 0 THEN NULL ELSE IIAARD END),
IIIAPBD = IIIAPBD /(CASE WHEN IIIAARD = 0 THEN NULL ELSE IIIAARD END),
IVAPBD = IVAPBD /(CASE WHEN IVAARD = 0 THEN NULL ELSE IVAARD END),
VAPBD = VAPBD /(CASE WHEN VAARD = 0 THEN NULL ELSE VAARD END),
VIAPBD = VIAPBD /(CASE WHEN VIAARD = 0 THEN NULL ELSE VIAARD END),
VIIAPBD = VIIAPBD /(CASE WHEN VIIAARD = 0 THEN NULL ELSE VIIAARD END),
VIIIAPBD = VIIIAPBD /(CASE WHEN VIIIAARD = 0 THEN NULL ELSE VIIIAARD END),
IXAPBD = IXAPBD /(CASE WHEN IXAARD = 0 THEN NULL ELSE IXAARD END),
XAPBD = XAPBD /(CASE WHEN XAARD = 0 THEN NULL ELSE XAARD END),
XIAPBD = XIAPBD /(CASE WHEN XIAARD = 0 THEN NULL ELSE XIAARD END)
GO

UPDATE #PART_FIVE
SET
NSAAR = NSAAR /(CASE WHEN A.NS = 0 THEN NULL ELSE A.NS END),
IAAR = IAAR /(CASE WHEN A.I = 0 THEN NULL ELSE A.I END),
IIAAR = IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A.II END),
IIIAAR = IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III END),
IVAAR = IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END),
VAAR = VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END),
VIAAR = VIAAR /(CASE WHEN A.VI = 0 THEN NULL ELSE A.VI END),
VIIAAR = VIIAAR /(CASE WHEN A.VII = 0 THEN NULL ELSE A.VII END),
VIIIAAR = VIIIAAR /(CASE WHEN A.VIII = 0 THEN NULL ELSE A.VIII END),
IXAAR = IXAAR /(CASE WHEN A,IX = 0 THEN NULL ELSE A,IX END),
XAAR = XAAR /(CASE WHEN A.X = 0 THEN NULL ELSE A.X END),
XIAAR = XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.XI END),
NSAARD = NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END),
IAARD = IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END),
IIAARD = IIAARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END),
IIIAARD = IIIAARD /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END),
IVAARD = IVAARD /(CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END),
VAARD = VAARD /(CASE WHEN A.VD = 0 THEN NULL ELSE A.VD END),
VIAARD = VIAARD /(CASE WHEN A.VID = 0 THEN NULL ELSE A.VID END),
VIIAARD = VIIAARD /(CASE WHEN A.VIID = 0 THEN NULL ELSE A.VIID END),
VIIIAARD = VIIIAARD /(CASE WHEN A.VIIID = 0 THEN NULL ELSE A.VIIID END),
IXAARD = IXAARD /(CASE WHEN A.IXD = 0 THEN NULL ELSE A.IXD END),
XAARD = XAARD /(CASE WHEN A.XD = 0 THEN NULL ELSE A.XD END),
XIAARD = XIAARD /(CASE WHEN A.XID = 0 THEN NULL ELSE A.XID END)
FROM #PART_ONE AS A
GO

UPDATE #PART_FIVE
SET
IPB = IPB / IA,
IIPB = IIPB / IIA,
IIIPB = IIIPB / IIIA,
IVPB = IVPB / IVA,
VPB = VPB / VA,
VIPB = VIPB / VIA,
VIIPB = VIIPB / VIIA,
VIIIPB = VIIIPB / VIIIA,
IXPB = IXPB / IXA,
XPB = XPB / XA,
XIPB = XIPB / XIA,
IPBD = IPBD / IAD,
IIPBD = IIPBD / IIAD,
IIIPBD = IIIPBD / IIIAD,
IVPBD = IVPBD / IVAD,
VPBD = VPBD / VAD,
VIPBD = VIPBD / VIAD,
VIIPBD = VIIPBD / VIIAD,
VIIIPBD = VIIIPBD / VIIIAD,
IXPBD = IXPBD / IXAD,
XPBD = XPBD / XAD,
XIPBD = XIPBD / XIAD
GO