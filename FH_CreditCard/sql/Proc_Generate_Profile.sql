 /*  Generate_Profile */
 CREATE PROCEDURE Generate_Profile
 (@target_month varchar(8))
 AS
 if  NOT exists (select * from dbo.sysobjects where id = object_id(N'[fuhwa_profile]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
   begin
     CREATE TABLE [fuhwa_profile] (
         [Statement_month] [varchar] (8) not null,
         [Low] [int] NULL ,
         [Medium] [int] NULL ,
         [High] [int] NULL ,
         [Not_Scored] [int] NULL
     );
     alter table fuhwa_profile add constraint p_statementmonth primary key (statement_month);
   end
 
 delete from fuhwa_profile where statement_month = @target_month;
 insert into fuhwa_profile (statement_month) values (@target_month);
 update fuhwa_profile
  set low = (select count(*) from vars where risk_level= 'L'),
      Medium = (select count(*) from vars where risk_level= 'M'),
      High = (select count(*) from vars where risk_level= 'H'),
      Not_Scored = (select count(*) from vars where flag in (7, 8, 9, 10));
  
 
 
