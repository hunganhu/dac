--The following code add new columns to result table to accomodate for new outputs
ALTER TABLE dbo.RESULT ADD RISK_GROUP INT, PROPENSITY_GROUP INT
GO

--The following codes change the constraints for MIS related columns so that NULL can be permitted
ALTER TABLE dbo.APP_INFO ALTER COLUMN MARTIAL_STATUS CHAR(1) NULL
ALTER TABLE dbo.APP_INFO ALTER COLUMN TENURE INT NULL
ALTER TABLE dbo.APP_INFO ALTER COLUMN JOB_TITLE VARCHAR(32) NULL
ALTER TABLE dbo.APP_INFO ALTER COLUMN COMPANY_NAME VARCHAR(48) NULL
ALTER TABLE dbo.APP_INFO ALTER COLUMN EDUCATION CHAR(1) NULL
ALTER TABLE dbo.APP_INFO ALTER COLUMN DOB CHAR(8) NULL
ALTER TABLE dbo.APP_INFO ALTER COLUMN INCOME INT NULL
ALTER TABLE dbo.APP_INFO ALTER COLUMN CHANNEL CHAR(12) NULL
ALTER TABLE dbo.APP_INFO ALTER COLUMN ZIP CHAR(3) NULL