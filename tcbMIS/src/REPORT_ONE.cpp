  AnsiString category_create_string = "";
  if(dimension_one.Length() != 0){
    if ((dimension_one == "PRESCREEN_CODE") || (dimension_one == "DECISION_AMOUNT"))
      category_create_string += dimension_one + " int, ";
    else
      category_create_string += dimension_one + " varchar(40), ";
  };
  if(dimension_two.Length() != 0){
    if ((dimension_two == "PRESCREEN_CODE") || (dimension_two == "DECISION_AMOUNT"))
      category_create_string += dimension_two + " int, ";
    else
      category_create_string += dimension_two + " varhar(40), ";
  };  
  AnsiString category_query_string = "";
  AnsiString category_group_string = "";
  if(dimension_one.Length() != 0){
    category_query_string += dimension_one + ", ";
    category_group_string += dimension_one;
  };
  if(dimension_two.Length() != 0){
    if(dimension_one.Length() != 0)
      category_group_string += ", ";
    category_group_string += dimension_two;  
    category_query_string += dimension_two + ", ";
  };  

  try{
    sql_stmt = "DROP TABLE ONE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
  
  sql_stmt = "CREATE TABLE ONE (";
  sql_stmt += category_create_string;
  sql_stmt += " NS float, I float, II float, III float, IV float, V float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO ONE ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string;
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL THEN 1 ELSE 0 END) AS NS, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 THEN 1 ELSE 0 END) AS I, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 THEN 1 ELSE 0 END) AS II, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 THEN 1 ELSE 0 END) AS III, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 THEN 1 ELSE 0 END) AS IV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 THEN 1 ELSE 0 END) AS V, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 THEN 1 ELSE 0 END) AS VI, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 THEN 1 ELSE 0 END) AS VII, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 THEN 1 ELSE 0 END) AS VIII, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 THEN 1 ELSE 0 END) AS IX, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 THEN 1 ELSE 0 END) AS X, ";
  sql_stmt += "       COUNT(*) AS XI, ";
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL THEN  ELSE 0 END) AS NSD, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 THEN APPLICATION_AMOUNT ELSE 0 END) AS ID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 THEN APPLICATION_AMOUNT ELSE 0 END) AS IID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 THEN APPLICATION_AMOUNT ELSE 0 END) AS IIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 THEN APPLICATION_AMOUNT ELSE 0 END) AS IVD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 THEN APPLICATION_AMOUNT ELSE 0 END) AS VD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 THEN APPLICATION_AMOUNT ELSE 0 END) AS VID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 THEN APPLICATION_AMOUNT ELSE 0 END) AS VIIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 THEN APPLICATION_AMOUNT ELSE 0 END) AS IXD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 THEN APPLICATION_AMOUNT ELSE 0 END) AS XD, ";
  sql_stmt += "       SUM(APPLICATION_AMOUNT) AS XID ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE TWO;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE TWO (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSMR float, IMR float, IIMR float, IIIMR float, IVMR float, VMR float, VIMR float, VIIMR float, VIIIMR float, IXMR float, XMR float, XIMR float, ";
  sql_stmt += " NSMRD float, IMRD float, IIMRD float, IIIMRD float, IVMRD float, VMRD float, VIMRD float, VIIMRD float, VIIIMRD float, IXMRD float, XMRD float, XIMRD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TWO ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string;  
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IVA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS VA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS VIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS VIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS VIIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS IXA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS XA, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN 1 ELSE 0 END) AS XIA, ";
  sql_stmt += "       0,        ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS IAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS IIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS IIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS IVAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS VAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS VIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS VIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS VIIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS IXAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS XAD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN MODULE_AMOUNT ELSE 0 END) AS XIAD, ";
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS NSMR, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS IMR, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS IIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS IIIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS IVMR, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS VMR, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS VIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS VIIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS VIIIMR, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS IXMR, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS XMR, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 2 THEN 1 ELSE 0 END) AS XIMR, ";
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS NSMRD,          ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS IMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS IIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS IIIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS IVMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS VMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS VIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS VIIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS VIIIMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS IXMRD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS XMRD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 2 THEN MODULE_AMOUNT ELSE 0 END) AS XIMRD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IVPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS VPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS VIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS VIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS VIIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS IXPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND MODULE_DECISION = 1 THEN PB ELSE 0 END) AS XPB, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN PB ELSE 0 END) AS XIPB,        ";
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS IPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS IIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS IIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS IVPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS VPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS VIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS VIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS VIIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS IXPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS XPBD, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN (PB * MODULE_AMOUNT) ELSE 0 END) AS XIPBD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS INPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS IINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS IIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS IVNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS VNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS VINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS VIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS VIIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS IXNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS XNPV, ";
  sql_stmt += "       SUM(CASE WHEN MODULE_DECISION = 1 THEN NPV ELSE 0 END) AS XINPV ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();
  
  try{
    sql_stmt = "DROP TABLE THREE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE THREE (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
//  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO THREE ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string;  
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IVA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VIIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IXA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS XA, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND Final_decision = 1 THEN 1 ELSE 0 END) AS XIA, ";
/*  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS ID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IVD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VIIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IXD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS XD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND Final_decision = 0 THEN 1 ELSE 0 END) AS XID, ";  */
  sql_stmt += "       0,        ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IVAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VIIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IXAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS XAD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS XIAD, ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS IPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS IIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS IIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS IVPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS VPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS VIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS VIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS VIIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS IXPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS XPB, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND Final_decision = 1 THEN PB ELSE 0 END) AS XIPB,        ";
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IVPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VIIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IXPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS XPBD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS XIPBD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS INPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IVNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VIIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IXNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS XNPV, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 1 AND Final_decision = 1 THEN NPV ELSE 0 END) AS XINPV ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();


  try{
    sql_stmt = "DROP TABLE FOUR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
  
  sql_stmt = "CREATE TABLE FOUR (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
//  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float, ";
  sql_stmt += " NSAA float, IAA float, IIAA float, IIIAA float, IVAA float, VAA float, VIAA float, VIIAA float, VIIIAA float, IXAA float, XAA float, XIAA float, ";
  sql_stmt += " NSAAD float, IAAD float, IIAAD float, IIIAAD float, IVAAD float, VAAD float, VIAAD float, VIIAAD float, VIIIAAD float, IXAAD float, XAAD float, XIAAD float, ";
  sql_stmt += " NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XAPB float, XIAPB float, ";
  sql_stmt += " NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float,  ";
  sql_stmt += " NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FOUR ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string;  
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS NSA, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IVA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VIIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IXA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS XA, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 0 AND Final_decision = 1 THEN 1 ELSE 0 END) AS XIA, ";  
/*  sql_stmt += "       SUM(CASE WHEN NPV IS NULL AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS NSD, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS ID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IVD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VIIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IXD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.045 AND Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS XD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 0 AND Final_decision = 0 THEN 1 ELSE 0 END) AS XID, ";  */
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS NSAD,               ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IVAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VIIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IXAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS XAD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 0 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS XIAD, ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS IPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS IIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS IIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS IVPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS VPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS VIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS VIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS VIIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS IXPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS XPB, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 0 AND Final_decision = 1 THEN PB ELSE 0 END) AS XIPB,        ";
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IVPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VIIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IXPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS XPBD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 0 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS XIPBD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS INPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IVNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VIIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IXNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS XNPV, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 0 AND Final_decision = 1 THEN NPV ELSE 0 END) AS XINPV, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE FOUR ";
  sql_stmt += "SET ";
  sql_stmt += "NSAA = (A.NSA + FOUR.NSA), ";
  sql_stmt += "IAA = (A.IA + FOUR.IA), ";
  sql_stmt += "IIAA = (A.IIA + FOUR.IIA), ";
  sql_stmt += "IIIAA = (A.IIIA + FOUR.IIIA), ";
  sql_stmt += "IVAA = (A.IVA + FOUR.IVA), ";
  sql_stmt += "VAA = (A.VA + FOUR.VA), ";
  sql_stmt += "VIAA = (A.VIA + FOUR.VIA), ";
  sql_stmt += "VIIAA = (A.VIIA + FOUR.VIIA), ";
  sql_stmt += "VIIIAA = (A.VIIIA + FOUR.VIIIA), ";
  sql_stmt += "IXAA = (A.IXA + FOUR.IXA), ";
  sql_stmt += "XAA = (A.XA + FOUR.XA), ";
  sql_stmt += "XIAA = (A.XIA + FOUR.XIA), ";
  sql_stmt += "NSAAD = (A.NSAD + FOUR.NSAD), ";
  sql_stmt += "IAAD = (A.IAD + FOUR.IAD), ";
  sql_stmt += "IIAAD = (A.IIAD + FOUR.IIAD), ";
  sql_stmt += "IIIAAD = (A.IIIAD + FOUR.IIIAD), ";
  sql_stmt += "IVAAD = (A.IVAD + FOUR.IVAD), ";
  sql_stmt += "VAAD = (A.VAD + FOUR.VAD), ";
  sql_stmt += "VIAAD = (A.VIAD + FOUR.VIAD), ";
  sql_stmt += "VIIAAD = (A.VIIAD + FOUR.VIIAD), ";
  sql_stmt += "VIIIAAD = (A.VIIIAD + FOUR.VIIIAD), ";
  sql_stmt += "IXAAD = (A.IXAD + FOUR.IXAD), ";
  sql_stmt += "XAAD = (A.XAD + FOUR.XAD), ";
  sql_stmt += "XIAAD = (A.XIAD + FOUR.XIAD), ";
  sql_stmt += "IAPB = (A.IPB + FOUR.IPB), ";
  sql_stmt += "IIAPB = (A.IIPB + FOUR.IIPB), ";
  sql_stmt += "IIIAPB = (A.IIIPB + FOUR.IIIPB), ";
  sql_stmt += "IVAPB = (A.IVPB + FOUR.IVPB), ";
  sql_stmt += "VAPB = (A.VPB + FOUR.VPB), ";
  sql_stmt += "VIAPB = (A.VIPB + FOUR.VIPB), ";
  sql_stmt += "VIIAPB = (A.VIIPB + FOUR.VIIPB), ";
  sql_stmt += "VIIIAPB = (A.VIIIPB + FOUR.VIIIPB), ";
  sql_stmt += "IXAPB = (A.IXPB + FOUR.IXPB), ";
  sql_stmt += "XAPB = (A.XPB + FOUR.XPB), ";
  sql_stmt += "XIAPB = (A.XIPB + FOUR.XIPB), ";
  sql_stmt += "IAPBD = (A.IPBD + FOUR.IPBD), ";
  sql_stmt += "IIAPBD = (A.IIPBD + FOUR.IIPBD), ";
  sql_stmt += "IIIAPBD = (A.IIIPBD + FOUR.IIIPBD), ";
  sql_stmt += "IVAPBD = (A.IVPBD + FOUR.IVPBD), ";
  sql_stmt += "VAPBD = (A.VPBD + FOUR.VPBD), ";
  sql_stmt += "VIAPBD = (A.VIPBD + FOUR.VIPBD), ";
  sql_stmt += "VIIAPBD = (A.VIIPBD + FOUR.VIIPBD), ";
  sql_stmt += "VIIIAPBD = (A.VIIIPBD + FOUR.VIIIPBD), ";
  sql_stmt += "IXAPBD = (A.IXPBD + FOUR.IXPBD), ";
  sql_stmt += "XAPBD = (A.XPBD + FOUR.XPBD), ";
  sql_stmt += "XIAPBD = (A.XIPBD + FOUR.XIPBD), ";
  sql_stmt += "NSANPV = (A.NSNPV + FOUR.NSNPV), ";
  sql_stmt += "IANPV = (A.INPV + FOUR.INPV), ";
  sql_stmt += "IIANPV = (A.IINPV + FOUR.IINPV), ";
  sql_stmt += "IIIANPV = (A.IIINPV + FOUR.IIINPV), ";
  sql_stmt += "IVANPV = (A.IVNPV + FOUR.IVNPV), ";
  sql_stmt += "VANPV = (A.VNPV + FOUR.VNPV), ";
  sql_stmt += "VIANPV = (A.VINPV + FOUR.VINPV), ";
  sql_stmt += "VIIANPV = (A.VIINPV + FOUR.VIINPV), ";
  sql_stmt += "VIIIANPV = (A.VIIINPV + FOUR.VIIINPV), ";
  sql_stmt += "IXANPV = (A.IXNPV + FOUR.IXNPV), ";
  sql_stmt += "XANPV = (A.XNPV + FOUR.XNPV), ";
  sql_stmt += "XIANPV = (A.XINPV + FOUR.XINPV) ";
  sql_stmt += "FROM THREE AS A ";
  if(category_create_string != ""){
    sql_stmt += "WHERE ";
    if(dimension_one != ""){
    	sql_stnt += "A.";
    	sql_stmt += dimension_one;
    	sql_stmt += " = FOUR.";
    	sql_stmt += dimnesion_one;
    };
    if(dimension_two != ""){
      if(dimension_one != "")
        sql_stmt += " AND ";
    	sql_stnt += "A.";
    	sql_stmt += dimension_two;
    	sql_stmt += " = FOUR.";
    	sql_stmt += dimnesion_two;        
    };		
  };	
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE FOUR_AR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE FOUR_AR(";
  sql_stmt += category_create_string;
  sql_stmt += "                      NS float, I float, II float, III float, IV float, V  float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += "                      NSD float, ID float, IID float, IIID float, IVD float, VD  float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FOUR_AR ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string;  
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL THEN 1 ELSE 0 END) AS NS, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 THEN 1 ELSE 0 END) AS I, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 THEN 1 ELSE 0 END) AS II, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 THEN 1 ELSE 0 END) AS III, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 THEN 1 ELSE 0 END) AS IV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 THEN 1 ELSE 0 END) AS V, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 THEN 1 ELSE 0 END) AS VI, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 THEN 1 ELSE 0 END) AS VII, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 THEN 1 ELSE 0 END) AS VIII, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 THEN 1 ELSE 0 END) AS IX, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 THEN 1 ELSE 0 END) AS X, ";
  sql_stmt += "       COUNT(*) AS XI, ";
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL THEN DECISION_AMOUNT ELSE 0 END) AS NSD, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 THEN DECISION_AMOUNT ELSE 0 END) AS ID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 THEN DECISION_AMOUNT ELSE 0 END) AS IID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 THEN DECISION_AMOUNT ELSE 0 END) AS IIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 THEN DECISION_AMOUNT ELSE 0 END) AS IVD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 THEN DECISION_AMOUNT ELSE 0 END) AS VD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 THEN DECISION_AMOUNT ELSE 0 END) AS VID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 THEN DECISION_AMOUNT ELSE 0 END) AS VIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 THEN DECISION_AMOUNT ELSE 0 END) AS VIIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 THEN DECISION_AMOUNT ELSE 0 END) AS IXD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 THEN DECISION_AMOUNT ELSE 0 END) AS XD, ";
  sql_stmt += "       SUM(DECISION_AMOUNT) AS XID ";
  sql_stmt += "FROM REPORT_BASE ";
  sql_stmt += "WHERE Module_decision = 0 ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();

//Data for part five of the report
  try{
    sql_stmt = "DROP TABLE FIVE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };
  

  sql_stmt = "CREATE TABLE FIVE (";
  sql_stmt += category_create_string;
  sql_stmt += " NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
//  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float, ";
  sql_stmt += " NSAA float, IAA float, IIAA float, IIIAA float, IVAA float, VAA float, VIAA float, VIIAA float, VIIIAA float, IXAA float, XAA float, XIAA float, ";
  sql_stmt += " NSAAD float, IAAD float, IIAAD float, IIIAAD float, IVAAD float, VAAD float, VIAAD float, VIIAAD float, VIIIAAD float, IXAAD float, XAAD float, XIAAD float, ";
  sql_stmt += " NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XAPB float, XIAPB float, ";
  sql_stmt += " NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float,  ";
  sql_stmt += " NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FIVE ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string;  
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS NSA,        ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IVA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS VIIIA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS IXA, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS XA, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 2 AND Final_decision = 1 THEN 1 ELSE 0 END) AS XIA, ";  
/*  sql_stmt += "       SUM(CASE WHEN NPV IS NULL AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS NSD, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.005 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS ID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.005 AND PB <= 0.010 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.010 AND PB <= 0.015 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.015 AND PB <= 0.020 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IVD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.020 AND PB <= 0.025 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.025 AND PB <= 0.030 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.030 AND PB <= 0.035 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.035 AND PB <= 0.040 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS VIIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.040 AND PB <= 0.045 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS IXD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.045 AND Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS XD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 2 AND Final_decision = 0 THEN 1 ELSE 0 END) AS XID, "; */
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS NSAD,               ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IVAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS VIIIAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS IXAD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS XAD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 2 AND Final_decision = 1 THEN DECISION_AMOUNT ELSE 0 END) AS XIAD, ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS IPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS IIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS IIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS IVPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS VPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS VIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS VIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS VIIIPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS IXPB, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS XPB, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 2 AND Final_decision = 1 THEN PB ELSE 0 END) AS XIPB,        ";
  sql_stmt += "       0, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IVPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS VIIIPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS IXPBD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS XPBD, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 2 AND Final_decision = 1 THEN (PB * DECISION_AMOUNT) ELSE 0 END) AS XIPBD,        ";
  sql_stmt += "       0,  ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS INPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IVNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS VIIINPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS IXNPV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 AND Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS XNPV, ";
  sql_stmt += "       SUM(CASE WHEN Module_decision = 2 AND Final_decision = 1 THEN NPV ELSE 0 END) AS XINPV, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";
  sql_stmt += "       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ";
  sql_stmt += "FROM REPORT_BASE ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE FIVE ";
  sql_stmt += "SET ";
  sql_stmt += "NSAA = (A.NSAA + FIVE.NSA), ";
  sql_stmt += "IAA = (A.IAA + FIVE.IA), ";
  sql_stmt += "IIAA = (A.IIAA + FIVE.IIA), ";
  sql_stmt += "IIIAA = (A.IIIAA + FIVE.IIIA), ";
  sql_stmt += "IVAA = (A.IVAA + FIVE.IVA), ";
  sql_stmt += "VAA = (A.VAA + FIVE.VA), ";
  sql_stmt += "VIAA = (A.VIAA + FIVE.VIA), ";
  sql_stmt += "VIIAA = (A.VIIAA + FIVE.VIIA), ";
  sql_stmt += "VIIIAA = (A.VIIIAA + FIVE.VIIIA), ";
  sql_stmt += "IXAA = (A.IXAA + FIVE.IXA), ";
  sql_stmt += "XAA = (A.XAA + FIVE.XA), ";
  sql_stmt += "XIAA = (A.XIAA + FIVE.XIA),          ";
  sql_stmt += "NSAAD = (A.NSAAD + FIVE.NSAD), ";
  sql_stmt += "IAAD = (A.IAAD + FIVE.IAD), ";
  sql_stmt += "IIAAD = (A.IIAAD + FIVE.IIAD), ";
  sql_stmt += "IIIAAD = (A.IIIAAD + FIVE.IIIAD), ";
  sql_stmt += "IVAAD = (A.IVAAD + FIVE.IVAD), ";
  sql_stmt += "VAAD = (A.VAAD + FIVE.VAD), ";
  sql_stmt += "VIAAD = (A.VIAAD + FIVE.VIAD), ";
  sql_stmt += "VIIAAD = (A.VIIAAD + FIVE.VIIAD), ";
  sql_stmt += "VIIIAAD = (A.VIIIAAD + FIVE.VIIIAD), ";
  sql_stmt += "IXAAD = (A.IXAAD + FIVE.IXAD), ";
  sql_stmt += "XAAD = (A.XAAD + FIVE.XAD), ";
  sql_stmt += "XIAAD = (A.XIAAD + FIVE.XIAD), ";
  sql_stmt += "IAPB = (A.IAPB + FIVE.IPB), ";
  sql_stmt += "IIAPB = (A.IIAPB + FIVE.IIPB), ";
  sql_stmt += "IIIAPB = (A.IIIAPB + FIVE.IIIPB), ";
  sql_stmt += "IVAPB = (A.IVAPB + FIVE.IVPB), ";
  sql_stmt += "VAPB = (A.VAPB + FIVE.VPB), ";
  sql_stmt += "VIAPB = (A.VIAPB + FIVE.VIPB), ";
  sql_stmt += "VIIAPB = (A.VIIAPB + FIVE.VIIPB), ";
  sql_stmt += "VIIIAPB = (A.VIIIAPB + FIVE.VIIIPB), ";
  sql_stmt += "IXAPB = (A.IXAPB + FIVE.IXPB), ";
  sql_stmt += "XAPB = (A.XAPB + FIVE.XPB), ";
  sql_stmt += "XIAPB = (A.XIAPB + FIVE.XIPB), ";
  sql_stmt += "IAPBD = (A.IAPBD + FIVE.IPBD), ";
  sql_stmt += "IIAPBD = (A.IIAPBD + FIVE.IIPBD), ";
  sql_stmt += "IIIAPBD = (A.IIIAPBD + FIVE.IIIPBD), ";
  sql_stmt += "IVAPBD = (A.IVAPBD + FIVE.IVPBD), ";
  sql_stmt += "VAPBD = (A.VAPBD + FIVE.VPBD), ";
  sql_stmt += "VIAPBD = (A.VIAPBD + FIVE.VIPBD), ";
  sql_stmt += "VIIAPBD = (A.VIIAPBD + FIVE.VIIPBD), ";
  sql_stmt += "VIIIAPBD = (A.VIIIAPBD + FIVE.VIIIPBD), ";
  sql_stmt += "IXAPBD = (A.IXAPBD + FIVE.IXPBD), ";
  sql_stmt += "XAPBD = (A.XAPBD + FIVE.XPBD), ";
  sql_stmt += "XIAPBD = (A.XIAPBD + FIVE.XIPBD), ";
  sql_stmt += "NSANPV = (A.NSANPV + FIVE.NSNPV), ";
  sql_stmt += "IANPV = (A.IANPV + FIVE.INPV), ";
  sql_stmt += "IIANPV = (A.IIANPV + FIVE.IINPV), ";
  sql_stmt += "IIIANPV = (A.IIIANPV + FIVE.IIINPV), ";
  sql_stmt += "IVANPV = (A.IVANPV + FIVE.IVNPV), ";
  sql_stmt += "VANPV = (A.VANPV + FIVE.VNPV), ";
  sql_stmt += "VIANPV = (A.VIANPV + FIVE.VINPV), ";
  sql_stmt += "VIIANPV = (A.VIIANPV + FIVE.VIINPV), ";
  sql_stmt += "VIIIANPV = (A.VIIIANPV + FIVE.VIIINPV), ";
  sql_stmt += "IXANPV = (A.IXANPV + FIVE.IXNPV), ";
  sql_stmt += "XANPV = (A.XANPV + FIVE.XNPV), ";
  sql_stmt += "XIANPV = (A.XIANPV + FIVE.XINPV) ";
  sql_stmt += "FROM FOUR AS A ";
  if(category_create_string != ""){
    sql_stmt += "WHERE ";
    if(dimension_one != ""){
    	sql_stnt += "A.";
    	sql_stmt += dimension_one;
    	sql_stmt += " = FIVE.";
    	sql_stmt += dimnesion_one;
    };
    if(dimension_two != ""){
      if(dimension_one != "")
        sql_stmt += " AND ";
    	sql_stnt += "A.";
    	sql_stmt += dimension_two;
    	sql_stmt += " = FIVE.";
    	sql_stmt += dimnesion_two;        
    };		
  };	

  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE FIVE_AR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE FIVE_AR(";
  sql_stmt += category_create_string;
  sql_stmt += "                      NS float, I float, II float, III float, IV float, V  float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += "                      NSD float, ID float, IID float, IIID float, IVD float, VD  float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO FIVE_AR ";
  sql_stmt += "SELECT ";
  sql_stmt += category_query_string;  
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL THEN 1 ELSE 0 END) AS NS, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.005 THEN 1 ELSE 0 END) AS I, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 THEN 1 ELSE 0 END) AS II, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 THEN 1 ELSE 0 END) AS III, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 THEN 1 ELSE 0 END) AS IV, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 THEN 1 ELSE 0 END) AS V, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 THEN 1 ELSE 0 END) AS VI, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 THEN 1 ELSE 0 END) AS VII, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 THEN 1 ELSE 0 END) AS VIII, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 THEN 1 ELSE 0 END) AS IX, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 THEN 1 ELSE 0 END) AS X, ";
  sql_stmt += "       COUNT(*) AS XI, ";
  sql_stmt += "       SUM(CASE WHEN NPV IS NULL THEN DECISION_AMOUNT ELSE 0 END) AS NSD, ";
  sql_stmt += "       SUM(CASE WHEN PB <= 0.05 THEN DECISION_AMOUNT ELSE 0 END) AS ID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.05 AND PB <= 0.10 THEN DECISION_AMOUNT ELSE 0 END) AS IID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.10 AND PB <= 0.15 THEN DECISION_AMOUNT ELSE 0 END) AS IIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.15 AND PB <= 0.20 THEN DECISION_AMOUNT ELSE 0 END) AS IVD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.20 AND PB <= 0.25 THEN DECISION_AMOUNT ELSE 0 END) AS VD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.25 AND PB <= 0.30 THEN DECISION_AMOUNT ELSE 0 END) AS VID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.30 AND PB <= 0.35 THEN DECISION_AMOUNT ELSE 0 END) AS VIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.35 AND PB <= 0.40 THEN DECISION_AMOUNT ELSE 0 END) AS VIIID, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.40 AND PB <= 0.45 THEN DECISION_AMOUNT ELSE 0 END) AS IXD, ";
  sql_stmt += "       SUM(CASE WHEN PB > 0.45 THEN DECISION_AMOUNT ELSE 0 END) AS XD, ";
  sql_stmt += "       SUM(DECISION_AMOUNT) AS XID ";
  sql_stmt += "FROM REPORT_BASE ";
  sql_stmt += "WHERE Module_decision = 2 ";
  if(category_group_string != "")
   sql_stmt += "GROUP BY " + category_group_string;
  command->CommandText = sql_stmt;
  command->Execute();

//Generate report tables
//First part
  try{
    sql_stmt = "DROP TABLE PART_ONE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE PART_ONE ";
  sql_stmt += "(NS float, I float, II float, III float, IV float, V float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

//Second part
  try{
    sql_stmt = "DROP TABLE PART_TWO;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE PART_TWO ";
  sql_stmt += "(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSMR float, IMR float, IIMR float, IIIMR float, IVMR float, VMR float, VIMR float, VIIMR float, VIIIMR float, IXMR float, XMR float, XIMR float, ";
  sql_stmt += " NSMRD float, IMRD float, IIMRD float, IIIMRD float, IVMRD float, VMRD float, VIMRD float, VIIMRD float, VIIIMRD float, IXMRD float, XMRD float, XIMRD float, ";
  sql_stmt += " NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float, ";
  sql_stmt += " NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

//Third part
  try{
    sql_stmt = "DROP TABLE PART_THREE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE PART_THREE ";
  sql_stmt += "(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
//  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float, ";
  sql_stmt += " NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

//Fourth part
  try{
    sql_stmt = "DROP TABLE PART_FOUR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };


  sql_stmt = "CREATE TABLE PART_FOUR ";
  sql_stmt += "(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
//  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float, ";
  sql_stmt += " NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float, ";
  sql_stmt += " NSAAR float, IAAR float, IIAAR float, IIIAAR float, IVAAR float, VAAR float, VIAAR float, VIIAAR float, VIIIAAR float, IXAAR float, XAAR float, XIAAR float, ";
  sql_stmt += " NSAARD float, IAARD float, IIAARD float, IIIAARD float, IVAARD float, VAARD float, VIAARD float, VIIAARD float, VIIIAARD float, IXAARD float, XAARD float, XIAARD float, ";
  sql_stmt += " NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XAPB float, XIAPB float, ";
  sql_stmt += " NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float,  ";
  sql_stmt += " NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float);";
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE AR;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE AR(NS float, I float, II float, III float, IV float, V  float, VI float, VII float, VIII float, IX float, X float, XI float, ";
  sql_stmt += "                 NSD float, ID float, IID float, IIID float, IVD float, VD  float, VID float, VIID float, VIIID float, IXD float, XD float, XID float);";
  command->CommandText = sql_stmt;
  command->Execute();

//Fifth part
  try{
    sql_stmt = "DROP TABLE PART_FIVE;";
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
  //    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  };

  sql_stmt = "CREATE TABLE PART_FIVE ";
  sql_stmt += "(NSA float, IA float, IIA float, IIIA float, IVA float, VA float, VIA float, VIIA float, VIIIA float, IXA float, XA float, XIA float, ";
//  sql_stmt += " NSD float, ID float, IID float, IIID float, IVD float, VD float, VID float, VIID float, VIIID float, IXD float, XD float, XID float, ";
  sql_stmt += " NSAD float, IAD float, IIAD float, IIIAD float, IVAD float, VAD float, VIAD float, VIIAD float, VIIIAD float, IXAD float, XAD float, XIAD float, ";
  sql_stmt += " NSAR float, IAR float, IIAR float, IIIAR float, IVAR float, VAR float, VIAR float, VIIAR float, VIIIAR float, IXAR float, XAR float, XIAR float, ";
  sql_stmt += " NSARD float, IARD float, IIARD float, IIIARD float, IVARD float, VARD float, VIARD float, VIIARD float, VIIIARD float, IXARD float, XARD float, XIARD float, ";
  sql_stmt += " NSPB float, IPB float, IIPB float, IIIPB float, IVPB float, VPB float, VIPB float, VIIPB float, VIIIPB float, IXPB float, XPB float, XIPB float, ";
  sql_stmt += " NSPBD float, IPBD float, IIPBD float, IIIPBD float, IVPBD float, VPBD float, VIPBD float, VIIPBD float, VIIIPBD float, IXPBD float, XPBD float, XIPBD float,  ";
  sql_stmt += " NSNPV float, INPV float, IINPV float, IIINPV float, IVNPV float, VNPV float, VINPV float, VIINPV float, VIIINPV float, IXNPV float, XNPV float, XINPV float, ";
  sql_stmt += " NSAAR float, IAAR float, IIAAR float, IIIAAR float, IVAAR float, VAAR float, VIAAR float, VIIAAR float, VIIIAAR float, IXAAR float, XAAR float, XIAAR float, ";
  sql_stmt += " NSAARD float, IAARD float, IIAARD float, IIIAARD float, IVAARD float, VAARD float, VIAARD float, VIIAARD float, VIIIAARD float, IXAARD float, XAARD float, XIAARD float, ";
  sql_stmt += " NSAPB float, IAPB float, IIAPB float, IIIAPB float, IVAPB float, VAPB float, VIAPB float, VIIAPB float, VIIIAPB float, IXAPB float, XAPB float, XIAPB float, ";
  sql_stmt += " NSAPBD float, IAPBD float, IIAPBD float, IIIAPBD float, IVAPBD float, VAPBD float, VIAPBD float, VIIAPBD float, VIIIAPBD float, IXAPBD float, XAPBD float, XIAPBD float,  ";
  sql_stmt += " NSANPV float, IANPV float, IIANPV float, IIIANPV float, IVANPV float, VANPV float, VIANPV float, VIIANPV float, VIIIANPV float, IXANPV float, XANPV float, XIANPV float);";
  command->CommandText = sql_stmt;
  command->Execute();
    