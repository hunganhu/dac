object frmMainUI: TfrmMainUI
  Left = 210
  Top = 61
  BorderStyle = bsDialog
  Caption = 'DAC '#20491#20154#20449#36024#35413#20998#31995#32113'('#34892#37559#23560#26696#29256') - '#25552#20379#20140#22478#37504#34892#20351#29992
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
      object GroupBox1: TGroupBox
        Left = 0
        Top = 8
        Width = 705
        Height = 121
        Caption = #30003#35531#20154#22522#26412#36039#26009
        TabOrder = 0
        object Label15: TLabel
          Left = 172
          Top = 30
          Width = 68
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
        object Label14: TLabel
          Left = 342
          Top = 30
          Width = 29
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
        object Label1: TLabel
          Left = 8
          Top = 30
          Width = 57
          Height = 13
          AutoSize = False
          Caption = #37109#23492#32232#34399
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 439
          Top = 30
          Width = 41
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
        object Label6: TLabel
          Left = 546
          Top = 31
          Width = 25
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
        object medtID: TMaskEdit
          Left = 244
          Top = 23
          Width = 89
          Height = 24
          AutoSize = False
          CharCase = ecUpperCase
          EditMask = '!L9999999999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 11
          TabOrder = 1
          OnExit = medtIDExit
        end
        object GroupBox5: TGroupBox
          Left = 8
          Top = 52
          Width = 209
          Height = 57
          Caption = #32879#32097#38651#35441
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object Label3: TLabel
            Left = 92
            Top = 30
            Width = 29
            Height = 13
            Alignment = taRightJustify
            AutoSize = False
            Caption = #34399#30908
          end
          object Label24: TLabel
            Left = 14
            Top = 31
            Width = 33
            Height = 13
            Alignment = taRightJustify
            AutoSize = False
            Caption = #21312#30908
          end
          object medtPhone: TMaskEdit
            Left = 125
            Top = 26
            Width = 69
            Height = 21
            EditMask = '9999999999;1;_'
            ImeMode = imDisable
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 10
            TabOrder = 1
            Text = '          '
          end
          object medtRegion: TMaskEdit
            Left = 51
            Top = 26
            Width = 33
            Height = 21
            EditMask = '!9999;1;_'
            ImeMode = imDisable
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 4
            TabOrder = 0
            Text = '    '
          end
        end
        object GroupBox7: TGroupBox
          Left = 226
          Top = 52
          Width = 201
          Height = 57
          Caption = #34892#21205#38651#35441
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          object Label8: TLabel
            Left = 87
            Top = 30
            Width = 29
            Height = 13
            Alignment = taRightJustify
            AutoSize = False
            Caption = #34399#30908
          end
          object Label9: TLabel
            Left = 9
            Top = 31
            Width = 33
            Height = 13
            Alignment = taRightJustify
            AutoSize = False
            Caption = #21312#30908
          end
          object medtCellPhone: TMaskEdit
            Left = 120
            Top = 26
            Width = 69
            Height = 21
            EditMask = '9999999999;1;_'
            ImeMode = imDisable
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 10
            TabOrder = 1
            Text = '          '
          end
          object medtCellRegion: TMaskEdit
            Left = 46
            Top = 26
            Width = 33
            Height = 21
            EditMask = '!9999;1;_'
            ImeMode = imDisable
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            MaxLength = 4
            TabOrder = 0
            Text = '    '
          end
        end
        object medtMSN: TMaskEdit
          Left = 66
          Top = 24
          Width = 103
          Height = 24
          AutoSize = False
          EditMask = '0000000000000;1;_'
          ImeMode = imDisable
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 13
          TabOrder = 0
          Text = '             '
          OnKeyDown = medtMSNKeyDown
        end
        object edtName: TEdit
          Left = 373
          Top = 24
          Width = 57
          Height = 24
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 2
        end
        object medtMonthlyIncome: TMaskEdit
          Left = 483
          Top = 24
          Width = 60
          Height = 24
          AutoSize = False
          CharCase = ecUpperCase
          EditMask = '9999999;1;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 7
          TabOrder = 5
          Text = '       '
          OnExit = medtIDExit
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
        TabOrder = 4
        OnClick = btnExit1Click
      end
      object GroupBox2: TGroupBox
        Left = 0
        Top = 138
        Width = 273
        Height = 65
        Caption = #36024#27454#36039#26009
        Color = clBtnFace
        ParentColor = False
        TabOrder = 1
        object Label12: TLabel
          Left = 8
          Top = 31
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
        object Label4: TLabel
          Left = 138
          Top = 31
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
          Left = 104
          Top = 33
          Width = 25
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
        object Label45: TLabel
          Left = 249
          Top = 32
          Width = 17
          Height = 13
          AutoSize = False
          Caption = #24180
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object medtAmountApplied: TMaskEdit
          Left = 67
          Top = 26
          Width = 30
          Height = 24
          EditMask = '999;1;_'
          ImeMode = imDisable
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 0
          Text = '   '
        end
        object cmbAppliedTerms: TComboBox
          Left = 198
          Top = 26
          Width = 49
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
          TabOrder = 1
          Items.Strings = (
            '2'
            '3'
            '5'
            '7')
        end
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
        TabOrder = 2
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
        TabOrder = 3
        OnClick = btnClearClick
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
          OnKeyPress = btnRptRegenKeyPress
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
    end
  end
end
