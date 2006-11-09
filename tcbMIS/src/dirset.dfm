object Form2: TForm2
  Left = 528
  Top = 267
  Width = 217
  Height = 403
  Caption = #36984#25799#22577#34920#30446#37636
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object dlbReport: TDirectoryListBox
    Left = -8
    Top = 0
    Width = 217
    Height = 305
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    ItemHeight = 16
    TabOrder = 0
    OnMouseDown = dlbReportMouseDown
  end
  object edtDir: TEdit
    Left = 0
    Top = 312
    Width = 209
    Height = 21
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 1
  end
  object Button1: TButton
    Left = 24
    Top = 344
    Width = 75
    Height = 25
    Caption = #30906#23450
    TabOrder = 2
    OnClick = Button1Click
  end
  object btnCancel: TButton
    Left = 120
    Top = 344
    Width = 75
    Height = 25
    Caption = #21462#28040
    TabOrder = 3
    OnClick = btnCancelClick
  end
end
