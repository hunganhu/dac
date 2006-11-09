object Form1: TForm1
  Left = 273
  Top = 131
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = #21512#20316#37329#24235' '#20491#20154#20449#36024' '#31649#29702#22577#34920#29986#29983#31243#24335
  ClientHeight = 444
  ClientWidth = 515
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
  object Label5: TLabel
    Left = 43
    Top = 152
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #22577#34920#36215#26085
  end
  object Label6: TLabel
    Left = 170
    Top = 152
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #22577#34920#36804#26085
  end
  object lblMessage: TLabel
    Left = 8
    Top = 238
    Width = 497
    Height = 153
    AutoSize = False
    Color = clBtnHighlight
    ParentColor = False
    WordWrap = True
  end
  object Label10: TLabel
    Left = 223
    Top = 220
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #22519#34892#35338#24687
  end
  object Label9: TLabel
    Left = 37
    Top = 170
    Width = 61
    Height = 13
    Caption = '(YYYMMDD)'
  end
  object Label11: TLabel
    Left = 165
    Top = 170
    Width = 61
    Height = 13
    Caption = '(YYYMMDD)'
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 193
    Height = 137
    Caption = #36039#26009#24235#30331#20837
    TabOrder = 0
    object Label3: TLabel
      Left = 3
      Top = 31
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = #24115#34399
    end
    object Label4: TLabel
      Left = 3
      Top = 87
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      BiDiMode = bdLeftToRight
      Caption = #23494#30908
      ParentBiDiMode = False
    end
    object edtAccount: TEdit
      Left = 56
      Top = 28
      Width = 121
      Height = 21
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 0
    end
    object edtPassword: TEdit
      Left = 56
      Top = 83
      Width = 121
      Height = 21
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      PasswordChar = '*'
      TabOrder = 1
    end
  end
  object Button1: TButton
    Left = 161
    Top = 407
    Width = 75
    Height = 25
    Caption = #29986#29983#22577#34920
    TabOrder = 5
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 278
    Top = 407
    Width = 75
    Height = 25
    Caption = #32080#26463
    TabOrder = 6
    OnClick = Button2Click
  end
  object edtBeginDate: TEdit
    Left = 8
    Top = 186
    Width = 121
    Height = 21
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 2
  end
  object edtEndDate: TEdit
    Left = 136
    Top = 186
    Width = 121
    Height = 21
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 3
  end
  object Button3: TButton
    Left = 328
    Top = 170
    Width = 97
    Height = 25
    Caption = ' '#35373#23450#22577#34920#30446#37636
    TabOrder = 4
    OnClick = Button3Click
  end
  object GroupBox2: TGroupBox
    Left = 224
    Top = 8
    Width = 281
    Height = 137
    Caption = #35373#23450#22577#34920#38754#21521
    TabOrder = 1
    object Label7: TLabel
      Left = 211
      Top = 26
      Width = 46
      Height = 13
      AutoSize = False
      Caption = #38754#21521#19968
    end
    object Label8: TLabel
      Left = 210
      Top = 84
      Width = 47
      Height = 13
      AutoSize = False
      Caption = #38754#21521#20108
    end
    object lblDimension1: TLabel
      Left = 184
      Top = 44
      Width = 89
      Height = 13
      AutoSize = False
      Color = clBtnHighlight
      ParentColor = False
    end
    object lblDimension2: TLabel
      Left = 184
      Top = 102
      Width = 89
      Height = 13
      AutoSize = False
      Color = clBtnHighlight
      ParentColor = False
    end
    object lbDimensions: TListBox
      Left = 8
      Top = 24
      Width = 97
      Height = 97
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      Items.Strings = (
        #20998#34892
        #21033#29575
        #29986#21697
        #36024#27454#37329#38989
        #37559#21806#36890#36335)
      Sorted = True
      TabOrder = 0
    end
    object btnAddDimension1: TButton
      Left = 120
      Top = 24
      Width = 41
      Height = 17
      Caption = '>>'
      TabOrder = 1
      OnClick = btnAddDimension1Click
    end
    object btnRemoveDimension1: TButton
      Left = 120
      Top = 48
      Width = 41
      Height = 17
      Caption = '<<'
      TabOrder = 2
      OnClick = btnRemoveDimension1Click
    end
    object btnAddDimension2: TButton
      Left = 120
      Top = 80
      Width = 41
      Height = 17
      Caption = '>>'
      TabOrder = 3
      OnClick = btnAddDimension2Click
    end
    object btnRemoveDimension2: TButton
      Left = 120
      Top = 104
      Width = 41
      Height = 17
      Caption = '<<'
      TabOrder = 4
      OnClick = btnRemoveDimension2Click
    end
  end
end
