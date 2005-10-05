//---------------------------------------------------------------------------

#ifndef ploanSQLH
#define ploanSQLH
//---------------------------------------------------------------------------
enum SQLCodes { Get_AppInfo_Record,
		Get_Loan_Record,
		Get_Prescreen_Result,
                Get_Branch_Ind,
                Get_Prescreen_Record,
		Write_Prescreen_Result,
                Write_Specific_Result_Data_Error,
                Write_Optimal_Result_Data_Error,
                Write_Specific_Result,
                Write_Optimal_Result,
                Write_No_Optimal_Result,
                Write_Decision_Result,
                Write_Decision_Result_Error,

                Calculate_Loan_Del_Number,
                Update_TS_Date,
                Get_PB_Parameters,
                Get_PB_test,
                Write_NPV_Result,
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
		Create_Procedure_TF_BAM_with_payment,
		Create_Procedure_TF_BAM_no_payment,
		Create_Procedure_TF_demographic_model,

                Drop_Working_Tables
		};
extern char *SQLCommands[];

#endif
