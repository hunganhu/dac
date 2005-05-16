//---------------------------------------------------------------------------

#ifndef ploanSQLH
#define ploanSQLH
//---------------------------------------------------------------------------
enum SQLCodes { Get_AppInfo_Record,
		Get_AppI_Record,
                Get_Maintenance_Record,
		Get_Filter_Result,
		Write_PreFilter_Result,
                Write_PLoan_Result,
                Get_PD,
                Get_Ploan_PD,
		Create_Working_Tables,

		Drop_Procedure_Prepare_Jcic_Data,
                Exec_Procedure_Prepare_Jcic_Data,
                Create_Procedure_Prepare_Jcic_Data,

		Insert_Daco_Table,
                Update_Inquiry_Date,
                Update_Base,
		Insert_Audit_Table,
		Insert_Intermediate_Table,

 		Drop_Procedure_Generate_Daco41_Score,
 		Exec_Procedure_Generate_Daco41_Score,
 		Create_Procedure_Generate_Daco41_Score,

		Drop_Procedure_Generate_Ploan_Score41,
		Exec_Procedure_Generate_Ploan_Score41,
		Create_Procedure_Generate_Ploan_Score41,
                Drop_Working_Tables
		};
extern char *SQLCommands[];

#endif
