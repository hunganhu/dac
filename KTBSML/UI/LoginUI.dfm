object frmLogin: TfrmLogin
  Left = 433
  Top = 292
  BorderStyle = bsDialog
  Caption = #30331#20837' DAC '#28136#20540#25151#36024#25480#20449#31995#32113
  ClientHeight = 160
  ClientWidth = 266
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
    TabOrder = 2
    OnClick = btnLoginClick
    OnKeyPress = btnLoginKeyPress
  end
  object btnCancel: TButton
    Left = 144
    Top = 120
    Width = 75
    Height = 25
    Caption = #21462#28040
    TabOrder = 3
    OnClick = btnCancelClick
  end
  object edtUser: TEdit
    Left = 85
    Top = 10
    Width = 148
    Height = 21
    ImeMode = imClose
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 0
  end
  object edtPassword: TEdit
    Left = 85
    Top = 42
    Width = 148
    Height = 21
    ImeMode = imDisable
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    PasswordChar = '*'
    TabOrder = 1
  end
end
