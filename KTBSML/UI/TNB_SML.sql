use KTB_SML;
go

create table BAM086 (
	msn		    char(14) not null, -- '�ӽнs��';
	inquiry_date	    char(10) not null, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	idn		    char(10) not null, -- '�ӽФH�����Ҹ�';
	data_yyy            char(3),           -- '��Ʀ~��';
	data_mm             char(2),           -- '��Ƥ��';
	bank_code           char(7),           -- '��w�N��';
	bank_name           char(40),          -- '��w�W��';
	account_code        char(1),           -- '�|�p���  (��Ӫ�)';
	account_code2       char(1),           -- '�|�p��ص��O  S ��ܦ���O';
	purpose_code        char(1),           -- '�γ~�O  (��Ӫ�)';
	contract_amt        char(10),          -- '�q�����B (�d��)';
	loan_amt            char(10),          -- '��ڥ��O�� (�d��)';
	pass_due_amt        char(10),          -- '�O�����٪��B (�d��)';
	pay_code_12         char(12),          -- '�̪�12�Ӥ��ٴڬ���  (��Ӫ�)';
	co_loan             char(1),           -- '�@�P�ɴڵ��O  * ��ܦ��@�P�ɴ�';
	un_mark             char(1),           -- '����p�U���O  * ��ܬ�����p�U';
	u_yyymmdd           char(8),           -- '�p�U���  �褸YYYYMMDD';
	u_rate              char(3),           -- '�ѶU���';
	ib_mark             char(1),           -- '����y�V���O  * ��ܬ�����y�V�D�k�H��´';
	iab_ban             char(8),           -- '�D�k�H��´�νs';
	iab_name            char(60),          -- '�D�k�H��´�W��';
	r_yyymmdd           char(7),           -- '�ٴڤ��';
	refund_amt          char(10),          -- '�ٴڪ��B (�d��)';
	ck_ref              char(1)            -- '�ٴڰO��  Y:�����ٲM N:�����ٲM 1.�ٲM 2.�M�v�n���� 3.�M�v�n��Q�� 4.�ॿ�`';
);
create index i_BAM086 on BAM086(msn, idn);
go

create table JAS002 (
	msn		char(14) not null, -- '�ӽнs��';
	inquiry_date	char(10) not null, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	idn		char(10) not null, -- '�ӽФH�����Ҹ�';
	ever_delinquent char(1),           -- '�«H���` YN';
	delinquent_date char(7),           -- '�̪�«H���`���';
	ever_bad_check  char(1),           -- '�h�����` YN';
	bad_check_date  char(7),           -- '�̪�h�����`���';
	ever_reject     char(1),           -- '�ڵ����ӰO�� YN';
	reject_date     char(7),           -- '�ڵ����Ӥ��  �ڵ����ӰO��N����즳������Ӥ�ک��w�Ѱ�����������ڵ����Ӥ��';
	ever_stop_card  char(1),           -- '�j��d���` YN';
	stop_card_date  char(7)            -- '�̪�j��d���';
);
create index i_jas002 on jas002(msn, idn);
go

create table KRM021 (
	msn		char(14) not null, -- '�ӽнs��';
	inquiry_date	char(10) not null, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	idn		char(10) not null, -- '�ӽФH�����Ҹ�';
	card_brand 	char (1),          -- '�d�W�N��  (��Ӫ�)';
	card_type 	char (1),          -- '�г��N��  (��Ӫ�)';
	issue 		char (3),          -- '�o�d���c�N��';
	issue_name 	char (40),         -- '�o�d���c�W��';
	start_date 	char (7),          -- '�ҥΤ��';
	stop_date 	char (7),          -- '���Τ��';
	stop_code 	char (1),          -- '���κ����N��  (��Ӫ�)';
	ab_code 	char (1),          -- '���έ�]�N��  (��Ӫ�)';
	m_s 		char (1),          -- '�D���d (YN)';
	limit 		char (6),          -- '�B�ס]�d���^';
	limit_type	char (1),          -- '�O�_��X�B�� (YN)';
	usage_type	char (1),          -- '��_�`���ϥ� (YN)';
	secure		char (1)           -- '��O���O (YN)';
);
create index i_krm021 on KRM021(msn, idn);
go

create table KRM023 (
	msn		char(14) not null, -- '�ӽнs��';
	inquiry_date	char(10) not null, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	idn		char(10) not null, -- '�ӽФH�����Ҹ�';
	yrmon 		char (5),          -- '��Ʀ~��';
	issue 		char (3),          -- '�o�d���c�N��';
	issue_name 	char (40),         -- '�o�d���c�W��';
	kr_code 	char (7),          -- '�H�Υd�O';
	limit 		char (5),          -- '�֩w�B�� (�d��)';
	payment 	char (3),          -- '��ú���B�ŶZ(��)(���+ +HML)';
	cash 		char (1),          -- '�w�ɲ{�����L(YN)';
	pay_code 	char (1)           -- 'ú�ڪ��p�N��  (��Ӫ�)';
);
create index i_krm023 on KRM023(msn, idn);
go

create table KRM037 (
	msn		char(14) not null, -- '�ӽнs��';
	inquiry_date	char(10) not null, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	idn		char(10) not null, -- '�ӽФH�����Ҹ�';
	idn_ban		char(10),          -- 'ú�ڤHID';
	bill_date	char(7),           -- '���b��';
	issue		char(3),           -- '�o�d���c�N��';
	issue_name 	char(24),          -- '�o�d���c�W��';
	card_type 	char(7),           -- '�H�Υd�O';
	perm_limit 	char(7),           -- '�ä[�B��  ���:�d��';
	temp_limit 	char(7),           -- '�{���B��  ���:�d��';
	cash_limit 	char(7),           -- '�w�ɲ{���B��  ���:�d��';
	payable 	char(3),           -- '�������I�b��(�ŶZ)';
	cash_lent 	char(8),           -- '�����w�ɲ{��  ���:��';
	last_paya 	char(3),           -- '�W�����I�b��(�ŶZ)';
	revol_bal 	char(9),           -- '�W���`���H��  ���:��';
	pay_stat 	char(1),           -- '�W��ú�ڪ��p�N��(���B)  (��Ӫ�)';
	pay_code 	char(1),           -- '�W��ú�ڪ��p�N��(�ɶ�)  (��Ӫ�)';
	revol_rate 	char(4),           -- '�W���`����vREVOL_BAL/PERM_LIMIT,��T�צܤp���I��2��(�|�ˤ��J),�D�ʤ���';
	pre_owed 	char(8),           -- '����������v�٫ݥI���B,������N�Ԯ��O�ڪ��B(�榡22�涵)+����������v�ٹw�ɲ{�����B(�榡23�涵),���]�A�����s�W�l�B�N�v(18��)';
	debt_code 	char(1),           -- '���v���A���O';
	close_code 	char(1),           -- '���v���׵��O(��Ӫ�)';
	clear_date 	char(7)            -- '���}���v���פ��';
);
create index i_krm037 on krm037(msn, idn);
go

create table STM007 (
	msn		char(14) not null, -- '�ӽнs��';
	inquiry_date	char(10) not null, -- 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
	idn		char(10) not null, -- '�ӽФH�����Ҹ�';
	query_date 	char(7),           -- '�d�ߤ��';
 	bank_code 	char(7),           -- '�d�߳��N��';
	bank_name 	char(40),          -- '�d�߳��W��';
	item_list 	char(10),          -- '�d�߶��ئ�C B:�«H D:���H K:�H�Υd��';
	inq_purpose_1	char(1),           -- '�d�߲z�ѽX';
	inq_purpose	char(30)           -- '�d�߲z�ѽX(�������)1:�s�~�ȥӽ�2:��~�ȩ���3:�s�~�ȥӽФέ�~�ȩ���';

);
create index i_stm007 on STM007(msn, idn);
go

create table COUNTER (
	seq_type	char(9) not null,               -- '�Ǹ�����, Ayyyymmdd: A20060412';
	seq_no		numeric(5) not null default 1   -- '�Ǹ���';
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
	marriage	int,        -- 0:���B / 1:�w�B / 2:���B
	child		int,        -- 0 / 1 / 2 / 3:�j��2��
	education	int,        -- 0:��s�� / 1:�j�� / 2:�M�� / 3:���B�ꤤ
	career		varchar(30) not null,
	income		int not null,
	app_amt		int not null,
	period		int not null,
	apr		decimal(8,5) not null,
	app_fee		int not null,
	branch		char(4),     -- �i�����
	agent		char(4),     -- �i����u
-- property1
	owner_id1	char(10) not null,
	owner_name1	varchar(30) not null,
	land_num1	varchar(30) not null,
	lien1		int,           -- 0: �L / 1:�Ĥ@���� / 2:�ĤG����ΥH�W
	first_lien1	int,
	relationship1	int, 	       -- 0:���H / 1:���� / 2;�t�� / 3:�l�k / 4:��L
	location1	int,           -- 0:��v�� / 1:�ӷ~�� / 2:��ӲV�X�� / 3:��L
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
	lien2		int,           -- 0: �L / 1:�Ĥ@���� / 2:�ĤG����ΥH�W
	first_lien2	int,
	relationship2	int,  -- 0:���H / 1:���� / 2;�t�� / 3:�l�k / 4:��L
	location2	int,           -- 0:��v�� / 1:�ӷ~�� / 2:��ӲV�X�� / 3:��L
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
