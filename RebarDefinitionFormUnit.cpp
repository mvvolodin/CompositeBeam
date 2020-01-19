//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RebarDefinitionFormUnit.h"
#include "String_doubleUnit.h" //������� ��������
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

extern 	RebarNamedList rebar_named_list;

TRebarDefinitionForm *RebarDefinitionForm;

//---------------------------------------------------------------------------
__fastcall TRebarDefinitionForm::TRebarDefinitionForm(TComponent* Owner)
	: TForm(Owner)
{
	cmb_bx_rebar_grade->Text=rebar_named_list.begin()->first;
	edt_R_s_n->Text=rebar_named_list.begin()->second.get_R_s();
	edt_diameter->Text="12";
	edt_safety_factor->Text="1.15";

	for (auto rebar:rebar_named_list) {
		cmb_bx_rebar_grade->Items->Add(rebar.first);
	}
	create_rebar();
}
//---------------------------------------------------------------------------
void __fastcall TRebarDefinitionForm::cmb_bx_rebar_gradeChange(TObject *Sender)
{
	String grade=cmb_bx_rebar_grade->Text;
	edt_R_s_n->Text=FloatToStr(rebar_named_list.find(grade)->second.get_R_s());
}
//---------------------------------------------------------------------------

void TRebarDefinitionForm::create_rebar()
{
	double diameter=0.0;
	double safety_factor=0.0;
	String_double_plus(lbl_diameter->Caption, edt_diameter->Text, &diameter);
	String_double_plus(lbl_safety_factor->Caption, edt_safety_factor->Text, &safety_factor);
	String grade=cmb_bx_rebar_grade->Text;
	double R_s=StrToFloat(edt_R_s_n->Text);
	rebar= Rebar(grade, R_s, diameter, safety_factor);
}
//---------------------------------------------------------------------------

void __fastcall TRebarDefinitionForm::bt_btn_OkClick(TObject *Sender)
{
	create_rebar();
	Close();
}
//---------------------------------------------------------------------------

