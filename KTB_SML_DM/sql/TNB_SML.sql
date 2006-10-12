USE KTB_SML;
GO

CREATE TABLE BAM086 (
	MSN		    CHAR(14) NOT NULL, -- '申請編號';                                                                      
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
CREATE INDEX I_BAM086 ON BAM086(MSN, IDN);
GO

CREATE TABLE JAS002 (
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                                                       
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
CREATE INDEX I_JAS002 ON JAS002(MSN, IDN);
GO

CREATE TABLE KRM021 (	
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                   
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';   
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
CREATE INDEX I_KRM021 ON KRM021(MSN, IDN);
GO

CREATE TABLE KRM023 (
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                        
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
CREATE INDEX I_KRM023 ON KRM023(MSN, IDN);
GO

CREATE TABLE KRM037 (
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                                                                                                                                                                                 
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
CREATE INDEX I_KRM037 ON KRM037(MSN, IDN);
GO

CREATE TABLE STM007 (
	MSN		CHAR(14) NOT NULL, -- '申請編號';                                                             
	INQUIRY_DATE	CHAR(10) NOT NULL, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';                            
	IDN		CHAR(10) NOT NULL, -- '申請人身分證號';                                                       
	QUERY_DATE 	CHAR(7),           -- '查詢日期';                                                             
 	BANK_CODE 	CHAR(7),           -- '查詢單位代號';                                                         
	BANK_NAME 	CHAR(40),          -- '查詢單位名稱';                                                         
	ITEM_LIST 	CHAR(10),          -- '查詢項目串列 B:授信 D:票信 K:信用卡等';                                
	INQ_PURPOSE_1	CHAR(1),           -- '查詢理由碼';                                                           
	INQ_PURPOSE	CHAR(30)           -- '查詢理由碼(中文註解)1:新業務申請2:原業務往來3:新業務申請及原業務往來'; 

);
CREATE INDEX I_STM007 ON STM007(MSN, IDN);
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
	MSN		CHAR(14) NOT NULL,     -- 案件編號
	SYSTEM_DATE	CHAR(14) NOT NULL,     -- 案件輸入時間 YYYYMMDDHHMMSS
	APPLICANT_ID	CHAR(10) NOT NULL,     -- 申請人身份證字號姓名  
	APPLICANT_NAME	VARCHAR(30) NOT NULL,  -- 申請人姓名
	BIRTHDAY	CHAR(7) NOT NULL,      -- 生日 YYYMMDD
	ZIP		CHAR(3),               -- 三位郵遞區號
	MARRIAGE	INT,                   -- 婚姻 0:未婚 / 1:已婚 / 2:離婚               
	CHILD		INT,                   -- 子女數 0 / 1 / 2 / 3:大於2個                  
	EDUCATION	INT,                   -- 教育 0:研究所 / 1:大學 / 2:專科 / 3:高、國中
	CAREER		VARCHAR(30) NOT NULL,  -- 職業
	INCOME		INT NOT NULL,          -- 年收入
	APP_AMT		INT NOT NULL,          -- 申貸金額 
	PERIOD		INT NOT NULL,          -- 申貸期間
	APR		DECIMAL(8,5) NOT NULL, -- 申貸利率
	APP_FEE		INT NOT NULL,          -- 開辦費
	BRANCH		CHAR(4),               -- 進件分行  
	AGENT		CHAR(4),               -- 進件員工  
-- PROPERTY1
	OWNER_ID1	CHAR(10) NOT NULL,     -- 所有人身分證字號
	OWNER_NAME1	VARCHAR(30) NOT NULL,  -- 所有人姓名
	LAND_NUM1	VARCHAR(30) NOT NULL,  -- 地號/建號
	LIEN1		INT,           -- 目前抵押順位 0: 無 / 1:第一順位 / 2:第二順位或以上          
	FIRST_LIEN1	INT,           -- 已抵押總金額                                                  
	RELATIONSHIP1	INT, 	       -- 與申請人關係 0:本人 / 1:父母 / 2;配偶 / 3:子女 / 4:其他     
	LOCATION1	INT,           -- 座落區域 0:住宅區 / 1:商業區 / 2:住商混合區 / 3:其他    
	LAND_FORBIT1	INT,           -- 土地是否列為禁建 0: NO / 1:YES                                  
	LAND_DEMOLISH1	INT,           -- 土地是否為都市計劃拆除 0: NO / 1:YES                                  
	LAND_COLLECT1	INT,           -- 土地是否為都市計劃徵收對象 0: NO / 1:YES                                  
	HOUSE_RAY1	INT,           -- 房屋是否為輻射屋 0: NO / 1:YES                                  
	HOUSE_SEASAND1	INT,           -- 房屋是否為海砂屋0: NO / 1:YES                                  
	HOUSE_DANGEROUS1	INT,   -- 房屋是否為危樓 0: NO / 1:YES                          
	NAV1		FLOAT,         -- 房屋淨值
	GAV1		FLOAT,         -- 房屋毛值
	HOUSE_DAMAGE1	INT,           -- 是否為餘屋及屋況無人管理且外觀、內部破損嚴重 0: NO / 1:YES
	HOUSE_LENT1	INT,           -- 目前有效之租賃契約是否超過五年或是不定期者 0: NO / 1:YES
	HOUSE_MISUSE1	INT,           -- 用途是否為祭祀公業、寺廟、神壇、賭場、祠堂、教堂、市場、酒店 0: NO / 1:YES
	HOUSE_COMPLEX1	INT,           -- 是否出入複雜或是附近有特種營業的小套房 0: NO / 1:YES
	HOUSE_BASEMENT1	INT,           -- 是否公寓大樓地下室 0: NO / 1:YES
-- PROPERTY2 
	OWNER_ID2	CHAR(10),      -- 所有人身分證字號 
	OWNER_NAME2	VARCHAR(30),   -- 所有人姓名       
	LAND_NUM2	VARCHAR(30),   -- 地號/建號        
	LIEN2		INT,           -- 目前抵押順位 0: 無 / 1:第一順位 / 2:第二順位或以上                           
	FIRST_LIEN2	INT,           -- 已抵押總金額                                                                 
	RELATIONSHIP2	INT,           -- 與申請人關係 0:本人 / 1:父母 / 2;配偶 / 3:子女 / 4:其他                                   
	LOCATION2	INT,           -- 座落區域 0:住宅區 / 1:商業區 / 2:住商混合區 / 3:其他                         
	LAND_FORBIT2	INT,           -- 土地是否列為禁建 0: NO / 1:YES                                               
	LAND_DEMOLISH2	INT,           -- 土地是否為都市計劃拆除 0: NO / 1:YES                                         
	LAND_COLLECT2	INT,           -- 土地是否為都市計劃徵收對象 0: NO / 1:YES                                     
	HOUSE_RAY2	INT,           -- 房屋是否為輻射屋 0: NO / 1:YES                                               
	HOUSE_SEASAND2	INT,           -- 房屋是否為海砂屋0: NO / 1:YES                                                
	HOUSE_DANGEROUS2	INT,   -- 房屋是否為危樓 0: NO / 1:YES                                                 
	NAV2		FLOAT,         -- 房屋淨值                                                                     
	GAV2		FLOAT,         -- 房屋毛值                                                                     
	HOUSE_DAMAGE2	INT,           -- 是否為餘屋及屋況無人管理且外觀、內部破損嚴重 0: NO / 1:YES                   
	HOUSE_LENT2	INT,           -- 目前有效之租賃契約是否超過五年或是不定期者 0: NO / 1:YES                     
	HOUSE_MISUSE2	INT,           -- 用途是否為祭祀公業、寺廟、神壇、賭場、祠堂、教堂、市場、酒店 0: NO / 1:YES   
	HOUSE_COMPLEX2	INT,           -- 是否出入複雜或是附近有特種營業的小套房 0: NO / 1:YES                         
	HOUSE_BASEMENT2	INT,           -- 是否公寓大樓地下室 0: NO / 1:YES                                              
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
	AUDIT_AGENT	CHAR(4),          -- 複審人員
	FINAL_CODE	INT,              -- 複審結果代碼 
	FINAL_MSG	VARCHAR(256)      -- 複審結果訊息
);
ALTER TABLE APP_FINAL ADD CONSTRAINT P_APP_FINAL PRIMARY KEY (MSN);
GO
