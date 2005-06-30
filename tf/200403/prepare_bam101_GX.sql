USE TF_KHJ
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200403]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200403]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200404]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200404]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200405]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200405]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200406]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200406]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200407]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200407]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200408]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200408]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200409]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200409]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200410]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200410]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200411]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200411]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[bam101_200412]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[bam101_200412]
GO

CREATE TABLE [dbo].[BAM101] (
	[idn] [char] (6) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[QRY_DT_AD] [char] (8) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[DATA_YYY] [char] (3) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[DATA_MM] [char] (2) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[WECDJ_AMT] [int] NULL ,
	[HIGZ_AMT] [int] NULL ,
	[MNSTRK_AMT] [int] NULL ,
	[U_AMT] [int] NULL ,
	[PQX_AMT] [int] NULL ,
	[F_AMT] [int] NULL ,
	[L_AMT] [int] NULL ,
	[NORMAL_LOAN_AMT] [int] NULL ,
	[PASS_DUE_AMT] [int] NULL ,
	[COLLECTION_AMT] [int] NULL ,
	[BAD_DEBT] [int] NULL
) ON [PRIMARY]
GO

INSERT INTO BAM101
SELECT idn, QRY_DT_AD, RTRIM(convert(char(3), convert(int,data_yyyy) - 1911)) as data_yyy, (case when len(data_mm) = 1 THEN '0' + data_mm ELSE data_mm END), WECDJ_AMT, HIGZ_AMT, MNSTRK_AMT, U_AMT, PQX_AMT, F_AMT, L_AMT, NORMAL_LOAN_AMT, PASS_DUE_AMT, COLLECTION_AMT, BAD_DEBT
FROM BAM101_DEDUP
GROUP BY idn, QRY_DT_AD, RTRIM(convert(char(3), convert(int,data_yyyy) - 1911)), (case when len(data_mm) = 1 THEN '0' + data_mm ELSE data_mm END), WECDJ_AMT, HIGZ_AMT, MNSTRK_AMT, U_AMT, PQX_AMT, F_AMT, L_AMT, NORMAL_LOAN_AMT, PASS_DUE_AMT, COLLECTION_AMT, BAD_DEBT
GO
--194250
ALTER TABLE BAM101 ADD MON_SINCE INT
GO

UPDATE BAM101
SET
MON_SINCE = CAST(data_yyy AS INT) * 12 + CAST(data_mm AS INT)
GO
--194250
SELECT *
INTO BAM101_200402
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200402')
GO

SELECT *
INTO BAM101_200403
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200403')
GO
--2179
SELECT *
INTO BAM101_200404
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200404')
GO
--14411
SELECT *
INTO BAM101_200405
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200405')
GO
--16557
SELECT *
INTO BAM101_200406
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200406')
GO
--18661
SELECT *
INTO BAM101_200407
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200407')
GO
--13655
SELECT *
INTO BAM101_200408
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200408')
GO
--5496
SELECT *
INTO BAM101_200409
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200409')
GO
--6671
SELECT *
INTO BAM101_200410
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200410')
GO
--6424
SELECT *
INTO BAM101_200411
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200411')
GO
--6604
SELECT *
INTO BAM101_200412
FROM BAM101
WHERE IDN IN
(SELECT IDN FROM dbo.data_availability WHERE JCIC_MONTH = '200412')
GO
--7080
CREATE INDEX IDX ON BAM101_200402(IDN)
CREATE INDEX IDX ON BAM101_200403(IDN)
CREATE INDEX IDX ON BAM101_200404(IDN)
CREATE INDEX IDX ON BAM101_200405(IDN)
CREATE INDEX IDX ON BAM101_200406(IDN)
CREATE INDEX IDX ON BAM101_200407(IDN)
CREATE INDEX IDX ON BAM101_200408(IDN)
CREATE INDEX IDX ON BAM101_200409(IDN)
CREATE INDEX IDX ON BAM101_200410(IDN)
CREATE INDEX IDX ON BAM101_200411(IDN)
CREATE INDEX IDX ON BAM101_200412(IDN)
