/* Create_Proc_Load_Input_Table */
 CREATE PROCEDURE Load_Input_Table
 (@target_month varchar(8))
 AS
 declare month_cur CURSOR local scroll static for
    select top 13 statement_month
      from statement
      where statement_month <= @target_month
      group by statement_month
      order by statement_month desc;
 open month_cur 
 declare @month varchar(8) 
 fetch next from month_cur into @month 
 while (@@fetch_status = 0)
  begin
     insert into source (Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
       Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
       Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, idn)
-- Below is the sql in DAC env.
     select Statement_month, Account_Status, Inactive_date_for_status_6, Credit_Limit, Purchase_Average_Daily_Balance,
       Cash_Average_Daily_Balance, Purchase_amount, Purchase_count, Cash_amount, Cash_Advance_count, Interest_charge,
       Late_Fee_Assessed, Payment_Amount, Minimum_Payment, __cycles_Past_Due, Total_bal_on_bill, masked_id
-- Below is the sql in Fuhwa env.
--     select [Statement month], [Account Status], [Inactive date for status 6], [Credit Limit], [Purchase Average Daily Balance],
--       [Cash Average Daily Balance], [Purchase amount], [Purchase count], [Cash amount], [Cash Advance count], [Interest charge],
--       [Late Fee Assessed], [Payment Amount], [Minimum Payment], [# cycles Past Due], [Total bal on bill], [Customer id]
     from statement
     where statement_month = @month
     fetch next from month_cur into @month
  end
 close month_cur 
 deallocate month_cur 
