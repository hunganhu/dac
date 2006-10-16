object frmReDirUI: TfrmReDirUI
  Left = 752
  Top = 185
  BorderStyle = bsDialog
  Caption = #35373#23450#24501#23529#22577#34920#29986#29983#30446#37636
  ClientHeight = 447
  ClientWidth = 264
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object dlbReReport: TDirectoryListBox
    Left = 0
    Top = 0
    Width = 265
    Height = 385
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    ItemHeight = 16
    TabOrder = 0
    OnMouseDown = dlbReReportMouseDown
  end
  object Button1: TButton
    Left = 48
    Top = 416
    Width = 75
    Height = 25
    Caption = #30906#23450
    TabOrder = 1
    OnClick = Button1Click
  end
  object btnDirCancel: TButton
    Left = 144
    Top = 416
    Width = 75
    Height = 25
    Caption = #21462#28040
    TabOrder = 2
    OnClick = btnDirCancelClick
  end
  object edtReDir: TEdit
    Left = 3
    Top = 388
    Width = 257
    Height = 21
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 3
  end
end
