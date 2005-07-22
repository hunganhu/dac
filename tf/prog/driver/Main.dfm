object Form1: TForm1
  Left = 286
  Top = 254
  Width = 696
  Height = 401
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
    Width = 14
    Height = 13
    Caption = 'idn'
  end
  object Label3: TLabel
    Left = 120
    Top = 248
    Width = 41
    Height = 13
    Caption = 'case_no'
  end
  object Label4: TLabel
    Left = 120
    Top = 280
    Width = 35
    Height = 13
    Caption = 'dac_sn'
  end
  object Label5: TLabel
    Left = 40
    Top = 88
    Width = 37
    Height = 20
    Caption = 'IDN:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label6: TLabel
    Left = 192
    Top = 88
    Width = 77
    Height = 20
    Caption = 'Case NO:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label7: TLabel
    Left = 376
    Top = 88
    Width = 67
    Height = 20
    Caption = 'Dac SN:'
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
    Width = 53
    Height = 16
    Caption = 'Case #:'
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
    Width = 31
    Height = 16
    Caption = 'IDN:'
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
    Caption = 'Dac SN:'
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
      'Provider=SQLOLEDB.1;Persist Security Info=True;User ID=oliver;In' +
      'itial Catalog=oliver;Data Source=giza;Password=oliver'
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
    Left = 80
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
    Left = 448
    Top = 88
    Width = 65
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
      'Provider=SQLOLEDB.1;Password=oliver;Persist Security Info=True;U' +
      'ser ID=oliver;Initial Catalog=oliver;Data Source=giza'
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
