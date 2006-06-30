USE KTB_FM;
GO

CREATE TABLE BAM086 (
	CASE_NO		    CHAR(14) NOT NULL, -- '�ӽнs��';
	INQUIRY_DATE	    CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	IDN		    CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';
	DATA_YYY            CHAR(3),           -- '��Ʀ~��';
	DATA_MM             CHAR(2),           -- '��Ƥ��';
	BANK_CODE           CHAR(7),           -- '��w�N��';
	BANK_NAME           CHAR(40),          -- '��w�W��';
	ACCOUNT_CODE        CHAR(1),           -- '�|�p���  (��Ӫ�)';
	ACCOUNT_CODE2       CHAR(1),           -- '�|�p��ص��O  S ��ܦ���O';
	PURPOSE_CODE        CHAR(1),           -- '�γ~�O  (��Ӫ�)';
	CONTRACT_AMT        CHAR(10),          -- '�q�����B (�d��)';
	LOAN_AMT            CHAR(10),          -- '��ڥ��O�� (�d��)';
	PASS_DUE_AMT        CHAR(10),          -- '�O�����٪��B (�d��)';
	PAY_CODE_12         CHAR(12),          -- '�̪�12�Ӥ��ٴڬ���  (��Ӫ�)';
	CO_LOAN             CHAR(1),           -- '�@�P�ɴڵ��O  * ��ܦ��@�P�ɴ�';
	UN_MARK             CHAR(1),           -- '����p�U���O  * ��ܬ�����p�U';
	U_YYYMMDD           CHAR(8),           -- '�p�U���  �褸YYYYMMDD';
	U_RATE              CHAR(3),           -- '�ѶU���';
	IB_MARK             CHAR(1),           -- '����y�V���O  * ��ܬ�����y�V�D�k�H��´';
	IAB_BAN             CHAR(8),           -- '�D�k�H��´�νs';
	IAB_NAME            CHAR(60),          -- '�D�k�H��´�W��';
	R_YYYMMDD           CHAR(7),           -- '�ٴڤ��';
	REFUND_AMT          CHAR(10),          -- '�ٴڪ��B (�d��)';
	CK_REF              CHAR(1)            -- '�ٴڰO��  Y:�����ٲM N:�����ٲM 1.�ٲM 2.�M�v�n���� 3.�M�v�n��Q�� 4.�ॿ�`';
);
CREATE INDEX I_BAM086 ON BAM086(CASE_NO, IDN);
GO

CREATE TABLE JAS002 (
	CASE_NO		CHAR(14) NOT NULL, -- '�ӽнs��';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
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
CREATE INDEX I_JAS002 ON JAS002(CASE_NO, IDN);
GO

CREATE TABLE KRM021 (
	CASE_NO		CHAR(14) NOT NULL, -- '�ӽнs��';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	IDN		CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';
        CARD_BRAND      Char(1),           --  �d�W�N��  (��Ӫ�)
        CARD_TYPE       Char(1),           --  �г��N��  (��Ӫ�)
        ISSUE           Char(3),           --  �o�d���c�N��
        ISSUE_NAME      Char(40),          --  �o�d���c�W��
        START_DATE      Char(7),           --  �ҥΤ��
        STOP_DATE       Char(7),           --  ���Τ��
        STOP_CODE       Char(1),           --  ���κ����N��  (��Ӫ�)
        AB_CODE         Char(1),           --  ���έ�]�N��  (��Ӫ�)
        M_S             Char(1),           --  �D���d(Y/N)
        RELA            Char(1),           -- �e�Ȫ��d�f�D���d���Y�N��  (��Ӫ�)
        LIMIT           Char(6),           --  �B�ס]�d���^
        RISK            Char(6),           -- �e�Ȱ��έ�]�עޡf���I�l�����B�]�d���^
        CLEAR_DATE      Char(7),           -- �e�Ȱ��έ�]�ע�f��ڵ��M���
        IDN_PRI         Char(10),          -- �e�Ȫ��d�f�D�d�H�����Ҹ�
        CNAME           Char(12),          -- �e�Ȫ��d�f�D�d�H�m�W
        REMARK          Char(40)           --  ���O
);
CREATE INDEX I_KRM021 ON KRM021(CASE_NO, IDN);
GO

CREATE TABLE KRM023 (
	CASE_NO		CHAR(14) NOT NULL, -- '�ӽнs��';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
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
CREATE INDEX I_KRM023 ON KRM023(CASE_NO, IDN);
GO

CREATE TABLE KRM037 (
	CASE_NO		CHAR(14) NOT NULL, -- '�ӽнs��';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	IDN		CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';
	IDN_BAN		CHAR(10),          -- 'ú�ڤHID';
	BILL_DATE	CHAR(7),           -- '���b��';
	ISSUE		CHAR(3),           -- '�o�d���c�N��';
	ISSUE_NAME 	CHAR(24),          -- '�o�d���c�W��';
	CARD_TYPE 	CHAR(7),           -- '�H�Υd�O';
	PERM_LIMIT 	CHAR(7),           -- '�ä[�B��  ���:�d��';
	TEMP_LIMIT 	CHAR(7),           -- '�{���B��  ���:�d��';
	CASH_LIMIT 	CHAR(7),           -- '�w�ɲ{���B��  ���:�d��';
	PAYABLE 	CHAR(3),           -- '�������I�b��(�ŶZ)';
	CASH_LENT 	CHAR(8),           -- '�����w�ɲ{��  ���:��';
	LAST_PAYA 	CHAR(3),           -- '�W�����I�b��(�ŶZ)';
	REVOL_BAL 	CHAR(9),           -- '�W���`���H��  ���:��';
	PAY_STAT 	CHAR(1),           -- '�W��ú�ڪ��p�N��(���B)  (��Ӫ�)';
	PAY_CODE 	CHAR(1),           -- '�W��ú�ڪ��p�N��(�ɶ�)  (��Ӫ�)';
	REVOL_RATE 	CHAR(4),           -- '�W���`����vREVOL_BAL/PERM_LIMIT,��T�צܤp���I��2��(�|�ˤ��J),�D�ʤ���';
	PRE_OWED 	CHAR(8),           -- '����������v�٫ݥI���B,������N�Ԯ��O�ڪ��B(�榡22�涵)+����������v�ٹw�ɲ{�����B(�榡23�涵),���]�A�����s�W�l�B�N�v(18��)';
	DEBT_CODE 	CHAR(1),           -- '���v���A���O';
	CLOSE_CODE 	CHAR(1),           -- '���v���׵��O(��Ӫ�)';
	CLEAR_DATE 	CHAR(7)            -- '���}���v���פ��';
);
CREATE INDEX I_KRM037 ON KRM037(CASE_NO, IDN);
GO

CREATE TABLE STM007 (
	CASE_NO		CHAR(14) NOT NULL, -- '�ӽнs��';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	IDN		CHAR(10) NOT NULL, -- '�ӽФH�����Ҹ�';
	QUERY_DATE 	CHAR(7),           -- '�d�ߤ��';
 	BANK_CODE 	CHAR(7),           -- '�d�߳��N��';
	BANK_NAME 	CHAR(40),          -- '�d�߳��W��';
	ITEM_LIST 	CHAR(10),          -- '�d�߶��ئ�C B:�«H D:���H K:�H�Υd��';
	INQ_PURPOSE_1	CHAR(1),           -- '�d�߲z�ѽX';
	INQ_PURPOSE	CHAR(30)           -- '�d�߲z�ѽX(�������)1:�s�~�ȥӽ�2:��~�ȩ���3:�s�~�ȥӽФέ�~�ȩ���';

);
CREATE INDEX I_STM007 ON STM007(CASE_NO, IDN);
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
	USERBRANCH	VARCHAR(32)                       -- BRANCH NAME
);
ALTER TABLE AGENT ADD CONSTRAINT P_AGENT PRIMARY KEY (USERID);

INSERT INTO AGENT VALUES ('test', 'test', 'test', 'test');
GO

CREATE TABLE APP_INFO (
	CASE_NO		CHAR(14) NOT NULL,     -- �ץ�s��
	APP_DATE	CHAR(14) NOT NULL,     -- �ץ��J�ɶ� YYYYMMDDHHMMSS
	PROD_TYPE	INT NOT NULL,	       -- ���~���O 1:�@��жU, 2:�W�U, 3:��l�B��U
-- Applicant
	APP_ID		CHAR(10) NOT NULL,     -- �ӽФH�����Ҧr���m�W
	APP_NAME	VARCHAR(30) NOT NULL,  -- �ӽФH�m�W
	APP_BIRTHDAY	CHAR(7),	       -- �ӽФH�ͤ� YYYMMDD
	APP_MARRIAGE	INT,                   -- �ӽФH�B�� 0:���B / 1:�w�B / 2:���B
	APP_EDUCATION	INT,                   -- �ӽФH�Ǿ� 0:��s�� / 1:�j�� / 2:�M�� / 3:���B�ꤤ(�t�H�U)
	APP_INCOME	INT,		       -- �ӽФH�~���J
	APP_QUALIFIED	INT,		       -- �ӽФH�ŦX�ӧ@���� 0: NO / 1:YES
-- Co-signer
	COS_ID		CHAR(10),              -- �@�P�ɴڤH�����Ҧr���m�W
	COS_NAME	VARCHAR(30),           -- �@�P�ɴڤH�m�W
	COS_BIRTHDAY	CHAR(7),               -- �@�P�ɴڤH�ͤ� YYYMMDD
	COS_MARRIAGE	INT,                   -- �@�P�ɴڤH�B�� 0:���B / 1:�w�B / 2:���B
	COS_EDUCATION	INT,                   -- �@�P�ɴڤH�Ǿ� 0:��s�� / 1:�j�� / 2:�M�� / 3:���B�ꤤ(�t�H�U)
	COS_INCOME	INT,		       -- �@�P�ɴڤH�~���J
	COS_QUALIFIED	INT,		       -- �@�P�ɴڤH�ŦX�ӧ@���� 0: NO / 1:YES		
-- Guarantor
	GUA_ID		CHAR(10),              -- �O�ҤH�����Ҧr���m�W  
	GUA_NAME	VARCHAR(30),           -- �O�ҤH�m�W
	GUA_BIRTHDAY	CHAR(7),               -- �O�ҤH�ͤ� YYYMMDD
	GUA_MARRIAGE	INT,                   -- �O�ҤH�B�� 0:���B / 1:�w�B / 2:���B               
	GUA_EDUCATION	INT,                   -- �O�ҤH�Ǿ� 0:��s�� / 1:�j�� / 2:�M�� / 3:���B�ꤤ(�t�H�U)
	GUA_INCOME	INT,		       -- �O�ҤH�~���J
	GUA_QUALIFIED	INT,		       -- �O�ҤH�ŦX�ӧ@���� 0: NO / 1:YES
-- Loan Info
	APP_AMT		INT,		       -- �ӶU���B
	PERIOD		INT,		       -- �ӶU����
	APR1		DECIMAL(8,5),          -- �Ĥ@�q�Q�v
	SEG1		INT,		       -- �Ĥ@�q����
	APR2		DECIMAL(8,5),          -- �ĤG�q�Q�v
	SEG2		INT,		       -- �ĤG�q����
	APR3		DECIMAL(8,5),          -- �ĤT�q�Q�v
	SEG3		INT,		       -- �ĤT�q����
	GRACE_PERIOD	INT,                   -- �e����
	APP_FEE		INT,                   -- �}��O
-- Collateral
	OWNER_ID	CHAR(10),              -- �Ҧ��H�����Ҧr��
	OWNER_NAME	VARCHAR(30),           -- �Ҧ��H�m�W
	LAND_NUM	VARCHAR(30),	       -- �a��/�ظ�
	RELATIONSHIP	INT, 	               -- �P�ӽФH���Y 0:���H / 1:���� / 2;�t�� / 3:�l�k / 4:��L
	GAV		FLOAT,                 -- �ЫΤ��
	NAV		FLOAT,                 -- �Ыβb��
	COL_QUALIFIED	INT,                   -- ��O�~�ŦX�ӧ@���� 0: NO / 1:YES
	PREMIUM_COL	INT,                   -- ��O�~�Ϭq�}�n�B�f�t�u�}�س]���q 0: NO / 1:YES
        MONTHLY_PAYMENT INT,	               -- �ثe�жU��I��
--
	INQUIRY_DATE	CHAR(8),               -- JCIC �d�ߤ�� YYYYMMDD
	BRANCH		VARCHAR(10),           -- �i�����
	EMP_ID		VARCHAR(10),           -- �i����u
	AUDITOR		VARCHAR(10)	       -- �x�f�H��
);
ALTER TABLE APP_INFO ADD CONSTRAINT P_APP_INFO PRIMARY KEY (CASE_NO);
GO

--
CREATE TABLE APP_RESULT (
	CASE_NO		CHAR(14) NOT NULL,  -- �ץ�s��
	FINAL_DATE	CHAR(14),           -- �f�֤�� YYYYMMDDHHMMSS
	APP_RSCORE	DECIMAL(8,5),       -- �ӽФH PDACO SCORE
	APP_PB		DECIMAL(8,5),       -- �ӽФH PB
	APP_SCRCODE	INT,                -- �ӽФH PRESCREEN�N�X
	APP_SCRMSG	VARCHAR(128),       -- �ӽФH PRESCREEN�T��
	COS_RSCORE	DECIMAL(8,5),       -- �@�P�ɴڤH PDACO SCORE
	COS_PB		DECIMAL(8,5),       -- �@�P�ɴڤH PB
	COS_SCRCODE	INT,                -- �@�P�ɴڤH PRESCREEN�N�X
	COS_SCRMSG	VARCHAR(128),       -- �@�P�ɴڤH PRESCREEN�T��
	GUA_RSCORE	DECIMAL(8,5),       -- �O�ҤH PDACO SCORE
	GUA_PB		DECIMAL(8,5),       -- �O�ҤH PB
	GUA_SCRCODE	INT,                -- �O�ҤH PRESCREEN�N�X
	GUA_SCRMSG	VARCHAR(128),       -- �O�ҤH PRESCREEN�T��
	INCOME_CONSIDER	INT,                -- �Ҳոպ⦬�J
	LOAN_AMOUNT_CONSIDER	INT,        -- �Ҳոպ�U�ڪ��B
	WEIGHTED_APR	DECIMAL(8,5),       -- �[�v�����Q�v
	MAX_LOAN_CAPACITY	INT,        -- �̤j�U���B��
	MS101		INT,                -- ��I��
	FM_PB		DECIMAL(8,5),       -- PB
	NPV		INT,                -- NPV
	APPROVED_AMOUNT	INT,                -- �֭�U�ڪ��B
	MIN_RATE1	DECIMAL(8,5),       -- �Ĥ@�q�̧C�i�U�Q�v
	MIN_RATE2	DECIMAL(8,5),       -- �ĤG�q�̧C�i�U�Q�v
	MIN_RATE3	DECIMAL(8,5),       -- �ĤT�q�̧C�i�U�Q�v
	SUGG_CODE	INT,                -- �f�ֵ��G�N�X, 1:�֭� 2:���� 3:�H�u�f��
	SUGG_MSG	VARCHAR(256),       -- �f�ֵ��G�T��
	REASON_MSG	VARCHAR(256),       -- �f�֪��[�T��
	RESULT_FILE_GEN_TIME CHAR(14)       -- ��X�����ͮɶ� YYYYMMDDHHMMSS
);
ALTER TABLE APP_RESULT ADD CONSTRAINT P_APP_RESULT PRIMARY KEY (CASE_NO);
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


