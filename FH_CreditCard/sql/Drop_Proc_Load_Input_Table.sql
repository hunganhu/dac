/* Drop_Proc_Load_Input_Table */
if exists (select * from dbo.sysobjects where id = object_id(N'[Load_Input_Table]')
 and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [Load_Input_Table]
