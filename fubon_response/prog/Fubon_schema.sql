
if exists (select * from sysobjects where id = object_id(N'Fubon_response_model') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table Fubon_response_model;

if exists (select * from sysobjects where id = object_id(N'Fubon_response_score') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table Fubon_response_score;

if exists (select * from sysobjects where id = object_id(N'bu_customer_bank_bond') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table bu_customer_bank_bond;

if exists (select * from sysobjects where id = object_id(N'bu_customer_bank_fund') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table bu_customer_bank_fund;

if exists (select * from sysobjects where id = object_id(N'bu_customer_futures') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table bu_customer_futures;

if exists (select * from sysobjects where id = object_id(N'bu_customer_insurance_group') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table bu_customer_insurance_group;

if exists (select * from sysobjects where id = object_id(N'bu_customer_insurance_life') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table bu_customer_insurance_life;

if exists (select * from sysobjects where id = object_id(N'bu_customer_investment_trust') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table bu_customer_investment_trust;

if exists (select * from sysobjects where id = object_id(N'bu_customer_property_insure') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table bu_customer_property_insure;

if exists (select * from sysobjects where id = object_id(N'bu_customer_security_bond') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table bu_customer_security_bond;

if exists (select * from sysobjects where id = object_id(N'bu_customer_security_brokage') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table bu_customer_security_brokage;

if exists (select * from sysobjects where id = object_id(N'cc_acct_credit_card') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table cc_acct_credit_card;

if exists (select * from sysobjects where id = object_id(N'cc_credit_card_statements') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table cc_credit_card_statements

if exists (select * from sysobjects where id = object_id(N'cc_party_bank_credit_card') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table cc_party_bank_credit_card

if exists (select * from sysobjects where id = object_id(N'rd_card_type_code') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table rd_card_type_code

if exists (select * from sysobjects where id = object_id(N'rd_credit_card_product_id') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table rd_credit_card_product_id
/*
CREATE TABLE Fubon_response_model (
	Primary_Cardholder_ID char(11),
	start_date datetime,
	end_date datetime,
	max_cycle char(6),
	now int,
	cashADV_ind int,
	revINT_ind int,
	expAMT_ind int,
	Vintage_ind int,
	wm_flag int,
	ins_flag int,
	close_flag int,
	stmt_3 int,
	stmt_6 int,
	stmt_9 int,
	stmt_12 int,
	card varchar(2),
	gender int,
	age int,
	edu int,
	fs003_6 float,
	fs072_9 float,
	fs089_3 float,
	fs096_12 float,
	ms035_12 float,
	ms062_12 float,
	rs001 float,
	fs191_12 float,
	fs204 float,
	ms023_12 float,
	fs197_9 float,
	age_t_r1 float,
	ms035_12_t_r1 float,
	rs001_r float,
	rs001_r_t_r1 float,
	FS089_3_n float,
	FS089_3_n_q float,
	FS089_3_n_q_t_r1 float,
	MS062_12_r float,
	MS062_12_r_t_r1 float,
	FS096_12_n float,
	FS096_12_n_t_r1 float,
	FS072_9_n float,
	FS072_9_n_t_r1 float,
	FS003_6_n float,
	FS003_6_n_r float,
	FS003_6_n_r_t_r1 float,
	rscore_r1 float,
	twentile_r1 float,
	rs001_t_t1 float,
	fs191_12_n float,
	fs191_12_n_r float,
	fs191_12_n_r_t_t1 float,
	FS204_t_t1 float,
	edu_t_t1 float,
	rscore_t1 float,
	twentile_t1 float,
	ms023_12_1k float,
	ms023_12_1k_t2 float,
	FS197_9_n float,
	FS197_9_n_t_t2 float,
	rscore_t2 float,
	twentile_t2 float,
	age_t_n1 float,
	edu_t_n1 float,
	fs204_t_n1 float,
	index7 float,
	rscore_n1 float,
	twentile_n1 float 
);
*/
CREATE TABLE Fubon_response_score (
	Primary_Cardholder_ID char(11) NOT NULL,
	segment varchar(20),
	twentile int,
	score float 
);
create index i_resp_score on Fubon_response_score(Primary_Cardholder_ID);

CREATE TABLE bu_customer_bank_bond (
	customer_id varchar(11) 
);
create index i_bank_bond on bu_customer_bank_bond(customer_id);

CREATE TABLE bu_customer_bank_fund (
	customer_id varchar(11)
);
create index i_bank_fund on bu_customer_bank_fund(customer_id);

CREATE TABLE bu_customer_futures (
	customer_id varchar(11)
);
create index i_futures on bu_customer_futures(customer_id);

CREATE TABLE bu_customer_insurance_group (
	customer_id varchar(11)
);
create index i_insurance_group on bu_customer_insurance_group(customer_id);

CREATE TABLE bu_customer_insurance_life (
	customer_id varchar(11)
);
create index i_insurance_life on bu_customer_insurance_life(customer_id);

CREATE TABLE bu_customer_investment_trust (
	customer_id varchar(11)
);
create index i_investment_trust on bu_customer_investment_trust(customer_id);

CREATE TABLE bu_customer_property_insure (
	customer_id varchar(11)
);
create index i_property_insure on bu_customer_property_insure(customer_id);

CREATE TABLE bu_customer_security_bond (
	customer_id varchar(11)
);
create index i_security_bond on bu_customer_security_bond (customer_id);

CREATE TABLE bu_customer_security_brokage (
	customer_id varchar(11)
);
create index i_security_brokage on bu_customer_security_brokage(customer_id);

CREATE TABLE cc_acct_credit_card (
	Acct_Status_Code char(1),
	Card_Cancel_Date datetime,
	Card_Issue_Date datetime,
	Card_Type_Code char(4),
	Primary_Cardholder_ID varchar(11),
	Product_ID varchar(12)
);
create index i_cc_account on cc_acct_credit_card(Primary_Cardholder_ID);

CREATE TABLE cc_credit_card_statements (
	Billing_Close_Date datetime,
	Last_Payment_Amt int,
	Monthly_Limit_Amt int,
	Primary_Cardholder_ID varchar(11),
	Revolving_Interest_Amt int,
	This_Term_Expenditure_Amt int,
	This_Term_Min_Payment int,
	This_Term_Total_Amt_Receivable int,
	This_Term_Cash_Advance_Amt int
);
create index i_cc_statement on cc_credit_card_statements(Primary_Cardholder_ID);

CREATE TABLE cc_party_bank_credit_card (
	DOB datetime,
	Education_Code char(1),
	Gender_Code char(1),
	Party_ID varchar(11)
);
create index i_cc_partyID on cc_party_bank_credit_card(Party_ID);

CREATE TABLE rd_card_type_code (
	Code char(4),
	Description char(14) 
);
create index i_typecode on rd_card_type_code(code);

CREATE TABLE rd_credit_card_product_id (
	Code char(10),
	Description char(86) 
);
create index i_prodID on rd_credit_card_product_id(code);