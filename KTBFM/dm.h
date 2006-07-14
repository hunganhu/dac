//---------------------------------------------------------------------------

#ifndef dmH
#define dmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "GSSBEST_JCIC20_OCX.h"
#include <OleServer.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TData : public TDataModule
{
__published:	// IDE-managed Components
  TADOConnection *connection;
  TADOCommand *command;
  TADOQuery *query;
  TADOConnection *ejcic_connection;
  TADOQuery *ejcic_query;
  TENGINE *ejcic;
private:	// User declarations
public:		// User declarations
  __fastcall TData(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TData *Data;
//---------------------------------------------------------------------------
#endif
