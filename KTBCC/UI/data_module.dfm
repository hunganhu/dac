object Data: TData
  OldCreateOrder = False
  Left = 316
  Top = 231
  Height = 282
  Width = 341
  object connection: TADOConnection
    ConnectionString = 
      'Provider=SQLOLEDB.1;Password=TEST;Persist Security Info=True;Use' +
      'r ID=TEST;Initial Catalog=TNB;Data Source=BASE'
    LoginPrompt = False
    Provider = 'SQLOLEDB.1'
    Left = 88
    Top = 24
  end
  object query: TADOQuery
    CacheSize = 60
    Connection = connection
    Parameters = <>
    Left = 56
    Top = 88
  end
  object command: TADOCommand
    Connection = connection
    Parameters = <>
    Left = 224
    Top = 56
  end
  object ejcic_query: TADOQuery
    Connection = ejcic_connection
    CursorType = ctStatic
    LockType = ltReadOnly
    EnableBCD = False
    Parameters = <>
    SQL.Strings = (
      'SELECT RS_DTM_CHAR FROM JC_T_REQ;')
    Left = 256
    Top = 152
  end
  object ejcic_connection: TADOConnection
    ConnectionString = 
      'Provider=OraOLEDB.Oracle.1;Password=DAC_USER2;Persist Security I' +
      'nfo=True;User ID=DAC_USER2;Data Source=JCICTDB'
    LoginPrompt = False
    Provider = 'OraOLEDB.Oracle.1'
    Left = 152
    Top = 152
  end
  object ejcic: TENGINE
    AutoConnect = False
    ConnectKind = ckRunningOrNew
    Left = 144
    Top = 72
  end
end
