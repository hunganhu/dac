use KTB_SML;
go

create table BAM086 (
	msn		    char(14) not null, -- '申請編號';
	inquiry_date	    char(10) not null, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	idn		    char(10) not null, -- '申請人身分證號';
	data_yyy            char(3),           -- '資料年度';
	data_mm             char(2),           -- '資料月份';
	bank_code           char(7),           -- '行庫代號';
	bank_name           char(40),          -- '行庫名稱';
	account_code        char(1),           -- '會計科目  (對照表)';
	account_code2       char(1),           -- '會計科目註記  S 表示有擔保';
	purpose_code        char(1),           -- '用途別  (對照表)';
	contract_amt        char(10),          -- '訂約金額 (千元)';
	loan_amt            char(10),          -- '放款未逾期 (千元)';
	pass_due_amt        char(10),          -- '逾期未還金額 (千元)';
	pay_code_12         char(12),          -- '最近12個月還款紀錄  (對照表)';
	co_loan             char(1),           -- '共同借款註記  * 表示有共同借款';
	un_mark             char(1),           -- '國際聯貸註記  * 表示為國際聯貸';
	u_yyymmdd           char(8),           -- '聯貸日期  西元YYYYMMDD';
	u_rate              char(3),           -- '參貸比例';
	ib_mark             char(1),           -- '資金流向註記  * 表示為資金流向非法人組織';
	iab_ban             char(8),           -- '非法人組織統編';
	iab_name            char(60),          -- '非法人組織名稱';
	r_yyymmdd           char(7),           -- '還款日期';
	refund_amt          char(10),          -- '還款金額 (千元)';
	ck_ref              char(1)            -- '還款記錄  Y:全部還清 N:部分還清 1.還清 2.清償積欠本息 3.清償積欠利息 4.轉正常';
);
create index i_BAM086 on BAM086(msn, idn);
go

create table JAS002 (
	msn		char(14) not null, -- '申請編號';
	inquiry_date	char(10) not null, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	idn		char(10) not null, -- '申請人身分證號';
	ever_delinquent char(1),           -- '授信異常 YN';
	delinquent_date char(7),           -- '最近授信異常日期';
	ever_bad_check  char(1),           -- '退票異常 YN';
	bad_check_date  char(7),           -- '最近退票異常日期';
	ever_reject     char(1),           -- '拒絕往來記錄 YN';
	reject_date     char(7),           -- '拒絕往來日期  拒絕往來記錄N本欄位有日期為該戶拒往已解除本欄位日期為拒絕往來日期';
	ever_stop_card  char(1),           -- '強制停卡異常 YN';
	stop_card_date  char(7)            -- '最近強制停卡日期';
);
create index i_jas002 on jas002(msn, idn);
go

create table KRM021 (
	msn		char(14) not null, -- '申請編號';
	inquiry_date	char(10) not null, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	idn		char(10) not null, -- '申請人身分證號';
	card_brand 	char (1),          -- '卡名代號  (對照表)';
	card_type 	char (1),          -- '標章代號  (對照表)';
	issue 		char (3),          -- '發卡機構代號';
	issue_name 	char (40),         -- '發卡機構名稱';
	start_date 	char (7),          -- '啟用日期';
	stop_date 	char (7),          -- '停用日期';
	stop_code 	char (1),          -- '停用種類代號  (對照表)';
	ab_code 	char (1),          -- '停用原因代號  (對照表)';
	m_s 		char (1),          -- '主附卡 (YN)';
	limit 		char (6),          -- '額度（千元）';
	limit_type	char (1),          -- '是否綜合額度 (YN)';
	usage_type	char (1),          -- '能否循環使用 (YN)';
	secure		char (1)           -- '擔保註記 (YN)';
);
create index i_krm021 on KRM021(msn, idn);
go

create table KRM023 (
	msn		char(14) not null, -- '申請編號';
	inquiry_date	char(10) not null, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	idn		char(10) not null, -- '申請人身分證號';
	yrmon 		char (5),          -- '資料年月';
	issue 		char (3),          -- '發卡機構代號';
	issue_name 	char (40),         -- '發卡機構名稱';
	kr_code 	char (7),          -- '信用卡別';
	limit 		char (5),          -- '核定額度 (千元)';
	payment 	char (3),          -- '應繳金額級距(元)(位數+ +HML)';
	cash 		char (1),          -- '預借現金有無(YN)';
	pay_code 	char (1)           -- '繳款狀況代號  (對照表)';
);
create index i_krm023 on KRM023(msn, idn);
go

create table KRM037 (
	msn		char(14) not null, -- '申請編號';
	inquiry_date	char(10) not null, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	idn		char(10) not null, -- '申請人身分證號';
	idn_ban		char(10),          -- '繳款人ID';
	bill_date	char(7),           -- '結帳日';
	issue		char(3),           -- '發卡機構代號';
	issue_name 	char(24),          -- '發卡機構名稱';
	card_type 	char(7),           -- '信用卡別';
	perm_limit 	char(7),           -- '永久額度  單位:千元';
	temp_limit 	char(7),           -- '臨時額度  單位:千元';
	cash_limit 	char(7),           -- '預借現金額度  單位:千元';
	payable 	char(3),           -- '本期應付帳款(級距)';
	cash_lent 	char(8),           -- '本期預借現金  單位:元';
	last_paya 	char(3),           -- '上期應付帳款(級距)';
	revol_bal 	char(9),           -- '上期循環信用  單位:元';
	pay_stat 	char(1),           -- '上期繳款狀況代號(金額)  (對照表)';
	pay_code 	char(1),           -- '上期繳款狀況代號(時間)  (對照表)';
	revol_rate 	char(4),           -- '上期循環比率REVOL_BAL/PERM_LIMIT,精確度至小數點後2位(四捨五入),非百分比';
	pre_owed 	char(8),           -- '未到期分期償還待付金額,未到期代墊消費款金額(格式22欄項)+未到期分期償還預借現金金額(格式23欄項),不包括本期新增餘額代償(18欄)';
	debt_code 	char(1),           -- '債權狀態註記';
	close_code 	char(1),           -- '債權結案註記(對照表)';
	clear_date 	char(7)            -- '不良債權結案日期';
);
create index i_krm037 on krm037(msn, idn);
go

create table STM007 (
	msn		char(14) not null, -- '申請編號';
	inquiry_date	char(10) not null, -- 'JCIC 資料查詢日期(yyy/mm/dd, yyy為民國年)';
	idn		char(10) not null, -- '申請人身分證號';
	query_date 	char(7),           -- '查詢日期';
 	bank_code 	char(7),           -- '查詢單位代號';
	bank_name 	char(40),          -- '查詢單位名稱';
	item_list 	char(10),          -- '查詢項目串列 B:授信 D:票信 K:信用卡等';
	inq_purpose_1	char(1),           -- '查詢理由碼';
	inq_purpose	char(30)           -- '查詢理由碼(中文註解)1:新業務申請2:原業務往來3:新業務申請及原業務往來';

);
create index i_stm007 on STM007(msn, idn);
go

create table COUNTER (
	seq_type	char(9) not null,               -- '序號類型, Ayyyymmdd: A20060412';
	seq_no		numeric(5) not null default 1   -- '序號值';
);
go

create table AGENT (
	userid		varchar(32) not null,             -- 'user id';
	userpw		varchar(32),                      -- 'user password';
	username	varchar(32),                      -- 'user name';
	userbranch	varchar(32)                       -- branch name
);
insert into AGENT values ('test', 'test', 'test', 'test');
go

create table app_info (
	msn		char(14) not null,
	system_date	char(14) not null,  -- yyyymmddhhmmss
	applicant_id	char(10) not null,
	applicant_name	varchar(30) not null,
	birthday	char(7) not null,   -- yyymmdd
	zip		char(3),
	marriage	int,        -- 0:未婚 / 1:已婚 / 2:離婚
	child		int,        -- 0 / 1 / 2 / 3:大於2個
	education	int,        -- 0:研究所 / 1:大學 / 2:專科 / 3:高、國中
	career		varchar(30) not null,
	income		int not null,
	app_amt		int not null,
	period		int not null,
	apr		decimal(8,5) not null,
	app_fee		int not null,
	branch		char(4),     -- 進件分行
	agent		char(4),     -- 進件員工
-- property1
	owner_id1	char(10) not null,
	owner_name1	varchar(30) not null,
	land_num1	varchar(30) not null,
	lien1		int,           -- 0: 無 / 1:第一順位 / 2:第二順位或以上
	first_lien1	int,
	relationship1	int, 	       -- 0:本人 / 1:父母 / 2;配偶 / 3:子女 / 4:其他
	location1	int,           -- 0:住宅區 / 1:商業區 / 2:住商混合區 / 3:其他
	land_forbit1	int,           -- 0: NO / 1:YES
	land_demolish1	int,           -- 0: NO / 1:YES
	land_collect1	int,           -- 0: NO / 1:YES
	house_ray1	int,           -- 0: NO / 1:YES
	house_seasand1	int,           -- 0: NO / 1:YES
	house_dangerous1	int,           -- 0: NO / 1:YES
	nav1		float,
	gav1		float,
	house_damage1	int,           -- 0: NO / 1:YES
	house_lent1	int,           -- 0: NO / 1:YES
	house_misuse1	int,           -- 0: NO / 1:YES
	house_complex1	int,           -- 0: NO / 1:YES
	house_basement1	int,            -- 0: NO / 1:YES
-- property2
	owner_id2	char(10),
	owner_name2	varchar(30),
	land_num2	varchar(30),
	lien2		int,           -- 0: 無 / 1:第一順位 / 2:第二順位或以上
	first_lien2	int,
	relationship2	int,  -- 0:本人 / 1:父母 / 2;配偶 / 3:子女 / 4:其他
	location2	int,           -- 0:住宅區 / 1:商業區 / 2:住商混合區 / 3:其他
	land_forbit2	int,           -- 0: NO / 1:YES
	land_demolish2	int,           -- 0: NO / 1:YES
	land_collect2	int,           -- 0: NO / 1:YES
	house_ray2	int,           -- 0: NO / 1:YES
	house_seasand2	int,           -- 0: NO / 1:YES
	house_dangerous2	int,           -- 0: NO / 1:YES
	nav2		float,
	gav2		float,
	house_damage2	int,           -- 0: NO / 1:YES
	house_lent2	int,           -- 0: NO / 1:YES
	house_misuse2	int,           -- 0: NO / 1:YES
	house_complex2	int,           -- 0: NO / 1:YES
	house_basement2	int            -- 0: NO / 1:YES
);
alter table app_info add constraint p_app_info primary key (msn);
go

-- premier review output
create table app_premier (
	msn		char(14) not null,
	premier_date	char(14),      -- yyyymmddhhmmss
        rscore		decimal(8,5),
	premier_code	int,
	premier_msg	varchar(256),
);
alter table app_premier add constraint p_app_premier primary key (msn);
go

-- final review output
create table app_final (
	msn		char(14) not null,
	final_date	char(14),      -- yyyymmddhhmmss
	pb		decimal(8,5),
	npv		int,
	optimal_amount	int,
	audit_agent	char(4),
	final_code	int,
	final_msg	varchar(256)
);
alter table app_final add constraint p_app_final primary key (msn);
go
