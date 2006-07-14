object frmLogin: TfrmLogin
  Left = 192
  Top = 107
  BorderStyle = bsDialog
  Caption = 'DAC '#19968#33324#36092#23627#25151#36024#35413#20998#31995#32113' - '#25552#20379#20140#22478#37504#34892#20351#29992
  ClientHeight = 316
  ClientWidth = 357
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 8
    Width = 281
    Height = 33
    AutoSize = False
    Caption = #27489#36814#20351#29992' DAC '#19968#33324#36092#23627#25151#36024#35413#20998#31995#32113#65292#26412#31995#32113#36969#29992#31684#22285#21253#25324#65306
    WordWrap = True
  end
  object Label2: TLabel
    Left = 32
    Top = 40
    Width = 281
    Height = 49
    AutoSize = False
    Caption = #8231#19968#33324#20998#26399#20184#27454#22411#36092#23627#36024#27454#20043#26032#36024#26696#20214#12289#22686#36024'      '#26696#20214#12289#21407#39192#38989#36681#36024#26696#20214#65288#20677#25552#20379#20491#20154#39080#38570#25552'     '#31034#12289#19981#20104#35413#20998#65289
    WordWrap = True
  end
  object Label3: TLabel
    Left = 24
    Top = 120
    Width = 113
    Height = 13
    AutoSize = False
    Caption = #35531#36664#20837#24115#34399#23494#30908#65306
  end
  object Label4: TLabel
    Left = 88
    Top = 152
    Width = 32
    Height = 13
    AutoSize = False
    Caption = #24115#34399
  end
  object Label5: TLabel
    Left = 88
    Top = 180
    Width = 32
    Height = 13
    AutoSize = False
    Caption = #23494#30908
  end
  object edtUser: TEdit
    Left = 121
    Top = 147
    Width = 120
    Height = 21
    TabOrder = 0
  end
  object edtPassword: TEdit
    Left = 120
    Top = 176
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
  end
  object btnLogin: TButton
    Left = 136
    Top = 216
    Width = 75
    Height = 25
    Caption = #30331#20837
    TabOrder = 2
    OnClick = btnLoginClick
  end
  object btnExit01: TButton
    Left = 272
    Top = 272
    Width = 75
    Height = 25
    Caption = #32080#26463#31243#24335
    TabOrder = 3
    OnClick = btnExit01Click
  end
end
