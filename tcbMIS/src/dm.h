//---------------------------------------------------------------------------

#ifndef dmH
#define dmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class Tdata : public TDataModule
{
__published:	// IDE-managed Components
  TADOConnection *connection;
  TADOCommand *command;
  TADOQuery *query;
private:	// User declarations
public:		// User declarations
  __fastcall Tdata(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tdata *data;
//---------------------------------------------------------------------------
#endif
