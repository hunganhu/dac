USE BSP_SML_20040830
GO

/*Prepare May 200405 data set*/
CREATE TABLE [IL_TESTING_200405] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[FM] [float] NULL ,
	[NAV] [float] NULL ,
	[GAV] [float] NULL ,
	[HU] [tinyint] NULL ,
	[TEST_CELL] [int] NULL ,
	[PRINCIPAL] [int] NULL ,
	[PRO_DECILE] [int] NULL ,
	[RISK] [float] NULL ,
	[OLE_DB] [char] (255) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[ZIP] [char] (3) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL
) ON [PRIMARY]
GO


INSERT INTO IL_TESTING_200405
SELECT MSN, IDN, TIME_STAMP, IDN1, IDN2, FIRST_LIEN_VALUE, NAV, GAV, NULL, TEST_CELL, PRINCIPAL, NULL, NULL, NULL, ZIP
FROM APP_INFO_BSP AS A
WHERE TIME_STAMP BETWEEN '20040525' AND '20040616'
GO

CREATE TABLE [IL_TESTING_200406] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[FM] [float] NULL ,
	[NAV] [float] NULL ,
	[GAV] [float] NULL ,
	[HU] [tinyint] NULL ,
	[TEST_CELL] [int] NULL ,
	[PRINCIPAL] [int] NULL ,
	[PRO_DECILE] [int] NULL ,
	[RISK] [float] NULL ,
	[OLE_DB] [char] (255) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[ZIP] [char] (3) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL
) ON [PRIMARY]
GO


INSERT INTO IL_TESTING_200406
SELECT MSN, IDN, TIME_STAMP, IDN1, IDN2, FIRST_LIEN_VALUE, NAV, GAV, NULL, TEST_CELL, PRINCIPAL, NULL, NULL, NULL, ZIP
FROM APP_INFO_BSP AS A
WHERE TIME_STAMP BETWEEN '20040616' AND '20040716'
GO

CREATE TABLE [IL_TESTING_200407] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[FM] [float] NULL ,
	[NAV] [float] NULL ,
	[GAV] [float] NULL ,
	[HU] [tinyint] NULL ,
	[TEST_CELL] [int] NULL ,
	[PRINCIPAL] [int] NULL ,
	[PRO_DECILE] [int] NULL ,
	[RISK] [float] NULL ,
	[OLE_DB] [char] (255) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[ZIP] [char] (3) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL
) ON [PRIMARY]
GO


INSERT INTO IL_TESTING_200407
SELECT MSN, IDN, TIME_STAMP, IDN1, IDN2, FIRST_LIEN_VALUE, NAV, GAV, NULL, TEST_CELL, PRINCIPAL, NULL, NULL, NULL, ZIP
FROM APP_INFO_BSP AS A
WHERE TIME_STAMP BETWEEN '20040716' AND '20040816'
GO

CREATE TABLE [IL_TESTING_200408] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[FM] [float] NULL ,
	[NAV] [float] NULL ,
	[GAV] [float] NULL ,
	[HU] [tinyint] NULL ,
	[TEST_CELL] [int] NULL ,
	[PRINCIPAL] [int] NULL ,
	[PRO_DECILE] [int] NULL ,
	[RISK] [float] NULL ,
	[OLE_DB] [char] (255) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[ZIP] [char] (3) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL
) ON [PRIMARY]
GO


INSERT INTO IL_TESTING_200408
SELECT MSN, IDN, TIME_STAMP, IDN1, IDN2, FIRST_LIEN_VALUE, NAV, GAV, NULL, TEST_CELL, PRINCIPAL, NULL, NULL, NULL, ZIP
FROM APP_INFO_BSP AS A
WHERE TIME_STAMP BETWEEN '20040816' AND '20040916'
GO

ALTER TABLE RESULT ADD RESERVED4 float, DECLINE_REASON int
GO
ALTER TABLE RESULT_R ADD RESERVED4 float, DECLINE_REASON int
GO

DELETE FROM IL_TESTING
GO

INSERT INTO IL_TESTING
SELECT *
FROM IL_TESTING_200405

EXEC SP_RENAME 'RESULT', 'RESULT_200405'
GO
EXEC SP_RENAME 'RESULT_R', 'RESULT_R_200405'
GO
EXEC SP_RENAME 'APP_INFO', 'APP_INFO_200405'
GO
EXEC SP_RENAME 'HU_RESULT', 'HU_RESULT_200405'
GO
EXEC SP_RENAME 'LU_RESULT', 'LU_RESULT_200405'
GO
EXEC SP_RENAME 'PM_RESULT', 'PM_RESULT_200405'
GO
EXEC SP_RENAME 'ERRORS', 'ERRORS_200405'
GO

CREATE TABLE [HU_RESULT] (
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[EXCLUSION] [int] NULL ,
	[MS110_3M] [float] NULL ,
	[MS110_6M] [float] NULL ,
	[MS110_9M] [float] NULL ,
	[MS106_3M] [float] NULL ,
	[MS106_6M] [float] NULL ,
	[MS106_9M] [float] NULL ,
	[MS106_12M] [float] NULL ,
	[MT110_42] [float] NULL ,
	[FS062_1K_6M] [int] NULL ,
	[APP_LAST_MONTH_BUCKET] [int] NULL ,
	[MS058_1K_9M] [float] NULL ,
	[MS120_9M] [float] NULL ,
	[FS016_3M] [int] NULL ,
	[FS016_6M] [int] NULL ,
	[FT016_43] [int] NULL ,
	[FS005_1K_3M] [int] NULL ,
	[FS005_1K_9M] [int] NULL ,
	[FS005_1k_12M] [int] NULL ,
	[FT005_1K_41] [int] NULL ,
	[MS056_1K_6M] [float] NULL ,
	[MS110_12M] [float] NULL ,
	[MT110_31] [float] NULL ,
	[MS101_3M] [float] NULL ,
	[MS101_9M] [float] NULL ,
	[MS101_12M] [float] NULL ,
	[MT101_41] [float] NULL ,
	[MS202_3M] [float] NULL ,
	[MS202_6M] [float] NULL ,
	[MT202_43] [float] NULL ,
	[FS040] [int] NULL ,
	[MT110_42_T] [float] NULL ,
	[FS062_1K_6M_T] [float] NULL ,
	[MT202_43_R] [float] NULL ,
	[MT202_43_R_T] [float] NULL ,
	[FS040_R] [float] NULL ,
	[FS040_R_T] [float] NULL ,
	[APP_LAST_MONTH_BUCKET_T] [float] NULL ,
	[MS058_1K_9M_T] [float] NULL ,
	[MS120_9M_T] [float] NULL ,
	[FT016_43_T] [float] NULL ,
	[FT005_1K_41_Q] [float] NULL ,
	[MS056_1K_6M_T] [float] NULL ,
	[MT110_31_Q] [float] NULL ,
	[MT101_41_T] [float] NULL ,
	[SCORE] [float] NULL,
        [TWENTILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [LU_RESULT] (
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[EXCLUSION] [int] NULL ,
	[MS011_3M] [float] NULL ,
	[MS011_6M] [float] NULL ,
	[MS011_9M] [float] NULL ,
	[MS011_12M] [float] NULL ,
	[FS003_1K_9M] [int] NULL ,
	[FS005_1K_3M] [int] NULL ,
	[FS016_3M] [int] NULL ,
	[FS016_6M] [int] NULL ,
	[FS016_9M] [int] NULL ,
	[FS016_12M] [int] NULL ,
	[FT016_31] [int] NULL ,
	[FS031] [int] NULL ,
	[FS040] [int] NULL ,
	[FS048] [int] NULL ,
	[FS008_12MPLUS] [int] NULL ,
	[RS005] [int] NULL ,
	[RS017] [int] NULL ,
	[FS003_1K_9M_Q] [int] NULL ,
	[FS040_Q] [int] NULL ,
	[FS048_Q] [int] NULL ,
	[FS003_1K_9M_Q_T] [float] NULL ,
	[FS040_Q_T] [float] NULL ,
	[FS048_Q_T] [float] NULL ,
	[FS005_1K_3M_Z] [int] NULL ,
	[MT011_31] [float] NULL ,
	[MT011_31_Z] [int] NULL ,
	[FS031_T] [float] NULL ,
	[FT016_31_T] [float] NULL ,
	[RS005_Z] [int] NULL ,
	[RS017_Z] [int] NULL ,
	[FS008_12MPLUS_Z] [int] NULL ,
	[SCORE] [float] NULL,
        [DECILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [PM_RESULT] (
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[HIT] [int] NULL ,
	[EXCLUSION] [int] NULL ,
	[FS029] [int] NULL ,
	[FS031] [int] NULL ,
	[FS042] [int] NULL ,
	[MS064] [float] NULL ,
	[MS119_3M] [float] NULL ,
	[MS011_3M] [decimal](13, 5) NULL ,
	[MS011_6M] [decimal](13, 5) NULL ,
	[MS023_9M] [float] NULL ,
	[FS105_9M] [float] NULL ,
	[FS106_9M] [float] NULL ,
	[FS106_12M] [float] NULL ,
	[MS104_3M] [float] NULL ,
	[MS104_6M] [float] NULL ,
	[MS104_9M] [float] NULL ,
	[MS105_9M] [float] NULL ,
	[MS106_9M] [float] NULL ,
	[MS108_12M] [float] NULL ,
	[MS118_3M] [float] NULL ,
	[MS118_6M] [float] NULL ,
	[MS118_9M] [float] NULL ,
	[FS212_3M] [float] NULL ,
	[FS212_6M] [float] NULL ,
	[FS030] [int] NULL ,
	[FS042_Q] [float] NULL ,
	[FT212_43] [float] NULL ,
	[FT212_43_R] [float] NULL ,
	[INT037_9] [float] NULL ,
	[INT037_9_Q] [float] NULL ,
	[INT062_9] [float] NULL ,
	[INT084_9] [float] NULL ,
	[INT084_9_R] [float] NULL ,
	[MS064_R] [float] NULL ,
	[MS108_12M_R] [float] NULL ,
	[MS119_3M_R] [float] NULL ,
	[MT011_43] [float] NULL ,
	[MT104_42] [float] NULL ,
	[MT104_42_Q] [float] NULL ,
	[MT118_42] [float] NULL ,
	[FS030_T] [float] NULL ,
	[FS042_Q_T] [float] NULL ,
	[FS106_12M_T] [float] NULL ,
	[FT212_43_R_T] [float] NULL ,
	[INT037_9_Q_T1] [float] NULL ,
	[INT062_9_T1] [float] NULL ,
	[INT084_9_R_T] [float] NULL ,
	[MS064_R_T] [float] NULL ,
	[MS108_12M_R_T] [float] NULL ,
	[MS119_3M_R_T] [float] NULL ,
	[MT011_43_Z] [int] NULL ,
	[MT104_42_Q_T] [float] NULL ,
	[MT118_42_T] [float] NULL ,
	[SCORE] [float] NULL ,
	[DECILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [ERRORS] (
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[ERRORS] [char] (255) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [RESULT] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TEST_CELL] [int] NOT NULL ,
	[HIGHEST_1] [int] NOT NULL ,
	[HIGHEST_2] [int] NOT NULL ,
	[RISK_SCORE] [float] NULL ,
	[RESERVED2] [int] NULL ,
	[RESERVED3] [float] NULL ,
        [RISK_CUT] [int] NULL, 
        [PROPENSITY_DECILE] [int] NULL,
	[REVIEWED] [int] NOT NULL,
	[SECURED_NPV] [float] NOT NULL,
	[UNSECURED_NPV] [float] NOT NULL,
        [RESERVED4] [float] NOT NULL,
        [DECLINE_REASON] [INT] NOT NULL
) ON [PRIMARY]
GO


CREATE TABLE [RESULT_R] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TEST_CELL] [int] NOT NULL ,
	[HIGHEST_1] [int] NOT NULL ,
	[HIGHEST_2] [int] NOT NULL ,
	[RISK_SCORE] [float] NULL ,
	[RESERVED2] [int] NULL ,
	[RESERVED3] [float] NULL ,
	[REVIEWED] [int] NOT NULL,
	[SECURED_NPV] [float] NOT NULL,
	[UNSECURED_NPV] [float] NOT NULL,
        [RESERVED4] [float] NOT NULL,
        [DECLINE_REASON] [INT] NOT NULL
) ON [PRIMARY]
GO

CREATE TABLE [APP_INFO] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[PRINCIPAL] [int] NOT NULL ,
	[TEST_CELL] [int] NOT NULL ,
	[GAV] [int] NOT NULL ,
	[NAV] [int] NOT NULL ,
	[ZIP] [char] (3) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[FIRST_LIEN_VALUE] [int] NOT NULL ,

) ON [PRIMARY]
GO


DELETE FROM IL_TESTING

INSERT INTO IL_TESTING
SELECT *
FROM IL_TESTING_200406

EXEC SP_RENAME 'RESULT', 'RESULT_200406'
GO
EXEC SP_RENAME 'RESULT_R', 'RESULT_R_200406'
GO
EXEC SP_RENAME 'APP_INFO', 'APP_INFO_200406'
GO
EXEC SP_RENAME 'HU_RESULT', 'HU_RESULT_200406'
GO
EXEC SP_RENAME 'LU_RESULT', 'LU_RESULT_200406'
GO
EXEC SP_RENAME 'PM_RESULT', 'PM_RESULT_200406'
GO
EXEC SP_RENAME 'ERRORS', 'ERRORS_200406'
GO

CREATE TABLE [HU_RESULT] (
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[EXCLUSION] [int] NULL ,
	[MS110_3M] [float] NULL ,
	[MS110_6M] [float] NULL ,
	[MS110_9M] [float] NULL ,
	[MS106_3M] [float] NULL ,
	[MS106_6M] [float] NULL ,
	[MS106_9M] [float] NULL ,
	[MS106_12M] [float] NULL ,
	[MT110_42] [float] NULL ,
	[FS062_1K_6M] [int] NULL ,
	[APP_LAST_MONTH_BUCKET] [int] NULL ,
	[MS058_1K_9M] [float] NULL ,
	[MS120_9M] [float] NULL ,
	[FS016_3M] [int] NULL ,
	[FS016_6M] [int] NULL ,
	[FT016_43] [int] NULL ,
	[FS005_1K_3M] [int] NULL ,
	[FS005_1K_9M] [int] NULL ,
	[FS005_1k_12M] [int] NULL ,
	[FT005_1K_41] [int] NULL ,
	[MS056_1K_6M] [float] NULL ,
	[MS110_12M] [float] NULL ,
	[MT110_31] [float] NULL ,
	[MS101_3M] [float] NULL ,
	[MS101_9M] [float] NULL ,
	[MS101_12M] [float] NULL ,
	[MT101_41] [float] NULL ,
	[MS202_3M] [float] NULL ,
	[MS202_6M] [float] NULL ,
	[MT202_43] [float] NULL ,
	[FS040] [int] NULL ,
	[MT110_42_T] [float] NULL ,
	[FS062_1K_6M_T] [float] NULL ,
	[MT202_43_R] [float] NULL ,
	[MT202_43_R_T] [float] NULL ,
	[FS040_R] [float] NULL ,
	[FS040_R_T] [float] NULL ,
	[APP_LAST_MONTH_BUCKET_T] [float] NULL ,
	[MS058_1K_9M_T] [float] NULL ,
	[MS120_9M_T] [float] NULL ,
	[FT016_43_T] [float] NULL ,
	[FT005_1K_41_Q] [float] NULL ,
	[MS056_1K_6M_T] [float] NULL ,
	[MT110_31_Q] [float] NULL ,
	[MT101_41_T] [float] NULL ,
	[SCORE] [float] NULL,
        [TWENTILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [LU_RESULT] (
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[EXCLUSION] [int] NULL ,
	[MS011_3M] [float] NULL ,
	[MS011_6M] [float] NULL ,
	[MS011_9M] [float] NULL ,
	[MS011_12M] [float] NULL ,
	[FS003_1K_9M] [int] NULL ,
	[FS005_1K_3M] [int] NULL ,
	[FS016_3M] [int] NULL ,
	[FS016_6M] [int] NULL ,
	[FS016_9M] [int] NULL ,
	[FS016_12M] [int] NULL ,
	[FT016_31] [int] NULL ,
	[FS031] [int] NULL ,
	[FS040] [int] NULL ,
	[FS048] [int] NULL ,
	[FS008_12MPLUS] [int] NULL ,
	[RS005] [int] NULL ,
	[RS017] [int] NULL ,
	[FS003_1K_9M_Q] [int] NULL ,
	[FS040_Q] [int] NULL ,
	[FS048_Q] [int] NULL ,
	[FS003_1K_9M_Q_T] [float] NULL ,
	[FS040_Q_T] [float] NULL ,
	[FS048_Q_T] [float] NULL ,
	[FS005_1K_3M_Z] [int] NULL ,
	[MT011_31] [float] NULL ,
	[MT011_31_Z] [int] NULL ,
	[FS031_T] [float] NULL ,
	[FT016_31_T] [float] NULL ,
	[RS005_Z] [int] NULL ,
	[RS017_Z] [int] NULL ,
	[FS008_12MPLUS_Z] [int] NULL ,
	[SCORE] [float] NULL,
        [DECILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [PM_RESULT] (
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[HIT] [int] NULL ,
	[EXCLUSION] [int] NULL ,
	[FS029] [int] NULL ,
	[FS031] [int] NULL ,
	[FS042] [int] NULL ,
	[MS064] [float] NULL ,
	[MS119_3M] [float] NULL ,
	[MS011_3M] [decimal](13, 5) NULL ,
	[MS011_6M] [decimal](13, 5) NULL ,
	[MS023_9M] [float] NULL ,
	[FS105_9M] [float] NULL ,
	[FS106_9M] [float] NULL ,
	[FS106_12M] [float] NULL ,
	[MS104_3M] [float] NULL ,
	[MS104_6M] [float] NULL ,
	[MS104_9M] [float] NULL ,
	[MS105_9M] [float] NULL ,
	[MS106_9M] [float] NULL ,
	[MS108_12M] [float] NULL ,
	[MS118_3M] [float] NULL ,
	[MS118_6M] [float] NULL ,
	[MS118_9M] [float] NULL ,
	[FS212_3M] [float] NULL ,
	[FS212_6M] [float] NULL ,
	[FS030] [int] NULL ,
	[FS042_Q] [float] NULL ,
	[FT212_43] [float] NULL ,
	[FT212_43_R] [float] NULL ,
	[INT037_9] [float] NULL ,
	[INT037_9_Q] [float] NULL ,
	[INT062_9] [float] NULL ,
	[INT084_9] [float] NULL ,
	[INT084_9_R] [float] NULL ,
	[MS064_R] [float] NULL ,
	[MS108_12M_R] [float] NULL ,
	[MS119_3M_R] [float] NULL ,
	[MT011_43] [float] NULL ,
	[MT104_42] [float] NULL ,
	[MT104_42_Q] [float] NULL ,
	[MT118_42] [float] NULL ,
	[FS030_T] [float] NULL ,
	[FS042_Q_T] [float] NULL ,
	[FS106_12M_T] [float] NULL ,
	[FT212_43_R_T] [float] NULL ,
	[INT037_9_Q_T1] [float] NULL ,
	[INT062_9_T1] [float] NULL ,
	[INT084_9_R_T] [float] NULL ,
	[MS064_R_T] [float] NULL ,
	[MS108_12M_R_T] [float] NULL ,
	[MS119_3M_R_T] [float] NULL ,
	[MT011_43_Z] [int] NULL ,
	[MT104_42_Q_T] [float] NULL ,
	[MT118_42_T] [float] NULL ,
	[SCORE] [float] NULL ,
	[DECILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [ERRORS] (
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[ERRORS] [char] (255) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [RESULT] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TEST_CELL] [int] NOT NULL ,
	[HIGHEST_1] [int] NOT NULL ,
	[HIGHEST_2] [int] NOT NULL ,
	[RISK_SCORE] [float] NULL ,
	[RESERVED2] [int] NULL ,
	[RESERVED3] [float] NULL ,
        [RISK_CUT] [int] NULL, 
        [PROPENSITY_DECILE] [int] NULL,
	[REVIEWED] [int] NOT NULL,
	[SECURED_NPV] [float] NOT NULL,
	[UNSECURED_NPV] [float] NOT NULL,
        [RESERVED4] [float] NOT NULL,
        [DECLINE_REASON] [INT] NOT NULL 
) ON [PRIMARY]
GO


CREATE TABLE [RESULT_R] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TEST_CELL] [int] NOT NULL ,
	[HIGHEST_1] [int] NOT NULL ,
	[HIGHEST_2] [int] NOT NULL ,
	[RISK_SCORE] [float] NULL ,
	[RESERVED2] [int] NULL ,
	[RESERVED3] [float] NULL ,
	[REVIEWED] [int] NOT NULL,
	[SECURED_NPV] [float] NOT NULL,
	[UNSECURED_NPV] [float] NOT NULL,
        [RESERVED4] [float] NOT NULL,
        [DECLINE_REASON] [INT] NOT NULL
) ON [PRIMARY]
GO

CREATE TABLE [APP_INFO] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[PRINCIPAL] [int] NOT NULL ,
	[TEST_CELL] [int] NOT NULL ,
	[GAV] [int] NOT NULL ,
	[NAV] [int] NOT NULL ,
	[ZIP] [char] (3) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[FIRST_LIEN_VALUE] [int] NOT NULL ,

) ON [PRIMARY]
GO


DELETE FROM IL_TESTING

INSERT INTO IL_TESTING
SELECT *
FROM IL_TESTING_200407

EXEC SP_RENAME 'RESULT', 'RESULT_200407'
GO
EXEC SP_RENAME 'RESULT_R', 'RESULT_R_200407'
GO
EXEC SP_RENAME 'APP_INFO', 'APP_INFO_200407'
GO
EXEC SP_RENAME 'HU_RESULT', 'HU_RESULT_200407'
GO
EXEC SP_RENAME 'LU_RESULT', 'LU_RESULT_200407'
GO
EXEC SP_RENAME 'PM_RESULT', 'PM_RESULT_200407'
GO
EXEC SP_RENAME 'ERRORS', 'ERRORS_200407'
GO

CREATE TABLE [HU_RESULT] (
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[EXCLUSION] [int] NULL ,
	[MS110_3M] [float] NULL ,
	[MS110_6M] [float] NULL ,
	[MS110_9M] [float] NULL ,
	[MS106_3M] [float] NULL ,
	[MS106_6M] [float] NULL ,
	[MS106_9M] [float] NULL ,
	[MS106_12M] [float] NULL ,
	[MT110_42] [float] NULL ,
	[FS062_1K_6M] [int] NULL ,
	[APP_LAST_MONTH_BUCKET] [int] NULL ,
	[MS058_1K_9M] [float] NULL ,
	[MS120_9M] [float] NULL ,
	[FS016_3M] [int] NULL ,
	[FS016_6M] [int] NULL ,
	[FT016_43] [int] NULL ,
	[FS005_1K_3M] [int] NULL ,
	[FS005_1K_9M] [int] NULL ,
	[FS005_1k_12M] [int] NULL ,
	[FT005_1K_41] [int] NULL ,
	[MS056_1K_6M] [float] NULL ,
	[MS110_12M] [float] NULL ,
	[MT110_31] [float] NULL ,
	[MS101_3M] [float] NULL ,
	[MS101_9M] [float] NULL ,
	[MS101_12M] [float] NULL ,
	[MT101_41] [float] NULL ,
	[MS202_3M] [float] NULL ,
	[MS202_6M] [float] NULL ,
	[MT202_43] [float] NULL ,
	[FS040] [int] NULL ,
	[MT110_42_T] [float] NULL ,
	[FS062_1K_6M_T] [float] NULL ,
	[MT202_43_R] [float] NULL ,
	[MT202_43_R_T] [float] NULL ,
	[FS040_R] [float] NULL ,
	[FS040_R_T] [float] NULL ,
	[APP_LAST_MONTH_BUCKET_T] [float] NULL ,
	[MS058_1K_9M_T] [float] NULL ,
	[MS120_9M_T] [float] NULL ,
	[FT016_43_T] [float] NULL ,
	[FT005_1K_41_Q] [float] NULL ,
	[MS056_1K_6M_T] [float] NULL ,
	[MT110_31_Q] [float] NULL ,
	[MT101_41_T] [float] NULL ,
	[SCORE] [float] NULL,
        [TWENTILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [LU_RESULT] (
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[EXCLUSION] [int] NULL ,
	[MS011_3M] [float] NULL ,
	[MS011_6M] [float] NULL ,
	[MS011_9M] [float] NULL ,
	[MS011_12M] [float] NULL ,
	[FS003_1K_9M] [int] NULL ,
	[FS005_1K_3M] [int] NULL ,
	[FS016_3M] [int] NULL ,
	[FS016_6M] [int] NULL ,
	[FS016_9M] [int] NULL ,
	[FS016_12M] [int] NULL ,
	[FT016_31] [int] NULL ,
	[FS031] [int] NULL ,
	[FS040] [int] NULL ,
	[FS048] [int] NULL ,
	[FS008_12MPLUS] [int] NULL ,
	[RS005] [int] NULL ,
	[RS017] [int] NULL ,
	[FS003_1K_9M_Q] [int] NULL ,
	[FS040_Q] [int] NULL ,
	[FS048_Q] [int] NULL ,
	[FS003_1K_9M_Q_T] [float] NULL ,
	[FS040_Q_T] [float] NULL ,
	[FS048_Q_T] [float] NULL ,
	[FS005_1K_3M_Z] [int] NULL ,
	[MT011_31] [float] NULL ,
	[MT011_31_Z] [int] NULL ,
	[FS031_T] [float] NULL ,
	[FT016_31_T] [float] NULL ,
	[RS005_Z] [int] NULL ,
	[RS017_Z] [int] NULL ,
	[FS008_12MPLUS_Z] [int] NULL ,
	[SCORE] [float] NULL,
        [DECILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [PM_RESULT] (
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[HIT] [int] NULL ,
	[EXCLUSION] [int] NULL ,
	[FS029] [int] NULL ,
	[FS031] [int] NULL ,
	[FS042] [int] NULL ,
	[MS064] [float] NULL ,
	[MS119_3M] [float] NULL ,
	[MS011_3M] [decimal](13, 5) NULL ,
	[MS011_6M] [decimal](13, 5) NULL ,
	[MS023_9M] [float] NULL ,
	[FS105_9M] [float] NULL ,
	[FS106_9M] [float] NULL ,
	[FS106_12M] [float] NULL ,
	[MS104_3M] [float] NULL ,
	[MS104_6M] [float] NULL ,
	[MS104_9M] [float] NULL ,
	[MS105_9M] [float] NULL ,
	[MS106_9M] [float] NULL ,
	[MS108_12M] [float] NULL ,
	[MS118_3M] [float] NULL ,
	[MS118_6M] [float] NULL ,
	[MS118_9M] [float] NULL ,
	[FS212_3M] [float] NULL ,
	[FS212_6M] [float] NULL ,
	[FS030] [int] NULL ,
	[FS042_Q] [float] NULL ,
	[FT212_43] [float] NULL ,
	[FT212_43_R] [float] NULL ,
	[INT037_9] [float] NULL ,
	[INT037_9_Q] [float] NULL ,
	[INT062_9] [float] NULL ,
	[INT084_9] [float] NULL ,
	[INT084_9_R] [float] NULL ,
	[MS064_R] [float] NULL ,
	[MS108_12M_R] [float] NULL ,
	[MS119_3M_R] [float] NULL ,
	[MT011_43] [float] NULL ,
	[MT104_42] [float] NULL ,
	[MT104_42_Q] [float] NULL ,
	[MT118_42] [float] NULL ,
	[FS030_T] [float] NULL ,
	[FS042_Q_T] [float] NULL ,
	[FS106_12M_T] [float] NULL ,
	[FT212_43_R_T] [float] NULL ,
	[INT037_9_Q_T1] [float] NULL ,
	[INT062_9_T1] [float] NULL ,
	[INT084_9_R_T] [float] NULL ,
	[MS064_R_T] [float] NULL ,
	[MS108_12M_R_T] [float] NULL ,
	[MS119_3M_R_T] [float] NULL ,
	[MT011_43_Z] [int] NULL ,
	[MT104_42_Q_T] [float] NULL ,
	[MT118_42_T] [float] NULL ,
	[SCORE] [float] NULL ,
	[DECILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [ERRORS] (
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL ,
	[ERRORS] [char] (255) COLLATE Chinese_Taiwan_Stroke_CS_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [RESULT] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TEST_CELL] [int] NOT NULL ,
	[HIGHEST_1] [int] NOT NULL ,
	[HIGHEST_2] [int] NOT NULL ,
	[RISK_SCORE] [float] NULL ,
	[RESERVED2] [int] NULL ,
	[RESERVED3] [float] NULL ,
        [RISK_CUT] [int] NULL, 
        [PROPENSITY_DECILE] [int] NULL,
	[REVIEWED] [int] NOT NULL,
	[SECURED_NPV] [float] NOT NULL,
	[UNSECURED_NPV] [float] NOT NULL,
        [RESERVED4] [float] NOT NULL,
        [DECLINE_REASON] [INT] NOT NULL 
) ON [PRIMARY]
GO


CREATE TABLE [RESULT_R] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TEST_CELL] [int] NOT NULL ,
	[HIGHEST_1] [int] NOT NULL ,
	[HIGHEST_2] [int] NOT NULL ,
	[RISK_SCORE] [float] NULL ,
	[RESERVED2] [int] NULL ,
	[RESERVED3] [float] NULL ,
	[REVIEWED] [int] NOT NULL,
	[SECURED_NPV] [float] NOT NULL,
	[UNSECURED_NPV] [float] NOT NULL,
        [RESERVED4] [float] NOT NULL,
        [DECLINE_REASON] [INT] NOT NULL
) ON [PRIMARY]
GO

CREATE TABLE [APP_INFO] (
	[MSN] [char] (20) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN1] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[IDN2] [char] (14) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[TIME_STAMP] [char] (12) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[PRINCIPAL] [int] NOT NULL ,
	[TEST_CELL] [int] NOT NULL ,
	[GAV] [int] NOT NULL ,
	[NAV] [int] NOT NULL ,
	[ZIP] [char] (3) COLLATE Chinese_Taiwan_Stroke_CS_AS NOT NULL ,
	[FIRST_LIEN_VALUE] [int] NOT NULL ,

) ON [PRIMARY]
GO


DELETE FROM IL_TESTING

INSERT INTO IL_TESTING 
SELECT *
FROM IL_TESTING_200408

EXEC SP_RENAME 'RESULT', 'RESULT_200408'
GO
EXEC SP_RENAME 'RESULT_R', 'RESULT_R_200408'
GO
EXEC SP_RENAME 'APP_INFO', 'APP_INFO_200408'
GO
EXEC SP_RENAME 'HU_RESULT', 'HU_RESULT_200408'
GO
EXEC SP_RENAME 'LU_RESULT', 'LU_RESULT_200408'
GO
EXEC SP_RENAME 'PM_RESULT', 'PM_RESULT_200408'
GO
EXEC SP_RENAME 'ERRORS', 'ERRORS_200408'
GO

