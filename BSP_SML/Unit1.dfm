object Form1: TForm1
  Left = 220
  Top = 105
  Width = 696
  Height = 480
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
    Left = 32
    Top = 8
    Width = 84
    Height = 13
    Caption = 'Connection String'
  end
  object Label2: TLabel
    Left = 24
    Top = 96
    Width = 64
    Height = 13
    Caption = 'Inquiry Month'
  end
  object Button1: TButton
    Left = 160
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 304
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Button2'
    TabOrder = 1
    OnClick = Button2Click
  end
  object edtConnectionString: TEdit
    Left = 32
    Top = 24
    Width = 617
    Height = 21
    ImeMode = imClose
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 2
    Text = 
      'Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security In' +
      'fo=False;Initial Catalog=DAC_SML_20050328'
  end
  object ckbGender: TCheckBox
    Left = 32
    Top = 64
    Width = 97
    Height = 17
    Caption = 'Gender'
    Checked = True
    State = cbChecked
    TabOrder = 3
  end
  object edtInqMonth: TEdit
    Left = 24
    Top = 112
    Width = 121
    Height = 21
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    TabOrder = 4
  end
end
