//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  AnsiString connection_string = edtConnectionString->Text;
  AnsiString inq_month = edtInqMonth->Text;
  CoInitialize(NULL);
  TADOConnection *connection = new TADOConnection(NULL);
  TADOQuery *query = new TADOQuery(NULL);
  TADOQuery *query2 = new TADOQuery(NULL);
  TADOCommand *command = new TADOCommand(NULL);
  connect(connection, connection_string);
  initialize_ado_objects(connection, query, query2, command);
  char error[2047];
  strcpy(error, " ");
  char idn[15], idn1[15], idn2[15], msn[21], time_stamp[13], ole_db[255],zip[4];
  double gav, nav, first_lien_value, pb, balance;
  int gender;
  int sc = 1;
  int pro, test_cell, principal;
  unsigned char hu;
  int status = 0;
try{
  AnsiString sql_stmt;
  sql_stmt = "SELECT * FROM APPLICANTS;";

  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Open();
  query->First();
  while(!query->Eof){
    strcpy(idn, query->FieldByName("IDN")->AsString.c_str());
    strcpy(idn1, query->FieldByName("IDN1")->AsString.c_str());
    strcpy(idn2, query->FieldByName("IDN2")->AsString.c_str());
    strcpy(msn, query->FieldByName("MSN")->AsString.c_str());
    strcpy(time_stamp, query->FieldByName("TIME_STAMP")->AsString.c_str());
    strcpy(ole_db, connection_string.c_str());
    strcpy(zip, query->FieldByName("ZIP")->AsString.c_str());

    test_cell = query->FieldValues["TEST_CELL"];
//    if(test_cell > 16){
      principal = query->FieldValues["PRINCIPAL"];
//      pro = 0;
//    }
//    else{
//      principal = 0;
//      pro = query->FieldValues["PRO_DECILE"];
//    }
    gav = query->FieldValues["GAV"];
    nav = query->FieldValues["NAV"];
    first_lien_value = query->FieldValues["FIRST_LIEN_VALUE"];
    if(ckbGender->Checked)
      gender = query->FieldValues["GENDER"];
    else
      gender = 1;  
//    gender = query->FieldValues["GENDER"];
//    hu = ((query->FieldValues["HU"] == 1) ? 1: 0);
//    pb = query->FieldValues["RISK"];
//    balance = query->FieldValues["BALANCE"];

//    inq_month = "";

    strcpy(ole_db, edtConnectionString->Text.c_str());

    status = DAC_SML_NPV(idn, idn1, idn2, msn, time_stamp, ole_db, gender, sc, principal,
                             test_cell, gav, nav, zip, first_lien_value, error);
    AnsiString error_string = error;
    AnsiString idn_str = idn;

    if(status != 0){
     capture_error(command, idn, error_string);
     Application->MessageBox("SHIT Happened", "SHIT", MB_OK);
    }
    query->Next();
  }
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(E.Message.SubString(0,14) == "無效的資料物件");
}
  query->Close();
  query2->Close();
  delete command;
  delete query;
  connection->Close();
  delete connection;
  CoUninitialize();
  if(status == 0)
    Application->MessageBox("Operation finished", "Done", MB_OK);
}
//---------------------------------------------------------------------------

void capture_error(TADOCommand *command, AnsiString idn, AnsiString error)
{
  AnsiString sql_stmt;
try{
  sql_stmt = "CREATE TABLE ERRORS(IDN CHAR(14), ERROR CHAR(255));";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
}
  sql_stmt = "INSERT INTO ERRORS(IDN, ERROR) VALUES(:idn, :err);";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Parameters->ParamValues["err"] = error;
  command->Execute();
}

void record_data(TADOCommand *command,AnsiString idn,AnsiString error_string)
{
  AnsiString sql_stmt;
try{
  sql_stmt = "INSERT INTO PM_RESULT SELECT * FROM PM;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(E.Message.SubString(0,14) == "無效的資料物件");
}
try{
  sql_stmt = "INSERT INTO HU_RESULT SELECT * FROM HU41;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(E.Message.SubString(0,14) == "無效的資料物件");
}
try{
  sql_stmt = "INSERT INTO LU_RESULT SELECT * FROM LU41;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(E.Message.SubString(0,14) == "無效的資料物件");
}
  if(error_string.Length()>0)
    capture_error(command, idn, error_string);
}




//---------------------------------------------------------------------------




void __fastcall TForm1::Button2Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------


void connect(TADOConnection *connection, AnsiString connection_string,
             int connection_timeout, int command_timeout)
{
  if(connection->Connected)
    connection->Close();
  connection->CommandTimeout = command_timeout;
  connection->ConnectionTimeout = connection_timeout;

  if(connection_string != "")
    connection->ConnectionString = connection_string;
  else{
    AnsiString ConnectionProvider = "Provider=SQLOLEDB.1;";
    AnsiString ConnectionPassword = "Password=";
    AnsiString ConnectionPersist = "Persist Security Info=True;";
    AnsiString ConnectionUser = "User ID=";
    AnsiString ConnectionCatalog = "Initial Catalog=";
    AnsiString ConnectionSource = "Data Source=";
    AnsiString password_ansi = "JCIC";

/*  ifstream passwd("passwd");
    string c_password = " ";
    char ch = ' ';
    while (ch != '\n'){
      passwd.get(ch);
      c_password += ch;
    }
    string password = decrypt(c_password);
    password_ansi = password.c_str();*/

    ConnectionUser += ("JCIC;");
    ConnectionPassword += (password_ansi + ";");
    ConnectionCatalog += ("DAPHNE;");
    ConnectionSource += "GIZA";
    connection->ConnectionString = ConnectionProvider + ConnectionPassword +
                                   ConnectionPersist + ConnectionUser +
                                   ConnectionCatalog + ConnectionSource;
//    connection->ConnectionString = "Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=BSP_2nd_Mortgage";
    }
  connection->Connected = true;
}

void initialize_ado_objects(TADOConnection *connection, TADOQuery *query,
                            TADOQuery * query2, TADOCommand *command,
                            int command_timeout)
{
  if(query->Active)
    query->Close();
  query->Connection = connection;
  query->CommandTimeout = command_timeout;
  query->EnableBCD = false;

  if(query2->Active)
    query2->Close();
  query2->Connection = connection;
  query2->CommandTimeout = command_timeout;
  query2->EnableBCD = false;


  command->Connection = connection;
  command->CommandTimeout = command_timeout;
  command->CommandType = cmdText;
}

void copy_table(TADOCommand *command,
                AnsiString source_table, AnsiString destination_table,
                AnsiString msn, AnsiString time_stamp)
{
    AnsiString sql_stmt;
try{
  sql_stmt = "DROP TABLE " + destination_table + ";";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}

  sql_stmt = "SELECT * INTO " + destination_table;
  sql_stmt+= " FROM " + source_table + " WHERE MSN = :msn AND TIME_STAMP = :time_stamp;";
  command->CommandText = sql_stmt.UpperCase();
  command->Parameters->ParamValues["msn"] = msn;
  command->Parameters->ParamValues["time_stamp"] = time_stamp;
  command->Execute();
}
