object formMain: TformMain
  Left = 84
  Top = 74
  Width = 868
  Height = 595
  Caption = 'DAC'#28136#20540#25151#36024#35413#20998#31995#32113#65288#34892#37559#23560#26696#29256#65289#9472#25552#20379#20140#22478#37504#34892#20351#29992
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
    Left = -1
    Top = 456
    Width = 858
    Height = 106
    AutoSize = False
    Color = clBtnHighlight
    ParentColor = False
    WordWrap = True
  end
  object Label4: TLabel
    Left = 0
    Top = 432
    Width = 65
    Height = 13
    AutoSize = False
    Caption = #22519#34892#35338#24687
  end
  object PageControlMain: TPageControl
    Left = 0
    Top = 0
    Width = 857
    Height = 425
    ActivePage = TabSheet2
    TabIndex = 1
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #21021#23529#26696#20214
      object GroupBoxApp: TGroupBox
        Left = 8
        Top = 8
        Width = 825
        Height = 57
        Caption = #30003#35531#20154#22522#26412#36039#26009
        TabOrder = 0
        object Label15: TLabel
          Left = 180
          Top = 20
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
          Left = 10
          Top = 20
          Width = 71
          Height = 13
          AutoSize = False
          Caption = #26696#20214#32232#34399
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label14: TLabel
          Left = 370
          Top = 20
          Width = 73
          Height = 16
          AutoSize = False
          Caption = #22995'        '#21517
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label11: TLabel
          Left = 544
          Top = 20
          Width = 49
          Height = 13
          AutoSize = False
          Caption = #24180#25910#20837
        end
        object Label2: TLabel
          Left = 648
          Top = 20
          Width = 32
          Height = 15
          AutoSize = False
          Caption = #33836#20803
        end
        object medtPrimaryID: TMaskEdit
          Left = 260
          Top = 20
          Width = 85
          Height = 21
          AutoSize = False
          CharCase = ecUpperCase
          EditMask = '>L<000000000;1;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 10
          TabOrder = 1
          Text = '          '
          OnExit = medtPrimaryIDExit
        end
        object edtPrimaryName: TEdit
          Left = 440
          Top = 20
          Width = 95
          Height = 21
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 2
        end
        object edtCaseNo: TEdit
          Left = 72
          Top = 20
          Width = 89
          Height = 21
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 0
        end
        object edtIncome: TMaskEdit
          Left = 595
          Top = 20
          Width = 37
          Height = 21
          EditMask = '99999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 5
          TabOrder = 3
        end
      end
      object GroupBoxProd: TGroupBox
        Left = 8
        Top = 72
        Width = 825
        Height = 49
        Caption = #36024#27454#36039#26009
        TabOrder = 1
        object Label3: TLabel
          Left = 8
          Top = 16
          Width = 65
          Height = 13
          AutoSize = False
          Caption = #30003#36024#37329#38989
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label5: TLabel
          Left = 178
          Top = 16
          Width = 57
          Height = 13
          AutoSize = False
          Caption = #30003#36024#26399#25976
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label12: TLabel
          Left = 112
          Top = 16
          Width = 33
          Height = 13
          AutoSize = False
          Caption = #33836#20803
        end
        object Label20: TLabel
          Left = 336
          Top = 16
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #24180
        end
        object edtAppAmount: TMaskEdit
          Left = 72
          Top = 16
          Width = 27
          Height = 21
          EditMask = '999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 0
        end
        object cbPeriod: TComboBox
          Left = 264
          Top = 16
          Width = 65
          Height = 21
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          TabOrder = 1
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            '3'
            '5'
            '7')
        end
      end
      object btnPrescreen: TButton
        Left = 200
        Top = 354
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
        OnClick = btnPrescreenClick
      end
      object btnClear: TButton
        Left = 376
        Top = 354
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
      object btnExit1: TButton
        Left = 752
        Top = 354
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
      object GroupBox_p1: TGroupBox
        Left = 7
        Top = 130
        Width = 826
        Height = 87
        Caption = #31532#19968#20214#25812#20445#21697#36039#26009
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        object Label23: TLabel
          Left = 8
          Top = 24
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #25152#26377#20154#22995#21517
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label56: TLabel
          Left = 248
          Top = 24
          Width = 121
          Height = 13
          AutoSize = False
          Caption = #25152#26377#20154#36523#20998#35657#23383#34399
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label58: TLabel
          Left = 8
          Top = 48
          Width = 105
          Height = 13
          AutoSize = False
          Caption = #25812#20445#21697#38272#29260#34399#30908
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label39: TLabel
          Left = 488
          Top = 24
          Width = 89
          Height = 13
          AutoSize = False
          Caption = #33287#30003#35531#20154#38364#20418
        end
        object edtOwnerName1: TEdit
          Left = 112
          Top = 24
          Width = 121
          Height = 21
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 0
        end
        object edtLandNum1: TEdit
          Left = 112
          Top = 48
          Width = 553
          Height = 21
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 3
        end
        object relationship1: TComboBox
          Left = 576
          Top = 24
          Width = 89
          Height = 21
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 2
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            #26412#20154
            #29238#27597
            #37197#20598
            #23376#22899
            #20854#20182)
        end
        object edtOwnerID1: TMaskEdit
          Left = 368
          Top = 24
          Width = 85
          Height = 21
          EditMask = '>L<000000000;1;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 10
          TabOrder = 1
          Text = '          '
        end
      end
      object GroupBox_p2: TGroupBox
        Left = 7
        Top = 226
        Width = 826
        Height = 87
        Caption = #31532#20108#20214#25812#20445#21697#36039#26009
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        object Label36: TLabel
          Left = 8
          Top = 24
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #25152#26377#20154#22995#21517
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label37: TLabel
          Left = 248
          Top = 24
          Width = 121
          Height = 13
          AutoSize = False
          Caption = #25152#26377#20154#36523#20998#35657#23383#34399
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label38: TLabel
          Left = 8
          Top = 48
          Width = 113
          Height = 13
          AutoSize = False
          Caption = #25812#20445#21697#38272#29260#34399#30908
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label53: TLabel
          Left = 488
          Top = 24
          Width = 89
          Height = 13
          AutoSize = False
          Caption = #33287#30003#35531#20154#38364#20418
          Enabled = False
        end
        object edtOwnerName2: TEdit
          Left = 112
          Top = 24
          Width = 121
          Height = 21
          Enabled = False
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 1
        end
        object edtLandNum2: TEdit
          Left = 112
          Top = 48
          Width = 553
          Height = 21
          Enabled = False
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 4
        end
        object relationship2: TComboBox
          Left = 576
          Top = 24
          Width = 89
          Height = 21
          Enabled = False
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 3
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            #26412#20154
            #29238#27597
            #37197#20598
            #23376#22899
            #20854#20182)
        end
        object edtOwnerID2: TMaskEdit
          Left = 368
          Top = 24
          Width = 85
          Height = 21
          Enabled = False
          EditMask = '>L<000000000;1;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 10
          TabOrder = 2
          Text = '          '
        end
        object cbP2: TCheckBox
          Left = 120
          Top = 0
          Width = 25
          Height = 17
          TabOrder = 0
          OnClick = cbP2Click
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = #35079#23529#26696#20214
      ImageIndex = 1
      object Label55: TLabel
        Left = 8
        Top = 16
        Width = 137
        Height = 13
        AutoSize = False
        Caption = #36890#36942#21021#23529#30340#30003#35531#20214#32232#34399
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Select: TButton
        Left = 288
        Top = 8
        Width = 75
        Height = 25
        Caption = #36664#20837
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = SelectClick
      end
      object finalReview: TButton
        Left = 184
        Top = 360
        Width = 75
        Height = 25
        Caption = #35413#20998
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnClick = finalReviewClick
      end
      object btnExit2: TButton
        Left = 744
        Top = 360
        Width = 75
        Height = 25
        Caption = #32080#26463#31243#24335
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
        OnClick = btnExit1Click
      end
      object edtMSN: TMaskEdit
        Left = 152
        Top = 8
        Width = 89
        Height = 21
        EditMask = 'a0000000000009;1;_'
        ImeMode = imClose
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        MaxLength = 14
        TabOrder = 0
        Text = '              '
      end
      object GroupBox1: TGroupBox
        Left = 0
        Top = 48
        Width = 825
        Height = 49
        Caption = #30003#35531#20154#22522#26412#36039#26009
        TabOrder = 2
        object Label69: TLabel
          Left = 370
          Top = 24
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #36523#20998#35657#23383#34399#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label75: TLabel
          Left = 180
          Top = 24
          Width = 65
          Height = 13
          AutoSize = False
          Caption = #22995'        '#21517#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblPrimaryID: TLabel
          Left = 448
          Top = 24
          Width = 113
          Height = 13
          AutoSize = False
        end
        object lblPrimaryName: TLabel
          Left = 263
          Top = 24
          Width = 98
          Height = 13
          AutoSize = False
        end
        object Label72: TLabel
          Left = 8
          Top = 24
          Width = 60
          Height = 13
          Caption = #26696#20214#32232#34399#65306
        end
        object lblMSN: TLabel
          Left = 72
          Top = 24
          Width = 97
          Height = 13
          AutoSize = False
        end
        object hidden_SystemDate: TLabel
          Left = 616
          Top = 24
          Width = 95
          Height = 13
          Caption = 'hidden_SystemDate'
          Visible = False
        end
        object hidden_Zip: TLabel
          Left = 744
          Top = 24
          Width = 53
          Height = 13
          Caption = 'hidden_Zip'
          Visible = False
        end
      end
      object GroupBox3: TGroupBox
        Left = -1
        Top = 106
        Width = 826
        Height = 119
        Caption = #31532#19968#20214#25812#20445#21697#36039#26009
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        object Label92: TLabel
          Left = 8
          Top = 24
          Width = 121
          Height = 13
          AutoSize = False
          Caption = #25152#26377#20154#36523#20998#35657#23383#34399#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label93: TLabel
          Left = 232
          Top = 24
          Width = 105
          Height = 13
          AutoSize = False
          Caption = #25812#20445#21697#38272#29260#34399#30908#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label95: TLabel
          Left = 8
          Top = 48
          Width = 65
          Height = 13
          AutoSize = False
          Caption = #37969#20272#32317#20540
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label97: TLabel
          Left = 232
          Top = 48
          Width = 65
          Height = 13
          AutoSize = False
          Caption = #37969#20272#28136#20540
        end
        object Label98: TLabel
          Left = 360
          Top = 48
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
        end
        object Label99: TLabel
          Left = 80
          Top = 80
          Width = 193
          Height = 13
          AutoSize = False
        end
        object lblOwnerID1: TLabel
          Left = 136
          Top = 24
          Width = 81
          Height = 13
          AutoSize = False
        end
        object lblLandHouseNum1: TLabel
          Left = 344
          Top = 24
          Width = 369
          Height = 13
          AutoSize = False
        end
        object Label74: TLabel
          Left = 152
          Top = 48
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
        end
        object Label13: TLabel
          Left = 232
          Top = 80
          Width = 161
          Height = 13
          AutoSize = False
          Caption = #25812#20445#21697#26159#21542#31526#21512#25215#20316#26781#20214
        end
        object Label24: TLabel
          Left = 8
          Top = 80
          Width = 89
          Height = 13
          AutoSize = False
          Caption = #25269#25276#32317#37329#38989
        end
        object Label26: TLabel
          Left = 152
          Top = 80
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
        end
        object hidden_appAmount: TLabel
          Left = 648
          Top = 80
          Width = 92
          Height = 13
          AutoSize = False
          Caption = 'hidden_appAmount'
          Visible = False
        end
        object hidden_APR: TLabel
          Left = 648
          Top = 64
          Width = 60
          Height = 13
          AutoSize = False
          Caption = 'hidden_APR'
          Visible = False
        end
        object hidden_appFee: TLabel
          Left = 649
          Top = 96
          Width = 74
          Height = 13
          Caption = 'hidden_appFee'
          Visible = False
        end
        object hidden_InquiryDate: TLabel
          Left = 648
          Top = 48
          Width = 92
          Height = 13
          Caption = 'hidden_InquiryDate'
          Visible = False
        end
        object hidden_monthly_income: TLabel
          Left = 648
          Top = 32
          Width = 114
          Height = 13
          Caption = 'hidden_monthly_income'
          Visible = False
        end
        object nav1: TMaskEdit
          Left = 296
          Top = 44
          Width = 61
          Height = 21
          EditMask = '99999999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 1
        end
        object rgQualified1: TRadioGroup
          Left = 396
          Top = 68
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 1
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 3
        end
        object gav1: TMaskEdit
          Left = 80
          Top = 44
          Width = 65
          Height = 21
          EditMask = '99999999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 0
        end
        object edtLienValue1: TMaskEdit
          Left = 80
          Top = 72
          Width = 65
          Height = 21
          EditMask = '999999999;0;_'
          ImeMode = imClose
          MaxLength = 9
          TabOrder = 2
        end
      end
      object GroupBox4: TGroupBox
        Left = -1
        Top = 234
        Width = 826
        Height = 111
        Caption = #31532#20108#20214#25812#20445#21697#36039#26009
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        object Label76: TLabel
          Left = 8
          Top = 24
          Width = 129
          Height = 13
          AutoSize = False
          Caption = #25152#26377#20154#36523#20998#35657#23383#34399#65306
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label77: TLabel
          Left = 232
          Top = 24
          Width = 105
          Height = 13
          AutoSize = False
          Caption = #25812#20445#21697#38272#29260#34399#30908#65306
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label78: TLabel
          Left = 8
          Top = 48
          Width = 65
          Height = 13
          AutoSize = False
          Caption = #37969#20272#32317#20540
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label79: TLabel
          Left = 232
          Top = 48
          Width = 65
          Height = 13
          AutoSize = False
          Caption = #37969#20272#28136#20540
          Enabled = False
        end
        object Label80: TLabel
          Left = 360
          Top = 48
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
          Enabled = False
        end
        object Label81: TLabel
          Left = 232
          Top = 80
          Width = 169
          Height = 13
          AutoSize = False
          Caption = #25812#20445#21697#26159#21542#31526#21512#25215#20316#26781#20214
          Enabled = False
        end
        object lblOwnerID2: TLabel
          Left = 136
          Top = 24
          Width = 61
          Height = 13
          AutoSize = False
          Enabled = False
        end
        object lblLandHouseNum2: TLabel
          Left = 352
          Top = 24
          Width = 369
          Height = 13
          AutoSize = False
          Enabled = False
        end
        object Label106: TLabel
          Left = 152
          Top = 48
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
          Enabled = False
        end
        object Label25: TLabel
          Left = 8
          Top = 80
          Width = 81
          Height = 13
          AutoSize = False
          Caption = #25269#25276#32317#37329#38989
          Enabled = False
        end
        object Label27: TLabel
          Left = 152
          Top = 80
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
          Enabled = False
        end
        object nav2: TMaskEdit
          Left = 296
          Top = 44
          Width = 61
          Height = 21
          Enabled = False
          EditMask = '99999999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 1
        end
        object rgQualified2: TRadioGroup
          Left = 396
          Top = 68
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 1
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 3
        end
        object gav2: TMaskEdit
          Left = 80
          Top = 44
          Width = 65
          Height = 21
          Enabled = False
          EditMask = '99999999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 0
        end
        object edtLienValue2: TMaskEdit
          Left = 80
          Top = 72
          Width = 65
          Height = 21
          EditMask = '999999999;0;_'
          ImeMode = imClose
          MaxLength = 9
          TabOrder = 2
        end
      end
      object finalReview_Clear: TButton
        Left = 296
        Top = 360
        Width = 75
        Height = 25
        Caption = #28165#38500
        TabOrder = 6
        OnClick = finalReview_ClearClick
      end
    end
    object TabSheet3: TTabSheet
      Caption = #23529#26597#22577#34920
      ImageIndex = 2
      object GroupBox9: TGroupBox
        Left = 0
        Top = 80
        Width = 513
        Height = 65
        Caption = #35079#23529#26696#20214#26085#22577#34920
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object Label41: TLabel
          Left = 169
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
        object Label42: TLabel
          Left = 240
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
        object Label43: TLabel
          Left = 288
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
        object Label44: TLabel
          Left = 344
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
        object btnFinalreviewDir: TButton
          Left = 8
          Top = 24
          Width = 129
          Height = 25
          Caption = #35373#23450#27284#26696#29986#29983#30446#37636
          TabOrder = 0
          OnClick = btnFinalreviewDirClick
        end
        object final_year: TMaskEdit
          Left = 207
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
        object final_month: TMaskEdit
          Left = 259
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
        object final_day: TMaskEdit
          Left = 311
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
        object btnFinalreviewReport: TButton
          Left = 387
          Top = 24
          Width = 75
          Height = 25
          Caption = #29986#29983#27284#26696
          TabOrder = 4
          OnClick = btnFinalReportClick
        end
      end
      object btnExit3: TButton
        Left = 160
        Top = 168
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
        OnClick = btnExit1Click
      end
      object GroupBox5: TGroupBox
        Left = 0
        Top = 8
        Width = 513
        Height = 65
        Caption = #21021#23529#26696#20214#26085#22577#34920
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object Label40: TLabel
          Left = 169
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
        object Label45: TLabel
          Left = 240
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
        object Label107: TLabel
          Left = 288
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
        object Label108: TLabel
          Left = 344
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
        object btnPreviewDir: TButton
          Left = 8
          Top = 24
          Width = 129
          Height = 25
          Caption = #35373#23450#27284#26696#29986#29983#30446#37636
          TabOrder = 0
          OnClick = btnPreviewDirClick
        end
        object primier_year: TMaskEdit
          Left = 207
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
        object primier_month: TMaskEdit
          Left = 259
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
        object primier_day: TMaskEdit
          Left = 311
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
        object btnPreviewReport: TButton
          Left = 387
          Top = 24
          Width = 75
          Height = 25
          Caption = #29986#29983#27284#26696
          TabOrder = 4
          OnClick = btnPreviewReportClick
        end
      end
    end
  end
end