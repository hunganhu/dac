/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: pdaco61SQL.h
**
** SAMPLE:  header file for pdaco61SQL.cpp
**
****************************************************************************/
//---------------------------------------------------------------------------

#ifndef pdaco61SQLH
#define pdaco61SQLH
//---------------------------------------------------------------------------
enum SQLCodes { Get_AppInfo_Record,
		Get_Prescreen_Result,
		Get_P0_Raw_Variables,
		Get_P1_Raw_Variables,
		Get_P2_Raw_Variables,
		Get_P3_Raw_Variables,
		Get_P4_Raw_Variables,
		Get_P5_Raw_Variables,
                Get_Overall_Lookup,
                Get_Final_Lookup,
		Create_Working_Tables,
                Drop_Working_Tables,

                Insert_PDACO_V61,
                Dedup_KRM021,
                Dedup_KRM023,
                Dedup_STM007,
                Dedup_BAM086,
                Insert_JAS002_DELINQUENT,
                Insert_JAS002_BAD_CHECK,
                Insert_JAS002_REJECT,
                Insert_JAS002_STOP_CARD,
                Dedup_KRM037,

                Prepare_JCIC_Source_Tables,
                Prepare_JCIC_Data,
                PDACO61_P0,
                PDACO61_P1,
                PDACO61_P2,
                PDACO61_P3,
                PDACO61_P4,
                PDACO61_P5,
                Insert_Audit_Table,
                Update_Audit_Table
		};

extern char *SQLCommands[];

#endif