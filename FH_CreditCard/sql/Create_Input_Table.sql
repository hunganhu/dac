/* Create_Input_Table */
 CREATE TABLE [source] (
        [Statement_month] [varchar] (8) NULL,
        [Account_Status] [varchar] (3) NULL,
        [Inactive_date_for_status_6] [float] NULL,
        [Credit_Limit] [float] NULL,
        [Purchase_Average_Daily_Balance] [float] NULL,
        [Cash_Average_Daily_Balance] [float] NULL,
        [Purchase_amount] [float] NULL,
        [Purchase_count] [float] NULL,
        [Cash_amount] [float] NULL,
        [Cash_Advance_count] [float] NULL,
        [Interest_charge] [float] NULL,
        [Late_Fee_Assessed] [float] NULL,
        [Payment_Amount] [float] NULL,
        [Minimum_Payment] [float] NULL,
        [__cycles_Past_Due] [float] NULL,
        [Total_bal_on_bill] [float] NULL,
        [PayCode] [char](1) NULL,
        [flag] [int] NULL,
        [month_since] [int] NULL,
        [age] [int] NULL,
        [idn] [float] NULL      	-- In DAC env.
--        [idn] [char] (11) NULL	-- In Fuhwa env.
);
