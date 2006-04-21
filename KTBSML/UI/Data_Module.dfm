object Data: TData
  OldCreateOrder = False
  Left = 683
  Top = 360
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
end
