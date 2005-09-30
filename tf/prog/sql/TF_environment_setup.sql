/*
  The database name is TESTDACDB, It may be changed as your wish.
*/
USE master
GO
CREATE DATABASE TESTDACDB
/*
ON 
( NAME = TESTDACDB_dat,
   FILENAME = 'c:\program files\microsoft sql server\mssql\data\testdacdbdat.mdf',
   SIZE = 1GB,
   FILEGROWTH = 5% )
LOG ON
( NAME = 'TESTDACDB_log',
   FILENAME = 'c:\program files\microsoft sql server\mssql\data\testdacdblog.ldf',
   SIZE = 100MB,
   FILEGROWTH = 10MB )
*/
GO

/*
  Create JCIC tables, INPUT tables, OUTPUT tables and MAINTENANCE tables.
*/

use TESTDACDB;
go

/* create JCIC tables */  
create table krm023 (
	app_sn		nvarchar(10) not null,
	data_time	char(8) not null,
	yrmon 		char (5),
	issue 		char (3),
	issue_name 	char (40),
	kr_code 	char (7),
	limit 		char (5),
	payment 	char (3),
	cash 		char (1),
	pay_code 	char (1)
);
create index i_krm023 on krm023(app_sn, data_time);
go

create table krm001 (
	app_sn		nvarchar(10) not null,
	data_time	char (8) not null,
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
create index i_krm001 on krm001(app_sn, data_time);
go

create table bam082 (
	app_sn		nvarchar(10) not null,
	data_time	char (8) not null,
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
create index i_bam082 on bam082(app_sn, data_time);
go

create table bam101 (
	app_sn		nvarchar(10) not null,
	data_time	char(8) not null,
	data_yyy	char(3),
	data_mm		char(2),
	wecdj_amt	int,
	higz_amt	int,
	mnstrk_amt	int,
	u_amt		int,
	pqx_amt		int,
	f_amt		int,
	l_amt		int,
	normal_loan_amt int,
	pass_due_amt	int,
	collection_amt	int
);
go
create index i_bam101 on bam101(app_sn, data_time);
go

create table jas002 (
	app_sn		nvarchar(10) not null,
	data_time	char(8) not null,
	ever_delinquent	char(1),
	delinquent_date	char(7),
	ever_bad_check	char(1),
	bad_check_date	char(7),
	ever_reject	char(1),
	reject_date	char(7),
	ever_stop_card	char(1),
	stop_card_date	char(7)

);
create index i_jas002 on jas002(app_sn, data_time);
go

create table stm001 (
	app_sn		nvarchar(10) not null,
	data_time	char(8) not null,
	query_date 	char (7),
	bank_code 	char (7),
	bank_name 	char (40),
	item_list 	char (10)
);
create index i_stm001 on stm001(app_sn, data_time);
go

/* create INPUT tables */  
create table ts (
	app_sn		nvarchar(10) not null,
	data_time	char(8) not null,
	loan1_payment1	int,
	loan1_payment2	int,
	loan1_payment3	int,
	loan1_payment4	int,
	loan1_payment5	int,
	loan1_payment6	int,
	loan2_payment1	int,
	loan2_payment2	int,
	loan2_payment3	int,
	loan2_payment4	int,
	loan2_payment5	int,
	loan2_payment6	int,
	loan3_payment1	int,
	loan3_payment2	int,
	loan3_payment3	int,
	loan3_payment4	int,
	loan3_payment5	int,
	loan3_payment6	int
);
alter table ts add constraint p_ts primary key (app_sn, data_time); 
go

create table app_info (
	app_sn		nvarchar(10) not null,
	data_time	char(12) not null,
	product_type	int not null check (product_type in (1, 2)),
	gender		int not null check (gender in (0, 1)),
	zip		char(3) not null,
	secretive	int not null check (secretive in (0, 1)),
	education_level	int not null check (education_level in (1, 2, 3, 4, 5, 6)),
	marriage_status	int not null check (marriage_status in (1, 2, 3, 4)),
	alien		int not null check (alien in (0, 1)),
	age_over_limit	int not null check (age_over_limit in (0, 1)),
	ts_cashcard_restricted	int not null check (ts_cashcard_restricted in (0, 1)),
	cof		decimal(5,4) not null check (cof > 0),
	roe		decimal(5,4) not null check (roe > 0),
	ts_tax_rate	decimal(5,4) not null check (ts_tax_rate > 0),
	tf_tax_rate	decimal(5,4) not null check (tf_tax_rate > 0),
	info_processing_cost	int not null check (info_processing_cost > 0),
	operation_cost	int not null check (operation_cost > 0),
	hr_cost		int not null check (hr_cost > 0),
	risk_level	int not null check (risk_level in (1, 2)),
	sales_channel	char(3) not null check (sales_channel in ('001', '002', '003',	                                                          '004', '005', '006',	                                                          '007', '008', '009')),
	commission	int not null check (commission >= 0),
	add_loan	nvarchar(10) not null,
	promotion_code	nvarchar(10) not null,
	sales_region	nvarchar(20) not null,
	sales_unit	nvarchar(20) not null,
	sales_userno	nvarchar(10) not null
);
alter table app_info add constraint p_app_info primary key(app_sn, data_time); 
go

create table loan_condition (
	app_sn		nvarchar(10) not null,
	tsn		char(12) not null,
	loan_amount	int not null check (loan_amount > 0),
	apr		float not null check (apr >= 0.15 and apr <=0.2),
	terms		int not null check (terms > 0),
	application_fee	int not null check (application_fee >= 0),
	credit_checking_fee	int not null check (credit_checking_fee >= 0),
	risk_mgmt_fee	int not null check (risk_mgmt_fee >= 0),
	risk_mgmt_fee_terms	int not null default 0 check (risk_mgmt_fee_terms >= 0),
	teaser_rate	decimal(5,4) not null check (teaser_rate >= 0),
	teaser_period	int not null check (teaser_period >= 0),
	grace_period	int not null check (grace_period >= 0)
	check (teaser_period < terms),
	check (grace_period < terms)
);
alter table loan_condition add constraint p_loan_condition primary key (app_sn, tsn); 
go

/* create OUTPUT tables */
create table prescreen (
	app_sn		nvarchar(10) not null,
	jcic_data_date	char(8) not null,
	app_data_time	char(12) not null,
	product_type	int not null,
	reason_code	int not null,
	reason_message	char(256) not null
);
alter table prescreen add constraint p_prescreen primary key (app_sn, jcic_data_date, app_data_time); 
go

create table approval_cal (
	app_sn		nvarchar(10) not null,
	tsn		char(12) not null,
	ts_data_date	char(8) not null,
	jcic_data_date	char(8) not null,
	app_data_time	char(12) not null,
	product_type	int not null check (product_type in (1, 2)),
	optimal_amount	int,
	specific_lending_amount	int,
	pb		float not null,
	npv		int not null,
	optimal		int not null check (Optimal in (0, 1)),
	reason_code	int not null,
	reason_message	char(256) not null,
	ext_monthly_payment int not null check (ext_monthly_payment >= 0)
);
alter table approval_cal add constraint p_approval primary key (app_sn, tsn, ts_data_date, jcic_data_date, app_data_time); 
go

create table decision_cal (
	app_sn		nvarchar(10) not null,
	tsn		char(12) not null,
	ts_data_date	char(8) not null,
	jcic_data_date	char(8) not null,
	app_data_time	char(12) not null,
	product_type	int not null check (product_type in (1, 2)),
	decision	int not null check (decision in (0, 1)),
	execution_time	char(12) not null,
	pb		float,
	npv		int,
	approved_amount	int not null check (approved_amount >= 0),
	reason_code	int not null,
	ext_monthly_payment	int not null check (ext_monthly_payment >= 0),
	audit_userno1	nvarchar(10) not null,
	change_code	nvarchar(10) not null,
	major_deviation_code	nvarchar(10) not null,
	minor_deviation_code 	nvarchar(10) not null,
	decline_code	nvarchar(10) not null,
	manual_code	nvarchar(10) not null,
	reason_message	char(256) not null
);
alter table decision_cal add constraint p_decision primary key (app_sn, execution_time); 
go


/* create 3 users with different privileges*/
/*
  The 3 users defined here are 'dacusr1', 'csusr1', 'dacusr2'. They may be changed as your wish.
  The passwords are strongly recommanded to be changed.
  The user dacusr1 is with 'Module execution' privilege. This account is for DAC¡¦s executable code to access the database.
  The user dacusr2 is with 'MIS report' privilege. This account is for TF¡¦s business or MIS personnel  to generate
		reports from the Module.
  The user csusr1 is with 'App-processing access' privilege. This account is for CyberSoft to load necessary data prior to calling
		DAC's executable.
 */
use master
go
sp_addlogin @loginame = 'dacusr1', @passwd = 'dacusr1', @defdb = 'TESTDACDB'
go
sp_addlogin @loginame = 'csusr1', @passwd = 'csusr1', @defdb = 'TESTDACDB'
go
sp_addlogin @loginame = 'dacusr2', @passwd = 'dacusr2', @defdb = 'TESTDACDB'
go

/*Grant to access TESTDACDB*/
use TESTDACDB
go
sp_grantdbaccess 'dacusr1'
go
sp_grantdbaccess 'csusr1'
go
sp_grantdbaccess 'dacusr2'
go
/* Grant dac and dacusr2 with ddladmin role*/
sp_addrolemember 'db_ddladmin', 'dacusr1'
go
sp_addrolemember 'db_ddladmin', 'dacusr2'
go

/*grant privileges to dacusr1*/
grant create table, create view to dacusr1;
grant select on ts to dacusr1;
grant select on app_info to dacusr1;
grant select on loan_condition to dacusr1;
grant select on krm023 to dacusr1;
grant select on krm001 to dacusr1;
grant select on bam082 to dacusr1;
grant select on bam101 to dacusr1;
grant select on jas002 to dacusr1;
grant select on stm001 to dacusr1;
grant select, insert on prescreen to dacusr1;
grant select, insert on approval_cal to dacusr1;
grant select, insert on decision_cal to dacusr1;
go

/*grant privileges to cybersoft*/
grant create table to csusr1;
grant select, insert on ts to csusr1;
grant select, insert on app_info to csusr1;
grant select, insert on loan_condition to csusr1;
grant select, insert on krm023 to csusr1;
grant select, insert on krm001 to csusr1;
grant select, insert on bam082 to csusr1;
grant select, insert on bam101 to csusr1;
grant select, insert on jas002 to csusr1;
grant select, insert on stm001 to csusr1;
grant select on prescreen to csusr1;
grant select on approval_cal to csusr1;
grant select on decision_cal to csusr1;
go

/*grant privileges to mis*/
grant create table, create view to dacusr2;
grant select on ts to dacusr2;
grant select on app_info to dacusr2;
grant select on loan_condition to dacusr2;
grant select on krm023 to dacusr2;
grant select on krm001 to dacusr2;
grant select on bam082 to dacusr2;
grant select on bam101 to dacusr2;
grant select on jas002 to dacusr2;
grant select on stm001 to dacusr2;
grant select on prescreen to dacusr2;
grant select on approval_cal to dacusr2;
grant select on decision_cal to dacusr2;
go
 
