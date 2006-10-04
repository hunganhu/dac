object frmBTResult: TfrmBTResult
  Left = 154
  Top = 98
  BorderStyle = bsDialog
  Caption = 'DAC '#19968#33324#36092#23627#25151#36024#35413#20998#31995#32113' - '#25552#20379#20140#22478#37504#34892#20351#29992
  ClientHeight = 606
  ClientWidth = 862
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 16
    Top = 8
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #35531#27880#24847#65306
    WordWrap = True
  end
  object Label4: TLabel
    Left = 16
    Top = 24
    Width = 305
    Height = 49
    AutoSize = False
    Caption = 
      #37341#23565#21407#39192#38989#36681#36024#26696#20214#65292'DAC'#35413#20998#27169#22411#20677#23601#30003#35531#20154' /  '#20849#21516#20511#27454#20154' / '#20445#35657#20154#20491#20154#20449#29992#25552#20379#39080#38570#25552#31034#65292' '#20006#19981#36914#34892#25151#36024#35413#20998#25110#25552#20379#26680#20934#33287#21542 +
      #24314#35696#12290
    WordWrap = True
  end
  object Label1: TLabel
    Left = 16
    Top = 96
    Width = 65
    Height = 13
    AutoSize = False
    Caption = #26696#20214#32232#34399#65306
  end
  object lblSN: TLabel
    Left = 96
    Top = 96
    Width = 145
    Height = 13
    AutoSize = False
  end
  object Label5: TLabel
    Left = 256
    Top = 96
    Width = 265
    Height = 13
    AutoSize = False
    Caption = #65288#35531#23559#20197#19978#26696#20214#32232#34399#27161#26126#22312#30003#35531#26360#21491#19978#35282#65289
  end
  object Label6: TLabel
    Left = 16
    Top = 297
    Width = 65
    Height = 13
    AutoSize = False
    Caption = #30003#35531#20154#65306
  end
  object lblAppName: TLabel
    Left = 104
    Top = 297
    Width = 145
    Height = 13
    AutoSize = False
  end
  object lblAppMsg: TLabel
    Left = 264
    Top = 297
    Width = 529
    Height = 13
    AutoSize = False
  end
  object lblCoAppMsg: TLabel
    Left = 264
    Top = 313
    Width = 529
    Height = 13
    AutoSize = False
  end
  object lblGuarantorMsg: TLabel
    Left = 264
    Top = 329
    Width = 529
    Height = 13
    AutoSize = False
  end
  object lblGuarantorName: TLabel
    Left = 104
    Top = 331
    Width = 145
    Height = 13
    AutoSize = False
  end
  object Label8: TLabel
    Left = 16
    Top = 331
    Width = 65
    Height = 13
    AutoSize = False
    Caption = #20445#35657#20154#65306
  end
  object Label10: TLabel
    Left = 16
    Top = 314
    Width = 81
    Height = 13
    AutoSize = False
    Caption = #20849#21516#20511#27454#20154#65306
  end
  object lblCoAppName: TLabel
    Left = 104
    Top = 314
    Width = 145
    Height = 13
    AutoSize = False
  end
  object Label12: TLabel
    Left = 16
    Top = 273
    Width = 225
    Height = 13
    AutoSize = False
    Caption = #36681#36024#26696#20214#20491#20154#39080#38570#25552#31034#32080#26524#22914#19979#65306
  end
  object lblExecution: TLabel
    Left = 16
    Top = 120
    Width = 769
    Height = 145
    AutoSize = False
  end
  object btnEnd: TButton
    Left = 776
    Top = 568
    Width = 75
    Height = 25
    Caption = #32080#26463#31243#24335
    TabOrder = 0
    OnClick = btnEndClick
  end
  object btnNext: TButton
    Left = 376
    Top = 568
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = btnNextClick
  end
  object btnScore: TButton
    Left = 272
    Top = 568
    Width = 75
    Height = 25
    Caption = 'btnScore'
    TabOrder = 2
    TabStop = False
    Visible = False
    OnClick = btnScoreClick
  end
end
