//---------------------------------------------------------------------------

#ifndef BSautoSQLH
#define BSautoSQLH
//---------------------------------------------------------------------------
enum SQLCodes { Clean_Temp_Tables,
                Bulk_Insert_Data,
                Check_Statement_Loaded,
                Check_Account_Loaded,
                DROP_PROCEDURE_Load_to_Statement,
                CREATE_PROCEDURE_Load_to_Statement,
                EXEC_PROCEDURE_Load_to_Statement,
                DROP_PROCEDURE_Load_to_Account,
                CREATE_PROCEDURE_Load_to_Account,
                EXEC_PROCEDURE_Load_to_Account,
                Check_Production_Statement_Loaded,
                Check_Production_Account_Loaded,
                Write_Log,

                SYSTEM_Exec_Advscore,
                SYSTEM_Exec_Bcp_PD,
                SYSTEM_Exec_Bcp_Profile,
                End_of_SQL};

extern char *SQLCommands[];

#endif
