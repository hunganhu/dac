if exists (select * from dbo.sysobjects where id = object_id(N'[vars]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [vars]
GO

CREATE TABLE [vars] (
        [Statement_month] [varchar] (8) NULL ,
        [idn] [char] (11) NULL ,
        [PayCode] [char] (1) NULL ,
        [flag] [int] NULL ,
        [month_since] [int] NULL ,
        [AMFS001_03] [decimal](13, 5) NULL default 0.0,
        [AMFS001_09] [decimal](13, 5) NULL default 0.0,
        [AMFS001_12] [decimal](13, 5) NULL default 0.0,
        [AMFT001_41] [decimal](13, 5) NULL default 0.0,
        [AMFT001_41_z] [decimal](13, 5) NULL default 0.0,
        [AMFS003_03] [decimal](13, 5) NULL default 0.0,
        [AMFS003_06] [decimal](13, 5) NULL default 0.0,
        [AMFS003_09] [decimal](13, 5) NULL default 0.0,
        [AMFT003_32] [decimal](13, 5) NULL default 0.0,
        [AMFT003_32_q] [decimal](13, 5) NULL default 0.0,
        [AMFT003_32_q_T] [decimal](13, 5) NULL default 0.0,
        [AMFS005_03] [decimal](13, 5) NULL default 0.0,
        [AMFS005_09] [decimal](13, 5) NULL default 0.0,
        [AMFS005_12] [decimal](13, 5) NULL default 0.0,
        [AMFT005_41] [decimal](13, 5) NULL default 0.0,
        [AMFT005_41_z] [decimal](13, 5) NULL default 0.0,
        [AMFS008_03] [decimal](13, 5) NULL default 0.0,
        [AMFS008_06] [decimal](13, 5) NULL default 0.0,
        [AMFS008_09] [decimal](13, 5) NULL default 0.0,
        [AMFT008_42] [decimal](13, 5) NULL default 0.0,
        [AMFT008_42_z] [decimal](13, 5) NULL default 0.0,
        [AMFS012_03] [decimal](13, 5) NULL default 0.0,
        [AMFS012_09] [decimal](13, 5) NULL default 0.0,
        [AMFS012_06] [decimal](13, 5) NULL default 0.0,
        [AMFT012_42] [decimal](13, 5) NULL default 0.0,
        [AMFT012_42_z] [decimal](13, 5) NULL default 0.0,
        [AMFS013_03] [decimal](13, 5) NULL default 0.0,
        [AMFS013_03_q] [decimal](13, 5) NULL default 0.0,
        [AMFS013_03_q_T] [decimal](13, 5) NULL default 0.0,
        [AMFS013_06] [decimal](13, 5) NULL default 0.0,
        [AMFS013_09] [decimal](13, 5) NULL default 0.0,
        [AMFS013_12] [decimal](13, 5) NULL default 0.0,
        [AMFT013_21] [decimal](13, 5) NULL default 0.0,
        [AMFT013_21_q] [decimal](13, 5) NULL default 0.0,
        [AMFS014_06] [decimal](13, 5) NULL default 0.0,
        [AMFS014_06_q] [decimal](13, 5) NULL default 0.0,
        [AMFS014_06_q_T] [decimal](13, 5) NULL default 0.0,
        [AMFS014_03] [decimal](13, 5) NULL default 0.0,
        [AMFS014_09] [decimal](13, 5) NULL default 0.0,
        [AMFS014_12] [decimal](13, 5) NULL default 0.0,
        [AMFT014_31] [decimal](13, 5) NULL default 0.0,
        [AMFT014_31_q] [decimal](13, 5) NULL default 0.0,
        [AMFT014_31_q_T] [decimal](13, 5) NULL default 0.0,
        [AMFS016_03] [decimal](13, 5) NULL default 0.0,
        [AMFS016_03_q] [decimal](13, 5) NULL default 0.0,
        [AMFS016_06] [decimal](13, 5) NULL default 0.0,
        [AMFS016_09] [decimal](13, 5) NULL default 0.0,
        [AMFT016_32] [decimal](13, 5) NULL default 0.0,
        [AMFT016_32_T] [decimal](13, 5) NULL default 0.0,
        [AMFS022_03] [decimal](13, 5) NULL default 0.0,
        [AMFS022_09] [decimal](13, 5) NULL default 0.0,
        [AMFS022_12] [decimal](13, 5) NULL default 0.0,
        [AMFT022_41] [decimal](13, 5) NULL default 0.0,
        [AMFT022_41_q] [decimal](13, 5) NULL default 0.0,
        [AMFT022_41_q_T] [decimal](13, 5) NULL default 0.0,
        [AMFS023_12] [decimal](13, 5) NULL default 0.0,
        [AMFS023_12_q] [decimal](13, 5) NULL default 0.0,
        [AMFS023_12_q_T] [decimal](13, 5) NULL default 0.0,
        [AMMS036_06] [decimal](13, 5) NULL default 0.0,
        [AMMS036_06_T] [decimal](13, 5) NULL default 0.0,
        [AMMS081_06] [decimal](13, 5) NULL default 0.0,
        [AMMS081_09] [decimal](13, 5) NULL default 0.0,
        [AMMS081_12] [decimal](13, 5) NULL default 0.0,
        [AMMT081_21] [decimal](13, 5) NULL default 0.0,
        [AMMT081_21_z] [decimal](13, 5) NULL default 0.0,
        [score] [float] NULL default 0.0,
        [pd] [float] NULL default 0.0,
        [risk_twentile] [int] NULL,
        [risk_level] [char] (1) NULL
)

insert into vars (statement_month, idn, paycode, flag, month_since)
select statement_month, idn, paycode, flag, month_since
from source
where statement_month='200402';

create unique index u_idn on vars (idn);