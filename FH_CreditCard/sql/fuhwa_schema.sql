CREATE TABLE [Account] (
	[Customer ID] [char] (11) NOT NULL,
	[Account ID] [char] (16) NOT NULL,
	[Account Type] [char] (1) NOT NULL,
	[Account Open Date] [float] NOT NULL,
	[Account Close Date] [float] NULL,
	[Cycle Date] [float] NOT NULL,
	[Secured Amount] [int] NOT NULL,
	[Channel] [char] (3) NOT NULL,
	[# of payment terms] [int] NULL,
	[Initial Loan Amount] [int] NULL,
	[Setup Fee] [int] NULL,
	[Maintenance Fee] [int] NOT NULL,
	[Maintenance Fee Term] [int] NULL,
	[Interest Rate] [float] NOT NULL,
	[Birthday] [float] NOT NULL,
	[Resident Phone] [char] (14) NOT NULL,
	[Residence City / Province] [char] (14) NOT NULL,
	[Residence ZIP] [char] (5) NOT NULL,
	[Gender] [char] (1) NOT NULL,
	[Annual Income] [int] NULL,
	[Home Ownership] [char] (1) NULL,
	[Educational Level] [char] (1) NULL,
	[Marital status] [char] (1) NULL,
	[Occupation Status] [char] (1) NULL,
	[# months in current residence] [int] NULL,
	[# months with current occupation] [int] NULL 
);
ALTER TABLE Account ADD CONSTRAINT P_Account PRIMARY KEY ([Customer ID]);


CREATE TABLE [statement] (
	[Customer ID] [char] (11)  NOT NULL,
	[Account ID] [char] (16)  NOT NULL,
	[Statement month] [char] (6)  NOT NULL,
	[Account Type] [char] (1) NOT NULL,
	[Total # open cards with Fuhwa] [int] NOT NULL,
	[Account Status] [char] (1)  NOT NULL,
	[Inactive date for status 6] [int] NULL,
	[APR] [float] NOT NULL,
	[Credit Limit] [int] NOT NULL,
	[Cash Credit Limit] [int] NOT NULL,
	[Purchase Average Daily Balance] [float] NOT NULL,
	[Cash Average Daily Balance] [float] NOT NULL,
	[Purchase balance] [int] NOT NULL,
	[Cash balance] [int] NOT NULL,
	[Purchase amount] [int] NOT NULL,
	[Purchase count] [int] NOT NULL,
	[Purchase Credited Amount] [int] NOT NULL,
	[Cash amount] [int] NOT NULL,
	[Cash Advance count] [int] NOT NULL,
	[Cash Credited Amount] [int] NULL,
	[other x-sell purchase amount] [int] NULL,
	[other x-sell purchase count] [int] NULL,
	[other x-sell purchase Credited amount] [int] NULL,
	[Offshore purchase amount] [int] NULL,
	[Offshore purchase count] [int] NULL,
	[Offshore purchase Credited amount] [int] NULL,
	[Offshore Cash amount] [int] NULL,
	[Offshore Cash count] [int] NULL,
	[Offshore Cash Credited amount] [int] NULL,
	[Interest charge] [int] NOT NULL,
	[Interest charge credited] [int] NULL,
	[Cash Advance Fee Assessed] [int] NOT NULL,
	[Cash Advance Fee Credited] [int] NULL,
	[Annual Fee Assessed] [int] NOT NULL,
	[Annual Fee Credited] [int] NULL,
	[Late Fee Assessed] [int] NOT NULL,
	[Late Fee Credited] [int] NULL,
	[Other Fee Assessed] [int] NOT NULL,
	[Other Fee Credited] [int] NULL,
	[Other Fee count] [int] NOT NULL,
	[Fraud Credit Amount] [int] NULL,
	[Fraud Cash Credit Amount] [int] NULL,
	[Bonus Points claimed] [int] NULL,
	[Unclaimed Bonus Points balance] [int] NULL,
	[Rebate (cash)] [int] NOT NULL,
	[Payment due date] [int] NOT NULL,
	[Payment amount] [int] NOT NULL,
	[Payment count] [int] NOT NULL,
	[Last payment date] [int] NULL,
	[Minimum Payment] [int] NOT NULL,
	[# cycles Past Due] [int] NOT NULL,
	[Total bal on bill] [int] NOT NULL 
);
ALTER TABLE [statement] ADD CONSTRAINT P_Statement PRIMARY KEY ([Statement month], [Customer ID]);
ALTER TABLE [statement] ADD CONSTRAINT C_credit_limit CHECK ([Credit Limit] > 0);
ALTER TABLE [statement] ADD CONSTRAINT F_CustID FOREIGN KEY ([Customer ID]) REFERENCES [Account] ([Customer ID]);

CREATE TABLE [credit_card_monthly_profile] (
   [Statement_month] [varchar] (8) not null,
   [Low] [int] NULL,
   [Medium] [int] NULL,
   [High] [int] NULL,
   [Not_Scored] [int] NULL
);
ALTER TABLE [credit_card_monthly_profile] ADD CONSTRAINT [p_stmt_month] PRIMARY KEY ([Statement_month]);

CREATE TABLE [credit_card_monthly_pd] (
   [Statement_month] [varchar] (8) NOT NULL,
   [Customer_id] varchar (11) NOT NULL,
   [PD] [float] NULL 
 );
ALTER TABLE [credit_card_monthly_pd] ADD CONSTRAINT [p_month_CustID] PRIMARY KEY ([Statement_month], [Customer_ID]);
