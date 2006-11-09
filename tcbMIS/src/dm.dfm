object data: Tdata
  OldCreateOrder = False
  Left = 496
  Top = 424
  Height = 257
  Width = 574
  object connection: TADOConnection
    LoginPrompt = False
    Mode = cmReadWrite
    Provider = 'IBMDADB2.1'
    Left = 40
    Top = 24
  end
  object command: TADOCommand
    CommandText = 
      '  UPDATE PART_FOUR'#13#10'  SET ('#13#10'  NSAAR, IAAR, IIAAR, IIIAAR, IVAAR' +
      ', VAAR, VIAAR, VIIAAR, VIIIAAR, IXAAR, XAAR, XIAAR,'#13#10'  NSAARD, I' +
      'AARD, IIAARD, IIIAARD, IVAARD, VAARD, VIAARD, VIIAARD, VIIIAARD,' +
      ' IXAARD, XAARD, XIAARD) ='#13#10'  (SELECT'#13#10'  NSAAR /(CASE WHEN A.NS =' +
      ' 0 THEN NULL ELSE A.NS END),'#13#10'  IAAR /(CASE WHEN A.I = 0 THEN NU' +
      'LL ELSE A.I END),'#13#10'  IIAAR /(CASE WHEN A.II = 0 THEN NULL ELSE A' +
      '.II END),'#13#10'  IIIAAR /(CASE WHEN A.III = 0 THEN NULL ELSE A.III E' +
      'ND),'#13#10'  IVAAR /(CASE WHEN A.IV = 0 THEN NULL ELSE A.IV END),'#13#10'  ' +
      'VAAR /(CASE WHEN A.V = 0 THEN NULL ELSE A.V END),'#13#10'  VIAAR /(CAS' +
      'E WHEN A.VI = 0 THEN NULL ELSE A.VI END),'#13#10'  VIIAAR /(CASE WHEN ' +
      'A.VII = 0 THEN NULL ELSE A.VII END),'#13#10'  VIIIAAR /(CASE WHEN A.VI' +
      'II = 0 THEN NULL ELSE A.VIII END),'#13#10'  IXAAR /(CASE WHEN A.IX = 0' +
      ' THEN NULL ELSE A.IX END),'#13#10'  XAAR /(CASE WHEN A.X = 0 THEN NULL' +
      ' ELSE A.X END),'#13#10'  XIAAR /(CASE WHEN A.XI = 0 THEN NULL ELSE A.X' +
      'I END),'#13#10'  NSAARD /(CASE WHEN A.NSD = 0 THEN NULL ELSE A.NSD END' +
      '),'#13#10'  IAARD /(CASE WHEN A.ID = 0 THEN NULL ELSE A.ID END),'#13#10'  II' +
      'AARD /(CASE WHEN A.IID = 0 THEN NULL ELSE A.IID END),'#13#10'  IIIAARD' +
      ' /(CASE WHEN A.IIID = 0 THEN NULL ELSE A.IIID END),'#13#10'  IVAARD /(' +
      'CASE WHEN A.IVD = 0 THEN NULL ELSE A.IVD END),'#13#10'  VAARD /(CASE W' +
      'HEN A.VD = 0 THEN NULL ELSE A.VD END),'#13#10'  VIAARD /(CASE WHEN A.V' +
      'ID = 0 THEN NULL ELSE A.VID END),'#13#10'  VIIAARD /(CASE WHEN A.VIID ' +
      '= 0 THEN NULL ELSE A.VIID END),'#13#10'  VIIIAARD /(CASE WHEN A.VIIID ' +
      '= 0 THEN NULL ELSE A.VIIID END),'#13#10'  IXAARD /(CASE WHEN A.IXD = 0' +
      ' THEN NULL ELSE A.IXD END),'#13#10'  XAARD /(CASE WHEN A.XD = 0 THEN N' +
      'ULL ELSE A.XD END),'#13#10'  XIAARD /(CASE WHEN A.XID = 0 THEN NULL EL' +
      'SE A.XID END)'#13#10'  FROM ONE AS A'#13#10'  WHERE A.APP_AMT_CAT = :DIM2 AN' +
      'D A.CI_BRANCH = :DIM1)'
    CommandTimeout = 300
    Connection = connection
    Parameters = <
      item
        Name = 'DIM2'
        Attributes = [paNullable]
        DataType = ftInteger
        NumericScale = 255
        Precision = 10
        Size = 4
        Value = Null
      end
      item
        Name = 'DIM1'
        Attributes = [paNullable]
        DataType = ftFixedChar
        NumericScale = 255
        Precision = 255
        Size = 20
        Value = Null
      end>
    Left = 120
    Top = 32
  end
  object query: TADOQuery
    Parameters = <>
    Left = 240
    Top = 40
  end
end
