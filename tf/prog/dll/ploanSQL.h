//---------------------------------------------------------------------------

#ifndef ploanSQLH
#define ploanSQLH
//---------------------------------------------------------------------------
enum SQLCodes { Get_AppInfo_Record,
		Get_Loan_Record,
		Get_Prescreen_Result,
                Get_Branch_Ind,
		Write_Prescreen_Result,

                Calculate_Loan_Del_Number,
                Get_PB_Input,
                Write_PB_Result,
                Write_NPV_Result,
                Write_PLoan_Result,
		Create_Working_Tables,

		Drop_Procedure_TF_prepare_jcic_data,
                Exec_Procedure_TF_prepare_jcic_data,
                Create_Procedure_TF_prepare_jcic_data,
		Create_Procedure_TF_loan_prescreen,

		Insert_Daco_Table,
                Update_Inquiry_Date,
                Update_Prescreen_Output,
		Insert_Audit_Table,
		Insert_Intermediate_Table,

 		Create_Procedure_TF_ploan_model,
		Create_Procedure_TF_demographic_model,
		Create_Procedure_TF_BAM_no_payment,
		Create_Procedure_TF_BAM_with_payment,

                Drop_Working_Tables
		};
extern char *SQLCommands[];

#endif
