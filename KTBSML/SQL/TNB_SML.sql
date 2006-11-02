USE KTB_SML;
GO

CREATE TABLE BAM086 (
	MSN		    CHAR(14) NOT NULL, -- '申請編號';                                                                      
	INQUIRY_DATE	    CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyyymmdd, yyyy為西元年)';                                     
	IDN		    CHAR(10) NOT NULL, -- '申請人身分證號';                                                                
	DATA_YYY            CHAR(3),           -- '資料年度';                                                                      
	DATA_MM             CHAR(2),           -- '資料月份';                                                                      
	BANK_CODE           CHAR(7),           -- '行庫代號';                                                                      
	BANK_NAME           CHAR(40),          -- '行庫名稱';                                                                      
	ACCOUNT_CODE        CHAR(1),           -- '會計科目  (對照表)';                                                            
	ACCOUNT_CODE2       CHAR(1),           -- '會計科目註記  S 表示有擔保';                                                    
	PURPOSE_CODE        CHAR(1),           -- '用途別  (對照表)';                                                              
	CONTRACT_AMT        INT,               -- '訂約金額 (千元)';                                                               
	LOAN_AMT            INT,               -- '放款未逾期 (千元)';                                                             
	PASS_DUE_AMT        INT,               -- '逾期未還金額 (千元)';                                                           
	PAY_CODE_12         CHAR(12),          -- '最近12個月還款紀錄  (對照表)';                                                  
	CO_LOAN             CHAR(1),           -- '共同借款註記  * 表示有共同借款';                                                
--	UN_MARK             CHAR(1),           -- '國際聯貸註記  * 表示為國際聯貸';                                                
--	U_YYYMMDD           CHAR(8),           -- '聯貸日期  西元YYYYMMDD';                                                        
--	U_RATE              CHAR(3),           -- '參貸比例';                                                                      
--	IB_MARK             CHAR(1),           -- '資金流向註記  * 表示為資金流向非法人組織';                                      
--	IAB_BAN             CHAR(8),           -- '非法人組織統編';                                                                
--	IAB_NAME            CHAR(60),          -- '非法人組織名稱';                                                                
--	R_YYYMMDD           CHAR(7),           -- '還款日期';                                                                      
--	REFUND_AMT          CHAR(10),          -- '還款金額 (千元)';                                                               
--	CK_REF              CHAR(1)            -- '還款記錄  Y:全部還清 N:部分還清 1.還清 2.清償積欠本息 3.清償積欠利息 4.轉正常'; 
);
CREATE INDEX I_BAM086 ON BAM086(MSN);
GO

CREATE TABLE JAS002 (
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                                                       
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyyymmdd, yyyy為西元年)';                                      
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
CREATE INDEX I_JAS002 ON JAS002(MSN);
GO

CREATE TABLE KRM021 (	
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                   
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyyymmdd, yyyy為西元年)';   
	IDN		CHAR(10) NOT NULL, -- '申請人身分證號';                              
        CARD_BRAND      Char(1),           --    卡名代號  (對照表)
        CARD_TYPE       Char(1),           --    標章代號  (對照表)
        ISSUE           Char(3),           --    發卡機構代號
        ISSUE_NAME      Char(40),          --   發卡機構名稱
        START_DATE      Char(7),           --    啟用日期
        STOP_DATE       Char(7),           --    停用日期
        STOP_CODE       Char(1),           --    停用種類代號  (對照表)
        AB_CODE         Char(1),           --    停用原因代號  (對照表)
        M_S             Char(1),           --    主附卡(Y/N)
        RELA            Char(1),           --    〔僅附卡〕主附卡關係代號  (對照表)
        LIMIT           Char(6),           --    額度（千元）
        RISK            Char(6),           --    〔僅停用原因＝Ｐ〕風險損失金額（千元）
        CLEAR_DATE      Char(7),           --    〔僅停用原因＝Ｕ〕欠款結清日期
        IDN_PRI         Char(10),          --   〔僅附卡〕主卡人身分證號
        CNAME           Char(12),          --   〔僅附卡〕主卡人姓名
        REMARK          Char(40)           --   註記
);
CREATE INDEX I_KRM021 ON KRM021(MSN);
GO

CREATE TABLE KRM023 (
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                        
	INQUIRY_DATE	CHAR(10),          -- 'JCIC 資料查詢日期(yyyymmdd, yyyy為西元年)';       
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
CREATE INDEX I_KRM023 ON KRM023(MSN);
GO

CREATE TABLE KRM037 (
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                                                                                                                                                                                 
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyyymmdd, yyyy為西元年)';                                                                                                                                                                
	IDN		CHAR(10) NOT NULL, -- '申請人身分證號';                                                                                                                                                                                           
	BILL_DATE	CHAR(7),           -- '結帳日';                                                                                                                                                                                                   
	ISSUE		CHAR(3),           -- '發卡機構代號';                                                                                                                                                                                             
	ISSUE_NAME 	CHAR(24),          -- '發卡機構名稱';                                                                                                                                                                                             
	CARD_TYPE 	CHAR(7),           -- '信用卡別';                                                                                                                                                                                                 
	PERM_LIMIT 	INT,               -- '永久額度  單位:千元';                                                                                                                                                                                      
	TEMP_LIMIT 	INT,               -- '臨時額度  單位:千元';                                                                                                                                                                                      
	CASH_LIMIT 	INT,               -- '預借現金額度  單位:千元';                                                                                                                                                                                  
	PAYABLE 	CHAR(3),           -- '本期應付帳款(級距)';                                                                                                                                                                                       
	CASH_LENT 	INT,               -- '本期預借現金  單位:元';                                                                                                                                                                                    
	LAST_PAYA 	CHAR(3),           -- '上期應付帳款(級距)';                                                                                                                                                                                       
	REVOL_BAL 	INT,               -- '上期循環信用  單位:元';                                                                                                                                                                                    
	PAY_STAT 	CHAR(1),           -- '上期繳款狀況代號(金額)  (對照表)';                                                                                                                                                                         
	PAY_CODE 	CHAR(1),           -- '上期繳款狀況代號(時間)  (對照表)';                                                                                                                                                                         
	REVOL_RATE 	INT,               -- '上期循環比率REVOL_BAL/PERM_LIMIT,精確度至小數點後2位(四捨五入),非百分比';                                                                                                                                  
	PRE_OWED 	INT,               -- '未到期分期償還待付金額,未到期代墊消費款金額(格式22欄項)+未到期分期償還預借現金金額(格式23欄項),不包括本期新增餘額代償(18欄)';                                                                              
	DEBT_CODE 	CHAR(1),           -- '債權狀態註記';                                                                                                                                                                                             
	CLOSE_CODE 	CHAR(1),           -- '債權結案註記(對照表)';                                                                                                                                                                                     
	CLEAR_DATE 	CHAR(7)            -- '不良債權結案日期';                                                                                                                                                                                         
);
CREATE INDEX I_KRM037 ON KRM037(MSN);
GO

CREATE TABLE STM007 (
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                                             
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyyymmdd, yyyy為西元年)';                            
	IDN		CHAR(10) NOT NULL, -- '申請人身分證號';                                                       
	QUERY_DATE 	CHAR(7),           -- '查詢日期';                                                             
 	BANK_CODE 	CHAR(7),           -- '查詢單位代號';                                                         
	BANK_NAME 	CHAR(40),          -- '查詢單位名稱';                                                         
	ITEM_LIST 	CHAR(10),          -- '查詢項目串列 B:授信 D:票信 K:信用卡等';                                
	INQ_PURPOSE_1	CHAR(1),           -- '查詢理由碼';                                                           
	INQ_PURPOSE	CHAR(30)           -- '查詢理由碼(中文註解)1:新業務申請2:原業務往來3:新業務申請及原業務往來'; 

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
	MSN		CHAR(14) NOT NULL,     -- 案件編號
	SYSTEM_DATE	CHAR(14) NOT NULL,     -- 案件輸入時間 YYYYMMDDHHMMSS
	APPLICANT_ID	CHAR(10) NOT NULL,     -- 申請人身份證字號姓名  
	APPLICANT_NAME	VARCHAR(30) NOT NULL,  -- 申請人姓名
	ZIP		CHAR(3),               -- 三位郵遞區號
	INCOME		INT NOT NULL,          -- 年收入
	APP_QUALIFIED	INT,		       -- 申請人符合承作條件 0: NO / 1:YES
	APP_AMT		INT NOT NULL,          -- 申貸金額 
	PERIOD		INT NOT NULL,          -- 申貸期間
	APR		DECIMAL(8,5) NOT NULL, -- 申貸利率
	APP_FEE		INT NOT NULL,          -- 開辦費
	BRANCH		VARCHAR(10),           -- 進件分行
	EMP_ID		VARCHAR(10),           -- 進件員工
	AUDITOR		VARCHAR(10),	       -- 徵審人員
-- PROPERTY1
	OWNER_ID1	CHAR(10) NOT NULL,     -- 所有人身分證字號
	OWNER_NAME1	VARCHAR(30) NOT NULL,  -- 所有人姓名
	LAND_NUM1	VARCHAR(60) NOT NULL,  -- 擔保品門牌號碼
	FIRST_LIEN1	INT,           -- 已抵押總金額                                                  
	RELATIONSHIP1	INT, 	       -- 與申請人關係 0:本人 / 1:父母 / 2;配偶 / 3:子女 / 4:其他     
	NAV1		FLOAT,         -- 鑑估淨值
	GAV1		FLOAT,         -- 鑑估總值
	QUALIFIED1	INT,           -- 0: NO / 1:YES                                  
-- PROPERTY2 
	OWNER_ID2	CHAR(10),      -- 所有人身分證字號 
	OWNER_NAME2	VARCHAR(30),   -- 所有人姓名       
	LAND_NUM2	VARCHAR(60),   -- 擔保品門牌號碼        
	FIRST_LIEN2	INT,           -- 已抵押總金額                                                                 
	RELATIONSHIP2	INT,           -- 與申請人關係 0:本人 / 1:父母 / 2;配偶 / 3:子女 / 4:其他                                   
	NAV2		FLOAT,         -- 鑑估淨值                                                                     
	GAV2		FLOAT,         -- 鑑估總值                                                                     
	QUALIFIED2	INT,           -- 0: NO / 1:YES                                  
	INQUIRY_DATE	CHAR(8)        -- JCIC 查詢日期 YYYYMMDD
);
ALTER TABLE APP_INFO ADD CONSTRAINT P_APP_INFO PRIMARY KEY (MSN);
GO

-- PREMIER REVIEW OUTPUT 初審
CREATE TABLE APP_PREMIER (
	MSN		CHAR(14) NOT NULL,  -- 案件編號
	PREMIER_DATE	CHAR(14),           -- 初審日期YYYYMMDDHHMMSS
        RSCORE		DECIMAL(8,5),       -- PDACO score
	PREMIER_CODE	INT,                -- 初審結果代碼      
	PREMIER_MSG	VARCHAR(256),       -- 初審結果訊息 
);
ALTER TABLE APP_PREMIER ADD CONSTRAINT P_APP_PREMIER PRIMARY KEY (MSN);
GO

-- FINAL REVIEW OUTPUT  複審
CREATE TABLE APP_FINAL (
	MSN		CHAR(14) NOT NULL,-- 案件編號
	FINAL_DATE	CHAR(14),         -- 複審日期 YYYYMMDDHHMMSS
	PB		DECIMAL(8,5),     -- PB
	NPV		INT,              -- NPV
	OPTIMAL_AMOUNT	INT,              -- 最高可貸金額
--	AUDIT_AGENT	CHAR(4),          -- 複審人員
	FINAL_CODE	INT,              -- 複審結果代碼 
	FINAL_MSG	VARCHAR(256)      -- 複審結果訊息
);
ALTER TABLE APP_FINAL ADD CONSTRAINT P_APP_FINAL PRIMARY KEY (MSN);
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
	EMP_ID		VARCHAR(32),                      -- 'USER EMPLOYEE ID';         
	USERBRANCH	CHAR(4)                           -- BRANCH CODE
);
ALTER TABLE AGENT ADD CONSTRAINT P_AGENT PRIMARY KEY (USERID);

INSERT INTO AGENT VALUES ('test', 'test', 'test', '99999', '0489');
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



