//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "dmcpp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TData *Data;
//---------------------------------------------------------------------------
__fastcall TData::TData(TComponent* Owner)
  : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
