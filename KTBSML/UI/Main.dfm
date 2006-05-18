object formMain: TformMain
  Left = 125
  Top = 77
  Width = 868
  Height = 688
  Caption = 'DAC'#20108#38918#20301#25151#36024#35413#20998#31995#32113#9472#25552#20379#20140#22478#37504#34892#20351#29992
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
    Top = 578
    Width = 858
    Height = 71
    AutoSize = False
    Color = clBtnHighlight
    ParentColor = False
    WordWrap = True
  end
  object PageControlMain: TPageControl
    Left = 0
    Top = 0
    Width = 857
    Height = 577
    ActivePage = TabSheet3
    TabIndex = 2
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #21021#23529#26696#20214
      object Label67: TLabel
        Left = 8
        Top = 515
        Width = 65
        Height = 13
        AutoSize = False
        Caption = #36914#20214#20998#34892
      end
      object Label68: TLabel
        Left = 152
        Top = 515
        Width = 65
        Height = 13
        AutoSize = False
        Caption = #36914#20214#21729#24037
      end
      object GroupBoxApp: TGroupBox
        Left = 8
        Top = 8
        Width = 825
        Height = 81
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
          Left = 370
          Top = 20
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
          Left = 525
          Top = 20
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
          Left = 575
          Top = 20
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
          Left = 625
          Top = 20
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
        object Label7: TLabel
          Left = 450
          Top = 20
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
        object Label6: TLabel
          Left = 8
          Top = 50
          Width = 57
          Height = 13
          AutoSize = False
          Caption = #23130#23035#29376#27841
        end
        object Label8: TLabel
          Left = 180
          Top = 50
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #25771#39178#23376#22899#25976
        end
        object Label9: TLabel
          Left = 370
          Top = 50
          Width = 65
          Height = 13
          AutoSize = False
          Caption = #25945#32946#31243#24230
        end
        object Label10: TLabel
          Left = 544
          Top = 50
          Width = 33
          Height = 13
          AutoSize = False
          Caption = #32887#26989
        end
        object Label11: TLabel
          Left = 688
          Top = 50
          Width = 49
          Height = 13
          AutoSize = False
          Caption = #24180#25910#20837
        end
        object Label2: TLabel
          Left = 785
          Top = 50
          Width = 32
          Height = 13
          AutoSize = False
          Caption = #33836#20803
        end
        object Label48: TLabel
          Left = 324
          Top = 50
          Width = 12
          Height = 13
          Caption = #20154
        end
        object Label70: TLabel
          Left = 688
          Top = 20
          Width = 65
          Height = 13
          AutoSize = False
          Caption = ' '#37109#36958#21312#34399
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
        end
        object medtPrimaryBirthYear: TMaskEdit
          Left = 490
          Top = 18
          Width = 28
          Height = 21
          AutoSize = False
          EditMask = '!999;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 2
        end
        object medtPrimaryBirthMonth: TMaskEdit
          Left = 544
          Top = 18
          Width = 25
          Height = 21
          AutoSize = False
          EditMask = '!99;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 2
          TabOrder = 3
        end
        object medtPrimaryBirthDate: TMaskEdit
          Left = 595
          Top = 18
          Width = 25
          Height = 21
          AutoSize = False
          EditMask = '!99;0;_'
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 2
          TabOrder = 4
        end
        object edtPrimaryName: TEdit
          Left = 72
          Top = 18
          Width = 95
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 0
        end
        object marriage: TComboBox
          Left = 72
          Top = 48
          Width = 97
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 6
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            #26410#23130
            #24050#23130
            #38626#23130)
        end
        object child: TComboBox
          Left = 260
          Top = 50
          Width = 61
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 7
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            '0'
            '1'
            '2'
            '2'#20154#20197#19978)
        end
        object education: TComboBox
          Left = 432
          Top = 50
          Width = 89
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 8
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            #30740#31350#25152
            #22823#23416
            #23560#31185
            #39640#12289#22283#20013)
        end
        object edtCareer: TEdit
          Left = 576
          Top = 50
          Width = 89
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 9
        end
        object edtIncome: TMaskEdit
          Left = 736
          Top = 50
          Width = 37
          Height = 21
          EditMask = '99999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 5
          TabOrder = 10
        end
        object edtZip: TMaskEdit
          Left = 752
          Top = 18
          Width = 30
          Height = 21
          EditMask = '000;1;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 5
          Text = '   '
        end
      end
      object GroupBoxProd: TGroupBox
        Left = 8
        Top = 96
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
        object Label4: TLabel
          Left = 180
          Top = 16
          Width = 65
          Height = 13
          AutoSize = False
          Caption = #30003#36024#21033#29575
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label5: TLabel
          Left = 370
          Top = 16
          Width = 57
          Height = 13
          AutoSize = False
          Caption = #30003#36024#26399#38291
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
        object Label19: TLabel
          Left = 330
          Top = 16
          Width = 23
          Height = 13
          AutoSize = False
          Caption = '%'
        end
        object Label20: TLabel
          Left = 512
          Top = 16
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #24180
        end
        object Label21: TLabel
          Left = 544
          Top = 16
          Width = 49
          Height = 13
          AutoSize = False
          Caption = #38283#36774#36027
        end
        object Label22: TLabel
          Left = 648
          Top = 16
          Width = 25
          Height = 13
          AutoSize = False
          Caption = #20803
        end
        object edtAppAmount: TMaskEdit
          Left = 72
          Top = 16
          Width = 27
          Height = 21
          EditMask = '999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 0
        end
        object cbPeriod: TComboBox
          Left = 440
          Top = 16
          Width = 65
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 2
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            '3'
            '5'
            '7')
        end
        object edtAppFee: TMaskEdit
          Left = 595
          Top = 16
          Width = 40
          Height = 21
          EditMask = '999999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 6
          TabOrder = 3
        end
        object edtAPR: TEdit
          Left = 260
          Top = 16
          Width = 61
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 1
        end
      end
      object btnPrescreen: TButton
        Left = 392
        Top = 514
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
        OnClick = btnPrescreenClick
      end
      object btnClear: TButton
        Left = 488
        Top = 514
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
      object btnExit1: TButton
        Left = 760
        Top = 514
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
      object GroupBox_p1: TGroupBox
        Left = 7
        Top = 154
        Width = 826
        Height = 175
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
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #22320#34399'/'#24314#34399
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label60: TLabel
          Left = 488
          Top = 48
          Width = 89
          Height = 13
          AutoSize = False
          Caption = #24231#33853#21312#22495
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label62: TLabel
          Left = 8
          Top = 72
          Width = 89
          Height = 13
          AutoSize = False
          Caption = #30446#21069#25269#25276#38918#20301
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
        object Label46: TLabel
          Left = 248
          Top = 72
          Width = 84
          Height = 13
          AutoSize = False
          Caption = #32317#25269#25276#37329#38989
        end
        object Label47: TLabel
          Left = 440
          Top = 72
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
        end
        object Label13: TLabel
          Left = 8
          Top = 104
          Width = 121
          Height = 13
          AutoSize = False
          Caption = #22303#22320#26159#21542#21015#28858#31105#24314
        end
        object Label49: TLabel
          Left = 248
          Top = 104
          Width = 185
          Height = 13
          AutoSize = False
          Caption = #22303#22320#26159#21542#28858#37117#24066#35336#30059#24501#25910#23565#35937
        end
        object Label50: TLabel
          Left = 544
          Top = 104
          Width = 185
          Height = 13
          AutoSize = False
          Caption = #22303#22320#26159#21542#28858#37117#24066#35336#30059#25286#38500#23565#35937
        end
        object Label24: TLabel
          Left = 8
          Top = 140
          Width = 121
          Height = 13
          AutoSize = False
          Caption = #25151#23627#26159#21542#28858#36667#23556#23627
        end
        object Label25: TLabel
          Left = 248
          Top = 140
          Width = 121
          Height = 13
          AutoSize = False
          Caption = #25151#23627#26159#21542#28858#28023#30722#23627
        end
        object Label28: TLabel
          Left = 544
          Top = 140
          Width = 113
          Height = 13
          AutoSize = False
          Caption = #25151#23627#26159#21542#28858#21361#27155
        end
        object edtOwnerName1: TEdit
          Left = 96
          Top = 24
          Width = 121
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 0
        end
        object edtLandNum1: TEdit
          Left = 96
          Top = 48
          Width = 361
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 3
        end
        object lien1: TComboBox
          Left = 96
          Top = 72
          Width = 81
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 5
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            #28961
            #31532#19968#38918#20301
            #31532#20108#38918#20301#25110#20197#19978)
        end
        object relationship1: TComboBox
          Left = 576
          Top = 24
          Width = 89
          Height = 21
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
        object location1: TComboBox
          Left = 576
          Top = 48
          Width = 89
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 4
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            #20303#23429#21312
            #21830#26989#21312
            #20303#21830#28151#21512#21312
            #20854#20182)
        end
        object edtOwnerID1: TMaskEdit
          Left = 368
          Top = 24
          Width = 85
          Height = 21
          EditMask = '>L<000000000;1;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 10
          TabOrder = 1
          Text = '          '
        end
        object edtLienValue1: TMaskEdit
          Left = 368
          Top = 72
          Width = 65
          Height = 21
          EditMask = '99999999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 6
        end
        object rgForbit1: TRadioGroup
          Left = 132
          Top = 92
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 7
        end
        object rgCollect1: TRadioGroup
          Left = 440
          Top = 92
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 8
        end
        object rgDemolish1: TRadioGroup
          Left = 728
          Top = 92
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 9
        end
        object rgHouseRay1: TRadioGroup
          Left = 132
          Top = 128
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 10
        end
        object rgSeasand1: TRadioGroup
          Left = 368
          Top = 128
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 11
        end
        object rgDangerous1: TRadioGroup
          Left = 656
          Top = 128
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 12
        end
      end
      object GroupBox_p2: TGroupBox
        Left = 7
        Top = 330
        Width = 826
        Height = 175
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
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #22320#34399'/'#24314#34399
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label51: TLabel
          Left = 488
          Top = 48
          Width = 89
          Height = 13
          AutoSize = False
          Caption = #24231#33853#21312#22495
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label52: TLabel
          Left = 8
          Top = 72
          Width = 89
          Height = 13
          AutoSize = False
          Caption = #30446#21069#25269#25276#38918#20301
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
        object Label54: TLabel
          Left = 248
          Top = 72
          Width = 84
          Height = 13
          AutoSize = False
          Caption = #32317#25269#25276#37329#38989
          Enabled = False
        end
        object Label57: TLabel
          Left = 440
          Top = 72
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
          Enabled = False
        end
        object Label59: TLabel
          Left = 8
          Top = 104
          Width = 121
          Height = 13
          AutoSize = False
          Caption = #22303#22320#26159#21542#21015#28858#31105#24314
          Enabled = False
        end
        object Label61: TLabel
          Left = 248
          Top = 104
          Width = 185
          Height = 13
          AutoSize = False
          Caption = #22303#22320#26159#21542#28858#37117#24066#35336#30059#24501#25910#23565#35937
          Enabled = False
        end
        object Label63: TLabel
          Left = 544
          Top = 104
          Width = 185
          Height = 13
          AutoSize = False
          Caption = #22303#22320#26159#21542#28858#37117#24066#35336#30059#25286#38500#23565#35937
          Enabled = False
        end
        object Label64: TLabel
          Left = 8
          Top = 140
          Width = 121
          Height = 13
          AutoSize = False
          Caption = #25151#23627#26159#21542#28858#36667#23556#23627
          Enabled = False
        end
        object Label65: TLabel
          Left = 248
          Top = 140
          Width = 121
          Height = 13
          AutoSize = False
          Caption = #25151#23627#26159#21542#28858#28023#30722#23627
          Enabled = False
        end
        object Label66: TLabel
          Left = 544
          Top = 140
          Width = 113
          Height = 13
          AutoSize = False
          Caption = #25151#23627#26159#21542#28858#21361#27155
          Enabled = False
        end
        object edtOwnerName2: TEdit
          Left = 96
          Top = 24
          Width = 121
          Height = 21
          Enabled = False
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 1
        end
        object edtLandNum2: TEdit
          Left = 96
          Top = 48
          Width = 361
          Height = 21
          Enabled = False
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 4
        end
        object lien2: TComboBox
          Left = 96
          Top = 72
          Width = 81
          Height = 21
          Enabled = False
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 6
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            #28961
            #31532#19968#38918#20301
            #31532#20108#38918#20301#25110#20197#19978)
        end
        object relationship2: TComboBox
          Left = 576
          Top = 24
          Width = 89
          Height = 21
          Enabled = False
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
        object location2: TComboBox
          Left = 576
          Top = 48
          Width = 89
          Height = 21
          Enabled = False
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 5
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            #20303#23429#21312
            #21830#26989#21312
            #20303#21830#28151#21512#21312
            #20854#20182)
        end
        object edtOwnerID2: TMaskEdit
          Left = 368
          Top = 24
          Width = 85
          Height = 21
          Enabled = False
          EditMask = '>L<000000000;1;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 10
          TabOrder = 2
          Text = '          '
        end
        object edtLienValue2: TMaskEdit
          Left = 368
          Top = 72
          Width = 65
          Height = 21
          Enabled = False
          EditMask = '99999999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 7
        end
        object rgForbit2: TRadioGroup
          Left = 132
          Top = 92
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 8
        end
        object rgCollect2: TRadioGroup
          Left = 440
          Top = 92
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 9
        end
        object rgDemolish2: TRadioGroup
          Left = 728
          Top = 92
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 10
        end
        object rgHouseRay2: TRadioGroup
          Left = 132
          Top = 128
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 11
        end
        object rgSeasand2: TRadioGroup
          Left = 368
          Top = 128
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 12
        end
        object rgDangerous2: TRadioGroup
          Left = 656
          Top = 128
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 13
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
      object edtBranch: TMaskEdit
        Left = 72
        Top = 512
        Width = 57
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 4
      end
      object edtAgent: TMaskEdit
        Left = 224
        Top = 512
        Width = 57
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 5
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
        Left = 192
        Top = 504
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
        OnClick = finalReviewClick
      end
      object btnExit2: TButton
        Left = 744
        Top = 504
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
      object edtMSN: TMaskEdit
        Left = 152
        Top = 8
        Width = 95
        Height = 21
        EditMask = '>L<0000000000000;1;_'
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
          Left = 260
          Top = 24
          Width = 3
          Height = 13
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
      object GroupBox2: TGroupBox
        Left = 0
        Top = 96
        Width = 825
        Height = 49
        Caption = #36024#27454#36039#26009
        TabOrder = 3
        object Label83: TLabel
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
        object Label84: TLabel
          Left = 180
          Top = 16
          Width = 65
          Height = 13
          AutoSize = False
          Caption = #30003#36024#21033#29575
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label85: TLabel
          Left = 370
          Top = 16
          Width = 57
          Height = 13
          AutoSize = False
          Caption = #30003#36024#26399#38291
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label86: TLabel
          Left = 112
          Top = 16
          Width = 33
          Height = 13
          AutoSize = False
          Caption = #33836#20803
        end
        object Label87: TLabel
          Left = 330
          Top = 16
          Width = 23
          Height = 13
          AutoSize = False
          Caption = '%'
        end
        object Label88: TLabel
          Left = 504
          Top = 16
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #24180
        end
        object Label89: TLabel
          Left = 544
          Top = 16
          Width = 49
          Height = 13
          AutoSize = False
          Caption = #38283#36774#36027
        end
        object Label90: TLabel
          Left = 648
          Top = 16
          Width = 25
          Height = 13
          AutoSize = False
          Caption = #20803
        end
        object edtAppAmount2: TMaskEdit
          Left = 72
          Top = 16
          Width = 29
          Height = 21
          EditMask = '999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 3
          TabOrder = 0
        end
        object cbPeriod2: TComboBox
          Left = 440
          Top = 16
          Width = 65
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 2
          Text = #35531#36984#25799
          Items.Strings = (
            #35531#36984#25799
            '3'
            '5'
            '7')
        end
        object edtAppFee2: TMaskEdit
          Left = 595
          Top = 16
          Width = 40
          Height = 21
          EditMask = '999999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 6
          TabOrder = 3
        end
        object edtAPR2: TEdit
          Left = 260
          Top = 16
          Width = 65
          Height = 21
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 1
        end
      end
      object GroupBox3: TGroupBox
        Left = -1
        Top = 146
        Width = 826
        Height = 175
        Caption = #31532#19968#20214#25812#20445#21697#36039#26009
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
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
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #22320#34399'/'#24314#34399#65306
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
          Caption = #25151#23627#27611#20540
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
          Caption = #25151#23627#28136#20540
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
          Left = 8
          Top = 80
          Width = 321
          Height = 13
          AutoSize = False
          Caption = #26159#21542#28858#39192#23627#21450#23627#27841#28961#20154#31649#29702#19988#22806#35264#12289#20839#37096#30772#25613#22196#37325
        end
        object Label100: TLabel
          Left = 8
          Top = 112
          Width = 313
          Height = 13
          AutoSize = False
          Caption = #30446#21069#26377#25928#20043#31199#36035#22865#32004#26159#21542#36229#36942#20116#24180#25110#26159#19981#23450#26399#32773
        end
        object Label101: TLabel
          Left = 432
          Top = 112
          Width = 129
          Height = 13
          AutoSize = False
          Caption = #26159#21542#20844#23507#22823#27155#22320#19979#23460
        end
        object Label102: TLabel
          Left = 8
          Top = 148
          Width = 377
          Height = 13
          AutoSize = False
          Caption = #29992#36884#26159#21542#28858#31085#31040#20844#26989#12289#23546#24287#12289#31070#22727#12289#36077#22580#12289#31072#22530#12289#25945#22530#12289#24066#22580#12289#37202#24215
        end
        object Label103: TLabel
          Left = 432
          Top = 80
          Width = 273
          Height = 13
          AutoSize = False
          Caption = #26159#21542#20986#20837#35079#38620#25110#26159#38468#36817#26377#29305#31278#29151#26989#30340#23567#22871#25151
        end
        object lblOwnerID1: TLabel
          Left = 136
          Top = 24
          Width = 81
          Height = 13
          AutoSize = False
        end
        object lblLandHouseNum1: TLabel
          Left = 320
          Top = 24
          Width = 369
          Height = 13
          AutoSize = False
        end
        object Label74: TLabel
          Left = 144
          Top = 48
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
        end
        object hidden_FirstLien1: TLabel
          Left = 624
          Top = 32
          Width = 83
          Height = 13
          Caption = 'hidden_FirstLien1'
          Visible = False
        end
        object nav1: TMaskEdit
          Left = 288
          Top = 44
          Width = 61
          Height = 21
          EditMask = '99999999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 1
        end
        object rgDamage1: TRadioGroup
          Left = 332
          Top = 68
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 2
        end
        object rgLent1: TRadioGroup
          Left = 332
          Top = 102
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 4
        end
        object rgBasement1: TRadioGroup
          Left = 560
          Top = 102
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 5
        end
        object rgMisuse1: TRadioGroup
          Left = 408
          Top = 136
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 6
        end
        object rgComplex1: TRadioGroup
          Left = 704
          Top = 68
          Width = 90
          Height = 35
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 3
        end
        object gav1: TMaskEdit
          Left = 72
          Top = 44
          Width = 61
          Height = 21
          EditMask = '99999999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 0
        end
      end
      object GroupBox4: TGroupBox
        Left = -1
        Top = 322
        Width = 826
        Height = 175
        Caption = #31532#20108#20214#25812#20445#21697#36039#26009
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        Visible = False
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
          Width = 73
          Height = 13
          AutoSize = False
          Caption = #22320#34399'/'#24314#34399#65306
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
          Caption = #25151#23627#27611#20540
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
          Caption = #25151#23627#28136#20540
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
          Left = 8
          Top = 80
          Width = 329
          Height = 13
          AutoSize = False
          Caption = #26159#21542#28858#39192#23627#21450#23627#27841#28961#20154#31649#29702#19988#22806#35264#12289#20839#37096#30772#25613#22196#37325
          Enabled = False
        end
        object Label82: TLabel
          Left = 8
          Top = 112
          Width = 321
          Height = 13
          AutoSize = False
          Caption = #30446#21069#26377#25928#20043#31199#36035#22865#32004#26159#21542#36229#36942#20116#24180#25110#26159#19981#23450#26399#32773
          Enabled = False
        end
        object Label91: TLabel
          Left = 432
          Top = 112
          Width = 129
          Height = 13
          AutoSize = False
          Caption = #26159#21542#20844#23507#22823#27155#22320#19979#23460
          Enabled = False
        end
        object Label94: TLabel
          Left = 8
          Top = 148
          Width = 377
          Height = 13
          AutoSize = False
          Caption = #29992#36884#26159#21542#28858#31085#31040#20844#26989#12289#23546#24287#12289#31070#22727#12289#36077#22580#12289#31072#22530#12289#25945#22530#12289#24066#22580#12289#37202#24215
          Enabled = False
        end
        object Label96: TLabel
          Left = 432
          Top = 80
          Width = 273
          Height = 13
          AutoSize = False
          Caption = #26159#21542#20986#20837#35079#38620#25110#26159#38468#36817#26377#29305#31278#29151#26989#30340#23567#22871#25151
          Enabled = False
        end
        object lblOwnerID2: TLabel
          Left = 136
          Top = 24
          Width = 61
          Height = 13
          AutoSize = False
          Caption = 'A123456789'
          Enabled = False
        end
        object lblLandHouseNum2: TLabel
          Left = 320
          Top = 24
          Width = 369
          Height = 13
          AutoSize = False
          Caption = '160-70-159/1080'
          Enabled = False
        end
        object Label106: TLabel
          Left = 144
          Top = 48
          Width = 12
          Height = 13
          AutoSize = False
          Caption = #20803
          Enabled = False
        end
        object hidden_FirstLien2: TLabel
          Left = 632
          Top = 32
          Width = 83
          Height = 13
          Caption = 'hidden_FirstLien2'
          Visible = False
        end
        object hidden_InquiryDate: TLabel
          Left = 728
          Top = 32
          Width = 62
          Height = 13
          AutoSize = False
          Visible = False
        end
        object nav2: TMaskEdit
          Left = 288
          Top = 44
          Width = 61
          Height = 21
          Enabled = False
          EditMask = '99999999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 1
        end
        object rgDamage2: TRadioGroup
          Left = 332
          Top = 68
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 2
        end
        object rgLent2: TRadioGroup
          Left = 332
          Top = 102
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 4
        end
        object rgBasement2: TRadioGroup
          Left = 560
          Top = 102
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 5
        end
        object rgMisuse2: TRadioGroup
          Left = 408
          Top = 136
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 6
        end
        object rgComplex2: TRadioGroup
          Left = 704
          Top = 68
          Width = 90
          Height = 35
          Columns = 2
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            #21542
            #26159)
          TabOrder = 3
        end
        object gav2: TMaskEdit
          Left = 72
          Top = 44
          Width = 61
          Height = 21
          Enabled = False
          EditMask = '99999999;0;_'
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          MaxLength = 8
          TabOrder = 0
        end
      end
      object finalReview_Clear: TButton
        Left = 304
        Top = 504
        Width = 75
        Height = 25
        Caption = #28165#38500
        TabOrder = 7
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
        TabOrder = 0
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
        TabOrder = 1
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
        TabOrder = 2
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
