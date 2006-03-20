object Form1: TForm1
  Left = 139
  Top = 112
  Width = 673
  Height = 521
  Caption = #21512#20316#37329#24235#37504#34892#20491#20154#20449#36024#35413#20998#31995#32113#32173#35703#20316#26989
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = #26032#32048#26126#39636
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 11
  object L_message: TLabel
    Left = 8
    Top = 416
    Width = 32
    Height = 16
    Caption = #35338#24687
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #26032#32048#26126#39636
    Font.Style = []
    ParentFont = False
  end
  object title: TLabel
    Left = 56
    Top = 8
    Width = 513
    Height = 33
    AutoSize = False
    Caption = #21512#20316#37329#24235#37504#34892#20491#20154#20449#36024#35413#20998#31995#32113#32173#35703#20316#26989
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = #26032#32048#26126#39636
    Font.Style = [fsBold]
    ParentFont = False
  end
  object message: TLabel
    Left = 56
    Top = 416
    Width = 568
    Height = 57
    AutoSize = False
    Caption = 
      '                                                                ' +
      '                                                                ' +
      '                                '
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #26032#32048#26126#39636
    Font.Style = []
    ParentColor = False
    ParentFont = False
    WordWrap = True
  end
  object Update: TButton
    Left = 280
    Top = 376
    Width = 75
    Height = 25
    Caption = #26356#26032
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #26032#32048#26126#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Visible = False
    OnClick = UpdateClick
  end
  object End: TButton
    Left = 368
    Top = 376
    Width = 75
    Height = 25
    Caption = #32080#26463
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #26032#32048#26126#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = EndClick
  end
  object connect: TButton
    Left = 160
    Top = 376
    Width = 105
    Height = 25
    Caption = #36899#25509#36039#26009#24235
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #26032#32048#26126#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = connectClick
  end
  object GroupBox1: TGroupBox
    Left = 56
    Top = 64
    Width = 273
    Height = 169
    Caption = #36039#37329#20358#28304
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #26032#32048#26126#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label2: TLabel
      Left = 32
      Top = 32
      Width = 80
      Height = 16
      Caption = #23384#27454#30334#20998#27604
    end
    object Label4: TLabel
      Left = 32
      Top = 64
      Width = 112
      Height = 16
      Caption = #21516#26989#25286#27454#30334#20998#27604
    end
    object Label7: TLabel
      Left = 32
      Top = 96
      Width = 80
      Height = 16
      Caption = #33258#26377#30334#20998#27604
    end
    object Label8: TLabel
      Left = 32
      Top = 128
      Width = 80
      Height = 16
      Caption = #28961#24687#30334#20998#27604
    end
    object Label11: TLabel
      Left = 232
      Top = 32
      Width = 13
      Height = 16
      Caption = '%'
    end
    object Label14: TLabel
      Left = 232
      Top = 64
      Width = 13
      Height = 16
      Caption = '%'
    end
    object Label16: TLabel
      Left = 232
      Top = 96
      Width = 13
      Height = 16
      Caption = '%'
    end
    object Label17: TLabel
      Left = 232
      Top = 128
      Width = 13
      Height = 16
      Caption = '%'
    end
    object fund_deposit_pct: TEdit
      Left = 152
      Top = 32
      Width = 73
      Height = 24
      TabOrder = 0
    end
    object fund_bank_pct: TEdit
      Left = 152
      Top = 64
      Width = 73
      Height = 24
      TabOrder = 1
    end
    object fund_self_pct: TEdit
      Left = 152
      Top = 96
      Width = 73
      Height = 24
      TabOrder = 2
    end
    object fund_free_pct: TEdit
      Left = 152
      Top = 128
      Width = 73
      Height = 24
      TabOrder = 3
    end
  end
  object GroupBox2: TGroupBox
    Left = 360
    Top = 64
    Width = 273
    Height = 169
    Caption = #36039#37329#25104#26412
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #26032#32048#26126#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Label5: TLabel
      Left = 32
      Top = 32
      Width = 96
      Height = 16
      Caption = #23384#27454#24179#22343#21033#29575
    end
    object Label19: TLabel
      Left = 32
      Top = 64
      Width = 96
      Height = 16
      Caption = #21516#26989#25286#27454#21033#29575
    end
    object Label20: TLabel
      Left = 240
      Top = 32
      Width = 13
      Height = 16
      Caption = '%'
    end
    object Label21: TLabel
      Left = 240
      Top = 64
      Width = 13
      Height = 16
      Caption = '%'
    end
    object cof_deposit: TEdit
      Left = 160
      Top = 32
      Width = 73
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object cof_bank: TEdit
      Left = 160
      Top = 64
      Width = 73
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
  end
  object GroupBox3: TGroupBox
    Left = 56
    Top = 248
    Width = 273
    Height = 105
    TabOrder = 2
    object Label1: TLabel
      Left = 40
      Top = 16
      Width = 64
      Height = 16
      Caption = #32929#26481#27402#30410
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 40
      Top = 56
      Width = 106
      Height = 16
      Caption = #26989#21209#29518#37329'('#27599#31558')'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
    end
    object Label12: TLabel
      Left = 232
      Top = 16
      Width = 13
      Height = 16
      Caption = '%'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
    end
    object Label15: TLabel
      Left = 232
      Top = 56
      Width = 13
      Height = 16
      Caption = '%'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
    end
    object roe: TEdit
      Left = 152
      Top = 16
      Width = 73
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object commission: TEdit
      Left = 152
      Top = 56
      Width = 73
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
  end
end
