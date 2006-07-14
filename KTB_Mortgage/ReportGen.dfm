object frmReportGen: TfrmReportGen
  Left = 257
  Top = 188
  BorderStyle = bsDialog
  Caption = 'DAC '#19968#33324#36092#23627#25151#36024#35413#20998#31995#32113' - '#25552#20379#20140#22478#37504#34892#20351#29992
  ClientHeight = 354
  ClientWidth = 705
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
  object Label13: TLabel
    Left = 5
    Top = 205
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #22519#34892#35338#24687
    Color = clWhite
    ParentColor = False
  end
  object lblMessage: TLabel
    Left = 8
    Top = 224
    Width = 697
    Height = 113
    AutoSize = False
    Color = clBtnHighlight
    ParentColor = False
  end
  object btnExit2: TButton
    Left = 624
    Top = 160
    Width = 75
    Height = 25
    Caption = #32080#26463#31243#24335
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = btnExit2Click
  end
  object GroupBox4: TGroupBox
    Left = 0
    Top = 72
    Width = 705
    Height = 65
    Caption = #37325#26032#29986#29983#22577#34920#27284
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Label19: TLabel
      Left = 147
      Top = 30
      Width = 105
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = #21407#27284#26696#29986#29983#26085#26399
    end
    object Label20: TLabel
      Left = 257
      Top = 30
      Width = 26
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = #27665#22283
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label21: TLabel
      Left = 328
      Top = 30
      Width = 10
      Height = 16
      AutoSize = False
      Caption = #24180
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label22: TLabel
      Left = 376
      Top = 30
      Width = 11
      Height = 16
      AutoSize = False
      Caption = #26376
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label23: TLabel
      Left = 424
      Top = 30
      Width = 12
      Height = 16
      AutoSize = False
      Caption = #26085
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label25: TLabel
      Left = 476
      Top = 30
      Width = 17
      Height = 17
      AutoSize = False
      Caption = #26178
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object btnRegenRptDir: TButton
      Left = 8
      Top = 24
      Width = 129
      Height = 25
      Caption = #35373#23450#27284#26696#29986#29983#30446#37636
      TabOrder = 0
      OnClick = btnRegenRptDirClick
    end
    object medtRptYear: TMaskEdit
      Left = 295
      Top = 23
      Width = 28
      Height = 24
      AutoSize = False
      EditMask = '!999;0;_'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 3
      ParentFont = False
      TabOrder = 1
    end
    object medtRptMonth: TMaskEdit
      Left = 347
      Top = 23
      Width = 25
      Height = 24
      AutoSize = False
      EditMask = '!99;0;_'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      ParentFont = False
      TabOrder = 2
    end
    object medtRptDate: TMaskEdit
      Left = 399
      Top = 23
      Width = 25
      Height = 24
      AutoSize = False
      EditMask = '!99;0;_'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      ParentFont = False
      TabOrder = 3
    end
    object btnRptRegen: TButton
      Left = 499
      Top = 24
      Width = 75
      Height = 25
      Caption = #29986#29983#27284#26696
      TabOrder = 4
      OnClick = btnRptRegenClick
    end
    object medtRptHour: TMaskEdit
      Left = 443
      Top = 24
      Width = 30
      Height = 24
      EditMask = '!99;0;_'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      ParentFont = False
      TabOrder = 5
    end
  end
  object GroupBox3: TGroupBox
    Left = 0
    Top = 0
    Width = 705
    Height = 65
    Caption = #27491#24120#29986#29983
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object btnNormalRptDir: TButton
      Left = 8
      Top = 24
      Width = 129
      Height = 25
      Caption = #35373#23450#27284#26696#29986#29983#30446#37636
      TabOrder = 0
      OnClick = btnNormalRptDirClick
    end
    object btnRptGen: TButton
      Left = 152
      Top = 24
      Width = 75
      Height = 25
      Caption = #29986#29983#27284#26696
      TabOrder = 1
      OnClick = btnRptGenClick
    end
  end
  object btnClose: TButton
    Left = 280
    Top = 160
    Width = 97
    Height = 25
    Caption = #32080#26463#22577#34920#29986#29983
    TabOrder = 3
    Visible = False
    OnClick = btnCloseClick
  end
end
