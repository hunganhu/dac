//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AdoHandle.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TADOHandler::TADOHandler()
{
    // initialize OLE
    ::CoInitialize(NULL);
    using namespace std;
    ADOConnect = new TADOConnection(NULL);
    ADOCommand = new TADOCommand(NULL);
    ADOCommand->Connection = ADOConnect;
}

//---------------------------------------------------------------------------
TADOHandler::~TADOHandler()
{
     delete ADOConnect;
     delete ADOCommand;
    // unload OLE
    ::CoUninitialize();
};
//---------------------------------------------------------------------------
bool TADOHandler::OpenDatabase(char *connection_string)
{
 if (! ADOConnect->Connected) {
    try {
       ADOConnect->ConnectionString = connection_string;
       ADOConnect->ConnectionTimeout = 30; // 30 seconds
       ADOConnect->CommandTimeout = 300;   // 5 minutes
       ADOConnect->Open();
    }
    catch (Exception &E) {
       fprintf(stderr,"Error: %s, %s\n", AnsiString(E.ClassName()), E.Message);
       return (false);
    }
 }
 return (true);
}
//---------------------------------------------------------------------------
bool TADOHandler::ExecSQLCmd(char * CmdString)
{
 try {
    ADOCommand->CommandText = CmdString;
    ADOCommand->CommandTimeout = 36000;    // 10 hours
    ADOCommand->CommandType = cmdText;
    ADOCommand->Execute();
 }
 catch (Exception &E){
       fprintf(stderr,"Error: %s, %s\n", AnsiString(E.ClassName()), E.Message);
       return (false);
 }
 return (true);
}
//---------------------------------------------------------------------------
bool TADOHandler::ExecSQLCmd (char * CmdString, const Variant *Parameters,
                              const int index)
{
 try {
    ADOCommand->CommandText = CmdString;
    ADOCommand->CommandTimeout = 36000;    // 10 hours
    ADOCommand->CommandType = cmdText;
    ADOCommand->Execute(VarArrayOf(Parameters, index));
 }
 catch (Exception &E){
       fprintf(stderr,"Error: %s, %s\n", AnsiString(E.ClassName()), E.Message);
       return (false);
 }
 return (true);
}
//---------------------------------------------------------------------------
void TADOHandler::CloseDatabase()
{
 ADOConnect->Close();
}
//---------------------------------------------------------------------------
