USE KTB_FM;
GO

CREATE TABLE BAM086 (
	CASE_NO		    CHAR(14) NOT NULL, -- '申請編號';
	INQUIRY_DATE	    CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	IDN		    CHAR(10) NOT NULL, -- '申請人身分證號';
	DATA_YYY            CHAR(3),           -- '資料年度';
	DATA_MM             CHAR(2),           -- '資料月份';
	BANK_CODE           CHAR(7),           -- '行庫代號';
	BANK_NAME           CHAR(40),          -- '行庫名稱';
	ACCOUNT_CODE        CHAR(1),           -- '會計科目  (對照表)';
	ACCOUNT_CODE2       CHAR(1),           -- '會計科目註記  S 表示有擔保';
	PURPOSE_CODE        CHAR(1),           -- '用途別  (對照表)';
	CONTRACT_AMT        CHAR(10),          -- '訂約金額 (千元)';
	LOAN_AMT            CHAR(10),          -- '放款未逾期 (千元)';
	PASS_DUE_AMT        CHAR(10),          -- '逾期未還金額 (千元)';
	PAY_CODE_12         CHAR(12),          -- '最近12個月還款紀錄  (對照表)';
	CO_LOAN             CHAR(1),           -- '共同借款註記  * 表示有共同借款';
	UN_MARK             CHAR(1),           -- '國際聯貸註記  * 表示為國際聯貸';
	U_YYYMMDD           CHAR(8),           -- '聯貸日期  西元YYYYMMDD';
	U_RATE              CHAR(3),           -- '參貸比例';
	IB_MARK             CHAR(1),           -- '資金流向註記  * 表示為資金流向非法人組織';
	IAB_BAN             CHAR(8),           -- '非法人組織統編';
	IAB_NAME            CHAR(60),          -- '非法人組織名稱';
	R_YYYMMDD           CHAR(7),           -- '還款日期';
	REFUND_AMT          CHAR(10),          -- '還款金額 (千元)';
	CK_REF              CHAR(1)            -- '還款記錄  Y:全部還清 N:部分還清 1.還清 2.清償積欠本息 3.清償積欠利息 4.轉正常';
);
CREATE INDEX I_BAM086 ON BAM086(CASE_NO, IDN);
GO

CREATE TABLE JAS002 (
	CASE_NO		CHAR(14) NOT NULL, -- '申請編號';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	IDN		CHAR(10) NOT NULL, -- '申請人身分證號';
	EVER_DELINQUENT CHAR(1),           -- '授信異常 YN';
	DELINQUENT_DATE CHAR(7),           -- '最近授信異常日期';
	EVER_BAD_CHECK  CHAR(1),           -- '退票異常 YN';
	BAD_CHECK_DATE  CHAR(7),           -- '最近退票異常日期';
	EVER_REJECT     CHAR(1),           -- '拒絕往來記錄 YN';
	REJECT_DATE     CHAR(7),           -- '拒絕往來日期  拒絕往來記錄N本欄位有日期為該戶拒往已解除本欄位日期為拒絕往來日期';
	EVER_STOP_CARD  CHAR(1),           -- '強制停卡異常 YN';
	STOP_CARD_DATE  CHAR(7)            -- '最近強制停卡日期';
);
CREATE INDEX I_JAS002 ON JAS002(CASE_NO, IDN);
GO

CREATE TABLE KRM021 (
	CASE_NO		CHAR(14) NOT NULL, -- '申請編號';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	IDN		CHAR(10) NOT NULL, -- '申請人身分證號';
        CARD_BRAND      Char(1),           --  卡名代號  (對照表)
        CARD_TYPE       Char(1),           --  標章代號  (對照表)
        ISSUE           Char(3),           --  發卡機構代號
        ISSUE_NAME      Char(40),          --  發卡機構名稱
        START_DATE      Char(7),           --  啟用日期
        STOP_DATE       Char(7),           --  停用日期
        STOP_CODE       Char(1),           --  停用種類代號  (對照表)
        AB_CODE         Char(1),           --  停用原因代號  (對照表)
        M_S             Char(1),           --  主附卡(Y/N)
        RELA            Char(1),           -- 〔僅附卡〕主附卡關係代號  (對照表)
        LIMIT           Char(6),           --  額度（千元）
        RISK            Char(6),           -- 〔僅停用原因＝Ｐ〕風險損失金額（千元）
        CLEAR_DATE      Char(7),           -- 〔僅停用原因＝Ｕ〕欠款結清日期
        IDN_PRI         Char(10),          -- 〔僅附卡〕主卡人身分證號
        CNAME           Char(12),          -- 〔僅附卡〕主卡人姓名
        REMARK          Char(40)           --  註記
);
CREATE INDEX I_KRM021 ON KRM021(CASE_NO, IDN);
GO

CREATE TABLE KRM023 (
	CASE_NO		CHAR(14) NOT NULL, -- '申請編號';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	IDN		CHAR(10) NOT NULL, -- '申請人身分證號';
	YRMON 		CHAR (5),          -- '資料年月';
	ISSUE 		CHAR (3),          -- '發卡機構代號';
	ISSUE_NAME 	CHAR (40),         -- '發卡機構名稱';
	KR_CODE 	CHAR (7),          -- '信用卡別';
	LIMIT 		CHAR (5),          -- '核定額度 (千元)';
	PAYMENT 	CHAR (3),          -- '應繳金額級距(元)(位數+ +HML)';
	CASH 		CHAR (1),          -- '預借現金有無(YN)';
	PAY_CODE 	CHAR (1)           -- '繳款狀況代號  (對照表)';
);
CREATE INDEX I_KRM023 ON KRM023(CASE_NO, IDN);
GO

CREATE TABLE KRM037 (
	CASE_NO		CHAR(14) NOT NULL, -- '申請編號';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	IDN		CHAR(10) NOT NULL, -- '申請人身分證號';
	IDN_BAN		CHAR(10),          -- '繳款人ID';
	BILL_DATE	CHAR(7),           -- '結帳日';
	ISSUE		CHAR(3),           -- '發卡機構代號';
	ISSUE_NAME 	CHAR(24),          -- '發卡機構名稱';
	CARD_TYPE 	CHAR(7),           -- '信用卡別';
	PERM_LIMIT 	CHAR(7),           -- '永久額度  單位:千元';
	TEMP_LIMIT 	CHAR(7),           -- '臨時額度  單位:千元';
	CASH_LIMIT 	CHAR(7),           -- '預借現金額度  單位:千元';
	PAYABLE 	CHAR(3),           -- '本期應付帳款(級距)';
	CASH_LENT 	CHAR(8),           -- '本期預借現金  單位:元';
	LAST_PAYA 	CHAR(3),           -- '上期應付帳款(級距)';
	REVOL_BAL 	CHAR(9),           -- '上期循環信用  單位:元';
	PAY_STAT 	CHAR(1),           -- '上期繳款狀況代號(金額)  (對照表)';
	PAY_CODE 	CHAR(1),           -- '上期繳款狀況代號(時間)  (對照表)';
	REVOL_RATE 	CHAR(4),           -- '上期循環比率REVOL_BAL/PERM_LIMIT,精確度至小數點後2位(四捨五入),非百分比';
	PRE_OWED 	CHAR(8),           -- '未到期分期償還待付金額,未到期代墊消費款金額(格式22欄項)+未到期分期償還預借現金金額(格式23欄項),不包括本期新增餘額代償(18欄)';
	DEBT_CODE 	CHAR(1),           -- '債權狀態註記';
	CLOSE_CODE 	CHAR(1),           -- '債權結案註記(對照表)';
	CLEAR_DATE 	CHAR(7)            -- '不良債權結案日期';
);
CREATE INDEX I_KRM037 ON KRM037(CASE_NO, IDN);
GO

CREATE TABLE STM007 (
	CASE_NO		CHAR(14) NOT NULL, -- '申請編號';
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	IDN		CHAR(10) NOT NULL, -- '申請人身分證號';
	QUERY_DATE 	CHAR(7),           -- '查詢日期';
 	BANK_CODE 	CHAR(7),           -- '查詢單位代號';
	BANK_NAME 	CHAR(40),          -- '查詢單位名稱';
	ITEM_LIST 	CHAR(10),          -- '查詢項目串列 B:授信 D:票信 K:信用卡等';
	INQ_PURPOSE_1	CHAR(1),           -- '查詢理由碼';
	INQ_PURPOSE	CHAR(30)           -- '查詢理由碼(中文註解)1:新業務申請2:原業務往來3:新業務申請及原業務往來';

);
CREATE INDEX I_STM007 ON STM007(CASE_NO, IDN);
GO

CREATE TABLE COUNTER (
	SEQ_TYPE	CHAR(9) NOT NULL,               -- '序號類型, Ayyyymmdd: A20060412';
	SEQ_NO		NUMERIC(5) NOT NULL DEFAULT 1   -- '序號值';
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
	CASE_NO		CHAR(14) NOT NULL,     -- 案件編號
	APP_DATE	CHAR(14) NOT NULL,     -- 案件輸入時間 YYYYMMDDHHMMSS
	PROD_TYPE	INT NOT NULL,	       -- 產品類別 1:一般房貸, 2:增貸, 3:原餘額轉貸
-- Applicant
	APP_ID		CHAR(10) NOT NULL,     -- 申請人身份證字號姓名
	APP_NAME	VARCHAR(30) NOT NULL,  -- 申請人姓名
	APP_BIRTHDAY	CHAR(7),	       -- 申請人生日 YYYMMDD
	APP_MARRIAGE	INT,                   -- 申請人婚姻 0:未婚 / 1:已婚 / 2:離婚
	APP_EDUCATION	INT,                   -- 申請人學歷 0:研究所 / 1:大學 / 2:專科 / 3:高、國中(含以下)
	APP_INCOME	INT,		       -- 申請人年收入
	APP_QUALIFIED	INT,		       -- 申請人符合承作條件 0: NO / 1:YES
-- Co-signer
	COS_ID		CHAR(10),              -- 共同借款人身份證字號姓名
	COS_NAME	VARCHAR(30),           -- 共同借款人姓名
	COS_BIRTHDAY	CHAR(7),               -- 共同借款人生日 YYYMMDD
	COS_MARRIAGE	INT,                   -- 共同借款人婚姻 0:未婚 / 1:已婚 / 2:離婚
	COS_EDUCATION	INT,                   -- 共同借款人學歷 0:研究所 / 1:大學 / 2:專科 / 3:高、國中(含以下)
	COS_INCOME	INT,		       -- 共同借款人年收入
	COS_QUALIFIED	INT,		       -- 共同借款人符合承作條件 0: NO / 1:YES		
-- Guarantor
	GUA_ID		CHAR(10),              -- 保證人身份證字號姓名  
	GUA_NAME	VARCHAR(30),           -- 保證人姓名
	GUA_BIRTHDAY	CHAR(7),               -- 保證人生日 YYYMMDD
	GUA_MARRIAGE	INT,                   -- 保證人婚姻 0:未婚 / 1:已婚 / 2:離婚               
	GUA_EDUCATION	INT,                   -- 保證人學歷 0:研究所 / 1:大學 / 2:專科 / 3:高、國中(含以下)
	GUA_INCOME	INT,		       -- 保證人年收入
	GUA_QUALIFIED	INT,		       -- 保證人符合承作條件 0: NO / 1:YES
-- Loan Info
	APP_AMT		INT,		       -- 申貸金額
	PERIOD		INT,		       -- 申貸期間
	APR1		DECIMAL(8,5),          -- 第一段利率
	SEG1		INT,		       -- 第一段期間
	APR2		DECIMAL(8,5),          -- 第二段利率
	SEG2		INT,		       -- 第二段期間
	APR3		DECIMAL(8,5),          -- 第三段利率
	SEG3		INT,		       -- 第三段期間
	GRACE_PERIOD	INT,                   -- 寬限期
	APP_FEE		INT,                   -- 開辦費
-- Collateral
	OWNER_ID	CHAR(10),              -- 所有人身分證字號
	OWNER_NAME	VARCHAR(30),           -- 所有人姓名
	LAND_NUM	VARCHAR(30),	       -- 地號/建號
	RELATIONSHIP	INT, 	               -- 與申請人關係 0:本人 / 1:父母 / 2;配偶 / 3:子女 / 4:其他
	GAV		FLOAT,                 -- 房屋毛值
	NAV		FLOAT,                 -- 房屋淨值
	COL_QUALIFIED	INT,                   -- 擔保品符合承作條件 0: NO / 1:YES
	PREMIUM_COL	INT,                   -- 擔保品區段良好且搭配優良建設公司 0: NO / 1:YES
        MONTHLY_PAYMENT INT,	               -- 目前房貸月付金
--
	INQUIRY_DATE	CHAR(8),               -- JCIC 查詢日期 YYYYMMDD
	BRANCH		VARCHAR(10),           -- 進件分行
	EMP_ID		VARCHAR(10),           -- 進件員工
	AUDITOR		VARCHAR(10)	       -- 徵審人員
);
ALTER TABLE APP_INFO ADD CONSTRAINT P_APP_INFO PRIMARY KEY (CASE_NO);
GO

--
CREATE TABLE APP_RESULT (
	CASE_NO		CHAR(14) NOT NULL,  -- 案件編號
	FINAL_DATE	CHAR(14),           -- 審核日期 YYYYMMDDHHMMSS
	APP_RSCORE	DECIMAL(8,5),       -- 申請人 PDACO SCORE
	APP_PB		DECIMAL(8,5),       -- 申請人 PB
	APP_SCRCODE	INT,                -- 申請人 PRESCREEN代碼
	APP_SCRMSG	VARCHAR(128),       -- 申請人 PRESCREEN訊息
	COS_RSCORE	DECIMAL(8,5),       -- 共同借款人 PDACO SCORE
	COS_PB		DECIMAL(8,5),       -- 共同借款人 PB
	COS_SCRCODE	INT,                -- 共同借款人 PRESCREEN代碼
	COS_SCRMSG	VARCHAR(128),       -- 共同借款人 PRESCREEN訊息
	GUA_RSCORE	DECIMAL(8,5),       -- 保證人 PDACO SCORE
	GUA_PB		DECIMAL(8,5),       -- 保證人 PB
	GUA_SCRCODE	INT,                -- 保證人 PRESCREEN代碼
	GUA_SCRMSG	VARCHAR(128),       -- 保證人 PRESCREEN訊息
	INCOME_CONSIDER	INT,                -- 模組試算收入
	LOAN_AMOUNT_CONSIDER	INT,        -- 模組試算貸款金額
	WEIGHTED_APR	DECIMAL(8,5),       -- 加權平均利率
	MAX_LOAN_CAPACITY	INT,        -- 最大貸款額度
	MS101		INT,                -- 月付金
	FM_PB		DECIMAL(8,5),       -- PB
	NPV		INT,                -- NPV
	APPROVED_AMOUNT	INT,                -- 核准貸款金額
	MIN_RATE1	DECIMAL(8,5),       -- 第一段最低可貸利率
	MIN_RATE2	DECIMAL(8,5),       -- 第二段最低可貸利率
	MIN_RATE3	DECIMAL(8,5),       -- 第三段最低可貸利率
	SUGG_CODE	INT,                -- 審核結果代碼, 1:核准 2:婉拒 3:人工審核
	SUGG_MSG	VARCHAR(256),       -- 審核結果訊息
	REASON_MSG	VARCHAR(256),       -- 審核附加訊息
	RESULT_FILE_GEN_TIME CHAR(14)       -- 輸出報表產生時間 YYYYMMDDHHMMSS
);
ALTER TABLE APP_RESULT ADD CONSTRAINT P_APP_RESULT PRIMARY KEY (CASE_NO);
GO

CREATE TABLE CFC (
	CFC_CODE	 char(4) NOT NULL,  -- 區域中心代碼
	CFC		 varchar(60),       -- 區域中心名稱
);
ALTER TABLE CFC ADD CONSTRAINT P_CFC PRIMARY KEY (CFC_CODE);
GO

CREATE TABLE BRANCH (
	BRANCH_CODE	 char(4) NOT NULL,  -- 分行代碼
	BRANCH		 varchar(60),       -- 分行名稱
	CFC_CODE	 char(4)            -- 區域中心代碼
);
ALTER TABLE BRANCH ADD CONSTRAINT P_BRANCH PRIMARY KEY (BRANCH_CODE);
GO


