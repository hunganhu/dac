SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[KRM037]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[KRM037](
	[IDN] [char](11) NULL,
	[INPUT_TIME] [char](12) NULL,
	[INQUIRY_DATE] [char](8) NULL,
	[BILL_DATE] [char](7) NULL,
	[ISSUE] [char](3) NULL,
	[ISSUE_NAME] [char](24) NULL,
	[CARD_TYPE] [char](7) NULL,
	[PERM_LIMIT] [int] NULL,
	[TEMP_LIMIT] [int] NULL,
	[CASH_LIMIT] [int] NULL,
	[PAYABLE] [char](3) NULL,
	[CASH_LENT] [int] NULL,
	[LAST_PAYA] [char](3) NULL,
	[REVOL_BAL] [int] NULL,
	[PAY_STAT] [char](1) NULL,
	[PAY_CODE] [char](1) NULL,
	[REVOL_RATE] [int] NULL,
	[PRE_OWED] [int] NULL,
	[DEBT_CODE] [char](1) NULL,
	[CLOSE_CODE] [char](1) NULL,
	[CLEAR_DATE] [char](7) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[MARRIAGE_LOOKUP]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[MARRIAGE_LOOKUP](
	[CODE] [int] NULL,
	[DES] [varchar](4) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[RESULT]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[RESULT](
	[IDN] [char](11) NULL,
	[INPUT_TIME] [char](12) NULL,
	[PB] [float] NULL,
	[MIN_RATE] [decimal](5, 4) NULL,
	[NPV] [float] NULL,
	[SUGGESTION] [varchar](50) NULL,
	[SUGGESTION_CODE] [int] NULL,
	[APPROVED_AMOUNT] [int] NULL,
	[RESULT_FILE_GEN_TIME] [char](12) NULL,
	[UNSECURED_BALANCE] [int] NULL,
	[APPROVED_AMOUNT_FSC] [int] NULL,
	[SPECIAL_NOTE] [char](100) NULL,
	[VERSION] [varchar](10) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[STM007]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[STM007](
	[IDN] [char](11) NULL,
	[INQUIRY_DATE] [char](8) NULL,
	[QUERY_DATE] [char](7) NULL,
	[BANK_CODE] [char](7) NULL,
	[BANK_NAME] [char](40) NULL,
	[ITEM_LIST] [char](10) NULL,
	[INPUT_TIME] [char](12) NULL,
	[INQ_PURPOSE_1] [char](1) NULL,
	[INQ_PURPOSE] [char](30) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[VAM102]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[VAM102](
	[IDN] [char](11) NULL,
	[INQUIRY_DATE] [char](8) NULL,
	[INPUT_TIME] [char](12) NULL,
	[DATA_DATE] [char](7) NULL,
	[MAINCODE] [char](1) NULL,
	[MAINNOTE] [char](36) NULL,
	[SUBCODE] [char](1) NULL,
	[SUBNOTE] [char](60) NULL,
	[NOTE] [char](100) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[APPLICANT]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[APPLICANT](
	[IDN] [char](11) NULL,
	[INPUT_TIME] [char](12) NULL,
	[CNAME] [char](20) NULL,
	[BIRTHDAY] [char](8) NULL,
	[ZIP] [char](3) NULL,
	[APPLICATION_AMOUNT] [int] NULL,
	[APPLICATION_RATE] [decimal](6, 5) NULL,
	[APPLICATION_TERMS] [int] NULL,
	[CHANNEL] [char](20) NULL,
	[APPLICATION_FEE] [int] NULL,
	[MARRIAGE] [int] NULL,
	[CHILDREN] [int] NULL,
	[EDUCATION] [int] NULL,
	[INPUT_USER] [varchar](40) NULL,
	[FIXED_LINE_PHONE] [varchar](10) NULL,
	[CELL_PHONE] [varchar](10) NULL,
	[MONTHLY_INCOME] [int] NULL,
	[PRODUCT_TYPE] [int] NULL,
	[BRANCH] [char](4) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[BAM086]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[BAM086](
	[IDN] [char](11) NULL,
	[INQUIRY_DATE] [char](8) NULL,
	[DATA_YYY] [char](3) NULL,
	[DATA_MM] [char](2) NULL,
	[BANK_CODE] [char](7) NULL,
	[BANK_NAME] [char](40) NULL,
	[ACCOUNT_CODE] [char](1) NULL,
	[ACCOUNT_CODE2] [char](1) NULL,
	[PURPOSE_CODE] [char](1) NULL,
	[CONTRACT_AMT] [int] NULL,
	[LOAN_AMT] [int] NULL,
	[PASS_DUE_AMT] [int] NULL,
	[PAY_CODE_12] [char](12) NULL,
	[CO_LOAN] [char](1) NULL,
	[INPUT_TIME] [char](12) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[BRANCH]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[BRANCH](
	[BRANCH_CODE] [varchar](4) NULL,
	[BRANCH] [varchar](60) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[CFC]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[CFC](
	[CFC] [varchar](4) NULL,
	[CFC_CODE] [varchar](60) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[CFC_BRANCH]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[CFC_BRANCH](
	[CFC_CODE] [nvarchar](4) NULL,
	[BRANCH_CODE] [nvarchar](4) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[CHILDREN_LOOKUP]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[CHILDREN_LOOKUP](
	[CODE] [int] NULL,
	[DES] [varchar](8) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[CREDIT_BLOCK_LIST]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[CREDIT_BLOCK_LIST](
	[IDN] [char](11) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[EDUCATION_LOOKUP]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[EDUCATION_LOOKUP](
	[CODE] [int] NULL,
	[DES] [varchar](4) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[JAS002]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[JAS002](
	[IDN] [char](11) NULL,
	[INQUIRY_DATE] [char](8) NULL,
	[EVER_DELINQUENT] [char](1) NULL,
	[DELINQUENT_DATE] [char](7) NULL,
	[EVER_BAD_CHECK] [char](1) NULL,
	[BAD_CHECK_DATE] [char](7) NULL,
	[EVER_REJECT] [char](1) NULL,
	[REJECT_DATE] [char](7) NULL,
	[EVER_STOP_CARD] [char](1) NULL,
	[STOP_CARD_DATE] [char](7) NULL,
	[INPUT_TIME] [char](12) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[KRM021]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[KRM021](
	[IDN] [char](11) NULL,
	[INQUIRY_DATE] [char](8) NULL,
	[CARD_BRAND] [char](1) NULL,
	[CARD_TYPE] [char](1) NULL,
	[ISSUE] [char](3) NULL,
	[ISSUE_NAME] [char](40) NULL,
	[START_DATE] [char](7) NULL,
	[STOP_DATE] [char](7) NULL,
	[STOP_CODE] [char](1) NULL,
	[AB_CODE] [char](1) NULL,
	[M_S] [char](1) NULL,
	[LIMIT] [char](6) NULL,
	[INPUT_TIME] [char](12) NULL,
	[RELA] [char](1) NULL,
	[RISK] [char](6) NULL,
	[CLEAR_DATE] [char](7) NULL,
	[IDN_PRI] [char](10) NULL,
	[CNAME] [char](12) NULL,
	[REMARK] [char](40) NULL
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[KRM023]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[KRM023](
	[IDN] [char](11) NULL,
	[INQUIRY_DATE] [char](8) NULL,
	[YRMON] [char](5) NULL,
	[ISSUE] [char](3) NULL,
	[ISSUE_NAME] [char](40) NULL,
	[KR_CODE] [char](7) NULL,
	[LIMIT] [char](5) NULL,
	[PAYMENT] [char](3) NULL,
	[CASH] [char](1) NULL,
	[PAY_CODE] [char](1) NULL,
	[INPUT_TIME] [char](12) NULL
) ON [PRIMARY]
END
