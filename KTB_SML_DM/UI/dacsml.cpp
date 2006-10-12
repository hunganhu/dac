//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Data_Module.cpp", Data); /* TDataModule: File Type */
USEFORM("LoginUI.cpp", frmLogin);
USEFORM("Main.cpp", formMain);
USEFORM("dirui.cpp", frmDirUI);
USEFORM("redirui.cpp", frmReDirUI);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TformMain), &formMain);
                 Application->CreateForm(__classid(TData), &Data);
                 Application->CreateForm(__classid(TfrmLogin), &frmLogin);
                 Application->CreateForm(__classid(TfrmReDirUI), &frmReDirUI);
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
