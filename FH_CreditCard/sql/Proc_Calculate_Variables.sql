 /*  Calculate_Variables_Scores */
 CREATE PROCEDURE Calculate_Variables_Scores
 AS
 update vars
 set amfs013_03=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 2 and vars.month_since and 
        a.paycode in ('F'))
 where flag in (11, 12);
 update vars
 set amfs013_06=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 5 and vars.month_since and 
        a.paycode in ('F'))
 where flag in (11, 12);
 update vars
 set amfs013_09=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 8 and vars.month_since and 
        a.paycode in ('F'))
 where flag in (11, 12);
 update vars
 set amfs013_12=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 11 and vars.month_since and 
        a.paycode in ('F'))
 where flag in (11, 12);
 update vars
 set AMFS014_06=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since- 5 and vars.month_since and 
        a.paycode in ('E', 'F'))
 where flag in (11, 12);
 update vars
 set AMFS014_03=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 2 and vars.month_since and 
        a.paycode in ('E', 'F'))
 where flag in (11, 12);
 update vars
 set AMFS014_09=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 8 and vars.month_since and 
        a.paycode in ('E', 'F'))
 where flag in (11, 12);
 update vars
 set AMFS014_12=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 11 and vars.month_since and 
        a.paycode in ('E', 'F'))
 where flag in (11, 12);
 update vars
 set amfs016_03=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 2 and vars.month_since and 
        a.late_fee_assessed > 0)
 where flag in (11, 12);
 update vars
 set amfs016_06=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 5 and vars.month_since and 
        a.late_fee_assessed > 0)
 where flag in (11, 12);
 update vars
 set amfs016_09=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 8 and vars.month_since and 
        a.late_fee_assessed > 0)
 where flag in (11, 12);
 update vars
 set AMMS036_06=
 (select  AVG((credit_limit - total_bal_on_bill) / credit_limit)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 5 and vars.month_since)
 where flag in (11, 12) and exists (select * from source where vars.idn=idn and 
       month_since between vars.month_since - 5 and vars.month_since);
 update vars
 set AMFS023_12=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 11 and vars.month_since and 
        a.interest_charge > 0 and
        a.purchase_amount + a.cash_amount = 0)
 where flag in (11, 12);
 update vars
 set AMFS012_03=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 2 and vars.month_since and 
        a.paycode in ('C'))
 where flag in (11, 12);
 update vars
 set AMFS012_09=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 8 and vars.month_since and 
        a.paycode in ('C'))
 where flag in (11, 12);
 update vars
 set AMFS012_06=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 5 and vars.month_since and 
        a.paycode in ('C'))
 where flag in (11, 12);
 update vars
 set AMFS003_03=
 (select  avg(Cash_Advance_count)
  from  source 
  where vars.idn=idn and 
           month_since between vars.month_since - 2 and vars.month_since and 
           cash_advance_count > 0)
 where flag in (11, 12) and exists (select  *
          from  source 
          where vars.idn=idn and 
           month_since between vars.month_since - 2 and vars.month_since and 
           cash_advance_count > 0);
 update vars
 set AMFS003_06=
 (select  avg(Cash_Advance_count)
  from  source 
  where vars.idn=idn and 
           month_since between vars.month_since - 5 and vars.month_since and 
           cash_advance_count > 0)
 where flag in (11, 12) and exists (select  *
          from  source
          where vars.idn=idn and 
           month_since between vars.month_since - 5 and vars.month_since and 
           cash_advance_count > 0);
 update vars
 set AMFS003_09=
 (select  avg(Cash_Advance_count)
  from  source a
  where vars.idn=idn and 
           a.month_since between vars.month_since - 8 and vars.month_since and 
           cash_advance_count > 0)
 where flag in (11, 12) and exists (select  *
       from  source
       where vars.idn=idn and 
           month_since between vars.month_since - 8 and vars.month_since and 
           cash_advance_count > 0);
 update vars
 set AMFS001_03=
 (select  SUM(Cash_Advance_count)
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 2 and vars.month_since and 
           cash_advance_count > 0)
 where flag in (11, 12) and exists (select *
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 2 and vars.month_since and 
           cash_advance_count > 0);
 
 update vars
 set AMFS001_09=
 (select  SUM(Cash_Advance_count)
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 8 and vars.month_since and 
           cash_advance_count > 0)
 where flag in (11, 12) and exists (select *
        from  source
        where vars.idn=idn and 
           month_since between vars.month_since - 8 and vars.month_since and 
           cash_advance_count > 0);
 update vars
 set AMFS001_12=
 (select  SUM(Cash_Advance_count)
  from  source a
  where vars.idn=a.idn and 
           a.month_since between vars.month_since - 11 and vars.month_since and 
           cash_advance_count > 0)
 where flag in (11, 12) and exists (select *
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 11 and vars.month_since and 
           cash_advance_count > 0);
 update vars
 set AMFS005_03=
 (select  SUM(Purchase_count)
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 2 and vars.month_since)
 where flag in (11, 12) and exists (select  *
      from  source
      where vars.idn=idn and 
           month_since between vars.month_since - 2 and vars.month_since);
 update vars
 set AMFS005_09=
 (select  SUM(Purchase_count)
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 8 and vars.month_since)
 where flag in (11, 12) and exists (select *
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 8 and vars.month_since);
 update vars
 set AMFS005_12=
 (select  SUM(Purchase_count)
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 11 and vars.month_since)
 where flag in (11, 12) and exists (select *
  from  source
  where vars.idn=idn and month_since between vars.month_since - 11 and vars.month_since);
 update vars
 set AMFS022_03=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 2 and vars.month_since and
        a.cash_amount > a.purchase_amount)
 where flag in (11, 12);
 update vars
 set AMFS022_09=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 8 and vars.month_since and
        a.cash_amount > a.purchase_amount)
 where flag in (11, 12);
 update vars
 set AMFS022_12=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 11 and vars.month_since and
        a.cash_amount > a.purchase_amount)
 where flag in (11, 12);
 update vars
 set AMFS008_03=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
        a.month_since between vars.month_since - 2 and vars.month_since and
        a.interest_charge > 0)
 where flag in (11, 12);
 update vars
 set AMFS008_06=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
           a.month_since between vars.month_since - 5 and vars.month_since and
           a.interest_charge > 0)
 where flag in (11, 12);
 update vars
 set AMFS008_09=
 (select  count(*)
  from  source a
  where vars.idn=a.idn and 
           a.month_since between vars.month_since - 8 and vars.month_since and
           a.interest_charge > 0)
 where flag in (11, 12);
 update vars
 set AMMS081_06=
 (select  MAX(
    case
      when Purchase_Average_Daily_Balance <> 0 then (Cash_Average_Daily_Balance / Purchase_Average_Daily_Balance)
      else NULL
    end)
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 5 and vars.month_since )
 where flag in (11, 12) and exists (select *
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 5 and vars.month_since );
 update vars
 set AMMS081_09=
 (select  MAX(
    case
      when Purchase_Average_Daily_Balance <> 0 then (Cash_Average_Daily_Balance / Purchase_Average_Daily_Balance)
      else NULL
    end)
  from  source a
  where vars.idn=a.idn and 
           a.month_since between vars.month_since - 8 and vars.month_since )
 where flag in (11, 12) and exists (select *
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 8 and vars.month_since );
 update vars
 set AMMS081_12=
 (select  MAX(
    case
      when Purchase_Average_Daily_Balance <> 0 then (Cash_Average_Daily_Balance / Purchase_Average_Daily_Balance)
      else NULL
    end)
  from  source a
  where vars.idn=a.idn and 
           a.month_since between vars.month_since - 11 and vars.month_since 
  group by a.idn)
 where flag in (11, 12) and exists (select *
  from  source
  where vars.idn=idn and 
           month_since between vars.month_since - 11 and vars.month_since );
 update vars
 set amfs013_03_q = amfs013_03 * amfs013_03,
     AMFT013_21= (AMFS013_09 - AMFS013_06) - (AMFS013_12 - AMFS013_09),
     AMFS014_06_q = AMFS014_06 * AMFS014_06,
     AMFT014_31= (AMFS014_06 - AMFS014_03) - (AMFS014_12 - AMFS014_09),
     AMFT016_32= (AMFS016_06 - AMFS016_03) - (AMFS016_09 - AMFS016_06),
     amfs016_03_q = amfs016_03 * amfs016_03,
     AMFS023_12_q = AMFS023_12 * AMFS023_12,
     AMFT012_42= AMFS012_03 - AMFS012_09 + AMFS012_06,
     AMFT003_32= (AMFS003_06 - AMFS003_03) - (AMFS003_09 - AMFS003_06),
     AMFT001_41= AMFS001_03 - (AMFS001_12 - AMFS001_09),
     AMFT005_41= AMFS005_03 - (AMFS005_12 - AMFS005_09),
     AMFT022_41= AMFS022_03 - (AMFS022_12 - AMFS022_09),
     AMFT008_42= AMFS008_03 - (AMFS008_09 - AMFS008_06),
     AMMT081_21= (AMMS081_09 - AMMS081_06) - (AMMS081_12 - AMMS081_09)
 where flag in (11, 12);
 update vars
 set AMFT013_21_q = AMFT013_21 * AMFT013_21,
     AMFT014_31_q = AMFT014_31 * AMFT014_31,
     AMFT022_41_q = AMFT022_41 * AMFT022_41,
     AMFT003_32_q = AMFT003_32 * AMFT003_32
 where flag in (11, 12);
 update vars
 set amfs013_03_q_T= 
      case
        when amfs013_03_q > 1.125 then 1.125
        else amfs013_03_q
      end,
    AMFS014_06_q_T= 
      case
        when AMFS014_06_q > 9 then 9
        else AMFS014_06_q
      end,
    AMFT014_31_q_T= 
      case
        when AMFT014_31_q < 1.25 then 1.25
        else AMFT014_31_q
      end,
    AMFT016_32_T= 
      case
        when AMFT016_32 < 0 then 0
        else AMFT016_32
      end,
    AMMS036_06_T= 
      case
        when AMMS036_06 < -1 then -1
        else AMMS036_06
      end,
    AMFS023_12_q_T= 
      case
        when AMFS023_12_q > 68 then 100
        else AMFS023_12_q
      end,
    AMFT012_42_z= 
      case
        when AMFT012_42 = 0 then 1
        else 0
      end,
    AMFT003_32_q_T= 
      case
        when AMFT003_32_q > 6 then 6
        else AMFT003_32_q
      end,
    AMFT001_41_z= 
      case
        when AMFT001_41 = 0 then 1
        else 0
      end,
    AMFT005_41_z= 
      case
        when AMFT005_41 = 0 then 1
        else 0
      end,
    AMFT022_41_q_T= 
      case
        when AMFT022_41_q < 2 then 0.25
        else AMFT022_41_q
      end,
    AMFT008_42_z= 
      case
        when AMFT008_42 = 0 then 1
        else 0
      end,
    AMMT081_21_z= 
      case
        when AMMT081_21 = 0 then 1
        else 0
      end
 where flag in (11, 12);
 update vars
 set score = 0.25753 + AMMS036_06_T * -0.18063
           + AMFS014_06_q_T * 0.01938
           + AMFT012_42_z * -0.04845
           + AMMT081_21_z * -0.04484
           + AMFT003_32_q_T * 0.05144
           + AMFT014_31_q_T * 0.01615
           + AMFS016_03_q * 0.02626
           + AMFT001_41_z * -0.0671
           + AMFS023_12_q_T * 0.00061875
           + AMFT016_32_T * 0.03552
           + AMFT005_41_z * -0.03313
           + AMFT022_41_q_T * 0.01453
           + AMFS013_03_q_T * 0.03648
           + AMFT013_21_q * 0.01103
           + AMFT008_42_z * 0.02106
 where flag in (11, 12);