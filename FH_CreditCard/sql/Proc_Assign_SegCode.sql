/* CREATE PROCEDURE Assign_SegCode */
/* change 
    Fuhwa 	 <-> DAC
   [Customer IN] <-> [masked_id]
   [Account Open Date] <-> [account_open_date]
*/

 CREATE PROCEDURE Assign_SegCode
 (@target_month varchar(6))
 AS
 declare @yyyymm int
 declare @yyyy int
 declare @mm int
 declare @next_month varchar(10)
 declare @six_month_ago varchar(10)
 set @yyyymm = cast (@target_month as int)
 set @yyyy = (@yyyymm - @yyyymm % 100 ) / 100
 set @mm = @yyyymm % 100
 if  @mm + 1 > 12
 begin
  set @mm = @mm + 1 - 12
  set @yyyy = @yyyy + 1
 end
 else
  set @mm = @mm + 1
 set @next_month = cast ((@yyyy * 100 + @mm) as char(6)) + '01'
 set @yyyymm = cast (@target_month as int)
 set @yyyy = (@yyyymm - @yyyymm % 100 ) / 100
 set @mm = @yyyymm % 100
 if  @mm < 6
 begin
  set @mm = @mm - 5 + 12
  set @yyyy = @yyyy - 1
 end
 else
  set @mm = @mm - 5
 set @six_month_ago = cast ((@yyyy * 100 + @mm) as char(6)) + '01'
 
 update source
 set flag = 8
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@six_month_ago and
       paycode in ('E', 'F');
 update source
 set flag = 7
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date] >=@six_month_ago and
       paycode not in ('E', 'F');
 update source
 set flag = 9
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
      ( account_status=6 and (inactive_date_for_status_6 <@next_month or inactive_date_for_status_6 is null ));
 update source
 set flag = 10
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode NOT in ('E', 'F') and
       0 = (select SUM(interest_charge)
            from source c
            where b.idn = c.idn and
                  c.month_since between b.month_since - 11 and b.month_since) and
       12<= (select count(*)
             from source c
             where b.idn = c.idn and
                   c.month_since between b.month_since - 11 and b.month_since and
                   paycode='X');
 update source
 set flag = 11
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode NOT in ('E', 'F') and
       0 = (select SUM(interest_charge)
            from source c
            where b.idn = c.idn and
            c.month_since between b.month_since - 11 and b.month_since) and
       12> (select count(*)
            from source c
            where b.idn = c.idn and
                  c.month_since between b.month_since - 11 and b.month_since and
                  paycode='X');
 update source
 set flag = 12
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode NOT in ('E', 'F') and
       0 < (select SUM(interest_charge)
            from source c
            where b.idn = c.idn and
                  c.month_since between b.month_since - 11 and b.month_since);
 update source
 set flag = 0
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       paycode in ('E', 'F');
 update source
 set flag = 1
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 1;
 update source
 set flag = 2
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 2;
 update source
 set flag = 3
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 3;
 update source
 set flag = 4
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 4;
 update source
 set flag = 5
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due = 5;
 update source
 set flag = 6
 from account a, source b
 where a.[masked_id] = b.idn and
       b.statement_month=@target_month and
       a.[account_open_date]<@six_month_ago and
       NOT (account_status=6 and inactive_date_for_status_6 <@next_month) and
       flag = 0 and __cycles_past_due >=6;
