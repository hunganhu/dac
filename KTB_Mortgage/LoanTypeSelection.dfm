object frmSelection: TfrmSelection
  Left = 99
  Top = 89
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
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 104
    Top = 32
    Width = 105
    Height = 13
    AutoSize = False
    Caption = #35531#36984#25799#25353#20214#39006#21029
  end
  object Label2: TLabel
    Left = 104
    Top = 72
    Width = 129
    Height = 13
    AutoSize = False
    Caption = #25110#25353#21491#20596#37397#29986#29983#22577#34920
  end
  object btnExit02: TButton
    Left = 776
    Top = 576
    Width = 75
    Height = 25
    Caption = #32080#26463#31243#24335
    TabOrder = 0
    OnClick = btnExit02Click
  end
  object cmbProduct: TComboBox
    Left = 207
    Top = 27
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 1
    OnChange = cmbProductChange
    Items.Strings = (
      #20840#26032#36092#23627#36024#27454
      #22686#36024
      #21407#39192#38989#36681#36024)
  end
  object btnReportGen: TButton
    Left = 241
    Top = 67
    Width = 75
    Height = 25
    Caption = #29986#29983#22577#34920
    TabOrder = 2
    OnClick = btnReportGenClick
  end
end
