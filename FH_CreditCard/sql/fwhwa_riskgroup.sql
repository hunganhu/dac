update Oliver.credit_card_monthly_pd
  set risk_group =
    (case 
        when pd is null   then 0
        when pd <= 0.0011 then 1
        when pd <= 0.0022 then 2
        when pd <= 0.0035 then 3
        when pd <= 0.0047 then 4
        when pd <= 0.0060 then 5
        when pd <= 0.0079 then 6
        when pd <= 0.0100 then 7
        when pd <= 0.0130 then 8
        when pd <= 0.0180 then 9
        when pd <= 0.0260 then 10
        when pd <= 0.0340 then 11
        when pd <= 0.0440 then 12
        when pd <= 0.0530 then 13
        when pd <= 0.0660 then 14
        when pd <= 0.0810 then 15
        when pd <= 0.1000 then 16
        when pd <= 0.1400 then 17
        when pd <= 0.2300 then 18
        when pd <= 0.3600 then 19
        else 20
     end)


select statement_month, count(*)
from Oliver.credit_card_monthly_pd
where risk_group is null
group by statement_month

select statement_month, count(*)
from Oliver.credit_card_monthly_pd
group by statement_month

select statement_month, count(*)
from dbo.credit_card_month_vars_adv
group by statement_month


alter table dbo.credit_card_month_vars_adv
 add risk_group int


update dbo.credit_card_month_vars_adv
  set risk_group =
    (case 
        when pd is null and flag in (0,1,2,3,4,5,6,8) then 97
        when pd is null and flag in (7) then 99
        when pd is null and flag = 9 then 98
        when pd <= 0.0011 and flag = 11 then 1
        when pd <= 0.0011 and flag = 10 then 2
        when pd <= 0.0011 and flag = 12 then 3
        when pd <= 0.0047 then 4
        when pd <= 0.0100 then 5
        when pd <= 0.0180 then 6
        when pd <= 0.0340 then 7
        when pd <= 0.0440 then 8
        when pd <= 0.0530 then 9
        when pd <= 0.0660 then 10
        when pd <= 0.0810 then 11
        when pd <= 0.1000 then 12
        when pd <= 0.1400 then 13
        when pd <= 0.2300 then 14
        when pd <= 0.3600 then 15
        else 16
     end)

select statement_month, risk_group, sum(pd), count(*)
from dbo.credit_card_month_vars_adv
group by statement_month, risk_group
order by statement_month, risk_group

create table credit_card_monthly_pd_rg (
  cycle_date char (8),
  customer_id char(11),
  flag int,
  pd decimal(7,4),
  risk_group varchar(3)
)

create table credit_card_monthly_profile_rg (
  cycle_date char (8),
  risk_group varchar(3),
  group_cnt int
)


insert into oliver.credit_card_monthly_pd_rg (cycle_date, customer_id, flag, pd)
select statement_month+'01', idn, flag, pd
from fw_score.dbo.credit_card_month_vars_adv

update oliver.credit_card_monthly_pd_rg
  set risk_group =
    (case 
        when pd is null and flag in (0,1,2,3,4,5,6,8) then 97
        when pd is null and flag in (7) then 99
        when pd is null and flag = 9 then 98
        when pd <= 0.0011 and flag = 11 then 1
        when pd <= 0.0011 and flag = 10 then 2
        when pd <= 0.0011 and flag = 12 then 3
        when pd <= 0.0047 then 4
        when pd <= 0.0100 then 5
        when pd <= 0.0180 then 6
        when pd <= 0.0340 then 7
        when pd <= 0.0440 then 8
        when pd <= 0.0530 then 9
        when pd <= 0.0660 then 10
        when pd <= 0.0810 then 11
        when pd <= 0.1000 then 12
        when pd <= 0.1400 then 13
        when pd <= 0.2300 then 14
        when pd <= 0.3600 then 15
        else 16
     end)


insert into oliver.credit_card_monthly_profile_rg (cycle_date, risk_group, group_cnt)
select cycle_date, risk_group, count(*)
from oliver.credit_card_monthly_pd_rg
group by cycle_date, risk_group



