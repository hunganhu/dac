 /*  Assign_ALL_PayCode */
 CREATE PROCEDURE Assign_ALL_PayCode
 AS
 declare month_cur CURSOR local scroll static for
    select distinct statement_month
      from source
      order by statement_month asc;
 open month_cur
 declare @month varchar(8)
 fetch next from month_cur into @month
 fetch next from month_cur into @month 
 while (@@fetch_status = 0)
  begin
 update source set paycode = 'A'
 from source as a
 where statement_month = @month and 
      exists ( select idn
              from source as b
              where a.idn = b.idn and 
                    month_since = a.month_since - 1 and
                    minimum_payment = 0 and
                    (purchase_average_daily_balance + cash_average_daily_balance <> 0 or 
                    purchase_amount + cash_amount <> 0) );
 update source set paycode = 'X'
 from source as a
 where statement_month = @month and 
      exists ( select idn
              from source as b
              where a.idn = b.idn and 
                    month_since = a.month_since - 1 and
                    minimum_payment = 0 and
                    purchase_average_daily_balance + cash_average_daily_balance = 0 and 
                    purchase_amount + cash_amount = 0 );
 update source set paycode = 'X'
 from source as a
 where statement_month = @month and 
      not exists ( select idn
              from source as b
              where a.idn = b.idn and 
                    month_since = a.month_since - 1 );
 update source set paycode = 'F'
 from source as a
 where statement_month = @month and
      exists ( select idn
              from source as b
              where a.idn = b.idn and 
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and 
                    payment_amount = 0);
 update source set paycode = 'E'
 from source as a
 where statement_month = @month and 
      exists ( select idn
              from source as b
              where a.idn = b.idn and 
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and 
                    payment_amount <> 0 and
                    payment_amount <  minimum_payment);
 update source set paycode = 'D'
 from source as a
 where statement_month = @month and 
      interest_charge > 0 and 
      late_fee_assessed > 0 and
      exists ( select idn
              from source as b
              where a.idn = b.idn and 
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and 
                    payment_amount <> 0 and
                    payment_amount >= minimum_payment);
 update source set paycode = 'C'
 from source as a
 where statement_month = @month and 
      interest_charge > 0 and 
      late_fee_assessed <= 0 and
      exists ( select idn
              from source as b
              where a.idn = b.idn and 
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and 
                    payment_amount <> 0 and
                    payment_amount >= minimum_payment);
 update source set paycode = 'B'
 from source as a
 where statement_month = @month and 
      interest_charge <= 0 and 
      late_fee_assessed > 0 and
      exists ( select idn
              from source as b
              where a.idn = b.idn and 
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and 
                    payment_amount <> 0 and
                    payment_amount >= minimum_payment);
 update source set paycode = 'A'
 from source as a
 where statement_month = @month and 
      interest_charge <= 0 and 
      late_fee_assessed <= 0 and
      exists ( select idn
              from source as b
              where a.idn = b.idn and 
                    month_since = a.month_since - 1  and
                    minimum_payment <> 0 and 
                    payment_amount <> 0 and
                    payment_amount >= minimum_payment);
     fetch next from month_cur into @month
  end
 close month_cur
 deallocate month_cur

