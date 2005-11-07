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

