//---------------------------------------------------------------------------

#ifndef BSautoSQLH
#define BSautoSQLH
//---------------------------------------------------------------------------
enum SQLCodes { Clean_Temp_Tables,
                Bulk_Insert_Data,
                Check_Statement_Loaded,
                Check_Account_Loaded,
                Insert_Into_Production_Tables,
                Check_Production_Statement_Loaded,
                Check_Production_Account_Loaded,
                End_of_SQL};

extern char *SQLCommands[];

#endif
