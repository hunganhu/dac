 /*  Generate_Score */
 CREATE PROCEDURE Generate_Score
  (@target_month varchar(8))
AS
 if  NOT exists (select * from dbo.sysobjects where id = object_id(N'[fuhwa_score]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   begin
     CREATE TABLE [fuhwa_score] (
         [Statement_month] [varchar] (8) not null,
         [Customer_id] varchar (11) not null,
         [score] [float] NULL ,
         [pd] [float] NULL 
     );
     alter table fuhwa_score add constraint p_month_CustID primary key (statement_month, customer_id);
   end
 
 delete from fuhwa_score where statement_month = @target_month;
  insert into fuhwa_score (statement_month, customer_id, score, pd)
  select statement_month, idn, score, pd
  from vars;
 
 
