connect to ESCORE user escorap1 using escorap1;

-- drop table aas001;
-- drop table aps001;
-- drop table bas001;
-- drop table bam086;
-- drop table bam302;
-- drop table bam303;
-- drop table bam304;
-- drop table dam103;
-- drop table dam203;
-- drop table das006;
-- drop table krm001;
-- drop table krm023;
-- drop table stm001;
-- drop table vam102;
-- drop table app_info;


-- create JCIC tables
create table AAS001 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	ID		    char(10),
	PNAME               Char(40),
	IS_LOST             Char(1),
	NULL2               Char(7),
	IS_FAKE_DATE        Char(1),
	FAKE_DATE           Char(7)
) IN "ESCORETABSP";
create index i_AAS001 on AAS001(CASE_SN, IDN);

COMMENT ON TABLE AAS001 IS '自然人姓名,身分證冒用 (單筆)';
COMMENT ON COLUMN AAS001.CASE_SN	IS '申請編號';
COMMENT ON COLUMN AAS001.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN AAS001.IDN 		IS '申請人身分證號';
COMMENT ON COLUMN AAS001.ID 		IS '身分證號';
COMMENT ON COLUMN AAS001.PNAME		IS '中文姓名';
COMMENT ON COLUMN AAS001.IS_LOST	IS '是否有身分證補發 YN';
COMMENT ON COLUMN AAS001.NULL2		IS '空白';
COMMENT ON COLUMN AAS001.IS_FAKE_DATE	IS '是否有身分證冒用 YN';
COMMENT ON COLUMN AAS001.FAKE_DATE	IS '身分證冒用日期';

create table APS001 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	RGY_ADDRESS         Char(66),
	ENG_NAME            Char(20),
	BIRTHDAY            Char(7)
) IN "ESCORETABSP";
create index i_aps001 on APS001(CASE_SN, IDN);

COMMENT ON TABLE APS001	IS '自然人個人資料 (單筆 J01)';
COMMENT ON COLUMN APS001.CASE_SN	IS '申請編號';
COMMENT ON COLUMN APS001.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN APS001.IDN 		IS '申請人身分證號';
COMMENT ON COLUMN APS001.RGY_ADDRESS	IS '戶籍地址';
COMMENT ON COLUMN APS001.ENG_NAME	IS '英文姓名';
COMMENT ON COLUMN APS001.BIRTHDAY	IS '出生日期';

create table BAS001 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	EVER_DELINQUENT     Char(1),
	DATA_YYYMMDD        Char(7)
) IN "ESCORETABSP";
create index i_BAS001 on BAS001(CASE_SN, IDN);

COMMENT ON TABLE BAS001 IS '授信異常紀錄（三年內逾催或五年內呆帳）';
COMMENT ON COLUMN BAS001.CASE_SN	IS '申請編號';
COMMENT ON COLUMN BAS001.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN BAS001.IDN	 	IS '申請人身分證號';
COMMENT ON COLUMN BAS001.EVER_DELINQUENT	IS '授信異常 YN';
COMMENT ON COLUMN BAS001.DATA_YYYMMDD	IS '最近授信異常日期 YYYMMDD';


create table BAM086 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	DATA_YYY            Char(3),
	DATA_MM             Char(2),
	BANK_CODE           Char(7),
	BANK_NAME           Char(40),
	ACCOUNT_CODE        Char(1),
	ACCOUNT_CODE2       Char(1),
	PURPOSE_CODE        Char(1),
	CONTRACT_AMT        char(10),
	LOAN_AMT            char(10),
	PASS_DUE_AMT        char(10),
	PAY_CODE_12         Char(12),
	CO_LOAN             Char(1),
	UN_MARK             Char(1),
	U_YYYMMDD           Char(8),
	U_RATE              char(3),
	IB_MARK             Char(1),
	IAB_BAN             Char(8),
	IAB_NAME            Char(60),
	R_YYYMMDD           Char(7),
	REFUND_AMT          char(10),
	CK_REF              Char(1)
) IN "ESCORETABSP";
create index i_BAM086 on BAM086(CASE_SN, IDN);

COMMENT ON TABLE BAM086 IS '授信資料明細及還款紀錄資訊-行庫別 (多筆 B32,B63-B64)';
COMMENT ON COLUMN BAM086.CASE_SN	IS '申請編號';
COMMENT ON COLUMN BAM086.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN BAM086.IDN	 	IS '申請人身分證號';
COMMENT ON COLUMN BAM086.DATA_YYY 	IS '資料年度';
COMMENT ON COLUMN BAM086.DATA_MM 	IS '資料月份';
COMMENT ON COLUMN BAM086.BANK_CODE	IS '行庫代號';
COMMENT ON COLUMN BAM086.BANK_NAME 	IS '行庫名稱';
COMMENT ON COLUMN BAM086.ACCOUNT_CODE	IS '會計科目  (對照表)';
COMMENT ON COLUMN BAM086.ACCOUNT_CODE2	IS '會計科目註記  S 表示有擔保';
COMMENT ON COLUMN BAM086.PURPOSE_CODE	IS '用途別  (對照表)';
COMMENT ON COLUMN BAM086.CONTRACT_AMT	IS '訂約金額 (千元)';
COMMENT ON COLUMN BAM086.LOAN_AMT	IS '放款未逾期 (千元)';
COMMENT ON COLUMN BAM086.PASS_DUE_AMT	IS '逾期未還金額 (千元)';
COMMENT ON COLUMN BAM086.PAY_CODE_12	IS '最近12個月還款紀錄  (對照表)';
COMMENT ON COLUMN BAM086.CO_LOAN	IS '共同借款註記  * 表示有共同借款';
COMMENT ON COLUMN BAM086.UN_MARK 	IS '國際聯貸註記  * 表示為國際聯貸';
COMMENT ON COLUMN BAM086.U_YYYMMDD	IS '聯貸日期  西元YYYYMMDD';
COMMENT ON COLUMN BAM086.U_RATE		IS '參貸比例';
COMMENT ON COLUMN BAM086.IB_MARK 	IS '資金流向註記  * 表示為資金流向非法人組織';
COMMENT ON COLUMN BAM086.IAB_BAN	IS '非法人組織統編';
COMMENT ON COLUMN BAM086.IAB_NAME	IS '非法人組織名稱';
COMMENT ON COLUMN BAM086.R_YYYMMDD	IS '還款日期';
COMMENT ON COLUMN BAM086.REFUND_AMT	IS '還款金額 (千元)';
COMMENT ON COLUMN BAM086.CK_REF		IS '還款記錄  Y:全部還清 N:部分還清 1.還清 2.清償積欠本息 3.清償積欠利息 4.轉正常';

create table BAM302 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	DATA_YYY            Char(3),
	DATA_MM             Char(2),
	BANK_CODE           Char(7),
	BANK_NAME           Char(40),
	ACCOUNT_CODE        Char(1),
	ACCOUNT_CODE2       Char(1),
	PURPOSE_CODE        Char(1),
	GID_CODE            Char(1),
	M_ID                Char(10),
	M_ID_NAME           Char(60),
	REL_CODE            Char(2),
	CONTRACT_AMT        char(10),
	LOAN_AMT            char(10),
	PASS_DUE_AMT        char(10)
) IN "ESCORETABSP";
create index i_BAM302 on BAM302(CASE_SN, IDN);


COMMENT ON TABLE BAM302 IS '授信保證資料(B31之共同債務資料 (多筆 B31-B33)';
COMMENT ON COLUMN BAM302.CASE_SN	IS '申請編號';
COMMENT ON COLUMN BAM302.IDN 		IS '申請人身分證號';
COMMENT ON COLUMN BAM302.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN BAM302.DATA_YYY	IS '資料年度';
COMMENT ON COLUMN BAM302.DATA_MM	IS '資料月份';
COMMENT ON COLUMN BAM302.BANK_CODE 	IS '行庫代號';
COMMENT ON COLUMN BAM302.BANK_NAME	IS '行庫名稱';
COMMENT ON COLUMN BAM302.ACCOUNT_CODE	IS '會計科目  (對照表)';
COMMENT ON COLUMN BAM302.ACCOUNT_CODE2	IS '會計註記  S 表示有擔保';
COMMENT ON COLUMN BAM302.PURPOSE_CODE	IS '用途別  (對照表)';
COMMENT ON COLUMN BAM302.GID_CODE 	IS '身分代號  (對照表)';
COMMENT ON COLUMN BAM302.M_ID		IS '主債務人身分證號';
COMMENT ON COLUMN BAM302.M_ID_NAME 	IS '主債務人名稱';
COMMENT ON COLUMN BAM302.REL_CODE	IS '關係代號  (對照表)';
COMMENT ON COLUMN BAM302.CONTRACT_AMT	IS '訂約金額 (千元)';
COMMENT ON COLUMN BAM302.LOAN_AMT	IS '放款未逾期 (千元)';
COMMENT ON COLUMN BAM302.PASS_DUE_AMT	IS '逾期未還金額 (千元)';

create table BAM303 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	DATA_YYY            Char(3),
	DATA_MM             Char(2),
	BANK_CODE           Char(7),
	BANK_NAME           Char(40),
	ACCOUNT_CODE        Char(1),
	ACCOUNT_CODE2       Char(1),
	PURPOSE_CODE        Char(1),
	GID_CODE            Char(1),
	M_ID                Char(10),
	M_ID_NAME           Char(60),
	REL_CODE            Char(2),
	CONTRACT_AMT        char(10),
	LOAN_AMT            char(10),
	PASS_DUE_AMT        char(10)
) IN "ESCORETABSP";
create index i_BAM303 on BAM303(CASE_SN, IDN);

COMMENT ON TABLE BAM303	IS '授信保證資料(B31之共同債務資料 (多筆 B31-B33)';
COMMENT ON COLUMN BAM303.CASE_SN	IS '申請編號';
COMMENT ON COLUMN BAM303.IDN	 	IS '申請人身分證號';
COMMENT ON COLUMN BAM303.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN BAM303.DATA_YYY 	IS '資料年度';
COMMENT ON COLUMN BAM303.DATA_MM 	IS '資料月份';
COMMENT ON COLUMN BAM303.BANK_CODE	IS '行庫代號';
COMMENT ON COLUMN BAM303.BANK_NAME	IS '行庫名稱';
COMMENT ON COLUMN BAM303.ACCOUNT_CODE	IS '會計科目  (對照表)';
COMMENT ON COLUMN BAM303.ACCOUNT_CODE2	IS '會計註記  S 表示有擔保';
COMMENT ON COLUMN BAM303.PURPOSE_CODE	IS '用途別  (對照表)';
COMMENT ON COLUMN BAM303.GID_CODE	IS '身分代號  (對照表)';
COMMENT ON COLUMN BAM303.M_ID	IS '主債務人身分證號';
COMMENT ON COLUMN BAM303.M_ID_NAME	IS '主債務人名稱';
COMMENT ON COLUMN BAM303.REL_CODE	IS '關係代號  (對照表)';
COMMENT ON COLUMN BAM303.CONTRACT_AMT	IS '訂約金額 (千元)';
COMMENT ON COLUMN BAM303.LOAN_AMT	IS '放款未逾期 (千元)';
COMMENT ON COLUMN BAM303.PASS_DUE_AMT	IS '逾期未還金額 (千元)';


create table BAM304 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	DATA_YYY            Char(3),
	DATA_MM             Char(2),
	BANK_CODE           Char(7),
	BANK_NAME           Char(40),
	ACCOUNT_CODE        Char(1),
	ACCOUNT_CODE2       Char(1),
	PURPOSE_CODE        Char(1),
	GID_CODE            Char(1),
	M_ID                Char(10),
	M_ID_NAME           Char(60),
	REL_CODE            Char(2),
	CONTRACT_AMT        char(10),
	LOAN_AMT            char(10),
	PASS_DUE_AMT        char(10)
) IN "ESCORETABSP";
create index i_BAM304 on BAM304(CASE_SN, IDN);

COMMENT ON TABLE BAM304	IS '授信保證資料(B31之其他債務資料) (多筆 B31-B33)';
COMMENT ON COLUMN BAM304.CASE_SN	IS '申請編號';
COMMENT ON COLUMN BAM304.IDN	 	IS '申請人身分證號';
COMMENT ON COLUMN BAM304.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN BAM304.DATA_YYY	IS '資料年度';
COMMENT ON COLUMN BAM304.DATA_MM 	IS '資料月份';
COMMENT ON COLUMN BAM304.BANK_CODE	IS '行庫代號';
COMMENT ON COLUMN BAM304.BANK_NAME	IS '行庫名稱';
COMMENT ON COLUMN BAM304.ACCOUNT_CODE	IS '會計科目  (對照表)';
COMMENT ON COLUMN BAM304.ACCOUNT_CODE2	IS '會計註記  S 表示有擔保';
COMMENT ON COLUMN BAM304.PURPOSE_CODE	IS '用途別  (對照表)';
COMMENT ON COLUMN BAM304.GID_CODE 	IS '身分代號  (對照表)';
COMMENT ON COLUMN BAM304.M_ID		IS '主債務人身分證號';
COMMENT ON COLUMN BAM304.M_ID_NAME	IS '主債務人名稱';
COMMENT ON COLUMN BAM304.REL_CODE	IS '關係代號  (對照表)';
COMMENT ON COLUMN BAM304.CONTRACT_AMT	IS '訂約金額 (千元)';
COMMENT ON COLUMN BAM304.LOAN_AMT	IS '放款未逾期 (千元)';
COMMENT ON COLUMN BAM304.PASS_DUE_AMT 	IS '逾期未還金額 (千元)';

create table DAM103 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	CK_DATE             Char(7),
	ISSUE_BANK          Char(7),
	BANK_NAME           Char(40),
	ACCNT_NO            Char(9),
	CK_TYPE             Char(1),
	CHECK_NO            Char(9),
	AMT                 char(10),
	CANCEL_DATE         Char(7),
	COMPANY_BOUNCE      Char(1),
	RESP_ID             Char(10),
	RESP_NAME           Char(60)
) IN "ESCORETABSP";
create index i_DAM103 on DAM103(CASE_SN, IDN);

COMMENT ON TABLE DAM103	IS '退票 (多筆 D01 D03, 取代DAM102)';
COMMENT ON COLUMN DAM103.CASE_SN	IS '申請編號';
COMMENT ON COLUMN DAM103.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN DAM103.IDN 		IS '申請人身分證號';
COMMENT ON COLUMN DAM103.CK_DATE 	IS '退票日期';
COMMENT ON COLUMN DAM103.ISSUE_BANK 	IS '退票行庫';
COMMENT ON COLUMN DAM103.BANK_NAME 	IS '行庫名稱';
COMMENT ON COLUMN DAM103.ACCNT_NO 	IS '帳號';
COMMENT ON COLUMN DAM103.CK_TYPE 	IS '票據種類 C=支票,N=本票,R=匯票,O=其他';
COMMENT ON COLUMN DAM103.CHECK_NO 	IS '支票號碼';
COMMENT ON COLUMN DAM103.AMT	 	IS '金額 (元)';
COMMENT ON COLUMN DAM103.CANCEL_DATE 	IS '清償註記日期';
COMMENT ON COLUMN DAM103.COMPANY_BOUNCE	IS '公司退票 YN';
COMMENT ON COLUMN DAM103.RESP_ID 	IS '公司統編或負責人身分證號';
COMMENT ON COLUMN DAM103.RESP_NAME 	IS '公司名稱或負責人姓名';

create table DAM203 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	BEG_DATE            Char(7),
	END_DATE            Char(7),
	RSN                 Char(1),
	SOURCE              Char(1),
	UPD_DATE            Char(7),
	RELIEF              Char(1),
	S_BAN               Char(8),
	S_BAN_NAME          Char(60)
) IN "ESCORETABSP";
create index i_DAM203 on DAM203(CASE_SN, IDN);

COMMENT ON TABLE DAM203 IS '拒往 (多筆 D01 D03, 取代DAM202)';
COMMENT ON COLUMN DAM203.CASE_SN	IS '申請編號';
COMMENT ON COLUMN DAM203.IDN 		IS '申請人身分證號';
COMMENT ON COLUMN DAM203.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN DAM203.BEG_DATE	IS '拒往開始日期';
COMMENT ON COLUMN DAM203.END_DATE	IS '拒往解除日期  有日期或七個 0 表示已解除';
COMMENT ON COLUMN DAM203.RSN		IS '拒往解除理由  1:清償解除 2:暫予解除';
COMMENT ON COLUMN DAM203.SOURCE		IS '資料來源  1:當事人申請 2:票交所公告';
COMMENT ON COLUMN DAM203.UPD_DATE	IS '資料更新日期';
COMMENT ON COLUMN DAM203.RELIEF		IS '拒往解除狀況  A:已解除';
COMMENT ON COLUMN DAM203.S_BAN		IS '個人擔任非法人組織之統編（獨資或含夥）';
COMMENT ON COLUMN DAM203.S_BAN_NAME	IS '個人擔任非法人組織之統編名稱';

create table DAS006 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	BOUNCE_AMT          char(12),
	BOUNCE_CNT          char(10),
	LATEST_BOUNCE       Char(7),
	WRITEOFF_AMT        char(12),
	WRITEOFF_CNT        char(10),
	LATEST_WRITEOFF     Char(7),
	IDN_BOUNCE_AMT      char(12),
	IDN_BOUNCE_CNT      char(10),
	IDN_LATEST_BOUNCE   Char(7),
	IDN_WRITEOFF_AMT    char(12),
	IDN_WRITEOFF_CNT    char(10),
	IDN_LATEST_WRITEOFF Char(7)
) IN "ESCORETABSP";
create index i_DAS006 on DAS006(CASE_SN, IDN);

COMMENT ON TABLE DAS006 IS '主體連帶退票摘要紀錄 (單筆摘要)';
COMMENT ON COLUMN DAS006.CASE_SN	IS '申請編號';
COMMENT ON COLUMN DAS006.IDN		IS '申請人身分證號';
COMMENT ON COLUMN DAS006.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN DAS006.BOUNCE_AMT	IS '主體退票未清償註記總金額 (千元)';
COMMENT ON COLUMN DAS006.BOUNCE_CNT	IS '主體退票未清償註記總張數';
COMMENT ON COLUMN DAS006.LATEST_BOUNCE	IS '主體最近一次退票日期';
COMMENT ON COLUMN DAS006.WRITEOFF_AMT	IS '主體退票已清償註記總金額 (千元)';
COMMENT ON COLUMN DAS006.WRITEOFF_CNT	IS '主體退未已清償註記票總張數';
COMMENT ON COLUMN DAS006.LATEST_WRITEOFF	IS '主體最近一次已清償註記註銷日期';
COMMENT ON COLUMN DAS006.IDN_BOUNCE_AMT	IS '連帶退票未清償註記總金額 (千元)';
COMMENT ON COLUMN DAS006.IDN_BOUNCE_CNT	IS '連帶退票未清償註記總張數';
COMMENT ON COLUMN DAS006.IDN_LATEST_BOUNCE	IS '連帶最近一次退票日期';
COMMENT ON COLUMN DAS006.IDN_WRITEOFF_AMT	IS '連帶退票已清償註記總金額 (千元)';
COMMENT ON COLUMN DAS006.IDN_WRITEOFF_CNT 	IS '連帶退未已清償註記票總張數';
COMMENT ON COLUMN DAS006.IDN_LATEST_WRITEOFF	IS '連帶最近一次已清償註記註銷日期';

create table jas002 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	ever_delinquent char(1),
	delinquent_date char(7),
	ever_bad_check  char(1),
	bad_check_date  char(7),
	ever_reject     char(1),
	reject_date     char(7),
	ever_stop_card  char(1),
	stop_card_date  char(7)
) IN "ESCORETABSP";
create index i_jas002 on jas002(CASE_SN, IDN);

COMMENT ON TABLE  jas002	IS '信用異常紀錄 (B, D, K)';
COMMENT ON COLUMN jas002.CASE_SN	IS '申請編號';
COMMENT ON COLUMN jas002.IDN	 	IS '申請人身分證號'; 
COMMENT ON COLUMN jas002.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN jas002.ever_delinquent	IS '授信異常 YN';
COMMENT ON COLUMN jas002.delinquent_date	IS '最近授信異常日期';
COMMENT ON COLUMN jas002.ever_bad_check	IS '退票異常 YN';
COMMENT ON COLUMN jas002.bad_check_date	IS '最近退票異常日期';
COMMENT ON COLUMN jas002.ever_reject	IS '拒絕往來記錄 YN';
COMMENT ON COLUMN jas002.reject_date	IS '拒絕往來日期  拒絕往來記錄N本欄位有日期為該戶拒往已解除本欄位日期為拒絕往來日期';
COMMENT ON COLUMN jas002.ever_stop_card	IS '強制停卡異常 YN';
COMMENT ON COLUMN jas002.stop_card_date	IS '最近強制停卡日期';

create table KRM001 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	Card_Brand 	char (1),
	Card_Type 	char (1),
	Issue 		char (3),
	Issue_Name 	char (40),
	Start_Date 	char (7),
	Stop_Date 	char (7),
	Stop_Code 	char (1),
	AB_Code 	char (1),
	M_S 		char (1),
	Limit 		char (6),
	LIMIT_TYPE	char (1),
	USAGE_TYPE	char (1),
	SECURE		char (1)
) IN "ESCORETABSP";
create index i_krm001 on KRM001(CASE_SN, IDN);

COMMENT ON TABLE KRM001	IS '信用卡資訊 (多筆 K01)';
COMMENT ON COLUMN KRM001.CASE_SN	IS '申請編號';
COMMENT ON COLUMN KRM001.IDN	 	IS '申請人身分證號';
COMMENT ON COLUMN KRM001.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN KRM001.CARD_BRAND	IS '卡名代號  (對照表)';
COMMENT ON COLUMN KRM001.CARD_TYPE	IS '標章代號  (對照表)';
COMMENT ON COLUMN KRM001.ISSUE		IS '發卡機構代號';
COMMENT ON COLUMN KRM001.ISSUE_NAME	IS '發卡機構名稱';
COMMENT ON COLUMN KRM001.START_DATE	IS '啟用日期';
COMMENT ON COLUMN KRM001.STOP_DATE	IS '停用日期';
COMMENT ON COLUMN KRM001.STOP_CODE	IS '停用種類代號  (對照表)';
COMMENT ON COLUMN KRM001.AB_CODE 	IS '停用原因代號  (對照表)';
COMMENT ON COLUMN KRM001.M_S		IS '主附卡 (YN)';
COMMENT ON COLUMN KRM001.LIMIT		IS '額度（千元）';
COMMENT ON COLUMN KRM001.LIMIT_TYPE	IS '是否綜合額度 (YN)';
COMMENT ON COLUMN KRM001.USAGE_TYPE 	IS '能否循環使用 (YN)';
COMMENT ON COLUMN KRM001.SECURE		IS '擔保註記 (YN)';


create table KRM023 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	Yrmon 		char (5),
	Issue 		char (3),
	Issue_Name 	char (40),
	KR_Code 	char (7),
	Limit 		char (5),
	Payment 	char (3),
	Cash 		char (1),
	Pay_code 	char (1)
) IN "ESCORETABSP";
create index i_krm023 on KRM023(CASE_SN, IDN);

COMMENT ON TABLE KRM023	IS '信用卡戶繳款資訊（最近１２月） (多筆 K23)';
COMMENT ON COLUMN KRM023.CASE_SN	IS '申請編號';
COMMENT ON COLUMN KRM023.IDN 		IS '申請人身分證號';
COMMENT ON COLUMN KRM023.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN KRM023.YRMON 		IS '資料年月';
COMMENT ON COLUMN KRM023.ISSUE		IS '發卡機構代號';
COMMENT ON COLUMN KRM023.ISSUE_NAME	IS '發卡機構名稱';
COMMENT ON COLUMN KRM023.KR_CODE	IS '信用卡別';
COMMENT ON COLUMN KRM023.LIMIT 		IS '核定額度 (千元)';
COMMENT ON COLUMN KRM023.PAYMENT	IS '應繳金額級距(元)(位數+ +HML)';
COMMENT ON COLUMN KRM023.CASH 		IS '預借現金有無(YN)';
COMMENT ON COLUMN KRM023.PAY_CODE	IS '繳款狀況代號  (對照表)';


create table KRM037 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	idn_ban		char(10),
	bill_date	char(7),
	issue		char(3),
	issue_name 	char(24),
	card_type 	char(7),
	perm_limit 	char(7),
	temp_limit 	char(7),
	cash_limit 	char(7),
	payable 	char(3),
	cash_lent 	char(8),
	last_paya 	char(3),
	revol_bal 	char(9),
	pay_stat 	char(1),
	pay_code 	char(1),
	revol_rate 	char(4),
	pre_owed 	char(8),
	debt_code 	char(1),
	close_code 	char(1),
	clear_date 	char(7)
) IN "ESCORETABSP";
create index i_krm037 on krm037(CASE_SN, IDN);

COMMENT ON TABLE KRM037	IS '信用卡戶帳款及循環比率資訊（最近１２月）(多筆 K33,遇額度合計時,其他文字性欄位空白,數值欄位置0)';
COMMENT ON COLUMN KRM037.CASE_SN	IS '申請編號';
COMMENT ON COLUMN KRM037.IDN 		IS '申請人身分證號';
COMMENT ON COLUMN KRM037.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN KRM037.idn_ban 	IS '繳款人ID';
COMMENT ON COLUMN KRM037.bill_date	IS '結帳日';
COMMENT ON COLUMN KRM037.issue		IS '發卡機構代號';
COMMENT ON COLUMN KRM037.issue_name	IS '發卡機構名稱';
COMMENT ON COLUMN KRM037.card_type	IS '信用卡別';
COMMENT ON COLUMN KRM037.perm_limit	IS '永久額度  單位:千元';
COMMENT ON COLUMN KRM037.temp_limit	IS '臨時額度  單位:千元';
COMMENT ON COLUMN KRM037.cash_limit	IS '預借現金額度  單位:千元';
COMMENT ON COLUMN KRM037.payable	IS '本期應付帳款(級距)';
COMMENT ON COLUMN KRM037.cash_lent	IS '本期預借現金  單位:元';
COMMENT ON COLUMN KRM037.last_paya 	IS '上期應付帳款(級距)';
COMMENT ON COLUMN KRM037.revol_bal	IS '上期循環信用  單位:元';
COMMENT ON COLUMN KRM037.pay_stat	IS '上期繳款狀況代號(金額)  (對照表)';
COMMENT ON COLUMN KRM037.pay_code	IS '上期繳款狀況代號(時間)  (對照表)';
COMMENT ON COLUMN KRM037.revol_rate	IS '上期循環比率REVOL_BAL/PERM_LIMIT,精確度至小數點後2位(四捨五入),非百分比';
COMMENT ON COLUMN KRM037.pre_owed	IS '未到期分期償還待付金額,未到期代墊消費款金額(格式22欄項)+未到期分期償還預借現金金額(格式23欄項),不包括本期新增餘額代償(18欄)';
COMMENT ON COLUMN KRM037.debt_code 	IS '債權狀態註記';
COMMENT ON COLUMN KRM037.close_code	IS '債權結案註記(對照表)';
COMMENT ON COLUMN KRM037.clear_date	IS '不良債權結案日期';

create table STM001 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	Query_Date 	char(7),
	Bank_Code 	char(7),
	Bank_Name 	char(40),
	Item_List 	char(10)
) IN "ESCORETABSP";
create index i_stm001 on STM001(CASE_SN, IDN);

COMMENT ON TABLE STM001 IS '被查詢紀錄 (多筆)';
COMMENT ON COLUMN STM001.CASE_SN	IS '申請編號';
COMMENT ON COLUMN STM001.IDN		IS '申請人身分證號';
COMMENT ON COLUMN STM001.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN STM001.QUERY_DATE	IS '查詢日期';
COMMENT ON COLUMN STM001.BANK_CODE	IS '查詢單位代號';
COMMENT ON COLUMN STM001.BANK_NAME	IS '查詢單位名稱';
COMMENT ON COLUMN STM001.ITEM_LIST	IS '查詢項目串列 B:授信 D:票信 K:信用卡等';


create table STM007 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	query_date 	char(7),
 	bank_code 	char(7),
	bank_name 	char(40),
	item_list 	char(10),
	inq_purpose_1	char(1),
	inq_purpose	char(30)

) IN "ESCORETABSP";
create index i_stm007 on STM007(CASE_SN, IDN);

COMMENT ON TABLE STM007 IS '最近三個月內非Z類產品被查詢紀錄 (多筆)';
COMMENT ON COLUMN STM007.CASE_SN	IS '申請編號';
COMMENT ON COLUMN STM007.IDN		IS '申請人身分證號';
COMMENT ON COLUMN STM007.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN STM007.query_date	IS '查詢日期';
COMMENT ON COLUMN STM007.bank_code	IS '查詢單位代號';
COMMENT ON COLUMN STM007.bank_name	IS '查詢單位名稱';
COMMENT ON COLUMN STM007.item_list	IS '查詢項目串列 B:授信 D:票信 K:信用卡等';
COMMENT ON COLUMN STM007.inq_purpose_1	IS '查詢理由碼';
COMMENT ON COLUMN STM007.inq_purpose 	IS '查詢理由碼(中文註解)1:新業務申請2:原業務往來3:新業務申請及原業務往來';

create table VAM102 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	IDN_BAN 	char(10),
	DATA_DATE 	char(7),
	MAINCODE 	char(1),
	MAINNOTE 	char(36),
	SUBCODE 	char(1),
	SUBNOTE 	char(60),
	NOTES	 	char(100)
) IN "ESCORETABSP";
create index i_vam102 on VAM102(CASE_SN, IDN);

COMMENT ON TABLE VAM102 IS '補充/註記資訊';
COMMENT ON COLUMN VAM102.CASE_SN	IS '申請編號';
COMMENT ON COLUMN VAM102.IDN		IS '申請人身分證號';
COMMENT ON COLUMN VAM102.inquiry_date	IS 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
COMMENT ON COLUMN VAM102.IDN_BAN	IS '統編/身分證號';
COMMENT ON COLUMN VAM102.DATA_DATE	IS '訊息登錄日期';
COMMENT ON COLUMN VAM102.MAINCODE	IS '訊息種類大項代碼  (對照表)';
COMMENT ON COLUMN VAM102.MAINNOTE	IS '訊息種類大項';
COMMENT ON COLUMN VAM102.SUBCODE	IS '訊息種類細項代碼  (對照表)';
COMMENT ON COLUMN VAM102.SUBNOTE	IS '訊息種類細項';
COMMENT ON COLUMN VAM102.NOTES		IS '訊息內容';

-- create INPUT tables
create table app_info (
	Case_SN		char(12) not null,
	system_date	char(7) not null,
	system_time	char(6) not null,
	Applicant_ID	char(10) not null,
	applicant_name	varchar(30) not null,
	Guarantor_ID	char(10),
	Loan_purpose	int not null,
	education	int not null,
	Birthday	char(7) not null,
	Marriage_status	int not null,
	Annual_Income	int not null,
	Home_ownership	int not null,
	Year_at_current_address	int not null,
	Car_brand	varchar(20),
	Car_year	int,
	Car_cc		int,
	Company_name	varchar(60) not null,
	Job_title	varchar(40) not null,
	Year_at_current_job	int not null,
	Product_type_code	int not null,
	App_amt		int not null,
	Term_1		int not null,
	Term_2		int,
	Term_3		int,
	APR_1		decimal(7,4) not null,
	APR_2		decimal(7,4),
	APR_3		decimal(7,4),
	grace_period	int not null,
	Fee_1		int,
	Fee_2		decimal(7,4),
	system_date_final	char(7),
	system_time_final	char(6),
	Approval_final	int,
	Account_No	varchar(20),
	Open_date	char(7),
	Loan_amt	int,
	Loan_APR	decimal(7,4),
	Loan_term	int,
	Fee_1_final	int,
	Fee_2_final	decimal(7,4),
	NPV_final	int,
	CI_key		char(10),
	CI_name		varchar(30),
	CI_branch	varchar(20),
	branch		varchar(20) not null,
	agent		varchar(30) not null,
	errcode		char(4),
	agent_id	varchar(10) not null
) IN "ESCORETABSP";
alter table app_info add constraint p_app_info primary key (CASE_SN);

COMMENT ON TABLE app_info	IS 'DAC個人信貸表格';
COMMENT ON COLUMN app_info.CASE_SN	IS '申請編號(不可重複)';
COMMENT ON COLUMN app_info.system_date	IS '申請輸入系統日期';
COMMENT ON COLUMN app_info.system_time	IS '申請輸入系統時間';
COMMENT ON COLUMN app_info.Applicant_ID	IS '申請人身分證號';
COMMENT ON COLUMN app_info.applicant_name	IS '申請人姓名';
COMMENT ON COLUMN app_info.Guarantor_ID	IS '保證人身分證號';
COMMENT ON COLUMN app_info.Loan_purpose	IS '資金用途';
COMMENT ON COLUMN app_info.education	IS '教育程度';
COMMENT ON COLUMN app_info.Birthday	IS '申請人生日';
COMMENT ON COLUMN app_info.Marriage_status	IS '申請人婚姻狀態';
COMMENT ON COLUMN app_info.Annual_Income	IS '申請人年所得收入';
COMMENT ON COLUMN app_info.Home_ownership	IS '申請人現住房屋狀況';
COMMENT ON COLUMN app_info.Year_at_current_address	IS '申請人現址已居住年數';
COMMENT ON COLUMN app_info.Car_brand	IS '申請人車籍資料(廠牌)';
COMMENT ON COLUMN app_info.Car_year	IS '申請人車籍資料(年份)';
COMMENT ON COLUMN app_info.Car_cc	IS '申請人車籍資料(排氣量)';
COMMENT ON COLUMN app_info.Company_name	IS '申請人任職公司';
COMMENT ON COLUMN app_info.Job_title	IS '申請人職稱';
COMMENT ON COLUMN app_info.Year_at_current_job	IS '申請人現職服務年資';
COMMENT ON COLUMN app_info.Product_type_code	IS '產品種類碼';
COMMENT ON COLUMN app_info.App_amt	IS '申貸金額';
COMMENT ON COLUMN app_info.Term_1	IS '期數(第一期)';
COMMENT ON COLUMN app_info.Term_2	IS '期數(第二期)';
COMMENT ON COLUMN app_info.Term_3	IS '期數(第三期)';
COMMENT ON COLUMN app_info.APR_1	IS '利率(第一期)';
COMMENT ON COLUMN app_info.APR_2	IS '利率(第二期)';
COMMENT ON COLUMN app_info.APR_3	IS '利率(第三期)';
COMMENT ON COLUMN app_info.grace_period	IS '寬限期';
COMMENT ON COLUMN app_info.Fee_1	IS '開辦費(固定金額部分)';
COMMENT ON COLUMN app_info.Fee_2	IS '開辦費(百分比部分)';
COMMENT ON COLUMN app_info.system_date_final	IS '結案輸入系統日期yyymmdd';
COMMENT ON COLUMN app_info.system_time_final	IS '結案輸入系統時間hhmiss';
COMMENT ON COLUMN app_info.Approval_final	IS '最終准駁/撥款決定';
COMMENT ON COLUMN app_info.Account_No	IS '撥款帳號';
COMMENT ON COLUMN app_info.Open_date	IS '撥款日期';
COMMENT ON COLUMN app_info.Loan_amt	IS '撥款金額';
COMMENT ON COLUMN app_info.Loan_APR	IS '撥款利率';
COMMENT ON COLUMN app_info.Loan_term	IS '最終貸款期數';
COMMENT ON COLUMN app_info.Fee_1_final	IS '實收開辦費(固定金額部分)';
COMMENT ON COLUMN app_info.Fee_2_final	IS '實收開辦費(百分比部分)';
COMMENT ON COLUMN app_info.NPV_final	IS '預期經濟價值(依實撥金額重算)';
COMMENT ON COLUMN app_info.CI_key	IS '業務員身分證號';
COMMENT ON COLUMN app_info.CI_name	IS '業務人員姓名';
COMMENT ON COLUMN app_info.CI_branch	IS '業務人員所屬分行';
COMMENT ON COLUMN app_info.branch	IS '經辦分行';
COMMENT ON COLUMN app_info.agent	IS '經辦行員姓名';
COMMENT ON COLUMN app_info.agent_id	IS '經辦行員ID';
COMMENT ON COLUMN app_info.errcode	IS '案件查詢結果';

-- create OUTPUT tables
create table app_result (
	Case_SN		char(12) not null,
	system_time	char(13),
	Applicant_PB	decimal(7,4),
	Guarantor_PB	decimal(7,4),
	Applicant_PB_adj	decimal(7,4),
	NPV		int,
	PS_code		int,
	PS_msg		varchar(256),
	PS_code_g	int,
	PS_msg_g	varchar(256),
	Guarantor_pass	int,
	Fund_deposit_pct	decimal(7,4),
	Fund_bank_pct	decimal(7,4),
	Fund_self_pct	decimal(7,4),
	Fund_free_pct	decimal(7,4),
	COF_deposit	decimal(7,4),
	COF_bank	decimal(7,4),
	ROE		decimal(7,4),
	Commission	decimal(7,4),
	Approval_code	int,
	Approval_msg	varchar(512),
	Lowest_Rate_1	decimal(7,4),
	Lowest_Rate_2	decimal(7,4),
	Lowest_Rate_3	decimal(7,4)
) IN "ESCORETABSP";
alter table app_result add constraint p_app_result primary key (CASE_SN);

COMMENT ON TABLE app_result	IS 'DAC個人信貸模組結果表格';
COMMENT ON COLUMN app_result.CASE_SN	IS '申請編號(不可重複)';
COMMENT ON COLUMN app_result.system_time	IS '模組執行系統時間';
COMMENT ON COLUMN app_result.Applicant_PB	IS '申請人預期壞帳機率';
COMMENT ON COLUMN app_result.Guarantor_PB	IS '保證人預期壞帳機率';
COMMENT ON COLUMN app_result.Applicant_PB_adj	IS '申請人預期壞帳機率(調整後)';
COMMENT ON COLUMN app_result.NPV	IS '預期經濟價值';
COMMENT ON COLUMN app_result.PS_code	IS '申請人預先篩選結果碼';
COMMENT ON COLUMN app_result.PS_msg	IS '申請人預先篩選結果訊息';
COMMENT ON COLUMN app_result.PS_code_g	IS '保證人預先篩選結果碼';
COMMENT ON COLUMN app_result.PS_msg_g	IS '保證人預先篩選結果訊息';
COMMENT ON COLUMN app_result.Guarantor_pass	IS '保證人符合信用標準';
COMMENT ON COLUMN app_result.Fund_deposit_pct	IS '資金來源來自存款百分比';
COMMENT ON COLUMN app_result.Fund_bank_pct	IS '資金來源來自同業拆款百分比';
COMMENT ON COLUMN app_result.Fund_self_pct	IS '資金來源自有百分比';
COMMENT ON COLUMN app_result.Fund_free_pct	IS '資金來源無息百分比';
COMMENT ON COLUMN app_result.COF_deposit	IS '存款平均利率';
COMMENT ON COLUMN app_result.COF_bank	IS '同業拆款利率';
COMMENT ON COLUMN app_result.ROE	IS 'ROE';
COMMENT ON COLUMN app_result.Commission	IS '業務獎金(% of loan Amt)';
COMMENT ON COLUMN app_result.Approval_code	IS '模組准駁建議碼';
COMMENT ON COLUMN app_result.Approval_msg	IS '模組准駁建議訊息';
COMMENT ON COLUMN app_result.Lowest_Rate_1	IS '最低可承作利率(第一期)';
COMMENT ON COLUMN app_result.Lowest_Rate_2	IS '最低可承作利率(第二期)';
COMMENT ON COLUMN app_result.Lowest_Rate_3	IS '最低可承作利率(第三期)';

create table COUNTER (
	SEQ_TYPE	char(01) not null,
	SEQ_NO		numeric(10) not null
) IN "ESCORETABSP";
COMMENT ON TABLE  COUNTER		IS '序號資料表';
COMMENT ON COLUMN COUNTER.SEQ_TYPE	IS '序號類型, W:WEB';
COMMENT ON COLUMN COUNTER.SEQ_NO	IS '序號值';


-- create FINANCIAL tables
create table TCB_FIN_INFO (
	Fund_deposit_pct	decimal(7,4),
	Fund_bank_pct	decimal(7,4),
	Fund_self_pct	decimal(7,4),
	Fund_free_pct	decimal(7,4),
	COF_deposit	decimal(7,4),
	COF_bank	decimal(7,4),
	ROE		decimal(7,4),
	Commission	decimal(7,4)
) IN "ESCORETABSP";

insert into TCB_FIN_INFO (Fund_deposit_pct, Fund_bank_pct, Fund_self_pct,
                             Fund_free_pct, COF_deposit, COF_bank, ROE, Commission)
      values (80.0, 14.0, 3.0, 3.0, 1.26, 1.38, 1.68, 0.0);
CREATE TRIGGER TCB_FIN_UPDATE
  AFTER UPDATE ON TCB_FIN_INFO
  REFERENCING OLD AS O
  FOR EACH ROW MODE DB2SQL
  INSERT INTO TCB_FIN_INFO_LOG
    (SYSTEM_TIME, USER_ID, FUND_DEPOSIT_PCT, FUND_BANK_PCT, FUND_SELF_PCT, FUND_FREE_PCT,
     COF_DEPOSIT, COF_BANK, ROE, COMMISSION)
  VALUES (CURRENT TIMESTAMP, USER, O.FUND_DEPOSIT_PCT, O.FUND_BANK_PCT, O.FUND_SELF_PCT, O.FUND_FREE_PCT,
    O.COF_DEPOSIT, O.COF_BANK, O.ROE, O.COMMISSION);


COMMENT ON TABLE TCB_FIN_INFO	IS 'DAC個人信貸模組財務資訊';
COMMENT ON COLUMN TCB_FIN_INFO.Fund_deposit_pct	IS '資金來源來自存款百分比';
COMMENT ON COLUMN TCB_FIN_INFO.Fund_bank_pct	IS '資金來源來自同業拆款百分比';
COMMENT ON COLUMN TCB_FIN_INFO.Fund_self_pct	IS '資金來源自有百分比';
COMMENT ON COLUMN TCB_FIN_INFO.Fund_free_pct	IS '資金來源無息百分比';
COMMENT ON COLUMN TCB_FIN_INFO.COF_deposit	IS '存款平均利率';
COMMENT ON COLUMN TCB_FIN_INFO.COF_bank		IS '同業拆款利率';
COMMENT ON COLUMN TCB_FIN_INFO.ROE		IS 'ROE';
COMMENT ON COLUMN TCB_FIN_INFO.Commission	IS '業務獎金';


create table TCB_FIN_INFO_LOG (
	user_id		varchar(16),
	system_time	varchar(30),
	Fund_deposit_pct	decimal(7,4),
	Fund_bank_pct	decimal(7,4),
	Fund_self_pct	decimal(7,4),
	Fund_free_pct	decimal(7,4),
	COF_deposit	decimal(7,4),
	COF_bank	decimal(7,4),
	ROE		decimal(7,4),
	Commission	decimal(7,4)
) IN "ESCORETABSP";

COMMENT ON TABLE TCB_FIN_INFO_LOG	IS 'DAC個人信貸模組財務資訊修改紀錄';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.user_id		IS '使用者ID';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.system_time	IS '更改資料之系統時間';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Fund_deposit_pct	IS '資金來源來自存款百分比';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Fund_bank_pct	IS '資金來源來自同業拆款百分比';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Fund_self_pct	IS '資金來源自有百分比';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Fund_free_pct	IS '資金來源無息百分比';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.COF_deposit	IS '存款平均利率';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.COF_bank		IS '同業拆款利率';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.ROE		IS 'ROE';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Commission	IS '業務獎金';

-- Trigger for table TCB_FIN_INFO_LOG need to be added here


-- Create Permanent Working Tables
 create table bam086_dedup (
 	case_sn		char(12),
 	IDN		char(11),
	inquiry_date	char(10),
 	data_yyy	char(3),
 	data_mm		char(2),
 	bank_code	char(7),
 	bank_name	char(40),
 	account_code	char(1),
 	account_code2	char(1),
 	purpose_code	char(1),
 	contract_amt	numeric(10),
 	loan_amt	numeric(10),
 	pass_due_amt	numeric(10),
 	pay_code_12	char(12),
 	co_loan		char(1),
	un_mark         char(1),
	u_yyymmdd       char(8),
	u_rate          numeric(3),
	ib_mark         char(1),
	iab_ban         char(8),
	iab_name        char(60),
	r_yyymmdd       char(7),
	refund_amt      numeric(10),
	ck_ref          char(1),
 	bank_code2	char(3),
	mon_since	int,
	cycle		int,
	now		int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_bam086_dedup on bam086_dedup(CASE_SN, IDN);

 create table bam086_bucket (
 	case_sn		char(12),
 	idn		char(11),
	bank_code	char(7),
	mon_since	int,
	bucket		float
 ) IN "ESCORETABSP";
 create index i_bam086_bucket on bam086_bucket(CASE_SN, IDN);

 create table krm001_dedup (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	card_brand	char(1),
 	card_type	char(1),
 	issue		char(3),
 	issue_name	char(40),
 	start_date	char(7),
 	stop_date	char(7),
 	stop_code	char(1),
 	ab_code		char(1),
 	m_s		char(1),
 	limit		char(6),
	limit_type	char(1),
	usage_type	char(1),
	secure		char(1),
 	start_mon_since int,
 	end_mon_since	int,
	now		int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_krm001_dedup on krm001_dedup(CASE_SN, IDN);

 create table krm023_dedup (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	yrmon		char(5),
 	issue		char(3),
 	issue_name	char(40),
 	kr_code		char(7),
 	limit		char(5),
 	payment		char(3),
 	cash		char(1),
 	pay_code	char(1),
 	mon_since	int,
 	payment_amt	float,
 	bucket_def_1k	int default 0,
 	bucket_ef_1k	int default 0,
 	bucket_f_1k	int default 0,
	now		int,
	curr_inqmon	int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_krm023_dedup on krm023_dedup(CASE_SN, IDN, issue, mon_since);

 create table KRM037_dedup (
 	CASE_SN		char(12) not null,
 	inquiry_date	char(10) not null,
 	IDN		char(10) not null,
 	idn_ban		char(10),
 	bill_date	char(7),
 	issue		char(3),
 	issue_name 	char(24),
 	card_type 	char(7),
 	perm_limit 	char(7),
 	temp_limit 	char(7),
 	cash_limit 	char(7),
 	payable 	char(3),
 	cash_lent 	char(8),
 	last_paya 	char(3),
 	revol_bal 	char(9),
 	pay_stat 	char(1),
 	pay_code 	char(1),
 	revol_rate 	char(4),
 	pre_owed 	char(8),
 	debt_code 	char(1),
 	close_code 	char(1),
 	clear_date 	char(7),
 	mon_since	int,
 	payment_amt	float,
 	bucket_def_1k	int default 0,
 	bucket_ef_1k	int default 0,
 	bucket_f_1k	int default 0,
	now		int,
	curr_inqmon	int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_krm037_dedup on krm037_dedup(CASE_SN, IDN);
 
 create table stm001_dedup (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	query_date	char(7),
 	bank_code	char(7),
 	bank_name	char(40),
 	item_list	char(10),
 	query_mon_since int,
	now		int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_stm001_dedup on stm001_dedup(CASE_SN, IDN);

create table stm007_dedup (
	CASE_SN		char(12) not null,
	IDN		char(10) not null,
	inquiry_date	char(10) not null,
	query_date 	char(7),
 	bank_code 	char(7),
	bank_name 	char(40),
	item_list 	char(10),
	inq_purpose_1	char(1),
	inq_purpose	char(30),
 	query_mon_since int,
	now		int,
 	cnt		int
) IN "ESCORETABSP";
create index i_stm007_dedup on stm007_dedup(CASE_SN, IDN);

 create table jas002_t (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	reason		char(1),
 	date_happen	char(7),
 	mon_since	int
 ) IN "ESCORETABSP";
 create index i_jas002_t on jas002_t(CASE_SN, IDN);

 create table jas002_t_dedup (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	reason		char(1),
 	date_happen	char(7),
 	mon_since	int,
	now		int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_jas002_t_dedup on jas002_t_dedup(CASE_SN, IDN);

 create table pdaco_cal (
	case_sn		char(12),
	idn		char(11),
	inquiry_date	char(10),
	now		int,
	age		int,
	jas002_defect	int default 0,
	krm001_hit	int default 0,
	krm023_hit	int default 0,
	max_bucket	int default 0,
	fs044		int default 0,
	fs334		int default 0,
	cash_max_bucket	int default 0,
	cash_utilization int default 0,
	ind001		int default 0,
	revolving_amt	int default 0,
	fs102_3m	decimal(16,8),
	fs102_6m	decimal(16,8),
	fs102_9m	decimal(16,8),
	ft102_42	decimal(16,8),
	ft102_42_r	decimal(16,8),
	ft102_42_r_tran	decimal(16,8),
	fs031		decimal(16,8),
	fs031_tran	decimal(16,8),
	fs005_1k_3m	decimal(16,8),
	fs005_1k_3m_q	decimal(16,8),
	fs005_1k_3m_q_tran	decimal(16,8),
	fs059_1k_3m	decimal(16,8),
	fs059_1k_6m	decimal(16,8),
	fs059_1k_9m	decimal(16,8),
	ft059_1k_43	decimal(16,8),
	ft059_1k_43_r	decimal(16,8),
	ft059_1k_43_r_tran	decimal(16,8),
	ft059_1k_42	decimal(16,8),
	ft059_1k_42_q	decimal(16,8),
	ft059_1k_42_q_tran	decimal(16,8),
	fs051		decimal(16,8),
	fs051_tran	decimal(16,8),
	fs016_3m	decimal(16,8),
	fs101_3m	decimal(16,8),
	int015_3	decimal(16,8),
	int015_3_tran	decimal(16,8),
	fs205_1k_3m	decimal(16,8),
	fs205_1k_3m_q	decimal(16,8),
	fs205_1k_3m_q_tran	decimal(16,8),
	fs212_1k_3m	decimal(16,8),
	fs212_1k_6m	decimal(16,8),
	ft212_1k_43	decimal(16,8),
	ft212_1k_43_q	decimal(16,8),
	ft212_1k_43_q_tran	decimal(16,8),
	ms117_6m	decimal(16,8),
	fs014_9m	decimal(16,8),
	fs101_9m	decimal(16,8),
	int028_9	decimal(16,8),
	int028_9_tran	decimal(16,8),
	sex		int,
	sex_tran	int,
	app_last_month_bucket	int,
	app_last_month_bucket_tran	int,
	pdaco_score	decimal(16,8),
	pdaco_twen	decimal(16,8),
	pb_in		decimal(16,8),
	npv		decimal(16,2),
	ps_code		int,
	ps_msg		varchar(128),
	cut		decimal(16,8),
	loan_amount	decimal(16,8),
	apr		decimal(16,8),
	term		decimal(16,8),
	amortization_rate	decimal(16,8),
	monthly_payment	decimal(16,8),
	ms047		decimal(16,8),
	ms080		decimal(16,8),
	ms093		decimal(16,8),
	ms094		decimal(16,8),
	ms105		decimal(16,8),
	wi006_9m	decimal(16,8),
	wi006_9mb	decimal(16,8),
	wi008_3m	decimal(16,8),
	wi008_9m	decimal(16,8),
	wi002_12m	decimal(16,8),
	wi004_12m	decimal(16,8),
	ln_35_047	decimal(16,8),
	ln_35_080	decimal(16,8),
	ln_35_006_9mb	decimal(16,8),
	ln_35_008_9m	decimal(16,8),
	ln_35_002_12m	decimal(16,8),
	ln_35_004_12m	decimal(16,8),
	ln_35_004_12m_q	decimal(16,8),
	ln_35_004_12m_q_t1	decimal(16,8),
	ln_35_004_12m_q_t4	decimal(16,8),
	ln_35_002_12m_t1	decimal(16,8),
	ln_35_002_12m_t3	decimal(16,8),
	ln_35_080_t	decimal(16,8),
	ln_35_006_9mb_t1	decimal(16,8),
	ln_35_006_9mb_t4	decimal(16,8),
	ln_35_047_t	decimal(16,8),
	ln_35_080_9m_r	decimal(16,8),
	ln_35_080_9m_r_t2	decimal(16,8),
	ci_score	decimal(16,8),
	cpdaco_score	decimal(16,8),
	reserve1	float,
	reserve2	float,
	reserve3	float,
	reserve4	float,
	reserve5	float,
	reserve6	float,
	reserve7	float,
	reserve8	float,
	reserve9	varchar(20),
	reserve10	varchar(20)
	
 ) IN "ESCORETABSP";
 create index i_pdaco_cal on pdaco_cal(CASE_SN, IDN);

 create table t1 (
    case_sn   char(12),
    mon	      int,
    deliquent_line float
 ) IN "ESCORETABSP";
 create index i_t1 on t1(CASE_SN);

 create table open_card (
    case_sn	char(12),
    issue	char(3),
    now		int,
    mon		int
 ) IN "ESCORETABSP";
 create index i_open_card on open_card(CASE_SN);

 create table open_line (
    case_sn	char(12),
    issue	char(3),
    mon		int,
    now		int,
    cards	int,
    bucket	int
 ) IN "ESCORETABSP";
 create index i_open_line on open_line(CASE_SN);


 create table latest_stmt_mon (
    case_sn	char(12),
    issue	char(3),
    mon		int
 ) IN "ESCORETABSP";
 create index i_latest_stmt_mon on latest_stmt_mon(CASE_SN);


 create table latest_line (
    case_sn	char(12),
    issue	char(3),
    mon		int,
    mob		int
 ) IN "ESCORETABSP";
 create index i_latest_line on latest_line(CASE_SN);

 create table pdaco_result  (
	case_sn		char(12),
	idn		char(11),
	inquiry_date	char(10),
	now		int,
	age		int,
	jas002_defect	int default 0,
	krm001_hit	int default 0,
	krm023_hit	int default 0,
	max_bucket	int default 0,
	fs044		int default 0,
	fs334		int default 0,
	cash_max_bucket	int default 0,
	cash_utilization int default 0,
	ind001		int default 0,
	revolving_amt	int default 0,
	fs102_3m	decimal(16,8),
	fs102_6m	decimal(16,8),
	fs102_9m	decimal(16,8),
	ft102_42	decimal(16,8),
	ft102_42_r	decimal(16,8),
	ft102_42_r_tran	decimal(16,8),
	fs031		decimal(16,8),
	fs031_tran	decimal(16,8),
	fs005_1k_3m	decimal(16,8),
	fs005_1k_3m_q	decimal(16,8),
	fs005_1k_3m_q_tran	decimal(16,8),
	fs059_1k_3m	decimal(16,8),
	fs059_1k_6m	decimal(16,8),
	fs059_1k_9m	decimal(16,8),
	ft059_1k_43	decimal(16,8),
	ft059_1k_43_r	decimal(16,8),
	ft059_1k_43_r_tran	decimal(16,8),
	ft059_1k_42	decimal(16,8),
	ft059_1k_42_q	decimal(16,8),
	ft059_1k_42_q_tran	decimal(16,8),
	fs051		decimal(16,8),
	fs051_tran	decimal(16,8),
	fs016_3m	decimal(16,8),
	fs101_3m	decimal(16,8),
	int015_3	decimal(16,8),
	int015_3_tran	decimal(16,8),
	fs205_1k_3m	decimal(16,8),
	fs205_1k_3m_q	decimal(16,8),
	fs205_1k_3m_q_tran	decimal(16,8),
	fs212_1k_3m	decimal(16,8),
	fs212_1k_6m	decimal(16,8),
	ft212_1k_43	decimal(16,8),
	ft212_1k_43_q	decimal(16,8),
	ft212_1k_43_q_tran	decimal(16,8),
	ms117_6m	decimal(16,8),
	fs014_9m	decimal(16,8),
	fs101_9m	decimal(16,8),
	int028_9	decimal(16,8),
	int028_9_tran	decimal(16,8),
	sex		int,
	sex_tran	int,
	app_last_month_bucket	int,
	app_last_month_bucket_tran	int,
	pdaco_score	decimal(16,8),
	pdaco_twen	decimal(16,8),
	pb_in		decimal(16,8),
	npv		decimal(16,2),
	ps_code		int,
	ps_msg		varchar(128),
	cut		decimal(16,8),
	loan_amount	decimal(16,8),
	apr		decimal(16,8),
	term		decimal(16,8),
	amortization_rate	decimal(16,8),
	monthly_payment	decimal(16,8),
	ms047		decimal(16,8),
	ms080		decimal(16,8),
	ms093		decimal(16,8),
	ms094		decimal(16,8),
	ms105		decimal(16,8),
	wi006_9m	decimal(16,8),
	wi006_9mb	decimal(16,8),
	wi008_3m	decimal(16,8),
	wi008_9m	decimal(16,8),
	wi002_12m	decimal(16,8),
	wi004_12m	decimal(16,8),
	ln_35_047	decimal(16,8),
	ln_35_080	decimal(16,8),
	ln_35_006_9mb	decimal(16,8),
	ln_35_008_9m	decimal(16,8),
	ln_35_002_12m	decimal(16,8),
	ln_35_004_12m	decimal(16,8),
	ln_35_004_12m_q	decimal(16,8),
	ln_35_004_12m_q_t1	decimal(16,8),
	ln_35_004_12m_q_t4	decimal(16,8),
	ln_35_002_12m_t1	decimal(16,8),
	ln_35_002_12m_t3	decimal(16,8),
	ln_35_080_t	decimal(16,8),
	ln_35_006_9mb_t1	decimal(16,8),
	ln_35_006_9mb_t4	decimal(16,8),
	ln_35_047_t	decimal(16,8),
	ln_35_080_9m_r	decimal(16,8),
	ln_35_080_9m_r_t2	decimal(16,8),
	ci_score	decimal(16,8),
	cpdaco_score	decimal(16,8),
	reserve1	float,
	reserve2	float,
	reserve3	float,
	reserve4	float,
	reserve5	float,
	reserve6	float,
	reserve7	float,
	reserve8	float,
	reserve9	varchar(20),
	reserve10	varchar(20)
 ) IN "ESCORETABSP";
