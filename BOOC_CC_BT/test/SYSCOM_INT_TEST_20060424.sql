exec sp_addlinkedserver 'NBCOMPUTER'

SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.APP_INFO

USE BOOC_INT_TEST_20060424
GO

SELECT * INTO APP_INFO FROM NBCOMPUTER.BOOC_INQUIRY.dbo.APP_INFO
SELECT * INTO APP_R FROM NBCOMPUTER.BOOC_INQUIRY.dbo.APP_R
SELECT * INTO RESULT FROM NBCOMPUTER.BOOC_INQUIRY.dbo.RESULT
SELECT * INTO BAM087 FROM NBCOMPUTER.BOOC_INQUIRY.dbo.BAM087
SELECT * INTO KRM021 FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM021
SELECT * INTO STM007 FROM NBCOMPUTER.BOOC_INQUIRY.dbo.STM007
SELECT * INTO KRM023 FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM023
SELECT * INTO KRM037 FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM037
SELECT * INTO JAS002 FROM NBCOMPUTER.BOOC_INQUIRY.dbo.JAS002

DELETE FROM RESULT
--Normal case no 1
DECLARE @CASE CHAR(4)
DECLARE @CASE_ORI CHAR(11)
SET @CASE = 'C001'
SET @CASE_ORI = '9504130007  ' 
UPDATE APP_INFO SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE APP_R SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE BAM087 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE JAS002 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM021 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM023 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM037 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE STM007 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI

--Normal case no 2
DECLARE @CASE CHAR(4)
DECLARE @CASE_ORI CHAR(11)
SET @CASE = 'C002'
SET @CASE_ORI = '9504040069  '
UPDATE APP_INFO SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE APP_R SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE BAM087 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE JAS002 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM021 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM023 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM037 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE STM007 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI

--Reset cash card util to 50%
DECLARE @CASE_ORI CHAR(11)
SET @CASE_ORI = '9504130007  '
INSERT INTO APP_INFO SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.APP_INFO WHERE CASE_SN = @CASE_ORI
INSERT INTO APP_R SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.APP_R WHERE CASE_SN = @CASE_ORI
INSERT INTO RESULT SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.RESULT WHERE CASE_SN = @CASE_ORI
INSERT INTO BAM087 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.BAM087 WHERE CASE_SN = @CASE_ORI
INSERT INTO KRM021 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM021 WHERE CASE_SN = @CASE_ORI
INSERT INTO STM007 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.STM007 WHERE CASE_SN = @CASE_ORI
INSERT INTO KRM023 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM023 WHERE CASE_SN = @CASE_ORI
INSERT INTO KRM037 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM037 WHERE CASE_SN = @CASE_ORI
INSERT INTO JAS002 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.JAS002 WHERE CASE_SN = @CASE_ORI

DECLARE @CASE CHAR(4)
DECLARE @CASE_ORI CHAR(11)
SET @CASE = 'C003'
SET @CASE_ORI = '9504130007  '
UPDATE APP_INFO SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE APP_R SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE BAM087 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE JAS002 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM021 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM023 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM037 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE STM007 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI

UPDATE APP_INFO SET CASH_CARD_UTIL_CAP = 0.5 WHERE CASE_SN = 'C003'
SELECT * FROM BAM087 WHERE CASE_SN = 'C003'
UPDATE BAM087 SET LOAN_AMT = 0.7 * CONTRACT_AMT1 WHERE CASE_SN = 'C003'

--Reset credit card and cash card balance to NT$100,000
DECLARE @CASE_ORI CHAR(11)
SET @CASE_ORI = '9504040069  '
INSERT INTO APP_INFO SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.APP_INFO WHERE CASE_SN = @CASE_ORI
INSERT INTO APP_R SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.APP_R WHERE CASE_SN = @CASE_ORI
INSERT INTO RESULT SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.RESULT WHERE CASE_SN = @CASE_ORI
INSERT INTO BAM087 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.BAM087 WHERE CASE_SN = @CASE_ORI
INSERT INTO KRM021 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM021 WHERE CASE_SN = @CASE_ORI
INSERT INTO STM007 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.STM007 WHERE CASE_SN = @CASE_ORI
INSERT INTO KRM023 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM023 WHERE CASE_SN = @CASE_ORI
INSERT INTO KRM037 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM037 WHERE CASE_SN = @CASE_ORI
INSERT INTO JAS002 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.JAS002 WHERE CASE_SN = @CASE_ORI

DECLARE @CASE CHAR(4)
DECLARE @CASE_ORI CHAR(11)
SET @CASE = 'C004'
SET @CASE_ORI = '9504040069  '
UPDATE APP_INFO SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE APP_R SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE BAM087 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE JAS002 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM021 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM023 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM037 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE STM007 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI

UPDATE APP_INFO SET CC_BALANCE_CAP = 100000 WHERE CASE_SN = 'C004'


--Reset credit card utilization cap to 30%
DECLARE @CASE_ORI CHAR(11)
SET @CASE_ORI = '9504040069  '
INSERT INTO APP_INFO SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.APP_INFO WHERE CASE_SN = @CASE_ORI
INSERT INTO APP_R SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.APP_R WHERE CASE_SN = @CASE_ORI
INSERT INTO RESULT SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.RESULT WHERE CASE_SN = @CASE_ORI
INSERT INTO BAM087 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.BAM087 WHERE CASE_SN = @CASE_ORI
INSERT INTO KRM021 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM021 WHERE CASE_SN = @CASE_ORI
INSERT INTO STM007 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.STM007 WHERE CASE_SN = @CASE_ORI
INSERT INTO KRM023 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM023 WHERE CASE_SN = @CASE_ORI
INSERT INTO KRM037 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.KRM037 WHERE CASE_SN = @CASE_ORI
INSERT INTO JAS002 SELECT * FROM NBCOMPUTER.BOOC_INQUIRY.dbo.JAS002 WHERE CASE_SN = @CASE_ORI

DECLARE @CASE CHAR(4)
DECLARE @CASE_ORI CHAR(11)
SET @CASE = 'C005'
SET @CASE_ORI = '9504040069  '
UPDATE APP_INFO SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE APP_R SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE BAM087 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE JAS002 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM021 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM023 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE KRM037 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI
UPDATE STM007 SET CASE_SN = @CASE, IDN = @CASE WHERE CASE_SN = @CASE_ORI

UPDATE APP_INFO SET CREDIT_CARD_UTIL_CAP = 0.2 WHERE CASE_SN = 'C005'


SELECT * FROM RESULT


SELECT * INTO DRIVER FROM NBCOMPUTER.BOOC_INQUIRY.dbo.DRIVER

DELETE FROM DRIVER

INSERT INTO DRIVER
SELECT IDN, GENDER, APPLICATION_DATE, CASE_SN
FROM APP_INFO