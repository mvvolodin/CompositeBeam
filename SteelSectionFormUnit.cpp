//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SteelSectionFormUnit.h"
#include "Data_Profil.h"
#include "Get_param_sect_func.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SteelSectionDefinitionFrameUnit"
#pragma resource "*.dfm"
TSteelSectionForm *SteelSectionForm;

bool flag_sect_change; // ���� ��������� ���������� �������
int  Type_sect;   // ��� ������� - ������� ��� �����
COMMON_SECT *common_sect;
bool flag_image;

//---------------------------------------------------------------------------
__fastcall TSteelSectionForm::TSteelSectionForm(TComponent* Owner)
	: TForm(Owner)
{
   common_sect = &com_sect; //��� �������� ���������� ���������� ������������� �������� ���� ������

}
//---------------------------------------------------------------------------
void __fastcall TSteelSectionForm::FormShow(TObject *Sender) //���������� ������� ���������� ������ ��� �����
{                                                            // ��� ��� ������������ �����



}
//---------------------------------------------------------------------------
//���������� �������, ������� ������� ���������
//---------------------------------------------------------------------------

void __fastcall TSteelSectionForm::SectSelectCom_ParamFrame1Button_ApplyClick(TObject *Sender)

{
  SteelSectionDefinitionFrame->Button_ApplyClick(Sender);// ������� ���������� � frame
  Get_param_sect(&(common_sect->dvutavr), &param_sect);//��������� ��������� �������������� �����������
  Close();
}
//---------------------------------------------------------------------------
//���������� ������� ����� ����� �������� "�10", "�20" ...
//---------------------------------------------------------------------------

void __fastcall TSteelSectionForm::SectSelectCom_ParamFrame1ComboBox_profilChange(TObject *Sender)

{
  SteelSectionDefinitionFrame->ComboBox_profilChange(Sender);//�������� ������� Frame

}
//---------------------------------------------------------------------------
//���������� ������� ����� ���� ������� "�", "�", "�".....
//---------------------------------------------------------------------------
void __fastcall TSteelSectionForm::SectSelectCom_ParamFrame1RadioGroupGOST57837Click(TObject *Sender)

{
  SteelSectionDefinitionFrame->RadioGroupGOST57837Click(Sender);

}
//---------------------------------------------------------------------------

