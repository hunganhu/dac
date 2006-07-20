object frmDirUI: TfrmDirUI
  Left = 752
  Top = 140
  BorderStyle = bsDialog
  Caption = #35373#23450#24501#23529#22577#34920#29986#29983#30446#37636
  ClientHeight = 450
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
  object btnDirCancel: TButton
    Left = 144
    Top = 416
    Width = 75
    Height = 25
    Caption = #21462#28040
    TabOrder = 0
    OnClick = btnDirCancelClick
  end
  object btnDirSet: TButton
    Left = 48
    Top = 416
    Width = 75
    Height = 25
    Caption = #30906#23450
    TabOrder = 1
    OnClick = btnDirSetClick
  end
  object edtDir: TEdit
    Left = 3
    Top = 388
    Width = 257
    Height = 21
    TabOrder = 2
  end
  object dlbReport: TDirectoryListBox
    Left = 0
    Top = 0
    Width = 265
    Height = 385
    ItemHeight = 16
    TabOrder = 3
    OnMouseDown = dlbReportMouseDown
  end
end
