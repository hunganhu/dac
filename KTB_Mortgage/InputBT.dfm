object frmBTInput: TfrmBTInput
  Left = 154
  Top = 106
  BorderStyle = bsDialog
  Caption = 'DAC '#19968#33324#36092#23627#25151#36024#35413#20998#31995#32113' - '#25552#20379#20140#22478#37504#34892#20351#29992
  ClientHeight = 579
  ClientWidth = 862
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label9: TLabel
    Left = 16
    Top = 516
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #36914#20214#20998#34892
  end
  object Label4: TLabel
    Left = 16
    Top = 24
    Width = 305
    Height = 65
    AutoSize = False
    Caption = 
      #37341#23565#21407#39192#38989#36681#36024#26696#20214#65292'DAC'#35413#20998#27169#22411#20677#23601#30003#35531#20154' /  '#20849#21516#20511#27454#20154' / '#20445#35657#20154#20491#20154#20449#29992#25552#20379#39080#38570#25552#31034#65292' '#20006#19981#36914#34892#25151#36024#35413#20998#25110#25552#20379#26680#20934#33287#21542 +
      #24314#35696#12290
    WordWrap = True
  end
  object Label3: TLabel
    Left = 16
    Top = 8
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #35531#27880#24847#65306
    WordWrap = True
  end
  object Label62: TLabel
    Left = 16
    Top = 540
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #24501#23529#20154#21729
  end
  object Label61: TLabel
    Left = 128
    Top = 516
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #36914#20214#21729#24037
  end
  object gbApp: TGroupBox
    Left = 4
    Top = 104
    Width = 841
    Height = 49
    Caption = #30003#35531#20154#22522#26412#36039#26009
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #22995#21517
    end
    object Label2: TLabel
      Left = 104
      Top = 24
      Width = 73
      Height = 13
      AutoSize = False
      Caption = #36523#20998#35657#23383#34399
    end
    object edtAppName: TEdit
      Left = 37
      Top = 19
      Width = 52
      Height = 21
      ImeMode = imChinese
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 0
    end
    object medtAppID: TMaskEdit
      Left = 176
      Top = 19
      Width = 73
      Height = 21
      CharCase = ecUpperCase
      EditMask = '!l999999999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 10
      TabOrder = 1
    end
  end
  object gbCoApp: TGroupBox
    Left = 4
    Top = 168
    Width = 841
    Height = 49
    Caption = #20849#21516#20511#27454#20154#22522#26412#36039#26009
    TabOrder = 2
    object Label13: TLabel
      Left = 8
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #22995#21517
    end
    object Label14: TLabel
      Left = 104
      Top = 24
      Width = 73
      Height = 13
      AutoSize = False
      Caption = #36523#20998#35657#23383#34399
    end
    object edtCoAppName: TEdit
      Left = 37
      Top = 19
      Width = 52
      Height = 21
      ImeMode = imChinese
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 0
    end
    object medtCoAppID: TMaskEdit
      Left = 176
      Top = 19
      Width = 73
      Height = 21
      CharCase = ecUpperCase
      EditMask = '!l999999999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 10
      TabOrder = 1
    end
  end
  object gbGuarantor: TGroupBox
    Left = 4
    Top = 232
    Width = 841
    Height = 57
    Caption = #20445#35657#20154#22522#26412#36039#26009
    TabOrder = 4
    object Label24: TLabel
      Left = 8
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #22995#21517
    end
    object Label25: TLabel
      Left = 104
      Top = 24
      Width = 73
      Height = 13
      AutoSize = False
      Caption = #36523#20998#35657#23383#34399
    end
    object edtGuarantorName: TEdit
      Left = 37
      Top = 19
      Width = 52
      Height = 21
      ImeMode = imChinese
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 0
    end
    object medtGuarantorID: TMaskEdit
      Left = 176
      Top = 19
      Width = 73
      Height = 21
      CharCase = ecUpperCase
      EditMask = '!l999999999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 10
      TabOrder = 1
    end
  end
  object btnExit: TButton
    Left = 768
    Top = 548
    Width = 75
    Height = 25
    Caption = #32080#26463#31243#24335
    TabOrder = 10
    OnClick = btnExitClick
  end
  object btnClear: TButton
    Left = 472
    Top = 512
    Width = 41
    Height = 25
    Caption = #28165#38500
    TabOrder = 9
    OnClick = btnClearClick
  end
  object btnScore: TButton
    Left = 416
    Top = 512
    Width = 49
    Height = 25
    Caption = #35413#20998
    TabOrder = 8
    OnClick = btnScoreClick
  end
  object medtUnderwriter: TMaskEdit
    Left = 72
    Top = 536
    Width = 49
    Height = 21
    EditMask = '!9999999;0;_'
    ImeMode = imClose
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    MaxLength = 7
    TabOrder = 7
  end
  object medtEmployee: TMaskEdit
    Left = 184
    Top = 512
    Width = 47
    Height = 21
    EditMask = '!9999999;0;_'
    ImeMode = imClose
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    MaxLength = 7
    TabOrder = 6
  end
  object medtBranch: TMaskEdit
    Left = 72
    Top = 512
    Width = 46
    Height = 21
    EditMask = '!9999;0;_'
    ImeMode = imClose
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    MaxLength = 4
    TabOrder = 5
  end
  object ckbCoApp: TCheckBox
    Left = 148
    Top = 165
    Width = 17
    Height = 17
    TabOrder = 1
    OnClick = ckbCoAppClick
  end
  object ckbGuarantor: TCheckBox
    Left = 115
    Top = 229
    Width = 17
    Height = 17
    TabOrder = 3
    OnClick = ckbGuarantorClick
  end
end
