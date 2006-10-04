object frmReloadInput: TfrmReloadInput
  Left = 158
  Top = 106
  BorderStyle = bsDialog
  Caption = 'DAC '#19968#33324#36092#23627#25151#36024#35413#20998#31995#32113' - '#25552#20379#20140#22478#37504#34892#20351#29992
  ClientHeight = 579
  ClientWidth = 858
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
    Top = 531
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #36914#20214#20998#34892
  end
  object Label61: TLabel
    Left = 128
    Top = 531
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #36914#20214#21729#24037
  end
  object Label62: TLabel
    Left = 16
    Top = 555
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #24501#23529#20154#21729
  end
  object gbApp: TGroupBox
    Left = 4
    Top = 3
    Width = 841
    Height = 97
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
    object Label3: TLabel
      Left = 256
      Top = 24
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #20986#29983#26085#26399
    end
    object Label4: TLabel
      Left = 320
      Top = 24
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #27665#22283
    end
    object Label5: TLabel
      Left = 388
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #24180
    end
    object Label6: TLabel
      Left = 501
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #26085
    end
    object Label7: TLabel
      Left = 446
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #26376
    end
    object Label8: TLabel
      Left = 8
      Top = 48
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #23130#23035#29376#27841
    end
    object Label10: TLabel
      Left = 132
      Top = 48
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #23416#27511
    end
    object Label11: TLabel
      Left = 271
      Top = 48
      Width = 73
      Height = 13
      AutoSize = False
      Caption = #26680#23526#24180#25910#20837
    end
    object Label12: TLabel
      Left = 8
      Top = 72
      Width = 233
      Height = 13
      AutoSize = False
      Caption = #24050#30906#35469#30003#35531#20154#22522#26412#36039#26009#31526#21512#25215#20570#26781#20214
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
    object medtAppBirthYear: TMaskEdit
      Left = 352
      Top = 19
      Width = 32
      Height = 21
      EditMask = '!999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 3
      TabOrder = 2
    end
    object medtAppBirthMonth: TMaskEdit
      Left = 408
      Top = 19
      Width = 32
      Height = 21
      EditMask = '!99;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      TabOrder = 3
    end
    object medtAppBirthDate: TMaskEdit
      Left = 464
      Top = 19
      Width = 32
      Height = 21
      EditMask = '!99;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      TabOrder = 4
    end
    object cmbAppMarriage: TComboBox
      Left = 67
      Top = 43
      Width = 54
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 5
      Items.Strings = (
        #26410#23130
        #24050#23130)
    end
    object cmbAppEducation: TComboBox
      Left = 163
      Top = 44
      Width = 102
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 6
      Items.Strings = (
        #30740#31350#25152#21450#20197#19978
        #22823#23416
        #23560#31185
        #39640#22283#20013'('#21547#20197#19979')')
    end
    object medtAppAnnualIncome: TMaskEdit
      Left = 346
      Top = 44
      Width = 118
      Height = 21
      EditMask = '!999999999;1;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 9
      TabOrder = 7
      Text = '         '
    end
    object cmbAppQualified: TComboBox
      Left = 234
      Top = 68
      Width = 39
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 8
      Items.Strings = (
        #26159
        #21542)
    end
  end
  object gbCoApp: TGroupBox
    Left = 4
    Top = 102
    Width = 841
    Height = 97
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
    object Label15: TLabel
      Left = 256
      Top = 24
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #20986#29983#26085#26399
    end
    object Label16: TLabel
      Left = 320
      Top = 24
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #27665#22283
    end
    object Label17: TLabel
      Left = 388
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #24180
    end
    object Label18: TLabel
      Left = 501
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #26085
    end
    object Label19: TLabel
      Left = 446
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #26376
    end
    object Label20: TLabel
      Left = 8
      Top = 48
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #23130#23035#29376#27841
    end
    object Label21: TLabel
      Left = 132
      Top = 48
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #23416#27511
    end
    object Label22: TLabel
      Left = 271
      Top = 48
      Width = 73
      Height = 13
      AutoSize = False
      Caption = #26680#23526#24180#25910#20837
    end
    object Label23: TLabel
      Left = 8
      Top = 72
      Width = 257
      Height = 13
      AutoSize = False
      Caption = #24050#30906#35469#20849#21516#20511#27454#20154#22522#26412#36039#26009#31526#21512#25215#20570#26781#20214
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
    object medtCoAppBirthYear: TMaskEdit
      Left = 352
      Top = 19
      Width = 32
      Height = 21
      EditMask = '!999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 3
      TabOrder = 2
    end
    object medtCoAppBirthMonth: TMaskEdit
      Left = 408
      Top = 19
      Width = 32
      Height = 21
      EditMask = '!99;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      TabOrder = 3
    end
    object medtCoAppBirthDate: TMaskEdit
      Left = 464
      Top = 19
      Width = 32
      Height = 21
      EditMask = '!99;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      TabOrder = 4
    end
    object cmbCoAppMarriage: TComboBox
      Left = 67
      Top = 43
      Width = 54
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 5
      Items.Strings = (
        #26410#23130
        #24050#23130)
    end
    object cmbCoAppEducation: TComboBox
      Left = 163
      Top = 44
      Width = 102
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 6
      Items.Strings = (
        #30740#31350#25152#21450#20197#19978
        #22823#23416
        #23560#31185
        #39640#22283#20013'('#21547#20197#19979')')
    end
    object medtCoAppAnnualIncome: TMaskEdit
      Left = 346
      Top = 44
      Width = 118
      Height = 21
      EditMask = '!999999999;1;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 9
      TabOrder = 7
      Text = '         '
    end
    object cmbCoAppQualified: TComboBox
      Left = 266
      Top = 68
      Width = 39
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 8
      Items.Strings = (
        #26159
        #21542)
    end
  end
  object gbGuarantor: TGroupBox
    Left = 4
    Top = 201
    Width = 841
    Height = 97
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
    object Label26: TLabel
      Left = 256
      Top = 24
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #20986#29983#26085#26399
    end
    object Label27: TLabel
      Left = 320
      Top = 24
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #27665#22283
    end
    object Label28: TLabel
      Left = 388
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #24180
    end
    object Label29: TLabel
      Left = 501
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #26085
    end
    object Label30: TLabel
      Left = 446
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #26376
    end
    object Label31: TLabel
      Left = 8
      Top = 48
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #23130#23035#29376#27841
    end
    object Label32: TLabel
      Left = 132
      Top = 48
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #23416#27511
    end
    object Label33: TLabel
      Left = 271
      Top = 48
      Width = 73
      Height = 13
      AutoSize = False
      Caption = #26680#23526#24180#25910#20837
    end
    object Label34: TLabel
      Left = 8
      Top = 72
      Width = 233
      Height = 13
      AutoSize = False
      Caption = #24050#30906#35469#20445#35657#20154#22522#26412#36039#26009#31526#21512#25215#20570#26781#20214
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
    object medtGuarantorBirthYear: TMaskEdit
      Left = 352
      Top = 19
      Width = 32
      Height = 21
      EditMask = '!999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 3
      TabOrder = 2
    end
    object medtGuarantorBirthMonth: TMaskEdit
      Left = 408
      Top = 19
      Width = 32
      Height = 21
      EditMask = '!99;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      TabOrder = 3
    end
    object medtGuarantorBirthDate: TMaskEdit
      Left = 464
      Top = 19
      Width = 32
      Height = 21
      EditMask = '!99;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      TabOrder = 4
    end
    object cmbGuarantorMarriage: TComboBox
      Left = 67
      Top = 43
      Width = 54
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 5
      Items.Strings = (
        #26410#23130
        #24050#23130)
    end
    object cmbGuarantorEducation: TComboBox
      Left = 163
      Top = 44
      Width = 102
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 6
      Items.Strings = (
        #30740#31350#25152#21450#20197#19978
        #22823#23416
        #23560#31185
        #39640#22283#20013'('#21547#20197#19979')')
    end
    object medtGuarantorAnnualIncome: TMaskEdit
      Left = 346
      Top = 44
      Width = 118
      Height = 21
      EditMask = '!999999999;1;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 9
      TabOrder = 7
      Text = '         '
    end
    object cmbGuarantorQualified: TComboBox
      Left = 234
      Top = 68
      Width = 39
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 8
      Items.Strings = (
        #26159
        #21542)
    end
  end
  object gbApplication: TGroupBox
    Left = 4
    Top = 300
    Width = 841
    Height = 73
    Caption = #30003#36024#36039#26009
    TabOrder = 5
    object Label35: TLabel
      Left = 8
      Top = 24
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #30003#35531#37329#38989
    end
    object Label36: TLabel
      Left = 184
      Top = 24
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #20511#27454#26399#38291
    end
    object Label37: TLabel
      Left = 144
      Top = 24
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #33836#20803
    end
    object Label38: TLabel
      Left = 276
      Top = 24
      Width = 21
      Height = 13
      AutoSize = False
      Caption = #26376
    end
    object Label39: TLabel
      Left = 382
      Top = 24
      Width = 21
      Height = 13
      AutoSize = False
      Caption = #26376
    end
    object Label40: TLabel
      Left = 304
      Top = 24
      Width = 49
      Height = 13
      AutoSize = False
      Caption = #23532#38480#26399
    end
    object Label41: TLabel
      Left = 502
      Top = 24
      Width = 21
      Height = 13
      AutoSize = False
      Caption = #20803
    end
    object Label42: TLabel
      Left = 408
      Top = 24
      Width = 49
      Height = 13
      AutoSize = False
      Caption = #38283#36774#36027
    end
    object Label43: TLabel
      Left = 536
      Top = 24
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #21033#29575#20849#20998
    end
    object Label44: TLabel
      Left = 632
      Top = 24
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #27573
    end
    object Label45: TLabel
      Left = 8
      Top = 48
      Width = 49
      Height = 13
      AutoSize = False
      Caption = #31532#19968#27573
    end
    object Label46: TLabel
      Left = 86
      Top = 48
      Width = 67
      Height = 13
      AutoSize = False
      Caption = #26376#65292#24180#21033#29575
    end
    object Label47: TLabel
      Left = 197
      Top = 48
      Width = 17
      Height = 13
      AutoSize = False
      Caption = '%'
    end
    object Label48: TLabel
      Left = 221
      Top = 48
      Width = 49
      Height = 13
      AutoSize = False
      Caption = #31532#20108#27573
    end
    object Label49: TLabel
      Left = 299
      Top = 48
      Width = 67
      Height = 13
      AutoSize = False
      Caption = #26376#65292#24180#21033#29575
    end
    object Label50: TLabel
      Left = 410
      Top = 48
      Width = 17
      Height = 13
      AutoSize = False
      Caption = '%'
    end
    object Label51: TLabel
      Left = 434
      Top = 48
      Width = 49
      Height = 13
      AutoSize = False
      Caption = #31532#19977#27573
    end
    object Label52: TLabel
      Left = 512
      Top = 48
      Width = 67
      Height = 13
      AutoSize = False
      Caption = #26376#65292#24180#21033#29575
    end
    object Label53: TLabel
      Left = 626
      Top = 48
      Width = 17
      Height = 13
      AutoSize = False
      Caption = '%'
    end
    object medtAppAmount: TMaskEdit
      Left = 67
      Top = 20
      Width = 67
      Height = 21
      EditMask = '!999999;1;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 6
      TabOrder = 0
      Text = '      '
    end
    object medtMaturity: TMaskEdit
      Left = 241
      Top = 21
      Width = 29
      Height = 21
      EditMask = '!999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 3
      TabOrder = 1
    end
    object medtGracePeriod: TMaskEdit
      Left = 347
      Top = 21
      Width = 31
      Height = 21
      EditMask = '!99;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 2
      TabOrder = 2
    end
    object medtAppFee: TMaskEdit
      Left = 451
      Top = 21
      Width = 45
      Height = 21
      EditMask = '!99999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 5
      TabOrder = 3
    end
    object cmbInterestSections: TComboBox
      Left = 592
      Top = 20
      Width = 33
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 4
      OnChange = cmbInterestSectionsChange
      Items.Strings = (
        '1'
        '2'
        '3')
    end
    object medtInterestPeriodOne: TMaskEdit
      Left = 51
      Top = 45
      Width = 29
      Height = 21
      EditMask = '!999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 3
      TabOrder = 5
    end
    object medtInterestOne: TMaskEdit
      Left = 155
      Top = 45
      Width = 38
      Height = 21
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 6
    end
    object medtInterestPeriodTwo: TMaskEdit
      Left = 264
      Top = 45
      Width = 29
      Height = 21
      EditMask = '!999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 3
      TabOrder = 7
    end
    object medtInterestTwo: TMaskEdit
      Left = 368
      Top = 45
      Width = 41
      Height = 21
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 8
    end
    object medtInterestPeriodThree: TMaskEdit
      Left = 477
      Top = 45
      Width = 29
      Height = 21
      EditMask = '!999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 3
      TabOrder = 9
    end
    object medtInterestThree: TMaskEdit
      Left = 581
      Top = 45
      Width = 44
      Height = 21
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 10
    end
  end
  object gbCollectral: TGroupBox
    Left = 4
    Top = 375
    Width = 841
    Height = 145
    Caption = #25812#20445#21697#36039#26009
    TabOrder = 6
    object Label54: TLabel
      Left = 8
      Top = 24
      Width = 73
      Height = 13
      AutoSize = False
      Caption = #25152#26377#20154#22995#21517
    end
    object Label55: TLabel
      Left = 144
      Top = 24
      Width = 113
      Height = 13
      AutoSize = False
      Caption = #25152#26377#20154#36523#20998#35657#23383#34399
    end
    object Label56: TLabel
      Left = 336
      Top = 24
      Width = 89
      Height = 13
      AutoSize = False
      Caption = #33287#30003#35531#20154#38364#20418
    end
    object Label57: TLabel
      Left = 8
      Top = 48
      Width = 73
      Height = 13
      AutoSize = False
      Caption = #22320#34399' / '#24314#34399
    end
    object Label59: TLabel
      Left = 8
      Top = 96
      Width = 161
      Height = 13
      AutoSize = False
      Caption = #30906#35469#25812#20445#21697#31526#21512#25215#20570#26781#20214
    end
    object Label60: TLabel
      Left = 8
      Top = 120
      Width = 505
      Height = 13
      AutoSize = False
      Caption = #25812#20445#21697#26159#21542#20301#26044#21271#39640#32291#24066#12289#26032#31481#24066#12289#21488#20013#24066#12289#21488#21335#24066#12289#27573#33391#22909#19988#25645#37197#20778#33391#24314#35373#20844#21496
    end
    object Label58: TLabel
      Left = 8
      Top = 72
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #25151#23627#27611#20540
    end
    object Label63: TLabel
      Left = 144
      Top = 72
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #33836#20803
    end
    object Label64: TLabel
      Left = 200
      Top = 72
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #25151#23627#28136#20540
    end
    object Label65: TLabel
      Left = 336
      Top = 72
      Width = 33
      Height = 13
      AutoSize = False
      Caption = #33836#20803
    end
    object Label66: TLabel
      Left = 392
      Top = 72
      Width = 161
      Height = 13
      AutoSize = False
      Caption = #20272#35336#30446#21069#21407#25151#36024#27599#26376#25033#20184
    end
    object Label67: TLabel
      Left = 640
      Top = 72
      Width = 25
      Height = 13
      AutoSize = False
      Caption = #20803
    end
    object edtOwnerName: TEdit
      Left = 77
      Top = 19
      Width = 52
      Height = 21
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 0
    end
    object medtOwnerID: TMaskEdit
      Left = 256
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
    object cmbRelation: TComboBox
      Left = 426
      Top = 20
      Width = 63
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 2
      Items.Strings = (
        #26412#20154
        #29238#27597
        #37197#20598
        #23376#22899
        #20854#20182)
    end
    object cmbCollectralQualified: TComboBox
      Left = 162
      Top = 92
      Width = 39
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 7
      Items.Strings = (
        #26159
        #21542)
    end
    object cmbPremiumCollectral: TComboBox
      Left = 514
      Top = 116
      Width = 39
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 8
      Items.Strings = (
        #26159
        #21542)
    end
    object medtGAV: TMaskEdit
      Left = 72
      Top = 67
      Width = 64
      Height = 21
      EditMask = '!999999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 6
      TabOrder = 4
    end
    object medtNAV: TMaskEdit
      Left = 264
      Top = 67
      Width = 64
      Height = 21
      EditMask = '!999999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 6
      TabOrder = 5
    end
    object medtMonthlyMortgagePayment: TMaskEdit
      Left = 552
      Top = 67
      Width = 80
      Height = 21
      EditMask = '!9999999;0;_'
      ImeMode = imClose
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      MaxLength = 7
      TabOrder = 6
    end
    object edtLandSerial: TEdit
      Left = 77
      Top = 43
      Width = 252
      Height = 21
      ImeMode = imChinese
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 3
    end
  end
  object medtBranch: TMaskEdit
    Left = 72
    Top = 524
    Width = 48
    Height = 21
    EditMask = '!9999;0;_'
    ImeMode = imClose
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    MaxLength = 4
    TabOrder = 7
  end
  object btnExit: TButton
    Left = 768
    Top = 548
    Width = 75
    Height = 25
    Caption = #32080#26463#31243#24335
    TabOrder = 12
    OnClick = btnExitClick
  end
  object btnClear: TButton
    Left = 472
    Top = 527
    Width = 41
    Height = 25
    Caption = #28165#38500
    TabOrder = 11
    OnClick = btnClearClick
  end
  object btnScore: TButton
    Left = 416
    Top = 527
    Width = 49
    Height = 25
    Caption = #35413#20998
    TabOrder = 10
    OnClick = btnScoreClick
  end
  object medtUnderwriter: TMaskEdit
    Left = 72
    Top = 551
    Width = 55
    Height = 21
    EditMask = '!9999999;0;_'
    ImeMode = imClose
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    MaxLength = 7
    TabOrder = 9
  end
  object medtEmployee: TMaskEdit
    Left = 184
    Top = 527
    Width = 55
    Height = 21
    EditMask = '!9999999;0;_'
    ImeMode = imClose
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    MaxLength = 7
    TabOrder = 8
  end
  object ckbCoApp: TCheckBox
    Left = 148
    Top = 100
    Width = 17
    Height = 17
    TabOrder = 1
    OnClick = ckbCoAppClick
  end
  object ckbGuarantor: TCheckBox
    Left = 117
    Top = 196
    Width = 17
    Height = 17
    TabOrder = 3
    OnClick = ckbGuarantorClick
  end
end
