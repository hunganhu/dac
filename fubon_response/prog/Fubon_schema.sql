
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

if exists (select * from sysobjects where id = object_id(N'card_life') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   drop table card_life;

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
	customer_id varchar(11),
	Acct_Open_Date datetime,
	Most_Current_Outstanding_Bal decimal(18, 0) 
);
create index i_bank_fund on bu_customer_bank_fund(customer_id);

CREATE TABLE bu_customer_futures (
	customer_id varchar(11),
	Acct_Open_Date datetime,
	Acct_Cancel_Date datetime,
	activity_ind char(1),
	position_ind char(1) 
);
create index i_futures on bu_customer_futures(customer_id);

CREATE TABLE bu_customer_insurance_group (
	customer_id varchar(11),
	acct_open_date datetime,
	acct_close_date datetime,
	Group_Policy_Catery_Code char(1),
	Product_Level3_Code char(7) 
);
create index i_insurance_group on bu_customer_insurance_group(customer_id);

CREATE TABLE bu_customer_insurance_life (
	customer_id varchar(11),
	acct_open_date datetime,
	acct_close_date datetime,
	Life_Insurance_Product_Code char(4),
	Product_Level3_Code char(7) 
);
create index i_insurance_life on bu_customer_insurance_life(customer_id);

CREATE TABLE bu_customer_investment_trust (
	customer_id varchar(11),
	Most_Current_Outstanding_Bal decimal(18, 2) 
);
create index i_investment_trust on bu_customer_investment_trust(customer_id);

CREATE TABLE bu_customer_property_insure (
	customer_id varchar(11),
	Acct_Open_Date datetime,
	Acct_Close_Date datetime,
	Acct_Type_Code char(2),
	Product_Level3_Code char(7) 
);
create index i_property_insure on bu_customer_property_insure(customer_id);

CREATE TABLE bu_customer_security_bond (
	customer_id varchar(11),
	Acct_Open_Date datetime,
	most_current_outstanding_bal decimal(18, 2) 
);
create index i_security_bond on bu_customer_security_bond (customer_id);

CREATE TABLE bu_customer_security_brokage (
	customer_id varchar(11),
	Acct_Open_Date datetime,
	Acct_Close_Date datetime,
	Activity_Ind char(1),
	margin_loan_ind char(1),
	treasury_stock_ind char(1) 
);
create index i_security_brokage on bu_customer_security_brokage(customer_id);

CREATE TABLE cc_acct_credit_card (
	Acct_Nbr varchar(11),
	Acct_Status_Code char(1),
	Campaign_Strategy_ID varchar(11),
	Card_Activate_Ind char(1),
	Card_Cancel_Date datetime,
	Card_Cancel_Reason_Code char(2),
	Card_Issue_Date datetime,
	Card_Reissue_Reason_Code char(1),
	Card_Type_Code char(4),
	Cash_Advance_Daily_Limit_Amt int,
	Cash_Advance_Daily_Limit_Cnt smallint,
	Cash_Advance_Monthly_Limit_Amt int,
	Cash_Advance_Monthly_Limit_Cnt smallint,
	Cash_Advance_Single_Txn_Limit int,
	Credit_Card_Level_Code char(1),
	Customer_ID varchar(11),
	Daily_Limit_Amt int,
	Daily_Single_Txn_Limit_Amt int,
	Daily_Txn_Limit_Cnt smallint,
	Last_Txn_Date datetime,
	Monthly_Limit_Amt int,
	Monthly_Txn_Limit_Cnt smallint,
	Nbr_of_Year_Effective char(1),
	Next_Annual_Fee_Payment_Date datetime,
	Primary_Cardholder_ID varchar(11),
	Primary_Credit_Card_Nbr varchar(25),
	Primary_Relation_Type_Code char(2),
	Product_ID varchar(12),
	Secondary_Card_Ind char(1),
	Short_Message_Ind char(1) 
);
create index i_cc_account on cc_acct_credit_card(Primary_Cardholder_ID);

CREATE TABLE cc_credit_card_statements (
	ATM_Fee_Refunded_Ind char(1),
	Annual_Fee int,
	Auto_TFR_Payment_Type_Code char(1),
	Billing_Close_Date datetime,
	Card_Lost_Service_Fee int,
	Cardholder_Level_Type_Code char(1),
	Cardholder_Status_Code char(1),
	Cash_Advance_Daily_Limit_Amt int,
	Cash_Advance_Daily_Limit_Cnt smallint,
	Cash_Advance_Monthly_Limit_Amt int,
	Cash_Advance_Monthly_Limit_Cnt smallint,
	Cash_Advance_Single_Txn_Limit int,
	Credit_Card_Interest_Code char(1),
	Credit_Cycle_Limit_Amt int,
	Credit_Rating_Date datetime,
	Dailiy_Limit_Amt int,
	Daily_Txn_Limit_Cnt smallint,
	Force_Payment_Ind char(1),
	Last_Late_Payment_Interest_Amt int,
	Last_Payment_Amt int,
	Last_Revolving_Interest_Amt int,
	Last_Service_Fee int,
	Last_Unpaid_Expenditure_Amt int,
	Late_Payment_Interest_Amt int,
	Min_Payment_Ratio decimal(5, 2),
	Monthly_Limit_Amt int,
	Monthly_Txn_Limit_Cnt smallint,
	Payment_GT_Min_Amt_Ind char(1),
	Primary_Cardholder_ID varchar(11),
	Revolving_Interest_Amt int,
	Service_Fee int,
	Single_Txn_Limit_Amt int,
	This_Term_Credit_Amt int,
	This_Term_Credit_Rating_Nbr char(2),
	This_Term_Expenditure_Amt int,
	This_Term_Min_Payment int,
	This_Term_Total_Amt_Receivable int,
	This_Term_Cash_Advance_Amt int,
	This_Term_Cash_Advance_cnt smallint 
);
create index i_cc_statement on cc_credit_card_statements(Primary_Cardholder_ID);

CREATE TABLE cc_party_bank_credit_card (
	Adjust_Credit_Limit_Ind char(1),
	Annual_Fee_Waiver_Ind char(1),
	Annual_Income_Amt int,
	Cardholder_Job_Level_Code char(2),
	Cardholder_Occupation_Code char(2),
	Checking_Reject_Ind char(1),
	Cross_Selling_Ind char(1),
	DOB datetime,
	Education_Code char(1),
	Exception_Ind char(1),
	Gender_Code char(1),
	Mail_DM_Ind char(1),
	Mail_Statement_Ind char(1),
	Marital_Status_Code char(1),
	Party_ID varchar(11),
	ROC_Citizen_Ind char(1) 
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