USE TF_KHJ
GO

/*----TF_KHJ----*/
DROP TABLE MS_200409
GO
DROP TABLE MS_TMP
GO
DROP TABLE MS_TMP1
GO
DROP TABLE MS_TMP2
GO
DROP TABLE Lines
GO
DROP TABLE Open_Issuer
GO

CREATE TABLE MS_200409
(IDN Char(6),
MS001_3M_1k float, MS001_6M_1k float, MS001_9M_1k float, MS001_12M_1k float,
MS002_3M_1k float, MS002_6M_1k float, MS002_9M_1k float, MS002_12M_1k float,
MS003_3M_1k float, MS003_6M_1k float, MS003_9M_1k float, MS003_12M_1k float,
MS004_3M_1k float, MS004_6M_1k float, MS004_9M_1k float, MS004_12M_1k float,
MS005_3M_1k float, MS005_6M_1k float, MS005_9M_1k float, MS005_12M_1k float,
MS006_3M_1k float, MS006_6M_1k float, MS006_9M_1k float, MS006_12M_1k float,
MS007_3M float,	MS007_6M float,	MS007_9M float,	MS007_12M float,
MS008_3M float,	MS008_6M float,	MS008_9M float,	MS008_12M float,
MS009_3M float,	MS009_6M float,	MS009_9M float,	MS009_12M float,
MS010_3M float,	MS010_6M float,	MS010_9M float,	MS010_12M float,
MS011_3M float,	MS011_6M float,	MS011_9M float,	MS011_12M float,
MS012_3M float,	MS012_6M float,	MS012_9M float,	MS012_12M float,
MS013_3M float,	MS013_6M float,	MS013_9M float,	MS013_12M float,
MS014_3M float,	MS014_6M float,	MS014_9M float,	MS014_12M float,
MS015_3M float,	MS015_6M float,	MS015_9M float,	MS015_12M float,
MS016_3M float,	MS016_6M float,	MS016_9M float,	MS016_12M float,
MS017_3M float,	MS017_6M float,	MS017_9M float,	MS017_12M float,
MS018_3M float,	MS018_6M float,	MS018_9M float,	MS018_12M float,
MS019_3M float,	MS019_6M float,	MS019_9M float,	MS019_12M float,
MS020_3M float,	MS020_6M float,	MS020_9M float,	MS020_12M float,
MS021_3M float,	MS021_6M float,	MS021_9M float,	MS021_12M float,
MS022_3M float,	MS022_6M float,	MS022_9M float,	MS022_12M float,
MS023_3M float,	MS023_6M float,	MS023_9M float,	MS023_12M float,
MS024_3M float,	MS024_6M float,	MS024_9M float,	MS024_12M float,
MS025_3M float,	MS025_6M float,	MS025_9M float,	MS025_12M float,
MS026_3M float,	MS026_6M float,	MS026_9M float,	MS026_12M float,
MS027_3M float,	MS027_6M float,	MS027_9M float,	MS027_12M float,
MS028_3M float,	MS028_6M float,	MS028_9M float,	MS028_12M float,
MS029_3M float,	MS029_6M float,	MS029_9M float,	MS029_12M float,
MS030_3M float,	MS030_6M float,	MS030_9M float,	MS030_12M float,
MS031 float,			
MS032 float,
MS033 float,
MS034 float,
MS035 float,
MS036 float,
MS037 float,
MS038 float,
MS039 float,
MS040 float,
MS041 float,
MS042 float,
MS043 float,
MS044 float,
MS045 float,
MS046 float,
MS047 float,
MS048 float,			
MS049 float,			
MS050 float,			
MS051 float,			
MS052 float,			
MS053 float,			
MS054 float,			
MS055 float,			
MS056_3M_1k float, MS056_6M_1k float, MS056_9M_1k float, MS056_12M_1k float,
MS057_3M_1k float, MS057_6M_1k float, MS057_9M_1k float, MS057_12M_1k float,
MS058_3M_1k float, MS058_6M_1k float, MS058_9M_1k float, MS058_12M_1k float,
MS059 float,			
MS060 float,			
MS061 float,			
MS062 float,			
MS063 float,			
MS064 float,
MS065 float,
MS066 float,
MS067 float,
MS068 float,
MS069 float,
MS070 float,
MS071 float,
MS072 float,
MS073 float,
MS074 float,
MS075 float,
MS076 float,
MS077 float,
MS078 float,
MS079 float,
MS080 float,
MS081 float,
MS082 float,
MS083 float,
MS084 float,
MS085 float,
MS086 float,
MS087 float,
MS088 float,
MS089 float,
MS090 float,
MS091 float,
MS092 float,
MS093 float
)
GO


INSERT INTO MS_200409(IDN)
SELECT
IDN
FROM BASE_200409
GO
/*220*/
UPDATE MS_200409
SET
MS001_3M_1k=0,	MS001_6M_1k=0,	MS001_9M_1k=0,	MS001_12M_1k=0,
MS002_3M_1k=0,	MS002_6M_1k=0,	MS002_9M_1k=0,	MS002_12M_1k=0,
MS003_3M_1k=0,	MS003_6M_1k=0,	MS003_9M_1k=0,	MS003_12M_1k=0,
MS004_3M_1k=0,	MS004_6M_1k=0,	MS004_9M_1k=0,	MS004_12M_1k=0,
MS005_3M_1k=0,	MS005_6M_1k=0,	MS005_9M_1k=0,	MS005_12M_1k=0,
MS006_3M_1k=0,	MS006_6M_1k=0,	MS006_9M_1k=0,	MS006_12M_1k=0,
MS008_3M=0,	MS008_6M=0,	MS008_9M=0,	MS008_12M=0,
MS009_3M=0,	MS009_6M=0,	MS009_9M=0,	MS009_12M=0,
MS010_3M=0,	MS010_6M=0,	MS010_9M=0,	MS010_12M=0,
MS011_3M=0,	MS011_6M=0,	MS011_9M=0,	MS011_12M=0,
MS012_3M=0,	MS012_6M=0,	MS012_9M=0,	MS012_12M=0,
MS013_3M=0,	MS013_6M=0,	MS013_9M=0,	MS013_12M=0,
MS014_3M=0,	MS014_6M=0,	MS014_9M=0,	MS014_12M=0,
MS015_3M=0,	MS015_6M=0,	MS015_9M=0,	MS015_12M=0,
MS016_3M=0,	MS016_6M=0,	MS016_9M=0,	MS016_12M=0,
MS017_3M=0,	MS017_6M=0,	MS017_9M=0,	MS017_12M=0,
MS023_3M=0,	MS023_6M=0,	MS023_9M=0,	MS023_12M=0,
MS024_3M=0,	MS024_6M=0,	MS024_9M=0,	MS024_12M=0,
MS025_3M=0,	MS025_6M=0,	MS025_9M=0,	MS025_12M=0,
MS026_3M=0,	MS026_6M=0,	MS026_9M=0,	MS026_12M=0,
MS056_3M_1k=0,	MS056_6M_1k=0,	MS056_9M_1k=0,	MS056_12M_1k=0,
MS057_3M_1k=0,	MS057_6M_1k=0,	MS057_9M_1k=0,	MS057_12M_1k=0,
MS058_3M_1k=0,	MS058_6M_1k=0,	MS058_9M_1k=0,	MS058_12M_1k=0
FROM BASE_200409 AS A, MS_200409
WHERE A.IDN = MS_200409.IDN AND
      AVAIL_FLAG & 0x1 = 0x1
GO
/*171, KRM023*/ 
UPDATE MS_200409
SET
MS018_3M=0,	MS018_6M=0,	MS018_9M=0,	MS018_12M=0,
MS019_3M=0,	MS019_6M=0,	MS019_9M=0,	MS019_12M=0,
MS020_3M=0,	MS020_6M=0,	MS020_9M=0,	MS020_12M=0,
MS021_3M=0,	MS021_6M=0,	MS021_9M=0,	MS021_12M=0,
MS027_3M=0,	MS027_6M=0,	MS027_9M=0,	MS027_12M=0,
MS028_3M=0,	MS028_6M=0,	MS028_9M=0,	MS028_12M=0,
MS029_3M=0,	MS029_6M=0,	MS029_9M=0,	MS029_12M=0,
MS030_3M=0,	MS030_6M=0,	MS030_9M=0,	MS030_12M=0
FROM BASE_200409 AS A, MS_200409
WHERE A.IDN = MS_200409.IDN AND
      AVAIL_FLAG & 0x2 = 0x2
GO
/*181, KRM001*/
UPDATE MS_200409
SET
MS031=0,
MS032=0,
MS033=0,
MS034=0,
MS035=0,
MS036=0,
MS037=0,
MS038=0,
MS039=0,
MS040=0,
MS041=0,
MS042=0,
MS043=0,
MS044=0,
MS045=0,
MS046=0,
MS047=0,
MS048=0,
MS049=0,
MS050=0,
MS051=0,
MS052=0,
MS053=0,
MS054=0,
MS055=0,
MS059=0,
MS060=0,
MS061=0,
MS062=0,
MS063=0,
MS064=0,
MS065=0,
MS066=0,
MS067=0,
MS068=0,
MS069=0,
MS070=0,
MS071=0,
MS072=0,
MS073=0,
MS074=0,
MS075=0,
MS076=0,
MS077=0,
MS078=0,
MS079=0,
MS080=0,
MS081=0,
MS082=0,
MS083=0,
MS084=0,
MS085=0,
MS086=0,
MS087=0,
MS088=0,
MS089=0,
MS090=0,
MS091=0,
MS092=0,
MS093=0
FROM BASE_200409 AS A, MS_200409
WHERE A.IDN = MS_200409.IDN AND
      AVAIL_FLAG & 0x4 = 0x4
GO 
/*185, BAM085*/
CREATE TABLE MS_TMP
(IDN Char(6),
 Mon INT,
 V1 float,
 V2 float,
 V3 float)
GO

CREATE TABLE MS_TMP1
(IDN Char(6),
 Mon INT,
 V1 float)
GO

/*----Create temp table for calculating incremental line----*/
CREATE TABLE Lines
(IDN Char(6),
 Issuer char(3),
 Mon int,
 Line float)
GO

CREATE TABLE Open_Issuer
(IDN Char(6),
 Issuer char(3),
 MON int)
GO

CREATE TABLE MS_TMP2
(IDN Char(6),
 Issuer char(3),
 Mon int,
 Line float)
GO

