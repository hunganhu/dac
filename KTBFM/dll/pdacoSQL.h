/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: pdacoSQL.h
**
** SAMPLE:  header file for pdacoSQL.cpp
**
****************************************************************************/
//---------------------------------------------------------------------------

#ifndef pdacoSQLH
#define pdacoSQLH
//---------------------------------------------------------------------------
enum SQLCodes { Get_AppInfo_Record,
		Get_Prescreen_Result,
		Get_PDACO_Score,
		Create_Working_Tables,
                Drop_Working_Tables,

		Drop_PROCEDURE_PREPARE_JCIC_DATA,
                Exec_PROCEDURE_PREPARE_JCIC_DATA,
                Create_PROCEDURE_PREPARE_JCIC_DATA,

		Drop_PROCEDURE_GENERATE_PDACO_SCORE,
                Exec_PROCEDURE_GENERATE_PDACO_SCORE,
                Create_PROCEDURE_GENERATE_PDACO_SCORE
		};

extern char *SQLCommands[];

#endif
