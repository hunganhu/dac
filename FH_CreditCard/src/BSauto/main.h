//---------------------------------------------------------------------------

#ifndef mainH
#define mainH


#define CONNECTION_STRING  "FILE NAME=./AdoConnect.udl"
#define NSTEPS (sizeof step / sizeof step[0])
#define DEBUG  if (Debug) fprintf
//---------------------------------------------------------------------------



enum SQLCodes { Drop_Input_Table,
                End_of_SQL};

int step[] = {
 Drop_Input_Table,
 End_of_SQL
};

char *SQLNames[]= {"Drop_Input_Table",
                "Drop_Proc_Generate_Score"
 };


char *SQLCommands[] = {
/* Drop_Input_Table */
" if exists (select * from dbo.sysobjects where id = object_id(N'[source_adv]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)"
"    drop table source_adv;"
" if exists (select * from dbo.sysobjects where id = object_id(N'[account_adv]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)"
"    drop table account_adv;",

 };

#endif
