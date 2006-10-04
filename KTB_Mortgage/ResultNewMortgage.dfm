object frmRegularResult: TfrmRegularResult
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
  object Label1: TLabel
    Left = 21
    Top = 56
    Width = 65
    Height = 13
    AutoSize = False
    Caption = #26696#20214#32232#34399#65306
  end
  object lblSN: TLabel
    Left = 109
    Top = 56
    Width = 145
    Height = 13
    AutoSize = False
  end
  object Label3: TLabel
    Left = 269
    Top = 56
    Width = 265
    Height = 13
    AutoSize = False
    Caption = #65288#35531#23559#20197#19978#26696#20214#32232#34399#27161#26126#22312#30003#35531#26360#21491#19978#35282#65289
  end
  object lblExecution: TLabel
    Left = 24
    Top = 90
    Width = 809
    Height = 127
    AutoSize = False
  end
  object btnEnd: TButton
    Left = 776
    Top = 576
    Width = 75
    Height = 25
    Caption = #32080#26463#31243#24335
    TabOrder = 0
    OnClick = btnEndClick
  end
  object GroupBox1: TGroupBox
    Left = 16
    Top = 224
    Width = 825
    Height = 345
    Caption = #27169#32068#35413#20998#32080#26524
    TabOrder = 1
    object Label49: TLabel
      Left = 8
      Top = 24
      Width = 73
      Height = 13
      AutoSize = False
      Caption = #27169#32068#24314#35696#65306
    end
    object Shape1: TShape
      Left = 8
      Top = 40
      Width = 65
      Height = 1
    end
    object lblSuggestion1: TLabel
      Left = 8
      Top = 56
      Width = 713
      Height = 13
      AutoSize = False
    end
    object Label51: TLabel
      Left = 8
      Top = 80
      Width = 65
      Height = 13
      AutoSize = False
      Caption = #26680#20934#37329#38989#65306
    end
    object lblAmount: TLabel
      Left = 72
      Top = 80
      Width = 38
      Height = 13
      AutoSize = False
    end
    object Label53: TLabel
      Left = 115
      Top = 80
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #33836#20803
    end
    object Label54: TLabel
      Left = 8
      Top = 99
      Width = 97
      Height = 13
      AutoSize = False
      Caption = #26368#20302#21487#36024#21033#29575#65306
    end
    object lblPeriod1: TLabel
      Left = 159
      Top = 99
      Width = 33
      Height = 13
      AutoSize = False
    end
    object Label56: TLabel
      Left = 199
      Top = 99
      Width = 17
      Height = 13
      AutoSize = False
      Caption = #26399
    end
    object Label57: TLabel
      Left = 111
      Top = 99
      Width = 41
      Height = 13
      AutoSize = False
      Caption = #31532#19968#27573
    end
    object lblRate1: TLabel
      Left = 215
      Top = 99
      Width = 33
      Height = 13
      AutoSize = False
    end
    object Label59: TLabel
      Left = 255
      Top = 99
      Width = 65
      Height = 13
      AutoSize = False
      Caption = '%'#24180#21033#29575#65292
    end
    object Label60: TLabel
      Left = 319
      Top = 99
      Width = 41
      Height = 13
      AutoSize = False
      Caption = #31532#20108#27573
    end
    object lblPeriod2: TLabel
      Left = 367
      Top = 99
      Width = 33
      Height = 13
      AutoSize = False
    end
    object Label62: TLabel
      Left = 407
      Top = 99
      Width = 17
      Height = 13
      AutoSize = False
      Caption = #26399
    end
    object lblRate2: TLabel
      Left = 423
      Top = 99
      Width = 33
      Height = 13
      AutoSize = False
    end
    object Label64: TLabel
      Left = 463
      Top = 99
      Width = 65
      Height = 13
      AutoSize = False
      Caption = '%'#24180#21033#29575#65292
    end
    object Label65: TLabel
      Left = 527
      Top = 99
      Width = 41
      Height = 13
      AutoSize = False
      Caption = #31532#19977#27573
    end
    object lblPeriod3: TLabel
      Left = 575
      Top = 99
      Width = 33
      Height = 13
      AutoSize = False
    end
    object Label67: TLabel
      Left = 615
      Top = 99
      Width = 17
      Height = 13
      AutoSize = False
      Caption = #26399
    end
    object lblRate3: TLabel
      Left = 631
      Top = 99
      Width = 33
      Height = 13
      AutoSize = False
    end
    object Label69: TLabel
      Left = 671
      Top = 99
      Width = 65
      Height = 13
      AutoSize = False
      Caption = '%'#24180#21033#29575
    end
    object lblSuggestion2: TLabel
      Left = 8
      Top = 120
      Width = 713
      Height = 209
      AutoSize = False
    end
  end
  object btnNext: TButton
    Left = 400
    Top = 576
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 2
    OnClick = btnNextClick
  end
  object btnScore: TButton
    Left = 272
    Top = 576
    Width = 75
    Height = 25
    Caption = 'btnScore'
    TabOrder = 3
    TabStop = False
    Visible = False
    OnClick = btnScoreClick
  end
end
