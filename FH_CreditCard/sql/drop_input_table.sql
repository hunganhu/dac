/* Drop_Input_Table */
if exists (select * from dbo.sysobjects where id = object_id(N'[source]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
  drop table source;
