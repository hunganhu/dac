/*
  The database name is TESTDACDB, It may be changed as you wish.
*/
create database TESTDACDB;
go

use TESTDACDB;
go

create table app_info (
  case_sn		char(12) not null,
  idn			char(11) not null,
  dac_sn		decimal(3,0) not null,
  application_date	char(8) not null,
  product_code		varchar(7) not null,
  project_source	varchar(7) null,
  case_source		varchar(7) not null,
  recommender		char(1) not null,
  guarantor		char(1) not null,
  principal		int not null,
  repayment		char(1) not null,
  periods		int not null,
  grace_period		int not null,
  num_int_period	int not null,
  appropriation		char(1) not null,
  zip			char(3) not null,
  segment		varchar(7) not null,
  application_fee	int not null,
  risk_mgmt_fee		int not null,
  acct_mgmt_fee		int not null,
  bt_fee		int not null,
  inquiry_date		char(8) not null,
  early_close_period	int not null,
  ever_bad_check	int,
  ever_reject		int
);
alter table app_info add constraint p_app_info primary key(case_sn, idn, dac_sn); 
go

create table app_i (
  case_sn		char(12) not null,
  idn			char(11) not null,
  dac_sn		decimal(3,0) not null,
  seq			decimal(1,0) not null,
  apr			decimal(5,3) not null,
  periods		int not null
);
alter table app_i add constraint p_app_i primary key (case_sn, idn, dac_sn, seq); 
go

create table app_r (
  case_sn		char(12) not null,
  idn			char(11) not null,
  dac_sn		decimal(3,0) not null,
  decision		int not null,
  final_decision_date	char(8) not null,
  system_date		char(8) not null
);
alter table app_r add constraint p_app_r primary key (case_sn); 
go

create table ploan_result (
  case_sn		char(12) not null,
  idn			char(11) not null,
  dac_sn		decimal(3,0) not null,
  application_date	char(8),
  ev			decimal(16,2),
  pb			decimal(10,6),
  return_msg		varchar(256),
  checksum1		decimal(10,6),
  checksum2		decimal(10,6)
);
alter table ploan_result add constraint p_ploan_result primary key (case_sn, idn, dac_sn); 
go
  
create table bam085 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	data_yyy 	char (5),
	data_mm 	char (2),
	bank_code 	char (7),
	bank_name 	char (40),
	account_code 	char (1),
	account_code2 	char (1),
	purpose_code 	char (1),
	contract_amt 	int,
	loan_amt 	int,
	pass_due_amt 	int,
	pay_code_12 	char (12),
	co_loan 	char (1)
);
create index i_bam085 on bam085(case_sn, idn, dac_sn);
go

create table bas001 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	ever_delinquent	char(1),
	data_yyymmdd	char(7)
);
create index i_bas001 on bas001(case_sn, idn, dac_sn);
go

create table dam103 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	ck_date		char(7),
	issue_bank	char(7),
	bank_name	char(40),
	accnt_no	char(9),
	ck_type		char(1),
	check_no	char(9),
	amt		decimal(10,0),
	cancel_date	char(7),
	company_bounce	char(1),
	resp_id		char(10),
	resp_name	char(60)
);
create index i_dam103 on dam103(case_sn, idn, dac_sn);
go

create table dam203 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	beg_date	char(7),
	end_date	char(7),
	rsn		char(1),
	source		char(1),
	upd_date	char(7),
	relief		char(1),
	s_ban		char(8),
	s_ban_name	char(60)
);
create index i_dam203 on dam203(case_sn, idn, dac_sn);
go

create table krm021 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	card_brand 	char (1),
	card_type 	char (1),
	issue 		char (3),
	issue_name 	char (40),
	start_date 	char (7),
	stop_date 	char (7),
	stop_code 	char (1),
	ab_code 	char (1),
	m_s 		char (1),
	limit 		char (6)
);
create index i_krm021 on krm021(case_sn, idn, dac_sn);
go

create table krm023 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	yrmon 		char (5) not null,
	issue 		char (3) not null,
	issue_name 	char (40),
	kr_code 	char (7),
	limit 		char (5),
	payment 	char (3),
	cash 		char (1),
	pay_code 	char (1)
);
create index i_krm023 on krm023(case_sn, idn, dac_sn);
go

create table krm034 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	idn_ban		char(10),
	bill_date	char(7),
	issue		char(3),
	issue_name	char(24),
	card_type	char(7),
	perm_limit	decimal(7,0),
	cash_yn		char(1),
	last_paya	char(3),
	pay_stat	char(1),
	pay_code	char(1),
	debt_code	char(1),
	close_code	char(1),
	clear_date	char(7)
);
create index i_krm034 on krm034(case_sn, idn, dac_sn);
go

create table krm037 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	idn_ban		char(10),
	bill_date	char(7),
	issue		char(3),
	issue_name	char(24),
	card_type	char(7),
	perm_limit 	decimal(7,0),
	temp_limit	decimal(7,0),
	cash_limit	decimal(7,0),
	payable		char(3),
	cash_lent	decimal(8,0),
	last_paya	char(3),
	revol_bal	decimal(9,0),
	pay_stat	char(1),
	pay_code	char(1),
	revol_rate	decimal(4,0),
	pre_owed	decimal(8,0),
	debt_code	char(1),
	close_code	char(1),
	clear_date	char(7)
);
create index i_krm037 on krm037(case_sn, idn, dac_sn);
go



create table stm001 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	bank_code 	char (7),
	bank_name 	char (40),
	query_date 	char (7),
	item_list 	char (10)
);
create index i_stm001 on stm001(case_sn, idn, dac_sn);
go

create table stm002 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	query_date	char(7),
	bank_code	char(7),
	bank_name	char(40),
	query_item	char(3)
);
create index i_stm002 on stm002(case_sn, idn, dac_sn);
go

create table stm007 (
	case_sn		char(12) not null,
	idn		char(11) not null,
  	dac_sn		decimal(3,0) not null,
	query_date	char(7),
	bank_code	char(7),
	bank_name	char(40),
	item_list	char(10),
	inq_purpose_1	char(1),
	inq_purpose	char(30)
);
create index i_stm007 on stm007(case_sn, idn, dac_sn);
go

create table intermediate (
     case_sn char(12),
     idn char (11),
     dac_sn decimal(3,0),
     inquiry_date char(8),
     x1 decimal (14, 5),	
     x2 decimal (14, 5),
     x3 decimal (14, 5),
     x4 decimal (14, 5),
     x5 decimal (14, 5),
     x6 decimal (14, 5),
     x7 decimal (14, 5),
     x8 decimal (14, 5),
     x9 decimal (14, 5),
     x10 decimal (14, 5),
     x11 decimal (14, 5),
     x12 decimal (14, 5),
     x13 decimal (14, 5),
     x14 decimal (14, 5),
     x15 decimal (14, 5),
     x16 decimal (14, 5),
     x17 decimal (14, 5),
     x18 decimal (14, 5),
     x19 decimal (14, 5),
     x20 decimal (14, 5),
     x21 decimal (14, 5),
     x22 decimal (14, 5),
     x23 decimal (14, 5),
     x24 decimal (14, 5),
     x25 decimal (14, 5),
     x26 decimal (14, 5),
     x27 decimal (14, 5),
     x28 decimal (14, 5),
     x29 decimal (14, 5),
     x30 decimal (14, 5),
     x31 decimal (14, 5),
     x32 decimal (14, 5),
     x33 decimal (14, 5),
     x34 decimal (14, 5),
     x35 decimal (14, 5)
     );
create index i_intermediate on intermediate(case_sn, idn, dac_sn);
go

create table ploan_maintenance (
   cof				decimal(6,3) not null,
   roe				decimal(6,3) not null,
   leverage_ratio		decimal(6,3) not null,
   query_fee			decimal(9,2) not null,
   commission_ratio		decimal(6,3) not null,
   m1_recovery_ratio		decimal(6,3) not null,
   m1_avg_late_days		int not null,
   m1_penalty_rate		decimal(6,3) not null,
   m6_recovery_ratio		decimal(6,3) not null,
   m6_avg_late_days		int not null,
   m6_penalty_rate		decimal(6,3) not null,
   early_closing_fee_pct	decimal(6,3) not null,
   early_closing_fee_collectable_ratio	decimal(6,3) not null,
   early_closing_period		int not null,
   acquisition_data_cost	decimal(9,2) not null,
   acct_mgmt_cost		decimal(9,2) not null,
   short_message_expense	decimal(9,2) not null,
   phone_expense_north		decimal(9,2) not null,
   phone_expense_central	decimal(9,2) not null,
   phone_expense_south		decimal(9,2) not null,
   legal_exec_north		decimal(9,2) not null,
   legal_query_north		decimal(9,2) not null,
   legal_detain_ratio_north	decimal(6,3) not null,
   legal_auction_north		decimal(9,2) not null,
   legal_staff_north		decimal(9,2) not null,
   legal_exec_central		decimal(9,2) not null,
   legal_query_central		decimal(9,2) not null,
   legal_detain_ratio_central	decimal(6,3) not null,
   legal_auction_central	decimal(9,2) not null,
   legal_staff_central		decimal(9,2) not null,
   legal_exec_south		decimal(9,2) not null,
   legal_query_south		decimal(9,2) not null,
   legal_detain_ratio_south	decimal(6,3) not null,
   legal_auction_south		decimal(9,2) not null,
   legal_staff_south		decimal(9,2) not null,
   recovery_ratio		decimal(9,2) not null,
   legal_action_period		int not null
);
go

insert into ploan_maintenance
  (cof, roe, leverage_ratio, commission_ratio, m1_recovery_ratio, m1_avg_late_days, m1_penalty_rate, m6_recovery_ratio, m6_avg_late_days, m6_penalty_rate, early_closing_fee_pct, early_closing_fee_collectable_ratio, early_closing_period,
   acquisition_data_cost, acct_mgmt_cost, short_message_expense, phone_expense_north, phone_expense_central, phone_expense_south, legal_exec_north, legal_query_north, legal_detain_ratio_north, legal_auction_north, legal_staff_north, 
   legal_exec_central, legal_query_central, legal_detain_ratio_central, legal_auction_central, legal_staff_central, legal_exec_south, legal_query_south, legal_detain_ratio_south, legal_auction_south, legal_staff_south, recovery_ratio, legal_action_period, query_fee)
values (1.7, 10.0, 90.0, 35.0, 95.0, 6, 10.0, 2.0, 180, 20.0, 3.0, 50.0, 12,
   210.0, 3.0, 1.36, 39.0, 28.0, 50.0, 963.0, 618.0, 0.56, 800.0, 1500.0,
   1600.0, 700.0, 0.80, 1000.0, 1500.0, 325.0, 535.0, 0.32, 600.0, 1500.0, 25.0, 12, 500.0);
go

create table ploan_maintenance_history (
   system_time			datetime not null,
   [user_id]			varchar(15) not null,
   cof				decimal(6,3) not null,
   roe				decimal(6,3) not null,
   query_fee			decimal(9,2) not null,
   leverage_ratio		decimal(6,3) not null,
   commission_ratio		decimal(6,3) not null,
   m1_recovery_ratio		decimal(6,3) not null,
   m1_avg_late_days		int not null,
   m1_penalty_rate		decimal(6,3) not null,
   m6_recovery_ratio		decimal(6,3) not null,
   m6_avg_late_days		int not null,
   m6_penalty_rate		decimal(6,3) not null,
   early_closing_fee_pct	decimal(6,3) not null,
   early_closing_fee_collectable_ratio	decimal(6,3) not null,
   early_closing_period		int not null, 
   acquisition_data_cost	decimal(9,2) not null,
   acct_mgmt_cost		decimal(9,2) not null,
   short_message_expense	decimal(9,2) not null,
   phone_expense_north		decimal(9,2) not null,
   phone_expense_central	decimal(9,2) not null,
   phone_expense_south		decimal(9,2) not null,
   legal_exec_north		decimal(9,2) not null,
   legal_query_north		decimal(9,2) not null,
   legal_detain_ratio_north	decimal(6,3) not null,
   legal_auction_north		decimal(9,2) not null,
   legal_staff_north		decimal(9,2) not null,
   legal_exec_central		decimal(9,2) not null,
   legal_query_central		decimal(9,2) not null,
   legal_detain_ratio_central	decimal(6,3) not null,
   legal_auction_central	decimal(9,2) not null,
   legal_staff_central		decimal(9,2) not null,
   legal_exec_south		decimal(9,2) not null,
   legal_query_south		decimal(9,2) not null,
   legal_detain_ratio_south	decimal(6,3) not null,
   legal_auction_south		decimal(9,2) not null,
   legal_staff_south		decimal(9,2) not null,
   recovery_ratio		decimal(9,2) not null,
   legal_action_period		int not null
);
go

create trigger trigger_maintenance_delete
  on ploan_maintenance
  instead of delete
as 
  set nocount on
  print 'Sorry - you cannot delete this data'
go

create trigger trigger_maintenance_insert
  on ploan_maintenance
  instead of insert
as
  set nocount on 
  print 'Sorry - you cannot insert this data'
go

create trigger trigger_maintenance_update
  on ploan_maintenance
  after update
as 
  insert into ploan_maintenance_history 
    (system_time, [user_id], cof, roe, query_fee, leverage_ratio, commission_ratio, m1_recovery_ratio, m1_avg_late_days, m1_penalty_rate, m6_recovery_ratio, m6_avg_late_days, m6_penalty_rate, early_closing_fee_pct, early_closing_fee_collectable_ratio,/* early_closing_period,*/ acquisition_data_cost, acct_mgmt_cost, short_message_expense, phone_expense_north, phone_expense_central, phone_expense_south, legal_exec_north, legal_query_north, legal_detain_ratio_north, legal_auction_north, legal_staff_north, legal_exec_central, legal_query_central, legal_detain_ratio_central, legal_auction_central, legal_staff_central, legal_exec_south, legal_query_south, legal_detain_ratio_south, legal_auction_south, legal_staff_south, recovery_ratio, legal_action_period)
  select getdate(), user_name(), cof, roe, query_fee, leverage_ratio, commission_ratio, m1_recovery_ratio, m1_avg_late_days, m1_penalty_rate, m6_recovery_ratio, m6_avg_late_days, m6_penalty_rate, early_closing_fee_pct, early_closing_fee_collectable_ratio,/* early_closing_period,*/ acquisition_data_cost, acct_mgmt_cost, short_message_expense, phone_expense_north, phone_expense_central, phone_expense_south, legal_exec_north, legal_query_north, legal_detain_ratio_north, legal_auction_north, legal_staff_north, legal_exec_central, legal_query_central, legal_detain_ratio_central, legal_auction_central, legal_staff_central, legal_exec_south, legal_query_south, legal_detain_ratio_south, legal_auction_south, legal_staff_south, recovery_ratio, legal_action_period
  from deleted
go

/* create 3 users with different privileges*/
/*
  The 3 users defined here are 'dac', 'gssusr1', 'dacusr2'. They may be changed as your wish. The passwords are strongly recommanded
   to be changed.
  The user dacusr1 is with 'Module execution' privilege. This account is for DAC¡¦s executable code to access the database.
  The user dacusr2 is with 'MIS report' privilege. This account is for Fuhwa¡¦s business or MIS personnel  to generate
      reports from the Module.
  The user gssusr1 is with 'App-processing access' privilege. This account is for GSS to load necessary data prior to calling
      DAC's executable.
 */
use master
go
sp_addlogin @loginame = 'dacusr1', @passwd = 'dacusr1', @defdb = 'TESTDACDB'
go
sp_addlogin @loginame = 'gssusr1', @passwd = 'gssusr1', @defdb = 'TESTDACDB'
go
sp_addlogin @loginame = 'dacusr2', @passwd = 'dacusr2', @defdb = 'TESTDACDB'
go

/*Grant to access TESTDACDB*/
use TESTDACDB
go
sp_grantdbaccess 'dacusr1'
go
sp_grantdbaccess 'gssusr1'
go
sp_grantdbaccess 'dacusr2'
go
/* Grant dac and dacusr2 with ddladmin role*/
sp_addrolemember 'db_ddladmin', 'dacusr1'
go
sp_addrolemember 'db_ddladmin', 'dacusr2'
go

/*grant privileges to dac*/
grant create table, create view to dacusr1;
grant select on app_info to dacusr1;
grant select on app_i to dacusr1;
grant select on app_r to dacusr1;
grant select, update on krm021 to dacusr1;
grant select, update on krm023 to dacusr1;
grant select, update on bam085 to dacusr1;
grant select, update on bas001 to dacusr1;
grant select, update on dam103 to dacusr1;
grant select, update on dam203 to dacusr1;
grant select, update on stm001 to dacusr1;
grant select, insert, delete, update on ploan_result to dacusr1;
grant select, insert, delete, update on intermediate to dacusr1;
grant select, insert, delete, update on ploan_maintenance to dacusr1;
grant select, insert, delete, update on ploan_maintenance_history to dacusr1;
go

/*grant privileges to gss*/
grant create table to gssusr1;
grant select, insert, delete, update on app_info to gssusr1;
grant select, insert, delete, update on app_i to gssusr1;
grant select, insert, delete, update on krm021 to gssusr1;
grant select, insert, delete, update on krm023 to gssusr1;
grant select, insert, delete, update on bam085 to gssusr1;
grant select, insert, delete, update on bas001 to gssusr1;
grant select, insert, delete, update on dam103 to gssusr1;
grant select, insert, delete, update on dam203 to gssusr1;
grant select, insert, delete, update on stm001 to gssusr1;
grant select, insert, delete, update on app_r to gssusr1;
grant select on ploan_result to gssusr1;
go

/*grant privileges to mis*/
grant create table, create view to dacusr2;
grant select on app_info to dacusr2;
grant select on app_i to dacusr2;
grant select on app_r to dacusr2;
grant select on ploan_result to dacusr2;
go
 
