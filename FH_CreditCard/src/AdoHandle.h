//---------------------------------------------------------------------------

#ifndef AdoHandleH
#define AdoHandleH
//---------------------------------------------------------------------------
//#include <Classes.hpp>
#include <ADODB.hpp>
#include <stdio.h>

//---------------------------------------------------------------------------
class TADOHandler
{
private:	// User declarations
        TADOConnection *ADOConnect;
        TADOCommand    *ADOCommand;
public:		// User declarations
        TADOHandler();
        ~TADOHandler();
        //bool OpenDatabase();
        bool OpenDatabase(char *);
        void CloseDatabase();
        bool ExecSQLCmd(char *);
        bool ExecSQLCmd(char * , const Variant *, const int);
};
#endif

