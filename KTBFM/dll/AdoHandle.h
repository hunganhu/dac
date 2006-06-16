/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005, 2006
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: AdoHandle.h
**
** Description: header file for class TADOHandler
**
****************************************************************************/
//---------------------------------------------------------------------------

#ifndef AdoHandleH
#define AdoHandleH
//---------------------------------------------------------------------------
#include <ADODB.hpp>
#include <stdio.h>

#pragma package(smart_init)
//---------------------------------------------------------------------------
class TADOHandler
{
private:	// User declarations
        String          errormsg;
public:		// User declarations
        TADOConnection *ADOConnect;
        TADOCommand    *ADOCommand;
        TADOHandler();
        ~TADOHandler();
        bool OpenDatabase(char * CmdString);
        void CloseDatabase();
        bool ExecSQLCmd(char * CmdString);
        bool ExecSQLCmd(char * CmdString, const Variant *Parameters,
                        const int index);
        bool ExecSQLQry(char * CmdString, TADODataSet * dataset);
        bool ExecSQLQry(char * CmdString, const Variant *Parameters,
                        const int index, TADODataSet * dataset);
};
#endif

