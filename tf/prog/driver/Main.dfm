object Form1: TForm1
  Left = 1
  Top = 392
  Width = 683
  Height = 351
  Caption = 'TF Approval Module'
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
    Left = 40
    Top = 40
    Width = 150
    Height = 20
    Caption = 'Connection String:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 120
    Top = 264
    Width = 41
    Height = 13
    Caption = 'appDate'
  end
  object Label3: TLabel
    Left = 120
    Top = 248
    Width = 33
    Height = 13
    Caption = 'appSN'
  end
  object Label4: TLabel
    Left = 120
    Top = 280
    Width = 39
    Height = 13
    Caption = 'jcicDate'
  end
  object Label5: TLabel
    Left = 40
    Top = 88
    Width = 55
    Height = 20
    Caption = 'appSN'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label6: TLabel
    Left = 208
    Top = 88
    Width = 70
    Height = 20
    Caption = 'appDate'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label7: TLabel
    Left = 368
    Top = 88
    Width = 66
    Height = 20
    Caption = 'jcicDate'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Start_time: TLabel
    Left = 80
    Top = 224
    Width = 47
    Height = 13
    Caption = 'Start_time'
  end
  object End_time: TLabel
    Left = 264
    Top = 224
    Width = 44
    Height = 13
    Caption = 'End_time'
  end
  object count: TLabel
    Left = 120
    Top = 296
    Width = 27
    Height = 13
    Caption = 'count'
  end
  object Label8: TLabel
    Left = 32
    Top = 224
    Width = 37
    Height = 16
    Caption = 'Start:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label9: TLabel
    Left = 48
    Top = 248
    Width = 49
    Height = 16
    Caption = 'appSN'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label10: TLabel
    Left = 48
    Top = 264
    Width = 61
    Height = 16
    Caption = 'appDate'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label11: TLabel
    Left = 48
    Top = 280
    Width = 58
    Height = 16
    Caption = 'jcicDate'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label12: TLabel
    Left = 48
    Top = 296
    Width = 44
    Height = 16
    Caption = 'Count:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label13: TLabel
    Left = 192
    Top = 224
    Width = 32
    Height = 16
    Caption = 'End:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Validate: TBitBtn
    Left = 40
    Top = 136
    Width = 153
    Height = 25
    Caption = 'Input Validation'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = ValidateClick
  end
  object Edit1: TEdit
    Left = 200
    Top = 40
    Width = 433
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = 
      'Provider=SQLOLEDB.1;Password=Emily1013;Persist Security Info=Tru' +
      'e;User ID=sa;Initial Catalog=TF_TEST;Data Source=OLIVER\DAISY'
  end
  object CheckBox1: TCheckBox
    Left = 40
    Top = 192
    Width = 121
    Height = 17
    Caption = 'Local Connected'
    TabOrder = 2
    OnClick = CheckBox1Click
  end
  object Edit2: TEdit
    Left = 96
    Top = 88
    Width = 105
    Height = 21
    TabOrder = 3
  end
  object Edit3: TEdit
    Left = 280
    Top = 88
    Width = 81
    Height = 21
    TabOrder = 4
  end
  object UnitTest: TButton
    Left = 536
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Unit Test'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 5
    OnClick = UnitTestClick
  end
  object Edit4: TEdit
    Left = 432
    Top = 88
    Width = 73
    Height = 21
    TabOrder = 6
    Text = '1'
  end
  object Button1: TButton
    Left = 488
    Top = 136
    Width = 75
    Height = 25
    Caption = 'End'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 7
    OnClick = Button1Click
  end
  object Validate_NoConn: TBitBtn
    Left = 256
    Top = 136
    Width = 153
    Height = 25
    Caption = ' Validation NoConn'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 8
    OnClick = ValidateNoConnClick
  end
  object ADOConnection1: TADOConnection
    ConnectionString = 
      'Provider=SQLOLEDB.1;Password=Emily1013;Persist Security Info=Tru' +
      'e;User ID=sa;Initial Catalog=TF_TEST;Data Source=OLIVER\DAISY'
    Provider = 'SQLOLEDB.1'
    Left = 416
    Top = 272
  end
  object ADOQuery1: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 488
    Top = 272
  end
  object ADOQuery2: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 552
    Top = 272
  end
end
