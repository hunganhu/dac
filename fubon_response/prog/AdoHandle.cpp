//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AdoHandle.h"
#include <string>
using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TADOHandler::TADOHandler()
{
    // initialize OLE
    ::CoInitialize(NULL);
    ADOConnect = new TADOConnection(NULL);
    ADOConnect->CursorLocation = clUseClient;
    ADOCommand = new TADOCommand(NULL);
    ADOCommand->Connection = ADOConnect;
    errormsg = "";
}

//---------------------------------------------------------------------------
TADOHandler::~TADOHandler()
{
   ADOConnect->Close();
   delete ADOCommand;
   delete ADOConnect;
   // unload OLE
   ::CoUninitialize();
};
//---------------------------------------------------------------------------
/*
 This is a example to use the function OpenDatabase with connect string as input.
 TADOHandler *dbhandle;
 dbhandle = new TADOHandler();
 if (! dbhandle->OpenDatabase(connect_string)) {
    fprintf(stderr, "Error: Could not connect to database.");
    delete dbhandle;
    return (1);
 }
*/
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
        throw;
    }
 }
 return (true);
}
//---------------------------------------------------------------------------
/*
 This is a example to use the function ExecSQLCmd to execute a SQL command with no host variable.

 TADOHandler *dbhandle;
 dbhandle = new TADOHandler();
 if (! dbhandle->OpenDatabase(connect_string)) {
    fprintf(stderr, "Error: Could not connect to database.");
    delete dbhandle;
    return (1);
 }
 if (! dbhandle->ExecSQLCmd(SQLCommands[step[i]])) {
    delete dbhandle;
    return (1);
 }
*/
bool TADOHandler::ExecSQLCmd(char * CmdString)
{
 try {
    ADOCommand->CommandText = CmdString;
    ADOCommand->CommandTimeout = 36000;    // 10 hours
    ADOCommand->CommandType = cmdText;
    ADOCommand->Execute();
 }
 catch (Exception &E){
        throw;
 }
 return (true);
}
//---------------------------------------------------------------------------
/*
 This is a example to use the function ExecSQLCmd to execute a SQL command with a host variable.
  hostVars[0] = target_month;
  if (! dbhandle->ExecSQLCmd(SQLCommands[step[i]], hostVars, 0)) {
     delete dbhandle;
     return (1);
  }
*/
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
        throw;
 }
 return (true);
}
//---------------------------------------------------------------------------
/*
 This is a example to use the function ExecSQLQry to execute a sql command without host variables. The
 execution result is stored in a dataset.

 TADOHandler *dbhandle;
 TADODataSet *ds;

 dbhandle = new TADOHandler();
 ds = new TADODataSet(NULL);
 if (! dbhandle->OpenDatabase(connect_string)) {
    fprintf(stderr, "Error: Could not connect to database.");
    delete dbhandle;
    return (1);
 }
 fprintf(stderr, "%s: Test with no parameters.\n", CurrDateTime());
 if (!dbhandle->ExecSQLQry("select * from oliver.credit_card_monthly_profile", ds)) {
    delete dbhandle;
    return (1);
 }
 ds->First();
 while (!ds->Eof) {
   month = ds->FieldValues["statement_month"];
   low = ds->FieldValues["low"];
   medium = ds->FieldValues["medium"];
   high = ds->FieldValues["high"];
   not_scored = ds->FieldValues["not_scored"];

   fprintf (stderr, "Month = %s, ", month);
   fprintf (stderr, "Low = %s, ", low);
   fprintf (stderr, "Medium = %s, ", medium);
   fprintf (stderr, "High = %s, ", high);
   fprintf (stderr, "Not_Scored = %s\n", not_scored);
   ds->Next();
 }
*/
bool TADOHandler::ExecSQLQry (char * CmdString, TADODataSet * dataset)
{
 try {
    ADOCommand->CommandText = CmdString;
    ADOCommand->CommandTimeout = 300;    // 300 seconds
    ADOCommand->CommandType = cmdText;
    dataset->Recordset = ADOCommand->Execute();
 }
 catch (Exception &E){
        throw;
 }
 return (true);
}
//---------------------------------------------------------------------------
/*
 This is a example to use the function ExecSQLQry with input host variables.The
 execution result is stored in a dataset.

 TADOHandler *dbhandle;
 TADODataSet *ds;

 dbhandle = new TADOHandler();
 ds = new TADODataSet(NULL);
 if (! dbhandle->OpenDatabase(connect_string)) {
    fprintf(stderr, "Error: Could not connect to database.");
    delete dbhandle;
    return (1);
 }
 fprintf(stderr, "%s: Test with parameters.\n", CurrDateTime());
 hostVars[0] = "200407";
 if (!dbhandle->ExecSQLQry("select * from credit_card_monthly_profile where statement_month=:v1",
                           hostVars, 0, ds)) {
    delete dbhandle;
    return (1);
 }
 ds->First();
 while (!ds->Eof) {
   month = ds->FieldValues["statement_month"];
   low = ds->FieldValues["low"];
   medium = ds->FieldValues["medium"];
   high = ds->FieldValues["high"];
   not_scored = ds->FieldValues["not_scored"];

   fprintf (stderr, "Month = %s, ", month);
   fprintf (stderr, "Low = %s, ", low);
   fprintf (stderr, "Medium = %s, ", medium);
   fprintf (stderr, "High = %s, ", high);
   fprintf (stderr, "Not_Scored = %s\n", not_scored);
   ds->Next();
 }
*/
bool TADOHandler::ExecSQLQry (char * CmdString, const Variant *Parameters,
                              const int index, TADODataSet * dataset)
{
 try {
    ADOCommand->CommandText = CmdString;
    ADOCommand->CommandTimeout = 300;    // 300 seconds
    ADOCommand->CommandType = cmdText;
    dataset->Recordset = ADOCommand->Execute(VarArrayOf(Parameters, index));
 }
 catch (Exception &E){
        throw;
 }
 return (true);
}

//---------------------------------------------------------------------------
void TADOHandler::CloseDatabase()
{
 try {
    ADOConnect->Close();
 } catch (Exception &E) {
        throw;
 }
}
//---------------------------------------------------------------------------


