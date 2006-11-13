object Form1: TForm1
  Left = 237
  Top = 141
  Width = 1027
  Height = 123
  Caption = 'Form1'
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
    Left = 8
    Top = 19
    Width = 86
    Height = 13
    Caption = 'Connectiion String'
  end
  object Label2: TLabel
    Left = 33
    Top = 48
    Width = 60
    Height = 13
    Caption = 'Product type'
  end
  object Button1: TButton
    Left = 376
    Top = 48
    Width = 75
    Height = 25
    Caption = '&Start'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 616
    Top = 48
    Width = 75
    Height = 25
    Caption = 'E&xit'
    TabOrder = 1
    OnClick = Button2Click
  end
  object edtConnectionString: TEdit
    Left = 104
    Top = 16
    Width = 897
    Height = 21
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 2
    Text = 
      'Provider=SQLOLEDB.1;Password=thisisatest;Persist Security Info=T' +
      'rue;User ID=test;Initial Catalog=BOOC_Inquiry;Data Source=NBCOMP' +
      'UTER'
  end
  object cmbProduct: TComboBox
    Left = 104
    Top = 44
    Width = 145
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 3
    Text = 'Credit Card'
    Items.Strings = (
      'Credit Card'
      'BT')
  end
end
