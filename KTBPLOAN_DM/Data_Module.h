//---------------------------------------------------------------------------

#ifndef Data_ModuleH
#define Data_ModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <OleServer.hpp>
#include "GSSBEST_JCIC20_OCX.h"

//---------------------------------------------------------------------------
class TData : public TDataModule
{
__published:	// IDE-managed Components
  TADOConnection *connection;
  TADOQuery *query;
  TADOCommand *command;
  TADOQuery *ejcic_query;
  TADOConnection *ejcic_connection;
        TENGINE *ejcic;
private:	// User declarations
public:		// User declarations
  __fastcall TData(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TData *Data;
//---------------------------------------------------------------------------
#endif
