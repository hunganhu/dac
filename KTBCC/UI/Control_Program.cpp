//---------------------------------------------------------------------------

#include <vcl.h>
#include <ADODB.hpp>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("mainui.cpp", frmMainUI);
USEFORM("LoginUI.cpp", frmLogin);
USEFORM("Data_Module.cpp", Data); /* TDataModule: File Type */
USEFORM("DirUI.cpp", frmDirUI);
USEFORM("ReDirUI.cpp", frmReDirUI);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TfrmMainUI), &frmMainUI);
     Application->CreateForm(__classid(TData), &Data);
     Application->CreateForm(__classid(TfrmLogin), &frmLogin);
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