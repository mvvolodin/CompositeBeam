﻿//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ConcreteDefinitionFormUnit.h"
#include "String_doubleUnit.h"  //Функции проверяющие правильность ввода данных в поля формы
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TConcreteDefinitionForm *ConcreteDefinitionForm;
extern std::vector <ConcreteBasic> concrete_basic;

//---------------------------------------------------------------------------
__fastcall TConcreteDefinitionForm::TConcreteDefinitionForm(TComponent* Owner)
	: TForm(Owner)
{
	lbl_gamma_b->Caption=(lbl_gamma_b->Caption + u"\u03B3"+u"b");
    lbl_gamma_bt->Caption=(lbl_gamma_bt->Caption + u"\u03B3"+u"bt");
	lbl_epsilon_b_lim->Caption=(lbl_epsilon_b_lim->Caption+u"\u03B5"+u"lim");
	cmb_bx_concrete_grade_list->Text=concrete_data_.begin()->first;
	edt_R_bn->Text=concrete_data_.begin()->second.get_R_bn();
	edt_R_btn->Text=concrete_data_.begin()->second.get_R_btn();
	edt_E_b->Text=concrete_data_.begin()->second.get_E_b();

	for(auto concrete:concrete_basic) {
		cmb_bx_concrete_grade_list->Items->Add(concrete.get_grade());
	}
		cmb_bx_concrete_grade_list->ItemIndex=4;//Бетон B25 по умолчанию
		cmb_bx_concrete_grade_listChange(nullptr);

		init_concrete();
}
//---------------------------------------------------------------------------
void __fastcall TConcreteDefinitionForm::cmb_bx_concrete_grade_listChange(TObject *Sender)
{
	String grade= cmb_bx_concrete_grade_list->Text;
	auto it_concrete=std::find_if(concrete_basic.begin(),concrete_basic.end(),
		[grade](ConcreteBasic concrete_basic){return concrete_basic.get_grade()==grade;});
	edt_R_bn->Text=FloatToStr(it_concrete->get_R_bn());
	edt_R_btn->Text=FloatToStr(it_concrete->get_R_btn());
	edt_E_b->Text=FloatToStr(it_concrete->get_E_b());
}
//---------------------------------------------------------------------------

void __fastcall TConcreteDefinitionForm::BtBtnConcreteChoiceClick(TObject *Sender)
{
	init_concrete();
	Close();
}
//---------------------------------------------------------------------------
void TConcreteDefinitionForm::init_concrete()
{   int rc=0; //rc- return code -код ошибки. Присваиваем начальное значение.
	String grade="";
	double R_bn=0.0;
	double R_btn=0.0;
	double E_b=0.0;
	double gamma_b=0.0;
	double gamma_bt=0.0;
	double epsilon_b_lim=0.0;
	grade=cmb_bx_concrete_grade_list->Text;
	R_bn=StrToFloat(edt_R_bn->Text);
	R_btn=StrToFloat(edt_R_btn->Text);
	E_b=StrToFloat(edt_E_b->Text);
	rc=String_double_plus(lbl_gamma_b->Caption, edt_gamma_b->Text, &gamma_b);
	if (rc>0) return;
	rc=String_double_plus(lbl_gamma_bt->Caption, edt_gamma_bt->Text, &gamma_bt);
	if (rc>0) return;
	rc=String_double_plus(lbl_epsilon_b_lim->Caption, edt_epsilon_b_lim->Text, &epsilon_b_lim);
	if (rc>0) return;
	ConcreteBasic concrete_basic (grade, E_b, R_bn, R_btn);
	concrete_=Concrete(concrete_basic, gamma_b, gamma_bt, epsilon_b_lim);
};

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

