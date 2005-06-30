USE TF_GX
GO

/*----TF_GX----*/
DROP TABLE RS_200303, RS_TMP, RS_TMP1
GO

CREATE TABLE RS_200303
(IDN Char(6),
RS001 int,
RS002_1K int,
RS003_1K int,
RS004_1K int,
RS005 int,
RS006 int,
RS007 int,
RS008 int,
RS009 int,
RS010 int,
RS011 int,
RS012 int,
RS013 int,
RS014 int,
RS015 int,
RS016 int,
RS017 int,
RS018 int,
RS019 int,
RS020 int,
RS021 int,
RS022 int,
RS023 int,
RS024 int)
GO

INSERT INTO RS_200303
SELECT IDN,
0 AS RS001,
NULL AS RS002_1K,
NULL AS RS003_1K,
NULL AS RS004_1K,
NULL AS RS005,
NULL AS RS006,
NULL AS RS007,
NULL AS RS008,
NULL AS RS009,
NULL AS RS010,
NULL AS RS011,
NULL AS RS012,
NULL AS RS013,
NULL AS RS014,
NULL AS RS015,
NULL AS RS016,
NULL AS RS017,
NULL AS RS018,
NULL AS RS019,
NULL AS RS020,
NULL AS RS021,
NULL AS RS022,
NULL AS RS023,
NULL AS RS024
FROM Base_200303
GO
--220
/*UPDATE RS
SET
RS007=0
FROM BAS001_200303_Base_New AS A
WHERE A.IDN = RS.IDN*/

UPDATE RS_200303
SET
RS002_1K=0,
RS003_1K=0,
RS004_1K=0,
RS008=0,
RS009=0
FROM Base_200303 AS A
WHERE A.IDN = RS_200303.IDN AND
      A.AVAIL_FLAG & 0x01 = 0x01
GO
--171, KRM023
UPDATE RS_200303
SET
RS005=0,
RS006=0,
/*RS010=0,
RS011=0,*//*Not implemented*/
RS015=0,
RS016=0,
RS017=0,
RS018=0,
RS019=0,
RS020=0,
RS021=0,
RS022=0,
RS023=0,
RS024=0
FROM Base_200303 AS A
WHERE A.IDN = RS_200303.IDN AND
      A.AVAIL_FLAG & 0x02 = 0x02
GO
--181, KRM001
UPDATE RS_200303
SET
RS012=0,
RS013=0,
RS014=0
FROM Base_200303 AS A
WHERE A.IDN = RS_200303.IDN AND
      A.AVAIL_FLAG & 0x10 = 0x10
GO
--220, STM001

CREATE TABLE RS_TMP
(IDN Char(6),
 V1 int,
 V2 int
)
GO

/*--Create temp table RS_TMP1--*/
CREATE TABLE RS_TMP1
(IDN Char(6),
 V1 int)
GO