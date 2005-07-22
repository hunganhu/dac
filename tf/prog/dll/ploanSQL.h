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

		Drop_Procedure_TF_prepare_jcic_data,
                Exec_Procedure_TF_prepare_jcic_data,
                Create_Procedure_TF_prepare_jcic_data,

		Insert_Daco_Table,
                Update_Prescreen_Output,
                Update_Base,
		Insert_Audit_Table,
		Insert_Intermediate_Table,

 		Drop_Procedure_TF_ploan_model,
 		Exec_Procedure_TF_ploan_model,
 		Create_Procedure_TF_ploan_model,

		Drop_Procedure_TF_loan_prescreen,
		Exec_Procedure_TF_loan_prescreen,
		Create_Procedure_TF_loan_prescreen,

		Drop_Procedure_TF_demographic_model,
		Exec_Procedure_TF_demographic_model,
		Create_Procedure_TF_demographic_model,

		Drop_Procedure_TF_BAM_no_payment,
		Exec_Procedure_TF_BAM_no_payment,
		Create_Procedure_TF_BAM_no_payment,

		Drop_Procedure_TF_BAM_with_payment,
		Exec_Procedure_TF_BAM_with_payment,
		Create_Procedure_TF_BAM_with_payment,

                Drop_Working_Tables
		};
extern char *SQLCommands[];

#endif
