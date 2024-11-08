USE TNB_PLOAN_TEST
GO

SELECT TOP 1 * INTO KRM021 FROM SYSCOM_BOOC_TEST.DBO.KRM021
SELECT TOP 1 * INTO KRM001 FROM BOOC.DBO.KRM001
SELECT TOP 1 * INTO KRM023 FROM BOOC.DBO.KRM023 
SELECT TOP 1 * INTO BAM086 FROM BOOC.DBO.BAM086 
--SELECT TOP 1 * INTO BAM087 FROM SYSCOM_BOOC_TEST.DBO.BAM087
SELECT TOP 1 * INTO STM007 FROM SYSCOM_BOOC_TEST.DBO.STM007 
SELECT TOP 1 * INTO JAS002 FROM BOOC.DBO.JAS002 
GO

DELETE FROM KRM021
DELETE FROM KRM001
DELETE FROM KRM023
DELETE FROM BAM086
--DELETE FROM BAM087
DELETE FROM STM007
DELETE FROM JAS002
GO

ALTER TABLE KRM021 ADD INPUT_TIME CHAR(12)
ALTER TABLE KRM021 ALTER COLUMN INQUIRY_DATE CHAR(8)
GO
ALTER TABLE KRM023 ADD INPUT_TIME CHAR(12)
ALTER TABLE KRM023 ALTER COLUMN INQUIRY_DATE CHAR(8)
GO
ALTER TABLE STM007 ADD INPUT_TIME CHAR(12)
ALTER TABLE STM007 ALTER COLUMN INQUIRY_DATE CHAR(8)
GO
ALTER TABLE BAM086 ADD INPUT_TIME CHAR(12)
ALTER TABLE BAM086 ALTER COLUMN INQUIRY_DATE CHAR(8)
GO
ALTER TABLE JAS002 ADD INPUT_TIME CHAR(12)
ALTER TABLE JAS002 ALTER COLUMN INQUIRY_DATE CHAR(8)
GO

CREATE TABLE APPLICANT
(IDN CHAR(11), INPUT_TIME CHAR(12), CNAME CHAR(20), BIRTHDAY CHAR(8), PHONE CHAR(10), ZIP CHAR(3),OCCUPATION CHAR(4), JOB CHAR(4), ON_BOARD_YM CHAR(6), APPLICATION_AMOUNT INT, APPLICATION_RATE DECIMAL(5,4), APPLICATION_TERMS INT, CHANNEL CHAR(20), APPLICATION_FEE INT)
GO


--Suggestion Code: 1: Module approved "模組建議核准" ; 2: Module declined "模組建議婉拒"; 3: Module manual "模組建議人工審核"; 

CREATE TABLE RESULT
(IDN CHAR(11), INPUT_TIME CHAR(12), PB FLOAT, MIN_RATE DECIMAL(5,4), NPV FLOAT, SUGGESTION CHAR(16), SUGGESTION_CODE INT, APPROVED_AMOUNT INT, Result_File_GEN_Time Char(12))
GO

CAEATE TABLE APPROVAL_DECISION
(IDN CHAR(11), INPUT_TIME CHAR(12), DECISION_INPUT_TIME CHAR(12), DECISION INT, APPROVED_AMOUNT INT, APPROVED_RATE DECIMAL(5,4), APPROVED_TERMS INT, DECLINE_REASON_CODE INT)


