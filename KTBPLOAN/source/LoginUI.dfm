object frmLogin: TfrmLogin
  Left = 489
  Top = 279
  BorderStyle = bsDialog
  Caption = #30331#20837' DAC '#20491#20154#20449#36024#35413#20998#31995#32113
  ClientHeight = 160
  ClientWidth = 247
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 73
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #20351#29992#32773#24115#34399
  end
  object Label2: TLabel
    Left = 8
    Top = 48
    Width = 72
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #20351#29992#32773#23494#30908
  end
  object btnLogin: TButton
    Left = 40
    Top = 120
    Width = 75
    Height = 25
    Caption = #30331#20837
    TabOrder = 0
    OnClick = btnLoginClick
  end
  object btnCancel: TButton
    Left = 144
    Top = 120
    Width = 75
    Height = 25
    Caption = #21462#28040
    TabOrder = 1
    OnClick = btnCancelClick
  end
  object edtUser: TEdit
    Left = 85
    Top = 10
    Width = 148
    Height = 21
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 2
  end
  object edtPassword: TEdit
    Left = 85
    Top = 42
    Width = 148
    Height = 21
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    PasswordChar = '*'
    TabOrder = 3
  end
end
