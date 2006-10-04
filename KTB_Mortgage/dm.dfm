object Data: TData
  OldCreateOrder = False
  Left = 627
  Top = 365
  Height = 284
  Width = 322
  object connection: TADOConnection
    ConnectionString = 
      'Provider=SQLOLEDB.1;Password=test;Persist Security Info=True;Use' +
      'r ID=test;Initial Catalog=KTB_FM;Data Source=NBCOMPUTER\NBCOMPUT' +
      'ER2K'
    Provider = 'SQLOLEDB.1'
    Left = 48
    Top = 104
  end
  object command: TADOCommand
    Connection = connection
    Parameters = <>
    Left = 160
    Top = 64
  end
  object query: TADOQuery
    Connection = connection
    Parameters = <>
    Left = 232
    Top = 128
  end
  object ejcic_connection: TADOConnection
    ConnectionString = 
      'Provider=OraOLEDB.Oracle.1;Password=DAC_USER2;Persist Security I' +
      'nfo=True;User ID=DAC_USER2;Data Source=JCICDB'
    Provider = 'OraOLEDB.Oracle.1'
    Left = 40
    Top = 192
  end
  object ejcic_query: TADOQuery
    Connection = ejcic_connection
    Parameters = <>
    Left = 128
    Top = 144
  end
  object ejcic: TENGINE
    AutoConnect = False
    ConnectKind = ckRunningOrNew
    Left = 72
    Top = 32
  end
end
