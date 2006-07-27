USE PLOAN_MKT
GO

ALTER TABLE APPLICANT ALTER COLUMN PRODUCT_CODE VARCHAR(5)
GO
ALTER TABLE MAIL_LIST ADD PRODUCT_CODE VARCHAR(5)
GO
ALTER TABLE MAIL_LIST ADD DM_CELL VARCHAR(2), DM_BATCH VARCHAR(2), MAIL_DATE VARCHAR(8)
GO
ALTER TABLE PRODUCT_LOOKUP ALTER COLUMN CODE VARCHAR(5)
GO

UPDATE MAIL_LIST
SET
ADDRESS = REPLACE(ADDRESS, ',', '')
GO
