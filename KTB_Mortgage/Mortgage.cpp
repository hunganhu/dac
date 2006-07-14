//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Login.cpp", frmLogin);
USEFORM("LoanTypeSelection.cpp", frmSelection);
USEFORM("Exit.cpp", frmExitDialog);
USEFORM("InputNewMortgage.cpp", frmRegularInput);
USEFORM("Reminder01.cpp", frmSNReminder1);
USEFORM("ResultNewMortgage.cpp", frmRegularResult);
USEFORM("InputReload.cpp", frmReloadInput);
USEFORM("InputBT.cpp", frmBTInput);
USEFORM("ResultBT.cpp", frmBTResult);
USEFORM("dm.cpp", Data); /* TDataModule: File Type */
USEFORM("Reminder02.cpp", frmSNReminder2);
USEFORM("Reminder03.cpp", frmSNReminder3);
USEFORM("ResultReload.cpp", frmReloadResult);
USEFORM("ReportGen.cpp", frmReportGen);
USEFORM("ReDirUI.cpp", frmReDirUI);
USEFORM("DirUI.cpp", frmDirUI);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TfrmSelection), &frmSelection);
     Application->CreateForm(__classid(TData), &Data);
     Application->CreateForm(__classid(TfrmLogin), &frmLogin);
     Application->CreateForm(__classid(TfrmRegularResult), &frmRegularResult);
     Application->CreateForm(__classid(TfrmReloadInput), &frmReloadInput);
     Application->CreateForm(__classid(TfrmBTInput), &frmBTInput);
     Application->CreateForm(__classid(TfrmBTResult), &frmBTResult);
     Application->CreateForm(__classid(TfrmRegularInput), &frmRegularInput);
     Application->CreateForm(__classid(TfrmReloadResult), &frmReloadResult);
     Application->CreateForm(__classid(TfrmReportGen), &frmReportGen);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  catch (...)
  {
     try
     {
       throw Exception("");
     }
     catch (Exception &exception)
     {
       Application->ShowException(&exception);
     }
  }
  return 0;
}
//---------------------------------------------------------------------------
