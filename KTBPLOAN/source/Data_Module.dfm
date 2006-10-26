object Data: TData
  OldCreateOrder = False
  Left = 404
  Top = 409
  Height = 282
  Width = 341
  object connection: TADOConnection
    LoginPrompt = False
    Left = 88
    Top = 24
  end
  object query: TADOQuery
    Connection = connection
    Parameters = <>
    Left = 56
    Top = 88
  end
  object command: TADOCommand
    Connection = connection
    Parameters = <>
    Left = 240
    Top = 104
  end
  object ejcic_connection: TADOConnection
    ConnectionString = 
      'Provider=OraOLEDB.Oracle.1;Password=DAC_USER2;Persist Security I' +
      'nfo=True;User ID=DAC_USER2;Data Source=JCICTDB'
    Provider = 'OraOLEDB.Oracle.1'
    Left = 64
    Top = 144
  end
  object ejcic_query: TADOQuery
    Connection = ejcic_connection
    Parameters = <>
    Left = 152
    Top = 152
  end
  object ejcic: TENGINE
    AutoConnect = False
    ConnectKind = ckRunningOrNew
    Left = 184
    Top = 16
  end
end
