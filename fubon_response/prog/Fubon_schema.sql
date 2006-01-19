use DAC_MODEL;
go
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
	Acct_Nbr varchar(25),
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
	Description varchar(60) 
);
create index i_typecode on rd_card_type_code(code);

CREATE TABLE rd_credit_card_product_id (
	Code char(10),
	Description char(86) 
);
create index i_prodID on rd_credit_card_product_id(code);

