if exists (select * from sysobjects where id = object_id(N'ANALYSIS') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table ANALYSIS
GO

if exists (select * from sysobjects where id = object_id(N'APP_I') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table APP_I
GO

if exists (select * from sysobjects where id = object_id(N'APP_INFO') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table APP_INFO
GO

if exists (select * from sysobjects where id = object_id(N'APP_R') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table APP_R
GO

if exists (select * from sysobjects where id = object_id(N'BAM087') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table BAM087
GO

if exists (select * from sysobjects where id = object_id(N'DACO_V4_1_CAL') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table DACO_V4_1_CAL
GO

if exists (select * from sysobjects where id = object_id(N'JAS002') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table JAS002
GO

if exists (select * from sysobjects where id = object_id(N'KRM021') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table KRM021
GO

if exists (select * from sysobjects where id = object_id(N'KRM023') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table KRM023
GO

if exists (select * from sysobjects where id = object_id(N'PM') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table PM
GO

if exists (select * from sysobjects where id = object_id(N'RESULT') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table RESULT
GO

if exists (select * from sysobjects where id = object_id(N'KRM037') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table KRM037
GO

if exists (select * from sysobjects where id = object_id(N'KRM040') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table KRM040
GO

if exists (select * from sysobjects where id = object_id(N'STM007') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table STM007
GO

CREATE TABLE ANALYSIS (
	IDN char (11) NOT NULL,
	CASE_SN char (12) NOT NULL,
	APPLICATION_DATE char (8) NOT NULL,
	CARD_TYPE char (1) NOT NULL,
	SUGGESTED_LINE int NOT NULL,
	EV int NULL,
	PB decimal(6, 4) NULL,
	RESULT_CODE int NOT NULL,
	RESULT_MESSAGE char (256) NOT NULL,
	RISK_TWENTILE int NULL,
	PROPENSITY_TWENTILE int NULL
) ON PRIMARY
GO

CREATE TABLE APP_I (
	CASE_SN char (12) NOT NULL,
	IDN char (11) NOT NULL,
	SEQ int NOT NULL,
	APR float NOT NULL,
	PERIODS int NOT NULL
) ON PRIMARY
GO

CREATE TABLE APP_INFO (
	CASE_SN char (12) NOT NULL,
	IDN char (11) NOT NULL,
	APPLICATION_DATE char (8) NOT NULL,
	PRODUCT_CODE int NOT NULL,
	BT_AMOUNT int NOT NULL,
	NO_OF_BT int NOT NULL,
	TEASER_PERIOD int NOT NULL,
	BT_FEE_CODE int NOT NULL,
	BT_FEE int NULL,
	BT_FEE_PERCENTAGE float NULL,
	CC_INTEREST_RATE float NOT NULL,
	GENDER int NOT NULL,
	CHANNEL char (12) NOT NULL,
	ZIP char (3) NOT NULL,
	CARD_TYPE char (1) NOT NULL,
	DOB char (8) NOT NULL,
	INCOME int NOT NULL,
	EDUCATION char (1) NULL,
	COMPANY_NAME nvarchar (24) NULL,
	JOB_TITLE nvarchar (16) NULL,
	TENURE int NULL,
	BT_FEE_MONTH int NOT NULL,
	MARTIAL_STATUS char (1) NOT NULL,
	CASH_CARD_UTIL_CAP FLOAT,
	CC_BALANCE_CAP INT,
	CREDIT_CARD_UTIL_CAP FLOAT
) ON PRIMARY
GO

CREATE TABLE APP_R (
	CASE_SN char (12) NOT NULL,
	IDN char (11) NOT NULL,
	PRODUCT_CODE int NOT NULL,
	LINE int NOT NULL,
	DECISION int NOT NULL,
	FINAL_DECISION_DATE char (8) NOT NULL,
	SYSTEM_DATE char (8) NOT NULL
) ON PRIMARY
GO

CREATE TABLE BAM087 (
	CASE_SN char (12) NOT NULL,
	IDN char (11) NOT NULL,
	DATA_YYY char (3) NULL,
	DATA_MM char (2) NULL,
	BANK_CODE char (7) NULL,
	BANK_NAME char (40) NULL,
	ACCOUNT_CODE char (1) NULL,
	ACCOUNT_CODE2 char (1) NULL,
	PURPOSE_CODE char (1) NULL,
	CONTRACT_AMT1 int NULL,
	CONTRACT_AMT int NULL,
	LOAN_AMT int NULL,
	PASS_DUE_AMT int NULL,
	PAY_CODE_12 char (12) NULL,
	CO_LOAN char (1) NULL,
	ACT_Y_MARK char (1) NULL,
	CONTRACT_AMT_Y int NULL
) ON PRIMARY
GO

CREATE TABLE DACO_V4_1_CAL (
	CASE_SN char (12) NULL,
	HIT int NULL,
	EXCLUSION int NULL,
	GENDER int NULL,
	FS002_6M_1K float NULL,
	FS016_9M float NULL,
	FS101_9M float NULL,
	FS040 float NULL,
	FS068 float NULL,
	FS069 float NULL,
	FS031 float NULL,
	MS203_3M_1K float NULL,
	MS203_6M_1K float NULL,
	MS203_9M_1K float NULL,
	APP_LAST_MONTH_BUCKET float NULL,
	FS008_12MPLUS float NULL,
	MS011_3M float NULL,
	MS011_6M float NULL,
	MS011_9M float NULL,
	MS011_12M float NULL,
	MS009_3M float NULL,
	MS009_6M float NULL,
	FS005_3M_1K float NULL,
	MS056_6M_1K float NULL,
	MS110_3M float NULL,
	MS110_6M float NULL,
	MS106_3M float NULL,
	MS106_6M float NULL,
	MT110_43 float NULL,
	FS002_6M_1K_Q float NULL,
	MS056_6M_1K_TRAN float NULL,
	FS069_TRAN float NULL,
	INT015_9M float NULL,
	FS031_TRAN float NULL,
	MT203_42_1K float NULL,
	APP_LAST_MONTH_BUCKET_T1 float NULL,
	FS008_12MPLUS_R float NULL,
	MT011_31 float NULL,
	MT009_43 float NULL,
	SEX_TRAN int NULL,
	MT110_43_TRAN float NULL,
	FS002_6M_1K_Q_TRAN float NULL,
	INT015_9M_TRAN float NULL,
	MT203_42_1K_R float NULL,
	MT011_31_Z float NULL,
	MT009_43_Q float NULL,
	MT203_42_1K_R_TRAN float NULL,
	MT009_43_Q_TRAN2 float NULL,
	SCORE decimal(9, 8) NULL,
	RISK_TWENTILE int NULL
) ON PRIMARY
GO

CREATE TABLE JAS002 (
	IDN char (11) NULL,
	EVER_DELINQUENT char (1) NULL,
	DELINQUENT_DATE char (7) NULL,
	EVER_BAD_CHECK char (1) NULL,
	BAD_CHECK_DATE char (7) NULL,
	EVER_REJECT char (1) NULL,
	REJECT_DATE char (7) NULL,
	EVER_STOP_CARD char (1) NULL,
	STOP_CARD_DATE char (7) NULL,
	CASE_SN char (12) NULL
) ON PRIMARY
GO

CREATE TABLE KRM021 (
	IDN char (11) NULL,
	CARD_BRAND char (1) NULL,
	CARD_TYPE char (1) NULL,
	ISSUE char (3) NULL,
	ISSUE_NAME char (40) NULL,
	START_DATE char (7) NULL,
	STOP_DATE char (7) NULL,
	STOP_CODE char (1) NULL,
	AB_CODE char (1) NULL,
	M_S char (1) NULL,
	LIMIT char (6) NULL,
	RISK char (6) NULL,
	CLEAR_DATE char (7) NULL,
	PIDN char (10) NULL,
	CNAME char (12) NULL,
	CASE_SN char (12) NULL
) ON PRIMARY
GO

CREATE TABLE KRM023 (
	IDN char (11) NOT NULL,
	YRMON char (5) NOT NULL,
	ISSUE char (3) NOT NULL,
	ISSUE_NAME char (40) NULL,
	KR_CODE char (7) NULL,
	LIMIT char (5) NULL,
	PAYMENT char (3) NULL,
	CASH char (1) NULL,
	PAY_CODE char (1) NULL,
	CASE_SN char (12) NOT NULL
) ON PRIMARY
GO

CREATE TABLE PM (
	CASE_SN char (12) NULL,
	HIT int NULL,
	EXCLUSION int NULL,
	FS029 int NULL,
	FS031 int NULL,
	FS042 int NULL,
	MS064 float NULL,
	MS119_3M float NULL,
	MS011_3M decimal(13, 5) NULL,
	MS011_6M decimal(13, 5) NULL,
	MS023_9M float NULL,
	FS105_9M float NULL,
	FS106_9M float NULL,
	FS106_12M float NULL,
	MS104_3M float NULL,
	MS104_6M float NULL,
	MS104_9M float NULL,
	MS105_9M float NULL,
	MS106_9M float NULL,
	MS108_12M float NULL,
	MS118_3M float NULL,
	MS118_6M float NULL,
	MS118_9M float NULL,
	FS212_3M float NULL,
	FS212_6M float NULL,
	FS030 int NULL,
	FS042_Q float NULL,
	FT212_43 float NULL,
	FT212_43_R float NULL,
	INT037_9 float NULL,
	INT037_9_Q float NULL,
	INT062_9 float NULL,
	INT084_9 float NULL,
	INT084_9_R float NULL,
	MS064_R float NULL,
	MS108_12M_R float NULL,
	MS119_3M_R float NULL,
	MT011_43 float NULL,
	MT104_42 float NULL,
	MT104_42_Q float NULL,
	MT118_42 float NULL,
	FS030_T float NULL,
	FS042_Q_T float NULL,
	FS106_12M_T float NULL,
	FT212_43_R_T float NULL,
	INT037_9_Q_T1 float NULL,
	INT062_9_T1 float NULL,
	INT084_9_R_T float NULL,
	MS064_R_T float NULL,
	MS108_12M_R_T float NULL,
	MS119_3M_R_T float NULL,
	MT011_43_Z int NULL,
	MT104_42_Q_T float NULL,
	MT118_42_T float NULL,
	SCORE decimal(14, 8) NULL,
	DECILE int NULL,
	PROPENSITY_TWENTILE int NULL
) ON PRIMARY
GO

CREATE TABLE RESULT (
	CASE_SN char (12) NOT NULL,
	IDN char (11) NOT NULL,
	APPLICATION_DATE char (8) NOT NULL,
	SUGGESTED_LINE int NOT NULL,
	PRODUCT_CODE int NOT NULL,
	RESULT_CODE int NOT NULL,
	RESULT_MESSAGE char (256) NOT NULL,
	EV int NULL,
	PB decimal(6, 4) NULL,
	DOLLAR_BAD DECIMAL(5,4)
) ON PRIMARY
GO

CREATE TABLE STM007 (
	IDN char (11) NULL,
	QUERY_DATE char (7) NULL,
	BANK_CODE char (7) NULL,
	BANK_NAME char (40) NULL,
	ITEM_LIST char (10) NULL,
	INQ_PURPOSE1 char (1) NULL,
	INQ_PURPOSE char (30) NULL,
	CASE_SN char (12) NULL
) ON PRIMARY
GO

CREATE TABLE KRM037 (
	CASE_SN char (12) NOT NULL,
	IDN char (11) NOT NULL,
	BILL_DATE char (7) NULL,
	ISSUE char (3) NULL,
	ISSUE_NAME char (24) NULL,
	CARD_TYPE char (7) NULL,
	PERM_LIMIT int NULL,
	TEMP_LIMIT int NULL,
	CASH_LIMIT int NULL,
	PAYABLE char (3) NULL,
	CASH_LENT int NULL,
	LAST_PAYA char (3) NULL,
	REVOL_BAL int NULL,
	PAY_STAT char (1) NULL,
	PAY_CODE char (1) NULL,
	REVOL_RATE int NULL,
	PRE_OWED int NULL,
	DEBT_CODE char (1) NULL,
	CLOSE_CODE char (1) NULL,
	CLEAR_DATE char (7) NULL
) ON PRIMARY
GO

CREATE TABLE KRM040 (
	CASE_SN char (12) NOT NULL,
	IDN char (11) NOT NULL,
	BILL_DATE char (7) NULL,
	ISSUE char (3) NULL,
	ISSUE_NAME char (24) NULL,
	BILL_MARK char (2) NULL,
	CARD_TYPE char (7) NULL,
	PERM_LIMIT int NULL,
	TEMP_LIMIT int NULL,
	CASH_LIMIT int NULL,
	PAYABLE int NULL,
	CASH_LENT int NULL,
	LAST_PAYA int NULL,
	REVOL_BAL int NULL,
	PAY_STAT char (1) NULL,
	PAY_CODE char (1) NULL,
	REVOL_RATE int NULL,
	PRE_OWED int NULL,
	DEBT_CODE char (1) NULL,
	CLOSE_CODE char (1) NULL,
	CLEAR_DATE char (7) NULL
) ON PRIMARY
GO




CREATE INDEX I_BAM087 ON BAM087(CASE_SN, IDN);
CREATE INDEX I_JAS002 ON JAS002(CASE_SN, IDN);
CREATE INDEX I_KRM021 ON KRM021(CASE_SN, IDN);
CREATE INDEX I_KRM023 ON KRM023(CASE_SN, IDN);
CREATE INDEX I_STM007 ON STM007(CASE_SN, IDN);
CREATE INDEX I_BAM037 ON BAM037(CASE_SN, IDN);
CREATE INDEX I_BAM040 ON BAM040(CASE_SN, IDN);
GO