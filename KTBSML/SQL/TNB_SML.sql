USE KTB_SML;
GO

CREATE TABLE BAM086 (
	MSN		    CHAR(14) NOT NULL, -- '�ӽнs��';                                                                      
	INQUIRY_DATE	    CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyyymmdd, yyyy���褸�~)';                                     
	IDN		    CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';                                                                
	DATA_YYY            CHAR(3),           -- '��Ʀ~��';                                                                      
	DATA_MM             CHAR(2),           -- '��Ƥ��';                                                                      
	BANK_CODE           CHAR(7),           -- '��w�N��';                                                                      
	BANK_NAME           CHAR(40),          -- '��w�W��';                                                                      
	ACCOUNT_CODE        CHAR(1),           -- '�|�p���  (��Ӫ�)';                                                            
	ACCOUNT_CODE2       CHAR(1),           -- '�|�p��ص��O  S ��ܦ���O';                                                    
	PURPOSE_CODE        CHAR(1),           -- '�γ~�O  (��Ӫ�)';                                                              
	CONTRACT_AMT        INT,               -- '�q�����B (�d��)';                                                               
	LOAN_AMT            INT,               -- '��ڥ��O�� (�d��)';                                                             
	PASS_DUE_AMT        INT,               -- '�O�����٪��B (�d��)';                                                           
	PAY_CODE_12         CHAR(12),          -- '�̪�12�Ӥ��ٴڬ���  (��Ӫ�)';                                                  
	CO_LOAN             CHAR(1),           -- '�@�P�ɴڵ��O  * ��ܦ��@�P�ɴ�';                                                
--	UN_MARK             CHAR(1),           -- '����p�U���O  * ��ܬ�����p�U';                                                
--	U_YYYMMDD           CHAR(8),           -- '�p�U���  �褸YYYYMMDD';                                                        
--	U_RATE              CHAR(3),           -- '�ѶU���';                                                                      
--	IB_MARK             CHAR(1),           -- '����y�V���O  * ��ܬ�����y�V�D�k�H��´';                                      
--	IAB_BAN             CHAR(8),           -- '�D�k�H��´�νs';                                                                
--	IAB_NAME            CHAR(60),          -- '�D�k�H��´�W��';                                                                
--	R_YYYMMDD           CHAR(7),           -- '�ٴڤ��';                                                                      
--	REFUND_AMT          CHAR(10),          -- '�ٴڪ��B (�d��)';                                                               
--	CK_REF              CHAR(1)            -- '�ٴڰO��  Y:�����ٲM N:�����ٲM 1.�ٲM 2.�M�v�n���� 3.�M�v�n��Q�� 4.�ॿ�`'; 
);
CREATE INDEX I_BAM086 ON BAM086(MSN);
GO

CREATE TABLE JAS002 (
	MSN		CHAR(14) NOT NULL, -- '�ӽнs��';                                                                       
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyyymmdd, yyyy���褸�~)';                                      
	IDN		CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';                                                                 
	EVER_DELINQUENT CHAR(1),           -- '�«H���` YN';                                                                    
	DELINQUENT_DATE CHAR(7),           -- '�̪�«H���`���';                                                               
	EVER_BAD_CHECK  CHAR(1),           -- '�h�����` YN';                                                                    
	BAD_CHECK_DATE  CHAR(7),           -- '�̪�h�����`���';                                                               
	EVER_REJECT     CHAR(1),           -- '�ڵ����ӰO�� YN';                                                                
	REJECT_DATE     CHAR(7),           -- '�ڵ����Ӥ��  �ڵ����ӰO��N����즳������Ӥ�ک��w�Ѱ�����������ڵ����Ӥ��';
	EVER_STOP_CARD  CHAR(1),           -- '�j��d���` YN';                                                                
	STOP_CARD_DATE  CHAR(7)            -- '�̪�j��d���';                                                               
);                                                                                                          
CREATE INDEX I_JAS002 ON JAS002(MSN);
GO

CREATE TABLE KRM021 (	
	MSN		CHAR(14) NOT NULL, -- '�ӽнs��';                                   
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyyymmdd, yyyy���褸�~)';   
	IDN		CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';                              
        CARD_BRAND      Char(1),           --    �d�W�N��  (��Ӫ�)
        CARD_TYPE       Char(1),           --    �г��N��  (��Ӫ�)
        ISSUE           Char(3),           --    �o�d���c�N��
        ISSUE_NAME      Char(40),          --   �o�d���c�W��
        START_DATE      Char(7),           --    �ҥΤ��
        STOP_DATE       Char(7),           --    ���Τ��
        STOP_CODE       Char(1),           --    ���κ����N��  (��Ӫ�)
        AB_CODE         Char(1),           --    ���έ�]�N��  (��Ӫ�)
        M_S             Char(1),           --    �D���d(Y/N)
        RELA            Char(1),           --    �e�Ȫ��d�f�D���d���Y�N��  (��Ӫ�)
        LIMIT           Char(6),           --    �B�ס]�d���^
        RISK            Char(6),           --    �e�Ȱ��έ�]�עޡf���I�l�����B�]�d���^
        CLEAR_DATE      Char(7),           --    �e�Ȱ��έ�]�ע�f��ڵ��M���
        IDN_PRI         Char(10),          --   �e�Ȫ��d�f�D�d�H�����Ҹ�
        CNAME           Char(12),          --   �e�Ȫ��d�f�D�d�H�m�W
        REMARK          Char(40)           --   ���O
);
CREATE INDEX I_KRM021 ON KRM021(MSN);
GO

CREATE TABLE KRM023 (
	MSN		CHAR(14) NOT NULL, -- '�ӽнs��';                                        
	INQUIRY_DATE	CHAR(10),          -- 'JCIC ��Ƭd�ߤ��(yyyymmdd, yyyy���褸�~)';       
	IDN		CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';                                  
	YRMON 		CHAR (5),          -- '��Ʀ~��';                                        
	ISSUE 		CHAR (3),          -- '�o�d���c�N��';                                    
	ISSUE_NAME 	CHAR (40),         -- '�o�d���c�W��';                                    
	KR_CODE 	CHAR (7),          -- '�H�Υd�O';                                        
	LIMIT 		CHAR (5),          -- '�֩w�B�� (�d��)';                                 
	PAYMENT 	CHAR (3),          -- '��ú���B�ŶZ(��)(���+ +HML)';                    
	CASH 		CHAR (1),          -- '�w�ɲ{�����L(YN)';                                
	PAY_CODE 	CHAR (1)           -- 'ú�ڪ��p�N��  (��Ӫ�)';                          
);
CREATE INDEX I_KRM023 ON KRM023(MSN);
GO

CREATE TABLE KRM037 (
	MSN		CHAR(14) NOT NULL, -- '�ӽнs��';                                                                                                                                                                                                 
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyyymmdd, yyyy���褸�~)';                                                                                                                                                                
	IDN		CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';                                                                                                                                                                                           
	BILL_DATE	CHAR(7),           -- '���b��';                                                                                                                                                                                                   
	ISSUE		CHAR(3),           -- '�o�d���c�N��';                                                                                                                                                                                             
	ISSUE_NAME 	CHAR(24),          -- '�o�d���c�W��';                                                                                                                                                                                             
	CARD_TYPE 	CHAR(7),           -- '�H�Υd�O';                                                                                                                                                                                                 
	PERM_LIMIT 	INT,               -- '�ä[�B��  ���:�d��';                                                                                                                                                                                      
	TEMP_LIMIT 	INT,               -- '�{���B��  ���:�d��';                                                                                                                                                                                      
	CASH_LIMIT 	INT,               -- '�w�ɲ{���B��  ���:�d��';                                                                                                                                                                                  
	PAYABLE 	CHAR(3),           -- '�������I�b��(�ŶZ)';                                                                                                                                                                                       
	CASH_LENT 	INT,               -- '�����w�ɲ{��  ���:��';                                                                                                                                                                                    
	LAST_PAYA 	CHAR(3),           -- '�W�����I�b��(�ŶZ)';                                                                                                                                                                                       
	REVOL_BAL 	INT,               -- '�W���`���H��  ���:��';                                                                                                                                                                                    
	PAY_STAT 	CHAR(1),           -- '�W��ú�ڪ��p�N��(���B)  (��Ӫ�)';                                                                                                                                                                         
	PAY_CODE 	CHAR(1),           -- '�W��ú�ڪ��p�N��(�ɶ�)  (��Ӫ�)';                                                                                                                                                                         
	REVOL_RATE 	INT,               -- '�W���`����vREVOL_BAL/PERM_LIMIT,��T�צܤp���I��2��(�|�ˤ��J),�D�ʤ���';                                                                                                                                  
	PRE_OWED 	INT,               -- '����������v�٫ݥI���B,������N�Ԯ��O�ڪ��B(�榡22�涵)+����������v�ٹw�ɲ{�����B(�榡23�涵),���]�A�����s�W�l�B�N�v(18��)';                                                                              
	DEBT_CODE 	CHAR(1),           -- '���v���A���O';                                                                                                                                                                                             
	CLOSE_CODE 	CHAR(1),           -- '���v���׵��O(��Ӫ�)';                                                                                                                                                                                     
	CLEAR_DATE 	CHAR(7)            -- '���}���v���פ��';                                                                                                                                                                                         
);
CREATE INDEX I_KRM037 ON KRM037(MSN);
GO

CREATE TABLE STM007 (
	MSN		CHAR(14) NOT NULL, -- '�ӽнs��';                                                             
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyyymmdd, yyyy���褸�~)';                            
	IDN		CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';                                                       
	QUERY_DATE 	CHAR(7),           -- '�d�ߤ��';                                                             
 	BANK_CODE 	CHAR(7),           -- '�d�߳��N��';                                                         
	BANK_NAME 	CHAR(40),          -- '�d�߳��W��';                                                         
	ITEM_LIST 	CHAR(10),          -- '�d�߶��ئ�C B:�«H D:���H K:�H�Υd��';                                
	INQ_PURPOSE_1	CHAR(1),           -- '�d�߲z�ѽX';                                                           
	INQ_PURPOSE	CHAR(30)           -- '�d�߲z�ѽX(�������)1:�s�~�ȥӽ�2:��~�ȩ���3:�s�~�ȥӽФέ�~�ȩ���'; 

);
CREATE INDEX I_STM007 ON STM007(MSN);
GO

CREATE TABLE VAM102 (
	MSN char (14) NOT NULL,
	IDN char (11),
	INQUIRY_DATE char (8),
	DATA_DATE char (7),
	MAINCODE char (1),
	MAINNOTE char (36),
	SUBCODE char (1),
	SUBNOTE char (60),
	NOTE char (256)
) ON PRIMARY
CREATE INDEX I_VAM102 ON VAM102(MSN);
GO

CREATE TABLE APP_INFO (
	MSN		CHAR(14) NOT NULL,     -- �ץ�s��
	SYSTEM_DATE	CHAR(14) NOT NULL,     -- �ץ��J�ɶ� YYYYMMDDHHMMSS
	APPLICANT_ID	CHAR(10) NOT NULL,     -- �ӽФH�����Ҧr���m�W  
	APPLICANT_NAME	VARCHAR(30) NOT NULL,  -- �ӽФH�m�W
	ZIP		CHAR(3),               -- �T��l���ϸ�
	INCOME		INT NOT NULL,          -- �~���J
	APP_QUALIFIED	INT,		       -- �ӽФH�ŦX�ӧ@���� 0: NO / 1:YES
	APP_AMT		INT NOT NULL,          -- �ӶU���B 
	PERIOD		INT NOT NULL,          -- �ӶU����
	APR		DECIMAL(8,5) NOT NULL, -- �ӶU�Q�v
	APP_FEE		INT NOT NULL,          -- �}��O
	BRANCH		VARCHAR(10),           -- �i�����
	EMP_ID		VARCHAR(10),           -- �i����u
	AUDITOR		VARCHAR(10),	       -- �x�f�H��
-- PROPERTY1
	OWNER_ID1	CHAR(10) NOT NULL,     -- �Ҧ��H�����Ҧr��
	OWNER_NAME1	VARCHAR(30) NOT NULL,  -- �Ҧ��H�m�W
	LAND_NUM1	VARCHAR(60) NOT NULL,  -- ��O�~���P���X
	FIRST_LIEN1	INT,           -- �w����`���B                                                  
	RELATIONSHIP1	INT, 	       -- �P�ӽФH���Y 0:���H / 1:���� / 2;�t�� / 3:�l�k / 4:��L     
	NAV1		FLOAT,         -- Ų���b��
	GAV1		FLOAT,         -- Ų���`��
	QUALIFIED1	INT,           -- 0: NO / 1:YES                                  
-- PROPERTY2 
	OWNER_ID2	CHAR(10),      -- �Ҧ��H�����Ҧr�� 
	OWNER_NAME2	VARCHAR(30),   -- �Ҧ��H�m�W       
	LAND_NUM2	VARCHAR(60),   -- ��O�~���P���X        
	FIRST_LIEN2	INT,           -- �w����`���B                                                                 
	RELATIONSHIP2	INT,           -- �P�ӽФH���Y 0:���H / 1:���� / 2;�t�� / 3:�l�k / 4:��L                                   
	NAV2		FLOAT,         -- Ų���b��                                                                     
	GAV2		FLOAT,         -- Ų���`��                                                                     
	QUALIFIED2	INT,           -- 0: NO / 1:YES                                  
	INQUIRY_DATE	CHAR(8)        -- JCIC �d�ߤ�� YYYYMMDD
);
ALTER TABLE APP_INFO ADD CONSTRAINT P_APP_INFO PRIMARY KEY (MSN);
GO

-- PREMIER REVIEW OUTPUT ��f
CREATE TABLE APP_PREMIER (
	MSN		CHAR(14) NOT NULL,  -- �ץ�s��
	PREMIER_DATE	CHAR(14),           -- ��f���YYYYMMDDHHMMSS
        RSCORE		DECIMAL(8,5),       -- PDACO score
	PREMIER_CODE	INT,                -- ��f���G�N�X      
	PREMIER_MSG	VARCHAR(256),       -- ��f���G�T�� 
);
ALTER TABLE APP_PREMIER ADD CONSTRAINT P_APP_PREMIER PRIMARY KEY (MSN);
GO

-- FINAL REVIEW OUTPUT  �Ƽf
CREATE TABLE APP_FINAL (
	MSN		CHAR(14) NOT NULL,-- �ץ�s��
	FINAL_DATE	CHAR(14),         -- �Ƽf��� YYYYMMDDHHMMSS
	PB		DECIMAL(8,5),     -- PB
	NPV		INT,              -- NPV
	OPTIMAL_AMOUNT	INT,              -- �̰��i�U���B
--	AUDIT_AGENT	CHAR(4),          -- �Ƽf�H��
	FINAL_CODE	INT,              -- �Ƽf���G�N�X 
	FINAL_MSG	VARCHAR(256)      -- �Ƽf���G�T��
);
ALTER TABLE APP_FINAL ADD CONSTRAINT P_APP_FINAL PRIMARY KEY (MSN);
GO

CREATE TABLE COUNTER ( 
	SEQ_TYPE	CHAR(9) NOT NULL,               -- '�Ǹ�����, Ayyyymmdd: A20060412';    
	SEQ_NO		NUMERIC(5) NOT NULL DEFAULT 1   -- '�Ǹ���';                            
);
ALTER TABLE COUNTER ADD CONSTRAINT P_COUNTER PRIMARY KEY (SEQ_TYPE);
GO

CREATE TABLE AGENT ( 
	USERID		VARCHAR(32) NOT NULL,             -- 'USER ID';
	USERPW		VARCHAR(32),                      -- 'USER PASSWORD';         
	USERNAME	VARCHAR(32),                      -- 'USER NAME';
	EMP_ID		VARCHAR(32),                      -- 'USER EMPLOYEE ID';         
	USERBRANCH	CHAR(4)                           -- BRANCH CODE
);
ALTER TABLE AGENT ADD CONSTRAINT P_AGENT PRIMARY KEY (USERID);

INSERT INTO AGENT VALUES ('test', 'test', 'test', '99999', '0489');
GO

CREATE TABLE CFC (
	CFC_CODE	 char(4) NOT NULL,  -- �ϰ줤�ߥN�X
	CFC		 varchar(60),       -- �ϰ줤�ߦW��
);
ALTER TABLE CFC ADD CONSTRAINT P_CFC PRIMARY KEY (CFC_CODE);
GO

CREATE TABLE BRANCH (
	BRANCH_CODE	 char(4) NOT NULL,  -- ����N�X
	BRANCH		 varchar(60),       -- ����W��
	CFC_CODE	 char(4)            -- �ϰ줤�ߥN�X
);
ALTER TABLE BRANCH ADD CONSTRAINT P_BRANCH PRIMARY KEY (BRANCH_CODE);
GO

-- Working tables to generate daily reports.

CREATE TABLE PRELIMITARY_REPORT (
	MSN CHAR (14) NOT NULL,
	PREMIER_DATE CHAR (14),
	RSCORE DECIMAL(7, 4),
	PREMIER_CODE INT,
	PREMIER_MSG VARCHAR (256),
	SYSTEM_DATE CHAR (14),
	APPLICANT_ID CHAR (10),
	APPLICANT_NAME VARCHAR (30),
	INCOME INT,
	APP_AMT INT,
	PERIOD INT,
	APR DECIMAL(7, 4),
	APP_FEE INT,
	BRANCH CHAR (4),
	BRANCH_NAME VARCHAR (20),
	REGION_NAME VARCHAR (20),
	AUDITOR CHAR (10),
	OWNER_ID1 CHAR (10),
	OWNER_NAME1 VARCHAR (30),
	LAND_NUM1 VARCHAR (60),
	RELATIONSHIP1 INT,
	OWNER_ID2 CHAR (10),
	OWNER_NAME2 VARCHAR (30),
	LAND_NUM2 VARCHAR (60),
	RELATIONSHIP2 INT,
	ZIP CHAR (3),
	INQUIRY_DATE CHAR (8)
);
ALTER TABLE PRELIMITARY_REPORT ADD CONSTRAINT P_PRELIMITARY_REPORT PRIMARY KEY (MSN);
GO

CREATE TABLE FINAL_REPORT (
	MSN CHAR (14) NOT NULL,
	FINAL_DATE CHAR (14),
	PB DECIMAL(7, 4),
	NPV INT,
	OPTIMAL_AMOUNT INT,
	FINAL_CODE INT,
	FINAL_MSG VARCHAR (256),
	SYSTEM_DATE CHAR (14),
	APPLICANT_ID CHAR (10),
	APPLICANT_NAME VARCHAR (30),
	INCOME INT,
	APP_AMT INT,
	PERIOD INT,
	APR DECIMAL(7, 4),
	APP_FEE INT,
	BRANCH CHAR (4),
	BRANCH_NAME VARCHAR (20),
	REGION_NAME VARCHAR (20),
	AUDITOR CHAR (10),
	OWNER_ID1 CHAR (10),
	OWNER_NAME1 VARCHAR (30),
	LAND_NUM1 VARCHAR (60),
	FIRST_LIEN1 INT,
	RELATIONSHIP1 INT,
	NAV1 FLOAT,
	GAV1 FLOAT,
	QUALIFIED1 INT,
	OWNER_ID2 CHAR (10),
	OWNER_NAME2 VARCHAR (30),
	LAND_NUM2 VARCHAR (60),
	FIRST_LIEN2 INT,
	RELATIONSHIP2 INT,
	NAV2 FLOAT,
	GAV2 FLOAT,
	QUALIFIED2 INT,
	ZIP CHAR (3),
	INQUIRY_DATE CHAR (8)
);
ALTER TABLE FINAL_REPORT ADD CONSTRAINT P_FINAL_REPORT PRIMARY KEY (MSN);
GO

CREATE TABLE CREDIT_BLOCK_LIST (
	IDN char (11) NOT NULL ,
	MSN char (14) 
);
ALTER TABLE CREDIT_BLOCK_LIST ADD CONSTRAINT P_CREDIT_BLOCK_LIST PRIMARY KEY (IDN);
GO



