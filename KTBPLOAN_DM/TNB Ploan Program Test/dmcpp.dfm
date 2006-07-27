object Data: TData
  OldCreateOrder = False
  Left = 346
  Top = 293
  Height = 314
  Width = 377
  object connection: TADOConnection
    ConnectionString = 
      'Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security In' +
      'fo=False;User ID=test;Initial Catalog=PLOAN_MKT;Data Source=NBCO' +
      'MPUTER\NBCOMPUTER2K'
    Provider = 'SQLOLEDB.1'
    Left = 88
    Top = 56
  end
  object query: TADOQuery
    Connection = connection
    Parameters = <>
    Left = 184
    Top = 56
  end
end
