//---------------------------------------------------------------------------

#ifndef dmcppH
#define dmcppH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TData : public TDataModule
{
__published:	// IDE-managed Components
  TADOConnection *connection;
  TADOQuery *query;
private:	// User declarations
public:		// User declarations
  __fastcall TData(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TData *Data;
//---------------------------------------------------------------------------
#endif
