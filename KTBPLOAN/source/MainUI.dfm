object frmMainUI: TfrmMainUI
  Left = 252
  Top = 87
  BorderStyle = bsDialog
  Caption = 'DAC '#20491#20154#20449#36024#35413#20998#31995#32113' - '#25552#20379#20140#22478#37504#34892#20351#29992
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
  object lblMessage: TLabel
    Left = 0
    Top = 472
    Width = 713
    Height = 121
    AutoSize = False
    Color = clBtnHighlight
    ParentColor = False
  end
  object Label47: TLabel
    Left = 120
    Top = 22
    Width = 102
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #30446#21069#24037#20316#21040#32887#24180#26376
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
        Left = 196
        Top = 276
        Width = 57
        Height = 13
        AutoSize = False
        Caption = #36914#20214#21729#24037
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label50: TLabel
        Left = 8
        Top = 276
        Width = 57
        Height = 13
        AutoSize = False
        Caption = #36914#20214#20998#34892
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
        Height = 161
        Caption = #30003#35531#20154#22522#26412#36039#26009
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
          Left = 555
          Top = 30
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
          Left = 4
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
        object Label3: TLabel
          Left = 8
          Top = 64
          Width = 57
          Height = 16
          AutoSize = False
          Caption = #23130#23035#29376#27841
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label8: TLabel
          Left = 324
          Top = 64
          Width = 57
          Height = 16
          AutoSize = False
          Caption = #25945#32946#31243#24230
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label10: TLabel
          Left = 475
          Top = 64
          Width = 41
          Height = 16
          AutoSize = False
          Caption = #24180#25910#20837
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label24: TLabel
          Left = 159
          Top = 64
          Width = 73
          Height = 14
          AutoSize = False
          Caption = #25206#39178#23376#22899#25976
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label9: TLabel
          Left = 593
          Top = 64
          Width = 17
          Height = 16
          AutoSize = False
          Caption = #20803
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object medtID: TMaskEdit
          Left = 186
          Top = 24
          Width = 89
          Height = 24
          AutoSize = False
          CharCase = ecUpperCase
          EditMask = '!L9999999999;0;_'
          ImeMode = imDisable
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 11
          TabOrder = 1
        end
        object medtBirthYear: TMaskEdit
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
        object medtBirthMonth: TMaskEdit
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
          Left = 641
          Top = 24
          Width = 29
          Height = 24
          AutoSize = False
          EditMask = '!999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 5
        end
        object medtBirthDate: TMaskEdit
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
        object edtName: TEdit
          Left = 41
          Top = 26
          Width = 64
          Height = 24
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 0
        end
        object medtAnnualIncome: TMaskEdit
          Left = 518
          Top = 59
          Width = 73
          Height = 24
          EditMask = '999999999;1;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 9
          TabOrder = 9
          Text = '         '
        end
        object cmbChildren: TComboBox
          Left = 231
          Top = 59
          Width = 81
          Height = 24
          Style = csDropDownList
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 16
          TabOrder = 7
          Items.Strings = (
            #28961
            #19968#20491
            #20108#20491
            #19977#20491#20197#19978
            #20854#20182)
        end
        object cmbMarriage: TComboBox
          Left = 66
          Top = 59
          Width = 81
          Height = 24
          Style = csDropDownList
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 16
          TabOrder = 6
          Items.Strings = (
            #26410#23130
            #24050#23130
            #38626#23130
            #20854#20182)
        end
        object cmbEducation: TComboBox
          Left = 382
          Top = 59
          Width = 81
          Height = 24
          Style = csDropDownList
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 16
          TabOrder = 8
          Items.Strings = (
            #22283#20013
            #39640#20013
            #22823#23560
            #30889#22763
            #21338#22763
            #20854#20182)
        end
        object GroupBox5: TGroupBox
          Left = 16
          Top = 96
          Width = 185
          Height = 57
          Caption = #22266#32178#38651#35441
          TabOrder = 10
          object Label26: TLabel
            Left = 8
            Top = 28
            Width = 33
            Height = 16
            AutoSize = False
            Caption = #21312#30908
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label27: TLabel
            Left = 83
            Top = 28
            Width = 24
            Height = 16
            AutoSize = False
            Caption = #34399#30908
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object medtFixedPhoneRegion: TMaskEdit
            Left = 36
            Top = 24
            Width = 33
            Height = 24
            EditMask = '!999;1;_'
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 3
            TabOrder = 0
            Text = '   '
          end
          object medtFixedPhoneNumber: TMaskEdit
            Left = 111
            Top = 22
            Width = 66
            Height = 24
            EditMask = '99999999;1;_'
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 8
            TabOrder = 1
            Text = '        '
          end
        end
        object GroupBox6: TGroupBox
          Left = 216
          Top = 95
          Width = 185
          Height = 57
          Caption = #34892#21205#38651#35441
          TabOrder = 11
          object Label29: TLabel
            Left = 8
            Top = 28
            Width = 33
            Height = 16
            AutoSize = False
            Caption = #21312#30908
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label30: TLabel
            Left = 83
            Top = 28
            Width = 24
            Height = 16
            AutoSize = False
            Caption = #34399#30908
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object medtCellRegion: TMaskEdit
            Left = 36
            Top = 24
            Width = 31
            Height = 24
            EditMask = '!9999;1;_'
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 4
            TabOrder = 0
            Text = '    '
          end
          object medtCellNumber: TMaskEdit
            Left = 111
            Top = 22
            Width = 66
            Height = 24
            EditMask = '99999999;1;_'
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 8
            TabOrder = 1
            Text = '        '
          end
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
        TabOrder = 6
        OnClick = btnExit1Click
      end
      object GroupBox2: TGroupBox
        Left = 0
        Top = 178
        Width = 705
        Height = 78
        Caption = #36024#27454#36039#26009
        Color = clBtnFace
        ParentColor = False
        TabOrder = 1
        object Label12: TLabel
          Left = 8
          Top = 53
          Width = 57
          Height = 16
          Alignment = taRightJustify
          AutoSize = False
          Caption = #30003#36024#37329#38989
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 160
          Top = 53
          Width = 57
          Height = 12
          Alignment = taRightJustify
          AutoSize = False
          Caption = #30003#36024#21033#29575
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object Label4: TLabel
          Left = 283
          Top = 53
          Width = 57
          Height = 12
          Alignment = taRightJustify
          AutoSize = False
          Caption = #30003#36024#26399#25976
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object Label5: TLabel
          Left = 122
          Top = 55
          Width = 31
          Height = 16
          AutoSize = False
          Caption = #33836#20803
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label39: TLabel
          Left = 260
          Top = 54
          Width = 12
          Height = 16
          Caption = '%'
        end
        object Label44: TLabel
          Left = 400
          Top = 54
          Width = 41
          Height = 13
          AutoSize = False
          Caption = #38283#36774#36027
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label45: TLabel
          Left = 373
          Top = 54
          Width = 17
          Height = 13
          AutoSize = False
          Caption = #26376
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label46: TLabel
          Left = 493
          Top = 54
          Width = 17
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
        object Label43: TLabel
          Left = 8
          Top = 25
          Width = 57
          Height = 16
          Alignment = taRightJustify
          AutoSize = False
          Caption = #23560#26696#31278#39006
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object medtAmountApplied: TMaskEdit
          Left = 67
          Top = 48
          Width = 50
          Height = 24
          EditMask = '9999999;0;_'
          ImeMode = imDisable
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 7
          TabOrder = 1
        end
        object medtTerms: TMaskEdit
          Left = 342
          Top = 47
          Width = 28
          Height = 24
          EditMask = '999;1;_'
          ImeMode = imDisable
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 3
          Text = '   '
        end
        object medtInterestRate: TMaskEdit
          Left = 222
          Top = 48
          Width = 35
          Height = 24
          EditMask = '99\.99;1;_'
          ImeMode = imDisable
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 5
          TabOrder = 2
          Text = '  .  '
        end
        object medtApplicationFee: TMaskEdit
          Left = 447
          Top = 48
          Width = 42
          Height = 24
          EditMask = '99999;1;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 5
          TabOrder = 4
          Text = '     '
        end
        object cmbProduct: TComboBox
          Left = 67
          Top = 19
          Width = 61
          Height = 21
          Style = csDropDownList
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ParentFont = False
          TabOrder = 0
          Items.Strings = (
            #20415#21033#36890
            #34218#20778#36024)
        end
      end
      object edtChannel: TEdit
        Left = 254
        Top = 270
        Width = 121
        Height = 24
        ImeMode = imDisable
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        MaxLength = 16
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
        TabOrder = 4
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
        TabOrder = 5
        OnClick = btnClearClick
      end
      object edtBranch: TEdit
        Left = 65
        Top = 270
        Width = 120
        Height = 24
        ImeMode = imClose
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        MaxLength = 16
        TabOrder = 2
      end
    end
    object tsReportGen: TTabSheet
      Caption = #22577#34920#27284#29986#29983
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
        TabOrder = 2
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
        TabOrder = 0
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
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 2
          ParentFont = False
          TabOrder = 4
        end
      end
    end
  end
end
