if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ANALYSIS]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ANALYSIS]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[APP_I]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[APP_I]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[APP_INFO]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[APP_INFO]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[APP_R]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[APP_R]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BAM087]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[BAM087]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[DACO_V4_1_CAL]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[DACO_V4_1_CAL]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[JAS002]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[JAS002]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[KRM021]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[KRM021]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[KRM023]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[KRM023]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PM]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PM]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[RESULT]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[RESULT]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[STM007]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[STM007]
GO

CREATE TABLE [dbo].[ANALYSIS] (
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[APPLICATION_DATE] [char] (8) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[CARD_TYPE] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[SUGGESTED_LINE] [int] NOT NULL ,
	[EV] [int] NULL ,
	[PB] [decimal](6, 4) NULL ,
	[RESULT_CODE] [int] NOT NULL ,
	[RESULT_MESSAGE] [char] (256) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[RISK_TWENTILE] [int] NULL ,
	[PROPENSITY_TWENTILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[APP_I] (
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[SEQ] [int] NOT NULL ,
	[APR] [float] NOT NULL ,
	[PERIODS] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[APP_INFO] (
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[APPLICATION_DATE] [char] (8) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[PRODUCT_CODE] [int] NOT NULL ,
	[BT_AMOUNT] [int] NOT NULL ,
	[NO_OF_BT] [int] NOT NULL ,
	[TEASER_PERIOD] [int] NOT NULL ,
	[BT_FEE_CODE] [int] NOT NULL ,
	[BT_FEE] [int] NULL ,
	[BT_FEE_PERCENTAGE] [float] NULL ,
	[CC_INTEREST_RATE] [float] NOT NULL ,
	[GENDER] [int] NOT NULL ,
	[CHANNEL] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[ZIP] [char] (3) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[CARD_TYPE] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[DOB] [char] (8) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[INCOME] [int] NOT NULL ,
	[EDUCATION] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[COMPANY_NAME] [nvarchar] (24) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[JOB_TITLE] [nvarchar] (16) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[TENURE] [int] NULL ,
	[BT_FEE_MONTH] [int] NOT NULL ,
	[MARTIAL_STATUS] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL,
	CASH_CARD_UTIL_CAP FLOAT, 
	CC_BALANCE_CAP INT, 
	CREDIT_CARD_UTIL_CAP FLOAT 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[APP_R] (
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[PRODUCT_CODE] [int] NOT NULL ,
	[LINE] [int] NOT NULL ,
	[DECISION] [int] NOT NULL ,
	[FINAL_DECISION_DATE] [char] (8) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[SYSTEM_DATE] [char] (8) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[BAM087] (
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[DATA_YYY] [char] (3) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[DATA_MM] [char] (2) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[BANK_CODE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[BANK_NAME] [char] (40) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[ACCOUNT_CODE] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[ACCOUNT_CODE2] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[PURPOSE_CODE] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CONTRACT_AMT1] [int] NULL ,
	[CONTRACT_AMT] [int] NULL ,
	[LOAN_AMT] [int] NULL ,
	[PASS_DUE_AMT] [int] NULL ,
	[PAY_CODE_12] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CO_LOAN] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[ACT_Y_MARK] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CONTRACT_AMT_Y] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[DACO_V4_1_CAL] (
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[HIT] [int] NULL ,
	[EXCLUSION] [int] NULL ,
	[GENDER] [int] NULL ,
	[FS002_6M_1K] [float] NULL ,
	[FS016_9M] [float] NULL ,
	[FS101_9M] [float] NULL ,
	[FS040] [float] NULL ,
	[FS068] [float] NULL ,
	[FS069] [float] NULL ,
	[FS031] [float] NULL ,
	[MS203_3M_1K] [float] NULL ,
	[MS203_6M_1K] [float] NULL ,
	[MS203_9M_1K] [float] NULL ,
	[APP_LAST_MONTH_BUCKET] [float] NULL ,
	[FS008_12MPLUS] [float] NULL ,
	[MS011_3M] [float] NULL ,
	[MS011_6M] [float] NULL ,
	[MS011_9M] [float] NULL ,
	[MS011_12M] [float] NULL ,
	[MS009_3M] [float] NULL ,
	[MS009_6M] [float] NULL ,
	[FS005_3M_1K] [float] NULL ,
	[MS056_6M_1K] [float] NULL ,
	[MS110_3M] [float] NULL ,
	[MS110_6M] [float] NULL ,
	[MS106_3M] [float] NULL ,
	[MS106_6M] [float] NULL ,
	[MT110_43] [float] NULL ,
	[FS002_6M_1K_Q] [float] NULL ,
	[MS056_6M_1K_TRAN] [float] NULL ,
	[FS069_TRAN] [float] NULL ,
	[INT015_9M] [float] NULL ,
	[FS031_TRAN] [float] NULL ,
	[MT203_42_1K] [float] NULL ,
	[APP_LAST_MONTH_BUCKET_T1] [float] NULL ,
	[FS008_12MPLUS_R] [float] NULL ,
	[MT011_31] [float] NULL ,
	[MT009_43] [float] NULL ,
	[SEX_TRAN] [int] NULL ,
	[MT110_43_TRAN] [float] NULL ,
	[FS002_6M_1K_Q_TRAN] [float] NULL ,
	[INT015_9M_TRAN] [float] NULL ,
	[MT203_42_1K_R] [float] NULL ,
	[MT011_31_Z] [float] NULL ,
	[MT009_43_Q] [float] NULL ,
	[MT203_42_1K_R_TRAN] [float] NULL ,
	[MT009_43_Q_TRAN2] [float] NULL ,
	[SCORE] [decimal](9, 8) NULL ,
	[RISK_TWENTILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[JAS002] (
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[EVER_DELINQUENT] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[DELINQUENT_DATE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[EVER_BAD_CHECK] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[BAD_CHECK_DATE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[EVER_REJECT] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[REJECT_DATE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[EVER_STOP_CARD] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[STOP_CARD_DATE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[KRM021] (
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CARD_BRAND] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CARD_TYPE] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[ISSUE] [char] (3) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[ISSUE_NAME] [char] (40) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[START_DATE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[STOP_DATE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[STOP_CODE] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[AB_CODE] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[M_S] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[LIMIT] [char] (6) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[RISK] [char] (6) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CLEAR_DATE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[PIDN] [char] (10) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CNAME] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[KRM023] (
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[YRMON] [char] (5) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[ISSUE] [char] (3) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[ISSUE_NAME] [char] (40) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[KR_CODE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[LIMIT] [char] (5) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[PAYMENT] [char] (3) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CASH] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[PAY_CODE] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PM] (
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
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
	[SCORE] [decimal](14, 8) NULL ,
	[DECILE] [int] NULL ,
	[PROPENSITY_TWENTILE] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[RESULT] (
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[APPLICATION_DATE] [char] (8) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[SUGGESTED_LINE] [int] NOT NULL ,
	[PRODUCT_CODE] [int] NOT NULL ,
	[RESULT_CODE] [int] NOT NULL ,
	[RESULT_MESSAGE] [char] (256) COLLATE Chinese_Taiwan_Stroke_CI_AS NOT NULL ,
	[EV] [int] NULL ,
	[PB] [decimal](6, 4) NULL,
	DOLLAR_BAD DECIMAL(5,4) 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[STM007] (
	[IDN] [char] (11) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[QUERY_DATE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[BANK_CODE] [char] (7) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[BANK_NAME] [char] (40) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[ITEM_LIST] [char] (10) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[INQ_PURPOSE1] [char] (1) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[INQ_PURPOSE] [char] (30) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL ,
	[CASE_SN] [char] (12) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL 
) ON [PRIMARY]
GO

