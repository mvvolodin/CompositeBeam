object FrmAddImpacts: TFrmAddImpacts
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = ' '#1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1099#1077' '#1074#1086#1079#1076#1077#1081#1089#1090#1074#1080#1103
  ClientHeight = 138
  ClientWidth = 285
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object btn_close: TButton
    Left = 202
    Top = 103
    Width = 75
    Height = 25
    Caption = #1047#1072#1082#1088#1099#1090#1100
    TabOrder = 0
  end
  object btn_cancel: TButton
    Left = 121
    Top = 103
    Width = 75
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1080#1090#1100
    TabOrder = 1
  end
  object btn_ok: TButton
    Left = 40
    Top = 103
    Width = 75
    Height = 25
    Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100
    TabOrder = 2
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 269
    Height = 89
    Caption = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1099#1077' '#1091#1088#1072#1074#1085#1086#1074#1077#1096#1077#1085#1085#1099#1077' '#1085#1072#1087#1088#1103#1078#1077#1085#1080#1103
    TabOrder = 3
    object lbl_sigma_bi: TLabel
      Left = 8
      Top = 28
      Width = 58
      Height = 13
      Caption = #1041#1077#1090#1086#1085', '#1052#1055#1072
    end
    object lbl_sigma_si: TLabel
      Left = 8
      Top = 55
      Width = 77
      Height = 13
      Caption = #1040#1088#1084#1072#1090#1091#1088#1072', '#1052#1055#1072
    end
    object edt_sigma_bi: TEdit
      Left = 205
      Top = 24
      Width = 52
      Height = 21
      TabOrder = 0
    end
    object edt_sigma_si: TEdit
      Left = 205
      Top = 52
      Width = 52
      Height = 21
      TabOrder = 1
    end
  end
end
