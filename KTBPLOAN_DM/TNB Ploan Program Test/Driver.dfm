object Form1: TForm1
  Left = 93
  Top = 145
  BorderStyle = bsDialog
  Caption = 'Form1'
  ClientHeight = 135
  ClientWidth = 1028
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
    Left = 16
    Top = 24
    Width = 110
    Height = 13
    Caption = 'ADO Connection String'
  end
  object Edit1: TEdit
    Left = 144
    Top = 13
    Width = 897
    Height = 21
    ImeMode = imClose
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 0
    Text = 
      'Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security In' +
      'fo=False;User ID=test;Initial Catalog=PLOAN_MKT_20060626;Data So' +
      'urce=NBCOMPUTER\NBCOMPUTER2K'
  end
  object Button1: TButton
    Left = 520
    Top = 72
    Width = 75
    Height = 25
    Caption = '&End'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 360
    Top = 72
    Width = 75
    Height = 25
    Caption = '&Scoring'
    TabOrder = 2
    OnClick = Button2Click
  end
end
