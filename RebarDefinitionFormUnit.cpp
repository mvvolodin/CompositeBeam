//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RebarDefinitionFormUnit.h"
#include "String_doubleUnit.h" //������� ��������
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRebarDefinitionForm *RebarDefinitionForm;
//---------------------------------------------------------------------------
__fastcall TRebarDefinitionForm::TRebarDefinitionForm(TComponent* Owner)
	: TForm(Owner)
{
	rebar_named_list=new RebarNamedList();
	fill_rebar_named_list(rebar_named_list);

	cmb_bx_rebar_grade->Text=rebar_named_list->begin()->first;
	edt_R_s_n->Text=rebar_named_list->begin()->second.get_R_s();
	edt_diameter->Text="12";
	edt_safety_factor->Text="1.15";

	for (IteratorRebarNamedList it = rebar_named_list->begin(); it != rebar_named_list->end(); it++){
		cmb_bx_rebar_grade->Items->Add(it->first);
		};
	create_rebar();
}
//---------------------------------------------------------------------------
void __fastcall TRebarDefinitionForm::cmb_bx_rebar_gradeChange(TObject *Sender)
{
	String grade=cmb_bx_rebar_grade->Text;
	edt_R_s_n->Text=FloatToStr(rebar_named_list->find(grade)->second.get_R_s());
//  edt_R_s_n->Text=FloatToStr(rebar_named_list[grade].get_R_s());
// ��������� � ���������� [] �� ��������, ��� �� ������ �� ��, ��� ���� � ������ grade
//���������� �� �����, ����� �� ����� cotr �� ���������. ������ ���������� ��������� ��� ���������
//����������� � ��� �����, ��� ���� ���� �� ����� ������
}
//---------------------------------------------------------------------------

void TRebarDefinitionForm::create_rebar()
{
	double diameter=0.0;
	double safety_factor=0.0;
	int rc=0; //rc- return code -��� ������
	rc=String_double_plus(lbl_diameter->Caption, edt_diameter->Text, &diameter);
	if (rc>0) return;
	rc=String_double_plus(lbl_safety_factor->Caption, edt_safety_factor->Text, &safety_factor);
	if (rc>0) return;
	String grade=cmb_bx_rebar_grade->Text;
	double R_s=StrToFloat(edt_R_s_n->Text);
	rebar= Rebar(grade, R_s, diameter, safety_factor); //Copy initialization???
}
//---------------------------------------------------------------------------

void __fastcall TRebarDefinitionForm::bt_btn_OkClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TRebarDefinitionForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	create_rebar();
}
//---------------------------------------------------------------------------


