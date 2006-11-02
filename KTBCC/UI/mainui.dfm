object frmMainUI: TfrmMainUI
  Left = 331
  Top = 175
  BorderStyle = bsDialog
  Caption = 'DAC'#20449#29992#21345#35413#20998#31995#32113' - '#25552#20379#20140#22478#37504#34892#20449#29992#21345#20351#29992
  ClientHeight = 599
  ClientWidth = 717
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object lblMessage: TLabel
    Left = 8
    Top = 472
    Width = 697
    Height = 113
    AutoSize = False
    Color = clBtnHighlight
    ParentColor = False
  end
  object Label13: TLabel
    Left = 5
    Top = 453
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #22519#34892#35338#24687
    Color = clWhite
    ParentColor = False
  end
  object Label28: TLabel
    Left = 176
    Top = 0
    Width = 38
    Height = 13
    Caption = 'Label28'
  end
  object pcUI: TPageControl
    Left = 0
    Top = 0
    Width = 713
    Height = 449
    ActivePage = tsDataEntry
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabIndex = 0
    TabOrder = 0
    OnEnter = pcUIEnter
    object tsDataEntry: TTabSheet
      Caption = #36039#26009#36664#20837
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnEnter = tsDataEntryEnter
      object Label11: TLabel
        Left = 8
        Top = 262
        Width = 57
        Height = 13
        AutoSize = False
        Caption = #36914#20214#31649#36947
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label36: TLabel
        Left = 204
        Top = 263
        Width = 45
        Height = 13
        AutoSize = False
        Caption = #24180#25910#20837
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label37: TLabel
        Left = 332
        Top = 263
        Width = 13
        Height = 13
        AutoSize = False
        Caption = #20803
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object GroupBox1: TGroupBox
        Left = 0
        Top = 8
        Width = 705
        Height = 129
        Caption = #20027#21345#30003#35531#20154
        TabOrder = 0
        object Label15: TLabel
          Left = 112
          Top = 31
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #36523#20998#35657#23383#34399
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label1: TLabel
          Left = 286
          Top = 31
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #20986#29983#24180#26376#26085
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label16: TLabel
          Left = 429
          Top = 31
          Width = 12
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
        object Label17: TLabel
          Left = 477
          Top = 31
          Width = 12
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
        object Label18: TLabel
          Left = 525
          Top = 31
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
        object Label6: TLabel
          Left = 307
          Top = 62
          Width = 89
          Height = 17
          AutoSize = False
          Caption = #19977#30908#37109#36958#21312#34399
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label7: TLabel
          Left = 358
          Top = 31
          Width = 29
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
        object Label14: TLabel
          Left = 8
          Top = 31
          Width = 33
          Height = 16
          Alignment = taRightJustify
          AutoSize = False
          Caption = #22995#21517
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbForeignBride: TCheckBox
          Left = 443
          Top = 62
          Width = 118
          Height = 17
          Alignment = taLeftJustify
          Caption = #26159#21542#28858#22806#31821#26032#23064
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object medtPrimaryID: TMaskEdit
          Left = 186
          Top = 24
          Width = 89
          Height = 24
          AutoSize = False
          CharCase = ecUpperCase
          EditMask = '!L9999999999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 11
          TabOrder = 1
        end
        object medtPrimaryBirthYear: TMaskEdit
          Left = 396
          Top = 24
          Width = 28
          Height = 24
          AutoSize = False
          EditMask = '!999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 2
        end
        object medtPrimaryBirthMonth: TMaskEdit
          Left = 448
          Top = 24
          Width = 25
          Height = 24
          AutoSize = False
          EditMask = '!99;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 2
          TabOrder = 3
        end
        object medtZIP: TMaskEdit
          Left = 396
          Top = 56
          Width = 28
          Height = 24
          AutoSize = False
          EditMask = '!999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 6
        end
        object medtPrimaryBirthDate: TMaskEdit
          Left = 500
          Top = 24
          Width = 25
          Height = 24
          AutoSize = False
          EditMask = '!99;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 2
          TabOrder = 4
        end
        object edtPrimaryName: TEdit
          Left = 41
          Top = 26
          Width = 64
          Height = 24
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 0
        end
        object GroupBox5: TGroupBox
          Left = 8
          Top = 56
          Width = 281
          Height = 57
          Caption = #38651#35441
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          object Label3: TLabel
            Left = 162
            Top = 24
            Width = 29
            Height = 13
            Alignment = taRightJustify
            AutoSize = False
            Caption = #34399#30908
          end
          object Label24: TLabel
            Left = 84
            Top = 25
            Width = 33
            Height = 13
            Alignment = taRightJustify
            AutoSize = False
            Caption = #21312#30908
          end
          object rbFixedLine: TRadioButton
            Left = 8
            Top = 16
            Width = 73
            Height = 17
            Alignment = taLeftJustify
            Caption = #22266#32178#38651#35441
            Checked = True
            TabOrder = 0
            TabStop = True
          end
          object rbMobile: TRadioButton
            Left = 8
            Top = 32
            Width = 73
            Height = 17
            Alignment = taLeftJustify
            Caption = #34892#21205#38651#35441
            TabOrder = 1
          end
          object medtPhone: TMaskEdit
            Left = 195
            Top = 20
            Width = 69
            Height = 21
            EditMask = '9999999999;1;_'
            ImeMode = imDisable
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 10
            TabOrder = 3
            Text = '          '
          end
          object medtRegion: TMaskEdit
            Left = 121
            Top = 20
            Width = 33
            Height = 21
            EditMask = '!9999;1;_'
            ImeMode = imDisable
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 4
            TabOrder = 2
            Text = '    '
          end
        end
        object cbManagerRecommended: TCheckBox
          Left = 304
          Top = 88
          Width = 121
          Height = 17
          Alignment = taLeftJustify
          Caption = #26159#21542#28858#20027#31649#25512#34214
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
      end
      object btnExit1: TButton
        Left = 624
        Top = 384
        Width = 75
        Height = 25
        Caption = #32080#26463#31243#24335
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        OnClick = btnExit1Click
      end
      object GroupBox2: TGroupBox
        Left = 0
        Top = 179
        Width = 705
        Height = 65
        Caption = #38468#21345#30003#35531#20154
        Color = clBtnFace
        Enabled = False
        ParentColor = False
        TabOrder = 2
        object Label2: TLabel
          Left = 112
          Top = 31
          Width = 72
          Height = 12
          Alignment = taRightJustify
          AutoSize = False
          Caption = #36523#20998#35657#23383#34399
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object Label4: TLabel
          Left = 286
          Top = 31
          Width = 73
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #20986#29983#24180#26376#26085
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object Label10: TLabel
          Left = 525
          Top = 31
          Width = 12
          Height = 16
          AutoSize = False
          Caption = #26085
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label9: TLabel
          Left = 477
          Top = 31
          Width = 12
          Height = 16
          AutoSize = False
          Caption = #26376
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label5: TLabel
          Left = 429
          Top = 31
          Width = 12
          Height = 16
          AutoSize = False
          Caption = #24180
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label8: TLabel
          Left = 358
          Top = 31
          Width = 33
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #27665#22283
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label12: TLabel
          Left = 8
          Top = 31
          Width = 33
          Height = 16
          Alignment = taRightJustify
          AutoSize = False
          Caption = #22995#21517
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object medtSecondaryBirthYear: TMaskEdit
          Left = 396
          Top = 24
          Width = 28
          Height = 24
          AutoSize = False
          EditMask = '!999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 2
        end
        object medtSecondaryBirthMonth: TMaskEdit
          Left = 448
          Top = 24
          Width = 25
          Height = 24
          AutoSize = False
          EditMask = '!99;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 2
          TabOrder = 3
        end
        object medtSecondaryBirthDate: TMaskEdit
          Left = 500
          Top = 24
          Width = 25
          Height = 24
          AutoSize = False
          EditMask = '!99;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 2
          TabOrder = 4
        end
        object medtSecondaryID: TMaskEdit
          Left = 186
          Top = 24
          Width = 89
          Height = 24
          AutoSize = False
          CharCase = ecUpperCase
          EditMask = '!L99999999999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 12
          TabOrder = 1
        end
        object edtSecondaryName: TEdit
          Left = 41
          Top = 26
          Width = 64
          Height = 24
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 0
        end
      end
      object cbSecondaryCardApplied: TCheckBox
        Left = 11
        Top = 147
        Width = 118
        Height = 17
        Alignment = taLeftJustify
        Caption = #26159#21542#26377#30003#35531#38468#21345
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = cbSecondaryCardAppliedClick
      end
      object edtChannel: TEdit
        Left = 66
        Top = 257
        Width = 121
        Height = 24
        ImeMode = imDisable
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        MaxLength = 4
        TabOrder = 3
      end
      object btnScore: TButton
        Left = 248
        Top = 344
        Width = 75
        Height = 25
        Caption = #35413#20998
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        OnClick = btnScoreClick
        OnKeyPress = btnScoreKeyPress
      end
      object btnClear: TButton
        Left = 344
        Top = 344
        Width = 75
        Height = 25
        Caption = #28165#38500
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
        OnClick = btnClearClick
      end
      object medtMonthlyIncome: TMaskEdit
        Left = 249
        Top = 257
        Width = 80
        Height = 24
        EditMask = '!999999999;0;_'
        MaxLength = 9
        TabOrder = 4
      end
      object cbForcedScoring: TCheckBox
        Left = 363
        Top = 261
        Width = 134
        Height = 17
        Alignment = taLeftJustify
        Caption = #29694#26377#23458#25142#24375#21046#35413#20998
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
    end
    object tsReportGen: TTabSheet
      Caption = #23529#26597#22577#34920#27284#29986#29983
      ImageIndex = 1
      OnEnter = tsReportGenEnter
      object btnExit2: TButton
        Left = 624
        Top = 384
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
      object GroupBox3: TGroupBox
        Left = 0
        Top = 8
        Width = 705
        Height = 65
        Caption = #27491#24120#29986#29983
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
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
          OnKeyPress = btnRptGenKeyPress
        end
      end
      object GroupBox4: TGroupBox
        Left = 0
        Top = 80
        Width = 705
        Height = 65
        Caption = #37325#26032#29986#29983#22577#34920#27284
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
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
          Left = 468
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
          Left = 491
          Top = 24
          Width = 75
          Height = 25
          Caption = #29986#29983#27284#26696
          TabOrder = 5
          OnClick = btnRptRegenClick
          OnKeyPress = btnRptRegenKeyPress
        end
        object medtRptHour: TMaskEdit
          Left = 443
          Top = 24
          Width = 25
          Height = 24
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
          TabOrder = 4
        end
      end
    end
    object tsDecision: TTabSheet
      Caption = #20934#39361#27770#23450#36664#20837
      ImageIndex = 2
      OnEnter = tsDecisionEnter
      object Label26: TLabel
        Left = 8
        Top = 16
        Width = 145
        Height = 16
        AutoSize = False
        Caption = #20027#21345#30003#35531#20154#36523#20998#35657#23383#34399
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label27: TLabel
        Left = 251
        Top = 16
        Width = 89
        Height = 16
        Alignment = taRightJustify
        AutoSize = False
        Caption = #36039#26009#36664#20837#26085#26399
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label29: TLabel
        Left = 347
        Top = 16
        Width = 33
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
      object Label30: TLabel
        Left = 411
        Top = 16
        Width = 17
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = #24180
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label31: TLabel
        Left = 459
        Top = 16
        Width = 17
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = #26376
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label32: TLabel
        Left = 507
        Top = 16
        Width = 17
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = #26085
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object medtID: TMaskEdit
        Left = 152
        Top = 10
        Width = 89
        Height = 24
        CharCase = ecUpperCase
        EditMask = '!L9999999999;0;_'
        ImeMode = imDisable
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        MaxLength = 11
        TabOrder = 0
      end
      object btnExit3: TButton
        Left = 624
        Top = 384
        Width = 75
        Height = 25
        Caption = #32080#26463#31243#24335
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        OnClick = btnExit3Click
      end
      object medtEntryYear: TMaskEdit
        Left = 382
        Top = 11
        Width = 27
        Height = 24
        EditMask = '!999;0;_'
        ImeMode = imClose
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        MaxLength = 3
        TabOrder = 1
      end
      object medtEntryMonth: TMaskEdit
        Left = 434
        Top = 11
        Width = 25
        Height = 24
        EditMask = '!99;0;_'
        ImeMode = imClose
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        MaxLength = 2
        TabOrder = 2
      end
      object medtEntryDate: TMaskEdit
        Left = 478
        Top = 11
        Width = 25
        Height = 24
        EditMask = '!99;0;_'
        ImeMode = imClose
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        MaxLength = 2
        TabOrder = 3
        OnExit = medtEntryDateExit
      end
      object GroupBox6: TGroupBox
        Left = 8
        Top = 48
        Width = 449
        Height = 81
        Caption = #23529#26597#32080#26524
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        object Label33: TLabel
          Left = 69
          Top = 20
          Width = 60
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #26680#20934#38989#24230
        end
        object Label34: TLabel
          Left = 197
          Top = 20
          Width = 20
          Height = 13
          AutoSize = False
          Caption = #20803
        end
        object Label35: TLabel
          Left = 72
          Top = 53
          Width = 57
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #23113#25298#21407#22240
          Enabled = False
        end
        object rbApproval: TRadioButton
          Left = 8
          Top = 19
          Width = 49
          Height = 17
          Alignment = taLeftJustify
          Caption = #26680#20934
          Checked = True
          TabOrder = 0
          TabStop = True
          OnClick = rbApprovalClick
        end
        object rbDecline: TRadioButton
          Left = 8
          Top = 53
          Width = 49
          Height = 17
          Alignment = taLeftJustify
          Caption = #23113#25298
          TabOrder = 3
          OnClick = rbDeclineClick
        end
        object medtApprovedLine: TMaskEdit
          Left = 132
          Top = 16
          Width = 59
          Height = 24
          EditMask = '9999999;1;_'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imDisable
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 7
          ParentFont = False
          TabOrder = 1
          Text = '       '
        end
        object cmbDeclineReason: TComboBox
          Left = 136
          Top = 48
          Width = 145
          Height = 21
          Enabled = False
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          TabOrder = 2
          Text = #32887#26989#19981#26126
          Items.Strings = (
            #32887#26989#19981#26126
            #25298#32085#30332#21345#34892#26989
            #20844#21496#19981#26126' ('#21547#28961#27492#20844#21496')'
            #20844#21496#20572#26989' / '#27463#26989
            #26366#32147#25298#32085#24448#20358
            #20381#29031#27169#32068#24314#35696
            #31192#23494#25142
            #36817#26399#23494#38598#30003#35531
            #36000#20661#32317#38989#36942#39640
            #38263#23621#30064#22320#65292#32879#32097#32371#27454#19981#26131
            #20182#34892#21574#24115
            #25142#25919#35531#38936#32000#37636#30064#24120
            #28961#32887#26989
            #23458#25142#33256#26178#27770#23450#19981#30003#35531
            #29031#26371#24460#30456#38364#36039#26009#26377#30097#24942)
        end
      end
      object btnStoreDecision: TButton
        Left = 248
        Top = 344
        Width = 75
        Height = 25
        Caption = #20786#23384
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        OnClick = btnStoreDecisionClick
        OnKeyPress = btnStoreDecisionKeyPress
      end
      object btnClearDecision: TButton
        Left = 344
        Top = 344
        Width = 75
        Height = 25
        Caption = #28165#38500
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
        OnClick = btnClearDecisionClick
      end
      object btnGetSuggestion: TButton
        Left = 152
        Top = 344
        Width = 75
        Height = 25
        Caption = #21462#24471#24314#35696
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        Visible = False
        OnClick = btnGetSuggestionClick
        OnKeyPress = btnGetSuggestionKeyPress
      end
    end
  end
end
